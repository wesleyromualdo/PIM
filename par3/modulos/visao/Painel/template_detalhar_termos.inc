<?php

global $simec;
$arrEstado = $this->arEstados;
?>
    <?php if (!empty($this->arTermos)):?>
    <h4 class="h4">Termos - <?= $this->descricao;?>
        <?php if($this->superuser):?>
            <div class="pull-right">
                <button class="btn btn-info btn-sm" id="btn-sql-termo" title="SQL">
                    <i class="fa fa-database"></i>
                </button>
            </div>
            <div class="row">
                <div class="col-lg-12" style="display: none;" id="div-sql-termo">
                    <pre id="sql-modal-termo"><?= $this->sql;?></pre>
                </div>
            </div>
            <script>
                $('#btn-sql-termo').on('click',function () {
                    $('#div-sql-termo').slideToggle();
                });
            </script>
        <?php endif;?>
    </h4>
    <div class="row">
        <form id="form-pesquisa-termos">
            <?php if($this->intaidtermo):?>
                <?php foreach ($this->intaidtermo as $intaid):?>
                    <input type="hidden" name="intaidtermo[]" value="<?= $intaid;?>" />
                <?php endforeach;?>
            <? endif;?>
            <input type="hidden" name="descricaotermo" id="descricao" value="<?= $this->descricao;?>"/>
            <div class="row">
            <div class="col-lg-6">
                <?php
                echo $simec
                    ->radio(
                        'itrid',
                        'Esfera',
                        ($_POST['itrid']?:0),
                        [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'],
                        array('maxlength' => '255'),
                        array('label-size' => 2)
                    );
                ?>
            </div>

            <div class="col-lg-6">
                <div class="form-group">
                    <label for="estuf" class="col-sm-2 col-md-2 col-lg-2 control-label">Estado:</label>
                    <div class="col-sm-10 col-md-10 col-lg-10">
                        <select name="estuf[]" id="estuf" class="form-control" multiple="multiple">
                            <?php foreach($arrEstado as $key => $value):?>
                                <option value="">Selecione</option>
                                <option value="<?= $key;?>"><?= $value?></option>
                            <?php endforeach?>
                        </select>
                    </div>
                </div>
            </div>
<!--            <div class="col-lg-12">-->
<!--                <div class="col-lg-12">-->
<!--                    <div class="form-group">-->
<!--                        <label for="estuf" class="col-lg-offset-5 col-sm-3 col-md-3 col-lg-3 control-label">Tipos de Assistência:</label>-->
<!--                        <div class="col-sm-3 col-md-3 col-lg-3" style="margin-left:-55px;">-->
<!--                            <select name="intaidtermo" id="estuf" class="form-control">-->
<!--                                --><?php
//
//                                ?>
<!--                                <option value="">Selecione</option>-->
<!--                                --><?php //foreach($this->arTiposAssistencia as $key => $value):?>
<!--                                    <option value="--><?//= $key;?><!--">--><?//= $value?><!--</option>-->
<!--                                --><?php //endforeach?>
<!--                            </select>-->
<!--                        </div>-->
<!--                    </div>-->
<!--                </div>-->
<!--            </div>-->
            <div class="col-lg-12">
                <div class="" id="div-muncod" style="display:<?= $_POST['itrid'] == 1 ?'none':''?>;">
                    <?php
                    $arrMunicipio = array();
                    if ($_POST['muncod'] && $_POST['itrid'] != 1) {
                        $_POST['muncod'] = tratarArrayParaMultiSelect($_POST['muncod']);//trata o array
                        $municipio = new Territorios_Model_Municipio();//instancia a classe município
                        //lista todos os municípios relacionados ao Estado(estuf) informado
                        $arrMunicipio = $municipio->pegarSQLSelect($_POST['estuf']);
                    }
                    ?>

                    <div class="form-group">
                        <label for="muncod" class="col-sm-1 col-md-1 col-lg-1 control-label">Município:</label>
                        <div class="col-lg-12 col-md-12 col-sm-12">
                            <select name="muncod[]" id="muncod" class="form-control" multiple="multiple"></select>
                        </div>
                    </div>
                    <div style="clear:both"></div>
                </div>
            </div>
        </div>
        <br>
        <div class="row">
            <div class="col-lg-12">
                <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                    <input type="hidden" name="xls" id="xls" value="0">
                    <button type="button" class="btn btn-info" id="bt_termo_pesquisa">
                        <i class="fa fa-search"></i>Pesquisar
                    </button> |
                    <button class="btn btn-primary xls" type="submit" id="xls-termo"><i class="fa fa-file-excel-o">
                        </i> Gerar Excel
                    </button> |
                    <button type="reset" class="btn btn-default-bright limpar">
                        <i class="fa fa-eraser"></i>Limpar
                    </button>
                </div>
            </div>

        </div>
        </form>
        <br>
        <div class="row">
            <div class="col-lg-12">
                <div class="col-lg-6 pull-left">
                    <span id="termo-full_count"><?= $this->arTermos[0]['full_count'];?></span> Registros encontrados
                </div>
                <div class="col-lg-6 pull-right">
                    <div class="input-group">
                        <input
                                class="form-control js-busca-no-resultado input-lg"
                                placeholder="Digite o texto para busca" type="text"
                        >
                        <div class="input-group-addon">
                            <select name="itrid" id="">
                                <option value="E">Estado</option>
                                <option value="M">Município</option>
                            </select>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <br>

    <div style="height: 400px;overflow-y: scroll;" id="div-table-termos">
        <input type="hidden" name="limit"  id="limit"  value="<?= $this->limit;?>"/>
        <input type="hidden" name="offset" id="offset" value="<?= $this->offset;?>"/>
        <input type="hidden" name="fim" id="fim" value=""/>
        <div class="fixed-head" style="position: fixed;">
            <table
                class="table table-striped table-bordered table-hover table-responsive table-condensed"
                style="margin:0px;"
                id="div-cabecalho-termo"
            >
                <thead>
                <tr>
                    <td class="text-center">Entidade</td>
                    <td class="text-center">Documento</td>
                    <td class="text-center">Processo</td>
                    <td class="text-center">Atendimento</td>
                </tr>
                </thead>
            </table>
        </div>
        <table class="table table-striped table-bordered table-hover table-condensed table-responsive" id="table-termos">
            <thead>
            <tr>
                <td class="text-center">Entidade</td>
                <td class="text-center">Documento</td>
                <td class="text-center">Processo</td>
                <td class="text-center">Atendimento</td>
            </tr>
            </thead>
            <tbody id="table-termo-body">
            <?php foreach ($this->arTermos as $termo):?>
                <tr>
                    <td class="text-center" ><?= $termo['entidade'];?></td>
                    <td class="text-center" ><?= $termo['documento'];?></td>
                    <td class="text-center" ><?= formata_numero_processo_sem_html($termo['pronumeroprocesso']);?></td>
                    <td class="text-center" ><?= $termo['atendimento'];?></td>
                </tr>
            <?php endforeach;?>
            </tbody>
        </table>
    </div>
<?php else:?>
    <div class="alert alert-info"><p align="center">Nenhum registro encontrado</p></div>
<?php endif;?>

<script>
    $('.limpar').click(function() {
        $("#estuf").val('').select2();
        $("#muncod").val('').select2();
        $("input[name='itrid']:checked").attr('checked',false).parent('label').removeClass('active');
        $("input[name='itrid']").filter("[value='0']").attr('checked',true).parent('label').addClass('active');
    });

    $(function(){
        $("[name='estuf[]']").select2({  theme: "classic"});
        $("[name='muncod[]']").select2({  theme: "classic"});
        // $(".chosen-select").chosen();
    });
    function redimensionarTheadTermos() {
        var table = $('#table-termos');
        var head =  $('#div-cabecalho-termo');

        table.find('thead').find('tr:first').find('td').each(function (index, value) {
            head.find('thead').find('td').eq(index).width($(this).width());
        });

        //esconder/mostar header de acordo com a resolução
        if($(window).width() < 800 ) {
            head.hide();
            table.css('margin-bottom', 0);
            table.css('margin-top', '0px');
        }else {
            head.show();
        }
    }

    redimensionarTheadTermos();
    $(window).on('resize',function() {
        redimensionarTheadTermos();
        console.log('resize');
    });

    $(document).ready(function() {

        $('#loading').hide();
        $("#div-table-termos").scroll(function(event) {
            var clientHeight = event.target.clientHeight;
            if($('#fim').val() == 't'){
                return;
            }
            if($(this).scrollTop() + $(this).innerHeight() >= $(this)[0].scrollHeight) {
                var limit     = parseInt($('#limit').val());
                var offset    = parseInt($('#offset').val()) + 50;
                var descricao = $('[name=descricaotermo]').val();
                var arr = [];$("[name='iboxtermos[intaidtermo][]']:checked").each(function() {
                    arr.push($(this).val());
                });
                $('#loading').hide();
                $.ajax({
                    url: '/par3/par3.php?modulo=painel/listatermos&acao=A',
                    data: {
                        limit:     limit,
                        offset:    offset,
                        datatype: 'json',
                        descricao: descricao,
                        intaidtermo: arr
                    },
                    beforeSend: function(msg) {
                        $('#loading').hide();
                    },
                    success: function(data) {
                        var res = JSON.parse(data);
                        if(Array.isArray(res.arTermos)) {
                            $('#offset').val(offset);
                            $('#limit').val(limit);
                            $.each(res.arTermos, function (index, termo) {
                                var pronumeroprocesso  =
                                    termo.pronumeroprocesso.substr(0, 5) +'.' +
                                    termo.pronumeroprocesso.substr(5, 6) + '/' +
                                    termo.pronumeroprocesso.substr(11, 4) + '-' +
                                    termo.pronumeroprocesso.substr(15, 2);
                                $('#table-termos').find('#table-termo-body').append(
                                    "<tr>\n" +
                                    "<td class=\"text-center\">"+termo.entidade+"</td>\n" +
                                    "<td class=\"text-center\">"+termo.documento+"</td>\n" +
                                    "<td class=\"text-center\">"+pronumeroprocesso+"</td>\n" +
                                    "<td class=\"text-center\">"+termo.atendimento+"</td>\n"+
                                    "</tr>"
                                );
                            });
                            redimensionarTheadTermos();
                        }else{
                            $('#fim').val('t');
                        }
                    }
                });
            }
        });
    });

    $('.js-busca-no-resultado').on('keyup',function() {
        var val = $(this).val();
        var itrid = $('[name=itrid]').val();
        var descricao = $('[name=descricaotermo]').val();
        var tbody     = $('#table-termos').find('#table-termo-body');
        var arr = [];$("[name='iboxtermos[intaidtermo][]']:checked").each(function(){
            arr.push($(this).val());
        });
        $.ajax({
            url: '/par3/par3.php?modulo=painel/listatermos&acao=A',
            data: {
                datatype     : 'json',
                descricao    : descricao,
                inudescricao : val,
                itrid        : itrid,
                intaidtermo  : arr
            },
            beforeSend: function(msg) {
                $('#loading').hide();
            },
            success: function(data) {
                var res = JSON.parse(data);
                $('#termo-full_count').html(0);
                if(Array.isArray(res.arTermos)) {
                    tbody.html('');
                    $('#termo-full_count').html(res.arTermos[0].full_count);
                    $.each(res.arTermos, function (index, termo) {
                        var pronumeroprocesso  =
                            termo.pronumeroprocesso.substr(0, 5) +'.' +
                            termo.pronumeroprocesso.substr(5, 6) + '/' +
                            termo.pronumeroprocesso.substr(11, 4) + '-' +
                            termo.pronumeroprocesso.substr(15, 2);
                        tbody.append(
                            "<tr>\n" +
                            "<td class=\"text-center\">"+termo.entidade+"</td>\n" +
                            "<td class=\"text-center\">"+termo.documento+"</td>\n" +
                            "<td class=\"text-center\">"+pronumeroprocesso+"</td>\n" +
                            "<td class=\"text-center\">"+termo.atendimento+"</td>\n"+
                        "</tr>"
                        );
                    });
                    redimensionarTheadTermos();
                }
            }
        });
        redimensionarTheadTermos();
    });

    $('#bt_termo_pesquisa').on('click',function(evt){
        evt.preventDefault();
        renderListagem();
    });

    $("#xls-termo").on("click", function (evt) {
        evt.preventDefault();
        var form = $('#form-pesquisa-termos').serialize();
        window.location.assign(
            "/par3/par3.php?modulo=painel/xlstermos&acao=A&"+form
        );
    });

    $('#muncod').one('focus',function() {
        console.log('on focus');
        if(Array.isArray($('#estuf').val())){
            return;
        }
        listarMunicipios();
    });

    $('#div-muncod').one('click',function() {
        console.log('on click');
        if(Array.isArray($('#estuf').val())){
            return;
        }
        listarMunicipios();
    });

    $('[name=itrid]').change(function(){
        if($(this).val() == 1){
            $('#div-muncod').hide();
            $('#muncod').empty();
            $('#muncod').trigger("chosen:updated");
        }else{
            $('#div-muncod').show();
        }
    });

    function listarMunicipios()
    {
        console.log('testes');
        var options = jQuery('#muncod');
        options.empty();
        options.trigger('chosen:updated');
        options.append(new Option('', ''));
        jQuery.ajax({
            type: "GET",
            url:'/par3/par3.php?modulo=painel/rotas&acao=A',
            data: { req:'carregarTodosMunicipios'},
            success: function(retorno){
                options.append(new Option('', ''));
                $.each(JSON.parse(retorno), function() {
                    options.append(new Option(this.descricao, this.codigo));
                });
                options.val(muncod);
                options.trigger('chosen:updated');
            }
        });
    }

    function carregarMunicipioAnalise(estuf, muncod) {
        if(estuf != '' && $('[name=itrid]:checked').val() != 1) {
            var options = jQuery('#muncod');
            options.empty();
            options.trigger('chosen:updated');
            if(!Array.isArray(estuf)){
                return;
            }
            $('#loading').hide();
            options.append(new Option('', ''));
            jQuery.ajax({
                type: "GET",
                url: '/par3/par3.php?modulo=painel/rotas&acao=A',
                data: { requisicao:'carregarMunicipiosAnalise',estuf : estuf },
                success: function(retorno){
                    options.append(new Option('', ''));
                    $.each(JSON.parse(retorno), function() {
                        options.append(new Option(this.descricao, this.codigo));
                    });
                    options.focus();
                    options.val(muncod);
                    options.trigger('chosen:updated');
                }
            });
        }
    }

    jQuery('#estuf').change(function() {
        if($('[name=itrid]:checked').val() == 1) {
            return;
        }else {
            if(!Array.isArray($(this).val())) {
                listarMunicipios();
            }else{
                carregarMunicipioAnalise($('#estuf').val(),$('#muncod').val());
            }
        }
    });

    function renderListagem() {
        var descricao = $('[name=descricaotermo]').val();
        var tbody     = $('#table-termos').find('#table-termo-body');
        var arr = [];$("[name='iboxtermos[intaidtermo][]']:checked").each(function(){
            arr.push($(this).val());
        });
        var form = $('#form-pesquisa-termos').serialize();
        $.ajax({
            url: '/par3/par3.php?modulo=painel/listatermos&acao=A&'+form,
            data: {
                datatype     : 'json',
                descricao    : descricao,
                intaidtermo  : arr
            },
            beforeSend: function(msg) {
                $('#loading').hide();
            },
            success: function(data) {
                var res = JSON.parse(data);
                $('#termo-full_count').html(0);
                if(Array.isArray(res.arTermos)) {
                    tbody.html('');
                    $('#termo-full_count').html(res.arTermos[0].full_count);
                    $.each(res.arTermos, function (index, termo) {
                        var pronumeroprocesso  =
                            termo.pronumeroprocesso.substr(0, 5) +'.' +
                            termo.pronumeroprocesso.substr(5, 6) + '/' +
                            termo.pronumeroprocesso.substr(11, 4) + '-' +
                            termo.pronumeroprocesso.substr(15, 2);
                        tbody.append(
                            "<tr>\n" +
                            "<td class=\"text-center\">"+termo.entidade+"</td>\n" +
                            "<td class=\"text-center\">"+termo.documento+"</td>\n" +
                            "<td class=\"text-center\">"+pronumeroprocesso+"</td>\n" +
                            "<td class=\"text-center\">"+termo.atendimento+"</td>\n"+
                            "</tr>"
                        );
                    });
                    redimensionarTheadTermos();
                } else {
                    tbody.html('');
                }
            }
        });
        redimensionarTheadTermos();
    }
</script>