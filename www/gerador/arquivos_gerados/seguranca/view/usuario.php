<?php
/**
 * View da representando a tabela usuario
 *
 * @category visao
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-08-2016
 * @link     no link
 */$controllerUsuario     = new Par3_Controller_Usuario();
$modelUsuario          = new Par3_Model_Seguranca_Usuario("$_GET['usucpf']"});

switch ($_POST['acao']) {
	case 'salvar':
	    $controllerUsuario->salvar($_POST);
	    break;
	case 'inativar':
	    $id = $_GET['usucpf'];
	    $controllerUsuario->inativar($id);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <?php echo $simec->hidden('acao', 'acao', ''); ?>

    <input type="hidden" name="usucpf" id="usucpf" value="<?php echo $usucpf ?>"/>
      <?php echo $simec->hidden('usucpf', 'usucpf', $modelUsuario->usucpf ); ?>
          <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) Usuario(a)</h3>
                </div>
                <div class="ibox-content">
              <?php     
                     echo $simec->input('regcod', 'regcod', $objUsuario->regcod , array('required'=>'required', 'maxlength'=>'2') );    
                     echo $simec->input('usunome', 'usunome', $objUsuario->usunome , array('maxlength'=>'100') );    
                     echo $simec->email('usuemail', 'usuemail', $objUsuario->usuemail , array('maxlength'=>'100') );    
                     echo $simec->input('usustatus', 'usustatus', $objUsuario->usustatus , array('required'=>'required', 'maxlength'=>'1') );    
                     echo $simec->input('usufoneddd', 'usufoneddd', $objUsuario->usufoneddd , array('required'=>'required', 'maxlength'=>'2') );    
                     echo $simec->input('usufonenum', 'usufonenum', $objUsuario->usufonenum , array('required'=>'required', 'maxlength'=>'10') );    
                     echo $simec->input('ususenha', 'ususenha', $objUsuario->ususenha , array('maxlength'=>'100') );    
                     echo $simec->data('usudataultacesso', 'usudataultacesso', $objUsuario->usudataultacesso , array('required'=>'required') );    
                     echo $simec->input('usunivel', 'usunivel', $objUsuario->usunivel , array('required'=>'required', 'class' => 'inteiro') );    
                     echo $simec->input('usufuncao', 'usufuncao', $objUsuario->usufuncao , array('required'=>'required', 'maxlength'=>'100') );    
                     echo $simec->input('ususexo', 'ususexo', $objUsuario->ususexo , array('required'=>'required', 'maxlength'=>'1') );    
                     echo $simec->input('orgcod', 'orgcod', $objUsuario->orgcod , array('required'=>'required', 'maxlength'=>'5') );    
                     echo $simec->input('unicod', 'unicod', $objUsuario->unicod , array('required'=>'required', 'maxlength'=>'5') );    
                     echo $simec->radio('usuchaveativacao', 'usuchaveativacao', $objUsuario->usuchaveativacao , array('required'=>'required') , array('type' => 'radio radio-info radio', 'style' => 'inline') );    
                     echo $simec->input('usutentativas', 'usutentativas', $objUsuario->usutentativas , array('required'=>'required') );    
                     echo $simec->textArea('usuprgproposto', 'usuprgproposto', $objUsuario->usuprgproposto , array('required'=>'required', 'maxlength'=>'1000') );    
                     echo $simec->textArea('usuacaproposto', 'usuacaproposto', $objUsuario->usuacaproposto , array('required'=>'required', 'maxlength'=>'1000') );    
                     echo $simec->textArea('usuobs', 'usuobs', $objUsuario->usuobs , array('required'=>'required') );    
                     echo $simec->input('ungcod', 'ungcod', $objUsuario->ungcod , array('required'=>'required', 'maxlength'=>'6') );    
                     echo $simec->input('usudatainc', 'usudatainc', $objUsuario->usudatainc , array('required'=>'required') );    
                     echo $simec->radio('usuconectado', 'usuconectado', $objUsuario->usuconectado , array('required'=>'required') , array('type' => 'radio radio-info radio', 'style' => 'inline') );    
                     echo $simec->input('pflcod', 'pflcod', $objUsuario->pflcod , array('required'=>'required', 'class' => 'inteiro') );    
                     echo $simec->input('suscod', 'suscod', $objUsuario->suscod , array('required'=>'required', 'maxlength'=>'1') );    
                     echo $simec->input('usunomeguerra', 'usunomeguerra', $objUsuario->usunomeguerra , array('required'=>'required', 'maxlength'=>'20') );    
                     echo $simec->input('orgao', 'orgao', $objUsuario->orgao , array('required'=>'required', 'maxlength'=>'100') );    
                     echo $simec->input('muncod', 'muncod', $objUsuario->muncod , array('required'=>'required', 'maxlength'=>'7') );    
                     echo $simec->data('usudatanascimento', 'usudatanascimento', $objUsuario->usudatanascimento , array('required'=>'required') );    
                     echo $simec->input('usudataatualizacao', 'usudataatualizacao', $objUsuario->usudataatualizacao , array('required'=>'required') );    
                     echo $simec->input('entid', 'entid', $objUsuario->entid , array('required'=>'required', 'class' => 'inteiro') );    
                     echo $simec->input('tpocod', 'tpocod', $objUsuario->tpocod , array('required'=>'required', 'maxlength'=>'1') );    
                     echo $simec->input('carid', 'carid', $objUsuario->carid , array('required'=>'required', 'class' => 'inteiro') );
                ?>
              </div>
                <div class="ibox-footer">
                    <div>
                        <button type="button" class="btn btn-success salvar"> <i class="fa fa-save"></i>Salvar </button>
                    </div>
                </div>
 
           </div>
</form>