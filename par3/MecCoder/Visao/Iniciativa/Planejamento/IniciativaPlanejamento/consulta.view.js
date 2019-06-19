jQuery(document).ready(function ()
{
    ClassePlanejamento.iniciar();

});

var ClassePlanejamento = {

    iniciar: function () {
        this.selectCiclo();
        this.selectSituacao();
        this.selectDimensao();
    },

    selectCiclo: function () {

        _JS.ciclo.unshift({codigo: '', descricao: ''});
        jQuery('#cicid').view('iterate', {
            'template'	: jQuery('.template-ciclo .js-select'),
            'collection'	: _JS.ciclo
        });

    },

    selectSituacao: function () {

        _JS.situacao.unshift({codigo: '', descricao: ''});
        jQuery('#situacao').view('iterate', {
            'template'	: jQuery('.template-situacao .js-select'),
            'collection'	: _JS.situacao
        });

    },

    selectDimensao: function () {

        _JS.dimensao.unshift({codigo: '', descricao: ''});
        jQuery('#dimid').view('iterate', {
            'template'	: jQuery('.template-dimensao .js-select'),
            'collection'	: _JS.dimensao
        });

    },

}


