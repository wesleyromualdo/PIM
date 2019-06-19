<?php
/**
 * Tela de dados do prefeito
 *
 * @category visao
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01/10/2015
 * @link     no link
 */
$renderEntidade                       = new Par3_Controller_Entidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade      = new Par3_Model_InstrumentoUnidadeEntidade();

$inuid = $_REQUEST['inuid'];
$tenid = $_REQUEST['tenid'] = Par3_Model_InstrumentoUnidadeEntidade::PREFEITO;

switch($_REQUEST['req']){
	case 'salvar':
	    $controllerInstrumentoUnidadeEntidade->salvarEntidade($_POST);
        die();
	    break;
	default:
        $objPessoaFisica = $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($inuid, $tenid);
        $objEndereco     = new Par3_Model_Endereco($objPessoaFisica->endid);

        $objmunicipio = $controllerInstrumentoUnidadeEntidade->buscaMunicipio($objPessoaFisica->entcpf,'prefeito');

        $arrParamHistorico              = array();
        $arrParamHistorico['inuid']     = $inuid;
        $arrParamHistorico['tenid']     = $tenid;
        $arrParamHistorico['booCPF']    = true;

//         $controllerInstrumentoUnidadeEntidade->atualizaDadosEXSAPE(array( 'tenid'=> $tenid, 'inuid'=> $inuid, 'entid' => $objPessoaJuridica->entid));

	    $disabled = 'disabled="disabled"';

        $display = Array(
        	'entdtnascimento' => 'N',
            'enttelefonecelular' => 'N',
            'enttelefonefax' => 'N',
        );
	    break;
}
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">

    <input type="hidden" name="inuid"   id="inuid"  value="<?php echo $inuid?>"/>
    <input type="hidden" name="req"                 value="salvar"/>
    <input type="hidden" name="tenid"   id="tenid"  value="<?php echo $tenid; ?>"/>
    <input type="hidden" name="menu"                value="<?php echo $_REQUEST['menu']; ?>"/>

    <div class="ibox">
    	<div class="ibox-title">
            <div class="row">
            	<div class="col-md-12" >
        	       <h3 id="entidade">Dados do(a) Prefeito(a)</h3>
                </div>
    		</div>
    	</div>
    	<div class="ibox-content">
    		<?php
    		if( $itrid === '1' )
				$file = "msgCadastroEstadual.php";
			else
				$file = "msgCadastroMunicipal.php";

			$pastaMensagem = 'par3/modulos/principal/mensagem/';
			include APPRAIZ.$pastaMensagem.$file;
    		?>
		</div>
    	<div class="ibox-content" id="div_prefeito">
    		<?php $renderEntidade->formPessoaFisica($disabled, $objPessoaFisica, null, $display);?>
    	</div>
    	<div class="ibox-title">
        	<h3>Endereço do(a) Prefeito(a)</h3>
    	</div>
    	<div class="ibox-content">
    		<?php $renderEntidade->formEnderecoEntidade($disabled, $objEndereco,$objmunicipio);?>
    	</div>
    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-4 text-left">
					<button type="button" class="btn btn-success previous" >Anterior</button>
				</div>
				<div class="col-lg-4 text-center">
                <?php if (!Par3_Model_UsuarioResponsabilidade::perfilConsulta()) : ?>
                    <?php $attr = Par3_Model_UsuarioResponsabilidade::permissaoEscrita($disabled, $disabledForm); ?>
					<button <?php echo $attr ?> type="submit" class="btn btn-success salvar" ><i class="fa fa-save"></i> Salvar prefeito</button>
                <?php endif; ?>
				</div>
				<div class="col-lg-4 text-right">
					<button type="button" class="btn btn-success next" >Próximo</button>
				</div>
			</div>
    	</div>
    </div>
</form>
<div class="ibox">
	<div class="ibox-title">
	    <h3>Histórico Modificações</h3>
	</div>
    <?php $controllerInstrumentoUnidadeEntidade->formHistorico($arrParamHistorico); ?>
</div>
<script>
$(document).ready(function()
{
	$('.previous').click(function(){
		var url = window.location.href.toString().replace('prefeito', 'prefeitura');
		window.location.href = url;
	});

	$('.next').click(function(){
		var url = window.location.href.toString().replace('prefeito', 'secretaria');
		window.location.href = url;
	});
});
</script>
