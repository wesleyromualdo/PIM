<?php
/**
 * Classe de mapeamento da entidade par3.obra_tipo_x_documento_apoio.
 *
 * @version $Id$
 * @since 2017.07.06
 */

/**
 * Par3_Model_obra_tipo_x_documento_apoio: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Renner Nascentes Tanizaki <rennertanizaki@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_obra_tipo_x_documento_apoio($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_obra_tipo_x_documento_apoio($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $odaid 
 * @property int $otpid 
 */
class Par3_Model_ObraTipoXDocumentoApoio extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_x_documento_apoio';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'otpid' => array('tabela' => 'par3.obra_tipo', 'pk' => 'otpid'),
        'odaid' => array('tabela' => 'par3.obra_documento_apoio', 'pk' => 'odaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'odaid' => null,
        'otpid' => null,
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
            'odaid' => array('allowEmpty' => true,'Digits'),
            'otpid' => array('allowEmpty' => true,'Digits'),
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
