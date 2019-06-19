<?php

namespace Modules\Seguranca\Entities;

use DB;
use Modules\Seguranca\Entities\Base\TipoResponsabilidadeBase;

class TipoResponsabilidade extends TipoResponsabilidadeBase {
    
    public function __construct()
    {
        parent::__construct();
    }
    
    public function scopeAtivo($query) {
        return $query->where('status', 'A');
    }
    
    public function getTipoResponsabilidadeByPerfil($objContainer){
        if(!$objContainer)
        {
            return false;
        }
        $havePerfil = false;
        $sisid = !empty($objContainer->sisid) ? $objContainer->sisid : $_SESSION['sisid'];
        $tbTipoRespPerfil = with(new TipoResponsabilidadePerfil())->getTable();
        foreach ($objContainer->perfis as $pflcod => $perfil) {
            
           $tiposResponsabilidades[$pflcod] = \DB::table($this->getTable().' AS tr')->select(['tpr.trpcod', 'tpr.tprcod', 'tpr.pflcod', 'tr.tprdsc', 'tr.tpr_join_schema', 'tr.tpr_join_table', 'tr.tpr_join_column', 'tr.tpr_join_table_column_dsc', 'tr.tpr_join_table_column_status'])
                ->leftJoin($tbTipoRespPerfil.' AS tpr', function ($join) use ($pflcod) {
                    $join->on('tpr.tprcod', '=', 'tr.tprcod')
                    ->where('tpr.pflcod', '=', $pflcod) 
                    ->where('tpr.status', '=', 'A');

                })
                ->where(['tr.status' => 'A', 'tr.sisid' => $sisid] )
                ->orderBy('tr.tprdsc', 'ASC')->get();
            if (count($tiposResponsabilidades[$pflcod]) > 0) {
                $havePerfil = true;
                $usrResponsabilidade = new UsuarioResponsabilidade();
                $objContainer->UsuarioResponsabilidades[$pflcod] = $usrResponsabilidade->getResponsabilidadeUsuarioByPerfil($pflcod, $objContainer->usucpf, $tiposResponsabilidades[$pflcod]);
            }
        }
        if ($havePerfil) {
            return $tiposResponsabilidades;
        } else {
            return false;
        }
    }
    
    public static function getTipoResponsabilidadeBySistema ($sisid) {
        $tbTipoResp = TipoResponsabilidade::getTableName();
        $tbTipoRespPerfil = with(new TipoResponsabilidadePerfil())->getTable();

        $tiposResponsabilidades = DB::table($tbTipoResp.' as tr')
            ->select('trp.trpcod', 'trp.tprcod', 'trp.pflcod', 'tr.tprdsc', 'tr.tpr_join_schema', 'tr.tpr_join_table', 'tr.tpr_join_column', 'tr.tpr_join_table_column_dsc', 'tr.tpr_join_table_column_status')
            ->join($tbTipoRespPerfil.' as trp', 'trp.tprcod', '=', 'tr.tprcod')
            ->where([['tr.status', '=', 'A'], ['tr.sisid','=', $sisid] ])
            ->orderBy('tr.tprdsc', 'ASC')
            ->get();

        return $tiposResponsabilidades;
    }
}