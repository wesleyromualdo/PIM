<?php 

if($_REQUEST['muncod']){
	$_SESSION['proinfantil']['mds']['muncod'] = $_REQUEST['muncod']; 
}

if($_REQUEST['requisicao'] == 'salvarQuantidades'){
	
	if($_REQUEST['alaquantidade']){
		
		$arAlaquantidade = explode(',', $_REQUEST['alaquantidade']);
		$arEntcodent = explode(',', $_REQUEST['entcodent']);
		
		$sql = '';
		foreach($arAlaquantidade as $k => $v){
			
			$alaquantidade = $v ? $v : 0;
			
			$mdsid = $db->pegaUm("select mdsid from proinfantil.mdssuplementacao where entcodent = '{$arEntcodent[$k]}'");
			
			if(!$mdsid){
				
				$sql .= "insert into proinfantil.mdssuplementacao
								(mdsdtinclusao, usucpfinclusao, mdsstatus, entcodent, mdsquantidadepbf)
							values
								(now(), '{$_SESSION['usucpf']}', 'A', '{$arEntcodent[$k]}', {$alaquantidade}); ";
				
			}else{
				
				$sql .= "update proinfantil.mdssuplementacao set 
							mdsquantidadepbf = {$alaquantidade}
						where
							mdsid = {$mdsid}; ";
			}
		}
					
		if($sql){
			$db->executar($sql);
			if($db->commit()){
				$db->sucesso('suplementacaomds/listaCreche', '');
			}			
		}
	}

	$db->insucesso('Não foi possível completar a operação!', '', 'suplementacaomds/listaCreche');
	die;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
include  "_funcoes_mds.php";
echo "<br/>";

$abacod_tela = 57559;
$arMnuid = array();

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$linha1 = "Lista de Creches";
$linha2 = '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica campo obrigatório.';

monta_titulo($linha1, $linha2);

cabecalhoMunicipio();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		$('#btnSalvar').click(function(){
			var qtds = $('[name=qtd_alunos[]]').val();

			var arQtds = new Array();
			$.each($('[name=alaquantidade_temp[]]'), function(i, v){
				arQtds.push(v.value);
			});

			var arEntcodent = new Array();
			$.each($('[name=entcodent_temp[]]'), function(i, v){
				arEntcodent.push(v.value);
			});
			
			$('[name=requisicao]').val('salvarQuantidades');
			$('[name=entcodent]').val(arEntcodent);
			$('[name=alaquantidade]').val(arQtds);
			$('#formAlunosAtendidos').submit();
		});
		$('#btnLimpar').click(function(){
			$('.normal').val('');
		});
	});
</script>
<?php 
$sql = "select distinct
            ent.entcodent,
            ent.entnome,
            pro.prcqtdalunoinfantil,
            '
            <input type=\"text\" name=\"alaquantidade_temp[]\" size=\"8\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal(\'######\',this.value);\" value=\"' || coalesce(mdsquantidadepbf,0) || '\" />
            <input type=\"hidden\" name=\"entcodent_temp[]\" value=\"' || ent.entcodent || '\" />
            ' as campo
        from
            entidade.entidade ent
        inner join
            entidade.endereco ede on ede.entid = ent.entid
        inner join
            territorios.municipio mun on mun.muncod = ede.muncod
        inner join 
        	proinfantil.procenso pro on pro.entcodent = ent.entcodent
        left join 
        	proinfantil.mdssuplementacao sup on sup.entcodent = ent.entcodent        
		where 
			ent.entstatus = 'A'
		and 
			ent.entcodent is not null
				
		and
			mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
		order by 
			ent.entnome";

$cabecalho = array('Cod. INEP', 'Nome da Escola', 'Qtd. Matrículas na Creche', 'Qtd. Crianças Oriundas PBF');

$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '', '', array(80, null, 120, 120));
?>
<form name="formAlunosAtendidos" id="formAlunosAtendidos" method="post">
	<input type="hidden" name="requisicao" />
	<input type="hidden" name="alaquantidade" />
	<input type="hidden" name="entcodent" />
</form>
<?php if(checkPerfil(array(PERFIL_ADMINISTRADOR,EQUIPE_MUNICIPAL))): ?>
	<center>
		<p>
		<input type="button" value="Salvar" id="btnSalvar">
		<input type="button" value="Limpar tudo" id="btnLimpar">
		</p>		
	</center>
<?php endif; ?>