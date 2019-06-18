<?php 
include_once APPRAIZ .'par3/classes/controller/Aviso_conteudo.class.inc';

$controller = new Par3_Controller_Aviso_conteudo();

if ( $_REQUEST['requisicao'] ){
    switch ($_REQUEST['requisicao']) {
        case 'carregaMunicipiosAviso':
            ob_clean();
            print simec_json_encode($controller->{$_REQUEST['requisicao']}( $_REQUEST['estuf'] ));
            die;
            break;
        default:
            $controller->{$_REQUEST['requisicao']}( $_REQUEST );
    }
}

$dados = $controller->carregarAvisoConteudo();
extract($dados);

$url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cadastroAviso';

$arrEstado = Par3_Model_Aviso_conteudo::getEstado();
$arrMunicipio = Par3_Model_Aviso_conteudo::getEstadoMunicipio();

?>
<style>
.modal-backdrop{
    z-index: -1 !important;
}
</style>
<div class="ibox">
	<div class="ibox-title">
		<div class="row">
			<div class="col-md-12">
				<h2 class="center">Cadastro de aviso</h2>
			</div>
		</div>
	</div>
	<?php echo $simec->tab ( criaAbaPainelAviso(), $url );?>
	<div class="ibox-content">
		<div class="row">
			<form name="formAviso" id="formAviso" class="form form-horizontal" method="post">
				<input type="hidden" name="requisicao" id="requisicao" value="">
				<input type="hidden" name="actid" id="actid" value="<?php echo $actid; ?>">
    			<?php echo $simec->input('acttitulo', 'Título', $acttitulo, array('maxlength' => '255', 'required'), array('input-size' => '6')); ?>
    			<?php echo $simec->periodo('act', 'Período do aviso', array($actinicio, $actfim), array('class' => 'datemask', 'required'), array('input-size' => '6')) ?>
    			<?php 
    			$sql = "SELECT
							pflcod as codigo,
							s.sisdiretorio || ' - ' || pfldsc as descricao
						FROM
							seguranca.perfil p
                        join seguranca.sistema s on s.sisid = p.sisid
						WHERE
							p.sisid IN (231, 23)
    					ORDER BY
    						pfldsc";
    			echo $simec->radio('acttipoesfera', 'Esfera', ($acttipoesfera ? $acttipoesfera : 'T'), ['T' => 'Todos', 'E' => 'Estadual', 'M' => 'Municipal'], ['required']);
                echo $simec->select('estuf', 'Estado', $estuf, $arrEstado, ['multiple'=>'multiple'], ['input-size' => '7']);
                echo $simec->select('muncod', 'Município', $muncod, $arrMunicipio, ['multiple'=>'multiple'], ['input-size' => '7']);
                echo $simec->select('pflcod', 'Perfis associados', $pflcod, $sql, array('maxlength'=>'255', 'multiple'=>'multiple'), array('input-size' => '7'));
    			echo $simec->radio('actpermiteciente','Permitir aviso de ciente?',($actpermiteciente ? $actpermiteciente : 'f'),array('t' => 'Sim','f' => 'Não'),array('required'));
    			echo $simec->textarea('actconteudo', 'Detalhe', $actconteudo, array('class'=>'summernote', 'rows'=>30, 'cols'=>'100%', 'required'));
    			?>
			</form>
		</div>
	</div>
	<div class="ibox-footer">
		<div class="form-actions col-md-offset-3">
			<button type="button" id="btn_salvar" class="btn btn-success">
				<i class="fa fa-save"></i> Salvar
			</button>
			<button type="button" class="btn btn-secondary" onclick="location.href='par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=aviso';">
				<i class="fa fa-arrow-left"></i> Voltar
			</button>
		</div>
	</div>
</div>
<script type="text/javascript">
	jQuery(document).ready(function (){   
		 			
		validacaoAutomatica = new ValidacaoAutomatica();
		
		validacaoAutomatica.addCampo('name', 'acttitulo', 'Título')
						   .addValidaObrigatorio();
		
		validacaoAutomatica.addCampo('name', 'actinicio', 'Período: Data de Início')
						   .addValidaObrigatorio();
		
		validacaoAutomatica.addCampo('name', 'actfim', 'Período: Data Fim')
						   .addValidaObrigatorio();
		
		validacaoAutomatica.addCampo('name', 'actconteudo', 'Conteúdo do Aviso')
						   .addValidaExterno(function (){
							   var actconteudo = jQuery('[name=actconteudo]').code().trim();
							   return (actconteudo != '<p><br></p>' ? true : 'O <b>conteúdo</b> do aviso deve ser preenchido'); 
							});
		
		validacaoAutomatica.addCampo('id', 'pflcod', 'Perfis associados')
						   .addValidaExterno(function (){
							   var pflcod = $("#pflcod").chosen().val();
							   
							   return (pflcod != null ? true : 'O campo <b>perfis associados</b> deve ser preenchido'); 
							});
		   
		jQuery('#btn_salvar').click(function (){
			validacaoAutomatica.enviarFormulario('formAviso', 'requisicao', 'salvarAvisoConteudo');
		});

        $(function() {
            renderizeEstado();
            renderizeMunicipio();

            if($('#estuf').val() != null) {
                var muncod = ($('#muncod').val() ? $('#muncod').val().join(',') : '');
                carregarMunicipio($('#estuf').val().join(','), muncod);
            }

            $("input:radio[name=itrid], select[name=estuf]").change(function() {
                renderizeMunicipio();
            });

            $('#estuf').change(function(){
                var estufSelecionados = $(this).val();
                if (estufSelecionados != null) {
                    estufSelecionados = estufSelecionados.join(',');
                }
                carregarMunicipio(estufSelecionados);
            });

            $('input[name="acttipoesfera"]').change(function () {
                renderizeEstado();
                renderizeMunicipio();
            });

            function renderizeMunicipio() {
                var filtroMunicipio = $("select[name=muncod]").parents("div.form-group");
                if ($('input:radio[name=acttipoesfera]:checked').val() === 'E' || !$('select[name=estuf]').val() || $('input:radio[name=acttipoesfera]:checked').val() === 'T') {
                    filtroMunicipio.slideUp();
                } else {
                    filtroMunicipio.slideDown();
                }
            }

            function renderizeEstado() {
                var filtroEstado = $("select[name=estuf]").parents("div.form-group");
                if ($('input:radio[name=acttipoesfera]:checked').val() != 'T') {
                    filtroEstado.slideDown();
                } else {
                    filtroEstado.slideUp();
                }
            }
        });

        function carregarMunicipio(estuf, muncod) {
            var options = $('#muncod');
            
            if (muncod == null && $('#muncod').val()) {
                muncod = $('#muncod').val().join(',');
            }else if (muncod == null) {
				muncod = '';
			}

            var arrMuncod = muncod.split(',');
            options.empty();
            options.trigger('chosen:updated');

            if (estuf != null) {
                var arrEstuf = estuf.split(',');

                for (var i = 0, len = arrEstuf.length; i < len; i++) {
                    var estuf = arrEstuf[i];
                    if(estuf != '') {
                        options.append(new Option('', ''));
                        $.getJSON('', 'requisicao=carregaMunicipiosAviso&estuf='+estuf, function(retorno) {
                            var parsed = JSON.parse(retorno);
                            options.append(new Option('', ''));
                            $.each(parsed, function() {
                                options.append(new Option(this.mundescricao, this.muncod));
                            });
                            options.focus();
                            options.val(arrMuncod);
                            options.trigger('chosen:updated');

                            var orderedOptions = $('#muncod option').sort(function(a,b) {
                                if (a.text > b.text) return 1;
                                else if (a.text < b.text) return -1;
                                else return 0
                            });

                            $('#muncod').empty().append(orderedOptions);
                            $('#muncod').trigger('chosen:updated');
                        });
                    }
                }
            }
        }
	});
</script>


