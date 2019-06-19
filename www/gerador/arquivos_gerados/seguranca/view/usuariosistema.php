<?php
/**
 * View da representando a tabela usuario_sistema
 *
 * @category visao
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-08-2016
 * @link     no link
 */$controllerUsuario_sistema     = new Par3_Controller_Usuario_sistema();
$modelUsuario_sistema          = new Par3_Model_Seguranca_Usuario_sistema("$_GET['usucpf']"});

switch ($_POST['acao']) {
	case 'salvar':
	    $controllerUsuario_sistema->salvar($_POST);
	    break;
	case 'inativar':
	    $id = $_GET['usucpf'];
	    $controllerUsuario_sistema->inativar($id);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <?php echo $simec->hidden('acao', 'acao', ''); ?>

    <input type="hidden" name="usucpf" id="usucpf" value="<?php echo $usucpf ?>"/>
      <?php echo $simec->hidden('usucpf', 'usucpf', $modelUsuario_sistema->usucpf ); ?>
    <input type="hidden" name="sisid" id="sisid" value="<?php echo $sisid ?>"/>
      <?php echo $simec->hidden('sisid', 'sisid', $modelUsuario_sistema->sisid ); ?>
          <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) Usuario_sistema(a)</h3>
                </div>
                <div class="ibox-content">
              <?php     
                     echo $simec->input('susstatus', 'susstatus', $objUsuario_sistema->susstatus , array('required'=>'required', 'maxlength'=>'1') );    
                     echo $simec->input('pflcod', 'pflcod', $objUsuario_sistema->pflcod , array('required'=>'required', 'class' => 'inteiro') );    
                     echo $simec->input('susdataultacesso', 'susdataultacesso', $objUsuario_sistema->susdataultacesso , array('required'=>'required') );    
                     echo $simec->input('suscod', 'suscod', $objUsuario_sistema->suscod , array('required'=>'required', 'maxlength'=>'1') );
                ?>
              </div>
                <div class="ibox-footer">
                    <div>
                        <button type="button" class="btn btn-success salvar"> <i class="fa fa-save"></i>Salvar </button>
                    </div>
                </div>
 
           </div>
</form>