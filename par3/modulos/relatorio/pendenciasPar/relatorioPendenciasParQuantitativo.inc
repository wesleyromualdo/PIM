<?php
function colunasOrigem(){
    return array(
        array(
            'codigo'    => 'estuf',
            'descricao' => 'UF'
        ),
        array(
            'codigo'    => 'mundescricao',
            'descricao' => 'Municípios'
        ),
        array(
            'codigo'    => 'esddsc',
            'descricao' => 'Situação'
        ),
        array(
            'codigo'    => 'obras_par',
            'descricao' => 'Pendência de Obras do PAR'
        ),
        array(
            'codigo'    => 'cacs',
            'descricao' => 'CACS'
        ),
        array(
            'codigo'    => 'habilitacao',
            'descricao' => 'Habilitação'
        ),
        array(
            'codigo'    => 'cae',
            'descricao' => 'CAE'
        ),
        array(
            'codigo'    => 'contas',
            'descricao' => 'Prestação de Contas'
        ),
        array(
            'codigo'    => 'monitoramento_par',
            'descricao' => 'Monitoramento PAR'
        ),
        array(
            'codigo'    => 'pne',
            'descricao' => 'PME/PEE'
        ),
        array(
            'codigo'    => 'siope',
            'descricao' => 'SIOPE'
        ),
    );
}

function colunasDestino(){
    return array(
        array(
            'codigo'    => 'estuf',
            'descricao' => 'UF'
        ),
        array(
            'codigo'    => 'mundescricao',
            'descricao' => 'Municípios'
        ),
        array(
            'codigo'    => 'esddsc',
            'descricao' => 'Situação'
        ),
        array(
            'codigo'    => 'obras_par',
            'descricao' => 'Pendência de Obras do PAR'
        ),
        array(
            'codigo'    => 'cacs',
            'descricao' => 'CACS'
        ),
        array(
            'codigo'    => 'habilitacao',
            'descricao' => 'Habilitação'
        ),
        array(
            'codigo'    => 'cae',
            'descricao' => 'CAE'
        ),
        array(
            'codigo'    => 'contas',
            'descricao' => 'Prestação de Contas'
        ),
        array(
            'codigo'    => 'monitoramento_par',
            'descricao' => 'Monitoramento PAR'
        ),
        array(
            'codigo'    => 'pne',
            'descricao' => 'PME/PEE'
        ),
        array(
            'codigo'    => 'siope',
            'descricao' => 'SIOPE'
        ),
    );
}

function montaFiltrosDinamicos($arrayColunasDest)
{
    global $simec;
    $options = array(
    	'Com Pendência' => 'Possui',
    	'Sem Pendência' => 'Não Possui',
    	''  => 'Todos',
    );
    $arrayColunasDest = array_filter($arrayColunasDest);
    foreach($arrayColunasDest as $coluna){
        if(!in_array($coluna['codigo'], array('estuf','mundescricao','esddsc'))) {
            $valor = $_REQUEST['sfiltro']['pendencia'][$coluna['codigo']] != '' ?
                $_REQUEST['sfiltro']['pendencia'][$coluna['codigo']] : '';
            echo '<div class="col-lg-4" align="">';
            echo $simec->radio(
                    'sfiltro[pendencia]['.$coluna['codigo'].']',
                    $coluna['descricao'],
                    $valor,
                    $options,
                    array(),
                    array('style'=>'')
            );
            echo '</div>';
        }
    }
}

$relatorioPendencias = new Par3_Model_RelatorioPendenciaQuantitativo();

switch($_REQUEST['requisicao']){
    case 'montaFiltrosDinamicos':
        ob_clean();
        $filtros = json_decode(str_replace('\\', '', $_REQUEST['filtros']), true);
        $filtros = is_array($filtros) ? $filtros : array();
        foreach($filtros as $k1 => $v1){
            if($k1 != 'zero'){
        $filtros[$k1]['descricao'] = ($v1['descricao']);
            }
        }
        montaFiltrosDinamicos($filtros);
    die;
    break;

    case 'carregaMunicipios':
        $municipio = new Territorios_Model_Municipio();
        $response = $municipio->lista(['muncod', 'mundescricao'], ["estuf = '{$_REQUEST['estuf']}'"]);
        print simec_json_encode($response);
    die;
    break;

    case 'salvarFiltro':
        ob_clean();
        global $db;
        $filtros = ($_POST['filtros']);
        $desc = ($_POST['descFiltro']);
        $sql  = "INSERT INTO par3.relatorio_quantitativo_filtros (usucpf, descricao, filtros)";
        $sql .= " VALUES('{$_SESSION['usucpf']}', '$desc', '$filtros')";
        echo $sql;
        $db->executar($sql);
        $db->commit();
    die;
    break;

    case 'submitForm':
        $sql = "SELECT filtros FROM par3.relatorio_quantitativo_filtros where codigo = {$_POST['idFiltro']}";
        $filtros = $db->pegaUm($sql);
        echo $filtros;
    die;
    break;
    case 'carregarMunicipiosAnalise':
        //lista todos os municípios relacionados ao Estado(estuf) informado
        ob_clean();
        $municipio = new Territorios_Model_Municipio();//instancia a classe município
        echo simec_json_encode($municipio->carregar($municipio->pegarSQLSelect($_GET['estuf'])));
        die;
        break;
    case 'carregarTodosMunicipios':
        //lista todos os municípios relacionados ao Estado(estuf) informado
        ob_clean();
        $municipio = new Territorios_Model_Municipio();//instancia a classe município
        $arrMun = $municipio->recuperarListaMunicipios();
        echo simec_json_encode($arrMun);
        die;
        break;
}

if (count($_REQUEST['sfiltro']) > 0) {
    $relatorioPendencias->setWhere($_REQUEST['sfiltro']);
}

if ($_REQUEST['requisicao'] == 'xls') {
    $relatorioPendencias->buildDataGrid($xlsDownload = true);
    header("Content-Disposition: attachment; filename=Relatorio_Pendencias_PAR_Quantitativo.xls");
    exit;
}


include APPRAIZ . 'includes/Agrupador.php';
require APPRAIZ . 'includes/cabecalho.inc';
?>
<style type="text/css">
    .fa-file-excel-o{
        font-size:15px;
        color: #fff;
        cursor: pointer;
    }
    .fa-file-excel-o:hover{
        color: #fff;
        -webkit-box-shadow: 0px 0px 10px 0px rgba(0,0,0,0.5);
        -moz-box-shadow:    0px 0px 10px 0px rgba(0,0,0,0.5);
        box-shadow:         0px 0px 10px 0px rgba(0,0,0,0.5);
    }
    .footer{
        bottom: -38px;
    }

    #wrapper{
       width: 103%;
    }

    .relatorio-pendencias .fa-exclamation-triangle {
        color: darkred;
    }
    .relatorio-pendencias .fa-check {
        color: green;
    }
    div#meusFiltros_chosen {
        margin-left: 18px;
    }

    @media print{
        .notprint{
            display: none
        }
        .div_rolagem{
            display: none
        }
        .div_rol{
            display: none
        }
    }

    @media screen {
        .notscreen { display: none; }
        .div_rol{display: none;}
    }
    .div_rolagem{
        overflow-x: auto; overflow-y: auto; height: 50px;
    }
    .div_rol{
        overflow-x: auto; overflow-y: auto; height: 50px;
    }
</style>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/agrupador.js"></script>
<div class="ibox" style="width:98%;">
<form method="post" name="formulario" id="formulario" class="form form-horizontal notprint">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="muncod_hid" value="<?php echo $_REQUEST['sfiltro']['mundescricao'];?>">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-10" >
                    <h3 id="entidade">Programas do PAR - Relatório Quantitativo de Pendências</h3>
                </div>
            </div>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-6" >
                    <div id="linha_agrupadores">
                        <div class="col-lg-1 text-left" ><label for="calunas">Colunas:</label></div>
                        <div class="col-lg-offset-2 text-center">
                            <?php
                            if(isset($_REQUEST['cols'])){//recupera as colunas selecionadas do filtro
                                $colunasDestino = colunasOrigem();
                                $colunasSalvas = array();
                                foreach($_REQUEST['cols'] as $colunasPost){
                                    foreach($colunasDestino as $index => $colunaValores) {
                                        if($colunaValores['codigo'] == $colunasPost){
                                            array_push($colunasSalvas,$colunasDestino[$index]);
                                            break;
                                        }
                                    }
                                }
                            }
                            #Montar e exibe colunas
                            $arrayColunasOrig = colunasOrigem();
                            $arrayColunasDest = isset( $colunasSalvas ) ? $colunasSalvas : colunasDestino();
                            $colunas = new Agrupador('formulario');
                            $colunas->setOrigem('nCols', null, $arrayColunasOrig);
                            $colunas->setDestino('cols', null, $arrayColunasDest);
                            $colunas->exibir();
                            ?>
                        </div>
                    </div>
                </div>
                <div class="col-lg-6">
                    <?php
                    echo $simec
                        ->radio(
                            'sfiltro[itrid]',
                            'Esfera',
                            ($_REQUEST['sfiltro']['itrid']?:0),
                            [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'],
                            array('style'=>''),
                            ['label-size' => 2]
                        );
                    ?>

                    <?php
                    $sql = Territorios_Model_Estado::pegarSQLSelect($_REQUEST);
                    $arrAttr = ['data-placeholder' => 'Unidade Federativa','multiple' => 'multiple'];
                    $class   = ['label-size' => 2,'input-size' => '8'];
                    echo $simec->select('sfiltro[estuf]', 'UF', $_REQUEST['sfiltro']['estuf'], $sql, $arrAttr,$class);
                    ?>
                    <div id="linha_muncod">
                        <?php
                        $arrMunicipio = array();
                        if ($_REQUEST['sfiltro']['mundescricao'] && $_REQUEST['sfiltro']['itrid'] != 1) {
                            $arrMundescricao = $_REQUEST['sfiltro']['mundescricao'];
                            $_REQUEST['sfiltro']['mundescricao'] = tratarArrayParaMultiSelect($arrMundescricao);
                            $municipio = new Territorios_Model_Municipio();//instancia a classe município
                            //lista todos os municípios relacionados ao Estado(estuf) informado
                            $arrMunicipio = $municipio->pegarSQLSelect($_REQUEST['sfiltro']['estuf']);
                        }

                        $mundescricao = $_REQUEST['sfiltro']['mundescricao'];
                        $arrAttr = ['placeHolder' => 'Municipio','multiple' => 'multiple'];
                        $class   = ['label-size' => 2,'input-size' => '8'];
                        echo $simec->select('sfiltro[mundescricao]', 'Município', $mundescricao,$arrMunicipio,$arrAttr,$class);
                        ?>
                    </div>
                </div>
            </div>
            <hr>
            <h3 align="center">Filtros Dinâmicos</h3>
            <div id="div_filtrosDinamicos" class="row">
                <?php
                montaFiltrosDinamicos($arrayColunasDest);
                ?>
            </div>
            <hr>
            <div class="row">
                    <?php
                    $mEstadoDocumento = new Workflow_Model_EstadoDocumento();
                    $rsEsd = $mEstadoDocumento->getArrayEstadoDocumentoPAR3();
                    $arrAttr = array('maxlength' => '255','multiple' => 'multiple','class' => 'col-lg-offset-3');
                    $class   = array('input-size' => '6');
                    echo $simec->select (
                            'sfiltro[esdid]',
                            'Situação',
                            $_REQUEST['sfiltro']['esdid'],
                            $rsEsd, $arrAttr,$class
                    );
                    ?>
                </div>
            <div class="ibox-footer">
                <div class="row">
                    <div class="col-lg-5 col-lg-offset-4">
                        <?php
                            $sql  = "SELECT codigo, descricao ";
                            $sql .="FROM par3.relatorio_quantitativo_filtros where usucpf = '{$_SESSION['usucpf']}'";
                            $arrAttr = ['data-placeholder' => 'Meus Filtros'];
                            echo $simec->select('meusFiltros', null, $_REQUEST['meusFiltros'], $sql, $arrAttr);
                        ?>
                        <button type="submit" class="btn btn-success buscar" style="width:110px;">
                            <i class="fa fa-search"></i> Filtrar
                        </button>

                        <button class="btn btn-info modal-filtro" data-toggle="modal" data-target="#modalFiltros">
                            <i class="fa fa-floppy-o"></i> Salvar Filtros
                        </button>

                        <button type="submit" class="btn btn-primary xls" style="width:110px;">
                            <i class="fa fa-file-excel-o"></i> XLS
                        </button>

                        <button type="submit"
                                class="btn btn-warning"
                                style="width:110px;"
                                onclick="javascript:$('#div_cabecalho').css('display', ''); window.print();">
                            <i class="glyphicon glyphicon-print"></i> Imprimir
                        </button>
                    </div>

                </div>
            </div>
        </div>
</form>
</div>

<div class="ibox">
    <div class="ibox-content" style="width:98%; height:auto; overflow-x:auto; overflow-y:auto;">
        <div id="div_cabecalho" class="row col-lg-offset-2" style="display:none;">
            <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
                <tr bgcolor="#ffffff">
                    <td valign="top" width="50" rowspan="2">
                        <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                    </td>
                    <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    </td>
                    <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                        Impresso por: <b> <?=$_SESSION['usunome'];?></b><br/>
                        Hora da Impressão: <?=date( 'd/m/Y - H:i:s' );?><br />
                    </td>
                </tr>
            </table>
            <hr style="color:black; width:95%; margin-left:-18px;">
        </div>

        <div class="clearfix" style="margin-bottom:10px;"></div>
        <div class="relatorio-pendencias">
        <?php
            if( $relatorioPendencias->showGrid ) {
                $relatorioPendencias->buildDataGrid();
            }else{
        ?>
                <div class="alert alert-info" role="alert">
                    <p class="text-center">Selecione as colunas que deseja ver no seu relatório.</p>
                </div>
        <?php
            }
        ?>
        </div>
    </div>
</div>

<div class="ibox float-e-margins animated modal" id="modalFiltros" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="" id="" class="form form-horizontal">
            <div class="modal-content animated flipInY">
                <div class="ibox-title" tipo="integrantes">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                        <span aria-hidden="true">&times;</span>
                    </button>
                    <h3>Salvar Filtros.</h3>
                </div>
                <div class="ibox-content">
                    <input type="hidden" name="req" value="salvar"/>
                    <div id="conteudo-modal">
                        Nome do Filtro: <input type="text" name="filtroDescricao" id="filtroDescricao">
                    </div>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-2 col-md-offset-2 col-lg-offset-2">
                        <button type="submit" class="btn btn-success salvar-filtro">
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>


<script type="text/javascript">
    $(function(){

        $(".xls").on("click", function(){
            $("#requisicao").val("xls");
            $('#cols option').prop('selected', true);
            $("#formulario").submit();
        });

        $(".buscar").on("click", function(){
            $("#requisicao").val("");
            $('#cols option').prop('selected', true);
            $("#formulario").submit();

        });

        $('.modal-filtro').on('click', function(event){
            event.preventDefault();
        });

        $(".salvar-filtro").on("click", function(event) {
            $("#requisicao").val("");
            $('#cols option').prop('selected', true);
            var formData = $("#formulario").serializeArray();
            var descFiltro = $('#filtroDescricao').val();
            data = JSON.stringify(formData);
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=salvarFiltro&descFiltro="+descFiltro+"&filtros="+data,
                async: false,
                success: function(result) {
                }
            });

        });

        $('#meusFiltros').change(function() {
            var idFiltro = $(this).val();

            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=submitForm&idFiltro="+idFiltro,
                async: true,
                success: function(result) {
                    var form = $('<form action="" name="form-filtro" id="form-filtro" method="post"></form>')
                        .appendTo('body');
                    var formData = JSON.parse(result);
                    $.each(formData, function (key, value) {
                        $("<input type='hidden' value='" + value.value + "' >")
                            .attr("id", value.name)
                            .attr("name", value.name)
                            .appendTo("#form-filtro");
                    });
                    $('#form-filtro').submit();
                }
            });
        });

        $( document ).ready(function() {
            var tds = $('td');
            tds.each(function() {
                if( $(this).text() == 'Com Pendência' ){
                    $(this).html('<div class="fa fa-exclamation-triangle"></div>');
                }
                if( $(this).text() == 'Sem Pendência' ) {
                    $(this).html('<div class="fa fa-check"></div>');
                }
            });
        });

        var muncod_hid = jQuery('[name="muncod_hid"]').val();

        jQuery('#sfiltro-mundescricao').append(new Option('', ''));

        // if (jQuery('[name="sfiltro[estuf]"]').val() != '') {
        //     carregarMunicipio( jQuery('[name="sfiltro[estuf]"]').val(), muncod_hid );
        // }

        // $('#sfiltro-itrid').change(function(){
        //     if($(this).val()=='1,3'){
        //         $('#linha_muncod').hide();
        //     }else{
        //         $('#linha_muncod').show();
        //     }
        // });

        // $('#sfiltro-estuf').change(function(){
        //     if ( jQuery('[name="sfiltro[mundescricao]"]').val() != 'E' ) {
        //         var estuf = jQuery(this).val();
        //         carregarMunicipio( estuf, muncod_hid );
        //     }
        // });

        $('.btn-mover-coluna').click(function(){
            var retorno = '{"zero":0';
            var count = 0;
        	$('#cols option').each(function(){
        		retorno += ',"'+count+'":{"codigo":"'+$(this).val()+'","descricao":"'+$(this).html()+'"}';
        		count = parseFloat(count)+1;
        	});
        	retorno += '}';

        	$.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=montaFiltrosDinamicos&filtros="+retorno,
                async: false,
                success: function(result) {
                    $('#div_filtrosDinamicos').html(result);
                }
            });
        });
    });




    //
    // function carregarMunicipio( estuf, muncod_hid ) {
    //     if ( estuf != '' ) {
    //         var options = jQuery('#sfiltro-mundescricao');
    //         jQuery.ajax({
    //             type: "POST",
    //             url: window.location.href,
    //             data: "requisicao=carregaMunicipios&estuf="+estuf,
    //             async: false,
    //             success: function(result) {
    //                 options.empty();
    //                 options.append(new Option('', ''));
    //                 var result = JSON.parse(result);
    //                 $.each(result, function() {
    //                     options.append(new Option(this.mundescricao, this.mundescricao));
    //                 });
    //                 options.focus();
    //                 if (muncod_hid) {
    //                     options.val(muncod_hid);
    //                 }
    //                 options.trigger('chosen:updated');
    //             }
    //         });
    //     }
    // }






    jQuery('#sfiltro-estuf').change(function(){
        if($('#sfiltro-itrid').val() == '1,3') {
            return;
        }else{
            if(!Array.isArray($(this).val())){
                listarMunicipios();
            }else{
                carregarMunicipioAnalise($('#sfiltro-estuf').val(),$('#sfiltro-mundescricao').val());
            }
        }
    });

    function listarMunicipios()
    {
        console.log('testes');
        var options = jQuery('#sfiltro-mundescricao');
        options.empty();
        options.trigger('chosen:updated');
        options.append(new Option('', ''));
        jQuery.ajax({
            type: "GET",
            url: window.location.href,
            data: { requisicao:'carregarTodosMunicipios'},
            success: function(retorno){
                options.append(new Option('', ''));
                $.each(JSON.parse(retorno), function() {
                    options.append(new Option(this.descricao, this.codigo));
                });
                // options.val(muncod);
                options.trigger('chosen:updated');
            }
        });
    }

    function carregarMunicipioAnalise(estuf, muncod) {
        if(estuf != '' && $('#sfiltro-itrid:checked').val() != 1) {
            var options = jQuery('#sfiltro-mundescricao');
            options.empty();
            options.trigger('chosen:updated');
            if(!Array.isArray(estuf)){
                return;
            }
            $('#loading').hide();
            options.append(new Option('', ''));
            jQuery.ajax({
                type: "GET",
                url: window.location.href,
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

    $('#sfiltro-mundescricao').one('focus',function() {
        console.log('on focus');
        if(Array.isArray($('#sfiltro-estuf').val())){
            return;
        }
        listarMunicipios();
    });

    $('#linha_muncod').one('click',function() {
        console.log('on click');
        if(Array.isArray($('#sfiltro-estuf').val())){
            return;
        }
        listarMunicipios();
    });

    $('[name="sfiltro[itrid]"]').on('change',function() {
        if($(this).val() == 1){
            $('#linha_muncod').hide();
            $('#sfiltro-mundescricao').empty();
            $('#sfiltro-mundescricao').trigger("chosen:updated");
        }else{
            $('#linha_muncod').show();
            carregarMunicipioAnalise($('#sfiltro-estuf').val(),$('#sfiltro-mundescricao').val());
        }
    });
</script>
