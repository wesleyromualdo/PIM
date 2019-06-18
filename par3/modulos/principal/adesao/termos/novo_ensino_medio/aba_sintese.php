<script>
$(document).ready(function(){	
	$('.enviar').click(function(){
		var url 	= 'par3.php?modulo=principal/adesao/termo&acao=A';
		var dados 	= '&requisicao=enviarSelecao'+
						'&adpid=<?php echo $adpid ?>'+
						'&inuid=<?php echo $inuid ?>'+
						'&prgid=<?php echo $prgid ?>';

		swal({
            title: "<span style='font-size: 27px'>Atenção</span>",
            text: "Tem certeza que deseja enviar as escolas Sorteadas e Selecionadas ao MEC?",
            showCancelButton: true,
            confirmButtonColor: "#DD6B55", confirmButtonText: "Ok",
            closeOnConfirm: "on",
            cancelButtonText: "Cancelar",
            html: true
        }, function (isConfirm) {
            if (isConfirm) {
				$("#loading").show();
               window.location.href = url+dados;
            } else {
                return false;
            }
        });
	});
});
</script>
<div class="panel-body">
    <div class="col-md-11">
        <div id="conteudo_sintese">
            <center>
                <h3>Sintese de escolas que participarão do Programa Novo Ensino Médio</h3>
            </center>
            <hr class="divider">
            <center>
                <h3>Escolas Sorteadas</h3>
            </center>
    		<?php 
			    $dados = array(
			        'adpid'          => $adpid,
			        'situacao'       => 1,
			        'somenteLeitura' => true,
			        'inuid'          => $inuid
			    );
			    $controllerNovoEnsinoMedio->listaEscolas($dados); 
		    ?>
            <hr class="divider">
            <center>
                <h3>Escolas Selecionadas</h3>
            </center>
    		<?php 
			    $dados = array(
			        'adpid'          => $adpid,
			        'situacao'       => 2,
			        'somenteLeitura' => true,
			        'inuid'          => $inuid,
			        'selecionadas'   => 'S'
			    );
			    $controllerNovoEnsinoMedio->listaEscolas($dados); 
		    ?>
        </div>
        <center>
            <?php
                if (
                    ($esdid == WF_ESDID_EMCADASTRAMENTO_PNEM) &&
                    ($result['adpresposta'] != 'N') &&
                    ($escolasSelecionadas > 0) &&
                    (!$vencido)
                ) {
            ?>
            <button type="button" class="btn btn-primary enviar">
            	<i class="fa fa-thumbs-up"></i> 
            	Confirmar e Enviar
            </button>
            <?php } 
            if($esdid == WF_ESDID_ENVIADOAOMEC_PNEM){ ?>
            <a onclick="imprimir('pdf_sintese')" target="_blank">
                <button class="btn btn-warning imprimir"><i class="fa fa-print"></i> Imprimir</button>
            </a>
            <?php 
            }?>
        </center>
    </div>
</div>