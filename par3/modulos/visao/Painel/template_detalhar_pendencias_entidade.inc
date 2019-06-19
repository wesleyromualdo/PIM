<?php if (!empty($this->arPendencias)):?>
    <?php
    global $simec;
    $arrEstado = $this->arEstados;

    switch ($_REQUEST['requisicao']) {
        case 'carregarMun':
            $municipio = new Territorios_Model_Municipio();
            print simec_json_encode($municipio->lista(array('muncod', 'mundescricao||\' - \'||estuf as mundescricao'), array("estuf = '{$_REQUEST['estuf']}'")));
            die;
            break;
    }

    function iconPendencia($pendencia) {
        $icone = "";
        switch ($pendencia) {
            case 1: $icone  = '<div class="fa fa-check-circle success"></div>';break;
            case 2: $icone  = '<div class="fa fa-exclamation-triangle danger"></div>';break;
            case 3: $icone  = '<div class="fa fa-exclamation-triangle warning"></div>';break;
            default: $icone = $pendencia;
        }
        return $icone;
    }

    function trPendencia($pendencia) {
        $icone = "";
        switch ($pendencia) {
            case 'Sem Pendência': $icone  = '<div class="fa fa-check-circle success"></div>';break;
            case 'Pendência':     $icone  = '<div class="fa fa-exclamation-triangle danger"></div>' ;break;
            case 'Alerta':        $icone  = '<div class="fa fa-exclamation-triangle warning"></div>'  ;break;
            default:;
        }
        return $icone;
    }
    ?>
    <h4 class="h4">Pendências - <?= $this->descricao;?>
        <?php if($this->superuser):?>
            <div class="pull-right">
                <button class="btn btn-info btn-sm" id="btn-sql-pendencia" title="SQL">
                    <i class="fa fa-database"></i>
                </button>
            </div>
            <div class="row">
                <div class="col-lg-12" style="display: none;" id="div-sql-pendencia"><pre id="sql-modal-pendencia"><?= $this->sql;?></pre></div>
            </div>
            <script>
                $('#btn-sql-pendencia').on('click',function () {
                    $('#div-sql-pendencia').slideToggle();
                });
            </script>
        <?php endif;?>
    </h4>
    <style>
        body {
            margin: 0;
            font-family: Arial, Helvetica, sans-serif;
        }

        .top-container {
            background-color: #f1f1f1;
            padding: 30px;
            text-align: center;
        }

        .header {
            padding: 10px 16px;
            background: #555;
            color: #f1f1f1;
        }

        .content {
            padding: 16px;
        }

        .sticky {
            position: fixed;
            top: 0;
            width: 100%;
        }

        .sticky + .content {
            padding-top: 102px;
        }

    </style>
    <div class="row">
        <form id="form-pesquisa-pendencias">
            <?php if($this->tipid):?>
                <?php foreach ($this->tipid as $tip):?>
                    <input type="hidden" name="tipid[]" value="<?= $tip;?>" />
                <?php endforeach;?>
            <? endif;?>
            <input type="hidden" name="descricao-pendencias" id="descricao" value="<?= $this->descricao;?>"/>
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
                        <button type="button" class="btn btn-info" id="bt_pendencia_pesquisa">
                            <i class="fa fa-search"></i>Pesquisar
                        </button> |
                        <button class="btn btn-primary xls" type="submit" id="xls-pendecias"><i class="fa fa-file-excel-o">
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
                    <span id="pendencia-full_count"><?= $this->arPendencias[0]['full_count'];?></span> Registros encontrados
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

    <hr>
    <div class="row">
        <div class="col-lg-12 pull-left">
            <b>Legenda:</b>
            <?= iconPendencia(1)?> Sem Pendência |
            <?= iconPendencia(2)?> Pendência |
            <?= iconPendencia(3)?> Alerta |
            <i class="fa fa-link success"></i> Acessar Pendências |
            <i class="fa fa-refresh info"></i> Atualizar Pendências
        </div>
    </div>
    <div id="div_debug"></div>
    <input type="hidden" name="inuid" value="">
    <input type="hidden" name="cae" value="">
    <input type="hidden" name="par" value="">
    <input type="hidden" name="cacs" value="">
    <input type="hidden" name="pne" value="">
    <input type="hidden" name="habilita" value="">
    <input type="hidden" name="monitoramento" value="">
    <input type="hidden" name="siope" value="">
    <div class="text-pendencia" style="font-size: 20px; font-weight: bold;"></div>
    <div class="progress" style="display: none;">
        <div class="progress-bar" role="progressbar" aria-valuenow="1" aria-valuemin="0" aria-valuemax="100" style="width: 1%; font-size: 20px; font-weight: bold;">0%</div>
    </div>
    <div style="height: 400px;overflow-y: scroll;position:relative;" id="div-table-pendencias">
        <input type="hidden" name="limit"  id="limit"  value="<?= $this->limit;?>"/>
        <input type="hidden" name="offset" id="offset" value="<?= $this->offset;?>"/>
        <input type="hidden" name="fim" id="fim" value=""/>
        <div class="fixed-head" style="position: fixed;">
            <table class="table table-striped table-bordered table-hover table-responsive table-condensed" style="margin:0px;overflow: scroll;" id="div-cabecalho">
                <thead>
                <tr>
                    <td class="text-center">Ações</td>
                    <td class="text-center">Situação</td>
                    <td class="text-center">Entidade</td>
                    <td class="text-center">CAE</td>
                    <td class="text-center">CACS</td>
                    <td class="text-center">Habilitação </td>
                    <td class="text-center">Monitoramento PAR</td>
                    <td class="text-center">Obras PAR</td>
                    <td class="text-center">PNE</td>
                    <td class="text-center">Contas</td>
                    <td class="text-center">SIOPE</td>
                </tr>
                </thead>
            </table>
        </div>
        <div id="loading-pendencias"
             style="
                background-color:black;
                opacity:0.8;
                width: 100%;
                height: 100%;
                position: absolute;
                top: 0;
                left: 0;
                display:none;
                z-index: 999999999999;">
        </div>
        <table class="table table-striped table-bordered table-hover table-responsive table-condensed table-head-fixed content" id="table-pendencias">
            <thead style="">
            <tr>
                <td class="text-center">Ações</td>
                <td class="text-center">Situação</td>
                <td class="text-center">Entidade</td>
                <td class="text-center">CAE</td>
                <td class="text-center">CACS</td>
                <td class="text-center">Habilitação </td>
                <td class="text-center">Monitoramento PAR</td>
                <td class="text-center">Obras PAR</td>
                <td class="text-center">PNE</td>
                <td class="text-center">Contas</td>
                <td class="text-center">SIOPE</td>
            </tr>
            </thead>
            <tbody id="table-pendencia-body">
            <?php foreach ($this->arPendencias as $pendencia):?>
                <tr id="tr_<?= $pendencia['inuid']; ?>">
                    <td class="text-center" >
                        <a
                        target='_blank'
                        href="<?= '/par3/par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid='.$pendencia['inuid'];?>"
                        class="success"
                        style="margin: 5px;"
                        title="Clique para acessar as pendências"
                        >
                            <i class="fa fa-link"></i>
                        </a>
                        <a
                            class="js-atualizar-pendencias info"
                            id="<?= $pendencia['inuid']; ?>"
                            title="Clique para atualizar as pendências"
                            style="margin: 5px;"
                        >
                            <i class="fa fa-refresh"></i>
                        </a>
                    </td>
                    <td class="text-center"  id="td_situacao_<?= $pendencia['inuid']; ?>"><?= trPendencia($pendencia['descricao']);?></td>
                    <td class="text-center" ><?= $pendencia['entidade'];?></td>
                    <td class="text-center"  id="td_cae_<?= $pendencia['inuid']; ?>"><?= iconPendencia($pendencia['cae']);?></td>
                    <td class="text-center"  id="td_cacs_<?= $pendencia['inuid']; ?>"><?= iconPendencia($pendencia['cacs']);?></td>
                    <td class="text-center"  id="td_habilitacao_<?= $pendencia['inuid']; ?>"><?= iconPendencia($pendencia['habilitacao']);?></td>
                    <td class="text-center"  id="td_monitoramento_par_<?= $pendencia['inuid']; ?>"><?= iconPendencia($pendencia['monitoramento_par']);?></td>
                    <td class="text-center"  id="td_obras_par_<?= $pendencia['inuid']; ?>"><?= iconPendencia($pendencia['obras_par']);?></td>
                    <td class="text-center"  id="td_pne_<?= $pendencia['inuid']; ?>"><?= iconPendencia($pendencia['pne']);?></td>
                    <td class="text-center"  id="td_contas_<?= $pendencia['inuid']; ?>"><?= iconPendencia($pendencia['contas']);?></td>
                    <td class="text-center"  id="td_siope_<?= $pendencia['inuid']; ?>"><?= iconPendencia($pendencia['siope']);?></td>
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
    $(function () {
        $(document).find('#loading').hide();
        var document = $("#div-table-pendencias"),
            left = 0,
            scrollTimer = 0;

        // Detect horizontal scroll start and stop.
        document.on("scroll", function () {
            var docLeft = document.scrollLeft();
            if(left !== docLeft) {
                var self = this, args = arguments;
                if(!scrollTimer) {
                    // We've not yet (re)started the timer: It's the beginning of scrolling.
                    startHScroll.apply(self, args);
                }
                window.clearTimeout(scrollTimer);
                scrollTimer = window.setTimeout(function () {
                    scrollTimer = 0;
                    // Our timer was never stopped: We've finished scrolling.
                    stopHScroll.apply(self, args);
                }, 100);
                left = docLeft;
            }
        });

        // Horizontal scroll started - Make div's absolutely positioned.
        function startHScroll () {
            // console.log("Scroll Start");
            $(".fixed-head")
            // Clear out any left-positioning set by stopHScroll.
                .css("left","")
                .each(function () {
                    var $this = $(this),
                        pos = $this.offset();
                    // Preserve our current vertical position...
                    $this.css("top", pos.top)
                })
                // ...before making it absolutely positioned.
                .css("position", "absolute");
            redimensionarThead();
        }

        // Horizontal scroll stopped - Make div's float again.
        function stopHScroll () {
            var leftScroll = $("#div-table-pendencias").scrollLeft();
            // console.log("Scroll Stop");
            $(".fixed-head")
            // Clear out any top-positioning set by startHScroll.
                .css("top","")
                .each(function () {
                    var $this = $(this),
                        pos = $this.position();
                    // Preserve our current horizontal position, munus the scroll position...
                    $this.css("left", pos.left-leftScroll);
                })
                // ...before making it fixed positioned.
                .css("position", "fixed");
            redimensionarThead();
        }
    });

    function redimensionarThead() {
        var table = $('#table-pendencias');
        var head =  $('#div-cabecalho');
        head.show();
        //copiar dimensões da thead da tabela
        table.find('thead').find('tr:first').find('td').each(function (index, value) {
            $('#div-cabecalho').find('thead').find('td').eq(index).width($(this).width());
        });

        //esconder/mostar header de acordo com a resolução
        if($(window).width() < 800 ) {
            head.hide();
            table.css('margin-bottom', 0);
            table.css('margin-top', '0px');
        }else {
        head.show();
        // table.css('margin-bottom', 0);
        // table.css('margin-top', '-' + (20 + parseInt(table.find('thead').find('tr:first').find('td').eq(0).height())) + 'px');
        }

        // var lastTd = $('#div-cabecalho').find('thead').find('tr').find('td:last');
        // var width = parseInt(lastTd.width());
        // var diferencaLargura = Math.abs($('#div-table-pendencias').width() - $('#table-pendencias').width());
        // console.log(width - diferencaLargura);
        // lastTd.width(52);
    }

    $(window).on('resize',function() {
        redimensionarThead();
        // console.log('resize entidade');
    });

    $(document).ready(function() {
        redimensionarThead();
        // $('#table-pendencias').find('thead').hide();

        $('#loading').hide();
        // $("#table-pendencia-body").scroll(function(event) {
        $("#div-table-pendencias").scroll(function(event) {
            var clientHeight = event.target.clientHeight;
            // console.log($(this).scrollTop() + $(this).innerHeight() >= $(this)[0].scrollHeight);
            if($('#fim').val() == 't'){
                return;
            }
            // console.log($('#table-pendencias').height(), $('#div-table-termos').scrollTop(),clientHeight);
            // var scrollCalculo = $('#table-termos').height() - ($('#div-table-termos').scrollTop() + clientHeight);
            // if($('#table-pendencias').height() <= ($('#div-table-pendencias').scrollTop()+clientHeight)) {
            if($(this).scrollTop() + $(this).innerHeight() >= $(this)[0].scrollHeight) {
                var limit     = parseInt($('#limit').val());
                var offset    = parseInt($('#offset').val()) + 50;
                var descricao = $('[name=descricao-pendencias]').val();
                var arr = [];$("[name='iboxpendencias[tipid][]']:checked").each(function(){
                    arr.push($(this).val());
                });
                $('#loading').hide();
                var form = $('#form-pesquisa-pendencias').serialize();
                $.ajax({
                    url: '/par3/par3.php?modulo=painel/listapendencias&acao=A&'+form,
                    data: {
                        limit    : limit,
                        offset   : offset,
                        datatype : 'json',
                        descricao: descricao,
                        tipid    : arr
                    },
                    beforeSend: function(msg) {
                        $('#loading').hide();
                    },
                    success: function(data) {
                        var res = JSON.parse(data);
                        if(Array.isArray(res.arPendencias)) {
                            $('#offset').val(offset);
                            $('#limit').val(limit);
                            $.each(res.arPendencias, function (index, pendencia) {
                                var inuid = pendencia.inuid;
                                // console.log(inuid);
                                $('#table-pendencias').find('#table-pendencia-body').append(
                                    "<tr id='tr_"+pendencia.inuid+"'>\n" +
                                    "<td class=\"text-center col-lg-2\">" +
                                    "<a  class='success'" +
                                    " style='margin: 5px;' "+
                                    "title='Clique para acessar a pendência'"+
                                    "target='_blank'"+
                                    "href='/par3/par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid="+inuid+"'>"+
                                    "<i class='fa fa-link'></i>"+
                                    "</a> " +
                                    "<a class='js-atualizar-pendencias info'"+
                                    " style='margin: 5px;' "+
                                    "onclick='atualizaPendencias("+pendencia.inuid+")' " +
                                    "title='Clique para atualizar as pendências' " +
                                    "id='"+pendencia.inuid+"'>"+
                                    "<i class='fa fa-refresh'></i>" +
                                    "</a>"+
                                    "</td>\n" +
                                    "<td class=\"text-center col-lg-2\" id='td_situacao_"+pendencia.inuid+"'>"+trPendencia(pendencia.descricao)+"</td>\n" +
                                    "<td class=\"text-center col-lg-3\">"+pendencia.entidade+"</td>\n" +
                                    "<td class=\"text-center col-lg-2\" id='td_cae_"+pendencia.inuid+"'>"+iconPendencia(pendencia.cae)+"</td>\n" +
                                    "<td class=\"text-center col-lg-2\" id='td_cacs_"+pendencia.inuid+"'>"+iconPendencia(pendencia.cacs)+"</td>\n" +
                                    "<td class=\"text-center col-lg-2\" id='td_habilitacao_"+pendencia.inuid+"'>"+iconPendencia(pendencia.habilitacao)+"</td>\n" +
                                    "<td class=\"text-center col-lg-3\" id='td_monitoramento_par_"+pendencia.inuid+"'>"+iconPendencia(pendencia.monitoramento_par)+"</td>\n" +
                                    "<td class=\"text-center col-lg-2\" id='td_obras_par_"+pendencia.inuid+"'>"+iconPendencia(pendencia.obras_par)+"</td>\n" +
                                    "<td class=\"text-center col-lg-2\" id='td_pne_"+pendencia.inuid+"'>"+iconPendencia(pendencia.pne)+"</td>\n" +
                                    "<td class=\"text-center col-lg-2\" id='td_contas_"+pendencia.inuid+"'>"+iconPendencia(pendencia.contas)+"</td>\n" +
                                    "<td class=\"text-center col-lg-2\" id='td_siope_"+pendencia.inuid+"'>"+iconPendencia(pendencia.siope)+"</td>\n" +
                                    "            </tr>"
                                );
                            });
                            redimensionarThead();
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
        var descricao = $('[name=descricao-pendencias]').val();
        var tbody     = $('#table-pendencias').find('#table-pendencia-body');
        var arr = [];$("input:checkbox[name='iboxpendencias[tipid][]']:checked").each(function(){
            arr.push($(this).val());
        });
        $.ajax({
            url: '/par3/par3.php?modulo=painel/listapendencias&acao=A',
            data: {
                datatype     : 'json',
                descricao    : descricao,
                tipid        : arr,
                inudescricao :val,
                itrid        : itrid
            },
            beforeSend: function(msg) {
                $('#loading').hide();
            },
            success: function(data) {
                var res = JSON.parse(data);
                if(Array.isArray(res.arPendencias)) {
                    tbody.html('');
                    $.each(res.arPendencias, function (index, pendencia) {
                        var inuid = pendencia.inuid;
                        tbody.append(
                            "<tr id='tr_"+pendencia.inuid+"'>\n" +
                            "<td class=\"text-center col-lg-2\">" +
                            "<a  class='success'" +
                            " style='margin: 5px;' "+
                            "title='Clique para acessar a pendência'"+
                            "target='_blank'"+
                            "href='/par3/par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid="+inuid+"'>"+
                            "<i class='fa fa-link'></i>"+
                            "</a> " +
                            "<a class='js-atualizar-pendencias info'"+
                            " style='margin: 5px;' "+
                            "onclick='atualizaPendencias("+pendencia.inuid+")' " +
                            "title='Clique para atualizar as pendências' " +
                            "id='"+pendencia.inuid+"'>"+
                            "<i class='fa fa-refresh'></i>" +
                            "</a>"+
                            "</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_situacao_"+pendencia.inuid+"'>"+trPendencia(pendencia.descricao)+"</td>\n" +
                            "<td class=\"text-center col-lg-3\">"+pendencia.entidade+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_cae_"+pendencia.inuid+"'>"+iconPendencia(pendencia.cae)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_cacs_"+pendencia.inuid+"'>"+iconPendencia(pendencia.cacs)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_habilitacao_"+pendencia.inuid+"'>"+iconPendencia(pendencia.habilitacao)+"</td>\n" +
                            "<td class=\"text-center col-lg-3\" id='td_monitoramento_par_"+pendencia.inuid+"'>"+iconPendencia(pendencia.monitoramento_par)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_obras_par_"+pendencia.inuid+"'>"+iconPendencia(pendencia.obras_par)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_pne_"+pendencia.inuid+"'>"+iconPendencia(pendencia.pne)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_contas_"+pendencia.inuid+"'>"+iconPendencia(pendencia.contas)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_siope_"+pendencia.inuid+"'>"+iconPendencia(pendencia.siope)+"</td>\n" +
                            "            </tr>"
                        );
                    });
                    redimensionarThead();
                }
            }
        });
        redimensionarThead();
    });

    function iconPendencia(pendencia) {
        var icone = "";
        switch (pendencia) {
            case '1': icone  = '<div class="fa fa-check-circle success"></div>';break;
            case '2': icone  = '<div class="fa fa-exclamation-triangle danger"></div>';break;
            case '3': icone  = '<div class="fa fa-exclamation-triangle warning"></div>';break;
            default:icone = pendencia;
        }
        return icone;
    }


    function trPendencia(pendencia) {
        var icone = "";
        switch (pendencia) {
            case 'Sem Pendência': icone  = '<div class="fa fa-check-circle success"></div>';break;
            case 'Pendência':     icone  = '<div class="fa fa-exclamation-triangle danger"></div>' ;break;
            case 'Alerta':        icone  = '<div class="fa fa-exclamation-triangle warning"></div>'  ;break;
            default:;
        }
        return icone;
    }

    $("#xls-pendecias").on("click", function (evt) {
        evt.preventDefault();
        var form = $('#form-pesquisa-pendencias').serialize();
        window.location.assign("/par3/par3.php?modulo=painel/xlspendencias&acao=A&" + form);
    });

    $(document).ready(function(){
        $('.js-atualizar-pendencias').on('click',function() {
            // console.log($(this).attr('id'));
            atualizaPendencias($(this).attr('id'));
        });
    });


    function atualizaPendencias( inuid ){
        // $('#loading-pendencias').fade();
        $('#div-table-pendencias').fadeOut();
        $('.progress').show();
        $("#modal-visualiza-pendencia").modal("show");

        jQuery('[name="inuid"]').val(inuid);
        jQuery('.text-pendencia').html('Atualizando Pendência - Conselho CAE');

        var action  = '&requisicao=atualizaPendencias&tipo=cae&inuid='+inuid;
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="cae"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - Obras PAR');
                jQuery('.progress-bar').html('14%');
                jQuery('.progress-bar').css('width', '14%');
                setTimeout(pendenciaPAR,1000);
            }
        });
    }

    function pendenciaPAR() {
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=par&inuid='+inuid;

        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="par"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - CACS');
                jQuery('.progress-bar').html('28%');
                jQuery('.progress-bar').css('width', '28%');
                setTimeout(pendenciaCACS,1000);
            }
        });
    }

    function pendenciaCACS(){
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=cacs&inuid='+inuid;

        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="cacs"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - PNE');
                jQuery('.progress-bar').html('42%');
                jQuery('.progress-bar').css('width', '42%');
                setTimeout(pendenciaPNE,1000);
            }
        });
    }

    function pendenciaPNE(){
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=pne&inuid='+inuid;

        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="pne"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - Habilita');
                jQuery('.progress-bar').html('56%');
                jQuery('.progress-bar').css('width', '56%');
                setTimeout(pendenciaHabilita,1000);
            }
        });
    }

    function pendenciaHabilita(){
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=habilita&inuid='+inuid;

        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="habilita"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - Monitoramento PAR');
                jQuery('.progress-bar').html('70%');
                jQuery('.progress-bar').css('width', '70%');
                setTimeout(pendenciaMonitoramento,1000);
            }
        });
    }

    function pendenciaMonitoramento(){
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=monitoramento&inuid='+inuid;

        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="monitoramento"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando Pendência - SIOPE');
                jQuery('.progress-bar').html('84%');
                jQuery('.progress-bar').css('width', '84%');
                setTimeout(pendenciaSiope,1000);
            }
        });
    }

    function pendenciaSiope(){
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = jQuery('[name="inuid"]').val();
        var action  = '&requisicao=atualizaPendencias&tipo=siope&inuid='+inuid;

        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                //jQuery("#div_debug").html(resp);
                jQuery('[name="siope"]').val(resp);
                jQuery('.text-pendencia').html('Atualizando View de Pendências');
                jQuery('.progress-bar').html('100%');
                jQuery('.progress-bar').css('width', '100%');
                setTimeout(atualizaPendenciaView,1000);
            }
        });
    }

    function atualizaPendenciaView() {
        var caminho = window.location.href;
        caminho = "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A";
        var inuid = jQuery('[name="inuid"]').val();
        var cae = jQuery('[name="cae"]').val();
        var par = jQuery('[name="par"]').val();
        var cacs = jQuery('[name="cacs"]').val();
        var pne = jQuery('[name="pne"]').val();
        var habilita = jQuery('[name="habilita"]').val();
        var monitoramento = jQuery('[name="monitoramento"]').val();
        var siope = jQuery('[name="siope"]').val();

        var action  = '&requisicao=atualizaPendenciaView&tipo=siope&inuid='+inuid+'&cae='+cae+'&par='+par+'&cacs='+cacs+'&pne='+pne+'&habilita='+habilita+'&monitoramento='+monitoramento+'&siope='+siope;

        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: true,
            success: function (resp) {
                jQuery('.text-pendencia').html('Pendências Atualizadas com Sucesso!');
                $('.progress').hide();
            }
        });
        fecharPendencia();
    }

    function fecharPendencia() {
        var inuid = jQuery('[name="inuid"]').val();
        var arr = [];$("input:checkbox[name='iboxpendencias[tipid][]']:checked").each(function(){
            arr.push($(this).val());
        });

        jQuery.ajax({
            url: '/par3/par3.php?modulo=painel/listapendencias&acao=A',
            data: {
                datatype : 'json',
                tipid    : arr,
                inuid    : inuid
            },
            success: function (data) {
                var res = JSON.parse(data);
                if (Array.isArray(res.arPendencias)) {
                    $.each(res.arPendencias, function (index, pendencia) {
                        var inuid = pendencia.inuid;
                        $('#td_situacao_' + inuid).html(trPendencia(pendencia.descricao));
                        $('#td_cae_' + inuid).html(iconPendencia(pendencia.cae));
                        $('#td_cacs_' + inuid).html(iconPendencia(pendencia.cacs));
                        $('#td_habilitacao_' + inuid).html(iconPendencia(pendencia.habilitacao));
                        $('#td_monitoramento_par_' + inuid).html(iconPendencia(pendencia.monitoramento_par));
                        $('#td_obras_par_' + inuid).html(iconPendencia(pendencia.obras_par));
                        $('#td_pne_' + inuid).html(iconPendencia(pendencia.pne));
                        $('#td_contas_' + inuid).html(iconPendencia(pendencia.contas));
                        $('#td_siope_' + inuid).html(iconPendencia(pendencia.siope));
                    });
                    redimensionarThead();
                }
                $('#tr_'+inuid).addClass('success');
                $('#div-table-pendencias').fadeIn();
                redimensionarThead();
                setTimeout(function(){
                    $('#tr_'+inuid).removeClass('success');
                    $('.text-pendencia').html('')
                },1000);
            }
        });
        $('[name="inuid"]').val('');
        $('[name="cae"]').val('');
        $('[name="par"]').val('');
        $('[name="cacs"]').val('');
        $('[name="pne"]').val('');
        $('[name="habilita"]').val('');
        $('[name="monitoramento"]').val('');
        $('[name="siope"]').val('');
    }

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

    $('#bt_pendencia_pesquisa').on('click',function(evt) {
        evt.preventDefault();
        renderLista();
    });

    function renderLista() {
        var itrid = $('[name=itrid]').val();
        var descricao = $('[name=descricao-pendencias]').val();
        var tbody     = $('#table-pendencias').find('#table-pendencia-body');
        var arr = [];$("input:checkbox[name='iboxpendencias[tipid][]']:checked").each(function(){
            arr.push($(this).val());
        });
        var form = $('#form-pesquisa-pendencias').serialize();
        $.ajax({
            url: '/par3/par3.php?modulo=painel/listapendencias&acao=A&'+form,
            data: {
                datatype     : 'json',
                descricao    : descricao,
                tipid        : arr
            },
            beforeSend: function(msg) {
                $('#loading').hide();
            },
            success: function(data) {
                var res = JSON.parse(data);
                $('#pendencia-full_count').html(0);
                if(Array.isArray(res.arPendencias)) {
                    $('#pendencia-full_count').html(res.arPendencias[0].full_count);
                    tbody.html('');
                    $.each(res.arPendencias, function (index, pendencia) {
                        var inuid = pendencia.inuid;
                        tbody.append(
                            "<tr id='tr_"+pendencia.inuid+"'>\n" +
                            "<td class=\"text-center col-lg-2\">" +
                            "<a  class='success'" +
                            " style='margin: 5px;' "+
                            "title='Clique para acessar a pendência'"+
                            "target='_blank'"+
                            "href='/par3/par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid="+inuid+"'>"+
                            "<i class='fa fa-link'></i>"+
                            "</a> " +
                            "<a class='js-atualizar-pendencias info'"+
                            " style='margin: 5px;' "+
                            "onclick='atualizaPendencias("+pendencia.inuid+")' " +
                            "title='Clique para atualizar as pendências' " +
                            "id='"+pendencia.inuid+"'>"+
                            "<i class='fa fa-refresh'></i>" +
                            "</a>"+
                            "</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_situacao_"+pendencia.inuid+"'>"+trPendencia(pendencia.descricao)+"</td>\n" +
                            "<td class=\"text-center col-lg-3\">"+pendencia.entidade+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_cae_"+pendencia.inuid+"'>"+iconPendencia(pendencia.cae)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_cacs_"+pendencia.inuid+"'>"+iconPendencia(pendencia.cacs)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_habilitacao_"+pendencia.inuid+"'>"+iconPendencia(pendencia.habilitacao)+"</td>\n" +
                            "<td class=\"text-center col-lg-3\" id='td_monitoramento_par_"+pendencia.inuid+"'>"+iconPendencia(pendencia.monitoramento_par)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_obras_par_"+pendencia.inuid+"'>"+iconPendencia(pendencia.obras_par)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_pne_"+pendencia.inuid+"'>"+iconPendencia(pendencia.pne)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_contas_"+pendencia.inuid+"'>"+iconPendencia(pendencia.contas)+"</td>\n" +
                            "<td class=\"text-center col-lg-2\" id='td_siope_"+pendencia.inuid+"'>"+iconPendencia(pendencia.siope)+"</td>\n" +
                            "            </tr>"
                        );
                    });
                    redimensionarThead();
                }else {
                    tbody.html('');
                }
            }
        });
        redimensionarThead();
    }
</script>