<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/6/2017
 * Time: 5:53 PM
 */?>

<div class="row">
    <div class="pcol-md-12">
        <table style='border: 1px solid #000000; border-collapse: collapse;'>
            <thead>
                <th>Ano</th>
                <th>Item Composição</th>
                <th>Unidade</th>
                <th>Valor Referência</th>
                <th>Valor Quantidade</th>
                <th>Total</th>
            </thead>
            <tbody>
            <?php foreach($dadosItensComposicao as $key => $values){?>
               <tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>
                    <td><?php echo $values['ipiano']?></td>
                    <td><?php echo $values['itcespecificacao']?></td>
                    <td><?php echo $values['unidesc']?></td>
                    <td>R$ <?php echo number_format($values['ipivalorreferencia'],2,',','.')?></td>
                    <td><?php echo $values['ipiquantidade']?></td>
                    <td>R$ <?php echo number_format(($values['ipiquantidade'] * $values['ipivalorreferencia']),2,',','.')?></td>
                </tr>
            <?php } ?>
            </tbody>
        </table>
    </div>
</div>
<br/>

