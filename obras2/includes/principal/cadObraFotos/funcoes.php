<?php 


function excluir(){
	
	global $db;
	
	$obrasArquivos	= new ObrasArquivos( $_REQUEST['oarid'] );
	$obrasArquivos->popularDadosObjeto( Array('oarstatus' => 'I') )
				  ->salvar();
	$obrasArquivos->commit();
	
	echo "<script type=\"text/javascript\">
			alert('Operação realizada com sucesso.');
			document.location.href = document.location.href;
		  </script>";
}

function download(){
	
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$obraArquivo = new ObrasArquivos();
	$arDados = $obraArquivo->buscaDadosPorArqid( $_REQUEST['arqid'] );
	$eschema = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');
	
	$file = new FilesSimec(null,null,$eschema);
	return $file->getDownloadArquivo($_REQUEST['arqid']);
	
	echo '<script type="text/javascript">
			document.location.href = document.location.href;
		  </script>';
}

function salvar()
{
	// Limita o tamanho do upload para no máximo 10Mb
	$maxUploadSize = 10485760;

	if ($maxUploadSize < $_FILES["arquivo"]["size"]) {
		echo '<script type="text/javascript"> 
						alert("O tamanho máximo para upload de arquivos é de 10Mb.");
						document.location.href = document.location.href;
				  </script>';
	} else {
		global $db;
	
		require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

		$campos	= array("obrid"		=> $_SESSION['obras2']['obrid'],
						"oardesc" 	=> "'".$_POST['oardesc']."'",
						"oardata" 	=> "'".formata_data($_POST['oardata'])."'",
						"tpaid" 	=> TIPO_OBRA_ARQUIVO_OUTROS,
						"oardata" 	=> " now()");
		
		$file = new FilesSimec("obras_arquivos", $campos, 'obras2');
		if($_FILES["arquivo"]){	
			
			$arquivoSalvo = $file->setUpload($_POST['oardesc']);	
			if($arquivoSalvo){
				echo '<script type="text/javascript"> 
							alert("Operação realizada com sucesso.");
							document.location.href = document.location.href;
					</script>';
			}
		}
	}
}

