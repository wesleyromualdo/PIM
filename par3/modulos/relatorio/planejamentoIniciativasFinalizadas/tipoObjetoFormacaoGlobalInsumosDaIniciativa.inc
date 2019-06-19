
<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/8/2017
 * Time: 3:32 PM
 */?>

<div class="row">
    <div class="col-md-12">
        <h3>Insumos da Iniciativa</h3>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <table style='border: 1px solid #000000; border-collapse: collapse;'>
            <thead>
            <th>Descrição</th>
            <th>Unidade Insumo 1</th>
            <th>Qtd Insumo 1</th>
            <th>Unidade Insumo 2</th>
            <th>Qtd Insumo 2</th>
            <th>Quantidade</th>
            <th>Valor Unitário</th>
            <th>Valor Total</th>
            </thead>
            <tbody>
            <?php foreach($dadosInsumosDaIniciativa as $key => $value) :?>
                <tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>
                    <td style='border: solid 1px #000000; padding:5px;'> <?php echo $value['ipiindescricao'];?></td>
                    <td style='border: solid 1px #000000; padding:5px;'> <?php echo $value['ipiinunidadeinsumo1'];?></td>
                    <td style='border: solid 1px #000000; padding:5px;'> <?php echo $value['ipiinqtdinsumo1'];?></td>
                    <td style='border: solid 1px #000000; padding:5px;'> <?php echo $value['ipiinunidadeinsumo2'];?></td>
                    <td style='border: solid 1px #000000; padding:5px;'> <?php echo $value['ipiinqtdinsumo2'];?></td>
                    <td style='border: solid 1px #000000; padding:5px;'> <?php echo $value['ipiinqtd'];?></td>
                    <td style='border: solid 1px #000000; padding:5px;'>R$ <?php echo number_format($value['ipiinvalunit'],2,',','.');?></td>
                    <td style='border: solid 1px #000000; padding:5px;'>R$ <?php echo number_format($value['ipiinvaltotal'],2,',','.');?></td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        </table>
    </div>
</div>
<br/>