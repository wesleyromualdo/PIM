<?php

/**
 * Classe de mapeamento da entidade spoemendas.anexogeral.
 *
 * $Id$
 */
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Mapeamento da entidade spoemendas.anexogeral.
 *
 * @see Modelo
 */
class Spoemendas_Model_Anexogeral extends Modelo {

    /**
     *
     * @var Simec_Helper_FlashMessage
     */
    protected $_message;

    public function __construct() {
        $this->_message = new Simec_Helper_FlashMessage('spoemendas/comunicados');
    }

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'spoemendas.anexogeral';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'arqid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'arqid' => null,
        'pdcid' => null,
        'arpdtinclusao' => null,
        'arpstatus' => null,
        'arpdsc' => null,
        'arptipo' => null,
        'angdsc' => null,
        'angtipoanexo' => null,
        'angano' => null,
        'angtip' => null,
        'solid' => null,
        'angobservacao' => null,
    );

    public function cadastrarAnexo($solid, $angdsc) {
        /* Verificando extensão. */
        //    if (array_search(
        //            strtolower(end(explode('.', $_FILES['file']['name']))), 
        //            $this->_arExtencoes) === false) {
        //        $this->_message->addMensagem('Por favor, envie arquivos somente com as extensões permitidas: <code>'.  implode(',', $this->_arExtencoes) .'</code>.', Simec_Helper_FlashMessage::AVISO);
        //        return false;
        //    } else {
        
        //caso exista anexo, remove e inclui novo arquivo
        if ($this->existeAnexo($solid, $angdsc)) {
            $this->deletarFile($this->existeAnexo($solid, $angdsc));
        }
        $campos = array(
            "angdsc" => "'{$_REQUEST['anexos']['angdsc']}'",
            "angtip" => "'{$_REQUEST['anexos']['angtip']}'",
            "angano" => "'{$_SESSION['exercicio']}'",
            "solid" => $solid,
            "angobservacao" => "'{$_REQUEST['anexos']['angobservacao']}'",        
        );
        $file = new FilesSimec("anexogeral", $campos, "spoemendas");

        if ($file->setUpload($_FILES['anexos_anexo']['name'], '', true)) {
            return $_FILES['anexos_anexo']['value'] = $file->getIdArquivo();
        } else {
            return false;
        }
    }

    public function downloadAnexo($arqid, $esquema='spoemendas', $tabela='anexogeral') {
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        if ($arqid) {
            $file = new FilesSimec($tabela, array(), $esquema);
            $file->getDownloadArquivo($arqid);
        }
    }

    public function deletarFile($arqid) {
        $sql = "DELETE FROM {$this->stNomeTabela} WHERE arqid = {$arqid}";
        $this->executar($sql);
        if ($this->commit()) {
            $this->_message->addMensagem('Arquivo deletado com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
            return true;
        } else {
            $this->_message->addMensagem('Falha ao deletar arquivo.', Simec_Helper_FlashMessage::ERRO);
            return false;
        }
    }

    public function existeAnexo($solidAnexo, $angdsc) {
        $sql = "SELECT arqid FROM spoemendas.anexogeral as pag INNER JOIN spoemendas.solicitacoes ps ON ps.solid = pag.solid WHERE ps.solid = {$solidAnexo} AND angdsc = '{$angdsc}'";
        $arqid = $this->pegaUm($sql);
        if ($arqid) {
            return $arqid;
        }
    }

}
