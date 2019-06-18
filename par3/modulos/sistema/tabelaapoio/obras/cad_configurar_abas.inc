<?php

$ctrObraTipo    = new Par3_Controller_ObraTipo();
$ctrAbas        = new Par3_Controller_ObraAba();
$ctrConfAba     = new Par3_Controller_ObraConfiguracaoAba();
$ctrConfDoc     = new Par3_Controller_ObraConfiAbaTipoDoc();
$ctrTipoFoto    = new Par3_Controller_ObraAbaTipoFoto();

switch ($_REQUEST['requisicao']) {
    case 'vinculaAba':
        ob_clean();
        echo $ctrConfAba->salvar($_REQUEST);
        die;
        break;
    case 'formConfiguracao':
        ob_clean();
        $ctrConfAba->formConfiguracao($_REQUEST);
        die;
        break;
    case 'salvarConfiguracaoDoc':
        ob_clean();
        $resp = $ctrConfDoc->salvar($_REQUEST);
        if(is_array($resp)){
            echo simec_json_encode($resp);
        }else{
            echo $resp;
        }
        die;
        break;
    case 'salvarConfiguracaoDocFotos':
        ob_clean();
        $resp = $ctrConfAba->salvarConfFotos($_REQUEST);
        if(is_array($resp)){
            echo simec_json_encode($resp);
        }else{
            echo $resp;
        }
        die;
        break;
    case 'recuperarTipoFoto':
        ob_clean();
        $mTipoFoto = $ctrTipoFoto->recuperar($_REQUEST['atfid']);
        require_once 'configurar_aba/tipoFoto.inc';
        die;
        break;
    case 'salvarTipoFoto':
        ob_clean();
        $resp = $ctrTipoFoto->salvar($_REQUEST);
        echo simec_json_encode($resp);
        die;
        break;
    case 'atualizarListagemTipoFoto':
        $ovpid = isset($_REQUEST['ovpid'])?$_REQUEST['ovpid']:'';
        ob_clean();
        $arrTipoFoto = $ctrConfAba->pegarSelectComboTipoFotos($_REQUEST['otpid']);
        echo $simec->select('atfid[]', '', '', $arrTipoFoto,array('required'),array('input-size'=>12)) ;
        die;
        break;
    default:
        break;
}
global $simec;
$disabled = 'disabled';
if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-2">
                <a href="par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=configurar_abas"><i class="fa fa-arrow-left" aria-hidden="true"></i>voltar para a pesquisa</a>
            </div>
            <div class="col-md-8">
                <h3 class="center">Configuração de Abas</h3>
            </div>
            <div class="col-md-2">
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="col-md-offset-2 col-md-8 center">
                <div class="form-group ">
                    <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label" style="text-align: right">Tipo de Obra: </label>
                    <div class="col-sm-5 col-md-5 col-lg-5 ">
                        <h4>
                            <?php echo $ctrObraTipo->getDescricao()?>
                        </h4>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="col-md-12 center">
                <?php echo $ctrAbas->listar($_REQUEST);?>
            </div>
        </div>
    </div>
</div>