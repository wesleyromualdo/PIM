<?php
/**
 * Classe de mapeamento da entidade par3.analise_engenharia_obra_estudo_demanda_observacao.
 *
 * @version $Id$
 * @since 2017.11.17
 */

/**
 * Par3_Model_Analise_engenharia_obra_estudo_demanda_observacao: Tabela utilizada para cadastrar as observações do estudo de demanda
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Analise_engenharia_obra_estudo_demanda_observacao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Analise_engenharia_obra_estudo_demanda_observacao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $aerid FK par3.analise_engenharia_obra_aba_respostas
 * @property string $aed_observacao Observação da análise de engeniaria do estudo de demanda
 */
class Par3_Model_AnaliseEngenhariaObraEstudoDemandaObservacao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.analise_engenharia_obra_estudo_demanda_observacao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aedid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'aegid' => array('tabela' => 'par3.analise_engenharia_obra', 'pk' => 'aegid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'aedid' => null,
        'aegid' => null,
        'aed_observacao' => null,
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
            'aedid' => array('Digits'),
            'aegid' => array('allowEmpty' => true,'Digits'),
            'aed_observacao' => array('allowEmpty' => true),
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

    public function recuperarObservacao($aegid)
    {
        return $this->recuperar("SELECT * FROM {$this->stNomeTabela} WHERE aegid = {$aegid}");
    }
}
