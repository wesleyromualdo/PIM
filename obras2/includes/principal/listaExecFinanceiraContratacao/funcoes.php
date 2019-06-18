<?php 


/**
 * Função responsável pela exclusão lógica de determinada construtora extra.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $cexid
 * @return bool
 */
function excluirConstrutora($cexid){
    global $db;
    $cpf = $_SESSION['usucpf'];
    if($cpf)
    {
        if(verificaSeTemMedicao($cexid)){
            die('Não foi possuível excluir construtora, pois existem medições cadastradas');
        }
        else {
            $sql = "UPDATE obras2.construtoraextra SET cexstatus = 'I', usucpfinativacao = '{$cpf}', cexdtinativacao = 'now()' WHERE cexid = {$cexid}";

            if ($db->executar($sql)) {
                $sql2 = "UPDATE obras2.contratoconstrutoraextra SET ccestatus = 'I',  usucpfinativacao = '{$cpf}', ccedtinativacao = 'now()' WHERE cexid = {$cexid}";

                if ($db->executar($sql2)) {
                    $db->commit();
                    die('Construtora excluída com sucesso');
                }
            }
        }
    }
    else
    {
        die('Erro ao excluir a Construtora ');
    }
    return false;
}

/**
 * Função responsável por verificar se a construtora extra já possui medição cadastrada.
 * Esta função foi criada para evitar que o usuário exclua uma construtora que possui medição vinculada a ela.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $cexid
 * @return bool
 */
function verificaSeTemMedicao($cexid){
    global $db;

    $sql = "SELECT medid FROM
              obras2.contratoconstrutoraextra cce
            INNER JOIN obras2.medicoes med ON cce.cceid = med.cceid AND med.medstatus = 'A'
            WHERE cexid = {$cexid}";

    $resultado = $db->pegaUm($sql);

    $retorno = true;
    if(!$resultado){
        $retorno = false;
    }

    return $retorno;
}

/**
 * Função responsável pela exclusão lógica de determinada construtora extra.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $cexid
 * @return bool
 */
function excluirAditivo($crtid){
    global $db;

    if(verificaSeTemMedicaoAditivo($crtid)){
        die('Não foi possuível excluir o aditivo, pois existem medições cadastradas');
    }
    else {
        $sql = "UPDATE obras2.contrato SET crtstatus = 'I' WHERE crtid = {$crtid}";
        if ($db->executar($sql)) {
            if($db->commit()) {
                die('Aditivo excluído com sucesso');
            } else{
                die('Erro ao excluir a Construtora ');
            }
        }
    }

    return false;
}

function verificaSeTemMedicaoAditivo($crtid){
    global $db;

    $sql = "SELECT mecid FROM obras2.medicaocontrato WHERE crtid = {$crtid} AND mecstatus = 'A'";

    $resultado = $db->pegaUm($sql);

    $retorno = true;
    if(!$resultado){
        $retorno = false;
    }

    return $retorno;
}

