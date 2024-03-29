<?php
	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>, Alexandre Soares Diniz
	 * Programadores: Renê de Lima Barbosa <renedelima@gmail.com>, Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
	 * Módulo: segurança
	 * Finalidade: Consultar informações a resppeito das contas dos usuários
	 * Data de criação:
	 * Última modificação: 29/08/2006
	 */

	$modulo = $_REQUEST['modulo'] ;
	$encontrou = false;

	
if ($_POST['ajaxsisid']){
	monta_perfil($_POST['ajaxsisid']);
	exit;	
}
	
if ($_SESSION['sisid']!=4) {$habil = 'N'; $_REQUEST['codigo']=$_SESSION['sisid'];} else $habil = 'S';

if ( $_REQUEST['formulario'] ) {
		$usucpf  = $_REQUEST['usucpf'];
		$usunome = $_REQUEST['usunome'];
		$codigo  = $_REQUEST['codigo'];
		$suscod  = $_REQUEST['suscod'];
		$pflcod  = $_REQUEST['pflcod'];
		$muncod  = $_REQUEST['muncod'];		
		$usuchaveativacao = $_REQUEST['usuchaveativacao'];
		$sitperfil = $_REQUEST['sitperfil'];
	} else {
		$codigo = $_SESSION['sisid'];
	}

if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'listar_mun'){	
	$sql = "SELECT muncod, mundescricao as mundsc 
		FROM territorios.municipio 
		WHERE estuf = '".$_REQUEST['estuf']."' ORDER BY mundsc";
	$dados = $db->carregar($sql);
	
	$enviar = '';
	if($dados) $dados = $dados; else $dados = array();
			$enviar .= "<option value=\"\"> Todos </option> \n";
	foreach ($dados as $data) {
		$enviar .= "<option value= ".$data['muncod'].">  ".simec_htmlentities($data['mundsc'])." </option> \n";
	}
			
	die($enviar);
}
	
?>
<?php include APPRAIZ . "includes/cabecalho.inc"; ?>
<br/>
<?php

	$db->cria_aba( $abacod_tela, $url, $parametros );
	if ( $_REQUEST['acao'] == 'A') {
		$titulo_modulo = 'Alterar o Cadastro de Usuários do Simec';
	}
	if ( $_REQUEST['acao'] == 'C' ) {
		$titulo_modulo = 'Consultar o Cadastro de Usuários do Simec';
	}
	monta_titulo( $titulo_modulo, '' );

?>
<form method="POST" name="formulario">
<script type="text/javascript" src="../includes/prototype.js"></script>
	<input type="hidden" name="formulario" value="1"/>
	<input type=hidden name="modulo" value="<?= $modulo ?>"/>
	<input type=hidden name="act" value="0"/>
	
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">CPF (ou parte do CPF):</td>
			<td><?=campo_texto('usucpf','','','',16,14,'###.###.###-##','', '', '', '', '', '', '', "this.value=mascaraglobal('###.###.###-##',this.value);");?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Nome completo (ou parte do nome):</td>
			<td><?=campo_texto('usunome','','','',50,50,'','');?></td>
		</tr>
		<!--
		<tr>
		<td align='right' class="SubTituloDireita">Município:</td>
			<td><?
				//campo_texto('muncod','','','',10,7,'#######','');
			?></td>
		</tr>
		-->
		<tr bgcolor="#F2F2F2">
			<td align='right' class="SubTituloDireita">Unidade Federativa:</td>
			<td>
				<?php
					$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
					$db->monta_combo("regcod",$sql,"","&nbsp;",'listar_municipios','');
					//echo  obrigatorio(); 
				?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Município:</td>
			<td>
				<div id="muncod_on" style="display:none;">
					<select
						id="muncod"
						name="muncod"
						onchange=""
						class="CampoEstilo"
						<?
						///3campo  do monta combo - verifica se é editavel
						//$editavel == 'S' ? "" : 'disabled="disabled"' 
						?>
					></select>
					<?
					//echo obrigatorio(); 
					?>
				</div>
				<div id="muncod_off" style="color:#909090;">Selecione uma Unidade Federativa.</div>
			</td>
		</tr>
		
		
		<tr>
			<td align='right' class="SubTituloDireita">Sistema:</td>
			<td>
				<?php
					$sql = "select s.sisid as codigo, s.sisdsc as descricao from seguranca.sistema s where s.sisstatus='A'";
					$db->monta_combo( "codigo", $sql,$habil, 'Qualquer (selecione)', 'montar_perfil', '' );
				?>	
			</td>
		</tr>
		<?php if( $codigo ): ?>
		<tr>
			<td align='right' class="SubTituloDireita">Perfil:</td>
			<td id="pflid">
				<?php
//					$pflnivel = $db->pegaUm( "select min( pflnivel ) from seguranca.perfil p inner join seguranca.perfilusuario pu on pu.pflcod = p.pflcod and pu.usucpf = '". $_SESSION['usucpf'] ."'" );
//					$sql = sprintf(
//						"select p.pflcod as codigo, p.pfldsc as descricao from seguranca.perfil p where p.sisid = %d and p.pflstatus = 'A' and p.pflnivel >= %d",
//						$codigo,
//						$pflnivel
//					);
/*
				$arrPerfil = pegaPerfilgeral(); //pega todos os perfis do usuário
				$arrPerfil = retornaPflcodFilhos($arrPerfil); //retornar todos os perfis associados (seguranca.perfilpermissao)
				$perfilSuperUser = $db->testa_superuser(); //testa se o usuário é super usuário
				
				if(!$perfilSuperUser){
				
					$sql = "select
								pflcod AS codigo,
								pfldsc AS descricao
							FROM
								 seguranca.perfil per
							WHERE
								 pflstatus = 'A' 
							AND
								sisid = ".$codigo."
							AND
								pflcod in (".implode(",",$arrPerfil).")
						    ORDER BY
						    	pfldsc";
					
				}else{
						$sql = "SELECT
							 pflcod AS codigo,
							 pfldsc AS descricao
						FROM
							 seguranca.perfil
						WHERE
							 pflstatus = 'A' and sisid = ".$codigo."
						order by
							pfldsc";
				}
*/			
						$sql = "SELECT
							 pflcod AS codigo,
							 pfldsc AS descricao
						FROM
							 seguranca.perfil
						WHERE
							 pflstatus = 'A' and sisid = ".$codigo."
						order by
							pfldsc";
				
		
					$pflcod = $_POST['pflcod'];
					
					$db->monta_combo( "pflcod", $sql, 'S', 'Todos', '', '' );
				?>	
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Unidade Orçamentária:</td>
			<td>
			<?php
				/*$sql = "SELECT 
							u2.unicod as codigo, u.unidsc || ' - ' || u2.unidsc as descricao
						FROM
							public.unidade u
						INNER JOIN 
							public.unidade u2 on u2.orgcod = u.unicod
						INNER JOIN 
							seguranca.usuario usu on usu.unicod = u2.unicod
						INNER JOIN 
							seguranca.usuario_sistema us on us.usucpf = usu.usucpf
						WHERE
							u2.unistatus='A' AND u2.unitpocod = 'U' AND us.sisid = ". $_SESSION['sisid'] ."
						GROUP BY 
							u2.unicod, u.unidsc, u2.unidsc
						ORDER BY 
							u2.unicod, u.unidsc, u2.unidsc";
				*/
			//Alterado o SQL acima pois deve pegar todas as unidades independente de está vinculado com os usuários
				$sql = 	"SELECT 
							unicod as codigo, unidsc as descricao
						FROM public.unidade
							where orgcod = '26000' and unistatus = 'A' 
							ORDER BY unicod, unidsc";
				campo_popup('unidade',$sql,'Selecione','','400x400','30', 
								array(
									array("codigo" => "unicod","descricao" => "Código","numeric" => "1"),
									array("codigo" => "unidsc || ' - ' || unidsc","descricao" => "Descricão","numeric" => "0")),
									1
								);
			?>
			</td>
		</tr>		
		<?php endif; ?>
		<? if(!isset($_REQUEST['formulario'])) $suscod = 'A'; ?>
		<?php if ( $_REQUEST['acao'] == 'A' || $_REQUEST['acao'] == 'C' ): ?>
			<tr bgcolor="#F2F2F2">			
				<td align='right' class="SubTituloDireita">Status geral do usuário:</td>
				<td>
					<input id="status_qualquer" type="radio" name="suscod" value="" <?= $suscod == '' ? 'checked="checked"' : "" ?>/>
					<label for="status_qualquer">Qualquer</label>
					
					<input id="status_ativo" type="radio" name="suscod" value="A" <?= $suscod == 'A' ? 'checked="checked"' : "" ?>/>
					<label for="status_ativo">Ativo</label>
					
					<input id="status_pendente" type="radio" name="suscod" value="P" <?= $suscod == 'P' ? 'checked="checked"' : "" ?>/>
					<label for="status_pendente">Pendente</label>
					
					<input id="status_bloqueado" type="radio" name="suscod" value="B" <?= $suscod == 'B' ? 'checked="checked"' : "" ?>/>
					<label for="status_bloqueado">Bloqueado</label>
				</td>
			</tr>
		<?php endif; ?>
		<tr bgcolor="#F2F2F2">			
			<td align='right' class="SubTituloDireita">Já acessou o sistema:</td>
			<td>
				<input id="acesso_qualquer" type="radio" name="usuchaveativacao" value="" <?= $usuchaveativacao == '' ? 'checked="checked"' : "" ?>/>
				<label for="acesso_sim">Qualquer</label>
			
				<input id="acesso_sim" type="radio" name="usuchaveativacao" value="t" <?= $usuchaveativacao == 't' ? 'checked="checked"' : "" ?>/>
				<label for="acesso_sim">Sim</label>
				
				<input id="acesso_nao" type="radio" name="usuchaveativacao" value="f" <?= $usuchaveativacao == 'f' ? 'checked="checked"' : "" ?>/>
				<label for="acesso_nao">Não</label>
			</td>
		</tr>
		<tr bgcolor="#F2F2F2">			
			<td align='right' class="SubTituloDireita">Situação Perfil:</td>
			<td>
				<input type="radio" name="sitperfil" value="V" onclick="SelPerfil();" <?= $sitperfil == 'V' ? 'checked="checked"' : "" ?>/>
				<label >Qualquer</label>
			
				<input  type="radio" name="sitperfil" value="D" onclick="SelPerfil();" <?= $sitperfil == 'D' ? 'checked="checked"' : "" ?>/>
				<label >Desejado</label>
				
				<input type="radio" name="sitperfil" value="A" onclick="SelPerfil();" <?= $sitperfil == 'A' ? 'checked="checked"' : "" ?>/>
				<label >Atribuído</label>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0">
			<td ></td>
			<td><input type='button' class="botao" name='consultar' value='Consultar' onclick="ProcuraUsuario('')">
			<input type='button' class="botao" name='visualizarXls' value='Visualizar XLS' onclick="ProcuraUsuario('xls')"></td>
		</tr>
	</table>
</form>
<?php
	unset( $nlinhas );
	unset( $sql );

	$cabecalho = array( 'Ações', 'CPF', 'Nome Completo', 'Telefone', 'UF', 'Município', 'Órgão', 'Unidade', 'Cargo / Função',  'Data da Última Atualização' );
	
	// insere filtros de consulta
	if ($_REQUEST['usunome']) 		 $texto = " and TRANSLATE(usuario.usunome, 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ilike TRANSLATE('%".$_REQUEST['usunome']."%', 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ";
	if ($_REQUEST['usucpf']) 		 $texto.= " and usuario.usucpf like '%".str_to_upper(corrige_cpf($_REQUEST['usucpf']))."%'";
	if ($_REQUEST['suscod']) 		 $texto.= " and usuariosistema.suscod = '".$_REQUEST['suscod']."'";
	if ($_REQUEST['codigo']) 		 $texto.= " and usuariosistema.sisid = '".$_REQUEST['codigo']."'";
	/*UF e Municipio*/
	if ($_REQUEST['regcod_disable']) $texto.= " and usuario.regcod = '".$_REQUEST['regcod_disable']."'";
	if ($_REQUEST['muncod']) 		 $texto.= " and usuario.muncod = '".$_REQUEST['muncod']."'";
	//Unidade Orcamentaria
	if ( $_REQUEST['unidade'] ) 	 $texto.= " and unidadex.unicod = '" . (integer) $_REQUEST['unidade'] . "'" ;	
	
	// Se já acessou o sistema
	if ( $_REQUEST["usuchaveativacao"] ) $texto .= " and usuario.usuchaveativacao = '" . $_REQUEST["usuchaveativacao"] . "'";

	if ( $_REQUEST["sitperfil"] == '' ){
		//$_REQUEST["sitperfil"] = 'V';
		$_REQUEST["sitperfil"] = 'A';
	}
	
	// verifica situação do perfil (Desejado ou atribuido)
	if ( $_REQUEST["sitperfil"] == 'D' ){
		if ( $_REQUEST["pflcod"] ) $texto .= " and (usuariosistema.pflcod = " . $_REQUEST["pflcod"] . ")";
	}
	else if( $_REQUEST["sitperfil"] == 'A' ){
		if ( $_REQUEST["pflcod"] ) $texto .= " and (perfil.pflcod = " . $_REQUEST["pflcod"] . ")";
	}
	else if( $_REQUEST["sitperfil"] == 'V' ){
		if ( $_REQUEST["pflcod"] ) $texto .= " and (perfil.pflcod = " . $_REQUEST["pflcod"] . " OR usuariosistema.pflcod = " . $_REQUEST["pflcod"] . ")";
	}
	
	//Verifica usuário responsabilidade
	//Verifica se exite a tabela 'usuarioresponsabilidade' no módulo
	/*
	if($_SESSION['sisdiretorio'] && $_SESSION['usucpf'] && !$perfilSuperUser){
		
		//Verifica Permissão de Perfil (seguranca.perfilpermissao)
		if($arrPerfil){
			$andPerfilPermissao = "AND perfil.pflcod in (".implode(",",$arrPerfil).") ";
		}
		
		$sql = "SELECT 
					count(1)
				FROM 
					pg_namespace n, pg_class c
				WHERE 
					n.oid = c.relnamespace
				AND
					c.relkind = 'r'     -- no indices
				AND
					n.nspname not like 'pg\\_%' -- no catalogs
				AND
					n.nspname != 'information_schema' -- no information_schema
				AND
					n.nspname = '{$_SESSION['sisdiretorio']}'
				AND
					c.relname = 'usuarioresponsabilidade'";
		if($db->pegaUm($sql)){
			$sql = "select * from {$_SESSION['sisdiretorio']}.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A'";
			$arrDados = $db->carregar($sql);
			if($arrDados){
				foreach($arrDados as $dado){
					foreach($dado as $campo => $valor){
						if($campo != "rpuid" && $campo != "pflcod" && $campo != "usucpf" && $campo != "rpustatus" && $campo != "rpudata_inc"){
							if($valor){
								$arrCampo[$campo][] = $valor;
							}
						}
					}
				}
			}
		}
	}
	*/
	if($arrCampo){
		foreach($arrCampo as $campo => $valor){
			if($campo && is_array($valor)){
				$arrWhere[] = "resp.$campo in ('".implode("','",$valor)."') ";
			}
		}
		$arrWhere[] = "resp.rpustatus = 'A'";
		$innerResponsabilidade = " INNER JOIN {$_SESSION['sisdiretorio']}.usuarioresponsabilidade resp ON usuario.usucpf = resp.usucpf ".($arrWhere ? " AND ( ".implode(" AND ",$arrWhere)." ) " : "");
	}
		
	if ( $_REQUEST['pflcod'] ) {
		$join_sql = sprintf(
				"
				seguranca.perfil perfil 
				inner join seguranca.perfilusuario perfilusuario    on perfil.pflcod = perfilusuario.pflcod and perfil.pflcod = %d
				right join seguranca.usuario usuario		    	on usuario.usucpf = perfilusuario.usucpf
				left join 
				(
				  select 
					unicod, 
					unidsc
				  from 
					public.unidade
				  where 
					unitpocod = 'U'
				) as unidadex on usuario.unicod = unidadex.unicod				
				",
				$_REQUEST['pflcod']
		);
	} else {
		$join_sql =
			"
			(
			  select 
				unicod, 
				unidsc
			  from 
				public.unidade
			  where 
				unitpocod = 'U'
			) as unidadex
			right join seguranca.usuario usuario	on usuario.unicod = unidadex.unicod
			left join seguranca.perfilusuario perfilusuario on usuario.usucpf = perfilusuario.usucpf
			left join seguranca.perfil perfil on perfil.pflcod = perfilusuario.pflcod 
			"; 		
	}
		
	// seleciona qual ação o botão que aparece na listagem deve acionar
	if ( $_REQUEST['acao'] == 'A' ) {
		$funcao = "alterar_usu";
	} else {
		$funcao = "consultar_usu";
	}
	
	if ( $_REQUEST['act'] == 'procura' ) {
		$campos = "'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Cadastro \" onclick=\"$funcao(' || '''' || usuario.usucpf || '''' || ')\">
		   		<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar e-mail \" onclick=\"enviarEmail_usu(\"../geral/envia_email_usuario.php?usuID=\",673,515,' || '''' || usuario.usucpf || '''' || ')\">' as acao,  
		   		usuario.usucpf, 
		   		usuario.usunome as nomeusuario,		   			   		
		   		'(' || usuario.usufoneddd || ') ' 
		   		|| usuario.usufonenum as fone , 
		   		usuario.regcod, 
		   		municipio.mundescricao, 
		   		entidade.entnome as orgao,
				CASE WHEN entidade.entid = 390402 THEN trim(usuario.orgao) ELSE trim(unidadex.unidsc) END as unidsc,
		   		COALESCE(cargo.cardsc,'')||' / '||COALESCE(usuario.usufuncao,'') as cargo,
				to_char(usuario.usudataatualizacao,'dd/mm/YYYY HH24:MI') as data";
		
		$groupby = "usuario.usucpf, usuario.usunome, usuario.usufoneddd, 
				usuario.usufonenum, usuario.regcod, entidade.entid, entidade.entnome, 
				unidadex.unidsc, usuario.orgao, usuario.usudataatualizacao , municipio.mundescricao, cargo.cardsc, usuario.usufuncao";
	} elseif( $_REQUEST['act'] == 'visualizaXls' ){
		$campos = "municipio.muncod, 
		   		municipio.mundescricao, 
		   		usuario.regcod,
		   		usuario.usucpf, 
		   		usuario.usunome as nomeusuario,	
		   		usufuncao,	   			   		
		   		'(' || usuario.usufoneddd || ') ' 
		   		|| usuario.usufonenum as fone, 
		   		usuario.usuemail,
		   		entidade.entnome as orgao,
		   		CASE WHEN entidade.entid = 390402 THEN trim(usuario.orgao) ELSE trim(unidadex.unidsc) END as unidsc,
		   		to_char(usuario.usudataatualizacao,'dd/mm/YYYY HH24:MI') as data
		   		";
		
		$groupby = "municipio.muncod, municipio.mundescricao, usuario.regcod, usuario.usucpf, usuario.usunome, usufuncao, usuario.usufoneddd, 
					usuario.usufonenum, usuario.usuemail, entidade.entnome, entidade.entid, usuario.orgao, unidadex.unidsc, usuario.usudataatualizacao";
		
	}

    //se o acesso for através do MODULO SEGURANCA SIS
    if ($_SESSION['sisid'] == 4) {
        $join_usuario_sistema = ' left join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf ';
    } else {
        $join_usuario_sistema = ' inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf ';
    }
		
	$sql= " SELECT DISTINCT  
		   		 	$campos
			FROM 
				" . $join_sql . "

			left join  territorios.municipio municipio 	    	on municipio.muncod = usuario.muncod
			{$join_usuario_sistema}
			left join  entidade.entidade entidade		    	on usuario.entid = entidade.entid
			left join  public.cargo cargo		    			on cargo.carid = usuario.carid
			$innerResponsabilidade	
			WHERE 
				usunome is not null $texto $andPerfilPermissao
			GROUP BY 
				$groupby
			ORDER BY 
				nomeusuario";

	// exibe o resultado da consulta
	if ( $_REQUEST['act'] == 'procura' ) {
		$db->monta_lista( $sql, $cabecalho, 100, 20, '', '' ,'' );
	} elseif( $_REQUEST['act'] == 'visualizaXls' ){
		ob_clean();
		
		$arCabecalho = array('Cód Município', 'Município', 'UF', 'CPF', 'Nome', 'Função', 'Telefone', 'E-mail', 'Órgão', 'Unidade', 'Data da Última Atualização');
		$arDados = $db->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();

		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelUsuario_".$_SESSION['sisdiretorio'].date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelUsuario_".$_SESSION['sisdiretorio'].date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%');
	    die;
		
	}
	
	
function monta_perfil($sisid = null){
	global $db;
	
	if (!$sisid){
		return false;
	}
	
	$sql = "SELECT
			 pflcod AS codigo,
			 pfldsc AS descricao
			FROM
			 seguranca.perfil
			WHERE
			 pflstatus = 'A' AND
			 sisid = $sisid";
	
	$db->monta_combo( "pflcod", $sql, 'S', 'Todos', '', '' );	
}
?>
<script language="javascript">

	function envia_email( cpf ){
		e = "?modulo=sistema/geral/envia_email&acao=A&cpf=" + cpf;
		window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=590,height=490");
	}


	function montar_perfil(sisid){
		if (sisid == ''){
			return;	
		}
		if (document.getElementsByName('pflcod')){
			document.getElementsByName('pflcod')[0].disabled = true;	
		}	
		// Faz uma requisição ajax							
		var req = new Ajax.Request('?modulo=sistema/usuario/consusuario&acao=A', {
									method:      'post',
									parameters:  '&ajaxsisid=' + sisid,
									asynchronous: false,
									onComplete:   function (res)
									{	
	
		        						document.getElementById('pflid').innerHTML = res.responseText;	
		        								  
									}
								});
	}

	/*
		from cadastrar_usuario_2.php
	*/
	<?php
		//$sql = "SELECT estuf, muncod, estuf || ' - ' || mundescricao as mundsc FROM territorios.municipio ORDER BY 3 ";
	?>
	//var lista_mun = new Array();
	<?// $ultimo_cod = null; ?>
	<?// foreach ( $db->carregar( $sql ) as $unidade ) :?>
		<?// if ( $ultimo_cod != $unidade['estuf'] ) : ?>
			//lista_mun['<?= $unidade['estuf'] ?>'] = new Array();
			<? //$ultimo_cod = $unidade['estuf']; ?>
		<? //endif; ?>
		//lista_mun['<?= $unidade['estuf'] ?>'].push( new Array( '<?= $unidade['muncod'] ?>', '<?= addslashes( trim( $unidade['mundsc'] ) ) ?>' ) );
	<?// endforeach; ?>
	
	
	
	function listar_municipios( estuf )
    {
    	validar_mun = true;    
        var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );        
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		
		/*		
        return new Ajax.Updater('muncod', '<?=$_SESSION['sisdiretorio'] ?>.php?modulo=sistema/usuario/consusuario&acao=<?=$_REQUEST['acao'] ?>',
         {
         
            method: 'post',
            parameters: '&servico=listar_mun&estuf=' + estuf,
            onComplete: function(res)
            {	
            // alert(res.responseText);                   
            }
        });
        */

        var url = '<?=$_SESSION['sisdiretorio'] ?>.php?modulo=sistema/usuario/consusuario&acao=<?=$_REQUEST['acao'] ?>';

        <?php if ( $_SESSION['sisid'] == 147 ): ?>
        var url = '<?=$_SESSION['sisarquivo']?>.php?modulo=sistema2/usuario2/consusuario&acao=<?=$_REQUEST['acao'] ?>';
        <?php endif; ?>

        // Faz uma requisição ajax, passando o parametro 'ordid', via POST
        var req = new Ajax.Request(url, {
            method:     'post',
            parameters: '&servico=listar_mun&estuf=' + estuf,
            onComplete: function (res)
            {
                document.getElementById( 'muncod' ).innerHTML = res.responseText;
            }
        });
							  
    }
	
	function listar_municipios2( regcod )
	{
		
		
		var campo_select = document.getElementById( 'muncod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		
		var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );

		if ( !lista_mun[regcod] )
		{
			validar_mun = false;
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			return;
		}
		
		validar_mun = true;
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		var j = lista_mun[regcod].length;
		for ( var i = 0; i < j; i++ )
		{
			campo_select.options[campo_select.options.length] = new Option( lista_mun[regcod][i][1], lista_mun[regcod][i][0], false, lista_mun[regcod][i][0] == '<?= $muncod ?>' );
		}
		for ( i=0; i< campo_select.options.length; i++ )
		{
			if ( campo_select.options[i].value == '<?= $muncod ?>' ) {
				campo_select.options[i].selected = true;
			}
		}
	}
		



	function ProcuraUsuario(tipo) {
		if ( document.formulario.usunome ) {
			if(tipo == 'xls'){
				document.formulario.act.value = "visualizaXls";
			} else {
				document.formulario.act.value = "procura";
			}
			document.formulario.submit()
		}
	}

	function consultar_usu( usucpf ){
	<?php if ( $_SESSION['sisid'] == 147 ): ?>
		window.location = "<?=$_SESSION['sisarquivo']?>.php?modulo=sistema2/usuario2/cadusuario&acao=C&usucpf="+ usucpf;
	<?php else: ?>
		window.location = "<?=$_SESSION['sisarquivo']?>.php?modulo=sistema/usuario/cadusuario&acao=C&usucpf="+ usucpf;
	<?php endif; ?>
	}

	function alterar_usu( usucpf ){
	<?php if ( $_SESSION['sisid'] == 147 ): ?>
		window.location = "<?=$_SESSION['sisarquivo']?>.php?modulo=sistema2/usuario2/cadusuario&acao=A&usucpf="+ usucpf;
	<?php else: ?>
		window.location = "<?=$_SESSION['sisarquivo']?>.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf="+ usucpf;
	<?php endif; ?>
	}

	function SelPerfil(){
		if ( document.formulario.pflcod.value == '' ) {
			alert('Selecione o Perfil antes de escolher a Situação Perfil.');
			document.formulario.sitperfil[0].checked = false;
			document.formulario.sitperfil[1].checked = false;
			document.formulario.sitperfil[2].checked = false;
		}
	}

</script>