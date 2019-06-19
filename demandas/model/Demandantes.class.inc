<?php

class Demandantes_Model extends Modelo
{
    /*
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.entidades_funcao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('enfid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributosTabela = array('enfid', 'enfdsc', 'enfstatus');

    protected $arAtributos = array(
        'enfdsc' => null,
        'enfstatus' => null
    );
    
    public function getVariaveisPessoaJuridicaPai( $ddtid )
    {
        $sql = "SELECT d.ddtid, jf.ejfid, jf.enjid, jf.enfid, ej.enjcnpj, ej.enjdsc 
                FROM demandas.demandantes d
                	INNER JOIN corporativo.entidades_juridicaxfuncao jf ON jf.ejfid = d.ejfid AND jf.ejfstatus = 1
                	INNER JOIN corporativo.entidades_juridica ej ON ej.enjid = jf.enjid AND ej.enjstatus = 1
                WHERE d.ddtid = {$ddtid}";
        
        $arDados = $this->pegaLinha($sql);
        $arDados = $arDados ? $arDados : array();
        
        return $arDados;
    }

    public function getFuncaoEntidade( $enfid = '', $ddtid = '' ){
        
        if( $ddtid ) $filtro = " and ddtid = {$ddtid}";
        if( $enfid ) $filtro .= " and enfid = {$enfid}";
        
        $sql = "select enfid, enfdsc from corporativo.entidades_funcao where enfstatus = 'A' $filtro";
        $arDados = $this->pegaLinha($sql);
        $arDados = $arDados ? $arDados : array();
        
        return $arDados;
    }
    
    function __construct()
    {
        parent::__construct();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listaEntidade( $arrPost )
    {
        $filtro = '';
        if( $arrPost['enjdsc'] ){
            $filtro .= " and enjdsc ilike removeAcento('%".$arrPost['enjdsc']."%')";
        }
        if( $arrPost['enfid'] ){
            $filtro .= " and ef.enfid = {$arrPost['enfid']} ";
        }
        
        if( $arrPost['ddtid'] ) $filtro .= " and d.ddtid = {$arrPost['ddtid']}";
        
        $sql = "select distinct d.ddtid, ej.enjdsc, d.ejfid, ejf.enfid, ejf.enjid
                from demandas.demandantes d
                	INNER JOIN corporativo.entidades_juridicaxfuncao ejf ON ejf.ejfid = d.ejfid
                	inner join corporativo.entidades_juridica ej on ej.enjid = ejf.enjid
                	inner join corporativo.entidades_funcao ef on ef.enfid = ejf.enfid and ef.enfstatus = 'A'
                where ejf.ejfstatus = 1
                	$filtro";

        return $sql;
    }
    
    public function getIdVinculoDemandante($ddtid){
        $sql = "SELECT ejfid FROM demandas.demandantes WHERE ddtid = $ddtid";
        return $this->pegaUm($sql);
    }
    
    public function verificaExisteGestor($enfidpai){
        $sql = "SELECT enfidfilho FROM corporativo.entidades_representantes WHERE enfidpai='".$enfidpai."' AND enrgestor=TRUE";
        return $this->pegaUm($sql);
    }
    
    public function getDadosRepresentante($enfidfilho, $enfidpai = 1){
        $sql = "SELECT enrid, enfidpai, enfidfilho, enrqtdcadastrados, enrtipo, enrgestor, enrstatus FROM corporativo.entidades_representantes 
                    WHERE enfidfilho = $enfidfilho AND enfidpai = {$enfidpai} AND enrstatus = 1";
        return $this->pegaLinha($sql);
    }
        
    public function getDadosPessoaJuridica( $arrParam = array() )
    {        
        $filtro = '';
        if( $arrParam['enjid'] ) $filtro = " and ej.enjid = {$arrParam['enjid']} ";
        if( $arrParam['enfid'] ) $filtro .= " and ejf.enfid = {$arrParam['enfid']} ";
        
        $sql = "SELECT ej.enjid, enjcnpj, enjdsc, d.ejfid, enfid, enjstatus, enjrazaosocial, enjinscricaoestadual, enjemail, enjtelefone, enjendcep, enjendlogradouro, 
                	enjendcomplemento, enjendnumero, enjendbairro, muncod
                FROM corporativo.entidades_juridica ej 
                	INNER JOIN corporativo.entidades_juridicaxfuncao ejf ON ejf.enjid = ej.enjid
                	INNER JOIN demandas.demandantes d ON d.ejfid = ejf.ejfid
                WHERE ejf.ejfstatus = 1 and d.ddtid = {$arrParam['ddtid']} $filtro";
        
        if( $arrParam['retornoSQL'] == true ){
            return $sql;
        } elseif( $arrParam['retornoCount'] == true ) {
            
            return $this->pegaUm("SELECT count(*) FROM ({$sql}) as foo");
            
        } else {
            $arrDados = $this->pegaLinha($sql);
            $arrDados = $arrDados ? $arrDados : array();
        }
        
        return $arrDados; 
    }
    
    public function getDadosRepresentanteJuridica( $arrParam = array() )
    {
        $filtro = '';        
        if( $arrParam['enjid'] ) $filtro = " and ej.enjid = {$arrParam['enjid']} ";
        if( $arrParam['enfid'] ) $filtro .= " and ejf.enfid = {$arrParam['enfid']} ";
        
        $sql = "SELECT ejf.ejfid, ej.enjid, enjcnpj, enjdsc, enfid, enjstatus, enjrazaosocial, enjinscricaoestadual, enjemail, enjtelefone, enjendcep, enjendlogradouro, 
                	enjendcomplemento, enjendnumero, enjendbairro, muncod
                FROM corporativo.entidades_juridica ej
                	INNER JOIN corporativo.entidades_juridicaxfuncao ejf ON ejf.enjid = ej.enjid
                	INNER JOIN corporativo.entidades_associativo ea ON ea.ejfidfilho = ejf.ejfid 
                	INNER JOIN demandas.demandantes d ON d.ejfid = ea.ejfid 
                WHERE ejf.ejfstatus = 1 and d.ddtid = $ddtid $filtro";
        
        if( $arrParam['retornoSQL'] == true ){
            return $sql;
        } elseif( $arrParam['retornoCount'] == true ) {
            return $this->pegaUm("SELECT count(*) FROM ({$sql}) as foo");
        } else {
            $arrDados = $this->pegaLinha($sql);
            $arrDados = $arrDados ? $arrDados : array();
        }
        
        return $arrDados;
    }
    
    public function getDadosPessoaFisica( $arrParam = array() )
    {
        $filtro = '';
        if( $arrParam['ensid'] ) $filtro = " and ef.ensid = {$arrParam['ensid']} ";
        if( $arrParam['enfid'] ) $filtro .= " and eff.enfid = {$arrParam['enfid']} ";
        if( $arrParam['enjid'] ) $filtro .= " and ejf.enjid = {$arrParam['enjid']} ";
        
        if( $arrParam['tipo_representante'] == 'G' ){
            $sql = "SELECT ef.ensid, ef.enscpf, er.enrtipo, 'N' as gestor, ef.ensdsc, eff.enfid, ef.ensstatus, ef.ensemail, ef.enstelefone, ef.ensendcep as enjendcep, ef.ensendlogradouro as enjendlogradouro,
                    	ef.ensendcomplemento as enjendcomplemento, ef.ensendnumero as enjendnumero, ef.ensendbairro as enjendbairro,
                        ef.ensdatanascimento, ef.ensrg, ef.ensorgexpedidor, ef.enssexo
                    FROM corporativo.entidades_fisica ef
                    	INNER JOIN corporativo.entidades_fisicaxfuncao eff ON eff.ensid = ef.ensid AND eff.effstatus = 1
                    	INNER JOIN corporativo.entidades_representantes er ON er.enfidfilho = eff.enfid AND er.enrstatus = 1
                    WHERE ef.ensstatus = 1 AND er.enrtipo = 'G' $filtro";
        } elseif( $arrParam['tipoGestor'] ){
            $sql = "SELECT ef.ensid, ej.enjid, ejf.ejfid, ef.enscpf, 'E' as enrtipo, 'S' as gestor, ef.ensdsc, eff.enfid, ef.ensstatus, ef.ensemail, ef.enstelefone, ef.ensendcep as enjendcep, ef.ensendlogradouro as enjendlogradouro,
                    	ef.ensendcomplemento as enjendcomplemento, ef.ensendnumero as enjendnumero, ef.ensendbairro as enjendbairro,
                        ef.ensdatanascimento, ef.ensrg, ef.ensorgexpedidor, ef.enssexo
                    FROM corporativo.entidades_fisica ef
                    	INNER JOIN corporativo.entidades_fisicaxfuncao eff ON eff.ensid = ef.ensid AND eff.effstatus = 1
                    	INNER JOIN corporativo.entidades_associativo ea ON ea.effid = eff.effid
                    	INNER JOIN corporativo.entidades_juridicaxfuncao ejf ON ejf.ejfid = ea.ejfid AND ejf.ejfstatus = 1
                    	INNER JOIN corporativo.entidades_juridica ej ON ej.enjid = ejf.enjid
                    	LEFT JOIN demandas.demandantes d ON d.ejfid = ejf.ejfid and d.ddtid = {$arrParam['ddtid']}
                    WHERE ef.ensstatus = 1  $filtro";
        } else {        
            $sql = "SELECT ef.ensid, ej.enjid, ejf.ejfid, ef.enscpf, 'E' as enrtipo, 'N' as gestor, ef.ensdsc, eff.enfid, ef.ensstatus, ef.ensemail, ef.enstelefone, ef.ensendcep as enjendcep, ef.ensendlogradouro as enjendlogradouro,
                    	ef.ensendcomplemento as enjendcomplemento, ef.ensendnumero as enjendnumero, ef.ensendbairro as enjendbairro,
                        ef.ensdatanascimento, ef.ensrg, ef.ensorgexpedidor, ef.enssexo
                    FROM corporativo.entidades_fisica ef
                    	INNER JOIN corporativo.entidades_fisicaxfuncao eff ON eff.ensid = ef.ensid AND eff.effstatus = 1
                    	INNER JOIN corporativo.entidades_associativo ea ON ea.effid = eff.effid 
                    	INNER JOIN corporativo.entidades_juridicaxfuncao ejf ON ejf.ejfid = ea.ejfid AND ejf.ejfstatus = 1
                    	INNER JOIN corporativo.entidades_juridica ej ON ej.enjid = ejf.enjid
                    	INNER JOIN demandas.demandantes d ON d.ejfid = ejf.ejfid
                    WHERE ef.ensstatus = 1 and d.ddtid =  {$arrParam['ddtid']} $filtro";
        }
        
        if( $arrParam['retornoSQL'] == true ){
            return $sql;
        } elseif( $arrParam['retornoCount'] == true ) {            
            return $this->pegaUm("SELECT count(*) FROM ({$sql}) as foo");            
        } else {
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
        }
        
        return $arrDados;
    }
    
    public function getDadosFuncaoEntidade( $ddtid )
    {        
        $sql = "SELECT DISTINCT er.*, efp.enfdsc AS entidade_pai, eff.enfdsc AS entidade_filho, eff.enftipo
                FROM corporativo.entidades_representantes er
                	INNER JOIN corporativo.entidades_funcao efp ON  efp.enfid = er.enfidpai
                	INNER JOIN corporativo.entidades_funcao eff ON  eff.enfid = er.enfidfilho
                	INNER JOIN corporativo.entidades_juridicaxfuncao ej ON ej.enfid = er.enfidpai
                	INNER JOIN demandas.demandantes d ON d.ejfid = ej.ejfid
                WHERE ej.ejfstatus = 1 and d.ddtid = $ddtid";
        $arrDados = $this->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
    public function sqlListaPessoaFisica($arrPost){
        $filtro = '';
        
        if( $arrPost['ddtid'] ) $filtro = " and d.demid = {$arrPost['ddtid']}";
        if( $arrPost['dctid'] ) $filtro .= " and dc.dctid = {$arrPost['dctid']}";
        if( $arrPost['dcoid'] ) $filtro .= " and d.dcoid = {$arrPost['dcoid']}";
        
        $sql = "SELECT d.demid, d.docid, d.demid as id, ct.dctdsc, cd.dcddsc, es.esddsc, us.usunome, to_char(doc.docdatainclusao, 'DD/MM/YYYY') as datainclusao
                FROM demandas.demandas d
                	INNER JOIN demandas.demandas_portfolio dc ON dc.dcoid = d.dcoid AND dc.dcostatus = 1
                	INNER JOIN demandas.demandas_portfolio_descricao cd ON cd.dcdid= dc.dcdid
                	INNER JOIN demandas.demandas_portfolio_tipo ct ON ct.dctid = dc.dctid
                	INNER JOIN workflow.documento doc ON doc.docid = d.docid
                	INNER JOIN workflow.estadodocumento es ON es.esdid = doc.esdid
                	INNER JOIN seguranca.usuario us ON us.usucpf = d.demcpfresponsavel
                WHERE d.demstatus = 1 $filtro";        
        
        return $sql;
    }
    
    public function getMunicipio( $muncod )
    {
        $sql = "SELECT muncod, mundescricao FROM territorios.municipio WHERE muncod = '{$muncod}'";
        $arrDados = $this->pegaLinha($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
    public function carregarCardapio($arrPost ) {
        
        $filtro = '';
        if( $arrPost['dctid'] ) $filtro = " and dc.dctid = {$arrPost['dctid']}";
        
        $sql = "SELECT dc.dcoid AS codigo, dd.dcddsc AS descricao
                FROM demandas.demandas_portfolio dc
                	INNER JOIN demandas.demandas_portfolio_descricao dd ON dd.dcdid = dc.dcdid
                WHERE dc.dcostatus = 1 $filtro";
        $arrDados = $this->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;        
    }
    
    public function carregarItensCardapio($arrPost ) {
        
        $filtro = '';
        if( $arrPost['dcoid'] ) $filtro = " and ci.dcoid = {$arrPost['dcoid']}";
        
        $sql = "SELECT id.cidid AS codigo, id.ciddsc AS descricao 
                FROM demandas.demandas_portfolio_itens ci 
                	INNER JOIN demandas.demandas_portfolio_itens_descricao id ON id.cidid = ci.cidid
                WHERE ci.dcistatus = 1
                    AND ci.dciid NOT IN (SELECT dciid FROM demandas.demandas_itens WHERE deistatus = 1)
                    $filtro";
        $arrDados = $this->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;        
    }
    
    public function carregarItensVinculadosDemandas( $demid ) {
                
        $sql = "SELECT di.deiid, ci.dciid, id.ciddsc, CASE WHEN di.deivalor IS NOT NULL THEN di.deivalor ELSE ci.dcivalor END AS valor_referencia, di.deiquantidade
                FROM demandas.demandas_portfolio_itens ci
                	INNER JOIN demandas.demandas_portfolio_itens_descricao id ON id.cidid = ci.cidid 
                	INNER JOIN demandas.demandas_itens di ON di.dciid = ci.dciid AND di.deistatus = 1
                WHERE ci.dcistatus = 1 
                    AND di.demid = $demid";
        $arrDados = $this->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
    public function carregarLinhaItensDemandas( $cidid ) {
        
        $sql = "SELECT ci.dciid, id.ciddsc, ci.dcivalor
                FROM demandas.demandas_portfolio_itens ci
                	INNER JOIN demandas.demandas_portfolio_itens_descricao id ON id.cidid = ci.cidid 
                WHERE ci.dcistatus = 1
                	AND ci.cidid = $cidid";
        $arrDados = $this->pegaLinha($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
    public function getDemandaPorID( $demid ) {
                
        if($demid){
            $sql = "SELECT d.demid, d.dcoid, d.ddtid, dc.dctid, d.demstatus FROM demandas.demandas d
                    	INNER JOIN demandas.demandas_portfolio dc ON dc.dcoid = d.dcoid
                    WHERE demid = $demid";
            $arrDados = $this->pegaLinha($sql);
        }
        $arrDados = $arrDados ? $arrDados : array();
        
        return $arrDados;
    }
    
    public function salvarDemanda($arrPost) {

        $sql = "INSERT INTO demandas.demandas(dcoid, ddtid, docid, demcpfresponsavel, demstatus)
                VALUES({$arrPost['dcoid']}, {$arrPost['ddtid']}, {$arrPost['docid']}, '{$_SESSION['usucpf']}', 1) returning demid;";
        $demid = $this->pegaUm($sql);
        $this->commit();
        
        return $demid;
    }
    
    public function atualizarDemanda($arrPost) {
        
        $sql = "UPDATE demandas.demandas SET 
                    dcoid = 0,  
                    ddtid = 0, 
                    demstatus = 1, 
                    docid = 0, 
                    demcpfresponsavel = ''
                WHERE demid = 



INSERT INTO demandas.demandas(dcoid, ddtid, docid, demcpfresponsavel, demstatus)
                VALUES({$arrPost['dcoid']}, {$arrPost['ddtid']}, {$arrPost['docid']}, '{$_SESSION['usucpf']}', 1) returning demid;";
        $demid = $this->pegaUm($sql);
        $this->commit();
        
        return $demid;
    }
    
    public function salvarItemCardapioDemandaLinha($arrPost) {
        
        $sql = "INSERT INTO demandas.demandas_itens(demid, dciid, deistatus, deiquantidade, deivalor)
                VALUES({$arrPost['demid']}, {$arrPost['dciid_linha']}, 1, 0, {$arrPost['dcivalor_linha']})";
        $this->executar($sql);        
        $this->commit();
    }
    
    public function salvarItemCardapioDemanda($arrPost) {
                    
        foreach ($arrPost['dciid'] as $dciid) {
            $dcivalor       = $arrPost['valor_referencia'][$dciid];
            $deiquantidade  = $arrPost['deiquantidade'][$dciid];
            $deiid          = $arrPost['deiid'][$dciid];            
            
            $dcivalor = str_replace(array(".",","), array("","."), $dcivalor);
            
            $sql = "UPDATE demandas.demandas_itens SET 
                        deistatus = 1,
                        deiquantidade = {$deiquantidade},
                        deivalor = {$dcivalor}
                    WHERE deiid = {$deiid}"; 
            
            /*$sql = "INSERT INTO demandas.demandas_itens(demid, dciid, deistatus, deiquantidade, deivalor)
                    VALUES({$arrPost['demid']}, {$dciid}, 1, {$deiquantidade}, {$dcivalor})";*/
            $this->executar($sql);
        }        
        $this->commit();
    }
    
    public function listarDemandas($arrPost){
        $filtro = '';
        
        if( $arrPost['demid'] ) $filtro = " and d.demid = {$arrPost['demid']}";
        if( $arrPost['dctid'] ) $filtro .= " and dc.dctid = {$arrPost['dctid']}";
        if( $arrPost['dcoid'] ) $filtro .= " and d.dcoid = {$arrPost['dcoid']}";
        
        $sql = "SELECT d.demid, d.docid, d.demid as id, ct.dctdsc, cd.dcddsc, es.esddsc, us.usunome, to_char(doc.docdatainclusao, 'DD/MM/YYYY') as datainclusao 
                FROM demandas.demandas d
                	INNER JOIN demandas.demandas_portfolio dc ON dc.dcoid = d.dcoid AND dc.dcostatus = 1
                	INNER JOIN demandas.demandas_portfolio_descricao cd ON cd.dcdid= dc.dcdid
                	INNER JOIN demandas.demandas_portfolio_tipo ct ON ct.dctid = dc.dctid
                	INNER JOIN workflow.documento doc ON doc.docid = d.docid
                	INNER JOIN workflow.estadodocumento es ON es.esdid = doc.esdid
                	INNER JOIN seguranca.usuario us ON us.usucpf = d.demcpfresponsavel
                WHERE d.demstatus = 1 $filtro";
        
    
        return $sql;
    }
    
    public function salva_pessoa_juridica( $arrPost ){
        $arrPost['enjtelefone'] = str_replace(array('(', '-', ')', ' '), '', $arrPost['enjtelefone']);
        $arrPost['enjcnpj']     = str_replace(array('.', '-', '/'), '', $arrPost['enjcnpj']);
        $arrPost['enjendcep']   = str_replace(array('.', '-', '/'), '', $arrPost['enjendcep']);
        
        if($arrPost['enjid']){
            $sql = "UPDATE corporativo.entidades_juridica SET 
                        enjdsc              = '".str_to_upper($arrPost['enjdsc'])."', 
                        ".($arrPost['enjcnpj'] ? "enjcnpj = '".($arrPost['enjcnpj'])."'," : "")." 
                        enjrazaosocial      = '".$arrPost['enjrazaosocial']."', 
                        enjinscricaoestadual= '".$arrPost['enjinscricaoestadual']."', 
                        enjemail            = '".$arrPost['enjemail']."', 
                        enjtelefone         = '".$arrPost['enjtelefone']."', 
                        enjendcep           = '".$arrPost['enjendcep']."', 
                        enjendlogradouro    = '".$arrPost['enjendlogradouro']."', 
                        enjendcomplemento   = '".$arrPost['enjendcomplemento']."', 
                        enjendnumero        = '".$arrPost['enjendnumero']."', 
                        enjendbairro        = '".$arrPost['enjendbairro']."'
                    WHERE enjid = {$arrPost['enjid']}";
            
            $this->executar($sql);
        } else {
            $sql = "INSERT INTO corporativo.entidades_juridica(enjdsc, enjstatus, enjcnpj, enjrazaosocial, enjinscricaoestadual, enjemail, enjtelefone, enjendcep, enjendlogradouro, enjendcomplemento, enjendnumero, enjendbairro, muncod)
                    VALUES('".str_to_upper($arrPost['enjdsc'])."',
                           1,
                           '".$arrPost['enjcnpj']."',
                           '".$arrPost['enjrazaosocial']."',
                           '".$arrPost['enjinscricaoestadual']."',
                           '".$arrPost['enjemail']."',
                           '".$arrPost['enjtelefone']."',
                           '".$arrPost['enjendcep']."',
                           '".$arrPost['enjendlogradouro']."',
                           '".$arrPost['enjendcomplemento']."',
                           '".$arrPost['enjendnumero']."',
                           '".$arrPost['enjendbairro']."',
                           '".$arrPost['muncod']."') returning enjid";
            $enjid = $this->pegaUm($sql);
            
            $sql = "INSERT INTO corporativo.entidades_juridicaxfuncao(enjid, enfid, ejfstatus)
                    VALUES({$enjid}, {$arrPost['enfid']}, 1) returning ejfid";
            $ejfidfilho = $this->pegaUm($sql);
            
            if( $_REQUEST['tipo_representante'] != 'G' && !in_array($arrPost['enfid'], array(1) ) ){
                $sql = "INSERT INTO corporativo.entidades_associativo(ejfid, ejfidfilho) VALUES(".$arrPost['ejfid_pai'].", {$ejfidfilho})";
                $this->executar($sql);
            }
        }
        return $this->commit();
    }
    
    public function salva_pessoa_fisica( $arrPost ){
        $arrPost['enstelefone']         = str_replace(array('(', '-', ')', ' '), '', $arrPost['enstelefone']);
        $arrPost['enscpf']              = str_replace(array('.', '-', '/'), '', $arrPost['enscpf']);
        $arrPost['enjendcep']           = str_replace(array('.', '-', '/'), '', $arrPost['enjendcep']);
        $arrPost['ensdatanascimento']   = $arrPost['ensdatanascimento'] ? "'".formata_data_sql($arrPost['ensdatanascimento'])."'" : 'null';
        
        if($arrPost['ensid']){            
            $sql = "UPDATE corporativo.entidades_fisica SET
                        ensdsc              = '".str_to_upper($arrPost['ensdsc'])."',
                        --ensstatus           = 1,
                        --enscpf             = '".$arrPost['enscpf']."',
                        ensemail            = '".$arrPost['ensemail']."',
                        enstelefone         = '".$arrPost['enstelefone']."',
                        ensendcep           = '".$arrPost['enjendcep']."',
                        ensendlogradouro    = '".$arrPost['enjendlogradouro']."',
                        ensendcomplemento   = '".$arrPost['enjendcomplemento']."',
                        ensendnumero        = '".$arrPost['enjendnumero']."',
                        ensendbairro        = '".$arrPost['enjendbairro']."',
                        ensdatanascimento   = ".$arrPost['ensdatanascimento'].",
                        ensrg               = '".$arrPost['ensrg']."',
                        ensorgexpedidor = '".str_to_upper($arrPost['ensorgexpedidor'])."',
                        enssexo             = '".str_to_upper($arrPost['enssexo'])."'
                    WHERE ensid = {$arrPost['ensid']}";
            
            $this->executar($sql);
        } else {            
            $sql = "INSERT INTO corporativo.entidades_fisica(ensdsc, ensstatus, enscpf, ensemail, enstelefone, ensendcep, ensendlogradouro, ensendcomplemento, ensendnumero, ensendbairro, ensdatanascimento, ensrg, ensorgexpedidor, enssexo)
                    VALUES('".str_to_upper($arrPost['ensdsc'])."',
                           1,
                           '".$arrPost['enscpf']."',
                           '".$arrPost['ensemail']."',
                           '".$arrPost['enstelefone']."',
                           '".$arrPost['enjendcep']."',
                           '".$arrPost['enjendlogradouro']."',
                           '".$arrPost['enjendcomplemento']."',
                           '".$arrPost['enjendnumero']."',
                           '".$arrPost['enjendbairro']."',
                           ".$arrPost['ensdatanascimento'].",
                           '".$arrPost['ensrg']."',
                           '".str_to_upper($arrPost['ensorgexpedidor'])."',
                           '".str_to_upper($arrPost['enssexo'])."') returning ensid";
            
            $ensid = $this->pegaUm($sql);
            $sql = "INSERT INTO corporativo.entidades_fisicaxfuncao(ensid, enfid, effstatus)
                    VALUES({$ensid}, {$arrPost['enfid']}, 1) returning effid";
            $effid = $this->pegaUm($sql);
            
            if( $_REQUEST['tipo_representante'] != 'G' ){
                $sql = "INSERT INTO corporativo.entidades_associativo(effid, ejfid) VALUES($effid, ".$arrPost['ejfid'].")";
                $this->executar($sql);
            }
        }
        return $this->commit();
    }
}