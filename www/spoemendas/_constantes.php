<?php

$is_producao = true;
if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
    $is_producao = false;
}

define('PFL_TED', '');
define('PERFIL_UO_EQUIPE_TECNICA', '');
define('edevalordisponivel', '');

// --Equipe responsável
define('EMAIL_SIMEC_ANALISTA', 'MaykelBraz@mec.gov.br');
define('EMAIL_SIMEC_DESENVOLVEDOR', 'LucasGomes@mec.gov.br');

// -- tipo de documento workflow
define('WF_TPDID_SPOEMENDAS', $is_producao ? 252 : 362);
define('WF_TPDID_SPOEMENDAS_SOLICITACOES', 263);
define('WF_TPDID_SPOEMENDAS_SOLICITACOES_FINANCEIRAS', $is_producao ? 297 : 365);
define('WF_TPDID_SPOEMENDAS_ANEXO_OFICIO', $is_producao ? 301 : 369);

// -- estados do documento
define('ESD_EM_PREENCHIMENTO', $is_producao ? 1685 : 2094);
define('ESD_EM_ANALISE', $is_producao ? 1686 : 2097); // -- Aguardando elaboração do plano de trabalho - FNDE
define('ESD_AGUARDANDO_AJUSTES_PARLAMENTAR', $is_producao ? 1687 : 2096);
define('ESD_AGUARDANDO_AJUSTES_PARLAMENTAR_PAR_CONV', $is_producao ? 2119 : 2119);
define('ESD_AGUARDANDO_AJUSTES_PARLAMENTAR_TED', $is_producao ? 2125 : 2125);
define('ESD_AGUARDANDO_REITOR', $is_producao ? 1695 : 2098);
define('ESD_ANALISA_SECRETARIA', $is_producao ? 1696 : 2099);
define('ESD_LIMITE_LIBERADO', $is_producao ? 1701 : 2102);
define('ESD_AGUARD_AJUST_UO', $is_producao ? 1697 : 2100);
define('ESD_AGUARD_ANALISE_FNDE', 1688);
define('ESD_EMENDA_IMPEDIMENTO', $is_producao ? 1969 : 2108);
define('ESD_RECEBIDO', $is_producao ? 1982 : 2127);

//producao?desenvolvimento
define('ESD_NAO_ENVIADO', $is_producao ? 1967 : 2109);
define('ESD_ENVIADO', $is_producao ? 1968 : 2110);

// -- transições
define('TRANS_AJUSTE_RA_RET_ANALISE_RA', 3123);

/**
 * Identifica o nome do sistema. Utilizado para armazenar dados na sessão.
 */
define('MODULO', $_SESSION['sisdiretorio']);

// -- Perfis
define('PFL_SUPER_USUARIO', 1448);
define('PFL_PARLAMENTAR', 1458);
define('PFL_CGO_EQUIPE_ORCAMENTARIA', 1449);
define('PFL_CGF_EQUIPE_FINANCEIRA', 1449);
define('PFL_ASPAR', 1460);
define('PFL_REITOR', 1473);
define('PFL_UO_EQUIPE_TECNICA', 1457);
define('PFL_UO_EQUIPE_FINANCEIRA', $is_producao ? 1786 : 1786);
define('PFL_SECRETARIAS', 1459);
define('PFL_CONSULTA_GERAL', 1498);
define('PFL_FNDE', $is_producao ? 1612 : 1579);
define('PFL_SPO_EQUIPE_FINANCEIRA', $is_producao ? 1785 : 1785);

// tipos de execução F/E
define('EMENDA_DO_PAR', 1);
define('EMENDA_DO_TED', 2);
define('EMENDA_DO_CONV', 3);
define('EMENDA_DO_DIR', 4);
define('EXECUCAO_PAR_CONV', 5);

// emendas
define('EMENDA_IMPOSITIVA', 6);

// periodos
define('PERIODO_EFETIVO', 'E');
define('PERIODO_PREENCHIMENTO', 'P');

// util
define('DATA_COMPLETA', 'DD/MM/YYYY');

define('SISID', 237);

//condicoes exception excel
define('NAO_MODELO', 1);
define('CONTEM_LETRAS_VALOR', 2);
define('EXCEL_TAMANHO_CAMPOS_CORRETO', 3);

define('IDENTIFICADOR_SISTEMA_SPO_EMENDAS', 237);
define('EDIT_TODAS_FORMA_EXEC', 6);

// -- Constante de tipo de impedimento
define('TIPO_IMPEDIMENTO_6_DO_RP7', $is_producao ? 53 : 23);