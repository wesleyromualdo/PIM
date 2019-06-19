<?php
define ( 'CLASSES_GERAL'                , APPRAIZ . "demandas/classes/" );
define ( 'CLASSES_CONTROLE'             , APPRAIZ . 'demandas/classes/controller/' );
define ( 'CLASSES_HELPER'               , APPRAIZ . 'demandas/classes/helper/' );
define ( 'CLASSES_MODELO'               , APPRAIZ . 'demandas/model/' );
define ( 'CLASSES_MODELO_CORPORATIVO'   , APPRAIZ . 'corporativo/model/' );
define ( 'CLASSES_WS'                   , APPRAIZ . 'demandas/classes/ws/' );
define ( 'CLASSES_VISAO'                , APPRAIZ . 'includes/classes/view/' );
define ( 'CLASSES_HTML'                 , APPRAIZ . 'includes/classes/html/' );

set_include_path ( CLASSES_GERAL . PATH_SEPARATOR . 
				   CLASSES_CONTROLE . PATH_SEPARATOR . 
				   CLASSES_MODELO . PATH_SEPARATOR . 
                   CLASSES_MODELO_CORPORATIVO . PATH_SEPARATOR . 
				   CLASSES_VISAO . PATH_SEPARATOR .
				   CLASSES_WS . PATH_SEPARATOR .
				   CLASSES_HELPER . PATH_SEPARATOR .
				   CLASSES_HTML . PATH_SEPARATOR . get_include_path () );
$__arAutoload = [function(){},];
function __autoload($class) {
	if (PHP_OS != "WINNT") { // Se "não for Windows"
		$separaDiretorio = ":";
		$include_path = get_include_path ();
		$include_path_tokens = explode ( $separaDiretorio, $include_path );
	} else { // Se for Windows
		$raiz = strtolower ( substr ( APPRAIZ, 0, 2 ) );
		$separaDiretorio = ";$raiz";
		$include_path = get_include_path ();
		$include_path = str_replace ( '.;', $raiz, strtolower ( $include_path ) );
		$include_path = str_replace ( '/', '\\', $include_path );
		$include_path_tokens = explode ( $separaDiretorio, $include_path );
		$include_path_tokens = str_replace ( "//", "/", $include_path_tokens );
		$include_path_tokens [0] = str_replace ( $raiz, '', $include_path_tokens [0] );
	}  
	foreach ( $include_path_tokens as $prefix ) {
        // Recupera a última posição do array, substituindo o array_pop para parar o erro de parâmetro por referência
        $aClasse = explode('_', $class);
        
		$file = $aClasse[(count($aClasse)-2)];

		$path[0] = strtolower($prefix) . $file . '.class.inc';
		$path[1] = strtolower($prefix) . $file . '.php';
		$path[2] = strtolower($prefix) . $class . '.class.inc';
		$path[3] = strtolower($prefix) . $class . '.php';
		
		foreach ( $path as $thisPath ) {
			if (file_exists ( $thisPath )) {
				require_once $thisPath;
				return;
			}
		}
	}
    global $__arAutoload;
    foreach ($__arAutoload as $fn){
        if($fn($class)){
            return;
        }
    }
}?>