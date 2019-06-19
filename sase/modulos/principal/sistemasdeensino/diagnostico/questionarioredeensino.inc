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


$sql = "SELECT * FROM sase.questionariopergunta WHERE qpecod = '4' ORDER BY qpedescricao ASC";

$perguntas = $db->carregar($sql);



?>
<form class="form-horizontal" id="" method="post" style="margin-left: 20px" enctype="multipart/form-data">

    <?php

foreach ($perguntas as $pergunta):

    $sqlitemperguntas  = "SELECT * FROM sase.questionarioitempergunta WHERE qpeid = {$pergunta['qpeid']}";

    $temperguntas= $db->carregar($sqlitemperguntas);
    switch ($pergunta['qpetipo']) {
        case 'R';


            ?>
            <div class="form-group" id="pergunta_<?=$pergunta['qpeid']?>">
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
                        <label class="radio-inline" ">
                            <input type="radio" class="questao_<?=$pergunta['qpeid']?>"   name="questao_<?=$pergunta['qpeid']?>"
                            <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                                disabled
                            <?php endif;?>
                            <?=$valorItem == $item['qipid'] ? "checked" : ""?> value="<?=$item['qipid']?>">
                            <?=$item['qpidescricao']?>
                        </label>
                    <? endforeach;?>
                </div>
            </div>

            <? break;
        case 'C';
            ?>
            <div class="form-group">
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
                                <input type="checkbox" name="questao_<?=$pergunta['qpeid']?>"  <?=$valorItem == $item['qipid'] ? "checked" : ""?> value="<?=$item['qipid']?>"
                                    <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                                        disabled
                                    <?php endif;?>
                                >
                                <?=$item['qpidescricao']?>
                            </label>
                        </div>

                    <? endforeach;?>
                </div>
            </div>

            <? break;
        case 'F';
            ?>
            <div class="form-group">
                <label class="col-md-4 control-label" for="questao_<?=$pergunta['qpeid']?>"><?=$pergunta['qpedescricao']?></label>
                <div  class="col-md-8">

                    <input id="questao_<?=$pergunta['qpeid']?>" name="questao_<?=$pergunta['qpeid']?>" class="input-file" type="file"
                        <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                            disabled
                        <?php endif;?>
                    >
                </div>
            </div>




            <? break;
        case 'T';


                $sql = "SELECT   CAST (r.qreresposta AS CHAR(2000))             FROM sase.questionariopergunta qr
                                        INNER JOIN sase.questionarioresposta r on qr.qpeid = r.qpeid
                                    WHERE
                                    r.qrestatus = 'A' AND
                                        r.{$campotabela} = '{$estadomunicipio}' AND
                                        qr.qpeid = {$pergunta['qpeid']}
                                       ";

            $valorItem = $db->pegaum($sql);

            ?>
            <div class="form-group" id="pergunta_<?=$pergunta['qpeid']?>">
                <label class="col-md-4 control-label" for="questao_<?=$pergunta['qpeid']?>"><?=$pergunta['qpedescricao']?></label>
                <div  class="col-md-4">

                    <input id="questao_<?=$pergunta['qpeid']?>"  value="<?=$valorItem.''?>" name="questao_<?=$pergunta['qpeid']?>" type="text" maxlength="4" pattern="[0-9]+$" class="form-control input-md camponumber"
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
            <a href="#messages" aria-controls="messages" role="tab" data-toggle="tab" id="btnv04" type="button" name="singlebutton" class="btn btn-primary "><span class="glyphicon glyphicon-arrow-left"></span> Voltar</a>
            <?php if(in_array(PFLCOD_SASE_SECRETARIO, $perfis)):?>
                <button class="btn btn-primary" role="tab" data-toggle="tab" id="btn04"  type="button"><span class="glyphicon glyphicon-ok"></span> Salvar</button>
                <a href="#settings2" aria-controls="settings2" role="tab" data-toggle="tab" id="btnp044" type="button" name="singlebutton" class="btn btn-primary ">Próxima <span class="glyphicon glyphicon-arrow-right"></span></a>
            <?php endif;?>
            <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                <a href="#settings2" aria-controls="settings2" role="tab" data-toggle="tab" id="btnp04" type="button" name="singlebutton" class="btn btn-primary ">Próxima <span class="glyphicon glyphicon-arrow-right"></span></a>
            <?php endif;?>
        </div>
    </div>
</form>

<script>
    $(document).ready(function () {
        $('.camponumber').mask('9999');
        $("#pergunta_17").show();
        $("#pergunta_18").show();
        $("#pergunta_20").hide();
        $("input[name$='questao_16']").click(function () {
            if ($(this).val() == '39'){

                $("#questao_18").val('')
                $("#pergunta_17").show("slow");


            }else{
                $("#questao_17").val('');

                $("#pergunta_17").hide("slow");


            }


        });


        $("#btnp04").click(function () {
            $("#presentation_4").removeClass('active');
            $("#presentation_5").addClass('active');

        });

        $("#btnv04").click(function () {
            $("#presentation_4").removeClass('active');
            $("#presentation_3").addClass('active');

        });



        $("input[name$='questao_19']").click(function () {
            if ($(this).val() == '41'){
                $("#pergunta_20").show('slow')
            }else{
                $("#pergunta_20").hide("slow");
                $("input[name$='questao_20']").removeAttr('checked');

            }

        });

        var questao_16 = $("input[name$='questao_16']:checked").val();

        if(questao_16 == 39 ){

            $("#pergunta_17").show();


        }else if(questao_16 == 40 ){

            $("#pergunta_17").hide();

        }else {
            $("#pergunta_17").hide();

        }

        var questao_19 = $("input[name$='questao_19']:checked").val();
        if (questao_19 == '41'){
            $("#pergunta_20").show('slow')
        }else {
            $(".questao_20").prop("checked", false);
        }

        $("#btn04").click(function () {

            var questao_15 = $("input[name$='questao_15']").val();
            if(!questao_15){
                alert("Preencher pergunta 4.1 Qual o total de escolas da rede de ensino com cadastro no Instituto Nacional de Estudos e Pesquisas Educacionais Anísio Teixeira (INEP)?");
                return false;
            }
            var questao_16 = $("input[name$='questao_16']:checked").val();

            if(questao_16 ){
                if(questao_16 == 39 ){
                    var questao_17 = $("input[name$='questao_17']").val();
                    if(!questao_17){
                        alert("Preencher pergunta 4.2.1. Qual o quantitativos de escolas com conselhos escolares implantados?");
                        return false;
                    }
                }else{

                }

                var questao_18 = $("input[name$='questao_18']").val();
                if(!questao_18){
                    alert("Preencher pergunta 4.3	Quantas escolas possuem grêmios estudantis em funcionamento?");
                    return false;
                }

                var questao_19 = $("input[name$='questao_19']:checked").val();

                if(questao_19 ){
                    if(questao_19 == 41 ){
                        var questao_20 = $("input[name$='questao_20']:checked").val();
                        if(!questao_20){
                            alert("Preencher pergunta 4.4.1. Qual o mecanismo utilizado para a escolha do diretor escolar? ");
                            return false;
                        }
                    }
                }else{
                    alert("Preencher pergunta 4.4.0 Há normas específicas para escolha de diretor escolar?");
                    return false;
                }



                var form_data = new FormData();

                form_data.append('aba4', 'true');

                form_data.append('questao_15', (questao_15 === undefined)?'':questao_15);
                form_data.append('questao_16', (questao_16 === undefined)?'':questao_16);
                form_data.append('questao_17', (questao_17 === undefined)?'':questao_17);
                form_data.append('questao_18', (questao_18 === undefined)?'':questao_18);
                form_data.append('questao_19', (questao_19 === undefined)?'':questao_19);
                form_data.append('questao_20', (questao_20 === undefined)?'':questao_20);


                var campotabela = "<?=$campotabela?>";
                var estadomunicipio = "<?=$estadomunicipio?>";
                form_data.append('campotabela', campotabela);
                form_data.append('estadomunicipio', estadomunicipio);


                $.ajax({
                    url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
                    dataType: 'text',  // what to expect back from the PHP script, if anything
                    cache: false,
                    contentType: false,
                    processData: false,
                    data: form_data,
                    type: 'post',
                    success: function(response){
                        alert("Os dados foram salvos com sucesso! ");
                    }
                });


            }else {
                alert("4.2.0 A rede de ensino possui legislação para criação de conselhos escolares?");
                return false;
            };



        });

        $("#btnp044").click(function () {

            var questao_15 = $("input[name$='questao_15']").val();
            if(!questao_15){
                alert("Preencher pergunta 4.1 Qual o total de escolas da rede de ensino com cadastro no Instituto Nacional de Estudos e Pesquisas Educacionais Anísio Teixeira (INEP)?");
                return false;
            }
            var questao_16 = $("input[name$='questao_16']:checked").val();

            if(questao_16 ){
                if(questao_16 == 39 ){
                    var questao_17 = $("input[name$='questao_17']").val();
                    if(!questao_17){
                        alert("Preencher pergunta 4.2.1. Qual o quantitativos de escolas com conselhos escolares implantados?");
                        return false;
                    }
                }else{

                }

                var questao_18 = $("input[name$='questao_18']").val();
                if(!questao_18){
                    alert("Preencher pergunta 4.3	Quantas escolas possuem grêmios estudantis em funcionamento?");
                    return false;
                }

                var questao_19 = $("input[name$='questao_19']:checked").val();

                if(questao_19 ){
                    if(questao_19 == 41 ){
                        var questao_20 = $("input[name$='questao_20']").val();
                        if(!questao_20){
                            alert("Preencher pergunta 4.4.1. Qual o mecanismo utilizado para a escolha do diretor escolar? ");
                            return false;
                        }
                    }
                }else{
                    alert("Preencher pergunta 4.4.0 Há normas específicas para escolha de diretor escolar?");
                    return false;
                }



                var form_data = new FormData();

                form_data.append('aba4', 'true');

                form_data.append('questao_15', (questao_15 === undefined)?'':questao_15);
                form_data.append('questao_16', (questao_16 === undefined)?'':questao_16);
                form_data.append('questao_17', (questao_17 === undefined)?'':questao_17);
                form_data.append('questao_18', (questao_18 === undefined)?'':questao_18);
                form_data.append('questao_19', (questao_19 === undefined)?'':questao_19);
                form_data.append('questao_20', (questao_20 === undefined)?'':questao_20);


                var campotabela = "<?=$campotabela?>";
                var estadomunicipio = "<?=$estadomunicipio?>";
                form_data.append('campotabela', campotabela);
                form_data.append('estadomunicipio', estadomunicipio);


                $.ajax({
                    url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
                    dataType: 'text',  // what to expect back from the PHP script, if anything
                    cache: false,
                    contentType: false,
                    processData: false,
                    data: form_data,
                    type: 'post',
                    success: function(response){
                        alert("Os dados foram salvos com sucesso! ");
                        $("#presentation_4").removeClass('active');
                        $("#presentation_5").addClass('active');
                    }
                });


            }else {
                alert("4.2.0 A rede de ensino possui legislação para criação de conselhos escolares?");
                return false;
            };



        });



    });
</script>

