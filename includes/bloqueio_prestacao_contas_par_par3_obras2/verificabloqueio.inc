<?php
$sistema_bloqueio = $_SESSION['sisid']; 
// 147 Obras 2
// 23 Par 2
// 231 PAR 3
if(($sistema_bloqueio == 147) || ($sistema_bloqueio == 23) || ($sistema_bloqueio == 231)){ 
    // inclui o constante
    
    include_once APPRAIZ . "par/modulos/principal/administracaoCienciaOmissao.inc";
    
    include_once APPRAIZ . "par/modulos/principal/administracaoCienciaOmissaoCACS.inc";
   // die('deu certo');
}

