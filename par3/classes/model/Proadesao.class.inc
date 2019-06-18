<?php

class Par3_Model_Proadesao extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par3.proadesao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pfaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pfaid' => null,
        'prgid' => null,
        'pfadatainicial' => null,
        'pfadatafinal' => null,
        'pfaano' => null,
        'pfaicone' => null,
        'pfaesfera' => null,
        'pfastatus' => null,
        'pfatermomunicipio' => null,
        'pfamsgboasvindas' => null,
        'pfamsgdeaceitacao' => null,
        'pfamsgdenaoaceitacao' => null,
        'pfatermoestado' => null,
        'pfabackground' => null,
        'pfafundocor' => null,
        'pfasobre' => null,
        'pfavalortotalprograma' => null,
        'arqid' => null,
        'arqidmanual' => null,
     );

	protected $stOrdem = null;

	/**
	 * Função pegarEntidAtivoPorTipo
	 * - retorna o entid ativo.
	 *
	 * @return integer entid.
	 *
	 */
	public function pegarAdesaoPorPrograma($prgid, $pfaano = null)
	{

		$aryWhere[] = "pfastatus = 'A'";

		if($prgid) $aryWhere[] = "prgid = {$prgid}";
		if($pfaano) $aryWhere[] = "pfaano = {$pfaano}";

		$sql = "SELECT	pfaid, 
						prgid, 
						pfadatainicial, 
						pfadatafinal, pfaano, pfaicone, 
       					pfaesfera, pfastatus, pfatermoestado, pfatermomunicipio, pfamsgboasvindas, 
       					pfamsgdeaceitacao, pfamsgdenaoaceitacao, pfafundocor, pfabackground, 
       					pfasobre, pfastatus, pfavalortotalprograma
				FROM 	par3.proadesao
					 	".(is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '')."";

		return $this->pegaLinha($sql);
	}
	
	public function recuperaUFs($pfaid)
	{
	    $sql = "SELECT estuf FROM par3.proadesao_ufs WHERE pfaid = $pfaid";

	    return $this->carregarColuna($sql);
	}
	
	public function salvarUFs($arrPost)
	{
	    $arrValues = array();
	    foreach($arrPost['ufs'] as $estuf){
	        $arrValues[] = "({$arrPost['pfaid']}, '$estuf')";
	    }
	    
	    $sql = "DELETE FROM par3.proadesao_ufs WHERE pfaid = {$arrPost['pfaid']};";
	    
	    if($arrValues[0] != ''){
	        $sql .= "
                INSERT INTO par3.proadesao_ufs(pfaid, estuf)
                VALUES ".implode(', ', $arrValues).";";
	    }

	    $this->executar($sql);
	    $this->commit();
	}

    public function pegarProgramas($inuid){
        if(!$inuid){
        	return false;
        }



        $hoje = date("Y-m-d");     
        
        $sql = "
            --BUSCAS TODOS PROGRAMAS QUE ESTÃO RESTRITOS AS UNIDADES (instrumentounidade), RESPEITANDO AS REGRAS DE ADESÃO.
            SELECT  DISTINCT pro.prgid,
                    pa.pfadatainicial,
                    pa.pfadatafinal,
                    pa.pfaicone,
                    pa.pfaid,
                    pa.pfaesfera,
                    pro.prgdsc,
                    pro.prgabreviatura,
                    pro.prgarquivo,
                    pa.pfafundocor,
                    pa.pfabackground,
                    pfastatus,
                    
                    CASE
                        WHEN pfadatainicial <= '{$hoje}'::date AND pfadatafinal >= '{$hoje}'::date AND (pap.adpresposta <> 'S' OR pap.adpresposta IS NULL) THEN 'ATIVO'
                        WHEN pfadatainicial <= '{$hoje}'::date AND pfadatafinal >= '{$hoje}'::date AND pap.adpresposta = 'S' THEN 'CADASTRADO'
                        WHEN pfadatainicial > '{$hoje}'::date THEN 'INATIVO'
                        WHEN pfadatafinal < '{$hoje}'::date AND pap.adpresposta = 'S' THEN 'CADASTRADO'
                        ELSE 'A adesão ao programa encerrou dia '||TO_CHAR(pfadatafinal, 'DD/MM/YYYY')||'.'
                    END as vigencia                                
            FROM par3.proadesao pa
            JOIN par3.programa pro ON pro.prgid = pa.prgid AND prgstatus = 'A'
            JOIN par3.programarestricaoestadomunicipio  ppr on ppr.prgid = pro.prgid 
            LEFT JOIN par3.prodesaoprograma pap ON pap.pfaid = pa.pfaid AND pap.adpresposta = 'S' AND pap.inuid = {$inuid} AND
		              pa.pfaid IN (SELECT pfaid FROM par3.proadesao 
                                    WHERE (prgid, pfadatafinal) IN (SELECT prgid, MAX(pfadatafinal) FROM par3.proadesao GROUP BY prgid) )
            WHERE   
                CASE    
                    WHEN (SELECT DISTINCT TRUE FROM par3.proadesao_ufs p WHERE p.pfaid = pa.pfaid) THEN
                        (SELECT DISTINCT TRUE FROM par3.proadesao_ufs p2 
                        INNER JOIN par3.instrumentounidade inu ON inu.estuf = p2.estuf AND inu.itrid <> 2 AND inu.inuid = $inuid)
                    ELSE TRUE
                END

            UNION

            --BUSCAS TODOS PROGRAMAS DE AMBAS ESFERAS ESTADUAL/MUNICÍPAL, MENOS OS PROGRAMAS QUE SÃO RESTRITOS A UNIDADES. (unidades que estão gravaos na tabela par3.programarestricaoestadomunicipio com a falg 'N' do campo prgrestrito).
            SELECT  pro.prgid,
                    pa.pfadatainicial,
                    pa.pfadatafinal,
                    pa.pfaicone,
                    pa.pfaid,
                    pa.pfaesfera,
                    pro.prgdsc,
                    pro.prgabreviatura,
                    pro.prgarquivo,
                    pa.pfafundocor,
                    pa.pfabackground,
                    pfastatus,
                    
                    CASE
                        WHEN pfadatainicial <= '{$hoje}'::date AND pfadatafinal >= '{$hoje}'::date AND (pap.adpresposta <> 'S' OR pap.adpresposta IS NULL) THEN 'ATIVO'
                        WHEN pfadatainicial <= '{$hoje}'::date AND pfadatafinal >= '{$hoje}'::date AND pap.adpresposta = 'S' THEN 'CADASTRADO'
                        WHEN pfadatainicial > '{$hoje}'::date THEN 'INATIVO'
                        WHEN pfadatafinal < '{$hoje}'::date AND pap.adpresposta = 'S' THEN 'CADASTRADO'
                        ELSE 'A adesão ao programa encerrou dia '||TO_CHAR(pfadatafinal, 'DD/MM/YYYY')||'.'
                    END as vigencia                                
            FROM par3.proadesao pa                                
            INNER JOIN par3.programa pro ON pro.prgid = pa.prgid AND prgstatus = 'A'
            LEFT  JOIN par3.prodesaoprograma pap ON pap.pfaid = pa.pfaid AND pap.adpresposta = 'S' AND pap.inuid = {$inuid}
                
            WHERE    
                CASE    
                    WHEN (SELECT DISTINCT TRUE FROM par3.proadesao_ufs p WHERE p.pfaid = pa.pfaid) THEN
                        (SELECT DISTINCT TRUE FROM par3.proadesao_ufs p2 
                        INNER JOIN par3.instrumentounidade inu ON inu.estuf = p2.estuf AND inu.itrid <> 2 AND inu.inuid = $inuid)
                    ELSE TRUE
                END AND
                pa.pfastatus = 'A' AND 
                prgrestrito = 'N' AND 
                (
                    CASE WHEN pro.prgid = 2 
                        THEN(
                            SELECT DISTINCT TRUE
                            FROM par3.instrumentounidade inu
                            INNER JOIN par3.proadesao_lista_escolas_prioritarias pae ON (inu.itrid = 2 AND pae.muncod = inu.muncod AND pae.tp_dependencia = 3) OR (inu.itrid <> 2 AND pae.estuf = inu.estuf AND pae.tp_dependencia = 2)
                            WHERE inuid = {$inuid}
                        )
                        ELSE TRUE
                    END
                )
                /*Proemi só irá aparecer caso seja estadual*/
                AND (
                    CASE WHEN prgabreviatura ilike '%PROEMI%' 
                        THEN 
                            CASE WHEN (SELECT count(inu.inuid) FROM par3.instrumentounidade inu where muncod is null and itrid <> 2 and inu.inuid = {$inuid}) > 0 
                                THEN TRUE
                                ELSE FALSE
                            END
                        ELSE TRUE
                    END
                ) AND
                pa.pfaid IN (SELECT pfaid FROM par3.proadesao WHERE (prgid, pfadatafinal) IN (SELECT prgid, MAX(pfadatafinal) FROM par3.proadesao GROUP BY prgid) ) ";
                            
        //ver($sql);
        return $this->carregar($sql);
    }

    public function pegarPrograma($prgid)
    {
        $hoje = date("Y-m-d");
        
        $sql = "SELECT
                    pro.prgid,
                    pa.pfadatainicial,
                    pa.pfadatafinal,
                    pa.pfaicone,
                    pa.pfaesfera,
                    pro.prgdsc,
                    pro.prgabreviatura,
                    pfastatus,
                    pro.prgarquivo,
                    (CASE WHEN pa.pfadatafinal < now() then 'S' else 'N' end) as bovenceu
                FROM par3.proadesao pa
                INNER JOIN par3.programa pro ON pro.prgid = pa.prgid
                WHERE pro.prgid = $prgid AND pa.pfastatus = 'A'";

        return $this->pegaLinha($sql);
    }

    public function pegarAdesoes()
    {


        $sql = "SELECT
                    pro.prgid,
                    pa.pfadatainicial,
                    pa.pfadatafinal,
                    pa.pfaicone,
                    pa.pfaesfera,
                    pa.pfaano,
                    pa.pfaid,
                    pa.pfastatus,
                    pa.pfasobre,
                    pfastatus,
                    pro.prgdsc
                FROM par3.proadesao pa
                INNER JOIN par3.programa as pro on pro.prgid = pa.prgid ";

        return $this->carregar($sql);
    }
    
    public function pegarDadosAdesaoPrograma($pfaid)
    {
        $sql = "SELECT
                    pro.prgid,
                    pa.pfadatainicial,
                    pa.pfadatafinal,
                    pa.pfaesfera,
                    pa.pfaano,
                    pa.pfaid,
                    pa.pfastatus,
                    pfastatus,
                    pro.prgdsc, 
                    pa.pfavalortotalprograma
                FROM par3.proadesao pa
                INNER JOIN par3.programa as pro on pro.prgid = pa.prgid
                WHERE pa.pfaid = $pfaid";

        return $this->pegaLinha($sql);
    }

    public function montarSQLSimples($arrPost){
	    $sql = "SELECT  
                        pfaid as id, 
                        prgid as codigo, 
                        pfaano, 
                        to_char(pfadatainicial, 'DD/MM/YYYY') as pfadatainicial, 
                        to_char(pfadatafinal, 'DD/MM/YYYY') as pfadatafinal, 
                        pfastatus
                    FROM par3.proadesao
                    WHERE pfastatus = 'A' AND prgid = {$arrPost['prgid']} ";

	    return $sql;
    }

    public function pegaAdesao($arrPost){
        $sql = "SELECT * FROM par3.proadesao WHERE pfaid = {$arrPost['pfaid']}";

        return $this->carregar($sql);
    }
    
    public function pegaArquivoResolucao($arqid){
        $sql = "SELECT arqnome FROM public.arquivo WHERE arqid = {$arqid}";

        return $this->pegaUm($sql);
    }

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['pfaano']           = array();
        $erros['erros']['prgid']            = array();
        $erros['erros']['pfadatainicial']   = array();
        $erros['erros']['pfadatafinal']     = array();

        //Validações
        if(9999 < (int)$campos['pfaano'] ||(int)$campos['pfaano'] < 1){array_push($erros['erros']['pfaano'], 'Informe o ano com 4 inteiros e válida');}
        if(empty($campos['pfaano']) || $campos['pfaano'] == ''){array_push($erros['erros']['pfaano'], 'Campo Obrigatório');}

        if ($campos['prgid'] == '' || $campos['prgid'] == null) {
            array_push($erros['erros']['prgid'], 'Selecione um programa');
        }


        if($campos['pfadatainicial'] == ''){array_push($erros['erros']['pfadatainicial'],'O período de início não pode ser vazio');}
        //if(!verifica_data($campos['pfadatainicial'])){array_push($erros['erros']['pfadatainicial'],'Digite uma data válida');}

        if($campos['pfadatafinal'] == ''){array_push($erros['erros']['pfadatafinal'],'O período de fim não pode ser vazio');}
        if($campos['pfadatainicial'] > $campos['pfadatafinal']){array_push($erros['erros']['prgperiodo'],'O início do período não pode ser maior do que o fim do período');}
        //if(!verifica_data($campos['pfadatainicial'])){array_push($erros['erros']['pfadatainicial'],'Digite uma data válida');}


        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }


    /**
     * @param $dados
     * @throws Exception
     */
    public function updateSobre($dados){
        $sql = "UPDATE par3.proadesao SET pfasobre = '{$dados['pfasobre']}' WHERE prgid = {$dados['prgid']}";
        try{
            $this->executar($sql);
            return $this->commit();
        }catch (Exception $e){
            throw new Exception($e->getMessage());
        }

    }



}