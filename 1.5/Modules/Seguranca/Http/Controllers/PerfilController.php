<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Illuminate\Routing\Router;
use Modules\Seguranca\Entities\Perfil;
//use Modules\Seguranca\Entities\Sistema;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Modules\Seguranca\Http\Requests\MenuRequest;
use Modules\Seguranca\Http\Requests\PerfilRequest;
use Yajra\Datatables\Datatables;

class PerfilController extends Controller
{
    private $perfil;
    private $prefixo;

    public function __construct()
    {
        parent::__construct();
        $this->perfil = new Perfil();
        $this->prefixo = $_SESSION['prefixo'];
    }
    /**
     * Display a listing of the resource.
     * @return Response
     */
    public function index()
    {
        return view('seguranca::perfil.index', ['prefixo' => $this->prefixo]);
    }

    /**
     * Show the form for creating a new resource.
     * @return Response
     */
    public function create()
    { 
        $sisid = $_SESSION['sisid'];
        return view('seguranca::perfil.criar', ['sisid' => $sisid, 'prefixo' => $this->prefixo]);
    }

    /**
     * Store a newly created resource in storage.
     * @param  Request $request
     * @return Response
     */
    public function store(PerfilRequest $request)
    {
        $sisid = $_SESSION['sisid'];
        $this->perfil->fill($request->all() + ['sisid' => $sisid, 'pflstatus' => 'A', 'pflresponsabilidade' => 'N']);
        $this->perfil->save();

        return redirect()->route($this->prefixo.'.perfil.index')->with('success', 'Perfil salvo com sucesso');
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
        return view('seguranca::perfil.editar', compact('perfil'), compact('prefixo'));
    }

    /**
     * Update the specified resource in storage.
     * @param  Request $request
     * @return Response
     */
    public function update(PerfilRequest $request, $id)
    {
        Perfil::find($id)->update($request->all());
        return redirect()->route('perfil.index')->with('success', 'Perfil atualizado com sucesso');
    }

    /**
     * Remove the specified resource from storage.
     * @return Response
     */
    public function destroy($id)
    {
        Perfil::find($id)->update(['pflstatus' => 'I']);
        
        return redirect()->route('perfil.index')->with('success', 'Perfil excluido com sucesso');
    }
    

    
    /**
     * Retorna os perfis por sisid.
     * @return Response
     */
    public function getList(Request $request)
    {
        if ($request->ajax()) {
            $sisid = $_SESSION['sisid'];
//            $perfil = Perfil::ativo()->select('pflcod', 'pflnivel', 'pfldsc', 'pflsuperuser', 'pflinddelegar', 'pflsuporte', 'pflsncumulativo');
            $perfil = Perfil::where(['pflstatus' => 'A', 'sisid' => $sisid])->get();
//            dd($perfil);
            $datatables = Datatables::of($perfil)
                ->addColumn('action', function ($perfil) {
                    $url_edit = route("perfil.edit", $perfil->pflcod);
                    $url_destroy = route("perfil.destroy", $perfil->pflcod);
                    return <<<HTML
                        <div class="text-center">
                            <a href="{$url_edit}" class="btn btn-sm btn-warning"><i class="fa fa-pencil"></i></a>
                            <a href="{$url_destroy}" class="btn btn-sm btn-danger act_bt_delete"><i class="fa fa-trash"></i></a>
                        </div>
HTML;
                });
                
            return $datatables->make(true);
        }
    }
}
