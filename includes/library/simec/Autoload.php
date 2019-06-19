<?php

define('APPRAIZ_ZEND', APPRAIZ . 'includes/library/Zend/');
define('CLASSES_GERAL', APPRAIZ . "includes/classes/");
define('CLASSES_CONTROLE', APPRAIZ . $_SESSION['sisdiretorio'] . '/classes/controllers/');
define('CLASSES_MODELO', APPRAIZ . $_SESSION['sisdiretorio'] . '/classes/models/');
define('CLASSES_VISAO', APPRAIZ . $_SESSION['sisdiretorio'] . '/classes/views/');


set_include_path(
        CLASSES_GERAL . PATH_SEPARATOR .
        CLASSES_CONTROLE . PATH_SEPARATOR .
        CLASSES_MODELO . PATH_SEPARATOR .
        CLASSES_VISAO . PATH_SEPARATOR .
        get_include_path()
);

function __autoload($class) {

    require_once APPRAIZ . "includes/library/simec/funcoes.inc";
    require_once APPRAIZ . "includes/library/simec/abstract/Controller.php";
    require_once APPRAIZ . "includes/library/simec/abstract/Model.php";


    if (PHP_OS != "WINNT") { // Se "não for Windows"
        $separaDiretorio = ":";
        $include_path = get_include_path();
        $include_path_tokens = explode($separaDiretorio, $include_path);
    } else { // Se for Windows
        $separaDiretorio = ";c:";
        $include_path = get_include_path();
        $include_path = str_replace('.;', 'c:', strtolower($include_path));
        $include_path = str_replace('/', '\\', $include_path);
        $include_path_tokens = explode($separaDiretorio, $include_path);
        $include_path_tokens = str_replace("//", "/", $include_path_tokens);
        $include_path_tokens[0] = str_replace('c:', '', $include_path_tokens[0]);
    }

    foreach ($include_path_tokens as $prefix) {
		$arrayClass = explode('_', $class);
        $file = array_pop($arrayClass);
        $pathModule = $prefix . $file . '.php';
        if (file_exists($pathModule)) {
            require_once $pathModule;
        }
        $path[0] = $prefix . $class . '.class.inc';
        $path[1] = $prefix . $class . '.php';
        foreach ($path as $thisPath) {
            if (file_exists($thisPath)) {
                require_once $thisPath;
                return;
            }
        }
    }
}

urlAction();

/**
 * Metodo responsavel por pegar a action da controller e renderizar na tela.
 *
 * @name urlAction
 * @return void
 *
 * @throws Exception Controller não foi informada!
 * @throws Exception Action não foi informada!
 * @throws Exception Controller não existe!
 * @throws Exception Action não existe!
 *
 * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
 * @since 10/06/2013
 */
function urlAction() {
    // Se tiver o nome da controller e o nome da action,
    // Significa que e uma requisicao.
    if (isset($_POST['controller']) && isset($_POST['action'])) {
        header("Content-Type: text/html; charset=ISO-8859-1");
        // Encapsulando dados post
        $nameController = 'Controller_' . ucfirst($_POST['controller']);
        $nameAction = $_POST['action'] . 'Action';

        // Estanciando class Controller e exibindo na tela a action.
        $controller = new $nameController;
        echo $controller->$nameAction();

        // -- Caso a página requisitada seja uma página existente, realiza o log de estatísticas - Verificação necessária pois
        // -- ainda não foi possível reproduzir o erro no sistema financeiro que faz com que todas as imagens do tema do sistema
        // -- sejam requeridas pelo browers como um módulo. Esta mesma verificação é feita no controleAcesso no momento de
        // -- incluir os arquivos.
        if (file_exists(realpath(APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc"))) {
            simec_gravar_estatistica();
        }
        exit;
    }
}
