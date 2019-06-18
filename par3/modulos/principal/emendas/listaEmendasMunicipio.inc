<?php
set_time_limit(0);

$obEmenda = new Par3_Controller_EmendasParlamentares();

$modelUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

if (!$arrPermissao || !$arrPermissao['booVisualizar']) {
    simec_redirecionar('par3.php?modulo=inicio&acao=C', 'error', 'Acesso negado.');
}

switch ($_REQUEST['requisicao']) {
    case 'carregaMunicipios':
        $municipio = new Territorios_Model_Municipio();
        print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'")));
        die;
        break;
}

$arrEstado = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
$mInp = new Par3_Controller_IniciativaPlanejamento();
$arrIniciativa = $obEmenda->carregarIniciativaEmenda();
$sqlParlamentar = $obEmenda->pegaParlamenatar('');
$sqlWorkflow = $obEmenda->pegaAcaoWorkFlow();

$arResponsabilidadeMuncod = $obEmenda->pegaResponsabilidadeMunicipio();

if ($_REQUEST['inuid']) {
    $arMuncod = $obEmenda->pegaMuncodEmenda($_REQUEST['inuid']);
    $_REQUEST['muncod'] = ($arMuncod['muncod'] ? $arMuncod['muncod'] : '');
    $_REQUEST['estuf'] = ($arMuncod['estuf'] ? $arMuncod['estuf'] : '');
} else {
    $_REQUEST['muncod'] = ( !is_array($_REQUEST['muncod']) && !empty($_REQUEST['muncod']) ? $_REQUEST['muncod'] : $arResponsabilidadeMuncod);
}

require APPRAIZ . 'includes/cabecalho.inc';
$controleUnidade         = new Par3_Controller_InstrumentoUnidade();


if (empty($_REQUEST['inuid'])) {
    $db->cria_aba($abacod_tela, $url, '');
}
?>
<div id="debug"></div>
<div class="ibox">
    <div class="ibox-title">
<?php   if (empty($_REQUEST['inuid'])) {?>
            <div class="row">
                <div class="col-md-12">
                    <h2 class="center">Pesquisar Emendas</h2>
                </div>
            </div>
<?php   } else {?>
            <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
<?php   }?>
    </div>
    <div class="ibox-content">
<?php   if (!empty($_REQUEST['inuid'])) {?>
            <?php echo $controleUnidade->cabecalhoUnidade(); ?>
            <div class="row">&nbsp;</div>
<?php   }?>
        <div class="row">
            <form name="form-lista-emenda" id="form-lista-emenda" class="form-horizontal" method="post">
                <input type="hidden" name="requisicao" id="requisicao" value="filtrar"/>
                <input type="hidden" name="inuid" id="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>
                <div class="row">
                    <div class="col-lg-12" style="text-align: center !important;">
                        <div class="col-lg-5">
                            <?php echo $simec->input('emeano', 'Ano', ($_REQUEST['emeano'] ? $_REQUEST['emeano'] : '2018'), array('type' => 'number'), array('input-size' => '4')); ?>
                            <?php echo $simec->input('emecod', 'Emenda', $_REQUEST['emecod'], array('type' => 'number'), array('input-size' => '4')); ?>
                            <?php echo $simec->select('autid_f', 'Parlamentar', $_REQUEST['autid_f'], $sqlParlamentar, array('maxlength' => '255'), array()); ?>
                            <?php echo $simec->select('esdid', 'Situação', $_REQUEST['esdid'], $sqlWorkflow, array('maxlength' => '255'), array()); ?>
                        </div>
                        <div class="col-lg-5">
<?php                   if ($_REQUEST['inuid']) { ?>
                            <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255', 'disabled' => 'disabled'), array()); ?>
                            <?php echo $simec->select('muncod', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('maxlength' => '255', 'disabled' => 'disabled'), array()); ?>
<?php                   } else {?>
                            <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255'), array()); ?>
                            <?php echo $simec->select('muncod', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('maxlength' => '255'), array()); ?>
<?php                   }?>
                            <?php echo $simec->select('iniid', 'Iniciativa', $_REQUEST['iniid'], $arrIniciativa, array('maxlength' => '255'), array()); ?>
                            <?php echo $simec->radio('itrid', 'Esfera', $_REQUEST['itrid'], array('1' => 'Estadual', '2' => 'Municipal'), array(), array('input-size' => '4')); ?>
                        </div>
                    </div>
                </div>
            </form>
            <div class="ibox-footer">
                <div class="center">
                    <button
                            type="button"
                            id="btn-pesquisar"
                            data-dismiss="modal"
                            class="btn btn-info btn-pesquisar"
                            data-loading-text="Pesquisando, aguarde ...">
                        <i class="fa fa-search"></i>
                        Pesquisar
                    </button> |
                    <button type="reset" class="btn btn-default-bright" id="limpar">
                        <i class="fa fa-eraser"></i>
                        Limpar
                    </button>
                </div>
            </div>
           <!-- Listagem  -->
            <div class="float-e-margins" id="listagem-obras">
                <div class="ibox-content" id="div-listagem-obras" style="display: block;">
                    <div class="table-responsive" >
                        <div>
                            <div id="testeSQL">
                            <?php
                                echo $obEmenda->listarVisaoMunicipio($_REQUEST);
                            ?>
                            </div>
                        </div>
                    </div>
                    <br>
                    <br>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
jQuery('#limpar').click(function(){
    window.location.href = window.location;
});

jQuery(function() {

    if(jQuery('[name="estuf"]').val() != '') {
        carregarMunicipio(jQuery('[name="estuf"]').val(), <?=$_REQUEST['muncod']?>);
    }

    jQuery('select[name=estuf]').change(function(){
        carregarMunicipio(this.value);
    });
});

function carregarMunicipio(estuf, muncod) {
    if(estuf != '') {
        var options = jQuery('#muncod');
        options.empty();
        options.append(new Option('', ''));
        jQuery.post('', 'requisicao=carregaMunicipios&estuf='+estuf, function(retorno) {
            options.append(new Option('', ''));
            $.each(JSON.parse(retorno), function() {
                options.append(new Option(this.mundescricao, this.muncod));
            });
            options.focus();
            options.val(muncod);
            options.trigger('chosen:updated');
        });
    }
}

jQuery(".btn-pesquisar").click(function () {
    $btn = jQuery(this).button('loading');
    jQuery('[name="requisicao"]').val('filtrar');
    jQuery('[name="form-lista-emenda"]').submit();
});

function detalharEmendaIniciativa( emeid, edeid){
    var inuid = jQuery('[name="inuid"]').val();
    if( inuid != '' ){
        window.location.href = "par3.php?modulo=principal/emendas/detalharIniciativaParlamentar&acao=A&emeid="+emeid+"&edeid="+edeid+'&inuid='+inuid;
    } else {
        window.location.href = "par3.php?modulo=principal/emendas/detalharIniciativaParlamentar&acao=A&emeid="+emeid+"&edeid="+edeid;
    }
}
</script>
