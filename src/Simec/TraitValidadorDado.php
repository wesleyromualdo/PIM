<?php
namespace Simec;

trait TraitValidadorDado
{

    public function validar()
    {
        $reflexao = new \ReflectionClass($this);
        $atributos = $reflexao->getProperties();
        foreach ($atributos as $atributo) {
            $arComentario = explode("\n", $atributo->getDocComment());
            if ($arComentario) {
                foreach ($arComentario as $parte) {
                    if (preg_match('/\*\s+\@(.*)\s+(.*)/', $parte, $match)) {
                        if (isset($match[1])){
                            if($reflexao->hasMethod($match[1])) {
                                $metodo = $reflexao->getMethod($match[1]);
                                $metodo->invoke($this, $atributo->getName(), $atributo->getValue($this), $match[2]);
                            }elseif($match[1] == 'validador'){
                                $metodo = $reflexao->getMethod($match[2]);
                                $metodo->invoke($this, $atributo->getName(), $atributo->getValue($this));
                            }
                        }
                    }
                }
            }
        }
    }

    public function obrigatorio($campo, $valor, $argumento)
    {
        if(!$valor){
            throw new Excecao\Dado(sprintf("O campo %s é obrigatório.", $campo));
        }
    }

    public function tamanho($campo, $valor, $argumento)
    {
        if(strlen($valor) > (int) $argumento){
            throw new Excecao\Dado(sprintf("O campo %s suporta apenas %s caracteres.", $campo, (int) $argumento));
        }
    }
    
}
