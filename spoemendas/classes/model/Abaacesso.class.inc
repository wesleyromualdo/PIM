<?php
/**
 * Classe de mapeamento da entidade spoemendas.abaacesso.
 *
 * @version $Id$
 * @since 2017.06.26
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Spoemendas_Model_Abaacesso: sem descricao
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 * @author Jeferson Carlos AraÚjo <jeferson.araujo@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Abaacesso($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Abaacesso($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $apaid 
 * @property string $aacaba 
 * @property int $aacid  - default: nextval('spoemendas.abaacesso_aacid_seq'::regclass)
 */
class Spoemendas_Model_Abaacesso extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.abaacesso';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aacid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'apaid' => array('tabela' => 'spoemendas.acessoperfilabas', 'pk' => 'apaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'apaid' => null,
        'aacaba' => null,
        'aacid' => null,
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
            'apaid' => array('allowEmpty' => true,'Digits'),
            'aacaba' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 30))),
            'aacid' => array('Digits'),
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

    public function salvarAbas($aba)
    {
        $sql = "INSERT INTO $this->stNomeTabela
                                    (aacaba,apaid)
                                    VALUES('{$aba['aacaba']}',{$aba['apaid']})";
        $this->carregar($sql);
    }

    /**
     * @param $aba
     */
    public function removerAcessoAbas($apaid)
    {
        $sql = "DELETE FROM $this->stNomeTabela WHERE apaid = {$apaid} ";
        $this->executar($sql);
    }

}
