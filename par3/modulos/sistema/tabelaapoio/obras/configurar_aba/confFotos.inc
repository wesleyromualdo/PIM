<?php
// $this->model->ocaqtd_minimo         = $this->model->ocaqtd_minimo ? $this->model->ocaqtd_minimo : 3;
// $this->model->ocaqtd_maximo         = $this->model->ocaqtd_maximo ? $this->model->ocaqtd_maximo : 30;
// $this->model->ocatamanho_maximo     = $this->model->ocatamanho_maximo ? $this->model->ocatamanho_maximo : 5;
$arrTiposFoto[0]                    = $arrTiposFoto[0] ? $arrTiposFoto[0] : 'T';
$arrTipoFoto                        = $arrTiposFoto[0] ? $arrTipoFoto : $arrTipoFoto."UNION SELECT 'T' as codigo, 'Todos' as descricao";
?>
<form method="post" name="formFotos" id="formFotos" class="form form-horizontal">
    <div class="modal-content animated flipInY">
        <div class="ibox-title" tipo="integrantes">
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
            <h3 class="center" >Configurar Aba - Fotos do Terreno</h3>
            <h3 class="center" ><?php echo $ctrObraTipo->getDescricao()?></h3>
        </div>
        <div class="ibox-content">
            <input type="hidden" name="ocaid" value="<?php echo $arrPost['ocaid']; ?>"/>
            <?php
            echo $simec->input('ocaqtd_minimo', 'Quantidade mínima de fotos', ($this->model->ocaqtd_minimo == null) ? 3 : $this->model->ocaqtd_minimo , array('required','type'=>'number'), array('label-size'=>5));
            echo $simec->input('ocaqtd_maximo', 'Quantidade máxima de fotos', ($this->model->ocaqtd_maximo == null) ? 30 : $this->model->ocaqtd_maximo , array('required','type'=>'number'), array('label-size'=>5));
            echo $simec->input('ocatamanho_maximo', 'Tamanho máximo do arquivo (MB)', ($this->model->ocatamanho_maximo == null) ? 5 : $this->model->ocatamanho_maximo, array('required','type'=>'number'), array('label-size'=>5));
            ?>
            <div class="form-group ">
                <label for="atfid" class="col-sm-5 col-md-5 col-lg-5 control-label">Tipo: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                <div class="col-sm-6">
                    <form class="input-group" style="width: 100%;" id="selectTipoFoto" >
                        <div id="divSelectTipoFoto" class="col-sm-11" style="margin-left:-15px">
                            <?php echo $simec->select('atfid[]', '', $arrTiposFoto, $arrTipoFoto,array('required'),array('input-size'=>12)) ; ?>
                        </div>
                        <div class="col-sm-1">
                            <span class="input-group-btn" >
                                <button type="button" class="btn btn-success" id="btnTipoFoto" style="margin-left:-30px;" title="">
                                    <i id="iconeAcaoTipoFoto" class="fa fa-plus"></i>
                                </button>
                            </span>
                        </div>
                    </form>
                </div>
            </div>
            <form style="display: none;" id="formTipoFoto"></form>
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

    $('#atfid').prepend(new Option('', '', 'selected'));

    $('#btnTipoFoto').click(function (evt) {
        evt.preventDefault();
        $('#formTipoFoto').fadeIn();
        $('#salvarConf').attr('disabled', 'disabled');
        var selectTipo = $('#selectTipo').serialize();
        console.log(selectTipo);
        var caminho = window.location.href;
        var action = '&requisicao=recuperarTipoFoto&' + selectTipo;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#formTipoFoto').html(resp);
            }
        });
    });


    function salvar()
    {
        $("#div-erro").hide();
    	var caminho = window.location.href;
        var action  = '&requisicao=salvarConfiguracaoDocFotos&' + $('#formFotos').serialize();
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
                        swal.close();
                        $.each(res, function (index, erro) {//retorna mensagens de erro em json
                            $.each(erro, function (err, errMsg) {
                                if (errMsg < err.length) {
                                    return;//se a mensagem for vazia não retorna nada
                                }
                                    var divFormInput = $("#" + err).parent('div');
                                if(err == 'atfid'){
                                    $('#divSelectTipoFoto').closest('.form-group').addClass('has-error');
                                }else{
                                    divFormInput.closest('.form-group').addClass('has-error');
                                }
                                divFormInput.append("<div id='div-erro-" + err + "'></div>");
                                $("#div-erro-" + err).html("<span class='danger'>" + errMsg + "</span>");
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