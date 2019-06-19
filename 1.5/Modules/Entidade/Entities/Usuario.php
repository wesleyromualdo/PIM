<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\UsuarioBase;

class Usuario extends UsuarioBase {
    
    public function scopeAtivo($query) {
        return $query->where('usustatus', 1);
    }   
    
    public function criptografarSenha($plain_text, $password, $iv_len = 16)
    {
        $plain_text .= "\x13";
        $n = strlen($plain_text);
            if ($n % 16)
                $plain_text .= str_repeat("\0", 16 - ($n % 16));
        $i = 0;
        $enc_text = $this->getRandomIv($iv_len);
        $iv = substr($password ^ $enc_text, 0, 512);
            while ($i < $n) {
                $block = substr($plain_text, $i, 16) ^ pack('H*', md5($iv));
                $enc_text .= $block;
                $iv = substr($block . $iv, 0, 512) ^ $password;
                $i += 16;
            }
        return base64_encode($enc_text);
    }

    public function getRandomIv($iv_len) {
        $iv = '';
        while ($iv_len-- > 0) {
            $iv .= chr(mt_rand() & 0xff);
        }
        return $iv;
    }
    
    public function GetUsuariosSimular($request) {
        
        $usucpf = preg_replace("/\D+/", "", $request->usucpf);
        $usunome = $request->usunome;
        $pflcod = $request->pflcod;
        $where = $usucpf ? " and u.usucpf like '%".$usucpf."%'" : "";
        $where .= $usunome ? " and TRANSLATE(u.usunome, 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ilike TRANSLATE('%{$usunome}%', 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') " : "";
        $where .= $pflcod ?" and p.pflcod = ".$pflcod : "";
        $usuarios = \DB::select("select distinct case when u.usucpf = '{$_SESSION['usucpf']}'/*usucpf*/ 
                                                        then true
                                                        else false	
                                                  end as action,
                                                 u.usucpf as codigo,
                                                 u.usunome || ' - <span style=\"color: red\"><b>Usuário Origem.</b></span>' as descricao
                                            from seguranca.usuario u 
                                            join seguranca.perfilusuario pu on pu.usucpf = u.usucpf
                                            join seguranca.perfil p on p.pflcod = pu.pflcod
                                           where u.usucpf = '{$_SESSION['usucpforigem']}'/*cpforigem*/
                                             and p.sisid = {$_SESSION['sisid']}
                                 union
                                select distinct case when u.usucpf = '{$_SESSION['usucpf']}'/*usucpf*/ 
                                                        then true
                                                        else false	
                                                 end as action,
                                                u.usucpf as codigo,
                                                u.usunome as descricao
                                           from seguranca.usuario u
                                           join seguranca.perfilusuario pu on pu.usucpf = u.usucpf
                                           join seguranca.perfil p on p.pflcod = pu.pflcod and p.sisid = {$_SESSION['sisid']}
                                           join seguranca.usuario_sistema us on us.usucpf = u.usucpf and us.sisid = p.sisid
                                          where u.usucpf <> '{$_SESSION['usucpforigem']}'/*cpforigem*/
                                            and us.suscod = 'A'
                                            and p.pflnivel >= (select min(pflnivel) 
                                                                 from seguranca.perfil
                                                           inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod
                                                                where perfilusuario.usucpf = '{$_SESSION['usucpforigem']}' /*cpforigem*/
                                                                  and perfil.sisid = {$_SESSION['sisid']})
                                       {$where}
                              order by action desc, descricao");
        return $usuarios;
        
    }

    public function VerificaUsuarioSimular($request) {
        
        $usucpf = preg_replace("/\D+/", "", $request->usucpf);
        $usuarios = \DB::select("SELECT codigo, descricao 
                                   FROM (
                                    select distinct case when u.usucpf = '{$_SESSION['usucpf']}'/*usucpf*/ 
                                                        then true
                                                        else false	
                                                  end as action,
                                                 u.usucpf as codigo,
                                                 u.usunome || ' - <span style=\"color: red\"><b>Usuário Origem.</b></span>' as descricao
                                            from seguranca.usuario u 
                                            join seguranca.perfilusuario pu on pu.usucpf = u.usucpf
                                            join seguranca.perfil p on p.pflcod = pu.pflcod
                                           where u.usucpf = '{$_SESSION['usucpforigem']}'/*cpforigem*/
                                             and p.sisid = {$_SESSION['sisid']}
                                 union
                                select distinct case when u.usucpf = '{$_SESSION['usucpf']}'/*usucpf*/ 
                                                        then true
                                                        else false	
                                                 end as action,
                                                u.usucpf as codigo,
                                                u.usunome as descricao
                                           from seguranca.usuario u
                                           join seguranca.perfilusuario pu on pu.usucpf = u.usucpf
                                           join seguranca.perfil p on p.pflcod = pu.pflcod and p.sisid = {$_SESSION['sisid']}
                                           join seguranca.usuario_sistema us on us.usucpf = u.usucpf and us.sisid = p.sisid
                                          where u.usucpf <> '{$_SESSION['usucpforigem']}'/*cpforigem*/
                                            and us.suscod = 'A'
                                            and p.pflnivel >= (select min(pflnivel) 
                                                                 from seguranca.perfil
                                                           inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod
                                                                where perfilusuario.usucpf = '{$_SESSION['usucpforigem']}' /*cpforigem*/
                                                                  and perfil.sisid = {$_SESSION['sisid']})
                              order by action desc, descricao) as foo
                        where foo.codigo = '{$usucpf}'");
        $result['codigo'] = $usuarios[0]->codigo;
        $result['descricao'] = $usuarios[0]->descricao;
        return $result;
        
    }

    public static function getAvisoUsuarios($usucpf)
    {
        if (!empty($usucpf)) {
            //$qb = \DB::connection()->getDoctrineConnection()->createQueryBuilder();
            $result = Usuario::select(['sis.sisabrev', 'avb.avbmsg', 'avb.avburl', \DB::raw('to_char(avb.dataultimaalteracao, \'dd/mm/yyyy às hh:mi\') as datahora'), 'avb.avbstatus'])
                ->from('public.avisousuario AS avb')
                ->join('seguranca.sistema AS sis', 'sis.sisid', '=', 'avb.sisid')
                ->where('usucpf', '=', $usucpf)
//                ->setParameter('usucpf', $usucpf)
                ->orderBy('avb.dataultimaalteracao', 'DESC')
                ->take(30)->get()->toArray();
            return $result;
            //return $qb->execute()->fetchAll(\PDO::FETCH_ASSOC);
        }
        return array();
    }

    public static function getSistemasPorCpf($usucpf)
    {
        //$qb = \DB::connection()->getDoctrineConnection()->createQueryBuilder();
        $UsuarioSistemas = Usuario::select(['s.sisid', \DB::raw('trim(s.sisabrev) as sisabrev, trim(s.sisdsc) as sisdsc')])
            ->from('seguranca.usuario AS u')
            ->join('seguranca.perfilusuario AS pu', 'u.usucpf', '=', 'pu.usucpf')
            ->join('seguranca.perfil AS p', 'pu.pflcod', '=', 'p.pflcod')
            ->join('seguranca.sistema AS s', 'p.sisid', '=', 's.sisid')
            ->join('seguranca.usuario_sistema AS us', function ($query) {
                $query->on('s.sisid', '=', 'us.sisid')
                      ->on('u.usucpf', '=', 'us.usucpf');
            })
            ->where('u.usucpf', '=', $usucpf)
            ->where('us.suscod', '=', 'A')
            ->where('p.pflstatus', '=', 'A')
            ->where('s.sisstatus', '=', 'A')
            ->where('u.suscod', '=', 'A')
//            ->where('u.usucpf', '=', 'us.usucpf')
//            ->setParameter(':usucpf', $usucpf)
            ->groupBy('s.sisid')
            ->groupBy('s.sisabrev')
            ->groupBy('s.sisdsc')
            ->orderBy('s.sisabrev')->get()->toArray();
        //$result[0] = $UsuarioSistemas[0]['attributes'];
        return $UsuarioSistemas;
//        return $qb->execute()->fetchAll(\PDO::FETCH_ASSOC);
    }
}