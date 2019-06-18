<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

verificaSessao('orgao');
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int)($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE);
$obra = new Obras($_SESSION['obras2']['obrid']);
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];

if ($_GET['download']) {
    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid($_GET['download']);
    $eschema = 'obras2';

    $file = new FilesSimec(null, null, $eschema);
    $file->getDownloadArquivo($_GET['download']);

    die('<script type="text/javascript">
        window.close();
        </script>');
}

if ($_REQUEST['excluir'] == 'S') {
    $cumprimentoObjetoProcesso = new CumprimentoObjetoProcesso();
    $cumprimentoObjetoProcesso->excluir($_REQUEST['arqid'], $_REQUEST['tipo']);
    echo "<script>alert('Arquivo excluido com sucesso!');window.location.href = 'obras2.php?modulo=principal/cadProcessoCumprimentoObjeto&acao=A';</script>";
    exit;
}

if ($_POST['requisicao'] && $_POST['requisicao'] == 'processo') {
    $cumprimentoObjetoProcesso = new CumprimentoObjetoProcesso();
    $cumprimentoObjetoProcesso->salvarConclusao($_POST);
    echo "<script> alert('Registro salvo com sucesso!'); window.location.href = 'obras2.php?modulo=principal/cadProcessoCumprimentoObjeto&acao=A';</script>";
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A', $parametros, array());
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA, 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A', $parametros, array());
}

global $db;
$habilPag = false;
$estadoWorkflowObra = wf_pegarEstadoAtual($obra->docid);
if ($estadoWorkflowObra) {
    if ($estadoWorkflowObra['esdid'] == ESDID_OBJ_CONCLUIDO || $estadoWorkflowObra['esdid'] == ESDID_OBJ_INACABADA || $estadoWorkflowObra['esdid'] == ESDID_OBJ_CANCELADO) {
        $habilPag = true;
        $cumprimentoObjeto = new CumprimentoObjeto();
        if (!$cumprimentoObjeto->obrasPermitidas($obra->obrid)) {
            echo "<script>alert('No momento, a aba de Cumprimento do Objeto não encontra-se liberado para esta obra.'); window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A'</script>";
            die;
        }
        $coid = $cumprimentoObjeto->verificaExistencia($obrid);
        $estado = wf_pegarEstadoAtual($cumprimentoObjeto->docid);

        $habilitado = false;
        if (possui_perfil($pflcods)) {
            $habilitado = true;
        } else {
            echo "<script>alert('Acesso não permitido.'); window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A'</script>";
            die;
        }
        if ($estado['esdid'] == ESDID_CUMPRIMENTO_CADASTRAMENTO || $estado['esdid'] == ESDID_CUMPRIMENTO_DILIGENCIADO) {
            if (!possui_perfil(array(PFLCOD_SUPER_USUARIO)) || IS_PRODUCAO) {
                echo "<script>alert('Acesso não permitido. Favor, tramitar workflow antes de prosseguir.'); window.location.href='obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';</script>";
                die;
            }
        }
        print cabecalhoObra($obrid);
        $cumprimentoObjeto->criaSubAba($url, $habilitado, $obrid, $estado['esdid']);
        //monta_titulo('Conclusão do Processo', '');
        monta_titulo('Resultado da Análise', '');

        $cumprimentoObjetoConclusao = new CumprimentoObjetoConclusao();

        if ($cumprimentoObjetoConclusao->verificaExistencia($cumprimentoObjeto->coid)) {
            $numprocesso = $cumprimentoObjeto->retornaNumeroProcesso();
            $cumprimentoObjetoProcesso = new CumprimentoObjetoProcesso();
            $verificacaoObras = $cumprimentoObjetoProcesso->verificaObrasPorProcesso($numprocesso);
            $dados = $cumprimentoObjetoProcesso->capturaDados($numprocesso);
            $dadosArrayOriginal = $cumprimentoObjetoProcesso->capturaDadosArray($numprocesso);


        } else {
            $habilPag = false;
            $msgHabilPag = 'É necessário preencher a aba de Conclusão da Obra antes de prosseguir.';
        }


        $display = "style='display: none'";
        $displayParecerNovaPactuacao = "style='display: show'";

        if ($_POST['novaPactuacao'] == 14 && $_POST['novaPactuacao'] != "") {


            $display = "style='display: show'";
        } else {
            $displayParecerNovaPactuacao = "style='display: none'";
        }
        $dadosSolicitacoesObra = (capturaSolicitacoesObra($cumprimentoObjeto->obrid));
        $novaPac = 0;

        $dadosArray = array();

        if ($dadosArrayOriginal && ($dadosArrayOriginal[0]['arqid_pf'] || $dadosArrayOriginal[1]['arqid_pf']))
        {
            foreach ($dadosArrayOriginal as $dadosOriginal) {
                if ($dadosOriginal['nova_pactuacao'] == 'N') {
                    $dadosArray[0] = $dadosOriginal;
                } else {
                    $dadosArray[1] = $dadosOriginal;
                }
            }
        }

        if (count($dadosArray) > 1) {
            $novaPacFlag = 1;
            $habilitadoExcluir = false;
            $habilitadoNovaPac = false;

        } else {

            if ($dadosArray[0]['nova_pactuacao'] == 'N' && $dadosArray[0]['nova_pactuacao'] != "") {
                $novaPacFlag = 0;
                $display = "style='display: show'";
                $habilitadoExcluir = true;
            } else {
                $novaPacFlag = 1;
                $habilitadoExcluir = false;
                $display = "style='display: show'";
                $displayParecerNovaPactuacao = "style='display: none'";
                $habilitadoNormal = false;
            }
        }

        foreach ($dadosSolicitacoesObra as $dadosSol) {
            if ($dadosSol['tslid'] == 14) {
                $dadosValorNovaPac = $cumprimentoObjetoConclusao->capturaDadosValorParecerNovaPac($cumprimentoObjeto->obrid);
                $novaPac = 1;
                $novaPacFlag = 0;
                $displayParecerNovaPactuacao = "style='display: show'";
                $display = "style='display: none'";
                $habilitadoNormal = true;
                $dados['extrato_parecer'] = $dadosArray[1]['extrato_parecer'];
            }
        }

    }
}

$flagArquivo = true;
        if ($dadosArray[0]['datacompara'] == $dadosArray[1]['datacompara']) {
            if ($dadosArray[0]['horacompara'] > $dadosArray[1]['horacompara']) {
                  $novaPacFlag = 0;
                $display = "style='display: show'";
                $displayNovPac = "style='display: none'";
                $habilitadoNormal = false;
            }
        }
        if ($dadosArray[0]['datacompara'] > $dadosArray[1]['datacompara']) {
             $novaPacFlag = 0;
            $display = "style='display: show'";
            $displayNovPac = "style='display: none'";
            $habilitadoNormal = false;
        }elseif($dadosArray[1]['nova_pactuacao'] == 'S' && ($dadosArray[0]['horacompara'] < $dadosArray[1]['horacompara'])){
            $novaPacFlag = 1;
            $flagArquivo = false;
        }


$habilitaData = 'S';
$habilitaValorNovaPac = '' ;
if($estado['esdid'] == ESDID_CUMPRIMENTO_APROVADO  ){
    $habilitaValorNovaPac = 'disabled=""' ;
    $dadosArray[1]['valorNovaPac'] = 1;
    $habilitaData = 'N';
}

if (!(possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC)))) {
    $habilitado = false;

}

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript"
        src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script>
    $(document).ready(function () {
    });

    function excluir(arqid, tipo) {
        if (confirm('Você deseja realmente excluir este arquivo?')) {
            window.location = '?modulo=principal/cadProcessoCumprimentoObjeto&acao=A&excluir=S&arqid=' + arqid + '&tipo=' + tipo;
        }
    }
</script>
<?php if (!$habilPag): ?>
    <div class="col-md-12">
        <center>
        <span style="background: #f00; color: #fff; padding:5px; text-align: center;">
            <?php
            if ($msgHabilPag):
                echo $msgHabilPag;
            else:
                ?>
                Esta aba será liberada após a conclusão da obra.
            <?php
            endif; ?>
        </span>
        </center>
    </div>
<?php
else:
    if (!$verificacaoObras['result']): $habilitado = false; ?>
        <div class="col-md-12">
            <section class="alert alert-warning text-center">
                Esta aba será liberada após a conclusão do cumprimento do objeto de todas as obras do processo.
            </section>
        </div>
    <?php
    endif; ?>
    <div class="row" style="position:static;">
        <div class="col-md-10" style="position:inherit;">
            <form method="post" action="" id="form_processo" enctype="multipart/form-data">
                <input type="hidden" name="requisicao" id="requisicao" value="processo">
                <input type="hidden" name="numprocesso" id="coid" value="<?= $numprocesso ?>">
                <input type="hidden" name="copid" id="copid" value="<?= $dadosArray[0]['copid'] ?>">
                <input type="hidden" name="copidNovaPactuacao" id="copidNovaPactuacao" value="<?= $dadosArray[1]['copid'] ?>">
                <input type="hidden" name="obs" id="obs" value="<?= urldecode(urldecode($dadosArray[0]['extrato_parecer'])) ?>">
                <input type="hidden" name="obsNovaPactuacao" id="obsNovaPactuacao" value="<?= urldecode(urldecode($dadosArray[1]['extrato_parecer'])) ?>">
                <input type="hidden" name="NovaPacFlag" id="NovaPacFlag" value="<?= $novaPacFlag ?>">
                <input type="hidden" name="obrid" id="obrid" value="<?= $obra->obrid ?>">
                <input type="hidden" name="preid" id="preid" value="<?= $obra->preid ?>">
                <input type="hidden" name="docid" id="docid" value="<?= $cumprimentoObjeto->docid?>">
                <input type="hidden" name="valorNovaPac" id="valorNovaPac" value="<?= $dadosArray[1]['valorNovaPac']  ?>">


                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td class="SubtituloCentro">Obra</td>
                        <td class="SubtituloCentro">Aprovado 100% - Objetivo Cumprido</td>
                        <td class="SubtituloCentro">Reprovado 100% - Objetivo Não Cumprido</td>
                        <td class="SubtituloCentro">Aprovação parcial - Objetivo Cumprido</td>
                        <td class="SubtituloCentro">Valor a Devolver ao Erário Público</td>
                    </tr>
                    <?php
                    $total_devolucao = 0;
                    $aprovacaoA = 0;
                    $aprovacaoR = 0;
                    $aprovacaoP = 0;
                    foreach ($verificacaoObras['obras'] as $key => $obr):
                        $chave = $key + 1;
                        $cumprimentoObjetoConclusao = new CumprimentoObjetoConclusao();
                        $cumprimentoObjetoConclusao->capturaDados($obr['coid']);
                        $total_devolucao += $cumprimentoObjetoConclusao->valor_devolucao;
                        switch ($cumprimentoObjetoConclusao->aprovacao) {
                            case 'A':
                                $aprovacaoA++;
                                break;
                            case 'R':
                                $aprovacaoR++;
                                break;
                            case 'P':
                                $aprovacaoP++;
                                break;
                        }
                        ?>
                        <tr>
                            <td style="text-align: center;"><strong><?= $obr['obrid'] ?></strong></td>
                            <td style="text-align: center;">
                                <?= $cumprimentoObjetoConclusao->aprovacao == 'A' ? '<img margin src="/imagens/valida1.gif">' : '' ?>
                            </td>
                            <td style="text-align: center;">
                                <?= $cumprimentoObjetoConclusao->aprovacao == 'R' ? '<img src="/imagens/valida1.gif">' : '' ?>
                            </td>
                            <td style="text-align: center;">
                                <?= $cumprimentoObjetoConclusao->aprovacao == 'P' ? '<img src="/imagens/valida1.gif">' : '' ?>
                            </td>
                            <td style="text-align: center;">
                                <input type="text"
                                       value="<?= mascaraglobal($cumprimentoObjetoConclusao->valor_devolucao, '###.####.###.###,##') ?>"
                                       onkeyup="this.value=mascaraglobal('[.###],##',this.value);" name="valordevolucao"
                                       disabled>
                            </td>
                        </tr>
                    <?php
                    endforeach; ?>
                </table>
                <?php if ($aprovacaoA > $aprovacaoR && $aprovacaoA > $aprovacaoP) {
                    if ($aprovacaoP > 0)
                        $aprovacao = 'P';
                    else
                        $aprovacao = 'A';
                } else if ($aprovacaoP > $aprovacaoA) {
                    $aprovacao = 'P';
                } else if ($aprovacaoR > 0) {
                    $aprovacao = 'R';
                } else {
                    $aprovacao = 'P';
                }
                ?>
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <?php if ($novaPac == 1) { ?>
                        <tr>
                            <td class="SubTituloDireita">Nova pactuação:</td>
                            <td>
                                <input type="radio" name="novaPactuacao" id="novaPactuacao"
                                       value="14" <?= $novaPacFlag == '1' ? 'checked="checked"' : '' ?>> Sim
                                <input type="radio" name="novaPactuacao" id="novaPactuacao"
                                       value="1" <?= $novaPacFlag != '1' ? 'checked="checked"' : '' ?>> Não
                            </td>
                        <tr/>
                    <?php } ?>
                    <tr>
                        <td class="SubtituloDireita" width="50%">
                            <input type="radio" id="estadoAT" name="aprovacao"
                                   value="A" <?= $aprovacao == 'A' ? 'checked' : '' ?> disabled>
                        </td>
                        <td>
                            <label for="estadoAT"> Aprovado 100% - Objetivo Cumprido</label>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">
                            <input type="radio" id="estadoRT" name="aprovacao"
                                   value="R" <?= $aprovacao == 'R' ? 'checked' : '' ?> disabled>
                        </td>
                        <td>
                            <label for="estadoRT"> Reprovado 100% - Objetivo Não Cumprido</label>
                        </td>
                    </tr>
                    <tr id="td_estadoAP" <?= $display ?>>
                        <td class="SubtituloDireita">
                            <input type="radio" id="estadoAP" name="aprovacao"
                                   value="P" <?= $aprovacao == 'P' ? 'checked' : '' ?> disabled>
                        </td>
                        <td id="td_estadoAP">
                            <label for="estadoAP"> Aprovação parcial - Objetivo Cumprido</label>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Valor a Devolver ao Erário Público</td>
                        <td><input type="text" value="<?= mascaraglobal($total_devolucao, '###.####.###.###,##') ?>"
                                   onkeyup="this.value=mascaraglobal('[.###],##',this.value);" name="valordevolucao"
                                   disabled><img border="0" src="../imagens/obrig.gif"
                                                 title="Indica campo obrigatório."></td>
                    </tr>
                    <tr>
                        <!-- <td class="SubtituloDireita">Parecer Final - Infraestrutura FNDE</td>-->
                        <td class="SubtituloDireita">Análise Técnica Concluída - Infraestrutura FNDE</td>
                        <td>
                            <?php if ($dadosArray[0]['arqid_pf']): ?>
                                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                                    <thead>
                                    <tr>
                                        <th>Ações</th>
                                        <th>Arquivo</th>
                                        <th>Data de Inclusão</th>
                                        <th>Gravado por</th>
                                        <?php if ($dadosArray[0]['nova_pactuacao'] == 'S') {
                                            $habilita = false;
                                            ?>
                                            <th>Tipo</th>
                                        <?php } ?>

                                    </tr>
                                    </thead>
                                    <tbody>
                                    <tr>
                                        <td>
                                            <?php if ($habilitado): ?>
                                                <img <?= $display ?> id="excluir0" src="../imagens/excluir.gif"
                                                                     style="border:0; cursor:pointer;"
                                                                     title="Excluir Documento Anexo"
                                                                     onclick="excluir(<?= $dadosArray[0]['arqid_pf'] ?>,'arqid_pf')">
                                            <?php else: ?>
                                                <img <?= $display ?> id="excluir0" src="../imagens/excluir_01.gif"
                                                                     style="border:0;" title="Excluir Documento Anexo">
                                            <?php endif; ?>
                                        </td>
                                        <td>
                                            <a target="_blank"
                                               href="/obras2/obras2.php?modulo=principal/cadProcessoCumprimentoObjeto&acao=A&download=<?= $dadosArray[0]['arqid_pf'] ?>">
                                                <?= $dadosArray[0]['pfnome'] ?> .<?= $dadosArray[0]['pfextensao'] ?>
                                            </a>
                                        </td>
                                        <td><?= $dadosArray[0]['pfdata'] ?></td>
                                        <td><?= $dadosArray[0]['pfusunome'] ?></td>
                                        <?php if ($dadosArray[0]['nova_pactuacao'] == 'S') { ?>
                                            <td>Nova Pactuação</td>
                                        <?php } ?>
                                    </tr>
                                    </tbody>
                                </table>
                            <?php
                            else: ?>

                                <input type="file" name="parecerfinal"
                                       id="parecerfinal" <?= $habilitadoNormal ? 'disabled' : '' ?>><img border="0"
                                                                                                         src="../imagens/obrig.gif"
                                                                                                         title="Indica campo obrigatório.">
                            <?php
                            endif; ?>
                        </td>
                    </tr>

                    <tr id="tr_analiseTecNovPac" <?= $displayParecerNovaPactuacao ?> >
                        <td class="SubtituloDireita">Análise Técnica Concluída Nova Pactuação - Infraestrutura FNDE</td>
                        <td>
                            <?php if ($dadosArray[1]['arqid_pf']): ?>
                                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                                    <thead>
                                    <tr>
                                        <th>Ações</th>
                                        <th>Arquivo</th>
                                        <th>Data de Inclusão</th>
                                        <th>Gravado por</th>
                                        <?php if ($dadosArray[1]['nova_pactuacao'] == 'S') { ?>
                                            <th>Tipo</th>
                                        <?php } ?>
                                    </tr>
                                    </thead>
                                    <tbody>
                                    <tr>
                                        <td>
                                            <?php if ($habilitado): ?>
                                                <img  <?= $displayNovPac ?> id="excluir1" src="../imagens/excluir.gif"
                                                     style="border:0; cursor:pointer;" title="Excluir Documento Anexo"
                                                     onclick="excluir(<?= $dadosArray[1]['arqid_pf'] ?>,'arqid_pf')">
                                            <?php else: ?>
                                                <img  <?= $displayNovPac ?> id="excluir1" src="../imagens/excluir_01.gif" style="border:0;"
                                                     title="Excluir Documento Anexo">
                                            <?php endif; ?>
                                        </td>
                                        <td>
                                            <a target="_blank"
                                               href="/obras2/obras2.php?modulo=principal/cadProcessoCumprimentoObjeto&acao=A&download=<?= $dadosArray[1]['arqid_pf'] ?>">
                                                <?= $dadosArray[1]['pfnome'] ?> .<?= $dadosArray[1]['pfextensao'] ?>
                                            </a>
                                        </td>
                                        <td><?= $dadosArray[1]['pfdata'] ?></td>
                                        <td><?= $dadosArray[1]['pfusunome'] ?></td>
                                        <?php if ($dadosArray[1]['nova_pactuacao'] == 'S') { ?>
                                            <td>Nova Pactuação</td>
                                        <?php } ?>
                                    </tr>
                                    </tbody>
                                </table>
                            <?php else: ?>
                                <input type="file" name="parecerNovaPactuacao"
                                       id="parecerNovaPactuacao" <?= !$habilitadoNormal ? 'disabled' : '' ?>><img
                                        border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                            <?php endif; ?>
                        </td>

                    </tr>

                    <tr>
                        <!--  <td class="SubtituloDireita">Parecer Final Revisado - Infraestrutura FNDE</td> -->
                        <td class="SubtituloDireita">Análise Técnica Revisada Concluída - Infraestrutura FNDE</td>
                        <td>
                            <?php
                            if ($dadosArray[0]['arqid_pf_revisado']) {
                                $novaPacRevisada = 'N';
                            } elseif ($dadosArray[1]['arqid_pf_revisado']) {
                                $novaPacRevisada = 'S';
                            }

                            if ($dadosArray[0]['arqid_pf_revisado'] || $dadosArray[1]['arqid_pf_revisado']): ?>
                                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                                    <thead>
                                    <tr>
                                        <th>Ações</th>
                                        <th>Arquivo</th>
                                        <th>Data de Inclusão</th>
                                        <th>Gravado por</th>
                                        <?php if ($novaPacRevisada == 'S') { ?>
                                            <th>Tipo</th>
                                        <?php } ?>
                                    </tr>
                                    </thead>
                                    <tbody>
                                    <tr>
                                        <td>
                                            <?php if ($habilitado): ?>
                                                <img src="../imagens/excluir.gif" style="border:0; cursor:pointer;"
                                                     title="Excluir Documento Anexo"
                                                     onclick="excluir(<?= $dadosArray[0]['arqid_pf_revisado'] != '' ? $dadosArray[0]['arqid_pf_revisado'] : $dadosArray[1]['arqid_pf_revisado'] ?>,'arqid_pf_revisado')">
                                            <?php else: ?>
                                                <img src="../imagens/excluir_01.gif" style="border:0;"
                                                     title="Excluir Documento Anexo">
                                            <?php endif; ?>
                                        </td>
                                        <td>
                                            <a target="_blank"
                                               href="/obras2/obras2.php?modulo=principal/cadProcessoCumprimentoObjeto&acao=A&download=<?= $dadosArray[0]['arqid_pf_revisado'] != '' ? $dadosArray[0]['arqid_pf_revisado'] : $dadosArray[1]['arqid_pf_revisado'] ?>">
                                                <?= $dadosArray[0]['pfrnome'] != '' ? $dadosArray[0]['pfrnome'] : $dadosArray[1]['pfrnome'] ?>
                                                .<?= $dadosArray[0]['pfrextensao'] != '' ? $dadosArray[0]['pfrextensao'] : $dadosArray[1]['pfrextensao'] ?>
                                            </a>
                                        </td>
                                        <td><?= $dadosArray[0]['pfrdata'] != '' ? $dadosArray[0]['pfrdata'] : $dadosArray[1]['pfrdata'] ?></td>
                                        <td><?= $dadosArray[0]['pfrusunome'] != '' ? $dadosArray[0]['pfrusunome'] : $dadosArray[1]['pfrusunome'] ?></td>
                                        <?php if ($novaPacRevisada == 'S') { ?>
                                            <td>Nova Pactuação</td>
                                        <?php } ?>
                                    </tr>
                                    </tbody>
                                </table>
                            <?php else: ?>
                                <input type="file" name="parecerfinalrevisado" <?= !$habilitado ? 'disabled' : '' ?>>
                            <?php endif; ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita">Extrato do Parecer</td>

                        <td>
                            <?php
                            if ($dadosArray[0]['extrato_parecer'] && $novaPacFlag != 1) {
                                $extrato_parecer = $dadosArray[0]['extrato_parecer'];
                            } elseif ($dadosArray[1]['extrato_parecer']) {
                                $extrato_parecer = $dadosArray[1]['extrato_parecer'];
                            }
                            ?>
                            <?= campo_textarea('extrato_parecer', 'N', $habilitado ? 'S' : 'N', '', 100, 5, '', '', '', '', '', '', urldecode(urldecode($extrato_parecer))); ?>
                        </td>
                    </tr>

                </table>

<?php if ($novaPac == 1) {?>
    <div id="tabelaValorNovaPac" <?= $displayNovPac?>>
               <?= monta_titulo('Valor do Parecer Nova Pactuação', '');?>
                <table class="tabela"    bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
                    <tr>
                        <td class="SubtituloDireita" width="50%">
                            % Execução/Obra apurada:
                        </td>
                        <td>
                            <input value="<?php echo $dadosValorNovaPac['vpnexecucao'] != '' ? mascaraglobal($dadosValorNovaPac['vpnexecucao'], '####,##').'%' : '' ?>"
                                   onkeyup="this.value=mascaraglobal('####,##',this.value);"  <?echo $habilitaValorNovaPac;?> id="novoExecucao" name="novoExecucao" type="text" >
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="50%">
                            Novo valor da obra:
                        </td>
                        <td>
                            <input value="<?php echo $dadosValorNovaPac['vpnvalorobra'] != '' ? mascaraglobal($dadosValorNovaPac['vpnvalorobra'], '###.####.###.###,##') : '' ?>"
                                   onkeyup="this.value=mascaraglobal('[.###],##',this.value);"  <?= $habilitaValorNovaPac?> id="novoValorBase" name="novoValorBase" type="text" >
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="50%">
                            Valor FNDE:
                        </td>
                        <td>
                            <input value="<?php echo $dadosValorNovaPac['vpnvalorfnde'] != '' ? mascaraglobal($dadosValorNovaPac['vpnvalorfnde'], '###.####.###.###,##') : '' ?>"
                                   onkeyup="this.value=mascaraglobal('[.###],##',this.value);" <?= $habilitaValorNovaPac?> id="valorFNDE" name="valorFNDE" type="text">
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="0%">
                            Contrapartida:
                        </td>
                        <td>
                            <input value="<?php echo $dadosValorNovaPac['vpncontrapartida'] != '' ? mascaraglobal($dadosValorNovaPac['vpncontrapartida'], '###.####.###.###,##') : '' ?>"
                                   onkeyup="this.value=mascaraglobal('[.###],##',this.value);" <?= $habilitaValorNovaPac?> id="contrapartida" name="contrapartida" type="text">
                        </td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" width="20%">
                            Saldo em Conta:
                        </td>
                        <td>
                            <input value="<?php echo $dadosValorNovaPac['vpnsaldoconta'] != '' ? mascaraglobal($dadosValorNovaPac['vpnsaldoconta'], '###.####.###.###,##') : '' ?>"
                                   onkeyup="this.value=mascaraglobal('[.###],##',this.value);" <?= $habilitaValorNovaPac?> id="saldoEmConta" name="saldoEmConta" type="text">
                        </td>
                        <td class="SubtituloDireita" width="10%">
                            Data Base:
                        </td>
                        <td>
                           <?= campo_data2('dataBase', 'N', $habilitaData, '', 'N','','',formata_data($dadosValorNovaPac['vpndtbase']));?>
                        </td>
                    </tr>

                </table>
    </div>
 <?php }?>


                <table align="center">
                    <tr bgcolor="">
                        <td colspan="5" style="text-align: center">
                            <input type="button" value="Salvar" id="salvar_conclusao"
                                   style="cursor:pointer;" <?= !$habilitado ? 'disabled' : '' ?>/>
                        </td>
                    </tr>
                </table>
                <input type="hidden" name="valor_devolucao" id="valor_devolucao" value="<?= $total_devolucao ?>">
                <input type="hidden" name="aprovacao" id="aprovacao" value="<?= $aprovacao ?>">
            </form>
        </div>
        <div class="col-md-1 pull-right" style="position:inherit;">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td>
                        <?php
                        wf_desenhaBarraNavegacao($cumprimentoObjeto->docid, array('coid' => $cumprimentoObjeto->coid, 'docid' => $cumprimentoObjeto->docid, 'obrid' => $cumprimentoObjeto->obrid)); ?>
                    </td>
                </tr>
            </table>
        </div>
    </div>
<?php
endif; ?>
<script>
    $(document).ready(function () {
        $('#div_dialog_workflow').next().css('width', '100%');
        $('#salvar_conclusao').on('click', function () {
            var valorNovaPac = $("input[name='novaPactuacao']:checked").val();
            if (valorNovaPac == 14) {
                if ($('[name=parecerNovaPactuacao]').val() == '') {
                    $('#parecerNovaPactuacao').css('border', '1px solid red');
                    alert('Campo \'Parecer Final - Infraestrutura FNDE\' é de preenchimento obrigatório.');
                    return false;
                }
            } else if (valorNovaPac == 1) {
                if ($('[name=parecerfinal]').val() == '') {
                    alert('Campo \'Parecer Final - Infraestrutura FNDE\' é de preenchimento obrigatório.');
                    $('#parecerfinal').css('border', '1px solid red');
                    return false;
                }
            } else if ($('[name=parecerfinal]').val() != undefined) {
                if ($('[name=parecerfinal]').val() == '') {
                    $('#parecerfinal').css('border', '1px solid red');
                    alert('Campo \'Parecer Final - Infraestrutura FNDE\' é de preenchimento obrigatório.');
                    return false;
                }
            }
            $('#form_processo').submit();
        });


        <?php if($flagArquivo){?>
            $("#td_estadoAP").show();
            $("#tabelaValorNovaPac").hide();
            $("#extrato_parecer").text($("#obs").val());
            $("#parecerNovaPactuacao").prop('disabled', true);
            $("#parecerfinal").prop('disabled', false);
            $("#excluir0").show();
            $("#excluir1").hide();
            $('#parecerfinal').css('border', '1px solid #f5f5f5');
            $('#parecerNovaPactuacao').css('border', '1px solid #f5f5f5');
        <?php }?>
    });



    $("#novoExecucao").on("blur", function() {
        var value = $(this).val().length >= 1 ? $(this).val() + '%' : $(this).val();
        $(this).val( value );
    });

    $('#parecerfinal').click(function () {
        $('#parecerfinal').css('border', '1px solid #f5f5f5');
    });

    $('#parecerNovaPactuacao').click(function () {
        $('#parecerNovaPactuacao').css('border', '1px solid #f5f5f5');
    });

    $("*[name='novaPactuacao']").change(function () {
        valor = ($(this).attr('value'));
        if (valor == 14) {
            $("#td_estadoAP").hide();
            $("#tabelaValorNovaPac").show();
            $("#extrato_parecer").text($("#obsNovaPactuacao").val());
            $("#parecerNovaPactuacao").prop('disabled', false);
            $("#parecerfinal").prop('disabled', true);
            $("#excluir0").hide();
            $("#excluir1").show();
            $('#parecerfinal').css('border', '1px solid #f5f5f5');
            $('#parecerNovaPactuacao').css('border', '1px solid #f5f5f5');
        } else {
            $("#td_estadoAP").show();
            $("#tabelaValorNovaPac").hide();
            $("#extrato_parecer").text($("#obs").val());
            $("#parecerNovaPactuacao").prop('disabled', true);
            $("#parecerfinal").prop('disabled', false);
            $("#excluir0").show();
            $("#excluir1").hide();
            $('#parecerfinal').css('border', '1px solid #f5f5f5');
            $('#parecerNovaPactuacao').css('border', '1px solid #f5f5f5');
        }
    });

</script>