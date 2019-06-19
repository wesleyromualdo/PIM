<?php
/**
 * View da representando a tabela etapa_servico
 *
 * @category visao
 * @package  A1
 * @version $Id$
 * @author   Adminstrador de sistema <simec@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-06-2019
 * @link     no link
 */
$controllerEtapa_servico     = new EtapaServico_Controller_Etapa_servico();

switch ($_POST['acao']) {
	case 'salvar':
	    $controllerEtapa_servico->salvar($_POST);
	    break;
	case 'inativar':
	    $id = $_GET['co_etapa_servico'];
	    $controllerEtapa_servico->inativar($id);
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <?php echo $simec->hidden('acao', 'acao', ''); ?>

    <input type="hidden" name="co_etapa_servico" id="co_etapa_servico" value="<?php echo $_GET['co_etapa_servico'] ?>"/>
          <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) Etapa_servico(a)</h3>
                </div>
                <div class="ibox-content">
              <?php     
                     echo $simec->input('nu_codigo_etapa_servico', 'nu_codigo_etapa_servico', $objEtapa_servico->nu_codigo_etapa_servico , array('maxlength'=>'20') );    
                     echo $simec->input('ds_etapa_servico', 'ds_etapa_servico', $objEtapa_servico->ds_etapa_servico , array('maxlength'=>'255') );    
                     echo $simec->input('co_etapa_servico_pai', 'co_etapa_servico_pai', $objEtapa_servico->co_etapa_servico_pai , array('required'=>'required', 'class' => 'inteiro') );    
                     echo $simec->cpf('nu_cpf_inclusao', 'nu_cpf_inclusao', $objEtapa_servico->nu_cpf_inclusao , array('maxlength'=>'11') );    
                     echo $simec->input('dt_inclusao', 'dt_inclusao', $objEtapa_servico->dt_inclusao  );    
                     echo $simec->input('co_status', 'co_status', $objEtapa_servico->co_status , array('class' => 'inteiro') );
                ?>
              </div>
                <div class="ibox-footer">
                    <div>
                        <button type="submit" class="btn btn-success salvar"><i class="fa fa-save"></i> Salvar</button>
                    </div>
                </div>
 
           </div>
</form>