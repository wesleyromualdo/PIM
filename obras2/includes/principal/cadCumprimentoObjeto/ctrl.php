<?php 
require APPRAIZ . 'obras2/includes/principal/cadCumprimentoObjeto/funcoes.php';

verificaSessao('orgao');
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int)($_GET['obrid'] ? $_GET['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
$obra = new Obras($_SESSION['obras2']['obrid']);
if ($obra->obrid_par3) {
    $sql = 'SELECT inuid, inpid FROM par3.obra WHERE obrid = ' . $obra->obrid_par3;
    $dados = $obra->carregar($sql);
    $inuid = $dados['inuid'];
    $inpid = $dados['inpid'];
}
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];

/* Verificando se a ação para adicionar uma nova construtora foi disparada. */
if (isset($_POST["req"]) && $_POST["req"] == "novaConstrutora") {
    $retorno = false;
    if ($_POST["razaosocial"] != "" && $_POST["cnpj"] && $obrid != "") {
        $cnpj = preg_replace("/[^\\d]/", "", $_POST["cnpj"]);

        if (cadastrarNovaConstrutora($obrid, ($_POST["razaosocial"]), $cnpj)) {
            $retorno = true;
        }
    }
    ob_end_clean();
    echo json_encode($retorno);
    exit;
}

if (isset($_POST['req']) && $_POST['req'] == 'novoPrazo') {
    if (gravarNovoPrazo($obrid, $_POST['novoprazo'])) {
        echo 'true';
        exit;
    }
    echo 'false';
}

/* Verificando se não houve um bloqueio/desbloqueio forçado (manual). */
if (!verificaBloqueioManual($obrid)) {
    $situacaoObra = verificaSituacaoObra($obrid);
    $co = new CumprimentoObjeto();

    $pagamentosEfetuados = $obra->getPagamentoPar($obrid);
    $pagamentoEfetivadoExists = false;

    if ($situacaoObra == 8) { // OBRA CANCELADA
        // VERIFICANDO SE A ORIGEM DA OBRA É 'CONVÊNIO'.
        if (verificaOrigemObra($obrid) == 2 || verificaNumeroConvenio($obrid) == true) {
            $pagamentoEfetivadoExists = true;
        } else {
            // VERIFICANDO SE EXISTE PELO MENOS UM PAGAMENTO EFETIVADO.
            if (is_array($pagamentosEfetuados)) {
                foreach ($pagamentosEfetuados as $pagamento) {
                    if (strpos($pagamento["pagsituacaopagamento"], "EFETIVADO") !== false) {
                        $pagamentoEfetivadoExists = true;
                        break;
                    }
                }
            }
        }

        if (!$pagamentoEfetivadoExists) { // SE NÃO HOUVER NENHUM PAGAMENTO EFETIVADO
            $co->bloqueiaCumprimentoObra($obrid);
        } else {
            $rgaid = verificaLeituraNotificacaoCumprimentoObjeto($obrid);
            if ($rgaid) {
                $co->liberaCumprimentoObra($obrid, false);
            } else {
                $co->liberaCumprimentoObra($obrid);
            }
        }
    } elseif ($situacaoObra == 7) { // OBRA INACABADA
        $rgaid = verificaLeituraNotificacaoCumprimentoObjeto($obrid);
        if ($rgaid) {
            $co->liberaCumprimentoObra($obrid, false);
        } else {
            $co->liberaCumprimentoObra($obrid);
        }
    } elseif ($situacaoObra == 6) { // OBRA CONCLUÍDA
        $vigencia = null;
        $situacaoEfetivada = false; // Variável que indica se a obra está 100% paga.

        if (verificaOrigemObra($obrid) == 2 || verificaNumeroConvenio($obrid) == true) { // CONVÊNIO
            // Para a origem 'Convênio' é assumido que a obra está 100% paga.
            $situacaoEfetivada = true;

            // Pesquisando na tabela 'registroatividades'.
            $dtvigencia = verificaLeituraNotificacaoObraConcluida($obrid);

            if ($dtvigencia) { // Se houver data de vigência na tabela 'registroatividades'...
                $dtAtual = new DateTime("now");

                // Obtendo a data da tabela 'registroatividades' e somando mais 45 dias.
                $dtvigencia = new DateTime($dtvigencia);
                $dtvigencia->add(new DateInterval("P45D"));

                // Verificando a vigência a partir da nova data (45 dias).
                if ($dtvigencia->diff($dtAtual) > 0) {
                    $vigencia = false;
                } else {
                    $vigencia = true;
                }
            } else {
                $dtfimconvenio = verificaDataFimConvenio($obrid);
                if ($dtfimconvenio) {
                    $dtAtual = new DateTime("now");
                    $dtfimconvenio = new DateTime($dtfimconvenio);

                    // Verificando se a vigência a partir da nova data (45 dias).
                    if ($dtfimconvenio->diff($dtAtual) > 0) {
                        $vigencia = false;
                    } else {
                        $vigencia = true;
                    }
                } else {
                    // Caso não haja data de vigência campo 'Fim' na aba 'Recursos' (para convênio) e nem na tabela 'registroatividades', bloqueia a aba 'Cumprimnento do objeto'.
                    $co->bloqueiaCumprimentoObra($obrid);
                }
            }
        } else { // PROCESSO
            // Verificando a data de vigência da aba "Recursos".
            $vigencia = verificaVigenciaObra($obrid);

            if ($vigencia === null) { // Se não existir data de vigência na aba recursos, pesquisar na tabela 'registroatividades'.
                // Pesquisando na tabela 'registroatividades'.
                $dtvigencia = verificaLeituraNotificacaoObraConcluida($obrid);

                if ($dtvigencia) { // Se houver data de vigência na tabela 'registroatividades'...
                    $dtAtual = new DateTime("now");

                    // Obtendo a data da tabela 'registroatividades' e somando mais 45 dias.
                    $dtvigencia = new DateTime($dtvigencia);

                    $dtvigencia->add(new DateInterval("P45D"));

                    // Verificando se a vigência a partir da nova data (45 dias).
                    if ($dtvigencia->diff($dtAtual) > 0) {
                        $vigencia = false;
                    } else {
                        $vigencia = true;
                    }
                } else {
                    // Caso não haja data de vigência na aba 'Recursos' e nem na tabela 'registroatividades', bloqueia a aba 'Cumprimnento do objeto'.
                    $co->bloqueiaCumprimentoObra($obrid);
                }
            }
        }

        if ($vigencia === false) { // Obra vencida.
            if ($pagamentosEfetuados) {
                $calc = 0;
                array_walk($pagamentosEfetuados, function ($v, $k) use (&$calc) {
                    if (strpos($v["pagsituacaopagamento"], "EFETIVADO") !== false) {
                        $percentual = round($v["percentualpagamento"], 2);
                        $calc += $percentual;
                    }
                }, $calc);

                if ($calc < 100) {
                    $situacaoEfetivada = false;
                } else {
                    $situacaoEfetivada = true;
                }
            }

            if ($situacaoEfetivada === false) {
                // Se a obra não for 100% efetivada, bloqueia-se a aba 'Cumprimento do objeto'.
                $co->bloqueiaCumprimentoObra($obrid);
            } else {
                // Se todas as previsões de providência estiverem vencidas, liberar a aba 'Cumprimento do objeto'.
                if (obterSituacaoPrevisaoProvidencia($obrid)) {
                    $rgaid = verificaLeituraNotificacaoCumprimentoObjeto($obrid);
                    if ($rgaid) {
                        $co->liberaCumprimentoObra($obrid, false);
                    } else {
                        $co->liberaCumprimentoObra($obrid);
                    }
                } else {
                    // Se pelo menos uma previsão de providência não estiver vencida, bloquear a aba 'Cumprimento do objeto'.
                    $co->bloqueiaCumprimentoObra($obrid);
                }
            }
        } elseif ($vigencia === true) { // Obra em vigência.
            if ($pagamentosEfetuados) {
                $calc = 0;
                array_walk($pagamentosEfetuados, function ($v, $k) use (&$calc) {
                    if (strpos($v["pagsituacaopagamento"], "EFETIVADO") !== false) {
                        $percentual = round($v["percentualpagamento"], 2);
                        $calc += $percentual;
                    }
                }, $calc);

                if ($calc < 100) {
                    $situacaoEfetivada = false;
                } else {
                    $situacaoEfetivada = true;
                }
            }

            if ($situacaoEfetivada === false) {
                // Se a obra não for 100% efetivada, bloqueia-se a aba 'Cumprimento do objeto'.
                $co->bloqueiaCumprimentoObra($obrid);
            } else {
                // Se todas as previsões de providência estiverem vencidas, liberar a aba 'Cumprimento do objeto'.
                if (obterSituacaoPrevisaoProvidencia($obrid)) {
                    $solicitacoes = capturaSolicitacoesObra($obrid);
                    $solUsoSaldoDeferida = false;
                    foreach ($solicitacoes as $solicitacao) {
                        if ($solicitacoes["aprovado"] == "S" && $solicitacoes["tslid"] == 2) {
                            $solUsoSaldoDeferida = true;
                            break;
                        }
                    }

                    // Se houver solicitação de uso de saldo deferida, bloqueia a aba 'Cumprimento do objeto'.
                    if ($solUsoSaldoDeferida) {
                        $co->bloqueiaCumprimentoObra($obrid);
                    } else {
                        if (verificaComposicaoConcluida($obrid)) {
                            $rgaid = verificaLeituraNotificacaoCumprimentoObjeto($obrid);
                            if ($rgaid) {
                                $co->liberaCumprimentoObra($obrid, false);
                            } else {
                                $co->liberaCumprimentoObra($obrid);
                            }
                        } else {
                            $co->bloqueiaCumprimentoObra($obrid);
                        }
                    }
                } else {
                    // Se pelo menos uma previsão de providência não estiver vencida, bloquear a aba 'Cumprimento do objeto'.
                    $co->bloqueiaCumprimentoObra($obrid);
                }
            }
        } else {
            $co->bloqueiaCumprimentoObra($obrid);
        }
    }
}

if ($_REQUEST['liberacoo'] == 'true') {
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        $co = new CumprimentoObjeto();
        $r = $co->liberaCumprimentoObra($obrid);

        $arDado = array();

        $arDado['obrid'] = $obrid;
        $arDado['rgaautomatica'] = 'true';
        $arDado['rgadscsimplificada'] = 'Cumprimento do Objeto liberado manualmente';
        $arDado['rgadsccompleta'] = 'Cumprimento do Objeto liberado manualmente pelo usuário: (' . $_SESSION['usucpf'] . ') ' . $_SESSION['usunome'] . '';
        $arDado['arqid'] = 'NULL';
        $arDado['usucpf'] = $_SESSION['usucpf'];

        $sql = "INSERT INTO obras2.registroatividade ( usucpf, arqid, obrid, rgaautomatica, rgadscsimplificada, rgadsccompleta) VALUES (
                  '{$arDado['usucpf']}',
                  {$arDado['arqid']},
                  {$arDado['obrid']},
                  {$arDado['rgaautomatica']},
                  '{$arDado['rgadscsimplificada']}',
                  '{$arDado['rgadsccompleta']}'
                  )";

        $db->executar($sql);
        $ret = $db->commit(); 
        
    }
    echo "<script>alert('Cumprimento do objeto liberado com sucesso!');window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
    exit;
}

if ($_REQUEST['bloqueiacoo'] == 'true') {
    if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
        $co = new CumprimentoObjeto();
        $r = $co->bloqueiaCumprimentoObra($obrid);

        if ($r) {
            $arDado = array();

            $arDado['obrid'] = $obrid;
            $arDado['rgaautomatica'] = 'true';
            $arDado['rgadscsimplificada'] = 'Cumprimento do Objeto bloqueado manualmente';
            $arDado['rgadsccompleta'] = 'Cumprimento do Objeto bloqueado manualmente pelo usuário: (' . $_SESSION['usucpf'] . ') ' . $_SESSION['usunome'] . '';
            $arDado['arqid'] = 'NULL';
            $arDado['usucpf'] = $_SESSION['usucpf'];

            $sql = "INSERT INTO obras2.registroatividade ( usucpf, arqid, obrid, rgaautomatica, rgadscsimplificada, rgadsccompleta) VALUES (
                      '{$arDado['usucpf']}',
                      {$arDado['arqid']},
                      {$arDado['obrid']},
                      {$arDado['rgaautomatica']},
                      '{$arDado['rgadscsimplificada']}',
                      '{$arDado['rgadsccompleta']}'
                      )";

            $db->executar($sql);
            $db->commit();

            echo "<script>alert('Ação realizada com sucesso!');window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
        } else {
            echo "<script>alert('Não foi possível realizar essa ação! Contate um administrador do sistema.');window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
        }
    } else {
        echo "<script>alert('Ação não permitida!');window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
    }
    exit;
}

if ($_REQUEST['download'] == 'S') {
    $questaoCumprimentoArquivo = new QuestaoCumprimentoArquivo();
    $questaoCumprimentoArquivo->download($_REQUEST['arqid']);
    echo "<script>window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
    exit;
}

if ($_REQUEST['excluir'] == 'S') {
    $questaoCumprimentoArquivo = new QuestaoCumprimentoArquivo();
    $questaoCumprimentoArquivo->excluir($_REQUEST['arqid']);
    echo "<script>alert('Arquivo excluido com sucesso!');window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
    exit;
}

if (isset($_REQUEST['req']) && $_REQUEST['req'] == 'salvar') {
    $cumprimentoObjetoDocumentacao = new CumprimentoObjetoDocumentacao();
    $cumprimentoObjetoDocumentacao->salvar($_REQUEST);
    echo "<script>alert('Arquivo incluído com sucesso!'); window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros, array());
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros, array());
}

include_once APPRAIZ . '/includes/workflow.php';

$habilPag = false;
$estadoWorkflowObra = wf_pegarEstadoAtual($obra->docid);

if ($estadoWorkflowObra) {
    $cumprimentoObjeto = new CumprimentoObjeto();

    if (!$cumprimentoObjeto->obrasPermitidas($obra->obrid)) {
        if (!possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
            echo "<script>alert('No momento, a aba \"Cumprimento do Objeto\" não se encontra liberada para esta obra.'); window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A';</script>";
            die();
        }
    } else {
        $habilPag = true;

        $coid = $cumprimentoObjeto->verificaExistencia($obrid);
        $wfEstadoAtualCumprimento = wf_pegarEstadoAtual($cumprimentoObjeto->docid);
        $cumprimentoObjetoDocumentacao = new CumprimentoObjetoDocumentacao();
        $cumprimento = $cumprimentoObjetoDocumentacao->montaQuestionario($obrid);

        $habilitado = false;
        if (possui_perfil($pflcods)) {
            $habilitado = true;
        }
        print cabecalhoObra($obrid);
        $cumprimentoObjeto->criaSubAba($url, $habilitado, $obrid, $wfEstadoAtualCumprimento['esdid']);
    }
}





$arrEsclarecimentoObra = array(
    1 => 'Projeto aprovado pelo FNDE, a ser executado.',
    2 => 'Memorial e Especificações referentes ao Projeto aprovado pelo FNDE.',
    3 => 'Planilha Orçamentária referente ao Projeto aprovado pelo FNDE.',
    4 => 'Vistoria final da obra executada, inserida pela órgão responsável;',
    5 => 'Documento comprobatório de que a obra foi entregue conforme estabelecido em contrato com a empresa executora;',
    6 => 'Pendências listadas em aba específica da obra, que devem ser superadas/justificadas;',
    7 => 'Documento comprobatório de que o objeto referido foi integralmente realizado conforme pactuado com o FNDE;',
    8 => 'Certidão de Registro de Imóveis recente e atualizada do terreno onde foi executada a obra;',
    9 => 'Valores já devolvidos a União via GRU;',
    10 => 'Documento elaborado pelo órgão responsável quando da solicitação de recursos ao FNDE para a execução da obra;'
);

$arrEsclarecimentoConstrutora = array(
    1 => 'Contrato firmado entre o órgão responsável e a empresa vencedora da licitação para a execução da obra;',
    2 => 'Documento único que reúne todas as medições efetuadas durante a execução da obra;',
    3 => 'Planilha Orçamentária apresentada pela empresa vencedora da licitação para a execução da obra;',
    4 => 'Comprovantes fiscais das respectivas medições realizadas durante a execução da obra;',
    5 => 'Contratos financeiros adicionais firmados entre o órgão responsável e a empresa executora da obra;'
);

$arrSituacaoDoItem = array(
    1 => 'Conferir se os documentos aqui disponibilizados contemplam todas as construtoras que executaram a obra. Caso haja ausência de algum deles, inserí-los no campo.',
    2 => 'Conferir se os documentos aqui disponibilizados contemplam todas as construtoras que executaram a obra, inclusive dos aditivos de valor. Caso haja ausência de alguma delas, inserí-las no campo.',
    3 => 'Conferir se os documentos aqui disponibilizados contemplam todas as construtoras que executaram a obra, inclusive dos aditivos de valor. Caso haja ausência de alguma delas, inserí-las no campo.',
    4 => 'Conferir se os documentos aqui disponibilizados contemplam todas as construtoras que executaram a obra, inclusive dos aditivos de valor. Caso haja ausência de alguma delas, inserí-las no campo.',
    5 => 'Inserir o(s) Contrato(s) do(s) Aditivo(s) e sua(s) Planilha(s) Orçamentária(s).'
);