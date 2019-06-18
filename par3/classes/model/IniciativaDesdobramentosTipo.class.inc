<?php
class Par3_Model_IniciativaDesdobramentosTipo extends Modelo
{
    /*
  * Nome da tabela especificada
  * @var string
  * @access protected
  */
    protected $stNomeTabela = "par3.iniciativa_desdobramento_tipo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('tipid');

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'tipid'            => null,
        'tipdsc'           => null,
        'tipstatus'        => null,
        'tipsituacao'      => null,
        'tipdtinclusao'    => null,
        'tipdtinativacao'  => null,
        'tipcpfinclusao'   => null,
        'tipcpfinativacao' => null
    );

    public function pegarSQLSelectCombo($tipid = null)
    {
        $where = $tipid?"WHERE tipid = $tipid":'';
        return "SELECT  tipid as codigo,tipdsc as descricao FROM {$this->stNomeTabela} $where";
    }

    public function verificarExisteDesdobramentoTipo(array $arrPost)
    {
        $where = $arrPost['tipid']?" AND tipid != {$arrPost['tipid']}":'';
        $sql = "SELECT * FROM {$this->stNomeTabela} WHERE upper(tipdsc) = '".str_to_upper($arrPost['tipdsc'])."' AND tipstatus = 'A' {$where}";
        return $this->pegaLinha($sql,0,0);
    }

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['tipdsc'] = array();

        //Validações
        if($campos['tipdsc'] == '' || $campos['tipdsc'] == null){array_push($erros['erros']['tipdsc'],'O campo não pode ser vazio');}
        if($this->verificarExisteDesdobramentoTipo($campos)){array_push($erros['erros']['tipdsc'], 'Já existe Tipo de Desdobramento cadastrado com essa descrição');}
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }
}