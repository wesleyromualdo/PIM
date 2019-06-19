<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\MenuBase;

class Menu extends MenuBase {

    public static function getMenu($sisid, $usucpf)
    {
//        $prefixoSistema = "/{$_SESSION['sisdiretorio']}";
        $menuSession = (isset($_SESSION['menu_1_5_bd'][$_SESSION['usucpf']]) ? $_SESSION['menu_bd'][$_SESSION['usucpf']] : false);

        if (empty($menuSession)) {
//            $qb = \DB::connection()->getDoctrineConnection()->createQueryBuilder();
//            $qb->select('
//          mnu.mnucod,
//          mnu.mnuid,
//          mnu.mnuidpai,
//          mnu.mnudsc,
//          mnu.mnustatus,
//          mnu.mnulink,
//          mnu.mnulink as menu_url,
//          mnu.mnutipo,
//          mnu.mnustile,
//          mnu.mnuhtml,
//          mnu.mnusnsubmenu,
//          mnu.mnutransacao,
//          mnu.mnushow,
//          mnu.abacod,
//          mnu.mnuimagem
//        ')
//                ->from('seguranca.menu mnu, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu')
//                ->where('mnu.mnuid = pmn.mnuid ')
//                ->andWhere("pmn.pflcod = pfl.pflcod")
//                ->andWhere("pfl.pflcod = pfu.pflcod")
//                ->andWhere("pfu.usucpf = :usucpf ")
//                ->andWhere("mnu.sisid = :sisid ")
////                ->andWhere("mnu.mnushow = 't'")
//                ->andWhere("mnu.mnustatus = 'A'")
//                ->andWhere(' mnu.mnutipo=1 or mnu.mnuidpai is not null')
//                ->setParameter(':usucpf', $usucpf)
//                ->setParameter(':sisid', $sisid)
////                ->setParameter(':prefixoSistema', $prefixoSistema)
//                ->orderBy('mnu.mnucod')
//                ->addOrderBy('mnu.mnuid')
//                ->addOrderBy('mnu.mnuidpai')
////                ->addOrderBy('mnu.mnudsc');
//
//            $menuBd_old = $qb->execute()->fetchAll(\PDO::FETCH_ASSOC);
            $menuBd = Menu::select(['mnu.mnucod',
                                    'mnu.mnuid',
                                    'mnu.mnuidpai',
                                    'mnu.mnudsc',
                                    'mnu.mnustatus',
                                    'mnu.mnulink',
                                    'mnu.mnulink as menu_url',
                                    'mnu.mnutipo',
                                    'mnu.mnustile',
                                    'mnu.mnuhtml',
                                    'mnu.mnusnsubmenu',
                                    'mnu.mnutransacao',
                                    'mnu.mnushow',
                                    'mnu.abacod',
                                    'mnu.mnuimagem'
                                  ])
                ->from('seguranca.menu AS mnu')
                ->join('seguranca.perfilmenu AS pmn', 'pmn.mnuid', '=', 'mnu.mnuid')
                ->join('seguranca.perfil AS pfl', 'pfl.pflcod', '=', 'pmn.pflcod')
                ->join('seguranca.perfilusuario AS pfu', 'pfu.pflcod', '=', 'pfl.pflcod')
                ->where('pfu.usucpf', $usucpf)
                ->where('mnu.sisid', $sisid)
//                ->where('mnu.mnushow = 't'')
                ->where('mnu.mnustatus', 'A')
                ->where(function ($q) {
                    $q->whereNotNull('mnu.mnuidpai')->orWhere('mnu.mnutipo', 1);
                        
                })
                ->orderByRaw('mnu.mnucod, mnu.mnuid, mnu.mnuidpai, mnu.mnudsc')->get()->toArray();
            $_SESSION['menu_bd'][$_SESSION['usucpf']] = $menuBd;
            //dd($sisid, $usucpf, $menuBd, $menuBd_old, $qb->getSql());
        } else {
            $menuBd = $menuSession;
        }
        return $menuBd;
    }

    public static function getMenuPorSistemaCpfHierarquico($sisid, $usucpf)
    {

        $menus = array();
        unset($_SESSION['menu']);
        unset($_SESSION['menu_1_5']);

        $menuSession = (isset($_SESSION['menu_1_5'][$_SESSION['usucpf']]) ? $_SESSION['menu_1_5'][$_SESSION['usucpf']] : false);

        if (!empty($usucpf)) {
            if (!empty($menuSession)) {
                $menus = $menuSession;
            } else {
                $menusBd = self::getMenu($sisid, $usucpf);

                if (is_array($menusBd)) {
                    foreach ($menusBd as $key => $menuBd) {
                        if (empty($menuBd['mnuidpai'])) {
                            $menus[$menuBd['mnuid']] = $menuBd;
                            unset($menusBd[$key]);
                            $menus[$menuBd['mnuid']]['itensMenu'] = self::tratarMenus($menusBd, $menuBd);
                        }
                    }
                }
                // dd($menus);
                $_SESSION['menu_1_5'][$_SESSION['usucpf']] = $menus;
            }
        }

//        end($menus);
//        $key = key($menus);
//        if (!is_array($menus[$key]))
//            unset($menus[$key]);

        return $menus;
    }

    public static function tratarMenus($menusBd, $menuPaiBd)
    {
        $menus = array();
        foreach ($menusBd as $key => $menuBd) {
            if (!empty($menuBd['mnuidpai']) && $menuPaiBd['mnuid'] == $menuBd['mnuidpai']) {
                $menus[$menuBd['mnuid']] = $menuBd;
                unset($menusBd[$key]);
                if($menus[$menuBd['mnuid']]['mnutipo'] !== 1 && $menus[$menuBd['mnuid']]['mnusnsubmenu'] == false){
                    unset($menus[$menuBd['mnuid']]['itensMenu']);
                } else {
                    $menus[$menuBd['mnuid']]['itensMenu'] = self::tratarMenus($menusBd, $menuBd, $key);
                }
            }
        }
        return $menus;
    }
}