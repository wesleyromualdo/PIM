<?php
ini_set("memory_limit", "2024M");
set_time_limit(0);

//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Visao.class.inc";
include_once APPRAIZ . "includes/library/simec/Listagem.php";


// carrega as funções específicas do módulo
include_once '../par/_constantes.php';
include_once '../par/_funcoes.php';
include_once '../par/_funcoesPar.php';
include_once APPRAIZ .'/par/classes/modelo/PreObra.class.inc';
include_once APPRAIZ .'/par/classes/modelo/PreTipoDocumento.class.inc';
include_once APPRAIZ .'/par/classes/modelo/PreObraAnalise.class.inc';
include_once APPRAIZ .'/par/classes/controle/PreObraControle.class.inc';
include_once APPRAIZ .'/par/classes/controle/SubacaoControle.class.inc';
include_once '../obras2/_funcoes_obras_par.php';
//include_once '../par/autoload.php';

simec_magic_quotes();
/* if( gettype( $db->link ) != 'resource' ){
	$db = new cls_banco(); // corrigindo a perda do link do $db
} */


$arrPerfil = pegaPerfilGeral();
$arrPerfil = $arrPerfil ? $arrPerfil : array();

$boVisualizaTudo = false;
if(in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrPerfil)){
    $boVisualizaTudo = true;
}

// Painel do Administrador e Super Usuário
if ($boVisualizaTudo || $db->testa_superuser()) {
    $painelCabecalho = array(
        array('titulo' => 'WorkFlow', 'funcao' => 'montarPainelWorkflow', 'icon' => 'tasks'),
    );
}

// Painel do perfil CONSULTA GERAL
if($boVisualizaTudo || in_array(PAR_PERFIL_CONSULTA, $arrPerfil)){
    $painelCabecalho[] = array('titulo' => 'Base Nacional Comum', 'funcao' => 'montarPainelBaseNacional', 'icon' => 'tasks');
}


/**
* @TODO Tratamento para colocar o layout antigo nas telas de sistemas que não tem o jquery compativel ainda com o layout novo
*/
$arrModulo = explode( '/', $_GET['modulo']);
//$modulo = reset($arrModulo);
$modulo = $arrModulo[1];
if(!empty($modulo) && $modulo == 'painel'){
   $_SESSION['sislayoutbootstrap'] = true;
} else {
   $_SESSION['sislayoutbootstrap'] = false;
}

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";

prepararDetalheProcesso();
prepararDetalhePendenciasObras();
prepararDetalheFuncionalProgramatica();
verificaPendenciasDemandas();

include APPRAIZ .'par/modulos/principal/programas/proinfancia/visualizarPreObra.inc';
        //&acao=A&preid={$dados['preid']}"';

?>