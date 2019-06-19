<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\ErroBase;

class Erro extends ErroBase
{
    public static function tipoErro($errtipo){
        switch ($errtipo) {
            case $errtipo == 'DB':
                return 'Erro de Banco de dados (sintaxe, etc)';
                break;
            case $errtipo == 'PR':
                return 'Erro de Programaзгo (expected array, etc)';
                break;
            case $errtipo == 'QB':
                return 'Queda no banco';
                break;
            case $errtipo == 'WS':
                return 'WebService';
                break;
            case $errtipo == 'EN':
                return 'Encoding no banco';
                break;
            case $errtipo == 'PD':
                return 'Erro na Conexгo (possivelmente pdeinterativo)';
                break;
            case $errtipo == 'DC':
                return 'Diretуrio Cheio';
                break;
            case $errtipo == 'AI':
                return 'Arquivo inexistente';
                break;
            case $errtipo == 'DV':
                return 'Diversos';
                break;
            default:
                return 'Vazio';
        }
    }
}