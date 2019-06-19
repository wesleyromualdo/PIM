<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

//Chamada de programa
//include  APPRAIZ."includes/cabecalho.inc";

// altera o ano de exercício (caso o usuário solicite)
if ( $_REQUEST['exercicio'] AND $_SESSION['exercicio'] != $_REQUEST['exercicio'] ) {
	$_SESSION['exercicio'] = $_REQUEST['exercicio'];  
	$exercicio = $_SESSION['exercicio'];
	$sql = sprintf(
		"SELECT prsexercicioaberto FROM %s.programacaoexercicio WHERE prsano = '%s'",
		$_SESSION['sisdiretorio'],
		$_REQUEST['exercicio']
	);
	$_SESSION['exercicioaberto'] = $db->pegaUm( $sql );
}

$usucpf = $_SESSION['usucpf'];
$pflcod = pegaPerfil($usucpf);

unset($_SESSION["emenda"]);

/*if( $_SESSION['exercicio'] >= '2012' ){
	header( "Location: ../muda_sistema.php?sisid=128" );
} else {*/
	if($db->testa_superuser()){
		//header( "Location: ?modulo=principal/listaPlanoTrabalho&acao=A" );
		header( "Location: ?modulo=principal/listaDeEmendas&acao=A" );
	}else{
		if($pflcod == SUPER_USUARIO || $pflcod == ADMINISTRADOR_INST){
			//header( "Location: ?modulo=principal/listaPlanoTrabalho&acao=A" );
			header( "Location: ?modulo=principal/listaDeEmendas&acao=A" );
		} elseif ($pflcod == INSTITUICAO_BENEFICIADA){
			header( "Location: ?modulo=principal/listaInstituicoesEntBen&acao=A&retorno=listaPlanoTrabalho" );
		} elseif( $pflcod == ADMINISTRADOR_REFORMULACAO ){
			header( "Location: ?modulo=principal/listaPlanoTrabalho&acao=A" );
		} else {
			header( "Location: ?modulo=principal/listaDeEmendas&acao=A" );
		}
	}
//}

?>
<br>
<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr bgcolor="#e7e7e7">
	  <td><h1>Bem-vindo</h1></td>
	</tr>
</table>
