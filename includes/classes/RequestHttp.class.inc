<?php
class RequestHttp{
	// guarda url's cadastradas
	static $url = array("to-pdf" => "http://ws.mec.gov.br/ws-server/htmlParaPdf");
	private $sisdir;

    protected $ultimoArquivoSalvo = '';

    public function ultimoArquivoSalvo()
    {
        return $this->ultimoArquivoSalvo;
    }

	function __construct(){
		$this->sisdir = $_SESSION['sisdiretorio'];
	}

    public function toPdf($html){
        // prepara requisição
        $data    = array('conteudoHtml' => $html);
        $options = array('http' => array('method'  => 'POST',
                                         'header'  => 'Content-type: application/x-www-form-urlencoded',
                                         'content' => http_build_query($data)));
        $context = stream_context_create($options);

        // requisita geração
        $uri  = 'http://ws.mec.gov.br/ws-server/htmlParaPdf';
        $file = fopen($uri, 'rb', false, $context);
        if (!$file) {
            throw new Exception('Erro ao requisitar geração de arquivo PDF.');
        }

        // captura arquivo
        $response = stream_get_contents($file);
        if ($response === false) {
            throw new Exception('Erro ao capturar arquivo PDF.');
        }
        return $response;
    }

    public function toPdfDownload($html, $arNome){

    	header('Content-Type: application/pdf');
		header('Content-Disposition: attachment; filename="' . $arNome . '.pdf"');

    	echo self::toPdf($html);
    }

    public function toPdfSave($html, Array $arParam){
		$pasta = APPRAIZ.'arquivos/'.$this->sisdir;
		/* verifica se a pasta existe, se não existir é criada */
		if(!file_exists($pasta)){
			mkdir("{$pasta}", 0777, true);
		}
    	$caminho = APPRAIZ . 'arquivos/' . $this->sisdir . '/' . $arParam['nome'] . '.pdf';
        $this->ultimoArquivoSalvo = $caminho;

    	$response = self::toPdf($html);

    	$fp = fopen($caminho, "w");
		if ($fp) {
		  stream_set_write_buffer($fp, 0);
		  fwrite($fp, $response);
		  fclose($fp);
		}
		self::save($caminho, $arParam);
    }

    private function save($caminhoArq, Array $arParam){
    	try{
    		if ( empty($caminhoArq) || empty($arParam['tabela']) || empty($arParam['campo']) || empty($arParam['extensao']) ){
    			throw new Exception("Faltam parametros no SAVE (" . get_class($this) . ")");
    		}
			$obFile = new FilesSimec($arParam['tabela'], $arParam['campo'], $this->sisdir);
			$obFile->setMover($caminhoArq, $arParam['extensao']);

			return true;
    	}catch(Exception $e){
    		echo $e->getMessage();
    		return false;
    	}
    }
}



//$obFile = new FilesSimec("anexo", $arCampo, $_SESSION['sisdiretorio']);
//$obFile->setUpload();
?>