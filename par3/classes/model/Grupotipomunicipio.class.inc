<?php
/**
 * Classe de mapeamento da entidade territorios.grupotipomunicipio.
 *
 * @version $Id$
 * @since 2017.06.27
 */

/**
 * Territorios_Model_Grupotipomunicipio: sem descricao
 *
 * @package Territorios\Model
 * @uses Simec\Db\Modelo
 * @author Ricardo Macedo Rangel Borges <ricardoborges@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Territorios_Model_Grupotipomunicipio($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Territorios_Model_Grupotipomunicipio($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $gtmid  - default: nextval('territorios.grupotipomunicipio_gtmid_seq'::regclass)
 * @property string $gtmdsc 
 * @property bool $gtmcumulativo  - default: false
 * @property string $gtmstatus  - default: 'A'::bpchar
 */
class Par3_Model_Grupotipomunicipio extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'territorios.grupotipomunicipio';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'gtmid',
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
        'gtmid' => null,
        'gtmdsc' => null,
        'gtmcumulativo' => null,
        'gtmstatus' => null,
    );

    public function pegarSQLSelectCombo($gtmid = array())
    {
    	$gtmid = is_array($gtmid) ? implode(',',$gtmid):$gtmid;
    	$where = "WHERE gtmstatus = 'A'";
    	$where  = $gtmid ? " AND gtmid in({$gtmid})":'';
    		
    	return "SELECT  gtmid as codigo, gtmdsc as descricao FROM {$this->stNomeTabela} $where";
    }
    
    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'gtmid' => array('Digits'),
            'gtmdsc' => array(new Zend_Validate_StringLength(array('max' => 100))),
            'gtmcumulativo' => array(),
            'gtmstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

}
