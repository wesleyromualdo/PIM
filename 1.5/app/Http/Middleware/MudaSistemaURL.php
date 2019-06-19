<?php

namespace App\Http\Middleware;

use Closure;
use Modules\Seguranca\Entities\Rota;

class MudaSistemaURL
{

    public function handle($request, Closure $next)
    {
        if ($request && !($request->ajax())) {

            $rotaAtual = \Route::current()->uri();
            $module = \Route::current()->getPrefix();
            $prefixo =  str_replace('/', '', $module);

            $arrPublicUrl = $this->publicUrls();

            if (!in_array($rotaAtual,$arrPublicUrl)) {
                $existeSistemaComPrefixoInformado = \Modules\Seguranca\Entities\PerfilUsuario::checaSeUsuarioTemPerfilAssociadoAoPrefixo($prefixo);

                if (!$existeSistemaComPrefixoInformado) {
//                    abort(403, 'Sem permissão de acesso ao módulo.');
                }

                $possuiAcessoURI = Rota::VerificaPermissao($rotaAtual);
                if (!$possuiAcessoURI) {
//                    abort(403, 'Sem permissão de acesso a esta tela.');
                }
            }

        }
        return $next($request);
    }

    public function publicUrls(){
        return ['logout'];
    }
}