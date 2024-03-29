<?php

ini_set("memory_limit", "1024M");

//$arCpfsAutorizados = array('72324414104', '89183215115','91112796134','54002192172', '00704963159', '73282081120', '12004839104', '03106929111', '02394817127','82910600106', '70353441104', '02487072130', '05958805681', '02780807660', '03700155689', '00764786105', '12013579349', '93706057115');
//
//if(!in_array($_SESSION['usucpf'], $arCpfsAutorizados)){
//	echo "<br/><br/><center><p><font color=\"red\">Voce não tem permissão para visualizar esta tela.</font></p></center>";
//	die;
//}

global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd;

$nome_bd     = 'simec_desenvolvimento';
$servidor_bd = '192.168.222.45';
$porta_bd    = '5433';
$usuario_db  = 'simec';
$senha_bd    = 'phpsimecao';

$db2 = new cls_banco();

function montaSqlDadosTabela($shema, $table)
{
	$sql = "SELECT
				column_name,
				is_nullable,
				data_type,
				character_maximum_length
			FROM
				information_schema.columns
			WHERE
				table_schema = '{$shema}'
			AND
				table_name = '{$table}'";

	return $sql;
}

if($_REQUEST['req'] == 'exportar'){

	$sisid = $_REQUEST['sisid'];

	// Monta INSERT seguranca.sistema
	$sql = "select * from seguranca.sistema where sisid = {$_REQUEST['sisid']}";
	$dataSistema = $db->pegaLinha($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tableSistema = $db->carregar(montaSqlDadosTabela('seguranca', 'sistema'));

	if($dataSistema){
		foreach($tableSistema as $table){
			if(isset($dataSistema[$table['column_name']])){
				$arCampos[]  = $table['column_name'];
				$arValores[] = empty($dataSistema[$table['column_name']]) ? 'null' : "'".simec_htmlentities($dataSistema[$table['column_name']])."'";
			}
		}
		if(is_array($arCampos) && is_array($arValores)){
			$insertSistema = "INSERT INTO seguranca.sistema<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
		}
	}

	// Monta INSERT seguranca.perfil
	$sql = "SELECT * FROM seguranca.perfil WHERE sisid = {$_REQUEST['sisid']}";
	$dataPerfil = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tablePerfil = $db->carregar(montaSqlDadosTabela('seguranca', 'perfil'));

	if($dataPerfil){
		$insertPerfil = '';
		foreach($dataPerfil as $data){
			unset($table);
			unset($arCampos);
			unset($arValores);
			foreach($tablePerfil as $table){
				foreach($data as $column_name => $column_value){
					if($table['column_name'] == $column_name && $table['column_name'] != 'pflcod'){
						$arCampos[]  = $table['column_name'];
						$arValores[] = empty($data[$table['column_name']]) ? 'null' : "'".simec_htmlentities($data[$table['column_name']])."'";
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertPerfil .= "INSERT INTO seguranca.perfil<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	// Monta INSERT abas
	$sql = "SELECT * FROM seguranca.aba WHERE sisid = {$_REQUEST['sisid']}";
	$dataAba = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tableAba = $db->carregar(montaSqlDadosTabela('seguranca', 'aba'));

	if($dataAba){
		$insertAba = '';
		foreach($dataAba as $data){
			unset($table);
			unset($arCampos);
			unset($arValores);
			foreach($tableAba as $table){
				foreach($data as $column_name => $column_value){
					if($table['column_name'] == $column_name && $table['column_name'] != 'abacod'){
						$arCampos[]  = $table['column_name'];
						$arValores[] = empty($data[$table['column_name']]) ? 'null' : "'".simec_htmlentities($data[$table['column_name']])."'";
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertAba .= "INSERT INTO seguranca.aba<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	// Monta INSERT menu
	$sql = "SELECT * FROM seguranca.menu WHERE sisid = {$_REQUEST['sisid']} ORDER BY mnucod";
	$dataMenu = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tableMenu = $db->carregar(montaSqlDadosTabela('seguranca', 'menu'));

	if ($dataMenu) {
		$insertMenu = $updateMenuPai = '';
		foreach($dataMenu as $data){
			unset($table, $arCampos, $arValores);
			foreach($tableMenu as $table){
				foreach($data as $column_name => $column_value){
					if($table['column_name'] == $column_name && $table['column_name'] != 'mnuid'){
						$arCampos[]  = $table['column_name'];
						if($table['column_name'] == 'mnuidpai' && $data[$table['column_name']]>0){
							$arValores[] = 'null';
							$sql = "select * from seguranca.menu where mnuid = {$data[$table['column_name']]}";
							$rsMenuPai = $db->pegaLinha($sql);
							$stMenuPai = "(SELECT mnuid FROM seguranca.menu WHERE sisid = {$_REQUEST['sisid']} and mnudsc = '{$rsMenuPai['mnudsc']}' ".(empty($rsMenuPai['mnulink']) ? "and (mnulink is null or trim(mnulink) = '')" : "and mnulink = '{$rsMenuPai['mnulink']}'" )." and mnucod = '{$rsMenuPai['mnucod']}')";
							$updateMenuPai .= "UPDATE seguranca.menu SET mnuidpai = {$stMenuPai} WHERE mnudsc = '{$data['mnudsc']}' ".(empty($data['mnulink']) ? "and (mnulink is null or trim(mnulink) = '')" : "and mnulink = '{$data['mnulink']}'" )." and mnucod = '{$data['mnucod']}' and sisid = {$_REQUEST['sisid']};<br/><br/>";
						}else if($table['column_name'] == 'abacod' && $data[$table['column_name']]>0){
							$sql = "select * from seguranca.aba where abacod = {$data[$table['column_name']]}";
							$rsAba = $db->pegaLinha($sql);
							$arValores[] = "(SELECT abacod FROM seguranca.aba WHERE sisid = {$_REQUEST['sisid']} and abadsc = '{$rsAba['abadsc']}')";
						}else{
							$arValores[] = empty($data[$table['column_name']]) ? 'null' : "'".simec_htmlentities($data[$table['column_name']])."'";
						}
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertMenu .= "INSERT INTO seguranca.menu<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	// Monta INSERT seguranca.aba_menu
	$sql = "SELECT abm.*, aba.abadsc, mnu.mnucod, mnu.mnudsc, mnu.mnulink FROM seguranca.aba_menu abm
			JOIN seguranca.aba aba ON aba.abacod = abm.abacod
			JOIN seguranca.menu mnu ON mnu.mnuid = abm.mnuid
			WHERE mnu.sisid = {$_REQUEST['sisid']} AND aba.sisid = {$_REQUEST['sisid']}";
	$dataAbaMenu = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tableAbaMenu = $db->carregar(montaSqlDadosTabela('seguranca', 'aba_menu'));

	if($dataAbaMenu){
		$insertAbaMenu = '';
		foreach($dataAbaMenu as $data){
			unset($table, $arCampos, $arValores);
			foreach($tableAbaMenu as $table){
				foreach($data as $column_name => $column_value){
					if($table['column_name'] == $column_name){
						$arCampos[]  = $table['column_name'];
						if($table['column_name'] == 'abacod'){
							$arValores[] = "(SELECT abacod FROM seguranca.aba WHERE sisid = {$_REQUEST['sisid']} AND abadsc = '{$data['abadsc']}')";
						}else if($table['column_name'] == 'mnuid'){
							$arValores[] = "(SELECT mnuid FROM seguranca.menu WHERE sisid = {$_REQUEST['sisid']} AND mnucod = '{$data['mnucod']}' AND mnudsc = '{$data['mnudsc']}' ".(empty($data['mnulink']) ? "and mnulink is null" : "and mnulink = '{$data['mnulink']}'" ).")";
                        } else {
                            $arValores[] = empty($column_value)?'null':$column_value;
                        }
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertAbaMenu .= "INSERT INTO seguranca.aba_menu<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	// Monta INSERT seguranca.perfilmenu
	$sql = "select pm.*, pf.pfldsc, mn.mnucod, mn.mnudsc, mn.mnulink from seguranca.perfilmenu pm
			join seguranca.perfil pf on pf.pflcod = pm.pflcod
			join seguranca.menu mn on mn.mnuid = pm.mnuid
			where pf.sisid = {$_REQUEST['sisid']} and mn.sisid = {$_REQUEST['sisid']}";
	$dataPerfilMenu = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tablePerfilMenu = $db->carregar(montaSqlDadosTabela('seguranca', 'perfilmenu'));

	if($dataPerfilMenu){
		$insertPerfilMenu = '';
		foreach($dataPerfilMenu as $data){
			unset($table);
			unset($arCampos);
			unset($arValores);
			foreach($tablePerfilMenu as $table){
				foreach($data as $column_name => $column_value){
					if($table['column_name'] == $column_name){
						$arCampos[]  = $table['column_name'];
						if($table['column_name'] == 'mnuid' && $data[$table['column_name']]>0){
							$arValores[] = "(select mnuid from seguranca.menu where mnucod = '{$data['mnucod']}' and mnudsc = '{$data['mnudsc']}' ".(empty($data['mnulink']) ? "and mnulink is null" : "and mnulink = '{$data['mnulink']}'" )." and sisid = {$_REQUEST['sisid']})";
						}else if($table['column_name'] == 'pflcod' && $data[$table['column_name']]>0){
							$arValores[] = "(select pflcod from seguranca.perfil where pfldsc = '{$data['pfldsc']}' and sisid = {$_REQUEST['sisid']})";
						}else{
							$arValores[] = empty($data[$table['column_name']]) ? 'null' : "'".simec_htmlentities($data[$table['column_name']])."'";
						}
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertPerfilMenu .= "INSERT INTO seguranca.perfilmenu<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	// Monta INSERT seguranca.perfilusuario
	$sql = "SELECT pu.*, pfldsc FROM seguranca.perfilusuario pu
			JOIN seguranca.perfil pf on pf.pflcod = pu.pflcod
			WHERE sisid = {$_REQUEST['sisid']} AND pfldsc = 'Super Usuário'";
	$dataPerfilUsuario = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tablePerfilUsuario = $db->carregar(montaSqlDadosTabela('seguranca', 'perfilusuario'));

	if($dataPerfilUsuario){
		$insertPerfilUsuario = '';
		foreach($dataPerfilUsuario as $data){
			unset($table);
			unset($arCampos);
			unset($arValores);
			foreach($tablePerfilUsuario as $table){
				foreach($data as $column_name => $column_value){
					if($table['column_name'] == $column_name){
						$arCampos[]  = $table['column_name'];
						if($table['column_name'] == 'pflcod' && $data[$table['column_name']]>0){
							$arValores[] = "(select pflcod from seguranca.perfil where pfldsc = '{$data['pfldsc']}' and sisid = {$_REQUEST['sisid']})";
						}else{
							$arValores[] = empty($data[$table['column_name']]) ? 'null' : "'".simec_htmlentities($data[$table['column_name']])."'";
						}
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertPerfilUsuario .= "INSERT INTO seguranca.perfilusuario<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	// Monta INSERT workflow.tipodocumento
	$sql = "SELECT * FROM workflow.tipodocumento WHERE sisid = {$_REQUEST['sisid']}";
	$dataWfTipodocumento = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tableWfTipodocumento = $db->carregar(montaSqlDadosTabela('workflow', 'tipodocumento'));

	if($dataWfTipodocumento){
		$insertWfTipodocumento = '';
		foreach($dataWfTipodocumento as $data){
			unset($table);
			unset($arCampos);
			unset($arValores);
			foreach($tableWfTipodocumento as $table){
				foreach($data as $column_name => $column_value){
					if($table['column_name'] == $column_name && $table['column_name'] != 'tpdid'){
						$arCampos[]  = $table['column_name'];
						$arValores[] = empty($data[$table['column_name']]) ? 'null' : "'".simec_htmlentities($data[$table['column_name']])."'";
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertWfTipodocumento .= "INSERT INTO workflow.tipodocumento<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	// Monta INSERT workflow.estadodocumento
	$sql = "SELECT est.*, tpddsc FROM workflow.estadodocumento est
			JOIN workflow.tipodocumento tpd ON tpd.tpdid = est.tpdid
			WHERE tpd.sisid = {$_REQUEST['sisid']}";
	$dataWfEstadoDocumento = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tableWfEstadoDocumento = $db->carregar(montaSqlDadosTabela('workflow', 'estadodocumento'));

	if($dataWfEstadoDocumento){
		$insertWfEstadoDocumento = '';
		foreach($dataWfEstadoDocumento as $data){
			unset($table);
			unset($arCampos);
			unset($arValores);
			foreach($tableWfEstadoDocumento as $table){
				foreach($data as $column_name => $column_value){
					if($table['column_name'] == $column_name && $table['column_name'] != 'esdid'){
						$arCampos[]  = $table['column_name'];
						if($table['column_name'] == 'tpdid'){
							$arValores[] = "(SELECT tpdid FROM workflow.tipodocumento WHERE sisid = {$_REQUEST['sisid']} AND tpddsc = '{$data['tpddsc']}')";
						}else{
							$arValores[] = empty($data[$table['column_name']]) ? 'null' : "'".simec_htmlentities($data[$table['column_name']])."'";
						}
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertWfEstadoDocumento .= "INSERT INTO workflow.estadodocumento<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	// Monta INSERT workflow.acaoestadodoc
	$sql = "SELECT aed.*, eso.esddsc as esddscorigem, esd.esddsc as esddscdestino, tpd.tpddsc FROM workflow.acaoestadodoc aed
			JOIN workflow.estadodocumento eso ON eso.esdid = aed.esdidorigem
			JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdiddestino
			JOIN workflow.tipodocumento tpd ON tpd.tpdid = eso.tpdid AND tpd.tpdid = esd.tpdid
			WHERE tpd.sisid = {$_REQUEST['sisid']}";
	$dataWfAcaoEstadoDocumento = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tableWfAcaoEstadoDocumento = $db->carregar(montaSqlDadosTabela('workflow', 'acaoestadodoc'));

	if($dataWfAcaoEstadoDocumento){
		$insertWfAcaoEstadoDocumento = '';
		foreach($dataWfAcaoEstadoDocumento as $data){
			unset($table);
			unset($arCampos);
			unset($arValores);
			foreach($tableWfAcaoEstadoDocumento as $table){
				foreach($data as $column_name => $column_value){

					if($table['column_name'] == $column_name && $table['column_name'] != 'aedid'){
						$arCampos[]  = $table['column_name'];
						if($table['column_name'] == 'esdidorigem'){
							$arValores[] = "(SELECT esdid FROM workflow.estadodocumento esd
											JOIN workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
											WHERE tpd.sisid = {$_REQUEST['sisid']} AND esd.esddsc = '{$data['esddscorigem']}')";
						}else if($table['column_name'] == 'esdiddestino'){
							$arValores[] = "(SELECT esdid FROM workflow.estadodocumento esd
											JOIN workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
											WHERE tpd.sisid = {$_REQUEST['sisid']} AND esd.esddsc = '{$data['esddscdestino']}')";
						}else{
							$arValores[] = empty($data[$table['column_name']]) ? 'null' : "'".simec_htmlentities($data[$table['column_name']])."'";
						}
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertWfAcaoEstadoDocumento .= "INSERT INTO workflow.acaoestadodoc<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	// Monta INSERT workflow.estadodocumentoperfil
	$sql = "SELECT edp.*, pfl.pfldsc, eso.esddsc as esddscorigem, esd.esddsc as esddscdestino, tpd.tpddsc,
			aeddscrealizar, aeddscrealizada FROM workflow.estadodocumentoperfil edp
			JOIN workflow.acaoestadodoc aed ON aed.aedid = edp.aedid
			JOIN workflow.estadodocumento eso ON eso.esdid = aed.esdidorigem
			JOIN workflow.estadodocumento esd ON esd.esdid = aed.esdiddestino
			JOIN seguranca.perfil pfl ON pfl.pflcod = edp.pflcod
			JOIN workflow.tipodocumento tpd ON tpd.tpdid = eso.tpdid AND tpd.tpdid = esd.tpdid
			WHERE tpd.sisid = {$_REQUEST['sisid']}";
	$dataWfEstadoDocumentoPerfil = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tableWfEstadoDocumentoPerfil = $db->carregar(montaSqlDadosTabela('workflow', 'estadodocumentoperfil'));

	if($dataWfEstadoDocumentoPerfil){
		$insertWfEstadoDocumentoPerfil = '';
		foreach($dataWfEstadoDocumentoPerfil as $data){
			unset($table);
			unset($arCampos);
			unset($arValores);
			foreach($tableWfEstadoDocumentoPerfil as $table){
				foreach($data as $column_name => $column_value){
					if($table['column_name'] == $column_name){
						$arCampos[]  = $table['column_name'];
						if($table['column_name'] == 'aedid'){
							$arValores[] = "(
											SELECT aedid FROM workflow.acaoestadodoc
											WHERE esdidorigem = (SELECT esdid FROM workflow.estadodocumento esd
																JOIN workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
																WHERE tpd.sisid = {$_REQUEST['sisid']} AND esd.esddsc = '{$data['esddscorigem']}')
											AND esdiddestino = (SELECT esdid FROM workflow.estadodocumento esd
																JOIN workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
																WHERE tpd.sisid = {$_REQUEST['sisid']} AND esd.esddsc = '{$data['esddscdestino']}')
											AND aeddscrealizar = '{$data['aeddscrealizar']}'
											AND aeddscrealizada = '{$data['aeddscrealizada']}'
											)";
						}else if($table['column_name'] == 'pflcod'){
							$arValores[] = "(SELECT pflcod FROM seguranca.perfil WHERE sisid = {$_REQUEST['sisid']} AND pfldsc = '{$data['pfldsc']}')";
						}
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertWfEstadoDocumentoPerfil .= "INSERT INTO workflow.estadodocumentoperfil<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	// Monta INSERT esquema.usuarioresposabilidade
	$sql = "SELECT ur.*, pfldsc FROM {$dataSistema['sisdiretorio']}.usuarioresponsabilidade ur
			JOIN seguranca.perfil pf ON pf.pflcod = ur.pflcod
			WHERE pf.sisid = {$_REQUEST['sisid']}";
	$dataUsuarioResponsabilidade = $db->carregar($sql);

	// Recupera campos do banco de destino, definido com a variavel $db2
	$tableUsuarioResponsabilidade = $db->carregar(montaSqlDadosTabela($dataSistema['sisdiretorio'], 'usuarioresponsabilidade'));

	if($dataUsuarioResponsabilidade){
		$insertUsuarioResponsabilidade = '';
		foreach($dataUsuarioResponsabilidade as $data){
			unset($table);
			unset($arCampos);
			unset($arValores);
			foreach($tableUsuarioResponsabilidade as $table){
				foreach($data as $column_name => $column_value){
					if($table['column_name'] == $column_name){
						$arCampos[]  = $table['column_name'];
						if($table['column_name'] == 'pflcod'){
							$arValores[] = "(SELECT pflcod FROM seguranca.perfil WHERE sisid = {$_REQUEST['sisid']} AND pfldsc = '{$data['pfldsc']}')";
						}else if($table['column_name'] == 'rpudata_inc'){
							$arValores[] = 'now()';
						}else{
							$arValores[] = empty($data[$table['column_name']]) ? 'null' : "'".simec_htmlentities($data[$table['column_name']])."'";
						}
					}
				}
			}
			if(is_array($arCampos) && is_array($arValores)){
				$insertUsuarioResponsabilidade .= "INSERT INTO {$dataSistema['sisdiretorio']}.usuarioresponsabilidade<br/>&nbsp;&nbsp;(".implode(',', $arCampos).")<br/>VALUES<br/>&nbsp;&nbsp;(".implode(',', $arValores).");<br/><br/>";
			}
		}
	}

	if($_REQUEST['exportar_arquivo'] == 'S'){
		header('Content-Type: application/txt');
		header('Content-Disposition: attachment; filename=script_exportacao_sistema'.date('YmdHis').'.sql');
		header('Pragma: no-cache');
	}

	// IMPRIME OS SCRIPTS
	echo "<pre>";
	echo "<BR/><BR/><BR/>--INSERE SISTEMA <BR/><BR/><BR/>";
	echo $insertSistema;
	echo "<BR/><BR/><BR/>--INSERE PERFIL<BR/><BR/><BR/>";
	echo $insertPerfil;
	echo "<BR/><BR/><BR/>--INSERE ABA<BR/><BR/><BR/>";
	echo $insertAba;
	echo "<BR/><BR/><BR/>--INSERE MENU<BR/><BR/><BR/>";
	echo $insertMenu;
	echo "<BR/><BR/><BR/>--ATUALIZAR MENU IDPAI<BR/><BR/><BR/>";
	echo $updateMenuPai;
	echo "<BR/><BR/><BR/>--INSERE ABA MENU<BR/><BR/><BR/>";
	echo $insertAbaMenu;
	echo "<BR/><BR/><BR/>--INSERE PERFIL MENU<BR/><BR/><BR/>";
	echo $insertPerfilMenu;
	echo "<BR/><BR/><BR/>--INSERE PERFIL USUÁRIO<BR/><BR/><BR/>";
	echo $insertPerfilUsuario;
	echo "<BR/><BR/><BR/>--INSERE WORKFLOW TIPO DOCUMENTO<BR/><BR/><BR/>";
	echo $insertWfTipodocumento;
	echo "<BR/><BR/><BR/>--INSERE WORKFLOW ESTADO DOCUMENTO<BR/><BR/><BR/>";
	echo $insertWfEstadoDocumento;
	echo "<BR/><BR/><BR/>--INSERE WORKFLOW AÇÃO ESTADO DOCUMENTO<BR/><BR/><BR/>";
	echo $insertWfAcaoEstadoDocumento;
	echo "<BR/><BR/><BR/>--INSERE WORKFLOW AÇÃO ESTADO DOCUMENTO PERFIL<BR/><BR/><BR/>";
	echo $insertWfEstadoDocumentoPerfil;
	echo "<BR/><BR/><BR/>--INSERE RESPONSABILIDADES<BR/><BR/><BR/>";
	echo $insertUsuarioResponsabilidade;
	echo "</pre>";

// 	ver($dataWfEstadoDocumentoPerfil, $arTpdid);
	die;
}

include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

$linha1 = "Exportar Sistema";
$linha2 = "Escolha um SIstema para Exportar";
monta_titulo($linha1, $linha2);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	$(function(){
		$('.exportarSistema').click(function(){
			if($('[name=sisid]').val() == ''){
				alert('Selecione um sistema primeiro!');
				$('[name=sisid]').focus();
				return false;
			}
			$('[name=req]').val('exportar');
			$('#formulario').submit();
		});
	});
</script>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" name="req" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloDireita">Escolha um sistema:</td>
			<td>
				<?php
					$sql = "SELECT
								sisid as codigo,
								sisid || ' - ' || sisdsc as descricao
							FROM seguranca.sistema
							ORDER BY sisid";

					unset($sisid);
					$db->monta_combo('sisid', $sql, 'S', 'Selecione...', '', '', '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Exportar para arquivo?</td>
			<td>
				<input type="radio" name="exportar_arquivo" value="S" />&nbsp;Sim&nbsp;
				<input type="radio" name="exportar_arquivo" value="N" checked="checked"/>&nbsp;Não
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita"></td>
			<td class="subtituloEsquerda">
				<input type="button" value="Enviar" class="exportarSistema" />
			</td>
		</tr>
	</table>
</form>