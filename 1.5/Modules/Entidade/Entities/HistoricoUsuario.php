<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\HistoricoUsuarioBase;
use Yajra\Datatables\Datatables;

class HistoricoUsuario extends HistoricoUsuarioBase {
   
//    public function scopeAtivo($query) {
//        return $query->where('susstatus', 'A')->orderBy('sisdsc', 'asc');
//    }
    
    public function getList($request) {

        $collection = Sistema::select('sistema.sisid', 'sistema.sisdsc', 'sistema.sisdiretorio', 'us.suscod')
                ->leftjoin(UsuarioSistema::getTableName().' AS us' , function ($join) use ($request) {
                    $join->on('us.sisid', '=', 'sistema.sisid')
                    ->where('us.usucpf', '=', $request->usucpf);
                    //->where('pu.suscod', '=', 'A');
                 })->where('sisstatus', 'A')->orderBy('sisdsc', 'asc')->get();
                 //dd($collection[52]);
        $datatables = Datatables::of($collection);

        return $datatables->make(true);

    }
    
    public static function getHistorico ($request) {
        $result = HistoricoUsuario::selectRaw('to_char(htudata, \'DD/MM/YYYY\') AS htudata, htudsc, suscod, usucpfadm')->where(['usucpf' => $request->usucpf, 'sisid' => $request->sisid])->get();
        foreach ($result as $r) {
            $historico[] = $r['attributes'];
        }
        return $historico;
    }
    
    public static function salvaHistorico ($request){
        if ($request->usucpf != $_SESSION['usucpf']) {
            $usucpfadm = $_SESSION['usucpf'];
        }
        //dd($request);
        $historicoUsuario = new HistoricoUsuario();
        //$this->validate($request, $this->menu->rules);
        //dd($historicoUsuario);
        $historicoUsuario->fill($request->all() + ['usucpfadm' => $usucpfadm]);
        //dd($historicoUsuario);
        return $historicoUsuario->save();
        //"INSERT INTO seguranca.historicousuario ( htudsc, usucpf, sisid, suscod, usucpfadm ) VALUES ( '%s', '%s', %d, '%1s', %s )",
    }
    
}