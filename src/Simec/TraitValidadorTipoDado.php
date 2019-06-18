<?php
namespace Simec;

trait TraitValidadorTipoDado
{
    public function numerico($campo, $valor)
    {
        if(!is_numeric($valor)){
            throw new Excecao\Dado(sprintf("O campo %s deve ser do tipo numérico.", $campo));
        }
    }
    
    public function uf($campo, $valor)
    {
        if ($valor && !in_array($valor, [
            'AC','AL','AP','AM','BA','CE','DF','ES','GO','MA','MT',
            'MS','MG','PA','PB','PR','PE','PI','RJ','RN','RS','RO',
            'RR','SC','SP','SE','TO',])) {
            throw new \Simec\Excecao\Dado('UF inválida.');
        }
    }
    
    public function cep($campo, $valor){
        
    }
    
    public function cpf($campo, $valor){
        
    }
    
}
