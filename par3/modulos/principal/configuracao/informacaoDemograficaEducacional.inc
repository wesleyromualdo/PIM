<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 7/11/2017
 * Time: 10:22 AM
 */
include_once "config.inc";
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . 'includes/library/simec/view/Helper.php';
initAutoload();
require_once APPRAIZ .'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";
require_once APPRAIZ . "includes/funcoesspo_componentes.php";
require_once APPRAIZ . "par3/classes/model/IndicadorDemografico.class.inc";
require_once APPRAIZ . "par3/classes/model/Iniciativa_indicador_demografico_status.class.inc";
require_once APPRAIZ . "par3/classes/model/IniciativaIndicadorDemograficoIndicadorStatus.class.inc";

/*Controller*/
$controllerInformacaoDemograficaEducacional = new Par3_Controller_GuiaPlanejamentoInformacaoDemograficaEducacional();

$modelMunicipio = new Par3_Model_Municipio();
$modeloIndicadoresDemograficos = new Par3_Model_IndicadorDemografico();
$modeloIniciativaIndicadoresDemograficosStatus = new Par3_Model_IniciativaIndicadorDemograficoIndicadorStatus();
$modelIniciativa = new Par3_Model_Iniciativa();
$modelIniciativaIniciativasEstados = new Par3_Model_IniciativaIniciativasEstados();
$modelIniciativaIniciativasMunicipio = new Par3_Model_IniciativaIniciativasMunicipios();
$modelIndicadoresDemograficoStatus = new Par3_Model_Iniciativa_indicador_demografico_status();
$arrayStatusIndicadores = $modelIndicadoresDemograficoStatus->fetchAll();
//verifica se existe uma iniciativa associada
if(!isset($_REQUEST['iniid'])){
    $url =  "http://simec-local/par3/par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=listaIniciativas";
    $tipoRed = 'error';
    $msg = "É necessário incluir a Iniciativa.";
    simec_redirecionar($url,$tipoRed,$msg);
}else{
      $getIniciativa = $modelIniciativa->getDadosIniciativaDadosDemograficos($_REQUEST);
      $ide = new Par3_Model_Ide();
      $ide->tipo = $modelIniciativa->retornaEsferaIniciativa($getIniciativa);

      $arrListaIndicadoresDemograficos = array();
      $descrMuniOrEstad = "";
      if($ide->tipo == 'municipio'){
          $getMunicipio = $modelMunicipio->getMunicipio($getIniciativa['muncod']);
          $descMunicipio = $getMunicipio['mundescricao'];
          $descrMuniOrEstad = $descMunicipio;
          $ide->muncod = $getIniciativa['muncod'];
          $arrListaIndicadoresDemograficos = $controllerInformacaoDemograficaEducacional->arrayIndicadoresDemograficosExistentes($descMunicipio);
      }elseif($ide->tipo == 'estado'){
          $ide->estuf = ($getIniciativa['estuf'] != '')?$getIniciativa['estuf']:null;
          $descrMuniOrEstad = $getIniciativa['estdescricao'];
          $arrListaIndicadoresDemograficos = $controllerInformacaoDemograficaEducacional->arrayIndicadoresDemograficosExistentes($getIniciativa['estdescricao']);
      }

}

if(isset($_REQUEST['req'])){
    switch ($_REQUEST['req']) {
        case 'pesquisar':
            ob_clean();
            $controllerInformacaoDemograficaEducacional->filtroTabelaIndicadores($_REQUEST);
            exit();
            break;
        case 'alteraValueDemonstraivo':
            ob_clean();
            $getIniciativaIndicaroDemograficoStatus = $modeloIniciativaIndicadoresDemograficosStatus->getLinhaIndicador($_REQUEST['iniciativa'],$_REQUEST['tipoCheckBox'],trim(strtolower($_REQUEST['indicadorTabela'])));
            //ver($getIniciativaIndicaroDemograficoStatus); exit();

            //recebe o valor do
            $saveIniindsstatus = $getIniciativaIndicaroDemograficoStatus['iniindsstatus'];
            if($saveIniindsstatus == '1'){
                $saveIniindsstatus = '0';
            }else{
                $saveIniindsstatus = '1';
            }


            $getIndicadorDemografico =  $modeloIndicadoresDemograficos->getIndicadorDemograficoByCod(trim(strtolower($_REQUEST['indicadorTabela'])));


            $arrayPopular = array(
                 'iniindsid' => $getIniciativaIndicaroDemograficoStatus['iniindsid'], //chave primaria da tabela iniciativa_indicador_demografico_indicador_status
                 'iniid' => $_REQUEST['iniciativa'], // chave primária da tabela iniciativa
                 'iniidsid' => $_REQUEST['tipoCheckBox'], //chave primaria da tabela
                 'iniindsstatus' => $saveIniindsstatus, // status da linha
                 'inddid' => $getIndicadorDemografico['inddid'], //chave primária da tabela indicador demográfico
            );
            //var_dump($arrayPopular); exit();
            $controllerInformacaoDemograficaEducacional->gravarIniciativaIndicadorDemograficoStatus($arrayPopular);
            die();
            break;
        case 'xls':
            ob_clean();
            $controllerInformacaoDemograficaEducacional->filtroTabelaIndicadores($_REQUEST);
            header("Content-Disposition: attachment; filename=iniciativa_indicadorDemografico.xls");
            die;
            break;
        case 'imprimir':
            ob_clean();
            $controllerInformacaoDemograficaEducacional->filtroTabelaIndicadores($_REQUEST);
            die();
            break;
        case 'atualizarListagem':
            ob_clean();
            echo $controllerItemPregaoSicarp->listar($_REQUEST);
            die;
            break;
        case 'salvar':
            ob_clean();
                if(isset($_POST)){
                   // $modeloIniciativaStatusIndicadorDemografico->salvar($_POST);
                }else{

                }
            die();
            break;
        default:

            break;
    }
}else{

}

?>

<div class="panel panel-info">
    <div class="panel-heading">
        Informações Demográficas e Educacionais
    </div>
    <div class="panel-body">
        <!-- Descrição do grupo indicador -->
        <div class="row">
            <form name="form-filtro" id="form-filtro" class="form-horizontal">
                <div class="row">
                    <div class="col-md-offset-2 col-md-8 center">
                        <div class="form-group">
                            <label class="col-md-offset-1 col-lg-offset-1 col-sm-3 control-label">Descrição do Grupo/Indicador:</label>
                            <div class="col-sm-7 col-md-7">

                                <?php
                                $arrListaIndicadoresDemograficos = $modeloIndicadoresDemograficos->arrayToInputSelect();
                                echo $simec->select ( 'inidesc[]', '',null, $arrListaIndicadoresDemograficos, array (
                                    'placeHolder' => '',
                                    'maxlengh' => 50,
                                    $boIndicador,
                                    'required',
                                    'multiple' => 'multiple',
                                ) ); ?>
                            </div>
                        </div>

                        <!--filtros-->
                        <div class="ibox float-e-margins">
                            <div class="ibox-content" id="avancado" style="display: block;">
                                <div class="form-group">
                                    <label class="col-md-offset-1 col-lg-offset-1 col-sm-2 control-label"></label>
                                    <div class="col-lg-7 col-md-7">
                                        <?php
                                            $arrayStatus = $modelIndicadoresDemograficoStatus->fetchAll();
                                            foreach($arrayStatus as $status){
                                                ?>
                                                <div class="checkbox checkbox-default checkbox-inline ">
                                                    <input name="filterTableIndicadoresStatus[]"
                                                           id="<?php if(isset($status['iniidsid'])){
                                                               if($status['iniidsid'] == 1){ echo "plt";}
                                                               if($status['iniidsid'] == 2){ echo "analiseTecnicaFilter";}
                                                               if($status['iniidsid'] == 3){ echo "analiseMeritoFilter";}
                                                           }?>"
                                                           value="<?php if(isset($status['iniidsid'])){echo $status['iniidsid'];}?>" type="checkbox">
                                                    <label for="filterTableIndicadoresPlanoDeContas"><?php if(isset($status['iniidsdesc'])){echo $status['iniidsdesc'];}?></label>
                                                </div>
                                                <? } ?>
                                        <div class="checkbox checkbox-default checkbox-inline">
                                            <input name="filterTableIndicadoresStatus[]" value="0" id="todosFilter"  type="checkbox" onclick="js_desabilitaFiltrosStatus();"/>
                                            <label for="todosFilter"> Todos </label>
                                        </div>
                                        <div class="checkbox checkbox-default checkbox-inline">
                                            <input name="iniid" value="<?php echo $_REQUEST['iniid']; ?>" id="iniid" type="hidden"/>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="ibox-footer">
                                <div id="unidadeMedidaListagem">
                            <div class="ibox-footer">
                                <div class="center">
                                    <button type="button" class="btn btn-primary" id="pesquisar" value="pesquisar"><i class="fa fa-search"></i>
                                        Pesquisar
                                    </button>
                                    |
                                    <button type="button" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> XLS
                                    </button>
                                    <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir</button>|
                                    <button type="reset" class="btn btn-success" id="limpar"><i class="fa fa-eraser"></i> Limpar</button>
                                </div>
                            </div>     </div>
                            </div>


                            <div class="ibox-title">
                                <div id="tb_filtroIndicadores">
                                    <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="table_ide" data-qtd-acoes="2">
                                        <thead>
                                            <th>Indicador</th>
                                            <th>Plano de Trabalho</th>
                                            <th>Análise Técnica</th>
                                            <th>Análise de Mérito</th>
                                        </thead>
                                        <tbody>
                                        <?php if(isset($_REQUEST['iniid'])){
                                            $controllerInformacaoDemograficaEducacional->montaTabelaSemFiltros($_REQUEST,$ide,$descrMuniOrEstad);
                                        }else{?>
                                            <div class="alert alert-info" role="alert">Nenhum Registro Encontrado!</div>
                                        <?php } ?>
                                        </tbody>
                                    </table>
                                </div>
                            </div>

                        </div>
                    </div>
                </div>
            </form>
        </div>
    </div>
</div>

<div class="ibox-footer">
    <div class="center">
        <a href="par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=cadastroIniciativaItensComposicao&iniid=<?= $_GET['iniid'] ?>"
           class="btn btn-primary xls"
           style="width: 110px;">
            <i class="fa fa-mail-reply"></i> Voltar
        </a>
    </div>
</div>

<div id="div-impressao" style="display: none">
    <div id="div_cabecalho" class="row col-lg-offset-2">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Relatório Descrição de Iniciativas<br>
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
<br>

<script type="text/javascript">

 function ajax_alterIndicadoresDemograficaos(iniid,tipoCheckBox,indicadorTabela) {
     //alert(iniid);
     var UrlData = "&req=alteraValueDemonstraivo&iniciativa="+iniid+"&tipoCheckBox="+tipoCheckBox+"&indicadorTabela="+indicadorTabela+"&iniid="+iniid;
     $.ajax({
         type: "POST",
         url: 'par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=informacaoDemograficaEducacional'+UrlData,
         success: function(data) {
            console.log("FOI");
             // $("#tb_filtroIndicadores").html(resp);
         }
     });

 }

 $("#pesquisar").click(function (evt) {
     console.log($("#form-filtro").serialize());
     evt.preventDefault();
     $.ajax({
         type: "POST",
         url: 'par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A&guia=informacaoDemograficaEducacional',
         data: '&req=pesquisar&'+$("#form-filtro").serialize(),
         async: false,
         success: function (resp) {
             $("#tb_filtroIndicadores").html(resp);
             return true;
         }
     });
 });

 function js_expandTabela(indicadorDemografico){
    var tabela = document.getElementById('tr_tabela_'+indicadorDemografico);
    var btnExpand = document.getElementById("btn_show_tabela_"+indicadorDemografico);
     tabela.removeAttribute("style");
     btnExpand.removeAttribute("class");
     btnExpand.setAttribute("class","btn btn-primary btn-sm glyphicon glyphicon-minus");
     btnExpand.setAttribute("onclick","js_hiddeTabelaIndicadorDemografico('"+indicadorDemografico+"')");

 }

 function js_hiddeTabelaIndicadorDemografico(indicadorDemografico){
    var tabele = document.getElementById('tr_tabela_'+indicadorDemografico);
    tabele.style.display = 'none';
    tabele.removeAttribute("class");
    tabele.removeAttribute("onclick");
    tabele.setAttribute("onclick","js_expandTabela("+indicadorDemografico+")");

     var btnExpand = document.getElementById("btn_show_tabela_"+indicadorDemografico);
     btnExpand.setAttribute("class","btn btn-primary btn-sm glyphicon glyphicon-plus");
     btnExpand.setAttribute("onclick","js_expandTabela('"+indicadorDemografico+"')");
 }


 jQuery("#limpar").click(function () {
     location.reload();
     $('#inidesc').val('null').trigger("chosen:update");
     $("#pesquisar").click();
 });
 $(".xls").on("click", function () {
     window.location.assign(window.location.href + "&req=xls&" + jQuery('#form-filtro').serialize()+'&'+$('.form-listagem').serialize());
 });


 $("#imprimir").on("click", function () {
     var caminho = window.location.href;
     var action = '&req=imprimir&' + $("#form-filtro").serialize()+'&'+$('.form-listagem').serialize();
     $.ajax({
         type: "POST",
         url: caminho,
         data: action,
         async: false,
         success: function (resp) {
             $("#impressao-content").html(resp);
             var container =  $("#impressao-content");
             var table = container.find('table');
             var th = container.find('th');
             var td = container.find('td');
             table.css('width','100%')
             table.css('border-collapse','collapse');
             container.find(".popover,button,.modal").remove();
             container.find("table,td,th").css('border','1px solid black');
             container.find('tr:even').css('background-color', 'silver');
             th.css('height','50px');
             td.css('text-align','center');
             var w = window.open();
             w.document.write($('#div-impressao').html());
             w.document.title = 'Relatório -teste';
             w.print();
             w.close();
             $("#impressao-content").html("");
             //atualizarListagem($("#form-filtro").serialize()+'&'+$('.form-listagem').serialize());
             return true;
         }
     });

     local.reload();
 });

 function js_desabilitaFiltrosStatus(){
     document.getElementById('plt').setAttribute('disabled','disabled');
     document.getElementById('analiseTecnicaFilter').setAttribute('disabled','disabled');
     document.getElementById('analiseMeritoFilter').setAttribute('disabled','disabled');
     document.getElementById('analiseMeritoFilter').setAttribute('disabled','disabled');

     document.getElementById('analiseTecnicaFilter').removeAttribute('checked');
     document.getElementById('analiseMeritoFilter').removeAttribute('checked');
     document.getElementById('analiseMeritoFilter').removeAttribute('checked');

     document.getElementById('todosFilter').removeAttribute('onclick');
     document.getElementById('todosFilter').setAttribute('onclick','js_habilitaFiltroStatus();');

 }

 function js_habilitaFiltroStatus(){
     document.getElementById('plt').removeAttribute('disabled');
     document.getElementById('analiseTecnicaFilter').removeAttribute('disabled');
     document.getElementById('analiseMeritoFilter').removeAttribute('disabled');
     document.getElementById('todosFilter').removeAttribute('onclick');
     document.getElementById('todosFilter').setAttribute('onclick','js_desabilitaFiltrosStatus();');
 }
</script>
