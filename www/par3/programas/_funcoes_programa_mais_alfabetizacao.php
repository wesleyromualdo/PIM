<?php

require_once APPRAIZ.'/includes/classes/Modelo.class.inc';
require_once APPRAIZ.'/par3/classes/model/AdesaoPrograma.class.inc';
require_once APPRAIZ.'/par3/classes/model/adesao_programa/CoordenadorMaisAlfabetizacao.class.inc';
require_once APPRAIZ.'/par3/classes/model/adesao_programa/Adesaoalfabetizacao.class.inc';

function wf_validade_vigencia($inuid)
{
    $modelAdesaoPrograma = new Par3_Model_AdesaoPrograma();
    $result = $modelAdesaoPrograma->recuperaPorInuid($inuid, 10, 2018);
    $objCoordenador = new Par3_Model_CoordenadorMaisAlfabetizacao();
    $objAdmEscolas = new Par3_Model_Adesaoalfabetizacao();
    $adpid = $result['adpid'];

    $situacaoAtual = wf_pegarEstadoAtual($result['docid']);
    $esdid = $situacaoAtual['esdid'];

    //Sobre quantidade escolas já salvas
    $dadosSobreEscola['inuid'] = $inuid;
    $dadosSobreEscola['adpid'] = $adpid;

    $dadosSobreEscola['carga_horaria'] = '10';
    $totalEscolasGrupo1 = $objAdmEscolas->retornaQtdSalvoGrupo($dadosSobreEscola);
    $dadosSobreEscola['carga_horaria'] = '5';
    $totalEscolasGrupo2 = $objAdmEscolas->retornaQtdSalvoGrupo($dadosSobreEscola);


    $existeAdesao1 = $objAdmEscolas->existeEscolas($inuid, $adpid, '10');
    if ($existeAdesao1 && $totalEscolasGrupo1==0) {
        $restricaoMinimaGrupo1 = true;
    }
    $existeAdesao2 = $objAdmEscolas->existeEscolas($inuid, $adpid, '5');
    if ($existeAdesao2 && $totalEscolasGrupo2==0) {
        $restricaoMinimaGrupo2 = true;
    }

    /****************/
    $arrRestricoes = array();

    if ($result['adpresposta'] != 'S') {
        $arrRestricoes[]['dsc'] = '<span style="color:red" > Termo não aceito </span>';
    }
    if (!$objCoordenador->existeCoordenador($inuid, $adpid)) {
        $arrRestricoes[]['dsc'] = '<span style="color:red"> Dados do Coordenador não preenchidos </span>';
    }
    if (!$objAdmEscolas->existeEscolas($inuid, $adpid)) {
        $arrRestricoes[]['dsc'] = '<span style="color:red"> Nenhuma escola selecionada </span>';
    }
    if ($restricaoMinimaGrupo1) {
        $arrRestricoes[]['dsc'] = '<span style="color:red"> Selecione no mínimo uma escola no grupo 01</span>';
    }
    if ($restricaoMinimaGrupo2) {
        $arrRestricoes[]['dsc'] = '<span style="color:red"> Selecione no mínimo uma escola no grupo 02</span>';
    }

    /*****************/
    if (
        (($esdid == WF_ESDID_EMCADASTRAMENTO_MAISALFABETIZACAO)) &&
        ($result['adpresposta'] != 'N') &&
        (!$bloqueiaValidade) &&
        count($arrRestricoes) == 0
    ) {
        return true;
    }
}