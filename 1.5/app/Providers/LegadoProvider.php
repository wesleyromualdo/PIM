<?php

namespace App\Providers;

use Illuminate\Support\ServiceProvider;
use Illuminate\View\FileViewFinder;

class LegadoProvider extends ServiceProvider
{
    /**
     * Bootstrap the application services.
     *
     * @return void
     */
    public function boot()
    {
        // Integra a sessao do legado com o Laravel
        $aSessao = \App\Session::integrar();
        view()->composer('layouts/app', function ($view) use ($aSessao) {
            $view->with('user', (object) $aSessao);
        });
    }

    /**
     * Register the application services.
     *
     * @return void
     */
    public function register()
    {
    }
}
