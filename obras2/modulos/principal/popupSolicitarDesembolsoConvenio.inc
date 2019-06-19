<?php

ini_set("memory_limit", "512M");
ini_set('max_execution_time', 80);

$processo              = $_GET['processo'];
$convenio              = $_GET['convenio'];
$obrid                 = $_GET['obrid'];
$habilitado            = true;
$habilita              = 'S';
$solicitacaoDesembolso = new SolicitacaoDesembolsoConvenio();
$dadosConvenio         = $solicitacaoDesembolso->pegaDadosConvenioPorProcesso($processo);
$obrasConvenio         = $solicitacaoDesembolso->pegaObrasPorProcesso($processo);
$pflcods               = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE);
$pendenciaObras        = false;
$validacaoMsg          = array();

if(!possui_perfil( $pflcods )){
    $habilitado = false;
    $habilita   = 'N';
}

if($_GET['download']){
    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $obraArquivo = new ObrasArquivos();
    $arDados     = $obraArquivo->buscaDadosPorArqid($_GET['download']);
    $eschema     = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');
    $file        = new FilesSimec(null, null, $eschema);

    $file->getDownloadArquivo( $_GET['download'] );

    die('<script type="text/javascript">
			window.close();
		  </script>');
}


if($_POST){
    $dados = $_POST;
    $solicitacaoDesembolso = new SolicitacaoDesembolsoConvenio();

    if ($dados['sdcid']) {
        $solicitacaoDesembolso->carregarPorId($dados['sdcid']);
    } else {

        $sql = "
            SELECT
                COUNT(*)
            FROM obras2.solicitacao_desembolso_convenio sv
            JOIN workflow.documento d ON d.docid = sv.docid
            WHERE d.esdid NOT IN(".ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO.", ".ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO.") AND sv.sdcstatus = 'A' AND sv.processo = '$processo'";

        if ($db->pegaUm($sql) > 0) {
            echo '<script type="text/javascript">
                        alert("Solicitação enviada com sucesso.");
                        window.close();
                  </script>';
            exit;
        }

        $solicitacaoDesembolso->docid = wf_cadastrarDocumento(TPDID_SOLICITACAO_DESEMBOLSO, 'Fluxo da Solicitação de Desembolso - Convênio');
        $solicitacaoDesembolso->usucpf = $_SESSION['usucpf'];
        $solicitacaoDesembolso->commit();
    }

    $solicitacaoDesembolso->processo = $processo;
    $solicitacaoDesembolso->convenio = $convenio;
    $solicitacaoDesembolso->sdcjustificativa = $dados['sdcjustificativa'];
    $solicitacaoDesembolso->sdcobs = $dados['sdcobs'];

    $solicitacaoDesembolso->sdcpercsolicitado = desformata_valor($dados['sdcpercsolicitado']);
    $solicitacaoDesembolso->sdcparcela = $dados['sdcparcela'];
    $solicitacaoDesembolso->sdcpercpagamento = desformata_valor($dados['sdcpercpagamento']);
    $solicitacaoDesembolso->sdcobstec = $dados['sdcobstec'];

    $solicitacaoDesembolso->salvar();
    $solicitacaoDesembolso->commit();

   if ($_FILES['arquivo']['name'][0] != '') {

        require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $arquivos = $_FILES;

        foreach ($arquivos['arquivo']['name'] as $key => $value) {

            if(empty($value))
                continue;

            $files = array(
                'arquivo' => array(
                    'name' => $arquivos['arquivo']['name'][$key],
                    'type' => $arquivos['arquivo']['type'][$key],
                    'tmp_name' => $arquivos['arquivo']['tmp_name'][$key],
                    'error' => $arquivos['arquivo']['error'][$key],
                    'size' => $arquivos['arquivo']['size'][$key],
                )
            );
            $_FILES = $files;
            $file = new FilesSimec('arquivo', null, 'obras2');
            $file->setPasta('obras2');
            $file->setUpload($dados['arquivodescricao'][$key], 'arquivo', false);
            $arqid = $file->getIdArquivo();


            if ($arqid) {
                $_POST['arquivo'][] = $arqid;

                $arquivoSolicitacao = new SolicitacaoDesembolsoConvenioArquivos();
                $arquivoSolicitacao->sdcid = $solicitacaoDesembolso->sdcid;
                $arquivoSolicitacao->arqid = $arqid;
                $arquivoSolicitacao->sdastatus = 'A';
                $arquivoSolicitacao->salvar();
            }
        }

        verificaArqid($solicitacaoDesembolso->sdcid, $_POST['arquivo']);

    }
    $solicitacaoDesembolso->commit();

    if ($_POST['tramitacao'] == 1){
        wf_alterarEstado($solicitacaoDesembolso->docid, AEDID_SOLICITACAO_DESEMBOLSO_CORRECAO_PARA_ANALISE, '', array('sldid' => '', 'obrid' => '', 'processo' => $processo, 'sdcid' => $solicitacaoDesembolso->sdcid));
    }

    echo '<script type="text/javascript">
            alert("Solicitação enviada com sucesso.");
            document.location.href = "/obras2/obras2.php?modulo=principal/popupSolicitarDesembolsoConvenio&acao=A&obrid='.$obrid.'&processo='.$processo.'&convenio='.$convenio.'" + "&sdcid=' . $solicitacaoDesembolso->sdcid . '";
      </script>';
    exit;
    extract($dados);
}

if($processo) {

    $sql = "
            SELECT
                COUNT(*)
            FROM obras2.solicitacao_desembolso_convenio sv
            JOIN workflow.documento d ON d.docid = sv.docid
            WHERE d.esdid NOT IN(".ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO.", ".ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO.") AND sv.sdcstatus = 'A' AND processo = '$processo'";

    if ($db->pegaUm($sql) > 0) {
        $habilitado = false;
        $habilita = 'N';
        $msgB = '<p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> Este convênio possui uma solicitação em análise, aguarde a conclusão da solicitação para solicitar novamente.</p>';
    } else {

        // Verifica se possui uma solicitação DEFERIDA e se o percentual é menor que o atual
        $sql = "
            SELECT
                sdcpercpagamento
            FROM obras2.solicitacao_desembolso_convenio sv
            JOIN workflow.documento d ON d.docid = sv.docid
            WHERE d.esdid IN(".ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO.") AND sv.sdcstatus = 'A' AND processo = '$processo'
            ORDER BY sv.sdcid DESC";
        $ultPercSolicitacao = $db->pegaUm($sql);
        $percAtual = $solicitacaoDesembolso->pegaPercentualExecucao($processo);

        if($percAtual <= $ultPercSolicitacao){
            $habilitado = false;
            $habilita = 'N';
            $msgB .= '<p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> Já existe uma solicitação referente ao último percentual de execução. Para fazer uma nova solicitação é necessário um percentual maior.</p>';
        }


        if($ultPercSolicitacao > 25 && $ultPercSolicitacao < 50){
            $habilitado = false;
            $habilita = 'N';
            $msgB .= '<p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> É necessário atingir um maior que 50% para solicitar a 3ª parcela.</p>';
        }
    }
}


if($_REQUEST['sdcid']){
    $solicitacaoDesembolso->carregarPorId($_REQUEST['sdcid']);
    extract($solicitacaoDesembolso->getDados());
    $estado     = wf_pegarEstadoAtual($docid);
    $habilitado = false;
    $habilita   = 'N';

    if($estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO){
        $habilitado = true;
        $habilita   = 'S';
    }
}

echo '<br />';
monta_titulo( 'Solicitação de Desembolso', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

?>

<html>
<head>
    <title>Solicitação de Desembolso</title>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
</head>

<body topmargin="0" leftmargin="0">


<form method="post" id="formulario" name="formulario" enctype="multipart/form-data">
    <input type="hidden" name="tramitacao" value="" />
    <input type="hidden" name="sdcid" value="<?=$sdcid?>" />
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <?
        $s = new SolicitacaoDesembolso();

        if($obrid) {
            $obrasVermelho = $s->verificaObrasEmVermelho($obrid);
            if ($obrasVermelho):
                $habilitado = false;
                $habilita   = 'N';
                $msgB       = '<p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> O seu município possui obras desatualizadas. É necessário que todas as obras estejam atualizadas para fazer a solicitação de desembolso.</p>';
            endif;
        }


        ?>
        <tr id="quadro-pendencias-topo">

        </tr>
            <? if($sdcid): ?>
            <tr>
                <td class="SubTituloDireita" width="20%">Situação da solicitação:</td>
                </td>
                <td>
                    <? if (possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))): ?>
                         <b><?=$estado['esddsc']?></b>
                    <? else: ?>
                        <? if ($estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO) : ?>
                            <b style="color:#00AA00"><?=$solicitacao['esddsc']?></b>
                        <? elseif ( $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO) : ?>
                            <b style="color:red"><?=$estado['esddsc']?></b>
                        <? elseif ( $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO) : ?>
                            <b><?=$estado['esddsc']?></b>
                        <? else: ?>
                            <b style="">Aguardando Análise FNDE</b>
                        <? endif; ?>
                    <? endif; ?>
                </td>
            </tr>
        <? endif; ?>

        <tr>
            <td class="SubTituloDireita" width="20%">Vencimento do Convênio:</td>
            </td>
            <td>
                <?

                ?>
                <?php echo campo_texto('fimvigencia', 'S', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', formata_data($dadosConvenio['dcodatafim'])); ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">Percentual Execução:</td>
            </td>
            <td>
                <?
                $sdcpercsolicitado = ($sdcpercsolicitado) ? $sdcpercsolicitado : $solicitacaoDesembolso->pegaPercentualExecucao($processo);
                ?>
                <?php echo campo_texto('sdcpercsolicitado', 'S', 'N', '', 7, 6, '', '', 'right', '', 0, '', '', number_format($sdcpercsolicitado, 2, ',', '.')); ?>
            </td>

        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">Checklist de solicitação:</td>
            </td>
            <td>
                <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center">
                    <tr>
                        <th style="width: 37px">Ação</th>
                        <th>Obras</th>
                        <th>Situação</th>
                        <th>% Execução</th>
                        <? if (true): ?>
                        <th>Pendências</th>
                        <? endif; ?>
                    </tr>

                    <? foreach($obrasConvenio as $obra): ?>


                        <?
                        // Verifica se cronograma da obra possui etapas vencidas
                        $sql = "SELECT
                                    COUNT(*)
                                FROM obras2.obras o
                                JOIN workflow.documento d ON d.docid = o.docid
                                JOIN obras2.cronograma c ON c.obrid = o.obrid AND c.crostatus = 'A'
                                JOIN obras2.itenscomposicaoobra ic ON ic.obrid = o.obrid AND ic.croid = c.croid AND ic.icostatus = 'A' AND ic.relativoedificacao = 'D'
                                JOIN obras2.itenscomposicao i ON i.itcid = ic.itcid AND i.itcstatus = 'A'
                                WHERE o.obrid = {$obra['obrid']} AND (ic.icodterminoitem < now() AND COALESCE(icopercexecutado,0) < 100) AND d.esdid NOT IN (".ESDID_OBJ_CONCLUIDO.")";

                        if ($db->pegaUm($sql) > 0) {
                            $habilitado = false;
                            $habilita = 'N';
                            $msgB .= '<p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> ('.$obra['obrid'].') '.$obra['obrnome'].' - Para fazer a solicitação é necessário atualizar as datas de início e término de cada etapa do cronograma, conforme previsão de execução física da obra. Clique <a href="#" onclick="abrePopupEditaCronograma('.$obra['obrid'].')">aqui</a> para editar os prazos do cronograma.</p>';
                        }
                        ?>

                        <? $pendencia = $solicitacaoDesembolso->verificaChecklistObra($obra['obrid']); ?>

                        <tr>
                            <td>
                                <img align="absmiddle" src="/imagens/icone_lupa.png" style="cursor: pointer" onclick="javascript: alterarObr(<?= $obra['obrid'] ?>);" title="Ver Obra" />
                                <img align="absmiddle" src="/imagens/edit_on.gif" style="cursor: pointer" title="Ver Checklist" obrid="<?= $obra['obrid'] ?>" class="obra-pendencia"/>
                            </td>
                            <td>
                                <div class="obra-pendencia" style="cursor: pointer" obrid="<?= $obra['obrid'] ?>" >(<?= $obra['obrid'] ?>) <?= $obra['obrnome'] ?></div>
                                <div class="checklist-obra" id="checklist-<?= $obra['obrid'] ?>" style="display:none">
                                    <?
                                    if(true): ?>
                                    <br/>
                                    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" align="center">
                                        <?
                                        $questionario = $solicitacaoDesembolso->pegaQuestionarioObra($obra['obrid']);
                                        $pendencia    = false;
                                        $msg          = '';
                                        ?>
                                        <? foreach($questionario as $questao): ?>
                                            <?
                                            $pendencia = ($questao['qsvresposta'] != 't') ? true : $pendencia;
                                            $pendenciaQuestao = (!empty($questao['pendencia'])) ? simec_htmlspecialchars(strip_tags($questao['pendencia'])) : simec_htmlspecialchars(strip_tags($questao['qstobs']));
                                            ?>
                                            <? if ($divisao != $questao['itcdsc']): $divisao = $questao['itcdsc']; $c++;?>
                                                <tr>
                                                    <th colspan="4"><?=$c?> - <?=$questao['itcdsc']?></th>
                                                </tr>
                                            <? endif; ?>
                                            <tr>
                                                <td><span><?=$questao['qstnumero']?></span>) <?=$questao['qstdsc']?></td>
                                                <td><input disabled="true" type="radio" value="t" <?= ($questao['qsvresposta'] == 't')?'checked':''; ?> > SIM </td>
                                                <td><input disabled="true" type="radio" value="f" <?= ($questao['qsvresposta'] == 'f' || empty($questao['qsvresposta']))?'checked':''; ?> > NÃO <?= ($questao['vdrobrigatorio'] == 't') ? '<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">' : '' ?></td>
                                                <td><img title="<?=$pendenciaQuestao?>" src="/imagens/<?= ($questao['qsvresposta'] == 't') ? 'valida1.gif' : 'valida5.gif'; ?>" /></td>
                                            </tr>
                                            <input type="hidden" value="<?= $questao['qsvresposta'] ?>" name="qsvresposta[<?= $questao['qstid'] ?>]"/>
                                            <?
                                            if($obra['esdid'] == 693) continue;
                                            $msg .= ' ' . $pendenciaQuestao;
                                            $pendenciaObras = ($questao['qsvresposta'] != 't') ? true : $pendenciaObras;
                                            ?>
                                        <? endforeach; ?>
                                    </table>
                                    <? else:
                                        $pendencia = false;
                                    endif; ?>
                                </div>
                            </td>
                            <td>
                                <?=$obra['esddsc']?>
                            </td>
                            <td>
                                <?=$obra['percentual_execucao']?>
                            </td>
                            <? if (true): ?>
                                <td><center><img obrid="<?= $obra['obrid'] ?>"  title="<?=simec_htmlspecialchars($msg)?>" class="obra-pendencia" style="cursor: pointer" title="" src="/imagens/<?= (!$pendencia) ? 'valida1.gif' : 'valida5.gif'; ?>" /></center></td>
                            <? endif; ?>
                        </tr>
                    <? endforeach; ?>
                </table>
            </td>
            <?
            if ($pendenciaObras || !empty($msgB)){
                $habilitado = false;
                $habilita = 'N';
            }

            if(possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) )){
                $habilitado = true;
                $habilita   = 'S';
            }

            if(possui_perfil( array(PFLCOD_CALL_CENTER) )){
                $habilitado = false;
                $habilita   = 'N';
            }
            ?>


            <td style="padding: 16px;">
                <?php
                if ($docid && possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) wf_desenhaBarraNavegacao($docid, array('obrid' => '', 'sldid' => '', 'processo' => $processo, 'sdcid' => $solicitacaoDesembolso->sdcid));
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="20%">Justificativa:</td>
            <td>
                <?
                $obrigatorio = ($sdcpercsolicitado < 10) ? 'S' : 'N';
                ?>
                <? echo campo_textarea('sdcjustificativa', $obrigatorio, $habilita, '', 100, 5, '', '', '', '', '', '');?>
            </td>
            <td></td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">Observação:</td>
            <td>
                <? echo campo_textarea('sdcobs', 'N', $habilita, '', 100, 5, '', '', '', '', '', '');?>
            </td>
            <td></td>
        </tr>

        <tr>
            <?
            if ($pendenciaObras)
                $validacaoMsg[] = 'Uma ou mais obras possuem pendencias que impedem a solicitação do desembolso, clique no nome da obra para visualizar os detalhes.';
            ?>
            <td colspan="3" id="quadro-pendencias">
                <? if((!empty($validacaoMsg) || !empty($msgB)) && (!isset($estado['esdid']) || $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO)):?>
                    <div style="border: 2px solid #cc0000; padding: 10px; margin: 10px; font-size: 14px;">
                        <p style="font-weight: bold; text-align: center; color: #cc0000; font-size: 16px;">Providências</p>
                        <?=$msgB?>
                        <? foreach ($validacaoMsg as $key => $msg): ?>
                            <p style=" font-weight: bold;color:#cc0000;"><img src="../imagens/exclama.gif"/> <?=(is_string($key)) ? $key . ') ' : ''?><?=$msg?></p>
                        <? endforeach; ?>
                    </div>
                <? endif; ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Anexos</td>
            <td colspan="2">
                <div>
                    <table id="table_anexos" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                        <tr>
                            <td class="">
                                <? if ($habilitado): ?>
                                    <div style=""><img src="/imagens/gif_inclui.gif" alt=""/> <a href="" id="adicionar_anexo">Adicionar</a></div>
                                <? endif; ?>
                            </td>
                            <td>Nome</td>
                            <td>Descrição</td>
                        </tr>

                        <?
                        $arquivosV = new SolicitacaoDesembolsoConvenioArquivos();
                        $arquivos = ($sdcid) ? $arquivosV->pegaArquivosPorSdcid($sdcid) : array();
                        ?>
                        <? if (!empty($arquivos)): ?>
                            <? foreach ($arquivos as $arquivo): ?>
                                <tr class="anexos">
                                    <td class="SubTituloEsquerda" style="width: 56px;">
                                        <? if ($habilitado): ?>
                                            <span style=""><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span>
                                        <? endif; ?>
                                    </td>
                                    <td class="SubTituloEsquerda">
                                        <input type="hidden" value="<?=$arquivo['arqid']?>" name="arquivo[]" id="arquivo"/>
                                        <a target="_blank" href="/obras2/obras2.php?modulo=principal/popupSolicitarVinculada&acao=A&download=<?=$arquivo['arqid']?>">
                                            <?=$arquivo['arqnome']?>.<?=$arquivo['arqextensao']?>
                                        </a>
                                    </td>
                                    <td class="SubTituloEsquerda"><input <?= (!$habilitado) ? 'disabled="disabled"' : '' ?>  type="text" maxlength="255" size="50" value="<?=$arquivo['arqdescricao']?>" name="arquivodescricao[]" id="arquivodescricao"/></td>
                                </tr>
                            <? endforeach; ?>
                        <? endif; ?>
                        <? if ($habilitado): ?>
                            <tr class="anexos">
                                <td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td>
                                <td class="SubTituloEsquerda"><input type="file" name="arquivo[]" id="arquivo"/></td>
                                <td class="SubTituloEsquerda"><input maxlength="255" size="50" type="text" name="arquivodescricao[]" id="arquivodescricao"/></td>
                            </tr>
                        <? endif; ?>

                    </table>
                </div>
            </td>
        </tr>
        <?
        if(
            $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA ||
            $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO ||
            $estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO
        ){

            $habilitaAnalise = 'N';
            if($estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA && possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))){
                $habilitaAnalise = 'S';
            }
            ?>
            <tr bgcolor="#DEDEDE">
                <th colspan="3">
                    <label class="TituloTela" style="color:#000000;">Análise Técnica</label>
                </th>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="20%">Percentual de execução do convênio validado.</td>
                <td>
                    <?php echo campo_texto('sdcpercpagamento', 'S', $habilitaAnalise, '', 7, 6, '#######,##', '', 'right', '', 0, '', '', number_format($sdcpercpagamento, 2, ',', '.')); ?>
                </td>
                <td></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="20%">Parcela:</td>
                <td>
                    <?php echo campo_texto('sdcparcela', 'S', $habilitaAnalise, '', 7, 6, '##', '', 'right', '', 0, '', '', $sdcparcela); ?>
                </td>
                <td></td>
            </tr>

            <tr>
                <td class="SubTituloDireita" width="20%">Observação:</td>
                <td>
                    <? echo campo_textarea('sdcobstec', 'N', $habilitaAnalise, '', 100, 5, '', '', '', '', '', '');?>
                </td>
                <td></td>
            </tr>
        <?
        }
        ?>

        <tr bgcolor="#DEDEDE">
            <td colspan="3">
                <?
                if(!$habilitado && $habilitaAnalise != 'S' && !possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))): ?>

                    <input class="disabled" type="button" name="botao" value="Solicitar" onclick="" title="<?= (!empty($validacaoMsg) || !empty($msgB)) ? 'Verificar o quadro de providências no topo desta página.' : 'Aguardando Análise FNDE'?>"/>

                <? else: ?>
                    <input type="button" name="botao" value="<?= ($habilitaAnalise == 'S' || isset($_REQUEST['sdcid'])) ? 'Salvar' : 'Solicitar'?>" onclick="salvaSolicitacao();"/>

                    <? if($estado['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO): ?>
                        <input type="button" name="botao" value="Enviar para Análise FNDE" onclick="tramitaAnalise();"/>
                    <? endif; ?>

                <? endif; ?>
                <input type="button" name="botao" value="Fechar" onclick="window.close()"/>
            </td>
        </tr>



    </table>
</form>
</body>
<script>
    $(function(){
        $('.obra-pendencia').on('click', function(){
            var obrid = $(this).attr('obrid');
            $('#checklist-' + obrid).slideToggle(200);
        });

        $('#quadro-pendencias').appendTo('#quadro-pendencias-topo');

    });
</script>

<script type="text/javascript">
    $(function(){
        $('#adicionar_anexo').click(function (e) {
            $('#table_anexos').append($('<tr class="anexos anexos-base"><td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td><td class="SubTituloEsquerda"><input type="file" name="arquivo[]" id="arquivo"/></td><td class="SubTituloEsquerda"><input <?= (!$habilitado) ? 'disabled="disabled"' : '' ?>   maxlength="255" size="50" type="text" name="arquivodescricao[]" id="arquivodescricao"/></td></tr>').removeClass('anexos-base'));
            e.preventDefault();
        });
        $('.excluir_anexo').live('click',function (e) {
            $(this).parents('tr.anexos').remove();
            e.preventDefault();
        });
    });

    function tramitaAnalise() {
        $('[name="tramitacao"]').val(1);
        $('#formulario').submit();
    }

    function salvaSolicitacao(){
        var msg = '';

        <? if($obrigatorio == 'S'): ?>
        if(!$('[name="sdcjustificativa"]').val()){
            msg += "Informe a Justificativa.\n";
        }
        <? endif; ?>

        if(msg != ''){
            alert(msg);return false;
        }

        $('#formulario').submit();
    }


</script>


<?

function verificaArqid($sdcid, $arqid)
{
    if(is_array($arqid)) {
        foreach ($arqid as $key => $id) {
            $arquivo = new Arquivo($id);
            $arquivo->arqdescricao = $_POST['arquivodescricao'][$key];
            $arquivo->salvar();
            $arquivo->commit();
        }
    }

    $arqid = (is_array($arqid)) ? $arqid : array();
    $arquivosV = new SolicitacaoDesembolsoConvenioArquivos();
    $arquivos = $arquivosV->pegaArquivosPorSdcid($sdcid);

    if(!empty($arquivos)) {
        foreach ($arquivos as $arquivo) {
            if (array_search($arquivo['arqid'], $arqid) === false) {
                $arquivosV->carregarPorId($arquivo['svaid']);
                $arquivosV->svastatus = 'I';
                $arquivosV->salvar();
                $arquivosV->commit();
                $arquivosV->clearDados();
            }
        }
    }
}

?>




<script type="text/javascript">

    function abrePopupEditaCronograma(obrid){
        var url = "/obras2/obras2.php?modulo=principal/etapas_da_obra&acao=E" + "&obrid=" + obrid;
        popup1 = window.open(
            url,
            "editarCronograma",
            "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
        );

        return false;
    }

    function alterarObr(obrid){
        window.open('?modulo=principal/cadObra&acao=A&obrid='+obrid,
            'ObraSolicitacaoDesembolsoDetalhe', "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
    }

</script>