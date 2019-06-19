<?php
/**
 * Tela de dados da dirigente
 *
 * @category visao
 * @package  A1
 * @author   Fellipe Esteves <fellipesantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
$renderDirigente                      = new Par3_Controller_Entidade();
$controleUnidade         			  = new Par3_Controller_InstrumentoUnidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade      = new Par3_Model_InstrumentoUnidadeEntidade();

$inuid = $_REQUEST['inuid'];
$itrid = $controleUnidade->pegarItrid($inuid);

if ($itrid === '2') {
    $tenid = Par3_Model_InstrumentoUnidadeEntidade::DIRIGENTE;
} else {
    $tenid = Par3_Model_InstrumentoUnidadeEntidade::SECRETARIO_ESTADUAL_EDUCACAO;
}

switch ($_REQUEST['req']) {
	case 'salvarDirigente':
		$_POST['inuid'] = $inuid;
		$_POST['tenid'] = $tenid;
	    $controllerInstrumentoUnidadeEntidade->salvarInformacoesDirigente($_POST);
	    break;
	default:
        $objPessoaFisica      = $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($inuid, $tenid);
        $arrPost              = array();
        $arrPost['inuid']     = $inuid;
        $arrPost['tenid']     = $tenid;

//         $controllerInstrumentoUnidadeEntidade->atualizaDadosEXSAPE(array( 'tenid'=> $tenid, 'inuid'=> $inuid, 'entid' => $objPessoaJuridica->entid));

        if ($itrid != '2'){
            $listaHistoricoDirigentes = $modelInstrumentoUnidadeEntidade->carregaArrayHistoricoEntidade($arrPost);

            $_REQUEST['tenid'] = $tenid;

            $disabled = 'disabled="disabled"';

            $display = Array(
            	'entdtnascimento' => 'N',
                'enttelefonecelular' => 'N',
                'enttelefonefax' => 'N',
            );

            $arrEsconde = array(
                'entnivelensino' => true,
                'entempoatuacao' => true,
                'entcursomec' => true,
                'entcursomecdescricao' => true,
            );
        }
	    break;
}
?>
<form method="post" name="formulario" id="formulario-dirigente" class="form form-horizontal" >

    <input type="hidden" name="inuid"   id="inuid" value="<?php echo $inuid?>"/>
    <input type="hidden" name="req"     value="salvarDirigente"/>
    <input type="hidden" name="tenid"   value="<?php echo $tenid; ?>"/>
	<input type="hidden" required />

    <div class="ibox">
    	<div class="ibox-title">
    		<div class="row">
                <div class="col-md-10">
        	       <h3 id="entidade">Dados do(a) <?php echo ($itrid === '2') ? 'Dirigente Municipal' : 'Secretário(a) Estadual '; ?> de Educação</h3>
                </div>
            </div>
    	</div>
    		<?php
    		if( $itrid === '1' )//{
				$file = "msgCadastroEstadual.php";
			else
				$file = "msgCadastroDirigenteMunicipal.php";

			$pastaMensagem = 'par3/modulos/principal/mensagem/';
            ?>
    	<div class="ibox-content">
			<?php include APPRAIZ.$pastaMensagem.$file; ?>
		</div>
			<?php
// 			}
    		?>
    	<div class="ibox-content">
    		<?php $renderDirigente->formPessoaFisica('disabled', $objPessoaFisica, null, $display);?>
    	</div>
    	<?php
    		if( $itrid === '2' ){
    	?>
    	<div class="ibox-title">
    		<div class="row">
                <div class="col-md-12">
        	       <h3>Informações Adicionais</h3>
                </div>
            </div>
    	</div>
    	<div class="ibox-content">
    		<?php $renderDirigente->formDirigente($disabled, $objPessoaFisica, $arrEsconde);?>
    	</div>
    	<?php
            }
    	?>
    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-4 text-left">
					<button type="button" class="btn btn-success previous" >Anterior</button>
				</div>
				<div class="col-lg-4 text-center">
                <?php if (!Par3_Model_UsuarioResponsabilidade::perfilConsulta()) : ?>
                    <?php $attr = Par3_Model_UsuarioResponsabilidade::permissaoEscrita($disabled, $disabled); ?>
					<button id="salvar-dirigente" <?php echo $attr; ?> type="button" class="btn btn-success salvar" ><i class="fa fa-save"></i> Salvar dirigente</button>
                <?php endif; ?>
				</div>
				<div class="col-lg-4 text-right">
					<button type="button" class="btn btn-success next" >Próximo</button>
				</div>
			</div>
    	</div>
    </div>
	<?php if (count($listaHistoricoDirigentes) > 0 && is_array($listaHistoricoDirigentes)): ?>
    <div class="ibox">
    	<div class="ibox-title">
    	    <h3>Histórico de Modificações</h3>
    	</div>
    	<div class="ibox-content">
    		<table class="table table-hover dataTable">
    			<thead>
    				<tr>
    					<th>CPF</th>
    					<th>Nome</th>
    					<th>Email</th>
    					<th>Inicio do mandato</th>
    					<th>Fim do Mandato</th>
    					<th>Data de Inativação</th>
    				</tr>
    			</thead>
    			<?php foreach ($listaHistoricoDirigentes as $historico) : ?>
    			<tr>
    				<td><?php echo formatar_cpf($historico['entcpf']); ?></td>
    				<td><?php echo $historico['entnome']; ?></td>
    				<td><?php echo $historico['entemail']; ?></td>
                    <td><?php echo formata_data($historico['entdt_inicio_mandato']); ?></td>
                    <td><?php echo formata_data($historico['entdt_fim_mandato']); ?></td>
                    <td><?php echo formata_data($historico['entdtinativacao']); ?></td>
    			</tr>
    			<?php endforeach;?>
    		</table>
    	</div>
    </div>
    <?php endif; ?>
</form>
<script>
	$(document).ready(function() {
		if ($('#entcursomec:checked').val() == 't') {
			$('#entcursomecdescricao').closest('.form-group').removeClass('hidden');
			$('#entcursomecdescricao').attr('required', 'required');
		} else {
			$('#entcursomecdescricao').closest('.form-group').addClass('hidden');
			$('#entcursomecdescricao').val('');
			$('#entcursomecdescricao').removeAttr('required');
		}

		$('input[name="entcursomec"]').change(function() {
			if ($(this).val() == 't') {
				$('#entcursomecdescricao').closest('.form-group').removeClass('hidden');
				$('#entcursomecdescricao').attr('required', 'required');
			} else {
				$('#entcursomecdescricao').closest('.form-group').addClass('hidden');
				$('#entcursomecdescricao').val('');
				$('#entcursomecdescricao').removeAttr('required');
			}
		})

		$('.previous').click(function(){
			var url = window.location.href.toString().replace('dirigente', 'secretaria');
			window.location.href = url;
		});

		$('.next').click(function(){
			var url = window.location.href.toString().replace('dirigente', 'equipe');
			window.location.href = url;
		});

		$("#salvar-dirigente").click(function() {
            $('.loading-dialog-par3').show();
            $("#formulario-dirigente").submit();
		});
	});
</script>
