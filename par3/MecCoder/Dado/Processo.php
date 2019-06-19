<?php
namespace Simec\Par3\Dado;

/**
 * Representação dos dados de processo
 */
class Processo extends \Simec\AbstractDado
{

    /**
     * Formata uma string com a máscara de processo
     * @return string
     */
    public static function formatar($processo)
    {
        preg_match('/(\d{5})(\d{6})(\d{4})(\d{2})/',preg_replace("[^0-9]", "", $processo),$match);
        if($match){
            return sprintf('%s.%s/%s-%s',$match[1],$match[2],$match[3],$match[4]);
        }
        return '';
    }
}
