<table style="border: 1px solid black;border-collapse: collapse;text-align: justify;" border="3">
    <tr>
        <td align="center">CPF</td>
        <td align="center">Nome</td>
        <td align="center">Situação</td>
    </tr>
    <tbody>
    <?php foreach ($arrVn as $nutricionista): ?>
        <tr>
            <td align="center"><?= formatar_cpf($nutricionista['vncpf'])?></td>
            <td align="center"><?= $nutricionista['entnome']?></td>
            <td align="center"><?= $nutricionista['sndescricao']?></td>
        </tr>
    <?php endforeach;?>
    </tbody>
</table>

