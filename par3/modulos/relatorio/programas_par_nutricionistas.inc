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
                'codigo'    => 'entcpf',
                'descricao' => 'CPF'
            ),
            array(
                'codigo'    => 'entnome',
                'descricao' => 'Nome'
            ),
            array(
                'codigo'    => 'dancrn',
                'descricao' => 'CRN'
            ),
            array(
                'codigo'    => 'dancrnuf',
                'descricao' => 'Região do CRN'
            ),
            array(
                'codigo'    => 'vndatavinculacao',
                'descricao' => 'Data da Vinculação'
            ),
            array(
                'codigo'    => 'vndatadesvinculacao',
                'descricao' => 'Data da Desvinculação'
            ),
            array(
                'codigo'    => 'sndescricao',
                'descricao' => 'Situação'
            ),
            array(
                'codigo'    => 'danemailprincipal',
                'descricao' => 'E-mail Principal'
            ),
            array(
                'codigo'    => 'danemailalternativo',
                'descricao' => 'E-mail alternativo'
            ),
            array(
                'codigo'    => 'dantelefonefixo',
                'descricao' => 'Tel.Comercial'
            ),
            array(
                'codigo'    => 'dantelefonecelular',
                'descricao' => 'Celular'
            ),
            array(
                'codigo'    => 'esfera',
                'descricao' => 'Esfera'
            ),
            array(
                'codigo'    => 'tendsc',
                'descricao' => 'Cargo/função'
            ),
            array(
                'codigo'    => 'dntvdescricao',
                'descricao' => 'Tipo de Vínculo'
            )
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
                'codigo'    => 'entnome',
                'descricao' => 'Nome'
            ),
            array(
                'codigo'    => 'danemailprincipal',
                'descricao' => 'E-mail Principal'
            ),
            array(
                'codigo'    => 'dantelefonecelular',
                'descricao' => 'Celular'
            ),
            array(
                'codigo'    => 'esfera',
                'descricao' => 'Esfera'
            ),
            array(
                'codigo'    => 'tendsc',
                'descricao' => 'Cargo/função'
            ),
            array(
                'codigo'    => 'dntvdescricao',
                'descricao' => 'Tipo de Vínculo'
            )
        );
    }
if(isset($_POST['cols'])){
    $colunasDestino = colunasDestino();
    $colunasSalvas = array();
    foreach($colunasDestino as $index => $value) {
        foreach($_POST['cols'] as $colunasPost){
            if($value['codigo'] == $colunasPost){
                array_push($colunasSalvas,$colunasDestino[$index]);
            }
        }
    }

}
    $relatorioNutricionista = new Par3_Model_RelatorioNutricionista();

    if (count($_POST['sfiltro']) > 0) {
        $relatorioNutricionista->setWhere($_POST['sfiltro']);
    }

    if ($_POST['requisicao'] == 'xls') {
        $relatorioNutricionista->buildDataGrid($xlsDownload = true);
        exit;
    }

    switch ($_REQUEST['requisicao']) {
        case 'carregaMunicipios':
            $municipio = new Territorios_Model_Municipio();
            $response = $municipio->lista(['muncod', 'mundescricao'], ["estuf = '{$_REQUEST['estuf']}'"]);
            print simec_json_encode($response);
            die;
            break;
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

    @media print{
        .notprint{
            display: none
        }
        .div_rolagem{
            display: none
        }
        .div_rol{
            display: 'none'
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

<form method="post" name="formulario" id="formulario" class="form form-horizontal notprint">
    <input type="hidden" name="requisicao" id="requisicao" value="">
    <input type="hidden" name="muncod_hid" value="<?php echo $_POST['sfiltro']['mundescricao'];?>">
    <div class="ibox">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-10" >
                    <h3 id="entidade">Programas do PAR - Nutricionistas</h3>
                </div>
            </div>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-2 text-right" ><label for="calunas">Colunas</label></div>
                <div class="col-lg-10 text-center">
                    <?php

                        if(isset($_POST['cols'])){//recupera as colunas selecionadas do filtro
                            $colunasDestino = colunasOrigem();
                            $colunasSalvas = array();
                            foreach($_POST['cols'] as $colunasPost){
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
                        $arrayColunasDest = isset($colunasSalvas)?$colunasSalvas:colunasDestino();
                        $colunas = new Agrupador('formulario');
                        $colunas->setOrigem('nCols', null, $arrayColunasOrig);
                        $colunas->setDestino('cols', null, $arrayColunasDest);
                        $colunas->exibir();
                    ?>
                </div>
            </div>

            <div class="row">
                <div class="col-lg-2 text-right"><label for="estuf">UF</label></div>
                <div class="col-lg-10 text-center">
                    <?php
                        $sql = Territorios_Model_Estado::pegarSQLSelect($_POST);
                        $arrAttr = ['data-placeholder' => 'Unidade Federativa'];
                        echo $simec->select('sfiltro[estuf]', null, $_POST['sfiltro']['estuf'], $sql, $arrAttr);
                    ?>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-2 text-right"><label for="mundescricao">Município</label></div>
                <div class="col-lg-10 text-center">
                    <?php
                        $mundescricao = simec_htmlentities($_POST['sfiltro']['mundescricao']);
                        $arrAttr = ['placeHolder' => 'Municipio'];
                        echo $simec->select('sfiltro[mundescricao]', null, $mundescricao, $arrAttr);
                    ?>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-2 text-right"><label for="itrid">Esfera</label></div>
                <div class="col-lg-10 text-center">
                    <?php
                        $itrid = simec_htmlentities($_POST['sfiltro']['itrid'] );
                        $sql = 'SELECT \'1,3\' AS codigo, \'Estadual\' AS descricao UNION SELECT \'2\' AS codigo, \'Municipal\' AS descricao';
                        $arrAttr = ['data-placeholder' => 'Esfera'];
                        echo $simec->select('sfiltro[itrid]', null, $itrid, $sql, $arrAttr);
                    ?>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-2 text-right"><label for="itrid">Cargo</label></div>
                <div class="col-lg-10 text-center">
                    <?php
                        $tenid = simec_htmlentities($_POST['sfiltro']['tenid']);
                        $sql = 'select tenid as codigo, tendsc as descricao from par3.tipoentidade where tenid in (7, 8,16)';
                        $arrAttr = ['data-placeholder' => 'Cargo'];
                        echo $simec->select('sfiltro[tenid]', null, $tenid, $sql, $arrAttr);
                    ?>
                </div>
            </div>
            <div class="row">
                <div class="col-lg-2 text-right"><label for="itrid">Possui Vínculo?</label></div>
                <div style="margin-left: 265px;">
                    <?php
                    $vnstatus = simec_htmlentities($_POST['sfiltro']['vnstatus']);
                    echo $simec->radio('sfiltro[vnstatus]',null,$vnstatus?$vnstatus:'',array('A' => 'Sim','I' => 'Não','' => 'Todos'));
                    ?>
                </div>
            </div>
            <br>
            <div class="ibox-footer">
                <div class="row">
                    <div class="col-lg-5 col-lg-offset-4">
                        <button type="submit" class="btn btn-success salvar" style="width:110px;"><i class="fa fa-search"></i> Filtrar</button>
                        <button type="submit" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> XLS</button>
                        <button type="submit" class="btn btn-warning" style="width:110px;" onclick="javascript:$('#div_cabecalho').css('display', ''); window.print();"><i class="glyphicon glyphicon-print"></i> Imprimir </button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</form>

<div class="ibox-content" style="width:98%; height:auto; overflow-x:auto; overflow-y:auto;">
    <div id="div_cabecalho" class="row col-lg-offset-2" style="display:none;">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    DIRETORIA DE AÇÕES EDUCACIONAIS - DIRAE <br>
                    COORDENAÇÃO - GERAL DO PROGRAMA DE ALIMENTAÇÃO ESCOLAR - CGPAE
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

    <?php
        if( $relatorioNutricionista->showGrid ){
            $relatorioNutricionista->buildDataGrid();
        }else{
    ?>
            <div class="alert alert-info" role="alert">
                <p class="text-center">Selecione as colunas que deseja ver no seu relatório.</p>
            </div>
    <?php
        }
    ?>
</div>

<script type="text/javascript">
    $(function(){

        $(".xls").on("click", function(){
            $("#requisicao").val("xls");
            $('#cols option').prop('selected', true);
            $("#formulario").submit();
        });

        $(".salvar").on("click", function(){
            $("#requisicao").val("");
            $('#cols option').prop('selected', true);
            $("#formulario").submit();
        });

        var muncod_hid = jQuery('[name="muncod_hid"]').val();

        jQuery('#sfiltro-mundescricao').append(new Option('', ''));

        if (jQuery('[name="sfiltro[estuf]"]').val() != '') {
            carregarMunicipio( jQuery('[name="sfiltro[estuf]"]').val(), muncod_hid );
        }

        jQuery('[name="sfiltro[estuf]"]').change(function(){
            if ( jQuery('[name="sfiltro[mundescricao]"]').val() != 'E' ) {
                var estuf = jQuery(this).val();
                carregarMunicipio( estuf, muncod_hid );
            }
        });
    });

    function carregarMunicipio( estuf, muncod_hid ) {
        if ( estuf != '' ) {
            var options = jQuery('#sfiltro-mundescricao');
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=carregaMunicipios&estuf="+estuf,
                async: false,
                success: function(result) {
                    options.empty();
                    options.append(new Option('', ''));
                    var result = JSON.parse(result);
                    $.each(result, function() {
                        options.append(new Option(this.mundescricao, this.mundescricao));
                    });
                    options.focus();
                    if (muncod_hid) {
                        options.val(muncod_hid);
                    }
                    options.trigger('chosen:updated');
                }
            });
        }
    }
</script>