<?php

function __autoload($class_name) {
	$arCaminho = array(
						APPRAIZ . "includes/classes/",
						APPRAIZ . "includes/classes/modelo/entidade/",
						APPRAIZ . "obras2/classes/modelo/",
						APPRAIZ . "obras2/classes/modelo/demanda/",
                        APPRAIZ . "obras2/classes/modelo/alerta/",
                        APPRAIZ . "obras2/classes/modelo/supervisao2/",
						APPRAIZ . "includes/classes/modelo/territorios/",
						APPRAIZ . "includes/classes/modelo/public/"
					  );
					  
	foreach($arCaminho as $caminho){
		$arquivo = $caminho . $class_name . '.class.inc';
		if ( file_exists( $arquivo ) ){
			require_once( $arquivo );
			break;	
		}
	}				  
}

