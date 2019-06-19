<?php

namespace App\AppListeners;


class LogSuccessfulLogin
{
    public function __construct()
    {

    }

    public function handle($event)
    {
        // ao fazer login apaga configuracao cacheada de banco de dados nos ambientes DSV, HMG, TEST
        \Cache::flush();
    }
}

