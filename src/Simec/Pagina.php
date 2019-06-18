<?php
namespace Simec;

class Pagina {
    protected $tamanho = 50;
    protected $numero = 0;
    
    public function __construct($numero = 0, $tamanho = 50)
    {
        $this->numero = $numero;
        $this->tamanho = $tamanho;
    }
    
    public function tamanho($tamanho = null){
        if($tamanho !== null){
            $this->tamanho = $tamanho;
        }
        return $this->tamanho;
    }
    
    public function numero($numero = null){
        if($numero !== null){
            $this->numero = $numero;
        }
        return $this->numero;
    }
    
    public function proxima(){
        $this->numero++;
        return $this->numero;
    }
    
    public function anterior(){
        $this->numero--;
        return $this->numero;
    }
}