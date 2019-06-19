<?php
/**
 * Classe de mapeamento da entidade public.modalidadeaplicacao.
 *
 * @version $id$
 */

/**
 * Public_Model_Modalidadeaplicacao
 *
 * @author MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 */
class Public_Model_Modalidadeaplicacao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'public.modalidadeaplicacao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'mapcod',
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
        'mapcod' => null,
        'mapdsc' => null,
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
            'mapcod' => array(  new Zend_Validate_StringLength(array('max' => 2))  ),
            'mapdsc' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 150))  ),
        );
    }

    public static function getQueryCombo()
    {
        $sql = <<<DML
SELECT map.mapcod AS codigo,
       map.mapcod || ' - ' || map.mapdsc AS descricao
  FROM public.modalidadeaplicacao map
DML;
        return $sql;
    }
}
