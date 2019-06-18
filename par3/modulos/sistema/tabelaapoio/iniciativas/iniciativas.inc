<?php
require APPRAIZ . 'includes/cabecalho.inc';

if(!$_REQUEST['menu']){
    $_REQUEST['menu'] = 'descricao';
}
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
                <h5 style="font-size: 18px; font-weight: normal;">Iniciativas do PAR</h5>
            </div>
            <div class="ibox-content">
                <div class="ibox-content-round-gray ibox-content">
                    <div class="row">
                        <div class="col-md-3 colunaMenu noprint">
                            <div id="menuDirigentes">
                                <div id="menuIniciativa">
                                    <h5><b>TABELAS APOIO</b></h5>
                                    <div class="menuIniciativa <?php echo ( $_REQUEST['menu'] === 'descricao' ? 'menuSelecionado' : '') ?>" id="descricao" tipo="descricao">
                                        Descrição de Iniciativas
                                    </div>

                                    <div class="menuIniciativa <?php echo ( $_REQUEST['menu'] === 'ciclo' ? 'menuSelecionado' : '') ?>" id="cicloPar" tipo="ciclo">
                                        Ciclos do PAR
                                    </div>


                                    <div class="menuIniciativa <?php echo ( $_REQUEST['menu'] === 'modalidades' ? 'menuSelecionado' : '') ?>" id="modalidades" tipo="modalidades">
                                        Modalidades
                                    </div>

                                    <div class="menuIniciativa <?php echo ( $_REQUEST['menu'] === 'tiposatendimento' ? 'menuSelecionado' : '') ?>" id="tiposatendimento" tipo="tiposatendimento">
                                        Tipos de atendimento
                                    </div>

                                    <div class="menuIniciativa <?php echo ( $_REQUEST['menu'] === 'desdobramentos' ? 'menuSelecionado' : '') ?>" id="desdobramentos" tipo="desdobramentos">
                                        Desdobramentos
                                    </div>

                                    <div class="space-25"></div>
                                    <div class="space-25"></div>

                                    <h5><b><i class="fa fa-plus"></i> Áreas</b></h5>

                                    <div class="menuIniciativa <?php echo ( $_REQUEST['menu'] === 'areas' ? 'menuSelecionado' : '') ?>" id="areas" tipo="areas">
                                        Áreas
                                    </div>

                                    <div class="menuIniciativa <?php echo ( $_REQUEST['menu'] === 'areas_relacionadas' ? 'menuSelecionado' : '') ?>" id="areas_relacionadas" tipo="areas_relacionadas">
                                        Áreas Relacionadas
                                    </div>

                                    <div class="space-25"></div>
                                    <div class="space-25"></div>

                                    <h5><b><i class="fa fa-plus"></i> Programas</b></h5>

                                    <div class="menuIniciativa <?php echo ( $_REQUEST['menu'] === 'programas' ? 'menuSelecionado' : '') ?>" id="programas" tipo="programas">
                                        Programas
                                    </div>

                                    <div class="menuIniciativa <?php echo ( $_REQUEST['menu'] === 'projetos' ? 'menuSelecionado' : '') ?>" id="projetos" tipo="projetos">
                                        Projetos
                                    </div>

                                    <div class="space-25"></div>
                                    <div class="space-25"></div>

                                    <h5><b><i class="fa fa-plus"></i> Objetos</b></h5>

                                    <div class="menuIniciativa <?php echo ( $_REQUEST['menu'] === 'tiposobjeto' ? 'menuSelecionado' : '') ?>" id="tiposobjeto" tipo="tiposobjeto">
                                        Tipos de Objeto
                                    </div>
                                </div>

                            </div>
                        </div>
                        <div class="col-md-9 colunaForm" id="print_area">
                            <?php
                            $pastaIniciativas = 'par3/modulos/sistema/tabelaapoio/iniciativas/';
                            switch ($_REQUEST['menu']) {
                                case 'descricao':
                                    include APPRAIZ . $pastaIniciativas . 'descricaoIniciativas.inc';
                                    break;
                                case 'ciclo':
                                    include APPRAIZ . $pastaIniciativas . 'ciclos_par.inc';
                                    break;
                                case 'programas':
                                    if($_REQUEST['submenu']){
                                        $controllerIniciativaProgramas = new Par3_Controller_IniciativaProgramasController();
                                        $programacao = $controllerIniciativaProgramas->recuperar($_REQUEST);
                                        include APPRAIZ . $pastaIniciativas . 'programas/atendimento_extraordinario.inc';
                                        break;
                                    }
                                    include APPRAIZ . $pastaIniciativas . 'programas.inc';
                                    break;
                                case 'projetos':
                                    include APPRAIZ . $pastaIniciativas . 'projetos.inc';
                                    break;
                                case 'tiposatendimento':
                                    include APPRAIZ . $pastaIniciativas . 'tiposAtendimento.inc';
                                    break;
                                case 'modalidades':
                                    include APPRAIZ . $pastaIniciativas . 'modalidades.inc';
                                    break;
                                case 'tiposobjeto':
                                    include APPRAIZ . $pastaIniciativas . 'tiposObjeto.inc';
                                    break;
                                case 'desdobramentos':
                                    include APPRAIZ . $pastaIniciativas . 'desdobramentos.inc';
                                    break;
                                case 'areas':
                                    include APPRAIZ . $pastaIniciativas . 'areas.inc';
                                    break;
                                case 'areas_relacionadas':
                                    include APPRAIZ . $pastaIniciativas . 'areasRelacionadas.inc';
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
        $('.menuIniciativa').click(function () {
            var menuAtual = '<?php echo $_REQUEST['menu']; ?>';
            var menu = $(this).attr('tipo');
            console.log(menu);
            var url = 'par3.php?modulo=sistema/tabelaapoio/iniciativas/iniciativas&acao=A&menu=' + menu;

            if (menu != menuAtual) {
                $(location).attr('href', url);
            }
        });
    });

</script>
