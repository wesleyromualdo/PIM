<?php

namespace App\Providers;

use Illuminate\Support\ServiceProvider;
use Illuminate\Support\Facades\Blade;

class AppServiceProvider extends ServiceProvider
{
    /**
     * Register any application services.
     *
     * @return void
     */
    public function register()
    {
        require_once __DIR__ . '/../Http/Helpers/Navigation.php';
    }

    public function boot()
    {
        defined('APPRAIZ') || define('APPRAIZ', realpath(dirname(__FILE__) . '/../../../../') . '/');

        $this->setCookieNavbar();
        $this->defineErrors();
        $this->setDirectives();
        $this->createDiretoryTmp();
    }

    public function createDiretoryTmp()
    {
        if (!file_exists('/tmp/view')) {
            mkdir('/tmp/view', 0777, true);
        }
        if (!file_exists('/tmp/cache')) {
            mkdir('/tmp/cache', 0777, true);
        }
        if (!file_exists('/tmp/session')) {
            mkdir('/tmp/session', 0777, true);
        }
    }

    private function setDirectives(){
        Blade::directive('money', function ($money) {
            return "<?php echo number_format($money, 2, ',', '.'); ?>";
        });

        Blade::directive('datetime', function ($expression) {
            return "<?php echo ($expression)->format('d/m/Y H:i'); ?>";
        });
    }

    private function setCookieNavbar()
    {
        if (empty($_COOKIE['navbar'])) {
            $_COOKIE['navbar'] = 'false';
        }
        $_COOKIE['navbar'] = ($_COOKIE['navbar'] === 'true'  ? null : 'mini-navbar' );
    }

    private function defineErrors()
    {
        if ($this->app->environment() !== 'production') {
            error_reporting(E_ALL ^ E_NOTICE ^ E_DEPRECATED ^ E_STRICT);
            ini_set('display_errors', 1);
            ini_set('display_startup_errors', 1);
        }
    }
}
