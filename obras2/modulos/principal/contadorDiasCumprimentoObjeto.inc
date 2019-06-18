<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<?php

function verificaDataLeituraNotificacaoCumprimentoObjeto($obrid) {

    global $db;

    $sql = "SELECT
              ra.rgadtinclusao
            FROM
              obras2.registroatividade ra
            WHERE
              ra.obrid = {$obrid}
            AND
              ra.rgadscsimplificada ILIKE '%leitura da notificação cumprimento%';
            ";

    $rgaDtInclusao = $db->pegaUm($sql);

    return $rgaDtInclusao;
}

function pegaCoid($obrid){

    global $db;

    $sqlCoid = "SELECT 
                            coid 
                        FROM 
                            obras2.cumprimento_objeto  
                        WHERE 
                            obrid = {$obrid} 
                        AND 
                            status = 'A' 
                        ORDER BY coid DESC LIMIT 1;";

    $coid = $db->pegaUm($sqlCoid);

    return $coid;
}

function verificaPrazoPreenchimentoCumprimentoObjeto($obrid){

    global $db;

    $sql = "SELECT 
                pcoid, pcodatavigencia, pcodataleitura 
            FROM 
                obras2.prazocumprimentoobjeto 
            WHERE 
                obrid = {$obrid}
            AND 
                pcostatus = 'A';
            ";

    $resultado = $db->pegaLinha($sql);

    return $resultado;

}

$resultado = verificaPrazoPreenchimentoCumprimentoObjeto($obrid);

if(!$resultado){

    $dtLeituraNotificacao = verificaDataLeituraNotificacaoCumprimentoObjeto($obrid);

    if($dtLeituraNotificacao){

        $data = DateTime::createFromFormat('Y-m-d', $dtLeituraNotificacao);
        $dataSomada = $data->add(new DateInterval('P60D'));
        $dataVigencia = $dataSomada->format('Y-m-d');

        $coid = pegaCoid($obrid);

        if($coid) {
            $sqlSalvar = "INSERT INTO
                                obras2.prazocumprimentoobjeto
                                  (obrid, coid,pcodataleitura, pcodatavigencia)
                                VALUES 
                                  ($obrid, $coid, '{$dtLeituraNotificacao}', '{$dataVigencia}');
                                ";

            if ($db->executar($sqlSalvar)) {
                $db->commit();
                $resultado = verificaPrazoPreenchimentoCumprimentoObjeto($obrid);
            } else {
                $resultado = false;
            }
        }
    }
    else{
        $resultado = false;
    }
}

if (!empty($resultado)) {
    $dataVigencia = $resultado['pcodatavigencia'];
    $dataLeitura = $resultado['pcodataleitura'];

    $objDtVigencia = new DateTime($dataVigencia);
    $objDtAtual = new DateTime('now');
    $diff = $objDtAtual->diff($objDtVigencia);
    $contadorDias = $diff->format('%R%a');
}

//$classe = ($contadorDias < 0) ? "alert alert-danger text-center" : "alert alert-success text-center";

if ( ($contadorDias > 0) || ( ((string) $contadorDias) === '+0')) {
    $contadorDias = $contadorDias + 1;
    $classe = "alert alert-success text-center";
    $mensagem = "Restam <u><b> $contadorDias </b></u> dias para o término do prazo de <br>preenchimento desta aba.";
} elseif ($contadorDias == 0) {
    $classe = "alert alert-success text-center";
    $mensagem = "O prazo para preenchimento desta aba termina hoje.";
} else {
    $classe = "alert alert-danger text-center";
    $mensagem = "Restam <u><b> $contadorDias </b></u> dias para o término do prazo de <br>preenchimento desta aba.";
}

?>

<div>
    <br/>
    <table class="<?php echo $classe ?>" style="border: 3px solid lightgray; border-collapse: collapse">
        <?php if (!$resultado): ?>
            <tr>
                <td><b>A entidade ainda não notificou a leitura do cumprimento do objeto</b></td>
            </tr>
        <?php else: ?>

            <tr>
                <td align="center" style="border: 3px solid lightgray;padding: 5px">
                    <b>Data da leitura da Notificação: <u><?php echo date('d/m/Y', strtotime($dataLeitura)) ?></u></b>
                </td>
            </tr>

            <tr>
                <td align="center" style="border: 3px solid lightgray;padding: 5px;">
                    <?php echo $mensagem; ?>
                </td>
            </tr>

            <?php if(possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, ))): ?>
            <tr>
                <td style="border: 3px solid lightgray; padding: 5px">
                    <input type="button" value="Redefinir prazo" onclick="abrirModalRedefinirPrazo();"/>
                </td>
            </tr>
            <?php endif; ?>

        <?php endif; ?>
    </table>

    <div class="dialog" title="Redefinir prazo" style="display:none;">
        <form id="formNovoPrazo">
        <div class="container">
            <div class="row">
                <div class="col-md-3">Quantidade (dias):</div>
                <div class="col-md-6"><?=campo_texto( 'novoprazopreenchimento', 'S', 'S', '', 10, 80, '', '', 'left', '', 0, 'id="novoPrazo"');  ?></div>
            </div>
            <br/>

            <div class="row">
                <div class="col-md-9" id="msgNovoPrazo" style="padding: 5px; width: 90% ;background-color: #f2dede; border: 1px solid #eed3d7;text-align: justify; font-weight: bold; font-size: 12px; color: #b94a48; display: none"></div>
            </div>
        </div>
        </form>
    </div>

    <script>

        function abrirModalRedefinirPrazo() {

            var originalFormNovoPrazo = $("#formNovoPrazo").html();

            $(".dialog").dialog({
                resizable: false,
                height: 210,
                width: 340,
                modal: true,
                buttons: {
                    "Salvar": function () {

                        var novoPrazo = $("#novoPrazo").val();
                        var reg = /^\d+$/;

                        if (novoPrazo == "") {
                            $("#msgNovoPrazo").show();
                            $("#msgNovoPrazo").html("Informe o novo prazo em dias.");
                            $("#novoPrazo").focus();
                            return false;
                        }

                        if(!reg.test(novoPrazo)) {
                            $("#msgNovoPrazo").show();
                            $("#msgNovoPrazo").html("Valor com formato inválido.");
                            $("#novoPrazo").focus();
                            return false;
                        }

                        $.ajax({
                            type: "post",
                            url: window.location.href,
                            data: {
                                "req": "novoPrazo",
                                "novoprazo": novoPrazo
                            },
                            async: false,
                            success: function (data) {
                                var retorno = (data == "true") || false;
                                if (!retorno) {
                                    $("#msgNovoPrazo").show();
                                    $("#msgNovoPrazo").html("Houve um problema ao redefinir o prazo.");
                                    return false;
                                }

                                $("#dialog").dialog("close");
                                alert("Prazo redefinido com sucesso.");
                                window.location.href = 'obras2.php?modulo=principal/cadCumprimentoObjeto&acao=A';
                            }
                        });
                    },
                    "Cancelar": function () {
                        $("#formNovoPrazo").html(originalFormNovoPrazo);
                        $(this).dialog("close");
                    }
                }
            });
        }
    </script>
</div>
