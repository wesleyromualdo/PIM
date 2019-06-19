<?php
/**
 * Classe de mapeamento da entidade par3.itenscomposicao_ata_pregao_historico.
 *
 * @version $Id$
 * @since 2017.06.05
 */

/**
 * Model_Itenscomposicao_ata_pregaoHistorico: sem descricao
 *
 * @package Model\Itenscomposicao
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Itenscomposicao_ata_pregaoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Itenscomposicao_ata_pregaoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hatid chave primária da tabelaitenscomposicao_ata_pregao_historico
 * @property  $hatcpf CPF do respnsável pela alteração
 * @property  $hatacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hatdtcriacao Data de criação do log
 * @property int $atpid Chave Primária - default: nextval('par3.itenscomposicao_ata_pregao_atpid_seq'::regclass)
 * @property string $atpnumeroata Número da Ata de Pregão
 * @property string $atpanovigencia
 * @property \Datetime(Y-m-d H:i:s) $atpdtinicio Data de início da Ata de Pregão
 * @property \Datetime(Y-m-d H:i:s) $atpdtfim Data de término da Ata de Pregão
 * @property string $atpobjeto Descrição do Objeto da Ata de Pregão
 * @property int $arqid Código do arquivo da Ata de Pregão
 * @property string $atpdescricaoanexo Descrição do arquivo da Ata de Pregão
 * @property "char" $atpstatus status da Ata de Pregão (soft delete)
 * @property "char" $atpsituacao situação da Ata de Pregão, indica se está ativo/inativo
 * @property \Datetime(Y-m-d H:i:s) $atpdtinclusao data de inclusão da Ata de Pregão
 * @property string $atpcpfinclusao cpf do responsável pela inclusao da Ata de Pregão
 * @property \Datetime(Y-m-d H:i:s) $atpdtinativacao data de inativação da Ata de Pregão
 * @property int $atpvigencia
 * @property string $atpcpfinativacao cpf do responsável pela inativação da Ata de Pregão
 */
class Par3_Model_ItensComposicaoAtaPregaoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.itenscomposicao_ata_pregao_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hatid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'atpid' => array('tabela' => 'par3.itenscomposicao_ata_pregao', 'pk' => 'atpid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hatid' => null,
        'hatcpf' => null,
        'hatacao' => null,
        'hatdtcriacao' => null,
        'atpid' => null,
        'atpnumeroata' => null,
        'atpanovigencia' => null,
        'atpdtinicio' => null,
        'atpdtfim' => null,
        'atpobjeto' => null,
        'arqid' => null,
        'atpdescricaoanexo' => null,
        'atpstatus' => null,
        'atpsituacao' => null,
        'atpdtinclusao' => null,
        'atpcpfinclusao' => null,
        'atpdtinativacao' => null,
        'atpvigencia' => null,
        'atpcpfinativacao' => null,
    );


    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_ItenscomposicaoAtaPregao $model,$acao)
    {

        $this->arAtributos['hatcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hatacao'] = $acao;
        $this->arAtributos['hatdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['atpid'] = $model->atpid;
        $this->arAtributos['atpnumeroata'] = $model->atpnumeroata;
        $this->arAtributos['atpanovigencia'] = $model->atpanovigencia;
        $this->arAtributos['atpdtinicio'] = $model->atpdtinicio;
        $this->arAtributos['atpdtfim'] = $model->atpdtfim;
        $this->arAtributos['atpobjeto'] = $model->atpobjeto;
        $this->arAtributos['arqid'] = $model->arqid;
        $this->arAtributos['atpdescricaoanexo'] = $model->atpdescricaoanexo;
        $this->arAtributos['atpstatus'] = $model->atpstatus;
        $this->arAtributos['atpsituacao'] = $model->atpsituacao;
        $this->arAtributos['atpdtinclusao'] = $model->atpdtinclusao;
        $this->arAtributos['atpcpfinclusao'] = $model->atpcpfinclusao;
        $this->arAtributos['atpdtinativacao'] = $model->atpdtinativacao;
        $this->arAtributos['atpvigencia'] = $model->atpvigencia;
        $this->arAtributos['atpcpfinativacao'] = $model->atpcpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}
