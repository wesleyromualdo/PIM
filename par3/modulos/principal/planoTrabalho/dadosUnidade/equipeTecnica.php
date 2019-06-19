<?php
/**
 * Tela de dados da Equipe Técnica
 *
 * @category visao
 * @package  A1
 * @author   Daniel Fiuza <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 14/02/2017
 * @link     no link
 */
$renderEntidade                      = new Par3_Controller_Entidade();
$controllerEntidade                  = new Par3_Controller_InstrumentoUnidadeEntidade();
$controllerEquipeTecnica             = new Par3_Controller_EquipeTecnica();
$controllerInstrumentoUnidade        = new Par3_Controller_InstrumentoUnidade();
$modelConfiguracoesPar               = new Par3_Model_ConfiguracoesPar();
$controllerConfiguracaoParUnidade    = new Par3_Controller_ConfiguracaoParUnidadeControle();

$inuid = $_REQUEST['inuid'];
$tenid = $_REQUEST['tenid'] = Par3_Model_InstrumentoUnidadeEntidade::EQUIPE_TECNICA;
$itrid = Par3_Controller_InstrumentoUnidade::pegarItrid($inuid);

$tcpid = ($itrid == 1) ? Par3_Model_TipoConfiguracoesPar::LIMITE_EQUIPE_ESTADUAL : Par3_Model_TipoConfiguracoesPar::LIMITE_EQUIPE_MUNICIPAL;
$_REQUEST['tcpid'] = $tcpid;

$confGlobal  = $modelConfiguracoesPar->retornaConfiguracaoGlobal($tcpid);//recupera o limite global
$couvalor = $controllerConfiguracaoParUnidade->retornaConfiguracaoUnidadeLimite($inuid,$tcpid);//recupera o limite da unidade

$limite = $couvalor != 0 || $couvalor? $couvalor : $confGlobal['copvalor'];
$qtdMembrosEquipeTecncia = $controllerEquipeTecnica->contarMembrosEquipeTecnica($_REQUEST['inuid']);

$limiteIntegranteRestante = $limite - $qtdMembrosEquipeTecncia;//calcula o limite de integrantes restantes permitidos

switch ($_REQUEST['req']) {
    case 'carregarCargos':
        ob_clean();
        $modelCargo = new Par3_Model_EquipeLocalCargo();
        echo $modelCargo->carregarJSONCombo($_REQUEST);
        die();
        break;
    case 'salvar':
        $controllerEquipeTecnica->salvar($_REQUEST);
        break;
    case 'editar':
        ob_clean();
        echo $controllerEquipeTecnica->formEquipeTecnica($_REQUEST);die;
        break;
    case 'recuperar':
        ob_clean();
        echo simec_json_encode($controllerEquipeTecnica->recuperarIntegranteEquipeTecnica($_REQUEST));die;
        break;
    case 'aplicar-limite':
        $controllerConfiguracaoParUnidade->salvar($_REQUEST);
        break;
    case 'limpar-limite':
        $_REQUEST['couvalor'] = 0;
        $controllerConfiguracaoParUnidade->salvar($_REQUEST);
        break;
    case 'inativar':
        $controllerEquipeTecnica->invativar($_REQUEST);
        break;
    default:
        $arrParamHistorico = array();
        $arrParamHistorico['inuid'] = $inuid;
        $arrParamHistorico['tenid'] = $tenid;
        $arrParamHistorico['booCPF'] = true;
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        break;
}


?>

<input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid ?>"/>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3>Equipe Técnica - Integrantes</h3>
            </div>
            <div class="col-md-6">
                <?php if($disabled == ''){ ?>
                    <button class="btn btn-success novo  pull-left" data-toggle="modal" data-target="#modal">
                        <i class="fa fa-plus-square-o"></i> Inserir Integrante
                    </button>
                <?php }?>
            </div>
        </div>
        <div class="row">
            <div class="col-md-12">
                <h4>Limite de Integrantes Restantes: <span id="limite-integrante"><?php echo isset($limiteIntegranteRestante) ? $limiteIntegranteRestante : ""; ?></span>
                    <?php if($controllerEquipeTecnica->limiteIntegrantesExcedido($_REQUEST) && $qtdMembrosEquipeTecncia > 0){?>
                        <span class="badge badge-danger">O limite máximo de integrantes foi atingido</span>
                    <?php } ?>
                    </h4>
                <?php if($modelUnidade->testa_superuser()){ ?>
                    <form id="form-limite-integrantes">
                        <input type="hidden" name="tcpid" value="<?php echo $tcpid; ?>"/>
                        <input type="text" id="limiteIntegrantes" title="limite integrantes" name="couvalor" value="<?php echo isset($limite) ? $limite : ""; ?>"/>
                        <button class="btn btn-success" id="aplicar-limite" title="Aplicar Limite">
                            <i class="fa fa-check"></i>
                        </button>
                        <button class="btn btn-danger" id="limpar-limite" title="Limpar Limite e Resta">
                            <i class="fa fa-eraser"></i>
                        </button>
                    </form>
                <?php } ?>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <input type="hidden" id="tcpid" value="<?php echo $tcpid?>"/>
        <?php
        $_REQUEST['elostatus'] = 'A';
        $controllerEquipeTecnica->listaEquipe($_REQUEST);
        ?>
    </div>
    <div class="ibox-footer">
        <div class="row">
            <div class="col-lg-6 text-left">
                <button type="button" class="btn btn-success previous" >Anterior</button>
            </div>
            <div class="col-lg-6 text-right">
                <button type="button" class="btn btn-success next" >Próximo</button>
            </div>
        </div>
    </div>
</div>
<?php require_once 'equipeTecnicaModal.php'; ?>
<div class="ibox ">
    <div class="ibox-title">
        <h5>Equipe Local - Histórico Modificações</h5>
<!--         <div class="ibox-tools"> -->
<!--             <a class="collapse-link"> -->
<!--                 <i class="fa fa-chevron-down"></i> -->
<!--             </a> -->
<!--         </div> -->
    </div>
    <?php $controllerEntidade->formHistorico($arrParamHistorico); ?>
</div>

<script>

    $(function(){
        $('#limiteIntegrantes').bind('keydown',soNums); //

    });
    function soNums(e){

        //teclas adicionais permitidas (tab,delete,backspace,setas direita e esquerda)
        keyCodesPermitidos = new Array(8,9,37,39,46);

        //numeros e 0 a 9 do teclado alfanumerico
        for(x=48;x<=57;x++){
            keyCodesPermitidos.push(x);
        }

        //numeros e 0 a 9 do teclado numerico
        for(x=96;x<=105;x++){
            keyCodesPermitidos.push(x);
        }

        //Pega a tecla digitada
        keyCode = e.which;

        //Verifica se a tecla digitada é permitida
        if ($.inArray(keyCode,keyCodesPermitidos) != -1){
            return true;
        }
        return false;
    }


    <?php if($disabled == ''){ ?>
    function editaIntegrante(id) {
        var inuid = $('#inuid').val();
        var caminho = window.location.href;
        var action = '&req=editar&entid='+id;
        //window.location.href = url + action + '&entid=' + id;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                console.log(resp);
                $("#conteudo-modal").html(resp);
                $("#modal").modal();
            }
        });
        $('#modal').on('hidden.bs.modal', function () {
            $("#conteudo-modal").html("");
        });
    }

    function inativaIntegranteEquipe(id) {
        var caminho = window.location.href;
        var action = '&req=recuperar&entid='+id;
        var user;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                var  user =  $.parseJSON(resp);
                swal({
                        title: "Remover registro com CPF: "+user.entcpf,
                        text: "Tem certeza que deseja remover "+user.entnome+" do cadastro de "+user.tendsc+"?",
                        type: "warning",
                        showCancelButton: true,
                        confirmButtonColor: "#DD6B55",
                        cancelButtonText: "Não",
                        confirmButtonText: "Sim",
                        closeOnConfirm: false
                    },
                    function(isConfirm) {
                        if (isConfirm) {
                            var inuid = $('#inuid').val();
                            var action = '&req=inativar';
                            window.location.href += action + '&entid=' + id;
                        }
                    }
                )
            }
        });
    }
<?php } ?>

    $(document).ready(function() {

        <?php if($modelUnidade->testa_superuser()){ ?>
        $("#aplicar-limite").click(function(evt){
            evt.preventDefault();
            jQuery('#form-limite-integrantes').isValid(salvarlimite);
        });
        function salvarlimite(isValid) {
            if (isValid) {
                window.location.assign(window.location.href + "&req=aplicar-limite&" + jQuery('#form-limite-integrantes').serialize());
            }
            return false;
        }

        $("#limpar-limite").click(function(evt){
            evt.preventDefault();
            jQuery('#form-limite-integrantes').isValid(limparlimite);
        });
        function limparlimite(isValid) {
            if (isValid) {
                window.location.assign(window.location.href + "&req=limpar-limite&" + jQuery('#form-limite-integrantes').serialize());
            }
            return false;
        }
        <?php } ?>


        <?php if($limiteIntegranteRestante <= 0){?>
            var salvar = $(".novo");
            salvar.click(function (evt) {
                    evt.preventDefault();
                    return false;
                });
            salvar.click(function (evt) {
                evt.preventDefault();
                swal({  title:'Aviso!', text:'O limite máximo de integrantes foi atingido.', type:'warning'});
            });
        <?php }?>

        $('.collapse-link').click( function() {
            var ibox = $(this).closest('div.ibox');
            var button = $(this).find('i');
            var content = ibox.find('div.ibox-content');
        });

        $('.previous').click(function(){
            var url = window.location.href.toString().replace('tecnico', 'dirigente');
            window.location.href = url;
        });

        $('.next').click(function(){
            var url = window.location.href.toString().replace('tecnico', 'equipe');
            window.location.href = url;
            console.log(window.location.href = url);
        });
    });
</script>