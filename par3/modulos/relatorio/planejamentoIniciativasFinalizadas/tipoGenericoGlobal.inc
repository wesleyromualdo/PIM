<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/9/2017
 * Time: 10:10 AM
 */
?>
<div class="row">
    <div class="pcol-md-12">
        <table style='border: 1px solid #000000; border-collapse: collapse;'>
            <thead>
            <th style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'>Ano</th>
            <th style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'>Item Composição</th>
            <th style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'>Unidade</th>
            <th style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'>Valor Referência</th>
            <th style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'>Quantidade</th>
            <th style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'>Total</th>
            </thead>
            <tbody>
            <?php foreach($dadosItensComposicao as $key => $values){?>
                <tr style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'>
                    <td style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'><?php echo $values['ipiano']?></td>
                    <td style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'><?php echo $values['itcespecificacao']?></td>
                    <td style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'><?php echo $values['unidsc']?></td>
                    <td style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'>R$ <?php echo number_format($values['ipivalorreferencia'],2,',','.')?></td>
                    <td style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'><?php echo $values['ipiquantidade']?></td>
                    <td style='border: solid 1px #000000; padding:5px; border-collapse: collapse;'>R$ <?php echo number_format(($values['ipivalorreferencia'] * $values['ipiquantidade']),2,',','.')?></td>
                </tr>
            <?php } ?>
            </tbody>
        </table>
    </div>
</div>
<br/>
