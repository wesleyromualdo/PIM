<?php
/**
 * Tela de dados da prefeitura
 *
 * @category visao
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
$renderEntidade                       = new Par3_Controller_Entidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade      = new Par3_Model_InstrumentoUnidadeEntidade();

$inuid = $_REQUEST['inuid'];
$tenid = $_REQUEST['tenid'] = Par3_Model_InstrumentoUnidadeEntidade::PREFEITURA;

switch ($_REQUEST['req']) {
	case 'salvar':
	    $controllerInstrumentoUnidadeEntidade->salvarEntidade($_POST);
	    break;
	case 'atualizaDados':
	    $controllerInstrumentoUnidadeEntidade->atualizaDadosSAPE($_REQUEST);
	    break;
	default:
        $objPessoaJuridica = $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($inuid, $tenid);
        $objEndereco       = new Par3_Model_Endereco($objPessoaJuridica->endid);
        $objmunicipio = $controllerInstrumentoUnidadeEntidade->buscaMunicipio($objPessoaJuridica->entcnpj,'prefeitura',$_GET['inuid']);
        $arrParamHistorico              = array();
        $arrParamHistorico['inuid']     = $inuid;
        $arrParamHistorico['tenid']     = $tenid;
        $arrParamHistorico['booCPF']    = false;

        $controllerInstrumentoUnidadeEntidade->atualizaDadosSAPE(array( 'tenid'=> $tenid, 'inuid'=> $inuid, 'menu' => 'prefeitura', 'entid' => $objPessoaJuridica->entid, $_REQUEST));

        $display = Array(
        	'entinscricaoestadual' => 'N',
            'entsigla' => 'N',
        );

        $disabled = 'disabled="disabled"';
//         if ($objPessoaJuridica->entid) {
//             $disabledForm = Array(
//                 'entcnpj'              => 'disabled="disabled"',
//                 'entnome'              => 'disabled="disabled"',
//                 'entrazaosocial'       => 'disabled="disabled"',
//                 'entemail'             => 'disabled="disabled"',
//                 'enttelefonecomercial' => 'disabled="disabled"',
//             );
//         } else {
//             $disabledForm = $disabled;
//         }
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
                <div class="col-md-10">
        	       <h3 id="entidade">Dados da Prefeitura</h3>
                </div>
            	<div class="col-md-2 text-right">
                    <?php if($disabled == ''){?>
            		  <button type="button" class="btn btn-success atualizar" tenid=<?php echo $tenid; ?> ><i class="fa fa-refresh"></i> Atualizar</button>
            		<?php $disabled = 'disabled';?>
                    <?php }?>
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
    	<div class="ibox-content">
    		<?php $renderEntidade->formPessoaJuridica($disabled, $objPessoaJuridica, null, $display);?>
    	</div>
    	<div class="ibox-title">
        	<h3>Endereço da Prefeitura</h3>
    	</div>
    	<div class="ibox-content">
    		<?php $renderEntidade->formEnderecoEntidade($disabled, $objEndereco,$objmunicipio);?>
    	</div>
    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-4 text-left">
					<button disabled type="button" class="btn btn-success" >Anterior</button>
				</div>
				<div class="col-lg-4 text-center" >
                <?php if (!Par3_Model_UsuarioResponsabilidade::perfilConsulta()) : ?>
                    <?php $attr = Par3_Model_UsuarioResponsabilidade::permissaoEscrita($disabled, $disabledForm); ?>
					<button <?php echo $attr; ?> type="submit" class="btn btn-success salvar" ><i class="fa fa-save"></i> Salvar prefeitura</button>
                <?php endif; ?>
				</div>
				<div class="col-lg-4 text-right">
					<button type="button" class="btn btn-success next" >Próximo</button>
				</div>
			</div>
    	</div>
    </div>
</form>
<!--
<div class="ibox">
	<div class="ibox-title">
	    <h3>Prefeitura - Histórico Modificações</h3>
	</div>
    <?php //$controllerInstrumentoUnidadeEntidade->formHistorico($arrParamHistorico); ?>
</div>
-->
<script>
$(document).ready(function()
{
	$('.next').click(function(){
		var url = window.location.href.toString().replace('prefeitura', 'prefeito');

		if (url.indexOf('prefeito'))
			url = url + '&menu=prefeito';

		window.location.href = url;
	});
});
</script>