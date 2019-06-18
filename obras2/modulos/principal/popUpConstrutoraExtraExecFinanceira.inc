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

$edicao = FALSE;

if( (isset($_REQUEST['cexid'])) && ( ! empty($_REQUEST['cexid'])) && ($_REQUEST['cexid'] != '') )
{
	$edicao = true;
	$cexid = $_REQUEST['cexid'];
	
	$sqlEdicao = "
		SELECT 
			CASE WHEN cceid_pai IS NOT NULL THEN
				'aditivo'
			ELSE
				'original'
			END as tipo,
			cexnumcnpj,
			cexrazsocialconstrutora,
			cex.lieid,
			cce.ccenumero,
			to_char(cce.ccedataassinatura,'DD/MM/YYYY') as ccedataassinatura,
			cce.ccevalor,
			cce.arqid,
			cce.cceid
		FROM 
			obras2.construtoraextra cex 
		INNER JOIN obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
		
		WHERE 
			cex.cexid = {$cexid}
			AND cce.ccestatus = 'A'
	";
	
	$dadosEdicao = $db->carregar($sqlEdicao);
	
	$dadosEdicao = ( is_array($dadosEdicao) ) ? $dadosEdicao : array();
	
	$temAditivo		= false;
	$dadosContrato	= array();
	$dadosAditivos	= array();
	
	foreach($dadosEdicao as $k => $v )
	{
		if( $v['tipo'] == "original" )
		{
			$dadosContrato = $v;
		}
		else
		{
			$temAditivo = true;
			$dadosAditivos[] = $v;
		}
	}
	$temAditivos = (count($dadosAditivos) > 0) ? TRUE : FALSE;
	if($temAditivos){
		$totalAditivos = count($dadosAditivos);
		$idControleObjDocumento = 0;
	}
}


/* Salva o formulário de inclusão */
if (isset($_REQUEST["requisicao"]) && $_REQUEST["requisicao"] == "salvar") 
{
	// Recupera a obra
    $obrid = $_SESSION['obras2']['obrid'];

    
    // Tabela Extra
    $cexnumcnpj 				=	str_replace(array('.', '/', '-'), '', $_REQUEST["cexnumcnpj"]);
    $cexnumcnpj					=	trim($cexnumcnpj);
    $cexnumcnpj 				=	($cexnumcnpj) ? "'{$cexnumcnpj}'": "NULL" ;
    $cexrazsocialconstrutora 	=	($_REQUEST["cexrazsocialconstrutora"]) ? "'{$_REQUEST["cexrazsocialconstrutora"]}'": "NULL" ;
    $usucpf 					=	($_REQUEST["usucpf"]) ? "'{$_REQUEST["usucpf"]}'": "NULL" ;
    
    $sqlInsert = "INSERT INTO 
					obras2.construtoraextra 
				( obrid, cexrazsocialconstrutora, cexnumcnpj, cexusucpf, cexdtinclusao, cexstatus, lieid)
				VALUES
    			( {$obrid}, {$cexrazsocialconstrutora}, {$cexnumcnpj}, {$usucpf}, 'now()',  'A', {$_REQUEST['licitacao']})
    			RETURNING cexid 
    		";
    $cexid = $db->pegaUm($sqlInsert);
	
    // CAso tenha sido inserido corretamente
    if( $cexid )
    {
    	// Dados do contrato original
    	$ccenumero_original 		=	( trim($_REQUEST["ccenumero_original"]) != '' ) ? "'{$_REQUEST["ccenumero_original"]}'" : "NULL" ;
    	$ccedataassinatura_original =	( trim($_REQUEST["ccedataassinatura_original"]) != '' )? "'". formata_data_sql($_REQUEST['ccedataassinatura_original']) . "'" : "NULL";
    	$ccevalor_original 			=	( trim($_REQUEST["ccevalor_original"]) != '' ) ? $_REQUEST["ccevalor_original"] : "NULL";
    	$ccevalor_original 			=	str_replace('.', '', $ccevalor_original);
    	$ccevalor_original 			=	str_replace(',', '.', $ccevalor_original);
    	
    	// Insere o arquivo do contrato original
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
    					"ccenumero" => $ccenumero_original,
    					"ccedataassinatura" => $ccedataassinatura_original,
    					"ccevalor" => $ccevalor_original,
    					"ccestatus" => "'A'",
    					"cexid" => $cexid,
    					"usucpfcriacao" => "'{$_SESSION['usucpf']}'",
    					"ccedtcriacao" => "'now()'"
    			);
    			

    			// Nome do arquivo que será inserido, tratamento
    			$nomeArquivo = str_replace('.pdf', '', $_FILES['arquivo']['name']);
    			$nomeArquivo = ($nomeArquivo == '') ? 'arquivo' : $nomeArquivo; 
    			
    			// Instancia um FileSimec parar poder inserir o primeiro arquivo do contrato original
    			$file = new FilesSimec("contratoconstrutoraextra", $campos, "obras2");
    			$file->setUpload( $nomeArquivo, 'arquivo', true, 'cceid');
    			$cceidPai = $file->getCampoRetorno();
    			
    			// Caso o contrato original tenha sido inserido corretamente
    			if( $cceidPai )
    			{	
    				// Verifica a existencia dos arquivos de aditivo
    				if( ($_FILES['arquivo_aditivo']['name'][0] != '') && (is_array($_FILES['arquivo_aditivo']['name'])) )
    				{
    					// Carrega a variável $_FILES no $arquivos
    					$arquivos = $_FILES;
    					
						foreach( $arquivos['arquivo_aditivo']['name'] as $key => $value )
						{
							
    						$files = array(
    								'arquivo' => array(
    										'name' => $arquivos['arquivo_aditivo']['name'][$key],
    										'type' => $arquivos['arquivo_aditivo']['type'][$key],
    										'tmp_name' => $arquivos['arquivo_aditivo']['tmp_name'][$key],
    										'error' => $arquivos['arquivo_aditivo']['error'][$key],
    										'size' => $arquivos['arquivo_aditivo']['size'][$key],
    								)
    						);
    						$_FILES = $files;
    						// Verifica se é um arquivo do tipo PDF
    						
    						if(in_array($_FILES['arquivo']['type'], $arrMimeType))
	    					{
	    						// Carrega as variáveis para inserir o aditivo em sua tabela
	    						
	    						$ccenumero_aditivo 			=	(trim($_REQUEST["ccenumero"][$key]) != '') 				? "'{$_REQUEST["ccenumero"][$key]}'" 									: "NULL" ;
	    						$ccedataassinatura_aditivo	=	(trim($_REQUEST["ccedataassinatura"][$key]) != '' ) 	? "'". formata_data_sql($_REQUEST['ccedataassinatura'][$key]) . "'" 	: "NULL";
	    						$ccevalor_aditivo 			=	(trim($_REQUEST["valorAditivo"][$key]) != '' ) 			? $_REQUEST["valorAditivo"][$key]										: "NULL";
	    						//$ccevalor_aditivo 			=	str_replace('.', '', $ccevalor_aditivo);
	    						//$ccevalor_aditivo 			=	str_replace(',', '.', $ccevalor_aditivo);
	    						
	    						// Carrega valores para o insert
	    						$campos = array(
	    								"ccenumero" => $ccenumero_aditivo,
	    								"ccedataassinatura" => $ccedataassinatura_aditivo,
	    								"ccevalor" => $ccevalor_aditivo,
	    								"ccestatus" => "'A'",
	    								"cexid" => $cexid,
	    								"cceid_pai" => $cceidPai,
	    								"usucpfcriacao" => "'{$_SESSION['usucpf']}'",
	    								"ccedtcriacao" => "'now()'",
	    								"ttaid"		   => 2
								);
	    						// Trata nome do arquivo aditivo
	    						$nomeArquivo = str_replace('.pdf', '', $arquivos['arquivo_aditivo']['name'][$key]);
	    						$nomeArquivo = ($nomeArquivo == '') ? 'arquivo' : $nomeArquivo;
	    						// Faz a inclusão dos arquivos dos aditivos
	    						$file = new FilesSimec('contratoconstrutoraextra', $campos, 'obras2');
	    						$file->setUpload($nomeArquivo, 'arquivo',  true, 'cceid');
	    						$cceid = $file->getCampoRetorno();
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
			                    alert('Empresa salva com sucesso');
			                    window.opener.location.reload();
			                  </script>";
		    						exit;
    					}
    				}
    				if($db->commit())
    				{
    					echo "<script>
			                    alert('Empresa salva com sucesso');
			                    window.opener.location.reload();
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

	$cexid = $_REQUEST["cexid"]; 
	if( $cexid )
	{
		// Salva a construtora extra
		$cexnumcnpj 				=	str_replace(array('.', '/', '-'), '', $_REQUEST["cexnumcnpj"]);
		$cexnumcnpj 				=	($cexnumcnpj) ? "'{$cexnumcnpj}'": "NULL" ;
		$cexrazsocialconstrutora 	=	(trim($_REQUEST["cexrazsocialconstrutora"]) != '') ? "'{$_REQUEST["cexrazsocialconstrutora"]}'": "NULL" ;
		$usucpf 					=	(trim($_REQUEST["usucpf"]) != '') ? "'{$_REQUEST["usucpf"]}'": "NULL" ;
		
		$sqlUpdate = "
			UPDATE
				obras2.construtoraextra
			SET
				cexrazsocialconstrutora = $cexrazsocialconstrutora,
				cexnumcnpj = $cexnumcnpj,
				cexusucpf = $usucpf,
				lieid = {$_REQUEST['licitacao']}
			WHERE 
				cexid = $cexid
		";
		
		if( $db->executar($sqlUpdate) ) {

            // Atualiza contrato principal
            $ccenumero_original = (trim($_REQUEST["ccenumero_original"]) != '') ? "'{$_REQUEST["ccenumero_original"]}'" : "NULL";
            $ccedataassinatura_original = (trim($_REQUEST["ccedataassinatura_original"]) != '') ? "'" . formata_data_sql($_REQUEST['ccedataassinatura_original']) . "'" : "NULL";
            $ccevalor_original = (trim($_REQUEST["ccevalor_original"]) != '') ? $_REQUEST["ccevalor_original"] : "NULL";
            $ccevalor_original = str_replace('.', '', $ccevalor_original);
            $ccevalor_original = str_replace(',', '.', $ccevalor_original);
            $arqidOld = $_REQUEST['arqidOld'];

            $sqlConsulta = "SELECT cceid FROM obras2.contratoconstrutoraextra WHERE cexid = {$cexid}";

            if ($db->pegaUm($sqlConsulta)) {

                // Com arquivo
                if ($arqidOld != '') {

                    //Nome do arquivo que será inserido, tratamento
                    $nomeArquivo = str_replace('.pdf', '', $_FILES['arquivo']['name']);
                    $nomeArquivo = ($nomeArquivo == '') ? 'arquivo' : $nomeArquivo;

                    $file = new FilesSimec();
                    $file->setUpload($nomeArquivo, '', false);
                    $arqid = $file->getIdArquivo();

                    $sqlUpdate1 = "
					UPDATE
						obras2.contratoconstrutoraextra
					SET
						ccenumero  = $ccenumero_original,
						ccedataassinatura  = $ccedataassinatura_original,
						ccevalor  = $ccevalor_original,
						ccestatus  =  'A',
						arqid 		= $arqid
					WHERE
						cceid = ( select cceid from obras2.contratoconstrutoraextra where cexid = {$cexid} AND cceid_pai IS NULL  AND ccestatus = 'A' LIMIT 1 )	
				";

                } else {

                    $sqlUpdate1 = "
					UPDATE
						obras2.contratoconstrutoraextra
					SET
						ccenumero  = $ccenumero_original,
						ccedataassinatura  = $ccedataassinatura_original,
						ccevalor  = $ccevalor_original,
						ccestatus  =  'A'
					WHERE
						cceid = ( select cceid from obras2.contratoconstrutoraextra where cexid = {$cexid} AND cceid_pai IS NULL  AND ccestatus = 'A' LIMIT 1 )	
				";
                }
            } else {
                if ($_FILES['arquivo']['error'] == 0) {
                    // Valida o tipo para ver se é pdf
                    $arrMimeType = array(
                        "application/pdf",
                        "application/x-pdf",
                        "application/acrobat",
                        "applications/vnd.pdf",
                        "text/pdf",
                        "text/x-pdf",
                        "application/save",
                        "application/force-download",
                        "application/download",
                        "application/x-download"
                    );
                    if (in_array($_FILES['arquivo']['type'], $arrMimeType)) {
                        //Nome do arquivo que será inserido, tratamento
                        $nomeArquivo = str_replace('.pdf', '', $_FILES['arquivo']['name']);
                        $nomeArquivo = ($nomeArquivo == '') ? 'arquivo' : $nomeArquivo;

                        $file = new FilesSimec();
                        $file->setUpload($nomeArquivo, '', false);
                        $arqid1 = $file->getIdArquivo();

                        $sqlUpdate1 = "INSERT INTO obras2.contratoconstrutoraextra 
                            (ccenumero, ccedataassinatura, ccevalor, arqid, ccestatus, cexid, usucpfcriacao, ccedtcriacao) VALUES 
                            ({$ccenumero_original}, {$ccedataassinatura_original}, {$ccevalor_original}, {$arqid1}, 'A', {$cexid},'{$_SESSION['usucpf']}','now()')";

                    } else {
                        echo "<script>
				                    alert('Falha ao cadastrar! \\nSomente arquivo no formato PDF.');
				                    window.location.href = window.location.href;
				                  </script>";
                        exit;
                    }
                } else {
                    echo "<script>
				                    alert('Falha ao cadastrar! \\nNecessário anexar arquivo do contrato.');
				                    window.location.href = window.location.href;
				                  </script>";
                    exit;
                }
            }

			// Executa atualizaçao do contrato principal e vai para os aditivos
			if($db->executar($sqlUpdate1))
			{
				$arrExluidos = $_REQUEST['aditivos_excluidos'];
				$arrExluidos = (is_array($arrExluidos)) ? $arrExluidos : Array();
				
				foreach($arrExluidos as $kE => $vE )
				{
					$sqlInativacao = " UPDATE obras2.contratoconstrutoraextra SET ccestatus = 'I' where cceid = {$vE} ";
					$db->executar($sqlInativacao);
				}

				$cceidPai = $db->pegaUm("select cceid from obras2.contratoconstrutoraextra where cexid = {$cexid} AND cceid_pai IS NULL  AND ccestatus = 'A' LIMIT 1");
				 
				// Caso o contrato original tenha sido inserido corretamente
				if( $cceidPai )
				{
					// Verifica a existencia dos arquivos de aditivo
					if( ($_FILES['arquivo_aditivo']['name'][0] != '') && (is_array($_FILES['arquivo_aditivo']['name'])) )
					{
						// Carrega a variável $_FILES no $arquivos
						$arquivos = $_FILES;
							
						foreach( $arquivos['arquivo_aditivo']['name'] as $key => $value )
						{
								
							$files = array(
									'arquivo' => array(
											'name' => $arquivos['arquivo_aditivo']['name'][$key],
											'type' => $arquivos['arquivo_aditivo']['type'][$key],
											'tmp_name' => $arquivos['arquivo_aditivo']['tmp_name'][$key],
											'error' => $arquivos['arquivo_aditivo']['error'][$key],
											'size' => $arquivos['arquivo_aditivo']['size'][$key],
									)
							);
							$_FILES = $files;
							// Verifica se é um arquivo do tipo PDF
							$arrMimeType = array("application/pdf", "application/x-pdf", "application/acrobat", "applications/vnd.pdf", "text/pdf", "text/x-pdf", "application/save", "application/force-download", "application/download", "application/x-download");
							if(in_array($_FILES['arquivo']['type'], $arrMimeType))
							{
								// Carrega as variáveis para inserir o aditivo em sua tabela
									
								$ccenumero_aditivo 			=	(trim($_REQUEST["ccenumero"][$key]) != '') 				? "'{$_REQUEST["ccenumero"][$key]}'" 									: "NULL" ;
								$ccedataassinatura_aditivo	=	(trim($_REQUEST["ccedataassinatura"][$key]) != '') 		? "'". formata_data_sql($_REQUEST['ccedataassinatura'][$key]) . "'" 	: "NULL";
								$ccevalor_aditivo 			=	(trim($_REQUEST["valorAditivo"][$key]) != '' ) 			? $_REQUEST["valorAditivo"][$key]											: "NULL";
								//$ccevalor_aditivo 			=	str_replace('.', '', $ccevalor_aditivo);
								//$ccevalor_aditivo 			=	str_replace(',', '.', $ccevalor_aditivo);
									
								// Carrega valores para o insert
								$campos = array(
										"ccenumero" => $ccenumero_aditivo,
										"ccedataassinatura" => $ccedataassinatura_aditivo,
										"ccevalor" => $ccevalor_aditivo,
										"ccestatus" => "'A'",
										"cexid" => $cexid,
										"cceid_pai" => $cceidPai,
										"usucpfcriacao" => "'{$_SESSION['usucpf']}'",
										"ccedtcriacao" => "'now()'",
										"ttaid"		   => 2
								);
								// Trata nome do arquivo aditivo
								$nomeArquivo = str_replace('.pdf', '', $arquivos['arquivo_aditivo']['name'][$key]);
								$nomeArquivo = ($nomeArquivo == '') ? 'arquivo' : $nomeArquivo;
								// Faz a inclusão dos arquivos dos aditivos
								$file = new FilesSimec('contratoconstrutoraextra', $campos, 'obras2');
								$file->setUpload($nomeArquivo, 'arquivo',  true, 'cceid');
								$cceid = $file->getCampoRetorno();
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
			                    alert('Empresa salva com sucesso');
			                    window.opener.location.reload();
			                  </script>";
							exit;
						}
					}
					if($db->commit())
					{
						echo "<script>
			                    alert('Empresa salva com sucesso');
			                    window.opener.location.reload();
			                  </script>";
						exit;
					}
				}
			}
			// Incluíndo os possíveis aditivos, caso existam
		}
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

function recuperaDadosArquivo($arqid){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$files = new FilesSimec();

	return $files->getDadosArquivo($arqid);
}

function montaComboLicitacao($obrid, $selectedValue = null){
    global $db;

    $execucaoFinanceira = new ExecucaoFinanceira();
    $arrObrids = $execucaoFinanceira->retornaObrids($obrid);
    $strObrids = implode(",", $arrObrids);

    $sql = "SELECT
              l.lieid     AS codigo,
              CASE WHEN l.molid IN (5, 6, 7) THEN
                m.moldsc
              ELSE
                l.lienumero || ' - ' || m.moldsc
              END  AS descricao
            FROM obras2.licitacaoextra l
            INNER JOIN obras2.modalidadelicitacao m ON l.molid = m.molid
            WHERE l.liestatus = 'A' AND obrid IN ({$strObrids})";

    return $db->monta_combo("licitacao", $sql, "S", "Selecione...", '', '', '', '', 'S', 'licitacao', '',  $selectedValue);
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
	<input type="hidden" id=cexid  name=cexid value="<?=$cexid ?>">
	<input type="hidden" id=tem_anexos  name=tem_anexos value="<?=$temAditivos ?>">
	<input type="hidden" name=edicao_arquivo id=edicao_arquivo value="<?=$edicao ?>">
    <!-- FORMULÁRIO DE INCLUSÃO DE MEDIÇÕES -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTitulocentro" colspan="2">Incluir Empresa Contratada</td>
        </tr>

        <tr>
            <td>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Licitação:
                        </td>
                        <td>
                        <?php
                            montaComboLicitacao($obrid,$dadosContrato['lieid']);
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                           CNPJ:
                        </td>
                        <td>
                             <input type="text" style="text-align:left;" name="cexnumcnpj"
	                                   id="cexnumcnpj" size="15" value="<?php echo $dadosContrato['cexnumcnpj'];  ?>"
	                                   onKeyUp="this.value=mascaraglobal('##.###.###/####-##', this.value); esvaziaDadosCnpj();"
	                                   title="" class='obrigatorio normal'
	                            />
	                            <span style="cursor: pointer" onclick="consultaCNPJFornecedor()">
                                <img border='0' src='../imagens/icone_lupa.png' title='Clique para consultar.'
                                     align="absmiddle"/>
	                            </span>
	                            <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'/>
                        </td>
                    </tr>
                     <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                           Razão Social:
                        </td>
                        <td style="background-color: #DCDCDC">
                            <b><span name="razao_construtora" id="razao_construtora"></span></b>
                            <input name="cexrazsocialconstrutora" id="cexrazsocialconstrutora" type="hidden" />
                        </td>
                    </tr>
					<tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                          UF:
                        </td>
                         <td style="background-color: #DCDCDC">
                            <b><span name="uf_fornecedor" id="uf_fornecedor"></span></b>
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                          Número Contrato:
                        </td>
                        <td>
                          
                            <input type="text" style="text-align:left;" name="ccenumero_original"
                                   id="ccenumero_original" size="15" maxlength="10" value="<?php echo $dadosContrato['ccenumero'];  ?>"
                                   onKeyUp=""
                                   onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();"
                                   onmouseout="MouseOut(this);" onblur="MouseBlur(this);"
                                   title="" class='obrigatorio normal'
                            />
                            <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'/>
                        
                        </td>
                    </tr>
                     <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                          Data de Assinatura do Contrato:
                        </td>
                        <td>
                            <?= campo_data2('ccedataassinatura_original', 'N', 'S', '', '', '','', $dadosContrato['ccedataassinatura']); ?>
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                     <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                          Valor do Contrato:
                        </td>
                        <td>
                        		<?php 
                        		if($edicao)
                        		{
                        			$valor = number_format($dadosContrato['ccevalor'], 2, ",", ".");
                        		}
                        		else
                        		{
                        			$valor =  "0,00";
                        		}
                        		?>
                                <input
                                        type="text"
                                        style="text-align:left;"
                                        name="ccevalor_original"
                                        id="ccevalor_original"
                                        onmouseout="MouseOut(this);"
                                        onfocus="MouseClick(this);this.select();"
                                        onmouseover="MouseOver(this);"
                                        onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                                        value="<?php echo $valor;?>" <?php // @todo tratar depois?>
                                        maxlength="18"
                                        size="17"
                                        class="obrigatorio normal"
                                />
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Arquivo do Contrato
                        </td>
                        <td>
                        
                            <?php
                            if($edicao){
                            	if($dadosContrato['arqid'])
                            	{
                            		$arquivo = recuperaDadosArquivo($dadosContrato['arqid']);
                            			
                            
                            ?>
                            <div class="divArquivoDownload">
                                <a class="link" onclick="excluirArquivo('<?=$dadosContrato['arqid']?>');">
                                    <img src="/imagens/excluir.gif" title="Excluir arquivo <?=$arquivo['arqnome']?>">
                                </a>
                                <a class="link" onclick="downloadArquivo(<?=$arquivo['arqid']; ?>)" alt='Efetuar download do anexo do Contrato' title='Efetuar download do anexo do Contrato'>
                                    <img src='/imagens/salvar.png' border='0'>
                                    <?= $arquivo['arqnome'] .'.'. $arquivo['arqextensao'] ?>
                                </a><br>
                            </div>
                            <div class="divArquivoUpload" style="display: none">
                                <input type="hidden" name="arqidOld" id="arqidOld" value=""/>
                                <input type="file" name="arquivo" id="arquivo"/>
                                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                <font face="Verdana" size="1" color="red">Somente arquivo no formato PDF.</font>
                            </div>
                           <?php }		
                            
                            ?>
                                <div class="divArquivoUpload">
                                    <input type="hidden" name="arq_contrato_??" id="arq_contrato_??" value=""/>
                                    <input type="file" name="arquivo" id="arquivo"/>
                                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                    <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                </div>
                            <?php } else {?>
                                 <div class="divArquivoUpload">
                                    <input type="hidden" name="arq_contrato_??" id="arq_contrato_??" value=""/>
                                    <input type="file" name="arquivo" id="arquivo"/>
                                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                    <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                </div>
                            <?php }  ?>
                                
                        </td>
                    </tr>

                   
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE INCLUSÃO DE MEDIÇÕES. -->

    <!-- FORMULÁRIO DE CONTRATO/ADITIVO DE VALOR. -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTitulocentro" colspan="2">Aditivo de Valor</td>
        </tr>

        <tr>
            <td>
           <div id="div_botao_aditivo" ><center><input type="button" id="botaoSalvar" value="Adicionar Aditivo de Valor" onclick="habilitaAditivoValor();"></center></div>  
            <div style="display: none" id="div_aditivo_valor">
            
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Número:
                        </td>
                        <td>
                            <input type="text" style="text-align:left;" name="ccenumero"
                                   id="ccenumero" size="15" maxlength="10" value=""
                                   onKeyUp=""
                                   onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();"
                                   onmouseout="MouseOut(this);" onblur="MouseBlur(this);"
                                   title="" class='obrigatorio normal'
                            />
                            <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'/>
                        </td>
                    </tr>
					 <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Data de Assinatura:
                        </td>
                        <td>
                          <?= campo_data2('ccedataassinatura', 'N', 'S', '', ''); ?>
                           <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor:
                        </td>
                        <td>
                          <input
                                        type="text"
                                        style="text-align:left;"
                                        name="ccevalor"
                                        id="ccevalor"
                                        onmouseout="MouseOut(this);"
                                        onfocus="MouseClick(this);this.select();"
                                        onmouseover="MouseOver(this);"
                                        onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                                        value="0,00" <?php // @todo tratar depois?>
                                        maxlength="18"
                                        size="17"
                                        class="obrigatorio normal"
                                />
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                    
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Arquivo:
                        </td>
                        <td>
                           
                                <div class="divArquivoUpload">
                                    <input type="hidden" name="arqMedicao" id="arqMedicao" value=""/>
                                    <input type="file" name="input_original_aditivo" id="arquivo_aditivo"/>
                                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                    <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                </div>
                        </td>
                    </tr>
                    
                    <tr>
	                    <td colspan="2">
	                    	<center>
								<input type="button" value="Adicionar Documento" onclick="atualizarListaDocumentos()"/>
	                    	</center>
	                   	<td>
                    </tr>
                    
                </table>
               </div>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO  DE INCLUSÃO DE MEDIÇÕES. -->

    <br/>
               		<div id="lista_excluidas_edicao">
               			
               		</div>
   
        <div id="acompanhamento" style="display: none">

            <!-- INÍCIO DA TABELA DE ACOMPANHAMENTO. -->
            <table width="95%" align="center" border="1" style="border-collapse: collapse" id="tbAcompanhamento">
                <thead>
               <tr>
                    <td class="tbAcompanhamentoTitulo">Ações</td>
                    <td class="tbAcompanhamentoTitulo">Número Aditivo de Valor</td>
                    <td class="tbAcompanhamentoTitulo">Data de Assinatura</td>
                    <td class="tbAcompanhamentoTitulo">Valor</td>
                    
                </tr>
                </thead>
                <tbody>
                <?php 
                 
                if(  $edicao )
                {
                	echo "<input type='hidden' name='edicao[]'  id='edicao' value='1' >";
                }
                
                if( $temAditivos && $edicao )
                {
                	$dadosAditivos = ($dadosAditivos) ? $dadosAditivos : array();
                	if(count($dadosAditivos) > 0 )
                	{
                		echo "<input type='hidden' id=total_aditivos value={$totalAditivos}>";
                		
	                	foreach( $dadosAditivos as $key => $value )
	                	{
	                		$valorAtual  = formata_numero_monetario($value['ccevalor']);

	                		echo "<tr>"
			                  . "<td> 
    								<input type='hidden' name='cceid[]' value='".$value['cceid']."' > 
    								<span class='acaoTbAcompanhamento' alt='Remover Aditivo' title='Remover Aditivo' onclick='removerAditivoEdicao(this, " . $idControleObjDocumento . ", ".$value['cceid'].")'><img src='/imagens/excluir.gif' alt='Excluir Aditivo'/></span>
									<a class='link'  alt='Efetuar download do anexo do Aditivo' title='Efetuar download do anexo do Aditivo' onclick='downloadArquivo({$value['arqid']})'> <img src='/imagens/salvar.png' border='0'>  </a>
								</td>"
			                  . "<td>" . $value['ccenumero'] . "</td>"
			                  . "<td>" . $value['ccedataassinatura'] . "</td>"
			                  . "<td>". $valorAtual . "</td>"
			                  . "</tr>";
							  $idControleObjDocumento++;
	                	}
                	}
                }
                
                ?>
                
                </tbody>
            </table>
            <!-- FIM DA TABELA DE ACOMPANHAMENTO. -->

            <br/>

            <div style="text-align: center">
            
            	<center>
               	<div id="loading" style="text-align: center; display: none"><img src="../imagens/wait.gif" />&nbsp;Aguarde.</div>
               </center>
            </div>
            
        </div>
        <center>
        		<input type="button" id="botaoSalvar" value="Salvar" onclick="salvarDocumento();"> 
               	<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
         </center>
      
</form>


    <script type="text/javascript">
        $(document).ready(function() 
        {   
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
    </script>


<script type="text/javascript">

    var listaObjDocumentos = [];
    var idControleObjDocumento	= 0;
    var excluiEdicao 			= false;
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
     
    function excluirArquivo(arqid){
         if(confirm('Você deseja realmente excluir este arquivo?')) {
        	 $("#edicao_arquivo").val(0);
             $("#arqidOld").val(arqid);
             $(".divArquivoDownload").hide("fast");
             $(".divArquivoUpload").show("fast");
         }
     }
    /**
     * Função responsável por atualizar a tabela de acompanhamento à medida que um novo documento é incluído.
     * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
     * @returns {boolean}
     */
    function atualizarListaDocumentos() 
	{
		var retornoValidacao = validarDadosFormulario(true);
		//var retornoValidacao = true;

		if (retornoValidacao !== true) {
			alert(retornoValidacao);
			return false;
		}

		$("#acompanhamento").show();
  		// Trata as questões do Aditivo
		var ccenumero 			= $("#ccenumero").val();
		var ccedataassinatura	= $("#ccedataassinatura").val(); 
		//Trata o valor do aditivo
		var valorAditivoTela = $("#ccevalor").val();
		var valorAditivo = $("#ccevalor").val();
		valorAditivo = valorAditivo.replace(/[.]/g, "");
		valorAditivo = valorAditivo.replace(/[,]/g, ".");
  		// Trata a questão do arquivo do aditivo
		var clone = $("#arquivo_aditivo").clone();
  		clone.attr('name',   "arquivo_aditivo[]");
  		clone.attr('id',   "arquivo_aditivo_" + idControleObjDocumento);
  		$("#arquivo_aditivo").val('');
  		$("#ccenumero").val('');
  		$("#ccedataassinatura").val('');
  		$("#ccevalor").val('');

        // Obj para tratar a questão das linhas da tabela
        objDocumento = {
			"id": idControleObjDocumento
		};


		listaObjDocumentos.push(objDocumento);
		idControleObjDocumento++;
  		
		$("#tbAcompanhamento > tbody:last-child").append(
                  "<tr>"
                  + " <input type='hidden' name='edicao[]' value='0' > <td><span class='acaoTbAcompanhamento' onclick='removerDocumentoTabelaAcompanhamento(this, " + objDocumento.id + ")'><img src='/imagens/excluir.gif' title='Excluir Aditivo' alt='Excluir Aditivo'/></span> <img src='/imagens/salvar_desabilitada.png' title='Para efeturar o download é necessário salvar o formulário.' alt='Para efeturar o download é necessário salvar o formulário.'></td>"
                  + "<td> <input type='hidden' name='ccenumero[]' 		value='"+ccenumero+"' > " + ccenumero + "</td>"
                  + "<td> <input type='hidden' name='ccedataassinatura[]' value='"+ccedataassinatura+"' > " + ccedataassinatura + "</td>"
                  + "<td> <input type='hidden' name='valorAditivo[]' 		value='"+valorAditivo+"' > " + valorAditivoTela + "<div style=\"display:none\" id='input_arquivo_"+idControleObjDocumento+"'></td>"
                  //+ "<td> </div></td>"
                  //+ "<td>" + objDocumento.valorTransacao + "</td>"
                  
                  + "</tr>"
		);
		$("#input_arquivo_"+idControleObjDocumento).append(clone);

		$("#ccevalor").val();
		var ccedataassinatura	= $("#ccedataassinatura").val();
  		
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
    	excluiEdicao = true;
		
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

        var valorContrato = $("#ccevalor_original").val();
        valorContrato = valorContrato.replace(".", "");
        valorContrato = valorContrato.replace(",", ".");
        
        var valorAditivo = $("#ccevalor").val();
        valorAditivo = valorAditivo.replace(".", "");
        valorAditivo = valorAditivo.replace(",", ".");

        
        if ($("#cexnumcnpj").val() == "") {
            return "Informe o CNPJ do fornecedor.";
        }
        else if ($("#razao_construtora").text() == "") {
            return "Clique no icone 'lupa' para pesquisar e informar a razão social do fornecedor.";
        }
        else if ($("#uf_fornecedor").text() == "") {
            return "Clique no icone 'lupa' para pesquisar e informar a UF do fornecedor.";
        }
        else if ($("#ccenumero_original").val() == "") 
        {
            return "Informe o Número Contrado.";
        }
        else if ($("#ccedataassinatura_original").val() == "") 
        {
            return "Informe a Data de Assinatura do Contrato.";
        }
        else if (parseFloat(valorContrato) <= 0 || ($("#ccevalor_original").val() == "")) 
		{
            return "O valor da transação deve ser maior que zero.";
        }
        else if ($("#arquivo").val() == "" && ($("#edicao_arquivo").val() != 1)) 
        {	
            return "Insira o Arquivo do Contrato";   
        }
        else if ($("#licitacao").val() == "")
        {
            return "Informe o Número da Licitação.";
        }

        if( validaAditivo )
        {
	        if ($("#ccenumero").val() == "") 
	        {
	        	return "Informe o Número do Aditivo.";
	        }
	        else if ($("#ccedataassinatura").val() == "") 
	        {
	        	return "Informe a Data de Assinatura do Aditivo";
	        }
	        else if (parseFloat(valorAditivo) <= 0 || ($("#ccevalor").val() == "")) 
	        {
	        	return "Insira o Valor do Aditivo";
	        }
	        else if ($("#ccevalor").val() == "") 
	        {
	        	return "Insira o Arquivo do Aditivo";
	        }
	        else if ($("#arquivo_aditivo").val() == "") 
	        {
	            return "Insira o Arquivo do Aditivo";
	        }
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

		var confirmacao = true;
      	if(excluiEdicao)
    	{
	    	if( confirm('Foram excluídos Aditivos, confirma as modificações?') ){
	    		confirmacao = true;
	    	}
	    	else
	    	{
	    		confirmacao = false;
	    	}
    	}
		
		if(confirmacao)
		{
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
		}
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

