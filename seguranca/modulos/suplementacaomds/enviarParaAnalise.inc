<?php 

if($_REQUEST['requisicao'] == 'salvaJustificativa'){
	
	if($_REQUEST['mjmid']){
		
		$sql = "update proinfantil.mdsjustificativamunicipio set 
					mjmjustificativa = '{$_REQUEST['mjmjustificativa']}'
				where
					mjmid = {$_REQUEST['mjmid']}";
		
	}else{
		
		$sql = "insert into proinfantil.mdsjustificativamunicipio
					(mjmjustificativa,muncod,mjmstatus)
				values
					('{$_REQUEST['mjmjustificativa']}', '{$_SESSION['proinfantil']['mds']['muncod']}', 'A');";
	}
	
	$db->executar($sql);
	if($db->commit()){
		$db->sucesso('suplementacaomds/enviarParaAnalise');
	}
}

if($_REQUEST['requisicao'] == 'tramitarAnalise'){
	if($_REQUEST['docid']){
		$sql = "update workflow.documento set esdid = ".WF_MDS_EM_ANALISE." where docid = {$_REQUEST['docid']};";		 
		$db->executar($sql);
		if($db->commit()){
			echo 1;
			die();
		}		
	}
	echo 2;
	die();
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once "_funcoes_mds.php";
echo "<br/>";

$sql = "select 
			cpmid 
		from 
			proinfantil.mdsdadoscriancapormunicipio cpm		 
		where 
			muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'";

$cpmid = $db->pegaUm($sql);

$docid = criaDocumento($cpmid);
$esdid = pegaEstadoAtual($docid);

$abacod_tela = 57559;
$arMnuid = array();

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$titulo_modulo = "Pendências";
$titulo_modulo2 = '<img src="../imagens/AtencaoP.gif" height="15" align="absmiddle" />&nbsp;Não impeditiva&nbsp;<img src="../imagens/excluir_2.gif" height="18" align="absmiddle" />&nbsp;Impeditiva';
monta_titulo( $titulo_modulo, $titulo_modulo2 );

$sql = "select 
			mjmid,
			mjmjustificativa 
		from 
			proinfantil.mdsjustificativamunicipio 
		where 
			muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'";

$rs = $db->pegaLinha($sql);

if($rs) extract($rs);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		$('#btnSalvar').click(function(){
			if($('[name=mjmjustificativa]').val() == ''){
				alert('O campo Justificativa é obrigatório!');
				$('[name=mjmjustificativa]').focus();
				return false;
			}
			$('[name=requisicao]').val('salvaJustificativa');
			$('#formPendencias').submit();
		});
		$('#btnTramitar').click(function(){

			var docid = $('[name=docid]').val();
			var cpmid = $('[name=cpmid]').val();
			
			$.ajax({
				url		: 'proinfantil.php?modulo=suplementacaomds/enviarParaAnalise&acao=A',
				type	: 'post',
				data	: 'requisicao=tramitarAnalise&docid='+docid+'&cpmid='+cpmid,
				success	: function(e){	
				e = trim(e);				
					if(e == 1){
						alert('Enviado para análise do MEC.');
						document.location.href = 'proinfantil.php?modulo=suplementacaomds/enviarParaAnalise&acao=A';
					}else{
						alert('Não foi possível enviar para análise!');
					}
				}
			});
		});
	});
</script>
<style>
.legenda{
	width: 100%;
	border: 1px solid black;
	text-align: left;
	float: right;	
}
</style>

<center>
	<form id="formPendencias" name="formPendencias" method="post">
		<input type="hidden" name="mjmid" value="<?php echo $mjmid ? $mjmid : ''; ?>" />
		<input type="hidden" name="docid" value="<?php echo $docid; ?>" />
		<input type="hidden" name="cpmid" value="<?php echo $cpmid; ?>" />
		<input type="hidden" name="requisicao" />
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3" >
			<tr>
				<td valign="top">
					<?php 
					
					$sql = "select distinct
					            ent.entcodent,
					            ent.entnome,
					            pro.prcqtdalunoinfantil,
					            coalesce(mdsquantidadepbf,0) as mdsquantidadepbf
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
								ent.tpcid in (1,2,3)			
							and
								mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
							order by 
								ent.entnome";
					
					$rsCreches = $db->carregar($sql);
					
					$sql = "select				            
					            sum(pro.prcqtdalunoinfantil) as alunos_mat,
					            sum(sup.mdsquantidadepbf) as alunos_pbf,
					            --(select cpmnumcriancas from proinfantil.mdscriancaspbfmunicipio cpm where cpm.muncod = mun.muncod) as alunos_mun
					            cpmnumcriancascompbf as alunos_mun,
					            cpmqtdcrianca03mun as alunos_mes
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
					        left join 
					        	proinfantil.mdsdadoscriancapormunicipio dcm on dcm.muncod = mun.muncod        
							where 
								ent.entstatus = 'A'
							and 
								ent.entcodent is not null
							and 
								ent.tpcid in (1,2,3)			
							and
								mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
							group by 
								mun.muncod,cpmnumcriancascompbf,cpmqtdcrianca03mun";
					
						$rs = $db->pegaLinha($sql);			
	
						$arPendencias = array();
					?>
	
					<table class="tabela" height="100%" width="100%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 style="width:100%;">
					
						<?php if($rsCreches): ?>
							<?php foreach($rsCreches as $creches): ?>
								<?php if($creches['mdsquantidadepbf'] > $creches['prcqtdalunoinfantil']): ?>
									<?php $arPendencias[] = 5; ?>
									<tr>
										<td class="subtituloDireita" width="60"><img src="../imagens/excluir_2.gif" align="absmiddle" /></td>
										<td style="background-color:#EDEDED;">&nbsp; Na creche <?php echo $creches['entnome']; ?> a quantidade de PBF é maior que a quantidade de matrículas;</td>
									</tr>                                                             
								<?php endif; ?>
							<?php endforeach; ?>
						<?php endif; ?>
					
						<?php if(!$rs['alunos_pbf']): ?>
							<?php $arPendencias[] = 1; ?>
							<tr>
								<td class="subtituloDireita" width="60"><img src="../imagens/excluir_2.gif" align="absmiddle" /></td>
								<td style="background-color:#EDEDED;">&nbsp;Necessário preencher a aba Lista de Creche;</td>
							</tr>
						<?php endif; ?>
						
						<?php if($rs['alunos_pbf'] > $rs['alunos_mun']): ?>
							<?php $arPendencias[] = 2; ?>
							<tr>
								<td class="subtituloDireita" width="60"><img src="../imagens/excluir_2.gif" align="absmiddle" /></td>
								<td style="background-color:#EDEDED;">&nbsp;O total de crianças informadas como beneficiárias do Programa Bolsa Família é superior aos beneficiários de 0 a 48 meses de famílias que recebem Bolsa Família no Município;</td>
							</tr>
						<?php endif; ?>
						
						<?php if($rs['alunos_pbf'] > $rs['alunos_mat']): ?>
							<?php $arPendencias[] = 3; ?>
							<tr>
								<td class="subtituloDireita" width="60"><img src="../imagens/excluir_2.gif" align="absmiddle" /></td>
								<td style="background-color:#EDEDED;">&nbsp;O total de crianças informadas como beneficiárias do Programa Bolsa Família é superior ao quantitativo de matrículas declaradas no Educacenso;</td>
							</tr>
						<?php endif; ?>
						
						<?php 
						$tapa 	 = $rs['alunos_mes'];
						$tampbf  = $rs['alunos_mun'];
						$tamc 	 = $rs['alunos_mat'];
						$tamcpbf = $rs['alunos_pbf'];
						$calculo1 = ($tampbf/$tapa)*100;
						$calculo2 = ($tamcpbf/$tamc)*100;					
						?>
						
						<?php if($calculo1 < $calculo2): ?>
							<?php $arPendencias[] = 4; ?>
							<tr>
								<td class="subtituloDireita" width="60"><img src="../imagens/AtencaoP.gif" align="absmiddle" /></td>
								<td style="background-color:#EDEDED;">&nbsp;O total de crianças informadas como beneficiárias do Programa Bolsa Família é superior ao % médio de atendimento da população de 0 a 48 meses no Município. É necessário justificar porque o atendimento ao público do Bolsa Família é superior ao atendimento ao público geral na faixa etária de 0 a 48 meses;</td>
							</tr>
							<tr>
								<td class="subtituloDireita" valign="top">
									&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif" />
								</td>
								<td>								
									<?php echo campo_textarea('mjmjustificativa', 'S', 'S', $label, 65, 8, 4000); ?>
								</td>
							</tr>
						<?php endif; ?>
						
						<?php if(in_array(4, $arPendencias)): ?>
							<tr>
								<td class="subtituloDireita" valign="top">
									&nbsp;
								</td>
								<td class="subtituloEsquerda">								
									<input type="button" value="Salvar" id="btnSalvar" />
								</td>
							</tr>
						<?php endif; ?>
						
						<?php if($esdid == WF_MDS_EM_CADASTRAMENTO && (empty($arPendencias) || ($mjmjustificativa && !in_array(1,$arPendencias) && !in_array(2,$arPendencias) && !in_array(3,$arPendencias)))): ?>
							<tr>							
								<td colspan="2" height="160">
									<center>
										<p><img src="../imagens/check_checklist.png" /></p>
										<font size="+1">O preenchimento dos dados foi concluído com sucesso! Para dar prosseguimento à solicitação, é necessário enviá-la para análise. Deseja enviá-la agora?</font>
										<p><input type="button" value="Sim" id="btnTramitar"/>
										<input type="button" value="Não" /></p>
									</center>
								</td>
							</tr>
						<?php endif; ?>
						
						<?php if($esdid == WF_MDS_EM_ANALISE): ?>
							<tr>							
								<td colspan="2" height="160">
									<center>
										<font size="+1">Enviado para análise.</font>
									</center>
								</td>
							</tr>
						<?php endif; ?>
					</table>		
				</td>			
			</tr>
		</table>
	</form>
</center>
<?php if(false): ?>
	<div style="position:absolute;right:80px;top:245px;">
		<?php
		$oculta = true;
		if(checkPerfil(array(PERFIL_ADMINISTRADOR))){
			$oculta = false;
		}
		wf_desenhaBarraNavegacao( $docid , array('cpmid' => $cpmid),  array('historico'=>$oculta) );
		?>
	</div>
<?php endif; ?>