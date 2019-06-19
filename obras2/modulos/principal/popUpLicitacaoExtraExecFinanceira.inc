<?php

header('content-type: text/html; charset=utf-8');

require APPRAIZ . "www/includes/webservice/pj.php";

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

if (empty($obrid) && empty($empid)) {
    die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
}

$acaoVisualizar = false; // Variável que indica se a tela é exibida para novo cadastro ou apenas para visualização.

if (!isset($obrid)) {
    echo "<script>alert('Faltam parâmetros para acessar esta tela.');</script>";
    exit;
}

$edicao 	= FALSE;
$visualizacao = FALSE;

if (($_REQUEST['edit'] == 'true' || $_REQUEST['visualizar'] == 'true') && (! empty($_REQUEST['lieid'])) && ($_REQUEST['lieid'] != '')) {
	
	if ($_REQUEST['edit'] == 'true') {
		$edicao = true;
	} else if ($_REQUEST['visualizar'] == 'true') {
		$visualizacao = true;
	}
	
	$lieid = $_REQUEST['lieid'];
	
	$sqlEdicao = "
		SELECT 
			lie.lieid,
			lie.molid,	
			lie.lienumero,
			to_char(lie.liedataedital,'DD/MM/YYYY') as liedataedital,
			ale1.arqid as arqidedital,
			liedescricaoedital,
			to_char(lie.liedatalicitacao,'DD/MM/YYYY') as liedatalicitacao,
			lie.liemeiopublicacaolicitacao,
			ale2.arqid as arqidhomologacao,
			liedescricaolicitacao
			
		FROM 
			obras2.licitacaoextra lie
		INNER JOIN obras2.arquivolicitacaoextra ale1 ON ale1.lieid = lie.lieid AND ale1.aletipo = 'E'
		INNER JOIN obras2.arquivolicitacaoextra ale2 ON ale2.lieid = lie.lieid AND ale2.aletipo = 'H'
		WHERE 
			lie.lieid = {$lieid}
	";
	
	$dadosEdicao = $db->pegaLinha($sqlEdicao);
	
	$dadosEdicao = ( is_array($dadosEdicao) ) ? $dadosEdicao : array();
	
	if ( count($dadosEdicao) < 1 ) {
		
		echo "	<script>
					alert(\"Erro ao recuperar dados de edição\");
					window.close();
				</script>";
		die();
	}
}


/* Salva o formulário de inclusão */
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "salvar") {
	
	$molid 						= ( $_REQUEST['molid']                      ) ?  $_REQUEST['molid']                       								: 'NULL';
	$lienumero 					= ( $_REQUEST['lienumero']         			) ?  $_REQUEST['lienumero']               									: 'NULL';
	$liedataedital 				= ( $_REQUEST['liedataedital']              ) ?  "'{$_REQUEST['liedataedital']}'"              							: 'NULL';
	$liedescricaoedital 		= ( $_REQUEST['liedescricaoedital']         ) ?  "'{$_REQUEST['liedescricaoedital']}'"          						: 'NULL';
	$liedatalicitacao 			= ( $_REQUEST['liedatalicitacao']           ) ?  "'". formata_data_sql($_REQUEST['liedatalicitacao']) ."'" 				: 'NULL';
	$liemeiopublicacaolicitacao = ( $_REQUEST['liemeiopublicacaolicitacao'] ) ?  "'". $_REQUEST['liemeiopublicacaolicitacao'] ."'"  					: 'NULL';
	$liedescricaolicitacao 		= ( $_REQUEST['liedescricaolicitacao']      ) ?  "'{$_REQUEST['liedescricaolicitacao']}'"      							: 'NULL'; 
	$obrid 						= $_SESSION['obras2']['obrid'];
	
	if ($molid != 1 && $molid != 2 && $molid != 3 && $molid != 4) {
		$lienumero = "NULL";
	}
	
	// Recupera a obra
    
    $usucpf 					=	($_REQUEST["usucpf"]) ? "'{$_REQUEST["usucpf"]}'": "NULL" ;
    
    $sqlInsert = "INSERT INTO 
					obras2.licitacaoextra
				( molid, lienumero, liedataedital, liedescricaoedital, liedatalicitacao, liestatus, obrid, usucpfinclusao, liemeiopublicacaolicitacao, liedescricaolicitacao)
				VALUES
    			( {$molid}, {$lienumero}, {$liedataedital}, {$liedescricaoedital}, $liedatalicitacao,  'A', {$obrid}, {$usucpf}, {$liemeiopublicacaolicitacao}, {$liedescricaolicitacao} )
    			RETURNING lieid 
    		";
    $lieid = $db->pegaUm($sqlInsert);
	
    // CAso tenha sido inserido corretamente
    if( $lieid )
    {
    	// Insere o arquivo do contrato original
    	if ( ($_FILES['arquivo_edital']['error'] == 0) && ($_FILES['arquivo_homologacao']['error'] == 0) )
    	{
    		// Valida o tipo para ver se é pdf
    		$arrMimeType = array("application/pdf", "application/x-pdf", "application/acrobat", "applications/vnd.pdf", "text/pdf", "text/x-pdf", "application/save", "application/force-download", "application/download", "application/x-download");
    		if( (in_array($_FILES['arquivo_edital']['type'], $arrMimeType)) && (in_array($_FILES['arquivo_homologacao']['type'], $arrMimeType)) ) 
    		{
    			// Incluo a classe de arquivos
    			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    			
				// Verifica a existencia dos arquivos de aditivo
    			if( (($_FILES['arquivo_edital']['name'][0] != '')) && (($_FILES['arquivo_homologacao']['name'][0] != '')) )
    			{
    				// Carrega a variável $_FILES no $arquivos
    				$arquivos = $_FILES;
					
    				$arquivos =(is_array($arquivos)) ? $arquivos : Array();
    				foreach ( $arquivos as $key => $value ) {
						
    					$maxUploadSize = 10485760;
    					if ( $maxUploadSize < $value['size']) {
    						echo '<script type="text/javascript">
								alert("O tamanho máximo para upload de arquivos é de 10Mb.");
								document.location.href = document.location.href;
						 	</script>';
    						die('');
    					}
    				}
    				
					foreach ( $arquivos as $key => $value ) {
						
						if ($key == "arquivo_edital") {
							$aletipo = "'E'";
						} elseif($key == "arquivo_homologacao") {
							$aletipo = "'H'";
						}
						
    					$files = array(
    							'arquivo' => array(
    								'name' => $value['name'],
    								'type' => $value['type'],
    								'tmp_name' =>$value['tmp_name'],
    								'error' => $value['error'],
    								'size' => $value['size']
    							)
    					);
    					
    					$_FILES = $files;
    					// Verifica se é um arquivo do tipo PDF
    						
    					if (in_array($_FILES['arquivo']['type'], $arrMimeType) && ($aletipo != '')) {
	    					
    						$campos = array(
    								"lieid" => $lieid,
    								"aletipo" => $aletipo
							);
    						// Trata nome do arquivo aditivo
    						$nomeArquivo = str_replace('.pdf', '', $_FILES['arquivo']['name']);
    						$nomeArquivo = ($nomeArquivo == '') ? 'arquivo' : $nomeArquivo;
    						// Faz a inclusão dos arquivos dos aditivos
    						$file = new FilesSimec('arquivolicitacaoextra', $campos, 'obras2');
    						$file->setUpload($nomeArquivo, 'arquivo',  true, 'aleid');
    						$aleid = $file->getCampoRetorno();
    					}
    					else
    					{
    						echo "<script>
			                    alert('Falha ao cadastrar! \\nSomente arquivo no formato PDF.');
				                    window.location.href = window.location.href;
				                  </script>";
	    						exit;
    					}
    				}
    					
    				if($db->commit())
    				{
    					echo "<script>
			                  alert('Licitação salva com sucesso');
			                  window.opener.location.reload();
			                </script>";
		    					exit;
    				}
    			}
    			
    			if ($db->commit()) {
    				echo "<script>
			                alert('Licitação salva com sucesso');
			                 window.opener.location.reload();
			                </script>";
    				exit;
    			} else {
	    			echo "<script>
	                    alert('Falha ao cadastrar! \\nSomente arquivo no formato PDF.');
	                    window.location.href = window.location.href;
	                  </script>";
	    			exit;
	    		}
    		}
    		else
    		{
    			echo "<script>
	                    alert('Falha ao cadastrar! \\nSomente arquivo no formato PDF.');
	                    window.location.href = window.location.href;
	                  </script>";
    			exit;
    		}
    	}
    	else
    	{
    		echo "<script>
	                    alert('Falha ao cadastrar!');
	                    window.location.href = window.location.href;
	                  </script>";
    		exit;
    	}
    	
    }
    else
    {
    	echo "<script>
	                    alert('Falha ao cadastrar!');
	                    window.location.href = window.location.href;
	                  </script>";
    	exit;
    }
}

/* Salva o formulário de edição */
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "editar")
{
	// Incluo a classe de arquivos
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$lieid = $_REQUEST['lieid']; 
	if ( $lieid ) {
		
		$molid 						= ( $_REQUEST['molid']                      ) ?  $_REQUEST['molid']                       								: 'NULL';
		$lienumero 					= ( $_REQUEST['lienumero']         			) ?  $_REQUEST['lienumero']               									: 'NULL';
		$liedataedital 				= ( $_REQUEST['liedataedital']              ) ?  "'{$_REQUEST['liedataedital']}'"              							: 'NULL';
		$liedescricaoedital 		= ( $_REQUEST['liedescricaoedital']         ) ?  "'{$_REQUEST['liedescricaoedital']}'"          						: 'NULL';
		$liedatalicitacao 			= ( $_REQUEST['liedatalicitacao']           ) ?  "'". formata_data_sql($_REQUEST['liedatalicitacao']) ."'" 				: 'NULL';
		$liemeiopublicacaolicitacao = ( $_REQUEST['liemeiopublicacaolicitacao'] ) ?  "'". $_REQUEST['liemeiopublicacaolicitacao'] ."'"  					: 'NULL';
		$liedescricaolicitacao 		= ( $_REQUEST['liedescricaolicitacao']      ) ?  "'{$_REQUEST['liedescricaolicitacao']}'"      							: 'NULL';
		$obrid 						= $_SESSION['obras2']['obrid'];
		$usucpf 					=	(trim($_REQUEST["usucpf"]) != '') 		  ? "'{$_REQUEST["usucpf"]}'": "NULL" ;
		
		if ($molid != 1 && $molid != 2 && $molid != 3 && $molid != 4) {
			$lienumero = "NULL";
		}
		
		$sqlUpdate = "
			UPDATE
				obras2.licitacaoextra
			SET
				molid						= $molid,
				lienumero					= $lienumero,
				liedataedital				= $liedataedital,
				liedescricaoedital			= $liedescricaoedital,
				liedatalicitacao			= $liedatalicitacao,
				obrid						= $obrid,
				liemeiopublicacaolicitacao	= $liemeiopublicacaolicitacao,
				liedescricaolicitacao		= $liedescricaolicitacao,
				usucpfalteracao 			= $usucpf,
				liedataalteracao 			= 'now()'
			WHERE 
				lieid = $lieid
		";
		
		if ( $db->executar($sqlUpdate) ) {
			
			// Com arquivo
			$arqidOldEdital			= $_REQUEST['arqideditalOld_edital'];
			$arqidOldHomologacao	= $_REQUEST['arqideditalOld_homologacao'];
			
			if ($arqidOldEdital != '' || $arqidOldHomologacao != '') {
				
				$arquivos = $_FILES;
				$arquivos =(is_array($arquivos)) ? $arquivos : Array();
				
				foreach ( $arquivos as $key => $value ) {
					$maxUploadSize = 10485760;
					if ( $maxUploadSize < $value['size']) {
						echo '<script type="text/javascript">
									alert("O tamanho máximo para upload de arquivos é de 10Mb.");
									document.location.href = document.location.href;
							 	</script>';
						die('');
					}
				}
				foreach ( $arquivos as $key => $value ) {
						
					if ($key == "arquivo_edital") {
							$aletipo = "'E'";
					} elseif($key == "arquivo_homologacao") {
								$aletipo = "'H'";
					}
							
	    			$files = array(
	    							'arquivo' => array(
	    								'name' => $value['name'],
	    								'type' => $value['type'],
	    								'tmp_name' =>$value['tmp_name'],
	    								'error' => $value['error'],
	    								'size' => $value['size']
	    							)
	    				);
	    					
					$_FILES = $files;
					// Verifica se é um arquivo do tipo PDF
					if ($files['arquivo']['size'] > 0) {
	    				$arrMimeType = array("application/pdf", "application/x-pdf", "application/acrobat", "applications/vnd.pdf", "text/pdf", "text/x-pdf", "application/save", "application/force-download", "application/download", "application/x-download");
		    			if (in_array($_FILES['arquivo']['type'], $arrMimeType) && ($aletipo != '')) {
			    					
		    				$nomeArquivo = str_replace('.pdf', '', $_FILES['arquivo']['name']);
		    				$nomeArquivo = ($nomeArquivo == '') ? 'arquivo' : $nomeArquivo;
		    				// Faz a inclusão dos arquivos dos aditivos
		    				$file = new FilesSimec('arquivo', null, 'obras2');
		    				$file->setPasta('obras2');
		    				$file->setUpload($nomeArquivo, 'arquivo',  false);
		    				$arqid = $file->getIdArquivo();
		    						
		    				if ($key == 'arquivo_edital') {
		    					$tipo = "'E'";
		    				} else if ($key == 'arquivo_homologacao') {
		    					$tipo = "'H'";	
		    				} else {
		    					$tipo = "";
		    				}
		    						
		    				if(($arqid) && ($tipo != "" ) && ($lieid != ""))
		    				{
		    					$sqlUpdateArqid = "
			    					UPDATE 
			    						obras2.arquivolicitacaoextra
			    					SET
			    						arqid = $arqid
			    					WHERE
			    						aletipo = $tipo
			    					AND
			    						lieid = $lieid";	
							}
		    				$db->executar($sqlUpdateArqid);
		    						
						}
		    			else
		    			{
		    				echo "	<script>
										alert('Falha ao cadastrar! \\nSomente arquivo no formato PDF.');
						            	window.location.href = window.location.href;
									</script>";
							exit;
						}
	    			}
				}
			}
			
			if($db->commit()) {
				echo "<script>
			                    alert('Licitação salva com sucesso');
			                    window.opener.location.reload();
			                  </script>";
				exit;
			}
		} else {
				echo "<script>
			                    alert('Erro ao salvar Licitação');
			                    window.opener.location.reload();
								window.close();
			                  </script>";
				exit;	
		}
	}
}


if($_REQUEST['download'] == 'S'){
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
	$arquivo = $file->getDownloadArquivo($arqid);
	echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
	exit;

}

function recuperaDadosArquivo($arqid){
	
	if (! $arqid) {
		echo "	<script>
						alert(\"Erro ao recuperar dados de edição\");
						window.close();
					</script>";
		die();
	}
	
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$files = new FilesSimec();

	return $files->getDadosArquivo($arqid);
}

?>

<body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="js/par.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<style type="text/css">

    tbody > tr:nth-child(even) {
        background-color: #f2f2f2;
    }

    td {
        vertical-align: middle;
        padding: 5px;
    }

    #tbAcompanhamento td {
        border: 1px solid #000;
        text-align: center;
        font-size: 13px;
    }

    .tbAcompanhamentoTitulo {
        font-weight: bold;
        text-align: center;
        border: 1px solid #000;
    }

    .acaoTbAcompanhamento {
        color: #0000FF;
        text-decoration: underline;
        cursor: pointer;
    }

    .detalheVisualizarPagamento {
        font-size: 14px;
    }

</style>

<form name=formulario id=formulario method=post enctype="multipart/form-data">
	<input type="hidden" name="requisicao" id="requisicao" value="salvar">
	<input type="hidden" id=tem_anexos  name=tem_anexos value="<?=$temAditivos ?>">
	<input type="hidden" name=edicao id=edicao value="<?=$edicao ?>">
	<input type="hidden" name=edicao_arquivo_edital id=edicao_arquivo_edital value="<?=$edicao ?>">
	<input type="hidden" name=edicao_arquivo_homologacao id=edicao_arquivo_homologacao value="<?=$edicao ?>">
	<input type="hidden" name="lieid" id="lieid" value="<?php echo $lieid; ?>"/>
    <!-- FORMULÁRIO DE INCLUSÃO DE MEDIÇÕES -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTitulocentro" colspan="2"><?php
             	if ($visualizacao) {
            		echo "Visualizar Licitação";
             	} else {
            		echo ($edicao) ? "Editar Licitação" : "Incluir Licitação";
             	}
             ?></td>
        </tr>

        <tr>
            <td>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">
					<tr>
						<th colspan="2">Dados da Licitação</th>
					</tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                           Órgão:
                        </td>
                        <td>
                            Educação Básica 
                        </td>
                    </tr>
                    <tr>
						<td width="265" class="subtitulodireita">Modalidade de Licitação:</td>
						<td>
							<?php 
								if($visualizacao && $dadosEdicao['molid'] ) {
									echo $db->pegaUm("SELECT moldsc AS descricao FROM obras2.modalidadelicitacao WHERE molid = {$dadosEdicao['molid']}");
									echo "<input type='hidden' name='molid' id='molid' value='{$dadosEdicao['molid']}' >";
								} else { 
									$modalidade = new ModalidadeLicitacao();
									$db->monta_combo('molid', $modalidade->listaCombo(), 'S', "Selecione...", 'abreNumLicUasg', '', '', '', 'S', 'molid', false, $dadosEdicao['molid']);
								}
								
							?>   
						</td>  
					</tr>
					<tr id="tr_numlic" style="display: none;">
						<td class="subtitulodireita">Número da Licitação:</td> 
						<td>
							<?php
								
								if( $visualizacao ) {
									echo $dadosEdicao['lienumero'];
								} else {
									echo campo_texto( 'lienumero', 'N', 'S', '', 9, 9, '##########', '', 'left', '', 0, 'id="lienumero"','', $dadosEdicao['lienumero'] );
									echo '<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>';
								}
								
								
							?>
							
						</td>
					</tr>
					<tr>
						<th colspan="2">Publicação de Edital</th>
					</tr>
					<tr id="publicacao" >
						<td class="SubTituloDireita">Data:</td>
						<td>
						<? 
							if( $visualizacao ) {
								echo $dadosEdicao['liedataedital'];
							} else {
								echo campo_data2('liedataedital', 'N', 'S', '', '', '','', $dadosEdicao['liedataedital']);
								echo '<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>';
							} 
						?>
						</td>
					</tr>
					
					<tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Anexo:
                        </td>
                        <td>
                        
                            <?php
                            
                            if ($visualizacao) {
                            	if ($dadosEdicao['arqidedital']) {
                            		$arquivo = recuperaDadosArquivo($dadosEdicao['arqidedital']);
                            		?>
                            		<div class="divArquivoDownload_edital">
										<a class="link" onclick="downloadArquivo(<?=$dadosEdicao['arqidedital']; ?>)" alt='Efetuar download do anexo do Contrato' title='Efetuar download do anexo do Contrato'>
										<img src='/imagens/salvar.png' border='0'>
										<?= $arquivo['arqnome'] .'.'. $arquivo['arqextensao'] ?>
										</a><br>
									</div>
									<?php 
								}
                            } else if ($edicao) {
                            	if ($dadosEdicao['arqidedital']) {
                            		$arquivo = recuperaDadosArquivo($dadosEdicao['arqidedital']);
                            ?>
                            <div class="divArquivoDownload_edital">
                                <a class="link" onclick="excluirArquivoEdital('<?=$dadosEdicao['arqidedital']?>');">
                                    <img src="/imagens/excluir.gif" title="Excluir arquivo <?=$arquivo['arqnome']?>">
                                </a>
                                <a class="link" onclick="downloadArquivo(<?=$dadosEdicao['arqidedital']; ?>)" alt='Efetuar download do anexo do Contrato' title='Efetuar download do anexo do Contrato'>
                                    <img src='/imagens/salvar.png' border='0'>
                                    <?= $arquivo['arqnome'] .'.'. $arquivo['arqextensao'] ?>
                                </a><br>
                            </div>
                           <?php 
								}
                            ?>
                            <div class="divArquivoUpload_edital" style="display: none">
                                <input type="hidden" name="arqideditalOld_edital" id="arqideditalOld_edital" value=""/>
                                <input type="file" name="arquivo_edital" id="arquivo_edital"/>
                                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                <font face="Verdana" size="1" color="red">Somente arquivo no formato PDF.</font>
                            </div>
                            <?php } else {?>
                                 <div class="divArquivoUpload_edital">
                                    <input type="hidden" name="arqidedital" id="arqidedital" value=""/>
                                    <input type="file" name="arquivo_edital" id="arquivo_edital"/>
                                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                    <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                </div>
                            <?php }  ?>
                                
                        </td>
                    </tr>
                    
                    <tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
						<td><? 
							if ($visualizacao) {
								echo $dadosEdicao['liedescricaoedital'];
							} else {
								echo campo_textarea( 'liedescricaoedital', 'N', 'S', '', 50, 2, 250, '', 0, '', false, NULL, $dadosEdicao['liedescricaoedital'] );
							}
						?></td>
					</tr>

                   	<tr>
						<th colspan="2">Homologação da Licitação</th>
					</tr>
					<tr id="publicacao" >
						<td class="SubTituloDireita">Data:</td>
						<td>
						<? 
							if ($visualizacao) {
								echo $dadosEdicao['liedatalicitacao'];
							} else {
								echo campo_data2('liedatalicitacao', 'N', 'S', '', '', '','', $dadosEdicao['liedatalicitacao']);
								echo '<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>'; 
							} ?>
						
						</td>
					</tr>
					<tr id="publicacao" >
						<td class="SubTituloDireita">Meio de Publicação:</td>
						<td>
							<? 
								if ($visualizacao) {
									echo $dadosEdicao['liemeiopublicacaolicitacao'];
								} else {
									echo campo_texto( 'liemeiopublicacaolicitacao', 'S', $habil, '', 50, 60, '',  '', 'left', '', 0,'','', $dadosEdicao['liemeiopublicacaolicitacao']);
								}
							?>
						</td>
					</tr>
					<tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Anexo:
                        </td>
                        <td>
                        
                            <?php
                           	
                            if ($visualizacao) {
                            	if ($dadosEdicao['arqidhomologacao']) {
                            		$arquivo = recuperaDadosArquivo($dadosEdicao['arqidhomologacao']);
                            		?>
										<div class="divArquivoDownload_homologacao">

                                			<a class="link" onclick="downloadArquivo(<?=$dadosEdicao['arqidhomologacao']; ?>)" alt='Efetuar download do anexo do Contrato' title='Efetuar download do anexo do Contrato'>
                                    			<img src='/imagens/salvar.png' border='0'>
                                    		<?= $arquivo['arqnome'] .'.'. $arquivo['arqextensao'] ?>
                                			</a><br>
                            			</div>
                            									<?php 
                            								}
							} else if ($edicao) {
                            	if($dadosEdicao['arqidhomologacao'])
                            	{
                            		$arquivo = recuperaDadosArquivo($dadosEdicao['arqidhomologacao']);
                            			
                            
                            ?>
                            <div class="divArquivoDownload_homologacao">
                                <a class="link" onclick="excluirArquivoHomologacao('<?=$dadosEdicao['arqidhomologacao']?>');">
                                    <img src="/imagens/excluir.gif" title="Excluir arquivo <?=$arquivo['arqnome']?>">
                                </a>
                                <a class="link" onclick="downloadArquivo(<?=$dadosEdicao['arqidhomologacao']; ?>)" alt='Efetuar download do anexo do Contrato' title='Efetuar download do anexo do Contrato'>
                                    <img src='/imagens/salvar.png' border='0'>
                                    <?= $arquivo['arqnome'] .'.'. $arquivo['arqextensao'] ?>
                                </a><br>
                            </div>
                           <?php }		
                            
                            ?>
                            <div class="divArquivoUpload_homologacao" style="display: none">
                                <input type="hidden" name="arqideditalOld_homologacao" id="arqideditalOld_homologacao" value=""/>
                                <input type="file" name="arquivo_homologacao" id="arquivo_homologacao"/>
                                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                <font face="Verdana" size="1" color="red">Somente arquivo no formato PDF.</font>
                            </div>
                            <?php } else {?>
                                 <div class="divArquivoUpload_homologacao">
                                    <input type="hidden" name="arqidhomologacao" id="arqidhomologacao" value=""/>
                                    <input type="file" name="arquivo_homologacao" id="arquivo_homologacao"/>
                                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                    <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                </div>
                            <?php }  ?>
                                
                        </td>
                    </tr>
                    
                     <tr>
						<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
						<td><?
							if ($visualizacao) {
								echo $dadosEdicao['liedescricaolicitacao'];
							} else {
								echo campo_textarea( 'liedescricaolicitacao', 'N', 'S', '', 50, 2, 250 , '', 0, '', false, NULL, $dadosEdicao['liedescricaolicitacao'] );
							}
						?></td>
					</tr>
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE INCLUSÃO DE MEDIÇÕES. -->


    <br/>
               		<div id="lista_excluidas_edicao">
               			
               		</div>
   
        <div id="acompanhamento" style="display: none">

            <br/>

            <div style="text-align: center">
            
            	<center>
               	<div id="loading" style="text-align: center; display: none"><img src="../imagens/wait.gif" />&nbsp;Aguarde.</div>
               </center>
            </div>
            
        </div>
        <center>
        		<?php
        			if( ! $visualizacao ) {
        		?> 
        			<input type="button" id="botaoSalvar" value="Salvar" onclick="salvarDocumento();">
        		<?php
        			}
        		?> 
               	<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
         </center>
      
</form>


    <script type="text/javascript">

    	var acaoEditar = false;

        $(document).ready(function() 
        {

        	acaoEditar = $("#lieid").val() != "" ? true : false;
        	
            var molid = jQuery('#molid').val();
        	if( molid == 1 || molid == 2 || molid == 3 || molid == 4 ){
        		$("#tr_numlic").show();
        	}else{
        		$("#tr_numlic").hide();
        	}
            
			if( $("#cexid").val() != '' )
			{
				consultaCNPJFornecedor(true);
				if( $("#tem_anexos").val() == 1 )
				{
					$("#acompanhamento").show();
            		

            		var total = ( $("#total_aditivos").val() != '' ) ? $("#total_aditivos").val() : 0;
            		
            		atualizaObjeto(total);
				}
			}
        });

        function abreNumLicUasg( id ){

        	if( id == 1 || id == 2 || id == 3 || id == 4 ){
        		$("#tr_numlic").show();
        	}else{
        		$("#tr_numlic").hide();
        	}
        }
        
    </script>


<script type="text/javascript">

    var listaObjDocumentos = [];
    var idControleObjDocumento	= 0;
    
    var acaoVisualizar = $("#acaoVisualizar").val();

    /**
     * Função responsável por atualizar o objeto para montar os anexos
     * idControleObjDocumento++;
     */
	function atualizaObjeto(total) 
 	{
     	for (i = 0; i < total; i++) 
		{	
			objDocumento = {
				"id": i
			};
			listaObjDocumentos.push(objDocumento);
			
		}
 		
    }
	
    /* Se for a tela de visualização, não precisa recarregar a tela pai. */
    if (typeof(acaoVisualizar) === "undefined") {
        //window.onunload = refreshParent;
    }

    /**
     * Função responsável por atualizar a página com a lista de itens na tela de notas fiscais. Esta atualização é feita
     * logo após o cadastro de uma nota fiscal.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     */
    function refreshParent() {
        window.opener.location.reload();
    }
     
    function excluirArquivoEdital(arqid) {
         if(confirm('Você deseja realmente excluir este arquivo?')) { 
        	 $("#edicao_arquivo_edital").val(0);
             $("#arqideditalOld_edital").val(arqid);
             $(".divArquivoDownload_edital").hide("fast");
             $(".divArquivoUpload_edital").show("fast");
         }
     }

    function excluirArquivoHomologacao(arqid) {
        if(confirm('Você deseja realmente excluir este arquivo?')) {
       	 	$("#edicao_arquivo_homologacao").val(0);
            $("#arqideditalOld_homologacao").val(arqid);
            $(".divArquivoDownload_homologacao").hide("fast");
            $(".divArquivoUpload_homologacao").show("fast");
        }
    }


    /**
     * Função responsável por remover da tabela de acompanhamento determinado documento.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @param elemento
     * @param indexObjDocumento
     */
    function removerDocumentoTabelaAcompanhamento(elemento, indexObjDocumento) {

        $(elemento).closest("tr").remove();
        listaObjDocumentos = listaObjDocumentos.filter(function(e) {
            return e.id != indexObjDocumento ;
        });

        if (listaObjDocumentos.length <= 0) {
            $("#acompanhamento").hide();
        }

    }
     
    function removerAditivoEdicao(elemento, indexObjDocumento, cceid) 
    {

    	$("#lista_excluidas_edicao").append(
                "<input type='hidden' name='aditivos_excluidos[]' id='aditivos_excluidos[]' value='"+cceid+"'> "
		);
    	
		
        $(elemento).closest("tr").remove();
        listaObjDocumentos = listaObjDocumentos.filter(function(e) {
            return e.id != indexObjDocumento ;
        });

        if (listaObjDocumentos.length <= 0) {
            $("#acompanhamento").hide();
        }

    }
  
    /**
     * Função responsável por validar os campos do formulário para inclusão de pagamentos.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @returns {*}
     */
    function validarDadosFormulario(validaAditivo) {

    	var molid = $("#molid").val();
    	
        if ($("#molid").val().trim() == "") {
            return "Informe a Modalidade de Licitação!";
        }
        if ( molid == 1 || molid == 2 || molid == 3 || molid == 4 ) {
        	if ($("#lienumero").val().trim() == "") {
            	return "Informe o Número da Licitação!";
        	}
        }
        if ($("#liedataedital").val().trim() == "") {
            return "Informe a Data da Publicação de Edital!";
        }

        var arqiOldEdital = $("#arqideditalOld_edital").val();
        
        if ( arqiOldEdital != "" && acaoEditar ) {
        	if ($("#arquivo_edital").val() == "" && ($("#edicao_arquivo_edital").val() != 1)) {
                return "Insira o Anexo do Edital!";   
            }
        }
        else if ($("#arquivo_edital").val() == "" && ($("#edicao_arquivo_edital").val() != 1)) 
        {
			return "Insira o Anexo do Edital!";   
		}
        
        
        if ( $.trim($('[name="liedescricaoedital"]').val()) == '' ) {
			return 'Informe a Descrição do Edital!';
		}
        if ($("#liedatalicitacao").val() == "") {
            return "Informe a Data da Homologação da Licitação!";
        }
        
        if ( $.trim($('[name="liemeiopublicacaolicitacao"]').val()) == '' ) {
            return "Informe o Meio de Publicação da Homologação da Licitação!";
        }


		var arqiOldHomologacao = $("#arqideditalOld_homologacao").val();
        
        if ( arqiOldHomologacao != "" && acaoEditar ) {
        	if ($("#arquivo_homologacao").val() == "" && ($("#edicao_arquivo_homologacao").val() != 1)) {
                return "Insira o Anexo da Homologação da Licitação!";   
            }
        }
        else if ($("#arquivo_homologacao").val().trim() == "" && ($("#edicao_arquivo_homologacao").val() != 1)) {
            return "Insira o Anexo da Homologação da Licitação!";   
        }
		
        
        
        if ( $.trim($('[name="liedescricaolicitacao"]').val()) == '' ) {
			return 'Informe a Descrição da Homologação da Licitação!';
        }
        return true;
    }

    function habilitaAditivoValor() {
    	 
        $("#div_aditivo_valor").show();
        $("#div_botao_aditivo").hide();
    }
    /**
     * Função responsável por requisitar a gravação dos documentos da transação.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @returns {boolean}
     */
    function salvarDocumento() {

		if( $("#edicao").val() == '1' )
		{
			$("#requisicao").val("editar");
		}

        var retornoValidacao = validarDadosFormulario(false);

        if (retornoValidacao !== true) {
            alert(retornoValidacao);
            return false;
        }
        
        $('#formulario').submit();
		
        return false;

    }

    /**
     * Funçao responsavel por verificar se o CNPJ fornecido está no formato válido.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=12078
     */
    function validarCPNJ(cnpj) {

        var retorno = false;

        $.ajax({
            url: window.location.href,
            type: "post",
            async: false,
            data: {
                "requisicao": "validarCNPJ",
                "cexnumcnpj": cnpj
            },
            success: function (data) {
                retorno = (data == "true") || false;
            }
        });

        return retorno;
    }

 	function downloadArquivo( arqid )
	{
 		if (typeof(arqid) == "undefined") {
 			alert("Erro ao fazer download do arquivo");
			window.close();
 		}
 	 	
		window.location='?modulo=principal/popUpConstrutoraExtraExecFinanceira&acao=O&download=S&arqid=' + arqid

	}

 	function esvaziaDadosCnpj()
	{
		$("#uf_fornecedor").html('');
		$("#razao_construtora").html('');
		$("#cexrazsocialconstrutora").val('');
        return true;
	}
	
    /**
     * Função para pesquisa de dados de um fornecedor por CNPJ.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=12078
     * @param pesquisa(boolean), quando acesso for de visualição passa true para pega atributo text do campo cnpj
     * caso inclusão passa false para pegar atributo val do campo cnpj
     * @returns {boolean}
     */
    function consultaCNPJFornecedor(pesquisa) {

        $("#razao_construtora").text("");
        $("#cexrazsocialconstrutora").val("");
        $("#uf_fornecedor").text("");

        if(pesquisa)
		{
            var cnpj = $("#cexnumcnpj").val();
            cnpj = cnpj.trim();
        }
        else
		{
            var cnpj = $("#cexnumcnpj").val();
        }

        if (cnpj.trim() == "") {
            alert("É obrigatório informar o CNPJ do fornecedor.");
            $("#cexnumcnpj").focus();
            return false;
        }

        if (!validarCPNJ(cnpj)) {
            alert("O CNPJ fornecido é inválido.");
            $("#cexnumcnpj").val("");
            $("#cexnumcnpj").focus();
            return false;
        }

        divCarregando();
        setTimeout(function () {
            var pessoaJudirica = new dCNPJ();
            pessoaJudirica.buscarDados(cnpj);
            if (pessoaJudirica.dados.no_empresarial_rf == "") {
                alert("Não há informações sobre o CNPJ informado em nossa base de dados.");
                return false;
            }
            $("#cexrazsocialconstrutora").val(pessoaJudirica.dados.no_empresarial_rf);
            $("#razao_construtora").text(pessoaJudirica.dados.no_empresarial_rf);
            $("#uf_fornecedor").text(pessoaJudirica.dados.sg_uf);
        }, 0);
        divCarregado();

        return false;
    }


</script>

</body>

