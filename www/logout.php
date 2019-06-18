<?php
	include "config.inc";

	$sisid = $_SESSION['sisid'];
    $aviso = isset($_SESSION['MSG_AVISO']) ? $_SESSION['MSG_AVISO'] : null;

	if (is_array($_SESSION)) {
		foreach ($_SESSION as $index => $session) {
			unset($_SESSION[$index]);
		}
	}

	$_SESSION = array();
    $_SESSION['MSG_AVISO'] = $aviso;

    header('Location: /login.php');
    
	exit();
?>