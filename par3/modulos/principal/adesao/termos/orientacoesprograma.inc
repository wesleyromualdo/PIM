<?php

$perfis = pegaPerfilGeral($_SESSION['usucpf']);
$simec = new Simec_View_Helper();
$textoAtual = "Orientações aba " . $nomeAba;

$textoOrientacao = str_replace('\"', '"', $textoOrientacao);

if ($orpid && is_array($perfis)) {

    //@TODO verificar porque a formatação não aparece
    if ((in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis) ||
        in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis))
    ) {
        echo "
			<form method=\"post\" name=\"formulario-orientacao\" id=\"formulario-orientacao\" class=\"form form-horizontal\">
				<input type=\"hidden\" value=\"salvar_orientacao\" id=\"requisicao\" name=\"requisicao\" />
				<input type=\"hidden\" value=\"{$orpid}\" id=\"orpid\" name=\"orpid\" />
				<center>
					<div class='ibox'>";
        echo $simec->textarea('orptexto', $textoAtual, $textoOrientacao, array('class' => 'summernote', 'id' => 'orientacao_' . $orpid));
        echo " 			<button type=\"submit\" class=\"btn btn-success salvar\" ><i class=\"fa fa-save\"></i> Salvar</button>
					</div>
				</center>
			</form>";
    } else {
        ?>

        <div class="ibox-title">
            <center>
                <h3><?php echo $textoAtual; ?></h3>
            </center>
        </div>


        <div style="height: 180px;overflow-y: scroll" id="termo">
            <?php
            echo $textoOrientacao;
            ?>

        </div>


        <?php
    }


}
?>
