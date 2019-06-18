<?php
/**
 * View da representando a tabela das
 *
 * @category visao
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio@teste.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-11-2016
 * @link     no link
 */$controllerDas     = new Par3_Controller_Das();
$modelDas          = new Par3_Model_Gestaogabinete_Das("$_GET['dasid']"});

switch ($_POST['acao']) {
	case 'salvar':
	    $controllerDas->salvar($_POST);
	    break;
	case 'inativar':
	    $id = $_GET['dasid'];
	    $controllerDas->inativar($id);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <?php echo $simec->hidden('acao', 'acao', ''); ?>

    <input type="hidden" name="dasid" id="dasid" value="<?php echo $dasid ?>"/>
      <?php echo $simec->hidden('dasid', 'dasid', $modelDas->dasid ); ?>
          <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) Das(a)</h3>
                </div>
                <div class="ibox-content">
              <?php     
                     echo $simec->input('dascodigo', 'dascodigo', $objDas->dascodigo , array('maxlength'=>'5') );    
                     echo $simec->input('dasstatus', 'dasstatus', $objDas->dasstatus , array('maxlength'=>'1') );
                ?>
              </div>
                <div class="ibox-footer">
                    <div>
                        <button type="button" class="btn btn-success salvar"> <i class="fa fa-save"></i>Salvar </button>
                    </div>
                </div>
 
           </div>
</form>