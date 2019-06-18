<?php

/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 26/08/2015
 * Time: 10:23
 */
require_once APPRAIZ . "includes/library/simec/Listagem.php";

class Legenda extends Modelo {

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.legendaindicadores";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('legid'); //,'muncod','stacod','docid');

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'legid' => null,
        'subiddial' => null,
        'legfxa1' => null,
        'legfxa2' => null,
        'legcor' => null,
        'legstatus' => null
    );

    /**
     * Campos da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arCampos = array(
        'legid',
        'subiddial',
        'legfxa1',
        'legfxa2',
        'legcor',
        'legstatus'
    );

    /**
     * Atributos da Tabela obrigatórios
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
        'legid',
        'subiddial',
        'legfxa1',
        'legfxa2',
        'legcor',
        'legstatus'
    );
    protected $atributosObrigatoriosString = array();

    /**
     * Atributos Integer da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosInt = array(
        'legid' => null,
        'subiddial' => null
    );
    public $msg;
    protected $ravid;
    protected $ratdata1periodo;
    protected $ratdata2periodo;

    public function __construct($id = null, $ravid = null, $ratdata1periodo = null, $ratdata2periodo = null) {
        parent::__construct($id);
        $this->ravid = $ravid;
        $this->ratdata1periodo = $ratdata1periodo;
        $this->ratdata2periodo = $ratdata2periodo;
    }

    public function setRavid($ravid) {
        $this->ravid = $ravid;
    }
    public function montaSqlRelatorio() {
        $ravid = $this->ravid;
        $ratdata1periodo = $this->ratdata1periodo;
        $ratdata2periodo = $this->ratdata2periodo;
        $esdid = ESDID_SASE_SEM_INFORMACAO_PLANCAR;

        if (isset($this->ravid)) {
            $sql = <<<DML
                select
                    mun.muncod,
                    mun.mundescricao,
                    case when spc.esdid = {$esdid} then 'X' end as spcseminfor,
                    case when spc.esdid <> {$esdid} then cast(spc.spccomplano as char) else '' end as spccomplanoS,
                    case when spc.esdid <> {$esdid} then cast(spc.spccomplano as char) else '' end as spccomplanoN,
                    case when spc.esdid <> {$esdid} then cast(spc.spcpagapiso as char) else '' end as spcpagapisoS,
                    case when spc.esdid <> {$esdid} then cast(spc.spcpagapiso as char) else '' end as spcpagapisoN,
                    case when spc.esdid <> {$esdid} then cast(spc.spccumprejornada as char) else '' end as spccumprejornadaS,
                    case when spc.esdid <> {$esdid} then cast(spc.spccumprejornada as char) else '' end as spccumprejornadaN,
                    stm.stmobservacoes,
                    mar.marid,
                    spc.esdid,
                    stm.stmid
                from sase.municipiosassistidosredeae mar
                inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
                inner join territorios.municipio mun on pcp.muncod = mun.muncod
                left join workflow.documento doc on doc.docid = pcp.docid
                left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
                left join sase.sitatualmunicipiotecredeae stm on stm.marid = mar.marid and stm.esdid = doc.esdid
                where ravid = {$ravid}
                order by mun.mundescricao
DML;
        } else {
            $this->msg = "Informe o código do relatório, campo 'ravid'";
            return false;
        }
        return $sql;
    }

    public function getTabelaRelatorio($perCampos = null) {
        $sql = $this->montaSqlRelatorio();
        if (!$sql) {
            return false;
        }

        $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $lista
                ->setQuery($sql)
                ->setCabecalho(
                        array(
                            'Municípios' => 'Situação',
                            'Sem Informação',
                            'Com Plano de Carreira' => array('Sim', 'Não'),
                            'Paga o Piso (Lei 11.738/2008)' => array('Sim', 'Não'),
                            'Cumpre Jornada (Lei 11.783/2008)' => array('Sim', 'Não'),
                            'Observação Quanto a meta 18 (PNE)'
                        )
        );
        $lista
                ->setCampos(array('mundescricao', 'spccomplanoS', 'spccomplanoN', 'spcpagapisoS', 'spcpagapisoN', 'spccumprejornadaS', 'spccumprejornadaN', 'stmobservacoes'))
                ->esconderColunas(array('muncod', 'marid', 'esdid', 'stmid'))
                ->addCallbackDeCampo('mundescricao', 'retornaColunaMundescricaoResConsolidados')
                ->addCallbackDeCampo('spccomplanos', 'retornaXTrue')
                ->addCallbackDeCampo('spcpagapisos', 'retornaXTrue')
                ->addCallbackDeCampo('spccumprejornadas', 'retornaXTrue')
                ->addCallbackDeCampo('spccomplanon', 'retornaXFalse')
                ->addCallbackDeCampo('spcpagapison', 'retornaXFalse')
                ->addCallbackDeCampo('spccumprejornadan', 'retornaXFalse');

        if ($perCampos) {
            $lista
                    ->addCallbackDeCampo('stmobservacoes', 'retornaColunaObservacaoResConsolidados');
        }

        $lista
                ->setLarguraColuna(array('mundescricao' => '20%', 'spccomplanoS' => '13%', 'spccomplanoN' => '13%', 'spcpagapisoS' => '13%', 'spcpagapisoN' => '13%', 'spccumprejornadaS' => '13%', 'spccumprejornadaN' => '13%', 'stmobservacoes' => '28%'));


        $lista
                ->setFormOff();

        return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    /**
     * Carrega a lista de situações cadastradas
     *
     * @param string $tipo:
     *      A - Padrão, carrega com a classe Simec_Listagem;
     *      B - Carrega utilizando a função monta_lista, da global $db;
     *
     * A ação de edição da lista utiliza a função 'editSituacao';
     */
    public function getLista($tipo = 'A') {
        if ($_REQUEST['subiddial']) {
            $where .= " AND subiddial = '{$_REQUEST['subiddial']}'";
        }
        if (($_REQUEST['legfxa1'] != null) && $_REQUEST['legfxa1']>= 0) {
            $where .= " AND legfxa1 = '{$_REQUEST['legfxa1']}'";
        }
        if (($_REQUEST['legfxa2'] != null) && $_REQUEST['legfxa2']>= 0) {
            $where .= " AND legfxa2 = '{$_REQUEST['legfxa2']}'";
        }

        switch ($tipo) {
            case 'B':
                global $db;
                $sql = <<<DML
                    select '<a style=\"cursor:pointer\" onclick=\"editLegenda( ' || legid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>'
                            || '&nbsp;<a style=\"cursor:pointer\" onclick=\"excluirLegenda( ' || legid || ' )\"><img src=\"/imagens/excluir.gif\"/></a>' as acao,
                        met.metchamada,
                        subiddial||'-'||subtitulo,
                        legfxa1,
                        legfxa2,
                        '<span style="background: ' || legcor || '" class="elementoCor">&nbsp;&nbsp;&nbsp;</span> ('|| upper(legcor)  ||')' as legcor
                    from sase.legendaindicadores leg
                      INNER JOIN sase.submetadial sbd USING(subiddial)
                      INNER JOIN sase.meta met USING(metid)
                    where 1=1 {$where}
                    order by legid
DML;
                $cabecalho = array('Ação', 'Meta', 'Indicador', 'Faixa 1', 'Faixa 2', 'Cor');
                $alinhamento = array('center', 'left', 'center', 'center', 'center');
                $db->monta_lista($sql, $cabecalho, 30, 5, 'N', '', 'N', 'listaLegenda', $larguras, $alinhamento);
                break;

            case 'A':
            default:
                $sql = <<<DML
                    select
                        legid,
                        subiddial,
                        legfxa1,
                        legfxa2,
                        '<span style="background: ' || legcor || '" class="elementoCor">&nbsp;&nbsp;&nbsp;</span>' as legcor
                    from sase.legendaindicadores leg
                    where 1=1 {$where}
                    order by legid
DML;
                $list = new Simec_Listagem();
                $list->setQuery($sql)
                        ->setCabecalho(array('Indicador', 'Faixa 1', 'Faixa 2', 'Cor'))
                        ->addAcao('edit', array(
                            'func' => 'editSituacao',
                            'extra-params' => array(
                                'spcid'
                            )
                        ))
                        ->setCampos(array('subiddial', 'legfxa1', 'legfxa2', 'legcor'))
                        ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                break;
        }
    }

}
