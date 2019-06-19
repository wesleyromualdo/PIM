<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 26/08/2015
 * Time: 10:23
 */

require_once APPRAIZ . "includes/library/simec/Listagem.php";

class Sitplancarprofessor extends Modelo {
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.sitplancarprofessor";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('spcid');//,'muncod','stacod','docid');

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'spcid' => null,
        'spcdsc' => null,
        'spccomplano' => null,
        'spcpagapiso' => null,
        'spccumprejornada' => null,
        'spccor' => null,
        'spcstatus' => null,
        'esdid' => null
    );

    /**
     * Campos da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arCampos = array(
        'spcid',
        'spcdsc',
        'spccomplano',
        'spcpagapiso',
        'spccumprejornada',
        'spccor',
        'spcstatus',
        'esdid'
    );

    /**
     * Atributos da Tabela obrigatórios
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
        'spcid',
        'spcdsc',
        'spccomplano',
        'spcpagapiso',
        'spccumprejornada',
        'spccor',
        'spcstatus',
        'esdid'
    );

    protected $atributosObrigatoriosString = array();

    /**
     * Atributos Integer da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosInt = array(
        'spcid' => null,
        'esdid' => null
    );

    public $msg;
    protected $ravid;
    protected $ratdata1periodo;
    protected $ratdata2periodo;

    public function __construct($id = null, $ravid = null, $ratdata1periodo = null, $ratdata2periodo = null){
        parent::__construct($id);
        $this->ravid = $ravid;
        $this->ratdata1periodo = $ratdata1periodo;
        $this->ratdata2periodo = $ratdata2periodo;
    }

    public function setRavid($ravid){
        $this->ravid = $ravid;
    }

    public function montaSqlRelatorio(){
        $ravid = $this->ravid;
        $ratdata1periodo = $this->ratdata1periodo;
        $ratdata2periodo = $this->ratdata2periodo;
        $esdid = ESDID_SASE_SEM_INFORMACAO_PLANCAR;

        if(isset($this->ravid)) {
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

    public function getTabelaRelatorio($perCampos = null){
        //if ( !empty($ravid) || is_numeric($ravid)){
            $sql = $this->montaSqlRelatorio();

            $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
            $lista
                ->setQuery($sql)
                ->setCabecalho(
                    array(
                        'Municípios'=>'Situação',
                        'Sem Informação',
                        'Com Plano de Carreira'=>array('Sim','Não'),
                        'Paga o Piso (Lei 11.738/2008)'=>array('Sim','Não'),
                        'Cumpre Jornada (Lei 11.783/2008)'=>array('Sim','Não'),
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

            if($perCampos) {
                $lista
                    ->addCallbackDeCampo('stmobservacoes', 'retornaColunaObservacaoResConsolidados');
            }

            $lista
                ->setLarguraColuna(array('mundescricao' => '20%', 'spccomplanoS' => '13%', 'spccomplanoN' => '13%', 'spcpagapisoS' => '13%', 'spcpagapisoN' => '13%', 'spccumprejornadaS' => '13%', 'spccumprejornadaN' => '13%', 'stmobservacoes' => '28%'));


            $lista
                ->setFormOff();

            return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        /*}else{
            $lista = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
            $lista
                ->setDados($dados)
                ->setCabecalho(
                    array(
                        'Municípios'=>'Situação',
                        'Sem Informação',
                        'Com Plano de Carreira'=>array('Sim','Não'),
                        'Paga o Piso (Lei 11.738/2008)'=>array('Sim','Não'),
                        'Cumpre Jornada (Lei 11.783/2008)'=>array('Sim','Não'),
                        'Observação Quanto a meta 18 (PNE)'
                    )
                )
                ->setCampos(array('mundescricao', 'spccomplanoS', 'spccomplanoN', 'spcpagapisoS', 'spcpagapisoN', 'spccumprejornadaS', 'spccumprejornadaN', 'stmobservacoes'))
                ->setLarguraColuna(array('mundescricao' => '20%', 'spccomplanoS' => '13%', 'spccomplanoN' => '13%', 'spcpagapisoS' => '13%', 'spcpagapisoN' => '13%', 'spccumprejornadaS' => '13%', 'spccumprejornadaN' => '13%', 'stmobservacoes' => '28%'))
                ->setFormOff();
            return $lista->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }*/
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
    public function getLista($tipo = 'A')
    {
        switch($tipo){
            case 'B':
                global $db;
                $sql = <<<DML
                    select
                        '<a style=\"cursor:pointer\" onclick=\"editSituacao( ' || spcid || ' )\"><img src=\"/imagens/alterar.gif\"/></a>' as acao,
                        spcdsc,
                        '<span style="background: ' || spccor || '" class="elementoCor">&nbsp;&nbsp;&nbsp;</span>' as spccor
                    from sase.sitplancarprofessor spc
                    order by spcid
DML;
                $cabecalho   = array('Ação', 'Situação', 'Cor');
                $alinhamento = array('center', 'left', 'center');
                $larguras    = array('5%', '90%', '5%');
                $db->monta_lista($sql,$cabecalho,30, 5,'N','','N','listaSituacao',$larguras,$alinhamento);
                break;

            case 'A':
            default:
                $sql = <<<DML
                    select
                        spcid,
                        spcdsc,
                        '<span style="background: ' || spccor || '" class="elementoCor">&nbsp;&nbsp;&nbsp;</span>' as spccor
                    from sase.sitplancarprofessor spc
                    order by spcid
DML;
                $list = new Simec_Listagem();
                $list->setQuery($sql)
                    ->setCabecalho(array('Situação', 'Cor'))
                    ->addAcao('edit', array(
                        'func' => 'editSituacao',
                        'extra-params' => array(
                            'spcid'
                        )
                    ))
                    ->setCampos(array('spcdsc', 'spccor'))
                    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                break;
        }
    }
    //legenda plano carreira município externo --> DIVAPE sase_mapas.php
    public function montaLegendaMapaExterno(Array $estuf){
        global $db;
        $whereEstuf = "'".implode("','", $estuf)."'";
            switch ($_SESSION['tipo']){
                case '238':
                $sql = <<<DML
                    with temp_doc as (select
                                pcp.pcpid,
                                case
                                    when pcp.docid is null then (select spcid from sase.sitplancarprofessor where spcid = 1)
                                    else spc.spcid
                                end as spcid
                            from sase.planocarreiraprofessor pcp
                            left join workflow.documento doc on pcp.docid = doc.docid
                            left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
                            WHERE pcp.estuf in ({$whereEstuf})
                            order by pcpid)
                    select
                        spc.spcid,
                        spc.spcdsc,
                        spc.spccor,
                        spc.esdid,
                        (select count(*) from temp_doc where spcid = spc.spcid) as total
                    from sase.sitplancarprofessor spc
                    group by spc.spcid, spc.spcdsc, spc.spccor
                    order by spc.spcid
DML;
                            
                break;
            case '240':
                $sql = <<<DML
                    with temp_doc as (select
                                pcp.pcpid,
                                case
                                    when pcp.docid2 is null then (select spmid from sase.sitplanomunicipio where spmid = 1)
                                    else spm.spmid
                                end as spmid
                            from sase.planocarreiraprofessor pcp
                            left join workflow.documento doc on pcp.docid2 = doc.docid
                            left join sase.sitplanomunicipio spm on spm.esdid = doc.esdid
                            WHERE pcp.estuf in ({$whereEstuf})
                            order by pcpid)
                    select
                        spm.spmid as spcid,
                        spm.spmdsc as spcdsc,
                        spm.spccor,
                        spm.esdid,
                        (select count(*) from temp_doc where spmid = spm.spmid) as total
                    from sase.sitplanomunicipio spm
                    group by spm.spmid, spm.spmdsc, spm.spccor
                    order by spm.spmid
DML;

                break;
            case '242':
                $sql = <<<DML
                    SELECT spccor, spcdsc, count(spccor) as total
                    FROM (
                        SELECT 
                            CASE
                                WHEN pcppcr IS NOT NULL THEN '#006600'
                                WHEN pcppcr IS NULL THEN '#FF0000'
                            END as spccor,
                            CASE
                                WHEN pcppcr IS NOT NULL THEN 'Com PCR'
                                WHEN pcppcr IS NULL THEN 'Sem PCR'
                            END as spcdsc
                        FROM sase.planocarreiraprofessor pcp
                        WHERE pcp.estuf in ({$whereEstuf})
                    ) t
                    GROUP BY spcdsc,spccor
DML;

                break;
            case '244':
                $sql = <<<DML
                    SELECT spccor, spcdsc, count(spccor) as total
                    FROM (
                        SELECT 
                            CASE
                                WHEN pcpadesao IS NOT NULL THEN '#006600'
                                WHEN pcpadesao IS NULL THEN '#FF0000'
                            END as spccor,
                            CASE
                                WHEN pcpadesao IS NOT NULL THEN 'Com Termo de Adesão'
                                WHEN pcpadesao IS NULL THEN 'Sem Termo de Adesão'
                            END as spcdsc
                        FROM sase.planocarreiraprofessor pcp
                        WHERE pcp.estuf in ({$whereEstuf})
                    ) t
                    GROUP BY spcdsc,spccor
DML;

                break;
            }  
//            ver($sql,d);
        $res = $db->carregar($sql);
        $htmlLi = "";
        $total = 0;
        if($res){
        foreach ($res as $r) {
            $total = $r['total'] + $r['totalsemregistro'];
            $htmlLi .= <<<HTML
                <li>
                    <table>
                        <tr>
                            <td><span style="background-color: {$r['spccor']};" class="elementoCor">&nbsp;&nbsp</span>&nbsp;&nbsp;<b>{$total}</b>&nbsp;&nbsp;</td>
                            <td class="dsc">{$r['spcdsc']}</td>
                        </tr>
                    </table>
                </li>
HTML;
        }
        }
        $html = <<<HTML
		<div id="chamadaLegenda" style="display:none"><a onclick="jQuery('#legendaConteudo').css({display:'block'});jQuery('#chamadaLegenda').css({display:'none'})" title="Mostrar Legenda"><img src="../imagens/sase/menu.png" style="width:15px;margin-top:4px;" /></a></div>
		<div id="legendaConteudo" style="display:block;width:150px;" class="topo_bootstrap_off">
			<div id="tituloLegenda">
				<div style="float:left;"><h5>&nbsp;Legenda:</h5></div>
				<div style="float:right;"><a onclick="jQuery('#legendaConteudo').css({display:'none'});jQuery('#chamadaLegenda').css({display:'block'})" title="Fechar Legenda"><img style="width:19px;"  src="../imagens/sase/sair.png" /></a></div>
				<div style="clear:both;height:1px;">&nbsp;</div>
			</div>
			<ul>
			    {$htmlLi}
			</ul>
			<div style="font-size:10px;font-color:;padding-left:10px;">É possível <b>arrastar</b> e fazer <b>zoom</b>.</div>
		</div>
HTML;
        echo $html;
    }
}