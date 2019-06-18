<?php /* ****arquivo vazio**** */

require APPRAIZ . 'obras2/includes/principal/vistoria/funcoes.php';


$orgid = $_SESSION['obras2']['orgid'];
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int) ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];


unset($_SESSION['supid']);

// Verifica se existe restrição gerada pela supervisão da empresa
$sup = new SupervisaoEmpresa();
$sup->verificaDivergenciaSituacaoRestricao($_SESSION['obras2']['obrid']);

$habilitaMods = false;

require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

switch ($_POST['op']) {
    case 'alterar':
        $_SESSION['obras2']['smiid'] = $_POST['smiid'];
        header('Location: ?modulo=principal/cadSupervisaoMI&acao=E');
        die;
    case 'apagar':
        $supervisaoMi = new SupervisaoMi( $_POST['smiid'] );
        $supervisaoMi->smistatus = 'I';
        $supervisaoMi->salvar();
        $db->commit();
}


if(isset($_REQUEST['supid']) || isset($_POST['sueid']) ){

    // Caso Supervisão Instituição
    if(isset($_REQUEST['supid'])){
        $vistoria = new Supervisao();
        $obrid = $_SESSION['obras2']['obrid'];
        $supid = $_REQUEST['supid'];

        $dadosVistoria = $vistoria->getSupervisao($obrid, $supid);
        $dataCalc      = isset($dadosVistoria[0]["dtinclusao"]) ? $dadosVistoria[0]["dtinclusao"] : date('Y-m-d');

        $dadosValidade = $vistoria->getValidadeSupUnidade($obrid, $supid);
        $validado      = (strtolower($dadosValidade[0]["validadaPeloSupervisorUnidade"]) == 's') ? true : false;
    }
    // Caso Supervisão Empresa
    elseif(isset($_POST['sueid'])){
        $vistoria = new SupervisaoEmpresa();
        $obrid    = $_SESSION['obras2']['obrid'];
        $sueid    = $_POST['sueid'];

        $dadosVistoria = $vistoria->listaDados(array('sueid'=>$sueid));
        $dataCalc      = isset($dadosVistoria[0]["suedtcadastro"]) ? $dadosVistoria[0]["suedtcadastro"] : date('Y-m-d');
        $validado      = (strtolower($dadosVistoria[0]["esddsc"]) == 'homologado') ? true : false;
    }

    //RN - Não permitir a edição/exclusão de vistorias pelo supervisor unidade ou gestor unidade 7 dias após a data de inclusão.
    $dataInicial  = explode('/',$dataCalc);
    $dataInicial  = $dataInicial[2].'-'.$dataInicial[1].'-'.$dataInicial[0];
    $dataFinal    = date('Y-m-d');
    $diffDatas    = ((((strtotime($dataFinal) - strtotime($dataInicial))/60)/60)/24);
    $habilitaMods = ( ($diffDatas >= 7) && (possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE)) && ($validado == true) ) ? false : true;

}

// para post do formulario empresa
switch ( $_POST['operacao'] ){
    case 'apagarSupervisaoEmpresa':
        if($habilitaMods){
            $supervisaoEmpresa = new SupervisaoEmpresa();
            $supervisaoEmpresa->excluir( $_POST['sueid'] );
            $db->commit();
            die("<script>
                    alert('Operação realizada com sucesso!');
                    window.location = '?modulo=principal/listVistoriaEmp&acao=A';
                 </script>");
        }else{
            die("<script>
                     alert('Você não possui permissão para excluir esse Acompanhamento!');
                     history.back(-1);
                  </script>");
        }
    case 'editarSupervisaoEmpresa':

        if($habilitaMods){
            $_SESSION['obras2']['sueid'] = $_POST['sueid'];
            die("<script>
                    window.location = '?modulo=principal/cadVistoriaEmpresa&acao=E';
                 </script>");
        }else{
            die("<script>
                     alert('Você não possui permissão para editar esse Acompanhamento!');
                     history.back(-1);
                  </script>");
        }
    case 'alterarSupervisaoFNDE':
        if(possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))){
            $_SESSION['obras2']['sfndeid'] = $_POST['sfndeid'];
            header('Location: ?modulo=principal/cadSupervisaoFNDE&acao=E');
            die;
        }else{
            die("<script>
                     alert('Você não possui permissão para alterar esse Acompanhamento!');
                     history.back(-1);
                  </script>");
        }
    case 'apagarSupervisaoFNDE':
        if($habilitaMods){
            $supervisaoFnde = new SupervisaoFNDE( $_POST['sfndeid'] );
            $supervisaoFnde->sfndestatus = 'I';
            $supervisaoFnde->salvar();

            $db->commit();
            die("<script>
                    alert('Operação realizada com sucesso!');
                    location.href = '?modulo=principal/listaSupervisaoFNDE&acao=A';
                 </script>");
        }else{
            die("<script>
                     alert('Você não possui permissão para alterar esse Acompanhamento!');
                     history.back(-1);
                  </script>");
        }
}

// Post do formulario de Arquivo ART
if( $_REQUEST['req'] ){
    $_REQUEST['req']();
    die();
}

$caminho_atual = 'obras2.php?modulo=principal/vistoria&';

$obrid = $_SESSION['obras2']['obrid'];

ini_set( "memory_limit", "100M" );

include APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

// @TODO Verificar para adaptar
// Executa as funções da tela

$vistoria = new Supervisao();

$responsaveisPossiveis = $vistoria->resgataTodosRsuidsPossiveis();

if( $_REQUEST["subacao"] == "VerificaVistoria" && $habilitaMods === true){

    // Verifica se existe vistorias com data maior do que a da que esta tentando excluir
    $param		 = array();
    $param['not(emsid)'] = true;
    $param['not(smiid)'] = true;
    $pode_excluir_vistoria = $vistoria->verificaExistenciaVistorias( $_REQUEST["supid"], $param );

    $habilita = $vistoria->podeatualizarvistoria( $_REQUEST["supid"] );

    if( $pode_excluir_vistoria && $habilita == 'S'){

        $boExcluida = $vistoria->deletarVistoria( $_REQUEST["supid"], $param );

        if ( true !== $boExcluida ){
            echo "<script>
	                 alert('" . $boExcluida . "');
	                 history.back(-1);
	              </script>";
        }

    }else{

        echo "<script>
                 alert('Você não possui permissão para excluir esse Acompanhamento!');
                 history.back(-1);
              </script>";

    }

}
elseif( $_REQUEST["subacao"] == "VerificaVistoria" && $habilitaMods === false){
    echo "<script>
            alert('Você não possui permissão para excluir esse Acompanhamento!');
            history.back(-1);
          </script>";
}

$esdid = 0;
//$_GET['acao'] = 'V';
$obra  = new Obras();

if(!$obra->carregarPorIdCache($obrid)){
    echo "<script>
            alert('Faltam parâmetros para acessar essa tela.');
            location.href = '?modulo=modulo=principal/inicioLista&acao=A';
          </script>";
    die();
}

if( $_GET['acao'] != 'V' ){
    // Inclusão de arquivos padrão do sistema
    include APPRAIZ . 'includes/cabecalho.inc';
    // Cria as abas do módulo
    echo '<br>';
    if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
        $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
    }else{
        $db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
    }


    $esdid = pegaEstadoObra( $obra->docid );
    if ( $db->testa_superuser() || $esdid == ESDID_OBJ_EXECUCAO || $esdid == ESDID_OBJ_PARALISADO || $esdid == ESDID_OBJ_CONCLUIDO || $esdid == ESDID_OBJ_INACABADA ){
        $habilitado = true;
        $habilita   = 'S';
    }else{
        $habilitado = false;
        $habilita 	= 'N';
    }
}
else{
    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $db->cria_aba($abacod_tela,$url,$parametros);
//	criaAbaVisualizacaoObra();
//	$somenteLeitura = true;
    $habilitado = false;
    $habilita   = 'N';
}

if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
    $habilitado = false;
    $habilita = 'N';
}

/**
 * Verifica se existe uma Trava no Cronograma. Caso o campo obras2.obras.obrtravaedicaocronograma = FALSE, deve-se bloquear as vistorias
 * para que o cronograma seja atualizado.
 */
$obr = new Obras();
$obr->carregarPorIdCache($obrid);
$str_trava_cronograma_vistoria = '';
if($obr->getTravaCronograma($obrid) == 'f'){
    $habilitado = false;
    $habilita = 'N';
    $str_trava_cronograma_vistoria = 'A inclusão de nova vistoria está boqueada até que o Cronograma da Obra seja atualizado.';
}

// Colocar aqui uma restrição na edição dos Arquivos ART caso necessário.
if(possui_perfil(array(PFLCOD_CALL_CENTER))){
    $habilitado_art = false;
}else{
    $habilitado_art = true;
}

echo cabecalhoObra($obrid);

echo alertaObraMi($obrid);
