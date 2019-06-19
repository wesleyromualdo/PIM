<?php
include_once('configuracao.php');
include_once('menu.php');
include_once('cabecalho.inc');

function listaEntidades( $dados ) {

    $ent = "inu.itrid <> 2 AND ent.tenid = 3";
    $dir = "inu.itrid <> 2 AND dir.tenid = 9";

    if($dados['esfera'] == '2') {
        $ent = 'inu.itrid = 2 AND ent.tenid = 1';
        $dir = 'inu.itrid = 2 AND dir.tenid = 2';
    }
    if(!empty($dados['estuf'])) {
        $where = "where inu.estuf = '{$dados['estuf']}'";
    }
    if(!empty($dados['muncod'])) {
        $where = "where inu.muncod = '{$dados['muncod']}'";
    }

    $sql = "SELECT DISTINCT
            inu.inuid, inu.inudescricao, inu.estuf, ent.entcnpj, '????' as exercicio, dir.entnome, dir.entcpf
            FROM par3.instrumentounidade inu
            INNER JOIN par3.instrumentounidade_entidade ent ON ent.inuid = inu.inuid 
              AND ( {$ent} ) AND ent.entstatus = 'A' AND ent.entcnpj IS NOT NULL
            INNER JOIN par3.instrumentounidade_entidade dir ON dir.inuid = inu.inuid 
              AND ( {$dir} ) AND dir.entstatus = 'A' AND dir.entnome IS NOT NULL
              AND dir.entcpf IS NOT NULL
              {$where}
            order by inu.estuf ASC, inu.inudescricao ASC";

    $cabecalho = array('Município', 'UF', 'CNPJ', 'Exercicio', 'Gestor', 'CPF');

    $list = new Simec_Listagem();
    $list->setCabecalho($cabecalho);
    $list->setFormFiltros('formulario');
    $list->esconderColunas(['id']);
    $list->addAcao('plus', 'listaContratos');
    $list->setQuery($sql);
    $list->addCallbackDeCampo('ecocnpjempresa','formatar_cnpj'); // função php para formatar cpf
    $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}

function listaContratos($inuid) {
    $sql = "SELECT conid as id, connumero, conano
                FROM par3.contratos
                WHERE inuid = {$inuid}";

    $cabecalho = array('Nº Contrato', 'Ano Contrato');

    $list = new Simec_Listagem();
    $list->setCabecalho($cabecalho);
    $list->setFormFiltros('formulario');
    $list->esconderColunas(['id']);
    $list->addAcao('plus', 'detalhesContrato');
    $list->setQuery($sql);
    $list->addCallbackDeCampo('ecocnpjempresa','formatar_cnpj'); // função php para formatar cpf
    $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}

function detalhesContrato($idcontrato) {
    $sql = "SELECT economeempresa,ecocnpjempresa
                FROM par3.empresas_contrato as eco 
                INNER JOIN par3.contratos as con ON con.conid = eco.conid
                WHERE ecostatus = 'A'
                AND   con.conid = {$idcontrato}";

    $listagem = new Simec_Listagem();
    $listagem->setCabecalho(['Empresa', 'CNPJ']);
    $listagem->setQuery($sql);
    $listagem->setTitulo('Dados das Empresas Contratadas:');
    $listagem->turnOffForm();
    $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    $sql = "SELECT DISTINCT
                rco.rconome,
                rco.rcocpf,
                rco.rcocargo
                FROM par3.contratos con
                INNER JOIN par3.responsavel_contrato rco ON rco.conid = con.conid
                WHERE
                con.constatus = 'A'
                AND rco.rcostatus = 'A'
                AND con.conid = '{$idcontrato}'";

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho(['Nome', 'CPF', 'Cargo/Função']);
        $listagem->setQuery($sql);
        $listagem->setTitulo('Dados do Responsável pelo Contrato:');
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

        $sql = "SELECT DISTINCT
                vco.vcoplaca,
                vco.vcochassi,
                vco.vcorenavam,
                fab.fabdsc,
                mve.mvedsc,
                vco.vcoano,
                vco.vcoqtdassentos
                FROM par3.contratos con
                INNER JOIN par3.veiculo_contrato         vco ON vco.conid = con.conid
                INNER JOIN par3.modelo_veiculo                          mve ON mve.mveid = vco.mveid
                INNER JOIN par3.fabricante_veiculo      fab ON fab.fabid = mve.fabid
                WHERE
                con.constatus = 'A'
                AND vco.vcostatus = 'A'
                AND mve.mvestatus = 'A'
                AND fab.fabstatus = 'A'
                AND vco.vcopossuirenavam = TRUE
                AND con.conid = '{$idcontrato}'";
        $cabecalho = array('Placa', 'Chassis', 'Renavam', 'Marca', 'Modelo', 'Ano', 'Quantidade de Assentos');
        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setTitulo('Dados do Veículo:');
        $listagem->esconderColunas(['id']);
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

        $sql = "SELECT DISTINCT
                vco.vcochassi,
                vco.vcoano,
                vco.vcoqtdassentos
                FROM par3.contratos con
                INNER JOIN par3.veiculo_contrato         vco ON vco.conid = con.conid
                INNER JOIN par3.modelo_veiculo                          mve ON mve.mveid = vco.mveid
                INNER JOIN par3.fabricante_veiculo      fab ON fab.fabid = mve.fabid
                WHERE
                con.constatus = 'A'
                AND vco.vcostatus = 'A'
                AND vco.vcopossuirenavam = FALSE
                AND con.conid = '{$idcontrato}'";
        $listagem = new Simec_Listagem();
        $listagem->setCabecalho(['Inscrição da Embarcação', 'Ano', 'Quantidade de Assentos']);
        $listagem->setQuery($sql);
        $listagem->setTitulo('Dados da Embarcação:');
        $listagem->esconderColunas(['id']);
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

}

switch ($_REQUEST['requisicao']) {
	case 'exportar':
		$lista = $adesaoPrograma->listaTermoAdesaoPnfcd($_REQUEST, 'data');
		header("Content-Type: application/vnd.ms-excel; charset=utf-8");
		header("Content-Disposition: attachment; filename=PNFCD.xls");
		header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
		header("Cache-Control: private",false);
		require('relatorioProgramaPnfcdXls.inc');
		die;
		break;
	case 'carregaMunicipios':
	    ob_clean();
		$municipio = new Territorios_Model_Municipio();
		print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'")));
		die;
		break;
	case 'listaContratos':
        ob_clean();
        listaContratos($_POST['dados'][0]); die;
		break;
    case 'detalhesContrato':
        ob_clean();
        detalhesContrato($_POST['dados'][0]);die;
        break;
	case 'downloadArquivo':
		if (!$_POST) {
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$prp = new Par3_Model_ProadesaoRespostasPnfcd();
			$ppp = $prp->pegarRespostasPorInuid($_GET['inuid']);
			$file = new FilesSimec("proadesao_respostas_pnfcd", array(), "par3");
			$file->getDownloadArquivo($ppp['pppresp1arqid']);
		}
		break;
}

?>

<div class="border-bottom white-bg dashboard-header" style="overflow: auto; margin-top: 40px;">
    <h2>Relatório:</h2>
    <div class="ibox-content">
        <?php listaEntidades($_REQUEST); ?>
    </div>
</div>

<script>
jQuery(document).ready(function() {
    var muncod_hid = jQuery('[name="muncod_hid"]').val();
    var $estuf = jQuery('[name="estuf"]');
    var $esfera = jQuery('[name="esfera"]');

    $('.table').removeClass('table-hover');
    $('.list-municipios').fadeIn();

    jQuery('#muncod').append(new Option('', ''));

    if ($estuf.val() != '') {
        carregarMunicipio( $estuf.val(), muncod_hid );
    }

    $estuf.on('change', function(){
        carregarMunicipio( jQuery(this).val(), muncod_hid );
    });

    $esfera.on('change', function() {
       if($(this).val() == 2) {
           $('.municipios').show();
       }else {
           $('.municipios').hide();
           $('#muncod').prop('selectedIndex',0);
       }
    });
    jQuery('[name="pesquisar"]').on('click', function() {
        if ( grecaptcha.getResponse() == '' ) {
            alert('Favor marque o captcha.');
            return false;
        }
    });

});

function carregarMunicipio( estuf, muncod_hid ){
	if ( estuf != '' ) {
   		var options = jQuery('#muncod');
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregaMunicipios&estuf="+estuf,
	   		async: false,
	   		success: function(result) {
                options.empty();
                options.append(new Option('', ''));
                var result = JSON.parse(result);
                $.each(result, function() {
                    options.append(new Option(this.mundescricao, this.muncod));
                });
                options.focus();
                if (muncod_hid) {
                	options.val(muncod_hid);
                }
                options.trigger('chosen:updated');
	   		}
	 	});
	}
}

</script>

<?php
$mensagemCarregar = $_SESSION['mensagem_carregar'];
unset($_SESSION['mensagem_carregar']);
?>
    <!-- Custom and plugin javascript -->
    <script src="/zimec/public/temas/simec/js/plugins/pace/pace.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/slimscroll/jquery.slimscroll.min.js"></script>

    <!-- Chosen -->
    <script src="/zimec/public/temas/simec/js/plugins/chosen/chosen.jquery.js"></script>

    <!-- JSKnob -->
    <script src="/zimec/public/temas/simec/js/plugins/jsKnob/jquery.knob.js"></script>

    <!-- Switcher -->
    <script src="/zimec/public/temas/simec/js/plugins/bootstrap-switch/bootstrap-switch.js"></script>

    <!-- NanoScroller -->
    <script src="/zimec/public/temas/simec/js/plugins/nanoscroll/jquery.nanoscroller.min.js"></script>

    <!-- JsTree -->
    <script src="/zimec/public/temas/simec/js/plugins/jstree/jstree.min.js"></script>

    <!-- iCheck -->
    <script src="/zimec/public/temas/simec/js/plugins/iCheck/icheck.min.js"></script>

    <!-- File Input -->
    <script src="/zimec/public/temas/simec/js/plugins/fileinput/fileinput.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/fileinput/fileinput_locale_pt-BR.js"></script>

    <!-- Summernote -->
    <script src="/zimec/public/temas/simec/js/plugins/summernote/summernote.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/summernote/summernote-pt-BR.js"></script>

    <!-- Flot -->
    <script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.tooltip.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.spline.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.resize.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.pie.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/flot/jquery.flot.symbol.js"></script>

    <!-- Dropzone -->
    <script src="/zimec/public/temas/simec/js/plugins/dropzone/dropzone.js"></script>

    <!-- Masked Input -->
    <script src="/zimec/public/temas/simec/js/plugins/maskedinput/jquery.maskedinput.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/inputmask/inputmask.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/inputmask/jquery.inputmask.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/inputmask/inputmask.numeric.extensions.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/inputmask/inputmask.regex.extensions.min.js"></script>

    <!-- Menu -->
    <script src="/zimec/public/temas/simec/js/plugins/metisMenu/jquery.metisMenu.js"></script>

    <!-- Datatables -->
    <script src="/zimec/public/temas/simec/js/plugins/dataTables/jquery.dataTables.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/dataTables/dataTables.bootstrap.js"></script>

    <!-- Gritter -->
    <script src="/zimec/public/temas/simec/js/plugins/gritter/jquery.gritter.min.js"></script>

    <!-- Bootbox -->
    <script src="/zimec/public/temas/simec/js/plugins/bootbox/bootbox.min.js"></script>

    <!-- Ion Range Slider -->
    <script src="/zimec/public/temas/simec/js/plugins/ionRangeSlider/ion.rangeSlider.min.js"></script>

    <!-- Validate -->
    <script src="/zimec/public/temas/simec/js/plugins/validate/jquery.validate.min.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/validate/jquery.validate.extend.js"></script>
    <script src="/zimec/public/temas/simec/js/plugins/validate/jquery.form.min.js"></script>

    <!-- Sweet Aler -->
    <script src="/zimec/public/temas/simec/js/plugins/sweetalert/sweetalert.min.js"></script>

    <!-- Add fancyBox main JS and CSS files -->
    <script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>
    <link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen" />

    <!-- Add Button helper (this is optional) -->
    <link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-buttons.css" />
    <script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-buttons.js"></script>

    <!-- Add Thumbnail helper (this is optional) -->
    <link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-thumbs.css" />
    <script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-thumbs.js"></script>

    <!-- Add Media helper (this is optional) -->
    <script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-media.js"></script>

    <!-- SIMEC -->
    <script src="/zimec/public/temas/simec/js/theme.js"></script>
    <script src="/zimec/public/temas/simec/js/simec.js"></script>
    <script src="/zimec/public/temas/simec/js/simec-eventos.js"></script>

    <script>
        $(document).ready(function() {

            if('<?php echo !empty($mensagemCarregar) ?>'){
                swal({title: "", text: "<?php echo $mensagemCarregar['text']; ?>", type: "<?php echo $mensagemCarregar['type']; ?>"});
            }
        });
    </script>

<?
unset($_SESSION['sislayoutbootstrap']);
?>