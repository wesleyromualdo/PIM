<?php

class Par3_Model_Municipio extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "territorios.municipio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("muncod");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'muncod' => null,
        'estuf' => null,
        'miccod' => null,
        'mescod' => null,
        'mundescricao' => null,
        'munprocesso' => null,
        'muncodcompleto' => null,
        'munmedlat' => null,
        'munmedlog' => null,
        'munhemis' => null,
        'munaltitude' => null,
        'munmedarea' => null,
        'muncepmenor' => null,
        'muncepmaior' => null,
        'munmedraio' => null,
        'munmerid' => null,
        'muncodsiafi' => null,
        'munpopulacao' => null,
    );

    public function montaTabelaEstados($arInner = array(), $arWhere = array(), $exporta = null, $tipo = '') {
        $link = " '' ";

        $perfis = pegaArrayPerfil($_SESSION['usucpf']);
    	if (is_array($perfis)) {
            if (count(array_intersect(unserialize(GRUPO_DE_PERFIL_ESTADUAL), $perfis)) > 0 || count(array_intersect(unserialize(GRUPO_DE_PERFIL_MUNICIPAL), $perfis)) > 0) {
                $link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreExecucaoOrcamento(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
            } else {
                $link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalho(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
            }
        } else if ($perfis !== NULL) {
            if (in_array($perfis, unserialize(GRUPO_DE_PERFIL_ESTADUAL)) || in_array($perfis, unserialize(GRUPO_DE_PERFIL_MUNICIPAL))) {
                $link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreExecucaoOrcamento(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
            } else {
                $link = " '<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abrePlanoTrabalho(\'municipio\',\''|| m.estuf ||'\',\''|| m.muncod ||'\',\''|| CASE WHEN iu.inuid IS NULL THEN 0 ELSE iu.inuid END 	||'\')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>'";
            }
        }

        $today = date('Y-m-d');
        $acaoBtn = $link . " as acao,";
        $cache = '3600';

        $cabecalho = array("Ação", "Código", "Município", "UF");

        $sql = "SELECT DISTINCT
        			" . $acaoBtn . "
					m.muncod, m.mundescricao, m.estuf
				FROM territorios.municipio m
				LEFT JOIN par.instrumentounidade 	iu ON iu.mun_estuf = m.estuf AND iu.muncod = m.muncod AND iu.itrid = 2
				{$arInner['join_grupo']}
				" . ( is_array($arWhere) && count($arWhere) ? ' WHERE ' . implode(' AND ', $arWhere) : '' ) . "
				ORDER BY m.mundescricao
				";

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS, "*");
        $listagem->setQuery($sql);
        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

    }

    public function descricaoMunicipio($muncod, $boMostraEstuf = true) {
        if ($boMostraEstuf) {
            return $this->pegaUm("SELECT estuf || ' - ' || mundescricao FROM territorios.municipio where muncod = '$muncod' ");
        } else {
            return $this->pegaUm("SELECT mundescricao FROM territorios.municipio where muncod = '$muncod' ");
        }
    }

    public function recuperarUF($muncod) {
        $sql = "SELECT estuf FROM {$this->stNomeTabela} WHERE muncod = '{$muncod}'";
        return $this->pegaUm($sql);
    }

    public function carregarMunicipio() {
        $sql = "SELECT
                        muncod AS codigo,
                        estuf ||' - '|| mundescricao AS descricao
                    FROM
                        {$this->stNomeTabela}
                    ORDER BY
                        mundescricao;";
        return $this->carregar($sql);
    }

    public function carregarMunicipioPorMuncod($muncod) {
        $sql = "SELECT
                        muncod AS codigo,
                        estuf ||' - '|| mundescricao AS descricao
                    FROM
                        {$this->stNomeTabela}
                    WHERE muncod = '{$muncod}'
                    ORDER BY
                        mundescricao;";
        return $this->pegaLinha($sql);
    }

    public function carregarGrupoMunicipio($booSQL){
          $sql = "SELECT
                    tpmid as codigo,
                    tpmdsc as descricao
                FROM
                    territorios.tipomunicipio
                WHERE
                    tpmstatus = 'A'
                    AND gtmid in ( 1,2 )
                ORDER BY
                    descricao";
          return ($booSQL) ? $sql : $this->carregar($sql);
    }

    public function montaSQLComboMunicipiosUF($inuid){
        $sql = "SELECT mun.muncod as codigo, mun.mundescricao as descricao
                FROM territorios.municipio mun
                INNER JOIN par3.instrumentounidade inu ON inu.estuf = mun.estuf
                WHERE inuid = $inuid";
        return $sql;
    }
    
    public function pegarSQLSelectCombo($muncod = array(), $estuf = array(), $opcoes = array())
    {
    	$muncod = is_array($muncod) ? implode(',', $muncod):$muncod;
    	$estuf = is_array($estuf) ? $estuf : array_filter(explode(",", $estuf));
    	
    	if(!empty($estuf)){
	    	if(count($estuf) == 1 && $estuf){
	    		$estuf = "'$estuf[0]'";
	    	} else {
	    		$estados = array();
	    		foreach($estuf as $e){
	    			// Verifica se já possui áspas simples. Se sim, mantém. Se não, inclui no iníco e final da sigla.
	    			if(strstr ($e, "'")){
	    				array_push($estados, $e);
	    			} else {
	    				array_push($estados, "'$e'");
	    			}
	    		}
	    		$estuf = implode(",", $estados);
	    	}
    	}
    	
    	
    	$where = $muncod ? " AND muncod in({$muncod})":'';
    	$where .= $estuf ? " AND estuf in({$estuf})":'';
    	$where = $where ? " WHERE 1=1 $where" : "";
    	    	
    	$sql = "SELECT  muncod as codigo, mundescricao as descricao FROM {$this->stNomeTabela} $where";
    	    	
    	if(array_key_exists('incluirOpcaoTodos', $opcoes)){
    		$sql = 
    			"SELECT * FROM (
    				(SELECT 'T' as codigo, 'Todos' as descricao)
    				UNION
    				({$sql})
    			) municipios
    			ORDER BY
    				CASE WHEN codigo = 'T' THEN 1 END,
    				descricao";
    	}

    	return $sql;
    }

    /**
     * @param $id
     * @return array|bool|mixed|NULL
     */
    function getMunicipio($id){
        $sql = "SELECT * FROM territorios.municipio WHERE muncod = '{$id}'";
        return $this->pegaLinha($sql);
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $uf
     * @return string
     * @description 'Esta função retorna o sql com os municípios com/sem filtro por estado'
     * @exemple
     *          Com filtro por 'estuf' $objeto->getSqlListaMunicipioByUF('AC'); //pega todos os municípios do Acre
     *          Sem filtro por 'estuf' $objeto->getSqlListaMunicipioByUF(); //pega todos os municípios de todos os estados
     */
    public function getSqlListaMunicipioByUF($uf = null){
        $sql = '';
        if($uf != null){
            $arrayEstados = explode(',',$uf);
            $where = ' WHERE ';
            $val = (count($arrayEstados)-1);
           if(count($arrayEstados) > 0){
                for($i = 0; $i <= count($arrayEstados); $i++){
                    if($val > $i){
                        $where .= "mun.estuf = '{$arrayEstados[$i]}' OR  ";
                    }elseif ($val == $i){
                        $where .= "mun.estuf = '{$arrayEstados[$i]}' ";
                    }
                }
           }
           if($where !=  ''){
               $sql = "select mun.muncod as codigo, mun.mundescricao as descricao from territorios.municipio as mun {$where}   GROUP BY mun.estuf, mun.muncod ORDER BY mun.mundescricao ASC ;";
           }
        }else{
           $sql = "select mun.muncod as codigo,mun.mundescricao as descricao from territorios.municipio as mun GROUP BY mun.estuf, mun.muncod ORDER BY mun.mundescricao ASC ";
        }
        return $sql;
    }


}
