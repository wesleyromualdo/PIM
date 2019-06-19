<?php
/**
 * Classe de mapeamento da entidade demandas.etapa_servico.
 *
 * @version $Id$
 * @since 2019.06.12
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * EtapaServicoModel_Etapa_servico: sem descricao
 *
 * @package EtapaServicoModel\Etapa
 * @uses Simec\Db\Modelo
 * @author Adminstrador De Sistema <simec@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new EtapaServicoModel_Etapa_servico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new EtapaServicoModel_Etapa_servico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $co_etapa_servico  - default: nextval('demandas.etapa_servico_co_etapa_servico_seq'::regclass)
 * @property string $nu_codigo_etapa_servico 
 * @property string $ds_etapa_servico 
 * @property int $co_etapa_servico_pai 
 * @property string $nu_cpf_inclusao 
 * @property \Datetime(Y-m-d H:i:s) $dt_inclusao  - default: now()
 * @property int $co_status  - default: 1
 */
class EtapaServicoModel_Etapa_servico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'demandas.etapa_servico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'co_etapa_servico',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'co_etapa_servico_pai' => array('tabela' => 'demandas.etapa_servico', 'pk' => 'co_etapa_servico'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'co_etapa_servico' => null,
        'nu_codigo_etapa_servico' => null,
        'ds_etapa_servico' => null,
        'co_etapa_servico_pai' => null,
        'nu_cpf_inclusao' => null,
        'dt_inclusao' => null,
        'co_status' => null,
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
            'co_etapa_servico' => array('Digits'),
            'nu_codigo_etapa_servico' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'ds_etapa_servico' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'co_etapa_servico_pai' => array('allowEmpty' => true,'Digits'),
            'nu_cpf_inclusao' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'dt_inclusao' => array(),
            'co_status' => array('Digits'),
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
