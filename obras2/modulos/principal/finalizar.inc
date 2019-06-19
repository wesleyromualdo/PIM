<?php
$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

if (empty($obrid) && empty($empid)) {
    die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
}


if ($empid && empty($obrid)) {
    $obr = new Obras();
    $obrid = $obr->pegaIdObraPorEmpid($empid);

    $obrid = (is_array($obrid) && count($obrid)) ? $obrid[0] : $obrid;

    if (empty($obrid)) {
        die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
    }
}

$obra = new Obras($obrid);
$estadoObra = $obra->pegaEstadoObra($obrid);

$execucaoFinanceira = new ExecucaoFinanceira();
$verificarExecucaoFinanceira = $execucaoFinanceira->verificarExecucaoFinanceira($obrid);
$processo = $execucaoFinanceira->retornarProcesso($obrid);
$quantidadeObras = $execucaoFinanceira->verificarQuantidadeObrasProcesso($obrid, $processo);
 
$verificarExecucaoFinanceiraComposicao = $execucaoFinanceira->verificarExecucaoFinanceiraComposicao($obrid);
$verificarExecucaoFinanceira = $execucaoFinanceira->verificarExecucaoFinanceiraFinalizada($obrid);
$execucaoFinanceiraFinalizada = $verificarExecucaoFinanceira > 0 ? true : false;
$retornaExecucaoFinanceira = $execucaoFinanceira->retornaExecucaoFinanceira($processo);
$arrExec = $execucaoFinanceira->retornaExecucaoFinanceira($processo);
$pendencias = $execucaoFinanceira->verificaPendenciasExecucaoFinanceira($obrid);
$docid = $arrExec['docid'];
$exeid = $arrExec['exeid'];

if ($_REQUEST['finalizar_devolucao'] == 'S') {
    if ($_POST['check'] != 'true') {
        $msg = 'Para finalizar a Execução Financeira é necessário declarar estar ciente das informações financeiras declaradas.';
        $retorno = array(
            'valor' => 'false',
            'msg' => ($msg)
        );
        echo json_encode($retorno);
        exit;
    }
    if ($estadoObra != ESDID_OBJ_CONCLUIDO && $estadoObra != ESDID_OBJ_CANCELADO && $estadoObra != ESDID_OBJ_INACABADA) {
        $msg = 'Nao foi possível finalizar a Execuçao Financeira, pois a obra não encontra-se em um dos seguintes estados: Concluída/Cancelada/Inacabada';
        $retorno = array(
            'valor' => 'false',
            'msg' => ($msg)
        );
        echo json_encode($retorno);
        exit;
    } else {
        $arrDados = array(
            'obrid' => $obrid,
            'exeprocesso' => $processo,
            'usucpf' => $_SESSION['usucpf'],
            'exeid' => $retornaExecucaoFinanceira['exeid'],
            'excid' => $retornaExecucaoFinanceira['excid']
        );

        if (!$verificarExecucaoFinanceiraComposicao) {
            $execucaoFinanceira->salvarExecucaoFinanceiraComposicao($arrDados, false);
        }
        $resp = $execucaoFinanceira->finalizarDevolucaoGru($obrid);

        if($quantidadeObras == 1 ){
            
            if(condicaoAcaoEnviarPCControleSocial($obrid)){
                wf_alterarEstado($docid, AEDID_ENVIAR_PARA_CONTROLE_SOCIAL, 'Enviar para Controle Social através do Finalizar Execução Financeira',array('obrid'=>$obrid) );
            }else if(condicaoAcaoEnviarPCAnalise($obrid)){
                wf_alterarEstado($docid, AEDID_ENVIAR_PARA_ANALISE, 'Enviar para análise do FNDE através do Finalizar Execução Financeira',array('obrid'=>$obrid) );
            }
            
        }

        $retorno = array('valor' => 'true');
        echo json_encode($retorno);
        exit;
    }
}

if ($_REQUEST['reabrirExecFinanceira'] == 'S') {
    ob_clean();
    global $db;

    $obrid = $_POST['obrid'];

    if ($obrid != '') {
        $docid = $db->pegaUm("SELECT
                          docid
                        FROM
                          obras2.execucaofinanceiracomposicao exc
                        INNER JOIN
                          obras2.execucaofinanceira exe ON exc.exeid = exe.exeid AND exe.exestatus = 'A'
                        WHERE exc.obrid = {$obrid} AND exc.excstatus = 'A' ");

        $estadoAtual = wf_pegarEstadoAtual($docid);

        if ($estadoAtual['esdid'] == ESDID_EM_CADASTRAMENTO) {
            $sqlUpdate = "UPDATE obras2.execucaofinanceiracomposicao SET excfinalizada = FALSE WHERE obrid = {$obrid}";

            if ($db->executar($sqlUpdate)) {
                $db->commit();
                $retorno = array('valor' => 'true');
                echo json_encode($retorno);
                exit;
            } else {
                $msg = 'Occoreu um erro ao reabrir a Execução Financeira.';
                $retorno = array(
                    'valor' => 'false',
                    'msg' => ($msg)
                );
                echo json_encode($retorno);
                exit;
            }
        } else {
            $msg = 'Não e possivel reabrir a Execução Financeira desta obra foi o processo da mesma já foi enviado para a Análise do CACS.';
            $retorno = array(
                'valor' => 'false',
                'msg' => ($msg)
            );
            echo json_encode($retorno);
            exit;
        }
    } else {
        $msg = 'Faltam parametros para realizar esta ação.';
        $retorno = array(
            'valor' => 'false',
            'msg' => ($msg)
        );
        echo json_encode($retorno);
        exit;
    }
    exit;
}


include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

include APPRAIZ . "includes/cabecalho.inc";

echo "<br>";

if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros);
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
}

echo cabecalhoObra($obrid);

$habilitado = true;
$habilita = 'S';

monta_titulo($titulo_modulo, '');
?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript"
        src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>


<?php

$execucaoFinanceira = new ExecucaoFinanceira();
echo $execucaoFinanceira->criaSubAba($url, $habilitado, $obrid);


?>
<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
<?php

require APPRAIZ . 'obras2/includes/principal/finalizar/css.php';


global $db;

if ($estadoObra == ESDID_OBJ_CONCLUIDO || $estadoObra == ESDID_OBJ_CANCELADO || $estadoObra == ESDID_OBJ_INACABADA) {
    $onclickCheckbox = "HabilitaDesabilitaEnviar()";
    $onclickFinalizarExecucao = "onclick=\"finalizarDevolucao(" . $obrid . ")\"";
    $title = "Finalizar a Execução Financeira desta Obra.";
} else {
    $onclickCheckbox = "";
    $onclickFinalizarExecucao = "onclick=\"finalizarDevolucao(" . $obrid . ")\"";
    $title = "title='Não é possível finalizar a Execução Financeira, pois a obra nao encontra-se concluida.'";
}

$arrPerfisPermitidos = array(
    PFLCOD_GESTOR_UNIDADE,
    PFLCOD_SUPER_USUARIO,
    listarObrasFinalizar
);
 
$listarObrasFinalizar = $execucaoFinanceira->listarObrasFinalizar($obrid, $processo);
 

if ($listarObrasFinalizar) {
    ?>
<div class="abaContainer">
    <?php if (verificaObraSigpc(null,$processo)) {
    echo '<table width="100%" align="center" border="0"  cellpadding="3" cellspacing="1">
        <tr>
            <td colspan="2" style="background: #f00; color: #fff; text-align: center">
                <b>Este processo já possui prestação de contas inserida no SiGPC.</b>
            </td>
        </tr>
    </table>';

    } ?>
    <div class="abaContent">
        <table width="100%">
            <?php if (($estadoObra == ESDID_OBJ_CONCLUIDO || $estadoObra == ESDID_OBJ_CANCELADO || $estadoObra == ESDID_OBJ_INACABADA) &&
                (!$execucaoFinanceiraFinalizada) && (possui_perfil($arrPerfisPermitidos))) { ?>
                <tr>
                    <?php if ($pendencias) { ?>
                        <td align="center">
                            <input type="checkbox" name="ciente" id="ciente" onclick="<?= $onclickCheckbox ?>">&nbsp;
                            <label style="font-size: 12px">Declaro que estou ciente e concordo com as informações
                                financeiras declaradas</label>&nbsp;&nbsp;<br>
                            <button id="enviaExecucao" <?= $onclickFinalizarExecucao ?> <?= $title ?>>Finalizar Execução Financeira
                            </button>
                        </td>
                    <?php } else { ?>
                        <label style="font-size: 12px">Não é possível finalizar a execução financeira, pois exitem pendências! </label>&nbsp;&nbsp;<br>
                        <button disabled="disabled">Finalizar Execução Financeira
                        </button>
                    <?php } ?>
                </tr>
            <?php } elseif (($execucaoFinanceiraFinalizada) && (possui_perfil(array(
                    PFLCOD_SUPER_USUARIO,
                    PFLCOD_GESTOR_MEC
                )))) { ?>
                <tr>
                    <td align="center">
                        <button id="reabrirExecucao" onclick="rebrirExecFianceira(<?php echo $obrid ?>)">
                        	Reabrir Execução Financeira
                        </button>
                    </td>
                </tr>
            <?php } ?>
            <tr>
                <td>
                    <table border="0" width="100%" class="table table-bordered" id="obras">
                        <tr>
                            <td>

                                <?php
                                $cabecalho = array(
                                    'ID da Obra',
                                    'Nome da Obra',
                                    'Situação da obra',
                                    'Situação Execução Financeira'
                                );
                                $db->monta_lista(
                                    $listarObrasFinalizar,
                                    $cabecalho,
                                    60,
                                    5,
                                    'N',
                                    'center',
                                    'N',
                                    "formulario",
                                    '',
                                    '',
                                    null,
                                    null
                                );
                                }
                                ?>
                            </td>
                        </tr>
                    </table>
                </td>
                <td valign="top">
                    <div align="right">

                        <?php
                        $sqlPar3 = "SELECT COUNT(*) 
                                FROM obras2.obras o
                                INNER JOIN par3.obra po ON po.obrid = o.obrid_par3 AND o.obridpai IS NULL
                                INNER JOIN par3.processoobracomposicao poc ON poc.obrid = po.obrid
                                INNER JOIN par3.processo pp ON pp.proid = poc.proid
                                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid
                                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid
                                LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                				LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A'
                                WHERE 
                                    o.obrid = {$obrid}
                                    AND o.obridpai IS NULL
                                    AND o.obrstatus = 'A'";
                        
                        
                        $sqlConv = "SELECT COUNT(*) 
                                FROM obras2.obras o
                                INNER JOIN painel.dadosconvenios pd ON pd.dcoprocesso = o.obrnumprocessoconv AND o.obridpai IS NULL 
                                INNER JOIN workflow.documento wfd ON wfd.docid = o.docid 
                                INNER JOIN workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid 
                                LEFT JOIN obras2.execucaofinanceiracomposicao exc ON exc.obrid = o.obrid AND exc.excstatus = 'A' 
                                LEFT JOIN obras2.devolucao_gru_execucao_financeira dev ON dev.excid = exc.excid AND dev.devstatus = 'A' 
                                    WHERE o.obrnumprocessoconv  = '{$processo}' AND o.obridpai IS NULL AND o.obrstatus = 'A'";
                        
                        


                            $arrDados['exeprocesso'] = $processo;
                            $arrDados['exeid'] = $exeid;
                            if($arrDados['exeprocesso'] != 0 || $arrDados['exeprocesso'] != "") {
                                if (!$docid) {
                                    $execucaoFinanceira->salvarExecucaoFinanceira($arrDados, true);
                                    echo "<script> window.location.href = 'obras2.php?modulo=principal/finalizar&acao=O';</script>";
                                }
                                if ($db->pegaLinha($sqlConv)["count"] == 0 && $db->pegaLinha($sqlPar3)["count"] == 0) {
                                    $res = wf_desenhaBarraNavegacao($docid, array('obrid' => $obrid),
                                        array('acoes' => false));
                                }
                            }
                        ?>
                    </div>
                </td>
            </tr>
        </table>
    </div>
</div>

<script type="text/javascript">
    function HabilitaDesabilitaEnviar() {
        if (document.getElementById('ciente').checked == true) {
            document.getElementById('enviaExecucao').disabled = ""
        }

        if (document.getElementById('ciente').checked == false) {
            document.getElementById('enviaExecucao').disabled = "disabled"
        }
    }

    function finalizarDevolucao(obrid) {

        var check = false;
        if (document.getElementById('ciente').checked == false) {
            alert("Para finalizar a Execução Financeira é necessário declarar estar ciente das informações financeiras declaradas.");
            return false;
        }
        else {
            check = true;
        }

        jQuery.ajax({
            url: '?modulo=principal/finalizar&acao=O&finalizar_devolucao=S',
            type: 'post',
            data: 'obrid=' + obrid + '&check=' + check,
            dataType: 'json',

            success: function (data) {

                if (data.valor == 'false') {
                    alert(data.msg);
                    document.location.href = '?modulo=principal/finalizar&acao=O';
                } else {
                    alert('Execução Financeira realizada com sucesso.');
                    document.location.href = '?modulo=principal/finalizar&acao=O';
                }

            }

        });
    }

    function rebrirExecFianceira(obrid) {

        if (obrid == undefined) {
            alert('Falmtam parametros para realizar esta ação.');
            return false;
        }

        jQuery.ajax({
            url: '?modulo=principal/finalizar&acao=O&reabrirExecFinanceira=S',
            type: 'post',
            data: 'obrid=' + obrid,
            dataType: 'json',

            success: function (data) {

                if (data.valor == 'false') {
                    alert(data.msg);
                    document.location.href = '?modulo=principal/finalizar&acao=O';
                } else {
                    alert('Execução Financeira realizada com sucesso.');
                    document.location.href = '?modulo=principal/finalizar&acao=O';
                }

            }

        });
    }
</script>
