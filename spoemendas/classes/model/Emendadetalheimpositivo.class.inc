<?php
/**
 * Classe de mapeamento da entidade emenda.emendadetalheimpositivo.
 *
 * @version $Id: Emendadetalheimpositivo.class.inc 129614 2017-08-25 18:39:30Z victormachado $
 * @since 2016.03.02
 */

/**
 * Spoemendas_Model_Emendadetalheimpositivo
 *
 * @package Emenda\Model
 * @uses Simec\Db\Modelo
 * @author Maykel Santos Braz <maykel.braz@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Emenda_Model_Emendadetalheimpositivo($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Emenda_Model_Emendadetalheimpositivo($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $edeid
 * @property string $edidescricao
 * @property string $ediimpositivo Verifica se Existe impositivo. NH - Não Há / TO - Total / PA - Parcial
 * @property string $usucpf
 * @property string $edistatus  - default: 'A'::bpchar
 * @property \Datetime(Y-m-d H:i:s) $edidata  - default: now()
 * @property numeric $edivalor
 * @property int $emdid
 * @property int $ediid  - default: nextval('emenda.emendadetalheimpositivo_ediid_seq'::regclass)
 */
class Spoemendas_Model_Emendadetalheimpositivo extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'emenda.emendadetalheimpositivo';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ediid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'edeid' => array('tabela' => 'emenda.emendadetalheentidade', 'pk' => 'edeid'),
        'emdid' => array('tabela' => 'emenda.emendadetalhe', 'pk' => 'emdid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'edeid' => null,
        'edidescricao' => null,
        'ediimpositivo' => null,
        'usucpf' => null,
        'edistatus' => null,
        'edidata' => null,
        'edivalor' => null,
        'emdid' => null,
        'ediid' => null,
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
            'edeid' => array('allowEmpty' => true,'Digits'),
            'edidescricao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2000))),
            'ediimpositivo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'edistatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'edidata' => array('allowEmpty' => true),
            'edivalor' => array('allowEmpty' => true),
            'emdid' => array('allowEmpty' => true,'Digits'),
            'ediid' => array('Digits'),
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
        if (strstr($this->edivalor, ',')) {
            $this->edivalor = str_replace(['.', ','], ['', '.'], $this->edivalor);
        }
        if (empty($this->usucpf)) {
            $this->usucpf = $_SESSION['usucpf'];
        }

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function atualizarImpedimentoByEdeid($edeid, $emdid, $dados)
    {
        $dadosImpedimento = $this->pegaLinha("SELECT ediid, emdid, COALESCE(edivalor, 0) edivalor, edidata, edistatus ,usucpf, ediimpositivo, edidescricao, edeid FROM emenda.emendadetalheimpositivo WHERE edeid = '{$edeid}' AND edistatus = 'A'");

        $dados['valorimpedido'] = empty($dados['valorimpedido']) ? 0 : $dados['valorimpedido'];

        if (empty($dadosImpedimento) && empty($dados['valorimpedido'])) {
            return;
        } elseif ($dadosImpedimento['edivalor'] == $dados['valorimpedido']) {
            return;
        } else {
            $modelEmDetImpositivo = new Spoemendas_Model_Emendadetalheimpositivo($dadosImpedimento['ediid']);
            $modelEmDetImpositivo->edistatus = 'I';
            $modelEmDetImpositivo->salvar();

            $modelEmDetImpositivo = new Spoemendas_Model_Emendadetalheimpositivo();
            $modelEmDetImpositivo->edivalor = $dados['valorimpedido'];
            $modelEmDetImpositivo->usucpf = $_SESSION['usucpf'];
            $modelEmDetImpositivo->edeid = $edeid;
            $modelEmDetImpositivo->emdid = $emdid;

            if (empty($dados['valorimpedido']))
            {
                // NH
                $modelEmDetImpositivo->ediimpositivo = 'NH';
                $modelEmDetImpositivo->edidescricao = 'PTA preenchido sistemicamente com base no retorno do SIOP';

            } elseif ($dados['valorimpedido'] < $dados['valor_rcl_apurada']){
                // PA
                $modelEmDetImpositivo->ediimpositivo = 'PA';
                $modelEmDetImpositivo->edidescricao = 'Impedimento gerado pelo sistema com base no retorno do SIOP';

            } else {
                // TO
                $modelEmDetImpositivo->ediimpositivo = 'TO';
                $modelEmDetImpositivo->edidescricao = 'Impedimento gerado pelo sistema com base no retorno do SIOP';
            }

            $ediid = $modelEmDetImpositivo->salvar();

            $modelEmImpBeneficiario = new Spoemendas_Model_Emendaimpedimentobeneficiario();
            $modelEmImpBeneficiario->ediid = $ediid;
            $modelEmImpBeneficiario->emiid = $dados['rp_atual'] == 7 ? 53 : 6;
            $modelEmImpBeneficiario->ireid = -1;
            $modelEmImpBeneficiario->eibdscobj = 'Outros (Outros)';

            $modelEmImpBeneficiario->salvar();
        }

    }
}
