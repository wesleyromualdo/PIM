<?php

namespace App\Http\Middleware;

use Closure;
use App\Entities\Model;
use Modules\Auditoria\Entities\Auditoria;


class QueryInterceptor
{

    public function handle($request, Closure $next)
    {  
        //Starta a modelo para impedir a persistência de dados quando simulando outro usuário
        $model = new Model();

        \DB::listen(function ($query)
        {
            Auditoria::salvarAuditoria($query->sql);
        
        });

        return $next($request);
    }
}