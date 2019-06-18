<?php
class AlertaCondicao extends Modelo
{
    public $arObras = array();

    public function __construct(){
    }

    public function executaFuncao($funcao) {
        if($funcao) {
            return $this->{$funcao}();
        }
    }

    public function condicaoTeste()
    {
        $saida = '';
        foreach($this->arObras as $key => &$obra) {
            $sql = <<<DML
                SELECT tobid FROM obras2.obras where obrid = $obra->obrid
DML;
            $tobid = $this->pegaUm($sql);
            if($tobid == 1) {
                $obra->apresentar = false;
                $obra->mensagem = 'Não ok';
                $saida = 'Nem todas as obras estão ok.';
            }
        }
        return $saida;
    }
}