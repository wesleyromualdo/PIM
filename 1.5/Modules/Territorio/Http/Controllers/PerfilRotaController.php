<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Illuminate\Routing\Router;
use Modules\Seguranca\Entities\Perfil;
use Modules\Seguranca\Entities\PerfilMenu;
//use Modules\Seguranca\Entities\Sistema;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Modules\Seguranca\Entities\Rota;
use Modules\Seguranca\Entities\RotaPerfil;
use Modules\Seguranca\Entities\Sistema;
use Yajra\Datatables\Datatables;

class PerfilRotaController extends Controller
{
    private $perfilRota;
    private $prefixo;

    public function __construct()
    {
        parent::__construct();
        $this->perfilRota = new RotaPerfil();
        $this->prefixo = $_SESSION['prefixo'];
    }
    /**
     * Display a listing of the resource.
     * @return Response
     */
    public function index()
    {
        $sistemas = Sistema::recuperaSistemasCombo();
        return view('seguranca::perfilrota.index', ['prefixo' => $this->prefixo],compact('sistemas'));
    }

    /**
     * Show the form for creating a new resource.
     * @return Response
     */
    public function create()
    { 
        $sisid = $_SESSION['sisid'];
        return view('seguranca::perfilrota.criar', ['sisid' => $sisid, 'prefixo' => $this->prefixo]);
    }

    /**
     * Store a newly created resource in storage.
     * @param  Request $request
     * @return Response
     */
    public function store(Request $request)
    {
        $sisid = $_SESSION['sisid'];
        $this->perfilRota->fill($request->all() + ['sisid' => $sisid, 'pflstatus' => 'A', 'pflresponsabilidade' => 'N']);
        $this->perfilRota->save();

        return redirect()->route($this->prefixo.'.perfilrota.index')->with('success', 'Perfil salvo com sucesso');
    }

    /**
     * Show the specified resource.
     * @return Response
     */
    public function show($id)
    {
        $perfil = RotaPerfil::find($id);
        return view('perfilrota.show', ['perfil' => $perfil, 'prefixo' => $this->prefixo]);
    }

    /**
     * Show the form for editing the specified resource.
     * @return Response
     */
    public function edit($id)
    {
        $perfilRota = RotaPerfil::find($id);
        $prefixo = $this->prefixo;
        return view('seguranca::perfilrota.editar', compact('perfilRota'), compact('prefixo'));
    }

    /**
     * Update the specified resource in storage.
     * @param  Request $request
     * @return Response
     */
    public function update(PerfilRequest $request, $id)
    {
        RotaPerfil::find($id)->update($request->all());
        return redirect()->route($this->prefixo . '.perfilrota.index')->with('success', 'Perfil atualizado com sucesso');
    }

    /**
     * Remove the specified resource from storage.
     * @return Response
     */
    public function destroy($id)
    {
        RotaPerfil::find($id)->update(['pflstatus' => 'I']);
        
        return redirect()->route($this->prefixo . '.perfilrota.index')->with('success', 'Perfil excluido com sucesso');
    }
    

    
    /**
     * retorna as rotas pelo sisid
     * @return Response
     */
    public function getList(Request $request)
    {
        if ($request->ajax()) {
            $sisid = $request['sisid'];
            $pflcod = $request->pflcod;
            $rotas = $this->perfilRota->recuperaRotasSistemaPorPerfil($sisid,$pflcod);

            $datatables = Datatables::of(collect($rotas))
                ->addColumn('action', function ($rota)  use ($pflcod){
                    $checked = $rota->rotaperfil == $pflcod ? 'checked="checked"' : '';
                    return <<<HTML
                        <div class="text-center">
                            <input type="checkbox"  class="chkPerfilRota" $checked id="co_rota" co_rota="{$rota->co_rota}" name="co_rota" value="{$rota->co_rota}"/>
                        </div>
HTML;
                });
            return $datatables->make(true);
        }
    }

    /**
     * retorna os perfis por sistemas
     * @param Request $request
     * @return mixed
     */
    public function getPerfil(Request $request){
        if($request->ajax()){
            $sisid = $request['sisid'];
            $perfis = Perfil::where([
                ['sisid','=',$sisid],
                ['pflstatus','=','A']
            ])->orderBy('pfldsc')->pluck('pfldsc','pflcod');
            return $perfis;
        }
    }

    public function associarRota(Request $request){

        if($request->ajax()){
            $pflcod = $request->pflcod;
            $co_rota  = $request->co_rota;
            $sisid = $request->sisid;
            $checkall = $request->checkall;
            if($checkall && $co_rota == null){
                $coRotas = $this->recuperaRotasSistemas($sisid);
                $this->deletaRotaPerfil($pflcod,$coRotas);
                if(!empty($coRotas)) {
                    foreach ($coRotas as $rota) {
                        $perfilRota = new RotaPerfil;
                        $perfilRota->co_rota = $rota->co_rota;
                        $perfilRota->pflcod = $pflcod;
                        $perfilRota->save();
                    }
                }
            }elseif(!$checkall && $co_rota == null){
                $coRotas = $this->recuperaRotasSistemas($sisid);
                $this->deletaRotaPerfil($pflcod,$coRotas);

            }else{
                $perfilRota = RotaPerfil::where(['pflcod' => $pflcod,'co_rota' => $co_rota])->get();
                if(!$perfilRota->isEmpty()){
                    RotaPerfil::where(['pflcod' => $pflcod,'co_rota' => $co_rota])->delete();
                }else{
                    $perfilRota = new RotaPerfil;
                    $perfilRota->fill($request->all());
                    $perfilRota->save();
                }
            }


        }
    }
    private function recuperaRotasSistemas($sisid){
        return Rota::select('co_rota')->where(['sisid' => $sisid])->get();
    }
    private function deletaRotaPerfil($pflcod,$coRotas){
        RotaPerfil::where('pflcod',$pflcod)->whereIn('co_rota',$coRotas)->delete();
    }

    /**
     * @param $status
     * @param $coRota
     */
    public function alteraStatusPerfilRota($status,$coRota){
         $bool = $status == '1' ? true : false;
         RotaPerfil::where('co_rota',$coRota)->update(array('st_habilitado'=>$bool));


    }


}
