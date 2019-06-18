<?php
/**
 * Tela de dados da SECRETARIA DE EDUCACAO
 *
 * @category visao
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-10-2015
 * @link     no link
 */
$renderEntidade                       = new Par3_Controller_Entidade();
$controleUnidade         			  = new Par3_Controller_InstrumentoUnidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade      = new Par3_Model_InstrumentoUnidadeEntidade();

$inuid = $_REQUEST['inuid'];
$tenid = $_REQUEST['tenid'] = Par3_Model_InstrumentoUnidadeEntidade::SECRETARIA_EDUCACAO;
$itrid = $controleUnidade->pegarItrid($inuid);

$objPessoaJuridica = $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($inuid, $tenid);

switch($_REQUEST['req']){
	case 'salvar':
	    $controllerInstrumentoUnidadeEntidade->salvarInformacoesSecretaria($_POST);
	    break;
	case 'atualizaDados':
	    $controllerInstrumentoUnidadeEntidade->atualizaDadosSAPE($_REQUEST);
	    break;
	default:
		$objPessoaJuridica 				= $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($inuid, $tenid);
		$objEndereco       				= new Par3_Model_Endereco($objPessoaJuridica->endid);
        $objmunicipio = $controllerInstrumentoUnidadeEntidade->buscaMunicipio($objPessoaJuridica->entcnpj, 'secretaria');


		if($objEndereco->endmunicipio != null) {

            $objmunicipio['mundsc'] = $objEndereco->endmunicipio;
        }

//69.935-000
        $arrParamHistorico              = array();
        $arrParamHistorico['inuid']     = $inuid;
        $arrParamHistorico['tenid']     = $tenid;
        $arrParamHistorico['booCPF']    = false;

		$disabledForm = $disabled;

        if($itrid != '2'){
            $controllerInstrumentoUnidadeEntidade->atualizaDadosSAPE(array( 'tenid'=> $tenid, 'inuid'=> $inuid, 'entid' => $objPessoaJuridica->entid));

            $disabled = 'disabled="disabled"';
            if($objPessoaJuridica->entid)
            {
                $disabledForm = Array(
        	        'entcnpj'              => $disabled,
        	        'entnome'              => $disabled,
        	        'entemail'             => $disabled,
        	        'enttelefonecomercial' => $disabled,
                );
            }
        }
	    break;
}

?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid?>"/>
    <input type="hidden" name="req" value="salvar"/>
    <input type="hidden" name="tenid" id="tenid" value="<?php echo $tenid; ?>"/>
    <input type="hidden" name="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
    <div class="ibox ">
    	<div class="ibox-title">
            <div class="row">
                <div class="col-md-10">
        	       <h3 id="entidade">Dados da Secretaria de Educação</h3>
                </div>
            </div>
    	</div>
		<?php if ( $itrid != '2' ) { ?>
    	<div class="ibox-content">
    		<?php
			$file = 'msgCadastroEstadual.php';
			$pastaMensagem = 'par3/modulos/principal/mensagem/';
			include APPRAIZ.$pastaMensagem.$file;
    		?>
		</div>
		<?php } ?>
    	<div class="ibox-content">
    		<?php
			$renderEntidade->formSecretaria($disabledForm, $objPessoaJuridica);?>
    	</div>
    	<div class="ibox-title">
    	    <h3>Endereço da Secretaria de Educação</h3>
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
					<button <?php echo $attr; ?> type="submit" class="btn btn-success salvar"><i class="fa fa-save"></i> Salvar secretaria</button>
                <?php endif; ?>
				</div>
				<div class="col-lg-4 text-right">
					<button type="button" class="btn btn-success next" >Próximo</button>
				</div>
			</div>
    	</div>
    </div>
</form>
<?php if( $itrid === '2' ) { ?>
<div class="ibox collapsed">
	<div class="ibox-title">
	    <h5>Histórico Modificações</h5>
        <div class="ibox-tools">
            <a class="collapse-link">
                <i class="fa fa-chevron-down"></i>
            </a>
        </div>
	</div>
    <?php $controllerInstrumentoUnidadeEntidade->formHistorico($arrParamHistorico); ?>
</div>
<?php } ?>
<script>
$(document).ready(function()
{
    $('.collapse-link').click( function() {

        var ibox = $(this).closest('div.ibox');
        var button = $(this).find('i');
        var content = ibox.find('div.ibox-content');
        content.slideToggle(200);
        button.toggleClass('fa-chevron-up').toggleClass('fa-chevron-down');
        ibox.toggleClass('').toggleClass('border-bottom');
        setTimeout(function () {
            ibox.resize();
            ibox.find('[id^=map-]').resize();
        }, 50);
    });

	$('.previous').click(function(){
		var url = window.location.href.toString().replace('secretaria', 'prefeito');
		window.location.href = url;
	});

	$('.next').click(function(){
		var url = window.location.href.toString().replace('secretaria', 'dirigente');
		window.location.href = url;
	});
});
</script>