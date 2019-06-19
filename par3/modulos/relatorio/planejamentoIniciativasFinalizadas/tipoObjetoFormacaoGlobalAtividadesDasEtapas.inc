
<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/8/2017
 * Time: 2:58 PM
 */?>
<div class="row">
    <div class="pcol-md-12">
        <h3>Atividade das Etapas</h3>
    </div>
</div>
<div class="row">
    <div class="pcol-md-12">
        <table style='border: 1px solid #000000; border-collapse: collapse;'>
            <thead>
            <th>Etapa</th>
            <th>Descrição Etapa</th>
            <th>Descrição da Atividade</th>
            <th>Quantidade de Dias</th>
            <th>Quantidade de Horas</th>
            <th>total</th>
            </thead>
            <tbody>
            <?php foreach($dadosDasAtividadesDasEtapas as $key => $value):?>
            <tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>
                <td ><?php echo $value['ipidatividade'];?></td>
                <td ><?php echo $value['ipiadescicao'];?></td>
                <td ><?php echo $value['ipiadescicaoatividade'];?></td>
                <td ><?php echo $value['ipiaquantdias'];?></td>
                <td ><?php echo $value['ipidquantidadehoras'];?></td>
                <td ><?php echo $value['ipidtotalhoras'];?></td>
            </tr>
            <?php endforeach;?>
            </tbody>
        </table>
    </div>
</div>
<br/>