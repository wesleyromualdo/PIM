<?php
class PosAcaoAlerta{

    public $aid;

    public function __construct(){
    }

    public function executaFuncao($funcao) {
        if($funcao) {
            return $this->{$funcao}();
        }
    }
}