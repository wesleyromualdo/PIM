<?php $relatorioBandaLarga = new Par3_Model_Relatorio_BandaLarga(); ?>
<table bgcolor="#f5f5f5" align="center" class="tabela" border="1">
    <tr>
        <td rowspan="2"><b>UF</b></td>
        <td rowspan="2"><b>Municipio</b></td>
        <td rowspan="2"><b>NP <br><span style="font-size: 9px; color: #d9534f">(Não Preenchido)</span></b></td>
        <td rowspan="2"><b>PP <br><span style="font-size: 9px; color: #f0ad4e">(Preenchimento Parcial)</span></b></td>
        <td rowspan="2"><b>PT <br><span style="font-size: 9px; color: #5cb85c">(Preenchimento Total)
        <?php foreach ($grupos as $grupo) : ?>
            <td colspan="<?php echo count($tabela[$grupo['grpid']]['perguntas']) ?>">
                <b><?php echo $grupo['grptitulo']; ?></b>
            </td>
        <?php endforeach ?>
    </tr>

    <tr>
        <?php foreach ($tabela as $grupo) : ?>
            <?php foreach ($grupo['perguntas'] as $pergunta) : ?>
                <td><b><?php echo $pergunta['pertitulo']; ?></b></td>
            <?php endforeach ?>
        <?php endforeach ?>
    </tr>

    <?php if (is_array($municipios) && count($municipios) > 0) : ?>
        <?php foreach ($municipios as $municipio) : ?>
            <?php if ($municipio && $municipio['mundescricao']): ?>
            <?php $respostas = $relatorioBandaLarga->carregarRespostasMunicipio($municipio['inuid'], $perguntas); ?>
            <?php $respostasPrincipais = $relatorioBandaLarga->carregarRespostasMunicipio($municipio['inuid'], $principais); ?>
            <?php $qtdRespostas = $relatorioBandaLarga->carregarQtdRespostasMunicipio($municipio['inuid']); ?>
            <tr bgcolor="#ffffff">
                <td><?php echo $municipio['estuf']; ?></td>
                <td><?php echo $municipio['mundescricao']; ?></td>
                <td align="center"><?php echo count($respostas) == 0 ? 'X' : ''; ?></td>
                <td align="center"><?php echo count($respostas) > 0 ? 'X' : ''; ?></td>
                <td align="center"><?php echo $qtdRespostas == 20 ? 'X' : ''; ?></td>
                <?php foreach ($tabela as $grupo) : ?>
                    <?php foreach ($grupo['perguntas'] as $pergunta) : ?>
                        <td>
                            <?php if (array_key_exists($pergunta['perid'], $respostas)) : ?>
                            <?php echo $respostas[$pergunta['perid']]['itptitulo']; ?>
                            <?php endif ?>
                        </td>
                    <?php endforeach ?>
                <?php endforeach ?>
            </tr>
        <?php endif; ?>
        <?php endforeach ?>
    <?php endif; ?>
</table>