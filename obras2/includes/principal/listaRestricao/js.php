<script type="text/javascript">
    
	function cadastroNovoDocid(rstid){
        var r1 = window.confirm('Você deseja cadastrar um Documento para a restrição '+rstid+' ?');
        if(r1){

            var url = '?modulo=principal/listaRestricao&acao=L&rstid='+rstid+'&novoDoc=true';
            janela = window.open(url, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); 
        }else{
            return false;
        }
    }
     
        
    /*function getEstados(){
        var estados = '';

        var elemento = document.getElementsByName('slEstado[]')[0];

        for (var i = 0; i < elemento.options.length; i++) {
            if (elemento.options[i].value != '')
            {
                estados += "'" + elemento.options[i].value + "',";
            }
        }

        return estados;
    }*/

    function ajaxEstado(){
        jQuery.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=filtrarMunicipio&estados=" + getEstados(),
            success: function(retorno) {
                jQuery('#idMunicipio').html(retorno);
            }});
    }
 
    function onOffCampo(campo){
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }

 
    function onOffBloco(bloco){
        var div_on = document.getElementById(bloco + '_div_filtros_on');
        var div_off = document.getElementById(bloco + '_div_filtros_off');
        var img = document.getElementById(bloco + '_img');
        var input = document.getElementById(bloco + '_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }
    
    function getLista(tipo){
        var formulario = document.formulario;
        var tipo_relatorio = tipo;
        prepara_formulario();
        document.getElementById('tipo_relatorio').value = tipo_relatorio;
        formulario.submit();
        document.getElementById('tipo_relatorio').value = '';
    }
    
    function alterarRestricao(rstid, obrid, empid){
        var url = '?modulo=principal/cadRestricao&acao=O&rstid='+rstid+'&obrid='+obrid+'&empid='+empid;
        janela = window.open(url, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); 
        janela.focus();
    }
    
    function cadastroNovoDocid(rstid){
        var r1 = window.confirm('Você deseja cadastrar um Documento para a restrição '+rstid+' ?');
        if(r1){

            var url = '?modulo=principal/listaRestricao&acao=L&rstid='+rstid+'&novoDoc=true';
            janela = window.open(url, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); 
        }else{
            return false;
        }
    }
    
    jQuery(function($){
        $.datepicker.regional['pt-BR'] = {
                closeText: 'Fechar',
                prevText: '&#x3c;Anterior',
                nextText: 'Pr&oacute;ximo&#x3e;',
                currentText: 'Hoje',
                monthNames: ['Janeiro','Fevereiro','Mar&ccedil;o','Abril','Maio','Junho','Julho','Agosto','Setembro','Outubro','Novembro','Dezembro'],
                monthNamesShort: ['Jan','Fev','Mar','Abr','Mai','Jun','Jul','Ago','Set','Out','Nov','Dez'],
                dayNames: ['Domingo','Segunda-feira','Ter&ccedil;a-feira','Quarta-feira','Quinta-feira','Sexta-feira','S&aacute;bado'],
                dayNamesShort: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                dayNamesMin: ['Dom','Seg','Ter','Qua','Qui','Sex','Sab'],
                weekHeader: 'Sm',
                dateFormat: 'dd/mm/yy',
                firstDay: 0,
                isRTL: false,
                showMonthAfterYear: false,
                yearSuffix: ''};
        $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
    });
         
            
setTimeout(function(){
    try{
    $1_11('#rstdtinclusao_de').mask('99/99/9999');
    $1_11('#rstdtsuperacao_de').mask('99/99/9999');
    $1_11('#rstdtinclusao_ate').mask('99/99/9999');
    $1_11('#rstdtsuperacao_ate').mask('99/99/9999');
    $1_11('#ultimotramite_de').mask('99/99/9999');
    $1_11('#ultimotramite_ate').mask('99/99/9999');
    }catch (e){
        console.log('mask erro');
    }
    var d = new Date();
    var data_hoje = d.toUTCString();
    
    $1_11("#rstdtinclusao_de").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        showAnim:'drop'
    });
    $1_11("#rstdtinclusao_ate").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        showAnim:'drop'
    });
    $1_11("#rstdtsuperacao_de").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });
    $1_11("#rstdtsuperacao_ate").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });

    $1_11("#ultimotramite_de").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });$1_11("#ultimotramite_ate").datepicker({
        numberOfMonths: 1,
        dateFormat: 'dd/mm/yy',
        showWeek: true,
        'showAnim':'drop'
    });
               
}, 500);

 
    $1_11(document).ready(function () {
        $1_11('select[name="strid[]"]').chosen();
        $1_11('select[name="prfid[]"]').chosen();
        $1_11('select[name="tooid[]"]').chosen();
        $1_11('select[name="tprid[]"]').chosen();
        $1_11('select[name="muncod[]"]').chosen();
        $1_11('select[name="tpoid[]"]').chosen();
        $1_11('select[name="esdid_ri[]"]').chosen();
    });


</script>
