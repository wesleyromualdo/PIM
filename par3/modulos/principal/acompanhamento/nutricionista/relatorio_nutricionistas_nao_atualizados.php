<?php
header ("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
header ("Cache-Control: no-cache, must-revalidate");
header ("Pragma: no-cache");
header ("Content-type: application/x-msexcel");
header ("Content-Disposition: attachment; filename=\"Nutricionista - Nutricionistas-nao-atualizados  - " . date('Y-m-d') . ".xls");
?>
<table>
    <thead>
    <tr>
        <th><b>UF</b></th>
        <th><b>Entidade</b></th>
        <th><b>CPF</b></th>
        <th><b>Nome</b></th>
        <th><b>Vinculação</b></th>
        <th><b>Data de Desvinculação</b></th>
        <th align="left"><b>Motivo da Desvinculação</b></th>
    </tr>
    </thead>
    <tbody>
    <?php foreach ($arrNutricionistas as $dados):?>
        <tr>
            <td><?php echo $dados['estuf']; ?></td>
            <td><?php echo $dados['inudescricao']; ?></td>
            <td><?php echo formatar_cpf($dados['vncpf']); ?></td>
            <td><?php echo $dados['usunome']; ?></td>
            <td><?php echo $dados['vnstatus']; ?></td>
            <td><?php echo formata_data_hora($dados['vndatadesvinculacao']); ?></td>
            <td><?php echo $dados['vnmotivodesvinculacao']; ?></td>
        </tr>
    <?php endforeach; ?>
    </tbody>
</table>