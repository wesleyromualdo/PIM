<?php
namespace Simec;

class Console
{

    public $arquivo;
    public $comando;
    public $argumentos;

    public function __construct()
    {
        global $argv;
        foreach ($argv as $idx => $arg) {
            if ($idx > 1) {
                $ar = explode('=',$arg);
                $i = array_shift($ar);
                $this->argumentos[$i] = array_shift($ar);
            } else {
                $this->arquivo = $argv[0];
                $this->comando = isset($argv[1]) ? $argv[1] : false;
            }
        }
    }
    
    public function rodar(){
        $reflection = new \ReflectionClass($this);
        if($this->comando && $reflection->hasMethod($this->comando)){
            $this->{$this->comando}();
        }
    }
    
    protected function executar(){
        $classe = $this->argumentos['classe'];
        $metodo = $this->argumentos['metodo'];
        $obj = new $classe();
        return call_user_func_array([$obj,$metodo], $this->argumentos);
    }
    
    protected function gerarModulo(){
        Corporativo\Gerador\Modulo::gerarModulo($this->argumentos['nome']);
    }
    
}
