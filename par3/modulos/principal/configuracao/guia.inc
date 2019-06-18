<?php

require_once APPRAIZ . 'includes/cabecalho.inc';

$itrid = $_POST['itrid'] ? $_POST['itrid'] : $_GET['itrid'];
if( !$itrid ){
	$itrid = $_REQUEST['itrid'] ? $_REQUEST['itrid'] : 1;
}

//ver($_POST,d);

switch ($_POST['tipo']) {
    case 'dimensao':
        $oDimensao = new Par3_Model_Dimensao;
        $oDimensao->preparaSalvar();
        simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
        break;
    case 'area':
        $oArea = new Par3_Model_Area;
        $oArea->preparaSalvar();
        simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
        break;

    case 'indicador':
        $oIndicadorController = new Par3_Controller_Indicador;
        $oIndicadorController->salvarIndicadorGuia();
        simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
        break;

    case 'criterio':
        $oCriterio = new Par3_Model_Criterio;
        $oCriterio->preparaSalvar();
        simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
        break;
    case 'iniciativa':
        $oIniciativa = new Par3_Model_PropostaIniciativa();
        $oIniciativa->preparaSalvar();
        simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
        break;
}

if ($_GET['acaoGuia'] == 'excluir') {
    switch ($_GET['tipoGuia']) {
        case 'dimensao':
            $oDimensao = new Par3_Model_Dimensao();
            $oDimensao->deletarDimensaoGuia($_GET['codigo']);
            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
            break;

        case 'area':
            $oArea = new Par3_Model_Area();
            $oArea->deletarAreaGuia($_GET['codigo']);
            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
            break;

        case 'indicador':
            $oIndicador = new Par3_Model_Indicador();
            $oIndicador->deletarIndicadorGuia($_GET['codigo']);
            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
            break;
        case 'iniciativa':
            $oIniciativa = new Par3_Model_PropostaIniciativa();
            $oIniciativa->deletarIniciativaGuia($_GET['codigo']);
            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
            break;

        case 'criterio':
            $oCriterioController = new Par3_Controller_Criterio();
            $sucesso = $oCriterioController->deletarCriterioGuia($_GET['codigo']);
            switch ($sucesso) {
                case 1:
                    simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
                case 2:
                    simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'error', 'O critério já está vinculado a outro critério. Remova a vinculação e tente novamente.');
                case 3:
                    simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'error', 'O critério já foi pontuado pela entidade.');
            }
            break;

    }
}

if ($_GET['acaoGuia'] == 'reordenar') {
    switch ($_GET['tipo']) {
        case 'criterio':
            $oCriterio = new Par3_Controller_Criterio();
            $oCriterio->reordenarItens($_GET['id'], $_GET['direcao']);
            simec_redirecionar('par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$itrid, 'success');
    }
}

if ($_POST['vinculaIndicador'] == 'salvar') {
    $oIndicador = new Par3_Controller_DimensaoPainel();
    $oIndicador->vincularIndicadoresPainelGuia($_POST['dimid'], $_POST['indices']);
    die();
}

if ($_POST['vinculaCriterio'] == 'salvar') {
    $oCriterioVinculacao = new Par3_Controller_CriterioVinculacao();
    $oCriterioVinculacao->vincularCriteriosGuia($_POST);
    die();
}

if ($_POST['filtro']) {
    $params = implode("%", $_POST['filtro']);
} else {
    $params = '0';
}

$arInstrumentos = Par3_Controller_ConfiguracaoControle::recuperarIntrumentosGuia($itrid);
$instrumento = $arInstrumentos[0];

echo $simec->title("Guia Diagnóstico");
?>

<input type="hidden" id="itrid" name="itrid" value="<?php echo $itrid; ?>">
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
    	<?php
    	$url = 'par3.php?modulo=principal/configuracao/guia&acao=A';
    	$abasGuias = array(
    	    0 => array( "descricao" => "Guia Diagnóstico", 	"link" => "par3.php?modulo=principal/configuracao/guia&acao=A" ),
    	    1 => array( "descricao" => "Guia Planejamento", "link" => "par3.php?modulo=principal/configuracao/guiaPlanejamento&acao=A" ),
    	);
        echo $simec->tab ( $abasGuias, $url );
        echo '<br>';
		$url = $_REQUEST['itrid'] ? 'par3.php?modulo=principal/configuracao/guia&acao=A&itrid='.$_REQUEST['itrid'] : 'par3.php?modulo=principal/configuracao/guia&acao=A&itrid=1';
		echo $simec->tab ( criaAbaGuia(), $url );
		?>
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>
                    Guia de Ações Padronizadas
                    <small>
                        <a href="#" onclick="$('.tree').jstree('close_all')">Fechar Todos</a> |
                        <a href="#" onclick="$('.tree').jstree('open_all')">Abrir Todos</a> |
                        <a href="par3.php?modulo=principal/configuracao/popupGuiaExcel&acao=A&itrid=<?php echo (empty($_REQUEST['itrid']) ? 1 : $_REQUEST['itrid']); ?>" target="_blank">Exportar Excel</a>
                    </small>
                </h5>
                <a class="collapse-link">
                    <div class="ibox-tools">
                        <i class="fa fa-chevron-up"></i>
                    </div>
                </a>
            </div>
            <div class="ibox-content tree" style="overflow: auto">
                <ul>
                    <?php
                    	// foreach ($arInstrumentos as $instrumento){ ?>
                        <li data-jstree='{"icon":false}'>
                            <div>
                                <div id="modalGuia" style="padding-left: 5px;">
                                    <a href="#" title="Incluir dimensão" onclick="popupGuia('incluir', 'dimensao', '<?= $instrumento['itrid']; ?>');">
                                        <i class="fa fa-plus-circle"></i>
                                    </a>
                                </div>
                                &nbsp;<?php echo $instrumento['itrdsc']; ?>
                            </div>
                            <?php $arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($instrumento['itrid']); ?>
                            <?php if ($arDimensoes){  ?>
                                <!--  <ul>-->
                                    <?php foreach ($arDimensoes as $dimensao){ ?>
                                        <li data-jstree='{"icon":false}'>
                                            <div>
                                                <div style="padding-left: 5px">
                                                    <a href="#" title="Incluir área" onclick="popupGuia('incluir', 'area', '<?= $dimensao['dimid']; ?>', '<?= $itrid; ?>')">
                                                        <i class="fa fa-plus-circle"></i>
                                                    </a>
                                                    <a href="#" title="Alterar dimensão" onclick="popupGuia('editar', 'dimensao', '<?= $dimensao['dimid']; ?>', '<?= $itrid; ?>')">
                                                        <i class="fa fa-edit"></i>
                                                    </a>
                                                    <a href="#" title="Vincular indicadores" onclick="popupGuiaIndicador('<?php echo $dimensao['dimid']; ?>', '<?php echo $dimensao['dimdsc']; ?>')">
                                                        <i class="fa fa-chain"></i>
                                                    </a>
                                                    <a href="#" title="Excluir dimensão" onclick="excluirItemGuia('dimensao', '<?= $dimensao['dimid']; ?>', '<?= $itrid; ?>')">
                                                        <i class="fa fa-trash-o"></i>
                                                    </a>
                                                </div>
                                                &nbsp;<?php echo $dimensao['dimcod'] . ' - ' . $dimensao['dimdsc']; ?>
                                            </div>

                                            <?php $arAreas = Par3_Controller_ConfiguracaoControle::recuperarAreasGuia($dimensao['dimid']); ?>
                                            <?php if ($arAreas){ ?>
                                                <ul>
                                                    <?php foreach ($arAreas as $area){ ?>
                                                        <li data-jstree='{"icon":false}'>
                                                            <div>
                                                                <div style="padding-left: 5px;">
                                                                    <a href="#" title="Incluir indicador" onclick="popupGuia('incluir', 'indicador', '<?= $area['areid']; ?>', '<?= $itrid; ?>')">
                                                                        <i class="fa fa-plus-circle"></i>
                                                                    </a>
                                                                    <a href="#" title="Alterar área" onclick="popupGuia('editar', 'area', '<?= $area['areid']; ?>', '<?= $itrid; ?>')">
                                                                        <i class="fa fa-edit"></i>
                                                                    </a>
                                                                    <a href="#" title="Excluir área" onclick="excluirItemGuia('area', '<?= $area['areid']; ?>', '<?= $itrid; ?>')">
                                                                        <i class="fa fa-trash-o"></i>
                                                                    </a>
                                                                </div>
                                                                &nbsp;<?php echo $dimensao['dimcod'] . "." . $area['arecod'] . ' - ' . $area['aredsc']; ?>
                                                            </div>

                                                            <?php $arIndicadores = Par3_Controller_ConfiguracaoControle::recuperarIndicadoresGuia($area['areid']); ?>
                                                            <?php if ($arIndicadores){ ?>
                                                                <ul>
                                                                    <?php foreach ($arIndicadores as $indicador){ ?>
                                                                        <li data-jstree='{"icon":false}'>
                                                                            <div>
                                                                                <div style="padding-left: 5px">
                                                                                <!--
                                                                                    <a href="#" title="Incluir Iniciativa" onclick="popupIniciativa('incluir','<?= $indicador['indid']; ?>',0, '<?= $itrid; ?>')">
                                                                                        <i class="fa fa-plus-circle"></i>
                                                                                    </a>
                                                                                -->
                                                                                    <a href="#" title="Incluir Componente" onclick="popupGuia('incluir', 'criterio', '<?= $indicador['indid']; ?>', '<?= $itrid; ?>')">
                                                                                        <i class="fa fa-plus-circle"></i>
                                                                                    </a>
                                                                                    <a href="#" title="Alterar indicador" onclick="popupGuia('editar', 'indicador', '<?= $indicador['indid']; ?>', '<?= $itrid; ?>')">
                                                                                        <i class="fa fa-edit"></i>
                                                                                    </a>
                                                                                    <a href="#" title="Excluir indicador" onclick="excluirItemGuia('indicador', '<?= $indicador['indid']; ?>', '<?= $itrid; ?>')">
                                                                                        <i class="fa fa-trash-o"></i>
                                                                                    </a>
                                                                                </div>
                                                                                &nbsp;<?php echo $indicador[$indicador['indid']] = $dimensao['dimcod'] . "." . $area['arecod'] . "." . $indicador['indcod'] . " - " . $indicador['inddsc']; ?>
                                                                            </div>


                                                                                <ul>
                                                                                    <?php $iniciativas = Par3_Controller_ConfiguracaoControle::recuperarIniciativaGuia($indicador['indid']); ?>
                                                                                    <?php if ($iniciativas) {


                                                                                            ?>
                                                                                            <li>Iniciativas
                                                                                                <ul>
                                                                                                    <?php   foreach ($iniciativas as $iniciativa) {?>
                                                                                                    <li data-jstree='{"icon":false}'><?php echo $iniciativa['pridesc'] ?>
                                                                                                        <div>
                                                                                                            <div
                                                                                                                style="margin-left: 5px">
                                                                                                                <a href="#"
                                                                                                                   title="Alterar Iniciativa"
                                                                                                                   onclick="popupIniciativa('editar', <?= $indicador['indid']; ?>, '<?= $iniciativa['priid']; ?>', '<?= $itrid; ?>')">
                                                                                                                    <i class="fa fa-edit"></i>

                                                                                                                </a>
                                                                                                            </div>
                                                                                                        </div>
                                                                                                    </li>

                                                                                                    <?php } ?>

                                                                                                </ul>
                                                                                            </li>
                                                                                        <?php

                                                                                    }?>
                                                                                        <li>Componentes
                                                                                            <ul>
                                                                                                <?php $arCriterios = Par3_Controller_ConfiguracaoControle::recuperarCriteriosGuia($indicador['indid']); ?>
                                                                                                <?php if ($arCriterios) { ?>
                                                                                                <?php foreach ($arCriterios as $criterio) { ?>
                                                                                                    <li data-jstree='{"icon":false}'>
                                                                                                        <div>
                                                                                                            <div style="margin-left: 5px">
                                                                                                                <!-- <a href="#" title="Incluir ação" onclick="popupGuia('incluir', 'acao', '<?= $criterio['crtid']; ?>')"> -->
                                                                                                                <!-- <i class="fa fa-plus-circle"></i> -->
                                                                                                                <!-- </a> -->
                                                                                                                <a href="#" title="Alterar componente" onclick="popupGuia('editar', 'criterio', '<?= $criterio['crtid']; ?>', '<?= $itrid; ?>')">
                                                                                                                    <i class="fa fa-edit"></i>
                                                                                                                </a>
                                                                                                                <a href="#" title="Excluir componente" onclick="excluirItemGuia('criterio', '<?= $criterio['crtid']; ?>', '<?= $itrid; ?>')">
                                                                                                                    <i class="fa fa-trash-o"></i>
                                                                                                                </a>
                                                                                                                <?php if ($arCriterios[1]) { // Quer dizer que ele tem mais que um critério ?>
                                                                                                                    <a href="#" title="Vinculação de Componentes" onclick="popupGuiaCriterio('<?= $criterio['crtid']; ?>', '<?= $itrid; ?>')">
                                                                                                                        <i class="fa fa-chain"></i>
                                                                                                                    </a>
                                                                                                                <?php } ?>
                                                                                                                <a href="#" title="Mover para cima" onclick="moverItem('criterio', 'cima', '<?= $criterio['crtid']; ?>', '<?= $itrid; ?>')">
                                                                                                                    <i class="fa fa-arrow-up"></i>
                                                                                                                </a>
                                                                                                                <a href="#" title="Mover para baixo" onclick="moverItem('criterio', 'baixo', '<?= $criterio['crtid']; ?>', '<?= $itrid; ?>')">
                                                                                                                    <i class="fa fa-arrow-down"></i>
                                                                                                                </a>
                                                                                                            </div>
                                                                                                            &nbsp;<?php echo "(" . $criterio['crtcod'] . ") " . $criterio['crtdsc']; ?>
                                                                                                        </div>
                                                                                                    </li>
                                                                                                <?php } ?>
                                                                                            </ul>
                                                                                        </li>
                                                                                    </ul>
                                                                            <?php } ?>
                                                                        </li>
                                                                    <?php } ?>
                                                                </ul>
                                                            <?php } ?>
                                                        </li>
                                                    <?php } ?>
                                                </ul>
                                            <?php } ?>
                                        </li>
                                    <?php } ?>
                                </ul>
                            <?php } ?>
                        </li>
                    <?php // } ?>
                </ul>
            </div>
        </div>
    </div>
</div>

<div id="modal-form" class="modal fade" aria-hidden="true">
    <div class="modal-dialog">
        <div class="ibox-title">
            <h5 id="html_modal-title"></h5>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form">
        </div>
    </div>

    <div id="modal-form-large" class="modal fade" aria-hidden="true">
        <div class="ibox-title">
            <h5 id="html_modal-title-large"></h5>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form-large">
        </div>
    </div>
</div>

<script>
    function popupGuia(acao, tipo, id, itrid) {
        var titulo = 'Gerenciar dimensão';

        switch (tipo) {
            case 'area':
                titulo = 'Gerenciar área';
                break;
            case 'indicador':
                titulo = 'Gerenciar indicador';
                break;
            case 'criterio':
                titulo = 'Gerenciar componentes do indicador';
                break;
        }

        $.ajax({
            type: "POST",
            url: 'par3.php?modulo=principal/configuracao/popupGuia&acao=A',
            data: '&acaoGuia=' + acao + '&tipoGuia=' + tipo + '&codigo=' + id + '&itrid=' + itrid,
            async: false,
            success: function (resp) {
                $('#html_modal-form').html(resp);
                $('#html_modal-title').html(titulo);
                $('#modal-form').modal();
            }
        });
    }

    function popupIniciativa(acao, indid, id, itrid)
    {
        var titulo = 'Gerenciar Iniciativa';

        $.ajax({
            type: "POST",
            url: 'par3.php?modulo=principal/configuracao/popupIniciativa&acao=A',
            data: '&tipo=iniciativa&codigo=' + id + '&itrid=' + itrid + '&indid=' + indid,
            async: false,
            success: function (resp) {
                $('#html_modal-form').html(resp);
                $('#html_modal-title').html(titulo);
                $('#modal-form').modal();
            }
        });
    }

    function popupGuiaIndicador(id, dimensao) {
        $.ajax({
            type: "POST",
            url: 'par3.php?modulo=principal/configuracao/popupGuiaDimensaoPainel&acao=A',
            data: '&dimid=' + id + '&dimensao=' + dimensao,
            async: false,
            success: function (resp) {
                $('#html_modal-form-large').html(resp);
                $('#html_modal-title-large').html('Vincular Indicadores para a Dimensão ' + dimensao);
                $('#modal-form-large').modal();
            }
        });
    }

    function popupGuiaCriterio(id, itrid) {
        $.ajax({
            type: "POST",
            url: 'par3.php?modulo=principal/configuracao/popupGuiaCriterio&acao=A',
            data: '&itrid=' + itrid + '&crtid=' + id,
            async: false,
            success: function (resp) {
                $('#html_modal-form-large').html(resp);
                $('#html_modal-title-large').html('Gerenciar regras de vinculação');
                $('#modal-form-large').modal();
            }
        });
    }

    function moverItem(tipo, direcao, id, itrid) {
        url = 'par3.php?modulo=principal/configuracao/guia&acao=A&itrid=' + itrid + '&acaoGuia=reordenar&id=' + id + '&tipo=' + tipo + '&direcao=' + direcao;
        document.location.href = url;
    }

    function excluirItemGuia(tipo, codigo, itrid) {
        url = 'par3.php?modulo=principal/configuracao/guia&acao=A&itrid=' + itrid + '&acaoGuia=excluir&tipoGuia=' + tipo + '&codigo=' + codigo;
        swal({
                title: "Tem certeza?",
                text: "Deseja deletar este registro?",
                type: "warning",
                showCancelButton: true,
                confirmButtonColor: "#DD6B55",
                cancelButtonText: "Cancelar",
                confirmButtonText: "Confirmar",
                closeOnConfirm: false
            },
            function () {
                document.location.href = url;
            });
    }
</script>