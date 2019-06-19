<?php

class Par3_Model_IniciativaAreasRelacionadasProgramas extends Modelo
{
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "par3.iniciativa_areasrelacionadas_programas";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('iapid');

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'prgid'  => array('tabela' => 'par3.programa',                        'pk' => 'prgid'),
        'iuoid' => array('tabela'  => 'par3.iniciativa_unidade_orcamentaria', 'pk' => 'iuoid'),
        'iarid' => array('tabela'  => 'par3.iniciativa_areas',                'pk' => 'iarid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'iapid'                  => null,
        'iapstatus'              => null,
        'iuoid'                  => null,
        'iarid'                  => null,
        'prgid'                  => null,
        'iapsituacao'            => null,
        'iapstatus'              => null,
        'iapdtinclusao'          => null,
        'iapcpfinclusao'         => null,
        'iapcpfinativacao'       => null,
        'iapdtinativacao'        => null,
    );

    public function salvarAreaRelacionadaProgramas($arrPost)
    {
        $modelHistorico = new Par3_Model_IniciativaAreasRelacionadasProgramasHistorico();
        //Valida campos
        $erro = $this->validarInput($arrPost);
        if($erro){return $erro;}

        $arrAreaRelacionada['iapstatus']        = 'A';
        $arrAreaRelacionada['iapcpfinclusao']   = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
        $arrAreaRelacionada['iapdtinclusao']    = date('Y-m-d H:m:s');
        $arrProgramas                           = prepararArraySelectMultiSelecao($arrPost['prgid']);
        $arrAreaRelacionada['iarid']            = $arrPost['iarid'];
        $arrAreaRelacionada['iuoid']            = $arrPost['iuoid'];
        $arrAreaRelacionada['iapdtinclusao']    = date('Y-m-d H:m:s');
        $arrAreaRelacionada['iapcpfinclusao']   = $_SESSION['usucpf'];
        try {
            foreach ($arrProgramas as $prg){
                $arrAreaRelacionada['prgid'] = $prg;
                if($iap = $this->recuperarPorAreaPrograma($arrAreaRelacionada['iarid'],$prg)){
                    $sql = <<<DML
                            UPDATE {$this->stNomeTabela}
                            SET
                            iapstatus        = 'A',
                            iapdtinclusao    = '{$arrAreaRelacionada['iapdtinclusao']}',
                            iapcpfinclusao   = '{$arrAreaRelacionada['iapcpfinclusao']}'
                            WHERE iarid = {$arrAreaRelacionada['iarid']} AND prgid = {$prg}
DML;
                    $this->executar($sql);
                    $this->commit();
                    $model = new Par3_Model_IniciativaAreasRelacionadasProgramas($iap['iapid']);
                    $modelHistorico->gravarHistorico($model,$modelHistorico::UPDATE);
                    $modelHistorico->commit();
                    continue;
                }
                $iapid = $this->salvarAreaRelacionadaPrograma($arrAreaRelacionada);
                $model = new Par3_Model_IniciativaAreasRelacionadasProgramas($iapid);
                $modelHistorico->gravarHistorico($model,$modelHistorico::CREATE);
                $modelHistorico->commit();
            }

            return $arrPost['iarid'];
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
    }

    public function salvarAreaRelacionadaPrograma($arrPost)
    {
        $mAreasRelacionadas = new Par3_Model_IniciativaAreasRelacionadasProgramas();
        $mAreasRelacionadas->popularDadosObjeto($arrPost);
        $iapid = $mAreasRelacionadas->salvar();
        $mAreasRelacionadas->commit();
        return $iapid;
    }

    public function desvincularAreaRelacionadaProgramas($iapid)
    {
        $modelHistorico = new Par3_Model_IniciativaAreasRelacionadasProgramasHistorico();
        $arrAreaRelacionadaPrograma['iapid']             = $iapid;
        $arrAreaRelacionadaPrograma['iapstatus']         = 'I';
        $arrAreaRelacionadaPrograma['iapdtinativacao']   = date('Y-m-d H:m:s');
        $arrAreaRelacionadaPrograma['iapcpfinativacao']  = $_SESSION['usucpf'];
        try {
            $this->popularDadosObjeto($arrAreaRelacionadaPrograma);
            $iapid = $sucesso = $this->salvar();
            $model = new Par3_Model_IniciativaAreasRelacionadasProgramas($iapid);
            $modelHistorico->gravarHistorico($model,$modelHistorico::DELETE);
            $modelHistorico->commit();
            $this->commit();
            return $sucesso;
        } catch (Simec_Db_Exception $e) {return 'erro';}
    }

    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['iarid'] = array();
        $erros['erros']['iuoid'] = array();
        $erros['erros']['prgid'] = array();

        $arrPrg = array();
        if($campos['prgid']){
            foreach($campos['prgid'] as $prg){
                if($prg == ''){
                    continue;
                }
                $arrPrg[] = $prg;
            }
        }

        if($campos['iarid'] == '' || $campos['iarid'] == null){array_push($erros['erros']['iarid'],'Selecione Área Relacionada');}
        if($campos['iuoid'] == '' || $campos['iuoid'] == null){array_push($erros['erros']['iuoid'],'Selecione uma Unidade Orçamentária');}
        if(empty($arrPrg)){array_push($erros['erros']['prgid'],'Selecione um Programa');}
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function recuperarPorAreaPrograma($iarid,$prgid)
    {
        $sql = "SELECT  *
                FROM {$this->stNomeTabela}
                WHERE iarid = {$iarid}
                AND   prgid = {$prgid}
                ";
        $teste = $this->recuperar($sql);
        return $teste;
    }

    public function recuperarPorArea($iarid)
    {
        $sql = "SELECT  *
                FROM {$this->stNomeTabela}
                WHERE iarid = {$iarid}
                AND iapstatus = 'A'
                ";
        $retorno = $this->recuperar($sql);
        return $retorno;
    }

    public function recuperarPrograma($iapid)
    {
        $sql = "SELECT prg.prgid,prg.prgdsc,prg.prgabreviatura
                FROM $this->stNomeTabela AS iap
                INNER JOIN par3.programa AS  prg ON prg.prgid = iap.prgid
                WHERE iap.iapid = {$iapid}
              ";
        return $this->recuperar($sql);
    }

    public function pegarSQLSelectComboAreasPorPrograma($prgid)
    {
    	$prgid = is_array($prgid) ? implode(',',$prgid):$prgid;
    	$where = "WHERE iap.iapstatus = 'A'";
    	$where .= $prgid ? " AND iap.prgid in({$prgid})":'';

    	return "SELECT iar.iarid as codigo, iar.iardsc as descricao
    			FROM {$this->stNomeTabela} AS iap
    			INNER JOIN par3.iniciativa_areas iar ON iar.iarid = iap.iarid
    			$where
    			ORDER BY 2";
    }


    public function dataGridSQL($arrPost)
    {
        $arrPrg = $arrPost['prgid']?implode($arrPost['prgid']):'';
        $iardsc = sanitizar_string_pesquisa($arrPost['iardsc']);
        $where  = $arrPost['iarid']?" AND iap.iarid = {$arrPost['iarid']}":'';
        $where .= $arrPost['iuoid']?" AND iap.iuoid = {$arrPost['iuoid']}":'';
        $where .= $arrPrg?" AND iap.prgid in ({$arrPrg})":'';
        $where .= $arrPost['iarsituacao']?" AND iarsituacao = {$arrPost['iarsituacao']}":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":" ";
        $inicio = $arrPost['req'] == 'imprimir' || $arrPost['req'] == 'xls' ? "SELECT ROW_NUMBER() OVER ({$orderBy}) AS qtd,*FROM (":'';
        $fim = $arrPost['req']    == 'imprimir' || $arrPost['req'] == 'xls'? ") as foo":'';

        $mEstadoDocumento  = new Workflow_Model_EstadoDocumento();
        $rsEstadoDocumento = $mEstadoDocumento->recuperarTodos('esdid,esddsc',array('tpdid ='.Par3_Model_Iniciativa::WORKFLOW_TIPO_DOCUMENTO_INICIATIVA));
        $arrEstadosDocumentos = array();
        foreach($rsEstadoDocumento as $esd){
            $arrEstadosDocumentos[] = $esd;
        }

        $estadosDocumentos = array();
        foreach($arrEstadosDocumentos as $esd){
            if($arrPost['req'] == 'imprimir' || $arrPost['req'] == 'xls'){
                $estadosDocumentos[] = <<<DML
             (CASE WHEN (SELECT count(*) FROM par3.iniciativa_areasrelacionadas_programas_estadodocumento WHERE esdid = ({$esd['esdid']}) AND iapid = iap.iapid AND iawstatus = 't') > 0 THEN 'Sim' ELSE 'Não' END) as estadounidade{$esd['esdid']}
DML;
                continue;
            }
            $estadosDocumentos[] = <<<DML
             (
                '<input type=\"checkbox\" class=\"classeunidademedida\" onclick=\"vincularEstadoDocumento({$esd['esdid']},'||iap.iapid||')\" id=\"classecheck{$esd['esdid']}'||iap.iarid||'\"'
                 ||(CASE WHEN (SELECT count(*) FROM par3.iniciativa_areasrelacionadas_programas_estadodocumento WHERE esdid = ({$esd['esdid']}) AND iapid = iap.iapid AND iawstatus = 't') > 0 THEN 'checked'ELSE '' END) ||
                 '/>'
             ) as estadounidade{$esd['esdid']}
DML;
        }
        $listaestdoc = implode(',',$estadosDocumentos);
        $sql = "
                {$inicio}
                SELECT DISTINCT
                    iap.iapid AS id,
                    CASE 
                    	WHEN prg.prgabreviatura IS NULL OR TRIM(prg.prgabreviatura) = '' THEN prg.prgdsc
                    	WHEN TRIM(prg.prgabreviatura) IS NOT NULL THEN prg.prgabreviatura
                    END as sigla,
                    iuo.iuodsc as unidadegestora,
                    iar.iarsigla||' - '||iar.iardsc as arearelacionada,
                    {$listaestdoc}
                FROM  par3.iniciativa_areas as iar
                INNER JOIN par3.iniciativa_areasrelacionadas_programas as iap ON iap.iarid = iar.iarid
                INNER JOIN par3.iniciativa_unidade_orcamentaria as iuo ON iuo.iuoid = iar.iuoid
                INNER JOIN par3.programa as prg ON prg.prgid = iap.prgid
                WHERE iap.iapstatus = 'A'
                AND iap.iarid = iar.iarid
                {$where}
                {$orderBy}
                {$fim}";
        return $sql;
    }
}//id,codigo,unidadegestora,arearelacionada,programas,
