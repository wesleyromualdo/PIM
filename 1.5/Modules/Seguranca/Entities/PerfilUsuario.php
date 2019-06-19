<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\PerfilUsuarioBase;
use Modules\Seguranca\Entities\Menu;
use Modules\Seguranca\Entities\UsuarioResponsabilidade;
use \Cache;

class PerfilUsuario extends PerfilUsuarioBase {
      
    public static $mnulink;
    
    public function __construct()
    {
        parent::__construct();
        //$this->mnulink = [];
    }
    public static function eSuperUsuario() {
        if (isset($_SESSION['usucpf']) && isset($_SESSION['sisid'])) {
            return false;
        }

        $perfil = Perfil::select('usucpf')
                ->perfil()
                ->where(['pflsuperuser' => 't', 'sisid' => $_SESSION['sisid'], 'usucpf' => $_SESSION['usucpf']])
                ->get();

        $_SESSION['testa_superuser'] = count($perfil) > 0;

        return $_SESSION['testa_superuser'];
    }

    public static function pegaPerfilGeral($usucpf = null, $sisid = null, $ignorarSessao = false) { {
            $usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
            $sisid = !$sisid ? $_SESSION['sisid'] : $sisid;

            if (isset($_SESSION['perfil'][$usucpf][$sisid]) && !$ignorarSessao) {

                $arrPerfil = $_SESSION['perfil'][$usucpf][$sisid];
            } else {

                $resultado = PerfilUsuario::select(['p.pflcod', 'pfldsc'])
                                ->leftjoin(Perfil::getTableName() . ' as p ', PerfilUsuario::getTableName() . '.pflcod', '=', 'p.pflcod')
                                ->where(PerfilUsuario::getTableName() . '.usucpf', '=', $_SESSION['usucpf'])
                                ->where('p.sisid', '=', $_SESSION['sisid'])
                                ->where('p.pflstatus', '=', 'A')->get();
                if (!$resultado)
                    return false;

                foreach ($resultado as $perfil) {
                    #ADVOGADO


                    $arrPerfil[] = $perfil->attributes['pflcod'];
                }
            }
            if (isset($arrPerfil)) {
                $_SESSION['perfil'][$usucpf][$sisid] = $arrPerfil;
            }
        }

        return isset($arrPerfil) ? $arrPerfil : false;
    }

   
    public function pegaPerfis($usucpf, $sisid = null, $aPflcod = []) {
        
        $query = PerfilUsuario::query();        
        if (!$usucpf)
        {
            return false;
        }

        $aPfl = $query->select(['p.pflcod']);
        $query->leftjoin(Perfil::getTableName() . ' as p ', $this->getTableName() . '.pflcod', '=', 'p.pflcod');
        $query->where($this->getTableName() . '.usucpf', '=', $usucpf);
        
        if ($sisid)
        {
            $query->where('p.sisid', '=', $sisid);
        }
        
        if (count($aPfl) > 0)
        {
            $query->whereIn('p.pflcod', $aPflcod);
        }
        
        $query->where('p.pflstatus', '=', 'A')->first();
        
        return $aPfl;
    }

    
    public static function atribuiDadosSistemaNaSessao($dadosSistema)
    {   
        if(is_array($dadosSistema)){
            foreach ($dadosSistema as $dadoSistema => $valor)
            {
                $_SESSION[$dadoSistema] = $valor;
            }
        }
    }
    
    public static function atribuiDadosMenuNaSessao($sisid,$usucpf)
    {
        $menusSistema = Menu::getMenuPorSistemaCpfHierarquico($sisid, $usucpf);
        $_SESSION['menu_1_5'][$usucpf] = $menusSistema;
        
    }
    
    public static function atribuiDadosPerfilNaSessao($sisid,$usucpf)
    {
        $perfis = PerfilUsuario::pegaPerfilPorUsuario($usucpf, $sisid);
        $arrayDePerfis = [];
        
        foreach ($perfis as $perfil)
        {
            $arrayDePerfis[$usucpf][$sisid][] = $perfil['pflcod'];            
        }
        
        $_SESSION['perfil'] = $arrayDePerfis;
    }
    

    public static function atribuiUsuarioResponsabilidadeNaSessao($sisid, $usucpf)
    {
        
        $usrResp = new UsuarioResponsabilidade;
        $responsabilidades = $usrResp->pegaResponsabilidadesUsuarioBySistema($sisid, $usucpf);
        $_SESSION['usuarioresponsabilidade'] = !empty($responsabilidades) ? $responsabilidades : '';
    }
    
    public static function checaSeUsuarioTemPerfilAssociadoAoPrefixo($prefix)
    {
        $usucpf = $_SESSION['usucpf'];                      
        
        if (!$prefix && !$usucpf)
        {
            return false;
        }
        
        //Adiciona o prefixo do sistema atual à Session.
        $_SESSION['prefixo'] = $prefix;
        
        //Caso o prefixo seja o mesmo encontrado no cache, reatribui os dados de sessão com dados existentes neste.
        if(Cache::get('dadosModuloSistemaCache')){
            
            $dadosSistemaCache = Cache::get('dadosModuloSistemaCache');

            if($dadosSistemaCache['sisdiretorio'] == $prefix){

                static::atribuiDadosSistemaNaSessao($dadosSistemaCache);
                static::atribuiDadosMenuNaSessao($dadosSistemaCache['sisid'],$usucpf);   
                static::atribuiDadosPerfilNaSessao($dadosSistemaCache['sisid'], $usucpf);
                static::atribuiUsuarioResponsabilidadeNaSessao($dadosSistemaCache['sisid'], $usucpf);
                
                return true;
            }
            
        }
        
        
        //$qb = \DB::connection()->getDoctrineConnection()->createQueryBuilder();
        $dadosSistema = PerfilUsuario::select(['s.sisid', 's.sislayoutbootstrap', 's.sisdiretorio', 's.sisarquivo', 's.sisdsc', 's.sisurl', 's.sisabrev', 's.sisexercicio', 's.paginainicial', 'p.pflnivel as usunivel', 'us.susdataultacesso', 'us.suscod', 's.sissnalertaajuda'])
                ->from('seguranca.usuario AS u')
                ->join('seguranca.perfilusuario AS pu', 'u.usucpf', '=', 'pu.usucpf')
                ->join('seguranca.perfil AS p', 'pu.pflcod', '=', 'p.pflcod')
                ->join('seguranca.sistema AS s', 'p.sisid', '=', 's.sisid')
                ->join('seguranca.usuario_sistema AS us', 's.sisid', '=', 'us.sisid')
                ->where('u.usucpf', '=', $usucpf)
                ->where('s.sisdiretorio', '=', $prefix)
                ->where('s.sisid', '=', $_SESSION['sisid'])
                ->where('p.pflstatus', '=',  'A')
                ->where('s.sisstatus', '=', 'A')
//                ->setParameter(':usucpf', $usucpf)
//                ->setParameter(':sisdiretorio', $prefix)
//                ->setParameter(':sisid', $_SESSION['sisid'])
                ->take(1)->get();

//        $dadosSistema = $qb->execute()->fetchAll(\PDO::FETCH_ASSOC);
        //dd($dadosSistema);
        //$dadosSistema[0] = (array) $dadosSistema[0];
//        dd($dadosSistema[0]['attributes']);
        //Retorna FALSE caso não se encontre nenhuma correspondência do prefixo informado na tabela sistema.
        if (empty($dadosSistema))
        {
            return false;
        }
        
        $sisidRecuperado = $dadosSistema[0]['attributes']['sisid'];
       
        Cache::put('dadosModuloSistemaCache', $dadosSistema[0]['attributes'], 30);
        
        static::atribuiDadosSistemaNaSessao($dadosSistema[0]['attributes']);
        static::atribuiDadosMenuNaSessao($sisidRecuperado,$usucpf);
        static::atribuiDadosPerfilNaSessao($sisidRecuperado, $usucpf);
        static::atribuiUsuarioResponsabilidadeNaSessao($sisidRecuperado, $usucpf);

        return true;
    }
    
    public static function in_array_recursivo($needle, $haystack) {
        foreach ($haystack as $key => $items) {
            !empty($items['mnulink']) ? self::$mnulink[] = $items['mnulink'] : '';
            if ($items['mnulink'] == $needle) {
                return true;                
            } elseif (!empty($items['itensMenu'])) {
                self::in_array_recursivo($needle, $items['itensMenu']);
            }
        }
    }
    
    public static function in_array_filho($needle, $haystack) {
        foreach ($haystack as $key => $items) {                       
//            !empty($items['mnulink']) ? $this->mnulink[] = $items['mnulink'] : '';
           if ($items['mnulink'] == $needle) {
                return true;    
            } elseif (!empty($items['itensMenu'])) {
                self::in_array_recursivo($needle, $items['itensMenu'], 'filho ');
            }
        }
        //return false;
        
    }
    
    public static function getListaPerfisUsuario($usucpf = null, $sisid = null)
    {
        $cpf = (!$usucpf) ? $_SESSION['usucpf'] : $usucpf;
        $sisid = (!$sisid) ? $_SESSION['sisid'] : $sisid;
        $lista = PerfilUsuario::select(['p.pflcod', 'pfldsc'])
            ->leftjoin(Perfil::getTableName() . ' as p ', PerfilUsuario::getTableName() . '.pflcod', '=', 'p.pflcod')
            ->where(PerfilUsuario::getTableName() . '.usucpf', '=', $cpf)
            ->where('p.sisid', '=', $sisid)
            ->where('p.pflstatus', '=', 'A')->pluck('pfldsc', 'p.pflcod');
        
        return $lista;
    }


    public static function verficarPermissaoEmularUsuario($usucpf, $sisid)
    {
//        $qb = \DB::connection()->getDoctrineConnection()->createQueryBuilder();
//        dd('aqui');
        $perfilusuario = PerfilUsuario::select(['pu.usucpf'])
            ->from('seguranca.perfilusuario AS pu')
            ->join('seguranca.perfil AS p', 'p.pflcod', '=', 'pu.pflcod')
            ->where('pu.usucpf', '=', $usucpf)
            ->where('p.sisid', '=', $sisid)
            ->where('p.pflsuporte', '=', 't')->get();
//            ->setParameter('usucpf', $usucpf)
//            ->setParameter('sisid', $sisid);
//        $perfilusuario = $qb->execute()->fetch(\PDO::FETCH_ASSOC);
        //dd($perfilusuario[0]['attributes']['usucpf']);
        if (isset($perfilusuario[0]['attributes']['usucpf'])) {
            return true;
        }
        return false;
    }

    public static function pegaPerfilPorUsuario($usucpf, $sisid)
    {
        if (!empty($usucpf) && !empty($sisid)) {
            //$qb = \DB::connection()->getDoctrineConnection()->createQueryBuilder();            
            $perfilUsuario = PerfilUsuario::select(['pu.pflcod', 'p.pfldsc'])
                ->from('seguranca.perfilusuario AS pu')
                ->join('seguranca.perfil AS p', 'p.pflcod', '=', 'pu.pflcod')
                ->where('pu.usucpf', '=', $usucpf)
                ->where('p.sisid', '=', $sisid)
                ->where('pflstatus', '=', 'A' )->get();
//                ->setParameter('usucpf', $usucpf)
//                ->setParameter('sisid', $sisid);
//            dd($result[0]['attributes']);
            $result[0] = $perfilUsuario[0]['attributes'];
            return $result;
            //dd($qb->execute()->fetchAll(\PDO::FETCH_ASSOC));
        }
        return array();
    }

}
