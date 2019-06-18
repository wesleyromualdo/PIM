<?php
$relatorioQuestionario = new Par3_Model_Relatorio_QuestoesEstrategicas();
$tableHtml = '';
$itrid = $_POST['sfiltro']['itrid'];

ini_set("max_execution_time","0");

switch ($_POST['requisicao']) {
    case 'carregaMunicipios':
        $municipio = new Territorios_Model_Municipio();
        $response = $municipio->lista(['muncod', 'mundescricao'], ["estuf = '{$_POST['estuf']}'"]);
        print simec_json_encode($response);
        die;
        break;
    case 'filtrar':
        $tableHtml = $relatorioQuestionario->carregarQuestionario($_POST['sfiltro']);
        break;
    case 'xls':        
        header("Content-Type:   application/vnd.ms-excel; charset=utf-8");
        header("Content-type:   application/x-msexcel; charset=utf-8");
        header("Content-Disposition: attachment; filename=relatorio_questoes_estrategicas.xls");
        header("Expires: 0");
        header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
        header("Cache-Control: private", false);
        $tableHtml = $relatorioQuestionario->carregarQuestionario($_POST['sfiltro']);
        echo $tableHtml;
        exit;
        break;
}

include APPRAIZ . 'includes/Agrupador.php';
require APPRAIZ . 'includes/cabecalho.inc';
?>
<style type="text/css">
    .scroll {
        width: 1200px;
        height: 500px;
        overflow: scroll;
    }
    .scroll::-webkit-scrollbar {
        width: 12px;
    }

    .scroll::-webkit-scrollbar-track {
        -webkit-box-shadow: inset 0 0 6px rgba(0,0,0,0.3); 
        border-radius: 10px;
    }

    .scroll::-webkit-scrollbar-thumb {
        border-radius: 10px;
        -webkit-box-shadow: inset 0 0 6px rgba(0,0,0,0.5); 
    }
    .fa-file-excel-o {
        font-size: 15px;
        color: #fff;
        cursor: pointer;
    }

    .fa-file-excel-o:hover {
        color: #fff;
        -webkit-box-shadow: 0px 0px 10px 0px rgba(0, 0, 0, 0.5);
        -moz-box-shadow: 0px 0px 10px 0px rgba(0, 0, 0, 0.5);
        box-shadow: 0px 0px 10px 0px rgba(0, 0, 0, 0.5);
    }

    .footer {
        bottom: -38px;
    }

    #wrapper {
        width: 103%;
    }

    @media print {
        .notprint {
            display: none
        }

        .div_rolagem {
            display: none
        }

        .div_rol {
            display: 'none'
        }
    }

    @media screen {
        .notscreen {
            display: none;
        }

        .div_rol {
            display: none;
        }
    }

    .div_rolagem {
        overflow-x: auto;
        overflow-y: auto;
        height: 50px;
    }

    .div_rol {
        overflow-x: auto;
        overflow-y: auto;
        height: 50px;
    }
</style>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/agrupador.js"></script>

<form method="post" name="formulario" id="formulario" class="form form-horizontal notprint">
    <input type="hidden" name="requisicao" id="requisicao" value="filtrar">
    <input type="hidden" name="muncod_hid" value="<?php echo $_POST['sfiltro']['muncod']; ?>">
    <div class="ibox">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-10">
                    <h3 id="entidade">Relatório Questões Estratégicas</h3>
                </div>
            </div>
        </div>
        <div class="ibox-content">
            <!--            <div class="row">
                            <div class="col-lg-2 text-right"><label for="sfiltro[itrid]">Esfera</label></div>
                            <div class="col-lg-10 text-center">
            <?php
            //echo $simec->select('sfiltro[itrid]', null, ($itrid ? $itrid : 2), array(1 => 'Estadual', 2 => 'Municipal'), $arrAttr);
            ?>
                            </div>
                        </div>-->

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
                    $arrAttr = ['placeHolder' => 'Municipio'];
                    echo $simec->select('sfiltro[muncod]', null, null, $arrAttr);
                    ?>
                </div>
            </div>

            <div class="row">
                <div class="col-lg-2 text-right"><label for="sfiltro[queid]">Questionário</label></div>
                <div class="col-lg-10">
                    <?php
                    echo $simec->select('sfiltro[queid]', null, ($queid ? $queid : 130), array(130 => 'Questões Estratégicas', 131 => 'Plano Nacional de Educação: Questões Complementares'), $arrAttr);
                    ?>
                </div>
            </div>

            <div class="ibox-footer">
                <div class="row">
                    <div class="col-lg-5 col-lg-offset-4">
                        <button type="submit" class="btn btn-success salvar" style="width:110px;"><i
                                class="fa fa-search"></i> Filtrar
                        </button>
                        <button type="submit" class="btn btn-primary xls" style="width:110px;"><i
                                class="fa fa-file-excel-o"></i> XLS
                        </button>
                        <!--                        <button type="submit" class="btn btn-warning" style="width:110px;"
                                                        onclick="javascript:$('#div_cabecalho').css('display', ''); window.print();">
                                                    <i class="glyphicon glyphicon-print"></i> Imprimir
                                                </button>-->
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
                    Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>
                    Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/>
                </td>
            </tr>
        </table>
        <hr style="color:black; width:95%; margin-left:-18px;">
    </div>

    <div class="clearfix" style="margin-bottom:10px;"></div>


    <?php if (!empty($tableHtml)) : ?>
        <div class="scroll">
            <?php echo $tableHtml; ?>
        </div>
    <?php else:
        ?>
        <div class="alert alert-info" role="alert">
            <p class="text-center">Preechar o filtro e clique em filtar ou XLS</p>
        </div>
    <?php endif; ?>

</div>

<script type="text/javascript">
    $(function () {

        $(".xls").on("click", function () {
            $("#requisicao").val("xls");
            $("#formulario").submit();
        });

        $(".salvar").on("click", function () {
            $("#formulario").submit();
        });

        var muncod_hid = jQuery('[name="muncod_hid"]').val();

        jQuery('#sfiltro-mundescricao').append(new Option('', ''));

        if (jQuery('[name="sfiltro[estuf]"]').val() != '') {
            carregarMunicipio(jQuery('[name="sfiltro[estuf]"]').val(), muncod_hid);
        }

        jQuery('[name="sfiltro[estuf]"]').change(function () {
            if (jQuery('[name="sfiltro[mundescricao]"]').val() != 'E') {
                var estuf = jQuery(this).val();
                carregarMunicipio(estuf, muncod_hid);
            }
        });
    });

    function carregarMunicipio(estuf, muncod_hid) {
        if (estuf != '') {
            var options = jQuery('#sfiltro-muncod');
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=carregaMunicipios&estuf=" + estuf,
                async: false,
                success: function (result) {
                    options.empty();
                    options.append(new Option('', ''));
                    var result = JSON.parse(result);
                    $.each(result, function () {
                        options.append(new Option(this.mundescricao, this.muncod));
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