<?php
/**
 * View da representando a tabela itemparecercacs
 *
 * @category visao
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-10-2016
 * @link     no link
 */$controllerItemparecercacs     = new Par3_Controller_Itemparecercacs();
$modelItemparecercacs          = new Par3_Model_Par_Itemparecercacs("$_GET['ipcacsid']"});

switch ($_POST['acao']) {
	case 'salvar':
	    $controllerItemparecercacs->salvar($_POST);
	    break;
	case 'inativar':
	    $id = $_GET['ipcacsid'];
	    $controllerItemparecercacs->inativar($id);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <?php echo $simec->hidden('acao', 'acao', ''); ?>

    <input type="hidden" name="ipcacsid" id="ipcacsid" value="<?php echo $ipcacsid ?>"/>
      <?php echo $simec->hidden('ipcacsid', 'ipcacsid', $modelItemparecercacs->ipcacsid ); ?>
          <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) Itemparecercacs(a)</h3>
                </div>
                <div class="ibox-content">
              <?php     
                     echo $simec->input('icoid', 'icoid', $objItemparecercacs->icoid , array('required'=>'required', 'class' => 'inteiro') );    
                     echo $simec->input('pcacsid', 'pcacsid', $objItemparecercacs->pcacsid , array('required'=>'required', 'class' => 'inteiro') );    
                     echo $simec->radio('ipcacsvalidado', 'ipcacsvalidado', $objItemparecercacs->ipcacsvalidado , array('required'=>'required') , array('type' => 'radio radio-info radio', 'style' => 'inline') );
                ?>
              </div>
                <div class="ibox-footer">
                    <div>
                        <button type="button" class="btn btn-success salvar"> <i class="fa fa-save"></i>Salvar </button>
                    </div>
                </div>
 
           </div>
</form>