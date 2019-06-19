<?php 
	include "C:/xampp/htdocs/ProjetoGit/simec/seguranca/modulos/principal/hcpconnect.php";
	$hcp = new hcpconnect(); 

	
	$recebe = $hcp -> PUT('PASTA/01/xz005.png','C:\xampp\htdocs\ProjetoGit\simec\www\imagens\0_alerta.png');
	var_dump($recebe);
?>

teste oi
