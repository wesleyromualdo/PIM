<?php
if($unidade->itrid == 2){
	$origem = 'Município '.$municipio->mundescricao.'-'.$municipio->estuf;
}else{
	$origem = 'SEDUC do(e)'.$uf->estdescricao;
}
?>
<div class="modal-dialog">
    <div class="modal-content animated ">
        <form method="post" name="formulario" id="formulario" class="form form-horizontal">
            <input type="hidden" name="requisicao" value="aceitarRT"/>
            <input type="hidden" name="vnid"       value="<?php echo $vinculo->vnid;?>"/>
            <div class="ibox-title text-center" tipo="integrantes">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
                <h3>Responsável Técnico</h3>
            </div>
            <div class="ibox-content">
                <div style="font: 12pt Arial,verdana;text-indent: 1.5em;text-align: justify !important; ">
                    <h3>Termo de Responsabilidade Técnica - <?php echo $origem; ?></h3>
                    <br>
            	    <p>Venho por meio deste informar que sou responsável técnico do Programa
            	    Nacional de Alimentação Escolar no âmbito do(a) <b><?php echo $origem; ?></b>,
            	    a partir de <b><?php echo $vinculo->vndatavinculacao;?></b>,
            	    desempenhando minhas atividades em conformidade com o Código de Ética vigente (Resolução CFN nº 334/2004).
            	    Comprometo-me a cumprir e fazer cumprir o estabelecido na regulamentação
            	    do exercício profissional do Nutricionista, através de Leis,
            	    Decretos ou Resoluções, bem como, assumo a responsabilidade pela
            	    veracidade das informações disponibilizadas neste formulário.</p>
                </div>
                <br>
                <?php if(!$vinculo->snaceito){ ?>
                <div class="text-center">
                    <input type="checkbox" name="vnaceito" value="TRUE" required="required" style="cursor:pointer"> Aceito
                </div>
                <?php } ?>
            </div>
            <?php if(!$vinculo->snaceito){ ?>
            <div class="ibox-footer text-center">
                <button type="submit" class="btn btn-success"><i class="fa fa-check"></i> Gravar</button>
            </div>
            <?php } ?>
        </form>
    </div>
</div>