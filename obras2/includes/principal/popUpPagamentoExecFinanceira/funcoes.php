<?php

function verificaValorTotalAtual( $ntfid, $dados )
{
	global $db;
	// Total que será inserido, zerando variável
	$pagoTotalInsercao = 0;
	// Caso exista a nota fiscal, que é fundamental para funcionar
	if( $ntfid )
	{
		// pego todos os documentos que foram inseridos
		$arrDocs  = $dados['ntfid'];
		$arrDocs = (is_array($arrDocs)) ? $arrDocs : Array();
		// se houverem notas
		if( (count($arrDocs) > 0 ) )
		{
			// Ciclo elas
			foreach($arrDocs as $k => $v)
			{
				// Verifico se é a verificação atual
				if( $v == $ntfid )
				{
					// Zero o valor do pagamento atual
					$pagoAgora	= 0 ;
					// CArrego o valor do pagamento atual e trato os pontos
					$pagoAgora	=	$dados["dotvalorpagodoc"][$k];
					$pagoAgora 	=	str_replace('.', '', $pagoAgora);
					$pagoAgora 	=	str_replace(',', '.', $pagoAgora);
					// Pego todas as somas para esta nota específica
					$pagoTotalInsercao = $pagoAgora + $pagoTotalInsercao; 
				}
			}
		}
		else
		{
			return false;
		}
		// Pegp p valor da nota que está sendo validada
		$valorTotalNota = $db->pegaUm("SELECT (COALESCE(SUM(ntfvalornota),0))  FROM obras2.notafiscal WHERE ntfid = {$ntfid} AND ntfstatus = 'A' ");
		
		// Trata para ignorar os que tenham sido excluídos na edição
		$clausulaExluidos = '';
		
		if( $dados['requisicao'] == 'editar' )
		{
			// Carrega o array com possíveis excluídas pois precisam ser ignoradas na soma
			$arrExcluidos = ($dados['documentos_excluidos']) ? $dados['documentos_excluidos'] : Array();
			// PEga e monta a estrutura para montar a clausula
			$strObrids = implode(", ", $arrExcluidos);
			//Caso tenha trago algo.
			if($strObrids != '')
			{	// Monta a clausula
				$clausulaExluidos = "AND dotid not in( {$strObrids} )";
			}
		
		}
		// Pega a soma total paga para aquela obra, ignorando possíveis edições de exclusão
		$valorPagoTotalNota = $db->pegaUm("SELECT (COALESCE(SUM(dotvalorpagodoc),0)) FROM obras2.documentotransacao WHERE ntfid = {$ntfid} AND dotstatus = 'A' {$clausulaExluidos} ");
		// CAso o valor da inserção mais o valor já pago na nota forem maiores que o valor total da nota ele retorna erro
        $pagoTotalInsercao = (float)$pagoTotalInsercao;
        $valorPagoTotalNota = (float)$valorPagoTotalNota;
        $valorTotalNota = (float)$valorTotalNota;
        $total = $pagoTotalInsercao + $valorPagoTotalNota;

		if( round($total, 2) > round($valorTotalNota, 2) )
		{
			return false;
		}
		else 
		{
			return true;
		}
	}
	else 
	{
		return false;
	}
}



function verificaValoresTotais( $dados )
{
	global $db;
	// dECLARA VARIÁveis necessárias para tratar os valores
	$pagoTotalInsercao	= 0; 
	$pgtvalortransacao	=	$dados["pgtvalortransacao"];
	$pgtvalortransacao 	=	str_replace('.', '', $pgtvalortransacao);
	$pgtvalortransacao 	=	str_replace(',', '.', $pgtvalortransacao);
	// Pega o array dos que estão sendo inseridos atualmente
	$arrDocs  = $dados['ntfid'];
	$arrDocs = (is_array($arrDocs)) ? $arrDocs : Array();
	// Variáveis para tratamento do editar
	$pagosAntes = 0;
	$pgtid 		= $dados["pgtid"];
	$clausulaExluidos = "";
	// Caso seja editar
	if( $dados['requisicao'] == 'editar' && ($pgtid) )
	{
		// Carrega o array com possíveis excluídas pois precisam ser ignoradas na soma
		$arrExcluidos = ($dados['documentos_excluidos']) ? $dados['documentos_excluidos'] : Array();
		
		// PEga e monta a estrutura para montar a clausula
		$strDotids = implode(", ", $arrExcluidos);
		// Caso tenha trago algo.
		if($strDotids != '')
		{
			// Monta clausula
			$clausulaExluidos = "AND dotid not in( {$strDotids} )";
		}
		// Pego o total que já havia sido pago nos inserts anteriores
		$pagosAntes = $db->pegaUm("SELECT (COALESCE(SUM(dotvalorpagodoc),0)) FROM obras2.documentotransacao WHERE pgtid = {$pgtid} AND dotstatus = 'A' {$clausulaExluidos} ");
	
	}
	// CAso tenha novos documentos para serem inseridos e a transação for maior que 0
	if((count($arrDocs) > 0 ) && ($pgtvalortransacao > 0) )
	{
		// Vou de linha por linha dos documentos
		foreach($arrDocs as $k => $v)
		{
			// PEgo as variáveis da nota
			$ntfid = $dados['ntfid'][$k];
			// Aqui eu verifico se não paguei mais que o valor total da nota fiscal
			$totalUnicoOk = verificaValorTotalAtual( $ntfid, $dados );
			// Caso este nó esteja correto ele continua a soma, senão retorna o erro e redireciona
			if( $totalUnicoOk )
			{
				$pagoAgora	= 0 ;
				$pagoAgora	=	$dados["dotvalorpagodoc"][$k];
				$pagoAgora 	=	str_replace('.', '', $pagoAgora);
				$pagoAgora 	=	str_replace(',', '.', $pagoAgora);
					
				$pagoTotalInsercao = $pagoAgora + $pagoTotalInsercao;
			}
			else 
			{
				$numNota = $db->pegaUm(" SELECT ntfnumnota from obras2.notafiscal  where ntfid = {$ntfid} AND ntfstatus = 'A' "); 
				echo "<script>
		                    alert('Os valores informados no pagamento da nota {$numNota} ultrapassam seu valor original.');
		                    window.location.href = window.location.href;
		                  </script>";
    			exit;
			}
		}
		
		// Aqui eu considero o total da inserção e o total que já foi talvez pago antes, já ignorando possíveis excluídos, este valor tem que ser igual ao informado como valor da transação
		
		if( ((string)($pagoTotalInsercao + $pagosAntes)) == ((string)$pgtvalortransacao) )
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}
	else
	{
		if( $dados['requisicao'] == 'editar' && ($pgtid) )
		{
			if( $pagosAntes == $pgtvalortransacao )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else 
		{
			return false;
		}
	}
	
	return false;
	
}





/**
 * Função responsável por carregar o elemento select com os contratos ou aditivos de valor.
 * @author José Carlos <jose.costa@castgroup.com.br>, Sérgio Henrique <sergio.henrique@castgroup.com.br>
 * @link https://gestaoaplicacoes.mec.gov.br/plugins/tracker/?aid=15249
 * @param $obrid Id da obra.
 * @param $medid Id da medição.
 * @param $entid Id da empresa quando a origem da obra for 'Entidade'.
 * @param $cexid Id da empresa quando a origem da obra for 'Construtuora extra (Cumprimento do Objeto)'.
 * @return array|mixed|NULL
 */
function carregarContratos($obrid, $crtid, $cceid, $medid=null) {

	global $db;

	$arrContratos = array();

	if (!$crtid && !$cceid) 
	{
		return $arrContratos;
	}

	$execucaoFinanceira = new ExecucaoFinanceira();
	$arrObrids = $execucaoFinanceira->retornaObrids($obrid);
	$strObrids = implode(",", $arrObrids);
	
	if (ctype_digit($crtid) && !$cceid) 
	{
		$sql = "
				SELECT
					'ori_' || crt.crtid as codigo,
					CASE WHEN ttaid IS NULL THEN
						'Contrato Original' || ' - ' || to_char(crt.crtdtassinatura, 'DD/MM/YYYY')
					WHEN ttaid = 2 THEN
                        'Aditivo de Valor' || ' - ' || to_char(crt.crtdtassinaturaaditivo, 'DD/MM/YYYY')
					ELSE
						'Aditivo de Valor/Prazo' || ' - ' || to_char(crt.crtdtassinaturaaditivo, 'DD/MM/YYYY')
					END as descricao
				FROM 
					obras2.contrato crt
				WHERE 
					(
						crt.crtid  = {$crtid}
						or
						(crt.crtidpai = (select crtidpai from obras2.contrato where crtid = {$crtid} order by crtid limit 1) AND ttaid in (2,3))
						or
                        (crt.crtid = (select crtidpai from obras2.contrato where crtid = {$crtid} order by crtid limit 1) AND ttaid in (2,3))
					    or
                        (crt.crtidpai_execfinceira = {$crtid})
                    ) AND crtstatus = 'A' order by descricao desc, crt.crtdtassinatura 
            ";

		if ($medid) { //@todo
			$sql .= "AND c.crtid NOT IN (SELECT mec.crtid
			FROM obras2.medicaocontrato mec
			WHERE mec.mecstatus = 'A' AND mec.medid = {$medid})";
		}
	}
	elseif (ctype_digit($cceid) && !$crtid) 
	{
		$sql = "
			SELECT
				'cex_' ||cce.cceid AS codigo,
				CASE WHEN cce.ttaid IS NULL THEN
					'Contrato Original' || ' - ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY')
				ELSE
					'Aditivo de Valor' || ' - ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY')
				END as descricao
			FROM 
		  		obras2.contratoconstrutoraextra cce
			WHERE 
				ccestatus = 'A'
			AND
			(
				cce.cceid  = {$cceid}
				or
				(cce.cceid_pai = {$cceid} AND ttaid in (2,3))
			)
		";

		if ($medid) { //@todo
			$sql .= "
			AND cce.cceid NOT IN (
			SELECT mec.cceid
			FROM obras2.medicaocontrato mec
			WHERE mec.mecstatus = 'A' AND mec.medid = {$medid}
			)"
			;
		}
	}

	$arrContratos = $sql ? $db->carregar($sql) : false;
	if (!$arrContratos) {
		$arrContratos = array();
	}
	
	return $arrContratos;
}

function montaComboContratosAditivos($obrid, $entid, $cxeid, $medid=null) {

	global $db;

	$arrContratosAditivos = carregarContratos($obrid, $entid, $cxeid, $medid);

	return $db->monta_combo("contratos", $arrContratosAditivos, "S", "Selecione...", "", '', '', '', 'S', 'contratos');
}

function recuperaDadosArquivo($arqid){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$files = new FilesSimec();

	return $files->getDadosArquivo($arqid);
}

function montaComboEmpresaContratada($obrid, $selectedValue=null) {

	global $db;

	$execucaoFinanceira = new ExecucaoFinanceira();
	$arrObrids = $execucaoFinanceira->retornaObrids($obrid);
	$strObrids = implode(",", $arrObrids);

	/*
	 * Na consulta é realizada uma concatenação dos IDs com os prefixos 'ent_' ou 'cex_'. Isso serve para identificar se
	 * a empresa foi cadastrada como entidade ou como construtora extra (Cumprimento do Objeto).
	*/
	$sql = "
	SELECT  
		CASE WHEN ttaid IS NULL THEN
			'ori_' || crt.crtid 
		ELSE
			(SELECT 'ori_' || c2.crtid  FROM obras2.contrato c2 WHERE c2.crtidpai = crt.crtid AND c2.ttaid is NULL AND c2.crtstatus = 'A' LIMIT 1)
		END as codigo,
		'(' ||
          (SELECT substr(cnpj, 1, 2) || '.' || substr(cnpj, 3, 3) || '.' || substr(cnpj, 6, 3) || '/' || substr(cnpj, 9, 4) ||
                  '-' || substr(cnpj, 13) AS cnpj
           FROM (SELECT cast(ent.entnumcpfcnpj AS VARCHAR) AS cnpj) a) || ') ' ||
          CASE WHEN ent.entnome NOTNULL THEN
            ent.entnome
          ELSE
            '-'
          END
          || ' (Contrato ' ||
          CASE WHEN crt.crtnumero NOTNULL THEN
            '- nº ' || crt.crtnumero || ' - '
          ELSE
            '- '
          END ||
          to_char(crt.crtdtassinatura, 'DD/MM/YYYY') || ')' AS descricao
		  FROM obras2.contrato crt 
		INNER JOIN entidade.entidade ent ON ent.entid = crt.entidempresa AND entstatus = 'A'
		WHERE 
			crt.crtid in  (SELECT DISTINCT
		                        crtid
		                FROM
		                        obras2.obrascontrato
		                WHERE
		                        ocrstatus = 'A'
		                        AND obrid in ( {$strObrids} ) 
		)
		and crtstatus = 'A'
		
		  UNION ALL
		        
		SELECT
		  'cex_' ||cce.cceid AS codigo,
		  '(' ||
		  (
		    SELECT substr(cnpj, 1, 2) || '.' || substr(cnpj, 3, 3) || '.' ||
			   substr(cnpj, 6, 3) || '/' || substr(cnpj, 9, 4) || '-' ||
			   substr(cnpj, 13) AS cnpj
		    FROM (SELECT cast(cex.cexnumcnpj AS VARCHAR) AS cnpj) a
		  ) || ') ' || cex.cexrazsocialconstrutora ||' (Contrato - nº ' || cce.ccenumero || ' - ' || to_char(cce.ccedataassinatura, 'DD/MM/YYYY') ||
		  ')'                 AS descricao
		FROM obras2.construtoraextra cex
		  INNER JOIN obras2.contratoconstrutoraextra cce ON cce.cexid = cex.cexid
		WHERE cex.cexstatus = 'A'
		      AND cce.ccedataassinatura IS NOT NULL
		      AND cce.ttaid IS NULL
		      AND
		      1=1 --cex.obrid IN ( {$strObrids} ) 
		      AND cce.ccestatus = 'A'
		      limit 10
	";

	return $db->monta_combo("empresaContratada", $sql, "S", "Selecione...", "montaComboContratos", '', '', '', 'S', 'empresaContratada', '',  $selectedValue);
	

}


function getSqlPagamentoTransacao($pgtid ) {
		return <<<SQL
		SELECT 
			to_char(pgt.pgtdtpagamento,'DD/MM/YYYY') as pgtdtpagamento,
			pgtnumtransacao,
			tptid,
			pgtvalortransacao,
			arqid,
			entid,
			cexid,
			crtid,
			cceid
		FROM
			obras2.pagamentotransacao pgt
		
		WHERE 
			pgt.pgtid = {$pgtid}
		AND
			pgtstatus = 'A'

SQL;

	}

function getDocumentoTransacao($pgtid) {
		return <<<SQL
		SELECT
				dot.dotid,
				CASE WHEN dot.crtid NOTNULL THEN
				  'ori_' || dot.crtid
                ELSE
                  'cex_' || dot.cceid
                END as contrato,
				ntf.ntfid,
				ntf.ntfnumnota as numero_documento,
				'Nota Fiscal' as tipo_documento,
				ntf.ntfvalornota as valor_documento,
				dot.dotvalorpagodoc
			FROM
				obras2.documentotransacao dot
				
				LEFT JOIN obras2.contratoconstrutoraextra  cce ON cce.cceid = dot.cceid
				LEFT JOIN obras2.contrato  crt ON crt.crtid = dot.crtid
				INNER JOIN obras2.notafiscal ntf ON ntf.ntfid = dot.ntfid
			WHERE 
				pgtid = {$pgtid}
			AND dotstatus = 'A'
SQL;
	
	}