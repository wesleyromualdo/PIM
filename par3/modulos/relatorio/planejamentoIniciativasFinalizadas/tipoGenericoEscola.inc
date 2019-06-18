<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/9/2017
 * Time: 10:40 AM
 */
?>


<div class="row">
    <div class="pcol-md-12">
        <table style='border: 1px solid #000000; border-collapse: collapse;'>
            <thead>
            <th width="50px;" style="border: 1px solid;">Ano</th>
            <th width="200px;" style="border: 1px solid;">Item de Composicao</th>
            <th width="200px;" style="border: 1px solid;">Unidade</th>
            <th width="200px;" style="border: 1px solid;">Valor Referência</th>
            <th width="200px;" style="border: 1px solid;">Quantidade</th>
<!--            <th class="pth">Valor</th>-->
            <th class="pth">Total</th>
            </thead>
            <tbody>
           <tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>
                <td style="border: 1px solid; border-collapse: collapse;"><?php echo $dbItemComposicao['ipiano'];?></td>
                <td style="border: 1px solid; border-collapse: collapse;"><?php echo $dbItemComposicao['itcdsc']?></td>
                <td style="border: 1px solid; border-collapse: collapse;"><?php echo $dbItemComposicao['unidsc']?></td>
                <td style="border: 1px solid; border-collapse: collapse;"> R$<?php echo number_format($dbItemComposicao['ipivalorreferencia'],2,',','.')?></td>
                <td style="border: 1px solid; border-collapse: collapse;"><?php echo $dbItemComposicao['ipiquantidade']?></td>
<!--                <td style='border: solid 1px #000000; padding:5px;'>R$ --><?php //echo number_format($dbItemComposicao['ipvalor_planejado_total'],0,',','.')?><!--</td>-->
                <td style="border: 1px solid; border-collapse: collapse;">R$ <?php echo number_format(($dbItemComposicao['ipivalorreferencia'] * $dbItemComposicao['ipiquantidade']),2,',','.');?></td>
            </tr>
            </tbody>
        </table>
    </div>
</div>
<br/>
<div class="row">
    <div class="pcol-md-12">
        <table style='border: 1px solid #000000; border-collapse: collapse;'>
            <thead>
            <th width="200px;">Escola</th>
            <th width="200px;">Código INEP</th>
            <th width="200px;">Localização</th>
            <th width="50px;">Quantidade</th>
            <th width="50px;">Quantidade de Salas</th>
            <th width="50px;">Quantidade de Alunos</th>
            </thead>
            <tbody>
            <?php foreach($dbItensComposicaoPorEscolas as $key => $value):?>
               <tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $value['escnome']; ?></td>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $value['esccodinep']; ?></td>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $value['escno_localizacao']; ?></td>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $value['ipequantidade']; ?></td>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $value['escqtd_salas']; ?></td>
                    <td style='border: solid 1px #000000; padding:5px;'><?php echo $value['escqtd_alunos']; ?></td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        </table>
    </div>
</div>
<br/>