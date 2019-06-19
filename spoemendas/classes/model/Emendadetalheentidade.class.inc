<?php
/**
 * Classe de mapeamento da entidade emenda.emendadetalheentidade.
 *
 * @version $Id$
 * @since 2016.03.07
 */

/**
 * Spoemendas_Model_Emendadetalheentidade: Define o conjunto de entidades beneficiadas pelo detalhe da emenda. O autor da emenda, divide o valor total do detalhe da emenda para diversas entidades.
 *
 * @package Spoemendas\Model
 * @uses Simec\Db\Modelo
 * @author Maykel Santos Braz <maykel.braz@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Emendadetalheentidade($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Emendadetalheentidade($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property numeric $edevalordisponivel Valor disponivel para empenho
 * @property int $edecarga
 * @property numeric $edevalorreducao Valor da Redução
 * @property string $ededataindicacao Data da indicação do recurso - default: now()
 * @property string $ededddresp
 * @property bool $edeemailenviado  - default: false
 * @property string $emdliberado Indica se a emenda está liberada pela casa civil ou não. S = Sim N = Nao - default: 'N'::character varying
 * @property string $edetelresp Telefone do responsável da instuição indicado pelo parlamentar
 * @property string $edemailresp E-mail do responsável da instuição indicado pelo parlamentar
 * @property string $edenomerep Nome do responsável da instuição indicado pelo parlamentar
 * @property string $edecpfresp CPF do responsável da instuição indicado pelo parlamentar
 * @property string $ededataalteracao A data da última alteração do detalhe da emenda.
 * @property string $usucpfalteracao É o último usuário de alteração do detalhe da emenda.
 * @property string $ededisponivelpta Determina se o registro está liberado para criação de PTA ou não. - default: 'N'::bpchar
 * @property string $edeobjeto Um texto que descreve o que deverá ser feito com essa emenda
 * @property string $edestatus Situação da tabela emendadetalheentidade. Ativo ou Inativo - default: 'A'::bpchar
 * @property numeric $edevalor Valor destinado à entidade. É uma parte do valor total da emendadetalhe
 * @property int $enbid Identificador da tabela entidade. Representa a entidade beneficiada
 * @property int $emdid Identificador da tabela emendadetalhe
 * @property int $edeid Identificador da tabela emendadetalheentidade - default: nextval('emenda.emendadetalheentidade_edeid_seq'::regclass)
 * @property int $edeprioridade Indica a ordem de prioridade
 * @property numeric $edelimiteempenhobeneficiario valor do limite de empenho do beneficiario
 */
class Spoemendas_Model_Emendadetalheentidade extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emenda.emendadetalheentidade';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'edeid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'enbid' => array('tabela' => 'emenda.entidadebeneficiada', 'pk' => 'enbid'),
        'usucpfalteracao' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'edeid' => null,
        'emdid' => null,
        'enbid' => null,
        'edevalor' => null,
        'edestatus' => null,
        'edeobjeto' => null,
        'ededisponivelpta' => null,
        'usucpfalteracao' => null,
        'ededataalteracao' => null,
        'edecpfresp' => null,
        'edenomerep' => null,
        'edemailresp' => null,
        'ededddresp' => null,
        'edetelresp' => null,
        'emdliberado' => null,
        'edeemailenviado' => null,
        'ededataindicacao' => null,
        'edevalorreducao' => null,
        'edecarga' => null,
        'edevalordisponivel' => null,
        'telefone' => null, // -- telefone completo
        'edeprioridade' => null,
        'edelimiteempenhobeneficiario' => null
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array()) {
        return array(
            'edeid' => array('Digits'),
            'emdid' => array('Digits'),
            'enbid' => array('Digits'),
            'edevalor' => array('allowEmpty' => true),
            'edestatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'edeobjeto' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1000))),
            'ededisponivelpta' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'usucpfalteracao' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))),
            'ededataalteracao' => array('allowEmpty' => true),
            'edecpfresp' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))),
            'edenomerep' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 200))),
            'edemailresp' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 100))),
            'edetelresp' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 15))),
            'emdliberado' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'edeemailenviado' => array(),
            'ededddresp' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 3))),
            'ededataindicacao' => array('allowEmpty' => true),
            'edevalorreducao' => array('allowEmpty' => true),
            'edecarga' => array('allowEmpty' => true, 'Digits'),
            'edevalordisponivel' => array('allowEmpty' => true),
            'edeprioridade' => array('Digits'),
            'edelimiteempenhobeneficiario' => array('allowEmpty' => true),
        );
    }

//end getCamposValidacao($dados)

    public function antesSalvar() {
        $telefone = str_replace(['(', ')', ' '], '', $this->telefone);
        $this->ededddresp = substr($telefone, 0, 2);
        $this->edetelresp = substr($telefone, 2);
        $this->edecpfresp = str_replace(['.', '-'], '', $this->edecpfresp);

        unset($this->arAtributos['telefone']);

        return parent::antesSalvar();
    }

    public function findByEdeid($edeid) {
        $sql = <<<DML
SELECT *
  FROM {$this->stNomeTabela} ede
  JOIN emenda.emendadetalhe USING(emdid)
  WHERE edeid = {$edeid} and emdstatus = 'A'
DML;
        return $this->pegaLinha($sql);
    }

    public function findBy(array $criterio, $colunas = ['*'])
    {
        $colunas = implode(', ', $colunas);

        $where = [];
        foreach ($criterio as $coluna => $valor)
        {
            $where[] = "{$coluna} = '{$valor}'";
        }
        $where = implode(' AND ', $where);

        $query = <<<DML
SELECT {$colunas}
  FROM {$this->stNomeTabela} ede
  WHERE {$where}
DML;
        return $this->carregar($query);
    }

    public function findBeneficiariosByEmeid($emeid)
    {
        $sql = <<<DML
SELECT *
  FROM {$this->stNomeTabela} ede
    INNER JOIN emenda.emendadetalhe emd USING(emdid)
  WHERE emd.emeid = {$emeid} and emdstatus = 'A'
DML;

        return $this->carregar($sql);
    }
}
