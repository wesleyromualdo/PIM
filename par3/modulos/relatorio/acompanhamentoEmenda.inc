<?php
set_time_limit(0);
/**
 * Arquivos que vão popular os campos
* Totas as consulta devem trazer apenas dados de iniciativas cadastradas
*/
$obModelEmenda  = new Par3_Model_EmendasParlamentares();
$sqlPartido     = $obModelEmenda->sqlPartido();
$sqlAutor       = $obModelEmenda->sqlAutor();

$mInp               = new Par3_Controller_IniciativaPlanejamento();
$arrEstado          = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
$arrMunicipio       = Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
$arrSituAnalise     = Par3_Model_IniciativaPlanejamento::getSituacaoAnalise();
$arrIniciativaAno   = Par3_Model_IniciativaPlanejamento::getInicitivaAno();
$arrIniciativa      = $mInp->getIniciativa(['inuid' => $_GET['inuid'], 'iniid' => $inp->iniid, 'consulta' => 'true'], true);

if (!$_REQUEST['requisicao'] && $_SESSION['filtro']['requisicao'] != '') {
    $_REQUEST = $_SESSION['filtro'];
}

include_once APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
?>
<style>
    .navbar-listagem {
        margin-top: 0px;
        float: right;
        width: 500px;
    }
    .anoClass label{
        margin-left: -4px;
    }
    .estadoClass a.chosen-single.chosen-default{
        width: 170px ;
        margin-left: 21px;
    }
    .estadoClass a.chosen-single.chosen-single-with-deselect{
        width: 170px ;
        margin-left: 21px;
    }
   /*.estadoClass .chosen-container-single .chosen-search .chosen-drop {
        width: 200px ;
    }*/
    .estadoClass .chosen-drop {
        width: 180px ;
        margin-left: 21px;
    }
    .estadoClass label{
        margin-left: -21px;
    }
    .linhaInferior .divdtPedito {
        width: 323px;
    }
    .linhaInferior {
        margin-left: 6.9%;
    }
    .divSitua a.chosen-single.chosen-default{
        top: 10px;
    }
    .divTecnico{
        top: 10px;
    }
</style>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Relatório de Acompanhamento de Emendas</h2>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row" id="div-filtro-oculto">
            <form name="form-iniciativaplanejamento" id="form-iniciativaplanejamento" class="form-horizontal" method="post">
                <div class="row">
                    <div class="col-lg-6">
                        <input type="hidden" name="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>
                        <input type="hidden" name="requisicao_form" value="filtrar"/>
                        <input type="hidden" name="inpid" id="inpid" value="<?php echo $inp->inpid; ?>"/>
                        
                        <?php
                        $_REQUEST['tipo_processo'] = (empty($_REQUEST['tipo_processo']) ? 'P' : $_REQUEST['tipo_processo']);
                        $arrOptions = array('O' => 'Obras', 'P' => 'PAR');
                        ?>
                        <?php echo $simec->radio('tipo_processo', 'Tipo de processo', $_REQUEST['tipo_processo'], $arrOptions, array(), array()); ?>
                        
                        
                        <div class="form-group " id="linha_agrupadores">
                            <label for="calunas" class="col-sm-3 col-md-3 col-lg-3 control-label">Agrupadores:</label>
                            
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <?php
                                if (isset($_REQUEST['cols'])) {//recupera as colunas selecionadas do filtro
                                    $colunasDestino = colunasOrigem();
                                    $colunasSalvas = array();
                                    foreach ($_REQUEST['cols'] as $colunasPost) {
                                        foreach ($colunasDestino as $index => $colunaValores) {
                                            if ($colunaValores['codigo'] == $colunasPost) {
                                                array_push($colunasSalvas, $colunasDestino[$index]);
                                                break;
                                            }
                                        }
                                    }
                                }
                                    #Montar e exibe colunas
                                    $arrayColunasOrig = colunasOrigem();
                                    $arrayColunasDest = isset($colunasSalvas) ? $colunasSalvas : colunasDestino();
                                    $colunas = new Agrupador('form-iniciativaplanejamento');
                                    $colunas->setOrigem('nCols', null, $arrayColunasOrig);
                                    $colunas->setDestino('cols', null, $arrayColunasDest);
                                    $colunas->exibir();
                                ?>
                            </div>
                        </div>
                        <?php
                        $sqlCategoriaO = "SELECT octid AS codigo, octid||' - '||octdsc AS descricao FROM par3.obra_categoria WHERE octstatus = 'A' ORDER BY 1";
                        $sqlLocalO = array('13, 14, 25' => 'Cobertura', '1, 2, 19, 20, 21, 23' => 'Creche', '4, 5, 6, 7, 8, 9, 10, 11, 15, 16, 17, 18, 22' => 'Escola', '12, 24' => 'Quadra');
                        ?>
                        <?php echo $simec->select('octid', 'Categoria de obra', $_POST['octid'], $sqlCategoriaO, array('multiple', 'class' => 'soObra'), array('input-size' => '7', 'groupStyle' => 'display: none')); ?>
                        <?php echo $simec->select('localO', 'Local de obra', $_POST['localO'], $sqlLocalO, array('class' => 'soObra'), array('input-size' => '7', 'groupStyle' => 'display: none')); ?>
                        <?php
                        $sqlSituacoesO = "SELECT esdid AS codigo, esdordem||' - '||esddsc AS descricao FROM workflow.estadodocumento WHERE tpdid=".PAR3_WORKFLOW_FLUXO_OBRAS." AND esdstatus='A' ORDER BY esdordem";
                        $sqlTiposObjeto = "SELECT intoid AS codigo, intoid||' - '||intodsc AS descricao FROM par3.iniciativa_tipos_objeto WHERE intostatus='A' AND intosituacao='A' ORDER BY 1";
                        ?>
                        <?php echo $simec->select('esdid_analise', 'Situação da análise', $_REQUEST['esdid_analise'], $arrSituAnalise, array('multiple', 'class'=>'soObraN'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('esdidO', 'Situação da obra', $_POST['esdidO'], $sqlSituacoesO, array('multiple', 'class' => 'soObra'), array('input-size' => '8', 'groupStyle' => 'display: none')); ?>
                        <?php echo $simec->select('intoid', 'Tipo de objeto', $_POST['intoid'], $sqlTiposObjeto, array('multiple'), array('input-size' => '8')); ?>
                    </div>
                    <div class="col-lg-6" id="">
                        <?php echo $simec->radio('itrid', 'Esfera', ($_REQUEST['itrid']?:0), [1 => 'Estadual',2 => 'Municipal',0 =>'Todos'], array('maxlength' => '255')); ?>
                        <div class="row">
                            <div class="col-lg-4 anoClass" style="margin-left: 17%;">
                                <?php echo $simec->select('estuf', 'Estado', $_REQUEST['estuf'], $arrEstado, array('maxlength' => '255'), array()); ?>    
                            </div>
                            <div class="col-lg-5" id="div-muncod">
                                <?php echo $simec->select('muncod', 'Município', $_REQUEST['muncod'], $arrMunicipio, array('multiple', 'maxlength' => '255'), array()); ?>
                            </div>
                        </div>
                        <?php echo $simec->input('emecod', 'Emenda', $_REQUEST['emecod'], array('type' => 'number'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('autid', 'Parlamentar', $_POST['autid'], $sqlAutor, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('parid', 'Partido', $_POST['parid'], $sqlPartido, array('maxlength' => '255'), array('input-size' => '8')); ?>
                        <?php echo $simec->input('inpid_filtro', 'Código planejamento', $_REQUEST['inpid_filtro'], array('type' => 'number'), array('input-size' => '8')); ?>
                        <?php echo $simec->select('indid', 'Descrição iniciativa', $_REQUEST['indid'], $arrIniciativa, array('multiple', 'maxlength' => '255'), array('input-size' => '8')); ?>
                        <div class="row">
                            <div class="col-lg-4 anoClass" style="margin-left: 17%;">
                                <?php echo $simec->select('iniano', 'Ano', $_REQUEST['iniano'], $arrIniciativaAno, array('multiple', 'maxlength' => '255'), array()); ?>
                            </div>
                            <div class="col-lg-5">
                                &nbsp;
                            </div>
                        </div>
                    </div>
                </div>
            </form>
            </div>
            <div class="ibox-title" id="btn-filtro-oculto" data-toggle="tooltip" title="Clique para MINIMIZAR / MAXIMIZAR os campos de filtro" style="cursor: pointer">
                <div class="ibox-tools">
                    <a>Ocultar Filtros <i id="i-filtro-oculto" class="fa fa-chevron-down"></i></a>
                </div>
            </div>
            <div class="ibox-footer">
                <div class="center">
                    <button type="button" id="btn-pesquisar" data-dismiss="modal" class="btn btn-primary btn-pesquisar" data-loading-text="Pesquisando, aguarde ..."><i class="fa fa-search"></i> Pesquisar</button>
                    <button type="button" class="btn btn-success" id="btn-limpar"><i class="fa fa-eraser"></i> Limpar</button>
                </div>
            </div>
            <div class="row">
           <!-- Listagem  -->
            <div class="float-e-margins" id="listagem-obras">
                <div class="ibox-content" id="div-listagem-obras" style=": block;">
                    <div class="table-responsive" >
                            <?php
                            //ver($_GET,  $_POST, $_REQUEST);
                            if ($_REQUEST['requisicao_form'] == 'filtrar') {
                                // Inclui componente de relatórios
                                include APPRAIZ . 'includes/classes/relatorio.class.bootstrap.inc';
                                
                                // instancia a classe de relatório
                                $rel = new montaRelatorioBootstrap();
                                
                                $agrupador  = montaAgrupador();
                                $coluna     = montaColuna();
                                $sql        = montaSql();
                                $dados      = $db->carregar($sql);
                                
                                $rel->setAgrupador($agrupador, $dados);
                                $rel->setColuna($coluna);
                                $rel->setTolizadorLinha(true);
                                $rel->setTotNivel(true);
                                $rel->setExpandir(false);
                                $rel->setCssColuna(array("width:35%"));
                                $rel->setMaxHeight(400);
                                //                         $rel->setTotalizador(false);
                                $rel->setMonstrarTolizadorNivel(true);
                                
                                echo $rel->getRelatorio();
                            }
                            ?>
                    </div>
                    <br>
                    <br>
                </div>
            </div>
            <!-- END Listagem  -->
        </div>
    </div>
</div>

<script type="text/javascript" src="../includes/agrupador.js"></script>
<script>
jQuery(function() {
    //mostra/esconde ibox opções avançadas do filtro
    jQuery("#btn-filtro-oculto").click(function () {
        $('#div-filtro-oculto').slideToggle();
        //chevron up/down
        $('#i-filtro-oculto').toggleClass(function () {
            if ($('#i-filtro-oculto').is(".fa-chevron-down")) {
                $('#i-filtro-oculto').removeClass('fa-chevron-down');
                return 'fa-chevron-up';
            } else {
                $('#i-filtro-oculto').removeClass('fa-chevron-up');
                return 'fa-chevron-down';
            }
        });
    });

    renderizeSomenteObras();
    jQuery("input:radio[name=tipo_processo]").change(function() {
        renderizeSomenteObras();
    });

    //tela de loading
    $(window).on('beforeunload',function(){
        $('.loading-dialog-par3').show();
    });
    $(window).on('click','#btn-pesquisar #btn-limpar',function(){
        $('.loading-dialog-par3').show();
    });
    $(window).load(function(){
        $('.loading-dialog-par3').hide();
    });
    //renderizeMunicipio();

    jQuery('select[name=estuf]').change(function(){
        if(!$('#div-muncod').is(':visible')){
            return false;
        }
        carregarMunicipio(this.value, 'valueMunicipio');
    });
});

function renderizeSomenteObras() {
    var tipos               = jQuery('[name="tipo_processo"]:checked').val();
    var flag                = (tipos && tipos.length == 1 && tipos[0] === 'O');
    var flagExisteAgpObra   = (jQuery('#nCols').find('option[value="obrdsc"]').length > 0 || jQuery('#cols').find('option[value="obrdsc"]').length > 0);
        
    if (flag) {
        jQuery('[name="intoid[]"]').val('1').attr('disabled', true).trigger('chosen:updated');
        jQuery(".soObra").parents("div.form-group").slideDown();
        jQuery(".soObraN").parents("div.form-group").slideUp();
        
        jQuery('[name="esdid_analise[]"]').find('option').prop('selected', false);
        jQuery('[name="esdid_analise[]"]').trigger('chosen:updated');

        if (flagExisteAgpObra == false){
            jQuery('#nCols').append('<option value="obrdsc">Obra</option>');            
        }
    } else {
        var valIntoid = jQuery('[name="intoid[]"]').chosen().val();
        if ( valIntoid!=null && valIntoid.length == 1 && valIntoid[0] == 1 ){       
            jQuery('[name="intoid[]"]').find('option').prop('selected', false);
        }   
        jQuery('[name="intoid[]"]').attr('disabled', false).trigger('chosen:updated');
        
        jQuery('[name="octid[]"]').find('option').prop('selected', false);
        jQuery('[name="octid[]"]').trigger('chosen:updated');
        
        jQuery('[name="localO[]"]').find('option').prop('selected', false);
        jQuery('[name="localO[]"]').trigger('chosen:updated');
        
        jQuery('[name="esdidO[]"]').find('option').prop('selected', false);
        jQuery('[name="esdidO[]"]').trigger('chosen:updated');
        
        jQuery(".soObra").parents("div.form-group").slideUp();
        jQuery(".soObraN").parents("div.form-group").slideDown();
        
        if (flagExisteAgpObra == true){
            jQuery('#nCols').find('option[value="obrdsc"]').remove(); 
            jQuery('#cols').find('option[value="obrdsc"]').remove();
        }
    }
}

    jQuery("#btn-limpar").click(function () {
        window.location.href = 'par3.php?modulo=relatorio/acompanhamentoEmenda&acao=A';
    });

    //envia as informações do formulário via POST
    jQuery(".btn-pesquisar").click(function () {
        $btn = jQuery(this).button('loading');
        /*var action = window.location;
        var $dataForm = $('#form-iniciativaplanejamento').serialize();
        window.location.href = action+'&requisicao=filtrar&'+$dataForm;*/
        jQuery('[name="intoid[]"]').attr('disabled', false).trigger('chosen:updated');
        jQuery('#cols option').prop('selected', true);
        jQuery('[name="form-iniciativaplanejamento"]').submit();
    });

$('[name=itrid]').change(function(){
    if($(this).val() == 1){
        $('#div-muncod').hide();
        $('#muncod').find('option').prop('selected', false);
        $('#muncod').trigger('chosen:updated');
    }else{
        $('#div-muncod').show();
    }
});
</script>
<?php
function montaSql()
{
    $arWhere        = array();
    $arWherePAR     = array();
    $arWhereObra    = array();
    $arWhereWith    = array();
    $arWhereFora    = array();
    
    if ($_POST['inpid_filtro']) {
        $arWhereWith[] = "inp.inpid = {$_POST['inpid_filtro']}";
        $arWhere[]     = "pro.inpid = {$_POST['inpid_filtro']}";
    }
    $_POST['iniano'] = ($_POST['iniano'] ? $_POST['iniano'] : 2018);
    if ($_POST['iniano']) {
        $_POST['iniano'] = (array) $_POST['iniano'];
        $arWhereWith[] = "pla.ipiano IN (" . implode(", ", $_POST['iniano']) . ")";
        $arWhere[]     = "ve.emeano IN (" . implode(", ", $_POST['iniano']) . ")";
    }
    if ($_POST['cicid']) {
        $_POST['cicid'] = (array) $_POST['cicid'];
        $arWhereWith[] = "ini.cicid IN (" . implode(", ", $_POST['cicid']) . ")";
        $arWhere[]     = "pro.cicid IN (" . implode(", ", $_POST['cicid']) . ")";
    }
    if ($_POST['dimid']) {
        $_POST['dimid'] = (array) $_POST['dimid'];
        $arWhereWith[] = "inp.dimid IN (" . implode(", ", $_POST['dimid']) . ")";
        $arWhere[]     = "pro.dimid IN (" . implode(", ", $_POST['dimid']) . ")";
    }
    if ($_POST['indid']) {
        $_POST['indid'] = (array) $_POST['indid'];
        $arWhereWith[] = "ini.indid IN (" . implode(", ", $_POST['indid']) . ")";
        $arWhere[]     = "pro.indid IN (" . implode(", ", $_POST['indid']) . ")";
    }
    
    if ($_POST['esdid_analise']) {
        $_POST['esdid_analise'] = (array) $_POST['esdid_analise'];
        $arWhereWith[] = "a.esdid_analise IN (" . implode(", ", $_POST['esdid_analise']) . ")";
        $arWhere[]     = "pro.esdid_analise IN (" . implode(", ", $_POST['esdid_analise']) . ")";
    }
    if ($_POST['esdidO']) {
        $_POST['esdidO'] = (array) $_POST['esdidO'];
        $arWhereWith[] = "a.esdid_obra IN (" . implode(", ", $_POST['esdidO']) . ")";
        $arWhere[]     = "pro.esdid_obra IN (" . implode(", ", $_POST['esdidO']) . ")";
    }
    if ($_POST['localO']) {
        $_POST['localO'] = (array) $_POST['localO'];
        $arWhereWith[] = "a.otpid IN (" . implode(", ", $_POST['localO']) . ")";
        $arWhere[]     = "pro.otpid IN (" . implode(", ", $_POST['localO']) . ")";
    }
    if ($_POST['octid']) {
        $_POST['octid'] = (array) $_POST['octid'];
        $arWhereWith[] = "a.octid IN (" . implode(", ", $_POST['octid']) . ")";
        $arWhere[]     = "pro.octid IN (" . implode(", ", $_POST['octid']) . ")";
    }
    if ($_POST['itrid']) {
        $_POST['itrid'] = (array) $_POST['itrid'];
        $arWhere[] = "iue.itrid IN (" . implode(", ", $_POST['itrid']) . ")";
    }
    if ($_POST['estuf']) {
//      $arWhereObra[] = "inu.estuf = '{$_POST['estuf']}'";
//      $arWherePAR[] = "eme.estuf = '{$_POST['estuf']}'";
        $arWhere[] = "ve.estuf = '{$_POST['estuf']}'";
    }
    if ($_POST['muncod']) {
        $_POST['muncod'] = (array) $_POST['muncod'];
//      $arWhereObra[] = "inu.muncod IN ('" . implode("', '", $_POST['muncod']) . "')";
//      $arWherePAR[] = "eme.muncod IN ('" . implode("', '", $_POST['muncod']) . "')";
        $arWhere[] = "ve.muncod IN ('" . implode("', '", $_POST['muncod']) . "')";
    }
    if ($_POST['intoid']) {
        $_POST['intoid'] = (array) $_POST['intoid'];
        $arWhereWith[] = "ini.intoid IN (" . implode(", ", $_POST['intoid']) . ")";
        $arWhere[]     = "pro.intoid IN (" . implode(", ", $_POST['intoid']) . ")";
    }
    if ($_POST['etaid']) {
        $_POST['etaid'] = (array) $_POST['etaid'];
        $arWhereWith[] = "inp.etaid IN (" . implode(", ", $_POST['etaid']) . ")";
        $arWhere[]     = "pro.etaid IN (" . implode(", ", $_POST['etaid']) . ")";
    }
    if ($_POST['autid']) {
        $_POST['autid'] = (array) $_POST['autid'];
        $arWhere[]     = "au.autid IN (" . implode(", ", $_POST['autid']) . ")";
    }
    if ($_POST['parid']) {
        $_POST['parid'] = (array) $_POST['parid'];
        $arWhere[]     = "pa.parid IN (" . implode(", ", $_POST['parid']) . ")";
    }
    if ($_POST['parid']) {
        $_POST['parid'] = (array) $_POST['parid'];
        $arWhere[]     = "pa.parid IN (" . implode(", ", $_POST['parid']) . ")";
    }
    if ($_POST['emecod']) {
        $_POST['emecod'] = (array) $_POST['emecod'];
        $arWhere[]     = "ve.emecod IN ('" . implode("', '", $_POST['emecod']) . "')";
    }
        
    $sql = "WITH iniciativa_emenda AS (
	select
		ie.inpid,
		'(' || ie.inpid || ') ' || ind.inddsc as iniciativa,
		pla.ipiano,
		pla.valor_planejado,
		pla.valor_indicado,
		ie.inevalor AS valor_designado_iniciativa,
		ana.valor_aprovado,
        a.vlr_processo as valor_obra,
		CASE WHEN a.objeto = 'Obra' THEN a.vlr_contrapartida ELSE ie.inecontrapartida END AS vlr_contrapartida,
        emp.vlr_empenho,
		pg.vlr_pagamento,
		ie.edeid,
		ie.emeid,
        inp.dimid,
        inp.etaid,
        ini.cicid,
        ini.indid,
        ini.intoid,
        a.obrdsc,
        a.otpid,
        a.octid,
        a.esdid_analise,
        a.esddsc_analise,
        a.esdid_obra,
        a.esddsc_obra
	FROM 
		par3.iniciativa_emenda ie
	INNER JOIN par3.iniciativa_planejamento inp ON inp.inpid = ie.inpid AND 
                                                   inp.inpstatus = 'A'
	INNER JOIN par3.iniciativa ini ON ini.iniid = inp.iniid AND 
                                      ini.inistatus = 'A'
	INNER JOIN par3.iniciativa_descricao ind ON ind.indid = ini.indid
    LEFT JOIN(
		select
			ipi.inpid,
			ipi.ipiano,
			sum((ipi.ipivalorreferencia * coalesce(ipi.ipiquantidade,0) )) as valor_planejado,
			sum(coalesce(eic.ieiquantidade,0) * ipi.ipivalorreferencia ) AS valor_indicado
		from
			par3.iniciativa_planejamento_item_composicao ipi
		LEFT JOIN par3.iniciativa_emenda_item_composicao eic ON eic.ipiid = ipi.ipiid AND 
                                                                eic.ieistatus = 'A'	
		where
			ipi.ipistatus = 'A'		
		group by
			ipi.inpid, ipi.ipiano
	) pla ON pla.inpid = inp.inpid
	LEFT JOIN (
		SELECT 
			ie.inpid, ie.anaano, /*d.esdid,*/ (ai.aicqtdaprovado * ai.aicvaloraprovado) AS valor_aprovado 
		FROM 
			par3.analise ie
		JOIN par3.analise_itemcomposicao ai ON ai.anaid = ie.anaid AND 
                                               ai.aicstatus = 'A'
        --JOIN workflow.documento d on d.docid = ie.docid
		WHERE 
			ie.anastatus = 'A' AND 
			ie.edeid IS NOT null
			AND ai.aicqtdaprovado > 0  
	) ana ON ana.anaano = pla.ipiano AND 
             ana.inpid = inp.inpid
	LEFT join
	(
			SELECT 
				edeid, 
                inpid, 
                null as obrdsc,
                null as otpid,
                null as octid,
                sum(vlr_processo) AS vlr_processo, 
                vlr_contrapartida, 
                proid, 
                intodsc AS objeto, 
                intoid, 
                esddsc as esddsc_analise, 
                esdid as esdid_analise, 
                null as esdid_obra,
                null as esddsc_obra
			from
			(
				SELECT DISTINCT
					a.edeid,
					a.inpid,
					(ai.aicqtdaprovado::numeric * ai.aicvaloraprovado)  AS vlr_processo,
					0 AS vlr_contrapartida,
					esd1.esddsc, 
					pp.proid, 
					ito.intodsc,
					ito.intoid,
					esd1.esdid
				FROM 
                    par3.analise a
				INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND 
                                                             ai.aicstatus = 'A'::bpchar
				INNER JOIN par3.iniciativa_emenda_item_composicao eic ON eic.ipiid = ai.ipiid AND 
                                                                         eic.ieistatus = 'A'
				INNER JOIN workflow.documento as doc1 on doc1.docid = a.docid
				INNER JOIN workflow.estadodocumento as esd1 on esd1.esdid = doc1.esdid
				LEFT JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND 
                                                           pp.inpid = a.inpid and 
                                                           pp.ppcstatus = 'A'
				LEFT JOIN par3.iniciativa_tipos_objeto ito ON ito.intoid = pp.intoid
				WHERE 
					a.anastatus = 'A'
			) AS foo
			GROUP BY 
				edeid, inpid, esddsc, proid, intodsc, vlr_contrapartida,intoid, esdid
		UNION ALL
			SELECT
				ie.edeid,
				io.inpid,
                o.obrdsc,
                ot.otpid,
                oc.octid,
				sum(o.obrvalor) AS vlr_processo,
				sum(o.obrvalor_contrapartida) AS vlr_contrapartida,
				po.proid, 
				'Obra' AS objeto,
				1 as intoid,
                null as esdid_analise,
                null as esddsc_analise, 
                esd.esdid as esdid_obra,
				esd.esddsc as esddsc_obra
			FROM 
                par3.iniciativa_emenda_obra io
			INNER JOIN par3.iniciativa_emenda ie ON ie.ineid = io.ineid AND 
                                                    ie.inestatus = 'A'
			INNER JOIN par3.obra o ON o.obrid = io.obrid AND 
                                      o.obrstatus = 'A'
        	join par3.obra_tipo ot on ot.otpid = o.otpid and ot.otpstatus='A'
        	join par3.obra_categoria oc on oc.octid = ot.octid and oc.octstatus='A'
			INNER JOIN workflow.documento as doc on doc.docid = o.docid
			INNER JOIN workflow.estadodocumento as esd on esd.esdid = doc.esdid
			LEFT JOIN par3.processoobracomposicao po ON po.obrid = io.obrid AND 
                                                        po.pocstatus = 'A'
			WHERE 
				io.ieostatus = 'A'
			GROUP BY 
				ie.edeid, io.inpid, esd.esddsc,esd.esdid, po.proid, o.obrdsc, ot.otpid, oc.octid
	) AS a ON a.edeid = ie.edeid AND 
              a.inpid = ie.inpid

 	LEFT JOIN par3.processo p
	    LEFT JOIN par3.documentotermo dt ON dt.proid = p.proid AND dt.dotstatus = 'A'
	    LEFT JOIN(
	    	SELECT 
	    		e.empcodigoespecie, e.empid, e.empnumeroprocesso, 
	    		e.empnumero, (sum(e.empvalorempenho) - COALESCE(ec.vlr_cancelado,0)) AS vlr_empenho 
	    	FROM 
	    		par3.empenho e
	        LEFT JOIN(
	        	SELECT 
	        		e.empcodigoespecie, e.empidpai, sum(e.empvalorempenho) AS vlr_cancelado 
	        	FROM 
	        		par3.empenho e 
	        	WHERE 
	        		e.empstatus = 'A' AND 
	        		e.empcodigoespecie in ('03', '13', '04') 
	        	GROUP BY e.empidpai, e.empcodigoespecie
	        ) ec ON ec.empidpai = e.empid
	        WHERE 
	        	e.empstatus = 'A' AND 
	        	e.empcodigoespecie = '01' 
	        GROUP BY e.empid, ec.vlr_cancelado, e.empnumeroprocesso
		) emp ON emp.empnumeroprocesso = p.pronumeroprocesso
	    LEFT JOIN(
	    	SELECT 
	    		p.empid, sum(p.pagvalorparcela) AS vlr_pagamento 
	    	FROM 
	    		par3.pagamento p 
	    	WHERE 
	    		p.pagstatus = 'A'
	    		and p.pagsituacaopagamento ilike '%efetivado%' 
	    	GROUP BY p.empid
		) pg ON pg.empid = emp.empid
    ON p.proid = a.proid AND 
       p.prostatus = 'A'              

	WHERE 
		ie.inestatus = 'A'
        ".(count($arWhereWith) > 0 ? " AND " . implode(" AND ", $arWhereWith) : "")."
)
SELECT distinct
	pa.parsigla as partido,
	au.autcod||' - '||au.autnome AS autor,
	ve.emecod as emenda,
	ve.enbnome as beneficiado,
	pro.iniciativa,
    pro.obrdsc,
	ve.emevalor as valor_emenda,
	pro.valor_planejado,	
	valor_designado_iniciativa,
	coalesce(pro.vlr_contrapartida,0) AS valor_contrapartida,
	coalesce(pro.vlr_empenho,0) AS valor_empenho,
	coalesce(pro.vlr_pagamento,0) AS valor_pagamento,
	pro.valor_indicado,
	pro.valor_aprovado,
    pro.valor_obra
FROM emenda.v_emendadetalheentidade ve
	INNER JOIN emenda.autor au ON au.autid = ve.autid
	INNER JOIN emenda.partido pa ON pa.parid = au.parid
	INNER JOIN(
		SELECT 
			DISTINCT  entcnpj, inuid 
		FROM 
			par3.instrumentounidade_entidade 
		WHERE 
			entstatus = 'A' AND entcnpj IS NOT NULL
	) iu on iu.entcnpj = ve.enbcnpj
    INNER JOIN par3.instrumentounidade iue ON iue.inuid = iu.inuid AND iue.inustatus = 'A'
	LEFT JOIN iniciativa_emenda pro ON pro.emeid = ve.emeid AND pro.edeid = ve.edeid and ve.emeano = pro.ipiano 
WHERE 
	ve.edestatus = 'A' and 
    ".(count($arWhere) > 0 ? implode(" AND ", $arWhere) . " AND " : "")."
	ve.etoid = 1";
    
    return $sql;
}

function montaAgrupador()
{
    if ($_POST['tipo_processo'] == 'P') {
        $agrupador = array(
                "agrupador" => array(),
                "agrupadoColuna" => array(
                                            'valor_emenda',
                                            'valor_empenho',
                                            'valor_pagamento',
//                                          'valor_designado',
                                            'valor_planejado',
                                            'valor_indicado',
                                            'valor_designado_iniciativa',
                                            'valor_contrapartida',
                                            'valor_aprovado'
                ),
                "associacaoTotalizadorColunaComAgrupador" => array(
                        'valor_emenda' => 'emenda',
                        'valor_planejado' => 'emenda',
                        'valor_indicado' => 'iniciativa',
                        'valor_aprovado' => 'iniciativa',
                        'valor_contrapartida' => 'iniciativa',
                        'valor_designado_iniciativa' => 'iniciativa'
                )
        );
    } else {
        $agrupador = array(
                "agrupador" => array(),
                "agrupadoColuna" => array(
                                            'valor_emenda',
                                            'valor_empenho',
                                            'valor_pagamento',
                                            'valor_designado_iniciativa',
                                            'valor_contrapartida',
                                            'valor_obra',
                                            'valor_indicado_obra',
                                            'valor_aprovado'
                ),
                "associacaoTotalizadorColunaComAgrupador" => array(
                    'valor_emenda' => 'emenda',
                    'valor_planejado' => 'emenda',
                    'valor_indicado' => 'iniciativa',
                    'valor_aprovado' => 'emenda',
                    'valor_obra' => 'iniciativa',
                    'valor_contrapartida' => 'iniciativa',
                    'valor_designado_iniciativa' => 'iniciativa'
                )
        );
    }
    
    foreach ($_POST['cols'] as $colName) {
        switch ($colName) {
            case 'beneficiado':
                array_push($agrupador['agrupador'], array('campo'=>'beneficiado', 'label'=>'Beneficiário'));
                break;
            case 'emenda':
                array_push($agrupador['agrupador'], array('campo'=>'emenda', 'label'=>'Emenda'));
                break;
            case 'autor':
                array_push($agrupador['agrupador'], array('campo'=>'autor', 'label'=>'Autor'));
                break;
            case 'obrdsc':
                array_push($agrupador['agrupador'], array('campo'=>'obrdsc', 'label'=>'Obra'));
                break;
            case 'partido':
                array_push($agrupador['agrupador'], array('campo'=>'partido', 'label'=>'Partido'));
                break;
            case 'iniciativa':
                array_push($agrupador['agrupador'], array('campo'=>'iniciativa', 'label'=>'Iniciativa'));
                break;
        }
    }
    return $agrupador;
}

function montaColuna()
{

    if ($_POST['tipo_processo'] == 'P') {
        $coluna    = array(
                array(
                        "campo"     => "valor_emenda",
                        "label"     => "Valor de emenda",
                        "blockAgp"  => "",
                        "type"      => "money"),
//                 array(
//                         "campo"     => "valor_planejado",
//                         "label"     => "Valor planejado",
//                         "blockAgp"  => "",
//                         "type"      => "money"),
                array(
                        "campo"     => "valor_designado_iniciativa",
                        "label"     => "Valor designado iniciativa",
                        "blockAgp"  => "",
                        "type"      => "money"),
                array(
                        "campo"     => "valor_contrapartida",
                        "label"     => "Valor de Contrapartida",
                        "blockAgp"  => "",
                        "type"      => "money"),
                array(
                        "campo"     => "valor_indicado",
                        "label"     => "Valor Indicado",
                        "blockAgp"  => "",
                        "type"      => "money"),
                array(
                        "campo"     => "valor_aprovado",
                        "label"     => "Valor Aprovado",
                        "blockAgp"  => "",
                        "type"      => "money"),
                array(
                        "campo"     => "valor_empenho",
                        "label"     => "Valor Empenhado",
                        "blockAgp"  => "",
                        "type"      => "money"),
                array(
                        "campo"     => "valor_pagamento",
                        "label"     => "Valor Pagamento",
                        "blockAgp"  => "",
                        "type"      => "money")
            
            );
    } else {
        $coluna    = array(
                array(
                        "campo"     => "valor_emenda",
                        "label"     => "Valor de emenda",
                        "blockAgp"  => "",
                        "type"      => "money"),
//              array(
//                      "campo"     => "valor_planejado",
//                      "label"     => "Valor planejado",
//                      "blockAgp"  => "",
//                      "type"      => "numeric"),
                array(
                        "campo" => "valor_designado_iniciativa",
                        "label" => "Valor designado iniciativa",
                        "blockAgp" => "",
                        "type" => "money"),
                array(
                        "campo"     => "valor_indicado_obra",
                        "label"     => "Valor Indicado",
                        "blockAgp"  => "",
                        "type"      => "money"),
                array(
                        "campo"     => "valor_obra",
                        "label"     => "Valor da Obra",
                        "blockAgp"  => "",
                        "type"      => "money"),
                array(
                        "campo"     => "valor_contrapartida",
                        "label"     => "Valor de Contrapartida",
                        "blockAgp"  => "",
                        "type"      => "money"),
                array(
                        "campo"     => "valor_empenho",
                        "label"     => "Valor Empenhado",
                        "blockAgp"  => "",
                        "type"      => "money"),
                array(
                        "campo"     => "valor_pagamento",
                        "label"     => "Valor Pagamento",
                        "blockAgp"  => "",
                        "type"      => "money")
            );
    }

    return $coluna;
}

function colunasOrigem()
{
    return array(
            array(
                    'codigo'    => 'autor',
                    'descricao' => 'Autor'
            ),
            array(
                    'codigo'    => 'beneficiado',
                    'descricao' => 'Beneficiário'
            ),
            array(
                    'codigo'    => 'emenda',
                    'descricao' => 'Emenda'
            ),
            array(
                    'codigo'    => 'iniciativa',
                    'descricao' => 'Iniciativa'
            ),
            array(
                    'codigo'    => 'obrdsc',
                    'descricao' => 'Obra'
            ),
            array(
                    'codigo'    => 'partido',
                    'descricao' => 'Partido'
            ),
    );
}

function colunasDestino()
{
    return array(
            array(
                    'codigo'    => 'partido',
                    'descricao' => 'Partido'
            ),
            array(
                    'codigo'    => 'autor',
                    'descricao' => 'Autor'
            ),
            array(
                    'codigo'    => 'emenda',
                    'descricao' => 'Emenda'
            ),
            array(
                    'codigo'    => 'beneficiado',
                    'descricao' => 'Beneficiário'
            ),
            array(
                    'codigo'    => 'iniciativa',
                    'descricao' => 'Iniciativa'
            ),
            array(
                    'codigo'    => 'obrdsc',
                    'descricao' => 'Obra'
            ),
    );
}
?>





















