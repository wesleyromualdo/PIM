<?php
$disabled = $dados['habilitado'] == 'true' ? '' : 'disabled="disabled"';
?>
<script>
$(document).ready(function(){

	var inuid = '<?php echo $_REQUEST['inuid']?>';
	var ppsid = '<?php echo $arrDadosIniciativa['ppsid']?>';
	var sbaid = '<?php echo $arrDadosIniciativa['sbaid']?>';
	var sbdid = '<?php echo $arrDadosIniciativa['sbdid']?>';

    $("#sbdplanointerno").change(function(){

        $('#sbdptres').empty();

        var url = window.location.href+'&req=buscaPTRESPI&sbdplanointerno='+$(this).val();

        $('#sbdptres').load(url, function(){
            $('#sbdptres').trigger("chosen:updated");
        });

    });

    $("#muncod").change(function(){

        var inuid  = $('#inuid').val();
        var sbaid  = $('#sbaid').val();
        var muncod = $('#muncod').val();
        var habilitado = '<?php echo $dados['habilitado']; ?>';

        var url = window.location.href+'&req=formEscolasMunicipio&inuid='+inuid+'&sbaid='+sbaid+'&muncod='+muncod+'&habilitado='+habilitado;

        $('#div_escolasMunicipio').load(url);

    });

    $('#btn_salvar').click(function(){

    	var url = window.location.href+'&req=salvaIniciativa';

    	$.ajax({
    	    method: "POST",
    	    url: url,
    	    data: $('#formIniciativa').serialize(),
    	    success: function(res) {
        	    if(res != 'secesso'){
            	    alert(res);
            	}else{
            	    alert('Dados salvos com sucesso.');
        	    	window.location.href = window.location.href;
                }
            }
        })
    });
});
</script>
<form method="post" name="formIniciativa" id="formIniciativa" class="form form-horizontal">
    <input type=hidden name=inuid         id=inuid         value=<?php echo $arrDadosIniciativa['inuid']; ?> />
    <input type=hidden name=sbaid         id=sbaid         value=<?php echo $arrDadosIniciativa['sbaid']; ?> />
    <input type=hidden name=sbdid         id=sbdid         value=<?php echo $arrDadosIniciativa['sbdid']; ?> />
    <input type=hidden name=ppscronograma id=ppscronograma value=<?php echo $arrDadosIniciativa['ppscronograma']; ?> />
    <div class="modal-content animated flipInY">
        <div class="ibox-title" tipo="integrantes">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            <h3>Iniciativa</h3>
        </div>
        <div class="ibox-content">
           <b><?php echo $arrDadosIniciativa['ppsdsc'];?></b><br><br>
           Cronograma: <?php echo $arrDadosIniciativa['cronograma'];?> <br>
           Tipo iniciativa: <?php echo $arrDadosIniciativa['ptsdescricao'];?>
        </div>
        <div class="ibox-content">
            <div class="row">
            	<div class="col-lg-4">
            	   <?php echo $arrDadosIniciativa['tordescricao']; ?>: R$ <?php echo formata_valor($arrOrcamentoUnidade[$arrDadosIniciativa['torid']]['valor']); ?>
				</div>
            	<div class="col-lg-4">
            	   Valor Utilizado: R$ <?php echo formata_valor($arrOrcamentoUnidade[$arrDadosIniciativa['torid']]['valor_utilizado']); ?>
				</div>
            	<div class="col-lg-4">
            	   Valor restante: R$ <?php echo formata_valor($arrOrcamentoUnidade[$arrDadosIniciativa['torid']]['valor_restante']); ?>
            	   <input type=hidden id=valor_restante name=valor_restante value=<?php echo ($arrOrcamentoUnidade[$arrDadosIniciativa['torid']]['valor_restante']+$arrDadosIniciativa['valor_iniciativa']);?> >
				</div>
			</div>
		</div>
		<div class="ibox-content">
            <div class="row">
                <div class="col-lg-12">
                    <div class="btn-group">
                        <button class="btn btn-primary" type="button">2016</button>
                    </div>
                </div>
            </div>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-12" style="overflow: fixed;">
                    <div class="panel-body">
                        <?php
                        if($arrDadosIniciativa['ppscronograma'] == 2){
                            include_once APPRAIZ.'par3/modulos/principal/planoTrabalho/programas/iniciativa/iniciativaEscola.php';
                        }else{
                            include_once APPRAIZ.'par3/modulos/principal/planoTrabalho/programas/iniciativa/iniciativaGlobal.php';
                        }
                        ?>
                    </div>
                </div>
			</div>
		</div>
        <div class="ibox-footer">
        	<div class="col-sm-offset-2 col-md-offset-2 col-lg-offset-2">
            	<button type="button" class="btn btn-success" id="btn_salvar" <?php echo $disabled;?>>
            		<i class="fa fa-floppy-o"></i> Salvar Iniciativa
            	</button>
            </div>
        </div>
    </div>
</form>
<div class="ibox float-e-margins animated modal" id="modalIniciativa" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog" id="modalIniciativa_html">
    </div>
</div>