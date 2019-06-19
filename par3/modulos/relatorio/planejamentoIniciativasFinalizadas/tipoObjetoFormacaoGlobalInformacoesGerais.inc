

<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 11/8/2017
 * Time: 2:02 PM
 */?>

<div class="row">
    <div class="col-md-12">
        <h3>Informações Gerais</h3>
    </div>
</div>
        <div class="row">
    <div class="col-md-12">
        <table style='border: 1px solid #000000; border-collapse: collapse;'>
            <thead>
            <th>Professores Curistas</th>
            <th>Orientadores</th>
            <th>Formadores</th>
            <th>Tutores</th>
            <th>Preparadores</th>
            <th>Supervisores</th>
            </thead>
            <tbody>
            <tr style='border: solid 1px #000000; padding:5px; white-space: nowrap;'>
                <td><?php echo $dadosInfomacoesGerais['qt_professores'];?></td>
                <td><?php echo $dadosInfomacoesGerais['qt_ipiforientadores'];?></td>
                <td><?php echo $dadosInfomacoesGerais['qt_ipifformadores'];?></td>
                <td><?php echo $dadosInfomacoesGerais['qt_ipiftutores'];?></td>
                <td><?php echo $dadosInfomacoesGerais['qt_ipifpreparadores'];?></td>
                <td><?php echo $dadosInfomacoesGerais['qt_ipifsupervisores'];?></td>
            </tr>
            </tbody>
        </table>
    </div>
</div>
<br/>