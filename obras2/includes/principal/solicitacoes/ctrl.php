<?php 
require APPRAIZ . 'obras2/includes/principal/solicitacoes/funcoes.php';

$usucpf = $_SESSION['usucpf'];
$perfis = pegaPerfilGeral( $usucpf );

if($_REQUEST['requisicao'] && $_REQUEST['requisicao'] == 'aprova_solicitacao'){
    global $db;
    $slcid = $_POST['slcid'];

    try {
        $retorno = posAcaoObraSolicitacao(null,$slcid);
        $mensagem = 'Solicitação atualizada com sucesso!';
        $query = <<<DML
            UPDATE obras2.solicitacao SET aprovado = 'S' , slccpfaprovado = '{$usucpf}'  WHERE slcid = $slcid

DML;
        $db->executar($query);
        $db->commit();
    } catch(Exception $e) {
        $log = $e->getMessage();
        $retorno = false;
        $mensagem = 'Falha no disparo de email.';
    }
    echo simec_json_encode(array('retorno' => $retorno, 'mensagem' => $mensagem, 'log' => $log));
    die();
}

if($_REQUEST['requisicao'] && $_REQUEST['requisicao'] == 'reprova_solicitacao'){
    global $db;
    $slcid = $_POST['slcid'];

    try {

        $sql = "select * from obras2.solicitacao where slcid = $slcid";
        $dados = $db->pegaLinha($sql);


        $sql = "select * from workflow.documento where docid = $dados[docid]";
        $dados1 = $db->pegaLinha($sql);

        if($dados1['esdid'] == ESDID_SOLICITACOES_DEFERIDO){

            $acao = 3845;
        }
        if($dados1['esdid'] == ESDID_SOLICITACOES_INDEFERIDO){

            $acao = 3846;
        }

        wf_alterarEstado($dados['docid'],$acao,"Pedido reprovado",array());
        $mensagem = 'Solicitação atualizada com sucesso!';
        $query ="UPDATE obras2.solicitacao SET reprovado = 'S' WHERE slcid = $slcid";
        $db->executar($query);
        $db->commit();
    } catch(Exception $e) {
        $log = $e->getMessage();
        $retorno = false;
        $mensagem = 'Falha no disparo de email.';
    }
    echo simec_json_encode(array('retorno' => $retorno, 'mensagem' => $mensagem, 'log' => $log));
    die();
}

verificaSessao('orgao');
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int) ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_GESTOR_UNIDADE);
$obra = new Obras();
$obra->carregarPorIdCache($_SESSION['obras2']['obrid']);
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];

$habilitado = false;
$habilitado_super = false;
if (possui_perfil($pflcods)) {
    $habilitado = true;
    if(possui_perfil(array(PFLCOD_SUPER_USUARIO))){
        $habilitado_super = true;
    }
}

if( $_GET['acao'] != 'V' ){
	// Inclusão de arquivos padrão do sistema
	include APPRAIZ . 'includes/cabecalho.inc';
	// Cria as abas do módulo
	echo '<br>';
	if($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ) {
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros,array());
	} else {
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros,array());
	}
}
print cabecalhoObra($obrid);
monta_titulo_obras('Solicitações', $linha2);
