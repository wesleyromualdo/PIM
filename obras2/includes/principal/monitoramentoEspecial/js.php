<script type="text/javascript">
    function abreAtividade(mesid, obrid) {
        var url = "/obras2/obras2.php?modulo=principal/cadastroAtividadeMonitoramentoEspecial&acao=A" +
            "&mesid=" + mesid + "&obrid=" + obrid;
        popup1 = window.open(
            url,
            "Monitoramento Especial",
            "width=1200,height=700,scrollbars=yes,scrolling=no,resizebled=no"
        );
        return false;
    }
    function novaAtividade(obrid) {
        var url = "/obras2/obras2.php?modulo=principal/cadastroAtividadeMonitoramentoEspecial&acao=A" + "&obrid=" + obrid;
        popup1 = window.open(
            url,
            "Monitoramento Especial",
            "width=1200,height=700,scrollbars=yes,scrolling=no,resizebled=no"
        );
        return false;
    }
</script>