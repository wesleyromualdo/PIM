<?php

/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 10/13/2017
 * Time: 10:24 AM
 */

/**
 * Processamentos ajax da página
 */
$controllerPar3Obra = new Par3_Controller_Obra();
$controllerPar3Agrupador = new par3_Controller_Agrupador();
$formAgrupadorHelper = new Par3_Helper_FormHelper();
$formAgrupadorHelper->setNameForm('form-agrupador')
                    ->setArquivo('agrupadorForm')
                    ->setMethod('POST')
                    ->setIsModal(null);

$listaAgrupador = new Simec_Listagem();
$listaAgrupador->setCabecalho(array("Código","ID da Obra","Agrupador"));
$listaAgrupador->addAcao('edit','js_editarAgrupador');
$listaAgrupador->addAcao('delete',array('func'=>'js_deleteAgrupador','extra-params'=>array('agrdesc')));
$listaAgrupador->addCallbackDeCampo('preid','genericReplaceArrayAgg');
$listaAgrupador->setQuery($controllerPar3Agrupador->fetchAllList());
$listaAgrupador->turnOnOrdenacao();
$listaAgrupador->turnOnPesquisator();
$listaAgrupador->turnOnPrototipo();
$listaAgrupador->setTamanhoPagina(10);
//Processamentos de Requisição PSEUDO:CONTROLLER
switch ($_REQUEST['req']){
    case 'salvarAgrupador':
        ob_clean();
        $_REQUEST = utf8_decode_recursive($_REQUEST);
            if($controllerPar3Agrupador->isValid() == true){
                $idAgrupador = $controllerPar3Agrupador->salvar();
                echo $idAgrupador;
            }
        die();
        break;
}


//Processamentos Ajax da Página
if(isset($_REQUEST['method'])) {
    switch ($_REQUEST['method']) {
        case 'validaListaPreId':
            ob_clean();
            if ($_REQUEST['preid'] != '') {
                $isValid = $controllerPar3Obra->listaObraIsValid($_REQUEST['preid']);
                echo $isValid;
            }
            die();
            break;
        case "preencheForm":
            ob_clean();
            if (isset($_POST['agrid'])) {
                echo simec_json_encode($controllerPar3Agrupador->create($_POST['agrid']));
            }
            die();
            break;
        case "inativaAgrupador":
            ob_clean();
            $controllerPar3Agrupador->inativaAgrupador($_POST['agrid']);
            die();
            break;
        case "validaDescAgrupador":
            ob_clean();
            $exists = $controllerPar3Agrupador->validaDescricaoAgrupador($_REQUEST['agrdesc']);
            echo (int) $exists['agrdesc'];
            die();
            break;
    }
}
?>
<div class="ibox-title">
    <div class="row">
        <div class="col-md-4"></div>
        <div class="col-md-4"> <h3 class="center">Agrupador</h3></div>
        <div class="col-md-4"></div>
    </div>
</div>
<?php
$formAgrupadorHelper->renderForm();
?>

<div class="ibox-content" style="padding-top: 80px;">
    <div class="row">
        <div class="col-md-12 center">
            <?php $listaAgrupador->render();?>
        </div>
    </div>
</div>

<script>
    //configurações ao carregara tela
    $(document).ready(function (){
        $(".navbar-listagem").addClass("pull-left");
        $(".navbar-pesquisa").addClass("pull-left");
    });


    $(".btn-salvar-agrupador").on('click', function(){
        if(js_validaDados() == true){
            var acao = ($("#agrid").val() == '')?'insert':'update'; //novo ou alteração
            var msg;
            var titulo = "Agrupador";

            if(acao=='insert'){
                msg = " - " + $("input[type=text][name=agrdesc]").val() +': Salvo com Sucesso !'
            }else if(acao == 'update'){
                val = $("input[type=hidden][name=agrid]").val();
                msg = $("input[type=hidden][name=agrid]").val() + " - "+ $("input[type=text][name=agrdesc]").val() + " Alterado com Sucesso!";
            }

            $.ajax({
                type: "POST",
                url: window.location.href,
                data: '&req=salvarAgrupador&'+$("#form-agrupador_id").serialize(),
                success: function(response) {
                    nuMsg = (acao == 'insert')? response+msg:msg;
                    swal({
                            html: true,
                            title: titulo,
                            text: nuMsg,
                            type: "success",
                            confirmButtonText: "OK",
                            closeOnConfirm: false
                        }, function (isConfirm) {
                            window.location.href = window.location;
                        }

                    );
                }
            });
        }
    });

    $(".btn-cancelar-agrupador").on('click', function(){
        $("#form-agrupador_id").reset;
    });


    /**
     * valida campo preid quando perder o focu
     */
    $("#preid").blur(function() {
        var $this = $( this ); //armazeno o ponteiro em uma variavel
        valorAoPerderFocu = $this.val();
        if($( this ).val() != ''){ js_cleanHasErrors();}
        cleanValue = js_limpaStringCaractereInicialFinal(valorAoPerderFocu,',');
        cleanValueDuplicate = js_cleanArrayDuplicateValue(cleanValue.split(","));
        $this.val(cleanValueDuplicate);
        //valida os ID de obra que existem
        var valuePreId = $("#preid").val();
        //caso exista alguma obra para ser validado
        if(cleanValueDuplicate != ''){
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: '&method=validaListaPreId&preid='+cleanValueDuplicate,
                async: false,
                beforeSend: function(){
                    $("#span_preid").html("<i class='fa fa-refresh'></i><b> Carregando...</b>");
                },
                success: function (resp) {
                    if(resp != 'true'){
                        swal("Atenção", resp,"warning");
                        $(".btn-salvar-agrupador").attr('disabled','disabled');
                    }else{
                        $(".btn-salvar-agrupador").removeAttr('disabled');
                    }

                },
                complete: function(){
                    $("#span_preid").html(" ");
                },
            });
        }

    });

    /**
     * valida campo agrdesc não aceitar nada a não ser letras
     */
    $("#agrdesc").blur(function() {
        //this.value = this.value.replace(/[^A-Za-záàâãéèêíïóôõöúçñÁÀÂÃÉÈÍÏÓÔÕÖÚÇÑ ]/g,'');
        if(this.value != '' && this.value != null){
            var valor = this.value.replace(/[^A-Za-záàâãéèêíïóôõöúçñÁÀÂÃÉÈÍÏÓÔÕÖÚÇÑ ]/g,'');
            var jaExiste = js_validaDescricaoAgrupador(valor);
            if(jaExiste == false){
                return false;
            }
        }
    });

    /**
     * valida campo da obra para aceitar vírgula
     */
    $("#preid").keyup(function() {
        var $this = $( this ); //armazeno o ponteiro em uma variavel
        valorClean = js_limpaValoresInputAceitarSomenteNumerosVirgula($this.val());
        $this.val(valorClean);
    });

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param valor | string <String separada por determinado caracteres 'qualCaractere'>
     * @param qualCaractere | String <String que deve ser tirada do início e do fim>
     * @example stringVal: ,10 ,50 ,87,15      ,18,
     *          função: js_limpaStringCaractereInicialFinal(stringVal,',');
     *          retorno: 10,50,87,15,18
     *
     */
    function js_limpaStringCaractereInicialFinal(valor,qualCaractere){
        valor = valor.replace(/\s/g,''); //remove espaços em branco
        valor = valor.toLowerCase(); //passa todas as letras para
        arrayValor = valor.split(qualCaractere);
        cleanVal = arrayValor.filter(function(her){
            return her !== '';
        });
        return cleanVal.join(',');
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param stringSuja
     * @return {string}
     * @description Esta função limpa string permitindo somente números e vírgula
     */
    function js_limpaValoresInputAceitarSomenteNumerosVirgula(stringSuja){

        var valor = stringSuja.toLowerCase();
        var valor = valor.replace(/[a-zA-Z]+$|[/aáàâãéêèëíìïîãoóòöõúùüýç~´`!*+.'"!@#$%¨&*()_{}^´´.><:;/°ºª¹²³|?\-\=\[\]]|[\b]+,/,'');
        var valor = valor.replace(' ',',');
        var valor = valor.replace('  ','');
        var valor = valor.replace(',,',',');
        return valor;
    }

    function js_validaDados(){
        retorno = true;
        if($("#preid").val() == ''){
            js_hasError($("#preid"),'Campo "ID da Obra" Obrigatório!');
            retorno  = false;
        }

        if($("#agrdesc").val() == ''){
            js_hasError($("#agrdesc"),'Campo "Agrupador" Obrigatório!');
            retorno  = false;
        }

        return retorno;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param inputElement
     * @param msgError
     * @description Esta mostra error de validação e mostra uma mensagem sobre o mesmo error
     */
    function js_hasError(inputElement,msgError){
        inputElement.closest('.form-group').addClass('form-group has-error');
        spanElement = "#span_"+inputElement.attr('name');
        $(spanElement).show();
        $(spanElement).css('color','red');
        $(spanElement).html(msgError);
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description limpa a tela dos erros
     */
    function js_cleanHasErrors(){
        $(".has-error").removeClass('has-error');
        $(".span-error").html(' ');
    }


    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param idAgrupador
     * @description esta função carrega os dados para o formulário, pára edição
     */
    function js_editarAgrupador(idAgrupador){
        js_cleanHasErrors();
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: '&method=preencheForm&agrid='+idAgrupador,
            async: false,
            success: function (resp) {
                reponseJSON = jQuery.parseJSON(resp);
                preid = reponseJSON.preid;
                preid = preid.replace("{","");
                preid = preid.replace("}","");
                $("#preid").val(preid);
                $("#agrid").val(reponseJSON.agrid);
                $("#agrdesc").val(reponseJSON.agrdesc);
            }
        });
    }

    function js_deleteAgrupador(idAgrupador,descAgrupador){
        swal({
                title: " Remover Agrupador",
                type: "warning",
                text: "Tem certeza que deseja removero: "+idAgrupador+" - "+descAgrupador,
                showCancelButton: true,
                confirmButtonColor: "#DD6B55", confirmButtonText: "Sim",
                closeOnConfirm: "on",
                cancelButtonText: "Não",
                html: true
            }
            , function (isConfirm) {
                if (isConfirm) {
                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        data: '&method=inativaAgrupador&agrid='+idAgrupador,
                        async: false,
                        success: function (resp) {
                            window.location.href = window.location;
                        }
                    });
                }
            });
    }

    function js_cleanArrayDuplicateValue(arr){
        var unicValues = [];
        $.each(arr, function(i, el){
            if($.inArray(el, unicValues) === -1) unicValues.push(el);
        });

        return unicValues;
    }

    /**
     * @author Leo Kenzley <leo.olivera@castgroup.com.br>
     * @param valor
     * @return {boolean}
     * @description true = não existe agrupador com essa descrição já cadastrado,
     *              false = já existe agrupador com essa descrição cadastrado.
     */
    function js_validaDescricaoAgrupador(valor){
        var result = false;
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: '&method=validaDescAgrupador&agrdesc='+valor,
                async: false,
                beforeSend: function(){
                    $("#span_agrdesc").html("<i class='fa fa-refresh'></i><b> Carregando...</b>");
                },
                success: function (resp) {
                    if(resp == 0){ //não existe agrupador cadastracom com essa descrição
                        result = true;
                    }
                    if(resp == 1){
                        swal({
                                title: " Agrupador Já Cadastrado",
                                type: "warning",
                                text: "O agrupador : \""+valor+"\" já está cadastrado!",
                                showCancelButton: false,
                                closeOnConfirm: "on",
                                html: true
                            }
                            , function (closeOnConfirm) {
                                if (closeOnConfirm) {
                                    $("input[type=text][name=agrdesc]").val(null);
                                    $("input[type=text][name=agrdesc]").focus();
                                }
                            });

                        result = false;
                    }
                },
                complete: function(){
                    $("#span_agrdesc").html(" ");
                },
            });
        return result;
    }
</script>