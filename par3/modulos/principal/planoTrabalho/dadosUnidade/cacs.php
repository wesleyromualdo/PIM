<?php

/**
 * Tela CACS
 *
 * @category visao
 * @package  A1
 * @author   Fellipe Esteves <fellipesantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
$inuid = $_REQUEST['inuid'];

$modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
$modelCACS = new Par3_Model_CACS();

$itrid = $modelInstrumentoUnidade->itrid;

if( $itrid === '1' )
	$value = $modelInstrumentoUnidade->estuf;
elseif( $itrid === '2' )
	$value = $modelInstrumentoUnidade->muncod;
else 
	$value = $modelInstrumentoUnidade->estuf;

$pastaMensagem = 'par3/modulos/principal/mensagem/';

$listaCACS = $modelCACS->listarConselheiros($value, $inuid);



$perfil = pegaPerfil($_SESSION['usucpf']);

$arrPerfilPermitido = array (Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO_DESENVOLVEDOR, Par3_Model_UsuarioResponsabilidade::CONTROLE_USUARIO);

$usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();
$habilResetaCacs = $usuarioResponsabilidade->possuiPerfil($arrPerfilPermitido);

?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <div class="ibox">
    	<div class="ibox-title">
    	    <h3>Conselho de Acompanhamento e Controle Social (CACS-Fundeb)</h3>
    	</div>
    	<div class="ibox-content">
			<?php if( $itrid === '1' ) : ?>
			<?php include_once APPRAIZ . $pastaMensagem . "msgCACSEstadual.php"; ?>
			<?php else : ?>
			<?php include_once APPRAIZ . $pastaMensagem . "msgCACSMunicipal.php"; ?>
			<?php endif; ?>
		</div>
	    <?php if (count($listaCACS) > 0 && is_array($listaCACS)): ?>
    		<div class="ibox-title" style="margin-bottom: 10px;">
	    	    <h3>Situação do Conselho: <?php echo $listaCACS[0]['sit_conselheiro']; ?></h3>
    		</div>
	    	<div class="ibox-content">
	    		<table class="table table-hover dataTable">
	    			<thead>
	    				<tr>
                            <?php if( $habilResetaCacs ) : ?>
	    					    <th width="16%">Ação</th>
                            <?php endif;?>
	    					<th width="16%">CPF</th>
	    					<th width="25%">Nome</th>
	    					<th width="25%">Email</th>
	    					<th width="12%">Situação</th>
	    					<th width="12%">Atuação</th>
	    					<th width="12%">Vinculo</th>
	    					<th width="10%">Cargo</th>
	    				</tr>
	    			</thead>
	    			<?php foreach ($listaCACS as $cacs) : ?>
	    			<tr>
                        <?php if( $habilResetaCacs ) : ?>
                            <td><button class="btn-alteraSenhaCacs btn-primary btn-sm glyphicon glyphicon-pencil" id="alteraSenha" value="<?= $cacs['cpf_conselheiro']?>"></button></td>
                        <?php endif;?>
	    				<td><?php echo formatar_cpf($cacs['cpf_conselheiro']); ?></td>
	    				<td><?php echo $cacs['no_conselheiro']; ?></td>
	    				<td><?php echo $cacs['email_conselheiro']; ?></td>
	    				<td><?php echo $cacs['sit_conselheiro']; ?></td>
	    				<td><?php echo $cacs['ds_segmento']; ?></td>
	    				<td><?php echo $cacs['tp_membro']; ?></td>
	    				<td><?php echo $cacs['ds_funcao']; ?></td>
	    			</tr>
	    			<?php endforeach;?>
	    		</table>
	    	</div>
	    <?php else: ?>
	    <div class="ibox-content">
			<?php
				switch ($itrid) {
					case '1':
						$descricaoEsfera = 'estado';
						break;
					case '2':
						$descricaoEsfera = 'município';
						break;
					case '3':
						$descricaoEsfera = 'distrito';
						break;
				}
			?>
	    	<div class="alert alert-warning">Nenhum conselheiro vinculado para este <?php echo $descricaoEsfera; ?>.</div>
	    </div>
	    <?php endif; ?>
		<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-6 text-left">
					<button type="button" class="btn btn-success previous" >Anterior</button>
				</div>
				<div class="col-lg-6 text-right">
					<button type="button" class="btn btn-success next" >Próximo</button>
				</div>
			</div>
		</div>
    </div>
</form>

<script>
    jQuery(function() {

        $(window).on('beforeunload',function(){
            $('.loading-dialog-par3').show();
        });
        $(window).on('click','#btn-pesquisar #limpar',function(){
            $('.loading-dialog-par3').show();
        });
        $(window).load(function(){
            $('.loading-dialog-par3').hide();
        });
        //renderizeMunicipio();

        if(jQuery('[name="estuf"]').val() != '') {
            carregarMunicipio(jQuery('[name="estuf"]').val(), <?=$_REQUEST['muncod']?>);
        }

        jQuery("input:radio[name=itrid], select[name=estuf]").change(function() {
            //renderizeMunicipio();
        });

        jQuery('select[name=estuf]').change(function(){
            carregarMunicipio(this.value);
        });

        function renderizeMunicipio() {
            var filtroMunicipio = jQuery("select[name=muncod]").parents("div.form-group");
            if (jQuery('input:radio[name=itrid]:checked').val() === '1' || !jQuery('select[name=estuf]').val()) {
                filtroMunicipio.slideUp();
            } else {
                filtroMunicipio.slideDown();
            }
        }
    });

	$(document).ready(function()
	{
		$('.previous').click(function(){
			var url = window.location.href.toString().replace('comite', 'nutricionista');
			window.location.href = url;
		});

		$('.next').click(function(){
			var url = window.location.href.toString().replace('comite', 'conselho');
			window.location.href = url;
		});

        $('.btn-alteraSenhaCacs').click(function(evt){
            evt.preventDefault();
            var cpf = $(this).val();
            var msg = "Tem certeza que deseja resetar a senha deste usuario?";

            swal({
                    title: "Confirmar",
                    text: msg,
                    html: true,
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: true
                }, function (isConfirm) {
                    if (isConfirm) {
                        $('.loading-dialog-par3').show();
                        var action = '&requisicao=resetarSenhaCacs&cpf=' + cpf;
                        window.location.href = window.location.href + action;
                    } else {
                        return false;
                    }
                }
            );
        });
	});
</script>