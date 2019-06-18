<?php
/**
 * Classe de mapeamento da entidade public.fonterecurso.
 *
 * @version $id$
 */

/**
 * Public_Model_Fonterecurso
 *
 * @author MAYKEL SANTOS BRAZ <maykel.braz@mec.gov.br>
 */
class Public_Model_Fonterecurso extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'public.fonterecurso';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'foncod',
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
        'foncod' => null,
        'fondsc' => null,
        'fonstatus' => null,
        'fonabrev' => null,
        'grfid' => null,
        'clasproporc' => null,
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
            'foncod' => array(  new Zend_Validate_StringLength(array('max' => 3))  ),
            'fondsc' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 150))  ),
            'fonstatus' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
            'fonabrev' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))  ),
            'grfid' => array( 'allowEmpty' => true, 'Digits'  ),
            'clasproporc' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))  ),
        );
    }

    public static function getQueryCombo()
    {
        $sql = <<<DML
SELECT fon.foncod AS codigo,
       fon.foncod || ' - ' || fon.fondsc AS descricao
  FROM public.fonterecurso fon
  WHERE fon.fonstatus = 'A'
  ORDER BY fon.foncod
DML;
        return $sql;
    }
}
