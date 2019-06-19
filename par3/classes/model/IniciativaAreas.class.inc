<?php

class Par3_Model_IniciativaAreas extends Modelo
{
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "par3.iniciativa_areas";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('iarid');

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'iuoid' => array('tabela' => 'par3.iniciativa_unidade_orcamentaria', 'pk' => 'iuoid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'iarid'            => null,
        'co_unidade_org'   => null,
        'iarid_pai'        => null,
        'iuoid'            => null,
        'iarsigla'         => null,
        'iardsc'           => null,
        'iarstatus'        => null,
        'iarsituacao'      => null,
    );

    public function montarSQLSimples($arrPost)
    {
        $iardsc   = sanitizar_string_pesquisa($arrPost['iardsc']);
        $iarsigla = $arrPost['sigla'];

        $whereSituacao  = $arrPost['iarsituacao'] == 'I'? "WHERE iarsituacao = 'I'" : "WHERE iarstatus = 'A'";

        $where    = $arrPost['iardsc']?" AND upper(removeacento(iardsc)) LIKE upper(removeacento('%{$iardsc}%'))":'';
        if($iarsigla){
            $iar = new Par3_Model_IniciativaAreas($iarsigla);
            $where .= $iar->iarsigla ?" AND iarsigla LIKE '%{$iar->iarsigla}%'":'';
        }
        //$where   .= $arrPost['iarsituacao']?" AND iarsituacao = '{$arrPost['iarsituacao']}'":'';
        $where   .= $arrPost['iarid']?" AND iar.iarid = {$arrPost['iarid']} OR co_unidade_org = {$arrPost['iarid']}":'';
        $where   .= $arrPost['iuoid']?" AND iar.iuoid = {$arrPost['iuoid']}":'';


        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                    iarid as id,
                    ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                    iar.co_unidade_org AS codigo,
                    iar.iardsc,
                    iar.iarsigla as sigla,
                    iuo.iuodsc,
                    iar.iarsituacao
                FROM {$this->stNomeTabela} as iar
                INNER JOIN par3.iniciativa_unidade_orcamentaria as iuo ON iuo.iuoid = iar.iuoid
                {$whereSituacao}
                    {$where}
                    {$orderBy}";
        return $sql;
    }

    public function verificarExisteArea(array $arrPost)
    {
        $where = $arrPost['iarid']?" AND iarid != {$arrPost['iarid']}" : ' ';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE upper(iardsc) = '".str_to_upper($arrPost['iardsc'])."' AND iarstatus = 'A' {$where}";
        return $this->pegaLinha($sql,0,0);
    }

    public function verificarExisteCodigoArea(array $arrPost)
    {
        $where  = $arrPost['iarid']?" AND iarid != {$arrPost['iarid']}" : ' ';
        $where .= $arrPost['co_unidade_org']?" AND co_unidade_org = {$arrPost['co_unidade_org']}" : ' ';
        $where .= $arrPost['iuoid']?" AND iuoid = {$arrPost['iuoid']}" : ' ';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE iarstatus = 'A' {$where}";
        return $this->pegaLinha($sql,0,0);
    }

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['iardsc'] = array();
        $erros['erros']['iuoid']  = array();
        $erros['erros']['co_unidade_org']  = array();



        //Validações
        if($campos['iuoid'] == ''  || $campos['iuoid']  == null){array_push($erros['erros']['iuoid'],'O campo não pode ser vazio');}
        if($campos['iardsc'] == '' || $campos['iardsc'] == null){array_push($erros['erros']['iardsc'],'O campo não pode ser vazio');}
        if($campos['co_unidade_org'] == '' || $campos['co_unidade_org'] == null){array_push($erros['erros']['co_unidade_org'],'O campo não pode ser vazio');}
        if($this->verificarExisteArea($campos)){array_push($erros['erros']['iardsc'], 'Já existe Área cadastrada com essa descrição');}
        if($this->verificarExisteCodigoArea($campos)){array_push($erros['erros']['co_unidade_org'], 'Já existe Área cadastrada com esse código');}
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }
//"SELECT  iarid as codigo, iardsc as descricao FROM  par3.iniciativa_areas WHERE iuoid = {$irr->iuoid} AND iarstatus = 'A' ";
    public function pegarSQLSelectComboSigla($arrPost = null)
    {
        $where = '';
        if($arrPost){
            $where  = $arrPost['iuoid']?" AND iuoid = {$arrPost['iuoid']}":"";
        }
        $sql = "SELECT DISTINCT
                	iarid as codigo,
                	iarsigla||' - '||iardsc as descricao
                FROM
                	par3.iniciativa_areas
                WHERE iarstatus = 'A'
                {$where}
                ORDER BY
                	2";
        return $sql;
    }

    public function pegarSQLSelectComboDescricao($arrPost = null)
    {
    	$where = '';
    	if($arrPost){
            $where   = $arrPost['iuoid']        ? " AND iuoid      = {$arrPost['iuoid']}":"";
            $where  .= $arrPost['areafilho']    ? " AND iarid != {$arrPost['areafilho']}":"";
            $where  .= $arrPost['iarid']        ? " AND iarid IN (".implode(', ',$arrPost['iarid']).")" : "";
        }
    	$sql = "SELECT DISTINCT
    	    		iarid as codigo,
    	    		iarsigla||' - '||iardsc as descricao
		    	FROM
		    		par3.iniciativa_areas
		    	WHERE iarstatus = 'A'
			    	{$where}
		    	ORDER BY
			    	2";



    	return $sql;
    }


    public function pegarSQLSelectComboDescricaoUnidade($inuid)
    {

        $sql = "SELECT DISTINCT
    	    		iar.iarid as codigo,
    	    		iar.iarsigla||' - '||iar.iardsc as descricao
		    	FROM
		    		par3.iniciativa_areas iar
		    		INNER JOIN par3.iniciativa_iniciativas_areas iniiar ON iniiar.iarid = iar.iarid 
		    	    INNER JOIN par3.iniciativa as ini ON ini.iniid = iniiar.iniid
		    	    inner join par3.iniciativa_planejamento as inp on inp.iniid =  ini.iniid
		    	WHERE inp.inpstatus  = 'A' and inp.inuid = {$inuid}
			    
		    	ORDER BY
			    	2";



        return $sql;
    }



    public function listaAreas($arrPost = null)
    { 
        $sql = $this->pegarSQLSelectComboSigla($arrPost);
        $rsArea = $this->carregar($sql);
        return is_array($rsArea)?simec_json_encode($rsArea):false;
    }

    public function getIniciativaAreaById($iarid){
        $sql = "SELECT * FROM par3.iniciativa_areas as iniciativa_areas WHERE iniciativa_areas.iarid = {$iarid}";
        $result = $this->pegaLinha($sql);
        return $result;
    }

    public function verificaLigacaoPlanejamentoById($iarid){
        $sql = "SELECT iniciativa_areas.* FROM par3.iniciativa_planejamento as iniciativa_planejamento
                    INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iniciativa_planejamento.iniid
                    INNER JOIN par3.iniciativa_iniciativas_areas as iniciativa_iniciativas_areas ON iniciativa_iniciativas_areas.iniid = iniciativa.iniid
                    INNER JOIN par3.iniciativa_areas as iniciativa_areas ON iniciativa_areas.iarid = iniciativa_iniciativas_areas.iarid
                    WHERE iniciativa_areas.iarid = {$iarid}";
        $result = $this->carregar($sql);
        return $result;
    }

    public function getAreaPorIniciativa(){
        $sql = "SELECT  DISTINCT iarid as codigo, (ia.iarsigla ||' - '|| ia.iardsc) as descricao FROM par3.iniciativa_planejamento ip
                INNER JOIN par3.iniciativa_iniciativas_areas iia USING (iniid)
                INNER JOIN par3.iniciativa_areas ia USING (iarid)
                WHERE ip.inpstatus = 'A' 
                ORDER BY descricao ASC";
        return $sql;
    }
}