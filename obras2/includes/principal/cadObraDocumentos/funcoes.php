<?php 

function excluir(){
    
    
    if(possui_perfil(Array(PFLCOD_GESTOR_UNIDADE,PFLCOD_SUPERVISOR_UNIDADE))){
        echo "<script type=\"text/javascript\">
			alert('Você não tem permissões suficientes para excluir esse documento!');
			document.location.href = document.location.href;
		  </script>";
    }else{
	
	global $db;
	
	$obrasArquivos	= new ObrasArquivos( $_REQUEST['oarid'] );
	$obrasArquivos->popularDadosObjeto( Array('oarstatus' => 'I') )
				  ->salvar();
	$obrasArquivos->commit();
	
	echo "<script type=\"text/javascript\">
			alert('Operação realizada com sucesso!');
			document.location.href = document.location.href;
		  </script>";
    }
}

function download(){
	
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$obraArquivo = new ObrasArquivos();
	$arDados = $obraArquivo->buscaDadosPorArqid( $_REQUEST['arqid'] );
	$eschema = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');
	
	$file = new FilesSimec(null,null,$eschema);
	$file->getDownloadArquivo( $_REQUEST['arqid'] );
	
	die('<script type="text/javascript">
			document.location.href = document.location.href;
		  </script>');
}

function salvar(){
	
	global $db;
	
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	$campos	= array("obrid"		=> $_SESSION['obras2']['obrid'],
					"oardesc" 	=> "'".$_POST['oardesc']."'",
					"tpaid" 	=> $_POST['tpaid'],
					"oardata" 	=> " now() ");
	
	$file = new FilesSimec("obras_arquivos", $campos, 'obras2');
	if($_FILES["arquivo"]){	
		
		$arquivoSalvo = $file->setUpload($_POST['oardesc']);	
		if($arquivoSalvo){
			echo '<script type="text/javascript"> 
                                    alert("Arquivo anexado com sucesso.");
                                    document.location.href = document.location.href;
                              </script>';
		}
	}
}
