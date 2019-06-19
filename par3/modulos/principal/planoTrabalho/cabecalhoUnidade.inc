<?php

include_once APPRAIZ.'par3/modulos/principal/planoTrabalho/alertas/alertaTermoNaoAssinado.php';

$obModelUnidade         = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$arrEsdDiag             = $obModelUnidade->retornaEstadosPrePlanejamento();
$mdWorkdlow             = new Workflow_Model_Documento($obModelUnidade->docid);
$controllerRestricao    = new Par3_Controller_Restricao();
$controllerPendencia    = new Par3_Controller_Pendencia();
$helperPendencia        = new Par3_Helper_PendenciasEntidade();

if ($_REQUEST['req'] == 'salvarJustificativaTreinamento') {
	$obModelUnidade->salvarJustificativaTreinamento($_REQUEST['$inuid']);
	die;
}

/**
 * Cabecalho da Unidade
 *
 * @category Componente
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 24/09/2015
 * @link     no link
 */

$esfera = ($$obModelUnidade->itrid === '2') ? 'Municipal' : 'Estadual';

$url = explode('=', $_SESSION['favurl']);
$rastro = explode('&', $url[1]);
$rastro = $rastro[0];
$urlPlanoTrabalho          = 'par3.php?modulo=principal/planoTrabalho/';
$urlBtnDadosUnidade        = $urlPlanoTrabalho.'dadosUnidade&acao=A&inuid='.$_REQUEST['inuid'];

$urlBtnEmendas        = 'par3.php?modulo=principal/emendas/listaEmendasMunicipio&acao=A&inuid='.$_REQUEST['inuid'];

$urlBtnObras = $urlPlanoTrabalho.'obraPlanejamento&acao=A&inuid='.$_REQUEST['inuid'];
$msgObras = " ++ ";

//$testaPreenchimentoUnidade = Par3_Controller_InstrumentoUnidade::verificaPercentualPreenchimentoUnidade($_REQUEST['inuid']);
$testaPreenchimentoUnidade = 100;
$urlBtnPNE = $urlPlanoTrabalho.'pne&acao=A&inuid='.$_REQUEST['inuid'];

if ($testaPreenchimentoUnidade != 100) {
    $msgPNE = 'Falta preencher os Dados da Unidade.';
}

//$testaPreenchimentoPNE = Par3_Controller_Pne::verificaPreenchimentoPNE($_REQUEST['inuid']);
$testaPreenchimentoPNE = 100;
$urlBtnQuestoesEstrategicas = $urlPlanoTrabalho.'questoesEstrategicas&acao=A&inuid='.$_REQUEST['inuid'];

if ($testaPreenchimentoPNE != 100) {
    $msgQuestoesEstrategicas = 'Falta terminar o PNE.';
}

//$testaPreenchimentoQuestoesEstrategicas = Par3_Controller_InstrumentoUnidade::verificaPreencimentoQuestoes($_REQUEST['inuid']);
$testaPreenchimentoQuestoesEstrategicas = 100;
$urlBtnAcompanhamento = $urlPlanoTrabalho.'acompanhamento&acao=A&inuid='.$_REQUEST['inuid'];

if ($testaPreenchimentoQuestoesEstrategicas != 100) {
    $msgAcompanhamento = 'Falta finalizar as Questões Estratégicas.';
}

$testaPreenchimentoAcompanhamento = 100;
$testaPendencias = 100;

 $urlBtnPendencias = $urlPlanoTrabalho.'pendencias&acao=A&inuid='.$_REQUEST['inuid'];
//$urlBtnPendencias = 'par3.php?modulo=par3/controle/pendencia&acao=A&inuid='.$_REQUEST['inuid'];
$urlBtnIndicadoresQualitativos = $urlPlanoTrabalho.'indicadoresQualitativos&acao=A&inuid='.$_REQUEST['inuid'];
$urlBtnPlanejamento = $urlPlanoTrabalho.'planejamento&acao=A&inuid='.$_REQUEST['inuid'];

$situacao = recuperaSituacaoPreenchimentoDiagnostico($_REQUEST['inuid']);
$testaPreenchimentoIndicadoresQualitativos = $situacao['percent'];

$testaPreenchimentoIndicadoresQualitativos = 100;
$testaPreenchimentoPlanejamento = 100;

$corSeta = '#e3e3e3';
$corFundoUnidade  = '#e3e3e3';
$corFundoPNE = '#e3e3e3';
$corFundoQuestoesEstrategicas = '#e3e3e3';
$corFundoAcompanhamento = '#e3e3e3';
$corFundoPendencias = '#e3e3e3';
$corFundoIndicadoresQualitativos  = '#e3e3e3';
$corFundoPlanejamento  = '#e3e3e3';
$corFundoEmendas  = '#e3e3e3';
$corFundoObras  = '#e3e3e3';

$setaDadosUnidade             = 'style="display:none;"';
$setaPne                      = 'style="display:none;"';
$setaQuestoesEstrategicas     = 'style="display:none;"';
$setaAcompanhamento           = 'style="display:none;"';
$setaPendencias               = 'style="display:none;"';
$setaIndicadoresQualitativos  = 'style="display:none;"';
$setaPlanejamento             = 'style="display:none;"';
$setaObras                    = 'style="display:none;"';
$setaEmendas                  = 'style="display:none;"';

switch($rastro){
	case 'principal/planoTrabalho/pne':
	    $setaPne       = '';
	    $corPreenchido = '#1c83c6';
	    $ProxCor       = '#f8ab59';
        unset($urlBtnPNE);

	    if ($corFundoPNE !== '') {
    	    $corSeta = $corFundoPNE;
	    }

	    break;
	case 'principal/planoTrabalho/questoesPNE':
	case 'principal/planoTrabalho/questoesEstrategicas':
	case 'principal/planoTrabalho/bandaLargaQuestionario':
	    $setaQuestoesEstrategicas = '';
	    $posicaoSeta          = '58.35%';
	    $corPreenchido        = '#f8ab59';
	    $ProxCor              = '#DE6466';
        unset($urlBtnQuestoesEstrategicas);

	    if ($corFundoQuestoesEstrategicas !== '') {
    	    $corSeta = $corFundoQuestoesEstrategicas;
	    }

	    break;
	case 'principal/planoTrabalho/acompanhamento':
	case 'principal/planoTrabalho/acompanhamento/solicitaReprogramacao':
	    $setaAcompanhamento = '';
	    $corPreenchido      = '#22c6c8';
	    $ProxCor            = '#f8ab59';
        unset($urlBtnAcompanhamento);

	    if ($corFundoAcompanhamento !== '') {
    	    $corSeta = $corFundoAcompanhamento;
	    }

	    break;
	case 'par3/controle/pendencia':
// 	case 'principal/planoTrabalho/pendencias':
	    $setaPendencias = '';
	    $corPreenchido      = '#DE6466';
	    $ProxCor            = '#f8ab59';
        unset($urlBtnPendencias);

	    if ($corFundoPendencias !== '') {
    	    $corSeta = $corFundoPendencias;
	    }

	    break;
	case 'principal/planoTrabalho/indicadoresQualitativos':
	    $setaIndicadoresQualitativos = '';
	    $corPreenchido                = '#DE6466';
	    $ProxCor = '#A0EFEF';
        unset($urlBtnIndicadoresQualidativos);

	    if ($corFundoIndicadoresQualidativos !== '') {
    	    $corSeta = $corFundoIndicadoresQualidativos;
	    }

	    break;
	case 'principal/planoTrabalho/planejamento':
	    $setaPlanejamento = '';
	    $corPreenchido = $ProxCor = '#A0EFEF';
        unset($urlBtnPlanejamento);

	    if ($corFundoPlanejamento !== '') {
    	    $corSeta = $corFundoPlanejamento;
	    }

	    break;
    case 'principal/planoTrabalho/obraPlanejamento':
        //$setaIndicadoresQualitativos = '';
        $corPreenchido  = '#DE6466';
        $ProxCor = '#A0EFEF';
        $setaObras = '';
        $urlBtnObras = '';
        unset($urlBtnIndicadoresQualidativos);
        if ($corFundoIndicadoresQualidativos !== '') {
            $corSeta = $corFundo;
        }
        break;
    case 'principal/emendas/listaEmendasMunicipio':
        $corPreenchido  = '#DE6466';
        $ProxCor = '#A0EFEF';
        $setaEmendas = '';
        $urlBtnEmendas = '';
        unset($urlBtnIndicadoresQualidativos);
        if ($corFundoIndicadoresQualidativos !== '') {
            $corSeta = $corFundo;
        }
        break;
	case 'principal/planoTrabalho/dadosUnidade':
	default:
	    $setaDadosUnidade = '';
	    $corPreenchido    = '#000000';
	    $ProxCor          = '#1c83c6';

	    if ($corFundoUnidade !== '') {
    	    $corSeta = $corFundoUnidade;
	    }
        unset($urlBtnDadosUnidade);
	    break;
}

if ($testaPreenchimentoUnidade > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoUnidade === 100) {
        $corFim = '#1c83c6';
        $corFundoUnidade  = '#1ab394';
    }

    $corIni = 0;

    if ($testaPreenchimentoUnidade > 20) $corIni = $testaPreenchimentoUnidade -20;

    $corFundoBarraUnidade = '
            background: -moz-linear-gradient(left, #1ab394 '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoUnidade.'%, '.$corFim.' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#1ab394), color-stop('.floor($testaPreenchimentoUnidade-1).
                '%,'.$corFim.'), color-stop(100%,'.$corFim.')); /*Chrome,Safari4+*/';
}

if ($testaPreenchimentoPNE > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoPNE === 100) {
        $corFim = '#f8ab59';
        $corFundoPNE = '#1c83c6';
    } else {
//         $testaPreenchimentoAcompanhamento = 0;
    }

    $corIni = 0;

    if ($testaPreenchimentoPNE > 20) $corIni = $testaPreenchimentoPNE -20;

    $corFundoBarraPNE = '
            background: -moz-linear-gradient(left, #1c83c6 '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoPNE.'%, '.$corFim.' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#1c83c6), color-stop('.floor($testaPreenchimentoPNE-1).
                '%,'.$corFim.'), color-stop(100%,'.$corFim.')); /*Chrome,Safari4+*/';
}

if ($testaPreenchimentoQuestoesEstrategicas > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoQuestoesEstrategicas === 100) {
        $corFim = '#22c6c8';
        $corFundoQuestoesEstrategicas = '#f8ab59';
    } else {
        $testaPreenchimentoIndicadoresQualitativos = 0;
    }

    $corIni = 0;

    if ($testaPreenchimentoQuestoesEstrategicas > 20) $corIni = $testaPreenchimentoQuestoesEstrategicas -20;

    $corFundoBarraQuestoesEstrategicas = '
            background: -moz-linear-gradient(left, #f8ab59 '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoQuestoesEstrategicas.'%, '.$corFim.' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#f8ab59), color-stop('.
                floor($testaPreenchimentoQuestoesEstrategicas-1).'%,'.$corFim.
                '), color-stop(100%,'.$corFim.')); /* Chrome,Safari4+ */';
}

if ($testaPreenchimentoAcompanhamento > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoAcompanhamento === 100) {
        $corFim = '#DE6466';
        $corFundoAcompanhamento = '#22c6c8';
    } else {
        $testaPreenchimentoQuestoesEstrategicas = 0;
    }

    $corIni = 0;

    if ($testaPreenchimentoAcompanhamento > 20) $corIni = $testaPreenchimentoAcompanhamento -20;

    $corFundoBarraAcompanhamento = '
            background: -moz-linear-gradient(left, #22c6c8 '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoAcompanhamento.'%, '.$corFim.' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#22c6c8), color-stop('.
                floor($testaPreenchimentoAcompanhamento-1).'%,'.$corFim.'), color-stop(100%,'.
                $corFim.')); /* Chrome,Safari4+ */';
}

if ($testaPendencias > 0) {
    $corFim = '#e3e3e3';

    if ($testaPendencias === 100) {
        $corFim = '#000';
        $corFundoPendencias = '#DE6466';
    }

    $corIni = 0;

    if ($testaPendencias > 20) $corIni = $testaPendencias -20;

    $corFundoBarraPendencias = '
            background: -moz-linear-gradient(left, #DE6466 '.$corIni.'%, '.$corFim.' '.
                $testaPendencias.'%, '.$corFim.
                ' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#DE6466), color-stop('.
                floor($testaPendencias-1).'%,'.$corFim.
                '), color-stop(100%,'.$corFim.')); /* Chrome,Safari4+ */';
}

if ($testaPreenchimentoIndicadoresQualitativos > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoIndicadoresQualitativos === 100) {
        $corFim = '#A0EFEF';
        $corFundoIndicadoresQualitativos = '#000';
    }

    $corIni = 0;

    if ($testaPreenchimentoIndicadoresQualitativos > 20) $corIni = $testaPreenchimentoIndicadoresQualitativos -20;

    $corFundoBarraIndicadoresQualitativos = '
            background: -moz-linear-gradient(left, #000000 '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoIndicadoresQualitativos.'%, '.$corFim.
                ' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#000000), color-stop('.
                floor($testaPreenchimentoIndicadoresQualitativos-1).'%,'.$corFim.
                '), color-stop(100%,'.$corFim.')); /* Chrome,Safari4+ */';
}

if ($testaPreenchimentoPlanejamento > 0) {
    $corFim = '#e3e3e3';

    if ($testaPreenchimentoPlanejamento === 100) {
        $corFim = '#A0EFEF';
        $corFundoPlanejamento = '#A0EFEF';
    }

    $corIni = 0;

    if ($testaPreenchimentoPlanejamento > 20) $corIni = $testaPreenchimentoPlanejamento -20;

    $corFundoBarraPlanejamento = '
            background: -moz-linear-gradient(left, #A0EFEF '.$corIni.'%, '.$corFim.' '.
                $testaPreenchimentoPlanejamento.'%, '.$corFim.
                ' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
                'color-stop('.$corIni.'%,#A0EFEF), color-stop('.
                floor($testaPreenchimentoPlanejamento-1).'%,'.$corFim.
                '), color-stop(100%,'.$corFim.')); /* Chrome,Safari4+ */';
}
$testaPreenchimentoObra = 20;
if($testaPreenchimentoObra > 0){
$corIni = "#F5C929";
$corFim = '#F5C929';
    $corFundoBarraObras = '
            background: -moz-linear-gradient(left, #DE6466 '.$corIni.'%, '.$corFim.' '.
        $testaPendencias.'%, '.$corFim.
        ' 100%); /* FF3.6+ */
            background: -webkit-gradient(linear, left top, right top, '.
        'color-stop('.$corIni.'%,#DE6466), color-stop('.
        floor($testaPendencias-1).'%,'.$corFim.
        '), color-stop(100%,'.$corFim.')); /* Chrome,Safari4+ */';
}

?>
<style>
.corFundoUnidade {
	background-color: <?php echo$corFundoUnidade;
	?>;
}

.corFundoUnidade:not  (.selecionado  ) div:hover {
	background-color: #18866F !important;
}

.barraUnidade { <?php echo $corFundoBarraUnidade; ?>

}

.corFundoPNE {
	background-color: <?php echo$corFundoPNE;
	?>;
}

.corFundoPNE:not  (.selecionado  ) div:hover {
	background-color: #144E73 !important;
}

.barraPNE { <?php echo $corFundoBarraPNE; ?>

}

.corFundoQuestoesEstrategicas {
	background-color: <?php echo$corFundoQuestoesEstrategicas;
	?>;
}

.corFundoQuestoesEstrategicas:not  (.selecionado  ) div:hover {
	background-color: #B18658 !important;
}

.barraQuestoesEstrategicas { <?php echo
	$corFundoBarraQuestoesEstrategicas; ?>

}

.corFundoAcompanhamento {
	background-color: <?php echo$corFundoAcompanhamento;
	?>;
}

.corFundoAcompanhamento:not  (.selecionado  ) div:hover {
	background-color: #248E90 !important;
}

.barraAcompanhamento { <?php echo $corFundoBarraAcompanhamento; ?>

}

.corFundoPendencias {
	background-color: <?php echo$corFundoPendencias;
	?>;
}

.corFundoPendencias:not  (.selecionado  ) div:hover {
	background-color: #AF4A4C !important;
}

.barraPendencias { <?php echo $corFundoBarraPendencias; ?>

}
.barraObras { background-color: #F5C929 !important;

}

.corFundoIndicadoresQualitativos {
	background-color: <?php echo$corFundoIndicadoresQualitativos;?>;
}

.corFundoIndicadoresQualitativos:not  (.selecionado  ) div:hover {
	background-color: #4A4A4A !important;
}

.barraIndicadoresQualitativos { <?php echo
	$corFundoBarraIndicadoresQualitativos; ?>
	width: 100%;
}

.corFundoPlanejamento {
	background-color: <?php echo$corFundoPlanejamento;
	?>;
}

.corFundoPlanejamento:not  (.selecionado  ) div:hover {
	background-color: #4A4A4A !important;
}

.barraPlanejamento { <?php echo $corFundoBarraPlanejamento; ?>
	width: 100%;
}

.corFundoUnidade.selecionado {
	background-color: #1ab394;
}

.corFundoUnidade.selecionado:after,.corFundoUnidade.selecionado .btnBalao:after
	{
	border-color: #1ab394 transparent;
}

.corFundoPNE.selecionado {
	background-color: #1c83c6;
}

.corFundoPNE.selecionado:after,.corFundoPNE.selecionado .btnBalao:after
	{
	border-color: #1c83c6 transparent;
}

.corFundoQuestoesEstrategicas.selecionado {
	background-color: #f8ab59;
}

.corFundoQuestoesEstrategicas.selecionado:after,.corFundoQuestoesEstrategicas.selecionado .btnBalao:after
	{
	border-color: #f8ab59 transparent;
}

.corFundoQuestoesEstrategicas.selecionado .btnBalao:after {
	margin-top: -5px;
}

.corFundoAcompanhamento.selecionado {
	background-color: #22c6c8;
}

.corFundoAcompanhamento.selecionado:after,.corFundoAcompanhamento.selecionado .btnBalao:after
	{
	border-color: #22c6c8 transparent;
}

.corFundoPendencias.selecionado {
	background-color: #DE6466;
}

.corFundoPendencias.selecionado:after,.corFundoPendencias.selecionado .btnBalao:after
	{
	border-color: #DE6466 transparent;
}

.corFundoIndicadoresQualitativos.selecionado {
	background-color: #000000;
}

.corFundoIndicadoresQualitativos.selecionado:after,.corFundoIndicadoresQualitativos.selecionado .btnBalao:after
	{
	border-color: #000000 transparent;
}

.corFundoPlanejamento.selecionado {
	background-color: #A0EFEF;
}

.corFundoPlanejamento.selecionado:after,.corFundoPlanejamento.selecionado .btnBalao:after
	{
	border-color: #A0EFEF transparent;
}


/*Obras*/
.corFundoObras.selecionado {
    background-color: #F5C929;
}

.corFundoObras.selecionado:after,.corFundoObras.selecionado .btnBalao:after
{
    border-color: #F5C929 transparent;
}

.corFundoObras.selecionado {
    background-color: #F5C929;
}

.corFundoObras.selecionado:after,.corFundoObras.selecionado .btnBalao:after
{
    border-color: #F5C929 transparent;
}

/* Emendas Parlamentares */

.corFundoEmendas {
	background-color: <?php echo $corFundoEmendas; ?>;
}

.corFundoEmendas:not  (.selecionado  ) div:hover {
	background-color: #18866F !important;
}

.corFundoEmendas.selecionado {
	background-color: #1ab394;
}

.corFundoEmendas.selecionado:after,.corFundoEmendas.selecionado .btnBalao:after{
	border-color: #1ab394 transparent;
}





.btnRedondo {
	float: left;
	cursor: pointer;
	border-radius: 100% 100% 100% 100%;
	-moz-border-radius: 100% 100% 100% 100%;
	-webkit-border-radius: 100% 100% 100% 100%;
}

.selecionado .btnBalao:after {
	content: "";
	bottom: -15px;
	left: 50px;
	border-width: 15px 15px 0;
	border-style: solid;
	display: block;
	width: 0;
	margin-left: 7px;
	margin-top: 2px;
}

.selecionado:after {
	content: "";
	bottom: -15px;
	left: 50px;
	border-width: 15px 15px 0;
	border-style: solid;
	border-color: #e3e3e3 transparent;
	display: block;
	width: 0;
	margin-left: 10px;
	margin-top: 44px;
}

.btnInterno {
	width: 44px;
	height: 44px;
	margin-top: 3px;
	margin-left: 3px;
	display: inline-block;
	position: absolute;
	z-index: +1 !important;
}

.selecionado {
	-webkit-animation-name: selecionado; /* Chrome, Safari, Opera */
	-webkit-animation-duration: 2.5s; /* Chrome, Safari, Opera */
	animation-iteration-count: infinite;
}

.selecionado:after {
	-webkit-animation-name: selecionado-border; /* Chrome, Safari, Opera */
	-webkit-animation-duration: 2.5s; /* Chrome, Safari, Opera */
	animation-iteration-count: infinite;
}

/* Chrome, Safari, Opera */
@
-webkit-keyframes selecionado {to { background-color:#e7eaec;

}

}

/* Standard syntax */
@
keyframes selecionado {to { background-color:#e7eaec;

}

}

/* Chrome, Safari, Opera */
@
-webkit-keyframes selecionado-border {to { border-color:#e7eaectransparent;

}

}

/* Standard syntax */
@
keyframes selecionado-border {to { border-color:#e7eaectransparent;

}

}
.btnExterno {
	width: 50px;
	height: 50px;
	display: block;
}

.btnInterno span {
	cursor: pointer;
	font-size: 30px;
	color: white;
	margin-top: 15%;
	margin-left: 22.5%;
}

.diagnosticoIcon {
	font-size: 29px !important;
	margin-left: 22% !important;
}

.textIcon {
	cursor: pointer;
	font-family: "Palatino Linotype";
	font-size: 14px;
	color: white;
	margin-top: 27%;
	margin-left: 15%;
}

.characterIcon {
	cursor: pointer;
	font-family: "Palatino Linotype";
	font-size: 30px;
	color: white;
	margin-left: 20%;
	margin-bottom: -6px;
}
/**
 * Barra que liga um balão ao outro
 */
.barraEvolucao {
	height: 5px;
	background-color: #e3e3e3;
	margin-left: 4px;
}

.dscBtn {
	font-size: 11px;
	font-family: "Arial";
	display: inline;
	margin-bottom: 4px;
	margin-left: 4px;
	line-height: 22px;
}

.detalheDscBtn {
	font-size: 9px;
	font-family: "Arial";
	margin-left: 4%;
}

.linhaWizard {
	padding: 5px 16px 15px 16px;
}

.ibox-content-round-gray {
	width: 99%;
	background-color: #f3f3f4;
	border-radius: 10px 10px 10px 10px;
	-moz-border-radius: 10px 10px 10px 10px;
	-webkit-border-radius: 10px 10px 10px 10px;
	border: 0px solid #000000;
}

.ibox-content-round-gray-border {
	width: 99%;
	background-color: #f3f3f4;
	border-radius: 10px 10px 10px 10px;
	-moz-border-radius: 10px 10px 10px 10px;
	-webkit-border-radius: 10px 10px 10px 10px;
	border: 1px solid #cacaca;
}

@media ( max-width : 1600px) {
	.custom-col-10 {
		padding-left: 6px;
	}
}

@media ( max-width : 1515px) {
	.custom-col-10 {
		padding-left: 8px;
	}
	.marcador {
		margin-left: 20px;;
	}
	.linhaWizard {
		margin-bottom: 2px;
	}
}

@media ( min-width : 1200px) and (max-width: 1470px) {
	.i1450 {
		display: none;
	}
	.custom-col-10 {
		padding-left: 10px;
	}
}

@media ( max-width : 1350px) {
	.custom-col-10 {
		padding-left: 13px;
	}
}

@media ( max-width : 1300px) {
	.custom-col-10 {
		padding-left: 17px;
	}
}

@media ( max-width : 1200px) {
	.i1450 {
		display: none;
	}
	.custom-col-10 {
		padding: 5px 0px 0px 3.8%;
	}
	.marcador {
		position: absolute;
		border-radius: 100% 0% 0% 100%;
		-moz-border-radius: 100% 0% 0% 100%;
		-webkit-border-radius: 100% 0% 0% 100%;
		border: 0px;
		width: 50px;
		height: 10px;
		margin-top: 21px;
		margin-left: -1%;
	}
	.barraIndicadoresQualitativos {
		width: 100%;
	}
	.linhaWizard .row {
		/*width: 20.2%;*/
		float: left;
	}
}
/**
 * Cycle Slideshow Configs.
 */
.cycle-nav span:hover {
	color: #5c5c5c;
	opacity: 0.5;
	z-index: 999;
}

.cycle-nav span {
	color: #CCC;
	font-size: 50px;
	opacity: 0.2;
	cursor: pointer;
}

@media ( min-width : 1200px) {
	.linhaWizard.slideshow {
		padding: 0;
		min-height: 69px;
		margin-right: 12px;
		width: 100%;
	}
	.linhaWizard {
		min-width: 100%;
	}
	.cycle-carousel-wrap .cycle-sentinel {
		display: none !important;
	}
	.cycle-carousel-wrap {
		min-width: 100%;
		height: 60px;
		overflow: hidden;
	}
	.cycle-slide {
		height: 60px !important;
		float: none !important;
	}
	.cycle-nav {
		position: absolute;
		margin-top: -75px;
		width: 100%;
		left: 0;
	}
	.cycle-nav .left {
		left: 0;
	}
	.cycle-nav .right {
		right: 0;
	}
	.cycle-carousel-wrap .cycle-slide {
		height: 60px;
	}
	.cycle-nav .up,.cycle-nav .down {
		display: none;
	}
}

@media ( max-width : 1200px) {
	.cycle-slide>div {
		width: 100%;
	}
	.cycle-sentinel.cycle-sentinel {
		height: 200px !important;
	}
	.cycle-carousel-wrap {
		width: 100%;
	}
	.linhaWizard.slideshow {
		min-height: 200px;
	}
	.linhaWizard {
		min-width: 100%;
		padding: 0;
	}
	.cycle-slide {
		width: 100% !important;
		height: 50px !important;
	}
	.cycle-nav {
		height: 200px;
		/* top: 0; */
		margin-top: -200px;
		text-align: center;
	}
	.cycle-nav .up {
		top: -50px;
	}
	.cycle-nav .down {
		top: 180px;
		left: -57px;
	}
	.cycle-nav .left,.cycle-nav .right {
		display: none;
	}
}

@media ( max-width : 768px) {
	.linhaWizard .row {
		width: 100%;
		float: left;
	}
}
</style>
<div class="row" style="padding-bottom: 3px;">
    <div class="col-md-12">
        <div class="col-md-4 text-center"></div>
        <div class="col-md-4 text-center">
            <button class="btn btn-success" id="btn-anterior-nav"><span class="glyphicon glyphicon-chevron-left left nav-anterior"></span></button>
            <button class="btn btn-success" id="btn-proximo-nav"><span class="glyphicon glyphicon-chevron-right left nav-proximo"></span></button>
        </div>
        <div class="col-md-4 text-center"></div>
    </div>
</div>
<div class="row linhaWizard noprint slideshow">
	<div class="col-lg-2 col-xs-12">
		<div class="row">
			<div>
				<div class="marcador corFundoUnidade"
					<?php echo $setaDadosUnidade;?>></div>
				<div id="btnDadosUnidade"
					class="btnRedondo btnExterno corFundoUnidade <?php echo $setaDadosUnidade!='' ? '' : 'selecionado'; ?>">
					<div class="btnRedondo btnBalao btnInterno"
						style="background-color: #1ab394">
						<span class="fa fa-file-text-o url" aria-hidden="true"
							url="<?php echo $urlBtnDadosUnidade;?>"></span>
					</div>
				</div>
			</div>
			<div>
				<label class="dscBtn">Dados da Unidade</label>
				<div class="barraEvolucao barraUnidade"></div>
				<label class="detalheDscBtn" style="color: #1ab394"><?php //echo $testaPreenchimentoUnidade?></label>
			</div>
		</div>
	</div>
	<div class="col-lg-2 col-xs-12">
		<div class="row">
			<div>
				<div class="marcador corFundoPNE" <?php echo $setaPne;?>></div>
				<div id="btnPNE"
					class="btnRedondo btnExterno corFundoPNE <?php echo $setaPne!='' ? '' : 'selecionado'; ?>">
					<div class="btnRedondo btnBalao btnInterno"
						style="background-color: #1c83c6">
						<label class="textIcon url" url="<?php echo $urlBtnPNE;?>"
							msg="<?php echo $msgPNE; ?>">PNE</label>
					</div>
				</div>
			</div>
			<div>
				<label class="dscBtn">Plano Nacional <span class="i1450">de Educação</span></label>
				<div class="barraEvolucao barraPNE"></div>
				<label class="detalheDscBtn" style="color: #1c83c6"><?php //echo $testaPreenchimentoPNE?></label>
			</div>
		</div>
	</div>
	<div class="col-lg-2 col-xs-12">
		<div class="row">
			<div>
				<div class="marcador corFundoQuestoesEstrategicas"
					<?php echo $setaQuestoesEstrategicas;?>></div>
				<div id="btnQuestoesEstrategicas"
					class="btnRedondo btnExterno corFundoQuestoesEstrategicas <?php echo $setaQuestoesEstrategicas!='' ? '' : 'selecionado'; ?>">
					<div class="btnRedondo btnBalao btnInterno"
						style="background-color: #f8ab59">
						<span class="characterIcon url"
							url="<?php echo $urlBtnQuestoesEstrategicas;?>"
							msg="<?php echo $msgQuestoesEstrategicas; ?>">Q</span>
					</div>
				</div>
			</div>
			<div>
				<label class="dscBtn">Questões <span class=i1450>Estratégicas</span></label>
				<div class="barraEvolucao barraQuestoesEstrategicas"></div>
				<label class="detalheDscBtn" style="color: #f8ab59"><?php //echo $testaPreenchimentoQuestoesEstrategicas?></label>
			</div>
		</div>
	</div>

	<div class="col-lg-2 col-xs-12">
		<div class="row">
			<div>
				<div class="marcador corFundoAcompanhamento"
					<?php echo $setaAcompanhamento;?>></div>
				<div id="btnAcompanhamento"
					class="btnRedondo btnExterno corFundoAcompanhamento <?php echo $setaAcompanhamento!='' ? '' : 'selecionado'; ?>">
					<div class="btnRedondo btnBalao btnInterno"
						style="background-color: #22c6c8;">
						<span class="fa fa-bar-chart url" aria-hidden="true"
							style="font-size: 26px; margin: 10px 0px 0px 7px;"
							url="<?php echo $urlBtnAcompanhamento;?>"
							msg="<?php echo $msgAcompanhamento; ?>"></span>
					</div>
				</div>
			</div>
			<div>
				<label class="dscBtn">Execução <span class=i1450>e Acompanhamento</span></label>
				<div class="barraEvolucao barraAcompanhamento"></div>
				<label class="detalheDscBtn" style="color: #22c6c8"><?php //echo $testaPreenchimentoAcompanhamento?></label>
			</div>
		</div>
	</div>
	<div class="col-lg-2 col-xs-12">
		<div class="row">
			<div>
				<div class="marcador corFundoPendencias"
					<?php echo $setaPendencias;?>></div>
				<div id="btnPendencias"
					class="btnRedondo btnExterno corFundoPendencias <?php echo $setaPendencias!='' ? '' : 'selecionado'; ?>">
					<div class="btnRedondo btnBalao btnInterno"
						style="background-color: #DE6466">
						<span class="fa fa-exclamation-triangle url" aria-hidden="true"
							style="font-size: 28px; margin: 14% 0px 0px 18%;"
							url="<?php echo $urlBtnPendencias;?>"
							msg="<?php echo $msgPendencias; ?>"></span>
					</div>
				</div>
			</div>
			<div>
				<label class="dscBtn">Pendências</label>
				<div class="barraEvolucao barraPendencias"></div>
				<label class="detalheDscBtn" style="color: #DE6466"><?php //echo $testaPendencias?></label>
			</div>
		</div>
	</div>
	<div class="col-lg-2 col-xs-12">
		<div class="row">
			<div>
				<div class="marcador corFundoIndicadoresQualitativos"
					<?php echo $setaIndicadoresQualitativos;?>></div>
				<div id="btnIndicadoresQualitativos"
					class="btnRedondo btnExterno corFundoIndicadoresQualitativos <?php echo $setaIndicadoresQualitativos!='' ? '' : 'selecionado'; ?>">
					<div class="btnRedondo btnBalao btnInterno"
						style="background-color: #000000; color: black;">
						<span class="fa fa-pencil-square-o diagnosticoIcon url"
							aria-hidden="true"
							url="<?php echo $urlBtnIndicadoresQualitativos;?>"
							msg="<?php echo $msgIndicadoresQualitativos; ?>"></span>
					</div>
				</div>
			</div>
			<div>
				<label class="dscBtn">Diagnóstico <span class="i1450"> <!--Qualitativo -->
				</span></label>
				<div class="barraEvolucao barraIndicadoresQualitativos"></div>
				<label class="detalheDscBtn" style="color: #DE6466"><?php //echo round($testaPreenchimentoIndicadoresQualitativos,1)?></label>
			</div>
		</div>
	</div>
	<?php
	if(!in_array($mdWorkdlow->esdid, $arrEsdDiag)){
	?>
	<div class="col-lg-2 col-xs-12">
		<div class="row">
			<div>
				<div class="marcador corFundoPlanejamento"
					<?php echo $setaPlanejamento;?>></div>
				<div id="btnPlanejamento"
					class="btnRedondo btnExterno corFundoPlanejamento <?php echo $setaPlanejamento!='' ? '' : 'selecionado'; ?>">
					<div class="btnRedondo btnBalao btnInterno"
						style="background-color: #A0EFEF">
						<span class="fa fa-pencil diagnosticoIcon url" aria-hidden="true"
							url="<?php echo $urlBtnPlanejamento;?>"
							msg="<?php echo $msgPlanejamento; ?>"></span>
					</div>
				</div>
			</div>
			<div>
				<label class="dscBtn">Planejamento <span class="i1450">
				</span></label>
				<div class="barraEvolucao barraPlanejamento"></div>
				<label class="detalheDscBtn" style="color: #A0EFEF"><?php //echo round($testaPreenchimentoIndicadoresQualitativos,1)?></label>
			</div>
		</div>
	</div>
	<?php
	}
	?>
	<div class="col-lg-2 col-xs-12">
		<div class="row">
			<div>
				<div class="marcador corFundoEmendas"
				<?php echo $setaEmendas;?>></div>
				<div id="btnDadosEmendas"
					class="btnRedondo btnExterno corFundoEmendas <?php echo $setaEmendas!='' ? '' : 'selecionado'; ?>">
					<div class="btnRedondo btnBalao btnInterno" style="background-color: #1ab394">
						<span class="fa fa-etsy url" aria-hidden="true" url="<?php echo $urlBtnEmendas;?>"></span>
					</div>
				</div>
			</div>
			<div>
				<label class="dscBtn">Emendas Parlamentares</label>
				<div class="barraEvolucao barraUnidade"></div>
				<label class="detalheDscBtn" style="color: #1ab394"><?php //echo $testaPreenchimentoUnidade?></label>
			</div>
		</div>
	</div>

    <?php
    if(!in_array($mdWorkdlow->esdid, $arrEsdDiag)){
    ?>
    <div class="col-lg-2 col-xs-12">
        <div class="row">
            <div>
                <div class="marcador corFundoObras"
                    <?php echo $setaObras;?>></div>
                <div id="btnIndicadoresQualitativos"
                     class="btnRedondo  btnExterno corFundoObras <?php echo $setaObras!='' ? '' : 'selecionado'; ?>">
                    <div class="btnRedondo btnBalao btnInterno"
                         style="background-color: #F5C929; color: black;">
						<span class="fa fa-building-o url"
                              aria-hidden="true"
                              url="<?php echo $urlBtnObras;?>"
                              msg="<?php echo $msgObras; ?>"></span>
                    </div>
                </div>
            </div>
            <div>
                <label class="dscBtn">Obras <span class="i1450">
				</span></label>
                <div class="barraEvolucao barraObras"></div>
                <label class="detalheDscBtn" style="color: #00e5ff"><?php //echo round($testaPreenchimentoObras,1)?></label>
            </div>
        </div>
    </div>
        <?php
    }
    ?>




</div>
<!--div class="cycle-nav">
	<span class="glyphicon glyphicon-chevron-left left nav-anterior"></span>
	<span class="glyphicon glyphicon-chevron-right right nav-proximo" id="arrow-right-vertical"></span>
	<span class="glyphicon glyphicon-chevron-up up nav-anterior"></span>
    <span class="glyphicon glyphicon-chevron-down down nav-proximo"></span>
</div -->
<?php
$urlFeiraoProgramas = 'par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid='.$_REQUEST['inuid'];
if( $_REQUEST['inuid'] !== ''){
	$urlCalendario = 'par3.php?modulo=principal/evento/calendario&acao=A&inuid='.$_REQUEST['inuid'];
}else{
	$urlCalendario = 'par3.php?modulo=principal/evento/calendario&acao=A';
}
$obModelo = new Par3_Model_InstrumentoUnidade();
?>

<div id="materialButton" class="click-to-toggle fixed-action-btn"
	style="bottom: 45px; right: 24px;">
	<a class="btn_par-floating btn_par-large red"> <i
		class="large material-icons  ">view_headline</i>
	</a>
	<ul>
		<li><a class="btn_par-floating blue-grey atendimento"
			title="Central de Orientações"><i class="material-icons">info_outline</i></a></li>
		<!--       <li><a class="btn_par-floating tourn-btn indigo" title="Tour"><i class="material-icons">language</i></a></li> -->
		<li><a class="btn_par-floating green printMenu" title="Imprimir"
			href="javascript:window.print();"><i class="material-icons">print</i></a></li>
		<li><a class="btn_par-floating amber" title="Calendário"
			href="<?php echo $urlCalendario; ?>"><i class="material-icons">today</i></a></li>
		<li><a class="btn_par-floating blue" title="Programas do MEC"
			href="<?php echo $urlFeiraoProgramas; ?>"><i class="material-icons">view_module</i></a></li>
	</ul>
</div>

<?php require_once("atendimento/tutoriais.php"); ?>
<script src="../library/jquery/jquery.cycle2.min.js"></script>
<script src="../library/jquery/jquery.cycle2.carousel.min.js"></script>
<script>
$(document).ready(function()
{
    $('.url').click(function(){
        var url = $(this).attr('url');
        var msg = $(this).attr('msg');
        if (url != '') {
            $(location).attr('href',url);
        }
    });

    $('.btn_par-large').click(function(){
        if( $('#materialButton').hasClass("active") ){
        	$('.click-to-toggle').closeFAB();
        }else{
        	$('.click-to-toggle').openFAB();
        }
    });

    $('.btnRedondo').on('mouseover', function() {
        $(this).closest('.row').find('.dscBtn').css('color', '#000');
    });

    $('.btnRedondo').on('mouseout', function() {
        $(this).closest('.row').find('.dscBtn').css('color', '#676A6C');
    });

    $(function () {
        $('.atendimento').click(function () {
            $('#modalAtendimento').modal();
        });
    });

	/**
	 * jQuery cycle-slideshow
	 * Menu dinamico
	 */
	var getStartPositionSlide = function () {
			return $('.slideshow div[class*="col-lg"]:not(.cycle-sentinel) .btnRedondo .url').index($('[url=""]'));
		},
		cycleOpts = {
			fx: "carousel",
			timeout: 0,
			carouselVisible: 6,
			carouselVertical: false,
			carouselFluid: true,
			//next: ".cycle-nav .nav-proximo",
			//prev: ".cycle-nav .nav-anterior",
            next: "#btn-proximo-nav",
			prev: "#btn-anterior-nav",
			allowWrap: false,
			slides: '> div', //'div[class*="col-lg"]:not(.cycle-sentinel)'
			log: false,
			autoHeight: true
		},
		slideShow = $('.slideshow');
	slideShow.cycle(cycleOpts);
	slideShow.on('cycle-post-initialize cycle-next cycle-prev', function (event, opts) {
		var cycleWrap = $('.cycle-carousel-wrap'),
			cycleWrapWidth = 0;
		cycleWrap.find(' > div').each(function () {
			cycleWrapWidth += $(this).outerWidth();
		});
		cycleWrap.width(
			cycleWrapWidth > cycleWrap.outerWidth() ? cycleWrapWidth : cycleWrap.outerWidth()
		);
		if (event.type === 'cycle-post-initialize') {
			setTimeout(function () {
				slideShow.cycle('goto', getStartPositionSlide());
			}, 35);
		} else if (event.type === 'cycle-prev') {
			if(opts.currSlide <= opts.carouselVisible && $(window).width() >= 1200) {
				slideShow.cycle('goto', opts.startingSlide);
			}
		}
	});

	var reBuildMenuCycle = function () {
		var browserWidth = $(window).width();
		if (browserWidth < 1200) {
			cycleOpts.carouselVisible = 4;
			cycleOpts.carouselVertical = true;
			cycleOpts.autoHeight = true;
		} else {
			cycleOpts.carouselVisible = 6;
			cycleOpts.carouselVertical = false;
			cycleOpts.autoHeight = false;
		}
		slideShow.cycle('destroy');
		slideShow.cycle(cycleOpts);
	};
	reBuildMenuCycle();
	$(window).on('resize', function (e) {
		reBuildMenuCycle();
	})
});

function js_anterior(){
    var proximo = $('.nav-anterior');
    proximo.trigger('click');
}

function js_proximo(){
    var proximo = $('.nav-proximo');
    proximo.trigger('click');
}

/**
 * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
 * @description Esta função desabilita o btn próximo do menu
 */
function jQueryShowBtnProxReturn(){
    document.getElementById('btn-proximo-nav').removeAttribute('class');
    document.getElementById('btn-proximo-nav').setAttribute('class','btn btn-success disabled');
}

/**
 * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
 * @description Esta função desabilita o btn próximo do menu
 */
function jQueryShowBtnAnteriorReturn(){
    document.getElementById('btn-proximo-nav').removeAttribute('class');
    document.getElementById('btn-proximo-nav').setAttribute('class','btn btn-success disabled');
}
</script>