<?php 

$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];


switch ( $_REQUEST['requisicao'] ){
	case 'apagar': 
		$regAtividade = new RegistroAtividade( $_REQUEST['rgaid'] );
            
                if($_REQUEST['rgaid'] && $regAtividade->rgaautomatica == 'f'  && possuiPerfil(PFLCOD_SUPER_USUARIO)){
                    $regAtividade->rgastatus = 'I';		
                    $regAtividade->salvar();

                    $db->commit();
                    die("<script>
                            alert('Operação realizada com sucesso!'); 
                            window.location = '?modulo=principal/listaRegistroAtividade&acao={$_GET['acao']}';
                         </script>");
                }else{
                    die("<script>
                            alert('Você não possui permissão para excluir esse Registro!!'); 
                            window.location = '?modulo=principal/listaRegistroAtividade&acao={$_GET['acao']}';
                         </script>");
                }

    case 'tornarimportante':


        $regAtividade = new RegistroAtividade( $_REQUEST['rgaid'] );

        $regAtividade->rgaimp = 't';
        $regAtividade->alterar($dados['rgaimp']);
        $regAtividade->salvar();
        break;

    case 'retirarimportancia':
        $regAtividade = new RegistroAtividade( $_REQUEST['rgaid'] );

        $regAtividade->rgaimp = 'f';
        $regAtividade->alterar($dados['rgaimp']);
        $regAtividade->salvar();
        break;
	case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die();



}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";


$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);

echo cabecalhoObra($obrid);

//monta_titulo($titulo_modulo, '');
monta_titulo_obras($titulo_modulo, '');

extract( $_POST );

$habilitado = true;
$habilita 	= 'S';
if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
	$habilitado = false;
	$habilita 	= 'N';
}
