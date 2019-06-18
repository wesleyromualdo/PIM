<style>
.detalhe{
    color:black;
}
</style>
<?php if (!$dados || !is_array($dados) || count($dados) == 0) : ?>
    <div class="ibox-content detalhe">
        <table class="table table-hover dataTable">
            <tr>
                <td colspan="4">
                    <div style="margin-top:20px;"
                         class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro"
                         id="tb_render">Nenhum registro encontrado
                    </div>
                </td>
            </tr>
        </table>
    </div>
<?php else : ?>
    <div class="ibox-content detalhe">
        <table class="table table-hover ">
            <thead>
                <tr>
                    <th>N° do processo</th>
                    <th>N° do documento</th>
                    <th>Tipo do Documento</th>
                    <th>Data de Vigência</th>
                </tr>
            </thead>
            <?php foreach ($dados as $dado) : ?>
            <tr>
                <td><?php echo $dado['prpnumeroprocesso']; ?></td>
                <td><?php echo $dado['dopnumerodocumento']; ?></td>
                <td><?php echo $dado['mdonome']; ?></td>
                <td><?php echo $dado['dopdatafimvigencia']; ?></td>
            </tr>
            <?php endforeach;?>
        </table>
    </div>
<?php endif; ?>