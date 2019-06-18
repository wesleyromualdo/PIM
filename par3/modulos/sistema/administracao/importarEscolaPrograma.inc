<?php
require APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "par3/classes/model/adesao_programa/EscolasEscolaAcessivel.class.inc";

// recupera carga do csv de escolas aptas por estado/municipio
$oEscolaAcessivel = new Par3_Model_EscolasEscolaAcessivel();
$oProAdesao = new Par3_Model_Proadesao();
$programas = $oProAdesao->pegarAdesoes();

if (!$_GET['pfaid']) die("<script>alert('Programa não encontrado');history.back(-1);</script>");

switch ($_REQUEST['req']){
        
    case 'salvarDadosPrograma':
        ob_clean();
        $controller = new Par3_Model_EscolasEscolaAcessivel();
        // atualiza valor total do programa
        $resValorPrograma = $controller->atualizaValorTotalPorPrograma($_POST);
        // realiza carga das escolas do programa 
        $resUploadCSV = $controller->importarCsvEscolas($_POST, $_POST['pfaid'], $_POST['pfaano_ciclo']);
        $url = "par3.php?modulo=sistema/administracao/importarEscolaPrograma&acao=A&requisicao=carregarDados&pfaid={$_POST['pfaid']}";
        simec_redirecionar($url, 'success');
        break;          
    default:
        break;
}

switch ($_REQUEST['requisicao']){
    case 'carregarDados':
        $proadesao = new Par3_Model_Proadesao($_REQUEST['pfaid']);
        $dadosPrograma = $proadesao->pegarDadosAdesaoPrograma($_REQUEST['pfaid']);
        $listaprogramas = $proadesao->pegarAdesoes();      
        $listaValorUfMunPrograma = $oEscolaAcessivel->getSomaValorEscolasAptasMunUF($_REQUEST['pfaid']);
        break;
    case "salvar_dados_escola":
        ob_clean();
        $oEscolaAcessivel->salvarDadosEscolaCarga($_POST);
        $url = "par3.php?modulo=sistema/administracao/importarEscolaPrograma&acao=A&requisicao=carregarDados&pfaid={$_REQUEST['pfaid']}";
        simec_redirecionar($url, 'success', 'Dados da Escola salvo com sucesso!');
        break;    

    case "excluir_carga_escola":
        ob_clean();
        $oEscolaAcessivel->excluirEscolaCarga($_GET);
        $url = "par3.php?modulo=sistema/administracao/importarEscolaPrograma&acao=A&requisicao=carregarDados&pfaid={$_REQUEST['pfaid']}";
        if($res['erro'] == 0){
            $mensagem = 'Carga de escolas excluida com sucesso!';
            simec_redirecionar($url, 'success', $mensagem);
        } else if($res['erro'] == 1) { 
            $mensagem = 'Não foi possivel excluir a carga de escolas, pois já existem escolas enviadas para Adesão!';
            simec_redirecionar($url, 'danger', $mensagem);
        }
        exit();
        break;   
    case "excluir_escola":
        ob_clean();
        $oEscolaAcessivel->excluirEscola($_GET['eeaid'], $_REQUEST['pfaid']);
        $url = "par3.php?modulo=sistema/administracao/importarEscolaPrograma&acao=A&requisicao=carregarDados&pfaid={$_REQUEST['pfaid']}";
        if($res['erro'] == 0){
            $mensagem = 'Escola excluida com sucesso!';
            simec_redirecionar($url, 'success', $mensagem);
        } else if($res['erro'] == 1) { 
            $mensagem = 'Não foi possivel excluir a escolas, pois já foi enviada para Adesão!';
            simec_redirecionar($url, 'danger', $mensagem);
        }
        exit();
        break;   

    case "editar_escola": 
        ob_clean();
        echo $oEscolaAcessivel->getDadosEscolasCarga($_GET);
        exit();
        break;
    
}

echo $simec->title('Configurações do programa', '');
?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox">
            <form method="post" name="formulario" id="formulario" class="form form-horizontal"  enctype="multipart/form-data">
                <input type="hidden" name="req" value="salvarDadosPrograma">
                <input type="hidden" name="pfaano_ciclo" value="<?php echo $dadosPrograma['pfaano']; ?>">
                <div class="ibox-title">
                    <h3>Importação de escolas do Programa</h3>
                </div>
                <div class="ibox-content">
                    <div class="form-group ">
                        <label for="pfaid" class="col-sm-3 col-md-3 col-lg-3 control-label">Programa: *</label>
                        <div class="col-sm-9 col-md-9 col-lg-9">
                            <select id="pfaid" name="pfaid" class="form-control" onchange="carregarDadosPrograma(this.value)" required="">
                                <option value="">Selecione o Programa</option>
                                <?php foreach($programas as $key) { ?>
                                    <?php if($key['pfaid'] == 12) { ?>
                                        <option value="<?php echo $key['pfaid']?>" <?php echo ($_GET['pfaid'] == $key['pfaid']) ? 'selected=""' : ''; ?>><?php echo $key['prgdsc']?> - <?php echo $key['pfaano']?> (<?php echo formata_data($key['pfadatainicial']) ?> a <?php echo formata_data($key['pfadatafinal'])?>)</option>
                                    <?php } ?>
                                <?php } ?>
                            </select>
                        </div>
                    </div>
                    <div id="opcao-escola-acessivel" >
                        <?php
                            $attribs = array('required' => 'required', 'maxlength' => '255', 'onkeyup' => "this.value=mascaraglobal('[.###],##',this.value)", 'onblur' => "MouseBlur(this);this.value=mascaraglobal('[.###],##',this.value);");
                            echo $simec->input('csvapta', 'Escolas aptas', null, array('type'=>'file', 'help' =>'Formatos permitidos: csv (colunas separadas por vírgulas em codificação UTF-8)'), array('input-size'=>'6'));
                            echo $simec->input('csvinapta', 'Escolas não aptas', null, array('type'=>'file', 'help' =>'Formatos permitidos: csv (colunas separadas por vírgulas em codificação UTF-8)'), array('input-size'=>'6'));
                            echo $simec->input('pfavalortotalprograma', 'Valor Total (R$)', formata_valor($dadosPrograma['pfavalortotalprograma']), $attribs, array('input-size' => '6'));
                        ?>
                    </div>
                    <div class="ibox-footer m-t-md">
                        <div class="row">
                            <div class="col-lg-3"></div>
                            <div class="col-lg-9">
                                <div>
                                    <button type="submit" id="salvar-carga" class="btn btn-success"><i class="fa fa-save"></i> Salvar</button>
                                    <a  href="?modulo=sistema/administracao/cadAdesao&acao=A" class="btn btn-white"><i class="fa fa-arrow-left"></i> Voltar</a>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </form>
            <!-- lista de escolas aptas / não aptas do programa escola acessível par3.escolas_ea -->
            <div class="ibox m-t-md">
                <div class="ibox-title" style="height: 60px">
                    <h5 style="padding-top: 7px;">Lista de Escolas Aptas</h5>
                        <div class="ibox-tools">
                            <button href="javascript:;" onclick="excluirCargaEscolasAptas(<?php echo $_GET['pfaid']; ?>)" id="excluir-carga-apta" class="btn btn-danger"><i class="fa fa-trash"></i> Excluir Aptas</button>
                        </div>
                </div>
                <div class="ibox-content">
                    <?php 
                        if($_GET['pfaid']) {
                            $oEscolaAcessivel->getListaEscolasAptasPrograma($_REQUEST['pfaid']);  
                        }  
                    ?>
                </div>
            </div>
            <div class="ibox">
                <div class="ibox-title" style="height: 60px">
                    <h5 style="padding-top: 7px;">Lista de Escolas Não aptas</h5>
                        <div class="ibox-tools">
                            <button href="javascript:;" onclick="excluirCargaEscolasInaptas(<?php echo $_GET['pfaid']; ?>)" id="excluir-carga-apta" class="btn btn-danger"><i class="fa fa-trash"></i> Excluir não aptas</button>
                        </div>
                </div>
                <div class="ibox-content">
                    <?php 
                        if($_GET['pfaid']) {
                            $oEscolaAcessivel->getListaEscolasInaptasPrograma($_REQUEST['pfaid']);  
                        }  
                    ?>      
                </div>
            </div>
            <div class="ibox">
                <div class="ibox-title">
                    <h3>Valor disponibilizado por Estado/Município</h3>
                </div>
                <div class="ibox-content">
                    <table class="table table-hover dataTable">
                        <thead>
                            <th>UF</th>
                            <th>Município</th>
                            <th>Esfera</th>
                            <th>Valor (R$)</th>
                        </thead>
                        <tbody>
                            <?php if (!empty($listaValorUfMunPrograma)) { ?>
                                <?php  foreach($listaValorUfMunPrograma as $key) { ?>
                                    <tr>
                                        <td><?php echo $key['estuf']?></td>
                                        <td><?php echo $key['municipio']?></td>
                                        <td><?php echo $key['esfera']?></td>
                                        <td><?php echo formata_valor($key['valordestinado'])?></td>
                                    </tr>
                                <?php } ?>  
                            <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    </div>
</div>

<div id="modalEditarEscola" class="ibox float-e-margins animated modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-title">
                <h5><i class="fa fa-angle-double-right"></i> Alterar Dados da Escola</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            </div>
            <div class="ibox-content">
                <form class="form-horizontal" method="post" name="formulario_escola" id="formulario_escola" enctype="multipart/form-data">
                    <input type="hidden" name="requisicao" value="salvar_dados_escola">
                    <input type="hidden" id="eeaid" name="eeaid" value="">    
                    <?php
                        echo $simec->input('eeacodinep', 'Código INEP', '', array('disabled', 'required'));
                        echo $simec->input('eeanome', 'Nome da Escola', '', array('disabled', 'maxlength' => '150', 'required'));
                        //echo $simec->input('eeavalordestinado', 'Valor Destinado (R$)', '', array('maxlength' => '15', 'required'));                        
                    ?>
                    <div class="form-group ">
                        <label for="eeavalordestinado" class="col-sm-3 col-md-3 col-lg-3 control-label">Valor Destinado (R$): <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                        <div class="col-sm-6 col-md-6 col-lg-6 ">
                            <input name="eeavalordestinado" id="eeavalordestinado" type="text" value="" class="form-control" required="required" maxlength="255" onkeyup="this.value=mascaraglobal('[.###],##',this.value)" onblur="MouseBlur(this);this.value=mascaraglobal('[.###],##',this.value);">
                        </div>
                        <div style="clear:both"></div>
                    </div>
                        
                    <?php
                    
                        $radioApta = array('t' => 'Sim', 'f' => 'Não');
                        echo $simec->radio('eeaapta', 'Apta', '', $radioApta, array('required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));                        
                        $radioSelecionada = array('t' => 'Sim', 'f' => 'Não');
                        echo $simec->radio('eeaselecionada', 'Selecionada', '', $radioSelecionada, array('required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
                        
                    ?>                       
                    <div class="form-group m-t-md">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label"></label>
                        <div  class="col-sm-9 col-md-9 col-lg-9 ">
                            <button type="button" name="salvar" id="salvar_escola" class="btn btn-primary" onclick="salvarAlteracaoEscola();"><i class="fa fa-save"></i> Salvar</button>
                        </div>
                    </div>
                </form>
            </div>
            <div class="ibox-footer">
                <div class="form-actions col-md-offset-3 text-right">
                    <button type="button" class="btn btn-white" data-dismiss="modal">Fechar</button>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    jQuery('[name*=prgid]').change(function (){              
        if(jQuery('#prgid').val() == '55'){
            jQuery('#opcao-escola-acessivel').show();
        } else {
            jQuery('#opcao-escola-acessivel').hide();
        }
    });
    
    $('#modal-disp').click(function (evt) {
        evt.preventDefault();
        $('#modal_val_disp').modal();
        $('#loading').hide();
    });
    
    
    $('#salvarvalores').click(function(evt){
        evt.preventDefault();
        $(document).find('#loading').hide();
        salvarValoresDestinados();
    });
    
    $('#salvar-carga').click(function(evt){
        evt.preventDefault();
        $('.loading-dialog-par3').show();
        if($('#pfaid').val() == ''){
            alert('Selecione o Programa!');
            return false;
        } else {
            $('#formulario').submit();
            $(document).find('#loading').show();
            $('#salvar-carga').prop('disabled', true); 
        }
    });
    
    function carregarDadosPrograma(pfaid){
        if(pfaid !== ''){
                window.location.href = '?modulo=sistema/administracao/importarEscolaPrograma&acao=A&requisicao=carregarDados&pfaid='+pfaid;
            }            
        }
    
    function excluirCargaEscolasAptas(pfaid){
        if(pfaid !== ''){
            if (confirm('Tem certeza que deseja excluir a carga com todas as Escolas Aptas?')) {
                window.location.href = '?modulo=sistema/administracao/importarEscolaPrograma&acao=A&requisicao=excluir_carga_escola&apta=true&pfaid='+pfaid;
            } 
        }
    }
    
    function excluirCargaEscolasInaptas(pfaid){
        if(pfaid !== ''){
            if (confirm('Tem certeza que deseja excluir a carga com todas as Escolas não aptas?')) {
                window.location.href = '?modulo=sistema/administracao/importarEscolaPrograma&acao=A&requisicao=excluir_carga_escola&apta=false&pfaid='+pfaid;
            } 
        }
    }
    
    function excluirEscola(eeaid){
        if(eeaid !== ''){
            if (confirm('Tem certeza que deseja excluir esse registro?')) {
                window.location.href = '?modulo=sistema/administracao/importarEscolaPrograma&acao=A&requisicao=excluir_escola&eeaid='+eeaid+'&pfaid='+<?php echo $_REQUEST['pfaid']; ?>;
            } 
        }
    }
    
    function alterarEscola(eeaid){
        jQuery.ajax({
            type: "POST",
            url: window.location,
            data: 'requisicao=editar_escola&eeaid=' + eeaid,
            async: false,
            success: function (resp) {
//                console.log(resp);
                var data = JSON.parse(resp);
                $("#modalEditarEscola").modal();
                
                $('#eeaid').val(data.eeaid);
                $('#eeacodinep').val(data.eeacodinep);
                $('#eeanome').val(data.eeanome);
                $('#eeavalordestinado').val(data.eeavalordestinado);
                if(data.eeaapta == 't'){
                    $("#eeaapta-t").prop( "checked", data.eeaapta );
                } else {
                    $("#eeaapta-f").prop( "checked", data.eeaapta );
                }
                if(data.eeaselecionada == 't'){
                    $("#eeaselecionada-t").prop( "checked", data.eeaselecionada );
                } else {
                    $("#eeaselecionada-f").prop( "checked", data.eeaselecionada );
                }
            }
        });        
    }
    
//    function excluirEscola(eeaid){
//        alert(eeaid);
//    }
    
    function salvarAlteracaoEscola(){    
        
        $('#formulario_escola').submit();
        $('#salvar_escola').prop('disabled', true);
//        var dadosEscola = $('#formulario_escola').serialize();
        
//        jQuery.ajax({
//            type: "POST",
//            url: window.location,
//            data: 'requisicao=salvar_dados_escola&eeaid=' + dadosEscola,
//            async: false,
//            success: function (resp) {
////                console.log(resp);
//                if(resp !== 'false'){
//                    $("#modalEditarEscola").hide();
//                } else {
//                    alert('Ocorreu um erro ao atualizar os dados da Escola!');                
//                    return false;
//                }
//                      
//            }
//        });
    }
    
            
    function salvarArquivo(){
        if(!$('#formimportarcsv').valid()){
            return;
        }
        var action = window.location.href+'&requisicao=importarcsv';
        //Como implementar submit de formulários enctype="multipart/form-data"
        var data = new FormData($('#formimportarcsv').get(0));
        
        $.ajax({
            type: "POST",
            url: action,
            contentType: false,//necessário para fazer upload
            processData: false,//necessário para fazer upload
            data: data,
            beforeSend: function () {
                $(document).find('#loading').hide();
                //$('.loading-dialog-par3').show();
            },
            success: function (resp) {
//                console.log(resp);
                $('#loading-dialog-par3').hide();
                $('#loading').hide();
                var res = $.parseJSON(resp);
                if (!isNaN(parseInt(res)) && isFinite(res)) {
                    $(this).find('div').removeClass('has-error');
                    $(this).find('.badge-danger').remove();
                    //$("#modal_importar_cfn").modal("hide");
                    msgSuccess(null,"Processamento realizado com sucesso.");
                    return false;
                }
            },
            complete : function() {
                $('.loading-dialog-par3').hide();
            },
            error : function () {
                $('.loading-dialog-par3').hide();
                $(document).find('#loading').hide();
                alert('Ocorreu um erro.');
            }
        });
    }
</script>