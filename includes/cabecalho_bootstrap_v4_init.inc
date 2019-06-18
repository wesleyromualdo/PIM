<?php  
// Verifica se a sessao expirou.
if( ! isset($_SESSION['usucpf']) )
{
    session_unset();
    $_SESSION['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
    header('Location: login.php');
    exit();
}



/*if ($_POST['carregarUsuariosSistema'])
{
    $usuariosSistema = carregarUsuariosSistema(true);

    $listing = new Listing();
    $listing->setTypePage('M');
    $listing->setActions(array('eye-open' => 'simularUsuariosSistemas'));
    $listing->setHead(array('CPF', 'Nome', 'Email'));
    $listing->listing($usuariosSistema);
    exit;
}
*/
if ($_POST['simularUsuariosSistema'])
{
    $cpf = $_POST['cpf'];

    global $db;

    $sql = "select distinct
                u.usucpf,
                u.usunome,
                u.usuemail
                --u.susdataultacesso,
                --u.usuacesso,
                from seguranca.usuario u
                inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
                inner join seguranca.perfil p on p.pflcod = pf.pflcod and   p.sisid = " . $_SESSION['sisid'] . "
                inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and us.sisid = p.sisid
                where us.suscod = 'A' and p.pflnivel >= (
                    select min(pflnivel) from seguranca.perfil
                    inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod
                    and perfil.sisid = " . $_SESSION['sisid'] . " )
                    and u.usucpf = '$cpf';";

    $usuario = $db->pegaLinha($sql);

    if ($usuario) {
        $_SESSION['usucpf'] = $usuario['usucpf'];
        $_SESSION['usunome'] = $usuario['usunome'];
        $_SESSION['usuemail'] = $usuario['usuemail'];
        unset($_SESSION['testa_superuser']);
        unset($_SESSION['testa_superuser_cache']);
        unset($_SESSION['acl']);
        unset($_SESSION['sistemas']);
        echo 'ok';
    }

    exit;
}
 

$sistemas = carregarSistemas();
$menus = carregarMenus($_SESSION['sisid'], $_SESSION['usucpf']);
$menuHtml = montaMenuBootstrap($menus);



if (!isset($_COOKIE['navbar'])) {
    $_COOKIE['navbar'] = 'false';
}
$navBar = $_COOKIE['navbar']=='false' ? 'mini-navbar' : null;

