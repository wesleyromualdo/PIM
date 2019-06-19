<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\UsuarioResponsabilidadeBase;
use Modules\Seguranca\Entities\TipoResponsabilidade;
use Modules\Seguranca\Entities\TipoResponsabilidadePerfil;
use Yajra\Datatables\Datatables;

class UsuarioResponsabilidade extends UsuarioResponsabilidadeBase {
    
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
    
    public function getResponsabilidadeUsuarioByPerfil($pflcod, $usucpf, $tiposResponsabilidades){
      
        if(!$pflcod || !$usucpf)
        {
            return false;
        }        
    
        foreach ($tiposResponsabilidades as $key => $responsabilidade) {
            if (!is_null($responsabilidade->tprcod)) {
                $responsabilidades[$responsabilidade->tprcod] = \DB::table($this->getTable().' AS ur')->select(['alias.'.$responsabilidade->tpr_join_column.' AS codigo',
                             'alias.'.$responsabilidade->tpr_join_table_column_dsc.' AS descricao'])
                     ->join($responsabilidade->tpr_join_schema.'.'.$responsabilidade->tpr_join_table.' AS alias', 'alias.'.$responsabilidade->tpr_join_column, '=', 'ur.responsabilidadecod')
                     ->where(['ur.usucpf' => $usucpf, 'ur.status' => 'A', 'ur.trpcod' => $responsabilidade->trpcod])->get();
            } else {
                $responsabilidades[0] = "";
            }
        }       
        return $responsabilidades;
    }
    
    public function getList($request) {
        if ($request->ajax()) {
            
            $trpcod = $request->trpcod;
            $tprcod = $request->tprcod;
            $pflcod = $request->pflcod;
            $tprjoincolumn = $request->tprjoincolumn;
            $tprjoindesc = $request->tprjoindesc;
            $tprjointable = $request->tprjointable;
            $tprjointablecolumnstatus = $request->tprjointablecolumnstatus;
            $esquema = $request->tprjoinschema;
            $cpf = $request->usucpf;
            $status = ($tprjointablecolumnstatus) ? true : false;
    
            $responsabilidadesUsuario =  \DB::table($esquema.'.'.$tprjointable.' AS alias')
                 ->select(['ur.trpcod', 'ur.urcod', 'alias.'.$tprjoincolumn.' AS codigo', 'alias.'.$tprjoindesc.' AS descricao'])
                 ->leftJoin($this->getTable().' AS ur', function($join) use($tprjoincolumn, $cpf, $trpcod) {
                     $join->on('alias.'.$tprjoincolumn, '=', 'ur.responsabilidadecod')
                     ->where('ur.usucpf', '=', $cpf)
                     ->where('ur.trpcod', '=', $trpcod)
                     ->where('ur.status', '=', 'A');
                 })
                 ->when($status, function($q){
                     return $q->where(['alias.'.$tprjointablecolumnstatus => 'A']);
                 })
                 ->orderBy('ur.urcod', 'ASC')->get();
            
            
            foreach ($responsabilidadesUsuario as $key => $value) {
                $objResponsabilidadesUsuario = new \stdClass();
                $objResponsabilidadesUsuario->urcod = $value->urcod;
                $objResponsabilidadesUsuario->trpcod = $trpcod;
                $objResponsabilidadesUsuario->tprcod = $tprcod;
                $objResponsabilidadesUsuario->pflcod = $pflcod;
                $objResponsabilidadesUsuario->codigo = $value->codigo;
                $objResponsabilidadesUsuario->descricao = $value->descricao;
                $arrResponsabilidadesUsuario[] = $objResponsabilidadesUsuario; 
            }
            $collection = collect($arrResponsabilidadesUsuario);
            
            $datatables = Datatables::of($collection)->addColumn('action', function ($collection) {
                
                $checked = $collection->urcod == "" ? "" : "checked";
                
                return '<input class="chkResponsabilidade" id="chkResponsabilidade" btnAtt="btnAbrirResp'.$collection->pflcod.'_'.$collection->tprcod.'" type="checkbox" trpcod="'.$collection->trpcod.'" value="'.$collection->codigo.'" '.$checked.'/>';
            });

            return $datatables->make(true);

        }
    }
    
    public function pegaResponsabilidadesUsuarioBySistema($sisid, $usucpf){
      
        if(!$sisid || !$usucpf)
        {
            return false;
        }        
        $tpResponsabilidade = TipoResponsabilidade::getTipoResponsabilidadeBySistema($sisid);
        if (!empty($tpResponsabilidade)) {
            foreach ($tpResponsabilidade as $key => $responsabilidade) {
                if ($responsabilidade->trpcod) {
                    $responsabilidades[$responsabilidade->pflcod][$responsabilidade->tpr_join_table] = /* \DB::table($this->getTable().' AS ur')*/
                         $this->select(["alias.*"])
                         ->from($this->getTable().' AS ur')
                         ->join($responsabilidade->tpr_join_schema.'.'.$responsabilidade->tpr_join_table.' AS alias', 'alias.'.$responsabilidade->tpr_join_column, '=', 'ur.responsabilidadecod')
                         ->where(['ur.usucpf' => $usucpf, 'ur.status' => 'A', 'ur.trpcod' => $responsabilidade->trpcod])->get()->toArray();
    //                dd($qb->getSql());
                }
            }
        }
        
        return !empty($responsabilidades) ? $responsabilidades : false;
    }
    
    
}