<?php
require APPRAIZ . 'includes/cabecalho.inc';
?>
<style>
    .ibox-content-round-gray {
        width: 99%;
        background-color: #f3f3f4;
        float: center;
        border-radius: 10px 10px 10px 10px;
        -moz-border-radius: 10px 10px 10px 10px;
        -webkit-border-radius: 10px 10px 10px 10px;
        border: 0px solid #000000;
    }

    .iconUnidade {
        margin-top: 0px !important;
        margin-left: 0px !important;
        font-size: 15px !important;
        background-color: black;
        border-radius: 20px 20px 20px 20px;
        -moz-border-radius: 20px 20px 20px 20px;
        -webkit-border-radius: 20px 20px 20px 20px;
        color: yellow !important;
    }

    .menuIniciativa {
        border-bottom: 1px solid #e3e3e3;
        padding: 5px;
        cursor: pointer;
        overflow: hidden;
        text-overflow: ellipsis;
        white-space: nowrap;
    }

    .menuIniciativa:hover {
        font-weight: bold;
    }

    .menuSelecionado {
        font-weight: bold;
        cursor: default !important;
    }

    @media (max-width: 1450px) {
        .ibox-content-round-gray {
            margin-top: 0.5%
        }
    }

    @media (max-width: 1200px) {
        .i1450 {
            display: inline;
        }

        .custom-col-10 {
            padding: 5px 0px 0px 3.8%;
        }

        .marcador {
            position: absolute;
            border-radius: 100% 0% 0% 100%;
            -moz-border-radius: 100% 0% 0% 100%;
            -webkit-border-radius: 100% 0% 0% 100%;
            border: 0px;
            width: 50px;
            height: 10px;
            margin-top: 21px;
            margin-left: -1%;
        }
    }


</style>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;">Guia de Obras</h5>
            </div>
            <div class="ibox-content">
                <div class="ibox-content-round-gray ibox-content">
                    <div class="row">
                        <div class="col-md-3 colunaMenu noprint">
                            <div id="menuDirigentes">
                                <div id="menuIniciativa">
                                    <h5><b><i class="fa fa-plus"></i> Cadastro de Obras</b></h5>
                                    <div class="menuObras <?php echo ( eregi('tipo_obra', $_REQUEST['menu']) ? 'menuSelecionado' : '') ?>" id="listar_tipo_obra" tipo="listar_tipo_obra" style="cursor:pointer">
                                        Tipo de Obra
                                    </div>
                                    <div class="space-25"></div>
                                    <div class="space-25"></div>
                                    <h5><b><i class="fa fa-plus"></i> Tabelas de Apoio</b></h5>
                                    <div class="menuObras <?php echo ( eregi('documento_obra', $_REQUEST['menu']) ? 'menuSelecionado' : '') ?>" id="documento_obra" tipo="documento_obra" style="cursor:pointer">
                                        Documentos
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-9 colunaForm" id="print_area">
                            <?php
                            $pastaIniciativas = 'par3/modulos/sistema/tabelaapoio/obras/';
                            switch ($_REQUEST['menu']) {
                                case 'listar_tipo_obra':
                                    include APPRAIZ . $pastaIniciativas . 'listar_tipo_obra.inc';
                                    break;
                                case 'tipo_obra':
                                    include APPRAIZ . $pastaIniciativas . 'tipo_obra.inc';
                                    break;
                                case 'tipo_obra_abas':
                                    include APPRAIZ . $pastaIniciativas . 'tipo_obra_abas.inc';
                                    break;
                                case 'tipo_obra_planilhas':
                                    include APPRAIZ . $pastaIniciativas . 'tipo_obra_planilhas.inc';
                                    break;
                                case 'documento_obra':
                                    include APPRAIZ . $pastaIniciativas . 'documento_obra.inc';
                                    break;
                                case 'cad_documento_obra':
                                    include APPRAIZ . $pastaIniciativas . 'cad_documento_obra.inc';
                                    break;
                                default:
                                    include APPRAIZ . $pastaIniciativas . 'guiaObras.inc';
                                    break;
                            }
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<script>
    $(document).ready(function () {
        $('.menuObras').click(function () {
            var menuAtual = '<?php echo $_REQUEST['menu']; ?>';
            var menu = $(this).attr('tipo');
            console.log(menu);
            var url = 'par3.php?modulo=sistema/tabelaapoio/guiaObras&acao=A&menu=' + menu;

            if (menu != menuAtual) {
                $(location).attr('href', url);
            }
        });
    });
</script>