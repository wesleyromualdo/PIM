<?php
/**
 * Classe de mapeamento da entidade spoemendas.solicitacaofinanceirahistorico.
 *
 * @version $Id$
 * @since 2017.09.11
 */

/**
 * Spoemendas_Model_Solicitacaofinanceirahistorico: Responsáve por historico de alteracoes de solicitacao financeira
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Solicitacaofinanceirahistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Solicitacaofinanceirahistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $sfhid Idenrificador do historico - default: nextval('spoemendas.solicitacaofinanceirahistorico_sfhid_seq'::regclass)
 * @property int $sfnid Identificador de solicitacao financeira
 * @property string $usucpf Idenrificador de usuario
 * @property \Datetime(Y-m-d H:i:s) $sfhdataalteracao Data de alteracao do registro - default: now()
 * @property string $sfhatual Conjunto de dados alterados
 * @property string $sfhantigo Conjunto de dados antes da modificacao
 */
class Spoemendas_Model_Solicitacaofinanceirahistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.solicitacaofinanceirahistorico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'sfhid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'sfnid' => array('tabela' => 'spoemendas.solicitacaofinanceira', 'pk' => 'sfnid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'sfhid' => null,
        'sfnid' => null,
        'usucpf' => null,
        'sfhdataalteracao' => null,
        'sfhatual' => null,
        'sfhantigo' => null,
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
            'sfhid' => array('Digits'),
            'sfnid' => array('Digits'),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'sfhdataalteracao' => array(),
            'sfhatual' => array(new Zend_Validate_StringLength(array())),
            'sfhantigo' => array(new Zend_Validate_StringLength(array())),
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

    /**
     * Retorna os dados do historico
     * @author Victor Eduardo Barreto
     * @param int $sfnid Identificador do historico
     * @return array
     */
    public function dadosHistoricoSolicitacao($sfnid) {
        $sql = <<<SQL
            SELECT sfhid,
                   (row_number() over (ORDER BY sfhid)) as seq,
                   to_char(sfhdataalteracao, 'DD/MM/YY HH:mm:ss'),
                   usunome
            FROM {$this->stNomeTabela}
            JOIN seguranca.usuario using(usucpf)
            WHERE sfnid = {$sfnid}
            ORDER BY sfhid DESC
SQL;
        return $this->carregar($sql);
    }

    /**
     * Prepara os dados para a criação de um de-para de histórico de alteração
     * @author Victor Eduardo Barreto
     * @return string Tabela comparativa
     */
    public function montaComparacaoHistorico() {
        try {
            $campos = [
                'Ano' => 'exercicio',
                'Unidade Orçamentária (UO)' => 'unicod',
                'Autor' => 'autid',
                'Interessado' => 'sfninteressado',
                'Emenda' => 'emeid',
                'PTRES' => 'ptrid',
                'Grupo de despesa (GND)' => 'sfngrupodespesa',
                'UG Executora' => 'sfnugexecutora',
                'Nota de Empenho' => 'sfnnotaempenho',
                'Vinculação' => 'vinc',
                'Fonte de Recurso Detalhada' => 'sfnfontedetalhada',
                'UF' => 'estuf',
                'Município' => 'muncod',
                'Contrato Repasse' => 'sfncontratorepasse',
                'Proposta no SICONV' => 'sfnpropostasiconv',
                'Convênio SIAFI' => 'sfnconveniosiafi',
                'Nº de Referência constante na nota de empenho no SIAFI' => 'sfnnumeroreferencia',
                'Outros' => 'sfnnumeroreferenciaoutros',
                'Máquina/Equipamento' => 'sfnmequipamento',
                'Objeto' => 'sfnobjetivo',
                'Grau de Prioridade' => 'sfngrauprioridade',
                'Valor a Solicitar' => 'sfpvalorpedido'
            ];

            return $this->diffHistorico($campos, ['Versão Anterior', 'Versão Alterada'], self::jsonStringToObject($this->sfhatual), self::jsonStringToObject($this->sfhantigo));
        } catch (Exception $e) {
            throw new Exception($e->getMessage());
        }
    }

    /**
     * Retorna um HTML com formulários em paralelo apresentando as diferenças
     * dos dados enviados
     * @author Victor Eduardo Barreto
     * @param array $campos Campos do formulário a serem comparados
     * @param array $titulos Títulos apresentados sobre os formulários
     * @param object $atual Dados atuais
     * @param object $antigo Dados antigos
     * @return string Tabela comparativa dos dados
     */
    public function diffHistorico($campos, array $titulos, stdClass $atual, $antigo) {
        $html = "
            <div class='row well'>
            <div class='col-sm-6'>
                <legend>{$titulos[0]}</legend>
            </div>
            <div class='col-sm-6'>
                <legend>{$titulos[1]}</legend>
            </div>
        ";

        foreach ($campos as $label => $value) {
            $stAnti = '';
            $stNovo = '';

            if ($atual->$value != $antigo->$value) {
                $stAnti = 'style="background-color:#f2dede; color:#b94a48"';
                $stNovo = 'style="background-color:#dff0d8; color:#468847"';

                $html .= "
                    <div class='col-sm-6'>
                        <div clas='form-group'>
                            <label class='pull-left'>{$label}</label>
                            <input class='form-control' {$stAnti} value='{$antigo->$value}' disabled>
                        </div>
                    </div>
                    <div class='col-sm-6'>
                        <div clas='form-group'>
                            <label class='pull-left'>{$label}</label>
                            <input class='form-control' {$stNovo} value='{$atual->$value}' disabled>
                        </div>
                    </div>
                ";
            }
        }
        return $html;
    }

    /**
     * Compara dados antigos e novos de solicitação financeira
     * @author Victor Eduardo Barreto
     * @param array $ant Dados salvos anteriormente
     * @param array $nov Dados novos a serem salvos
     * @return bool Resultado da comparação
     */
    public function historicoSolicitacao($dados) {
        try {
            $atu = str_replace('\\', '', $dados['sfhatual']);
            $ant = $this->recuperarTodos('sfhatual', ["sfnid = {$dados['sfnid']}"]);
            $ant = end($ant);
            // se houver modificação salva o historico
            if ($atu != $ant['sfhatual']) {
                $dados['usucpf'] = $_SESSION['usucpf'];
                $dados['sfhatual'] = $atu;
                $dados['sfhantigo'] = $ant['sfhatual'];
                $this->popularDadosObjeto($dados);
                $this->salvar();
                return true;
            }
        } catch (Exception $e) {
            $this->rollback();
            throw new Exception($e->getMessage());
        }
    }

    /**
     * Salva historico somente do valor do pedido alterado
     * @author Victor Eduardo Barreto
     * @throw Mensagem de erro
     * @param array $dados Dados da alteração do pedido
     * @return object Retorna o próprio modelo
     */
    public function historicoEditarPedido($dados) {
        try {
            $ant = $this->recuperarTodos('sfhatual', ["sfnid = {$dados['sfnid']}"]);
            $ant = end($ant);
            
            // converte o charset antes do decode
            $atu = self::jsonStringToObject($ant['sfhatual']);

            if (is_null($atu))
            {
                $atu = new stdClass();
            }

            // insere o novo valor
            $atu->sfpvalorpedido = formata_valor($dados['sfpvalorpedido']);
            $atu = json_encode($atu);
            // ajusta o charset antes de salvar
            $atu = (html_entity_decode($atu));
            $atu = str_replace('\\', '', $atu);
            // persiste no historico
            $dados['sfhatual'] = $atu;
            $dados['sfhantigo'] = $ant['sfhatual'];
            $this->popularDadosObjeto($dados);
            $this->salvar();
            $this->commit();
            
            return $this;
        } catch (Exception $e) {
            throw new Exception($e->getMessage());
        }
    }

    private static function jsonStringToObject ($json)
    {
        return json_decode(htmlentities(($json), ENT_NOQUOTES));
    }
}
