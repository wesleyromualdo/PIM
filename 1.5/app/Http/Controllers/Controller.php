<?php

namespace App\Http\Controllers;

use Illuminate\Foundation\Bus\DispatchesJobs;
use Illuminate\Routing\Controller as BaseController;
use Illuminate\Foundation\Validation\ValidatesRequests;
use Illuminate\Foundation\Auth\Access\AuthorizesRequests;
use Modules\Seguranca\Entities\Menu;

class Controller extends BaseController
{
    use AuthorizesRequests, DispatchesJobs, ValidatesRequests;

    public function __construct()
    {
        if (!isset($_COOKIE['navbar'])) {
            $_COOKIE['navbar'] = 'false';
        }

        $navBar = $_COOKIE['navbar'] == 'false' ? 'mini-navbar' : null;

        if (isset($_SESSION['sisid'])) {
            $sisid = (int)$_SESSION['sisid'];
            $usucpf = $_SESSION['usucpf'];
            $menus = Menu::getMenuPorSistemaCpfHierarquico($sisid, $usucpf);

            $config__ = \Cache::get("config_{$usucpf}");
            $database = (!empty($config__) ? $config__['database'] : env('DB_PIM_DBNAME') );

            \View::share(['menus'=>$menus, 'navBar'=>$navBar, 'database'=>$database]);
        }
    }
}
