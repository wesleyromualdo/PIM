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
//$arrEscola = Par3_Model_IniciativaPlanejamento::getEscolaIniciativa();
$arrSituAnalise = Par3_Model_IniciativaPlanejamento::getSituacaoAnalise();
$arrTecnico = Par3_Model_IniciativaPlanejamento::getTecnicoAnalise();
$arrIniciativaAno = Par3_Model_IniciativaPlanejamento::getInicitivaAno();
$sqlTipoAssitencia = Par3_Model_IniciativaPlanejamento::getTipos_Assistencia();

//$arrIniciativa = $mInp->getIniciativaPorInpAtivo(['inuid' => $_GET['inuid'], 'iniid' => $inp->iniid, 'consulta' => 'true'], true);

$arrIniciativa = $mInp->getIniciativaPesquisaIndicadorSelect();
switch ($_REQUEST['requisicao']){
    case 'filtrar':
        $_SESSION['filtro'] = $_REQUEST;
//         ob_clean();
//         echo $pesquisaAnalise->listar($_REQUEST);
//         die();
        break;
    case 'limpar_filtro':
        unset($_SESSION['filtro']);
    break;

    case 'detalharAnalise':
        ob_clean();

        $iniid = $_POST['dados'][1];
        $inpid = $_POST['dados'][0];
        $itd = new Par3_Controller_PesquisaIniciativaAnalise();
        $itd->listarAnalises($iniid, $inpid);
        die;
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
        $arrMun = $municipio->recuperarListaMunicipios();
        echo simec_json_encode($arrMun);die;
        break;
    default:
        $controllerVinculacao = new Par3_Controller_VinculacaoNutricionista();
        break;
    case 'carregaEscolas':
        $escolas = new Par3_Model_IniciativaPlanejamento();
        $arrEscola = $escolas->carregar($escolas::getEscolaIniciativa($_REQUEST['muncod']));
        $arrEscola = is_array($arrEscola)?$arrEscola:array();
        print simec_json_encode($arrEscola);
        die;
	break;
}

if(!$_REQUEST['requisicao'] && $_SESSION['filtro']['requisicao'] != ''){
    $_REQUEST = $_SESSION['filtro'];
}

require APPRAIZ . 'includes/cabecalho.inc';

$url = 'par3.php?modulo=principal/planoTrabalho/planejamento/pesquisarIndicadorAnalise&acao=A';
echo $simec->tab ( criaAbaPar (), $url );
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
                <h2 class="center">Pesquisa de Iniciativas de Estados e Municipios para Análise</h2>
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
                        <?php echo $simec->input('inpid_filtro', 'Código Planejamento', $_REQUEST['inpid_filtro'], array('type' => 'number'),array('input-size' => '8')); ?>
                        <?php echo $simec->select('indid', 'Iniciativa', $_REQUEST['indid'], $arrIniciativa, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('iarid', 'Área Relacionada', $_REQUEST['iarid'], $arrAreaRelacionada, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <div class="row">
                            <div class="col-lg-4 anoClass" style="margin-left: 17%;">
                                <?php echo $simec->select('iniano', 'Ano', $_REQUEST['iniano'], $arrIniciativaAno, array('maxlength' => '255'), array()); ?>
                            </div>
                            <div class="col-lg-5">
                                <?php echo $simec->select('cicid', 'Ciclo', $_REQUEST['cicid'], $arrCiclo, array('maxlength' => '255'), array()); ?>
                            </div>
                        </div>
                        <?php echo $simec->select('dimid', 'Dimensão', $_REQUEST['dimid'], $arrDimecao, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('esdid', 'Situação Iniciativa', $_REQUEST['esdid'], $arrSituacao, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('tipoassistencia', 'Tipo de Assistência', $_REQUEST['tipoassistencia'], $sqlTipoAssitencia, array('maxlength' => '255'), array('input-size' => '8')); ?>
                    </div>
                    <div class="col-lg-6" id="">
                        <div class="row">
                            <?php echo $simec->radio('itrid', 'Esfera', ($_REQUEST['itrid']?:0), [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'], array('maxlength' => '255')); ?>
                            <div class="col-lg-4 anoClass" style="margin-left: 17%;">
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
<!--                        <div class="col-lg-1 divdtPedito">-->
<!--                            <div class="form-group ">-->
<!--                                <label class="col-sm-3 col-md-3 col-lg-3 control-label">-->
<!--                                    Data do Pedido:-->
<!--                                </label>-->
<!--                                <div class="col-sm-4 col-md-4 col-lg-4  date ">-->
<!--                                    <div class="input-group" style="top:10px;">-->
<!--                                        <span class="input-group-addon"><span class="fa fa-calendar"></span></span>-->
<!--                                        <input name="desdataini" type="text" class="datemask form-control" style="width: 160px;" value="--><?php //echo $_REQUEST['desdataini']; ?><!--">-->
<!--                                    </div>-->
<!--                                </div>-->
<!--                            </div>-->
<!--                        </div>-->
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
                                echo $pesquisaAnalise->listar($_REQUEST);
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

        </div>
    </div>

</div>

<script>
jQuery(function() {
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
        window.location.href = 'par3.php?modulo=principal/planoTrabalho/planejamento/pesquisarIndicadorAnalise&acao=A';
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