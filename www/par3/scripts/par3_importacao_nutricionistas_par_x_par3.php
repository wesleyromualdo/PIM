<?php
//ini_set('max_execution_time', 0);
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// CPF do administrador de sistemas
$_SESSION['baselogin'] == "simec_desenvolvimento";
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

$sql = "SELECT snid FROM par3.situacaonutricionista LIMIT 1";

$snid = $db->pegaUm($sql);

if($snid != ''){
    $sql = "INSERT INTO par3.situacaonutricionista(snid, sndescricao)
            SELECT snid, sndescricao FROM par.situacaonutricionista;";

    $db->executar($sql);
    $db->commit();
}

if($_REQUEST['inuid']){
    $filtraUnidade = "AND inuid = {$_REQUEST['inuid']}";
}

$sql = "SELECT DISTINCT vncpf FROM par.vinculacaonutricionista
        WHERE
            vnstatus = 'A'
            AND vncpf NOT IN (SELECT entcpf FROM par3.instrumentounidade_entidade WHERE entcpf IS NOT NULL)
            $filtraUnidade";

$arrCpfs = $db->carregarColuna($sql);
$arrCpfs = is_array($arrCpfs) ? $arrCpfs : array();

$qtdEnt   = 0;
$qtdNutri = 0;

foreach($arrCpfs as $cpf){

    $sql = "SELECT DISTINCT
            	inu.inuid,
            	CASE WHEN vn.dutid = 11 THEN 7 ELSE 8 END as tenid,
            	vn.vncpf as entcpf,
            	e.entemail as entemail,
            	e.entnome as entnome,
            	'retorno do script' as entid,
            	dn.dnemailalternativo as dnemailalternativo,
            	dn.dnnomemae as dnnomemae,
            	vn.snid,
            	dn.dncrn,
            	dn.dncrnuf
            FROM
            	par.vinculacaonutricionista vn
            INNER JOIN entidade.entidade 		e   ON e.entnumcpfcnpj = vn.vncpf
            INNER JOIN par.dadosnutricionista 	dn  ON dn.dncpf = vn.vncpf
            INNER JOIN par.instrumentounidade 	iu  ON iu.inuid = vn.inuid
            INNER JOIN par3.instrumentounidade 	inu ON (inu.muncod = iu.muncod AND inu.itrid = 2) OR (inu.estuf = iu.estuf AND inu.itrid <> 1)
            WHERE
            	vn.dutid IN (11, 12)
            	AND vn.vnstatus = 'A'
            	AND vn.vncpf = '$cpf'
            ORDER BY
            	e.entnome";

    $nutricionista = $db->pegaLinha($sql);

    $sql = "SELECT entid FROM par3.instrumentounidade_entidade WHERE entcpf = '$cpf'";

    $entid = $db->pegaUm($sql);

    if(!$entid && $nutricionista['inuid'] != ''){
        $sql = "INSERT INTO par3.instrumentounidade_entidade(inuid, tenid, entcpf, entemail, entnome)
                VALUES({$nutricionista['inuid']}, {$nutricionista['tenid']}, '{$nutricionista['entcpf']}', '{$nutricionista['entemail']}', '{$nutricionista['entnome']}')
                RETURNING entid;";
        $entid = $db->pegaUm($sql);
        $db->commit();
        $qtdEnt++;
    }

    $sql = "SELECT TRUE FROM par3.dadosnutricionista WHERE entid = $entid";

    $booNutricionista = $db->pegaUm($sql);

    if($booNutricionista != 't'){

        $nutricionista['snid'] = $nutricionista['snid'] != '' ? $nutricionista['snid'] : 'null';
        $nutricionista['dncrn'] = $nutricionista['dncrn'] != '' ? $nutricionista['dncrn'] : 'null';

        $sql = "INSERT INTO par3.dadosnutricionista(usucpf, dnemailalternativo,
                                                    dnnomemae, snid, dncrn, dncrnuf,
                                                    dnstatus)
                VALUES($cpf, '{$nutricionista['dnemailalternativo']}',
                       '{$nutricionista['dnnomemae']}', {$nutricionista['snid']},
                       {$nutricionista['dncrn']}, '{$nutricionista['dncrnuf']}', 'A');";

        $db->executar($sql);
        $db->commit();
        $qtdNutri++;
    }
}

echo "Totais de registros alterados<br>";
echo "Entidade: $qtdEnt<br>";
echo "Nutricionista: $qtdNutri<br>";
?>