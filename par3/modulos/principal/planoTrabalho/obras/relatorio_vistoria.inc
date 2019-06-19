<?php
require_once APPRAIZ . "includes/classes/questionario/TelaBootstrap.class.inc";
require_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/MontatabelaBootstrap.class.inc";
require_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/cabecalho.inc";

$obrid = (int)$_REQUEST['obrid'];
$inuid = (int)$_REQUEST['inuid'];
$inuid = (int)$_REQUEST['inuid'];
$obqeid = (int)$_REQUEST['obqeid'];
if (empty($inuid) || empty($obrid)) {
    echo 'Não possui inuid ou obrid! ';
    exit;
}
$controllerObra = new Par3_Controller_Obra($inuid, $obrid);
$controllerObraXQuestionario = new Par3_Controller_ObraXQuestionario($obrid, 3);
$modelObra = new Par3_Model_Obra($obrid);
$modelBandaLarga = new Par3_Model_Relatorio_BandaLarga();
$descricaoTipoObra = $controllerObraXQuestionario->getDescricaoCategoriaTipoObra();

if($_REQUEST['salvar_questionario']){
    $modelAnaliseEngenhariaObra = new Par3_Model_AnaliseEngenhariaObra();
    $modelAnaliseEngenhariaObra->retirarPendencia($_REQUEST['obrid'], Par3_Model_ObraAba::RELATORIO_VISTORIA, 1);
}

switch ($_REQUEST['requisicao']) {
    case 'salvarVistoriaTipoReforma':
        ob_clean();
        echo $controllerObraXQuestionario->salvar($_POST);
        die();
        break;
    case 'verificaPreenchimentoQuestionario':
        ob_clean();
        echo $controllerObraXQuestionario->carregarBotaoWorkflow($obrid);
        die();
        break;
    case 'finalizarPreenchimentoQuestionario':
        ob_clean();
        $controllerObraXQuestionario->finalizarPreenchimentoQuestionario($obrid, $inuid);
        die();
        break;
    case 'reabrirPreenchimentoQuestionario':
        ob_clean();
        $controllerObraXQuestionario->reabrirPreenchimentoQuestionario($obrid, $inuid);
        die();
        break;
    case 'buscarObraPorId':
        ob_end_clean();
        $octid = $controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid;
        $controllerObra->getDescricaoResumidaObra((int)$_POST['obrid_reforma'], (int)$inuid, $octid);
        die();
        break;
    case 'validaObraReformada':
        ob_end_clean();
        $result = $controllerObraXQuestionario->modelObraXQuestionario->verificaObra($_POST['obrid_reforma'], $_REQUEST['inuid']);
        if($result ==  true || $result == 1){
            echo 'true';
        }else{
            echo 'false';
        }
        die();
        break;
    case 'escolapanel':
        $escid = $_GET['escid'];
        if(!$escid){
            ob_clean();
            echo '';die;
        }
        $esc = new Par3_Model_Escola($escid);
        $inp = new Par3_Model_IniciativaPlanejamento($_GET['inpid']);
        if($inp->etaid == 1){$labelEtapa = 'Ensino Infantil';}
        if($inp->etaid == 2){$labelEtapa = 'Ensino Fundamental';}
        if($inp->etaid == 3){$labelEtapa = 'Ensino Médio';}
        ob_clean();
        require 'template_escola_panel.php';
        die;
        break;
    default:
        $disabled = 'disabled';

        if($controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::REFORMA){
            if($controllerObraXQuestionario->modelObraXQuestionario->beneficiada_fnde == 'N'){
                $qrpid = $controllerObraXQuestionario->getQuestionario($obrid);
            }
        }

        if($controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::AMPLIACAO){
            if($controllerObraXQuestionario->modelObraXQuestionario->beneficiada_fnde == 'S'){
                if($controllerObraXQuestionario->modelObraXQuestionario->apliacao_mesmo_terreno == 'N'){
                    $qrpid = $controllerObraXQuestionario->getQuestionario($obrid);
                }
            }else{
                $qrpid = $controllerObraXQuestionario->getQuestionario($obrid);
            }
        }

        if($controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::CONSTRUCAO){
            $qrpid = $controllerObraXQuestionario->getQuestionario($obrid);
        }
//
//
//        if(
//            !empty($controllerObraXQuestionario->modelObraXQuestionario->queid) &&
//            !(
//                $controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::AMPLIACAO &&
//                $controllerObraXQuestionario->modelObraXQuestionario->beneficiada_fnde == 'S' &&
//                $controllerObraXQuestionario->modelObraXQuestionario->apliacao_mesmo_terreno == 'S'
//            )
//        ) {
//            $arrVigencia = $controllerObraXQuestionario->recuperarVigenciaQuestionario();
//
//            $qrpid = $controllerObraXQuestionario->getQuestionario($obrid);
//        }
        break;
}

?>
<style>
    .cl_obrigatorio{
        border: 1px solid red;
    }
</style>
<div class="ibox" id="painelProgramas">

    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center"> Relatório de Vistoria </h2>
                <h3 class="center"><?= (empty($_GET['obrid']) ? 'Questionário' : "Pré-Obra ID: {$controllerObra->model->obrid}") ?></h3>
                <h3 class="center">
                    <?= (empty($_GET['obrid']) ? '' : "<b>Tipo de Obra:</b> {$controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->otpdsc} / $descricaoTipoObra") ?>
                </h3>

            </div>
        </div>
    </div>

    <div class="ibox-content">
        <!--  FORM DE VISTORIA      -->
        <form method="post" name="form-vistoria" id="form-vistoria" class="form form-horizontal">
            <input type="hidden" name="requisicao" value="salvarVistoriaTipoReforma"/>
            <input type="hidden" name="obrid" id="obrid" value="<?php echo $obrid ?>"/>
            <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid ?>"/>
            <input type="hidden" name="obqeid" id="obqeid" value="<?php echo $controllerObraXQuestionario->modelObraXQuestionario->obqeid ?>"/>
            <?php
            $mpergunta = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
            $mpergunta->carregarPergunta(Par3_Model_ObraAba::RELATORIO_VISTORIA,1,$_GET['obrid']);
            $disableDadosBasicos = $mpergunta ->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::RELATORIO_VISTORIA,1,$_GET['obrid']);
            ?>
            <div class="row">
                <div class="col-md-12">
                    <?php
                    $controllerObraXQuestionario->getFormularioVistoria($inuid, $obrid);
                    ?>
                </div>
            </div>
        </form>
        </br>
        <?php
        if (!empty($qrpid)) {
            $msgDesabilitadoPNE = 'O questionário de banda larga só estará disponíveis para edição no periodo de {dt_inicio} a {dt_fim}';
            //RELATÓRIO DE VISTORIA
            $telaPNE = new TelaBootstrap(array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 400, 'relatorio' => 'modulo=relatorio/relatorioQuestoesEstrategicas&acao=A', 'habilitado' =>  ($disableDadosBasicos == ''? 'S':'N'), 'msgDesabilitadoPNE' => $msgDesabilitadoPNE));
        }
        ?>
    </div>
</div>


<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog  modal-lg" id="modal_html">
        <div class="ibox-content-round-gray ibox-content" id="conteudo_modal">
        </div>
    </div>
</div>
<script language="JavaScript">

    $(function () {
        //PESQUI
        $('#obrid_reforma').on('blur', function (e) {
            $.post(window.location.href, {
                requisicao: "buscarObraPorId",
                obrid_reforma: $(this).val()
            }, function (data) {
                $('#descricao_obra').html(data);
            });
            js_validaIdObraReforma($(this).val());
        });

        <?php if($controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::REFORMA):?>
        if ($('#obrid_reforma').val()) {
            $('#obrid_reforma').closest('.form-group').show();
            $.post(window.location.href, {
                requisicao: "buscarObraPorId",
                obrid_reforma: $('#obrid_reforma').val()
            }, function (data) {
                $('#descricao_obra').html(data);
            });
        }

        $('input[name="beneficiada_fnde"]').closest('.btn-primary').on('click', function () {
            $(".span_error").hide();//remove os errors
            $("#form-vistoria").find('.has-error').removeClass('has-error');//remove as classes que formaram os errors
            $(".chosen-select").val('').trigger("chosen:updated");//limpa os campos do tipo chosen-select

            if ($(this).find('input').val() == 'S') {

                $('#obrid_reforma').closest('.form-group').removeClass('hidden').show();
                $('#obrid_reforma').closest('.form-group').show().find('label').find('span').remove();
                $('#obrid_reforma').closest('.form-group').show().find('label').append('<span class="campo-obrigatorio" title="Campo obrigatório">*</span>');
                //$('#obrid_reforma').prop('required', true);

                $("#cod_escola").closest('.form-group').find('label').find('.campo-obrigatorio').remove();
                //$("#cod_escola").prop('required', false);
            } else {

                // $("#cod_escola").prop('required', true);
                $("#cod_escola").closest('.form-group').find('label').append('<span class="campo-obrigatorio" title="Campo obrigatório">*</span>');

                $('#obrid_reforma').val(' ');
                $('#obrid_reforma').closest('.form-group').hide().find('label').find('.campo-obrigatorio').remove();
                // $('#obrid_reforma').prop('required', false);
                $('#escid').removeAttr('required');

                //$('#cod_escola').closest('.form-group').hide();
                //$('#cod_escola').removeAttr('required');
            }
        });

        <?php if($controllerObraXQuestionario->modelObraXQuestionario->beneficiada_fnde == 'S'):?>
        $('#obrid_reforma').show();
        $('#obrid_reforma').closest('.form-group').removeClass('hidden').find('label').append('<span class="campo-obrigatorio" title="Campo obrigatório">*</span>');
        $('#escid').closest('.form-group').removeClass('hidden').find('span').empty();
        <?php endif ?>

        <?php if($controllerObraXQuestionario->modelObraXQuestionario->beneficiada_fnde == 'N'):?>
        $('#obrid_reforma').closest('.form-group').addClass('hidden');//campo id da obra fica oculto
        $('#cod_escola').closest('.form-group').removeClass('hidden').find('label').find('span').empty();
        $('#cod_escola').closest('.form-group').find('label').append('<span class="campo-obrigatorio" title="Campo obrigatório">*</span>');

        <?php endif ?>

        <?php endif ?>

        <?php if($controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::AMPLIACAO):?>



        //formata o
        $('input[name="beneficiada_fnde"]').closest('.btn-primary').on('click', function () {

            $(".span_error").hide();//remove os errors
            $("#form-vistoria").find('.has-error').removeClass('has-error');//remove as classes que formaram os errors
            $(".chosen-select").val('').trigger("chosen:updated");//limpa os campos do tipo chosen-select

            if ($(this).find('input').val() == 'S') {
                $('#apliacao_mesmo_terreno_S').closest('.form-group').removeClass('hidden');
                $('#apliacao_mesmo_terreno_S').closest('.form-group').removeClass('hidden').find('span').empty();
                $('#apliacao_mesmo_terreno_S').closest('.form-group').find('.control-label').append('<span class="campo-obrigatorio" title="Campo obrigatório">*</span>');


                $('#descricao_obra').show();
                $('#div_obrid_reforma').show();

                $('#obrid_reforma').closest('.form-group').removeClass('hidden').find('span').empty();
                $('#obrid_reforma').closest('.form-group').removeClass('hidden').find('label').append('<span class="campo-obrigatorio" title="Campo obrigatório">*</span>');
                // $('#form-vistoria').find('#obrid_reforma').prop('required',true);

                $('#escid').closest('.form-group').removeClass('hidden').find('span').empty(); //remove asterisco
                $('#escid').removeAttr('required');//remove o atributo required
                //  $('#form-vistoria').find('#escid').prop('required',false);//remove o atributo required
                //verifica se Escola beneficiada é uma escola padrão FNDE?: == SIM, caso seja já carrega o ID Obra e Selecionar escola*:

            }
            else {
                $('#apliacao_mesmo_terreno_S').closest('.form-group').addClass('hidden');
                $('#apliacao_mesmo_terreno').removeAttr('required');

                $('#descricao_obra').hide();

                $('#obrid_reforma').val(' ');
                $('#obrid_reforma').closest('.form-group').addClass('hidden').find('label').find('.campo-obrigatorio').remove();
                $('#obrid_reforma').removeAttr('required');

                $('#escid').removeAttr('required');
                $('#escid').closest('.form-group').removeClass('hidden').find('label').find('span').empty();
                $('#escid').closest('.form-group').removeClass('hidden').find('label').find('span').append('<span class="campo-obrigatorio" title="Campo obrigatório">*</span>');
            }
        });

        //Ao carregar a página, caso "A escola beneficiada é uma escola padrão FNDE?:  "
        //SIM
        <?php if($controllerObraXQuestionario->modelObraXQuestionario->beneficiada_fnde == 'S'):?>
        $('#apliacao_mesmo_terreno_S').closest('.form-group').removeClass('hidden');
        $('#apliacao_mesmo_terreno_S').closest('.form-group').find('span').empty();
        $('#apliacao_mesmo_terreno_S').closest('.form-group').find('.control-label').append('<span class="campo-obrigatorio" title="Campo obrigatório">*</span>');


        $('#descricao_obra').show();

        $('#obrid_reforma').closest('.form-group').removeClass('hidden').find('span').empty();
        $('#obrid_reforma').closest('.form-group').removeClass('hidden').find('label').append('<span class="campo-obrigatorio" title="Campo obrigatório">*</span>');
        // $('#obrid_reforma').closest('.form-group').find('input').prop('required',true);

        $('#escid').closest('.form-group').removeClass('hidden').find('span').empty(); //remove asterisco
        $('#escid').removeAttr('required');//remove o atributo required

        carregaTabelaDadosObra();
        $("#descricao_obra").show();

        <?php endif ?>

        //NÃO
        <?php if($controllerObraXQuestionario->modelObraXQuestionario->beneficiada_fnde == 'N'):?>
        $('#apliacao_mesmo_terreno_S').closest('.form-group').addClass('hidden');

        $('#descricao_obra').hide();

        $('#obrid_reforma').val(' ');
        $('#obrid_reforma').closest('.form-group').addClass('hidden').find('label').find('.campo-obrigatorio').remove();
        $('#obrid_reforma').removeAttr('required');

        $('#escid').closest('.form-group').removeClass('hidden').find('label').find('span').empty();
        $('#escid').closest('.form-group').removeClass('hidden').find('label').find('span').append('<span class="campo-obrigatorio" title="Campo obrigatório">*</span>');

        <?php endif ?>
        <?php endif ?>
        $('.salvarQuestionarioVistoria').on('click',function (evt) {
            isValid = js_validateFormRelatorioVistoria();
            if(isValid ==  true){
                evt.preventDefault();
                var form = $('#form-vistoria').serialize();
                var caminho = window.location.href + '<?php echo $helperUrlToReturn->renderComplementoUrl();?>';
                var action = form;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {

                    	if(  isNaN(parseFloat(resp)))
						{	
                    		 swal("Erro.", "Ocorreu um erro ao salvar o Relatório de Vistoria", "error");
							 return false;
						}
                    	else
                    	{
	                        cleanValue = '&obqeid='+resp;
	                        url = caminho.replace(cleanValue,'');
	                        url = url.replace(cleanValue,'');
	                        msgSuccess(url+'&obqeid='+resp,'Salvo com sucesso!');
                    	}
                    }
                });
            }
        });
        $.ajax({
            url: window.location.href + '&requisicao=verificaPreenchimentoQuestionario',
            global: false, async: false, method: 'POST', success: function (res) {
                $('#acao').html(res);
            }
        });
        $(document).on('click', '.finalizar', function () {
            window.location.href = window.location.href + '&requisicao=finalizarPreenchimentoQuestionario';
        });
        $(document).on('click', '.reabrir', function () {
            window.location.href = window.location.href + '&requisicao=reabrirPreenchimentoQuestionario';
        });
        $(document).on('click', '.vencido', function () {
            sweetAlert("Prazo Encerrado", "O questionário só pode ser reaberto dentro do prazo de vigência. (De <?php echo $arrVigencia['dt_inicio'] ?> até <?php echo $arrVigencia['dt_fim'] ?>)", "error");
        });
        $(document).ajaxStop(function () {
            $.ajax({
                url: window.location.href + '&requisicao=verificaPreenchimentoQuestionario',
                global: false,
                async: false,
                method: 'POST',
                success: function (res) {
                    $('#acao').html(res);
                }
            });
            attachInit();
            attachEvents();
        });
    });

    $(".printMenu").attr("href", "javascript:imprimeQ(136)");
</script>

<!--VALIDAÇÕES-->
<script type="text/javascript">

    <?php if($controllerObraXQuestionario->modelObraXQuestionario->escid &&
    $controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::AMPLIACAO):?>
    carregaPanelEscola(<?= $controllerObraXQuestionario->modelObraXQuestionario->escid;?>);
    <?php endif;?>

    <?php if($controllerObraXQuestionario->modelObraXQuestionario->cod_escola &&
    $controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::REFORMA):?>
    carregaPanelEscola(<?= $controllerObraXQuestionario->modelObraXQuestionario->cod_escola;?>);
    <?php endif;?>
    /**
     * valida campo da obra para não aceitar nada além de números ao digitar
     */
    $("#obrid_reforma").keyup(function() {
        var $this = $( this ); //armazeno o ponteiro em uma variavel
        valorClean = js_limpaValoresInputAceitarSomenteNumeros($this.val());
        $this.val(valorClean);
    });

    /**
     * valida campo da obra para não aceitar nada além de números ao perder o foco
     */
    $("#obrid_reforma").blur(function() {
        var $this = $( this ); //armazeno o ponteiro em uma variavel
        valorAoPerderFocu = $this.val();
        cleanValue = js_limpaStringCaractereInicialFinal(valorAoPerderFocu,',');
        $this.val(cleanValue);
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
    function js_limpaValoresInputAceitarSomenteNumeros(stringSuja){
        var valor = stringSuja.toLowerCase();
        var valor = valor.replace(/[a-zA-Z]+$|[/aáàâãéêèëíìïîãoóòöõúùüýç~´`!*+.'"!@#$%¨&*()_{}^´´.><:;/°ºª¹²³,|?\-\=\[\]]|[\b]+,/,'');
        var valor = valor.replace(' ',',');
        var valor = valor.replace('  ','');
        var valor = valor.replace(',,',',');
        return valor;
    }

    function js_validateFormRelatorioVistoria(){
        var isValid = true;
        var escolaBeneficiada = $('input[name=beneficiada_fnde]:checked', '#form-vistoria').val();//valor ativo de escolas beneficiadas do FNDE
        if(escolaBeneficiada != "S" && escolaBeneficiada != "N"){
            $('#beneficiada_fnde_S').closest('.form-group ').addClass('has-error');
            $('#span_beneficiada_fnde').show();
            return false;
        }


        <?php if($controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::AMPLIACAO){ ?>

        if (escolaBeneficiada == 'S') {
            if($('input[name="obrid_reforma"]').val() == ''){ //ID da Obra obrigatório
                isValid = false;
                $('#div_obrid_reforma').addClass('form-group has-error');
                $('#span_obrid_reforma').show();
            }

            if($('.apliacao_mesmo_terreno_S').hasClass('active') || $('._N').hasClass('active')){
            }else{
                isValid = false;
                $('.apliacao_mesmo_terreno_S').closest('.form-group').addClass('has-error');
                $('#span_apliacao_mesmo_terreno').show();
            }
        }
        if (escolaBeneficiada == 'N'){
            //Selecionar escola: deve ser obrigatório
            if($("#escid option:selected").val() == ''){
                isValid = false;
                $('#div_escid').addClass('form-group has-error');
                $('#span_escid').show();
            }
        }



        <?php } ?>


        <?php if($controllerObraXQuestionario->modelObraXQuestionario->modelObraTipo->octid == Par3_Model_ObraTipo::REFORMA){ ?>

        if (escolaBeneficiada == 'S') {
            if($('input[name="obrid_reforma"]').val() == ''){ //ID da Obra obrigatório
                $('#div_obrid_reforma').addClass('form-group has-error');
                $('#span_obrid_reforma').show();
                isValid = false;
            }
        }

        if (escolaBeneficiada == 'N') {
            if($("#cod_escola option:selected").val() == ''){ //ID da Obra obrigatório
                $('#div_cod_escola').addClass('form-group has-error');
                $('#span_cod_escola').show();
                isValid = false;
            }
        }
        <?php } ?>
        return isValid;
    }

    //
    $('.salvarQuestionarioVistoria').on('click',function () {
        var isValid = js_validateFormRelatorioVistoria();
        return isValid;
    });

    /**
     *
     * @param obrid_reforma
     */
    function js_validaIdObraReforma(obrid_reforma){
        var caminho = window.location.href;
        $.ajax({
            type: "POST",
            url: caminho,
            data: '&requisicao=validaObraReformada&obrid_reforma='+obrid_reforma,
            async: false,
            success: function (resp) {
                if(resp ==  'false'){
                    swal({
                        title: "Atenção",
                        text: "A obra digitada não é válida",
                        type: "warning"
                    });
                    $('#obrid_reforma').val('');
                    $('#obrid_reforma').focus('');
                }
            }
        });
    }

    function carregaTabelaDadosObra(){
        $.ajax({
            type: "POST",
            url: window.location,
            data: '&requisicao=buscarObraPorId&obrid_reforma='+$('#obrid_reforma').val(),
            async: false,
            success: function (data) {
                $('#descricao_obra').html(data);
            }
        });
    }

    $('#escid,#cod_escola').on('change',function() {
        var escid = $(this).val();
        carregaPanelEscola(escid);
    });

     function carregaPanelEscola(escid) {
         var url = window.location.href;
         $('#div-escola-panel').html('');
         $.ajax({
             type: "GET",
             url: window.location,
             data: '&requisicao=escolapanel&escid='+escid,
             async: false,
             success: function (data) {
                 $('#div-escola-panel').html(data);
             }
         });
     }
</script>