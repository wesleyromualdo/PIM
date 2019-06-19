<?php

namespace App\Http\Middleware;

use Closure;
use Modules\Seguranca\Entities\SessionUsuario;
use Illuminate\Support\Facades\Auth;
use App\Session;

class LegadoCheckSession
{
    /**
     * Handle an incoming request.
     *
     * @param  \Illuminate\Http\Request  $request
     * @param  \Closure  $next
     * @return mixed
     */
    public function handle($request, Closure $next)
    {
//        if(\Auth::check()){
//            return $next($request);
//        }

        if (!SessionUsuario::getUsuCpf()) {
            if(!isset($_SERVER['REQUEST_SCHEME'])){
                $_SERVER['REQUEST_SCHEME'] = 'http';
            }
            header('Location:' . $_SERVER['REQUEST_SCHEME'] . '://' . $_SERVER['HTTP_HOST']);
            die;
        }

//        Auth::loginUsingId(SessionUsuario::getUsuCpf());
        Session::integrar();
        return $next($request);
    }
}
