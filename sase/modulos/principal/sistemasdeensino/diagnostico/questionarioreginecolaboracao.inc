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
$perfis = pegaPerfilGeral($_SESSION['usucpf']);

$sql = "SELECT * FROM sase.questionariopergunta WHERE qpecod = '5' ORDER BY qpedescricao ASC";

$perguntas = $db->carregar($sql);
?>


<form class="form-horizontal" id="formSistemaensino" method="post" style="margin-left: 20px" enctype="multipart/form-data">


    <?php

$titulos = array(   1=> 'Sistema de Ensino',
    1=> 'Sistema de Ensino',
    1=> 'Sistema de Ensino',
    1=> 'Sistema de Ensino',
    1=> 'Sistema de Ensino');

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
                        <label class="radio-inline" for="questao21-0">
                            <input type="radio" name="questao_<?=$pergunta['qpeid']?>"
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
                        <div class="checkbox">
                            <label for="questao22-0">
                                <input type="checkbox"  class="questao_<?=$pergunta['qpeid']?>" name="questao_<?=$pergunta['qpeid']?>[]"
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
            ?>
            <div class="form-group" id="pergunta_<?=$pergunta['qpeid']?>">
                <label class="col-md-4 control-label" for="questao_<?=$pergunta['qpeid']?>"><?=$pergunta['qpedescricao']?></label>
                <div  class="col-md-5">

                    <input id="questao_<?=$pergunta['qpeid']?>" name="questao_<?=$pergunta['qpeid']?>" type="text" class="form-control input-md"
                        <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis)):?>
                            disabled
                        <?php endif;?>
                    >

                </div>
                <div  class="col-md-3">
            <?php if(in_array(PFLCOD_SASE_SECRETARIO, $perfis)):?>
                    <button id="addlist" type="button" class="btn btn-primary"><span class="glyphicon glyphicon-plus"></span></button>
            <?php endif;?>
                </div>
            </div>


            <div class="form-group" id="pergunta_<?=$pergunta['qpeid']?>">
                <label class="col-md-4 control-label" for="questao_<?=$pergunta['qpeid']?>"></label>
                <div  class="col-md-4">

                    <ul id="list">
                        <?php
                               $sql = "SELECT  r. qreresposta           FROM sase.questionariopergunta qr
                                        INNER JOIN sase.questionarioresposta r on qr.qpeid = r.qpeid
                                    WHERE
                                          r.qrestatus = 'A' AND
                                        r.{$campotabela} = '{$estadomunicipio}' AND
                                        r.qpeid = {$pergunta['qpeid']}
                                    ";

                        $valorItem = $db->carregar($sql);

                            $contador = 1;
                        if(!empty($valorItem)){
                          foreach ( $valorItem as  $item){
                              $qreresposta .= $item['qreresposta'].",";
                        ?>
                        <?php if(in_array(PFLCOD_SASE_SECRETARIO, $perfis)):?>
                        <button class="btn btn-danger  removeitemlist"  type="button" id="delete_111<?=$contador?>" style="margin-right: 10px;margin-bottom: 5px"><span class="glyphicon glyphicon-minus"></span></button>
                        <?php endif;?>
                        <span id="list_111<?=$contador?>"><?=$item['qreresposta']?></span><br id="sometbm_111<?=$contador?>">
                        <script>
                            $("#delete_111"+<?=$contador?>).click(function(){

                                var id = $(this).attr('id');
                                 var id = id.slice(7);

                                 $("#delete_111"+<?=$contador?>).hide();
                                 $('#list_111'+<?=$contador?>).hide();
                                 $('#sometbm_111'+<?=$contador?>).hide();

                               var str = $('#oculta_questao_22').val();
                                str =   str.replace('<?=$item['qreresposta'].","?>', "");
                                $('#oculta_questao_22').val(str);
                            });

                        </script>
                                                <?php $contador++ ;} }?>
                    </ul>

                    <input type="hidden" id="oculta_questao_<?=$pergunta['qpeid']?>" value="<?=$qreresposta?>" />
                </div>
            </div>

            <? break;


    }

    ?>







<? endforeach; ?>

    <div class="form-group">
        <label class="col-md-4 control-label" for="singlebutton"></label>
        <div class="col-md-4">
            <a href="#settings" aria-controls="settings" role="tab" data-toggle="tab" id="btnv05" type="button" name="singlebutton" class="btn btn-primary ">
                <span class="glyphicon glyphicon-arrow-left"></span> Voltar
            </a>
            <?php if(in_array(PFLCOD_SASE_SECRETARIO, $perfis)):?>
                <button class="btn btn-primary" role="tab" data-toggle="tab" id="btn05"  type="button">
                    <span class="glyphicon glyphicon-ok"></span> Salvar
                </button>
            <?php endif;?>
            <?php if(in_array(PFLCOD_SASE_PESQUISADOR_SASE, $perfis) && $_GET['quersituacao'] != 3 ):?>
                <button id="btnfinalizar" type="button" name="btnfinalizar" class="btn btn-primary ">Finalizar</button>
            <?php endif;?>
    </div>
</div>
</form>

<script>
    $(document).ready(function () {
        $("#pergunta_22").hide();
        $("#pergunta_24").hide();
        $("#pergunta_25").hide();
        $("input[name$='questao_21']").click(function () {


            if ($(this).val() == '48'){
                $("#pergunta_22").slideDown("slow");
                $("#list").add();
            }else{
                $("#pergunta_22").slideUp("slow");
                $("#list").hide();
                $("#oculta_questao_22").val('');


            }


        });
        $("#btnv05").click(function () {
            $("#presentation_5").removeClass('active');
            $("#presentation_4").addClass('active');

        });

        $("input[name$='questao_23']").click(function () {


            if ($(this).val() == '68'){
                $("#pergunta_24").slideDown("slow");
                $("#pergunta_25").slideDown("slow");
            }else{
                $("#pergunta_24").slideUp("slow");
                $("#pergunta_25").slideUp("slow");

                $(".questao_24").prop( "checked" , false);
                $(".questao_25").prop( "checked" , false);

            }


        });


         var arraylist = [];
         var i = 1;
        $('#addlist').click(function() {
            var new_task = $('#questao_22').val();
            new_task = new_task.trim();
            if(new_task == ''){
                alert("Campo Vazio!");
                return false;
            }
            new_task = new_task.replace(',','');
            if( $.inArray(new_task, arraylist) !== -1 ){
                alert("Instância já adicionada");
                $('#questao_22').val('');
                return false;
            };
            $('#list').append('<button class="btn btn-danger  removeitemlist"  type="button" id="delete_'+i+'" style="margin-right: 10px;margin-bottom: 5px"><span class="glyphicon glyphicon-minus"></span></button><span id="list_'+i+'">'+new_task+'</span><br id="sometbm_'+i+'">');
            arraylist.push(new_task)
            $('#questao_22').val('');

            $("#list").show();

            var paipe = ",";
            var antigo = $('#oculta_questao_22').val();
            var todoitem  = new_task.concat(paipe);
            var todalista  = antigo.concat(todoitem);
            $('#oculta_questao_22').val(todalista);


            console.log(arraylist);



        $("#delete_"+i).click(function(){

            var id = $(this).attr('id');
            var id = id.slice(7);

            $("#delete_"+id).remove();
            $('#list_'+id).remove();
            $('#sometbm_'+id).remove();

            var removeItem = $('#list_'+id).text();
            var filteredAry = arraylist.filter(function(e) { return e !== removeItem })
            var todalista =  filteredAry.toString();
            var paipe = ",";
            var todalista  = todalista.concat(paipe);
            $('#oculta_questao_22').val(todalista);


        });

            i++;
    });


        var questao_21 = $("input[name$='questao_21']:checked").val();
        if( questao_21  == 48 ){
            $("#pergunta_22").slideDown("slow");
        }

        var questao_23 = $("input[name$='questao_23']:checked").val();
        if( questao_23  == 68 ){
            $("#pergunta_24").slideDown("slow");
            $("#pergunta_25").slideDown("slow");
        }




        $("#btn05").click(function () {
            salvarQuestoesAba("#btn05", false, false);
//            var questao_21 = $("input[name$='questao_21']:checked").val();
//            if(!questao_21){
//                alert("Preencher pergunta 5.1.0 O estado/Distrito federal/município participa de alguma instância de negociação e pactuação das políticas educacionais junto ao estado e/ou outros municípios?");
//                return false;
//            }else{
//
//                if(questao_21 == 48 ){
//                    var questao_22 = $("#oculta_questao_22").val();
//                    if(questao_22 == ''){
//                        alert("Preencher pergunta 5.1.1 Qual(is) instância(s)?");
//                        return false;
//                    }
//                }
//            }
//            var questao_23 = $("input[name$='questao_23']:checked").val();
//
//
//            if(questao_23 ) {
//                if (questao_23 == 68) {
//                    var camposMarcados52 = new Array();
//                    $("input[type=checkbox][name='questao_24[]']:checked").each(function(){
//                        camposMarcados52.push($(this).val());
//                    });
//
//                    if(camposMarcados52 == ''){
//                        alert("Preencher pergunta 5.2.1 Assinale, a seguir, as formas de cooperação desenvolvidas:");
//                        return false;
//                    }
//
//                    var camposMarcados521 = new Array();
//                    $("input[type=checkbox][name='questao_25[]']:checked").each(function(){
//                        camposMarcados521.push($(this).val());
//                    });
//
//                    if(camposMarcados521 == ''){
//                        alert("Preencher pergunta 5.2.1 Assinale, a seguir, as formas de cooperação desenvolvidas:");
//                        return false;
//                    }
//                }
//            }else {
//                alert("Preencher pergunta 5.2.0 Existem formas de cooperação desenvolvidas entre o Estado e/ou outros municípios?");
//                return false;
//            }
//
//
//
//
//
//
//                var form_data = new FormData();
//
//                form_data.append('aba5', 'true');
//
//
//                form_data.append('questao_21', (questao_21 === undefined)?'':questao_21);
//                form_data.append('questao_22', (questao_22 === undefined)?'':questao_22);
//                form_data.append('questao_23', (questao_23 === undefined)?'':questao_23);
//                form_data.append('questao_24', (camposMarcados52 === undefined)?'':camposMarcados52);
//                form_data.append('questao_25', (camposMarcados521 === undefined)?'':camposMarcados521);
//
//
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
//
//                        alert("Os dados foram salvos com sucesso! ")
//
//
//                    }
//                });







        });


    });




</script>

