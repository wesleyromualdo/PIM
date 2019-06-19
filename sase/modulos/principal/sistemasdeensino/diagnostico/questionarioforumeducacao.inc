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


$sql = "SELECT * FROM sase.questionariopergunta WHERE qpecod = '3' ORDER BY qpedescricao ASC";

$perguntas = $db->carregar($sql);


$titulos = array(   1=> 'Sistema de Ensino',
    1=> 'Sistema de Ensino',
    1=> 'Sistema de Ensino',
    1=> 'Sistema de Ensino',
    1=> 'Sistema de Ensino');
?>
<form class="form-horizontal" id="" method="post" style="margin-left: 20px" enctype="multipart/form-data">

<?php

foreach ($perguntas as $pergunta):

    $sqlitemperguntas  = "SELECT * FROM sase.questionarioitempergunta WHERE qpeid = {$pergunta['qpeid']}";

    $temperguntas= $db->carregar($sqlitemperguntas);
    switch ($pergunta['qpetipo']) {
        case 'R';
            if($pergunta['qpeid'] != 12){
                $esconde = "questao12positivo";
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
                            <input type="radio"  class="questao_<?=$pergunta['qpeid']?>" name="questao_<?=$pergunta['qpeid']?>"
                                <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                                    disabled
                                <?php endif;?> <?=$valorItem == $item['qipid'] ? "checked" : ""?> value="<?=$item['qipid']?>">
                            <?=$item['qpidescricao']?>
                        </label>
                    <? endforeach;?>
                </div>
            </div>

            <? break;
        case 'C';
            ?>
            <div class="form-group questao12positivo">
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
                                <input type="checkbox"  class="questao_<?=$pergunta['qpeid']?>"  name="questao_<?=$pergunta['qpeid']?>"
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
            <div class="form-group questao12positivo">
                <label class="col-md-4 control-label"
                       for="questao_<?= $pergunta['qpeid'] ?>"><?= $pergunta['qpedescricao'] ?></label>
                <div class="col-md-8">
                    <?php if(!empty($arqid)){?>
                        <button type="button" id="linkArqid_14" class="btn btn-link btn-sm" style="cursor:pointer;" onclick="window.location='sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A&<?=$campourl?>&requisicao=downloadAnexo&amp;arqid=<?=$arqid?>'"><span class="glyphicon glyphicon-paperclip"></span> Anexo</button>
                        <input id="arqid_questao_<?= $pergunta['qpeid'] ?>" name="arqid_questao_<?= $pergunta['qpeid'] ?>" type="hidden" value="1"  />  <span style="font-size: 10px">*Para substituir o arquivo escolha outro arquivo.</span>
                    <?php    } ?>
                    <input id="questao_<?= $pergunta['qpeid'] ?>" name="questao_<?= $pergunta['qpeid'] ?>"
                           class="input-file" type="file" <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                        disabled
                    <?php endif;?>>
                </div>
            </div>




            <? break;



    }

    ?>








<? endforeach; ?>

    <div class="form-group">
        <label class="col-md-4 control-label" for="singlebutton"></label>
        <div class="col-md-4">
            <a href="#profile" aria-controls="profile" role="tab" data-toggle="tab" id="btnv03" type="button" name="singlebutton" class="btn btn-primary ">
                <span class="glyphicon glyphicon-arrow-left"></span> Voltar
            </a>
            <?php if(in_array(PFLCOD_SASE_SECRETARIO, $perfis)):?>
                <button class="btn btn-primary" role="tab" data-toggle="tab" id="btn03"  type="button">
                    <span class="glyphicon glyphicon-ok"></span> Salvar
                </button>
                <a href="#settings" aria-controls="settings" role="tab" data-toggle="tab" id="btnp033" type="button" name="singlebutton" class="btn btn-primary ">
                    Próxima <span class="glyphicon glyphicon-arrow-right"></span>
                </a>
            <?php endif;?>
            <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                <a href="#settings" aria-controls="settings" role="tab" data-toggle="tab" id="btnp03" type="button" name="singlebutton" class="btn btn-primary ">
                    Próxima <span class="glyphicon glyphicon-arrow-right"></span>
                </a>
            <?php endif;?>
        </div>
    </div>
</form>

<script>
    $(document).ready(function () {

        $("#btnp03").click(function () {
            $("#presentation_3").removeClass('active');
            $("#presentation_4").addClass('active');

        });

         $("#btnv03").click(function () {
            $("#presentation_3").removeClass('active');
            $("#presentation_2").addClass('active');

        });

        $(".questao12positivo").hide();
        $("input[name$='questao_12']").click(function () {


            if ($(this).val() == '33'){
                $(".questao12positivo").slideDown("slow");
            }else{
                $(".questao12positivo").slideUp("slow");
                $(".questao_13").prop( "checked" , false);

            }


        });

        var questao_12 = $("input[name$='questao_12']:checked").val();
        if (questao_12 == '33'){
            $(".questao12positivo").slideDown("slow");
        }


        $("#btn03").click(function () {
            salvarQuestoesAba("#btn03", false, false);
//            var questao_12 = $("input[name$='questao_12']:checked").val();
//
//            if(questao_12 ){
//                if(questao_12 == 33 ){
//                    var questao_13 = $("input[name$='questao_13']:checked").val();
//                    if(!questao_13){
//                        alert("Preencher pergunta 3.2 Qual ato legal instituiu o fórum municipal de educação?");
//                        return false;
//                    }
//
//                    var questao_14 = $("#questao_14").val();
//                    if(questao_14 == ''){
//                        if($("#arqid_questao_14").val() != '1'){
//
//                            alert("Preencher pergunta 3.3 Anexar cópia do ato legal mencionado no item anterior.");
//                            return false;
//                        }
//
//
//                    }else{
//
//                        var validaextenao =  validoFormatoArquivo(questao_14);
//                        if(!validaextenao){
//                            alert("Anexar arquivo do tipo PDF!");
//                            return false;
//                        }
//
//                    }
//
//
//
//                };
//
//
//                var form_data = new FormData();
//
//
//                if ($("#questao_14").val() != '' ) {    
//                    var file_data = $("#questao_14").prop('files')[0];
//                    form_data.append('questao_14', file_data);
//                }else {
//                    form_data.append('arqid_questao_14', 1);
//                }
//                form_data.append('aba3', 'true');
//                form_data.append('questao_12', (questao_12 === undefined)?'':questao_12);
//                form_data.append('questao_13', (questao_13 === undefined)?'':questao_13);
//                var campotabela = "<?=$campotabela?>"
//                var estadomunicipio = "<?=$estadomunicipio?>"
//                form_data.append('campotabela', campotabela);
//                form_data.append('estadomunicipio', estadomunicipio);
//
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
//                        if($("#arqid_questao_14").val() != 1) {
//                            $('#linkArqid_14').hide();
//                        }
//
//
//                        alert("Os dados foram salvos com sucesso!");
//                    }
//                });
//
//            }else {
//                alert("3.1 O fórum estadual/distrital/municipal de educação está instituído?");
//                return false;
//            };



        });

        $("#btnp033").click(function () {

            salvarQuestoesAba("#btnp033", true, false);
            $("#presentation_3").removeClass('active');
            $("#presentation_4").addClass('active');
//            var questao_12 = $("input[name$='questao_12']:checked").val();
//
//            if(questao_12 ){
//                if(questao_12 == 33 ){
//                    var questao_13 = $("input[name$='questao_13']:checked").val();
//                    if(!questao_13){
//                        alert("Preencher pergunta 3.2 Qual ato legal instituiu o fórum municipal de educação?");
//                        return false;
//                    }
//
//                    var questao_14 = $("#questao_14").val();
//                    if(questao_14 == ''){
//                        if($("#arqid_questao_14").val() != '1'){
//
//                            alert("Preencher pergunta 3.3 Anexar cópia do ato legal mencionado no item anterior.");
//                            return false;
//                        }
//
//
//                    }else{
//
//                        var validaextenao =  validoFormatoArquivo(questao_14);
//                        if(!validaextenao){
//                            alert("Anexar arquivo do tipo PDF!");
//                            return false;
//                        }
//
//                    }
//
//
//
//                };
//
//
//                var form_data = new FormData();
//
//
//                if($("#arqid_questao_14").val() != 1){
//                    var file_data = $("#questao_14").prop('files')[0];
//                    form_data.append('questao_14', file_data);
//                }else {
//                    form_data.append('arqid_questao_14', 1);
//                }
//                form_data.append('aba3', 'true');
//                form_data.append('questao_12', (questao_12 === undefined)?'':questao_12);
//                form_data.append('questao_13', (questao_13 === undefined)?'':questao_13);
//                var campotabela = "<?=$campotabela?>"
//                var estadomunicipio = "<?=$estadomunicipio?>"
//                form_data.append('campotabela', campotabela);
//                form_data.append('estadomunicipio', estadomunicipio);
//
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
//                        if($("#arqid_questao_14").val() != 1) {
//                            $('#linkArqid_14').hide();
//                        }
//                        alert("Os dados foram salvos com sucesso!");
//                        $("#presentation_3").removeClass('active');
//                        $("#presentation_4").addClass('active');
//                    }
//                });
//
//            }else {
//                alert("3.1 O fórum estadual/distrital/municipal de educação está instituído?");
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

