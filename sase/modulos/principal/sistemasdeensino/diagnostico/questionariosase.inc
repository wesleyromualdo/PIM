<?php
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

if (empty($_GET['muncod'])) {
    $estadomunicipio = $_GET['estuf'];
    $campotabela = "estuf";
    $pagina = "estado";
    $itrid = "1,3";
    $tenid = 9;
} else {
    $estadomunicipio = $_GET['muncod'];
    $campotabela = "muncod";
    $pagina = "municipio";
    $itrid = 2;
    $tenid = 4;
}



if($_GET['requisicao']  == 'downloadAnexo'){
    $file = new FilesSimec("questionarioresposta", NULL, "sase");
    $file->getDownloadArquivo($_GET['arqid']);
}



if ($_POST['aba1']) {

    $campotabela = $_POST['campotabela'];
    $estadomunicipio = $_POST['estadomunicipio'];

    $sql5 = "
    UPDATE sase.questionarioresponsavel set quersituacao  = 2
    WHERE querstatus = 'A'  and {$campotabela} = '{$estadomunicipio}'";
    $db->executar($sql5);

    $db->commit();


    if (!empty($_POST['questao_1'])) {

        $questao = str_replace("questao_", "", $_POST['questao_1']);
        deletaitemresposta(1, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_1'];
        $sqlquestao1 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";

        $db->executar($sqlquestao1);
        $db->commit();
    }

    if ( !empty($_POST['questao_2']))  {
        ver("questao_2");
        $questao = str_replace("questao_", "", $_POST['questao_2']);
        deletaitemresposta(2, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_2'];
        $sqlquestao2 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao2);
        $db->commit();
    }else{
        deletaitemresposta(2, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }



    if (!empty($_FILES)) {

        deletaResposta(3, $_SESSION['usucpf'], $campotabela, $estadomunicipio);

        if ($_FILES['questao_3']['size'] > 0) {
            $questao = str_replace("questao_", "", $_POST['questao_3']);
            $campos = array("qpeid" => "3",
                "usucpf" => "'" . $_SESSION['usucpf'] . "'",
                "qredata" => "now()",
                "qrestatus" => "'A'",
                $campotabela => "'" . $estadomunicipio . "'");

            $file = new FilesSimec("questionarioresposta", $campos, 'sase');
            $file->setUpload($_FILES['questao_3']['name']);

        }
    }else{

        if (empty($_POST['arqid_questao_3'])) {
            deletaResposta(3, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        }
    }

    if (!empty($_POST['questao_4'])) {
        ver("questao_4");
        $arrQuestao_4 = explode(',', $_POST['questao_4']);
        $questao = str_replace("questao_", "", $_POST['questao_4']);
        deletaitemresposta(4, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        foreach ($arrQuestao_4 as $item) {
            $sqlquestao4 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$item}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
            $db->executar($sqlquestao4);
            $db->commit();
        }
    }else{
        deletaitemresposta(4, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }


    $sql = "
    UPDATE sase.questionarioresponsavel set quersituacao  = 2
    WHERE usucpf = '{$_SESSION['usucpf']}' 
                and {$campotabela} = '{$estadomunicipio}'";
    $db->executar($sql);
    $db->commit();


    if (empty($_POST['aba5'])) {

        die;
    }
}

if ($_POST['aba2']) {

    $campotabela = $_POST['campotabela'];
    $estadomunicipio = $_POST['estadomunicipio'];
    if (!empty($_POST['questao_5'])) {
        $questao = str_replace("questao_", "", $_POST['questao_5']);
        deletaitemresposta(5, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_5'];
        $sqlquestao5 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao5);
        $db->commit();
    }else{
        deletaitemresposta(5, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }
    if (!empty($_POST['questao_6'])) {
        $questao = str_replace("questao_", "", $_POST['questao_6']);
        deletaitemresposta(6, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_6'];
        $sqlquestao6 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao6);
        $db->commit();
    }else{
        deletaitemresposta(6, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }

    if (!empty($_FILES)) {


        if ($_FILES['questao_7']['size'] > 0) {
            deletaResposta(7, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
            $campos = array("qpeid" => "7",
                "usucpf" => "'" . $_SESSION['usucpf'] . "'",
                "qredata" => "now()",
                "qrestatus" => "'A'",
                $campotabela => "'" . $estadomunicipio . "'");

            $file = new FilesSimec("questionarioresposta", $campos, 'sase');
            $file->setUpload($_FILES['questao_7']['name']);

        }
    }else{
        if (empty($_POST['arqid_questao_7'])) {
            deletaResposta(7, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        }


    }


    if (!empty($_POST['questao_8'])) {
        $questao = str_replace("questao_", "", $_POST['questao_8']);
        deletaitemresposta(8, $_SESSION['usucpf'], $campotabela, $estadomunicipio);

        $sqlquestao8 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$_POST['questao_8']}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";

        $db->executar($sqlquestao8);
        $db->commit();
    }else{
        deletaitemresposta(8, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }

    if (!empty($_POST['questao_9'])) {
        $arrQuestao_9 = explode(',', $_POST['questao_9']);

        $questao = str_replace("questao_", "", $_POST['questao_9']);
        deletaitemresposta(9, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        foreach ($arrQuestao_9 as $item) {

            $sqlquestao9 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$item}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";

            $db->executar($sqlquestao9);
            $db->commit();
        }
    }else{
        deletaitemresposta(9, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }

    if (!empty($_POST['questao_10'])) {
        $questao = str_replace("questao_", "", $_POST['questao_10']);
        deletaitemresposta(10, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_10'];
        $sqlquestao10 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao10);
        $db->commit();
    }else{
        deletaitemresposta(10, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }

    if (!empty($_POST['questao_11'])) {
        $questao = str_replace("questao_", "", $_POST['questao_11']);
        deletaitemresposta(11, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_11'];
        $sqlquestao11 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao11);
        $db->commit();
    }else{
        deletaitemresposta(11, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }



    $sql5 = "
    UPDATE sase.questionarioresponsavel set quersituacao  = 2
    WHERE querstatus = 'A'  and {$campotabela} = '{$estadomunicipio}'";
    $db->executar($sql5);

    $db->commit();
    if (empty($_POST['aba5'])) {
        die;
    }
}

if ($_POST['aba3']) {

    $campotabela = $_POST['campotabela'];
    $estadomunicipio = $_POST['estadomunicipio'];
    if (!empty($_POST['questao_12'])) {
        $questao = str_replace("questao_", "", $_POST['questao_12']);
        deletaitemresposta(12, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_12'];
        $sqlquestao12 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao12);
        $db->commit();
    }

    if (!empty($_POST['questao_13'])) {
        $questao = str_replace("questao_", "", $_POST['questao_13']);
        deletaitemresposta(13, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_13'];
        $sqlquestao13 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao13);
        $db->commit();
    }else{
        deletaitemresposta(13, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }

    if (!empty($_FILES)) {


        if ($_FILES['questao_14']['size'] > 0) {
            deletaResposta(14, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
            $campos = array("qpeid" => "14",
                "usucpf" => "'" . $_SESSION['usucpf'] . "'",
                "qredata" => "now()",
                "qrestatus" => "'A'",
                $campotabela => "'" . $estadomunicipio . "'");

            $file = new FilesSimec("questionarioresposta", $campos, 'sase');
            $file->setUpload($_FILES['questao_14']['name']);

        }
    }else{

        if (empty($_POST['arqid_questao_14'])) {
            deletaResposta(14, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        }

    }



    $sql5 = "
    UPDATE sase.questionarioresponsavel set quersituacao  = 2
    WHERE querstatus = 'A'  and {$campotabela} = '{$estadomunicipio}'";
    $db->executar($sql5);

    $db->commit();

    if (empty($_POST['aba5'])) {
        die;
    }
}

if ($_POST['aba4']) {


    $campotabela = $_POST['campotabela'];
    $estadomunicipio = $_POST['estadomunicipio'];
     if (isset($_POST['questao_15']) && $_POST['questao_15'] != NULL) {
        $questao = 15;
        deletaResposta(15, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_15'];
        $sqlquestao15 = "INSERT INTO sase.questionarioresposta ( qpeid, usucpf, qreresposta, qredata, qrestatus,{$campotabela} ) VALUES ( {$questao}, '{$_SESSION['usucpf']}' , '{$valor}', now(), 'A', '{$estadomunicipio}');";
        $db->executar($sqlquestao15);
        $db->commit();
    }else{
        deletaResposta(15, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }
    if (!empty($_POST['questao_16'])) {

        $questao = 16;
        deletaitemresposta(16, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_16'];
        $sqlquestao16 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao16);
        $db->commit();
    } else{
        deletaitemresposta(16, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }

     if (isset($_POST['questao_17']) && $_POST['questao_17'] != NULL) {
        if ($_POST['questao_17'] != 'undefined') {
            $questao = 17;
            deletaResposta(17, $_SESSION['usucpf'], $campotabela, $estadomunicipio);

            $valor = $_POST['questao_17'];
            $sqlquestao17 = "INSERT INTO sase.questionarioresposta ( qpeid, usucpf, qreresposta, qredata, qrestatus,{$campotabela}) VALUES ( {$questao}, '{$_SESSION['usucpf']}' , '{$valor}', now(), 'A', '{$estadomunicipio}');";
            $db->executar($sqlquestao17);
            $db->commit();
        }
    }else{

        deletaResposta(17, $_SESSION['usucpf'], $campotabela, $estadomunicipio);

    }


    if (isset($_POST['questao_18']) && $_POST['questao_18'] != NULL) {

        if ($_POST['questao_18'] != 'undefined') {
            $questao = 18;
            deletaResposta(18, $_SESSION['usucpf'], $campotabela, $estadomunicipio);

            $valor = $_POST['questao_18'];
            $sqlquestao18 = "INSERT INTO sase.questionarioresposta ( qpeid, usucpf, qreresposta, qredata, qrestatus,{$campotabela}) VALUES ( {$questao}, '{$_SESSION['usucpf']}' , '{$valor}', now(), 'A', '{$estadomunicipio}');";
            $db->executar($sqlquestao18);
            $db->commit();
        }
    } else {
            deletaResposta(18, $_SESSION['usucpf'], $campotabela, $estadomunicipio);

    }

    if (!empty($_POST['questao_19'])) {
        $questao = 19;
        deletaitemresposta(19, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_19'];
        $sqlquestao19 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao19);
        $db->commit();
    }else {
        deletaitemresposta(19, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }

    if (!empty($_POST['questao_20'])) {
        $questao = 20;
        deletaitemresposta(20, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_20'];
        $sqlquestao20 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao20);
        $db->commit();
    }else {
        deletaitemresposta(20, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }



    $sql5 = "
    UPDATE sase.questionarioresponsavel set quersituacao  = 2
    WHERE querstatus = 'A'  and {$campotabela} = '{$estadomunicipio}'";
    $db->executar($sql5);

    $db->commit();
    if (empty($_POST['aba5'])) {
        die;
    }
}
if ($_POST['aba5']) {

    $campotabela = $_POST['campotabela'];
    $estadomunicipio = $_POST['estadomunicipio'];
    if (!empty($_POST['questao_21'])) {
        $questao = 21;
        deletaitemresposta(21, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_21'];
        $sqlquestao21 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao21);
        $db->commit();
    }else {
        deletaitemresposta(21, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }
    if (!empty($_POST['questao_22'])) {
        $_POST['questao_22'] = substr_replace($_POST['questao_22'], '', -1);

        $arrQuestao_22 = explode(',', ($_POST['questao_22']));

        deletaResposta(22, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        foreach ($arrQuestao_22 as $item) {

            $sqlquestao22 = "INSERT INTO sase.questionarioresposta ( qpeid, usucpf, qreresposta, qredata, qrestatus,{$campotabela}) VALUES (  22, '{$_SESSION['usucpf']}' , '{$item}', now(), 'A', '{$estadomunicipio}')";

            $db->executar($sqlquestao22);
            $db->commit();
        }
    }else {
        deletaResposta(22, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }

    if (!empty($_POST['questao_23'])) {
        $questao = 23;
        deletaitemresposta(23, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        $valor = $_POST['questao_23'];

        $sqlquestao23 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$valor}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
        $db->executar($sqlquestao23);
        $db->commit();
    }else {
        deletaitemresposta(23, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }


    if (!empty($_POST['questao_24'])) {
        $arrQuestao_24 = explode(',', $_POST['questao_24']);
        deletaitemresposta(24, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        foreach ($arrQuestao_24 as $item) {
            $sqlquestao24 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$item}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";


            $db->executar($sqlquestao24);
            $db->commit();
        }
    }else {
        deletaitemresposta(24, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }
    if (!empty($_POST['questao_25'])) {

        $arrQuestao_25 = explode(',', $_POST['questao_25']);
        $questao = 25;
        deletaitemresposta(25, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
        foreach ($arrQuestao_25 as $item) {

            $sqlquestao25 = "INSERT INTO sase.questionarioitemresposta ( qipid, usucpf, qirdata,{$campotabela}) VALUES ( {$item}, '{$_SESSION['usucpf']}' , now(), '{$estadomunicipio}');";
            $db->executar($sqlquestao25);
            $db->commit();
        }
    }else {
        deletaitemresposta(25, $_SESSION['usucpf'], $campotabela, $estadomunicipio);
    }



    $sql5 = "
    UPDATE sase.questionarioresponsavel set quersituacao  = 2
    WHERE querstatus = 'A'  and {$campotabela} = '{$estadomunicipio}'";
    $db->executar($sql5);

    $db->commit();


        die;

}
if ($_POST['aba6']) {

    $campotabela = $_POST['campotabela'];
    $estadomunicipio = $_POST['estadomunicipio'];
    $sql6 = "
    UPDATE sase.questionarioresponsavel set quersituacao  = 3
    WHERE querstatus = 'A' 
    and {$campotabela} = '{$estadomunicipio}'";

    $db->executar($sql6);
    $db->commit();
    die;
}


function deletaitemresposta($questao, $usucpf, $campotabela, $estadomunicipio)
{


    if ($questao) {


        global $db;
        $sql = "DELETE from sase.questionarioitemresposta WHERE qirid in (
	SELECT ir.qirid
	FROM sase.questionariopergunta r
		INNER JOIN sase.questionarioitempergunta ip ON ip.qpeid = r.qpeid
		INNER JOIN sase.questionarioitemresposta ir ON ir.qipid = ip.qipid
	WHERE
		ir.usucpf = '{$usucpf}' AND
		ir." . $campotabela . " = '{$estadomunicipio}' AND
		r.qpeid in ({$questao} ) )";
        ver($sql);

        $db->executar($sql);
        $db->commit();
    }
}

function deletaResposta($questao, $usucpf, $campotabela, $estadomunicipio)
{

    if ($questao) {
        global $db;
        $sql = "DELETE from sase.questionarioresposta WHERE qreid in (
    SELECT qr.qreid
        FROM sase.questionariopergunta pq
            INNER JOIN sase.questionarioresposta qr on qr.qpeid = pq.qpeid
    
        WHERE
            qr.usucpf = '{$usucpf}' AND
            qr." . $campotabela . "= '{$estadomunicipio}' AND
            pq.qpeid in ({$questao})  )";
        $db->executar($sql);

        $db->commit();
    }
}

ob_start();
// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/Assessoramento.class.inc';
include_once '../../sase/classes/SituacaoAssessoramento.class.inc';

function downloadAnexo( $arqid ) {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec( "questionarioresposta", NULL, "sase" );
    $file->getDownloadArquivo( $arqid );
}

include APPRAIZ . 'includes/cabecalho.inc';
// include APPRAIZ . 'www/sase/_funcoes.php';
// --

$perfis = pegaPerfilGeral($_SESSION['usucpf']);

// --------------- Decisão da Página


if(!in_array(PFLCOD_SASE_PESQUISADOR_SASE,$perfis)){
    $verificausuario = '<>';
}else{
    $verificausuario = '=';
}


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'Diagnóstico <br/> Estado';

$sql = "SELECT questdescricao,
         datainicio,
         datafim
          from sase.questionariosase WHERE questid = '1'";
$questionario = $db->carregar($sql);

$datainicio = new DateTime( $questionario[0]['datainicio'] ); // Veja, o padrão aqui é o americano (mes/dia/ano).
$datafim    = new DateTime( $questionario[0]['datafim']); // Veja, o padrão aqui é o americano (mes/dia/ano).
$dataatual  = new DateTime( date("Y-m-d") ); // Padrão do MySQL também é aceito.

// estado
$sql = "SELECT  estdescricao
					FROM  territorios.estado  where estuf = '" . $_GET['estuf'] . "'";
$estado = $db->pegaUm($sql);

if(   $dataatual < $datainicio  ){
    echo "<script>alert('Questionário Indisponível!');
              location.href = 'sase.php?modulo=principal/sistemasdeensino/diagnostico/estado&acao=A';
          </script>";
}elseif ($dataatual > $datafim ){
    echo "<script>alert('Questionário Indisponível!');
              location.href = 'sase.php?modulo=principal/sistemasdeensino/diagnostico/estado&acao=A';
          </script>";

}

if (empty($_GET['muncod'])) {
    $sql = "SELECT count(*) FROM sase.questionarioresponsavel WHERE estuf  = '" . $_GET['estuf'] . "' and querstatus = 'A'";
    $sql2 = "SELECT count(*) FROM sase.questionarioresponsabilidade WHERE estuf  = '" . $_GET['estuf'] . "' and rpustatus = 'A' and usucpf ='{$_SESSION[usucpf]}' ";
} else {
    $sql = "SELECT count(*) FROM sase.questionarioresponsavel WHERE muncod  = '" . $_GET['muncod'] . "' and querstatus = 'A'";
    $sql2 = "SELECT count(*) FROM sase.questionarioresponsabilidade WHERE muncod  = '" . $_GET['muncod'] . "' and rpustatus = 'A' and usucpf ='{$_SESSION[usucpf]}' ";
}
$temcriado = $db->pegaUm($sql);
$abrequestionario  = $db->pegaUm($sql2);

if($abrequestionario == 0){
    echo "<script>alert('Questionário Indisponível!');
             history.go(-1);
          </script>";
}
$mundescricao = "";
if (!empty($_GET['muncod'])) {
    $sql = "SELECT mundescricao from territorios.municipio where  muncod =  '{$_GET['muncod']}'";
    $mundescricao = $db->pegaUm($sql);
    $ente= " Ente Federativo/UF: <b>".$mundescricao." / ".$_GET['estuf'] ."</b><br>";
}else{
    $ente = " Ente Federativo/UF: <b>".$estado."/".$_GET['estuf']." </b><br>";
}



if ($temcriado == 0) {
    if (empty($_GET['muncod'])) {
        $sql = "INSERT INTO sase.questionarioresponsavel ( usucpf, estuf,  questid, querstatus) VALUES ( '" . $_SESSION['usucpf'] . "', '" . $_GET['estuf'] . "', 1, 'A');";
    } else {
        $sql = "INSERT INTO sase.questionarioresponsavel ( usucpf,  muncod, questid, querstatus) VALUES ( '" . $_SESSION['usucpf'] . "', '" . $_GET['muncod'] . "', 1, 'A');";
    }

    $db->executar($sql);
}


if ($_GET['estuf']) {
    $sql = "SELECT
              u.usunome
            FROM sase.questionarioresponsabilidade r
             INNER JOIN seguranca.usuario u on u.usucpf = r.usucpf
            WHERE r.estuf  = '" . $_GET['estuf'] . "' and rpustatus = 'A' and r.pflcod = ".PFLCOD_SASE_SECRETARIO;

    $reponsavel = $db->pegaUm($sql);


}

if ($_GET['muncod']) {
    $sql = "SELECT
              u.usunome
            FROM sase.questionarioresponsabilidade r
             INNER JOIN seguranca.usuario u on u.usucpf = r.usucpf
            WHERE r.muncod  = '" . $_GET['muncod'] . "' and rpustatus = 'A' and r.pflcod = ".PFLCOD_SASE_SECRETARIO;
    $reponsavel = $db->pegaUm($sql);

}



// estado /
$sql = "
SELECT inuent.entnome
FROM par3.instrumentounidade_entidade as inuent
INNER JOIN par3.instrumentounidade as inu ON inu.inuid = inuent.inuid
INNER JOIN par3.tipoentidade as ten ON ten.tenid = inuent.tenid
WHERE inu.{$campotabela} = '{$estadomunicipio}'
AND   inu.itrid  in ({$itrid}) --1 Estadual,2 municipal, 3 distrito
AND inuent.tenid in ({$tenid}) --  9 Estadual , 4 municipal
AND inuent.entstatus = 'A'
order by inuent.entnome ASC;


";

$nomeSecretario = $db->carregar($sql);




// estado /

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<style>
    .container label {
        font-size: 12px;
    }
</style>

<div class="jumbotron" style="padding-top: 10px">
    <div class="container">
        <h3><?= $questionario[0]['questdescricao'] ?></h3> <br>
        <h5>  <b><?=$ente?>
            <?php if (!empty($nomeSecretario)) {

                foreach ($nomeSecretario as $nome) {
                    echo "Secretário(a): <b>" . $nome['entnome'] . " </b></br>";
                }
            } ?>
            Responsável pelo Preenchimento: <b><?=$reponsavel?></b>
                </h5>
        <br>
    </div>
</div>

<!-- Form Name -->
<legend>Questionário</legend>
<div>
    
    <ul class="nav nav-tabs" role="tablist" id="abas-questionario">
        <li role="presentation" id="presentation_1" class="active"><a href="#home" class="home" onclick="salvarQuestoesAba('#home', false, true);" aria-controls="home" role="tab" data-toggle="tab">Sistema de Ensino</a></li>
        <li role="presentation" id="presentation_2"><a href="#profile" class="profile" onclick="salvarQuestoesAba('#profile', false, true);" aria-controls="profile" role="tab" data-toggle="tab">Conselho de Educação</a></li>
        <li role="presentation" id="presentation_3"><a href="#messages" class="messages" onclick="salvarQuestoesAba('#messages', false, true);" aria-controls="messages" role="tab" data-toggle="tab">Fórum de Educação</a></li>
        <li role="presentation" id="presentation_4"><a href="#settings" class="settings" onclick="salvarQuestoesAba('#settings', false, true);" aria-controls="settings" role="tab" data-toggle="tab">Rede de Ensino</a></li>
        <li role="presentation" id="presentation_5"><a href="#settings2" class="settings2" onclick="salvarQuestoesAba('#settings2', false, true);" aria-controls="settings" role="tab" data-toggle="tab">Regime de Colaboração</a></li>
    </ul>
    
    <div class="tab-content" style="padding-top: 10px;">
        <div role="tabpanel" class="tab-pane fade in active" id="home"><? include APPRAIZ . "sase/modulos/principal/sistemasdeensino/diagnostico/questionariosistemaensino.inc"; ?></div>
        <div role="tabpanel" class="tab-pane fade" id="profile"><? include APPRAIZ . "sase/modulos/principal/sistemasdeensino/diagnostico/questionarioconselhoeducacao.inc"; ?></div>
        <div role="tabpanel" class="tab-pane fade" id="messages"><? include APPRAIZ . "sase/modulos/principal/sistemasdeensino/diagnostico/questionarioforumeducacao.inc"; ?></div>
        <div role="tabpanel" class="tab-pane fade" id="settings"><? include APPRAIZ . "sase/modulos/principal/sistemasdeensino/diagnostico/questionarioredeensino.inc"; ?></div>
        <div role="tabpanel" class="tab-pane fade" id="settings2"><? include APPRAIZ . "sase/modulos/principal/sistemasdeensino/diagnostico/questionarioreginecolaboracao.inc"; ?></div>
    </div>
</div>

<script type="text/javascript">
    $(document).ready(function () {

        var questao_12 = $("input[name$='questao_12']:checked").val();
        if (questao_12 == 33) {
            $(".questao12positivo").slideDown("slow");
        }
        
        /* Finalizar questionário */
        $("#btnfinalizar").click(function () {
            //Aba 1
            var questao_1 = $("input[name$='questao_1']:checked").val();
            if (questao_1) {
                if (questao_1 == 1) {
                    var questao_2 = $("input[name$='questao_2']:checked").val();
                    if (!questao_2) {
                        alert("Preencher pergunta 1.2 Qual ato legal instituiu o sistema estadual/distrital/municipal de ensino?");
                        mudarAba(1);
                        return false;
                    }
                    /*   dataType: 'json',*/
                    var questao_3 = $("#questao_3").val();
                    if (questao_3 == '') {
                        if($("#arqid_questao_3").val() != '1'){
                            alert("Preencher pergunta 1.3 Anexar cópia do ato legal mencionado no item anterior.");
                            mudarAba(1);
                            return false;
                        }
                    } else {

                        var validaextenao = validoFormatoArquivo(questao_3);
                        if (!validaextenao) {
                            alert("Anexar 1.3 Anexar cópia do ato legal mencionado no item anterior do tipo PDF!");
                            mudarAba(1);
                            return false;
                        }

                    }

                    var camposMarcados = new Array();
                    $("input[type=checkbox][name='questao_4[]']:checked").each(function () {
                        camposMarcados.push($(this).val());
                    });

                    if (camposMarcados == '') {
                        alert("Preencher pergunta 1.4 Qual a constituição do sistema de ensino? Marque as alternativas aplicáveis:");
                        mudarAba(1);
                        return false;
                    }
                }
                ;

            } else {
                alert("1.1 O estado/Distrito Federal/município possui sistema de ensino instituído? é Obrigatório!");
                mudarAba(1);
                return false;
            }
            ;
            var questao_5 = $("input[name$='questao_5']:checked").val();
            if (questao_5) {
                if (questao_5 == 13) {
                    var questao_6 = $("input[name$='questao_6']:checked").val();
                    if (!questao_6) {
                        alert("Preencher pergunta 2.2 Qual o ato legal de criação do conselho de educação?");
                        mudarAba(2);
                        return false;
                    }
                    var questao_7 = $("#questao_7").val();
                    if (questao_7 == '') {
                        if($("#arqid_questao_7").val() != '1'){
                            alert("Preencher pergunta 2.3 Anexar cópia do ato legal mencionado no item anterior.");
                            mudarAba(2);
                            return false;
                        }
                    } else {

                        var validaextenao = validoFormatoArquivo(questao_7);
                        if (!validaextenao) {
                            alert("Anexar 2.3 Anexar cópia do ato legal mencionado no item anterior do tipo PDF!");
                            mudarAba(2);
                            return false;
                        }
                    }

                    var questao_8 = $("input[name$='questao_8']:checked").val();
                    if (!questao_8) {
                        alert("Preencher pergunta 2.4 A composição do conselho de educação é paritária?");
                        mudarAba(2);
                        return false;
                    }

                    var camposMarcados25 = new Array();
                    $("input[type=checkbox][name='questao_9[]']:checked").each(function () {
                        camposMarcados25.push($(this).val());
                    });

                    if (camposMarcados25 == '') {
                        alert("Preencher pergunta 2.5 Assinale as alternativas equivalentes aos representantes no conselho de educação:");
                        mudarAba(2);
                        return false;
                    }

                    var questao_10 = $("input[name$='questao_10']:checked").val();
                    if (!questao_10) {
                        alert("Preencher pergunta 2.6 O conselho de educação possui dotação orçamentária própria?");
                        mudarAba(2);
                        return false;
                    }
                    var questao_11 = $("input[name$='questao_11']:checked").val();
                    if (!questao_11) {
                        alert("Preencher pergunta 2.7 O conselho de educação possui plena autonomia para a homologação dos atos?");
                        mudarAba(2);
                        return false;
                    }

                }
                ;

            } else {
                alert("2.1 O estado/Distrito Federal/município possui conselho de educação?");
                mudarAba(2);
                return false;
            }
            ;


            var questao_12 = $("input[name$='questao_12']:checked").val();

            if (questao_12) {
                if (questao_12 == 33) {
                    var questao_13 = $("input[name$='questao_13']:checked").val();
                    if (!questao_13) {
                        alert("Preencher pergunta 3.2 Qual ato legal instituiu o fórum municipal de educação?");
                        mudarAba(3);
                        return false;
                    }

                    var questao_14 = $("#questao_14").val();
                    if (questao_14 == '') {
                        if($("#arqid_questao_14").val() != '1'){
                            alert("Preencher pergunta 3.3 Anexar cópia do ato legal mencionado no item anterior.");
                            return false;
                        }
                    } else {
                        var validaextenao = validoFormatoArquivo(questao_14);
                        if (!validaextenao) {
                            alert("Anexar 3.3 Anexar cópia do ato legal mencionado no item anterior do tipo PDF!");
                            mudarAba(3);
                            return false;
                        }
                    }
                }
                ;

            } else {
                alert("3.1 O fórum estadual/distrital/municipal de educação está instituído?");
                mudarAba(3);
                return false;
            }
            ;
            var questao_15 = $("input[name$='questao_15']").val();
            if (!questao_15) {
                alert("Preencher pergunta 4.1 Qual o total de escolas da rede de ensino com cadastro no Instituto Nacional de Estudos e Pesquisas Educacionais Anísio Teixeira (INEP)?");
                mudarAba(4);
                return false;
            }
            var questao_16 = $("input[name$='questao_16']:checked").val();

            if (questao_16) {
                if (questao_16 == 39) {
                    var questao_17 = $("input[name$='questao_17']").val();
                    if (!questao_17) {
                        alert("Preencher pergunta 4.2.1. Qual o quantitativos de escolas com conselhos escolares implantados?");
                        mudarAba(4);
                        return false;
                    }
                } else {

                }
                var questao_18 = $("input[name$='questao_18']").val();
                if (!questao_18) {
                    alert("Preencher pergunta 4.3 Quantas escolas possuem grêmios estudantis em funcionamento?");
                    mudarAba(4);
                    return false;
                }

                var questao_19 = $("input[name$='questao_16']:checked").val();

                if (questao_19) {
                    if (questao_19 == 41) {
                        var questao_20 = $("input[name$='questao_20']").val();
                        if (!questao_20) {
                            alert("Preencher pergunta 4.4.1. Qual o mecanismo utilizado para a escolha do diretor escolar? ");
                            mudarAba(4);
                            return false;
                        }
                    }
                } else {
                    alert("Preencher pergunta 4.4.0 Há normas específicas para escolha de diretor escolar?");
                    mudarAba(4);
                    return false;
                }


            } else {
                alert("4.2.0 A rede de ensino possui legislação para criação de conselhos escolares?");
                mudarAba(4);
                return false;
            }
            ;

            var questao_21 = $("input[name$='questao_21']:checked").val();
            if (!questao_21) {
                alert("Preencher pergunta 5.1.0 O estado/Distrito federal/município participa de alguma instância de negociação e pactuação das políticas educacionais junto ao estado e/ou outros municípios?");
                mudarAba(5);
                return false;
            } else {
                if (questao_21 == 48) {
                    var questao_22 = $("#oculta_questao_22").val();
                    if (questao_22 == '') {
                        alert("Preencher pergunta 5.1.1 Qual(is) instância(s)?");
                        mudarAba(5);
                        return false;
                    }
                }
            }
            var questao_23 = $("input[name$='questao_23']:checked").val();


            if (questao_23) {
                if (questao_23 == 68) {
                    var camposMarcados52 = new Array();
                    $("input[type=checkbox][name='questao_24[]']:checked").each(function () {
                        camposMarcados52.push($(this).val());
                    });

                    if (camposMarcados52 == '') {
                        alert("Preencher pergunta 5.2.1 Assinale, a seguir, as formas de cooperação desenvolvidas:");
                        mudarAba(5);
                        return false;
                    }

                    var camposMarcados521 = new Array();
                    $("input[type=checkbox][name='questao_25[]']:checked").each(function () {
                        camposMarcados521.push($(this).val());
                    });

                    if (camposMarcados521 == '') {
                        alert("Preencher pergunta 5.2.1 Assinale, a seguir, as formas de cooperação desenvolvidas:");
                        mudarAba(5);
                        return false;
                    }
                }
            } else {
                alert("Preencher pergunta 5.2.0 Existem formas de cooperação desenvolvidas entre o Estado e/ou outros municípios?");
                mudarAba(5);
                return false;
            }


            var campotabela = "<?=$campotabela?>"
            var estadomunicipio = "<?=$estadomunicipio?>"


            var form_data = new FormData();

            form_data.append('aba6', 'true');
            form_data.append('campotabela', campotabela);
            form_data.append('estadomunicipio', estadomunicipio);

            $.ajax({
                url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
                dataType: 'text',  // what to expect back from the PHP script, if anything
                cache: false,
                contentType: false,
                processData: false,
                data: form_data,
                type: 'post',
                success: function (response) {
                    alert("Questionário Finalizado");
                    // history.go(-1);
                    location.href = 'sase.php?modulo=principal/sistemasdeensino/diagnostico/<?=$pagina?>&acao=A';

                }
            });
        });
    });
    
    
    function validoFormatoArquivo(arquivo) {
        var extensoes, ext, valido;
        extensoes = new Array('.pdf');

        ext = arquivo.substring(arquivo.lastIndexOf(".")).toLowerCase();
        valido = false;

        for (var i = 0; i <= arquivo.length; i++) {
            if (extensoes[i] == ext) {
                valido = true;
                break;
            }
        }

        if (valido) {
            return true;
        }

        return false;
    }
    
    
    /* Salvar todas as questões de todas as abas */
    function salvarQuestoesAba(btn, btnProximo, clickAba){
        if(!btnProximo && !clickAba){
            //console.log("Executou o botão salvar");
            salvarQuestoesSistemaEnsino(btn, btnProximo, clickAba);
        }else{
            if(clickAba && !btnProximo){
                //console.log("Clicou nas abas manualmente");
            }else{
                //console.log("Botão clicado foi: ", btn);
                
                if (btn == '#btnv000') {
                    // ID botão proximo aba 1 #btnv000
                    salvarQuestoesSistemaEnsino(btn, btnProximo, clickAba);
                } else if (btn == '#btnp022') {
                    // ID botão proximo aba 2 #btnp022
                    salvarQuestoesConselhoDeEducacao(btn, btnProximo, clickAba);
                } else if (btn == '#btnp033') {
                    // ID botão proximo aba 3 #btnp033
                    salvarQuestoesForumDeEducacao(btn, btnProximo, clickAba);
                } else if (btn == '#btnp044') {
                    // ID botão proximo aba 4 #btnp044       
                     salvarQuestoesRedeDeEnsino(btn, btnProximo, clickAba);
                }
                //console.log("Executou o botão próximo");
            } 
        }
    }
    
    /* Salvar questoes da aba Sistema de Ensino */
    function salvarQuestoesSistemaEnsino(btn, btnProximo, clickAba){ 
        
        var questao_1 = $("input[name$='questao_1']:checked").val();

        if (questao_1) {
            if (questao_1 == 1) {
                var questao_2 = $("input[name$='questao_2']:checked").val();
                if (!questao_2) {
                    alert("Preencher pergunta 1.2 Qual ato legal instituiu o sistema estadual/distrital/municipal de ensino?");
                    mudarAba(1);
                    return false;
                }
                
                var questao_3 = $("#questao_3").val();                
                if (questao_3 == '') {
                    if ($("#arqid_questao_3").val() != '1') {
                        alert("Preencher pergunta 1.3 Anexar cópia do ato legal mencionado no item anterior.");
                        mudarAba(1);
                        return false;
                    }
                } else {
                    var validaextenao = validoFormatoArquivo(questao_3);
                    if (!validaextenao) {
                        alert("Anexar arquivo do tipo PDF!");
                        mudarAba(1);
                        return false;
                    }
                }

                var camposMarcados = new Array();
                $("input[type=checkbox][name='questao_4[]']:checked").each(function () {
                    camposMarcados.push($(this).val());
                });

                if (camposMarcados == '') {
                    alert("Preencher pergunta 1.4 Qual a constituição do sistema de ensino? Marque as alternativas aplicáveis:");
                    mudarAba(1);
                    return false;
                }
            }
            
            var form_data = new FormData();

            form_data.append('aba1', 'true');
            form_data.append('questao_1', questao_1);
            form_data.append('questao_2', (questao_2 === undefined) ? '' : questao_2);
            if ($("#questao_3").val() != '' ) {
                var file_data = $("#questao_3").prop('files')[0];
                form_data.append('questao_3', file_data);
            } else {
                form_data.append('arqid_questao_3', 1);
            }
            form_data.append('questao_4', (camposMarcados === undefined) ? '' : camposMarcados);
            var campotabela = "<?=$campotabela?>"
            var estadomunicipio = "<?=$estadomunicipio?>"
            form_data.append('campotabela', campotabela);
            form_data.append('estadomunicipio', estadomunicipio);

            $.ajax({
                url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A',
                dataType: 'text',  
                cache: false,
                contentType: false,
                processData: false,
                data: form_data,
                type: 'post',
                success: function (response) {
                    if ($("#arqid_questao_3").val() != 1) {
                        $('#linkArqid_3').hide();
                    }
                    
                    if(btnProximo){
                        alert("Os dados do Sistema de Ensino foram salvos com sucesso!"); 
                    } else {                   
                        salvarQuestoesConselhoDeEducacao(btn, btnProximo, clickAba);
                    }
                    /* location.reload();*/
                }
            });

        } else {
            alert("1.1 O estado/Distrito Federal/município possui sistema de ensino instituído? é Obrigatório!");
            mudarAba(1);
            return false;
        }
    }
    
    /* Salvar questões da aba Conselho de Educação */
    function salvarQuestoesConselhoDeEducacao(btn, btnProximo, clickAba){    
    
        var questao_5 = $("input[name$='questao_5']:checked").val();

        if(questao_5 ) {
            if(questao_5 == 13 ){
                var questao_6 = $("input[name$='questao_6']:checked").val();
                if(!questao_6){
                    alert("Preencher pergunta 2.2 Qual o ato legal de criação do conselho de educação?");
                    mudarAba(2);
                    return false;
                }
                var questao_7 = $("#questao_7").val();
                if(questao_7 == ''){
                    if($("#arqid_questao_7").val() != '1'){
                        alert("Preencher pergunta 2.3 Anexar cópia do ato legal mencionado no item anterior.");
                        mudarAba(2);
                        return false;
                    }
                }else{
                    var validaextenao =  validoFormatoArquivo(questao_7);
                    if(!validaextenao){
                        alert("Anexar arquivo do tipo PDF!");
                        mudarAba(2);
                        return false;
                    }
                }

                var questao_8 = $("input[name$='questao_8']:checked").val();
                if(!questao_8){
                    alert("Preencher pergunta 2.4 A composição do conselho de educação é paritária?");
                    mudarAba(2);
                    return false;
                }

                var camposMarcados25 = new Array();
                $("input[type=checkbox][name='questao_9[]']:checked").each(function(){
                    camposMarcados25.push($(this).val());
                });

                if(camposMarcados25 == ''){
                    alert("Preencher pergunta 2.5 Assinale as alternativas equivalentes aos representantes no conselho de educação:");
                    mudarAba(2);
                    return false;
                }

                var questao_10 = $("input[name$='questao_10']:checked").val();
                if(!questao_10){
                    alert("Preencher pergunta 2.6 O conselho de educação possui dotação orçamentária própria?");
                    mudarAba(2);
                    return false;
                }
                var questao_11 = $("input[name$='questao_11']:checked").val();
                if(!questao_11){
                    alert("Preencher pergunta 2.7 O conselho de educação possui plena autonomia para a homologação dos atos?");
                    mudarAba(2);
                    return false;
                }
            };

            var form_data = new FormData();

            if ($("#questao_7").val() != '' ) {
                var file_data = $("#questao_7").prop('files')[0];
                form_data.append('questao_7', file_data);
            }else {
                form_data.append('arqid_questao_7', 1);
            }

            form_data.append('aba2', 'true');
            form_data.append('questao_5', (questao_5 === undefined)?'':questao_5);
            form_data.append('questao_6', (questao_6 === undefined)?'':questao_6);
            form_data.append('questao_8', (questao_8 === undefined)?'':questao_8);
            form_data.append('questao_9', (camposMarcados25 === undefined)?'':camposMarcados25);
            form_data.append('questao_10', (questao_10 === undefined)?'':questao_10);
            form_data.append('questao_11', (questao_11 === undefined)?'':questao_11);

            var campotabela = "<?=$campotabela?>"
            var estadomunicipio = "<?=$estadomunicipio?>"
            form_data.append('campotabela', campotabela);
            form_data.append('estadomunicipio', estadomunicipio);
            //alert(form_data);

            $.ajax({
                url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
                dataType: 'text',  // what to expect back from the PHP script, if anything
                cache: false,
                contentType: false,
                processData: false,
                data: form_data,
                type: 'post',
                success: function(response){
                    if($("#arqid_questao_7").val() != 1) {
                        $('#linkArqid_7').hide();
                    }
                    
                    if(btnProximo){
                        alert("Os dados do Conselho de Educação foram salvos com sucesso!");
                    } else {
                        salvarQuestoesForumDeEducacao(btn, btnProximo, clickAba);
                    }
                }
            });

        }else {
            alert("2.1 O estado/Distrito Federal/município possui conselho de educação?");
            mudarAba(2);
            return false;
        };
    }
    
    /* Salvar questões da aba Forum de Educação */
    function salvarQuestoesForumDeEducacao(btn, btnProximo, clickAba){
    
        var questao_12 = $("input[name$='questao_12']:checked").val();

        if(questao_12 ){
            if(questao_12 == 33 ){
                var questao_13 = $("input[name$='questao_13']:checked").val();
                if(!questao_13){
                    alert("Preencher pergunta 3.2 Qual ato legal instituiu o fórum municipal de educação?");
                    mudarAba(3);
                    return false;
                }

                var questao_14 = $("#questao_14").val();
                if(questao_14 == ''){
                    if($("#arqid_questao_14").val() != '1'){
                        alert("Preencher pergunta 3.3 Anexar cópia do ato legal mencionado no item anterior.");
                        mudarAba(3);
                        return false;
                    }
                }else{
                    var validaextenao =  validoFormatoArquivo(questao_14);
                    if(!validaextenao){
                        alert("Anexar arquivo do tipo PDF!");
                        mudarAba(3);
                        return false;
                    }
                }
            };

            var form_data = new FormData();
            if ($("#questao_14").val() != '' ) {    
                var file_data = $("#questao_14").prop('files')[0];
                form_data.append('questao_14', file_data);
            }else {
                form_data.append('arqid_questao_14', 1);
            }
            form_data.append('aba3', 'true');
            form_data.append('questao_12', (questao_12 === undefined)?'':questao_12);
            form_data.append('questao_13', (questao_13 === undefined)?'':questao_13);
            var campotabela = "<?=$campotabela?>"
            var estadomunicipio = "<?=$estadomunicipio?>"
            form_data.append('campotabela', campotabela);
            form_data.append('estadomunicipio', estadomunicipio);

            $.ajax({
                url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
                dataType: 'text',  // what to expect back from the PHP script, if anything
                cache: false,
                contentType: false,
                processData: false,
                data: form_data,
                type: 'post',
                success: function(response){
                    if($("#arqid_questao_14").val() != 1) {
                        $('#linkArqid_14').hide();
                    }
                    
                    if(btnProximo){
                        alert("Os dados do Fórum de Educação foram salvos com sucesso!");
                    } else {
                        salvarQuestoesRedeDeEnsino(btn, btnProximo, clickAba);
                    }
                }
            });

        }else {
            alert("3.1 O fórum estadual/distrital/municipal de educação está instituído?");
            mudarAba(3);
            return false;
        };
    }
        
    /* Salvar questões da aba Rede de Ensino */
    function salvarQuestoesRedeDeEnsino(btn, btnProximo, clickAba){
                                
        var questao_15 = $("input[name$='questao_15']").val();
        if(!questao_15){
            alert("Preencher pergunta 4.1 Qual o total de escolas da rede de ensino com cadastro no Instituto Nacional de Estudos e Pesquisas Educacionais Anísio Teixeira (INEP)?");
            mudarAba(4);
            return false;
        }
        var questao_16 = $("input[name$='questao_16']:checked").val();

        if(questao_16 ){
            if(questao_16 == 39 ){
                var questao_17 = $("input[name$='questao_17']").val();
                if(!questao_17){
                    alert("Preencher pergunta 4.2.1. Qual o quantitativos de escolas com conselhos escolares implantados?");
                    mudarAba(4);
                }
            }else{

            }

            var questao_18 = $("input[name$='questao_18']").val();
            if(!questao_18){
                alert("Preencher pergunta 4.3 Quantas escolas possuem grêmios estudantis em funcionamento?");                
                mudarAba(4);
                return false;
            }

            var questao_19 = $("input[name$='questao_19']:checked").val();

            if(questao_19 ){
                if(questao_19 == 41 ){
                    var questao_20 = $("input[name$='questao_20']:checked").val();
                    if(!questao_20){
                        alert("Preencher pergunta 4.4.1. Qual o mecanismo utilizado para a escolha do diretor escolar? ");
                        mudarAba(4);
                        return false;
                    }
                }
            }else{
                alert("Preencher pergunta 4.4.0 Há normas específicas para escolha de diretor escolar?");
                mudarAba(4);
                return false;
            }

            var form_data = new FormData();

            form_data.append('aba4', 'true');

            form_data.append('questao_15', (questao_15 === undefined)?'':questao_15);
            form_data.append('questao_16', (questao_16 === undefined)?'':questao_16);
            form_data.append('questao_17', (questao_17 === undefined)?'':questao_17);
            form_data.append('questao_18', (questao_18 === undefined)?'':questao_18);
            form_data.append('questao_19', (questao_19 === undefined)?'':questao_19);
            form_data.append('questao_20', (questao_20 === undefined)?'':questao_20);

            var campotabela = "<?=$campotabela?>";
            var estadomunicipio = "<?=$estadomunicipio?>";
            form_data.append('campotabela', campotabela);
            form_data.append('estadomunicipio', estadomunicipio);

            $.ajax({
                url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
                dataType: 'text',  // what to expect back from the PHP script, if anything
                cache: false,
                contentType: false,
                processData: false,
                data: form_data,
                type: 'post',
                success: function(response){
                    if(btnProximo){
                        alert("Os dados da Rede de Ensino foram salvos com sucesso! ");
                    } else {
                        salvarQuestoesRegimeDeColaboracao(btn, btnProximo, clickAba);
                    }
                }
            });

        }else {
            alert("4.2.0 A rede de ensino possui legislação para criação de conselhos escolares?");
            mudarAba(4);
            return false;
        };
    }
        
    /* Salvar questões da aba Regime de Colaboração */
    function salvarQuestoesRegimeDeColaboracao(btn, btnProximo, clickAba){
                    
        var questao_21 = $("input[name$='questao_21']:checked").val();
        if(!questao_21){
            alert("Preencher pergunta 5.1.0 O estado/Distrito federal/município participa de alguma instância de negociação e pactuação das políticas educacionais junto ao estado e/ou outros municípios?");
            mudarAba(5);
            return false;
        }else{
            if(questao_21 == 48 ){
                var questao_22 = $("#oculta_questao_22").val();
                if(questao_22 == ''){
                    alert("Preencher pergunta 5.1.1 Qual(is) instância(s)?");
                    mudarAba(5);
                    return false;
                }
            }
        }
        
        var questao_23 = $("input[name$='questao_23']:checked").val();

        if(questao_23 ) {
            if (questao_23 == 68) {
                var camposMarcados52 = new Array();
                $("input[type=checkbox][name='questao_24[]']:checked").each(function(){
                    camposMarcados52.push($(this).val());
                });

                if(camposMarcados52 == ''){
                    alert("Preencher pergunta 5.2.1 Assinale, a seguir, as formas de cooperação desenvolvidas:");
                    mudarAba(5);
                    return false;
                }

                var camposMarcados521 = new Array();
                $("input[type=checkbox][name='questao_25[]']:checked").each(function(){
                    camposMarcados521.push($(this).val());
                });

                if(camposMarcados521 == ''){
                    alert("Preencher pergunta 5.2.1 Assinale, a seguir, as formas de cooperação desenvolvidas:");
                    mudarAba(5);
                    return false;
                }
            }
        }else {
            alert("Preencher pergunta 5.2.0 Existem formas de cooperação desenvolvidas entre o Estado e/ou outros municípios?");
            mudarAba(5);
            return false;
        }

        var form_data = new FormData();

        form_data.append('aba5', 'true');

        form_data.append('questao_21', (questao_21 === undefined)?'':questao_21);
        form_data.append('questao_22', (questao_22 === undefined)?'':questao_22);
        form_data.append('questao_23', (questao_23 === undefined)?'':questao_23);
        form_data.append('questao_24', (camposMarcados52 === undefined)?'':camposMarcados52);
        form_data.append('questao_25', (camposMarcados521 === undefined)?'':camposMarcados521);

        var campotabela = "<?=$campotabela?>"
        var estadomunicipio = "<?=$estadomunicipio?>"
        form_data.append('campotabela', campotabela);
        form_data.append('estadomunicipio', estadomunicipio);

        $.ajax({
            url: 'sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A', // point to server-side PHP script
            dataType: 'text',  // what to expect back from the PHP script, if anything
            cache: false,
            contentType: false,
            processData: false,
            data: form_data,
            type: 'post',
            success: function(response){
                if(!clickAba){
                    alert("Os dados do Questionário foram salvos com sucesso!")
                }               

            }
        });
    }
    
    
    function mudarAba(aba){        
        //console.log("Executou função aba: "+aba);        
        var href = $("ul#abas-questionario li#presentation_"+aba).find('a').attr('href');
        //console.log("href: "+href);
        setTimeout(function(){
            var link_href = href.replace("#", "");
            //console.log("link_href: "+link_href);
            document.getElementsByClassName(link_href)[0].click();          
        }, 300);
    }
    
         
</script>


