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


// carrega as funções gerais
include_once "config.inc";
include "verificasistema.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/connection/adapter-connection.php";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// -- Simulação de usuários para o layout cinza. Ex: Obras2
if ($_POST['usucpf_simu']
    && ($_SESSION['superuser']
        || $db->testa_uma()
        || $_SESSION['usuuma'])
) {
    unset($_SESSION['testa_superuser']);
    unset($_SESSION['acl']);

    $_SESSION['usucpf'] = $_POST['usucpf_simu'];
}

/**
 * Fecha todas as conexões com base de dados.
 *
 * @see Simec\Core\Db\cls_banco
 * @see Simec\Db\connection
 */
function fechaDb()
{
    global $db;
    $db->close();
    connection::getInstance()->close();
}

simec_magic_quotes();
register_shutdown_function('fechaDb');

include_once APPRAIZ .'includes/library/simec/AcessoRapido/core/Controlador.php';
AcessoRapido\Controlador::pegarInstancia();

//Verifica permissões de acesso ao módulo requisitado via URL
include APPRAIZ . 'includes/testa_acesso.inc';
