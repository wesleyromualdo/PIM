<?php 


/**
 * Função responsável por cadastrar construtoras extras para o cumprimento do objeto em casos que não houve abertura de
 * obra vinculada mas que possuem mais de uma construtora.
 * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=7995
 * @param $obrid ID da obra.
 * @param $razaosocial Razão social da construtora.
 * @param $cnpj CNPJ da construtora.
 * @return bool
 */
function cadastrarNovaConstrutora($obrid, $razaosocial, $cnpj)
{

    global $db;

    $usucpf = $_SESSION["usucpf"];

    $query = "INSERT INTO 
                obras2.construtoraextra (obrid, cexrazsocialconstrutora, cexnumcnpj, cexusucpf) 
              VALUES ({$obrid}, '{$razaosocial}', '{$cnpj}', '{$usucpf}')";

    if ($db->executar($query)) {
        $db->commit();
        return true;
    }

    return false;
}

/**
 * Função responsável por verificar se uma obra está vencida pela data.
 * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $data Data no formato 'YYYY-mm-dd'.
 * @return bool
 * @deprecated
 */
function verificaVencimento($data)
{

    $dataAtual = new DateTime("now");
    $dataVerifcacao = new DateTime($data);

    if ($dataVerifcacao->diff($dataAtual) > 0) {
        return true;
    }
    return false;
}

/**
 * Função responsável por gravar um novo prazo para o preenchimento da aba.
 * @author: José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=8413
 * @param $data Data no formato 'YYYY-mm-dd'.
 * @return bool
 */
function gravarNovoPrazo($obrid, $novoPrazo)
{

    global $db;

    $objDataAtual = new DateTime('now');
    $novoPrazo = $objDataAtual->add(new DateInterval('P' . $novoPrazo . 'D'));
    $novoPrazo = $novoPrazo->format('Y-m-d');
    $usucpf = $_SESSION['usucpf'];

    $sql = "UPDATE obras2.prazocumprimentoobjeto
            SET
              pcodatavigencia = '{$novoPrazo}',
              pcodataultimaalteracao = 'now()',
              usucpf = '{$usucpf}',
              pcodataprorrogada = true
            WHERE 
              obrid = {$obrid};";

    if ($db->executar($sql)) {
        $db->commit();
        return true;
    }
    return false;
}



function getSqlObrasContrato($obrid) {

	$_sql  = "SELECT ocr.ocrid FROM obras2.obrascontrato ocr
                             WHERE ocr.ocrstatus = 'A' AND ocr.obrid =".$obrid;

	return $_sql;
} 