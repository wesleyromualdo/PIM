<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
/**
 * Autoload de classes dos Módulos SPO.
 * @see autoload.php
 */
require_once APPRAIZ . 'spo/autoload.php';

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';


#$perfis = pegaPerfilGeral();
$perfis = is_array(pegaPerfilGeral()) ? pegaPerfilGeral() : array() ;


/* Controle de exibição do de Simular Usuário */
if (in_array(PFL_SUPER_USUARIO, $perfis)
    || in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)
    || in_array(PFL_ASPAR, $perfis)
    ) {
    $exibirSimular = true;
}
// -- Export de XLS automático da Listagem
Simec_Listagem::iniciarMonitoramento($_SESSION['sisdiretorio']);

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
