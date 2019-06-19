<?php

namespace Modules\Seguranca\Entities;

use DB;
//use Illuminate\Routing\Router;
//use Illuminate\Http\Request;
//use Illuminate\Http\Response;
use Modules\Seguranca\Entities\Base\TipoResponsabilidadePerfilBase;
use Modules\Seguranca\Entities\UsuarioResponsabilidade;
use Yajra\Datatables\Datatables;

class TipoResponsabilidadePerfil extends TipoResponsabilidadePerfilBase {
    
    public function __construct()
    {
        parent::__construct();
    }
    
    public function scopeAtivo($query) {
        return $query->where('status', 'A');
    }
        
    
//    public function perfilSistemaByID($id) {
//        return $query->where('mnuid', $id);
//    }
    
    
}