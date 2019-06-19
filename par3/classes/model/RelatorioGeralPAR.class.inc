<?php
/**
 * Classe de Model do  Relatório Geral do PAR
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   THIAGO TASCA BARBOSA <thiagotasca@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-10-2018
 * @link     no link
 */

 /**
 * Par3_Model_RelatorioGeralPAR
 *
 * @category Class
 * @package  A1
 * @author    THIAGO TASCA BARBOSA <thiagotasca@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-10-2018
 * @link     no link
 */
class Par3_Model_RelatorioGeralPAR extends Modelo
{
    function __construct()
    {
        parent::__construct();
    }
    
    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function CarregaDadosEntidade($colunas){
        
        $sql = 'SELECT iu.inuid, iu.itrid, '.$colunas.'  
                FROM par3.instrumentounidade iu';
                
        return $sql;
    }

    public function carregaDadosPlanejamentoIniciativa($colunas){
        
        $sql = "SELECT  p.inpid, p.dimid, p.inpcronogramamesinicial, 
                        p.inpcronogramamesfinal,  ini.iniid, obj.intoid, 
                        obj.intodsc AS tipoobjeto, 
                        idesc.indid, idesc.inddsc AS nomeiniciativa   
        FROM par3.iniciativa_planejamento p
        INNER JOIN par3.iniciativa 			ini     ON ini.iniid = p.iniid
        INNER JOIN par3.iniciativa_tipos_objeto 	obj 	ON obj.intoid = ini.intoid AND intostatus = 'A'
        INNER JOIN par3.iniciativa_descricao 		idesc 	ON idesc.indid = ini.indid AND intostatus = 'A'
        WHERE ini.inistatus = 'A' AND inpstatus = 'A'";
        return 'teste';
        $dados = $this->carregar($sql);
        return $dados;
    }

    public function sqlCarregaDadosPlanejamentoItens($colunas){

        $sql = "SELECT  ip.inuid, ip.inpid, ipi.ipiid, ip.iniid, ig.iigid, ig.igrid,
                        ipi.ipivalorreferencia AS valoritem, 
                        ipi.ipiquantidade AS qtditem,  
                        ip.inpcronogramaano AS ano
        FROM par3.iniciativa_planejamento ip
        INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.inpid = ip.inpid 
        INNER JOIN par3.iniciativa_itenscomposicao_grupo ig ON ig.iigid = ipi.iigid
        WHERE ip.inpstatus = 'A' AND ipi.ipistatus = 'A' --AND ig.iigsituacao = 'A'";

        $dados = $this->carregar($sql);
        return $dados;
    }

    public function sqlCarregaIniciativasPorObj($intoid){

        $sqlIniciativa = " SELECT ini.iniid AS codigo,  idesc.inddsc AS descricao 
                            FROM par3.iniciativa ini
                            INNER JOIN par3.iniciativa_descricao idesc ON idesc.indid = ini.indid  where intoid =  ".$intoid;
      
        $dados = $this->carregar($sqlIniciativa);
        return $dados;
    }

    

    public function recuperaInuid($filtros, $itrid = NULL, $tipo = NULL)
    {
        if($tipo == 'processo' ){
            $sql = "SELECT inuid FROM par3.processo WHERE pronumeroprocesso in ( '".$filtros."')";
            $dados = $this->carregar($sql);
            return $dados;
        }
        if($tipo == 'empenho' ){
            $sql = "select inuid from par3.empenho e 
                    inner join par3.processo p on p.pronumeroprocesso = e.empnumeroprocesso 
                    where empnumero = '".$filtros."' ";
            $dados = $this->carregar($sql);
            return $dados;
        }

        if($tipo == 'termo' ){
        $sql = "select p.inuid, * from par3.documentotermo dot 
        inner join par3.processo p on p.proid = dot.proid
            where dotstatus = 'A' and dotid in ( '".$filtros."')"; // -- and dotnumero in ( '".$filtros."')";
          
            $dados = $this->carregar($sql);
            return $dados;
        }
        
        if($itrid == 1 || $itrid == 2 ){
            $where = "iu.itrid = ".$itrid." ".$filtros; 
        }else if( $itrid == 3){
            $where = "iu.itrid in ( 1, 2 ) ".$filtros; 
        }
        else{
            $where = " ".$filtros;
        }
        
        
        $sql = "SELECT  iu.inuid
        FROM par3.instrumentounidade iu
        WHERE ".$where;
       
        $dados = $this->carregar($sql);
        return $dados;
    }
    
    
    public function sqlCarregaRelatorioGeralPar($arrayColunasSql, $arrayColunasGroupBySql, $filtros, $filtrosInterno, $tabelasprioritarias, $filtrosiniciativadados, $arrFiltros = NULL, $arrayColunasSqlWith = NULL )
    {
       // Todos as requisições solicitadas
        $colunas            = implode(',',$arrayColunasSql);
        $groupBy            = implode(',',$arrayColunasGroupBySql);
        $LigacaoTermo       = " LEFT ";
        $LigacaoTermoValidado = " ";
        $LigacaoProcesso    = " LEFT ";
        $LigacaoItem        = " LEFT ";
        $LigacaoEmpenho     = " LEFT ";
        $LigacaoAnalise     = "  ";
        $LigacaoPagamento   = " LEFT ";
        $LigacaoEmenda      = " LEFT ";
        $ligacaoObra        = " LEFT ";
        $LigacaoPlanejamento    = " LEFT ";
        $ligIniPlaItemCom       = " LEFT ";
        $WITHItens              = " ";
        $WITHIniciativasDados   = " "; 
        $tabelaIniciativaDados  = " ";  
        $tabelasProcesso        = " ";
        $tabelasEmedas          = " ";
        $tabelasAdesaoSIGARP    = " ";
        $filtroObra             = ' ';
        $filtroPlaEntidade      = ' ';
        $filtroPlanejamento     = ' ';
        $empenhoSelecionado     = false;
        $ligProceAnalise        = ' '; 
        $LigacaoplanjEmpenho    = ' ';
        $ligsaldoprocessoempenho = ' ';
        $faseAnaliseFiltro      = false;
        $filtroItensAprovados   = ' AND ai.aicqtdaprovado > 0 ';
        $filtroItensAprovados2  = ' AND aicqtdaprovado > 0 ';


        // Tratamento de FILTROS
        if($arrFiltros){
            foreach( $arrFiltros as $key => $value ){ 
                if($key == 'filtroobra'){
                    $filtroObra = $value;
                    $ligacaoObra = ' INNER ';
                }

                if($key == 'filtroEmenda'){
                    $filtros .= $value;
                    $LigacaoEmenda = " INNER ";
                }

                if($key == 'filtroAtendimento'){
                    $filtroPlanejamento .= $value;
                }
                if($key == 'filtroItemAprovado'){
                    $filtroPlanejamento .= $value;
                }
                if($key == 'filtroNE'){
                    $LigacaoEmpenho = " INNER ";
                    $filtros .= " AND empenho.empnumero ='".$value."' ";
                }
                if($key == 'codplafiltro'){
                    $filtros .= " AND p.inpid = ".$value." ";
                    $filtroPlaEntidade = " AND ip.inpid = ".$value;
                }
            }
        }

        // controle de colunas especificas
        $coltipoProcesso    = false;
        $colDadosObras      = false;
        $colVlrProcesso     = false;
        $termoRejeitado     = false;

        // controle de tipo de colunas selecionadas (Planejamento, Analise)
        $colDadosAnalise    = false; // Alguma Colunas de análise selecionadas?
        $colPlanejamento    = false; // Alguma Colunas de Planejamento selecionadas?
        $colProcessos       = false; // Alguma Colunas de Processos selecionadas?
        $colTermos          = false; // Alguma Colunas de Termo selecionadas?
        $colEmpenho         = false; // Alguma Colunas de Empenho selecionadas?
        $colPagamento       = false; // Alguma Colunas de Pagamento selecionadas?

        if( count($tabelasprioritarias) ){
            
            // controle de campos selecionados e cruzamentos com filtros
            foreach ($tabelasprioritarias as $key => $value) {
                // verifica se blocos de colunas selecionadas para montagem do relatório
                if($value == 'planejamento'){
                    $colPlanejamento = true;
                }
                if( $value == 'colanalise' ){
                    $colDadosAnalise = true;
                    $LigacaoAnalise  = " LEFT ";
                    $ligIniPlaItemCom = " LEFT ";
                }
                if( $value == 'colunasProcessoselecionadas' ){
                    $colProcessos = true;
                    $ligIniPlaItemCom = " LEFT ";
                }
                if( $value == 'colTermos' ){
                    $colTermos      = true;
                    $ligIniPlaItemCom = " LEFT ";
                }
                if( $value == 'colEmpenhos' ){
                    $colEmpenho = true;
                    $ligIniPlaItemCom = " INNER ";
                }
                if( $value == 'colPagamento' ){
                    $colPagamento = true;
                    $ligIniPlaItemCom = " INNER ";
                }

                if($value == 'tipoprocesso'){
                    $coltipoProcesso = true;
                }
                if($value == 'obras'){
                    $colDadosObras = true;
                }
                if($value == 'valorprocesso'){
                    $colVlrProcesso = true;
                    $ligsaldoprocessoempenho = " LEFT JOIN par3.v_saldo_empenho_do_processo vsaldo ON vsaldo.proid = pro.proid  ";
                   // $ligProceAnalise = "AND ppc.anaid = p.anaid"; 
                }
                // filtros
                if( $value == 'filtrofaseAguardAnalise' ){
                    $filtroItensAprovados = '  ';
                    $filtroItensAprovados2 = '  ';
                }
                if( $value == 'filtrofaseAnalise' ){
                    $faseAnaliseFiltro = true;
                }
                if( $value == 'filtroTermo' ){
                    $filtroTermo = true;
                }
                
            } 

        // TRATAMENTO DE COLUNAS PARA O SELECT DO with
        $selectwith         = " ";
        $selectwithInternoGrupo  = " ";
        $selectwithInternoSemGrupo = " ";
        if(count($arrayColunasSqlWith) > 0 ){
            foreach ($arrayColunasSqlWith as $key => $value) {
                if($value == 'valoritem'){
                    $selectwith .= " valoritem, ";
                    $selectwithInternoGrupo .= " 
                    CASE WHEN ipc.ipivalorreferencia is not null then ipc.ipivalorreferencia
                    ELSE sum(icgi.gitqtd * (  SELECT max(itdvalor)
                                FROM par3.itenscomposicao_detalhamento id
                                WHERE icgi.itcid = id.itcid AND itdstatus = 'A' AND itdsituacao = 'A' AND (now() between itdperiodoinicio AND itdperiodofim OR itdperiodofim < now()))) 
                    END AS valoritem, 
                    "; 

                    $selectwithInternoSemGrupo .= "  
                    CASE WHEN ipc.ipivalorreferencia is not null then ipc.ipivalorreferencia
                    ELSE (SELECT max(itdvalor)
                        FROM par3.itenscomposicao_detalhamento id
                        WHERE ic.itcid = id.itcid AND itdstatus = 'A' AND itdsituacao = 'A' AND (now() between itdperiodoinicio AND itdperiodofim OR itdperiodofim < now())
                    ) END AS valoritem, ";
                }
                if($value == 'inddsc'){
                    $selectwith .= " idesc.inddsc, ";
                }
                if($value == 'item'){
                    $selectwith .= " item, ";
                    $selectwithInternoGrupo .= " igrnome as item, ";
                    $selectwithInternoSemGrupo .= " itcdsc as item, "; 
                }
                if($value == 'qtditem'){
                    $selectwith .= " qtditem, ";
                    $selectwithInternoGrupo .= " ipc.ipiquantidade AS qtditem, ";
                    $selectwithInternoSemGrupo .= " ipc.ipiquantidade AS qtditem, "; 
                }
                

            }
        }
            
            
            
            //ver($tabelasprioritarias);die();
            // controle de tabelas e ligações necessarias de acordo com as colunas
            foreach ($tabelasprioritarias as $key => $value) {
                //ver($value);die();

                // Termos
                if($filtroTermo){
                   $LigacaoTermo = " INNER ";
                   $colTermos = true;
                }
                if($value == 'termoValidado'){
                    $LigacaoTermoValidado = " INNER JOIN par3.documentotermovalidacao dtv ON dtv.dotid = dot.dotid AND dotstatus = 'A'  ";
                    $LigacaoTermo = " INNER ";
                    $colTermos = true;
                }
                if($value == 'termoPendente'){
                    $filtros .= " AND dot.dotrejeitado = 'N' ";
                    $filtros .= " AND dot.dotid NOT IN ( SELECT dtv.dotid FROM par3.documentotermovalidacao dtv WHERE dtv.dotid = dot.dotid AND dotstatus = 'A' ) ";
                    $LigacaoTermo = " INNER ";
                    $colTermos = true;
                }
                if($value == 'termoRejeitado'){
                    $filtros .= "   AND dot.dotrejeitado = 'S' 
                                    AND dotstatus = 'I'
                                    AND pro.proid not in (
                                    select valido.proid
                                    from par3.documentotermo valido
                                    where valido.proid = proid
                                    and dotstatus = 'A'
                                    and dotrejeitado = 'N' 
                                    ) ";
                    $termoRejeitado = true;
                    $colTermos = true;
                }
                if($value == 'termoComContrato'){
                    $LigacaoTermoContrato = " INNER JOIN par3.execucao_contrato ec ON ec.proid = pro.proid ";
                    $LigacaoTermo = " INNER ";
                    $colTermos = true;
                }
                if($value == 'termoSemContrato'){
                    $filtros .= " AND dot.proid NOT IN ( SELECT ec.proid FROM par3.execucao_contrato ec WHERE ec.proid = pro.proid ) ";
                    $colTermos = true;
                }
                if($value == 'TermoComNota'){
                    $LigacaoTermoNota = " INNER JOIN par3.execucao_notafiscal ntf ON ntf.proid = pro.proid AND ntf.ntfstatus = 'A' ";
                    $LigacaoTermo = " INNER ";
                    $colTermos = true;
                }
                if($value == 'TermoSemNota'){
                    $filtros .= " AND dot.proid NOT IN ( SELECT ntf.proid FROM par3.execucao_notafiscal ntf WHERE ntf.proid = pro.proid AND ntf.ntfstatus = 'A' ) ";
                    $colTermos = true;
                }

                // Outros
                if($value == 'processo'){
                    $LigacaoProcesso = " INNER ";
                 }
                 if($value == 'item'){
                    $LigacaoItem = " INNER ";
                 }
                 if($value == 'empenho'){
                    $empenhoSelecionado = true;
                 }
                 if($value == 'planejamento'){
                    $LigacaoPlanejamento    = " INNER ";
                    $LigacaoplanjEmpenho    = " AND empenho.inpid = p.inpid ";
                    $selectEmpenho          = " inpid, ";
                    $SelempItens            = " , ppc.inpid ";
                    $Selempobras            = " , null::integer as inpid ";
                 }
                 if($value == 'filtroitemaprovado' && $colDadosAnalise){ 
                        $LigacaoAnalise = " INNER ";
                 }
                 if($value == 'filtroitemnaoaprovado' && $colDadosAnalise){ 
                    $LigacaoAnalise = " LEFT ";
                }

                if( $value == 'filtroitemaadesaoSim'){
                    $tabelasAdesaoSIGARP = " INNER JOIN par3.itempregaosigarp ips ON p.itcid = ips.itcid AND pro.proid = ips.proid
		                                     INNER JOIN par3.itemadesaosigarp ias ON ias.item = ips.nu_seq_item AND adestatus = 'A' AND ias.proid = pro.proid  ";

                }else if($value == 'filtroitemaadesaoNao'){
                    $filtros .= " and (itcid, pro.proid) not in (   SELECT itcid, ips.proid FROM  par3.itempregaosigarp ips
                                                                    INNER JOIN par3.itemadesaosigarp ias ON ias.item = ips.nu_seq_item AND adestatus = 'A'
                                                                    WHERE ips.proid = pro.proid AND ias.proid = pro.proid )  ";

                }

                
                 

                 
                if($value == 'tipoobjeto'){
                    /*
                    $colTipoobjetoProcesso = " array_to_string(array(SELECT DISTINCT t.intodsc FROM par3.iniciativa_tipos_objeto t
                    INNER JOIN(
                        SELECT intoid, proid FROM par3.processoparcomposicao WHERE ppcstatus = 'A'
                        UNION ALL
                        SELECT intoid, proid FROM par3.processoobracomposicao WHERE pocstatus = 'A'
                    ) pp ON pp.intoid = t.intoid
                    WHERE t.intostatus = 'A'
                    AND pp.proid = pro.proid), ', ') AS tipo_objeto, ";
                    */
                }
                if($value == 'colanalise' || $colVlrProcesso ){
                    $colunasplanejemantoAnalise = ", aicaprovado, coalesce(aicqtdaprovado,0) as aicqtdaprovado, coalesce(qtd_total_aprovada,0), total_aprovado,  anaid, edeid ";
                    $colunasAnaliseItens = " aic.aicaprovado, aic.aicqtdaprovado,  aic.qtd_total_aprovada, aic.total_aprovado, aic.docid, aic.anaid, aic.edeid, ";
                    $tabelaAnaliseItens = 
                        " JOIN (
                                    SELECT a.anaid, a.docid, a.anaano, a.inpid, ai.ipiid, ai.aicqtdaprovado, ai.aicvaloraprovado, ai.aicaprovado, toa.qtd_total_aprovada, toa.total_aprovado, a.edeid
                                    FROM par3.analise a
                                        INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.aicstatus = 'A'
                                        INNER JOIN(
                                            SELECT ipiid, aicqtdaprovado AS qtd_total_aprovada, (aicqtdaprovado * aicvaloraprovado) AS total_aprovado 
                                            FROM(
                                                SELECT it.ipiid, sum(it.aicqtdaprovado) AS aicqtdaprovado, it.aicvaloraprovado
                                                FROM par3.analise_itemcomposicao it
                                                INNER JOIN par3.analise a ON a.anaid = it.anaid AND a.anastatus = 'A'
                                                WHERE it.aicstatus = 'A' ".$filtroItensAprovados2."  -- AND a.anaano = 2017 
                                                GROUP BY it.aicvaloraprovado, it.ipiid
                                            ) AS foo
                                        ) toa ON toa.ipiid = ai.ipiid
                                                WHERE a.anastatus = 'A' ".$filtroItensAprovados."  -- AND a.inpid = 112499 
                                ) aic ON aic.ipiid = ipc.ipiid AND aic.inpid = ipc.inpid ";
                    $colunasAnaliseGrupoItem = " aic.aicaprovado, aic.aicqtdaprovado, aic.qtd_total_aprovada, aic.total_aprovado, aic.docid, aic.anaid, aic.edeid, ";
                    $tabelasAnaliseGrupoItem = " JOIN (
                        SELECT a.anaid, a.docid, a.anaano, a.inpid, ai.ipiid, ai.aicqtdaprovado, ai.aicvaloraprovado, ai.aicaprovado, toa.qtd_total_aprovada, toa.total_aprovado, a.edeid
                        FROM par3.analise a
                            INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.aicstatus = 'A'
                            INNER JOIN(
                                SELECT ipiid, aicqtdaprovado AS qtd_total_aprovada, (aicqtdaprovado * aicvaloraprovado) AS total_aprovado -- , a.anaano
                                FROM(
                                    SELECT it.ipiid, sum(it.aicqtdaprovado) AS aicqtdaprovado, it.aicvaloraprovado
                                    FROM par3.analise_itemcomposicao it
                                    INNER JOIN par3.analise a ON a.anaid = it.anaid AND a.anastatus = 'A'
                                    WHERE it.aicstatus = 'A' ".$filtroItensAprovados2."   -- AND a.anaano = ipc.ipiano 
                                    GROUP BY it.aicvaloraprovado, it.ipiid
                                ) AS foo
                            ) toa ON toa.ipiid = ai.ipiid
                            WHERE a.anastatus = 'A'  ".$filtroItensAprovados."  
                    ) aic ON aic.ipiid = ipc.ipiid AND aic.inpid = ipc.inpid";
                    $groupbyanalisegrupoitem = " aic.aicqtdaprovado, aic.qtd_total_aprovada, aic.total_aprovado, aic.aicaprovado, aic.edeid, aic.docid, aic.anaid, ";
                 }

                 if($value == 'colfasepar'){
                   $tabelasFasePar = "  INNER JOIN workflow.documento doc  ON doc.docid = iu.docid
                                        INNER JOIN workflow.estadodocumento ed ON doc.esdid = ed.esdid ";
                 } 
                 

                 if($value == 'colFaseAnalise' or  $faseAnaliseFiltro){
                    $colFaseAnalise = ", doca.docid, eda.esddsc as esddscanalise, eda.esdid ";
                    $tabelasFaseAnalise = " LEFT JOIN workflow.documento doca  
                    INNER JOIN workflow.estadodocumento eda ON doca.esdid = eda.esdid 
                ON doca.docid = dplaan.docid ";
                                        
                  }

                 if($value == 'pagamento'){
                    $LigacaoPagamento = " INNER ";
                 }

                 if($value == 'iniciativaDados'){ 
                    $WITHIniciativasDados = " ,
                    iniciativadados AS (
                        SELECT ini.iniid, obj.intoid, obj.intodsc AS tipoobjeto, idesc.indid, idesc.inddsc , ta.intadsc as cronograma, tipas.intadsc AS tipoassistencia  
                        FROM par3.iniciativa 			ini 
                        INNER JOIN par3.iniciativa_tipos_objeto 	obj 	ON obj.intoid = ini.intoid AND intostatus = 'A'
                        INNER JOIN par3.iniciativa_descricao 		idesc 	ON idesc.indid = ini.indid AND intostatus = 'A'
                        INNER JOIN par3.iniciativa_tipos_atendimento	ta	ON ta.intaid = ini.intaid
                        INNER JOIN par3.iniciativa_tipos_assistencia 	tipas	ON tipas.intaid = ini.intaid
                        WHERE 1 = 1 ".$filtrosiniciativadados."
                    ) ";
                    $tabelaIniciativaDados = " INNER JOIN iniciativadados 	    id ON id.iniid = p.iniid ";
                 }

                 if($value == 'colunasitensselecionadas'){
                     
                 }
                 
                 if($value == 'colunasProcessoselecionadas' &&  ($colDadosObras) ){
                   
                        $tabelasProcesso = $ligacaoObra."  JOIN par3.obra o ON o.inuid = iu.inuid AND obrstatus = 'A' ".$filtroObra." 
                        LEFT JOIN par3.processoobracomposicao poc ON o.obrid = poc.obrid
                        LEFT JOIN par3.processoparcomposicao ppc ON p.inpid = ppc.inpid -- AND ppc.anaid = p.anaid 
                        ".$LigacaoProcesso."  JOIN par3.processo pro ON pro.proid = poc.proid and pro.inuid = iu.inuid or pro.proid = ppc.proid AND pro.prostatus = 'A' ".$ligsaldoprocessoempenho." ";

                 }else if($value == 'colunasProcessoselecionadas' &&  $colDadosAnalise  ){ //&& $colPlanejamento
                    $tabelasProcesso = $LigacaoProcesso."   JOIN par3.processo pro 
                                                            INNER JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid   
                                                            ON pro.inuid = iu.inuid AND p.inpid = ppc.inpid AND pro.prostatus = 'A' AND ppc.anaid = p.anaid  
                                                            ".$ligsaldoprocessoempenho." "; 
                 }else if($value == 'colunasProcessoselecionadas' && ( !$colDadosAnalise && !$colPlanejamento ) ){
                    $tabelasProcesso = $LigacaoProcesso."   JOIN par3.processo pro 
                                                            LEFT JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid  
                                                            ON pro.inuid = iu.inuid AND pro.prostatus = 'A' 
                                                            ".$ligsaldoprocessoempenho." "; 
                    
                 }else if($value == 'colunasProcessoselecionadas' && ( !$colDadosAnalise && $colPlanejamento ) ){
                    $tabelasProcesso = $LigacaoProcesso."   JOIN par3.processo pro 
                                                            INNER JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid  
                                                            ON pro.inuid = iu.inuid AND pro.prostatus = 'A' AND ppc.inpid = p.inpid 
                                                            ".$ligsaldoprocessoempenho." "; 
                    
                 } else if( $value == 'obras' ){
                    $tabelasProcesso = $ligacaoObra."  JOIN par3.obra o ON o.inuid = iu.inuid AND obrstatus = 'A' ".$filtroObra." 
                    LEFT JOIN par3.processoobracomposicao poc ON o.obrid = poc.obrid
                    ".$LigacaoProcesso."  JOIN par3.processo pro ON pro.proid = poc.proid and pro.inuid = iu.inuid";
                 }

                 if( $value == 'emenda'){
                    $tabelasEmedas = $LigacaoEmenda."   JOIN par3.iniciativa_emenda ie ON ie.edeid = p.edeid AND ie.inestatus = 'A'
                                        ".$LigacaoEmenda." JOIN emenda.v_emendadetalheentidade ve 
                                            ".$LigacaoEmenda." JOIN emenda.autor au ON au.autid = ve.autid 
                                        ON ve.emeid = ie.emeid ";
                }

            }
        }

        
        $tabelasDependentesDoProcesso   = " ";
        $tabelasTermos                  = " ";
        if( $colProcessos ){

        }
        if( $colEmpenho ){

        }
        if( $colTermos ){
            if($termoRejeitado){
                $tabelasTermos =  $LigacaoTermo." JOIN par3.documentotermo dot 
                    INNER JOIN par3.iniciativa_tipos_objeto ito ON ito.intoid = dot.intoid
                    ON dot.proid = pro.proid AND dot.dotstatus='I'
                    ".$LigacaoTermoValidado." ".$LigacaoTermoPendente." ".$LigacaoTermoContrato." ".$LigacaoTermoNota."
                ";
            }else{
                $tabelasTermos =  $LigacaoTermo." JOIN par3.documentotermo dot 
                    INNER JOIN par3.iniciativa_tipos_objeto ito ON ito.intoid = dot.intoid
                    ON dot.proid = pro.proid AND (dot.dotstatus='A' or dot.dotstatus='B')  
                    ".$LigacaoTermoValidado." ".$LigacaoTermoPendente." ".$LigacaoTermoContrato." ".$LigacaoTermoNota."
               ";
            }
           
        }
        if( $colPagamento ){
            $tabelasPagamento = $LigacaoPagamento." JOIN par3.pagamento pag ON empenho.empid = pag.empid AND pagsituacaopagamento != '9 - CANCELADO' ";
        }

        
        if($tabelasProcesso == " " && $colTermos ){
            $tabelasProcesso = $LigacaoProcesso."   JOIN par3.processo pro ON pro.inuid = iu.inuid AND pro.prostatus = 'A' "; // .$ligsaldoprocessoempenho." "; 
        }

        if($tabelasProcesso != " " && $empenhoSelecionado || $colPagamento ){
            $tabelasDependentesDoProcesso = $LigacaoEmpenho." JOIN (
                SELECT proid, empid, ".$selectEmpenho." empnumero, empcodigoespecie, valorempenhado, tipo, empanooriginal, empcodigoptres, empfonterecurso, empsituacao
                FROM (
                    SELECT distinct ppc.proid, e.empid,  e.empnumero, e.empcodigoespecie, (coalesce(e.empvalorempenho,0) - coalesce(ec.empvalorempenho,0)) as valorempenhado, 'par' as tipo, e.empanooriginal, e.empcodigoptres, e.empfonterecurso, e.empsituacao
                        ,null::integer as obrid ".$SelempItens."
                    FROM par3.processoparcomposicao ppc 
                    INNER JOIN par3.empenhoparcomposicao epc ON ppc.ppcid = epc.ppcid AND epc.epcstatus = 'A' 
                    INNER join par3.empenho e on e.empid = epc.empid AND e.empcodigoespecie = '01'
                    LEFT join par3.empenho ec on ec.empidpai = e.empid 
                     WHERE ppc.ppcstatus = 'A' -- ppc.proid = 89
                    UNION ALL
                    SELECT distinct poc.proid, e.empid, e.empnumero, e.empcodigoespecie, (coalesce(e.empvalorempenho,0) - coalesce(ec.empvalorempenho,0)) as valorempenhado, 'obras' as tipo, e.empanooriginal, e.empcodigoptres, e.empfonterecurso, e.empsituacao
                            ,poc.obrid AS obrid ".$Selempobras."
                    FROM par3.processoobracomposicao poc
                    INNER JOIN par3.empenhoobracomposicao epc ON poc.pocid = epc.pocid AND epc.eocstatus = 'A' ".$filtroObra."
                    INNER join par3.empenho e on e.empid = epc.empid AND e.empcodigoespecie = '01'
                    LEFT join par3.empenho ec on ec.empidpai = e.empid 
                     WHERE pocstatus = 'A' -- ppc.proid = 1530
                ) AS empenho WHERE valorempenhado > 0
            ) AS empenho ON empenho.proid = pro.proid ".$LigacaoplanjEmpenho." ".$tabelasPagamento;

        }

        $WITHPlaEntidade = "  SELECT 	dplaan.intoid, iniid, inuid, inpid, ano, m.moddsc,
                                        ipiid, iigid, itcid, iigid, igrid, anoiniciativa, unidsc,
                                        
                                        ".$selectwith."
                                        anoitem,
                                        obj.intodsc AS tipoobjeto,  ta.intadsc as cronograma, tipas.intadsc AS tipoassistencia  
                                        ".$colunasplanejemantoAnalise." ".$colFaseAnalise."
                                FROM (
                                -- DADOS DE PLANEJAMENTO + ANALISE QUANDO ITENS POR GRUPO
                                SELECT DISTINCT 
                                    ip.iniid, ip.inuid, ip.inpid, ip.modid, ip.inpcronogramaano AS ano,
                                    i.intoid, i.indid, i.intaid, 
                                    ipc.ipiid, iicg.itcid,iicg.iigid, iicg.igrid, iia.iniano AS anoiniciativa,  '-' as unidsc,
                                    ".$colunasAnaliseGrupoItem." 
                                    ".$selectwithInternoGrupo." 
                                    ipc.ipiano AS anoitem
                                     -- , ipc.ipiano AS anoiniciativa,
                                FROM par3.iniciativa_planejamento 	ip
                                JOIN par3.iniciativa 			i 	USING (iniid)
                                JOIN par3.iniciativa_iniciativas_anos 	iia 	USING (iniid)
                                JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                                JOIN par3.itenscomposicao_grupos 	icg 	USING (igrid)
                                JOIN par3.itenscomposicao_grupos_itens 	icgi 	USING (igrid)  
                                ".$ligIniPlaItemCom." JOIN par3.iniciativa_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = iia.iniano
                                -- DADOS ANALISE
                                ".$LigacaoAnalise." ".$tabelasAnaliseGrupoItem." 
                                WHERE iia.iianostatus = 'A' AND inpstatus = 'A' AND coalesce(ipc.ipiquantidade,0) > 0 ".$filtrosInterno." ".$filtroPlaEntidade." ".$filtrosiniciativadados."  -- AND ip.inuid in (' 378') -- AND ip.inpid = 143896 
                                GROUP BY ip.iniid, ip.inuid, ip.inpid, ano, ipc.ipiid, iicg.iigid, iia.iniano, igrnome, ipc.ipivalorreferencia, ".$groupbyanalisegrupoitem."
                                    i.intoid, i.indid, i.intaid
                                UNION ALL 
                                -- DADOS DE PLANEJAMENTO + ANALISE QUANDO ITENS SEM GRUPO
                                SELECT DISTINCT 
                                    ip.iniid, ip.inuid, ip.inpid, ip.modid, ip.inpcronogramaano AS ano,  i.intoid,i.indid, i.intaid,
                                    ipc.ipiid, iicg.itcid, iicg.iigid, iicg.igrid, iia.iniano AS anoiniciativa,  unidsc, 
                                    ".$colunasAnaliseItens."
                                    ".$selectwithInternoSemGrupo."
                                    ipc.ipiano AS anoitem
                                FROM par3.iniciativa_planejamento ip
                                JOIN par3.iniciativa i USING (iniid)
                                JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                                JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                                ".$ligIniPlaItemCom." JOIN par3.iniciativa_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = iia.iniano

                                -- DADOS ANALISE
                                ".$LigacaoAnalise." ".$tabelaAnaliseItens."

                                JOIN par3.itenscomposicao ic USING (itcid)
                                JOIN par3.unidade_medida um USING (uniid)
                                WHERE iia.iianostatus = 'A' AND inpstatus = 'A' AND coalesce(ipc.ipiquantidade,0) > 0  ".$filtrosInterno." ".$filtroPlaEntidade." ".$filtrosiniciativadados." -- ip.inpid = 112499 and iia.iniano = 2017 AND  ip.inuid = 1472 
                                ) AS dplaan
                                INNER JOIN par3.modalidade m ON m.modid = dplaan.modid
                                INNER JOIN par3.iniciativa_tipos_objeto obj ON obj.intoid = dplaan.intoid AND intostatus = 'A'
                                INNER JOIN par3.iniciativa_descricao 		idesc 	ON idesc.indid = dplaan.indid AND intostatus = 'A'
                                INNER JOIN par3.iniciativa_tipos_atendimento	ta	ON ta.intaid = dplaan.intaid
                                INNER JOIN par3.iniciativa_tipos_assistencia 	tipas	ON tipas.intaid = dplaan.intaid 
                                ".$tabelasFaseAnalise."
                                WHERE 1 = 1  ".$filtroPlanejamento."
                                
                            ";

        if($colPlanejamento || $colDadosAnalise ){
            $with = " WITH
            planejamento AS (
            -- PLANEJAMENTO ENTIDADE  
            ".$WITHPlaEntidade."
            ) "; 
            $tabelaPlanejamento = $LigacaoPlanejamento." JOIN planejamento 	    p  ON p.inuid = iu.inuid ";
        }else if($colProcessos && $colTermos && !$colPlanejamento && !$colDadosAnalise ){
            $with = " ";
            $tabelaPlanejamento = " ";
        }

        $sql = $with."
                SELECT DISTINCT
                        ".$colunas."
                FROM par3.instrumentounidade    iu
                ".$tabelaPlanejamento."
                ".$tabelasFasePar."                                         
                ".$tabelasProcesso." 
                ".$tabelasAdesaoSIGARP."
                ".$tabelasDependentesDoProcesso."
                ".$tabelasTermos."
                ".$tabelasEmedas."
                WHERE iu.inustatus = 'A'  ".$filtros."
                GROUP BY  ".$groupBy."
                ORDER BY 1
            ";

         // ver($sql); die();
        return $sql;
    }
}
