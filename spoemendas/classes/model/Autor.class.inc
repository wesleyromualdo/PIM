<?php
/**
 * Classe de mapeamento da entidade emenda.autor.
 *
 * @version $Id: Autor.class.inc 129549 2017-08-24 21:04:04Z victormachado $
 */

/**
 * Spoemendas_Model_Autor
 *
 * @package  Spoemendas\model
 * @author   MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 * @property type $name Description
 */
class Spoemendas_Model_Autor extends Modelo
{
    /**
     * @var string Nome da tabela especificada.
     */
    protected $stNomeTabela = 'emenda.autor';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'autid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'parid' => array('tabela' => 'emenda.partido', 'pk' => 'parid'),
        'tpaid' => array('tabela' => 'emenda.tipoautor', 'pk' => 'tpaid'),
        'estuf' => array('tabela' => 'territorios.estado', 'pk' => 'estuf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'autid' => null,
        'tpaid' => null,
        'estuf' => null,
        'parid' => null,
        'autnome' => null,
        'autnomeabreviado' => null,
        'autemail' => null,
        'autstatus' => null,
        'autcod' => null,
    );

    /**
     *
     * @param type $dados
     * @return type
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'autid' => array(  'Digits'  ),
            'tpaid' => array(  'Digits'  ),
            'estuf' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 2))  ),
            'parid' => array( 'allowEmpty' => true, 'Digits'  ),
            'autnome' => array(  new Zend_Validate_StringLength(array('max' => 255))  ),
            'autnomeabreviado' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))  ),
            'autemail' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))  ),
            'autstatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'autcod' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 10))  ),
        );
    }

    public function pegaPorEmeid($emeid)
    {
        $sql = <<<DML
SELECT aut.autid,
       aut.autnome,
       aut.autemail,
       aut.autcod,
       tpa.tpanome,
       gpa.gpanome,
       gpa.gpacategoria,
       aut.parid
  FROM {$this->stNomeTabela} aut
    INNER JOIN emenda.emenda eme USING(autid)
    LEFT JOIN emenda.tipoautor tpa USING(tpaid)
    LEFT JOIN emenda.grupoautor gpa USING(gpaid)
  WHERE eme.emeid = {$emeid}
DML;
        return $this->pegaLinha($sql);
    }

    /**
     * Retorna o ID do autor a partir do código do autor.
     *
     * @param $autcod
     * @return bool|mixed|NULL|string
     */
    public function carregarPorAutcod($autcod)
    {
        $sql = <<<SQL
          SELECT autid FROM {$this->stNomeTabela} WHERE autcod = '{$autcod}' AND autstatus = 'A'
SQL;
        return $this->pegaUm($sql);
    }

}
