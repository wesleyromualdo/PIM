<?php
$dimid = $_REQUEST['dimid'];

$perfils = pegaPerfils($_SESSION['usucpf']);

$arrPerfil = array(
    Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
    Par3_Model_UsuarioResponsabilidade::PREFEITO,
    Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL,
);

$habFinalizaDiagnostico = checaPerfilValido($arrPerfil, $perfils);

$habilidar = !$existePendenciaGeral;

if (!in_array($esdid, array(PAR3_ESDID_DIAGNOSTICO_ELABORACAO, PAR3_ESDID_DIAGNOSTICO_FINALIZADO) ) /*&& $situacao['percent'] >= 100*/) {
    $disabled = 'disabled="disabled"';
}

//se existir restrição para a unidade bloqueia o botão finalizar diagnóstico
$controllerRestricao = new Par3_Controller_Restricao();

if(in_array($esdid, array(PAR3_ESDID_DIAGNOSTICO_ELABORACAO, PAR3_ESDID_DIAGNOSTICO_FINALIZADO) ) && $situacao['percent'] >= 100 && $_REQUEST['resumo'] == 1)
 {  ?>
    <div class="widget navy-bg p-lg text-center">
        <div class="row">
            <div class="col-md-1">
                <i class="fa fa-thumbs-o-up fa-4x"></i>
            </div>
            <div class="col-md-11" style="text-align:left">
                <h3>PARABÉNS! VOCÊ CONCLUIU O PREENCHIMENTO DO DIAGNÓSTICO.</h3>
                <h4>Caso deseje enviar para elaboração o seu PAR clique abaixo.</h4>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-lg-12 text-right">
            <a target="_blank" id="btnImprimir" class="btn btn-primary btn-lg" href="<?php echo "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativosImpressao&acao=A&inuid={$_REQUEST['inuid']}"; ?>">
                <i class="fa fa-print"></i> Imprimir Diagnóstico
            </a>
            <input type="hidden" name="req" id="req">
            <?php #regra solicitada pelo julio pra não mostra esse botão por tempo inderteminado.
            /* Envio para elaboração desbloqueado na SS 4912*/
            if($diagnosticoFinalizado == 'N' && $esdid == PAR3_ESDID_DIAGNOSTICO_FINALIZADO){ ?>
            <!-- A pedido do FNDE a unidade pode finalizar independente de ter pendencias ou não
            <button id="btnFinalizar" class="btn <?php echo $habilidar ? 'btn-danger' : 'btn-default'; ?> btn-lg" type="button" <?php echo $habilidar ? null : 'disabled="disabled"'; ?>>
            -->
            <button id="btnFinalizar" class="btn btn-danger btn-lg" type="button" >
                <i class="fa fa-check-circle-o"></i> Finalizar Diagnóstico
            </button>
            <?php
            } ?>
        </div>
    </div>
    <br>
<?php
} else if (
    ($_SESSION['visualizar-sintese-diagnostico'] && $situacao['percent'] < 100 && $_REQUEST['resumo'] == 1) ||
    ($esdid == PAR3_WORKFLOW_FLUXO_DO_PAR_DIAGNOSTIGO && $situacao['percent'] < 100 && $_REQUEST['resumo'] == 1)
) { ?>
    <div class="widget red-bg p-lg text-center" id="nao_imprimir">
        <div class="row">
            <div class="col-md-1">
                <i class="fa fa-warning fa-4x"></i>
            </div>
            <div class="col-md-11 text-left">
                <h3>DIAGNÓSTICO</h3>
                <h4>Para finalizar o diagnóstico, será necessário preencher todos os itens indicados abaixo.</h4>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-lg-12 text-right">
            <a target="_blank" id="btnImprimir" class="btn btn-primary btn-lg" href="<?php echo "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativosImpressao&acao=A&inuid={$_REQUEST['inuid']}"; ?>">
                <i class="fa fa-print"></i> Imprimir Diagnóstico
            </a>
            <input type="hidden" name="req" id="req">
            <?php ?>
        </div>
    </div>
    <br>
    <?php if (is_array($arDimensoes)) : ?>
        <div class="row">
            <?php foreach ($arDimensoes as $dimensao) : ?>
                <?php $arAreas = Par3_Controller_ConfiguracaoControle::recuperarAreasGuia($dimensao['dimid']); ?>
                <div class="col-lg-3">
                    <div class="ibox float-e-margins">
                        <div class="ibox-title">
                            <h5><?php echo $dimensao['dimdsc']; ?></h5>
                        </div>
                        <div class="ibox-content no-margin">
                            <table class="table table-stripped">
                                <tbody>
                                <?php if(is_array($arAreas)){ ?>
                                <?php foreach ($arAreas as $area): ?>
                                    <?php $pontuacao = recuperaPontuacaoAreaDiagnostico($_REQUEST['inuid'], $area['areid']); ?>
                                    <?php $total = $oAreaController->pontuacaoArea($area['areid'], $_REQUEST['inuid']); ?>
                                    <tr>
                                        <?php if ($total['percent'] < 100) : ?>
                                            <td>
                                                <i class="fa fa-times-circle text-danger"></i>
                                            </td>
                                            <td>
                                                <?php $item['url'] = "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid={$_REQUEST['inuid']}&dimid={$dimensao['dimid']}&areid={$area['areid']}"; ?>
                                                <a class="text-primary pendencia" href="<?php echo $item['url']; ?>">
                                                    <?php echo $pontuacao['aredsc']; ?> <i class="fa fa-edit"></i>
                                                </a>
                                            </td>
                                        <?php endif;?>
                                    </tr>
                                <?php endforeach; ?>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
            <?php endforeach; ?>
        </div>
    <?php endif; ?>
<?php }

if (is_array($arDimensoes) && (!$existePendenciaGeral || $_SESSION['visualizar-sintese-diagnostico']) && !isset($_GET['resumo'])) {
    foreach($arDimensoes as $dimensoes){
        if($dimensoes['dimid'] == $dimid){
            $dimensao = $dimensoes;
        }
    }

    $ativa = 'active';
    $d = 1;

    if(!empty($dimensao['dimid'])){
    	$arAreas = Par3_Controller_ConfiguracaoControle::recuperarAreasGuia($dimensao['dimid']);
    }
  ?>
<div id="tab-<?php echo $dimensao['dimid']; ?>" class="tab-pane <?php echo $ativa; ?>">
    <div class="panel-body">
        <div class="panel-group" id="accordion">
            <?php if ($arAreas) : $a = 0; ?>
                <?php foreach ($arAreas as $kAre => $area) : $a++;
                    $dadosPercent = $oAreaController->pontuacaoArea($area['areid'], $_REQUEST['inuid']);
                    if ($_REQUEST['areid']) {
                        if ($_REQUEST['areid'] == $area['areid']) {
                            $ativaArea = 'in';
                            $statusArea = 'true';
                        } else {
                            $ativaArea = '';
                            $statusArea = 'false';
                        }
                    } else {
                        if ($a == 1) {
                            $ativaArea = 'in';
                            $statusArea = 'true';
                        } else {
                            $ativaArea = '';
                            $statusArea = 'false';
                        }
                    } ?>
                    <div class="panel panel-default">
                        <div class="panel-heading">
                            <h5 class="panel-title">
                                <a data-toggle="collapse" data-parent="#accordion" class="menuArea"
                                   href="#collapse-<?php echo $area['areid']; ?>"
                                   aria-expanded="true" class=""
                                   tipo="<?php echo $area['areid']; ?>">
                                    <div class="col-md-9"><?php echo $dimensao['dimcod'] . "." . $area['arecod'] . ' - ' . $area['aredsc']; ?></div>
                                    <div class="col-md-3">
                                        <div class="progress">
                                            <div style="width: <?php echo $dadosPercent['percent']; ?>%" aria-valuemax="10" aria-valuemin="0" aria-valuenow="<?php echo $dadosPercent['percent']; ?>" role="progressbar" class="progress-bar progress-bar-success">
                                                <span><?php echo number_format($dadosPercent['percent'],0); ?>% Preenchido</span>
                                            </div>
                                        </div>
                                    </div>
                                </a>
                            </h5>
                        </div>
                        <div id="collapse-<?php echo $area['areid']; ?>"
                             class="panel-collapse panelIndicadores collapse <?php echo $ativaArea; ?>"
                             aria-expanded="<?php echo $statusArea; ?>">
                            <div class="panel-body" style="background-color: #f3f3f4;">
                            <form method="post" name="formulario" id="formulario">
                                <input type="hidden" name="req" id="req[<?php echo $area['areid']; ?>]" value=""/>
                                <input type="hidden" name="url" id="url[<?php echo $area['areid']; ?>]" value=""/>
                                <input type="hidden" name="inuid" id="inuid" value="<?php echo $_REQUEST['inuid'] ?>"/>
                                <input type="hidden" name="dimid" id="dimid" value="<?php echo $_REQUEST['dimid'] ?>"/>
                                <input type="hidden" name="areid" id="areid[<?php echo $area['areid']; ?>]" value="<?php echo $_REQUEST['areid'] ?>"/>
                                <input type="hidden" name="indid" id="indid[<?php echo $area['areid']; ?>]" value="<?php echo $_REQUEST['indid'] ?>"/>
                                    <div class="row">
                                        <div class="col-md-3 colunaIndicadores">
                                            <h5>
                                                <b>INDICADORES</b>
                                            </h5>
                                            <?php
                                            $i = 0;
                                            $indid = 0;
                                            $arIndicadores = Par3_Controller_ConfiguracaoControle::recuperarIndicadoresGuia($area['areid']);
                                            ?>
                                            <?php if ($arIndicadores) { ?>
                                                <ul class="folder-list m-b-md"
                                                    style="padding: 0">
                                                    <?php foreach ($arIndicadores as $indicador) {

                                                        $i++;
                                                        $primeiroSelecao = '';
                                                        if ($i == 1) {
                                                            $indid = $indicador['indid'];
                                                            $primeiroSelecao = $_REQUEST['indid'] ? ' ' : 'menuSelecionado';
                                                        }

                                                        ?>
                                                        <li>
                                                            <a href="#"
                                                               class="menuIndicadores <?php echo $primeiroSelecao; ?> <?php echo($_REQUEST['indid'] === $indicador['indid'] ? 'menuSelecionado' : '') ?>"
                                                               href="#"
                                                               indid="<?php echo $indicador['indid']; ?>"
                                                               areid="<?php echo $area['areid']; ?>"
                                                               dimid="<?php echo $dimensao['dimid']; ?>">
                                                                <?php echo $oPontuacao->buscaPontuacao($_REQUEST['inuid'], $indicador['indid']) ? $iconPreenchido : $iconNaoPreenchido; ?>
                                                                <?php echo $indicador[$indicador['indid']] = $dimensao['dimcod'] . "." . $area['arecod'] . "." . $indicador['indcod'] . " - " . $indicador['inddsc']; ?>
                                                            </a>
                                                        </li>
                                                    <?php } ?>
                                                </ul>
                                            <?php } else { ?>
                                                <p class="alert alert-warning">Não existem indicadores cadastrados</p>
                                            <?php } ?>
                                        </div>
                                        <?php
                                        if ($_REQUEST['dimid'] == $dimensao['dimid'] && $_REQUEST['areid'] == $area['areid'] && $_REQUEST['indid']) {
                                            $indid = $_REQUEST['indid'];
                                        }

                                        $dadosIndicador = $oIndicador->recuperarDados($indid);
                                        $codigoindicador = $oIndicador->recuperaCodigoIndicador($indid);
                                        $metasGuia = $oIndicadorMeta->recuperarDadosColunaMeta($indid);
                                        $indjustificativa = "";
                                        $indmetas = "";
                                        $ptopontuacao = "";
                                        $dadosPontuacao = $oPontuacaoController->recuperaPontuacao($indid, $_REQUEST['inuid']);
                                        $dadosPontuacao = $dadosPontuacao ? $dadosPontuacao : array();
                                        $indjustificativa = $dadosPontuacao['ptojustificativa'];
                                        $ptopontuacao = $dadosPontuacao['ptopontuacao'];
                                        ?>
                                        <div class="col-md-9 colunaIndicadores">
                                            <div class="ibox">
                                                <?php if ($dadosIndicador['indid']) : ?>
                                                    <div class="ibox-title">
                                                        <?php echo $codigoindicador . " - " . $dadosIndicador['inddsc']; ?>
                                                    </div>
                                                    <div class="ibox-content alinhajust">
                                                        <h3>Descrição do Indicador</h3>
                                                        <?php
                                                            echo $dadosIndicador['indajuda'] . "<br><br>";

                                                            if( $dadosIndicador['indlei'] ){
                                                                echo $dadosIndicador['indlei'] . "<br><br>";
                                                            } else {
                                                                if( $metasGuia[0] ){
                                                                    $arrOrder = asort($metasGuia);
                                                                    echo "Metas do PNE relacionadas a este indicador: ".implode(', ', $metasGuia)." <br><br>";
                                                                }
                                                            }
                                                            $tipoPMEPEE = $itrid == 2 ? "PME" : "PEE";
                                                        ?>
                                                        <div class="form-group">
                                                            <p>
                                                                <label>Metas do <?php echo $tipoPMEPEE; ?> relacionadas a este indicador:</label>
                                                            </p>
                                                            <div class="row">
                                                            <?php
                                                            $dadosMeta = array();
                                                            for ($i = 1; $i <= 100; $i++) {
                                                                $dadosMeta[$i] = "Meta ".$i;
                                                            }

                                                            $metasM = array();
                                                            $metas  = array();
                                                                if($dadosPontuacao['ptoid']){
                                                                $metas = $oPontuacaoMeta->recuperarDados($dadosPontuacao['ptoid']);
                                                                if( is_array($metas) ){
                                                                    foreach( $metas as $meta ){
                                                                        $metasM[] = $meta['pmtmeta'];
                                                                    }
                                                                    $metas = $metasM;
                                                                }
                                                            }
                                                            echo $simec->select('metas['.$dadosIndicador['indid'].'][]', null, $metas, $dadosMeta, array('placeHolder' => 'Metas', 'maxlengh' => 1200, "$disabled"));

                                                            ?>
                                                            </div>
                                                        </div>
                                                    </div>
                                                    <div class="ibox-title">
                                                        <h3>Componentes do Indicador</h3>
                                                    </div>
                                                    <div class="ibox-content">
                                                        <?php
                                                            $arCriterios = Par3_Controller_ConfiguracaoControle::recuperarCriteriosGuia($dadosIndicador['indid']);
                                                            if ($arCriterios) {
                                                                $i = 0;
                                                                foreach ($arCriterios as $criterio) {
                                                                    $i++;
                                                                    $classEscondeDiv = 'esconde-div';
                                                                    $checked = '';
                                                                    if($dadosPontuacao['criterios'][$dadosPontuacao['ptoid']][$criterio['crtid']]){
                                                                        $classEscondeDiv = '';
                                                                        $checked = 'checked';
                                                                        $pontuacao[$criterio['crtid']] = $dadosPontuacao['criterios'][$dadosPontuacao['ptoid']][$criterio['crtid']]['pontuacao'] ? $dadosPontuacao['criterios'][$dadosPontuacao['ptoid']][$criterio['crtid']]['pontuacao'] : null;
                                                                    }
                                                                ?>
                                                                <div class="form-group">
                                                                        <?php if ($dadosIndicador['indrequerido'] == 'S' && $criterio['crvvinculo'] == 't') { $checked = 'checked'; $classEscondeDiv = ''; ?>
                                                                        <div class="checkbox" style="padding-left: 0px;">
                                                                            <input
                                                                                type="checkbox" style="display: none;"
                                                                                id="indicador-<?php echo $dimensao['dimid']; ?>-<?php echo $area['areid']; ?>-<?php echo $dadosIndicador['indid']; ?>-<?php echo $criterio['crtid']; ?>"
                                                                                name="indicador[<?php echo $dimensao['dimid']; ?>][<?php echo $area['areid']; ?>][<?php echo $dadosIndicador['indid']; ?>][]"
                                                                                value="<?php echo $criterio['crtid']; ?>" <?php echo $checked; ?>
                                                                                onclick="verificaRegra(this,<?php echo $dimensao['dimid']; ?>,<?php echo $area['areid']; ?>,<?php echo $dadosIndicador['indid']; ?>,<?php echo $criterio['crtid']; ?>, 1);" <?php echo $disabled ?>>
                                                                            <span style=""><?php echo $criterio['crtdsc']; ?></span>
                                                                        <?php } else { ?>
                                                                        <div class="checkbox">
                                                                            <input
                                                                                type="checkbox"
                                                                                id="indicador-<?php echo $dimensao['dimid']; ?>-<?php echo $area['areid']; ?>-<?php echo $dadosIndicador['indid']; ?>-<?php echo $criterio['crtid']; ?>"
                                                                                name="indicador[<?php echo $dimensao['dimid']; ?>][<?php echo $area['areid']; ?>][<?php echo $dadosIndicador['indid']; ?>][]"
                                                                                value="<?php echo $criterio['crtid']; ?>" <?php echo $checked; ?>
                                                                                onclick="verificaRegra(this,<?php echo $dimensao['dimid']; ?>,<?php echo $area['areid']; ?>,<?php echo $dadosIndicador['indid']; ?>,<?php echo $criterio['crtid']; ?>, 1);" <?php echo $disabled ?>>
                                                                            <label for="indicador-<?php echo $dimensao['dimid']; ?>-<?php echo $area['areid']; ?>-<?php echo $dadosIndicador['indid']; ?>-<?php echo $i; ?>"><?php echo $criterio['crtdsc']; ?></label>
                                                                        <?php } ?>
                                                                        <script>verificaRegra($("#indicador-<?php echo $dimensao['dimid']; ?>-<?php echo $area['areid']; ?>-<?php echo $dadosIndicador['indid']; ?>-<?php echo $criterio['crtid']; ?>"),<?php echo $dimensao['dimid']; ?>,<?php echo $area['areid']; ?>,<?php echo $dadosIndicador['indid']; ?>,<?php echo $criterio['crtid']; ?>, 2);</script>
                                                                    </div>
                                                                    <?php if( $criterio['crtpontuacao'] == 1 || $dadosIndicador['indrequerido'] == 'S' && $criterio['crvvinculo'] == 't'){ ?>
                                                                        <div class="<?php echo $classEscondeDiv; ?> row" style="padding-left: 20px; padding-right: 20px;" id="<?php echo $criterio['crtid']; ?>">
                                                                            <div id="ionrange_<?php echo $criterio['crtid']; ?>" ctrid="<?php echo $criterio['crtid']; ?>" ></div>
                                                                            <script>$('#ionrange_<?php echo $criterio['crtid']; ?>').ionRangeSlider({min: 0, max: 100, from: <?php echo $pontuacao[$criterio['crtid']] ? $pontuacao[$criterio['crtid']] : 0; ?>, postfix: " %"})</script>
                                                                            <input id="ptcpontuacao_<?php echo $criterio['crtid']; ?>" name="ptcpontuacao[<?php echo $criterio['crtid']; ?>]" type="hidden" value="<?php echo $pontuacao[$criterio['crtid']]; ?>">
                                                                        </div>
                                                                    <?php } ?>
                                                                </div>
                                                            <?php } ?>
                                                        <?php } else { ?>
                                                            <p class="alert alert-warning">
                                                                Sem critérios cadastrados
                                                                para o indicador.</p>
                                                        <?php } ?>
                                                        <div class="row"></div>

                                                        <div class="form-group">
                                                            <p>
                                                                <label>Justificativa:</label>
                                                            </p>
                                                            <textarea
                                                                name="indjustificativa[<?php echo $dadosIndicador['indid']; ?>]" <?php echo $disabled; ?>
                                                                id="indjustificativa[<?php echo $dadosIndicador['indid']; ?>]"
                                                                class="form-control"
                                                                required="required"
                                                                label-for="indjustificativa[<?php echo $dadosIndicador['indid']; ?>]"><?php echo $indjustificativa; ?></textarea>
                                                        </div>
                                                    </div>
                                                    <?php
                                                        $arrIndicadorAnterior = $oIndicador->buscaIndicadorAnterior($dadosIndicador['indid']);
                                                        $arrIndicadorProximo  = $oIndicador->buscaIndicadorProximo($dadosIndicador['indid']);
                                                    ?>
                                                    <div class="ibox-footer">
                                                        <div class="row">
                                                            <div class="col-md-5 text-left">
                                                                <?php if($arrIndicadorAnterior['indid'] != ''){?>
                                                                <button type="button"
                                                                        class="btn btn-success"
                                                                        onclick="window.location.href='par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=<?php echo $_REQUEST['inuid']?>&dimid=<?php echo $arrIndicadorAnterior['dimid']?>&areid=<?php echo $arrIndicadorAnterior['areid']?>&indid=<?php echo $arrIndicadorAnterior['indid']?>'">
                                                                    <i class="fa fa-arrow-left"></i>
                                                                    Indicador Anterior
                                                                </button>
                                                                <?php }?>
                                                            </div>
                                                            <div class="col-md-2">
                                                                <center>
                                                                   <?php if(!$disabled){?>
                                                                    <button type="submit"
                                                                            class="btn btn-success salvar"
                                                                            indid="<?php echo $dadosIndicador['indid']; ?>"
                                                                            areid="<?php echo $area['areid']; ?>"
                                                                            dimid="<?php echo $dimensao['dimid']; ?>" <?php echo $disabled; ?>>
                                                                        <i class="fa fa-save"></i>
                                                                        Salvar Indicador
                                                                    </button>
                                                                    <?php }?>
                                                                </center>
                                                            </div>
                                                            <div class="col-md-5 text-right">
                                                                <?php if($arrIndicadorProximo['indid'] != ''){?>
                                                                   <?php if(!$disabled){?>
                                                                <button type="submit"
                                                                        class="btn btn-success salvar"
                                                                        indid="<?php echo $dadosIndicador['indid']; ?>"
                                                                        areid="<?php echo $area['areid']; ?>"
                                                                        dimid="<?php echo $dimensao['dimid']; ?>" <?php echo $disabled; ?>
                                                                        url="par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=<?php echo $_REQUEST['inuid']?>&dimid=<?php echo $arrIndicadorProximo['dimid']?>&areid=<?php echo $arrIndicadorProximo['areid']?>&indid=<?php echo $arrIndicadorProximo['indid']?>">
                                                                    <i class="fa fa-save"></i>
                                                                    Salvar e Ir ao Próximo Indicador
                                                                    <i class="fa fa-arrow-right"></i>
                                                                </button>
                                                                   <?php }?>
                                                                <?php }?>
                                                            </div>
                                                        </div>
                                                    </div>
                                                <?php else : ?>
                                                    <p class="alert alert-warning">Indicador não cadastrado.</p>
                                                <?php endif; ?>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </form>
                        </div>
                    </div>
                <?php endforeach; ?>
            <?php endif; ?>
        </div>
    </div>
</div>
<?php
}

if ($esdid == PAR3_WORKFLOW_FLUXO_DO_PAR_ELABORACAO && 1 == 0) : ?>
    <p class="text-justify">
        Cada um dos indicadores do diagnóstico, bem como seus componentes, apontam aspectos desejáveis de boas políticas de educação, em suas várias dimensões. Revisar o instrumento diagnóstico, com especial atenção aos componentes não selecionados durante o processo, pode fornecer indicações importantes dos caminhos e estratégias a serem tomados para aprimorar as ofertas em educação no seu estado ou município nos próximos quatro anos.
    </p>
    <p class="text-justify">
        Clique nas dimensões e áreas listadas acima para revisitar o diagnóstico e rever os espaços apontados pelo instrumento para possíveis ações de aperfeiçoamento. Essas indicações, poderão ser utilizadas na fase de Elaboração do Plano de Ações Articuladas, de maneira a torna-lo mais adequado à realidade do seu estado ou município.
    </p>

    <br>

    <?php if ($arDimensoes) : $d = 0; ?>
        <?php foreach ($arDimensoes as $dimensao) : $a = 0; ?>
            <?php $pontuacao = recuperaPontuacaoDiagnostico($_REQUEST['inuid'], $dimensao['dimid']); $d++; ?>
            <?php $arAreas = Par3_Controller_ConfiguracaoControle::recuperarAreasGuia($dimensao['dimid']); ?>
            <div class="row">
                <div class="col-lg-12">
                    <table class="table table-bordered">
                        <thead>
                        <tr>
                            <th colspan="3" style="text-align: center;">
                                <?php echo "Dimensão {$d} - {$dimensao['dimdsc']}"; ?>
                                <span style="padding: 6px;" class="label label-success pull-right">Total Selecionado: <?php echo $pontuacao['pontuacao']; ?></span>
                            </th>
                        </tr>
                        </thead>
                        <tbody>
                        <?php if(is_array($arAreas)){ ?>
                        <?php foreach ($arAreas as $area) : $a++; ?>
                            <?php $pontuacaoArea = recuperaPontuacaoAreaDiagnostico($_REQUEST['inuid'], $area['areid']); ?>
                            <?php $item['url'] = "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid={$_REQUEST['inuid']}&dimid={$dimensao['dimid']}&areid={$area['areid']}"; ?>
                            <?php if ($d != 4): ?>
                                <tr>
                                    <td width="80%" style="vertical-align: middle;"><b><a class="area-link" href="<?php echo $item['url']; ?>"><?php echo "Área {$a}. {$area['aredsc']}"; ?></a></b></td>
                                    <td align="center" width="20%" class="" style="text-align: center; vertical-align: middle;">
                                        <div class="forum-info pull-left" style="margin-left: 40px;">
                                    <span class="views-number">
                                    <?php echo $pontuacaoArea['pontuacao'] ?>
                                    </span>
                                            <div>
                                                <small>Selecionado</small>
                                            </div>
                                        </div>
                                        <small style="text-align: center; margin-top: 12px; display: inline-block;">de</small>
                                        <div class="forum-info pull-right" style="margin-right: 40px;">
                                    <span class="views-number">
                                    <?php echo $pontuacaoArea['total'] ?>
                                    </span>
                                            <div>
                                                <small>Total</small>
                                            </div>
                                        </div>
                                    </td>
                                </tr>
                            <?php else : ?>
                                <?php
                                $arIndicadores = Par3_Controller_ConfiguracaoControle::recuperarIndicadoresGuia($area['areid']);

                                $arIndicadoresPontuacao = array();
                                $arIndicadoresPontuacao[0]['descricao'] = 'ate 25%';
                                $arIndicadoresPontuacao[0]['pontuacao'] = 0;

                                $arIndicadoresPontuacao[1]['descricao'] = 'entre 25% e 50%';
                                $arIndicadoresPontuacao[1]['pontuacao'] = 0;

                                $arIndicadoresPontuacao[2]['descricao'] = 'entre 50% e 75%';
                                $arIndicadoresPontuacao[2]['pontuacao'] = 0;

                                $arIndicadoresPontuacao[3]['descricao'] = 'acima de 75%';
                                $arIndicadoresPontuacao[3]['pontuacao'] = 0;

                                $i = 0;

                                foreach($arIndicadores as $indicador) {
                                    $pontuacaoIndicador = recuperaPontuacaoCriteriosDiagnostico($_REQUEST['inuid'], $indicador['indid']);
                                    $pontuacaoIndicador = is_array($pontuacaoIndicador) ? $pontuacaoIndicador : array();

                                    foreach ($pontuacaoIndicador as $pontuacao) {
                                        if ($pontuacao['percentual'] < 25 || empty($pontuacao['percentual'])) {
                                            $arIndicadoresPontuacao[0]['pontuacao']++;
                                        } else if ($pontuacao['percentual'] < 50 && $pontuacao['percentual'] > 25) {
                                            $arIndicadoresPontuacao[1]['pontuacao']++;
                                        } else if ($pontuacao['percentual'] < 75 && $pontuacao['percentual'] > 50) {
                                            $arIndicadoresPontuacao[2]['pontuacao']++;
                                        } else {
                                            $arIndicadoresPontuacao[3]['pontuacao']++;
                                        }
                                    }
                                }
                                ?>
                                <tr>
                                    <td width="80%" style="vertical-align: middle;"><b><a class="area-link" href="<?php echo $item['url']; ?>"><?php echo "Área {$a}. {$area['aredsc']}"; ?></a></b></td>
                                    <td align="center" width="20%" style="text-align: center; vertical-align: middle; padding: 9px;">
                                        <?php for ($i = 0; $i < 4; $i++) : ?>
                                            <div class="row" style="padding: 14px 0px 14px 0px; margin: -8px;">
                                                <div class="forum-info pull-left" style="margin-left: 30px; width: 35%;">
                                    <span class="views-number">
                                        <?php echo $arIndicadoresPontuacao[$i]['pontuacao'] ?>
                                    </span>
                                                    <div>
                                                        <small><?php echo $arIndicadoresPontuacao[$i]['descricao']; ?></small>
                                                    </div>
                                                </div>
                                                <small style="margin-right: 30px; text-align: center; margin-top: 12px; display: inline-block;">de</small>
                                                <div class="forum-info pull-right" style="margin-right: 40px;">
                                    <span class="views-number">
                                        <?php echo $pontuacaoArea['total'] ?>
                                    </span>
                                                    <div>
                                                        <small>Total</small>
                                                    </div>
                                                </div>
                                            </div>
                                        <?php endfor; ?>
                                    </td>
                                </tr>
                            <?php endif; ?>
                        <?php endforeach; ?>
                        <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>
        <?php endforeach; ?>
        <div class="row">
            <div class="col-lg-12 text-center">
                <a target="_blank" id="btnImprimir" class="btn btn-primary btn-lg" href="<?php echo "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativosImpressao&acao=A&inuid={$_REQUEST['inuid']}"; ?>">
                    <i class="fa fa-print"></i> Imprimir Diagnóstico
                </a>
            </div>
        </div>
    <?php endif; ?>
<?php endif;
//se estado do diagnostico estiver em diagnostico finalizado ou em uma etapa a frente libera "VOCÊ FINALIZOU A ETAPA DE DIAGNÓSTICO DO PLANO DE AÇÕES ARTICULADAS"
$arrayDiagnosticoToPlanejamento = array(PAR3_ESDID_ETAPA_PREPARATORIA_DO_DIAGNOSTICO,PAR3_ESDID_DIAGNOSTICO_ELABORACAO, PAR3_ESDID_DIAGNOSTICO_FINALIZADO);
if (!in_array($esdid,$arrayDiagnosticoToPlanejamento) && $situacao['percent'] >= 100 && ($_SESSION['visualizar-sintese-diagnostico']||$_REQUEST['resumo'])){ ?>
    <div class="widget navy-bg p-lg text-center">
        <div class="row">
            <div class="col-md-1">
                <i class="fa fa-thumbs-o-up fa-4x"></i>
            </div>
            <div class="col-md-11" style="text-align:left">
                <h3>VOCÊ FINALIZOU A ETAPA DE DIAGNÓSTICO DO PLANO DE AÇÕES ARTICULADAS</h3>
                <h4>O quadro abaixo informa quantos itens foram selecionados dentre os possíveis, em cada uma das dimensões e áreas.</h4>
            </div>
        </div>
    </div>
        <div class="row">
            <div class="col-lg-12 text-center">
                <a target="_blank" id="btnImprimir" class="btn btn-primary btn-lg" href="<?php echo "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativosImpressao&acao=A&inuid={$_REQUEST['inuid']}"; ?>">
                    <i class="fa fa-print"></i> Imprimir Diagnóstico
                </a>
            </div>
        </div>
<?php } ?>

