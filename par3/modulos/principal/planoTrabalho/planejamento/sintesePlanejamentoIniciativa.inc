
<?php

require_once APPRAIZ . 'par3/classes/model/Escolas.class.inc';

$url = 'par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=' . $_REQUEST['inuid'];
$mInp = new Par3_Controller_IniciativaPlanejamento();
$mItr = new Par3_Controller_InstrumentoUnidade();
$nomeItr = $mItr->pegarNomeEntidade($_REQUEST['inuid']);
$mInu = new Par3_Model_InstrumentoUnidade($_GET['inuid']);
$mDoc = new Workflow_Model_Documento($mInu->docid);




$textRelatorio = "MEC/FNDE <br>
                            Relatório Descrição de Iniciativas<br>";
switch ($_REQUEST['requisicao']) {
    case 'pesquisar':
        ob_clean();
        echo $mInp->listarDimensoes($_REQUEST);
        die;
        break;
    case 'xls':
        ob_clean();
        $mInp->listarDimensoes($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Planejamento_Iniciativas_" . $nomeItr . ".xls");
        die;
        break;
    case 'imprimir':
        ob_clean();
        $mInp->listarDimensoes($_REQUEST);
        die;
        break;
    case 'imprimirSintese':
        ob_clean();
        require_once "impressao-planejado.inc";
        die;
        break;
    case 'carregaDadosDimensao':
        $dimcod = $_POST['dados'][0];
        ob_clean();
        $mInp->carregaDadosDimensao($dimcod, $_REQUEST);
        die;
        break;
//    case 'finalizarPlanejamento':
//        ob_clean();
//        echo simec_json_encode($mInp->finalizarPanejamento($_REQUEST));
//        die();
//        break;
    case 'imprimirRelatorioIniciativasFinalizadas':
        ob_clean();
            if(isset($_REQUEST['inuid'])){
                $relatorio = new Par3_Model_Relatorio_RelatorioPlanejamentoIniciativasFinalizadasMunicipio();
                $relatorio->setTitle("RESUMO GERAL");
                $relatorio->setInstrumentoUnidade($_REQUEST['inuid']);
                $relatorio->render();
            }
        die;
        break;
        case 'getTituloRelatorio':
        ob_clean();
            if(isset($_REQUEST['inuid'])){
                $relatorio = new Par3_Model_Relatorio_RelatorioPlanejamentoIniciativasFinalizadasMunicipio();
                $relatorio->setInstrumentoUnidade($_REQUEST['inuid']);
                echo $relatorio->getTituloParUnidade();
            }
        die;
        break;
    default:
        break;

}
?>
<style>
    .navbar-listagem {
        display: none;
    }

</style>
<div class="ibox" >
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Finalizar Planejamento</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class=" col-md-12 ">
                        <div class=" col-md-6 center">
                            <div class="ibox float-e-margins">
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Ano:</label>
                                    <div class="col-md-7  col-md-7 col-lg-7">
                                        <input type="text" name="ano" id="ano" value="<?= $_GET['ano'] ?>" maxlength="4"
                                               class="form-control"/>
                                    </div>
                                </div>
                                <div id="div-ciclo">
                                    <?php $mCic = new Par3_Model_CicloPar();
                                    echo $simec->select('cicid', 'Ciclo', $_GET['cicid'], $mCic->pegarSQLSelectCombo(), array('maxlength' => '255'), array('input-size' => '7')); ?>
                                </div>
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label">Código:</label>
                                    <div class="col-md-7  col-md-7 col-lg-7">
                                        <input type="text" name="codigo" id="codigo" maxlength="8" value="<?= $_GET['codigo'] ?>"
                                               class="form-control"/>
                                    </div>
                                </div>
                                <div id="div-dimensao">
                                    <?php $mDim = new Par3_Model_Dimensao();
                                    echo $simec->select('dimid', 'Dimensão', $_GET['dimid'], $mDim->listarSelect(array('itrid' => $mItr->pegarItrid($_GET['inuid']))), array('maxlength' => '255'), array('input-size' => '7')); ?>
                                </div>
                                <div id="div-descricao">
                                    <?php $mInd = new Par3_Model_IniciativaDescricao();
                                    echo $simec->input('inddsc', 'Descrição', ($_GET['inddsc']), array('maxlength' => '255', 'placeholder' => 'Informe Descrição da Iniciativa'), array('input-size' => '7')); ?>
                                </div>
                                <div id="div-situacao">
                                    <?php echo $simec->select('situacao', 'Situação', $_GET['situacao'],$mInp->getEstadosIniciativas(), array('maxlength' => '255'), array('input-size' => '7')); ?>
                                </div>
                                <div id="div-tipoObjeto">
                                    <?php $mObj = new Par3_Model_IniciativaTiposObjeto();
                                    echo $simec->select('tipoobjeto', 'Tipo Objeto', $_GET['tipoobjeto'], $mObj->pegarSQLSelectComboUnidade($_GET['inuid']), array('maxlength' => '255'), array('input-size' => '7')); ?>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-6 center">
                            <div id="div-tipoAtendimento">
                                <?php $mAdm = new Par3_Model_IniciativaTiposAtendimento();
                                echo $simec->select('tipoatendimento', 'Tipo Atendimento', $_GET['tipoatendimento'], $mAdm->pegarSQLSelectCombo(), array('maxlength' => '255'), array('input-size' => '7')); ?>
                            </div>
                            <div id="div-escolas">
                                <?php $mEsc = new Par3_Model_Escola();
                                echo $simec->select('escola', 'Escolas', $_GET['escola'], $mEsc->pegarSQLSelectCombo($_GET['inuid']), array('maxlength' => '255'), array('input-size' => '7')); ?>
                            </div>
                            <div id="div-areasRelacionadas">
                                <?php $mArl = new Par3_Model_IniciativaAreas();
                                echo $simec->select('areas', 'Areas Relacionadas', $_GET['areas'], $mArl->pegarSQLSelectComboDescricaoUnidade($_GET['inuid']), array('maxlength' => '255'), array('input-size' => '7')); ?>
                            </div>
                            <div id="div-programas">
                                <?php $mPgr = new Par3_Model_IniciativaProgramas();
                                echo $simec->select('programas', 'Programas', $_GET['programas'], $mPgr->pegarSQLSelectComboSiglaUnidade($_GET['inuid']), array('maxlength' => '255'), array('input-size' => '7')); ?>
                            </div>
                            <div id="div-etapas">
                                <?php $mEtp = new Par3_Model_EnsinoEtapa();
                                echo $simec->select('etapas', 'Etapas', $_GET['etapas'], $mEtp->pegarSelectCombo(), array('maxlength' => '255'), array('input-size' => '7')); ?>
                            </div>
                            <div id="div-desdobramento">
                                <?php $mDsbr = new Par3_Model_IniciativaDesdobramentos();
                                echo $simec->select('desdobramento', 'Desdobramento', $_GET['desdobramento'], $mDsbr->pegarSelectCombo(), array('maxlength' => '255'), array('input-size' => '7')); ?>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
        </div>
        <div class="ibox-footer">
            <div class="center">
<!--                <button type="submit" class="btn btn-primary" id="pesquisar-planejamento-iniciativa"><i-->
<!--                            class="fa fa-search"></i>Pesquisar-->
<!--                </button>-->

                <button type="button" id="pesquisar-planejamento-iniciativa" data-dismiss="modal" class="btn btn-primary pesquisar-planejamento-iniciativa" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button>

                <button type="submit" class="btn btn-primary xls" style="width:110px;"><i
                            class="fa fa-file-excel-o"></i> Gerar Excel
                </button>
                <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i
                            class="glyphicon glyphicon-print"></i> Imprimir
                </button>
                |
                <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div id="planejamentoIniciativaListagem" style="overflow: scroll;">
            <?php $mInp->listarDimensoes($_REQUEST) ?>
        </div>
        <br>
        <div class="col-lg-12 text-right">
            <h3><b>Valor Total Planejamento: <?php echo $mInp->getValorTotalPlanejamento($_REQUEST) ?></b></h3>
        </div>
        <br>
        <br>
        <?
        if ($pendenciasImpeditivas > 0) { ?>
            <div class="widget red-bg p-lg text-center">
                <div class="row">
                    <div class="col-md-1">
                        <i class="fa fa-warning fa-4x"></i>
                    </div>
                    <div class="col-md-11 text-left">
                        <h3>Pendências do PAR </h3>
                        <h4>Para Finalizar Planejamento, será necessário resolver as Pendências do PAR.</h4>
                        <a class="btn btn-info pull-right" id="visualizarPendencias" role="button">Visualizar
                            Pendências do PAR </a>
                        <?php //endif; ?>

                    </div>
                </div>
            </div>
            <div class="ibox-footer" style="padding-left: 0px; margin-right: -14px">
                <div class="row">
                    <div class="col-lg-12 text-right">

                        <a class="btn btn-info pull-right" id="imprimirPlanejamto" role="button">Imprimir
                            Planejamento </a>

                    </div>
                </div>
            </div>
            <?
        } else if ($mInp->verificaExistenciaPendenciasPreenchimento($_REQUEST) > 0) { ?>

            <div class="widget red-bg p-lg text-center">
                <div class="row">
                    <div class="col-md-1">
                        <i class="fa fa-warning fa-4x"></i>
                    </div>
                    <div class="col-md-11 text-left">
                        <h3>Pendências de Preenchimento</h3>
                        <h4>Existem Iniciativas em cadastramento que ainda não foram finalizadas, tem certeza que
                            deseja Finalizar Planejamento somente com as Iniciativas Finalizadas?</h4>
                        <?php //if (in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfils)) : ?>
                        <a class="btn btn-info pull-right" id="vizualizarIniciativasII" role="button">Visualizar
                            Iniciativas </a>
                        <?php //endif; ?>

                    </div>
                </div>

            </div>
            <div class="ibox-footer" style="padding-left: 0px; margin-right: -14px">
                <div class="row">
                    <div class="col-lg-12 text-right">

                        <a class="btn btn-info pull-right" id="imprimirPlanejamto" role="button">Imprimir
                            Planejamento </a>

                    </div>
                </div>
            </div>
            <?
        } else {
            if ($mDoc->esdid == 2000) { ?>

                <div class="widget navy-bg p-lg text-center" id="nao_imprimir">
                    <div class="row">
                        <div class="col-md-1">
                            <i class="fa fa-thumbs-o-up fa-4x"></i>
                        </div>
                        <div class="col-md-11 text-left">

                            <h3>Planejamento Finalizado!</h3>
                            <h4>Não sera possível incluir novas iniciativas</h4>

                        </div>
                    </div>
                </div>
            <? } else { ?>
                <div class="widget navy-bg p-lg text-center" id="nao_imprimir">
                    <div class="row">
                        <div class="col-md-1">
                            <i class="fa fa-thumbs-o-up fa-4x"></i>
                        </div>
                        <div class="col-md-11 text-left">

                            <h4>Caso tenha concluído seu Planejamento e deseja enviar para Análise, clique em "Finalizar
                                Planejamento" abaixo.</h4>
                            <h4>Após a finalização , não sera possível incluir novas iniciativas</h4>

                        </div>
                    </div>
                </div>
            <? } ?>
            <div class="ibox-footer" style="padding-left: 0px; margin-right: -14px">
                <div class="row">
                    <div class="col-lg-12 text-right">

                        <a class="btn btn-info pull-right" id="imprimirPlanejamto" role="button">Imprimir
                            Planejamento </a>

                    </div>
                </div>
            </div>

        <? } ?>

        <div id="div-impressao" style="display: none">
            <div id="div_cabecalho" class="row col-lg-offset-2">
                <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
                    <tr bgcolor="#ffffff">
                        <td valign="top" width="50" rowspan="2">
                            <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                        </td>
                        <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                            <div class="">
                                <strong>SIMEC - </strong>Sistema Integrado de Ministério da Educação<br>
                                Ministério da Educação / SE - Secretaria Executiva
                                .::<div id="div-titulo-relatorio"> </div>
                            </div>
                        </td>
                        <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                            Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>
                            Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/>
                        </td>
                    </tr>
                </table>
                <hr style="color:black; width:95%; margin-left:-18px;">
            </div>
            <div class="clearfix" style="margin-bottom:10px;"></div>
            <div id="impressao-content">

            </div>
        </div>


        <script>


           // $("#div-principal").css("display", "block");

            $(document).ready(function () {

                if (<?=$mDoc->esdid;?> == 2000
                )
                {
                    swal("Planejamento Finalizado!", "O Planejamento dessa unidade já foi finalizado!", "warning");
                    return false;
                }
            });


//            $("#finalizarPlanejamento").click(function (evt) {
//                evt.preventDefault();
////                if (<?//// echo $existePendenciaRestricao;?>////) {
////
////                    swal("Erro", "Existem pendências para o município/estado", "error");
////                    return false;
////                }
//                var inpid = $(this).attr('inpid');
//                var caminho = window.location.href;
//                var action = 'requisicao=finalizarPlanejamento';
//                swal({
//                        title: "<b>Finalizar Planejamento</b>",
//                        text: "<b>Deseja Finalizar Planejamento? Após a finalização não será possível alterar/incluir as iniciativas planejadas</b>",
//                        type: "warning",
//                        html: true,
//                        showCancelButton: true,
//                        cancelButtonText: "Não",
//                        confirmButtonText: "Sim",
//                        closeOnConfirm: false
//                    }, function (isConfirm) {
//                        if (isConfirm) {
//                            $.ajax({
//                                type: "POST",
//                                url: caminho,
//                                data: action,
//                                async: false,
//                                success: function (resp) {
//                                    var res = $.parseJSON(resp);
//                                    console.log(res);
//                                    if (!isNaN(parseInt(res)) && isFinite(res)) {
//                                        msgSuccess('<?php //echo $url; ?>//&aba=consulta', "Planejamento Finalizado Com Sucesso!");
//                                    }
//                                    return false;
//                                }
//                            });
//                        }
//                        return false;
//                    }
//                );
//            });

            $("#visualizarPendencias").click(function () {
                window.open("par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid=<?=$_GET['inuid']?>#");
            });

            function vizualizarIniciativa(id) {
                window.location.href = "par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=<?=$_GET['inuid']?>&aba=cadastro&inpid=" + id;
            }

            $("#vizualizarIniciativasII").click(function () {
                window.open("par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=<?=$_GET['inuid']?>&aba=consulta");
            });
//
//            $("#imprimirPlanejamto").click(function () {
//                alert("Funcionalidade não disponível no mommento!")
//            });


            $("#limpar").click(function () {
                $('#ano').val('');
                $('#cicid').val('').trigger('chosen:updated');
                $('#codigo').val('');
                $('#dimid').val('').trigger('chosen:updated');
                $('#inddsc').val('');
                $('#situacao').val('').trigger('chosen:updated');
                $('#tipoobjeto').val('').trigger('chosen:updated');
                $('#tipoatendimento').val('').trigger('chosen:updated');
                $('#escola').val('').trigger('chosen:updated');
                $('#areas').val('').trigger('chosen:updated');
                $('#programas').val('').trigger('chosen:updated');
                $('#etapas').val('').trigger('chosen:updated');
                $('#desdobramento').val('').trigger('chosen:updated');
                window.location.assign('par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=<?=$_GET['inuid']?>&aba=sintese');


                //  $('#pesquisar-planejamento-iniciativa').click();

            });

            $(".xls").on("click", function () {
                window.location.assign(window.location.href + "&requisicao=xls&" + jQuery('#form-filtro').serialize() + '&' + $('.form-listagem').serialize());
            });

            $("#imprimir").on("click", function () {
                getTituloRelatorio(2);
                var caminho = window.location.href;
                var action = '&requisicao=imprimir&' + $("#form-filtro").serialize() + '&' + $('.form-listagem').serialize();
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        $("#impressao-content").html(resp);
                        var container = $("#impressao-content");
                        var table = container.find('table');
                        var th = container.find('th');
                        var td = container.find('td');
                        table.css('width', '100%')
                        table.css('border-collapse', 'collapse');
                        container.find(".popover,button,.modal").remove();
                        container.find("table,td,th").css('border', '1px solid black');
                        container.find('tr:even').css('background-color', 'silver');
                        th.css('height', '50px');
                        td.css('text-align', 'center');
                        var w = window.open();
                        w.document.write($('#div-impressao').html());
                        w.document.title = 'Relatório - Iniciativa Planejamentos - <?php echo $nomeItr; ?>';
                        w.print();
                        w.close();
                        $("#impressao-content").html("");
                        return true;
                    }
                });
            });
            
//            $("#imprimirSintese").on("click", function () {
//                getTituloRelatorio(2);
//                var caminho = window.location.href;
//                var action = '&requisicao=imprimirSintese';
//                $.ajax({
//                    type: "POST",
//                    url: caminho,
//                    data: action,
//                    async: false,
//                    success: function (resp) {
//                        $("#impressao-content").html(resp);
//                        var container = $("#impressao-content");
//                        var table = container.find('table');
//                        var th = container.find('th');
//                        var td = container.find('td');
//                        table.css('width', '100%')
//                        table.css('border-collapse', 'collapse');
//                        container.find(".popover,button,.modal").remove();
//                        container.find("table,td,th").css('border', '1px solid black');
//                        container.find('tr:even').css('background-color', 'silver');
//                        th.css('height', '50px');
//                        td.css('text-align', 'center');
//                        var w = window.open();
//                        w.document.write($('#div-impressao').html());
//                        w.document.title = 'Relatório - Iniciativa Planejamentos - <?php echo $nomeItr; ?>';
//                        w.print();
//                        w.close();
//                        $("#impressao-content").html("");
//                        return true;
//                    }
//                });
//            });

//            $(".pesquisar-planejamento-iniciativa").click(function (evt) {
//                $btn = jQuery(this).button('loading');
//                var caminho = window.location.href;
//                window.location.assign(caminho + "&" + jQuery('#form-filtro').serialize());
//                var action = '&requisicao=pesquisar&' + $("#form-filtro").serialize();
//
//                $.ajax({
//                    type: "POST",
//                    url: caminho,
//                    data: action,
//                    async: false,
//                    success: function (resp) {
//                        $("#planejamentoIniciativaListagem").html(resp);
//                        return true;
//                    }
//                });
//            });



            jQuery(".pesquisar-planejamento-iniciativa").click(function () {

                $btn = jQuery(this).button('loading');
                var caminho = window.location.href;
                var action = '&requisicao=pesquisar&' + $("#form-filtro").serialize();
                window.location.assign(caminho + "&" + jQuery('#form-filtro').serialize());

                $.ajax({
                    url: caminho,
                    type: 'post',
                    data: action,
                    success: function(resp){

                        $("#planejamentoIniciativaListagem").html(resp);

                    }
                });
            });


            $(function () {
                $('#ano').bind('keydown', soNums); //
                $('#codigo').bind('keydown', soNums);


            });
            function soNums(e) {

                //teclas adicionais permitidas (tab,delete,backspace,setas direita e esquerda)
                keyCodesPermitidos = new Array(8, 9, 37, 39, 46);

                //numeros e 0 a 9 do teclado alfanumerico
                for (x = 48; x <= 57; x++) {
                    keyCodesPermitidos.push(x);
                }

                //numeros e 0 a 9 do teclado numerico
                for (x = 96; x <= 105; x++) {
                    keyCodesPermitidos.push(x);
                }

                //Pega a tecla digitada
                keyCode = e.which;

                //Verifica se a tecla digitada é permitida
                if ($.inArray(keyCode, keyCodesPermitidos) != -1) {
                    return true;
                }
                return false;
            }
            $('#ano').bind('keypress', function (event)
            {
                var regex = new RegExp("^[0-9a-zA-Z\b]+$");
                var key = String.fromCharCode(!event.charCode ? event.which : event.charCode);
                if (!regex.test(key))
                {
                    event.preventDefault();
                    return false;
                }
            });
            $('#codigo').bind('keypress', function (event)
            {
                var regex = new RegExp("^[0-9a-zA-Z\b]+$");
                var key = String.fromCharCode(!event.charCode ? event.which : event.charCode);
                if (!regex.test(key))
                {
                    event.preventDefault();
                    return false;
                }
            });
            
            $('#imprimirPlanejamto').click(function (evt) {
                evt.preventDefault();
                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: '&requisicao=ajaxImprimePlanejado',
                    async: true,
                    beforeSend: function () {
                        $('#loading').show();
                    },
                    complete: function () {
                    },
                    success: function (resp) {
                        $('#loading').hide();
                        $('#quadroImprimePlanejado').html(resp);
                    }
                });
                $('#quadroImprimePlanejado').modal();
            });

/* OLD
            $("#imprimirPlanejamto").click(function () {
                var caminho = window.location.href;
                var action = "&requisicao=imprimirRelatorioIniciativasFinalizadas&inuid=<?php echo $_REQUEST['inuid'];?>";
                getTituloRelatorio(1);
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp){
                        $("#div-impressao").find("#impressao-content").html(resp);
                        var container = $("#div-impressao").find("#impressao-content");
                        var table = container.find('table');
                        var th = container.find('th');
                        var td = container.find('td');
                        table.css('width', '100%')
                        //table.css('border-collapse', 'collapse');
                        container.find(".popover,button,.modal").remove();
                        container.find("thead,th").css('background-color', 'silver');
                        container.find('th:even').css('background-color', 'silver');
                        th.css('height', '50px');
                        td.css('text-align', 'center');
                        var w = window.open();
                        w.document.write($('#div-impressao').html());
                        w.document.title = 'Relatório - Iniciativa Planejamentos - ';
                        w.print();
                        //w.close();
                        $("#impressao-content").html();
                        return true;
                    }
                });
            });
            */

            /**
             * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
             * @param imprimir
             * @description Esta função seta o cabecalho de impressao, 1 = impressão das iniciativas finalizadas, 2 =  impressão das iniciativas
             */
            function getTituloRelatorio(imprimir){
                if(imprimir == 1){
                    var caminho = window.location.href;
                    var action = "&requisicao=getTituloRelatorio&inuid=<?php echo $_REQUEST['inuid'];?>";
                    $.ajax({
                        type: "POST",
                        url: caminho,
                        data: action,
                        async: false,
                        success: function (resp){
                            $("#div-titulo-relatorio").html(resp);
                        }
                    });
                }

                if(imprimir == 2){
                    $("#div-titulo-relatorio").html('MEC/FNDE <br>Relatório Descrição de Iniciativas<br>');
                }
                
                if(imprimir == 3){
                    $("#div-titulo-relatorio").html('MEC/FNDE <br>Relatório de Planejamento<br>');
                }

            }

        </script>

<!--modal imprime planejado-->
<div class="ibox animated modal" id="quadroImprimePlanejado" tabindex="-1" role="dialog" aria-hidden="true"></div>