<?php
define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "www/par3/_funcoes.php";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/simec_funcoes.inc";
include_once APPRAIZ . "par3/classes/controller/Processo.class.inc";
include_once APPRAIZ . "par3/classes/model/Processo.class.inc";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '';
$_SESSION['usucpf'] 		= '';

$db = new cls_banco();
//57836
$arrPost = array('inpid' => $_REQUEST['inpid'],
				'iniano' => $_REQUEST['iniano'],
				'inuid' => $_REQUEST['inuid'],
				'ws_usuario' => 'juliov',
				'ws_senha' => '1oucinco'
				);

$obProcesso = new Par3_Controller_Processo();
$obProcesso->gerarProcesoFNDE($arrPost);

//http://dsv-simec.mec.gov.br/par3/testa_processo.php 
//05646593638