<?php
	
class PreCronograma extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.precronograma";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prcid' => null, 
									  	'itcid' => null, 
									  	'preid' => null,
    									'prcmes' => null,
    									'prcquinzena' => null
									  );
	
	
	public function carregaPreCronogramaPorQuinzena($preid,$quinzena)
	{
		$arrWhere[] = "preid = $preid";
		$arrWhere[] = "prcquinzena = $quinzena";
		$arrDados = $this->recuperarTodos("itcid,prcmes",$arrWhere);

		if(is_array($arrDados) && count($arrDados) > 0):
			foreach($arrDados as $dados):
				$arrQuinzena[ $dados['itcid'] ][ $dados['prcmes'] ] = "1";
			endforeach;
			return $arrQuinzena;
		else:
			return false;
		endif;
		
	}
	
	public function salvaCronogramaPorQuinzena($preid)
	{
		$sql = "delete from obras.precronograma where preid = $preid;";

		foreach($_POST['item_quinzena_1'] as $itcid => $q1):
			
			foreach($q1 as $mes => $valor):
				if($valor):
					$sql.= "insert into obras.precronograma (itcid,preid,prcmes,prcquinzena) values ('$itcid','$preid','$mes','1');";
				endif;
			endforeach;
		endforeach;
		
		foreach($_POST['item_quinzena_2'] as $itcid => $q2):
			
			foreach($q2 as $mes => $valor):
				if($valor):
					$sql.= "insert into obras.precronograma (itcid,preid,prcmes,prcquinzena) values ('$itcid','$preid','$mes','2');";
				endif;
			endforeach;
		endforeach;

		$this->executar($sql);
		if($this->commit($sql)){
			return true;
		}
		
	}
	
	public function getPreItensComposicaoFilhos($itcid = null, $preid = null, $somaTotal = 0, $ptoid = null, $ptlid = null)
	{
		
		if($ptlid){
			$stWhere .= " and itc.ptlid = {$ptlid}";
		}
		
		$sql = "SELECT
					pretipofundacao
				FROM obras.preobra pre
				WHERE preid = '{$_SESSION['par']['preid']}'";

		$tipoFundacao = $this->pegaUm($sql);
		
		$arTipos = array(2, 7);
		if(!empty($tipoFundacao) && in_array($ptoid, $arTipos)){
			$stWhere .= " AND itctipofundacao = '{$tipoFundacao}' ";
		}
		
// 		$sql = "select distinct
// 					sum(ppovalorunitario*itcquantidade) as valor
// 				from obras.preplanilhaorcamentaria ppo 
// 				inner join obras.preitenscomposicao itc on itc.itcid = ppo.itcid
// 				where 
// 					ppo.preid = $preid 
// 					and (itc.itccodigoitem ilike '{$itcid}.%' or itc.itccodigoitem = '{$itcid}' )
// 					$stWhere";
		$sql = "select
					sum((ppo.ppovalorunitario)*coalesce(pir.pirqtd, itc.itcquantidade)) as total
				from
					obras.preplanilhaorcamentaria ppo 
					inner join obras.preobra pre on pre.preid = {$preid}
					inner join obras.preitenscomposicao itc on itc.itcid = ppo.itcid
					left join obras.preitencomposicao_regiao pir on itc.itcid = pir.itcid and pir.estuf = pre.estuf
				where
					ppo.preid = {$preid}
					and (itc.itccodigoitem ilike '{$itcid}.%' or itc.itccodigoitem = '{$itcid}')
					$stWhere";
 //		ver($sql);
		return $this->pegaUm($sql);
		
//		$sql = "select itcid from obras.preitenscomposicao where itcidpai = '$itcid'  and itcstatus = 'A'";
//		$arrItem = $this->carregar($sql);
//		$arrItem = $arrItem ? $arrItem : array();
//		
//		foreach( $arrItem as $item ){
//			$somaTotal = $this->getPreItensComposicaoFilhos($item['itcid'], $preid, $somaTotal);
//		}
//		$sql = "select 
//					(ppo.ppovalorunitario * itc.itcquantidade) 
//				from 
//					obras.preplanilhaorcamentaria ppo
//				inner join
//					 obras.preitenscomposicao itc ON ppo.itcid = itc.itcid
//				where 
//					ppo.itcid  = {$itcid} 
//				and 
//					ppo.preid = {$preid}";
//		$somaTotal += $this->pegaUm($sql);
//		ver($somaTotal);
//		return $somaTotal;

	}
	
}									  