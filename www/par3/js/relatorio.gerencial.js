jQuery(function() {
    renderizeMunicipio();
    renderizeSomenteObras();

    jQuery("input:radio[name=itrid], select[name=estuf]").change(function() {
        renderizeMunicipio();
    });

    jQuery('select[name=estuf]').change(function(){
        carregarMunicipio(this.value);
    });

    jQuery('select[name="intoid[]"]').change(function(){
        renderizeSomenteObras();
    });

    jQuery('.export').click(function() {
        window.open(window.location.href + "&requisicao=exportar&" + jQuery('#formulario').serialize());
    });
});

function renderizeMunicipio() {
    var filtroMunicipio = jQuery("select[name=muncod]").parents("div.form-group");
    if (jQuery('input:radio[name=itrid]:checked').val() === '1' || !jQuery('select[name=estuf]').val()) {
        filtroMunicipio.slideUp();
    } else {
        filtroMunicipio.slideDown();
    }
}

function renderizeSomenteObras() {
    var tipos = jQuery('select[name="intoid[]"]').val();
    var flag = tipos && tipos.length == 1 && tipos[0] === '1';
    if (flag) {
        jQuery(".soObra").parents("div.form-group").slideDown();
        jQuery(".soObraN").parents("div.form-group").slideUp();
    } else {
        jQuery(".soObra").parents("div.form-group").slideUp();
        jQuery(".soObraN").parents("div.form-group").slideDown();
    }
}

function carregarMunicipio(estuf, muncod) {
    if(estuf != '') {
        var options = jQuery('#muncod');
        options.empty();
        options.append(new Option('', ''));
        jQuery.post('', 'requisicao=carregaMunicipios&estuf='+estuf, function(retorno) {
            options.append(new Option('', ''));
            $.each(JSON.parse(retorno), function() {
                options.append(new Option(this.mundescricao, this.muncod));
            });
            options.focus();
            options.val(muncod);
            options.trigger('chosen:updated');
        });
    }
}