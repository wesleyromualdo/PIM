<?php

class Simec_Upload
{
    protected $_erros = [
        UPLOAD_ERR_OK => 'Arquivo carregado com sucesso.',
        UPLOAD_ERR_INI_SIZE => 'O tamanho do arquivo é maior que o permitido.',
        UPLOAD_ERR_PARTIAL => 'Ocorreu um problema durante a transferência do arquivo.',
        UPLOAD_ERR_NO_FILE => 'O arquivo enviado estava vazio.',
        UPLOAD_ERR_NO_TMP_DIR => 'O servidor não pode processar o arquivo.',
        UPLOAD_ERR_CANT_WRITE => 'O servidor não pode processar o arquivo.',
        UPLOAD_ERR_EXTENSION => 'O arquivo recebido não é um arquivo válido.'
    ];

    protected $_file;
    protected $_errorMessage = [];
    protected $_streamFile;
    private $extension;
    private $allowedTypes = ['csv'];

    const SUCESSO = true;
    const ERRO = false;

    const ERRO_NENHUM_ARQUIVO = 'Não foi possível processar sua requisição. Nenhum arquivo foi enviado.';
    const ERRO_UPLOAD_ERROR = 'Não foi possível carregar o arquivo de liberações. Motivo: %s';
    const ERRO_TIPO_NAO_ACEITO = 'Não foi possível carregar o arquivo de liberações. Motivo: Apenas arquivos <strong>.%s</strong> são aceitos.';
    const ERRO_UPLOAD_SEND = 'Não foi possível validar o arquivo enviado.';
    const ERRO_UPLOAD_READ = 'Não foi possível ler o arquivo de lote enviado.';

    public function __construct(array $file = array())
    {
        if (is_array($file) && count($file)) {
            $this->_file = $file;
            $this->_validator();
            return $this;
        }

        throw new Simec_Upload_Exception(self::ERRO_NENHUM_ARQUIVO);
    }

    protected function _validator()
    {
        $this->extension = strtolower(substr($this->_file['name'], -3));

        if (UPLOAD_ERR_OK != $this->_file['error']) {
            throw new Simec_Upload_Exception(
                sprintf(self::ERRO_UPLOAD_ERROR, $this->_erros[$this->_file['error']])
            );
        }

        if (!in_array($this->extension, $this->allowedTypes)) {
            throw new Simec_Upload_Exception(
                sprintf(self::ERRO_TIPO_NAO_ACEITO, implode(',', $this->allowedTypes))
            );
        }

        if (!is_uploaded_file($this->_file['tmp_name'])) {
            throw new Simec_Upload_Exception(self::ERRO_UPLOAD_SEND);
        }

        if (!($this->_streamFile = file($this->_file['tmp_name']))) {
            throw new Simec_Upload_Exception(self::ERRO_UPLOAD_READ);
        }
    }

    public function setExtensionAllowed($extension)
    {
        if ($extension) {
            array_push($this->allowedTypes, $extension);
        }

        return $this;
    }

    public function getStreamFile($unsetHeader = false)
    {
        if ($unsetHeader) {
            array_shift($this->_streamFile);
        }

        return ($this->_streamFile) ? $this->_streamFile : false;
    }
}