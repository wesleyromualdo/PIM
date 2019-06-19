$.fn.mcFormAjax = function(){
    $.each($(this),function(){
        config = {
            'url':$(this).attr('action'),
            'data':$(this).serialize(),
            'type':$(this).attr('method'),
            'dataType': (dataType = $(this).attr('data-datatype')) ? dataType : 'json',
            'validate': (dataValidate = $(this).attr('data-validate')) ? eval(dataValidate) : function(){return true;},
            'success': (dataSuccess = $(this).attr('data-datatype')) ? eval(dataSuccess) : function(){}
        }
        $(this).on('submit',function(event){
            if(!$(this).validarFormulario()){
                return false;
            }
            if(!config.validate($.deparam($(this).serialize()))){
                return false;
            }
            $.ajax(config);
            event.preventDefault();
            event.stopPropagation();
            return false;
        });
    });
};
$.fn.limparObjeto = function(obj, clone){
    if(clone){
        newObj = {};
    }else{
        newObj = obj;
    }
    $.each(newObj,function(i,v){
        newObj[i] = null;
    });
    return newObj;
};
$.fn.preencherOpcoes = function () {
    $.each($(this), function () {
        try {
            var options = {
                'template': $($(this).data('tpl-item')),
                'collection': $(this).data('collection') ? eval($(this).data('collection')) : [],
                'callbackItem': $(this).data('callback') ? eval($(this).data('callback')) : function () {},
                'cleanBeforeInit': $(this).data('clean') ? true : false
            };
            if(options.template.length == 0){
                if($(this).prop('nodeName') == 'TABLE'){
                    var $trH = $('<tr>').attr('data-autogerado',1);
                    var $trB = $('<tr>').attr('data-autogerado',1);
                    $.each(options.collection, function(a,b){
                        $.each(b, function(i,v){
                            $trH.append($('<th>').html(i));
                            $trB.append($('<td>').attr('data-html-' + i, 1));
                        });
                        return false;
                    });
                    if(!$(this).find('thead').length){
                        $(this).append($('<thead>').append($trH));
                    }
                    options.template = $trB;
                }else{
                    throw 'O template não foi encontrado!';
                }
            }
            if(options.template.length > 1){
                throw 'Existem ' + options.template.length + ' referências para o template!';
            }
            $(this).view('iterate', options);
            if ($(this).data('options-name')) {
                var name = $(this).data('options-name');
                $.each($(this).children(), function () {
                    if ($(this).is('[data-generate-name]')) {
                        $(this).attr('name', $this.data('options-name'));
                    }
                    $.each($(this).find('[data-generate-name]'), function () {
                        $(this).attr('name', name);
                    });
                });
            }
        } catch (e) {
            console.log($(this), options);
            throw e;
        }
    });
    return $(this);
};
$.fn.formGroupRemoveError = function () {
    $.each($(this), function () {
        $(this).parents('.form-group:first').removeClass('has-error');
    });
    return $(this);
};
$.fn.formGroupAddError = function (nmClass) {
    $.each($(this), function () {
        $(this).parents('.form-group:first').addClass('has-error');
    });
    return $(this);
};
$.fn.validarFormulario = function () {
    $(this).find(':input.js-obrigatorio').formGroupRemoveError();
    var $campos = $(this).find(':input.js-obrigatorio').filter(function () {
        return !$(this).val();
    });
    if ($campos.length) {
        $campos.formGroupAddError();
        return false;
    }
    return true;
};

$.fn.cmpDate = function () {
    $.each($(this), function () {
        $(this).mask('99/99/9999');
        $(this).on('change',function(){
            var $min = $($(this).attr('data-min-selector'));
            var min = $(this).attr('data-min-value');
            var $max = $($(this).attr('data-max-selector'));
            var max = $(this).attr('data-max-value');
            if ($min.length) {
                if($(this).val() && $min.val() && ($(this).val().toDate() < $min.val().toDate())){
                    $(this).val('');
                    alert('Data menor que a permitida ' + $min.val());
                }
            }
            if ($max.length) {
                if($(this).val() && $max.val() && ($(this).val().toDate() > $max.val().toDate())){
                    $(this).val('');
                    alert('Data maior que a permitida' + $max.val());
                }
            }
            if (min) {
                if($(this).val() && ($(this).val().toDate() < min.toDate())){
                    $(this).val('');
                    alert('Data menor que a permitida ' + min + '.');
                }
            }
            if (max) {
                if($(this).val() && ($(this).val().toDate() < max.toDate())){
                    $(this).val('');
                    alert('Data maior que a permitida ' + max + '.');
                }
            }
        });
    });
};
/**
 * Plugin inverso ao param do jquery
 * @param {string} h
 * @returns {Object}
 */
(function (h) {
    h.deparam = function (i, j) {
        var d = {}, k = {"true": !0, "false": !1, "null": null};
        h.each(i.replace(/\+/g, " ").split("&"), function (i, l) {
            var m;
            var a = l.split("="), c = decodeURIComponent(a[0]), g = d, f = 0, b = c.split("]["), e = b.length - 1;
            /\[/.test(b[0]) && /\]$/.test(b[e]) ? (b[e] = b[e].replace(/\]$/, ""), b = b.shift().split("[").concat(b), e = b.length - 1) : e = 0;
            if (2 === a.length)
                if (a = decodeURIComponent(a[1]), j && (a = a && !isNaN(a) ? +a : "undefined" === a ? void 0 : void 0 !== k[a] ? k[a] : a), e)
                    for (; f <= e; f++)
                        c = "" === b[f] ? g.length : b[f], m = g[c] = f < e ? g[c] || (b[f + 1] && isNaN(b[f + 1]) ? {} : []) : a, g = m;
                else
                    h.isArray(d[c]) ? d[c].push(a) : d[c] = void 0 !== d[c] ? [d[c], a] : a;
            else
                c && (d[c] = j ? void 0 : "")
        });
        return d
    }
})(jQuery);
/**
 * Valida uma string de data
 * @returns {Boolean}
 */
String.prototype.validarData = function() {
    return this.toDate('d/m/Y').data() == this;
};
/**
 * 
 * @param {String} formato
 * @returns {Date}
 */
String.prototype.toDate = function(formato) {
    var formato = formato || 'd/m/Y';
    var d = m = Y = h = i = s = 0;
    switch (formato) {
        case 'Y/m/d':
            var match = this.match(/^(\d{4})\/(\d{2})\/(\d{2})(|\s(0[0-9]|1[0-9]||2[0-3])\:([0-5][0-9])(|\:([0-5][0-9])))$/);
            if (!match) {
                return new Date(0, 0, 0, 0, 0, 0);
            }
            Y = match[1] ? match[1] : 0;
            m = match[2] ? match[2] : 0;
            d = match[3] ? match[3] : 0;
            break;
        case 'd/m/Y':
            var match = this.match(/^(\d{2})\/(\d{2})\/(\d{4})(|\s(0[0-9]|1[0-9]||2[0-3])\:([0-5][0-9])(|\:([0-5][0-9])))$/);
            if (!match) {
                return new Date(0, 0, 0, 0, 0, 0);
            }
            Y = match[3] ? match[3] : 0;
            m = match[2] ? match[2] : 0;
            d = match[1] ? match[1] : 0;
            break;
        case 'm/d/Y':
            var match = this.match(/^(\d{2})\/(\d{2})\/(\d{4})(|\s(0[0-9]|1[0-9]||2[0-3])\:([0-5][0-9])(|\:([0-5][0-9])))$/);
            if (!match) {
                return new Date(0, 0, 0, 0, 0, 0);
            }
            Y = match[3] ? match[3] : 0;
            m = match[1] ? match[1] : 0;
            d = match[2] ? match[2] : 0;
            break;
    }
    var h=0;
    var i=0;
    var s=0;
    if(match[5]){
        h = match[5] ? match[5] : 0;
        i = match[6] ? match[6] : 0;
        s = match[8] ? match[8] : 0;
    }

    return new Date(parseInt(new Number(Y)), parseInt(new Number(m)) - 1, parseInt(new Number(d)), parseInt(new Number(h)), parseInt(new Number(i)), parseInt(new Number(s)));
};
Date.prototype.incDia = function(inc){
    inc = inc || 1;
    this.setDate(this.getDate() + inc);
    return this;
};
Date.prototype.incMes = function(inc){
    inc = inc || 1;
    this.setMonth(this.getMonth() + inc);
    return this;
};
Date.prototype.incAno = function(inc){
    inc = inc || 1;
    this.setFullYear(this.getFullYear() + inc);
    return this;
};
Date.prototype.data = function(formato){
    formato = formato || 'd/m/Y';
    switch (formato) {
        case 'Y/m/d':
            return this.ano() + '/' + this.mes() + '/' + this.dia();
        case 'd/m/Y':
            return this.dia() + '/' + this.mes() + '/' + this.ano();
        case 'm/d/Y':
            return this.mes() + '/' + this.dia() + '/' + this.ano();
    }
};
Date.prototype.dia = function(){
    var dia = this.getDate();
    if(dia < 10){
        return '0' + dia;
    }
    return '' + dia;
};
Date.prototype.ano = function(){
    return this.getFullYear();
};
Date.prototype.mes = function(){
    var mes = this.getMonth() + 1;
    if(mes < 10){
        return '0' + mes;
    }
    return '' + mes;
};
Date.prototype.anomes = function(){
    return this.getFullYear() + '' + this.mes();
};
var __Simec = function () {
    configs = {};
    inicial = function () {
        delete Array.prototype.sortObject;
        delete Array.prototype.objectToString;
        registrarComponentes();
    };
    var registrarComponentes = function () {
        $('form[data-ajax="true"]').mcFormAjax();
        $('[data-collection]').preencherOpcoes();
        $('.js-cmp-cpf').mask('999.999.999-99');
        $('.js-cmp-cnpj').mask('999.999.999/9999-99');
        $('.js-cmp-cep').mask('99.999-999');
        $('.js-cmp-data').cmpDate();
        $('.js-cmp-select').chosen();
        $('.js-form').on('submit', function (event) {
            if (!$(this).validarFormulario()) {
                event.preventDefault();
                event.stopPropagation();
                return false;
            }
        });
        $.each($(":input.js-obrigatorio"),function(){
            $(this).parents(".form-group:first").addClass("required");
        });
        $('.js-select-chosen').chosen();
    };
    inicial();
};
$(document).ready(function () {
    new __Simec();
});
