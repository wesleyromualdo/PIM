<?php
header ("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
header ("Cache-Control: no-cache, must-revalidate");
header ("Pragma: no-cache");
header ("Content-type: application/x-msexcel");
header ("Content-Disposition: attachment; filename=\"Nutricionista - Entidades  - " . date('Y-m-d') . ".xls");
?>
<table>
    <thead>
    <tr>
        <th><b>Esfera</b></th>
        <th><b>Código Entidade</b></th>
        <th><b>Código Município</b></th>
        <th><b>UF</b></th>
        <th><b>Nome da Entidade</b></th>
        <th><b>Situação</b></th>
        <th><b>Validado pelo Nutricionista - Em Análise no FNDE</b></th>
        <th><b>Não validado pelo Nutricionista</b></th>
        <th><b>Pendente de validação</b></th>
        <th><b>Cadastro aprovado</b></th>
        <th><b>Cadastro reprovado</b></th>
        <th><b>Vínculo Validado</b></th>
    </tr>
    </thead>
    <tbody>
    <?php foreach ($arrEntidade as $dados):?>
    <tr>
        <td><?php echo $dados['esfera']; ?></td>
        <td><?php echo $dados['inuid']; ?></td>
        <td><?php echo $dados['muncod']; ?></td>
        <td><?php echo $dados['estuf']; ?></td>
        <td><?php echo $dados['inudescricao']; ?></td>
        <td><?php echo $dados['descricao']; ?></td>
        <td><?php echo $dados['count_validados']; ?></td>
        <td><?php echo $dados['count_nao_validado']; ?></td>
        <td><?php echo $dados['count_pendentes']; ?></td>
        <td><?php echo $dados['count_aprovados']; ?></td>
        <td><?php echo $dados['count_reprovados']; ?></td>
        <td><?php echo $dados['count_vinculos_validados']; ?></td>
    </tr>
    <?php endforeach; ?>
    </tbody>
</table>