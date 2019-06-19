<?php
require APPRAIZ . 'includes/cabecalho.inc';

$renderEntidade = new Par3_Controller_Entidade();
$modelUsuario   = new Seguranca_Model_Usuario();
$modelPerfil    = new Seguranca_Model_Perfil();
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <div class="ibox">
    	<div class="ibox-title">
            <div class="row">
            	<div class="col-md-10" >
        	       <h3 id="entidade">Histórico de usuário PAR/PAR2011-2014</h3>
                </div>
    		</div>
    	</div>
    	<div class="ibox-content">
			<div class="row">
				<div class="col-lg-8 text-center">
            		<?php
            		echo $simec->cpf('usucpf', 'CPF', $_POST['usucpf'], array('class' => 'cpf', $disabled ));
            		echo $simec->input('usunome', 'Nome', $_POST['usunome'], array('class' => $form.'_entnome', 'maxlength' => '255', $disabled));
            		echo $simec->select('pflcod','Perfil', $_POST['pflcod'],$modelPerfil->montaSQLCombo('231'));
            		?>
                </div>
				<div class="col-lg-4 text-center">
				</div>
			</div>
    	</div>
    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-12 text-center">
					<button type="submit" class="btn btn-success salvar" ><i class="fa fa-search"></i> Pesquisar</button>
				</div>
			</div>
    	</div>
    </div>
</form>
<div class="ibox-content" style="overflow: outo;">
<?php
$_POST['sisid'] = '231, 23';
$modelUsuario->listaHistoricoUsuario($_POST);
?>
</div>