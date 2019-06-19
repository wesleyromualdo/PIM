<?php
/**
 * Classe de mapeamento da entidade emenda.partido.
 *
 * @version $Id: Partido.class.inc 108692 2016-03-01 14:22:46Z maykelbraz $
 */

/**
 * Spoemendas_Model_Partido
 *
 * @author MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 */
class Spoemendas_Model_Partido extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'emenda.partido';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'parid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'parid' => null,
        'parcodigo' => null,
        'parnome' => null,
        'parsigla' => null,
        'parstatus' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'parid' => array('Digits'),
            'parcodigo' => array('Digits'),
            'parnome' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'parsigla' => array(new Zend_Validate_StringLength(array('max' => 10))),
            'parstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
        );
    }

    public static function getQueryCombo()
    {
        $sql = <<<DML
SELECT par.parid AS codigo,
       par.parsigla || ' - ' || par.parnome AS descricao
  FROM emenda.partido par
  WHERE par.parstatus = 'A'
  ORDER BY par.parsigla
DML;
        return $sql;
    }
}
