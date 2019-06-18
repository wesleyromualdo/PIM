<?php 

function salvar(){

	global $db;

	if( $_REQUEST['frpid'] != '' ){

		$obras = new Obras();
		$obras->carregarPorIdCache($_SESSION['obras2']['obrid']);
		$obras->frpid = $_REQUEST['frpid'];
		$obras->stiid = $_REQUEST['stiid'];
		$obras->medidasexcecao = ($_REQUEST['medidasexcecao'] == 'S') ? true : false;

	    $arCamposNulo = array();
		if ( $_REQUEST['tooid'] ){
			$obras->tooid = $_REQUEST['tooid'];
		}else{
			$obras->tooid = null;
			$arCamposNulo[] = 'tooid';
		}

		$obras->salvar(true, true, $arCamposNulo);
		$obras->commit();
?>
	<script>
		alert('Operação realizada com sucesso!');
		window.location.href = window.location.href;
	</script>
<?php
	}
}

function exibeObrasConvenio(){
	global $db;

	include_once(APPRAIZ . "/obras2/modulos/principal/exibeObrasConvenio.inc");
	die;
}

function buscaTipoESituacaoInstrumento($obrid,$tooid,$obridPar3 = null){

    global $db;
    if($tooid != 5 && $tooid) {
        $sql = "SELECT e.prfid FROM obras2.obras o LEFT JOIN obras2.empreendimento e ON e.empid = o.empid WHERE o.obrid = $obrid";
        $prfid = $db->pegaUm($sql);
        if($prfid == 48) {
            $frpid = 9;
        }
    } else {
        $frpid = 9;
    }
    if($tooid == 1) {
        #pac
        $sql = <<<SQL
            -- PAC confirmar se o PAC sempre fio termo de compromisso
            SELECT

                '7' AS frpid -- TERMO COMPROMISSO
            FROM execucaofinanceira.processos p
            INNER JOIN execucaofinanceira.tipoexecucao t  ON t.tprid = p.tprid
            INNER JOIN execucaofinanceira.tipoprocesso tp ON tp.tipid = p.tipid
            INNER JOIN par.processoobra po ON po.pronumeroprocesso = p.pronumeroprocesso
            INNER JOIN par.processoobraspaccomposicao c ON c.proid = po.proid
            INNER JOIN obras.preobra pre ON pre.preid = c.preid
            INNER JOIN obras2.obras o ON o.preid = pre.preid AND o.obridpai is null AND o.obrstatus = 'A'
            WHERE po.prostatus = 'A' AND o.obrid = $obrid
SQL;

    } else if($tooid != null) {
        #par
        $sql = <<<SQL
            -- Tipo de Instrumento
            -- PAR
            SELECT
                CASE WHEN po.sisid = 57 THEN '2' ELSE '7' END AS tipo --CONVENIO, TERMO COMPROMISSO
            FROM execucaofinanceira.processos p
            INNER JOIN execucaofinanceira.tipoexecucao t  ON t.tprid = p.tprid
            INNER JOIN execucaofinanceira.tipoprocesso tp ON tp.tipid = p.tipid
            INNER JOIN par.processoobraspar po ON po.pronumeroprocesso = p.pronumeroprocesso
            INNER JOIN par.processoobrasparcomposicao c ON c.proid = po.proid
            INNER JOIN obras.preobra pre ON pre.preid = c.preid
            INNER JOIN obras2.obras o ON o.preid = pre.preid AND o.obridpai is null AND o.obrstatus = 'A'
            WHERE po.prostatus = 'A' AND o.obrid = $obrid
SQL;
        if(!empty($obridPar3)){
            $sql = "SELECT
                    CASE 
                      WHEN sisid = 57 THEN '2' ELSE '7' END AS tipo --CONVENIO, TERMO COMPROMISSO
                    FROM execucaofinanceira.processos p
                    INNER JOIN execucaofinanceira.tipoexecucao t  ON t.tprid = p.tprid
                    INNER JOIN execucaofinanceira.tipoprocesso tp ON tp.tipid = p.tipid
                    INNER JOIN par3.processoobracomposicao pc ON pc.proid =	p.proid
                    INNER JOIN par3.obra po ON po.obrid = pc.obrid 
                    INNER JOIN obras2.obras o ON o.obrid_par3 = po.obrid AND o.obrstatus = 'A'
                    WHERE pc.pocstatus = 'A' AND o.obrid = {$obrid}";
        }

    }

    $retorno = $db->pegaLinha("SELECT dt_fim_vigencia_termo as dtvigencia, CASE WHEN dt_fim_vigencia_termo >= NOW() THEN 1 ELSE 2 END stiid FROM obras2.v_vigencia_obra_2016 WHERE obrid = $obrid");

    if(!empty($obridPar3)){
        $sql = "SELECT  
                TO_CHAR(d.dotdatafimvigencia,'dd-mm-yyyy') AS  dtvigencia,
                CASE 
                    WHEN d.dotdatafimvigencia >= NOW() THEN 1 ELSE 2 
                END stiid
                FROM par3.processo  p
                INNER JOIN par3.processoobracomposicao pc ON pc.proid = p.proid  
                INNER JOIN par3.obra po ON po.obrid = pc.obrid
                INNER JOIN par3.documentotermo d ON d.proid = p.proid and d.dotstatus = 'A'
                INNER JOIN obras2.obras o ON o.obrid_par3 = po.obrid
                WHERE o.obrid =  {$obrid}
                AND p.prostatus = 'A'";
        $retorno = $db->pegaLinha($sql);
    }
    $stiid = $retorno['stiid'];
    $vigencia = $retorno['dtvigencia'];

    return array('frpid' => $frpid, 'stiid' => $stiid, 'dtvigencia' => $vigencia);

}


function getSqlConvenio($obrid = null)
{

	return <<<SQL
		SELECT
		        b.*,
		        CASE WHEN b.dcoid IS NULL THEN a.obrnumprocessoconv ELSE b.dcoprocesso END as dcoprocesso,
		        CASE WHEN b.dcoid IS NULL THEN a.obranoconvenio ELSE b.dcoano END as dcoano,
		        CASE WHEN b.dcoid IS NULL THEN a.numconvenio ELSE b.dcoconvenio END as dcoconvenio
		FROM
		        obras2.obras a
		LEFT JOIN painel.dadosconvenios b on b.dcoprocesso = Replace(Replace(Replace(a.obrnumprocessoconv,'.',''),'/',''),'-','')
		WHERE
		        obrid = $obrid
SQL;

}


function getSqlObrasComMesmoConvenio($numconvenio = null,  $obranoconvenio = null) 
{
	return <<<SQL
	     SELECT
	                count(*)
	        FROM
	                obras2.obras a
	        WHERE
	                a.numconvenio = '{$numconvenio}'
	            AND a.obranoconvenio = '{$obranoconvenio}'

SQL;


}



function getSQLSaldoConta($dcoprocesso){

	return <<<SQL
	SELECT
                substring(dfidatasaldo::varchar,6,2) || '/' ||
                substring(dfidatasaldo::varchar,0,5), SUM(dfisaldoconta) as dfisaldoconta,
                (SUM(dfisaldofundo) + SUM(dfisaldopoupanca) + SUM(dfisaldordbcdb)) AS totalaplicacao,
                (SUM(dfisaldoconta) + SUM(dfisaldofundo) + SUM(dfisaldopoupanca) + SUM(dfisaldordbcdb)) AS totalconta
          FROM
                painel.dadosfinanceirosconvenios dfi
          WHERE
                dfiprocesso = '{$dcoprocesso}'
          GROUP BY
                dfidatasaldo
          ORDER BY
                dfidatasaldo
SQL;
}


function getSqlDadosBancarios($dcoprocesso) {

	return  <<<SQL
    SELECT
                            to_char(drcdatapagamento, 'DD/MM/YYYY') as datapagamento,
                            drcordembancaria,
                            drcvalorpago,
                            'Banco: ' || drcbanco || '<br>Ag: ' || drcagencia || '<br>CC: ' || drcconta as drcdadosbancarios
                      FROM
                           painel.dadosrepassesconvenios drc
                      WHERE
                           drcprocesso = '{$dcoprocesso}'
                     ORDER BY
                            drcdatapagamento

SQL;
}


function getSQLTermoObra($preid, $obridPar3 ){
if(!empty($obridPar3)){

	return <<<SQL
		SELECT
                CASE
                    WHEN dv.dtvid IS NOT NULL THEN
                        TRUE
                    ELSE FALSE
                END AS terassinado,
                TO_CHAR(dp.dotdatainclusao,'dd/mm/YYYY') AS terdatainclusao,
                dt.arqid

                FROM par3.termocomposicao tc
                INNER JOIN par3.documentotermo dp ON dp.dotid = tc.dotid
                LEFT JOIN par3.documentotermovalidacao dv ON dv.dotid = dp.dotid
                LEFT JOIN par3.documentotermoarquivo dt ON dt.dotid = dv.dotid
                WHERE tc.obrid = '3046429' AND dp.dotstatus = 'A' AND dt.dtastatus = 'A'
SQL;
} else {

	return <<<SQL
		SELECT * FROM (SELECT terassinado,
		       to_char(terdatainclusao,'dd/mm/YYYY') as terdatainclusao,
		       tcp.arqid FROM par.termoobra teo
		        INNER JOIN par.termocompromissopac tcp ON teo.terid = tcp.terid
		        WHERE teo.preid = {$preid} AND terstatus = 'A'
		        ORDER BY  teo.terid DESC
		        ) as f
		 UNION

	    SELECT
	        CASE WHEN dv.dpvid IS NOT NULL THEN TRUE ELSE FALSE END terassinado,
	        to_char(dp.dopdatainclusao,'dd/mm/YYYY') as terdatainclusao,
	        dp.arqid_documento as arqid
	    FROM
	        par3.termocomposicao tc
	    INNER JOIN par.documentopar  dp ON dp.dopid = tc.dopid
	    LEFT JOIN par.documentoparvalidacao dv ON dv.dopid = dp.dopid
	    WHERE tc.preid = {$preid} AND dp.dopstatus = 'A'
SQL;
}
}


function getSQLEmpenhoObra($preid, $obridPar3 ){
if(!empty($obridPar3)){
        return <<<SQL
        SELECT 
                empenho_original AS empnumero,
                vlrempenho AS empvalorempenho
                FROM par3.v_saldo_empenho_por_obra WHERE obrid = {$obridPar3} AND vlrempenho > 0
SQL;
} else {
        return <<<SQL
        SELECT
                                ne as empnumero,
                                valorempenho as empvalorempenho
                                FROM par.v_saldo_obra_por_empenho
                                WHERE preid = '" . $preid . "' AND valorempenho > 0
SQL;
}
}	
 



function   getSQLPagamentoObra ($preid, $obridPar3){

    $camposDif = "pro.proagencia, 
                  pro.probanco,";
    $campos = "p.pagvalorparcela, 
                p.pagnumeroob, 
                to_char(p.pagdatapagamento,'dd/mm/YYYY') as pagsolicdatapagamento, 
                to_char(p.pagdatapagamentosiafi ,'dd/mm/YYYY') as pagdatapagamento";


    $tabela = "par.v_saldo_obra_por_empenho emp";

    $join = "INNER JOIN (

                        SELECT pronumeroprocesso, probanco, proagencia FROM par.processoobra pro WHERE pro.prostatus = 'A'
                        UNION
                        SELECT pronumeroprocesso, probanco, proagencia FROM par.processoobraspar pro WHERE pro.prostatus = 'A'

                ) pro ON pro.pronumeroprocesso = emp.processo
             INNER JOIN par.pagamento p ON p.empid = emp.empid";

    $and = "emp.preid = '" . $preid . "'
            AND emp.valorempenho > 0 ";

    if(!empty($obridPar3)){
        $camposDif = "emp.proagencia, 
                   emp.probanco,";

        $tabela = "par3.v_saldo_empenho_por_obra emp";

        $join = "INNER JOIN par3.pagamento p ON p.empid = emp.empid";

        $and = "emp.obrid = {$obridPar3}
                AND p.pagstatus = 'A'
                AND emp.vlrempenho > 0";
    }


    $sql = "SELECT 
                $camposDif
                $campos
                FROM $tabela
                  $join
                WHERE 
                    $and
                    AND p.pagstatus='A'  
                    and p.pagsituacaopagamento not ilike '%CANCELADO%'
                    and p.pagsituacaopagamento not ilike '%vala%'
                    and p.pagsituacaopagamento not ilike '%devolvido%'
                    and p.pagsituacaopagamento not ilike '%VALA CENTRO DE GESTÃO%'
                ORDER BY emp.empid, p.pagdatapagamento ASC";


    return $sql;
    
}

function getSqlEmpNumeroProcesso($empnumero){

return  <<<SQL
SELECT 
    e.empnumeroprocesso 
    FROM  par.empenho e
    WHERE e.empnumero = '{$empnumero}' AND empstatus = 'A'
SQL;
}


function getSqlDadosEmpenho($empnumero){

return  <<<SQL
SELECT 
    e.empnumeroprocesso, 
    e.empcnpj
    FROM  par.empenho e
    WHERE e.empnumero = '{$empnumero}' AND empstatus = 'A'
SQL;
}



function getSqlDadosFinanceiros(){

    return <<<SQL
SELECT
            '<span class="processo_detalhe" >'||dfi.dfiprocesso||'</span>' as dfiprocesso,
            dfi.dficnpj,
            iue.iuenome as razao_social,
            dfi.dfibanco,
            dfi.dfiagencia,
            dfi.dficonta,
            dfi.dfisaldoconta,
            dfi.dfisaldofundo,
            dfi.dfisaldopoupanca,
            dfi.dfisaldordbcdb,
            dfi.dfimesanosaldo
    FROM painel.dadosfinanceirosconvenios dfi
    left join par.instrumentounidadeentidade iue on iue.iuecnpj = dfi.dficnpj
    WHERE dfiprocesso = '{$processo}' AND dficnpj= '{$cnpj}'
    ORDER BY dfi.dfidatasaldo DESC;
SQL;
}