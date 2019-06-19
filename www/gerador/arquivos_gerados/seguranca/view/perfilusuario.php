<?php
/**
 * View da representando a tabela perfilusuario
 *
 * @category visao
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-08-2016
 * @link     no link
 */$controllerPerfilusuario     = new Par3_Controller_Perfilusuario();
$modelPerfilusuario          = new Par3_Model_Seguranca_Perfilusuario("$_GET['pflcod']"});

switch ($_POST['acao']) {
	case 'salvar':
	    $controllerPerfilusuario->salvar($_POST);
	    break;
	case 'inativar':
	    $id = $_GET['pflcod'];
	    $controllerPerfilusuario->inativar($id);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <?php echo $simec->hidden('acao', 'acao', ''); ?>

    <input type="hidden" name="pflcod" id="pflcod" value="<?php echo $pflcod ?>"/>
      <?php echo $simec->hidden('pflcod', 'pflcod', $modelPerfilusuario->pflcod ); ?>
    <input type="hidden" name="usucpf" id="usucpf" value="<?php echo $usucpf ?>"/>
      <?php echo $simec->hidden('usucpf', 'usucpf', $modelPerfilusuario->usucpf ); ?>
          <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) Perfilusuario(a)</h3>
                </div>
                <div class="ibox-content">
              <?php 
                ?>
              </div>
                <div class="ibox-footer">
                    <div>
                        <button type="button" class="btn btn-success salvar"> <i class="fa fa-save"></i>Salvar </button>
                    </div>
                </div>
 
           </div>
</form>