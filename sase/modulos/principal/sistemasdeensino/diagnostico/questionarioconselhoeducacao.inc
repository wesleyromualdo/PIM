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


$sql = "SELECT * FROM sase.questionariopergunta WHERE qpecod = '2' ORDER BY qpedescricao ASC";

$perguntas = $db->carregar($sql);



?>


<form class="form-horizontal" id="" style="margin-left: 20px">


    <?php

foreach ($perguntas as $pergunta):

    $sqlitemperguntas  = "SELECT * FROM sase.questionarioitempergunta WHERE qpeid = {$pergunta['qpeid']}";

    $temperguntas= $db->carregar($sqlitemperguntas);
    switch ($pergunta['qpetipo']) {
        case 'R';

            if($pergunta['qpeid'] != 5){
                $esconde = "questao5positivo";
            }else{
                $esconde = "";
            }
            ?>
            <div class="form-group <?=$esconde?>">
                <label class="col-md-4 control-label" for="questao_<?=$pergunta['qpeid']?>"><?=$pergunta['qpedescricao']?></label>
                <div  class="col-md-8">
                    <?php foreach ($temperguntas as $item):?>
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
                            <input type="radio" class="questao_<?=$pergunta['qpeid']?>"  name="questao_<?=$pergunta['qpeid']?>"  <?=$valorItem == $item['qipid'] ? "checked" : ""?> value="<?=$item['qipid']?>"
                                <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                                    disabled
                                <?php endif;?>>
                            <?=$item['qpidescricao']?>
                        </label>
                    <? endforeach;?>
                </div>
            </div>

            <? break;
        case 'C';
            ?>
            <div class="form-group questao5positivo">
                <label class="col-md-4 control-label" for="questao_<?=$pergunta['qpeid']?>"><?=$pergunta['qpedescricao']?></label>
                <div  class="col-md-8">
                    <?php foreach ($temperguntas as $item):?>
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
                                <input type="checkbox" class="questao_<?=$pergunta['qpeid']?>"  name="questao_<?=$pergunta['qpeid']?>[]"
                                    <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                                        disabled
                                    <?php endif;?>
                                    <?=$valorItem == $item['qipid'] ? "checked" : ""?> value="<?=$item['qipid']?>">
                                <?=$item['qpidescricao']?>
                            </label>
                        </div>

                    <? endforeach;?>
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

            $campourl = "estuf=".$_GET['estuf']."&muncod=".$_GET['muncod'];
            } else {
            $campourl = "estuf=".$_GET['estuf'];
            }


            ?>
            <div class="form-group questao5positivo">
                <label class="col-md-4 control-label"
                       for="questao_<?= $pergunta['qpeid'] ?>"><?= $pergunta['qpedescricao'] ?></label>
                <div class="col-md-8">
                    <?php if(!empty($arqid)){?>
                        <button type="button" id="linkArqid_7" class="btn btn-link btn-sm" style="cursor:pointer;" onclick="window.location='sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A&<?=$campourl?>&requisicao=downloadAnexo&amp;arqid=<?=$arqid?>'"><span class="glyphicon glyphicon-paperclip"></span> Anexo</button>
                        <input id="arqid_questao_<?= $pergunta['qpeid'] ?>" name="arqid_questao_<?= $pergunta['qpeid'] ?>" type="hidden" value="1"
                            <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                            disabled
                        <?php endif;?>/>  <span style="font-size: 10px">*Para substituir o arquivo escolha outro arquivo.</span>
                    <?php    } ?>
                    <input id="questao_<?= $pergunta['qpeid'] ?>" name="questao_<?= $pergunta['qpeid'] ?>"
                           class="input-file" type="file" <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                        disabled
                    <?php endif;?>>
                </div>
            </div>




            <? break;
        case 'T';
            ?>
            <div class="form-group questao5positivo">
                <label class="col-md-4 control-label" for="questao_<?=$pergunta['qpeid']?>"><?=$pergunta['qpedescricao']?></label>
                <div  class="col-md-8">

                    <input id="questao_<?=$pergunta['qpeid']?>" name="questao_<?=$pergunta['qpeid']?>" type="text" class="form-control input-md"
                        <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                            disabled
                        <?php endif;?>
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
        <a href="#home" aria-controls="home" role="tab" data-toggle="tab" id="btnv01" type="button" name="singlebutton" class="btn btn-primary ">
            <span class="glyphicon glyphicon-arrow-left"></span> Voltar
        </a>
        <?php if(in_array(PFLCOD_SASE_SECRETARIO, $perfis)):?>
            <button class="btn btn-primary" role="tab" data-toggle="tab" id="btn02"  type="button">
                <span class="glyphicon glyphicon-ok"></span> Salvar
            </button>
            <a href="#messages" aria-controls="messages" role="tab" data-toggle="tab" id="btnp022" type="button" name="singlebutton" class="btn btn-primary ">
                Próxima <span class="glyphicon glyphicon-arrow-right"></span>
            </a>
        <?php endif;?>
        <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
            <a href="#messages" aria-controls="messages" role="tab" data-toggle="tab" id="btnp02" type="button" name="singlebutton" class="btn btn-primary ">
                Próxima <span class="glyphicon glyphicon-arrow-right"></span>
            </a>
        <?php endif;?>
    </div>
</div>

</form>
<script>
    $(document).ready(function () {
        $(".questao5positivo").hide();
        $("input[name$='questao_5']").click(function () {


            if ($(this).val() == '13'){
                $(".questao5positivo").slideDown("slow");
            }else{
                $(".questao5positivo").slideUp("slow");
                $(".questao_6").prop( "checked" , false);
                $(".questao_8").prop( "checked" , false);
                $(".questao_9").prop( "checked" , false);
                $(".questao_10").prop( "checked" , false);
                $(".questao_11").prop( "checked" , false);
                $(".questao_12").prop( "checked" , false);

            }


        });

        var questao_5 = $("input[name$='questao_5']:checked").val();

        if(questao_5 == 13){
            $(".questao5positivo").slideDown("slow");
        }


        $("#btnp02").click(function () {
            $("#presentation_2").removeClass('active');
            $("#presentation_3").addClass('active');
        });
        $("#btnv01").click(function () {
            $("#presentation_2").removeClass('active');
            $("#presentation_1").addClass('active');
        });

        $("#btn02").click(function () {
            salvarQuestoesAba("#btn02", false, false);
//            var questao_5 = $("input[name$='questao_5']:checked").val();
//
//            if(questao_5 ){
//                if(questao_5 == 13 ){
//                    var questao_6 = $("input[name$='questao_6']:checked").val();
//                    if(!questao_6){
//                        alert("Preencher pergunta 2.2 Qual o ato legal de criação do conselho de educação?");
//                        return false;
//                    }
//                    var questao_7 = $("#questao_7").val();
//                    if(questao_7 == ''){
//                        if($("#arqid_questao_7").val() != '1'){
//
//                            alert("Preencher pergunta 2.3 Anexar cópia do ato legal mencionado no item anterior.");
//                            return false;
//                        }
//                    }else{
//
//                        var validaextenao =  validoFormatoArquivo(questao_7);
//                        if(!validaextenao){
//                            alert("Anexar arquivo do tipo PDF!");
//                            return false;
//                        }
//
//                    }
//
//                    var questao_8 = $("input[name$='questao_8']:checked").val();
//                    if(!questao_8){
//                        alert("Preencher pergunta 2.4 A composição do conselho de educação é paritária?");
//                        return false;
//                    }
//
//                    var camposMarcados25 = new Array();
//                    $("input[type=checkbox][name='questao_9[]']:checked").each(function(){
//                        camposMarcados25.push($(this).val());
//                    });
//
//                    if(camposMarcados25 == ''){
//                        alert("Preencher pergunta 2.5 Assinale as alternativas equivalentes aos representantes no conselho de educação:");
//                        return false;
//                    }
//
//                    var questao_10 = $("input[name$='questao_10']:checked").val();
//                    if(!questao_10){
//                        alert("Preencher pergunta 2.6 O conselho de educação possui dotação orçamentária própria?");
//                        return false;
//                    }
//                    var questao_11 = $("input[name$='questao_11']:checked").val();
//                    if(!questao_11){
//                        alert("Preencher pergunta 2.7 O conselho de educação possui plena autonomia para a homologação dos atos?");
//                        return false;
//                    }
//
//
//                };
//
//
//                var form_data = new FormData();
//
//
//                if ($("#questao_7").val() != '' ) {
//                    var file_data = $("#questao_7").prop('files')[0];
//                    form_data.append('questao_7', file_data);
//                }else {
//                    form_data.append('arqid_questao_7', 1);
//                }
//
//                form_data.append('aba2', 'true');
//                form_data.append('questao_5', (questao_5 === undefined)?'':questao_5);
//                form_data.append('questao_6', (questao_6 === undefined)?'':questao_6);
//                form_data.append('questao_8', (questao_8 === undefined)?'':questao_8);
//                form_data.append('questao_9', (camposMarcados25 === undefined)?'':camposMarcados25);
//                form_data.append('questao_10', (questao_10 === undefined)?'':questao_10);
//                form_data.append('questao_11', (questao_11 === undefined)?'':questao_11);
//
//                var campotabela = "<?=$campotabela?>"
//                var estadomunicipio = "<?=$estadomunicipio?>"
//                form_data.append('campotabela', campotabela);
//                form_data.append('estadomunicipio', estadomunicipio);
//                //alert(form_data);
//
//                $.ajax({
//                    url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
//                    dataType: 'text',  // what to expect back from the PHP script, if anything
//                    cache: false,
//                    contentType: false,
//                    processData: false,
//                    data: form_data,
//                    type: 'post',
//                    success: function(response){
//                        if($("#arqid_questao_7").val() != 1) {
//                            $('#linkArqid_7').hide();
//                        }
//
//                        alert("Os dados foram salvos com sucesso!");
//
//                    }
//                });
//
//            }else {
//                alert("2.1 O estado/Distrito Federal/município possui conselho de educação?");
//                return false;
//            };



        });


        $("#btnp022").click(function () {
            
            salvarQuestoesAba("#btnp022", true, false);
            $("#presentation_2").removeClass('active');
            $("#presentation_3").addClass('active');
            
//            var questao_5 = $("input[name$='questao_5']:checked").val();
//
//            if(questao_5 ){
//                if(questao_5 == 13 ){
//                    var questao_6 = $("input[name$='questao_6']:checked").val();
//                    if(!questao_6){
//                        alert("Preencher pergunta 2.2 Qual o ato legal de criação do conselho de educação?");
//                        return false;
//                    }
//                    var questao_7 = $("#questao_7").val();
//                    if(questao_7 == ''){
//                        if($("#arqid_questao_7").val() != '1'){
//
//                            alert("Preencher pergunta 2.3 Anexar cópia do ato legal mencionado no item anterior.");
//                            return false;
//                        }
//                    }else{
//
//                        var validaextenao =  validoFormatoArquivo(questao_7);
//                        if(!validaextenao){
//                            alert("Anexar arquivo do tipo PDF!");
//                            return false;
//                        }
//
//                    }
//
//                    var questao_8 = $("input[name$='questao_8']:checked").val();
//                    if(!questao_8){
//                        alert("Preencher pergunta 2.4 A composição do conselho de educação é paritária?");
//                        return false;
//                    }
//
//                    var camposMarcados25 = new Array();
//                    $("input[type=checkbox][name='questao_9[]']:checked").each(function(){
//                        camposMarcados25.push($(this).val());
//                    });
//
//                    if(camposMarcados25 == ''){
//                        alert("Preencher pergunta 2.5 Assinale as alternativas equivalentes aos representantes no conselho de educação:");
//                        return false;
//                    }
//
//                    var questao_10 = $("input[name$='questao_10']:checked").val();
//                    if(!questao_10){
//                        alert("Preencher pergunta 2.6 O conselho de educação possui dotação orçamentária própria?");
//                        return false;
//                    }
//                    var questao_11 = $("input[name$='questao_11']:checked").val();
//                    if(!questao_11){
//                        alert("Preencher pergunta 2.7 O conselho de educação possui plena autonomia para a homologação dos atos?");
//                        return false;
//                    }
//
//
//                };
//
//
//                var form_data = new FormData();
//
//
//                if($("#arqid_questao_7").val() != 1){
//                    var file_data = $("#questao_7").prop('files')[0];
//                    form_data.append('questao_7', file_data);
//                }else {
//                    form_data.append('arqid_questao_7', 1);
//                }
//
//                form_data.append('aba2', 'true');
//                form_data.append('questao_5', (questao_5 === undefined)?'':questao_5);
//                form_data.append('questao_6', (questao_6 === undefined)?'':questao_6);
//                form_data.append('questao_8', (questao_8 === undefined)?'':questao_8);
//                form_data.append('questao_9', (camposMarcados25 === undefined)?'':camposMarcados25);
//                form_data.append('questao_10', (questao_10 === undefined)?'':questao_10);
//                form_data.append('questao_11', (questao_11 === undefined)?'':questao_11);
//
//                var campotabela = "<?=$campotabela?>"
//                var estadomunicipio = "<?=$estadomunicipio?>"
//                form_data.append('campotabela', campotabela);
//                form_data.append('estadomunicipio', estadomunicipio);
//                //alert(form_data);
//
//                $.ajax({
//                    url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
//                    dataType: 'text',  // what to expect back from the PHP script, if anything
//                    cache: false,
//                    contentType: false,
//                    processData: false,
//                    data: form_data,
//                    type: 'post',
//                    success: function(response){
//                        if($("#arqid_questao_7").val() != 1) {
//                            $('#linkArqid_7').hide();
//                        }
//
//                        alert("Os dados foram salvos com sucesso!");
//                        $("#presentation_2").removeClass('active');
//                        $("#presentation_3").addClass('active');
//                    }
//                });
//
//            }else {
//                alert("2.1 O estado/Distrito Federal/município possui conselho de educação?");
//                return false;
//            };

        });


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


    });
</script>

