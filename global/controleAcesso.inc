<?php
// carrega a página solicitada pelo usuário
$sql = sprintf("select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '%s'", $_SESSION['usucpf']);

if (isset($_SESSION['chave'])) {
    $chave = $_SESSION['chave'];
} else {
    $chave = $db->pegaUm($sql);
    $_SESSION['chave'] = $chave;
}


if (empty($_SESSION['sisdiretorio'])) {
    $sisdiretorio = explode("/", $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI'])[1];
} else {
    $sisdiretorio = $_SESSION['sisdiretorio'];
}
if ($chave == 'f') {
    // leva o usuário para o formulário de troca de senha
    //if ($_SESSION['sisid'] == 147) { //obras 2
    if ($_SESSION['sisid'] == 147 || $sisdiretorio == 'obras2') {
        include APPRAIZ . $sisdiretorio . "/modulos/sistema2/usuario2/altsenha.inc";
    } else {
        include APPRAIZ . $sisdiretorio . "/modulos/sistema/usuario/altsenha.inc";
    }
    include APPRAIZ . "includes/rodape.inc";
} else if ($_REQUEST['modulo']) {
    // leva o usuário para a página solicitada, se ela existir
    $file = realpath(APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc");

    if (file_exists($file)) {
        include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc";
        include APPRAIZ . "includes/rodape.inc";
    } else {
        if($_POST['carregarUsuariosSistema']){
            include APPRAIZ . "includes/cabecalho.inc";
        }else{
            include_once APPRAIZ . "includes/classes/mvc/Roteador.class.inc";
            $Roteador = new mvc\Roteador();
            if($Roteador->rotaPossivel()){
                $Roteador->incluirRota();
            }else{
                include_once APPRAIZ . "src/Simec/Config.php";
                (new \Simec\Config())->autoload();
                if(!class_exists('\\Simec\\Simec') || !(new \Simec\Simec())->rodar()){
                    header("Location: ../erro404.php");
                }

            }
        }
    }
} else {
    var_dump('asdfasdf');exit;
    header("Location: ../login.php");
}