<form method="post" name="formDocumentos" id="formDocumentos" class="form form-horizontal">
    <div class="modal-content animated flipInY">
        <div class="ibox-title" tipo="integrantes">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            <h3 class="center" >Configurar Aba - Documentos Anexos</h3>
            <h3 class="center" ><?php echo $ctrObraTipo->getDescricao()?></h3>
        </div>
        <div class="ibox-content">
            <input type="hidden" name="ocaid" value="<?php echo $arrPost['ocaid']; ?>"/>
            <?php $this->listarDocumentos();?>
        </div>
        <div class="ibox-footer">
        	<div class="col-sm-offset-2 col-md-offset-2 col-lg-offset-2">
            	<button type="button" class="btn btn-success" id="salvarConf" onclick="salvar();">
            		<i class="fa fa-plus-square-o"></i> Salvar
            	</button>
            </div>
        </div>
    </div>
</form>
<script>
    function salvar()
    {
        $("#div-erro").hide();
    	var caminho = window.location.href;
        var action  = '&requisicao=salvarConfiguracaoDoc&' + $('#formDocumentos').serialize();
        $('[id*="octordem_"]').css('background-color','');
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                if(resp != 'sucesso'){
                	if(resp != 'erro'){
                        var res = $.parseJSON(resp);
                        $.each(res, function (index, erro) {//retorna mensagens de erro em json
                            $.each(erro, function (err, errMsg) {
                                console.log(err);
                                if (errMsg < err.length) {
                                    return;//se a mensagem for vazia não retorna nada
                                }
                                if(err == 'otdid'){
                                	swal("Erro.", 'Selecione pelo menos um documento.', "error");
                                	return false;
                                }else{
                                	$.each(errMsg, function (otdid, msg) {
                                        if (msg < otdid.length) {
                                            return;//se a mensagem for vazia não retorna nada
                                        }
                                        var divFormInput = $("#octordem_"+otdid).parent('div');
                                        $('#octordem_'+otdid).css('background-color','#ffcccc');
                                        $("#div-erro").show();
                                	});
                                }
                            });
                        });
                	}else{
                    	swal("Erro.", 'Occoreu um erro inexperado.', "error");
                    	return false;
                    }
                }else{
                	swal("Sucesso.", "Configuração salva com sucesso", "success");
                	$('.close').click();
                    return false;
                }
                return false;
            }
        });
    };

</script>