<?php
/**
 * Rotina que controla o acesso às páginas do módulo.
 *
 * Carrega as bibliotecas padrões do sistema e executa tarefas de inicialização.
 *
 * @package Simec\Core
 * @author Cristiano Cabral <cristiano.cabral@gmail.com>
 * @since 25/08/2008
 */

date_default_timezone_set ('America/Sao_Paulo');

/**
 * Obtém o tempo com precisão de microsegundos. Essa informação é utilizada para
 * calcular o tempo de execução da página.
 *
 * @return float
 * @see /includes/rodape.inc
 */
function getmicrotime(){
    list( $usec, $sec ) = explode( ' ', microtime() );
    return (float) $usec + (float) $sec;
}

// obtém o tempo inicial da execução
$Tinicio = getmicrotime();

// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . 'includes/library/simec/view/Helper.php';

initAutoload();
require_once APPRAIZ .'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";

$db = new cls_banco();
$simec = new Simec_View_Helper();
$_SESSION['sislayoutbootstrap'] = 'zimec';

?>