<?php
# carrega as bibliotecas
include_once APPRAIZ . "includes/PogProgressBar.php";
include_once APPRAIZ . "includes/envia_email_sis_geral_funcoes.inc";
include_once APPRAIZ . "includes/Agrupador.php";

if( $_REQUEST['req'] == 'exibirDestinatarios' ){

	$_SESSION['municipios'] = $_REQUEST['arMunicipio'] ? substr( $_REQUEST['arMunicipio'], 0, -2 ) : '';
    die();
} 

switch($_SESSION['sisarquivo']) {
	case 'cte':
		
		$_REQUEST['statusUsuario'] = ($_REQUEST['statusUsuario'] ) ? $_REQUEST['statusUsuario'] : 'A';
		
		$suscod = $_REQUEST['statusUsuario'] ? " us.suscod = '". $_REQUEST['statusUsuario'] ."' and ": ""; 
		$exibefiltromunicipios = true; // Mostra o filtro por municipios
		
		if($_REQUEST['filtromun'] == 'sim' && $_REQUEST['perfil'][0] ) {
			if($_REQUEST['perfil'] && $_REQUEST['municipios']) {
				
				// carregando os perfis escolhidos que possam ser filtrados por municipio
				$perfismun = $db->carregar("SELECT * FROM cte.tprperfil WHERE pflcod IN('". implode("','", $_REQUEST['perfil']) ."') AND tprcod = '2'");
				// se algum dos perfis selecionados pode ser filtrado por municipio
				if($perfismun) {
					$_REQUEST['perfil'] = array_flip($_REQUEST['perfil']);
					
					$joinIdeb = $_REQUEST['ideb'] ? " INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = ur.muncod " : "";
					$clausulaIdeb = $_REQUEST['ideb'] ?  " AND mtm.tpmid IN ( ". implode(", ", $_REQUEST['ideb'] ) ." ) " : "";
					
					foreach($perfismun as $per) {
						$sql = "SELECT distinct u.usuemail, u.usunome FROM cte.usuarioresponsabilidade ur
								LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf  
								INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
								$joinIdeb
								WHERE $suscod 
								ur.pflcod = ". $per['pflcod'] ."
								AND us.sisid = ". $_SESSION["sisid"] ." 
								AND rpustatus = 'A' 
								AND ur.muncod IN('". implode("','", $_REQUEST['municipios']) ."')
								$clausulaIdeb";
			
						$desti = $db->carregar($sql);
						// se existe algum usuario desses filtros, processar na op��o de outros
						if($desti) {
							foreach($desti AS $de) {
								$_REQUEST["pessoas"] .= $de['usunome']." <".$de['usuemail'].">,"; 
							}
						}
						unset($_REQUEST['perfil'][$per['pflcod']]);							
					}
					$_REQUEST['perfil'] = array_flip($_REQUEST['perfil']);
				}
			}
		}
	break;
}

//print_r($_REQUEST);
if ( $_REQUEST[ "formulario" ] == 2 ) {
	
	# captura as informa��es submetidas
	$orgao = (integer) $_REQUEST["orgao"] > 2 ? $_REQUEST["orgao"] : null;
	$tipoEnsino = (integer) $_REQUEST["tipoensino"] ? $_REQUEST["tipoensino"] : null;
	$uo = (array) $_REQUEST["unidadeorcamentaria"];
	$ug = (integer) $_REQUEST["unidadegestora"] ? $_REQUEST["unidadegestora"] : null;
	$perfis = (array) $_REQUEST["perfil"];
	$ideb = (array) $_REQUEST["ideb"];
	$outros = $_REQUEST["pessoas"];
	$assunto = $_REQUEST["assunto"];
	$conteudo = $_REQUEST["mensagem"];
	$statusUsuario = $_REQUEST["statusUsuario"];
	
	# identifica os destinat�rios
	$destinatarios = EmailSistema::identificar_destinatarios( $orgao, $tipoEnsino, $uo, $ug, $perfis, $outros, $statusUsuario, $ideb );

	if ( empty( $destinatarios ) ) {
		echo '<script type="text/javascript">alert( "N�o h� destinat�rios para os filtros indicados." )</script>';
	}
	else{	
	
		$sql = 			
				"select distinct u.usunome, u.usuemail, u.usucpf
				from seguranca.usuario u				
				where u.usucpf = '".$_SESSION['usucpf']."'
			    group by u.usunome, u.usuemail, u.usucpf						
			    ";
								
		$remetente = $db->carregar( $sql );
	
		array_push($destinatarios, $remetente);
	}
}
include_once APPRAIZ."includes/cabecalho_bootstrap_v3.inc";
?>
<!-- AUTO COMPLEMENTO -->
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/keys/keyEvents.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/tags/suggest.js"></script>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<!-- <script type="text/javascript" src="../includes/prototype.js"></script> -->
    
<style>
	.suggestMarked
	{
		background-color: #AAAAFF;
		width:500px; 
		border-style: solid; 
		border-width: 1px; 
		border-color: #DDDDFF;
		border-top-width: 0px;
		position: relative;
		z-index: 100;
	}
	.suggestUnmarked
	{
		background-color: #EEEEFF;
		width:500px; 
		border-style: solid; 
		border-width: 1px; 
		border-color: #DDDDFF;
		border-top-width: 0px;
		z-index: 100;
		position: relative;
	}
	
	fieldset{
		width: 475px;
		padding: 10px;
	}
	
	fieldset input{
		width: 150px;
	}
	fieldset label{
		display: block;
		width: 40px;
		float: left;
	}
	
	fieldset br{
		clear: both;
	}
	
</style>

<!-- EDITOR DE TEXTO -->
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="javascript" type="text/javascript">
	tinyMCE.init({
	 	mode : "specific_textareas",
	    textarea_trigger : "mce_editable",
		theme : "advanced",
		plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
		theme_advanced_buttons1 : "undo,redo,separator,bold,italic,underline,forecolor,backcolor,fontsizeselect,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist, code",
		theme_advanced_buttons2 : "",
		theme_advanced_buttons3 : "",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		extended_valid_elements : "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
		language : "pt_br",
		entity_encoding : "raw"
	});
</script>

<!-- EVENTOS DO FORMUL�RIO -->
<script language="javascript" type="text/javascript">
	function abrepopupMunicipio(){
		window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/combo_municipios_enviaemail.php','Municipios','width=400,height=400,scrollbars=1');
	}
	
	function abrepopupMunicipioUsuario(){
		window.open('http://<?=$_SERVER['SERVER_NAME']?>/geral/combo_municipios_enviaemail.php','Municipios','width=400,height=400,scrollbars=1');
	}
		
	function exibir_destinatarios() {
		// captura os campos
		var campo_orgao = document.getElementById( "orgao" );
		var campo_uo = document.getElementById( "unidadeorcamentaria" );
		var campo_ug = document.getElementById( "unidadegestora" );
		var campo_perfis = document.getElementById( "perfil" );
		var campo_municipios = document.getElementById( "municipios" );
		var campo_outros = document.getElementById( "pessoas" );
		var obStatusUsuario = document.getElementById( "statusUsuario" );
		var obIDEB = document.getElementById( "ideb" );
		// valores dos campos capturados
		var orgao = "";
		var uo = new Array();
		var ug = "";
		var perfis = new Array();
		var municipios = new Array();
		var arIDEB = new Array();
		var outros = "";
		// identifica o �rg�o selecionado
		if ( campo_orgao ) {
			if ( campo_orgao.selectedIndex != 0 ) {
				orgao = campo_orgao.options[campo_orgao.selectedIndex].value;
			}
		}
		if ( orgao ) {
			// captura as unidades or�ament�rias
			if ( campo_uo ) {
				for ( i=0; campo_uo.options[i]; i++ ) {
					if ( campo_uo.options[i].value ) {
						uo.push( campo_uo.options[i].value );
					}
				}
			}
			// captura as unidades gestoras
			if ( campo_ug ) {
				ug = campo_ug.options[campo_ug.selectedIndex].value;
			}
		}
		// captura os perfis
		if ( campo_perfis ) {
			for ( i=0; campo_perfis.options[i]; i++ ) {
				if ( campo_perfis.options[i].value ) {
					perfis.push( campo_perfis.options[i].value );
				}
			}
		}
		
		// captura IDEB
		if ( obIDEB ) {
			for ( i=0; obIDEB.options[i]; i++ ) {
				if ( obIDEB.options[i].value ) {
					arIDEB.push( obIDEB.options[i].value );
				}
			}
		}
		
		// captura os municipios
		if ( campo_municipios ) {
			for ( i=0; campo_municipios.options[i]; i++ ) {
				if ( campo_municipios.options[i].value ) {
					municipios.push( campo_municipios.options[i].value );
				}
			}
		}

		// captura outros destinat�rios
		if ( campo_outros ) {
			outros = campo_outros.value;
		}
		
		// captura Status dos Usu�rios
		statusUsuario = obStatusUsuario.value;
		
		var parametros = "";
		parametros += "?pessoas=" + escape( outros );
		
		parametros += "&statusUsuario=" + statusUsuario;
		if ( orgao ) {
			parametros += "&orgao=" + orgao;
			if ( uo ) {
				for( i=0; uo[i]; i++ ){
					parametros += "&unidadeorcamentaria[]=" + uo[i];
				}
			}
			if ( ug ) {
				parametros += "&unidadegestora=" + ug;
			}
		}
		if ( perfis ) {
			for( i=0; perfis[i]; i++ ){
				parametros += "&perfil[]=" + perfis[i];
			}
		}
		
		if( arIDEB ){
			for( i=0; arIDEB[i]; i++ ){
				parametros += "&ideb[]=" + arIDEB[i];
			}
		}
		
		var arMunicipio = "";
		if ( municipios ) {
			for( i=0; municipios[i]; i++ ){
				arMunicipio += "'" +municipios[i] + "', ";
			}
		}
				
		<?		
		if($exibefiltromunicipios) {
		?>
			if(document.getElementById("filtromunsim").checked) {
			parametros += '&filtromun=sim';
			} else {
				parametros += '&filtromun=nao';
			}
		<?}?>			
			
		return new Ajax.Request(window.location.href,{
									method: 'post',
									parameters: {
                    					req: 'exibirDestinatarios',
                    					parametros: parametros,
                    					arMunicipio: arMunicipio
                					},
									onComplete: function(res){
										window.open( "/geral/destinatarios.php"+ parametros, "Destinat�rios", "resizable=no,scrollbars=yes,status=no,width=640,height=480");
									}
								});			
		
	}
	function verificaBotaoFile( obj ){
		jQuery(obj).after('<input type="file" name="'+obj.name+'" onchange="verificaBotaoFile(this);" />');
	}
	
</script>
<!-- FORMUL�RIO -->
<script language="javascript" type="text/javascript">
	function submeter_formulario(){
		var formulario = document.getElementById( 'formulario' );
		formulario.formulario.value = 2;
		var unidades_orcamentarias = formulario.unidadeorcamentaria;
		if ( unidades_orcamentarias ) {
			if( unidades_orcamentarias.options[0].value == "" ) {
				
			}
			selectAllOptions( unidades_orcamentarias );
		}

		var municipiosUsuario = formulario.municipiosUsuario;
		if ( municipiosUsuario ) {
			selectAllOptions( municipiosUsuario );
		}

		selectAllOptions( document.getElementById( 'ideb' ) );
		formulario.submit();
	}
</script>
<?php echo $simec->title($titulo_modulo, ''); ?>
<div class="row wrapper border-bottom white-bg page-heading">
	<div class="wrapper wrapper-content animated fadeIn">
		<div class="row">
			<form action="/geral/envia_email_selecao.php" id="formulario" method="post" name="formulario" enctype="multipart/form-data" class="form form-horizontal">
				<input type="hidden" name="formulario" value="1"/>
				<div class="ibox">
					<div class="ibox-title">
						<h3>Formulario de email</h3>
					</div>
					<div class="ibox-content">


                                <?php
                                /*$sql = "SELECT ee.entid as codigo,
                                           case when coalesce(ee.entorgcod, '' ) != ''
                                              then ee.entorgcod || ' - ' || ee.entnome
                                           else ee.entnome
                                           end as descricao
                                        FROM
                                           entidade.entidade ee
                                        WHERE funid in (31,32) ORDER BY ee.entnome";*/
                                $sql = "SELECT orgcod as codigo,
												   orgdsc as descricao
												FROM
												   public.orgao
												WHERE organo = '".$_SESSION['exercicio']."' ORDER BY orgdsc";


                                $arrOrgaos = array('1' => '- Selecione -');
                                $teste =  array_push($arrOrgaos,$arrOrgaos1['2']= 'Todos');
                                $arrOrgaosQuery = $db->carregar( $sql );

                                if(!empty($arrOrgaosQuery)){
                                    foreach($arrOrgaosQuery as $key) {
                                         $arrOrgaos[$key['codigo']]= $key['descricao'];
                                    }
                                }
                                echo $simec->select("orgao", '�rg�o', null, $arrOrgaos, array('maxlength' => '255', null));
                                ?>

						<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<colgroup>
								<col style="width: 25%;">
								<col style="width: 75%;">
							</colgroup>
							<tbody>

								<? if( $_SESSION["sisid"] == 56){ //verifica se o sistema e o academico
                                    $sql = "SELECT
												  orgid as codigo,
												  orgdesc as descricao
												FROM
												  academico.orgao
												ORDER BY orgdesc";

                                    echo $simec->select("tipoEnsino", 'Tipo de Ensino', null, $sql, array('maxlength' => '255', null));
                                    ?>
								

								<?} ?>
								
								<? if( $_SESSION["sisid"] == 57){ //verifica se o sistema e o academico

                                    $sql = "SELECT
													iniid as codigo,
													ininome as descricao
												FROM emenda.iniciativa i
												WHERE i.inistatus = 'A'";

                                    echo $simec->select("ptiid", 'Iniciativa', null, $sql, array('maxlength' => '255', null));
                                    $checkProvisorio = array('true' => 'Sim', 'false' => 'N�o');
                                    echo $simec->radio('inisomente', 'Somente iniciativa', null,  $checkProvisorio);

                                    ?>


								<? } ?>
								
								<?php if( $_REQUEST["orgao"] > 2 ): ?>
								<tr>
									<td align='right' class="SubTituloDireita">Unidade Or�ament�ria:</td>
									<td>
										<?php
										$sql =
											"SELECT 
												unidade.unicod AS codigo, 
												unidade.unidsc AS descricao
											 FROM 
											 	public.unidade AS unidade
											 INNER JOIN 
											 	seguranca.usuario AS usuario ON usuario.unicod = unidade.unicod
											 WHERE 
											 	usuario.entid = '{$_REQUEST["orgao"]}' AND unitpocod = 'U'
											 GROUP BY 
											 	codigo, descricao
											 ORDER BY 
											 	unidsc";
										combo_popup(
											'unidadeorcamentaria', 
											$sql, 
											'Selecione o(s) Grupo(s) de Unidade(s)', 
											'400x400', 
											0, 
											array(), 
											'', 
											'S', 
											true,
											false,
											10,
											500 ,
											'',
											''
										);
										?>
									</td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita">Unidade Gestora:</td>
									<td>
										<?php
										$sql =
											"SELECT 
												ungcod AS codigo, 
												ungdsc AS descricao 
											 FROM 
											 	public.unidadegestora
											 WHERE 
											 	unitpocod = 'U'
					 						 ORDER BY 
												ungdsc";
										$arrUnidadesGestoras = $db->carregar( $sql );
										?>
										<select class="CampoEstilo"  id="unidadegestora" name="unidadegestora" style="width: 500px;">
											<option value="0" <?= ( $_REQUEST["unidadegestora"] == 0 ? 'selected' : '') ?> >Todos</option>
											<?php foreach( $arrUnidadesGestoras as $arrUnidadeGestora ): ?>
												<?php if ( $_REQUEST["unidadegestora"] !=  $arrUnidadeGestora[ 'codigo' ] ): ?>
													<option value="<?= $arrUnidadeGestora[ 'codigo' ] ?>" >
														<?= $arrUnidadeGestora[ 'descricao' ] ?>
													</option>
												<?php else: ?>
													<option value="<?= $arrUnidadeGestora[ 'codigo' ] ?>" >
														<?= $arrUnidadeGestora[ 'descricao' ] ?>
													</option>
												<?php endif; ?>
											<?php endforeach; ?>
										</select>
									</td>
								</tr>
								<?php endif; ?>			
						        <?php
										$sql =
											"select pflcod as codigo, pfldsc as descricao 
											from seguranca.perfil
											where sisid = '{$_SESSION["sisid"]}' and pflstatus = 'A'
											order by pfldsc";
					

                                        echo $simec->select("perfil[]", 'Selecione os Perfis', null, $sql, array('maxlength' => '255', null));
										?>

                                <div class="form-group ">
                                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Munic�pios dos usu�rios: </label>
                                    <div class="col-sm-9 col-md-9 col-lg-9 ">
                                        <select multiple="multiple" size="7" name="municipiosUsuario[]" id="municipiosUsuario" ondblclick="abrepopupMunicipioUsuario();" class="CampoEstilo" style="width:400px;" >
                                            <option value="">Duplo clique para selecionar da lista</option>
                                        </select>

                                        </div>
                                    </div>

								</tr>
								<?php if( $_SESSION["sisid"] == 23 || $_SESSION["sisid"] == 99 || $_SESSION["sisid"] == 100 || $_SESSION["sisid"] == 231 ){ //Se par, Piso Salarial ou Proinfancia Manuten��o. ?>


                                            <?php
                                            $sql = "select suscod as codigo, susdsc as descricao from seguranca.statususuario";
                                            echo $simec->select("statusUsuario", 'Status do Usu�rio', null, $sql, array('maxlength' => '255', null));
                                            ?>

								<?php } ?>
								<!--				
								<tr id="trfiltrouf">
									<td align='right' class="SubTituloDireita">UF:</td>
									<td>
										<?php
											$sqlComboUF = " select estuf as codigo, estdescricao as descricao
															from territorios.estado
															order by estdescricao";
											echo combo_popup( "estuf", $sqlComboUF, "UF", "400x400", 0, array(), "", "S", false, false, 5, 300 );
										?>
									</td>
								</tr>			
								-->
								<?
								// Exibe o filtro por municipios
								if($exibefiltromunicipios) {
								?>
								<tr>
									<td align='right' class="SubTituloDireita">Status do Usu�rio:</td>
									<td>
										<select class="CampoEstilo" id="statusUsuario" name="statusUsuario">
											<?php 
											
											$sql = "select suscod, susdsc from seguranca.statususuario";	
											$coStatus = $db->carregar( $sql );
										
											foreach( $coStatus as $arStatus ){ 
												
												$selected = $arStatus["suscod"] == $_REQUEST["statusUsuario"] ? 'selected="selected" ' : "" ?>
											
												<option <?= $selected; ?> value="<?php echo $arStatus["suscod"] ?>"><?php echo $arStatus["susdsc"] ?></option>
											<?php } ?>
											<option value="0">Todos</option>
										</select>
									</td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita">Deseja filtrar o perfil por munic�pios ?</td>
									<td><input type="radio" name="filtromun" id="filtromunsim" value="sim" onclick="if(this.checked){document.getElementById('trfiltromunicipio').style.display = '';}else{document.getElementById('trfiltromunicipio').style.display = 'none';}"> Sim&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type="radio" name="filtromun" id="filtromunnao" value="nao" onclick="if(this.checked){document.getElementById('trfiltromunicipio').style.display = 'none';}else{document.getElementById('trfiltromunicipio').style.display = '';}" checked> N�o</td>
								</tr>
								<tr id="trfiltromunicipio" style="display:none">
									<td align='right' class="SubTituloDireita">Munic�pios:</td>
									<td>
									<select multiple="multiple" size="5" name="municipios[]" 
					        		id="municipios"  
					        		ondblclick="abrepopupMunicipio();"  
					        		class="CampoEstilo" style="width:400px;" >
									<option value="">Duplo clique para selecionar da lista</option>
							        </select>
									</td>
								</tr>
								<tr>
									<td align='right' class="SubTituloDireita">Classifica��o IDEB</td>
									<td>
										<?php
										$agrupador = new Agrupador( "formulario" );
										
										$sql = sprintf( "select tpmid, tpmdsc from territorios.tipomunicipio where gtmid = 2 and tpmstatus = 'A'" );
										$tipos = $db->carregar( $sql );
										
										$origem = array();
										$destino = array();
										if ( $tipos ) {
											foreach ( $tipos as $tipo ) {
												if ( in_array( $tipo['tpmid'], (array) $_REQUEST['ideb'] ) ) {
													$destino[] = array(
														'codigo'    => $tipo['tpmid'],
														'descricao' => $tipo['tpmdsc']
													);
												} else {
													$origem[] = array(
														'codigo'    => $tipo['tpmid'],
														'descricao' => $tipo['tpmdsc']
													);
												}
											}
										}
										
										$agrupador->setOrigem( "ideb_", null, $origem );
										$agrupador->setDestino( "ideb", null, $destino );
										$agrupador->exibir();
										?>
									</td>
								</tr>			
								<? } ?>	

                                        <?php
                                        $sql = "select carid as codigo, cardsc as descricao from public.cargo where carid <> 9";
                                            echo $simec->select("cargo", 'Fun��o/Cargo:', null, $sql, array('maxlength' => '255', null));
                                            ?>


                                <?php

                                echo $simec->textarea("pessoas", 'Outros Destinat�rios:', null, $sql, array('maxlength' => '255', null));

                                ?>

								<? if($db->testa_superuser()) : ?>
                                    <?php echo $simec->textarea("sql", 'SQL<br> <font color=red>Consulta SQL contendo coluna "nome" e "email"</font>', $_REQUEST["sql"], null, array('maxlength' => '255', null));?>
								<? endif; ?>


                                <?php echo $simec->input("assunto", 'Assunto', $_REQUEST["assunto"], null, array('maxlength' => '255', null));?>
                                <div class="form-group ">
                                    <label for="assunto" class="col-sm-3 col-md-3 col-lg-3 control-label">Anexo: </label>
                                    <div class="col-sm-9 col-md-9 col-lg-9 ">
                                        <input type="file" name="anexo[]" onchange="verificaBotaoFile(this);" />

                                    </div>
                                </div>
                                <?php echo $simec->textarea("mensagem", 'Mensagem', $_REQUEST["mensagem"],  array('mce_editable' => 'true', 'style' =>'height: 150px'),null);?>

                                <div class="form-group ">
                                    <label for="assunto" class="col-sm-3 col-md-3 col-lg-3 control-label">Remetente: </label>
                                    <div class="col-sm-9 col-md-9 col-lg-9 ">
                                        <fieldset>
											<legend>Dados do Remetente</legend>
											<label for="stNomeRemetente" style="width: 50px;">Nome: </label><input style="margin-bottom: 5px" class="normal" type="text" name="stNomeRemetente" id="stNomeRemetente" /><br />
											<label for="stEmailRemetente" style="width: 50px;">E-Mail: </label><input class="normal" type="text" name="stEmailRemetente" id="stEmailRemetente" /><br /><br />
											* No caso do n�o preenchimento dos campos acima, o destinat�rio ser� o usu�rio logado no sistema. 
										</fieldset>
									</div>
                                </div>
                                <div class="form-group ">
                                    <label for="assunto" class="col-sm-3 col-md-3 col-lg-3 control-label"> </label>
                                    <div class="col-sm-9 col-md-9 col-lg-9 ">
										<input type='button' class="btn btn-success salvar botao" name='EnviarEmail' value='Enviar' onclick="submeter_formulario();"/>
										<!-- span style="cursor: pointer; color:#FF0000;" onclick="exibir_destinatarios();">Clique aqui para verificar a lista de destin�tarios.</span -->
								</div>
                                    </div>
							</tbody>
						</table>
					</div>
				</div>
			</form>
		</div>
	</div>
</div>