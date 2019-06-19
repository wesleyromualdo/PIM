<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Illuminate\Routing\Router;
use Modules\Seguranca\Entities\Perfil;
use Modules\Seguranca\Entities\Sistema;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Yajra\Datatables\Datatables;

class PerfilGlobalController extends Controller
{
    private $perfil;
    private $prefixo;

    public function __construct()
    {
        parent::__construct();
        $this->perfil = new Perfil();
        $this->prefixo = $_SESSION['prefixo'];
        $this->sistema = Sistema::where('sisstatus', 'A')->orderBy('sisdsc', 'asc')->pluck('sisdsc', 'sisid');
    }
    /**
     * Display a listing of the resource.
     * @return Response
     */
    public function index()
    {
        return view('seguranca::perfilglobal.index', ['prefixo' => $this->prefixo, 'sistema' => $this->sistema]);
    }

    /**
     * Show the form for creating a new resource.
     * @return Response
     */
    public function create(Request $request)
    { 
        $sisid = $request->sisid;
        return view('seguranca::perfilglobal.criar', ['sisid' => $sisid, 'prefixo' => $this->prefixo]);
    }

    /**
     * Store a newly created resource in storage.
     * @param  Request $request
     * @return Response
     */
    public function store(Request $request)
    {
        $this->validate($request, $this->perfil->rules);
//        $sisid = $request->sisid;
        Perfil::create($request->all() + [/*'sisid' => $sisid,*/ 'pflstatus' => 'A', 'pflresponsabilidade' => 'N']);
        
        return redirect()->route($this->prefixo.'.perfilglobal.index')->with('success', 'Perfil salvo com sucesso');
    }

    /**
     * Show the specified resource.
     * @return Response
     */
    public function show($id)
    {
        $perfil = Perfil::find($id);
        return view('Perfil.show', ['perfil' => $perfil, 'prefixo' => $this->prefixo]);
    }

    /**
     * Show the form for editing the specified resource.
     * @return Response
     */
    public function edit($id)
    {
        $perfil = Perfil::find($id);
        $prefixo = $this->prefixo;
        return view('seguranca::perfilglobal.editar', compact('perfil'), compact('prefixo'));
    }

    /**
     * Update the specified resource in storage.
     * @param  Request $request
     * @return Response
     */
    public function update(Request $request, $id)
    {
        $this->validate($request, $this->perfil->rules);
        Perfil::find($id)->update($request->all());
        
        return redirect()->route($this->prefixo . '.perfilglobal.index')->with('success', 'Perfil atualizado com sucesso');
    }

    /**
     * Remove the specified resource from storage.
     * @return Response
     */
    public function destroy($id)
    {
        Perfil::find($id)->update(['pflstatus' => 'I']);
        
        return redirect()->route($this->prefixo . '.perfilglobal.index')->with('success', 'Perfil excluido com sucesso');
    }
    

    
    /**
     * Retorna os perfis por sisid.
     * @return Response
     */
    public function getList(Request $request)
    {
        if ($request->ajax()) {
            $sisid = $request->sisid;
//            $perfil = Perfil::ativo()->select('pflcod', 'pflnivel', 'pfldsc', 'pflsuperuser', 'pflinddelegar', 'pflsuporte', 'pflsncumulativo');
            $perfil = Perfil::where(['pflstatus' => 'A', 'sisid' => $sisid])->get();
//            dd($perfil);
            $datatables = Datatables::of($perfil)
                ->addColumn('action', function ($perfil) {
                    $url_edit = route($this->prefixo . ".perfilglobal.edit", $perfil->pflcod);
                    $url_destroy = route($this->prefixo . ".perfilglobal.destroy", $perfil->pflcod);
                    return <<<HTML
                        <div class="text-center">
                            <a href="{$url_edit}" class="btn btn-sm btn-warning"><i class="fa fa-pencil"></i></a>
                            <a href="{$url_destroy}" class="btn btn-sm btn-danger act_bt_delete"><i class="fa fa-trash"></i></a>
                        </div>
HTML;
                })
                ->editColumn('pflsuperuser', function (Perfil $perfil) {
                    if($perfil->pflsuperuser == 't'){
                        return 'Sim';
                    }else {
                        return 'Não';
                    }
                })
                ->editColumn('pflinddelegar', function (Perfil $perfil) {
                    if($perfil->pflinddelegar == 't'){
                        return 'Sim';
                    }else {
                        return 'Não';
                    }
                })
                ->editColumn('pflsuporte', function (Perfil $perfil) {
                    if($perfil->pflsuporte == 't'){
                        return 'Sim';
                    }else {
                        return 'Não';
                    }
                })
                ->editColumn('pflsncumulativo', function (Perfil $perfil) {
                    if($perfil->pflsncumulativo == 't'){
                        return 'Sim';
                    }else {
                        return 'Não';
                    }
                });
                
            return $datatables->make(true);
        }
    }
}
