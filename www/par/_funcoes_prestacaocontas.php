<?php
function condicaoAcaoEnviarPCControleSocial($obrid)
{
    global $db;

    include_once APPRAIZ . "obras2/classes/modelo/ExecucaoFinanceira.class.inc";

    $execucaoFinanceira = new ExecucaoFinanceira();
    $processo = $execucaoFinanceira->retornarProcesso($obrid);
    $quantidadeObras = $execucaoFinanceira->verificarQuantidadeObrasProcesso($obrid, $processo);
    $obracCancelada = true;

    if ($quantidadeObras == 1) {
        $sql_finalizada = "SELECT excfinalizada FROM obras2.execucaofinanceiracomposicao where obrid = {$obrid} AND excstatus = 'A'";
        $finalizada = $db->pegaUm($sql_finalizada);

        //Verifica processo no processo com uma unica obra, se a obra está cancelada
        $docidObra = $db->pegaUm("SELECT docid FROM obras2.obras WHERE obrid = {$obrid}");
        if($docidObra) {
            $esdidObra = wf_pegarEstadoAtual($docidObra);
            $esdidObra = $esdidObra['esdid'];

            if($esdidObra != ESDID_OBJ_CANCELADO){
                $obracCancelada = false;
            }
        }
    } else {
        $exeid = $db->pegaUm("SELECT exeid FROM obras2.execucaofinanceiracomposicao WHERE obrid = {$obrid} AND excstatus = 'A'");
        if ($exeid) {
            $excid = $db->pegaUm("SELECT excid FROM obras2.execucaofinanceiracomposicao WHERE exeid = {$exeid} AND excfinalizada = FALSE AND excstatus = 'A'");
            if ($excid) {
                $finalizada = false;
            } else {
                $finalizada = 't';
            }
        } else {
            $finalizada = 'f';
        }
    }

    $proid = $db->pegaUm("SELECT proid_par FROM obras2.execucaofinanceiracomposicao exc 
                        INNER JOIN obras2.execucaofinanceira ex ON exc.exeid = ex.exeid WHERE exc.obrid = {$obrid} AND ex.exestatus = 'A' AND exc.excstatus = 'A' ");

    //Busca situação da prestaçao de contas
    if($proid) {
        $docidSituacaoFluxo = $db->pegaUm("SELECT docid FROM obras2.situacaoprestacaocontasobras WHERE proid_par = {$proid}");

        if($docidSituacaoFluxo){
            $esdidSituacao = wf_pegarEstadoAtual($docidSituacaoFluxo);
            $esdidSituacao = $esdidSituacao['esdid'];
        }

        //verifica se todas as obras de um processo Obras-PAR estão canceladas, caso estajam tramita direo para analise
        $sql1 = "SELECT o.docid
                FROM par.processoobraspar po
                INNER JOIN par.documentopar dp ON dp.proid = po.proid AND dp.dopstatus = 'A'
                INNER JOIN par.termocomposicao tc ON dp.dopid = tc.dopid
                INNER JOIN obras2.obras o ON tc.preid = o.preid AND o.obridpai ISNULL AND o.obrstatus = 'A'
                WHERE po.proid = {$proid}";

        $arrObras = $db->carregar($sql1);
        $arrObras =  $arrObras ? $arrObras : array();

        foreach ($arrObras as $obra){
            if($obra['docid']) {
                $esdidObra = wf_pegarEstadoAtual($obra['docid']);
                $esdidObra = $esdidObra['esdid'];

                if($esdidObra != ESDID_OBJ_CANCELADO){
                    $obracCancelada = false;
                    break;
                }
            }
        }
        //fim verificação obras-par canceladas
    }

    $sql = "SELECT 
                    COUNT(*)
                FROM obras2.obras o
                INNER JOIN par.termocomposicao pto ON pto.preid = o.preid
                INNER JOIN par.documentopar pd ON pd.dopid = pto.dopid AND pd.dopstatus = 'A'
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
				LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
				LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE 
                    o.obrid  = {$obrid}
                    AND o.obridpai IS NULL
                    AND o.obrstatus = 'A' ";

    $obraPar = $db->pegaUm($sql);

    if (($obraPar > 0) && ($finalizada == 't') && (!verificaObraSigpc($obrid, null))
        && ($esdidSituacao != ESDID_OBRAS_SPC_SEMVALOR) && (!$obracCancelada)) {
        return true;
    } else {
        return false;
    }
}

function posAcaoEnviarPCControleSocial($obrid)
{

    global $db;

    $proid = $db->pegaUm("SELECT proid_par FROM obras2.execucaofinanceiracomposicao exc 
                        INNER JOIN obras2.execucaofinanceira ex ON exc.exeid = ex.exeid WHERE exc.obrid = {$obrid} AND ex.exestatus = 'A' AND exc.excstatus = 'A' ");

    if ($proid) {

        //Trecho busca DOCID situaçao da PC caso nao tenha cria um docid
        $docidSituacaoFluxo = $db->pegaUm("SELECT docid FROM obras2.situacaoprestacaocontasobras WHERE proid_par = {$proid}");

        if (!$docidSituacaoFluxo) {
            $docidSituacaoFluxo = wf_cadastrarDocumento(TPDID_OBRAS_FLUXO_SITUACAO_PRESTACAO_CONTAS,
                "Cadastro de documento do Fluxo de Situação da prestação de contas - Via execução financeira",
                ESDID_OBRAS_SPC_NAO_ENVIADA);

            if ($docidSituacaoFluxo != '') {
                $insert = "INSERT INTO
    		    				obras2.situacaoprestacaocontasobras
    		    				(proid_par, docid)
    		    				VALUES
    		    				({$proid}, {$docidSituacaoFluxo})";

                $registroFluxoSituacao = $db->executar($insert);
            }
        } else {
            $registroFluxoSituacao = true;
        }

        //Trecho busca DOCID situaçao da OPC caso nao tenha cria um docid
        $docidSituacaoOPC = $db->pegaUm("SELECT docid FROM obras2.situacaoopcobras WHERE proid_par = {$proid}");

        if (!$docidSituacaoOPC) {
            $docidSituacaoOPC = wf_cadastrarDocumento(TPDID_OBRAS_FLUXO_OBRIGATORIEDADE_DE_PRESTAR_CONTAS,
                "Cadastro de documento do Fluxo de Situação da obrigatoriedade de prestar contas -Via execução financeira",
                ESDID_OBRAS_OPC_ADIMPLENTE);

            if ($docidSituacaoOPC != '') {
                $insert = "INSERT INTO
    		    				situacaoopcobras
    		    				(proid_par, docid)
    		    				VALUES
    		    				({$proid}, {$docidSituacaoOPC})";

                $registroFluxoOPC = $db->executar($insert);
            }
        } else {
            $registroFluxoOPC = true;
        }


        if (($registroFluxoSituacao && $docidSituacaoFluxo) && ($registroFluxoOPC && $docidSituacaoOPC)) {

            $esdidSituacao = wf_pegarEstadoAtual($docidSituacaoFluxo);
            $esdidSituacao = $esdidSituacao['esdid'];

            $esdidSituacaoOPC = wf_pegarEstadoAtual($docidSituacaoOPC);
            $esdidSituacaoOPC = $esdidSituacaoOPC['esdid'];

            // A Tramitação 1 diz respeito a SITUAÇÃO DA PRESTAÇÃO DE CONTAS
            if ($esdidSituacao == ESDID_OBRAS_SPC_NAO_ENVIADA) {

                $tramitacao1 = wf_alterarEstado($docidSituacaoFluxo,
                    AEDID_OBRAS_SPC_DE_NAO_ENVIADA_PARA_CONTROLE_SOCIAL,
                    "Enviado para 'Controle Social' através da funcionalidade 'Enviar Prestação de Contas' ", array());

            } elseif ($esdidSituacao == ESDID_OBRAS_SPC_NOTIFICADA_POR_OMISSAO) {
                $tramitacao1 = wf_alterarEstado($docidSituacaoFluxo, AEDID_OBRAS_SPC_DE_NOTIFICADA_PARA_CONTROLE_SOCIAL,
                    "Enviado para 'Controle Social' através da funcionalidade 'Enviar Prestação de Contas' ", array());
            } else {
                $tramitacao1 = false;
            }

            // A tramitaçao 2 diz respeito a SITUAÇÃO DA OBRIGATORIEDADE DE PRESTAR CONTAS
            if ($esdidSituacaoOPC == ESDID_OBRAS_OPC_INADIMPLENTE) {
                $tramitacao2 = wf_alterarEstado($docidSituacaoOPC, AEDID_OBRAS_OPC_DE_INADIMPLENTE_PARA_ADIMPLENTE,
                    "Enviado para 'Adimplente' através da funcionalidade 'Enviar Prestação de Contas' ", array());
            } elseif ($esdidSituacaoOPC == ESDID_OBRAS_OPC_ADIMPLENTE) {
                $tramitacao2 = true;
            } else {
                $tramitacao2 = false;
            }

            if ((!$tramitacao1) || (!$tramitacao2)) {
                die('Erro na tramitação do fluxo de prestação de contas');
            } else {
                $db->commit();
            }
        }

    } else {
        die('Erro na tramitação do fluxo de prestação de contas');
    }

}

function condicaoAcaoEnviarPCAnalise($obrid)
{
    global $db;

    include_once APPRAIZ . "obras2/classes/modelo/ExecucaoFinanceira.class.inc";

    $execucaoFinanceira = new ExecucaoFinanceira();
    $processo = $execucaoFinanceira->retornarProcesso($obrid);
    $quantidadeObras = $execucaoFinanceira->verificarQuantidadeObrasProcesso($obrid, $processo);
    $obracCancelada = true;

    if ($quantidadeObras == 1) {
        $sql_finalizada = "SELECT excfinalizada FROM obras2.execucaofinanceiracomposicao where obrid = {$obrid} AND excstatus = 'A'";
        $finalizada = $db->pegaUm($sql_finalizada);

        //Verifica processo no processo com uma unica obra, se a obra está cancelada
        $docidObra = $db->pegaUm("SELECT docid FROM obras2.obras WHERE obrid = {$obrid}");
        if($docidObra) {
            $esdidObra = wf_pegarEstadoAtual($docidObra);
            $esdidObra = $esdidObra['esdid'];

            if($esdidObra != ESDID_OBJ_CANCELADO){
                $obracCancelada = false;
            }
        }
    } else {
        $exeid = $db->pegaUm("SELECT exeid FROM obras2.execucaofinanceiracomposicao WHERE obrid = {$obrid} AND excstatus = 'A'");
        if ($exeid) {
            $excid = $db->pegaUm("SELECT excid FROM obras2.execucaofinanceiracomposicao WHERE exeid = {$exeid} AND excfinalizada = FALSE AND excstatus = 'A' ");
            if ($excid) {
                $finalizada = false;
            } else {
                $finalizada = 't';
            }
        } else {
            $finalizada = 'f';
        }
    }

    $sql = "SELECT 
                COUNT(*)
                FROM obras2.obras o 
                INNER JOIN par.termoobra pto ON pto.preid = o.preid AND o.obridpai IS NULL
                INNER JOIN par.termocompromissopac ptp ON ptp.terid = pto.terid AND ptp.terstatus = 'A' 
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
				LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE 
                    o.obrid  = {$obrid}
                    AND o.obridpai IS NULL
                    AND o.obrstatus = 'A' ";

    $obraPac = $db->pegaUm($sql);

    if ($obraPac > 0) {
        $campo = 'proid_pac';
    } else {
        $campo = 'proid_par';
    }

    //Trecho busca DOCID situaçao da PC
    $proid = $db->pegaUm("SELECT {$campo} FROM obras2.execucaofinanceiracomposicao exc 
                        INNER JOIN obras2.execucaofinanceira ex ON exc.exeid = ex.exeid WHERE exc.obrid = {$obrid} AND ex.exestatus = 'A' AND exc.excstatus = 'A' ");
    if($proid) {
        // Verifica processo sem saldo
        $docidSituacaoFluxo = $db->pegaUm("SELECT docid FROM obras2.situacaoprestacaocontasobras WHERE {$campo} = {$proid}");
        if($docidSituacaoFluxo) {
            $esdidSituacao = wf_pegarEstadoAtual($docidSituacaoFluxo);
            $esdidSituacao = $esdidSituacao['esdid'];
        }
        //fim verificação sem saldo

        //verifica se todas as obras de um processo Obras-PAR estão canceladas, caso estajam tramita direo para analise
        if ($obraPac > 0) {
            $arrObras = array();
        } else {
            $sql1 = "SELECT o.docid
                    FROM par.processoobraspar po
                    INNER JOIN par.documentopar dp ON dp.proid = po.proid AND dp.dopstatus = 'A'
                    INNER JOIN par.termocomposicao tc ON dp.dopid = tc.dopid
                    INNER JOIN obras2.obras o ON tc.preid = o.preid AND o.obridpai ISNULL AND o.obrstatus = 'A'
                    WHERE po.proid = {$proid}";

            $arrObras = $db->carregar($sql1);
            $arrObras =  $arrObras ? $arrObras : array();
        }
        foreach ($arrObras as $obra){
            if($obra['docid']) {
                $esdidObra = wf_pegarEstadoAtual($obra['docid']);
                $esdidObra = $esdidObra['esdid'];

                if($esdidObra != ESDID_OBJ_CANCELADO){
                    $obracCancelada = false;
                    break;
                }
            }
        }
        //fim verificação obras-par canceladas
    }

    if (($obraPac > 0 || $obracCancelada) && ($finalizada == 't') && (!verificaObraSigpc($obrid, null)) &&
        ($esdidSituacao != ESDID_OBRAS_SPC_SEMVALOR)) {
        return true;
    } else {
        return false;
    }

}

function posAcaoEnviarPCAnalise($obrid)
{
    global $db;

    $sql = "SELECT 
                COUNT(*)
                FROM obras2.obras o 
                INNER JOIN par.termoobra pto ON pto.preid = o.preid AND o.obridpai IS NULL
                INNER JOIN par.termocompromissopac ptp ON ptp.terid = pto.terid AND ptp.terstatus = 'A' 
                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
				LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                WHERE 
                    o.obrid  = {$obrid}
                    AND o.obridpai IS NULL
                    AND o.obrstatus = 'A' ";

    $obraPac = $db->pegaUm($sql);

    if($obraPac > 0){
        $campo = 'proid_pac';
    } else {
        $campo = 'proid_par';
    }

    $proid = $db->pegaUm("SELECT {$campo} FROM obras2.execucaofinanceiracomposicao exc 
                        INNER JOIN obras2.execucaofinanceira ex ON exc.exeid = ex.exeid WHERE exc.obrid = {$obrid} AND ex.exestatus = 'A' AND exc.excstatus = 'A' ");

    if ($proid) {

        //Trecho busca DOCID situaçao da PC caso nao tenha cria um docid
        $docidSituacaoFluxo = $db->pegaUm("SELECT docid FROM obras2.situacaoprestacaocontasobras WHERE {$campo} = {$proid}");

        if (!$docidSituacaoFluxo) {
            $docidSituacaoFluxo = wf_cadastrarDocumento(TPDID_OBRAS_FLUXO_SITUACAO_PRESTACAO_CONTAS,
                "Cadastro de documento do Fluxo de Situação da prestação de contas - Via execução financeira",
                ESDID_OBRAS_SPC_NAO_ENVIADA);

            if ($docidSituacaoFluxo != '') {
                $insert = "INSERT INTO
    		    				obras2.situacaoprestacaocontasobras
    		    				({$campo}, docid)
    		    				VALUES
    		    				({$proid}, {$docidSituacaoFluxo})";

                $registroFluxoSituacao = $db->executar($insert);
            }
        } else {
            $registroFluxoSituacao = true;
        }


        //Trecho busca DOCID situaçao da OPC caso nao tenha cria um docid
        $docidSituacaoOPC = $db->pegaUm("SELECT docid FROM obras2.situacaoopcobras WHERE {$campo} = {$proid}");

        if (!$docidSituacaoOPC) {
            $docidSituacaoOPC = wf_cadastrarDocumento(TPDID_OBRAS_FLUXO_OBRIGATORIEDADE_DE_PRESTAR_CONTAS,
                "Cadastro de documento do Fluxo de Situação da obrigatoriedade de prestar contas -Via execução financeira",
                ESDID_OBRAS_OPC_ADIMPLENTE);

            if ($docidSituacaoOPC != '') {
                $insert = "INSERT INTO
    		    				situacaoopcobras
    		    				({$campo}, docid)
    		    				VALUES
    		    				({$proid}, {$docidSituacaoOPC})";

                $registroFluxoOPC = $db->executar($insert);
            }
        } else {
            $registroFluxoOPC = true;
        }


        if (($registroFluxoSituacao && $docidSituacaoFluxo) && ($registroFluxoOPC && $docidSituacaoOPC)) {

            $esdidSituacao = wf_pegarEstadoAtual($docidSituacaoFluxo);
            $esdidSituacao = $esdidSituacao['esdid'];

            $esdidSituacaoOPC = wf_pegarEstadoAtual($docidSituacaoOPC);
            $esdidSituacaoOPC = $esdidSituacaoOPC['esdid'];

            // A Tramitação 1 diz respeito a SITUAÇÃO DA PRESTAÇÃO DE CONTAS
            if ($esdidSituacao == ESDID_OBRAS_SPC_NAO_ENVIADA) {
                $tramitacao1 = wf_alterarEstado($docidSituacaoFluxo, AEDID_OBRAS_SPC_DE_NAO_ENVIADA_PARA_ANALISE,
                    "Enviado para 'Aguardando Análise' através da funcionalidade 'Enviar Prestação de Contas' ", array());

            } elseif ($esdidSituacao == ESDID_OBRAS_SPC_NOTIFICADA_POR_OMISSAO) {
                $tramitacao1 = wf_alterarEstado($docidSituacaoFluxo, AEDID_OBRAS_SPC_DE_NOTIFICADA_PARA_ANALISE,
                    "Enviado para 'Aguardando Análise' através da funcionalidade 'Enviar Prestação de Contas' ", array());
            } else {
                $tramitacao2 = false;
            }

            // A tramitaçao 2 diz respeito a SITUAÇÃO DA OBRIGATORIEDADE DE PRESTAR CONTAS
            if ($esdidSituacaoOPC == ESDID_OBRAS_OPC_INADIMPLENTE) {
                $tramitacao2 = wf_alterarEstado($docidSituacaoOPC, AEDID_OBRAS_OPC_DE_INADIMPLENTE_PARA_ADIMPLENTE,
                    "Enviado para 'Adimplente' através da funcionalidade 'Enviar Prestação de Contas' ", array());
            } elseif ($esdidSituacaoOPC == ESDID_OBRAS_OPC_ADIMPLENTE) {
                $tramitacao2 = true;
            } else {
                $tramitacao2 = false;
            }

            if ((!$tramitacao1) || (!$tramitacao2)) {
                die('Erro na tramitação do fluxo de prestação de contas');
            } else {
                $db->commit();
            }
        }

    } else {
        die('Erro na tramitação do fluxo de prestação de contas');
    }
}

function condicaoAcaoRetornaPCCACSParaExecucao($obrid)
{

    global $db;

    $sql = "
    SELECT
              COUNT(*)
             FROM obras2.obras o
            INNER JOIN par.termocomposicao pto ON pto.preid = o.preid
            INNER JOIN par.documentopar pd ON pd.dopid = pto.dopid AND pd.dopstatus = 'A'
            INNER JOIN obras2.execucaofinanceiracomposicao exc ON o.obrid = exc.obrid
            INNER JOIN obras2.execucaofinanceira exe ON exc.exeid = exe.exeid
            INNER JOIN workflow.documento doc ON exe.docid = doc.docid AND doc.esdid = " . ESDID_PC_CONTROLE_SOCIAL . "
              WHERE o.obrid = {$obrid}
              AND o.obridpai IS NULL
              AND o.obrstatus = 'A'";

    $obraPar = $db->pegaUm($sql);

    if ($obraPar > 0) {
        return true;
    } else {
        return false;
    }
}

function posAcaoRetornaPCCACSParaExecucao($obrid)
{

    global $db;

    $proid = $db->pegaUm("SELECT proid_par FROM obras2.execucaofinanceiracomposicao exc 
                        INNER JOIN obras2.execucaofinanceira ex ON exc.exeid = ex.exeid WHERE exc.obrid = {$obrid}");

    if ($proid) {

        //Trecho busca DOCID situaçao da PC caso nao tenha cria um docid
        $docidSituacaoFluxo = $db->pegaUm("SELECT docid FROM obras2.situacaoprestacaocontasobras WHERE proid_par = {$proid}");

        if ($docidSituacaoFluxo) {

            $esdidSituacao = wf_pegarEstadoAtual($docidSituacaoFluxo);
            $esdidSituacao = $esdidSituacao['esdid'];

            // A Tramitação 1 diz respeito a SITUAÇÃO DA PRESTAÇÃO DE CONTAS
            if ($esdidSituacao == ESDID_OBRAS_SPC_CONTROLE_SOCIAL) {

                $tramitacao1 = wf_alterarEstado($docidSituacaoFluxo, AEDID_OBRAS_SPC_DE_CONTROLE_SOCIAL_PARA_NAOEVIADA,
                    "Retornado para 'Nao enviada' através da funcionalidade 'Retornar para Execuçao Financeira' ",
                    array());

            } else {
                $tramitacao1 = false;
            }

            $exeid = $db->pegaUm("SELECT exeid FROM obras2.execucaofinanceiracomposicao WHERE obrid = {$obrid}");

            if ($exeid) {
                $sqlFinalizado = "UPDATE obras2.execucaofinanceiracomposicao SET excfinalizada = FALSE WHERE exeid = {$exeid}";
                $tramitacao2 = $db->executar($sqlFinalizado);
            } else {
                $tramitacao2 = false;
            }

            if ((!$tramitacao1) || (!$tramitacao2)) {
                die('Erro na tramitação do fluxo de prestação de contas');
            } else {
                $db->commit();
            }
        }

    } else {
        die('Erro na tramitação do fluxo de prestação de contas');
    }

}

function condicaoAcaoRetornaPCAnaliseParaExecucao($obrid)
{
    global $db;

    $sql = "SELECT
              COUNT(*)
            FROM obras2.obras o
            INNER JOIN par.termoobra pto ON pto.preid = o.preid AND o.obridpai IS NULL
            INNER JOIN par.termocompromissopac ptp ON ptp.terid = pto.terid AND ptp.terstatus = 'A'
            INNER JOIN obras2.execucaofinanceiracomposicao exc ON o.obrid = exc.obrid
            INNER JOIN obras2.execucaofinanceira exe ON exc.exeid = exe.exeid
            INNER JOIN workflow.documento doc ON exe.docid = doc.docid AND doc.esdid = " . ESDID_PC_ANALISE . "
              WHERE o.obrid  = {$obrid}
    AND o.obridpai IS NULL
    AND o.obrstatus = 'A'";

    $obraPac = $db->pegaUm($sql);

    if ($obraPac > 0) {
        return true;
    } else {
        return false;
    }

}

function posAcaoRetornaPCAnaliseParaExecucao($obrid)
{

    global $db;

    $proid = $db->pegaUm("SELECT proid_pac FROM obras2.execucaofinanceiracomposicao exc 
                        INNER JOIN obras2.execucaofinanceira ex ON exc.exeid = ex.exeid WHERE exc.obrid = {$obrid}");

    if ($proid) {

        //Trecho busca DOCID situaçao da PC caso nao tenha cria um docid
        $docidSituacaoFluxo = $db->pegaUm("SELECT docid FROM obras2.situacaoprestacaocontasobras WHERE proid_pac = {$proid}");

        if ($docidSituacaoFluxo) {

            $esdidSituacao = wf_pegarEstadoAtual($docidSituacaoFluxo);
            $esdidSituacao = $esdidSituacao['esdid'];


            // A Tramitação 1 diz respeito a SITUAÇÃO DA PRESTAÇÃO DE CONTAS
            if ($esdidSituacao == ESDID_OBRAS_SPC_ANALISEFNDE) {

                $tramitacao1 = wf_alterarEstado($docidSituacaoFluxo, AEDID_OBRAS_SPC_DE_ANALISE_PARA_NAOEVIADA,
                    "Retornado para 'Nao enviada' através da funcionalidade 'Retornar para Execuçao Financeira' ",
                    array());

            } else {
                $tramitacao1 = false;
            }

            $exeid = $db->pegaUm("SELECT exeid FROM obras2.execucaofinanceiracomposicao WHERE obrid = {$obrid}");

            if ($exeid) {
                $sqlFinalizado = "UPDATE obras2.execucaofinanceiracomposicao SET excfinalizada = FALSE WHERE exeid = {$exeid}";
                $tramitacao2 = $db->executar($sqlFinalizado);
            } else {
                $tramitacao2 = false;
            }


            if ((!$tramitacao1) || (!$tramitacao2)) {
                die('Erro na tramitação do fluxo de prestação de contas');
            } else {
                $db->commit();
            }
        }

    } else {
        die('Erro na tramitação do fluxo de prestação de contas');
    }
}

function condicaoAcaoExecucaoFinanceiraEnviarAnaliseCACS($obrid)
{
    global $db;

    include_once APPRAIZ . "obras2/classes/modelo/ExecucaoFinanceira.class.inc";

    $execucaoFinanceira = new ExecucaoFinanceira();
    $processo = $execucaoFinanceira->retornarProcesso($obrid);

    $sql = "
SELECT  

            obrid,
            esdid,
            execucaonaofinalizada 
            
            FROM
                (
                    (
                        SELECT DISTINCT 
                        o.obrid,
                        wd.esdid,
                        COALESCE(exc.excfinalizada, FALSE) AS execucaonaofinalizada
            
                        FROM obras2.obras o 
                        INNER JOIN par.termoobra pto ON pto.preid = o.preid AND o.obridpai IS NULL
                        INNER JOIN par.termocompromissopac ptp ON ptp.terid = pto.terid AND ptp.terstatus = 'A' 
                        INNER JOIN workflow.documento wd ON wd.docid = o.docid
                        LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                        
                        WHERE o.obridpai IS NULL
                        AND o.obrstatus = 'A'
                        AND o.obrnumprocessoconv = '{$processo}'
                    )
                    UNION ALL
                    (
                        SELECT
                        o.obrid,
                        wd.esdid,
                        COALESCE(exc.excfinalizada, FALSE) AS execucaonaofinalizada
                        
                        FROM obras2.obras o
                        INNER JOIN par3.obra po ON po.obrid = o.obrid_par3 AND o.obridpai IS NULL
                        INNER JOIN par3.processoobracomposicao poc ON poc.obrid = po.obrid
                        INNER JOIN par3.processo pp ON pp.proid = poc.proid
                        INNER JOIN workflow.documento wd ON wd.docid = o.docid
                        LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                        
                        WHERE o.obridpai IS NULL
                        AND o.obrstatus = 'A'
                        AND o.obrnumprocessoconv = '{$processo}'
                    )
                    UNION ALL
                    (
                        SELECT 
                        o.obrid,
                        wd.esdid,
                        COALESCE(exc.excfinalizada, FALSE) AS execucaonaofinalizada
                    
                        FROM obras2.obras o
                        INNER JOIN painel.dadosconvenios pd ON pd.dcoprocesso = o.obrnumprocessoconv AND o.obridpai IS NULL
                        INNER JOIN workflow.documento wd ON wd.docid = o.docid
                        LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                       
                        WHERE o.obridpai IS NULL 
                        AND o.obrstatus = 'A'
                        AND o.obrnumprocessoconv = '{$processo}'                        
                    )
                    UNION ALL
                    (
                        SELECT 
                        o.obrid,
                        wd.esdid,
                         COALESCE(exc.excfinalizada, FALSE) AS execucaonaofinalizada
                        
                        FROM obras2.obras o
                        INNER JOIN par.termocomposicao pto ON pto.preid = o.preid
                        INNER JOIN par.documentopar pd ON pd.dopid = pto.dopid AND pd.dopstatus = 'A'
                        INNER JOIN workflow.documento wd ON wd.docid = o.docid
                        LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A'
                        
                        WHERE o.obridpai IS NULL 
                        AND o.obrstatus = 'A'
                        AND o.obrnumprocessoconv = '{$processo}'                       
                    )
                    UNION ALL
                    (
                        SELECT  
                        o.obrid,
                        wd.esdid,
                        COALESCE(exc.excfinalizada, FALSE) AS execucaonaofinalizada
                        
                        FROM obras2.obras o
                        INNER JOIN workflow.documento wd ON wd.docid = o.docid
                        LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                        
                        WHERE o.obridpai IS NULL 
                        AND o.obrstatus = 'A'
                        AND o.obrnumprocessoconv = '{$processo}'  
                    )
                ) AS foo
                GROUP BY 
                  obrid,
                  esdid,
            	  execucaonaofinalizada ";

    $result = $db->carregar($sql);
    $result = $result ? $result : array();

    $obrasNaoPermitidas = 0;
    $execucoesNaoFinalizada = 0;

    foreach ($result as $res) {
        $arrEstadosPermitidos = array(ESDID_OBJ_CONCLUIDO, ESDID_OBJ_CANCELADO, ESDID_OBJ_INACABADA);

        if (!in_array($res['esdid'], $arrEstadosPermitidos)) {
            $obrasNaoPermitidas++;
        }

        if ($res['execucaonaofinalizada'] == 'f') {
            $execucoesNaoFinalizada++;
        }
    }

    if ($obrasNaoPermitidas > 0 || $execucoesNaoFinalizada > 0) {
        return false;
    }

    return true;
}

function tramitarOPCparaAdimplentePAC($obrid)
{

    global $db;

    $sql = "SELECT spc.docid
        FROM obras2.obras o
        INNER JOIN par.processoobraspaccomposicao poc ON poc.preid = o.preid
        INNER JOIN obras2.situacaoopcobras spc ON spc.proid_pac = poc.proid
        WHERE o.obrid = {$obrid}";

    $result = $db->pegaLinha($sql);

    $estado_origem = wf_pegarEstadoAtual($result['docid']);
    if (!empty($estado_origem['esdid']) && $estado_origem['esdid'] == ESDID_OBRAS_OPC_INADIMPLENTE) {

        if (wf_alterarEstado($result['docid'], AEDID_OBRAS_OPC_DE_INADIMPLENTE_PARA_ADIMPLENTE,
            'Tramitado automaticamente via "Fase da Prestação de Contas"', array('obrid' => $obrid))) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }

}

function tramitarOPCparaAdimplente($obrid)
{

    global $db;
    $sql = "SELECT spc.docid
        FROM obras2.obras o
        INNER JOIN par.processoobrasparcomposicao poc ON poc.preid = o.preid
        INNER JOIN par.processoobraspar pro ON pro.proid = poc.proid AND pro.prostatus = 'A'
        INNER JOIN obras2.situacaoopcobras spc ON spc.proid_par = pro.proid
        WHERE o.obrid = {$obrid}";

    $result = $db->pegaLinha($sql);

    $estado_origem = wf_pegarEstadoAtual($result['docid']);

    if (!empty($estado_origem['esdid']) && $estado_origem['esdid'] == ESDID_OBRAS_OPC_INADIMPLENTE) {
        if (wf_alterarEstado($result['docid'], AEDID_OBRAS_OPC_DE_INADIMPLENTE_PARA_ADIMPLENTE,
            'Tramitado automaticamente via "Fase da Prestação de Contas"', array('obrid' => $obrid))) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }

}

function verificaObraSigpc($obrid, $processo)
{
    global $db;

    if ($obrid && empty($processo)) {
        $sqlSigpc = "SELECT
              tspid
            FROM
              obras2.execucaofinanceiracomposicao ec
            INNER JOIN
              obras2.execucaofinanceira e ON ec.exeid = e.exeid AND e.exestatus = 'A'
            INNER JOIN
              par.termossigpcpar t ON (e.proid_par = t.proid_par) OR (e.proid_pac = t.proid_pac)
            WHERE ec.obrid = {$obrid} AND ec.excstatus = 'A' ";
    } elseif (empty($obrid) && $processo) {
        $processo = trim($processo);
        $sqlSigpc = "SELECT tspid FROM par.termossigpcpar WHERE tspnumeroprocesso = '{$processo}' AND prpid ISNULL";
    } else {
        return false;
    }


    $existeSigpc = $db->pegaUm($sqlSigpc);

    if ($existeSigpc) {
        return true;
    } else {
        return false;
    }
}

function buscaItemExecutadoIntegralmente($sbdid){
    global $db;

    $sql = "SELECT * FROM
              (SELECT DISTINCT
              si.icoid,
              si.icodescricao,
              (CASE WHEN sbacronograma = 1
                THEN
                  (coalesce(si.icoquantidadetecnico, 0))
               ELSE (
                   SELECT DISTINCT CASE WHEN (s.frmid = 2) OR (s.frmid = 4 AND s.ptsid = 42) OR (s.frmid = 12 AND s.ptsid = 46)
                     THEN -- escolas sem itens
                       sum(coalesce(se.sesquantidadetecnico, 0))
                                   ELSE -- escolas com itens
                                     CASE WHEN sic.icovalidatecnico = 'S'
                                       THEN -- validado (caso não o item não é contado)
                                         sum(coalesce(ssi.seiqtdtecnico, 0))
                                     END
                                   END AS vlrsubacao
                   FROM entidade.entidade t
                     INNER JOIN entidade.funcaoentidade f ON f.entid = t.entid
                     LEFT JOIN entidade.entidadedetalhe ed ON t.entid = ed.entid
                     INNER JOIN entidade.endereco d ON t.entid = d.entid
                     LEFT JOIN territorios.municipio m ON m.muncod = d.muncod
                     LEFT JOIN par.escolas e ON e.entid = t.entid
                     INNER JOIN par.subacaoescolas se ON se.escid = e.escid
                     INNER JOIN par.subacaoitenscomposicao sic
                       ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
                     LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
                   WHERE
                     sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
                     AND sic.icoid = si.icoid
                     AND (t.entescolanova = FALSE OR t.entescolanova IS NULL) AND t.entstatus = 'A' AND f.funid = 3
                   GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico)
               END)                AS quantidade,
              si.icovalor          AS valorreferencia,
              coalesce(si.icoquantidaderecebida,0)  AS quantidaderecebida,
              coalesce(si.icovaloraquisicao,0) AS valoraquisicao,
              '<img src=\"../imagens/obras/check.png\" title=\"\" width=\"18\">' as objetivo,              
              '<img src=\"../imagens/obras/check.png\" title=\"\" width=\"18\">' as valor             
            FROM par.subacao s
              INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
              INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
              INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
              INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
            WHERE
              sd.sbdid = {$sbdid}
              AND si.icovalidatecnico = 'S'
              AND icomonitoramentocancelado = FALSE
            ORDER BY
              si.icodescricao) as foo WHERE foo.valorreferencia = foo.valoraquisicao AND foo.quantidade = foo.quantidaderecebida 
              AND foo.quantidade > 0";

    $arrItens = $db->carregar($sql);
    $arrItens = $arrItens ? $arrItens : array();

    return $arrItens;
}

function buscaItemExecutadoDiferentePactuado($sbdid){
    global $db;

    $sql = "SELECT 
                *,
              CASE WHEN quantidade > quantidaderecebida THEN 
                    '<img src=\"../imagens/seta_vermelho_baixo.png\" title=\"\" width=\"18\">'
                  WHEN quantidade < quantidaderecebida THEN
                    '<img src=\"../imagens/seta_verde.png\" title=\"\" width=\"18\">'
                  ELSE 
                    '<img src=\"../imagens/obras/check.png\" title=\"\" width=\"18\">'
              END as objetivo,
              CASE WHEN valorreferencia > valoraquisicao THEN 
                    '<img src=\"../imagens/seta_vermelho_baixo.png\" title=\"\" width=\"18\">'
                  WHEN valorreferencia < valoraquisicao THEN
                    '<img src=\"../imagens/seta_verde.png\" title=\"\" width=\"18\">'
                  ELSE 
                    '<img src=\"../imagens/obras/check.png\" title=\"\" width=\"18\">'
              END as valor
            FROM
              (SELECT DISTINCT
              si.icoid,
              si.icodescricao,
              (CASE WHEN sbacronograma = 1
                THEN
                  (coalesce(si.icoquantidadetecnico, 0))
               ELSE (
                   SELECT DISTINCT CASE WHEN (s.frmid = 2) OR (s.frmid = 4 AND s.ptsid = 42) OR (s.frmid = 12 AND s.ptsid = 46)
                     THEN -- escolas sem itens
                       sum(coalesce(se.sesquantidadetecnico, 0))
                                   ELSE -- escolas com itens
                                     CASE WHEN sic.icovalidatecnico = 'S'
                                       THEN -- validado (caso não o item não é contado)
                                         sum(coalesce(ssi.seiqtdtecnico, 0))
                                     END
                                   END AS vlrsubacao
                   FROM entidade.entidade t
                     INNER JOIN entidade.funcaoentidade f ON f.entid = t.entid
                     LEFT JOIN entidade.entidadedetalhe ed ON t.entid = ed.entid
                     INNER JOIN entidade.endereco d ON t.entid = d.entid
                     LEFT JOIN territorios.municipio m ON m.muncod = d.muncod
                     LEFT JOIN par.escolas e ON e.entid = t.entid
                     INNER JOIN par.subacaoescolas se ON se.escid = e.escid
                     INNER JOIN par.subacaoitenscomposicao sic
                       ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
                     LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
                   WHERE
                     sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
                     AND sic.icoid = si.icoid
                     AND (t.entescolanova = FALSE OR t.entescolanova IS NULL) AND t.entstatus = 'A' AND f.funid = 3
                   GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico)
               END)                AS quantidade,
              si.icovalor          AS valorreferencia,
              coalesce(si.icoquantidaderecebida,0)  AS quantidaderecebida,
              coalesce(si.icovaloraquisicao,0) AS valoraquisicao             
            FROM par.subacao s
              INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
              INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
              INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
              INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
            WHERE
              sd.sbdid = {$sbdid}
              AND si.icovalidatecnico = 'S'
              AND icomonitoramentocancelado = FALSE
            ORDER BY
              si.icodescricao) as foo WHERE (foo.valorreferencia <> foo.valoraquisicao OR foo.quantidade <> foo.quantidaderecebida) 
              AND foo.quantidaderecebida > 0";

    $arrItens = $db->carregar($sql);
    $arrItens = $arrItens ? $arrItens : array();

    return $arrItens;
}

function buscaItemNaoExecutado($sbdid){
    global $db;

    $sql = "SELECT DISTINCT
              si.icoid,
              si.icodescricao,
              (CASE WHEN sbacronograma = 1
                THEN
                  (coalesce(si.icoquantidadetecnico, 0))
               ELSE (
                   SELECT DISTINCT CASE WHEN (s.frmid = 2) OR (s.frmid = 4 AND s.ptsid = 42) OR (s.frmid = 12 AND s.ptsid = 46)
                     THEN -- escolas sem itens
                       sum(coalesce(se.sesquantidadetecnico, 0))
                                   ELSE -- escolas com itens
                                     CASE WHEN sic.icovalidatecnico = 'S'
                                       THEN -- validado (caso não o item não é contado)
                                         sum(coalesce(ssi.seiqtdtecnico, 0))
                                     END
                                   END AS vlrsubacao
                   FROM entidade.entidade t
                     INNER JOIN entidade.funcaoentidade f ON f.entid = t.entid
                     LEFT JOIN entidade.entidadedetalhe ed ON t.entid = ed.entid
                     INNER JOIN entidade.endereco d ON t.entid = d.entid
                     LEFT JOIN territorios.municipio m ON m.muncod = d.muncod
                     LEFT JOIN par.escolas e ON e.entid = t.entid
                     INNER JOIN par.subacaoescolas se ON se.escid = e.escid
                     INNER JOIN par.subacaoitenscomposicao sic
                       ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano AND sic.icostatus = 'A'
                     LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
                   WHERE
                     sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
                     AND sic.icoid = si.icoid
                     AND (t.entescolanova = FALSE OR t.entescolanova IS NULL) AND t.entstatus = 'A' AND f.funid = 3
                   GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico)
               END)                AS quantidade,
              si.icovalor          AS valorreferencia,
              coalesce(si.icoquantidaderecebida,0)  AS quantidaderecebida,
              coalesce(si.icovaloraquisicao,0) AS valoraquisicao,
              '<img src=\"../imagens/seta_vermelho_baixo.png\" title=\"\" width=\"18\">' as objetivo,
              '<img src=\"../imagens/seta_vermelho_baixo.png\" title=\"\" width=\"18\">' as valor
            FROM par.subacao s
              INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
              INNER JOIN par.termocomposicao tc ON tc.sbdid = sd.sbdid
              INNER JOIN par.subacaoitenscomposicao si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND si.icostatus = 'A'
              INNER JOIN par.propostaitemcomposicao pic ON pic.picid = si.picid
            WHERE
              sd.sbdid = {$sbdid}
              AND si.icovalidatecnico = 'S'
              AND (icomonitoramentocancelado = TRUE OR si.icoquantidaderecebida = 0 OR si.icoquantidaderecebida ISNULL)
            ORDER BY
              si.icodescricao";

    $arrItens = $db->carregar($sql);
    $arrItens = $arrItens ? $arrItens : array();

    return $arrItens;
}

function buscaItemNaoPrevisto($sbaid, $prpid){
    global $db;

    $sql = "SELECT
              inp.inpid as inpid,
              CASE WHEN inp.picid ISNULL THEN
                inp.inpnome
              ELSE
                pic.picdescricao
              END as nome,            
              CASE WHEN inp.inpquantidaderecebida IS NULL THEN
                '<center><p style=\"color:red;\"> Não monitorado </p></center>'
              ELSE
                inp.inpquantidaderecebida::varchar
              END
              as qtd_recebida,
              inp.inpvaloritem  as vlr_aquisicao
            FROM
              par.itemnaoprevistoacompanhamento inp
            LEFT JOIN
              par.propostaitemcomposicao pic ON inp.picid = pic.picid
            INNER JOIN 
              par.subacao s ON inp.sbaid = s.sbaid
            WHERE inp.inpstatus = 'A'
            AND inp.prpid = {$prpid}
            AND inp.sbaid = {$sbaid}";

    $arrItens = $db->carregar($sql);
    $arrItens = $arrItens ? $arrItens : array();

    return $arrItens;
}

function salvarImpugnacoes($dados) {
    
    global $db;
    //
    $atnid          = ($dados['atnid'])         ? $dados['atnid']               : FALSE;
    $atsid          = ($dados['atsid'])         ? $dados['atsid']               : FALSE;
    $atnobservacao  = ($dados['atnobservacao']) ? "'{$dados['atnobservacao']}'" : "NULL";
    $usucpf         = ($_SESSION['usucpf'])     ? "'{$_SESSION['usucpf']}'"     : "NULL";
    
    if ( (! $atsid) || (! $atnobservacao) ) {
        return Array("execucao" => FALSE);
    }
    
    $acao = ( $dados['atnid'] != "") ? "EDITAR" : "INSERIR";
    
    // Verifica se é para ver se é edição ou inclusão
    if( $acao == "EDITAR" ) {
        
        // Salvar Análise dos itens não previstos
        
        $sql = "
            UPDATE 
                par.analisetecitensnaoprevistos 
            SET
                atnobservacao = {$atnobservacao},
                atncpfedicao = {$usucpf},
                atndataedicao = 'now()'
            WHERE 
                atnid = {$atnid}
        ";
        
        if($db->executar($sql)) {
            
            $arrItensNaoPrevistos = Array();
            foreach($dados as $k => $v) {
            
                $inpid = "";
                $iipid = "";
                $campoComValores = "";
                $arrResult = Array();
                
                if( strpos($k, "impugnar_") !== false ) {
            
                    $campoComValores = str_replace("impugnar_","", $k);
                    
                    // verifico se existe o _ caso exista monta o array com ambos os campos
                    if( strpos($campoComValores, "_") !== false ) {
                        
                        $arrResult = explode('_', $campoComValores);
                        $inpid = $arrResult[0];
                        $iipid = $arrResult[1];
                        
                        $arrItensNaoPrevistos[$inpid]['inpid'] = $inpid;
                        $arrItensNaoPrevistos[$inpid]['valor'] = $v;
                        $arrItensNaoPrevistos[$inpid]['iipid'] = $iipid;
                        
                    } else { // Trata para caso seja um item novo (Nao deve ocorrer, mas pode ser uma exceção) 
                        
                        $inpid = str_replace("impugnar_","", $k);
                        $arrItensNaoPrevistos[$inpid]['inpid'] = $inpid;
                        $arrItensNaoPrevistos[$inpid]['valor'] = $v;
                    }
                }
            }
            
            if(count($arrItensNaoPrevistos) > 0) {
            
                foreach($arrItensNaoPrevistos as $key => $linha) {
            
                    if( $linha['iipid'] != "" && $linha['valor'] != "" && $atnid != "" ) {
            
                        $iipimpugnado   = ($linha['valor'] == "S") ? "TRUE" : "FALSE";
                        $valIipid       = $linha['iipid'];
                        $sqlItens .= "
                            UPDATE 
                                par.impugnacaoitensnaoprevistospc
                            SET
                                iipimpugnado = {$iipimpugnado}
                            WHERE
                                iipid = {$valIipid};
                        ";
                    } else if( $linha['inpid'] != "" && $linha['valor'] != "" && $atnid != "" ) {
            
                        $iipimpugnado = ($linha['valor'] == "S") ? "TRUE" : "FALSE";
            
                        $sqlItens .= "
                            INSERT INTO par.impugnacaoitensnaoprevistospc
                                (inpid, iipimpugnado, atnid)
                            VALUES
                                ({$linha['inpid']}, {$iipimpugnado}, {$atnid});
                        ";
                    }
            
                }
            
                if ($sqlItens != "") {
                    $db->executar($sqlItens);
                } else {
                    return Array("execucao" => FALSE);
                }
                
                if ($db->commit()) {
                    return Array("execucao" => TRUE);
                } else {
                    return Array("execucao" => FALSE);
                }
            
            } else {
            
                return Array("execucao" => FALSE);
                 
            }
        }
        
        
        // Salvar os itens impugnados
    } else {
        // Salvar Análise dos itens não previstos
        $sql = "
            INSERT INTO par.analisetecitensnaoprevistos
            (atsid, atnobservacao, atncpfinclusao )
            VALUES
            ({$atsid},{$atnobservacao}, {$usucpf} )
            RETURNING atnid;";
        $atnid = $db->pegaUm($sql);
        // Salvar os itens impugnados
        
        if ($atnid != "" ) {
            $arrItensNaoPrevistos = Array();
            foreach ($dados as $k => $v) {
                
                $inpid = "";
                if ( strpos($k, "impugnar_") !== false ) {
                    
                    $inpid = str_replace("impugnar_","", $k);
                    $arrItensNaoPrevistos[$inpid]['inpid'] = $inpid;
                    $arrItensNaoPrevistos[$inpid]['valor'] = $v;
                }
            }
            
            if(count($arrItensNaoPrevistos) > 0) {
                
                foreach($arrItensNaoPrevistos as $key => $linha) {
                    
                    if( $linha['inpid'] != "" && $linha['valor'] != "" && $atnid != "" ){
                    
                    $iipimpugnado = ($linha['valor'] == "S") ? "TRUE" : "FALSE";
                    
                    $sqlItens .= "   INSERT INTO par.impugnacaoitensnaoprevistospc
                            (inpid, iipimpugnado, atnid)
                        VALUES
                            ({$linha['inpid']}, {$iipimpugnado}, {$atnid});
                    ";
                    }
                    
                }   
                
                if ($sqlItens != "") {
                    $db->executar($sqlItens);
                } else {
                    return Array("execucao" => FALSE);
                }
                
                
            } else {
                return Array("execucao" => FALSE);
            } 
            
            if ( $db->commit() ) {
                return Array("execucao" => TRUE);
            } else {
                return Array("execucao" => FALSE);
            }
        } else {
        
            return Array("execucao" => FALSE);
             
        }  
    }
}

function retornaDadosAnaliseTecItensNaoPrevistos($dados){
    global $db;
    $atsid = $dados['atsid'];
    
    if($atsid != "") {
        $sql =   "SELECT 
        	atnid, 
        	atnobservacao
        FROM 
        	par.analisetecitensnaoprevistos 
        WHERE 
        	atsid = {$atsid}
      ";
        $analise = $db->pegaLinha($sql);
        $analise = (is_array($analise)) ? $analise : Array();
        
        if(count($analise) > 0) {
            
            $atnid = $analise['atnid'];

            if($atnid != "") {
                $sqlItens = 
                    "SELECT
                        inpid,
                        iipimpugnado,
                        iipid
                    from 
                        par.impugnacaoitensnaoprevistospc
                    WHERE 
                        atnid = {$atnid}";
                
                $dadosItensNaoPrevistos = $db->carregar($sqlItens);
                $dadosItensNaoPrevistos = (is_array($dadosItensNaoPrevistos)) ? $dadosItensNaoPrevistos : Array();
                $arrItensNaoPrevistos = Array();
                if(count($dadosItensNaoPrevistos) > 0 ) {
                    
                    foreach( $dadosItensNaoPrevistos as $k => $v) {
                        $arrItensNaoPrevistos[$v['inpid']] = $v;
                    }
                    
                }
            }
            
        }
        
        if( (count($dadosItensNaoPrevistos) > 0) && (count($analise)) ) {

            $dadosAnaliseNaoPrevista['existe'] = TRUE;
            $dadosAnaliseNaoPrevista['arrNaoPrevistos'] = $arrItensNaoPrevistos;
            $dadosAnaliseNaoPrevista['dadosAnalise'] = $analise;
        } else {
            $dadosAnaliseNaoPrevista['existe'] = FALSE;
        }
        
    } else {
        $dadosAnaliseNaoPrevista['existe'] = FALSE;
    }  
    return $dadosAnaliseNaoPrevista;
}

function montaTabelaItensImpugnados($prpid, $strSbaid, $arrSub = array())
{
    $arrSbaid = explode(',', $strSbaid);   
    $resultadoSubacoes = retornadadosItensImpugnadosPorSubacao($strSbaid, $prpid);
    
    if($resultadoSubacoes['existe']) {
        $tabela = "";
        // verifica se existe
        $arrSubacoes = $resultadoSubacoes['arrSubacoes'];
        $arrSubacoes = (is_array($arrSubacoes)) ? $arrSubacoes : Array();
        
        if(count($arrSubacoes) > 0) {
        
            foreach ($arrSubacoes as $sba) {
                $arrItens = Array();
                $tabela  .= "<table class='listagem' cellspacing='2' cellpadding='2' border='0' align='center' width='95%' >
                            <thead>
                                <tr align='center' style='background: #ccc; color: #000;'>
                                    <td colspan='5' style='font-size: 12px;'><b>Subação:</b> " . $sba['sbadsc'] . " </td>
                                </tr>
                                <tr align='center' >
                                    <td width='50%'> Item </td>
                                    <td width='10%'> Quantidade </td>    
                                    <td width='15%'> Valor </td>
                                    <td width='15%'> Valor Total </td>
                                    <td width='10%'> Condição </td>
                                </tr>
                            </thead>
                                <tbody>
                                        
                                        ";
                
                $arrItens = $sba['itens'];
                if( count($arrItens) > 0 ) {
                    foreach($arrItens as $key => $item) {
                        $tabela  .= "
                                <tr align='center'>
                                    <td> " . $item['descricao'] . " </td>
                                    <td> " . $item['quantidade'] . " </td>
                                    <td> " . $item['valor'] . " </td>
                                    <td> " . $item['valor_total'] . " </td>
                                    <td> Impugnado </td>
                                </tr>";
                    }
                }
                
                $tabela  .= "
                    <tbody>
                        </table><br>";
            }
        } else {
            if(is_array($arrSub) && count($arrSub) > 0 ){
                foreach ($arrSub as $subacao) {
                    $tabela .= "<table class='listagem' cellspacing='2' cellpadding='2' border='0' align='center' width='95%' >
                            <thead>
                                <tr align='center' style='background: #ccc; color: #000;'>
                                    <td colspan='5' style='font-size: 12px;'><b>Subação:</b> " . $subacao['sbadsc'] . " </td>
                                </tr>
                                <tr align='center' >
                                    <td width='50%'> Item </td>
                                    <td width='10%'> Quantidade </td>    
                                    <td width='15%'> Valor </td>
                                    <td width='15%'> Valor Total </td>
                                    <td width='10%'> Condição </td>
                                </tr>
                            </thead>
                                <tbody>
                                <tr> <td align=\"center\" style=\"color:#cc0000;\" colspan=\"5\"><center>Não foram encontrados registros de Itens Impugnados.</center></td></tr>
                                </tbody>
                                </table><br/><br/>";
                }
            } else {
                $tabela = '
                <table width="100%">
                    <tr> <td align="center" style="color:#cc0000;" ><center>Não foram encontrados registros de Itens Impugnados.</center></td></tr>
                </table>
                ';
            }
        }
        
    } else {

        if(is_array($arrSub) && count($arrSub) > 0 ){
            foreach ($arrSub as $subacao) {
                $tabela .= "<table class='listagem' cellspacing='2' cellpadding='2' border='0' align='center' width='95%' >
                            <thead>
                                <tr align='center' style='background: #ccc; color: #000;'>
                                    <td colspan='5' style='font-size: 12px;'><b>Subação:</b> " . $subacao['sbadsc'] . " </td>
                                </tr>
                                <tr align='center' >
                                    <td width='50%'> Item </td>
                                    <td width='10%'> Quantidade </td>    
                                    <td width='15%'> Valor </td>
                                    <td width='15%'> Valor Total </td>
                                    <td width='10%'> Condição </td>
                                </tr>
                            </thead>
                                <tbody>
                                <tr> <td align=\"center\" style=\"color:#cc0000;\" colspan=\"5\"><center>Não foram encontrados registros de Itens Impugnados.</center></td></tr>
                                </tbody>
                                </table><br/><br/>";
            }
        } else {
            $tabela = '
                <table width="100%">
                    <tr> <td align="center" style="color:#cc0000;" ><center>Não foram encontrados registros de Itens Impugnados.</center></td></tr>
                </table>
                ';
        }
        
    }
    return $tabela;

}
/**
 * @desc Retorna os dados dos itens impugnados e das suas respectivas subações
 * */
function retornadadosItensImpugnadosPorSubacao($strSbaid, $prpid) 
{
    global $db;
    $sql = "
        SELECT 
            sba.sbaid,
            sba.sbadsc,
            CASE WHEN inp.picid IS NOT NULL THEN
                pic.picdescricao
            ELSE
                inp.inpnome 
            END as descricao,
            CASE WHEN inp.inpquantidaderecebida IS NULL THEN
            	'<center><p style=\"color:red;\"> Não monitorado </p></center>'
            ELSE
            	inp.inpquantidaderecebida::VARCHAR
            END
            as quantidade,
            
            CASE WHEN inp.inpvaloritem IS NULL THEN
            	'<center>-</center>'
            ELSE
            	inp.inpvaloritem::varchar
            END
            as valor,
            coalesce((inp.inpquantidaderecebida * inp.inpvaloritem),0) as valor_total
        FROM 
        	par.impugnacaoitensnaoprevistospc iip
        INNER JOIN par.itemnaoprevistoacompanhamento inp ON inp.inpid = iip.inpid 
        INNER JOIN par.subacao sba ON sba.sbaid = inp.sbaid
        LEFT JOIN par.propostaitemcomposicao pic ON pic.picid = inp.picid
        WHERE 
        	sba.sbaid in ({$strSbaid}) 
        AND 
        	prpid = {$prpid}
        AND
	       inp.inpstatus = 'A'
        AND 
	       iip.iipimpugnado = TRUE
        ";
    
    $arrItens = $db->carregar($sql);
    $arrItens = (is_array($arrItens)) ? $arrItens : Array();
    $arrSubacoes = Array();
    
    if(count($arrItens) > 0 ) {
        foreach($arrItens as $k => $v ) {
            $arrSubacoes[$v['sbaid']]['sbaid']  = $v['sbaid'];
            $arrSubacoes[$v['sbaid']]['sbadsc'] = $v['sbadsc'];
            $arrSubacoes[$v['sbaid']]['itens'][] = $v;
        }
    }
    
    if( count($arrSubacoes) > 0 ){
        $arrRetorno['existe']       = TRUE;    
        $arrRetorno['arrSubacoes']  = $arrSubacoes;
    } else {
        $arrRetorno['existe']       = FALSE;
    }
    return $arrRetorno;
    
}

function buscaSbaidAnalise($sbdid, $prpid){
    global $db;

    $sql = "SELECT
    sd.sbaid
    FROM
    par.analisetecfnde atf
    INNER JOIN
    par.analisetecsubacao ats ON atf.atfid = ats.atfid
    INNER JOIN
    par.subacaodetalhe sd ON ats.sbdid = sd.sbdid
    WHERE atf.atfstatus = 'A'
    AND atf.atfid = (SELECT
    af.atfid
    FROM
    par.analisetecsubacao at
    INNER JOIN
    par.analisetecfnde af ON at.atfid = af.atfid
    WHERE at.sbdid = {$sbdid} AND af.prpid = {$prpid} AND at.atsstatus = 'A')";

    $arrSbaid = $db->carregar($sql);
    $arrSbaid = $arrSbaid ? $arrSbaid : array();
    $strSbaid = '';

    if(count($arrSbaid) > 0){
        foreach ($arrSbaid as $subacao){
            $strSbaid .= $subacao['sbaid'] . ",";
        }
    }

    $strSbaid = substr($strSbaid, 0, -1);

    return $strSbaid;
}


?>