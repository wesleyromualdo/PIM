<?php

class Par3_Model_CicloPar extends Modelo
{
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "par3.ciclo_par";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('cicid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'cicid'            => null,
        'cidid'            => null,
        'cicdsc'           => null,
        'cicdtinicio'      => null,
        'cicdtfim'         => null,
        'cicstatus'        => null,
        'cicsituacao'      => null,
        'cicvigencia'      => null,
        'cicdtinclusao'    => null,
        'ciccpfinclusao'   => null,
        'cicdtinativacao'  => null,
        'ciccpfinativacao' => null,
        'cicanos'          => null,
        'cicduracao'       => null,

    );

    /**
     * Tipos de Vigência cicvigencia
     */
    const VIGENTE      = 'VIGENTE';
    const FECHADO      = 'FECHADO';
    const NAO_INICIADO = 'NAO_INICIADO';

    //Cabeçaçho: 'Código','Ciclo','Início','Fim','Duração Ciclo','Anos','Vigência','Situação'
    public function montarSQLSimples($arrPost)
    {
        $cicdsc = sanitizar_string_pesquisa($arrPost['cicdsc']);
        $where  = $arrPost['cicdsc']?" AND upper(removeacento(cicdsc)) LIKE upper(removeacento('%{$cicdsc}%'))":'';
        $where .= $arrPost['cicsituacao']?" AND cicsituacao = '{$arrPost['cicsituacao']}'":'';
        $where .= $arrPost['cicid'] && validarInteiro($arrPost['cicid'])?" AND cicid = {$arrPost['cicid']}":'';
        $where .= $arrPost['cicvigencia']?" AND cicvigencia = '{$arrPost['cicvigencia']}'":'';
        $where .= $arrPost['ano'] && validarInteiro($arrPost['ano'])? "AND {$arrPost['ano']} BETWEEN cicdtinicio AND cicdtfim":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                cicid as id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                cicid as codigo,
                cicdsc as descricao,
                cicdtinicio,
                cicdtfim,
                cicduracao,
                cicanos,
                cicvigencia,
                cicsituacao
                FROM {$this->stNomeTabela}
                WHERE cicstatus = 'A'
                {$where}
                {$orderBy}
                ";
        return $sql;
    }

    public function verificarExisteCiclo(array $arrPost)
    {
        $where = $arrPost['cicid']?" AND cicid != {$arrPost['cicid']}":'';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE upper(cicdsc) = '".str_to_upper($arrPost['cicdsc'])."' AND cicstatus = 'A' {$where}";
        return $this->pegaLinha($sql,0,0);
    }

    public function verificarExisteDataFim($arrPost)
    {
        $where .= $arrPost['cicid']?" AND cicid != {$arrPost['cicid']}":'';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE cicdtfim = {$arrPost['cicdtfim']}  AND cicstatus = 'A' {$where}";
        return $this->pegaLinha($sql,0,0);
    }

    public function verificarExisteDataInicio($arrPost)
    {
        $where .= $arrPost['cicid']?" AND cicid != {$arrPost['cicid']}":'';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE cicdtinicio = {$arrPost['cicdtinicio']}  AND cicstatus = 'A' {$where}";
        return $this->pegaLinha($sql,0,0);
    }

    public function pegarSQLSelectCombo($cicid = array())
    {
        $cicid = is_array($cicid) ? implode(',', $cicid) : $cicid;
        $where  = "WHERE cicstatus = 'A' AND cicsituacao = 'A'";
        $where .= $cicid ? " AND cicid in({$cicid})" : '';
        return "SELECT  cicid as codigo, cicdsc as descricao FROM {$this->stNomeTabela} $where";
    }

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['cicdsc']      = array();
        $erros['erros']['cicdtfim']    = array();
        $erros['erros']['cicdtinicio'] = array();

        //Validações
        if($campos['cicdsc'] == '' || $campos['cicdsc'] == null){array_push($erros['erros']['cicdsc'],'O campo não pode ser vazio');}
        if($campos['cicdsc'] > 255){array_push($erros['erros']['cicdsc'],'A descricao informada possui mais de 255 caracteres.');}
        if($this->verificarExisteCiclo($campos)){array_push($erros['erros']['cicdsc'], 'Já existe Ciclo cadastrado com essa descrição');}


        /*if($campos['cicdtfim']){
            if ($this->verificarExisteDataFim($campos)) {array_push($erros['erros']['cicdtfim'], 'Já existe Ciclo cadastrado com o Ano Fim informado');}
        }
        if($campos['cicdtinicio']) {
            if ($this->verificarExisteDataInicio($campos)) {
                array_push($erros['erros']['cicdtinicio'], 'Já existe Ciclo cadastrado com o Ano de Início informado');
            }
        }*/
        if(9999 < (int)$campos['cicdtinicio'] ||(int)$campos['cicdtinicio'] < 1){array_push($erros['erros']['cicdtinicio'], 'Informe uma data de Início com 4 inteiros e válida');}
        if(9999 < (int)$campos['cicdtfim']||(int) $campos['cicdtfim'] < 1){array_push($erros['erros']['cicdtfim'], 'Informe uma data Fim com 4 números inteiros e válida');}
        if(empty($campos['cicdtfim'])){array_push($erros['erros']['cicdtfim'], 'O campo não pode ser vazio');}
        if(empty($campos['cicdtinicio'])){array_push($erros['erros']['cicdtinicio'], 'O campo não pode ser vazio');}

        if((int)$campos['cicdtinicio'] > (int)$campos['cicdtfim']){array_push($erros['erros']['cicdtfim'], 'O Ano Início não pode ser maior que o Ano Fim');}

        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    public function verificaLigacaoPlanejamento($cicid){
        $sql = "SELECT * FROM 
                  par3.iniciativa_planejamento as iniciativa_planejamento
                  INNER JOIN par3.iniciativa as iniciativa ON iniciativa.iniid = iniciativa_planejamento.iniid
                  INNER JOIN par3.ciclo_par as ciclo_par ON ciclo_par.cicid = iniciativa.cicid
		          WHERE ciclo_par.cicid = {$cicid}";
        $result = $this->carregar($sql);
        return $result;
    }

    public function getCicloPorIniciativa(){  
        $sql = "SELECT DISTINCT cp.cicid as codigo, cp.cicdsc as descricao FROM par3.iniciativa_planejamento ip
                INNER JOIN par3.iniciativa i USING (iniid)
                INNER JOIN par3.ciclo_par cp USING (cicid)
                WHERE ip.inpstatus = 'A'";
        return $sql;
    }
}