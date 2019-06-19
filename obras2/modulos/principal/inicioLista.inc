<?php

if ($_POST['requisicao'] && $_POST['requisicao'] == 'alerta') {
    if ($_SESSION['usucpforigem'] == $_SESSION['usucpf']) {
        $dados = $_POST;
        $_POST = array();
        $alertaObraLido = new AlertaObraLido();
        $ret = $alertaObraLido->salvarAlertaLido($dados, 'C');
        $alertaObra = new AlertaObra();
        $alertaObra->salvarAlerta($dados);
        $alerta = new Alerta();
        $alerta->salvarAcao($dados);
        $_POST = array();

        echo '<script>alert("Salvo com sucesso!"); window.location.href = window.location.href;</script>';
        die;
    } else {
        echo '<script>alert("No  possivel salvar ao simular um usurio!"); window.location.href = window.location.href;</script>';
        die;
    }
}

include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "obras2/classes/modelo/Avisoobras.inc";
include_once APPRAIZ . "obras2/classes/modelo/Anexoaviso.inc";


// O 'orgid'  necessrio para que a tela da obras seja carregada corretamente.
$arOrgid = verificaAcessoEmOrgid();
if (!in_array($_SESSION['obras2']['orgid'], $arOrgid)) {
    $_SESSION['obras2']['orgid'] = current($arOrgid);
}

define("PENDENCIA_BLOQUEIO_PAR", 1); // Bloqueio do PAR.
define("PENDENCIA_BLOQUEIO_RECURSOS", 2); // Bloqueio de recursos.
define("PENDENCIA_NOVOS_DESEMBOLSOS", 3); // Novos desembolsos.
define("PENDENCIA_PREENCHIMENTO", 4); // Preenchimento.
define("PENDENCIA_SOLICITACOES_DILIGENCIADAS", 5); // Solicitações diligenciadas.
define("PENDENCIA_OBRAS_TERMOS_A_VENCER", 6); // Obras com o termo prestes a vencer.
define("PENDENCIA_OBRAS_INACABADAS", 7); // Obras Inacabadas.

$avisos = new Obras2_Modelo_Avisoobras($avsoid);
$anexoAviso = new Obras2_Modelo_Anexoaviso();

if (($_POST['requisicao']) && ($_POST['requisicao'] == 'aviso_cumprimento')) {
    $cumprimentoObjeto = new CumprimentoObjeto();
    $cumprimentoObjeto->marcarAlertaLido($_POST['coid'], $_POST['obrid'], $_POST['obrnome']);
}

if ($_GET['download_cgu']) {
    $stop_date = new DateTime();
    $stop_date->modify('-1 month');

    $dir = APPRAIZ . 'arquivos/obras2/cgu/';
    $extension = ".csv";

    if ($_GET['nomerelatoriomanual']) {
        $nome = $_GET['nomerelatoriomanual'];
    } else {
        $nome = ($_GET['download_cgu'] == 1) ? "SIMEC_RELATORIO_CGU_" . $stop_date->format('Y-m') . $extension : "SIMEC_RELATORIO_CGU_SUPERIOR_" . $stop_date->format('Y-m') . $extension;
    }

    $arquivo = $dir . $nome;

    if (file_exists($arquivo)) {
        ob_clean();
        header('Content-type: text/csv');
        header('Content-Disposition: attachment; filename=' . $nome);
        readfile($arquivo);
    }
    die();
}

if ($_GET['download']) {
    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid($_GET['download']);
    $schema = 'obras2';

    $file = new FilesSimec(null, null, $schema);
    $file->getDownloadArquivo($_GET['download']);

    die('<script type="text/javascript">window.close();</script>');
}

if ($_GET['arquivo']) {
    include_once APPRAIZ . "includes/classes/file.class.inc";
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    ob_clean();
    $file = new FilesSimec();
    $file->getDownloadArquivo($_GET['arquivo']);
    exit;
}

/**
 * Método responsvel por pesquisar a estrutura do formulrio MI.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @return array|mixed|NULL
 */
function pegaEstruturaFormularioMI()
{

    global $db;

    $sql = "select * from obras2.tecnologiami";

    $tecnologias = $db->carregar($sql);
    foreach ($tecnologias as $key => $tecnologia) {
        $sql = "select * from obras2.formulariotecnologiami where ftistatus = 'A' and tmiid = " . $tecnologia['tmiid'];
        $questionario = $db->carregar($sql);
        $tecnologias[$key]['formulario'] = $questionario;
    }
    return $tecnologias;
}

/**
 * Método responsvel por pesquisar as obras do usurio.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @return array|mixed|NULL
 */
function pegaListaObras()
{

    global $db;

    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

        $arrObr = array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR);

        $arrUni = array(PFLCOD_GESTOR_UNIDADE);

        $arrOrg = array(
            PFLCOD_ADMINISTRADOR,
            PFLCOD_CADASTRADOR_INSTITUCIONAL,
            PFLCOD_CONSULTA_TIPO_DE_ENSINO,
            PFLCOD_SUPERVISOR_MEC,
            PFLCOD_GESTOR_MEC
        );

        $arrEst = array(PFLCOD_CONSULTA_ESTADUAL);

        $arPflcod = array();

        $orWhere = array();

        if (possui_perfil($arrEst)) {
            $arPflcod = array_merge($arPflcod, $arrEst);
            $orWhere['estuf'] = "mun.estuf IN (SELECT estuf FROM usuarioresponsabilidade urs WHERE
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    urs.estuf = edr.estuf)";
        }

        if (possui_perfil($arrObr)) {
            $arPflcod = array_merge($arPflcod, $arrObr);
            $orWhere['empid'] = "e.empid IN (SELECT urs.empid FROM usuarioresponsabilidade urs WHERE
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    urs.empid = e.empid)";
        }

        if (possui_perfil($arrOrg)) {
            $arPflcod = array_merge($arPflcod, $arrOrg);
            $orWhere['orgid'] = "e.orgid IN ( SELECT urs.orgid FROM usuarioresponsabilidade urs WHERE
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    urs.orgid = e.orgid)";
        }

        if (possui_perfil($arrUni)) {
            $arPflcod = array_merge($arPflcod, $arrUni);
            $orWhere['entidunidade'] = "e.entidunidade IN ( SELECT urs.entid FROM usuarioresponsabilidade urs WHERE
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    urs.entid = e.entidunidade)";
        }

        if (possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {

            $usuarioResp = new UsuarioResponsabilidade();

            $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
            $arEmpid = ($arEmpid ? $arEmpid : array(0));

            $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
            $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

            //Foi adicionado o empid na session para validao do => verificaSessao() nas prximas abas.
            $_SESSION['obras2']['empid'] = $arEmpid;

            $orWhere['sup'] = "e.empid IN ( SELECT urs.empid FROM usuarioresponsabilidade urs WHERE
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '",
                    $arEmpid) . "'))";
        }
        $orderSuperUsu = 'ASC';
        $whereSuperUsu = "";
    }

    if (verificaPerfisEspeciais()) {
        $orderSuperUsu = 'DESC';
        $whereSuperUsu = "WHERE  f.dtvistoria IS NOT NULL";
    }

    $orWhere = (is_array($orWhere) ? $orWhere : array());
    $sql = "WITH  usuarioresponsabilidade AS (
            SELECT entid, pflcod, estuf, orgid, empid FROM obras2.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}'
            )

            SELECT * FROM (
                SELECT DISTINCT ON (o.obrid)
                
                    -- Obra com vistoria
                    CASE WHEN o.obrdtultvistoria IS NOT NULL THEN
                        o.obrdtultvistoria

                    -- Obra MI sem vistoria usar a data do aceite
                    WHEN o.obrdtultvistoria IS NULL AND o.tpoid IN (104, 105) THEN
                        (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1) 

                    -- Obra convencional sem vistoria usar a data do inicio da execução
                    WHEN o.obrdtultvistoria IS NULL THEN
                        (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690))

                    ELSE 
                        o.obrdtultvistoria

                    END as dtvistoria,

                    o.obrid,
                    o.preid,
                    ''||p_conv.pronumeroprocesso||'' as processo,
                    p_conv.termo_convenio || ' / ' || p_conv.ano_termo_convenio as n_termo_ano,

                CASE WHEN e.prfid = 42 OR o.tooid = 4 THEN
                    '<font COLOR=\"green\" style=\"font-weight:bold\">(EP) </font>'
                ELSE
                    ''
                END || '(' || o.obrid || ') ' || o.obrnome || '' as obrnome,

                uni.entnome,
                mun.mundescricao || ' - ' || mun.estuf as municipio_uf,

                CASE WHEN o.tpoid IN (104, 105) THEN
                  (SELECT TO_CHAR(osmdtinicio, 'dd/mm/YYYY') FROM obras2.ordemservicomi WHERE obrid = o.obrid AND osmstatus = 'A' AND tomid = 1 ORDER BY osmid DESC LIMIT 1)
                ELSE TO_CHAR(oc.ocrdtinicioexecucao, 'dd/mm/YYYY')
                END AS inicio,
                
                str.strid,
                str.strdsc AS situacao,

                -- Obra concluida aplicar cor azul
                CASE 
                   
                    -- Obra em execução ou paralisada com vistoria
                    WHEN o.obrdtultvistoria IS NOT NULL AND ed.esdid IN (690, 691) THEN
                        DATE_PART('days', NOW() - obrdtultvistoria)

                    -- Obra MI em execução ou paralisada sem vistoria usar a data do aceite
                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) AND o.tpoid IN (104, 105) THEN
                        DATE_PART('days', NOW() - (SELECT os.osmdtinicio FROM obras2.obras obr JOIN obras2.ordemservicomi os ON os.obrid = obr.obrid WHERE obr.obrid = o.obrid AND os.tomid = 1 AND os.osmstatus = 'A' ORDER BY os.osmid DESC LIMIT 1))

                    -- Obra convencional em execução ou paralisada sem vistoria usar a data do inicio da execução
                    WHEN o.obrdtultvistoria IS NULL AND ed.esdid IN (690, 691) THEN
                        DATE_PART('days', NOW() - (SELECT MIN(htddata) FROM workflow.historicodocumento WHERE docid = d.docid AND aedid IN (SELECT aedid FROM workflow.acaoestadodoc WHERE esdiddestino = 690)))

                    ELSE
                        DATE_PART('days', NOW() - obrdtultvistoria)

                END as ultima_atualizacao,

                CASE WHEN o.obrpercentultvistoria > '99.95' THEN
                      ROUND(o.obrpercentultvistoria) || '%'
                    ELSE
                        o.obrpercentultvistoria || '%'
                END AS obrpercentultvistoria,

                tpo.tpodsc

            FROM obras2.obras o
            LEFT JOIN obras2.empreendimento e ON e.empid = o.empid
            LEFT JOIN entidade.endereco edr ON edr.endid = o.endid
            LEFT JOIN territorios.municipio mun ON mun.muncod = edr.muncod
            LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
            LEFT JOIN obras2.obrascontrato oc ON oc.ocrid = ocr.ocrid
            LEFT JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            LEFT JOIN obras2.tipologiaobra tpo ON tpo.tpoid = o.tpoid
            LEFT JOIN entidade.entidade uni ON uni.entid = e.entidunidade
            LEFT JOIN workflow.documento d ON d.docid = o.docid
            LEFT JOIN workflow.estadodocumento ed ON ed.esdid   = d.esdid
            LEFT JOIN obras2.situacao_registro_documento srd ON srd.esdid = d.esdid
            LEFT JOIN obras2.situacao_registro str ON str.strid = srd.strid
            -- Dados do Processo e Termo
            LEFT JOIN obras2.v_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid

            WHERE
                o.obridpai IS NULL
            AND  
                o.obrstatus = 'A'
           
            " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "

            ) as f
            
            /* Somente se for super usurio, pois ir exibir as obras mais atuais e  necessrio que exista a data da ultima vistoria*/
            {$whereSuperUsu}
            
            ORDER BY
                CASE
                  WHEN strid = 4 OR strid = 5 THEN 1 ELSE 2
                END,
                f.dtvistoria {$orderSuperUsu}
            LIMIT 5
    ";

    $lista = $db->carregar($sql);
    return $lista;
}

/**
 * Método responsvel por retornar um array com tipos de pendência que ser utilizado na montagem da tabela.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10779
 */
function getTiposPendencia()
{
    return array(
        PENDENCIA_BLOQUEIO_PAR,
        PENDENCIA_BLOQUEIO_RECURSOS,
        PENDENCIA_NOVOS_DESEMBOLSOS,
        PENDENCIA_PREENCHIMENTO,
        PENDENCIA_SOLICITACOES_DILIGENCIADAS,
        PENDENCIA_OBRAS_TERMOS_A_VENCER,
        PENDENCIA_OBRAS_INACABADAS
    );
}

/**
 * Método responsvel por retornar as pendências por tipo. Para os perfis especiais ()  pesquisada apenas a quantidade
 * de obras inseridas na categoria de pendência.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10779
 * @param $tipo Tipo de pendência.
 * @param $cpf CPF do usurio.
 * @return array|bool|mixed|NULL|string
 */
function getPendenciasPorTipo($tipo, $cpf)
{

    $arrPendencias = array();
    $flgPerfisEspeciais = verificaPerfisEspeciais();

    $arrListaObrids = getObridsUsuario();

    switch ($tipo) {
        case PENDENCIA_BLOQUEIO_PAR:
            $arrPendencias["nomeTipoPendencia"] = "Bloqueio PAR";
            $arrPendencias["dscTipoPendencia"] = "Obras com problemas que impedem o FNDE de realizar análise de novas demandas e de efetivar novos Termos de Compromisso com sua entidade.";
            //$arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesBloqueioPar() : getObrasPendentesPARPorCpf($cpf);
            $arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesBloqueioPar() : getObrasPendentesParPorID($arrListaObrids);
            break;

        case PENDENCIA_BLOQUEIO_RECURSOS:
            $arrPendencias["nomeTipoPendencia"] = "Bloqueio de Recursos";
            $arrPendencias["dscTipoPendencia"] = "Obras com problemas que impedem o FNDE de efetuar repasses dos recursos pactuados para QUAISQUER obras.";
            //$arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesBloqueioRecursos() : getObrasPendentesRecursosPorCpf($cpf);
            $arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesBloqueioRecursos() : getObrasPendentesBloqueioRecursosPorID($arrListaObrids);
            break;

        case PENDENCIA_NOVOS_DESEMBOLSOS:
            $arrPendencias["nomeTipoPendencia"] = "Novos desembolsos";
            $arrPendencias["dscTipoPendencia"] = "Obras com problemas que impedem o FNDE de efetuar repasses dos recursos pactuados para estas obras.";
            //$arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesNovosDesembolsos() : getObrasPendentesDesenbolsoPorCpf($cpf);
            $arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesNovosDesembolsos() : getObrasPendentesDesembolsoPorID($arrListaObrids);
            break;

        case PENDENCIA_PREENCHIMENTO:
            $arrPendencias["nomeTipoPendencia"] = "Preenchimento";
            $arrPendencias["dscTipoPendencia"] = "Obras com problemas nas informações prestadas no sistema.";
            //$arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesPreenchimento() : getObrasPendentesPreenchimentoPorCpf($cpf);
            $arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesPreenchimento() : getObrasPendentesPreenchimentoPorID($arrListaObrids);
            break;

        case PENDENCIA_SOLICITACOES_DILIGENCIADAS:
            $arrPendencias["nomeTipoPendencia"] = "Solicitações diligenciadas";
            $arrPendencias["dscTipoPendencia"] = "Obras com Solicitações em diligência.";
            $arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesDiligenciadas() : getObrasPendentesDiligenciaPorID($arrListaObrids);
            break;

        case PENDENCIA_OBRAS_TERMOS_A_VENCER:
            $arrPendencias["nomeTipoPendencia"] = "Obras com o Termos prestes a vencer";
            $arrPendencias["dscTipoPendencia"] = "O município possui obras cujo termo vence nos prximos 60 dias. Para prorrogar, acessar o MDULO - PAR no perfil do Prefeito Municipal.";
            //$arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesTermoVencendo() : getObrasVencendoPorCpf($cpf);
            $arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesTermoVencendo() : getObrasPendentesTermosVencendoPorID($arrListaObrids);
            break;

        case PENDENCIA_OBRAS_INACABADAS:
            $arrPendencias["nomeTipoPendencia"] = "Obras Inacabadas";
            $arrPendencias["dscTipoPendencia"] = "O município possui obras com status de INACABADA. Caso estas obras estejam concludas, o município deve inserir vistoria com esta Situação.";
            //$arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesInacabadas() : getObrasInacabadasPorCpf($cpf);
            $arrPendencias["pendencias"] = $flgPerfisEspeciais ? getQtdObrasPendentesInacabadas() : getObrasPendentesInacabadasPorID($arrListaObrids);
            break;
    }

    return $arrPendencias;

}

/**
 * Método responsvel por organizar as informações das obras com pendências.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10779
 * @param $tipoPendencia
 * @param $arrPendencia
 * @return array
 */
function getInfoObraPendencia($tipoPendencia, $arrPendencia)
{

    $pendenciaInfo = array();

    if (is_array($arrPendencia) && !empty($arrPendencia)) {

        switch ($tipoPendencia) {
            case PENDENCIA_BLOQUEIO_PAR:
                $pendenciaInfo["obrid"] = $arrPendencia["obrid"];
                $pendenciaInfo["estuf"] = $arrPendencia["estuf"];
                $pendenciaInfo["descricao"] = $arrPendencia["obrnome"];
                $pendenciaInfo["municipio"] = $arrPendencia["mundescricao"];
                $pendenciaInfo["execucao"] = $arrPendencia["obrpercentultvistoria"];
                $pendenciaInfo["situacao"] = $arrPendencia["pendencia"];
                $pendenciaInfo["url"] = "/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=" . $arrPendencia["obrid"];
                break;

            case PENDENCIA_BLOQUEIO_RECURSOS:
                $pendenciaInfo["obrid"] = $arrPendencia["obrid"];
                $pendenciaInfo["estuf"] = $arrPendencia["estuf"];
                $pendenciaInfo["descricao"] = $arrPendencia["predescricao"];
                $pendenciaInfo["municipio"] = $arrPendencia["descricao"];
                $pendenciaInfo["execucao"] = $arrPendencia["obrpercentultvistoria"];
                $pendenciaInfo["situacao"] = $arrPendencia["situacao"];
                $pendenciaInfo["url"] = "/obras2/obras2.php?modulo=principal/vistoria&acao=A&obrid=" . $arrPendencia["obrid"];;
                break;

            case PENDENCIA_NOVOS_DESEMBOLSOS:
                $pendenciaInfo["obrid"] = $arrPendencia["obrid"];
                $pendenciaInfo["estuf"] = $arrPendencia["estuf"];
                $pendenciaInfo["descricao"] = $arrPendencia["predescricao"];
                $pendenciaInfo["municipio"] = $arrPendencia["descricao"];
                $pendenciaInfo["execucao"] = $arrPendencia["obrpercentultvistoria"];
                $pendenciaInfo["situacao"] = $arrPendencia["situacao"];
                $pendenciaInfo["url"] = "/obras2/obras2.php?modulo=principal/listaRestricao&acao=O&obrid=" . $arrPendencia["obrid"];
                break;

            case PENDENCIA_PREENCHIMENTO:
                $pendenciaInfo["obrid"] = $arrPendencia["obrid"];
                $pendenciaInfo["estuf"] = $arrPendencia["estuf"];
                $pendenciaInfo["descricao"] = $arrPendencia["predescricao"];
                $pendenciaInfo["municipio"] = $arrPendencia["descricao"];
                $pendenciaInfo["execucao"] = $arrPendencia["obrpercentultvistoria"];
                $pendenciaInfo["situacao"] = $arrPendencia["situacao"];
                $pendenciaInfo["url"] = "/obras2/obras2.php?modulo=principal/listaRestricao&acao=O&obrid=" . $arrPendencia["obrid"];
                break;

            case PENDENCIA_SOLICITACOES_DILIGENCIADAS:
                $pendenciaInfo["obrid"] = $arrPendencia["obrid"];
                $pendenciaInfo["estuf"] = $arrPendencia["estuf"];
                $pendenciaInfo["descricao"] = $arrPendencia["obrnome"];
                $pendenciaInfo["municipio"] = $arrPendencia["mundescricao"];
                $pendenciaInfo["execucao"] = $arrPendencia["obrpercentultvistoria"];
                $pendenciaInfo["situacao"] = $arrPendencia["situacao"];
                $pendenciaInfo["url"] = "/obras2/obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A&obrid=" . $arrPendencia["obrid"];
                break;

            case PENDENCIA_OBRAS_TERMOS_A_VENCER:
                $pendenciaInfo["obrid"] = $arrPendencia["obrid"];
                $pendenciaInfo["estuf"] = $arrPendencia["estuf"];
                $pendenciaInfo["descricao"] = $arrPendencia["obrnome"];
                $pendenciaInfo["municipio"] = $arrPendencia["mundescricao"];
                /* Para este tipo de pendência, no h informações de 'execução' e 'Situação'. Para reaproveitar o cdigo,
                 * colocou-se nas respectivas posies destes itens as informações de 'nmero do termo' e 'data fim da
                 * vigncia'.
                 */
                $pendenciaInfo["execucao"] = $arrPendencia["numero_termo"];
                $pendenciaInfo["situacao"] = $arrPendencia["dt_fim_vigencia_termo"];

                if ($arrPendencia["fonte"] == "PAR") {
                    $url = "/par/par.php?modulo=principal/documentoParObras&acao=A";
                } else {
                    $url = "/par/par.php?modulo=principal/termoPac&acao=A";
                }

                $pendenciaInfo["url"] = $url;
                break;

            case PENDENCIA_OBRAS_INACABADAS:
                $pendenciaInfo["obrid"] = $arrPendencia["obrid"];
                $pendenciaInfo["estuf"] = $arrPendencia["estuf"];
                $pendenciaInfo["descricao"] = $arrPendencia["obrnome"];
                $pendenciaInfo["municipio"] = $arrPendencia["mundescricao"];
                $pendenciaInfo["execucao"] = null;
                $pendenciaInfo["situacao"] = null;
                $pendenciaInfo["url"] = "/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=" . $arrPendencia["obrid"];
                break;
        }
    }

    return $pendenciaInfo;
}

/**
 * Método responsvel por verificar se existem pendências em obras ligadas ao usurio logado no sistema.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10779
 * @return bool
 */
function verificaPendenciaExiste()
{

    $arrTiposPendencia = getTiposPendencia();
    $pendenciaExists = false;

    $arrTiposPendencia = (is_array($arrTiposPendencia)) ? $arrTiposPendencia : Array();

    foreach ($arrTiposPendencia as $tipoPendencia) {

        if ($pendenciaExists) {
            break;
        }

        $arrPendencias = getPendenciasPorTipo($tipoPendencia, $_SESSION['usucpf']);

        if (!empty($arrPendencias["pendencias"])) {
            $pendenciaExists = true;
        }
    }

    return $pendenciaExists;
}

/**
 * Método responsvel por verificar se o usurio logado faz parte do grupo de perfis especiais.
 * Para este grupo de usurios, a tabela de pendências ser montada de maneira diferente.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10779
 * @return bool
 */
function verificaPerfisEspeciais()
{
    $arrPerfisEspeciais = array(
        PFLCOD_ADMINISTRADOR,
        PFLCOD_SUPER_USUARIO,
        PFLCOD_CGIMP_GESTOR,
        PFLCOD_CGIMP_TECNICO,
        PFLCOD_GESTOR_MEC,
        PFLCOD_SUPERVISOR_MEC
    );
    return possui_perfil($arrPerfisEspeciais);
}

/**
 * Método responsvel por obter os IDs de todas as obras ligadas ao usurio.
 * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10779
 * @return array
 */
function getObridsUsuario()
{

    global $db;

    if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

        $arrObr = array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR);

        $arrUni = array(PFLCOD_GESTOR_UNIDADE);

        $arrOrg = array(
            PFLCOD_ADMINISTRADOR,
            PFLCOD_CADASTRADOR_INSTITUCIONAL,
            PFLCOD_CONSULTA_TIPO_DE_ENSINO,
            PFLCOD_SUPERVISOR_MEC,
            PFLCOD_GESTOR_MEC
        );

        $arrEst = array(PFLCOD_CONSULTA_ESTADUAL);

        $arPflcod = array();
        $orWhere = array();

        if (possui_perfil($arrEst)) {
            $arPflcod = array_merge($arPflcod, $arrEst);
            $orWhere['estuf'] = "mun.estuf IN (SELECT estuf FROM usuarioresponsabilidade urs WHERE
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    urs.estuf = edr.estuf)";
        }

        if (possui_perfil($arrObr)) {
            $arPflcod = array_merge($arPflcod, $arrObr);
            $orWhere['empid'] = "e.empid IN (SELECT urs.empid FROM usuarioresponsabilidade urs WHERE
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    urs.empid = e.empid)";
        }

        if (possui_perfil($arrOrg)) {
            $arPflcod = array_merge($arPflcod, $arrOrg);
            $orWhere['orgid'] = "e.orgid IN ( SELECT urs.orgid FROM usuarioresponsabilidade urs WHERE
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    urs.orgid = e.orgid)";
        }

        if (possui_perfil($arrUni)) {
            $arPflcod = array_merge($arPflcod, $arrUni);
            $orWhere['entidunidade'] = "e.entidunidade IN ( SELECT urs.entid FROM usuarioresponsabilidade urs WHERE
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    urs.entid = e.entidunidade)";
        }

        if (possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {

            $usuarioResp = new UsuarioResponsabilidade();

            $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
            $arEmpid = ($arEmpid ? $arEmpid : array(0));

            $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
            $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

            $orWhere['sup'] = "e.empid IN ( SELECT urs.empid FROM usuarioresponsabilidade urs WHERE
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '",
                    $arEmpid) . "'))";
        }
    }

    $orWhere = (is_array($orWhere) ? $orWhere : array());
    $sql = "WITH  usuarioresponsabilidade AS (
            SELECT entid, pflcod, estuf, orgid, empid FROM obras2.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}'
            )

            SELECT * FROM (
                SELECT DISTINCT ON (o.obrid)
                    o.obrid
            FROM obras2.obras o
            LEFT JOIN obras2.empreendimento e ON e.empid = o.empid
            LEFT JOIN entidade.endereco edr ON edr.endid = o.endid
            LEFT JOIN territorios.municipio mun ON mun.muncod = edr.muncod
            LEFT JOIN (SELECT MAX(oc.ocrid) AS ocrid, obrid FROM obras2.obrascontrato oc WHERE oc.ocrstatus = 'A' GROUP BY oc.obrid) ocr ON ocr.obrid = o.obrid
            LEFT JOIN obras2.obrascontrato oc ON oc.ocrid = ocr.ocrid
            LEFT JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
            LEFT JOIN entidade.entidade uni ON uni.entid = e.entidunidade
            LEFT JOIN workflow.documento d ON d.docid = o.docid
            LEFT JOIN workflow.estadodocumento ed ON ed.esdid   = d.esdid
            LEFT JOIN obras2.situacao_registro_documento srd ON srd.esdid = d.esdid
            LEFT JOIN obras2.situacao_registro str ON str.strid = srd.strid

            WHERE
                o.obridpai IS NULL
            AND  
                o.obrstatus = 'A'
            " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
            ) as f      
    ";

    $lista = $db->carregar($sql);
    $arrObrIds = array();
    $lista = (is_array($lista)) ? $lista : Array();
    foreach ($lista as $a) {
        array_push($arrObrIds, current($a));
    }
    return $arrObrIds;
}

?>

<style>

    /* O arquivo bootstrap.min-simec.css  utilizado no layout antigo. Nesse arquivo esto comentadas as classes que o
     * Bootstrap utiliza para tabelas. Portanto, para utilizar a formatao de tabelas do bootstrap  necessrio o trecho
     * a seguir.
     */
    table {
        border-collapse: collapse;
        border-spacing: 0;
    }

    th {
        text-align: left;
    }

    .table {
        width: 100%;
        margin-bottom: 20px;
    }

    .table thead > tr > th, .table tbody > tr > th, .table tfoot > tr > th, .table thead > tr > td, .table tbody > tr > td, .table tfoot > tr > td {
        padding: 8px;
        line-height: 1.428571429;
        vertical-align: top;
        border-top: 1px solid #dddddd;
    }

    .table thead > tr > th {
        vertical-align: bottom;
        border-bottom: 2px solid #dddddd;
    }

    .table caption + thead tr:first-child th, .table colgroup + thead tr:first-child th, .table thead:first-child tr:first-child th, .table caption + thead tr:first-child td, .table colgroup + thead tr:first-child td, .table thead:first-child tr:first-child td {
        border-top: 0;
    }

    .table tbody + tbody {
        border-top: 2px solid #dddddd;
    }

    .table .table {
        background-color: #ffffff;
    }

    .table-condensed thead > tr > th, .table-condensed tbody > tr > th, .table-condensed tfoot > tr > th, .table-condensed thead > tr > td, .table-condensed tbody > tr > td, .table-condensed tfoot > tr > td {
        padding: 5px;
    }

    .table-bordered {
        border: 1px solid #dddddd;
    }

    .table-bordered > thead > tr > th, .table-bordered > tbody > tr > th, .table-bordered > tfoot > tr > th, .table-bordered > thead > tr > td, .table-bordered > tbody > tr > td, .table-bordered > tfoot > tr > td {
        border: 1px solid #dddddd;
    }

    .table-bordered > thead > tr > th, .table-bordered > thead > tr > td {
        border-bottom-width: 2px;
    }

    .table-striped > tbody > tr:nth-child(odd) > td, .table-striped > tbody > tr:nth-child(odd) > th {
        background-color: #f9f9f9;
    }

    .table-hover > tbody > tr:hover > td, .table-hover > tbody > tr:hover > th {
        background-color: #f5f5f5;
    }

    table col[class*="col-"] {
        float: none;
        display: table-column;
    }

    table td[class*="col-"], table th[class*="col-"] {
        float: none;
        display: table-cell;
    }

    .table > thead > tr > td.active, .table > tbody > tr > td.active, .table > tfoot > tr > td.active, .table > thead > tr > th.active, .table > tbody > tr > th.active, .table > tfoot > tr > th.active, .table > thead > tr.active > td, .table > tbody > tr.active > td, .table > tfoot > tr.active > td, .table > thead > tr.active > th, .table > tbody > tr.active > th, .table > tfoot > tr.active > th {
        background-color: #f5f5f5;
    }

    .table > thead > tr > td.success, .table > tbody > tr > td.success, .table > tfoot > tr > td.success, .table > thead > tr > th.success, .table > tbody > tr > th.success, .table > tfoot > tr > th.success, .table > thead > tr.success > td, .table > tbody > tr.success > td, .table > tfoot > tr.success > td, .table > thead > tr.success > th, .table > tbody > tr.success > th, .table > tfoot > tr.success > th {
        background-color: #468847;
        border-color: #46773d;
    }

    .table-hover > tbody > tr > td.success:hover, .table-hover > tbody > tr > th.success:hover, .table-hover > tbody > tr.success:hover > td {
        background-color: #3d773e;
        border-color: #3c6635;
    }

    .table > thead > tr > td.danger, .table > tbody > tr > td.danger, .table > tfoot > tr > td.danger, .table > thead > tr > th.danger, .table > tbody > tr > th.danger, .table > tfoot > tr > th.danger, .table > thead > tr.danger > td, .table > tbody > tr.danger > td, .table > tfoot > tr.danger > td, .table > thead > tr.danger > th, .table > tbody > tr.danger > th, .table > tfoot > tr.danger > th {
        background-color: #b94a48;
        border-color: #af4353;
    }

    .table-hover > tbody > tr > td.danger:hover, .table-hover > tbody > tr > th.danger:hover, .table-hover > tbody > tr.danger:hover > td {
        background-color: #a74240;
        border-color: #9c3c4a;
    }

    .table > thead > tr > td.warning, .table > tbody > tr > td.warning, .table > tfoot > tr > td.warning, .table > thead > tr > th.warning, .table > tbody > tr > th.warning, .table > tfoot > tr > th.warning, .table > thead > tr.warning > td, .table > tbody > tr.warning > td, .table > tfoot > tr.warning > td, .table > thead > tr.warning > th, .table > tbody > tr.warning > th, .table > tfoot > tr.warning > th {
        background-color: #c09853;
        border-color: #bc7e48;
    }

    .table-hover > tbody > tr > td.warning:hover, .table-hover > tbody > tr > th.warning:hover, .table-hover > tbody > tr.warning:hover > td {
        background-color: #b78c43;
        border-color: #ab713f;
    }

    .table > thead > tr > td {
        text-align: center;
        font-weight: bold;
        vertical-align: middle;
        font-size: 12px;
    }

    .table > tbody > tr > td {
        vertical-align: middle;
        text-align: center;
        font-size: 12px;
    }

    .containerPrincipal {
        margin: 20px 15px 0 15px;
    }

    .containerListas {
        float: left;
        width: 79%;
    }

    .containerPainelDiversos {
        float: right;
        width: 20%;
        border: 1px solid #dcdcdc;
        -webkit-border-radius: 5px;
        -moz-border-radius: 5px;
        border-radius: 5px;
    }

    .wrapPainelDiversos {
        padding: 5px;
    }

    .mPainel {
        padding: 5px;
        border: 1px solid #dcdcdc;
        -webkit-border-radius: 5px;
        -moz-border-radius: 5px;
        border-radius: 5px;
        margin-bottom: 20px;
        background-color: #f0f0f0;
    }

    .mPainelTitulo {
        background-color: #E0EEEE;
        font-weight: bold;
        font-size: 15px;
        padding: 5px;
    }

    .mPainelConteudo {
        padding-top: 5px;
    }

    .mPainelConteudoBloco {
        background-color: #FFFFFF;
        padding: 5px;
        -webkit-border-radius: 5px;
        -moz-border-radius: 5px;
        border-radius: 5px;
    }

    .lnkNomeObra {
        color: #31708f;
        cursor: pointer;
    }

    .lnkNomeObra:hover {
        text-decoration: underline;

    }

</style>


<!--Inclue do arquivo que carrega o CSS da pagina Inicio do Obras-->
<link rel="stylesheet" type="text/css" media="screen, print" href="../includes/layoutNovoObras2.css">

<!-- Container principal da página -->
<div class="containerPrincipal" style="border: none">

    <!-- Container que conter a lista de obras e lista de pendências. -->
    <div class="containerListas">

        <!-- Painel que conter avisos. -->
        <div class="panel panel-danger">
            <?php
            $arrAvisos = $avisos->recuperaDados();

            // Ordenando os avisos para que sejam sempre ordenados pelo atributo 'avsoordem'.
            if (!empty($arrAvisos)) {
                usort($arrAvisos, function ($a, $b) {
                    return strcmp($a["avsoordem"], $b["avsoordem"]);
                });
            }

            if (!empty($arrAvisos)) {
                ?>
                <div class="panel-heading">
                    <div style="height: 15px;">
                        <h4 style="float: left">Atenção!</h4>
                        <?php
                        if (count($arrAvisos) > 1) {
                            ?>
                            <p class="text-right"
                               style="font-size: 12px; font-style: italic; text-decoration: underline;cursor: pointer;float: right"
                               onclick="exibirMaisAvisos(this)">Exibir mais avisos</p>
                            <?php
                        }
                        ?>
                    </div>
                </div>
                <div class="panel-body">
                    <?php
                    $aux = 1;
                    $flgAbrirDivOculta = count($arrAvisos) > 1 ? true : false;
                    $flgFecharDivOculta = false;
                    $arrAvisos = (is_array($arrAvisos)) ? $arrAvisos : Array();
                    foreach ($arrAvisos as $aviso) {
                        $arquivos = $aviso['avsoordem'] ? $anexoAviso->pegaArquivosPorOrdem($aviso['avsoordem']) : array();
                        echo "<div style='font-size: 13px;line-height: 1.5;text-align: justify'>
                            <p><span class='label label-danger'>{$aux}</span>&nbsp;{$aviso['avsodescricao']}</p>";

                        if (!empty($arquivos)) {
                            echo "<br/><ul class='list-unstyled'>";
                            $arquivos = (is_array($arquivos)) ? $arquivos : Array();
                            foreach ($arquivos as $arq) {
                                echo "<li class='text-center'>
                                    <a target='_blank' href=\"/obras2/obras2.php?modulo=principal/inicioLista&acao=A&arquivo={$arq['arqid']}\">
                                        <img src='/imagens/salvar.png' border='0'>&nbsp;{$arq['anxavdsc']}
                                    </a>
                                  </li>";
                            }
                            echo "</ul>";

                        }

                        echo "</div><hr/>";

                        if ($flgAbrirDivOculta) {
                            echo "<div id='divOcultaAvisos' style='display: none;'>";
                            $flgAbrirDivOculta = false;
                            $flgFecharDivOculta = true;
                        }

                        $aux++;
                    }
                    if ($flgFecharDivOculta) {
                        echo "</div>";
                    }
                    ?>
                </div>
            <?php } else { ?>
                <div class="panel-body">
                    <p style="font-size: 13px;">No existem avisos cadastrados no momento!</p>
                </div>
                <?php
            }
            ?>
        </div>
        <!-- Fim do painel que conter avisos. -->

        <!-- Lista de obras. -->
        <div>
            <div class="panel panel-success">
                <div class="panel-heading" style="background-color: #E0EEEE; color: #000000">

                    <?php
                    $arrObras = pegaListaObras();
                    $tituloPainelListaObras = "Lista de obras";
                    $listaObras = array();

                    if ($arrObras) {
                        $listaObras = new ArrayIterator($arrObras);
                        $flgObrasDesatualizada = false;

                        while ($listaObras->valid()) {
                            $obra = $listaObras->current();

                            if ($obra["ultima_atualizacao"] > 30 && ($obra["strid"] == 4 || $obra["strid"] == 5)) {
                                $flgObrasDesatualizada = true;
                                break;
                            }
                            $listaObras->next();
                        }

                        $listaObras->rewind();
                        $tituloPainelListaObras = $flgObrasDesatualizada ? "Obras desatualizadas" : $tituloPainelListaObras;
                    }
                    ?>
                    <h4><?php echo $tituloPainelListaObras; ?></h4>

                </div>
                <div class="panel-body">
                    <div class="listaObras">
                        <?php
                        if (!empty($listaObras)) {
                            ?>
                            <table class="table table-bordered">
                                <thead>
                                <tr class="active">
                                    <td>Ao</td>
                                    <td>ID</td>
                                    <td>ID Pré-obra</td>
                                    <td>N Processo</td>
                                    <td>N / Ano do termo / Convênio</td>
                                    <td>Obra</td>
                                    <td>Unidade Implantadora</td>
                                    <td>Município / UF</td>
                                    <td>Data de início da execução</td>
                                    <td>Situação da obra</td>
                                    <td>Última vistoria Instituição</td>
                                    <td>% Executado empresa</td>
                                    <td>Tipologia</td>
                                </tr>
                                </thead>
                                <tbody>
                                <?php
                                while ($listaObras->valid()) {
                                    $obra = $listaObras->current();
                                    $flgObraDesatualizada = null;

                                    $estiloSituacaoObra = "";
                                    $corDataVistoria = "";

                                    // Definindo a cor da data da vistoria e identificando se a obra est desatualizada.
                                    if ($obra["strid"] == 4 || $obra["strid"] == 5) {

                                        if ($flgObrasDesatualizada) {
                                            $flgObraDesatualizada = $obra["ultima_atualizacao"] > 30 ? true : false;
                                        }

                                        if ($obra["ultima_atualizacao"] <= 25) {
                                            $corDataVistoria = "#3CC03C";
                                        } elseif ($obra["ultima_atualizacao"] > 25 && $obra["ultima_atualizacao"] <= 30) {
                                            $corDataVistoria = "#FAD200";
                                        } else {
                                            $corDataVistoria = "#FF0000";
                                        }
                                    } elseif ($obra["strid"] == 6) {
                                        $corDataVistoria = "#0066CC";
                                        $estiloSituacaoObra = "style='color:#0066CC;font-weight:bold'";
                                    }

                                    // Formatando a data de vistoria.
                                    $dataUltimaVistoria = "";
                                    if ($obra["dtvistoria"]) {

                                        /*
                                         * O tratamento a seguir  feito porque algumas datas vem da base de dados
                                         * com os microsegundos. Isso causava erro.
                                         */
                                        if (strpos($obra["dtvistoria"], ".")) {
                                            $tmp = explode(".", $obra["dtvistoria"]);
                                            $dtUltVistoria = $tmp[0];
                                        } else {
                                            $dtUltVistoria = $obra["dtvistoria"];
                                        }
                                        /* Fim do tratamento da data. */


                                        $objDateTime = DateTime::createFromFormat("Y-m-d H:i:s", $dtUltVistoria);
                                        $dataUltimaVistoria = $objDateTime->format("d/m/Y");
                                        $dataUltimaVistoria = $dataUltimaVistoria . "<br/>(" . $obra["ultima_atualizacao"] . " dia(s))";
                                    }

                                    ?>
                                    <tr>
                                        <td>
                                            <span
                                                    style="background-color: #1ab394; border-color: #1ab394; color: #FFFFFF;"
                                                    class="btn btn-primary btn-sm glyphicon glyphicon-eye-open"
                                                    data-tooltip
                                                    title="Clique aqui para visualizar a página da obra."
                                                    onclick="goToObra(<?php echo $obra["obrid"]; ?>, <?php echo json_encode($flgObraDesatualizada); ?>)">
                                            </span>
                                        </td>
                                        <td style="color: #31708f"><?php echo $obra["obrid"]; ?></td>
                                        <td style="color: #31708f"><?php echo $obra["preid"]; ?></td>
                                        <td style="color: #31708f"><?php echo $obra["processo"]; ?></td>
                                        <td style="color: #31708f"><?php echo $obra["n_termo_ano"]; ?></td>
                                        <td style="color: #31708f">
                                            <span
                                                    class="lnkNomeObra"
                                                    onclick="goToObra(<?php echo $obra["obrid"]; ?>, <?php echo json_encode($flgObraDesatualizada); ?>)">
                                                <?php echo $obra["obrnome"]; ?>
                                            </span>

                                        </td>
                                        <td><?php echo $obra["entnome"]; ?></td>
                                        <td><?php echo $obra["municipio_uf"]; ?></td>
                                        <td><?php echo $obra["inicio"]; ?></td>
                                        <td <?php echo $estiloSituacaoObra; ?>><?php echo $obra["situacao"]; ?></td>
                                        <td style="color: <?php echo $corDataVistoria; ?>">
                                            <?php echo $dataUltimaVistoria; ?>
                                        </td>
                                        <td><?php echo $obra["obrpercentultvistoria"]; ?></td>
                                        <td><?php echo $obra["tpodsc"]; ?></td>
                                    </tr>
                                    <?php
                                    $listaObras->next();
                                }
                                ?>
                                </tbody>
                            </table>
                            <?php
                        } else {
                            echo "<p style='font-size: 13px'>No foram encontrados registros!</p><br/>";
                        }
                        ?>
                    </div>

                    <!-- Boto para acessar a lista de obras geral. -->
                    <div style="text-align: center; margin:0 auto">
                        <button
                                class="btn btn-success btn-lg"
                                style="background-color: #1ab394; border-color: #1ab394 "
                                onclick="goToListaObras()"
                        >
                            Ir para Lista de Obras Geral
                        </button>
                    </div>
                    <!-- Fim do boto para acessar a lista de obras geral. -->
                </div>
            </div>
        </div>
        <!-- Fim da lista de obras -->

        <!-- Lista de pendências. -->
        <div>
            <div class="panel panel-success">
                <div class="panel-heading" style="background-color: #E0EEEE; color: #000000">
                    <h4>pendências</h4>
                </div>
                <div class="panel-body">
                    <div class="listaPendencias">
                        <?php
                        if (!verificaPerfisEspeciais()) {
                            if (verificaPendenciaExiste()) { // Verificando se existe pendência.
                                ?>
                                <table class="table table-bordered">
                                    <thead>
                                    <tr class="active">
                                        <td>&nbsp;</td>
                                        <td>pendência</td>
                                        <td>Quantidade de Obras</td>
                                        <td>Descrição da pendência</td>
                                    </tr>
                                    </thead>
                                    <tbody>
                                    <?php
                                    $arrTiposPendencia = getTiposPendencia();
                                    $arrTiposPendencia = (is_array($arrTiposPendencia)) ? $arrTiposPendencia : Array();
                                    foreach ($arrTiposPendencia as $tipoPendencia) {
                                        $arrPendencias = getPendenciasPorTipo($tipoPendencia, $_SESSION['usucpf']);
                                        if (!empty($arrPendencias)) {
                                            if (!empty($arrPendencias["pendencias"])) {
                                                ?>
                                                <tr>
                                                    <td>
                                                            <span
                                                                    style="background-color: #1ab394; border-color: #1ab394; color: #FFFFFF;"
                                                                    class="btn btn-primary btn-sm glyphicon glyphicon-plus"
                                                                    title="Clique aqui para listar as pendências."
                                                                    onclick="expandirEsconderPendencia(this)">
                                                            </span>
                                                    </td>
                                                    <td><?php echo $arrPendencias["nomeTipoPendencia"]; ?></td>
                                                    <td><?php echo count($arrPendencias["pendencias"]); ?></td>
                                                    <td><?php echo $arrPendencias["dscTipoPendencia"]; ?></td>
                                                </tr>
                                                <tr class="linhaOcultaPendencias" style="display: none;">
                                                    <td>&nbsp;</td>
                                                    <td colspan="3">
                                                        <div class="containerPendenciasTipo">
                                                            <table class="table table-bordered">
                                                                <thead>
                                                                <tr class="active">
                                                                    <td>ID</td>
                                                                    <td>Descrição</td>
                                                                    <td>UF</td>
                                                                    <td>Município</td>
                                                                    <?php
                                                                    // O tipo "Obras inacabadas" no possui todas as colunas.
                                                                    if ($tipoPendencia != PENDENCIA_OBRAS_INACABADAS) {
                                                                        ?>
                                                                        <td><?php echo $tipoPendencia == PENDENCIA_OBRAS_TERMOS_A_VENCER ? "N do Termo" : "Exec (%)"; ?></td>
                                                                        <td><?php echo $tipoPendencia == PENDENCIA_OBRAS_TERMOS_A_VENCER ? "Data Fim da vigncia" : "Situação"; ?></td>
                                                                        <?php
                                                                    }
                                                                    ?>
                                                                </tr>
                                                                </thead>
                                                                <tbody>
                                                                <?php

                                                                $arrPendencias["pendencias"] = (is_array($arrPendencias["pendencias"])) ? $arrPendencias["pendencias"] : Array();
                                                                foreach ($arrPendencias["pendencias"] as $pendencia) {

                                                                    $pendenciaInfo = getInfoObraPendencia($tipoPendencia,
                                                                        $pendencia);

                                                                    echo "<tr>";
                                                                    echo "<td><a href='{$pendenciaInfo['url']}'>{$pendenciaInfo['obrid']}</a></td>";
                                                                    echo "<td>{$pendenciaInfo['descricao']}</td>";
                                                                    echo "<td>{$pendenciaInfo['estuf']}</td>";
                                                                    echo "<td>{$pendenciaInfo['municipio']}</td>";
                                                                    if ($tipoPendencia != PENDENCIA_OBRAS_INACABADAS) {
                                                                        echo "<td>{$pendenciaInfo['execucao']}</td>";
                                                                        echo "<td style='color: red'>{$pendenciaInfo['situacao']}</td>";
                                                                    }
                                                                    echo "</tr>";
                                                                    ?>
                                                                    <?php
                                                                }
                                                                ?>
                                                                </tbody>
                                                            </table>
                                                        </div>

                                                    </td>
                                                </tr>
                                                <?php
                                            }
                                        }
                                    }
                                    ?>
                                    </tbody>
                                </table>
                                <?php
                            } else {
                                echo "<p style='font-size: 13px'>O município <b><u>no possui</u></b> pendências!</p>";
                            }
                        } else {
                            ?>
                            <table class="table table-bordered">
                                <thead>
                                <tr class="active">
                                    <td>pendência</td>
                                    <td>Quantidade de Obras</td>
                                    <td>Descrição da pendência</td>
                                </tr>
                                </thead>
                                <tbody>
                                <?php
                                $arrTiposPendencia = getTiposPendencia();
                                $arrTiposPendencia = (is_array($arrTiposPendencia)) ? $arrTiposPendencia : Array();
                                foreach ($arrTiposPendencia as $tipoPendencia) {
                                    $arrPendencias = getPendenciasPorTipo($tipoPendencia, $_SESSION['usucpf']);
                                    if (!empty($arrPendencias)) {
                                        ?>
                                        <tr>
                                            <td><?php echo $arrPendencias["nomeTipoPendencia"]; ?></td>
                                            <td><?php echo $arrPendencias["pendencias"]; ?></td>
                                            <td><?php echo $arrPendencias["dscTipoPendencia"]; ?></td>
                                        </tr>
                                        <?php
                                    }
                                }
                                ?>
                                </tbody>
                            </table>
                            <?php
                        }
                        ?>
                    </div>
                </div>
            </div>
        </div>
        <!-- Fim da lista de pendências. -->

    </div>
    <!-- Fim do container que conter a lista de obras e lista de pendências. -->

    <!-- Container da seo 'Diversos'. -->
    <div class="containerPainelDiversos">

        <div class="wrapPainelDiversos">

            <!-- Ttulo do painel 'Diversos'. -->
            <div class="alert alert-success" style="background-color: #dcdcdc; color: #000000">
                <h4>Diversos</h4>
            </div>
            <!-- Fim do tulo do painel 'Diversos'. -->

            <!-- Bloco de Obras MI -->
            <div class="mPainel painelObrasMI">
                <div class="mPainelTitulo">Obras MI</div>
                <div class="mPainelConteudo" style="display: block;">
                    <button class="btn btn-default btn-sm btn-block" onclick="goToListaObrasMI()"><b>Lista de Obras
                            MI</b></button>
                    <br/>
                    <button class="btn btn-default btn-sm btn-block" onclick="goToListaObrasMI(true)"><b>Aguardando
                            emisso de OS</b></button>
                </div>
            </div>
            <!-- Fim do bloco de Obras MI -->

            <!-- Bloco de Relatórios. -->
            <div>
                <?php //ver(PFLCOD_CONSULTA_GERAL);

                $stop_date = new DateTime();
                $stop_date->modify('-1 month');
                $dir = APPRAIZ . 'arquivos/obras2/cgu/';
                $extension = ".csv";
                $nome = "SIMEC_RELATORIO_CGU_" . $stop_date->format('Y-m') . $extension;
                $arquivoRelCGU = $dir . $nome;

                if (possui_perfil(PFLCOD_SUPER_USUARIO)) {

                    ?>
                    <div class="mPainel painelRelatorios">
                        <div class="mPainelTitulo">Relatórios</div>
                        <div class="mPainelConteudo" style="display: block;">
                            <div class="panel panel-default">
                                <div class="mPainelConteudoBloco">
                                    <ul class="list-unstyled">
                                        <?php
                                        if(is_dir($dir)) {
                                            $diretorio = dir($dir);

                                            while ($arquivo = $diretorio->read()) {
                                                if (!file_exists($arquivo)) {
                                                    ?>
                                                    <li>
                                                        <a target="_blank"
                                                           href="obras2.php?modulo=principal/inicioLista&acao=A&download_cgu=1&nomerelatoriomanual=<?= $arquivo ?>">
                                                            <img src="/imagens/salvar.png" border="0">&nbsp;
                                                            <?php echo $arquivo; ?>
                                                        </a>
                                                    </li>
                                                    <?php
                                                }
                                            }
                                            $diretorio->close();
                                        } else {
                                            echo 'Diretrio no encontrado';
                                        }
                                        ?>
                                    </ul>
                                </div>
                            </div>
                        </div>
                    </div>
                    <?php
                } elseif (possui_perfil(PFLCOD_CONSULTA_GERAL)) {
                    if (file_exists($arquivoRelCGU)) {
                        ?>
                        <div class="mPainel painelRelatorios">
                            <div class="mPainelTitulo">Relatórios</div>
                            <div class="mPainelConteudo" style="display: block;">
                                <div class="panel panel-default">
                                    <div class="mPainelConteudoBloco">
                                        <ul class="list-unstyled">
                                            <li>
                                                <a target="_blank"
                                                   href="obras2.php?modulo=principal/inicioLista&acao=A&download_cgu=1">
                                                    <img src="/imagens/salvar.png" border="0">&nbsp;
                                                    Relatório CGU <?php echo $stop_date->format("m/Y"); ?>
                                                </a>
                                            </li>
                                            <li>
                                                <a target="_blank"
                                                   href="obras2.php?modulo=principal/inicioLista&acao=A&download_cgu=2">
                                                    <img src="/imagens/salvar.png" border="0">&nbsp;
                                                    Relatório CGU Ensino
                                                    Superior <?php echo $stop_date->format("m/Y"); ?>
                                                </a>
                                            </li>
                                        </ul>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <?php
                    }
                }
                ?>
            </div>
            <!-- Fim do bloco de relatórios. -->

            <!-- Bloco de Formulários de fiscalização. -->
            <div>
                <?php
                $tecnologiasMI = pegaEstruturaFormularioMI();
                if (!empty($tecnologiasMI)) {
                    ?>
                    <div class="mPainel painelFormulariosFiscalizacao">
                        <div class="mPainelTitulo">Formulários de fiscalização <span class="slide"
                                                                                     style="cursor: pointer;"
                                                                                     onclick="expandirEsconderPainel(this)">&#9660;</span>
                        </div>
                        <div class="mPainelConteudo" style="display: none;">
                            <?php
                            foreach ($tecnologiasMI as $tecnologiaMI) {
                                ?>

                                <div class="panel panel-default">
                                    <div class="panel-heading"><p style="text-align: center">
                                            <b><?php echo $tecnologiaMI["tminome"]; ?></b></p>
                                    </div>
                                    <div class="panel-body">
                                        <?php
                                        if (!empty($tecnologiaMI["formulario"])) {
                                            foreach ($tecnologiaMI["formulario"] as $formularioMI) {
                                                ?>
                                                <div>
                                                    <a class="" target="_blank"
                                                       href="obras2.php?modulo=principal/inicioLista&acao=A&arquivo=<?php echo $formularioMI['arqid']; ?>">
                                                        <img src="/imagens/salvar.png" border="0">
                                                        <?php echo $formularioMI["ftinome"]; ?>
                                                    </a>
                                                </div>
                                                <?php
                                            }
                                        }
                                        ?>
                                    </div>
                                </div>
                                <?php
                            }
                            ?>
                        </div>
                    </div>
                    <?php
                }
                ?>
            </div>
            <!-- Fim do bloco Formulários de fiscalização. -->

            <!-- Bloco de Ajuda / Manuais -->
            <div class="mPainel painelAjudaManuais">
                <div class="mPainelTitulo">Ajuda / Manuais <span class="slide" style="cursor: pointer;"
                                                                 onclick="expandirEsconderPainel(this)">&#9660;</span>
                </div>
                <div class="mPainelConteudo" style="display: none;">

                    <!-- Bloco Ajuda / Manuais para Gestor Pblico -->
                    <div class="panel panel-default">
                        <div class="panel-heading"><p style="text-align: center"><b>Gestor Pblico</b></p></div>
                        <div class="panel-body">
                            <div><a class="" target="_blank"
                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882732"><img
                                            src="/imagens/salvar.png" border="0"> 1 Orientaes ao Gestor Pblico</a>
                            </div>
                            <div><a class="" target="_blank"
                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882735"><img
                                            src="/imagens/salvar.png" border="0"> 2 Perguntas e Respostas
                                    Frequentes</a></div>
                            <div><a class="" target="_blank"
                                    href="/obras2/tutorial/cartilha_combate_zika_obras.jpg"><img
                                            src="/imagens/salvar.png" border="0"> Cartilha do Zika</a></div>
                        </div>
                    </div>
                    <!-- Fim do bloco Ajuda / Manuais para Gestor Pblico -->

                    <!-- Bloco Ajuda / Manuais para Fiscal -->
                    <div class="panel panel-default">
                        <div class="panel-heading"><p style="text-align: center"><b>Fiscal (Superviso da obra)</b></p>
                        </div>
                        <!--                        <div class="panel-body">-->
                        <!--                            <div><a class="" target="_blank"-->
                        <!--                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882788"><img-->
                        <!--                                            src="/imagens/salvar.png" border="0"> 1 Manual SIMEC Obras 2</a></div>-->
                        <!--                            <div><a class="" target="_blank"-->
                        <!--                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882790"><img-->
                        <!--                                            src="/imagens/salvar.png" border="0"> 2 Emisso e Aceite de Ordem de-->
                        <!--                                    Serviço - Obras MI</a></div>-->
                        <!--                            <div><a class="" target="_blank"-->
                        <!--                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882751"><img-->
                        <!--                                            src="/imagens/salvar.png" border="0"> 3 Insero de Medio</a></div>-->
                        <!--                            <div><a class="" target="_blank"-->
                        <!--                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882738"><img-->
                        <!--                                            src="/imagens/salvar.png" border="0"> 4 Orientaes Para Superao de-->
                        <!--                                    Restrições e Inconformidades</a></div>-->
                        <!--                            <div><a class="" target="_blank"-->
                        <!--                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882787"><img-->
                        <!--                                            src="/imagens/salvar.png" border="0"> 5 Requisitos Para Criao de Obras-->
                        <!--                                    Vinculadas</a></div>-->
                        <!--                            <div><a class="" target="_blank"-->
                        <!--                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882736"><img-->
                        <!--                                            src="/imagens/salvar.png" border="0"> 6 Orientaes Obras MI</a></div>-->
                        <!--                            <div><a class="" target="_blank"-->
                        <!--                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882721"><img-->
                        <!--                                            src="/imagens/salvar.png" border="0"> 7 Edio de Cronograma</a></div>-->
                        <!--                            <div><a class="" target="_blank"-->
                        <!--                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882769"><img-->
                        <!--                                            src="/imagens/salvar.png" border="0"> 8 Edio de Prazos do Cronograma</a>-->
                        <!--                            </div>-->
                        <!--                            <div><a class="" target="_blank"-->
                        <!--                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882770"><img-->
                        <!--                                            src="/imagens/salvar.png" border="0"> 9 Criao de Obra Vinculada</a></div>-->
                        <!--                            <div><a class="" target="_blank"-->
                        <!--                                    href="/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882732"><img-->
                        <!--                                            src="/imagens/salvar.png" border="0"> 10 Manual - Projeto Executivo</a>-->
                        <!--                            </div>-->
                        <!--                        </div>-->
                    </div>
                    <!-- Fim do bloco Ajuda / Manuais para Fiscal -->

                    <!-- Bloco Ajuda / Manuais - Vdeos tutoriais -->
                    <div class="panel panel-default" >
                        <div class="panel-heading"><p style="text-align: center"><b>Videos tutoriais</b></p></div>
                        <div class="panel-body">
                            <?php

                            $_01_obras2_solicitar_acesso = 'https://www.youtube.com/embed/sA-_TeMkKDQ';
                            $_02_obras2_info_pos_cadastro_inicia = 'https://www.youtube.com/embed/dhsRqVkweQo';
                            $_03_obras2_acesso_inicial = 'https://www.youtube.com/embed/RD98ETnc_Ns';
                            $_04_obras2_telainicial_e_busca = 'https://www.youtube.com/embed/BSlSf_z9MM4';
                            $_05_obras2_abas = 'https://www.youtube.com/embed/pHQMqbUB4uc';
                            $_06_obras2_dados_obra = 'https://www.youtube.com/embed/T3so3E66mic';
                            $_07_obras2_licitacao = 'https://www.youtube.com/embed/ZdGs195n-Xw';
                            $_08_obras2_contratacao = 'https://www.youtube.com/embed/GGptBZli9yA';
                            $_09_obras2_cronograma = 'https://www.youtube.com/embed/fVHhuymfdiE';
                            $_010_obras2_vistoria = 'https://www.youtube.com/embed/DD99ng0pfVU';
                            $_011_obras2_recursos = 'https://www.youtube.com/embed/YCL8OCCBiL4';
                            $_012_obras2_documentos = 'https://www.youtube.com/embed/znzUqSVg8cI';
                            $_013_obras2_galeria = 'https://www.youtube.com/embed/jTKQBdeWy7o';
                            $_014_obras2_restricoes = 'https://www.youtube.com/embed/2qkd8vATBqE';
                            $_015_obras2_exec_orcamentaria = 'https://www.youtube.com/embed/FeTDx8Y8aSA';
                            $_016_obras2_aditivos = 'https://www.youtube.com/embed/TrlCYgknNZQ';
                            $_017_obras2_paralisadas = 'https://www.youtube.com/embed/4pJYro1WnpY';

                            ?>

                            <div><a onclick="exibirVideoTutorialNovo('<?= $_01_obras2_solicitar_acesso ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 01 - Solicitao de Acesso ao Sistema</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_02_obras2_info_pos_cadastro_inicia ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 02 - Informações Aps o Cadastro</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_03_obras2_acesso_inicial ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 03 - Acesso Inicial</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_04_obras2_telainicial_e_busca ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 04 - Tela Inicial e Busca</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_05_obras2_abas ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 05 - Abas</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_06_obras2_dados_obra ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 06 - Dados da Obra</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_07_obras2_licitacao ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 07 - Licitao</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_08_obras2_contratacao ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 08 - Contratao</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_09_obras2_cronograma ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 09 - Cronograma</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_010_obras2_vistoria ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 10 - Vistoria</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_011_obras2_recursos ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 11 - Recursos</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_012_obras2_documentos ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 12 - Documentos</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_013_obras2_galeria ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 13 - Galeria</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_014_obras2_restricoes ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 14 - Restrições e Inconformidades</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_015_obras2_exec_orcamentaria ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 15 - Execução Orçamentária</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_016_obras2_aditivos ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 16 - Aditivos</a>
                            </div>
                            <div><a onclick="exibirVideoTutorialNovo('<?= $_017_obras2_paralisadas ?>')">
                                    <img src="/imagens/salvar.png" border="0"> 17 - Paralisadas</a>
                            </div>


                        </div>
                    </div>
                    <!-- Fim do bloco Ajuda / Manuais - Vdeos tutoriais -->
                </div>
            </div>
            <!-- Fim do bloco de Ajuda / Manuais -->

            <!-- Bloco de Contatos -->
            <div class="mPainel painelContatos">
                <div
                        class="mPainelTitulo"
                        style="cursor:pointer;
                        text-decoration: underline"
                        onclick="popupContatos()"
                        data-tooltip
                        title="CLIQUE AQUI para ver a lista de contatos do FNDE (telefone e e-mail's), e em caso de dúvidas tais como alteração no projeto, uso de saldo em conta, troca de terreno, troca de metodologia construtiva, dentre outras."
                >Contatos
                </div>
            </div>
            <!-- Fim do bloco de Contatos -->

        </div>
    </div>
    <!-- Fim do container da seo 'Diversos'. -->

</div>
<!-- Fim do container principal da página -->

<!-- Div que exibir os vdeos tutoriais. -->
<div  style="display: none"  id="dialog_tutorial_novo" title="Tutoriais">
    <iframe id="iframe" width="560" height="315"  src="https://www.youtube.com/embed/sA-_TeMkKDQ" frameborder="1"
            allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
</div>
<!-- Fim da div que exibir os vdeos tutoriais. -->

<!-- Tabela de contatos que  exibida na janela pop-up. -->
<div id="popup-contatos" style="display: none">

    <link rel="stylesheet" type="text/css" media="screen, print" href="../includes/Estilo.css">
    <?php echo monta_cabecalho_relatorio(100); ?>
    <br/>

    <table class="table" align="center" style="width: 100%;" cellpadding="3" cellspacing="1">
        <tr>
            <td rowspan="2" class="SubTituloCentro">Assunto</td>
            <!-- <td colspan="2" class="SubTituloCentro">Contato</td> -->
        </tr>
        <tr>
            <td class="SubTituloCentro">E-mail</td>
            <td class="SubTituloCentro">Telefone</td>
        </tr>
        <tr>
            <td>Alteração no projeto</td>
            <td>obras@fnde.gov.br</td>
            <td>2022-4663</td>
        </tr>
        <tr>
            <td>Uso de Saldo</td>
            <td>obras@fnde.gov.br</td>
            <td>2022-4638</td>
        </tr>
        <tr>
            <td>Troca de Terreno</td>
            <td>obras@fnde.gov.br</td>
            <td>2022-4663/4621</td>
        </tr>
        <tr>
            <td>Troca de metodologia construtiva</td>
            <td>obras@fnde.gov.br</td>
            <td>2022-4663/4621</td>
        </tr>
        <tr>
            <td>Senha: acesso, alteração, bloqueio</td>
            <td>senha.monitoramento@fnde.gov.br; atendimento.monitora@fnde.gov.br</td>
            <td>2022-5200/5402/5199/5201</td>
        </tr>
        <tr>
            <td>Obras MI - duvidas sobre SIGARP</td>
            <td>saladesituacaoMI@fnde.gov.br</td>
            <td>2022-4913</td>
        </tr>
        <tr>
            <td>Obras MI - duvidas sobre contratao</td>
            <td>saladesituacaoMI@fnde.gov.</td>
            <td>2022-4913</td>
        </tr>
        <tr>
            <td>Obras MI - duvidas sobre</td>
            <td>saladesituacaoMI@fnde.gov.br</td>
            <td>2022-4913</td>
        </tr>
        <tr>
            <td>Desembolso de parcelas</td>
            <td>monitoramento.obras@fnde.gov.br</td>
            <td>-</td>
        </tr>
        <tr>
            <td>Restrições e inconformidades</td>
            <td>atendimento.monitora@fnde.gov.br</td>
            <td>2022-5200/5402/5199/5201</td>
        </tr>
        <tr>
            <td>Paralisao de obras</td>
            <td>atendimento.monitora@fnde.gov.br</td>
            <td>2022-5200/5402/5199/5202</td>
        </tr>
        <tr>
            <td>Preenchimento SIMEC-Obras 2.0</td>
            <td>atendimento.monitora@fnde.gov.br</td>
            <td>2022-5200/5402/5199/5203</td>
        </tr>
        <tr>
            <td>Preenchimento SIMEC-PAR</td>
            <td>par@fnde.gov.br</td>
            <td>2022-5935/5915/5922/5854/5978</td>
        </tr>
        <tr>
            <td>Prorrogao de Termo de Compromisso/Convnios</td>
            <td>grupo.prorrogao@fnde.gov.br</td>
            <td>2022-4905/5046/4822</td>
        </tr>
        <tr>
            <td>Mobilirio / Equipamentos</td>
            <td>par@fnde.gov.br</td>
            <td>2022-5935/5915/5922/5854/5978</td>
        </tr>
        <tr>
            <td>Execução de obras - dúvidas de engenharia</td>
            <td>ver aba documentos - e-mail do técnico que acompanha o Estado</td>
            <td>
                -
            </td>
        </tr>
        <tr>
            <td>Mobilirio e Equipamento do PAR. Ufs: PB/TO/ES/AP/RN</td>
            <td>adriana.silveira@fnde.gov.br</td>
            <td>2022-5849</td>
        </tr>
        <tr>
            <td>Mobilirio e Equipamento do PAR. Ufs: SP/AL/SE</td>
            <td>ana.vasconcellos@fnde.gov.br</td>
            <td>2022-5802</td>
        </tr>
        <tr>
            <td>Mobilirio e Equipamento do PAR. Ufs: MG/RR/PA</td>
            <td>maria.floriano@fnde.gov.br</td>
            <td>2022-5804</td>
        </tr>
        <tr>
            <td>Mobilirio e Equipamento do PAR. Ufs: GO/CE/PR</td>
            <td>carlos.silva@fnde.gov.br</td>
            <td>2022-5946</td>
        </tr>
        <tr>
            <td>Mobilirio e Equipamento do PAR. Ufs: DF/SC/MS</td>
            <td>filipe.azevedo@fnde.gov.br</td>
            <td>2022-5844</td>
        </tr>
        <tr>
            <td>Mobilirio e Equipamento do PAR. Ufs: RS/AM/RJ</td>
            <td>karine.queiroga@fnde.gov.br</td>
            <td>2022-5383</td>
        </tr>
        <tr>
            <td>Mobilirio e Equipamento do PAR. Ufs: MT/PE/RO/MA</td>
            <td>ionei.vilar@fnde.gov.br</td>
            <td>2022-5836</td>
        </tr>
        <tr>
            <td>Mobilirio e Equipamento do PAR. Ufs: BA/PI/AC</td>
            <td>marcelle.santos@fnde.gov.br</td>
            <td>2022-4131</td>
        </tr>
        <tr>
            <td>Técnicos Call Center</td>
            <td>par@fnde.gov.br</td>
            <td>2022-5815/5978/5854/5948/5922</td>
        </tr>
    </table>
</div>
<!-- Fim da tabela de contatos que  exibida na janela pop-up. -->

<script type="text/javascript">

    /* Observao: usar como seletor do JQuery a varivel $1_11, pois ela evita conflito com as outras verses que
     * j esto no SIMEC.
     */

    /* Script que sero executados ao final do carregamento do documento. */
    $1_11(document).ready(function () {
        $1_11("[data-tooltip]").tooltip();
    });

    /**
     * Funo responsvel por exibir os avisos ocultos.
     * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8415
     */
    function exibirMaisAvisos(elemento) {
        $1_11("#divOcultaAvisos").slideToggle("fast", function () {
            $1_11(this).is(":visible") ? $1_11(elemento).html("Exibir menos avisos") : $1_11(elemento).html("Exibir mais avisos");
        });
    }

    /**
     * Funo responsvel por exibir uma lista de contatos em uma janela do tipo pop-up.
     * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8415
     */
    function popupContatos() {

        if (window.windowContatos) {
            window.windowContatos.close();
        }

        var popupContatos = $1_11("#popup-contatos").html();
        window.windowContatos = window.open("", "Contatos", "height=648,width=1024,scrollbars=yes");
        window.windowContatos.document.write(popupContatos);
        return true;
    }

    /**
     * Funo responsvel por expandir ou recolher os painis dentro da seo 'Diversos'.
     * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8415
     * @param elemento
     */
    function expandirEsconderPainel(elemento) {

        var elementoExpandido = $1_11(elemento).html("&#9660;").parent().next("div.mPainelConteudo");
        $1_11(elementoExpandido).slideToggle("fast", function () {
            $1_11(this).is(":visible") ? $1_11(elemento).html("&#9650;") : $1_11(elemento).html("&#9660;");
        });
    }

    /**
     * Funo responsvel por exibir ou ocultar a tabela de pendências dentro de cada tipo de pendência.
     * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10779
     *
     */
    function expandirEsconderPendencia(elemento) {

        var elementoExpandido = $1_11(elemento).parent().parent().next();
        $1_11(elementoExpandido).slideToggle('fast', function () {
            if ($1_11(this).is(":visible")) {
                $1_11(elemento).removeClass("glyphicon-plus");
                $1_11(elemento).addClass("glyphicon-minus");
            } else {
                $1_11(elemento).removeClass("glyphicon-minus");
                $1_11(elemento).addClass("glyphicon-plus");
            }
        });
    }

    /**
     * Funo responsvel por exibir os tutoriais em vdeo.
     * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8415
     * @param evento
     * @param element
     */
    function exibirVideoTutorial(e, element) {

        e.preventDefault();
        var videoUrl = "/obras2/tutorial/" + element;
        var videoNameOgv = videoUrl + '.ogv';
        var videoNameMp4 = videoUrl + '.mp4';
        var modalTitle = $1_11(this).text();

        $1_11('#dialog_tutorial').find('h3').text(modalTitle);
        $1_11('#dialog_tutorial').find('source:eq(0)').attr('src', videoNameMp4);
        $1_11('#dialog_tutorial').find('source:eq(1)').attr('src', videoNameOgv);
        $1_11('video').load();

        $1_11("#dialog_tutorial").dialog({
            modal: true,
            width: 880,
            close: function (ev, ui) {
                $1_11('video').load();
            },
            buttons: {
                "Fechar": function () {
                    $1_11(this).dialog("close");
                    $1_11("video").load();
                }
            }
        });
    }

    function exibirVideoTutorialNovo(url) {
        $1_11('#iframe').attr('src', url);
        $1_11("#dialog_tutorial_novo").dialog({
            height: "auto",
            width: 600,
            modal: true,
            close: function (ev, ui) {},
            buttons: {
                "Fechar": function () {
                    $1_11(this).dialog("close");
                }
            }
        });
    }

    /**
     * Funo responsvel pelo redirecionamento para a página de lista de obras MI.
     * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8415
     * @param emissaoOs Parmetro que indica se o redirecionamento  para a lista com Emisso de OS.
     */
    function goToListaObrasMI(emissaoOs) {

        var flgEmissaoOs = typeof(emissaoOs) !== "undefined" ? emissaoOs : false;

        if (typeof(flgEmissaoOs) === "boolean") {
            var url = "";
            if (flgEmissaoOs === true) {
                url = "?modulo=principal/listaObrasMI&acao=O";
            } else {
                url = "?modulo=principal/listaObrasMI&acao=A";
            }
            location.href = url;

        } else {
            alert("Parmetro invlido.");
        }
    }

    /**
     * Funo responsvel pelo redirecionamento para a página da obra. Se a obra estiver com data de vistoria
     * acima de 30 dias, o usurio  redirecionado para a aba "Vistoria". Caso contrrio,  redirecionado para a aba
     * "Dados da obra".
     * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8415
     */
    function goToObra(obrid, flgObraDesatualizada) {
        if (flgObraDesatualizada === true) {
            location.href = '?modulo=principal/vistoria&acao=A&obrid=' + obrid;
            /*trecho comentada para retirar o bloqueio das obras com prazo de vistoria vencida
//            location.href = '?modulo=principal/vistoria&acao=A&obrid=' + obrid + '&flgDataVistoriaVencida=' + true;
//        } else if (flgObraDesatualizada === false) {
//            //alert("Existem obras desatualizadas. Por favor, regularize a Situação.");
//            alert("O município possui obra(as) desatualizada(as) h mais de 30 dias. Informamos que, para obter acesso s outras" +
//                " abas, TODAS as obras com status execução ou PARALISADA devero estar atualizadas.");
*/
        } else {
            location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
        }
    }

    /**
     * Funo responsvel pelo redirecionamento para a página de lista de obras.
     * @author: Jos Carlos <jose.costa@castgroup.com.br>, Srgio Henrique <sergio.henrique@castgroup.com.br>
     * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8415
     */
    function goToListaObras() {
        location.href = "?modulo=principal/listaObras&acao=A";
    }


</script>

<?php

$alertaObj = new Alerta();
$alertas = $alertaObj->verificaExistencia();

if ($alertas) {

    foreach ($alertas as $key => $alerta) {
        $mostrar = true;
        $cumprimentoObjeto = new CumprimentoObjeto();


        foreach ($alerta['obras'] as $obra){
          $arrayObras .=  $obra->obrid.',';
        }
        $arrayObras = substr($arrayObras,0, $size-1);
        $dataAberturaCumpObj = $cumprimentoObjeto->dataLiberacaoCumprimentoObjeto($arrayObras);

        ?>
        <div id="dialog_alerta_<?php echo $alerta['id']; ?>" data-id="<?php echo $alerta['aid']; ?>"
             title="<img src='/imagens/exclamacao_checklist.png'> Notificao  <?php echo $alerta['titulo'] ? (' - ' . $alerta['titulo']) : ''; ?>"
             class="alerta_obras"
             style="display: none">
            <div class="" style="font-size: 14px; color: #002A6C; font-weight: bold;">
                <div>
                    <?php
                    if ($alerta['exibe_cabecalho']) : ?>
                        <p style="text-align:center;"><img
                                    src="data:image/png;base64,<?php echo base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif')) ?>"
                                    width="70"/><br/>
                        <p style="text-align: center;">
                            MINISTRIO DA EDUCAO<br>
                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAO<br>
                            DIRETORIA DE GESTO ARTICULAO E PROJETOS EDUCACIONAIS<br>
                            COORDENAO GERAL DE INFRAESTRUTURA EDUCACIONAL<br>
                            SBS Quadra 02 - Bloco F - Edifcio FNDE - 14 andar - CEP -70070-929
                        </p>
                    <?php
                    endif; ?>
                    <p style="text-align:right;">
                        N <?php echo str_pad($alerta['aid'], 7, '0', STR_PAD_LEFT);

                         if($dataAberturaCumpObj['dtaberturacumpobj'] != ''){
                             echo '<br /><br />Data da Notificao : '.$dataAberturaCumpObj['dtaberturacumpobj'];
                         }

                        ?>
                    </p><br>

                    <p>
                        Sr (a) <?php echo $_SESSION['usunome'] ?><br><br>
                    </p>
                    <div style="text-align: justify;">
                        <?php echo $alerta['mensagem'] ?>
                    </div>
                    <br>
                    <?php
                    if ($alerta['assinatura_gestor']):
                        ?>
                        <div style="text-align:center;">
                            Atenciosamente,<br>
                            <?php
                            if ($alerta['assinatura_gestor']['arqid']): ?>
                                <br/>
                                <?php if (file_exists(APPRAIZ . "arquivos/obras2/" . floor($alerta['assinatura_gestor']['arqid'] / 1000))): ?>
                                    <img src='data:image/png;base64,<?php echo base64_encode(file_get_contents(APPRAIZ . "arquivos/obras2/" . floor($alerta['assinatura_gestor']['arqid'] / 1000) . "/" . $alerta['assinatura_gestor']['arqid'])) ?>'
                                         style="width:200px;" alt=""/>
                                <?php endif; ?>
                            <?php
                            endif; ?>
                            <?php echo $alerta['assinatura_gestor']['descricao'] ?>
                        </div>
                    <?php
                    endif;

                    if ($alerta['arquivos']): ?>
                        <?php
                        foreach ($alerta['arquivos'] as $arquivo): ?>
                            <a style="font-size:14px; color:#FF0101;" target="_blank"
                               href="/obras2/obras2.php?modulo=principal/inicioLista&acao=A&download=<?php echo $arquivo['arqid'] ?>">
                                <img src='/imagens/salvar.png' border='0'>
                                <?php echo $arquivo['arqnome'] ?>
                            </a><br>
                        <?php
                        endforeach; ?>
                    <?php
                    endif; ?>
                    <br/><br/>
                    <form method="post" action="" id="alerta_<?php echo $alerta['aid'] ?>">
                        <input type="hidden" name="requisicao" value="alerta">
                        <input type="hidden" name="ra" value="<?php echo $alerta['ra'] ?>">
                        <input type="hidden" name="aid" value="<?php echo $alerta['aid'] ?>">
                        <input type="hidden" name="pos_acao" value="<?php echo $alerta['pos_acao'] ?>">
                        <table class="tabela">
                            <tr>
                                <td class="subTituloEsquerda">ID da Obra</td>
                                <td class="subTituloEsquerda">Nome</td>
                                <td class="subTituloEsquerda">Tipologia</td>
                                <td class="subTituloEsquerda">UF</td>
                                <td class="subTituloEsquerda">Município</td>
                                <?php
                                if ($alerta['condicao']) : ?>
                                    <td class="subTituloEsquerda">Observao</td>
                                <?php
                                endif; ?>
                            </tr>
                            <?php
                            foreach ($alerta['obras'] as $obra): if (!$obra->apresentar) {
                                $mostrar = false;
                            } ?>
                                <tr>
                                    <td>
                                        <input type="hidden" name="aoid[]" value="<?php echo $obra->aoid ?>">
                                        <input type="hidden" name="obrid[]" value="<?php echo $obra->obrid ?>">
                                        <input type="hidden" name="obrnome[]" value="<?php echo $obra->obrnome ?>">
                                        <?php echo $obra->obrid ?>
                                    </td>
                                    <td><?php echo $obra->obrnome ?></td>
                                    <td><?php echo $obra->tpodsc ?></td>
                                    <td><?php echo $obra->estuf ?></td>
                                    <td><?php echo $obra->mundescricao ?></td>
                                    <?php
                                    if ($alerta['condicao']) : ?>
                                        <td>
                                            <p style="color:red;">
                                                <?php
                                                if ($obra->mensagem) {
                                                    $mensagem = '<img title="" src="/imagens/valida5.gif"> ' . $obra->mensagem;
                                                } else {
                                                    $mensagem = '<img title="" src="/imagens/valida1.gif">';
                                                }
                                                echo $mensagem;
                                                ?>
                                            </p>
                                        </td>
                                    <?php
                                    endif; ?>
                                </tr>
                            <?php
                            endforeach; ?>
                        </table>
                        <br/>
                        <?php
                        if (possui_perfil(array(PFLCOD_GESTOR_UNIDADE))):
                            if ($mostrar) : ?>
                                <button type="submit">Li e Entendi</button>
                            <?php
                            else: ?>
                                <p class="" style="color:red;"><img title=""
                                                                    src="/imagens/valida5.gif"> <?php echo $alerta['mensagem_condicao'] ?>
                                </p><br>
                                <p class="" style="font-size:12px;">Verifique o painel acima.</p>
                                <button type="button" disabled>Li e Entendi</button>
                            <?php
                            endif;
                        elseif (possui_perfil(array(PFLCOD_SUPERVISOR_UNIDADE))): ?>
                            <strong>
                                <?php
                                if ($alerta['contato']) {
                                    echo 'Favor, entrar em contato com ' . $alerta['contato'] . '.';
                                } else {
                                    echo 'Favor, informar Responsvel.';
                                }
                                ?>
                            </strong>
                        <?php
                        endif; ?>
                    </form>
                </div>
            </div>
            <div style="clear: both"></div>
            <br>
        </div>
        <?php
    }
}

?>

<script type="text/javascript">
    jQuery(function () {
        if (jQuery('.alerta_obras')) {
            jQuery(".alerta_obras").dialog({
                modal: true,
                width: 860,
                dialogClass: 'info',
                <?php
                if(possui_perfil(array(PFLCOD_GESTOR_UNIDADE)) && $_SESSION['usucpforigem'] == $_SESSION['usucpf']):
                if($mostrar) : ?>
                open: function (event, ui) {
                    $(".ui-dialog-titlebar-close", ui.dialog | ui).hide();
                }
                <?php  endif;
                endif; ?>

            });
        }
    });
</script>
