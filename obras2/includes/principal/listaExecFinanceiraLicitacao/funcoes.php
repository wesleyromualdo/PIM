<?php



/**
 * Função responsável pela exclusão lógica de determinada medição.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $medid
 * @return bool
 */
function excluirLicitacao($lieid){
	global $db;

	$sql = "UPDATE obras2.licitacaoextra SET liestatus = 'I', usucpfinativacao = '{$_SESSION['usucpf']}', liedatainativacao = 'now()' WHERE lieid = {$lieid}";

	if($db->executar($sql)){
		$sql2 = "UPDATE obras2.arquivolicitacaoextra SET alestatus = 'I' WHERE lieid = {$lieid}";
		if($db->executar($sql2)){
			$db->commit();
			return true;
		}
	}
	return false;
}

function verificaSeTemContrato($lieid) {

    global $db;
    $sql = "SELECT lieid FROM obras2.construtoraextra WHERE lieid = {$lieid} AND cexstatus = 'A'";

    return $db->pegaUm($sql);

}
 


function getSqlLicitacaoextra($strObrids, $btnExcluir, $btnEditar){

	$_sql =<<<SQL
	select 
		' <center> ' || {$btnExcluir}
		|| {$btnEditar}
		|| '<img src="/imagens/icone_lupa.png" border=0 title="Exibir" onclick="visualizarLicitacao(' || lieid || ')"> </center>'
		as acoes,
		'<center><b>'|| obrid ||'</b></center>',
		'<center>'||mol.moldsc ||'</center>' as modalidade,
		lienumero as numero
	FROM 
		obras2.licitacaoextra lie
	INNER JOIN obras2.modalidadelicitacao mol ON mol.molid = lie.molid
	WHERE obrid in ({$strObrids})
	AND liestatus = 'A'

SQL;

	return  $_sql;
}