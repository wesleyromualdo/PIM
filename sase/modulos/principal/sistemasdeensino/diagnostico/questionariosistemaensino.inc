<?php

ob_start();


// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/Assessoramento.class.inc';
include_once '../../sase/classes/SituacaoAssessoramento.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
global $db;


$sql = "SELECT * FROM sase.questionariopergunta WHERE qpecod = '1' ORDER BY qpedescricao ASC";

$perguntas = $db->carregar($sql);


?>


<form class="form-horizontal" id="formSistemaensino" method="post" style="margin-left: 20px"
      enctype="multipart/form-data">


    <?php

    foreach ($perguntas as $pergunta):

        $sqlitemperguntas = "SELECT * FROM sase.questionarioitempergunta WHERE qpeid = {$pergunta['qpeid']}";

        $temperguntas = $db->carregar($sqlitemperguntas);
        switch ($pergunta['qpetipo']) {
            case 'R';

                if ($pergunta['qpeid'] != 1) {
                    $esconde = "questao1positivo";
                } else {
                    $esconde = "";
                }


                ?>
                <div class="form-group <?= $esconde ?>">
                    <label class="col-md-4 control-label"
                           for="questao_<?= $pergunta['qpeid'] ?>"><?= $pergunta['qpedescricao'] ?></label>
                    <div class="col-md-8">
                        <?php foreach ($temperguntas as $item): ?>

                            <?php
                            $sql = "SELECT   ir.qipid
                                    FROM sase.questionariopergunta r
                                        INNER JOIN sase.questionarioitempergunta ip ON ip.qpeid = r.qpeid
                                        INNER JOIN sase.questionarioitemresposta ir ON ir.qipid = ip.qipid
                                    WHERE
                                       r.qpestatus = 'A' AND
                                        ir.{$campotabela} = '{$estadomunicipio}' AND
                                        ip.qipid = {$item['qipid']}";

                            $valorItem = $db->pegaum($sql);
                            ?>

                            <label class="radio-inline" for="questao21-0">
                                <input type="radio" class="questao_<?= $pergunta['qpeid'] ?>"
                                    <?php if (in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)): ?>
                                        disabled
                                    <?php endif; ?>
                                       name="questao_<?= $pergunta['qpeid'] ?>" <?= $valorItem == $item['qipid'] ? "checked" : "" ?>
                                       value="<?= $item['qipid'] ?>">
                                <?= $item['qpidescricao'] ?>
                            </label>
                        <? endforeach; ?>
                    </div>
                </div>

                <? break;
            case 'C';
                ?>
                <div class="form-group questao1positivo">
                    <label class="col-md-4 control-label"
                           for="queformSistemaensinostao_<?= $pergunta['qpeid'] ?>"><?= $pergunta['qpedescricao'] ?></label>
                    <div class="col-md-8">
                        <?php foreach ($temperguntas as $item): ?>

                            <?php
                            $sql = "SELECT   ir.qipid
                                    FROM sase.questionariopergunta r
                                        INNER JOIN sase.questionarioitempergunta ip ON ip.qpeid = r.qpeid
                                        INNER JOIN sase.questionarioitemresposta ir ON ir.qipid = ip.qipid
                                    WHERE
                                        r.qpestatus = 'A' AND
                                        ir.{$campotabela} = '{$estadomunicipio}' AND
                                        ip.qipid = {$item['qipid']}";
                            $valorItem = $db->pegaum($sql);

                            ?>


                            <div class="checkbox">
                                <label for="questao22-0">
                                    <input type="checkbox" class="questao_<?= $pergunta['qpeid'] ?>"
                                        <?php if (in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)): ?>
                                            disabled
                                        <?php endif; ?>
                                           name="questao_<?= $pergunta['qpeid'] ?>[]" <?= $valorItem == $item['qipid'] ? "checked" : "" ?>
                                           value="<?= $item['qipid'] ?>">
                                    <?= $item['qpidescricao'] ?>
                                </label>
                            </div>

                        <? endforeach; ?>
                    </div>
                </div>

                <? break;
            case 'F';


                $sql = "SELECT   r.arqid           FROM sase.questionariopergunta qr
            INNER JOIN sase.questionarioresposta r on qr.qpeid = r.qpeid
            WHERE
            r.qrestatus = 'A' AND
            r.{$campotabela} = '{$estadomunicipio}' AND
            qr.qpeid = {$pergunta['qpeid']}
            ";

                $arqid = $db->pegaum($sql);
                if (!empty($_GET['muncod'])) {

                    $campourl = "estuf=" . $_GET['estuf'] . "&muncod=" . $_GET['muncod'];
                } else {
                    $campourl = "estuf=" . $_GET['estuf'];
                }


                ?>
                <div class="form-group questao1positivo">
                    <label class="col-md-4 control-label"
                           for="questao_<?= $pergunta['qpeid'] ?>"><?= $pergunta['qpedescricao'] ?></label>
                    <div class="col-md-8">
                        <?php if (!empty($arqid)) { ?>
                            <button type="button" id="linkArqid_<?= $pergunta['qpeid'] ?>" class="btn btn-link btn-sm"
                                    style="cursor:pointer;"
                                    onclick="window.location='sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A&<?= $campourl ?>&requisicao=downloadAnexo&amp;arqid=<?= $arqid ?>'">
                                <span class="glyphicon glyphicon-paperclip"></span> Anexo
                            </button>
                            <input id="arqid_questao_<?= $pergunta['qpeid'] ?>"
                                   name="arqid_questao_<?= $pergunta['qpeid'] ?>" type="hidden" value="1"
                                <?php if (in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)): ?>
                                    disabled
                                <?php endif; ?>
                            />  <span style="font-size: 10px">*Para substituir o arquivo escolha outro arquivo.</span>
                        <?php } ?>
                        <input id="questao_<?= $pergunta['qpeid'] ?>" value="1" name="questao_<?= $pergunta['qpeid'] ?>"
                               class="input-file" type="file"
                            <?php if (in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)): ?>
                                disabled
                            <?php endif; ?>
                        >
                    </div>
                </div>


                <? break;
            case 'T';
                ?>
                <div class="form-group questao1positivo">
                    <label class="col-md-4 control-label"
                           for="questao_<?= $pergunta['qpeid'] ?>"><?= $pergunta['qpedescricao'] ?></label>
                    <div class="col-md-8">

                        <input id="questao_<?= $pergunta['qpeid'] ?>" name="questao_<?= $pergunta['qpeid'] ?>"
                               type="text" class="form-control input-md"
                            <?php if (in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)): ?>
                                disabled
                            <?php endif; ?>
                        >
                    </div>
                </div>


                <? break;


        }

        ?>

    <? endforeach; ?>
    <div class="form-group">
        <label class="col-md-4 control-label" for="singlebutton"></label>
        <div class="col-md-4">
            <?php if (in_array(PFLCOD_SASE_SECRETARIO, $perfis)): ?>
                <button class="btn btn-primary" role="tab" data-toggle="tab" id="btn01" type="button">
                    <span class="glyphicon glyphicon-ok"></span> Salvar
                </button>
                <a href="#profile" aria-controls="profile" role="tab" data-toggle="tab" id="btnv000" type="button" name="singlebutton" class="btn btn-primary ">
                    Próxima <span class="glyphicon glyphicon-arrow-right"></span>
                </a>
            <?php endif; ?>
            <?php if (in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)): ?>
                <a href="#profile" aria-controls="profile" role="tab" data-toggle="tab" id="btnv00" type="button" name="singlebutton" class="btn btn-primary ">
                    Próxima <span class="glyphicon glyphicon-arrow-right"></span>
                </a>
            <?php endif; ?>
        </div>

    </div>
</form>


<script>
    $(document).ready(function () {
        $(".questao1positivo").hide();
        $("input[name$='questao_1']").click(function () {


            if ($(this).val() == '1') {/*   dataType: 'json',*/
                $(".questao1positivo").slideDown("slow");
            } else {
                $(".questao1positivo").slideUp("slow");

                $(".questao_2").prop("checked", false);
                $(".questao_4").prop("checked", false);
            }


        });

        var questao_1 = $("input[name$='questao_1']:checked").val();
        if (questao_1 == 1) {
            $(".questao1positivo").slideDown("slow");
        }

        function validoFormatoArquivo(arquivo) {
            var extensoes, ext, valido;
            extensoes = new Array('.pdf');

            ext = arquivo.substring(arquivo.lastIndexOf(".")).toLowerCase();
            valido = false;

            for (var i = 0; i <= arquivo.length; i++) {
                if (extensoes[i] == ext) {
                    valido = true;
                    break;
                }
            }

            if (valido) {
                return true;
            }

            return false;
        }


        $("#btn01").click(function () {
            
            salvarQuestoesAba("#btn01", false, false);
//            var questao_1 = $("input[name$='questao_1']:checked").val();
//
//            if (questao_1) {
//                if (questao_1 == 1) {
//                    var questao_2 = $("input[name$='questao_2']:checked").val();
//                    if (!questao_2) {
//                        alert("Preencher pergunta 1.2 Qual ato legal instituiu o sistema estadual/distrital/municipal de ensino?");
//                        return false;
//                    }
//                    /*   dataType: 'json',*/
//
//
//                    var questao_3 = $("#questao_3").val();
//
//
//                    if (questao_3 == '') {
//                        if ($("#arqid_questao_3").val() != '1') {
//
//                            alert("Preencher pergunta 1.3 Anexar cópia do ato legal mencionado no item anterior.");
//                            return false;
//                        }
//                    } else {
//
//                        var validaextenao = validoFormatoArquivo(questao_3);
//                        if (!validaextenao) {
//                            alert("Anexar arquivo do tipo PDF!");
//                            return false;
//                        }
//
//                    }
//
//                    var camposMarcados = new Array();
//                    $("input[type=checkbox][name='questao_4[]']:checked").each(function () {
//                        camposMarcados.push($(this).val());
//                    });
//
//                    if (camposMarcados == '') {
//                        alert("Preencher pergunta 1.4 Qual a constituição do sistema de ensino? Marque as alternativas aplicáveis:");
//                        return false;
//                    }
//                }
//                ;
//
//
//                var form_data = new FormData();
//
//                form_data.append('aba1', 'true');
//                form_data.append('questao_1', questao_1);
//                form_data.append('questao_2', (questao_2 === undefined) ? '' : questao_2);
//                if ($("#questao_3").val() != '' ) {
//                    var file_data = $("#questao_3").prop('files')[0];
//                    form_data.append('questao_3', file_data);
//
//                } else {
//                    form_data.append('arqid_questao_3', 1);
//                }
//                form_data.append('questao_4', (camposMarcados === undefined) ? '' : camposMarcados);
//                var campotabela = "<?=$campotabela?>"
//                var estadomunicipio = "<?=$estadomunicipio?>"
//                form_data.append('campotabela', campotabela);
//                form_data.append('estadomunicipio', estadomunicipio);
//
//                $.ajax({
//                    url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
//                    dataType: 'text',  // what to expect back from the PHP script, if anything
//                    cache: false,
//                    contentType: false,
//                    processData: false,
//                    data: form_data,
//                    type: 'post',
//                    success: function (response) {
//                        if ($("#arqid_questao_3").val() != 1) {
//                            $('#linkArqid_3').hide();
//                        }
//                        alert("Os dados foram salvos com sucesso!");
//
//                        /* location.reload();*/
//                    }
//                });
//
//            } else {
//                alert("1.1 O estado/Distrito Federal/município possui sistema de ensino instituído? é Obrigatório!");
//                return false;
//            }
//            ;


        });




        $("#btnv00").click(function () {
            $("#presentation_1").removeClass('active');
            $("#presentation_2").addClass('active');
        });

        $("#btnv000").click(function () {
            
            salvarQuestoesAba("#btnv000", true, false);            
            $("#presentation_1").removeClass('active');
            $("#presentation_2").addClass('active');
            
//            var questao_1 = $("input[name$='questao_1']:checked").val();
//
//            if (questao_1) {
//                if (questao_1 == 1) {
//                    var questao_2 = $("input[name$='questao_2']:checked").val();
//                    if (!questao_2) {
//                        alert("Preencher pergunta 1.2 Qual ato legal instituiu o sistema estadual/distrital/municipal de ensino?");
//                        return false;
//                    }
//                    /*   dataType: 'json',*/
//
//
//                    var questao_3 = $("#questao_3").val();
//
//
//                    if (questao_3 == '') {
//                        if ($("#arqid_questao_3").val() != '1') {
//
//                            alert("Preencher pergunta 1.3 Anexar cópia do ato legal mencionado no item anterior.");
//                            return false;
//                        }
//                    } else {
//
//                        var validaextenao = validoFormatoArquivo(questao_3);
//                        if (!validaextenao) {
//                            alert("Anexar arquivo do tipo PDF!");
//                            return false;
//                        }
//
//                    }
//
//                    var camposMarcados = new Array();
//                    $("input[type=checkbox][name='questao_4[]']:checked").each(function () {
//                        camposMarcados.push($(this).val());
//                    });
//
//                    if (camposMarcados == '') {
//                        alert("Preencher pergunta 1.4 Qual a constituição do sistema de ensino? Marque as alternativas aplicáveis:");
//                        return false;
//                    }
//                }
//                ;
//
//
//                var form_data = new FormData();
//
//                form_data.append('aba1', 'true');
//                form_data.append('questao_1', questao_1);
//                form_data.append('questao_2', (questao_2 === undefined) ? '' : questao_2);
//                if ($("#arqid_questao_3").val() != 1) {
//                    var file_data = $("#questao_3").prop('files')[0];
//                    form_data.append('questao_3', file_data);
//
//                } else {
//                    form_data.append('arqid_questao_3', 1);
//                }
//                form_data.append('questao_4', (camposMarcados === undefined) ? '' : camposMarcados);
//                var campotabela = "<?=$campotabela?>"
//                var estadomunicipio = "<?=$estadomunicipio?>"
//                form_data.append('campotabela', campotabela);
//                form_data.append('estadomunicipio', estadomunicipio);
//
//                $.ajax({
//                    url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
//                    dataType: 'text',  // what to expect back from the PHP script, if anything
//                    cache: false,
//                    contentType: false,
//                    processData: false,
//                    data: form_data,
//                    type: 'post',
//                    success: function (response) {
//                        if ($("#arqid_questao_3").val() != 1) {
//                            $('#linkArqid_3').hide();
//                        }
//                        alert("Os dados foram salvos com sucesso!");
//                        $("#presentation_1").removeClass('active');
//                        $("#presentation_2").addClass('active');
//                        /* location.reload();*/
//                    }
//                });
//
//            } else {
//                alert("1.1 O estado/Distrito Federal/município possui sistema de ensino instituído? é Obrigatório!");
//                return false;
//            }
//            ;
//
//
        });

    });
</script>

