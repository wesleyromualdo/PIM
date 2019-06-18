<script type="text/javascript">
    $(function () {
        $("#periodo")
            .find(".btn-limpar").on("click", function (e) {
            e.preventDefault();
            e.stopPropagation();

            if (!$("#periodo_prsid").val()) {
                $("#periodo_prsid").val("");
            }

            $("#periodo_prsdescricao").val("");
            $("#periodo_prsdatainicio").val("");
            $("#periodo_prsdatafim").val("");

            throw Error("parando o script padrão do render");
        });

        var date = new Date(), y = date.getFullYear(), m = date.getMonth();
        $.datepicker.setDefaults({
            minDate: new Date(y-1, m, 1),
            maxDate: new Date(y+1, m, 0)
        });
    })
</script>
<?php

$webForm = new Simec_View_Form('periodo');
if (array_key_exists('periodo', $_POST))
{
    $webForm->carregarDados($_POST['periodo']);
}
$webForm
    ->addHidden('prsid', '')
    ->addHidden('requisicao', 'salvarPeriodo')
    ->addTexto('Descrição', 'prsdescricao', 100)
    ->addCombo('Mês', 'prsmes', [
        ['codigo' => 1, 'descricao' => 'Janeiro'],
        ['codigo' => 2, 'descricao' => 'Fevereiro'],
        ['codigo' => 3, 'descricao' => 'Março'],
        ['codigo' => 4, 'descricao' => 'Abril'],
        ['codigo' => 5, 'descricao' => 'Maio'],
        ['codigo' => 6, 'descricao' => 'Junho'],
        ['codigo' => 7, 'descricao' => 'Julho'],
        ['codigo' => 8, 'descricao' => 'Agosto'],
        ['codigo' => 9, 'descricao' => 'Setembro'],
        ['codigo' => 10, 'descricao' => 'Outubro'],
        ['codigo' => 11, 'descricao' => 'Novembro'],
        ['codigo' => 12, 'descricao' => 'Dezembro']
    ])
    ->addData('Data Início', 'prsdatainicio')
    ->addData('Data Fim', 'prsdatafim')
    ->setRequisicao('salvarPeriodo')
    ->addBotoes(['limpar', 'salvar'])
    ->render();
