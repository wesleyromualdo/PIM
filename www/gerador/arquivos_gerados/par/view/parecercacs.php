<?php
/**
 * View da representando a tabela parecercacs
 *
 * @category visao
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-10-2016
 * @link     no link
 */$controllerParecercacs     = new Par3_Controller_Parecercacs();
$modelParecercacs          = new Par3_Model_Par_Parecercacs("$_GET['pcacsid']"});

switch ($_POST['acao']) {
	case 'salvar':
	    $controllerParecercacs->salvar($_POST);
	    break;
	case 'inativar':
	    $id = $_GET['pcacsid'];
	    $controllerParecercacs->inativar($id);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <?php echo $simec->hidden('acao', 'acao', ''); ?>

    <input type="hidden" name="pcacsid" id="pcacsid" value="<?php echo $pcacsid ?>"/>
      <?php echo $simec->hidden('pcacsid', 'pcacsid', $modelParecercacs->pcacsid ); ?>
          <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) Parecercacs(a)</h3>
                </div>
                <div class="ibox-content">
              <?php     
                     echo $simec->input('dopid', 'dopid', $objParecercacs->dopid , array('required'=>'required', 'class' => 'inteiro') );    
                     echo $simec->textArea('pcacsobservacao', 'pcacsobservacao', $objParecercacs->pcacsobservacao , array('required'=>'required') );    
                     echo $simec->input('pcacsparecer', 'pcacsparecer', $objParecercacs->pcacsparecer , array('class' => 'inteiro') );    
                     echo $simec->input('pcacscompleto', 'pcacscompleto', $objParecercacs->pcacscompleto , array('required'=>'required', 'maxlength'=>'1') );    
                     echo $simec->cpf('usucpf', 'usucpf', $objParecercacs->usucpf , array('maxlength'=>'11') );    
                     echo $simec->input('docid', 'docid', $objParecercacs->docid , array('required'=>'required', 'class' => 'inteiro') );
                ?>
              </div>
                <div class="ibox-footer">
                    <div>
                        <button type="button" class="btn btn-success salvar"> <i class="fa fa-save"></i>Salvar </button>
                    </div>
                </div>
 
           </div>
</form>