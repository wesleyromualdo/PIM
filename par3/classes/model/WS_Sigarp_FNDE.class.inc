<?php
class Par3_Model_WS_Sigarp_FNDE extends Modelo
{
	/*
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "par3.processo";

	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('proid');

	/**
	 * Atributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos     = array();
	
	public function retornarSQLItensIniciativa( $dotid, $iniid ){
		$sql = " SELECT procnpj, pronumeroprocesso, dotnumero, dotdatafimvigencia,
					inddsc, itcid, itcdsc, seq_sigarp, sum(ipiquantidadeaprovada) AS ipiquantidadeaprovada, ipivalorreferencia
                FROM(
                    select distinct p.procnpj, p.pronumeroprocesso, d.dotnumero, d.dotdatafimvigencia,
    					i.iniid, ind.inddsc, itc.itcid, itc.itcdsc, itc.itcidsigarp as seq_sigarp, it.ipiquantidadeaprovada, it.ipivalorreferencia, t.tecano, it.anaid
    				from par3.documentotermo d
    					inner join par3.processo p on p.proid = d.proid
    					INNER JOIN par3.processoparcomposicao pp ON pp.proid = p.proid AND pp.ppcstatus = 'A'
    					inner join par3.termocomposicao t on t.dotid = d.dotid and t.tecstatus = 'A'
    					inner join par3.v_analise_planejamento_item_composicao it on it.inpid = t.inpid and it.ipiano = t.tecano and it.ipistatus = 'A' AND it.anaid = pp.anaid
    					inner join par3.iniciativa_planejamento ip on ip.inpid = t.inpid and ip.inpstatus = 'A'
    					inner join par3.iniciativa i on i.iniid = ip.iniid
    					inner join par3.iniciativa_descricao ind on ind.indid = i.indid
    				    left join (
    				    	select distinct g.iniid, g.iigid, ic.itcid, ic.itcdsc, ic.itcidsigarp
    				        from par3.iniciativa_itenscomposicao_grupo g
    				    		inner join par3.itenscomposicao ic on ic.itcid = g.itcid
                                inner join par3.itenscomposicao_detalhamento id on id.itcid = ic.itcid and id.itdpregao = 'S'
    				        union all
    				        select distinct g.iniid, g.iigid, g.itcid, icg.igrnome as itcdsc, icg.igridsigarp as itcidsigarp
    				        from par3.iniciativa_itenscomposicao_grupo g
    				            inner join par3.itenscomposicao_grupos icg on icg.igrid = g.igrid
    				    ) itc on itc.iniid = i.iniid and it.iigid = itc.iigid
    				where t.dotid = $dotid
    					and i.iniid = $iniid
    					--and it.ipiaprovado in ('S', 'CS')
                        AND it.ipiquantidadeaprovada > 0
				) AS foo
				GROUP BY procnpj, pronumeroprocesso, dotnumero, dotdatafimvigencia, inddsc, itcid, itcdsc, seq_sigarp, ipivalorreferencia";
		return $sql;
	}
}