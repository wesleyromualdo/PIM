<?php
include_once( APPRAIZ. "emenda/classes/ImportacaoHabilitaFNDE.class.inc" );


// pega o endereço do diretório atual
//getcwd();

//K_TIPO_ENTIDADE, K_TIPO_DOCUMENTO, K_TIPO_DILIGENCIA
$arTipos = array( K_TIPO_ENTIDADE, K_TIPO_DOCUMENTO, K_TIPO_DILIGENCIA );

//importacaoHabilitaFNDE::start($diretorio, $arTipos);
$importaFNDE = new importacaoHabilitaFNDE(DIRETORIO_ORIGEM_HABILITA, $arTipos);
?>