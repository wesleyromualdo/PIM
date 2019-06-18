<?php
class Par3_Model_IniciativaDesdobramentos extends Modelo
{
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "par3.iniciativa_desdobramento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('desid');

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'tipid' => array('tabela' => 'par3.iniciativa_desdobramento_tipo', 'pk' => 'tipid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'desid'            => null,
        'tipid'            => null,
        'cidid'            => null,
        'desdsc'           => null,
        'desstatus'        => null,
        'dessituacao'      => null,
        'desdtinclusao'    => null,
        'descpfinclusao'   => null,
        'desdtinativacao'  => null,
        'descpfinativacao' => null,
        'desimutavel'      => null,
    );

    //Cabeçaçho: 'Código','Ciclo','Início','Fim','Duração Ciclo','Anos','Vigência','Situação'
    public function montarSQLSimples($arrPost)
    {
        $desdsc = sanitizar_string_pesquisa($arrPost['desdsc']);
        $where  = $arrPost['desdsc']?" AND upper(removeacento(desdsc)) LIKE upper(removeacento('%{$desdsc}%'))":'';
        $where .= $arrPost['dessituacao']?" AND dessituacao = '{$arrPost['dessituacao']}'":'';
        $where .= $arrPost['desid'] && validarInteiro($arrPost['desid'])?" AND desid = {$arrPost['desid']}":'';
        $where .= $arrPost['tipid'] && validarInteiro($arrPost['tipid'])?" AND des.tipid = {$arrPost['tipid']}":'';
        $where .= $arrPost['dessituacao']?" AND dessituacao = '{$arrPost['dessituacao']}'":'';

        $orderBy = $arrPost['ordenacao']? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                desid as id,
                ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                des.desid as codigo,
                tip.tipdsc,
                des.desdsc as descricao,
                des.dessituacao,
                des.desimutavel as imutavel
                FROM {$this->stNomeTabela} as des
                INNER JOIN par3.iniciativa_desdobramento_tipo as tip ON tip.tipid = des.tipid
                WHERE desstatus = 'A'
                {$where}
                {$orderBy}
                ";
        return $sql;
    }

    public function verificarExisteDesdobramento(array $arrPost)
    {
        $where = $arrPost['desid']?" AND desid != {$arrPost['desid']}" : ' ';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE upper(desdsc) = '".str_to_upper($arrPost['desdsc'])."' AND desstatus = 'A' {$where}";
        return $this->pegaLinha($sql,0,0);
    }

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['desdsc'] = array();
        $erros['erros']['tipid']  = array();

        //Validações
        if($campos['tipid'] == ''  || $campos['tipid']  == null){array_push($erros['erros']['tipid'],'O campo não pode ser vazio');}
        if($campos['desdsc'] == '' || $campos['desdsc'] == null){array_push($erros['erros']['desdsc'],'O campo não pode ser vazio');}
        if($this->verificarExisteDesdobramento($campos)){array_push($erros['erros']['desdsc'], 'Já existe Desdobramento cadastrado com essa descrição');}
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    /**
     * Lista de Desdobramentos.
     *
     * @return array|mixed|NULL
     */



    public function pegarSelectCombo(){
        $sql = "SELECT desid as codigo,desdsc as descricao FROM {$this->stNomeTabela}";

        return $sql;
    }





    public function listaDesdobramentos()
    {
        return $this->carregar("SELECT 	des.desid as codigo,
                                        des.desdsc as descricao
                                FROM {$this->stNomeTabela} as des
                                INNER JOIN par3.iniciativa_desdobramento_tipo as tip
                                    ON tip.tipid = des.tipid
                                WHERE desstatus = 'A' AND
                                dessituacao = 'A'");
    }

    public function getIniciativaDesdobramentoById($desid){
        $sql = "SELECT * FROM par3.iniciativa_desdobramento as iniciativa_desdobramento WHERE iniciativa_desdobramento.desid = {$desid}";
        $result = $this->pegaLinha($sql);
        return $result;
    }

    public function verificaLigacaoPlanejamentoById($desid){
        $sql = "SELECT * FROM par3.iniciativa_planejamento_desdobramentos iniplandesd WHERE iniplandesd.desid = {$desid}";
        $result = $this->carregar($sql);
        return $result;
    }
}