<?php

class Par3_Model_IniciativaPlanoInterno extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_plano_interno';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'inplintid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inplinprog' => array('tabela' => 'par3.programa', 'pk' => 'prgid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'inplintid'       => null,
        'inplinprog'      => null,
        'inplintptres'    => null,
    	'inplintdsc'      => null,
        'inplintano'      => null,
        'inplintstatus'   => null,
        'inplintsituacao' => null,
        'inplintdtcriacao'=> null,
        'inplintcpfcriacao'=> null,

    );


    public function montarSQLSimples($arrPost,$pdf = false)
    {
        $whereSituacao = $arrPost['inplintsituacao']?" WHERE inplintsituacao = '{$arrPost['inplintsituacao']}'": "WHERE inplintsituacao IN ('A', 'I')";

        $planoIntern = trim($arrPost['planoIntern']);
        $where .= $arrPost['planoIntern']?" AND upper(removeacento(plin.inplintdsc)) LIKE upper(removeacento('%{$planoIntern}%'))":'';
        $where .= $arrPost['ptres']?" AND CAST(plin.inplintptres AS TEXT) LIKE '%{$arrPost['ptres']}%'":'';
        $where .= $arrPost['prgPlano']?" AND CAST(plin.inplinprog AS TEXT) LIKE '%{$arrPost['prgPlano']}%'":'';
        $where .= $arrPost['ano']?" AND CAST(plin.inplintano AS TEXT) LIKE '%{$arrPost['ano']}%' ":'';
        $where .= $arrPost['inplintid'] && validarInteiro($arrPost['inplintid']) ?" AND plin.inplintid = {$arrPost['inplintid']}":'';
        $where .= " AND plin.inplintstatus = 'A' ";

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                plin.inplintid,
                plin.inplintid as codigo,
                pr.prgdsc,
                plin.inplintptres,
                plin.inplintdsc, 
                plin.inplintano";
        if($pdf){
            $sql .= ",plin.inplintsituacao as situacao";
        }else{
            $sql .= ",plin.inplintid as situacao";
        }        
                
                
        $sql .= " FROM {$this->stNomeTabela} plin
                INNER JOIN par3.programa pr
                ON pr.prgid = plin.inplinprog
                $whereSituacao
                {$where}
                order by plin.inplintid desc
                ";
             //ver($sql);
        return $sql;
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    /**
     * Métodopara montar select com os id's existentes na tabela.
     *
     * @return bool
     */

    public function populaHistorico($id,$acao){
        $modelHistorico = new Par3_Model_IniciativaPlanoInternoHistorico();
        $modelPlano = new Par3_Model_IniciativaPlanoInterno($id);//recupera programa
        $modelHistorico->gravarHistorico($modelPlano,$acao);//grava em hitórico
    }

    public function pegarSQLSelectCombo($inplintid = null)
    {
        $where = $inplintid ? "AND inplintid = $inplintid" : '';
        return "SELECT  inplintid as codigo,inplintdsc as descricao FROM {$this->stNomeTabela} WHERE inplintsituacao = 'A'";
    }

    public function pegarPTRESSelectCombo($inplintdsc = null)
    {
        $where = $inplintdsc ? "AND inplintdsc = $inplintdsc" : '';
        return "SELECT  inplintptres as codigo,inplintptres as descricao FROM {$this->stNomeTabela} WHERE inplintsituacao = 'A'";
    }

    public function pegarAnoSelectCombo($inplintptres = null)
    {
        $where = $inplintptres ? "AND inplintptres = $inplintptres" : '';
        return "SELECT  inplintano as codigo,inplintano as descricao FROM {$this->stNomeTabela} WHERE inplintsituacao = 'A'";
    }

    public function alterarSituacao($inplintid){
        $sqlConsulta = "SELECT inplintsituacao FROM {$this->stNomeTabela} WHERE inplintid = {$inplintid}";
        $consulta = $this->carregar($sqlConsulta);
        if($consulta[0]['inplintsituacao'] == 'I'){
            $situacao = 'A';
        }else{
            $situacao = 'I';
        }
        $sqlUpdate   = "UPDATE {$this->stNomeTabela} SET inplintsituacao = '{$situacao}' WHERE inplintid = {$inplintid}";
        $this->executar($sqlUpdate);
        $this->commit();

        $this->populaHistorico($inplintid,Par3_Model_IniciativaPlanoInternoHistorico::UPDATE);
        return true;
    }

    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['prgPlano']      = array();
        $erros['erros']['ptres']         = array();
        $erros['erros']['planoIntern']  = array();
        $erros['erros']['ano']           = array();

      
        if($campos['inplinprog'] == ''){array_push($erros['erros']['prgPlano'],'Deve ser informado um Programa');}
        if($campos['inplintptres'] == ''){array_push($erros['erros']['ptres'],'Deve ser informado o PTRES');}
        if(strlen($campos['inplintptres']) > 11){
            array_push($erros['erros']['ptres'],'O PTRES deve conter no máximo 11 digitos');
        } 
        if($campos['inplintptres'] != '' && !is_numeric($campos['inplintptres'])){array_push($erros['erros']['ptres'],'O PTRES deve ser número');}
        if($campos['inplintdsc'] == ''){array_push($erros['erros']['planoIntern'],'Deve ser informado um Plano Interno');}
        if($campos['inplintano'] == ''){array_push($erros['erros']['ano'],'Deve ser informado um Ano');}
           
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function removerPlano($arrPost){

        //VERIFICA SE TEM PLANO VINCULADO COM FONTE
        $sqlFonte = "SELECT * FROM par3.iniciativa_fonte_recurso WHERE inplintid = {$arrPost['inplintid']} and ifrstatus = 'A'";
        $cont = $this->pegaum($sqlFonte);

        if($cont != ''){
            $retorno = [
                "status" => 0,
                "mensagem" => "O Plano Interno já está vinculado a uma Fonte de Recurso ou a Análise, não poderá ser excluído!."
            ];
            return $retorno;
        }else{
            $retorno = [
                "status" => 1
                ];
        }
        
        //se estiver tudo ok... realiza o update logico.
        $sql = "UPDATE {$this->stNomeTabela} SET inplintsituacao = 'I', inplintstatus = 'I' WHERE inplintid = {$arrPost['inplintid']}";
        $result = $this->executar($sql);
        $this->commit();

        $this->populaHistorico($arrPost['inplintid'],Par3_Model_IniciativaPlanoInternoHistorico::DELETE);
        return $retorno;
    }



}
