<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\RotaPerfilBase;

class RotaPerfil extends RotaPerfilBase
{
    // -- Classe destinada para as regras de negócio da model

    public function __construct(array $attributes = array()) {
        parent::__construct($attributes);
        $this->setLabel();
        $this->setRules();
    }

    public function setLabel() {
       // --ex $this->labels['nome_campo'] = 'Descricao da label';
    }

    public function setRules() {
      // --ex  $this->rules['nome_campo'] = 'requerid|max';
    }

    /**
     *
     * @param $sisid
     * @param $pflcod
     * @return mixed
     */
    public function recuperaRotasSistemaPorPerfil($sisid,$pflcod){
        return Rota::select('tbr.pflcod','tbr.co_rota',Rota::getTableName().'.co_rota', 'ds_rota','ds_rota_descricao','sisid','tbr.pflcod as rotaperfil')
            ->leftJoin(\DB::raw('seguranca.tb_rota_perfil as tbr'), function($join) use($pflcod){
                $join->on('tbr.co_rota','=','seguranca.tb_rota.co_rota')
                    ->where('tbr.pflcod','=',$pflcod);

            })->where([
                ['sisid','=',$sisid]
            ])->orderBy('ds_rota')->get();
    }

//    /**
//     * @param $sisid
//     * @return mixed
//     */
//    public function recuperaRotaPerfilSistema($sisid){
//        return  Rota::select('tbr.co_rota', 'tbr.st_habilitado', 'sisid')
//            ->join(\DB::raw('seguranca.tb_rota_perfil as tbr'),'tbr.co_rota','=','seguranca.tb_rota.co_rota')
//            ->where('sisid',$sisid)->get();
//    }
}                