<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <a id="cancelarAtendimentoExtraordinario"><i class="fa fa-arrow-left"></i> voltar para a pesquisa</a>
                <h3 class="center">Programa <?php echo $programacao->prgdsc; ?> - Alterar</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <form name="form-atendimento-extraordinario" id="form-atendimento-extraordinario" class="form-horizontal">
            <div class="row">
                <?php
                $anos = array();
                $anoAtual = date('Y');
                $anoFim   = $anoAtual + 15;
                for($anoAtual;$anoAtual < $anoFim;$anoAtual++)
                {
                    $anos += array($anoAtual => $anoAtual);
                }

                global $simec;
                echo '<input type="hidden" name="prgid" value="' . $programacao->prgid . '"/>';
                echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
                echo '<div class="col-lg-2"><input type="text" disabled="" value="' . $programacao->prgid . '" class="form-control"></div>';
                echo '</div>';
                echo $simec->input('prgabreviatura', 'Sigla', $programacao->prgabreviatura, array('maxlength' => '10'));
                echo $simec->input('prgdsc', 'Programa', $programacao->prgdsc, array('maxlength' => '255', 'required'));
                echo $simec->input('prglink', 'Link', $programacao->prglink, array('maxlength' => '255'));
                echo $simec->input('prgarquivo', 'Arquivo', $programacao->prgarquivo, array('maxlength' => '255'));
                echo $simec->textarea('prgresumo', 'Resumo', $programacao->prgresumo);
                echo $simec->radio('prgatd_extraordinario', 'Atendimento Extraordinário', (!($programacao->prgatd_extraordinario) ? 'f' : $programacao->prgatd_extraordinario), array('t' => 'Sim', 'f' => 'Não'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
                echo $simec->radio('prgsituacao', 'Situação', (!($programacao->prgsituacao) ? 'A' : $programacao->prgsituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
                echo $simec->periodo('prgperiodo','Período', array(formata_data($programacao->prgperiodoinicio),formata_data($programacao->prgperiodofim)) ,array('required'), $attribs = array(), $config = array());
                ?>
                <div class="row">
                    <div class=" form-group">
                        <div class="col-sm-3 col-md-3 col-lg-3" id="prgperiodo" name="prgperiodo"></div>
                    </div>
                </div>
                <?php
                echo $simec->select('prganoreferencia', 'Ano de Referência', $programacao->prganoreferencia ,$anos, array('maxlength' => '255','required'));
                echo $simec->select('prgesfera', 'Esfera', $programacao->prgesfera ,array('E' =>'Estadual','M' => 'Municipal'), array('maxlength' => '255','required'));
                ?>
                <div id="alterar-imagem-input" style="display:  <?php echo $programacao->arqid ? 'none':''?>">
                    <?php
                    echo $simec->input('arqid', 'Imagem', $programacao->arqid, array('type' => 'file', 'help' => '', 'style' => 'height: auto !important;'));
                    ?>
                </div>
                <?php if($programacao->arqid) : ?>
                    <div class="form-group">
                        <label class="col-lg-3 control-label">Imagem Salva:</label>
                        <div class="col-lg-2">
                            <img class="image" id="imgPreview" src="../slideshow/slideshow/verimagem.php?newwidth=128&newheight=84&arqid=<?php echo $programacao->arqid?>&_sisarquivo=par3" alt="">
                            <a id="alterar-imagem">Alterar Imagem</a>
                        </div>
                    </div>
                <?php endif; ?>
            </div>
        </form>
    </div>
    <?php include_once 'pro_adesao.inc' ?>
    <?php include_once 'cursosAbas.inc' ?>
    <div class="ibox-footer">
        <div class="center">
            <button type="button" class="btn btn-danger" id="cancelarAtendimentoExtraordinario"><i class="fa fa-arrow-left"></i> voltar
            </button>
            <button type="button" class="btn btn-success" id="salvarAtendimentoExtraordinario">
                <i class="fa fa-check"></i>Salvar
            </button>
        </div>
    </div>
</div>
<script>
    $('#alterar-imagem').click(function(){
        $('#alterar-imagem-input').show();
    });
    // Variable to store your files
    var files;

    // Add events
    $(document).on('change', 'input[type=file]', prepareUpload);

    // Grab the files and set them to our variable
    function prepareUpload()
    {
        files = event.target.files;
    }

    <?php if($programacao->prgperiodoinicio && $programacao->prgperiodofim): ?>
        $('[name=prgperiodoinicio]').val('<?php echo formata_data($programacao->prgperiodoinicio)?>');
        $('[name=prgperiodofim]').val('<?php echo formata_data($programacao->prgperiodofim)?>');
    <?php endif; ?>

    $(document).on('click','#cancelarAtendimentoExtraordinario',function(evt){
        evt.preventDefault();
        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=programas');
    });

    jQuery('#form-atendimento-extraordinario').on('click',"input[name=prgsituacao ][value=I]",function(){
        swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará visível para novas Iniciativas", "warning");
    });

    jQuery('#form-atendimento-extraordinario').on('click',"input[name=prgatd_extraordinario ][value=f]",function(){
        swal({
            title: "Atenção.",
            text:  "A opção <b>Atendimento Extraordinário</b> está marcada como <b>NÃO</b>. As informações de <b>Período</b>, <b>Ano de Referência</b>, <b>Esfera</b>, <b>Imagem</b> serão perdidas. Tem certeza que deseja confirmar?",
            type: "warning",
            html: true,
            showCancelButton: true,
            confirmButtonColor: "#DD6B55",
            confirmButtonText: "Sim",
            cancelButtonText: "Não",
            closeOnConfirm: false
        }, function (isConfirm) {
            if (isConfirm) {
                var form = $('#form-atendimento-extraordinario').serialize();
                console.log(form);
                var caminho = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=programas';
                var action = '&req=salvar&' +form;
                $.ajax({
                    type: "POST",
                    url: caminho,
                    data: action,
                    async: false,
                    success: function (resp) {
                        var res = $.parseJSON(resp);

                        if (!isNaN(parseInt(res)) && isFinite(res)) {
                            swal({
                                title: "Sucesso.",
                                text:  "Programa salvo com sucesso.",
                                type: "success",
                                showCancelButton: false,
                                confirmButtonText: "Ok",
                                closeOnConfirm: true
                            }, function (isConfirm) {
                                if (isConfirm) {
                                    if($('#prgatd_extraordinario-f:checked').length > 0){
                                        window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=programas');
                                        return false;
                                    }
                                }
                            });
                            return false;
                        }
                        $.each(res,function(index,erro){//retorna mensagens de erro em json
                            $.each(erro,function(err,errMsg){
                                if(errMsg < err.length){
                                    return;//se a mensagem for vazia não retorna nada
                                }
                                var divFormInput =  $("#form-atendimento-extraordinario").find("[name="+err+"]").parent("div");
                                divFormInput.closest('.form-group').addClass('has-error');
                                divFormInput.append("<div id='div-"+err+"'></div>");
                                $("#div-"+err).html("<span class='danger'>"+errMsg+"</span>");
                            });
                        });
                        return false;
                    }
                });
                return false;
            }
        });
    });



    jQuery('#salvarAtendimentoExtraordinario').click(function (evt) {

        var form = $('#form-atendimento-extraordinario').serialize();

        evt.preventDefault();
        var arqid = '';
        if($('input[type=file]').val()) {
            var dados = uploadFiles();
            if (dados.success != 'true') {
                swal("Erro.", dados.error, "error");
                return false;
            }
            arqid = '&arqid='+dados.arqid;
        }

            var caminho = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=programas';
            var action = '&req=salvar&' +form+arqid;
            $.ajax({
                type: "POST",
                url: caminho,
                data: action,
                async: false,
                success: function (resp) {
                    var res = $.parseJSON(resp);
                    if (!isNaN(parseInt(res)) && isFinite(res)) {
                        swal({
                            title: "Sucesso.",
                            text:  "Programa salvo com sucesso.",
                            type: "success",
                            showCancelButton: false,
                            confirmButtonText: "Ok",
                            closeOnConfirm: true
                        }, function (isConfirm) {
                            if (isConfirm) {
                                if($('#prgatd_extraordinario-f:checked').length > 0){
                                    window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=programas');
                                    return false;
                                }
                                window.location.assign('par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=programas&submenu=atendimento_extraordinario&prgid=<?php echo $_REQUEST['prgid']?>');
                                return false;
                            }
                        });
                        return false;
                    }
                    $.each(res,function(index,erro){//retorna mensagens de erro em json
                        $.each(erro,function(err,errMsg){
                            if(errMsg < err.length){
                                return;//se a mensagem for vazia não retorna nada
                            }
                            var divFormInput =  $("#form-atendimento-extraordinario").find("[name="+err+"]").parent("div");
                            divFormInput.closest('.form-group').addClass('has-error');
                            divFormInput.append("<div id='div-"+err+"'></div>");
                            $("#div-"+err).html("<span class='danger'>"+errMsg+"</span>");
                        });
                    });
                    return false;
                }
            });
        return false;
    });

    $("#imgInp").change(function(){
        readURL(this);
    });

    // Catch the form submit and upload the files
    function uploadFiles()
    {
        event.stopPropagation(); // Stop stuff happening
        event.preventDefault(); // Totally stop stuff happening

        // START A LOADING SPINNER HERE

        // Create a formdata object and add the files
        var data = new FormData();
        $.each(files, function(key, value)
        {
            data.append(key, value);
        });

        var retorno;

        $.ajax({
            url : 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=programas&req=upload',
            type: 'POST',
            data: data,
            cache: false,
            async: false,
            processData: false, // Don't process the files
            contentType: false, // Set content type to false as jQuery will tell the server its a query string request
            success: function(data, textStatus, jqXHR)
            {
                var dados = $.parseJSON(data);
                console.log(dados);
                retorno = dados;
            },
            error: function(jqXHR, textStatus, errorThrown)
            {
                // Handle errors here
                console.log('ERRORS: ' + textStatus);
                return false;
                // STOP LOADING SPINNER
            }
        });
        return retorno;
    }
</script>
