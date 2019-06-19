<?php
$arInstrumentos = Par3_Controller_ConfiguracaoControle::recuperarIntrumentosGuia( $_REQUEST['itrid'] );
$itrid = null;
$dimid = null;
$areid = null;
$indid = null;
?>
<table>
    <thead>
    <tr>
        <th style="width: 10%"><b>ABRANGÊNCIA</b></th>
        <th style="width: 15%"><b>DIMENSÃO</b></th>
        <th style="width: 20%"><b>ÁREA</b></th>
        <th style="width: 25%"><b>INDICADOR</b></th>
        <th style="width: 30%"><b>COMPONENTE</b></th>
    </tr>
    </thead>
    <tbody>
    <?php foreach ($arInstrumentos as $instrumento): ?>

        <!-- DIMESOES -->
        <?php $arDimensoes = Par3_Controller_ConfiguracaoControle::recuperarDimensoesGuia($instrumento['itrid']); ?>
        <?php if ($arDimensoes): ?>
            <?php foreach ($arDimensoes as $dimensao): ?>

                <!-- AREAS -->
                <?php $arAreas = Par3_Controller_ConfiguracaoControle::recuperarAreasGuia($dimensao['dimid']); ?>
                <?php if ($arAreas): ?>
                    <?php foreach ($arAreas as $area): ?>

                        <!-- INDICADORES -->
                        <?php $arIndicadores = Par3_Controller_ConfiguracaoControle::recuperarIndicadoresGuia($area['areid']); ?>
                        <?php if ($arIndicadores): ?>
                            <?php foreach ($arIndicadores as $indicador): ?>

                                <!-- CRITERIOS -->
                                <?php $arCriterios = Par3_Controller_ConfiguracaoControle::recuperarCriteriosGuia($indicador['indid']); ?>
                                <?php if ($arCriterios): ?>
                                    <?php foreach ($arCriterios as $criterio): ?>

                                        <tr>
                                            <td style="width: 10%"><?php echo $itrid != $instrumento['itrid'] ? $instrumento['itrdsc'] : ''; ?></td>
                                            <td style="width: 15%"><?php echo $dimid != $dimensao['dimcod'] ? $dimensao['dimcod'] . ' - ' . $dimensao['dimdsc'] : ''; ?></td>
                                            <td style="width: 20%"><?php echo $areid != $area['areid'] ? $dimensao['dimcod'] . "." . $area['arecod'] . ' - ' . $area['aredsc'] : ''; ?></td>
                                            <td style="width: 25%"><?php echo $indid != $indicador['indid'] ? $indicador[$indicador['indid']] = $dimensao['dimcod'] . "." . $area['arecod'] . "." . $indicador['indcod'] . " - " . $indicador['inddsc'] : ''; ?></td>
                                            <td style="width: 30%"><?php echo "(" . $criterio['crtcod'] . ") " . $criterio['crtdsc']; ?></td>
                                        </tr>
                                        <?php $itrid = $instrumento['itrid']; ?>
                                        <?php $dimid = $dimensao['dimcod']; ?>
                                        <?php $areid = $area['areid']; ?>
                                        <?php $indid = $indicador['indid']; ?>
                                    <?php endforeach; ?>
                                <?php endif; ?>

                            <?php endforeach; ?>
                        <?php endif; ?>

                    <?php endforeach; ?>
                <?php endif; ?>

            <?php endforeach; ?>
        <?php endif; ?>

    <?php endforeach; ?>
    </tbody>
</table>

<?php
    header ("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
    header ("Cache-Control: no-cache, must-revalidate");
    header ("Pragma: no-cache");
    header ("Content-type: application/x-msexcel");
    header ("Content-Disposition: attachment; filename=\"guia-" . date('Y-m-d') . ".xls");
    exit();
?>