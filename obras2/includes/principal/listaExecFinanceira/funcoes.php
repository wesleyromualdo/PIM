<?php 


function uploadArquivosExecucaoOrc()
{
    $arrRetornoIdsArquivos = array();

    if ($_REQUEST['requisicao'] == 'salvar') {
        $arquivo_ordbanc = $_FILES['arquivo_ordbanc'];
        $arquivo_notafiscal = $_FILES['arquivo_notafiscal'];
        $arquivo_boletim = (isset($_FILES['arquivo_boletim']) ? $_FILES['arquivo_boletim'] : array());
    } elseif ($_REQUEST['requisicao'] == 'update') {
        $arquivo_ordbanc = (isset($_FILES['arquivo_ordbanc'])) ? $_FILES['arquivo_ordbanc'] : array();
        $arquivo_notafiscal = (isset($_FILES['arquivo_notafiscal'])) ? $_FILES['arquivo_notafiscal'] : array();
        $arquivo_boletim = (isset($_FILES['arquivo_boletim']) ? $_FILES['arquivo_boletim'] : array());
    }

    // Arquivo da ordem bancária
    if ($arquivo_ordbanc["name"] && $arquivo_ordbanc["type"] && $arquivo_ordbanc["size"] && $arquivo_ordbanc["error"] == 0) {
        $file = new FilesSimec();
        $file->setPasta('obras2');
        $nome_arq = "anexoordembancaria_" . $_SESSION['obras2']['obrid'] . '_' . time();
        $file->setUpload($nome_arq, 'arquivo_ordbanc', false, false);
        $arqid = $file->getIdArquivo();
        $arrRetornoIdsArquivos['peoarqid_ordembancaria'] = $arqid;
    } else {
        $arrRetornoIdsArquivos['peoarqid_ordembancaria'] = 'NULL';
        if ($_REQUEST['requisicao'] == 'salvar') {
            die("<script>
                        alert('O arquivo Comprovante da Transferência é obrigatório!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        }
    }

    // Arquivo da nota fiscal
    if ($arquivo_notafiscal["name"] && $arquivo_notafiscal["type"] && $arquivo_notafiscal["size"] && $arquivo_notafiscal["error"] == 0) {
        $file2 = new FilesSimec();
        $file2->setPasta('obras2');
        $nome_arq2 = "anexonotafiscal_" . $_SESSION['obras2']['obrid'] . '_' . time();
        $file2->setUpload($nome_arq2, 'arquivo_notafiscal', false, false);
        $arqid2 = $file2->getIdArquivo();
        $arrRetornoIdsArquivos['peoarqid_notafiscal'] = $arqid2;
    } else {
        $arrRetornoIdsArquivos['peoarqid_notafiscal'] = 'NULL';
        if ($_REQUEST['requisicao'] == 'salvar') {
            die("<script>
                        alert('O arquivo da Nota Fiscal é obrigatório!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        }
    }

    // Arquivo do Boletim de Medição
    if ($arquivo_boletim["name"] != '' && $arquivo_boletim["type"] != '' && $arquivo_boletim["size"] != '' && $arquivo_boletim["error"] == 0) {
        $file = new FilesSimec();
        $file->setPasta('obras2');
        $nome_arq = "anexoboletimmedicao_" . $_SESSION['obras2']['obrid'] . '_' . time();
        $file->setUpload($nome_arq, 'arquivo_boletim', false, false);
        $arqid = $file->getIdArquivo();
        $arrRetornoIdsArquivos['peoarqid_boletimmedicao'] = $arqid;
    } else {
        $arrRetornoIdsArquivos['peoarqid_boletimmedicao'] = '';
    }

    return $arrRetornoIdsArquivos;
}

function getArrayDadosForm()
{
    $arArquivos = uploadArquivosExecucaoOrc();
    $arDado = array();
    // Dados do formulário
    $arDado['obrid'] = $_SESSION['obras2']['obrid'];
    $arDado['peodtpagamento'] = ajusta_data($_POST['peodtpagamento']);
    $arDado['peonrordembancaria'] = trim($_POST['peonrordembancaria']);
    $arDado['peovlordembancaria'] = str_replace(' ', '', MoedaToBd($_POST['peovlordembancaria']));
    $arDado['peodtnotafiscal'] = ajusta_data($_POST['peodtnotafiscal']);
    $arDado['peonrnotafiscal'] = trim($_POST['peonrnotafiscal']);
    $arDado['peovlnotafiscal'] = str_replace(' ', '', MoedaToBd($_POST['peovlnotafiscal']));
    $arDado['peovlordembancaria'] = (float)str_replace(' ', '', $arDado['peovlordembancaria']);
    $arDado['peovlnotafiscal'] = (float)str_replace(' ', '', $arDado['peovlnotafiscal']);

    $arDado['peopercentfispagamento'] = MoedaToBd($_POST['peopercentfispagamento']);
    $arDado['peopercentfisacumulado'] = MoedaToBd($_POST['peopercentfisacumulado']);


    $arDado['peoarqid_ordembancaria'] = $arArquivos['peoarqid_ordembancaria'];
    $arDado['peoarqid_notafiscal'] = $arArquivos['peoarqid_notafiscal'];
    $arDado['peoarqid_boletimmedicao'] = (($arArquivos['peoarqid_boletimmedicao'] != '') ? $arArquivos['peoarqid_boletimmedicao'] : 'NULL');
    // Dados de inclusão
    $arDado['peodtinclusao'] = date('Y-m-d');
    $arDado['peodtupdate'] = date('Y-m-d');
    $arDado['usucpf'] = $_SESSION['usucpf'];
    $arDado['peostatus'] = 'A';

    return $arDado;
}




/**
 * Função responsável pela exclusão lógica de determinada medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $medid
 * @return bool
 */
function excluirMedicao($medid){
    global $db;

    $sql = "UPDATE obras2.medicoes SET medstatus = 'I', usucpfinativacao = '{$_SESSION['usucpf']}', meddtinativacao = 'now()' WHERE medid = {$medid}";

    if($db->executar($sql)){
        $sql2 = "UPDATE obras2.medicaocontrato SET mecstatus = 'I', usucpfinativacao = '{$_SESSION['usucpf']}', mecdtinativacao = 'now()' WHERE medid = {$medid}";
        if($db->executar($sql2)){
            $db->commit();
            return true;
        }
    }
    return false;
}

/**
 * Função responsável por verificar se determinada medição possui pendência de preenchimento.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $medid
 * @return bool
 */
function verificaPendenciaMedicao($medid){
    global $db;

    $sql= "SELECT * FROM obras2.medicoes WHERE medid = {$medid}";

    $resp = $db->carregar($sql);
    $resp = current($resp);
    $retorno = false;

    if($resp['mednummedicao'] == "" || $resp['meddtmedicao']== "" || $resp['meddtinicioexec']== "" || $resp['meddtfimexec']== "" || $resp['medvlrmedicao']== ""){
        $retorno = true;
    }
    else{
        $sql= "SELECT * FROM obras2.medicaocontrato WHERE medid = {$medid}";
        $resp2 = $db->carregar($sql);

        if(!$resp2){
            $retorno = true;
        }
        else{
            $resp2 = current($resp2);
            if($resp2['crtid'] == "" || $resp2['mecvlrreferencia'] == ""){
                $retorno = true;
            }
        }
    }

    return $retorno;
}



/**
 * Função responsável por verificar se a medição possui vinculação com alguma nota fiscal.
 * Esta função foi criada para evitar que o usuário exclua uma medição que esteja presente em alguma nota fiscal.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $medid
 * @return bool|mixed|NULL|string
 */
function verificaSeTemNotaFiscal($medid) {

    global $db;
    $sql = "
    SELECT mec.medid
    FROM obras2.notamedicao ntm
      INNER JOIN obras2.medicaocontrato mec ON ntm.mecid = mec.mecid AND mec.mecstatus = 'A'
    WHERE
      ntm.ntmstatus = 'A'
      AND
      mec.medid = {$medid};
    ";

    return $db->pegaUm($sql);

}

