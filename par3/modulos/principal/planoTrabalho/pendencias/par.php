<?php

$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$helperPendencia = new Par3_Helper_PendenciasEntidade();
$pendenciaBox    = $_REQUEST['pendencia'];
//restricao
$modelPendencias = new Par3_Model_Pendencias();
$arrPendencias        = $helperPendencia->recuperarPendenciasEntidade($_REQUEST['inuid']);
$arrTiposPendencia    = $helperPendencia->recuperarTiposPendencia();
$arAuxHelperPendencia = $helperPendencia->controleWidgetsPendenciaPrincipal($pendenciaBox, false);
$arAuxHelperPendencia['nomeEntidade'] = $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']);

$modelRestricao = new Par3_Model_Restricao();
$existeRestricao = $modelRestricao->existeRestricaoInstrumentoUnidadeById($_REQUEST['inuid']);
$arAuxHelperRestricao = array();
$htmlResponse = '';
$responseComplete = '';
//adicionado o $existeRestricao por valida se existe pendencias ou restrições
if( is_array( $arrPendencias ) && count($arrPendencias) > 0 || $existeRestricao == true) {
    $mun        = new Par3_Model_Municipio();
    $pendencias = array();
    $arAuxHelperPendencia = $helperPendencia->controleWidgetsPendenciaPrincipal($pendenciaBox, true);

    if(count($arrPendencias) > 0){ //se existe pendências
        foreach ( $arrPendencias[$arAuxHelperPendencia['type']] as $estado => $tipoPendencia ) {
            foreach ( $tipoPendencia as $municipio => $tipoPendencia2 ) {
                $arAuxHelperPendencia['municipios'][($mun->descricaoMunicipio($municipio))] = array(
                    'pendencias' => array_values($tipoPendencia2)
                );
            }
        }

       if(is_array($tipoPendencia2) && count($tipoPendencia2) > 0){
            $htmlResponse .= $helperPendencia->montaPendenciasObras(array_values($tipoPendencia2));
       }
    }
    if($existeRestricao ==  true){
        $arrayRestricoes = $modelRestricao->getObrasRestricoesInstrumentoUnidadeById($_REQUEST['inuid']);
        $htmlResponse .= $helperPendencia->montaRestricoesInstrumentoUnidade($arrayRestricoes);

    }
    $responseComplete = $helperPendencia->montaPendeciasRestricoesExiste($htmlResponse);
}else{
    $responseComplete = $helperPendencia->montaSemPendenciaSemRestricao($arAuxHelperPendencia['nomeEntidade']);
    }
echo $responseComplete;
//echo simec_json_encode($arAuxHelperPendencia);
die();
?>