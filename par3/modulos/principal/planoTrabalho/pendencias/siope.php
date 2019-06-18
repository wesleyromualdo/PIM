<?php
$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$helperPendencia = new Par3_Helper_PendenciasEntidade();
$instrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);

$pendenciaBox = $_REQUEST['pendencia'];
//$boExists = $this->consultaHabilitaEntidade( $_REQUEST['inuid'] );

$oSiope = new Par3_Model_Siope();

//Trecho de cÃ³digo comentado para retirar as validaÃ§Ãµes de pendencias antigas do siope e considerar apenas a situaÃ§Ã£o enviada pela carga
//historia 10818 link: https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=10818
//$cumprimento = $oSiope->listarCumprimentos($_REQUEST);
//$cumprimento = is_array($cumprimento) ? $cumprimento : array();
//$boExists = $instrumentoUnidade->verificaMunicipioSIOP($_REQUEST['inuid']);
//
//$arrSiope = array();
//foreach ($cumprimento as $v) {
//	if( $v['an_exercicio_mde'] == '2015' || $v['an_exercicio_mde'] == '2016'){
//		if( $v['vl_perc_aplic_med'] < 25 || $v['vl_apl_rem__prof_mag_rpm'] < 60 ){
//			$boExists = true;
//		}
//	}
//}

$resultado = $oSiope->transmissaoSiope($_REQUEST);
$transmissao = $resultado['0']['cod_situacao'];
$msgPendenciaSiope = $resultado['0']['situacao_desc'];

if($transmissao == 2){
    $boExists = true;
}

$arAuxHelperPendencia = $helperPendencia->controleWidgetsPendenciaPrincipal($pendenciaBox, $boExists, $msgPendenciaSiope);
$arAuxHelperPendencia['nomeEntidade'] = $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']);

echo simec_json_encode($arAuxHelperPendencia);
die();
?>