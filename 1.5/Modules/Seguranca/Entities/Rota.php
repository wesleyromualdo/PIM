<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\RotaBase;

class Rota extends RotaBase
{
    // -- Classe destinada para as regras de negócio da model


    public function __construct(array $attributes = array()) {
        parent::__construct($attributes);
        $this->setLabel();
        $this->setRules();
    }

    public function scopeHabilitado($query){
        return $query->where('st_habilitado', true);
    }

    public function setLabel() {
       // --ex $this->labels['nome_campo'] = 'Descricao da label';
    }

    public function setRules() {
      // --ex  $this->rules['nome_campo'] = 'requerid|max';
    }

    public function perfis()
    {
        return $this->hasMany(RotaPerfil::class,'co_rota','co_rota');//->where('st_habilitado',true);
    }

    /**
     * retorna todas as rotas permitidas para o perfil do usuario logado
     * @return array
     */
    public static function getRotasPermitidas(){
        $usucpf = $_SESSION['usucpf'];
        $sisid = $_SESSION['sisid'];
        $perfis = $_SESSION['perfil'][$usucpf][$sisid];

        $arrTmp = array();
        if(!empty($perfis)){
            $rotas = Rota::select('ds_rota')
                ->join('seguranca.tb_rota_perfil','seguranca.tb_rota_perfil.co_rota','=','seguranca.tb_rota.co_rota')
                ->where('sisid',$sisid)
                ->where('seguranca.tb_rota_perfil.st_habilitado',true)
                ->whereIn('seguranca.tb_rota_perfil.pflcod',$perfis)->get();



            if(!empty($rotas)){
                foreach ($rotas as $rota)
                    $arrTmp[] = $rota->ds_rota;
            }
        }
        return $arrTmp;
    }

    /**
     * verifica se o usuario possui acesso a rota com o seu perfil associado
     * @param $rotaAtual
     * @return bool
     */
    public static function VerificaPermissao($rotaAtual) {
        $rotasPermitidas = self::getRotasPermitidas();
        if (in_array($rotaAtual,$rotasPermitidas)) {
            return true;
        }
        return false;
    }

    /**
     * recupera todas as rotas do sistema na aplicaçao.
     * @param $sisid
     * @return array
     */
    public static function getRotasSistema($sisid){
        $sisdiretorio = Sistema::select('sisdiretorio')->where('sisid',$sisid)->first()->sisdiretorio;
        $allRotas = \Route::getRoutes();
        $rotasSistema = array();
        array_push($rotasSistema,$sisdiretorio); /*adiciona o root*/
//        array_push($rotasSistema,'demandashst/teste2/deveinserir2');/*apagar*/
//        array_push($rotasSistema,'demandashst/teste3/deveinserir3');/*apagar*/
//        array_push($rotasSistema,'demandashst/teste4/deveinserir4');/*apagar*/
//        array_push($rotasSistema,'demandashst/teste5/deveinserir5');/*apagar*/
        $sisdiretorio .= "/";
        foreach ($allRotas as $rota){
            if(substr($rota->getPath(),0,strlen($sisdiretorio)) == $sisdiretorio){
                array_push($rotasSistema,$rota->getPath());
            }
        }
        return $rotasSistema;
    }

    /**
     * @param $sisid
     * @return mixed
     */
    public static function getRotasSistemaDB($sisid){
         return Rota::select('co_rota','ds_rota','ds_rota_descricao')
            ->where([
                ['sisid','=',$sisid]
            ])->orderBy('ds_rota')->get();
    }

    /**
     * recupera a rota pela descricao e pelo sisid
     * @param $ds_rota
     * @param $sisid
     * @return mixed
     */
    public static function getRotaDescricao($ds_rota,$sisid){
        return Rota::select('co_rota','st_habilitado')
            ->where([
                ['ds_rota','=',$ds_rota],
                ['sisid','=',$sisid]
            ])->first();
    }


}
