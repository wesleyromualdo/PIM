<link rel="stylesheet" type="text/css" href="../../zimec/public/temas/simec/css/plugins/steps/jquery.steps.css" >
<script src="../../zimec/public/temas/simec/js/plugins/staps/jquery.steps.min.js" ></script>
<?php
switch($_REQUEST['questao']){
    case '1':
        ob_clean();
        require_once 'questao1.inc';die;
        break;
    case '2':
        ob_clean();
        require_once 'questao2.inc';die;
        break;
    case '3':
        ob_clean();
        require_once 'questao3.inc';die;
        break;
    case '4':
        ob_clean();
        require_once 'questao4.inc';die;
        break;
    case '5':
        ob_clean();
        require_once 'questao5.inc';die;
        break;
    case '6':
        ob_clean();
        $arrOtp =array();
//        if($_GET['qtdalunos']){
            $mInu = new Par3_Controller_InstrumentoUnidade();
            $qtdalunos = $_GET['qtdalunos'];
            $octid     = $_GET['octid'];
            $otpesfera = $mInu->buscaEsferaUnidade($_GET['inuid']);
            $mOed = new Par3_Model_ObraEstudoDemanda($_GET['oedid']);
            $tipoAmpliacao = '';
            $arrOtp = $mOed->listarTiposObra($qtdalunos,$otpesfera,$octid,$inpid,$mOed->oedquadra);
//        }
        require 'questao6.inc';die;
        break;
}

$arrOed = array();
$mOed = new Par3_Model_ObraEstudoDemanda();
$arrOed = $mOed->recuperarPorObra($obrid);

$arrTituloAbas[0] = $_GET['octid'] == 1? 'Motivo da Construção':'Motivo da Ampliação';
$arrTituloAbas[1] = 'Turnos de Atendimento';
$arrTituloAbas[2] = 'Séries Atendidas';
$arrTituloAbas[3] = 'Origem da Demanda';
$arrTituloAbas[4] = 'Demanda Reprimida';
$arrTituloAbas[5] = 'Tipo de Obra';

switch ($arrOed['omoid']){
    case 1:
        break;
    case 2:
        $arrTituloAbas = array_replace($arrTituloAbas,[3 => 'Justificar Expansão de Vagas',4 =>null]);
        break;
    case 3:
        $arrTituloAbas = array_replace($arrTituloAbas,[1 => 'Informar Tipo de Ampliação',2 => 'Origem da Demanda',4 =>null]);
        break;
    default:
        break;
}
?>
<style>
    .wizard .content {
        min-height: 100px;
        z-index: ;
    }
    .wizard .content > .body {
        width: 100%;
        height: auto;
        padding: 15px;
        position: relative;
    }
</style>
<?php if($mdObra->otpid):?>
    <style>
        .wizard > .steps a, .wizard > .steps a:hover, .wizard > .steps a:active {
            background: #6fd1bd;
            color: #fff;
        }
        .wizard > .steps a, .wizard > .steps a:hover, .wizard > .steps a:active {
            display: block;
            width: auto;
            margin: 0 0.5em 0.5em;
            padding: 8px;
            text-decoration: none;
            -webkit-border-radius: 5px;
            -moz-border-radius: 5px;
            border-radius: 5px;
        }
    </style>
<?php endif; ?>
<!--questionario-->
<form id="form" class="wizard wizard-big" >
    <input type="hidden" name="oedid" id="oedid" value="<?php echo $arrOed['oedid']; ?>"/>
    <input type="hidden" name="inuid" value="<?php echo $inuid; ?>"/>
    <input type="hidden" name="inpid" value="<?php echo $inpid; ?>"/>
    <input type="hidden" name="obrid" value="<?php echo $obrid; ?>"/>
    <input type="hidden" name="etaid" value="<?php echo $_GET['etaid']; ?>">
    <input type="hidden" name="omoid" value="<?php echo $arrOed['omoid'];; ?>">
    <h1 id="formtitulo1"><?php echo $arrTituloAbas[0];?></h1>
    <section id="formquestao1">
        <?php require_once 'questao1.inc'; ?>
    </section>

    <h1 id="formtitulo2"><?php echo $arrTituloAbas[1];?></h1>
    <section id="formquestao2">
        <?php require_once 'questao2.inc'; ?>
    </section>

    <h1 id="formtitulo3"><?php echo $arrTituloAbas[2];?></h1>
    <section id="formquestao3">
        <?php require_once 'questao3.inc'; ?>
    </section>

    <?php if($arrOed['omoid'] != 3): ?>
    <h1 id="formtitulo4"><?php echo $arrTituloAbas[3];?></h1>
    <section id="formquestao4">
        <?php require_once 'questao4.inc'; ?>
    </section>
    <?php endif; ?>

    <?php //CASO O MOTIVO DA OBRA SEJA DE CONSTRUÇÃO NO QUESTIONARIO CADASTRADO
        if($arrOed['omoid'] == 1):
    ?>
    <h1 id="formtitulo5"><?php echo $arrTituloAbas[4];?></h1>
    <section id="formquestao5">
        <?php require_once 'questao5.inc'; ?>
    </section>
    <?php endif;?>

    <h1 id="formtitulo6"><?php echo $arrTituloAbas[5];?></h1>
    <section id="formquestao6">
        <div id="div-tipo-obra"></div>
    </section>
</form>
<!--fim do questionario-->


<script>
    var arrEscolas = $('#escid').val();
    //**********************************************************************
    //Quando muda turno reseta as séries atendidas
    $(document).on('change','#oedturnos_atendimento-D,#oedturnos_atendimento-I',function(){
        $('#qtd-series input[id^="oed"]').val(0);
        $('#qtd-estimado-alunos').html(0);
        $('[name=qtd-estimado-alunos]').val(0);
    });
    //**********************************************************************

    $(document).on('change','.dadostipoobra',function(){
        var finish = $(document).find('a[href$="finish"]').parent('li');
        if($(this).val() != ''){
            finish.show();
        }else{
            finish.hide();
        }
    });

    $('.wizard').ready(function(){
        $(document).find('#loading').hide();
        $(document).find('[role=tab]').addClass('done');
    });
    //Eventos de criação do Wizard
    $(document).ready(function () {
        $(document).find('#loading').remove();
        $(document).find('[role=tab]').addClass('done');
        $(document).on('change','#omoid',function(){
            if($(this).val() == 1){
                var qtdAlunos = $(document).find('#qtd-estimado-alunos').html();
                $('#form').steps('remove',1);
                $("#form").steps("insert",1, {title: "Turnos de Atendimento", contentMode: "async", contentUrl: window.location.href + '&questao=2'});
                $('#form').steps('remove',2);
                $("#form").steps("insert",2, {title: "Séries Atendidas", contentMode: "async", contentUrl: window.location.href + '&questao=3'});
                $('#form').steps('remove',3);
                $("#form").steps("insert",3, {title: "Origem da Demanda", contentMode: "async", contentUrl: window.location.href + '&questao=4'});
                $('#form').steps('remove',4);
                $("#form").steps("insert",4, {title: "Demanda Reprimida", contentMode: "async", contentUrl: window.location.href + '&questao=5'});
                $('#form').steps('remove',5);
                $("#form").steps("insert",5, {title: "Tipo de Obra", contentMode: "async", contentUrl: window.location.href + '&questao=6'});
            }

            if($(this).val() == 2){
                $('#form').steps('remove',1);
                $("#form").steps("insert",1, {title: "Turnos de Atendimento", contentMode: "async", contentUrl: window.location.href + '&questao=2'});
                $('#form').steps('remove',2);
                $("#form").steps("insert",2, {title: "Séries Atendidas", contentMode: "async", contentUrl: window.location.href + '&questao=3'});
                $('#form').steps('remove',3);
                $("#form").steps("insert",3, {title: "Justificar Expansão de Vagas", contentMode: "async", contentUrl: window.location.href + '&questao=4'});
                $('#form').steps('remove',4);
                $("#form").steps("insert",4, {title: "Tipo de Obra", contentMode: "async", contentUrl: window.location.href + '&questao=6'});
                $('#form').steps('remove',5);
            }

            if($(this).val() == 3){
                $('#form').steps('remove',1);
                $("#form").steps("insert",1, {title: "Informar Tipo de Ampliação", contentMode: "async", contentUrl: window.location.href + '&questao=2'});
                $('#form').steps('remove',2);
                $("#form").steps("insert",2, {title: "Origem da Demanda", contentMode: "async", contentUrl: window.location.href + '&questao=3'});
                $('#form').steps('remove',3);
                $("#form").steps("insert",3, {title: "Tipo de Obra", contentMode: "async", contentUrl: window.location.href + '&questao=6'});
                $('#form').steps('remove',5);
                $('#form').steps('remove',4);
            }
            $(document).find('a[href$="finish"]').parent('li').hide();
        });
        $("#form").steps({
            headerTag: "h1",
            bodyTag: "section",
            enableCancelButton:0,
            transitionEffect: "slideLeft",
            <?php echo $mdObra->otpid ?'enableAllSteps:true,':'';?>
            labels: {
                finish: "<i class=\"fa fa-save\"></i> Enviar informações",
                next: "Próximo",
                previous: "Anterior"
            },
            showFinishButtonAlways: false,
            autoFocus:'true',
            onInit: function(event, currentIndex, newIndex){
                $(document).find('#loading').hide();
                $(document).find('a[href$="finish"]').parent('li').hide();
            },
            onStepChanging: function (event, currentIndex, newIndex) {
                // Always allow going backward even if the current step contains invalid fields!
                if (currentIndex > newIndex) {
                    return true;
                }
                var form = $(this);
                if (currentIndex < newIndex) {
                    // To remove error styles
                    $(".body:eq(" + newIndex + ") label.error", form).remove();
                    $(".body:eq(" + newIndex + ") .error", form).removeClass("error");
                }
                // Disable validation on fields that are disabled or hidden.
                form.validate().settings.ignore = ":disabled,:hidden";
                // Start validation; Prevent going forward if false
                if(form.valid()){
                    salvarQuestao(currentIndex+1);
                    return form.valid();
                }
            },
            onStepChanged: function (event, currentIndex, priorIndex){
                $(document).find('#loading').hide();
                checarPendencias();
                var omoid = $(document).find('#omoid').val();
                var index = currentIndex + 1;

                if(omoid == 2){

                }
                var ordem = $(document).find('#ordem-'+index).val();
                carregarAnalise(ordem);
//                $('#form-t-2').parent('[role=tab]').removeClass('done').addClass('error');

                //Libera para enviar somente após chegar no último passo
                $(document).find('a[href$="finish"]').parent('li').hide();
                //Salva os dados do formulário
                //Atualiza os tipos de Obra
//                $(".wizard-big").find('.content').css('height','500px');
                if((omoid == 1 && currentIndex == 5) || (omoid == 2 && currentIndex == 4) || (omoid == 3 && currentIndex == 3)){
                    var qtdAlunos = $(document).find('#qtd-estimado-alunos').html();
                    var oedid = $(document).find('[name=oedid]').val();
                    var caminho   = window.location.href;
                    var action    = '&questao=6&qtdalunos='+qtdAlunos+'&oedid='+oedid;
                    $(document).find('#div-tipo-obra').load(caminho+action);
                }
            },
            onFinishing: function (event, currentIndex) {
                var form = $(this);
                // Disable validation on fields that are disabled.
                // At this point it's recommended to do an overall check (mean ignoring only disabled fields)
                form.validate().settings.ignore = ":disabled";
                form.validate().settings.ignore = ":hidden";

                // Start validation; Prevent form submission if false
                return form.valid();
            },
            onFinished: function (event, currentIndex) {
                var form = $(this);
                form.attr('action',window.location.href + "&requisicao=salvaTipo");
                form.attr('method','POST');
                form.submit();
            }
        }).validate({
            errorPlacement: function (error, element) {
                element.before(error);
            }
        });

        checarPendencias();

    });
    function salvarQuestao(passo) {
        var oedid   = $(document).find('[name=oedid]').val();
        var obrid   = $(document).find('[name=obrid]').val();
        var etaid   = $(document).find('[name=etaid]').val();
        var caminho = window.location.href;
        var action  = 'requisicao=salvarestudodemanda&'+$("#form").serialize()+'&passo='+passo;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,

            success: function (resp) {
                $('#loading').hide();
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    $('#form').find('[name=oedid]').val(res);
                    return true;
                }
                return false;
            }
        });
    }
    function carregarAnalise(ordem)
    {
        $('#loading').hide();
        var caminho = window.location.href;
        var action = 'requisicao=carregarAnalise&ordem='+ordem;
        $(document).find('#loading').hide();
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                var res = JSON.parse(resp);
                if(resp != false){
                    $('#carregar-analise-'+ordem).html(res.template);
                    if(res.pendencia == 1){
                        var index = ordem - 1;
                        $('#form-p-'+index).find('input,textarea,select').removeAttr('disabled');
                    }
                }

            }
        });
    }

    function checarPendencias()
    {
        var caminho = window.location.href;
        var action  = 'requisicao=checarPendenciasAnalise';
        var omoid   = $(document).find('#omoid').val();
        var ordemReordenamento = [1,2,3,5,7,8];
        var ordemExpansao      = [1,2,3,6,8];
        var ordemNovosEspacos  = [1,9,5,8];
        var ordem = [];
        if(omoid == 1){
            ordem = ordemReordenamento;
        }
        if(omoid == 2){
            ordem = ordemExpansao;
        }
        if(omoid == 3){
            ordem = ordemNovosEspacos;
        }
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#loading').hide();
                if(!resp){
                    return false;
                }
                var res = JSON.parse(resp);
                var cont = 0;
                var pendencias = [];

               pendencias = res.map(function(key){
                    if(key.aer_pendencia === "S"){
                        return key.aepordem;
                    }
                });
                for(var cont = 0;cont < ordem.length;cont++) {
                    if(in_array(ordem[cont],pendencias)) {
                        $('#form-t-'+cont).parent('[role=tab]').removeClass('done').addClass('error');
                    }else {
                        if($('#form-t-'+cont).parent('[role=tab]').hasClass('current')) {
                            $('#form-t-'+cont).parent('[role=tab]').removeClass('error');
                        }
                    }
                }
                console.log(res);
                let pendencias2 = res.filter(function(key){
                    return (key.aer_pendencia === "S" && (key.aer_corrigido === '' || key.aer_corrigido === "N"));
                });

                $('div#estudo_demanda').children('span').remove();

                if(pendencias2.length > 0) {
                    $('div#estudo_demanda').html("<span class=\"fa fa-times-circle danger\"></span>&nbsp Estudo Demanda");
                }else{
                    $('div#estudo_demanda').html('<span class="fa fa-check iconUnidade" style="color:green !important;background-color: transparent;"></span>&nbsp Estudo Demanda');
                }
            }
        });
    }
</script>