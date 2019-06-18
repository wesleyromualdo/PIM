<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/6/2017
 * Time: 4:59 PM
 */
?>
<div class="row">
    <div class="pcol-md-12">
        <table style='border: 1px solid #000000; border-collapse: collapse;'>
            <thead>
                <th>Ano</th>
                <th width="200px;">Item de Composicao</th>
                <th width="200px;">Unidade</th>
                <th width="200px;">Valor Referência</th>
                <th width="200px;">Quantidade</th>
<!--                <th>Valor</th>-->
                <th>Total</th>
            </thead>
            <tbody>
           <tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>
                    <td style="color:red;"><?php ver($dbItemComposicao); echo $dbItemComposicao['ipiano'];?></td>
                    <td><?php echo $dbItemComposicao['itcdsc']?></td>
                    <td><?php echo $dbItemComposicao['unidsc']?></td>
                    <td> R$<?php echo number_format($dbItemComposicao['ipivalorreferencia'],2,',','.')?></td>
                    <td><?php echo $dbItemComposicao['quantidade_por_escola']?></td>
<!--                    <td>R$ --><?php //echo number_format($dbItemComposicao['ipvalor_planejado_total'],2,',','.')?><!--</td>-->
                    <td>R$ <?php echo number_format(($dbItemComposicao['ipivalorreferencia'] * $dbItemComposicao['quantidade_por_escola']),2,',','.');?></td>
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
                <th>Escola</th>
                <th>Código INEP</th>
                <th>Localização</th>
                <th>Quantidade</th>
                <th>Quantidade de Salas</th>
                <th>Quantidade de Alunos</th>
            </thead>
            <tbody>
            <?php foreach($dbItensComposicaoPorEscolas as $key => $value):?>
               <tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>
                    <td><?php echo $value['escnome']; ?></td>
                    <td><?php echo $value['esccodinep']; ?></td>
                    <td><?php echo $value['escno_localizacao']; ?></td>
                    <td><?php echo $value['ipequantidade']; ?></td>
                    <td><?php echo $value['escqtd_salas']; ?></td>
                    <td><?php echo $value['escqtd_alunos']; ?></td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        </table>
    </div>
    <br/>


