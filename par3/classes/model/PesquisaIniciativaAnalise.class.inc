<?php
class Par3_Model_PesquisaIniciativaAnalise extends Modelo
{
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('indid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        ''            => null,
        ''           => null,
        ''        => null,
        ''      => null,
        ''    => null,
        ''   => null,
        ''  => null,
        '' => null
    );

    public function montarSQLSimples($arrPost)
    {
    	
        $inddsc  = sanitizar_string_pesquisa($arrPost['inddsc']);
        $inpid   = $arrPost['codigo'];

        $join = '';
        $where = '';
        $where  .= $arrPost['inpid_filtro'] ? "  AND inp.inpid = {$arrPost['inpid_filtro']} ":'';
        $where  .= $arrPost['iarid'] ? "  AND ({$arrPost['iarid']}) in (SELECT DISTINCT
										                        iar.iarid
										                        FROM par3.iniciativa_iniciativas_areas iiar
										                        INNER JOIN par3.iniciativa_areas iar ON iar.iarid = iiar.iarid
										                        WHERE iniid = ini.iniid) ":'';
        
        $where  .= $arrPost['desid'] ? "  AND ({$arrPost['desid']}) in (SELECT DISTINCT
										        des.desid
										        FROM par3.iniciativa_planejamento_desdobramentos ipd
										        INNER JOIN par3.iniciativa_desdobramento des ON des.desid = ipd.desid
										        WHERE ipd.inpid = inp.inpid) ":'';
        
        
        $where  .= $arrPost['inddsc']?" AND upper(removeacento(ind.inddsc)) LIKE '%{$inddsc}%' ":'';
        $where  .= $arrPost['cicid'] && validarInteiro($arrPost['cicid'])?"AND ini.cicid = {$arrPost['cicid']}":"";
        $where  .= $arrPost['dimid'] && validarInteiro($arrPost['dimid'])?"AND inp.dimid = {$arrPost['dimid']}":"";
        $where  .= $arrPost['esdid'] ? " AND esd.esdid = {$arrPost['esdid']}" : ' and doc.esdid in ('.PAR3_ESDID_EM_ANALISE.', '.PAR3_ESDID_AGUARDANDO_ANALISE.', '.PAR3_ESDID_EM_DILIGENCIA.') ';
        
//        $where  .= $arrPost['muncod'] ? " AND iu.muncod = '{$arrPost['muncod']}'":'';

        if($arrPost['muncod'] && count($arrPost['muncod']) != 0 ){
            $arrMuncod = tratarArrayParaMultiSelect($arrPost['muncod']);
            if(count($arrMuncod) != 0){
                $where  .= " AND iu.muncod::integer in (".implode(',',$arrMuncod).")";
            }
        }

        if($arrPost['estuf'] && count($arrPost['estuf']) != 0 ) {
            $arrEstuf = tratarArrayParaMultiSelect($arrPost['estuf']);
            if(count($arrEstuf) != 0) {
                $arrEstuf = implode(',',array_map(function($estuf){return (string)"'".$estuf."'";},$arrEstuf));
                $where  .= $arrEstuf ? " AND iu.estuf in ({$arrEstuf})":'';
            }
        }

        $where  .= $arrPost['etaid'] ? " AND eta.etaid = {$arrPost['etaid']}":'';
        $where  .= $arrPost['intaid'] ? " AND inta.intaid = {$arrPost['intaid']}":'';
        $where  .= $arrPost['intoid'] ? " AND obj.intoid = {$arrPost['intoid']}":'';
        $where  .= $arrPost['indid'] ? " AND ini.iniid = {$arrPost['indid']}":'';
        $where  .= $arrPost['itrid'] && $arrPost['itrid'] != 0 ?($arrPost['itrid'] == 1 ? "AND iu.itrid in(1,3)" :"AND iu.itrid = {$arrPost['itrid']}"): '';

        if( $arrPost['tipoassistencia'] ){
            $where  .= " AND tpas.intaid = {$arrPost['tipoassistencia']} ";
        	$join .= " inner join par3.analise ana on ana.inpid = inp.inpid and ana.anastatus = 'A'
                       inner join par3.iniciativa_tipos_assistencia tpas on tpas.intaid = ana.intaid ";
        }
        
        if( $arrPost['iniano'] ){
        	$where  .= " AND ina.iniano = {$arrPost['iniano']}";
        	$join .= ' inner join par3.iniciativa_iniciativas_anos ina on ina.iniid = ini.iniid ';
        }
        if( $arrPost['prgid'] ){
        	$where  .= " AND inip.prgid = {$arrPost['prgid']}";
        	$join .= ' inner join par3.iniciativa_iniciativas_programas inip on inip.iniid = ini.iniid ';
        }
        if( $arrPost['usucpf_analise'] ){
        	$where .= " AND ana.cpftecnico = '{$arrPost['usucpf_analise']}'";
        	$join .= " inner join par3.analise ana on ana.inpid = inp.inpid and ana.anastatus = 'A' ";
        }
        if( $arrPost['esdid_analise']){

            //verificar se o array tem 1 posição e tratar se os valores estiverem separados por vírgula
            $arrCampo = $arrPost['esdid_analise'];
            $arrImplode = array();
            if(is_array($arrCampo)){
                $arr = array_filter($arrCampo,function($val){return !empty($val);});
                if(count($arr) == 1){
                    $arrImplode = explode(',',$arr[0]);
                }
                if(count($arr) > 1){
                    $arrImplode = $arr;
                }
                $_REQUEST['esdid_analise'] = $arrImplode;
            }
                $where  .= " AND inp.inpid in (SELECT
                                p.inpid
                            FROM
                                par3.iniciativa_planejamento p
                                inner join par3.analise a on a.inpid = p.inpid and a.anastatus = 'A'
                                inner join workflow.documento d on d.docid = a.docid
                            WHERE
                                d.esdid in (".implode(',',$arrImplode).") )";
        }
        if( $arrPost['escid'] ){
        	$where .= " AND inp.inpid in (SELECT ip.inpid
										FROM
											par3.iniciativa_planejamento ip
											inner join par3.iniciativa_iniciativas_anos iia on iia.iniid = ip.iniid
											inner join par3.iniciativa_itenscomposicao_grupo iicg on iicg.iniid = iia.iniid
											inner join par3.iniciativa_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipiano = iia.iniano
										    inner join par3.iniciativa_planejamento_item_composicao_escola ipe on ipe.ipiid = ipc.ipiid and ipe.ipestatus = 'A'
										WHERE ipc.ipistatus = 'A'
											and ipe.escid = {$arrPost['escid']}) ";
        }

        $orderBy  = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":" ";
        $contador = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls'? "ROW_NUMBER() OVER ({$orderBy}) AS qtd," : '';
        $inicio   = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls'? "SELECT ROW_NUMBER() OVER ({$orderBy}) AS qtd,*FROM (":'';
        $fim      = $arrPost['requisicao'] == 'imprimir' || $arrPost['requisicao'] == 'xls'? ") as foo":'';

        $orderBy  = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":" ORDER BY inp.inpid DESC";


        $sql = "{$inicio}
                SELECT DISTINCT
                    inp.inpid,
                    inp.inpid as codigo,
                    inp.iniid,
                    iu.inuid,
                    (dim.dimcod||' - '||dim.dimdsc) as dimensao,
                    case when iu.muncod is null then est.estuf else mun.estuf end as uf,
                    case when iu.muncod is null then est.estdescricao else mun.mundescricao end as entidade,
                    (inp.iniid||' - '||ind.inddsc) as ds_iniciativa,

                    (SELECT array_to_string(array(
                        SELECT DISTINCT
                        prg.prgdsc
                        FROM par3.iniciativa_iniciativas_programas iiprg
                        INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
                        WHERE iniid = ini.iniid
                    ),',')
                    ) as programas,
                    obj.intodsc as tipoobjeto,

                    --inp.iniid    as iniciativa,
                    eta.etadsc   as etapa,
                    mod.moddsc   as modalidade,

                    (SELECT array_to_string(array(
                        SELECT DISTINCT
                        des.desdsc
                        FROM par3.iniciativa_planejamento_desdobramentos ipd
                        INNER JOIN par3.iniciativa_desdobramento des ON des.desid = ipd.desid
                        WHERE ipd.inpid = inp.inpid
                    ),',')
                    ) as desdobramentos,

                    COALESCE(inpvalor_planejado_total,0) as valor

                    ,esd.esddsc as situacao
                    
                    -- Total aprovado
                    ,( WITH foo AS
                        ( SELECT
                             ipc.ipiaprovado
                            ,ipc.ipiquantidadeaprovada
                           
                           -- Se houver valores de ipi cadastrados desconsidera-se o valor de referência do itenscomposicao_detalhamento
                           ,COALESCE( (SELECT MAX(ipivalorreferencia) as ipivalorreferencia
                                        FROM par3.iniciativa_planejamento_item_composicao t1
                                        WHERE iigid = ipc.iigid AND inpid = ip.inpid AND ipistatus = 'A'
                                    ),( SELECT max(itdvalor)
                                        FROM par3.itenscomposicao_detalhamento id
                                        WHERE ic.itcid = id.itcid
                                        AND itdstatus = 'A'
                                        AND itdsituacao = 'A'
                                        AND (now() between itdperiodoinicio AND itdperiodofim)
                                    ) ) as valor_referencia
                            ,iia.iniano
                        FROM
                            par3.iniciativa_planejamento ip
                            JOIN par3.iniciativa i USING (iniid)
                            JOIN workflow.documento d ON d.docid = ip.docid
                            JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                            JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                            JOIN par3.v_analise_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = iia.iniano
                            JOIN par3.itenscomposicao ic USING (itcid)
                            JOIN par3.unidade_medida um USING (uniid)
                        WHERE
                            ip.inpid = inp.inpid
                            AND iia.iianostatus = 'A'
                            
                            -- IPI aprovado
                            AND ipc.ipiaprovado IS NOT NULL
                            
                            -- Estados bloqueados
                            AND d.esdid NOT IN( ".PAR3_ESDID_AGUARDANDO_VALIDACAO_COORDENADOR."
                                                ,".PAR3_ESDID_EM_VALIDACAO_COORDENADOR."
                                                ,".PAR3_ESDID_ANALISE_PLANEJAMENTO_APROVADO." )
                    ) SELECT SUM(foo.ipiquantidadeaprovada * valor_referencia) as total_aprov FROM foo ) as total_aprovado
                    
                FROM par3.iniciativa_planejamento as inp
	                inner join par3.instrumentounidade iu on iu.inuid = inp.inuid and iu.inustatus = 'A'
	                INNER JOIN par3.iniciativa as ini ON ini.iniid = inp.iniid
	                INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
	                --inner join par3.v_analise_planejamento_item_composicao ipc on ipc.inpid = inp.inpid and ipc.ipistatus = 'A'
	                INNER JOIN workflow.documento doc ON doc.docid = inp.docid
	                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
	                INNER JOIN par3.dimensao dim ON inp.dimid     = dim.dimid
	                INNER JOIN par3.ensino_nivel niv ON niv.nivid = inp.nivid
	                INNER JOIN par3.ensino_etapa eta ON eta.etaid = inp.etaid
	                INNER JOIN par3.modalidade   mod ON mod.modid = inp.modid
					$join
	                INNER JOIN par3.ciclo_par                    cic  ON cic.cicid   = ini.cicid
	                INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
	                INNER JOIN par3.iniciativa_tipos_atendimento inta ON inta.intaid = ini.intaid
	                LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
	                LEFT JOIN territorios.estado est on est.estuf = iu.estuf
                WHERE inpstatus = 'A'
                    {$where}
                    {$orderBy}
                    {$fim}";
        //ver($sql, d);
        return $sql;
    }

   

}