<?php 
require APPRAIZ . '\obras2\includes\principal\popUpPagamentoExecFinanceira\funcoes.php';

//require APPRAIZ . "www/includes/webservice/pj.php";

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

$acaoVisualizar = false; // Variável que indica se a tela é exibida para novo cadastro ou apenas para visualização.

if (empty($obrid) && empty($empid)) {
    die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
}

if (!isset($obrid)) {
    echo "<script>alert('Faltam parâmetros para acessar esta tela.');</script>";
    exit;
}

$edicao 		= FALSE;
$temDocumentos 	= FALSE;
$empresa = NULL;
if( (isset($_REQUEST['pgtid'])) && ( ! empty($_REQUEST['pgtid'])) && ($_REQUEST['pgtid'] != '') )
{
	$edicao = true;
	$pgtid = (int) $_REQUEST['pgtid'];

	$dadosEdicao = $db->pegaLinha(getSqlPagamentoTransacao($pgtid));
	
	$dadosEdicao = (is_array($dadosEdicao)) ? $dadosEdicao : Array();
	
	if( count($dadosEdicao) > 0)
	{
		if(( $dadosEdicao["crtid"] != '') && ( ! empty( $dadosEdicao["crtid"])))
		{
			$empresa = 'ori_' . $dadosEdicao["crtid"];
		}
		else if(( $dadosEdicao["cceid"] != '') && ( ! empty( $dadosEdicao["cceid"])))
		{
			$empresa = 'cex_' . $dadosEdicao["cceid"];
		}
		
		if($empresa == NULL)
		{
			
			echo "<script>
                    alert('Erro na recuperação de dados da edição.');
                    window.opener.location.reload();
					window.close();
                  </script>";
			exit;
		}

		$arrDocumentos = $db->carregar(getDocumentoTransacao($pgtid));
		$arrDocumentos = (is_array($arrDocumentos)) ? $arrDocumentos : Array();
		
		$temDocumentos = (count($arrDocumentos) > 0) ? TRUE : FALSE;
		
		if($temDocumentos)
		{
			$totalDocumentos = count($arrDocumentos);
			$idControleObjDocumento = 0;
		}
	}
	else
	{
		echo "<script>
                    alert('Erro na recuperação de dados da edição.');
                    window.opener.location.reload();
					window.close();
                  </script>"; 
		exit;
	}
}



/* Salva o formulário de inclusão */
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "salvar") 
{
	// Recupera a obra
    $obrid = $_SESSION['obras2']['obrid'];
    
    $pgtdtpagamento =	( trim($_REQUEST["pgtdtpagamento"]) != '' )? "'". formata_data_sql($_REQUEST['pgtdtpagamento']) . "'" : "NULL";
    $pgtnumtransacao 		=	( trim($_REQUEST["pgtnumtransacao"]) != '' ) ? "'{$_REQUEST["pgtnumtransacao"]}'" : "NULL" ;
    $tptid 		=	( trim($_REQUEST["tptid"]) != '' ) ? "'{$_REQUEST["tptid"]}'" : "NULL" ;
    $pgtvalortransacao 		=	( trim($_REQUEST["pgtvalortransacao"]) != '' ) ? "'{$_REQUEST["pgtvalortransacao"]}'" : "NULL" ;
    $pgtvalortransacao 			=	str_replace('.', '', $pgtvalortransacao);
    $pgtvalortransacao 			=	str_replace(',', '.', $pgtvalortransacao);
    $empresacontratada 		=	( trim($_REQUEST["empresacontratada"]) != '' ) ? "'{$_REQUEST["empresacontratada"]}'" : "NULL" ;
    
    if( ! ( verificaValoresTotais( $_REQUEST ))  )
    {
    	echo "<script>
		                    alert('Falha ao cadastrar! O valor da Transação deve ser totalmente distribuido nos documentos adicionados.');
		                    window.location.href = window.location.href;
		                  </script>";
    	exit;
    }
    
	if( strpos($empresacontratada, 'cex_') !== false)
	{
		//
		$campoEmpresa 	= "cexid";
		$idContrato		= str_replace('cex_', '', $empresacontratada);
		$labelCrt		= 'cceid';
		
		$idEmpresa		= $db->pegaUm("SELECT cexid FROM obras2.contratoconstrutoraextra WHERE  cceid = {$idContrato}");
		
	}
	else if( strpos($empresacontratada, 'ori_') !== false)
	{
		//
		$campoEmpresa 	= "entid";
		$idContrato		= str_replace('ori_', '', $empresacontratada);
		$labelCrt		= 'crtid';
		
		$idEmpresa		= $db->pegaUm("SELECT entidempresa FROM obras2.contrato WHERE  crtid = {$idContrato}");
		
	}
	
	if($idEmpresa && $obrid)
	{
		if ($_FILES['arquivo']['error'] == 0)
		{
			// Valida o tipo para ver se é pdf
			$arrMimeType = array("application/pdf", "application/x-pdf", "application/acrobat", "applications/vnd.pdf", "text/pdf", "text/x-pdf", "application/save", "application/force-download", "application/download", "application/x-download");
			if(in_array($_FILES['arquivo']['type'], $arrMimeType))
			{
				// Incluo a classe de arquivos
				include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				// Carrego os campos do contrato original
				$campos = array(
						"pgtdtpagamento" 	=> $pgtdtpagamento,
						$campoEmpresa 		=> $idEmpresa,
						"obrid"				=> $obrid, 
						$labelCrt			=> $idContrato,
						"pgtnumtransacao" 	=> $pgtnumtransacao,
						"tptid" 			=> "$tptid",
						"pgtvalortransacao" => $pgtvalortransacao,
						"pgtdtinclusao"		=> "'now()'",
						"pgtusucpfinclusao"	=> "'{$_SESSION['usucpf']}'"
				);
				 
		
				// Nome do arquivo que será inserido, tratamento
				$nomeArquivo = str_replace('.pdf', '', $_FILES['arquivo']['name']);
				$nomeArquivo = ($nomeArquivo == '') ? 'arquivo' : $nomeArquivo;
				 
				// Instancia um FileSimec parar poder inserir o primeiro arquivo do contrato original
				$file = new FilesSimec("pagamentotransacao", $campos, "obras2");
				$file->setUpload( $nomeArquivo, 'arquivo', true, 'pgtid');
				
				$pgtid = $file->getCampoRetorno();
				
				// Caso o contrato original tenha sido inserido corretamente
				if( $pgtid )
				{
					$arrDocumentos = (is_array($_REQUEST["ntfid"])) ? $_REQUEST["ntfid"] : Array();
					 
					if(count($arrDocumentos) > 0)
					{
						foreach( $arrDocumentos as $key => $v )
						{
							//@todo
							//print_r($_REQUEST['contrato'][$key]); 
							
							$campo = $_REQUEST['contrato'][$key];
							
							if( strpos($campo, 'ori_') !== false)
							{	
								$campoEmpresaDoc 	= "crtid";
								$idContratoDoc		= str_replace('ori_', '', $campo);
							}
							if( strpos($campo,'cex_') !== false)
							{	
								//
								$campoEmpresaDoc 	= "cceid";
								$idContratoDoc		= str_replace('cex_', '', $campo);
							}
							 
							$tdtid 				=	( trim($_REQUEST["tdtid"][$key]) != '' ) ? "'{$_REQUEST["tdtid"][$key]}'" : "NULL" ;
							$ntfid 				=	( trim($_REQUEST["ntfid"][$key]) != '' ) ? "'{$_REQUEST["ntfid"][$key]}'" : "NULL" ;
							$dotvalorpagodoc 	=	( trim($_REQUEST["dotvalorpagodoc"][$key]) != '' ) ? "{$_REQUEST["dotvalorpagodoc"][$key]}" : "NULL" ;
							$dotvalorpagodoc 			=	str_replace('.', '', $dotvalorpagodoc);
							$dotvalorpagodoc 			=	str_replace(',', '.', $dotvalorpagodoc);
							$dotstatus			= 	"'A'";
							$dotusucpfinclusao	= 	"'{$_SESSION['usucpf']}'";
							$dotdtinclusao		=	"'now()'";
							
							$sql = "INSERT INTO
										obras2.documentotransacao
									( pgtid, {$campoEmpresaDoc} , ntfid, tdtid, dotvalorpagodoc, dotstatus, dotusucpfinclusao, dotdtinclusao )
									VALUES
									({$pgtid}, {$idContratoDoc},  {$ntfid}, {$tdtid}, {$dotvalorpagodoc}, {$dotstatus}, {$dotusucpfinclusao}, {$dotdtinclusao})
							";
							$db->executar($sql);
						}
					}
					
					if($db->commit())
					{
						echo "<script>
				                    alert('Pagamento salvo com sucesso');
				                    window.opener.location.reload();
				                  </script>";
						exit;
					}
				}
				else
				{
					echo "<script>
		                    alert('Falha ao cadastrar! \\nFalha ao cadastrar contrato original.');
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
	                    alert('Falha ao cadastrar é necessário inserir um arquivo !');
	                    window.location.href = window.location.href;
	                  </script>";
			exit;
		}
	}
	else
	{
		echo "<script>
	                    alert('Falha ao cadastrar 2!');
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
	
	$pgtid = $_REQUEST["pgtid"]; 
	
	if( $pgtid )
	{
			
		$pgtdtpagamento =	( trim($_REQUEST["pgtdtpagamento"]) != '' )? "'". formata_data_sql($_REQUEST['pgtdtpagamento']) . "'" : "NULL";
	    $pgtnumtransacao 		=	( trim($_REQUEST["pgtnumtransacao"]) != '' ) ? "'{$_REQUEST["pgtnumtransacao"]}'" : "NULL" ;
	    $tptid 		=	( trim($_REQUEST["tptid"]) != '' ) ? "'{$_REQUEST["tptid"]}'" : "NULL" ;
	    $pgtvalortransacao 		=	( trim($_REQUEST["pgtvalortransacao"]) != '' ) ? "'{$_REQUEST["pgtvalortransacao"]}'" : "NULL" ;
	    $pgtvalortransacao 			=	str_replace('.', '', $pgtvalortransacao);
	    $pgtvalortransacao 			=	str_replace(',', '.', $pgtvalortransacao);
	    $empresacontratada 		=	( trim($_REQUEST["empresacontratada"]) != '' ) ? "'{$_REQUEST["empresacontratada"]}'" : "NULL" ;
		$usucpf 					=	(trim($_REQUEST["usucpf"]) != '') ? "'{$_REQUEST["usucpf"]}'": "NULL" ;
		
		
		if( ! ( verificaValoresTotais( $_REQUEST ))  )
		{
			echo "<script>
		                    alert('Falha ao cadastrar! O valor da Transação deve ser totalmente distribuido nos documentos adicionados.');
		                    window.location.href = window.location.href;
		                  </script>";
			exit;
		}
		
		if( strpos($empresacontratada, 'cex_') !== false)
		{
			//
			$campoEmpresa 	= "cexid";
			$idContrato		= str_replace('cex_', '', $empresacontratada);
			$labelCrt		= 'cceid';
			
			$outroContrato 	= 'crtid';
			$outraEmpresa 	= 'entid';
			
			$idEmpresa		= $db->pegaUm("SELECT cexid FROM obras2.contratoconstrutoraextra WHERE  cceid = {$idContrato}");
		
		}
		else if( strpos($empresacontratada, 'ori_') !== false)
		{
			//
			$campoEmpresa 	= "entid";
			$idContrato		= str_replace('ori_', '', $empresacontratada);
			$labelCrt		= 'crtid';
			
			$outroContrato 	= 'cceid';
			$outraEmpresa 	= 'cexid';
		
			$idEmpresa		= $db->pegaUm("SELECT entidempresa FROM obras2.contrato WHERE  crtid = {$idContrato}");
		
		}
		
		if( ! $idEmpresa )
		{
			echo "<script>
	                    alert('Falha ao cadastrar, faltam dados!');
	                    window.location.href = window.location.href;
	                  </script>";
			exit;
		}
		
		$arqidOld = $_REQUEST['arqidOld'];
		
		// Com arquivo
		if( $arqidOld != '' ) 
		{
			
			//Nome do arquivo que será inserido, tratamento
			$nomeArquivo = str_replace('.pdf', '', $_FILES['arquivo']['name']);
			$nomeArquivo = ($nomeArquivo == '') ? 'arquivo' : $nomeArquivo;
			
			$file = new FilesSimec();
			$file->setUpload($nomeArquivo, '', false);
			$arqid = $file->getIdArquivo();
			
			$sqlUpdate1 = "
				UPDATE obras2.pagamentotransacao SET 
					pgtdtpagamento 		= $pgtdtpagamento,
					$campoEmpresa 		= $idEmpresa,
					obrid			= $obrid, 
					$labelCrt		= $idContrato,
					$outroContrato 	= NULL,
					$outraEmpresa 	= NULL,
					pgtnumtransacao 	= $pgtnumtransacao,
					tptid 			= $tptid,
					pgtvalortransacao 	= $pgtvalortransacao,
					arqid			= $arqid
				WHERE 
					pgtid = {$pgtid}
			";
			
		}// Sem arquivo
		else
		{
			
			$sqlUpdate1 = "
				UPDATE obras2.pagamentotransacao SET 
					pgtdtpagamento 		= $pgtdtpagamento,
					$campoEmpresa 		= $idEmpresa,
					obrid			= $obrid, 
					$labelCrt		= $idContrato,
					$outroContrato 	= NULL,
					$outraEmpresa 	= NULL,
					pgtnumtransacao 	= $pgtnumtransacao,
					tptid 			= $tptid,
					pgtvalortransacao 	= $pgtvalortransacao
				WHERE 
					pgtid = {$pgtid}
			";
		}
		
		
		//
		// Executa atualizaçao do contrato principal e vai para os aditivos
		if( $db->executar($sqlUpdate1) )
		{
			$arrExluidos = $_REQUEST['documentos_excluidos'];
			$arrExluidos = (is_array($arrExluidos)) ? $arrExluidos : Array();
			
			foreach($arrExluidos as $kE => $vE )
			{
				$sqlInativacao = " UPDATE obras2.documentotransacao SET dotstatus = 'I' where dotid = {$vE} ";
				$db->executar($sqlInativacao);
			}

			$arrDocumentos = (is_array($_REQUEST["ntfid"])) ? $_REQUEST["ntfid"] : Array();
			
			if(count($arrDocumentos) > 0)
			{
				foreach( $arrDocumentos as $key => $v )
				{
						
					$campo = $_REQUEST['contrato'][$key];
						
					if( strpos($campo, 'ori_') !== false)
					{
						$campoEmpresaDoc 	= "crtid";
						$idContratoDoc		= str_replace('ori_', '', $campo);
					}
					if( strpos($campo,'cex_') !== false)
					{
						//
						$campoEmpresaDoc 	= "cceid";
						$idContratoDoc		= str_replace('cex_', '', $campo);
					}
		
					$tdtid 				=	( trim($_REQUEST["tdtid"][$key]) != '' ) ? "'{$_REQUEST["tdtid"][$key]}'" : "NULL" ;
					$ntfid 				=	( trim($_REQUEST["ntfid"][$key]) != '' ) ? "'{$_REQUEST["ntfid"][$key]}'" : "NULL" ;
					$dotvalorpagodoc 	=	( trim($_REQUEST["dotvalorpagodoc"][$key]) != '' ) ? "{$_REQUEST["dotvalorpagodoc"][$key]}" : "NULL" ;
					$dotvalorpagodoc 			=	str_replace('.', '', $dotvalorpagodoc);
					$dotvalorpagodoc 			=	str_replace(',', '.', $dotvalorpagodoc);
					$dotstatus			= 	"'A'";
					$dotusucpfinclusao	= 	"'{$_SESSION['usucpf']}'";
					$dotdtinclusao		=	"'now()'";
						
					$sql = "INSERT INTO
					obras2.documentotransacao
					( pgtid, {$campoEmpresaDoc} , ntfid, tdtid, dotvalorpagodoc, dotstatus, dotusucpfinclusao, dotdtinclusao )
					VALUES
					({$pgtid}, {$idContratoDoc},  {$ntfid}, {$tdtid}, {$dotvalorpagodoc}, {$dotstatus}, {$dotusucpfinclusao}, {$dotdtinclusao})
					";
					$db->executar($sql);
				}
			}
				
			if($db->commit())
			{
				echo "<script>
			                    alert('Pagamento salvo com sucesso');
			                    window.opener.location.reload();
			                  </script>";
				exit;
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
			// Incluíndo os possíveis aditivos, caso existam
		
	}
	print_r($_REQUEST);
	print_r($_FILES);
	die();
}

if($_REQUEST['download'] == 'S'){
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
	$arquivo = $file->getDownloadArquivo($arqid);
	echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
	exit;

}

if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "validarCNPJ") {
    ob_clean();
    $cexnumcnpj = trim($_POST["cexnumcnpj"]);
    echo json_encode(validarCnpj($cexnumcnpj));
    exit;
}

/* Requisição para solicitar o carregamento da combo de contratos.*/
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "carregaValorNota") 
{
	ob_clean();
	$ntfid = $_REQUEST['ntfid'];
	
	if($ntfid)
	{
		$sql = "
			SELECT 
				ntfvalornota 
			FROM 
				obras2.notafiscal
			WHERE
				ntfid = {$ntfid}
			";
	}
	
	$arrContratos = $sql ? $db->PegaLinha($sql) : false;
	if (!$arrContratos) 
	{
		$arrContratos = array();
	}
	
	echo json_encode($arrContratos);
	exit;
}

if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "carregarContratos") {
	ob_clean();
	
	$entidInfo = trim($_POST["entid"]);
	$arrEntidInfo = explode("_", $entidInfo);
	$origemEmpresa = $arrEntidInfo[0];
	
	$crtid = false;
	$cceid = false;

	if ($origemEmpresa == "ori") 
	{
		$crtid = $arrEntidInfo[1];
	}
	elseif ($origemEmpresa == "cex") 
	{
		$cceid = $arrEntidInfo[1];
	}

	echo json_encode(carregarContratos($obrid, $crtid, $cceid));
	exit;
}

if ( isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "carregarNotas") {
	ob_clean();
	
	$contrato = $_REQUEST["id"];
	$arrNotasComp = $_REQUEST["arrNotas"] ? $_REQUEST["arrNotas"] : array();
    $arrNotas = array ();

    foreach ($arrNotasComp as $nota){
        $temp = explode("-", $nota);
        if ($temp[1] == $contrato) {
            array_push($arrNotas, $temp[0]);
        }
    }

	if( strpos($contrato, 'cex_') !== false)
	{

		$idContrato		= str_replace('cex_', '', $contrato);
	

		$sql = "
				SELECT 
				DISTINCT 
					ntf.ntfid as codigo,
					ntf.ntfnumnota as descricao
				FROM 
					obras2.medicaocontrato mec
				INNER JOIN obras2.notamedicao ntm ON ntm.mecid = mec.mecid AND ntmstatus = 'A'
				INNER JOIN obras2.notafiscal  ntf ON ntf.ntfid = ntm.ntfid AND ntfstatus = 'A'

				WHERE  
					mec.cceid = {$idContrato}
				AND 
					ntfstatus = 'A'
				";
	
	}
	else if( strpos($contrato, 'ori_') !== false)
	{

		$idContrato		= str_replace('ori_', '', $contrato);
	
		$sql = "SELECT DISTINCT
					ntf.ntfid as codigo,
					ntf.ntfnumnota as descricao
				FROM
					obras2.medicaocontrato mec
				INNER JOIN obras2.notamedicao ntm ON ntm.mecid = mec.mecid AND ntmstatus = 'A'
				INNER JOIN obras2.notafiscal  ntf ON ntf.ntfid = ntm.ntfid AND ntfstatus = 'A'

				WHERE  
					mec.crtid = {$idContrato}
				AND 
					ntfstatus = 'A'";
	
	}
	
	$arrContratos = $sql ? $db->carregar($sql) : false;
	$arrContratos = $arrContratos ? $arrContratos : array();
	$arrRetorno = array();

	foreach ($arrContratos as $contrato){
        if(!in_array($contrato['codigo'], $arrNotas)){
           array_push($arrRetorno, $contrato);
        }
    }

//	ver($_REQUEST, d);
	
	echo json_encode($arrRetorno);
	die();
}

