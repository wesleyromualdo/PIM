<?php
$controleUnidade   = new Par3_Controller_InstrumentoUnidade();
$controlePontuacao = new Par3_Controller_Pontuacao();
$itrid = $controleUnidade->pegarItrid($_REQUEST['inuid']);
?>
<table>
    <tbody>
    <?php $arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($itrid); ?>
    <?php if ($arDimensoes): ?>
        <?php foreach ($arDimensoes as $dimensao): ?>
            <?php  $dimensaoTitle = ($dimid != $dimensao['dimcod']) ? $dimensao['dimcod'] . ' - ' . $dimensao['dimdsc'] : ''; ?>
            <?php $arAreas = Par3_Controller_ConfiguracaoControle::recuperarAreasGuia($dimensao['dimid']); ?>
            <?php if ($arAreas): ?>
                <?php foreach ($arAreas as $area): ?>
                    <?php $areaTitle = ($areid != $area['areid']) ? $dimensao['dimcod'] . '.' . $area['arecod'] . ' - ' . $area['aredsc'] : ''; ?>
                    <!-- INDICADORES -->
                    <?php $arIndicadores = Par3_Controller_ConfiguracaoControle::recuperarIndicadoresGuia($area['areid']); ?>
                    <?php if ($arIndicadores): ?>
                        <?php foreach ($arIndicadores as $indicador): ?>

                            <!-- DIMESOES -->
                            <tr style="vertical-align: top; background-color: #a6a6a6;">
                                <td width="10%" style="white-space: initial !important; padding: 5px; vertical-align: top;">
                                    <strong>DIMENSÃO</strong>
                                </td>
                                <td width="90%"  style="white-space: initial !important; padding: 5px; vertical-align: top;">
                                    <?php echo $dimensaoTitle; ?>
                                </td>
                            </tr>

                            <!-- AREAS -->
                            <tr style="background-color: #b3b3b3;">
                                <td width="10%" style="white-space: initial !important; padding: 5px; vertical-align: top;">
                                    <strong>ÁREA</strong>
                                </td>
                                <td width="90%" style="white-space: initial !important; padding: 5px; vertical-align: top;">
                                    <?php echo $areaTitle; ?>
                                </td>
                            </tr>

                            <!-- INDICADORES -->
                            <tr style="background-color: #bfbfbf;">
                                <td width="10%" style="white-space: initial !important; padding: 5px; vertical-align: top;">
                                    <strong>INDICADOR</strong>
                                </td>
                                <td width="90%" style="white-space: initial !important; padding: 5px; vertical-align: top;">
                                    <?php echo $indid != $indicador['indid'] ? $indicador[$indicador['indid']] = $dimensao['dimcod'] . "." . $area['arecod'] . "." . $indicador['indcod'] . " - " . $indicador['inddsc'] : ''; ?>
                                </td>
                            </tr>

                            <?php
                                $dadosPontuacao = $controlePontuacao->recuperaPontuacao($indicador['indid'], $_REQUEST['inuid']);
                                $dadosPontuacao = $dadosPontuacao ? $dadosPontuacao : array();
                            ?>
                            <!-- CRITERIOS -->
                            <?php $arCriterios = Par3_Controller_ConfiguracaoControle::recuperarCriteriosGuia($indicador['indid']); ?>
                            <?php if ($arCriterios): ?>
                            <tr style="background-color: #cccccc;">
                                <td width="10%" style="white-space: initial !important; padding: 5px; vertical-align: top;">
                                    <strong>COMPONENTE</strong>
                                </td>
                                <td width="90%" >
                                    <table>
                                    <?php foreach ($arCriterios as $criterio): ?>
                                        <?php $checked = $dadosPontuacao['criterios'][$dadosPontuacao['ptoid']][$criterio['crtid']] ? '&#9745;' : '&#9744;'; ?>
                                        <tr>
                                            <td style="white-space: initial !important; padding: 5px; vertical-align: top;">
                                                <?php echo $checked; ?>
                                                <?php echo "(" . $criterio['crtcod'] . ") " . $criterio['crtdsc']; ?>
                                                <?php if ( $criterio['crtpontuacao'] == 1 || $indicador['indrequerido'] == 'S' && $criterio['crvvinculo'] == 't' ) : ?>
                                                    <?php
                                                    $pontuacao[$criterio['crtid']] = $dadosPontuacao['criterios'][$dadosPontuacao['ptoid']][$criterio['crtid']]['pontuacao'] ? $dadosPontuacao['criterios'][$dadosPontuacao['ptoid']][$criterio['crtid']]['pontuacao'] : 0;
                                                    ?>
                                                    <small><b>(<?php echo $pontuacao[$criterio['crtid']] ?>%)</b></small>
                                                <?php endif; ?>
                                            </td>
                                        </tr>
                                    <?php endforeach; ?>
                                    </table>
                                </td>
                            </tr>
                            <!-- JUSTIFICATIVA -->
                            <tr style="background-color: #d9d9d9;">
                                <td width="10%" style="white-space: initial !important; padding: 5px; vertical-align: top;">
                                    <strong>JUSTIFICATIVA</strong>
                                </td>
                                <td width="90%">
                                    <table>
                                    <?php foreach ($arCriterios as $criterio): ?>
                                        <?php $indjustificativa = $indid != $indicador['indid'] ? $dadosPontuacao['ptojustificativa'] : null; ?>
                                        <?php if (!empty($indjustificativa)) : ?>
                                            <tr>
                                                <td width="" style="white-space: initial !important; padding: 5px; vertical-align: top;">
                                                    <?php echo $indjustificativa; ?>
                                                </td>
                                            </tr>
                                        <?php endif; ?>
                                        <?php $itrid = $instrumento['itrid']; ?>
                                        <?php $dimid = $dimensao['dimcod']; ?>
                                        <?php $areid = $area['areid']; ?>
                                        <?php $indid = $indicador['indid']; ?>
                                    <?php endforeach; ?>
                                    </table>
                                </td>
                            </tr>
                            <tr style="background-color: #fff;">
                                <td style="white-space: initial !important; padding: 5px; vertical-align: top;">&nbsp;</td>
                                <td style="white-space: initial !important; padding: 5px; vertical-align: top;">&nbsp;</td>
                            </tr>
                            <?php endif; ?>
                        <?php endforeach; ?>
                    <?php endif; ?>

                <?php endforeach; ?>
            <?php endif; ?>

        <?php endforeach; ?>
    <?php endif; ?>
    </tbody>
</table>

<?php
header('Last-Modified: ' . gmdate('D,d M YH:i:s') . ' GMT');
header('Cache-Control: no-cache, must-revalidate');
header('Pragma: no-cache');
header('Content-type: application/x-msexcel');
header('Content-Disposition: attachment; filename=\'diagnostico-' . date('Y-m-d') . '.xls');
die;
?>