<?php
/**
    * Arquivos que vão popular os campos
    * Totas as consulta devem trazer apenas dados de iniciativas cadastradas
*/
$pesquisaAnalise = new Par3_Controller_PesquisaIniciativaAnalise();
$mInp = new Par3_Controller_IniciativaPlanejamento();
$arrAreaRelacionada = Par3_Model_IniciativaAreas::getAreaPorIniciativa();
$arrCiclo = Par3_Model_CicloPar::getCicloPorIniciativa();
$arrDimecao = Par3_Model_Dimensao::getDimencaoPoriniciativa();
$arrSituacao = Par3_Model_IniciativaPlanejamento::getSituacaoIniciativa( 'and doc.esdid in ('.PAR3_ESDID_EM_ANALISE.', '.PAR3_ESDID_AGUARDANDO_ANALISE.', '.PAR3_ESDID_EM_DILIGENCIA.')' );
$arrTipoObjeto = Par3_Model_IniciativaPlanejamento::getTipoObjetoporIniciativa();
$arrTipoAtendimento = Par3_Model_IniciativaPlanejamento::getTipoAtendimentoIniciativa();
$arrPrograma = Par3_Model_IniciativaPlanejamento::getProgramaIniciativa();
$arrEtapa = Par3_Model_IniciativaPlanejamento::getEtapaIniciativa();
$arrDesdobramento = Par3_Model_IniciativaPlanejamento::getDesdobramentoIniciativa();
$arrSituAnalise = Par3_Model_IniciativaPlanejamento::getSituacaoAnalise();
$arrTecnico = Par3_Model_IniciativaPlanejamento::getTecnicoAnalise();
$arrIniciativaAno = Par3_Model_IniciativaPlanejamento::getInicitivaAno();
$sqlTipoAssitencia = Par3_Model_IniciativaPlanejamento::getTipos_Assistencia();

$arrIniciativa = $mInp->getIniciativaPorInpAtivo(['inuid' => $_GET['inuid'], 'iniid' => $inp->iniid, 'consulta' => 'true'], true);
$obAnalise = new Par3_Controller_AnalisePlanejamentoIniciativa();

switch ($_REQUEST['requisicao']){

    case 'limpar_filtro':
        unset($_SESSION['filtro']);
    break;


    case 'carregarMunicipiosAnalise':
        //lista todos os municípios relacionados ao Estado(estuf) informado
        ob_clean();
        $municipio = new Territorios_Model_Municipio();//instancia a classe município
        echo simec_json_encode($municipio->carregar($municipio->pegarSQLSelect($_GET['estuf'])));die;
        break;
    case 'carregarTodosMunicipios':
        //lista todos os municípios relacionados ao Estado(estuf) informado
        ob_clean();
        $municipio = new Territorios_Model_Municipio();//instancia a classe município
        $arrMun = $municipio->recuperarTodos("muncod as codigo, estuf||' - '||mundescricao as descricao",null,'2 ASC');
        echo simec_json_encode($arrMun);die;
        break;
    case 'verificaraprovacaoemlote':
        ob_clean();
        $arrAnalises = $_POST['analises'];

        echo $obAnalise->validarAprovacaoLote($arrAnalises);die;
        break;
    case 'tramitaAnaliseLote':

        $proxEst = $_POST['proxEst'];
        $estAtual = $_POST['estAtual'];
        ob_clean();
        require APPRAIZ . 'par3/modulos/principal/planoTrabalho/planejamento/formComentarioTramitacao.inc';die;
        break;
    case 'tramitaremlote':
        ob_clean();
        echo $obAnalise->tramitarEmLote($_POST['arrdocid'],$_POST['aedid'],$_POST['docdsc']);die;
        break;
    default:
        $controllerVinculacao = new Par3_Controller_VinculacaoNutricionista();
        break;
}

require APPRAIZ . 'includes/cabecalho.inc';

$url = 'par3.php?modulo=principal/planoTrabalho/planejamento/aprovarAnaliseLote&acao=A';
echo $simec->tab( criaAbaPar(), $url );
?>
<style>
    .navbar-listagem {
        margin-top: 0px;
        float: right;
        width: 500px;
    }
    .anoClass label{
        margin-left: -4px;
    }
    .estadoClass a.chosen-single.chosen-default{
        width: 170px ;
        margin-left: 21px;
    }
    .estadoClass a.chosen-single.chosen-single-with-deselect{
        width: 170px ;
        margin-left: 21px;
    }
   /*.estadoClass .chosen-container-single .chosen-search .chosen-drop {
        width: 200px ;
    }*/
    .estadoClass .chosen-drop {
        width: 180px ;
        margin-left: 21px;
    }
    .estadoClass label{
        margin-left: -21px;
    }
    .linhaInferior .divdtPedito {
        width: 323px;
    }
    .linhaInferior {
        margin-left: 6.9%;
    }
    .divSitua a.chosen-single.chosen-default{
        top: 10px;
    }
    .divTecnico{
        top: 10px;
    }
</style>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Aprova em Lote as Análise dos Planejamentos das Iniciativas</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-iniciativaplanejamento" id="form-iniciativaplanejamento" class="form-horizontal" method="post">
                <div class="row">
                    <div class="col-lg-6">
                        <input type="hidden" name="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>
                        <input type="hidden" name="requisicao_form" value="filtrar"/>
                        <input type="hidden" name="inpid" id="inpid" value="<?php echo $inp->inpid; ?>"/>
                        <?php echo $simec->select('iarid', 'Área Relacionada', $_REQUEST['iarid'], $arrAreaRelacionada, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->input('inpid_filtro', 'Código Planejamento', $_REQUEST['inpid_filtro'], array('type' => 'number'),array('input-size' => '8')); ?>
                        <div class="row">
                            <div class="col-lg-4 anoClass" style="margin-left: 17%;">
                                <?php echo $simec->select('iniano', 'Ano', $_REQUEST['iniano'], $arrIniciativaAno, array('maxlength' => '255'), array()); ?>
                            </div>
                            <div class="col-lg-5">
                                <?php echo $simec->select('cicid', 'Ciclo', $_REQUEST['cicid'], $arrCiclo, array('maxlength' => '255'), array()); ?>
                            </div>
                        </div>
                        <?php echo $simec->select('dimid', 'Dimensão', $_REQUEST['dimid'], $arrDimecao, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('indid', 'Iniciativa', $_REQUEST['indid'], $arrIniciativa, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('esdid', 'Situação Iniciativa', $_REQUEST['esdid'], $arrSituacao, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('tipoassistencia', 'Tipo de Assistência', $_REQUEST['tipoassistencia'], $sqlTipoAssitencia, array('maxlength' => '255'), array('input-size' => '8')); ?>
                    </div>
                    <div class="col-lg-6" id="">
                        <div class="row">
                            <?php echo $simec->radio('itrid', 'Esfera', ($_REQUEST['itrid']?:0), [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'], array('maxlength' => '255')); ?>
                            <div class="col-lg-4 estadoClass" style="margin-left: 17%;">
                                <?php
                                    $_POST['estuf'] = tratarArrayParaMultiSelect($_POST['estuf']);//trata o array
                                    $arrEstado = Territorios_Model_Estado::pegarSQLSelect ($_POST);
                                echo $simec->select('estuf[]', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255','multiple' => 'multiple'), array());
                                ?>
                            </div>
                            <div class="col-lg-5" id="div-muncod" style="display:<?= $_REQUEST['itrid'] == 1 ?'none':''?>;">
                                <?php
                                $arrMunicipio = array();
                                if($_POST['muncod'] && $_POST['itrid'] != 1){
                                    $_POST['muncod'] = tratarArrayParaMultiSelect($_POST['muncod']);//trata o array
                                    $municipio = new Territorios_Model_Municipio();//instancia a classe município
                                    //lista todos os municípios relacionados ao Estado(estuf) informado
                                    if($_POST['estuf']){
                                        $arrMunicipio = $municipio->pegarSQLSelect($_POST['estuf']);
                                    }else{
                                        $arrMunicipio = $municipio->recuperarTodos("muncod as codigo, estuf||' - '||mundescricao as descricao",null,'2 ASC');
                                        $arrMunicipio = simec_preparar_array($arrMunicipio);
                                    }
                                }
                                echo $simec->select('muncod[]', 'Município',$_POST['muncod'],$arrMunicipio, array('maxlength' => '255','multiple' => 'multiple'), array());
                                ?>
                            </div>
                        </div>
                        <?php echo $simec->select('intoid', 'Tipo do Objeto', $_REQUEST['intoid'], $arrTipoObjeto, array('maxlength' => '255'), array('input-size' => '8')); ?>
<!--                        --><?php //echo $simec->select('intaid', 'Tipo de Atendimento', $_REQUEST['intaid'], $arrTipoAtendimento, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('prgid', 'Programa', $_REQUEST['prgid'], $arrPrograma, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('etaid', 'Etapa', $_REQUEST['etaid'], $arrEtapa, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('desid', 'Desdobramento', $_REQUEST['desid'], $arrDesdobramento, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <div class="divTecnico">
                            <?php echo $simec->select('usucpf_analise', 'Técnico', $_REQUEST['usucpf_analise'], $arrTecnico, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        </div>
                    </div>
                </div>
                    <div class="row linhaInferior">
                        <?php
                        $_REQUEST['esdid_analise'] = tratarArrayParaMultiSelect($_REQUEST['esdid_analise']);
                        echo $simec->select('esdid_analise', 'Situação da Análise', $_REQUEST['esdid_analise'], $arrSituAnalise, array('maxlength' => '255','multiple' => 'multiple','class' => 'col-lg-offset-3'), array('input-size' => '6')); ?>
                    </div>
                </div>
            </form>
            <div class="ibox-footer">
                <div class="center">
                	<button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-primary btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button>
                    <button type="button" class="btn btn-success" id="btn-limpar"><i class="fa fa-eraser"></i> Limpar</button>
                    <button type="button" id="btn-tramitar-lote" data-dismiss="modal" class="btn btn-primary" data-loading-text="Tramitando, aguarde ..."><i class="fa fa-plus"></i> Aprovar em Lote</button>
                </div>
            </div>
           <!-- Listagem  -->
            <div class="float-e-margins" id="listagem-obras">
                <div class="ibox-content" id="div-listagem-obras" style=": block;">
                    <div class="table-responsive" >
                        <div>
                        	<div id="testeSQL" style="overflow: scroll;">
                            <?php
                            if($_REQUEST['requisicao_form'] == 'filtrar'){
                                echo $obAnalise->carregaAnaliseValidacaoCoordenador( $_REQUEST );
                            }
                            ?>
                            </div>

                        </div>
                    </div>
                    <br>
                    <br>
                </div>
            </div>
            <!-- END Listagem  -->
            <div class="ibox float-e-margins animated modal" id="modal_aprovacao_lote" role="dialog" aria-hidden="true">
                <div class="modal-dialog modal-lg">
                    <div class="modal-content">
                        <div class="ibox-title">
                            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                            <h3 id="modal_titulo" align="center">Tramitar em Lote</h3>
                        </div>
                        <div class="ibox-content" id="conteudo-modal"></div>
                        <div class="ibox-footer">
                            <div class="row">
                                <button type="button" id="fecharModal" data-dismiss="modal" class="btn btn-default">
                                    <i class="fa fa-times-circle-o"></i> Fechar
                                </button>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            
            <div class="ibox float-e-margins animated modal" id="modal_tramitar" role="dialog" aria-hidden="true">
                <div class="modal-dialog">
                    <div class="modal-content">
                        <div class="ibox-title">
                            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                            <h3 align="center">Confirmar Tramitação</h3>
                        </div>
                        <div class="ibox-content" id="conteudo-modal"></div>
                    </div>
                </div>
            </div>

        </div>
    </div>

</div>

<script>

function ajax_informarSituacao(docid,aedid,comentario,proxEst,estAtual)
{
    var data = {requisicao:'tramitaAnaliseLote',arrdocid:docid,aedid:aedid,comentario:comentario,proxEst:proxEst, estAtual:estAtual};

    $.ajax({
        type: "POST",
        url: window.location.href,
        data: data,
        async: false,
        success: function (resp) {
            $('#loading').hide();
            $("#modal_tramitar").find('#conteudo-modal').html('');
            $("#modal_tramitar").modal();
            $('#modal_aprovacao_lote').modal('hide');
            $("#modal_tramitar").find('#conteudo-modal').html(resp);
        }
    });
}

jQuery(function() {


    function ajax_verificarAnaliseAprovacaoEmLote(analises){
        var action = window.location.href;
        var data = {requisicao:'verificaraprovacaoemlote',analises:analises};
        $.ajax({
            type: "POST",
            url: action,
            data: data,
            async: false,
            success: function (resp) {
                if(resp == '0'){
                    alert('<h3>Selecione apenas registros com a mesma situação.</h3>');
                    return false;
                }
                $('#modal_aprovacao_lote').find('#conteudo-modal').html('');
                $('#modal_aprovacao_lote').modal();
                $('#modal_aprovacao_lote').find('#conteudo-modal').html(resp);
            }
        });
    }

	
    jQuery("#btn-tramitar-lote").click(function () {

    	var analises = $('input[name=checklote]:checked').map(function(){
            return $(this).val();
        }).get();

        if(analises.length <= 0){
            alert('<h3>Nenhuma registro selecionado</h3>');
            return false;
        }
        ajax_verificarAnaliseAprovacaoEmLote(analises);

    });


    $(document).on('click','#btnFecharTramitacao',function(){
        $('#modal_tramitar').modal('hide');
        $("#modal_tramitar").find('#conteudo-modal').html('');
        $('#modal_aprovacao_lote').modal('show');
    });
	
    //tela de loading
    $(window).on('beforeunload',function(){
        $('.loading-dialog-par3').show();
    });
    $(window).on('click','#btn-pesquisar #btn-limpar',function(){
        $('.loading-dialog-par3').show();
    });
    $(window).load(function(){
        $('.loading-dialog-par3').hide();
    });
    //renderizeMunicipio();


    jQuery('#estuf').change(function(){
        if($('[name=itrid]:checked').val() == 1){
            return;
        }else{
            if(!Array.isArray($(this).val())){
                listarMunicipios();
            }else{
                carregarMunicipioAnalise($('#estuf').val(),$('#muncod').val());
            }
        }
    });

    function listarMunicipios()
    {
        console.log('testes');
        var options = jQuery('#muncod');
        options.empty();
        options.trigger('chosen:updated');
        options.append(new Option('', ''));
        jQuery.ajax({
            type: "GET",
            url: window.location.href,
            data: { requisicao:'carregarTodosMunicipios'},
            success: function(retorno){
                options.append(new Option('', ''));
                $.each(JSON.parse(retorno), function() {
                    options.append(new Option(this.descricao, this.codigo));
                });
                options.val(muncod);
                options.trigger('chosen:updated');
            }
        });
    }

    function carregarMunicipioAnalise(estuf, muncod) {
        if(estuf != '' && $('[name=itrid]:checked').val() != 1) {
            var options = jQuery('#muncod');
            options.empty();
            options.trigger('chosen:updated');
            if(!Array.isArray(estuf)){
                return;
            }
            $('#loading').hide();
            options.append(new Option('', ''));
            jQuery.ajax({
                type: "GET",
                url: window.location.href,
                data: { requisicao:'carregarMunicipiosAnalise',estuf : estuf },
                success: function(retorno){
                    options.append(new Option('', ''));
                    $.each(JSON.parse(retorno), function() {
                        options.append(new Option(this.descricao, this.codigo));
                    });
                    options.focus();
                    options.val(muncod);
                    options.trigger('chosen:updated');
                }
            });
        }
    }

    $('#muncod').one('focus',function() {
        console.log('on focus');
        if(Array.isArray($('#estuf').val())){
            return;
        }
        listarMunicipios();
    });

    $('#div-muncod').one('click',function() {
        console.log('on click');
        if(Array.isArray($('#estuf').val()) || Array.isArray($('#muncod').val())){
            return;
        }
        listarMunicipios();
    });

    $('[name=itrid]').change(function(){
        if($(this).val() == 1){
            $('#div-muncod').hide();
            $('#muncod').empty();
            $('#muncod').trigger("chosen:updated");
        }else{
            $('#div-muncod').show();
        }
    });

    function renderizeMunicipio() {
        var filtroMunicipio = jQuery("select[name=muncod]").parents("div.form-group");
        if (jQuery('input:radio[name=itrid]:checked').val() === '1' || !jQuery('select[name=estuf]').val()) {
            filtroMunicipio.slideUp();
        } else {
            filtroMunicipio.slideDown();
        }
    }

});


    jQuery("#btn-limpar").click(function () {
        window.location.href = 'par3.php?modulo=principal/planoTrabalho/planejamento/aprovarAnaliseLote&acao=A';
    });

    //envia as informações do formulário via POST
    jQuery(".btn-pesquisar").click(function () {
        $btn = jQuery(this).button('loading');
        jQuery('[name="form-iniciativaplanejamento"]').submit();
    });


    function CadastrarAnalise( inpid, inuid, iniano, anaid ) {
        if( anaid ){
        	window.location.href = "par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inpid="+inpid+'&inuid='+inuid+'&iniano='+iniano+'&anaid='+anaid+'&aba=analisarPlanejamento';
        } else {
        	window.location.href = "par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inpid="+inpid+'&inuid='+inuid+'&iniano='+iniano+'&aba=analisarPlanejamento';
        }
    }


$('[name=tipo]').change(function(){
    if($(this).val() == 'OBRA'){
        $('#div-intoid').hide();
        $('#intoid').val('');
        $('#intoid').trigger("chosen:updated");
    }else{
        $('#div-intoid').show();
    }
});

$('[data-toggle="popover"]').popover({html:true, animation:true, delay:0,trigger: "hover"})
</script>