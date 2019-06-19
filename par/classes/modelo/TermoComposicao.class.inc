<?php

class TermoComposicao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.termocomposicao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array();

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'dopid' => null,
        'sbdid' => null,
        'preid' => null
    );

    public function excluir($terid) {
        $sql = "DELETE FROM {$this->stNomeTabela} WHERE terid = {$terid}";
        return $this->executar($sql);
    }
    
    public function inserirComposicao(array $arrInsert) {
        $dopid = $arrInsert['dopid'];
        $preid = $arrInsert['preid'];
        
        $sql = "INSERT INTO {$this->stNomeTabela} (dopid, preid) VALUES ({$dopid}, {$preid})";
        return $this->executar($sql);
    }
}
