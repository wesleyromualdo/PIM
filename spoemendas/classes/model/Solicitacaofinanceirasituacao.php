<?php
/**
 * Classe de mapeamento da entidade spoemendas.solicitacaofinanceirasituacao.
 *
 * @version $Id$
 * @since   2017.09.12
 */

/**
 * Spoemendas_Model_Solicitacaofinanceirasituacao: Contem informações das situações das solicitações Financeiras
 *
 * @package Spoemendas\Model
 * @uses    Simec\Db\Modelo
 * @author  Saulo Araújo Correia <saulo.correia@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Solicitacaofinanceirasituacao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Solicitacaofinanceirasituacao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int    $sfsid        Identificador da tabela - default: nextval('spoemendas.solicitacaofinanceirasituacao_sfsid_seq'::regclass)
 * @property string $sfsdescricao Descrição da situação da solicitação Financeira
 */
class Spoemendas_Model_Solicitacaofinanceirasituacao extends Modelo
{
    const PENDENTE_DE_AUTORIZACAO = 1;
    const AUTORIZADO              = 2;
    const AUTORIZADO_PARCIALMENTE = 3;
    const NAO_AUTORIZADO          = 4;
    const NAO_SOLICITADO          = 5;

    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.solicitacaofinanceirasituacao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = [
        'sfsid',
    ];

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = [
    ];

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = [
        'sfsid'        => null,
        'sfsdescricao' => null,
    ];

    /**
     * Validators.
     *
     * @param mixed[] $dados
     *
     * @return mixed[]
     */
    public function getCamposValidacao ($dados = [])
    {
        return [
            'sfsid'        => ['Digits'],
            'sfsdescricao' => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 50])],
        ];
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
    public function antesSalvar ()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function montarCombo(){
        return $this->carregar(<<<SQL
SELECT sfsid as codigo,
       sfsdescricao as descricao
FROM {$this->stNomeTabela}
ORDER BY 2
SQL
);
    }
    /**
     * Retorna todas as opções de situações para um pedido
     * @return array|mixed|NULL
     */
    public function pegaTodasSituacoes()
    {
        $sql = <<<DML
SELECT sfsid AS codigo, sfsdescricao AS descricao FROM {$this->stNomeTabela} ORDER BY sfsid ASC
DML;
        return $this->carregar($sql);
    }
}
