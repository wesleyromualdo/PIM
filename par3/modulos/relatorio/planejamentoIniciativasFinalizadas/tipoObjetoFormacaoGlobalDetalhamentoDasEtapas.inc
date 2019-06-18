
<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/8/2017
 * Time: 2:40 PM
 */?>
<div class="row">
    <div class="col-md-12">
        <h3>Detalhamento das Etapas</h3>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <table style='border: 1px solid #000000; border-collapse: collapse;'>
            <thead>
            <th>Atividade</th>
            <th>Descrição Etapa</th>
            <th>Unidade</th>
            <th>Quantidade</th>
            <th>Total de Horas</th>
            </thead>
            <tbody>
            <?php foreach($dadosDetalhamentoEtapas as $key => $value) :?>
               <tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>
                    <td> <?php echo $value['ipidatividade'];?></td>
                    <td> <?php echo $value['ipiddescricao'];?></td>
                    <td> <?php echo $value['ipidunidade'];?></td>
                    <td> <?php echo $value['ipidquantidadehoras'];?></td>
                    <td> <?php echo $value['ipidtotalhoras'];?></td>
                </tr>
            <?php endforeach; ?>
            </tbody>
        </table>
    </div>
</div>
<br/>