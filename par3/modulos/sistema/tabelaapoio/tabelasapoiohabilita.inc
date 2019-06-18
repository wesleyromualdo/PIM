<?php
if (!$_REQUEST['menu']) {
    $_REQUEST['menu'] = 'tiponaturezajuridica';
}

require APPRAIZ . 'includes/cabecalho.inc';
?>
<script src="js/par3.js"></script>
<link rel="stylesheet" type="text/css" href="css/menu_tabela_apoio.css" />

<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;">Tabelas de Apoio</h5>
            </div>
            <div class="ibox-content">
                <div class="ibox-content-round-gray ibox-content">
                    <h5 id="btn-menu-tabelaapoio" style="cursor: pointer;">
                        Tabelas de Apoio
                        <i class="fa fa-bars" title="clique para esconder/mostar os menus"></i>
                    </h5>
                    <div class="row">
                        <div id="side-menu-tabelaapoio" class="col-lg-2 col-md-2 colunaMenu noprint">
                            <div class="space-25"></div>
                          
                            
                            <!--Habilitação-->
                            <h5 class="menuopcoes" id="titulo-iniciativas"><b><i class="fa fa-plus"></i> Habilitação</b></h5>
                            <div id="div-iniciativasmenu" class="guiamenu col-md-offset-1 col-lg-offset-1 col-sm-offset-1" style="display: none;">
                                 <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'tiponaturezajuridica' ? 'menuSelecionado' : '') ?>" id="tiponaturezajuridica" tipo="tiponaturezajuridica">Tipo de Natureza Jurídica</div>
                                 <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'naturezajuridica' ? 'menuSelecionado' : '') ?>" id="naturezajuridica" tipo="naturezajuridica">Natureza Jurídica</div>
                                 <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'categoriaentidade' ? 'menuSelecionado' : '') ?>" id="categoriaentidade" tipo="categoriaentidade">Categoria de Entidades</div>
                                 <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'tipodocumento' ? 'menuSelecionado' : '') ?>" id="tipodocumento" tipo="tipodocumento">Documentos</div>
                                 <div class="menuopcoes menuopcao <?php echo ( $_REQUEST['menu'] === 'documentoobrigatorio' ? 'menuSelecionado' : '') ?>" id="documentoobrigatorio" tipo="documentoobrigatorio">Configuração de Documentos</div>
                                <br/>
                            </div>

                            
                        </div>

                        <div class="col-lg-10 col-md-10 colunaForm" id="print_area" id="demo" class="collapse">
                            <?php
                            $pastaHabilitacao   = 'par3/modulos/sistema/tabelaapoio/habilitacao/';
                            
                            $urlGuia                = '/par3/par3.php?modulo=sistema/tabelaapoio/tabelasapoiohabilita&acao=A&menu='.$_REQUEST['menu'];
                            switch ($_REQUEST['menu']) {
                                case 'tiponaturezajuridica':
                                    include APPRAIZ . $pastaHabilitacao . 'tiponaturezajuridica.inc';
                                    break;
                                case 'naturezajuridica':
                                    include APPRAIZ . $pastaHabilitacao . 'naturezajuridica.inc';
                                    break;
                                case 'categoriaentidade':
                                    include APPRAIZ . $pastaHabilitacao . 'categoriaentidade.inc';
                                    break;
                                case 'tipodocumento':
                                    include APPRAIZ . $pastaHabilitacao . 'tipodocumento.inc';
                                    break;
                                case 'documentoobrigatorio':
                                    include APPRAIZ . $pastaHabilitacao . 'documentoobrigatorio.inc';
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
        $('#btn-menu-tabelaapoio').on('click',function(evt) {
            evt.preventDefault();
            $('#side-menu-tabelaapoio').toggle("slide");
            $('#print_area').toggleClass(function() {
                if($(this).is('.col-lg-10')) {
                    $(this).removeClass('col-lg-10');
                    $(this).removeClass('col-md-10');
                    return 'col-lg-12 col-md-12';
                }
                $(this).removeClass('col-lg-12');
                $(this).removeClass('col-md-12');
                return 'col-lg-10 col-md-10';
            });
        });

        $('.menuopcao').click(function () {
            var menuAtual = '<?php echo $_REQUEST['menu']; ?>';
            var menu = $(this).attr('tipo');
            var url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoiohabilita&acao=A&menu=' + menu;

            if (menu != menuAtual) {
                $(location).attr('href', url);
            }
        });
    });

    /*******************************************/
    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-itenscomposicaomenu').show(function () {
        $('#titulo-itenscomposicao').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-iniciativasmenu').show(function () {
        $('#titulo-iniciativas').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-orcamentomenu').show(function () {
        $('#titulo-orcamento').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
        
	$('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-documentomenu').show(function () {
        $('#titulo-documento').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
	$('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-configuracaomenu').show(function () {
        $('#titulo-configuracao').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-servico-wsmenu').show(function () {
        $('#titulo-servivo-ws').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-planejamentomenu').show(function () {
        $('#titulo-planejamento').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-regrabloqueiomenu').show(function () {
        $('#titulo-regrabloqueio').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-itensobrasmenu').show(function () {
        $('#titulo-itensobras').find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('[tipo=<?php echo $_REQUEST['menu']; ?>]').parent('#div-itenstipoobrasmenu').show(function () {
        $('#titulo-itensobras, #titulo-tipoobra').each(function(){
            $(this).find('i').toggleClass(function () {
                if ($(this).is(".fa-plus")) {
                    return 'fa-minus';
                }
                return 'fa-plus';
            });
        });
        $('#div-itensobrasmenu').slideToggle();
    });






    /*********************************/

    $('#titulo-iniciativas').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    
    $('#titulo-servivo-ws').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    
    $('#titulo-planejamento').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    
    $('#titulo-regrabloqueio').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('#titulo-itenscomposicao').click(function () {
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itenscomposicaomenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('#titulo-itensobras').click(function () {
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });


    $('#titulo-tipoobra').click(function () {
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itenstipoobrasmenu').slideToggle();
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

    $('#titulo-orcamento').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideToggle();
        
        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    
    $('#titulo-documento').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideUp(function () {
            $('#titulo-configuracao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideToggle();

        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });
    
    $('#titulo-configuracao').click(function () {
        $('#div-itenscomposicaomenu').slideUp(function () {
            $('#titulo-itenscomposicao').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-itensobrasmenu').slideUp(function () {
            $('#titulo-itensobras').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-iniciativasmenu').slideUp(function () {
            $('#titulo-iniciativas').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-planejamentomenu').slideUp(function () {
            $('#titulo-planejamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-regrabloqueiomenu').slideUp(function () {
            $('#titulo-regrabloqueio').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-servico-wsmenu').slideUp(function () {
            $('#titulo-servivo-ws').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-orcamentomenu').slideUp(function () {
            $('#titulo-orcamento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-documentomenu').slideUp(function () {
            $('#titulo-documento').find('i').removeClass('fa-minus').addClass('fa-plus');
        });
        $('#div-configuracaomenu').slideToggle();

        $(this).find('i').toggleClass(function () {
            if ($(this).is(".fa-plus")) {
                return 'fa-minus';
            }
            return 'fa-plus';
        });
    });

</script>