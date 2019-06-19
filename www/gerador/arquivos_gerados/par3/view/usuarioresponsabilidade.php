<?php
/**
 * View da representando a tabela usuarioresponsabilidade
 *
 * @category visao
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-09-2016
 * @link     no link
 */$controllerUsuarioresponsabilidade     = new Par3_Controller_Usuarioresponsabilidade();
$modelUsuarioresponsabilidade          = new Par3_Model_Par3_Usuarioresponsabilidade("$_GET['rpuid']"});

switch ($_POST['acao']) {
	case 'salvar':
	    $controllerUsuarioresponsabilidade->salvar($_POST);
	    break;
	case 'inativar':
	    $id = $_GET['rpuid'];
	    $controllerUsuarioresponsabilidade->inativar($id);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <?php echo $simec->hidden('acao', 'acao', ''); ?>

    <input type="hidden" name="rpuid" id="rpuid" value="<?php echo $rpuid ?>"/>
      <?php echo $simec->hidden('rpuid', 'rpuid', $modelUsuarioresponsabilidade->rpuid ); ?>
          <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) Usuarioresponsabilidade(a)</h3>
                </div>
                <div class="ibox-content">
              <?php     
                     echo $simec->input('prgid', 'prgid', $objUsuarioresponsabilidade->prgid , array('required'=>'required', 'class' => 'inteiro') );    
                     echo $simec->input('estuf', 'estuf', $objUsuarioresponsabilidade->estuf , array('required'=>'required', 'maxlength'=>'2') );    
                     echo $simec->input('entid', 'entid', $objUsuarioresponsabilidade->entid , array('required'=>'required', 'class' => 'inteiro') );    
                     echo $simec->input('muncod', 'muncod', $objUsuarioresponsabilidade->muncod , array('required'=>'required', 'maxlength'=>'7') );    
                     echo $simec->input('rpudata_inc', 'rpudata_inc', $objUsuarioresponsabilidade->rpudata_inc , array('required'=>'required') );    
                     echo $simec->input('rpustatus', 'rpustatus', $objUsuarioresponsabilidade->rpustatus , array('required'=>'required', 'maxlength'=>'1') );    
                     echo $simec->cpf('usucpf', 'usucpf', $objUsuarioresponsabilidade->usucpf , array('required'=>'required', 'maxlength'=>'11') );    
                     echo $simec->input('pflcod', 'pflcod', $objUsuarioresponsabilidade->pflcod , array('required'=>'required', 'class' => 'inteiro') );
                ?>
              </div>
                <div class="ibox-footer">
                    <div>
                        <button type="button" class="btn btn-success salvar"> <i class="fa fa-save"></i>Salvar </button>
                    </div>
                </div>
 
           </div>
</form>