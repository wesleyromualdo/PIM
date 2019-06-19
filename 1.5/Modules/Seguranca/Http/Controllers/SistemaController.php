<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Illuminate\Routing\Router;
use Modules\Seguranca\Entities\Perfil;
use Modules\Seguranca\Entities\Sistema;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Modules\Seguranca\Http\Requests\MenuRequest;
use Modules\Seguranca\Http\Requests\SistemaRequest;
use Yajra\Datatables\Datatables;

class SistemaController extends Controller
{
    private $perfil;
    private $prefixo;

    public function __construct()
    {
        parent::__construct();
        $this->sistema = new Sistema();
        $this->prefixo = $_SESSION['prefixo'];
    }
    /**
     * Display a listing of the resource.
     * @return Response
     */
    public function index()
    {
        return view('seguranca::sistema.index', ['prefixo' => $this->prefixo]);
    }

    /**
     * Show the form for creating a new resource.
     * @return Response
     */
    public function create()
    { 
        $sisid = $_SESSION['sisid'];
        return view('seguranca::sistema.criar', ['prefixo' => $this->prefixo]);
    }

    /**
     * Store a newly created resource in storage.
     * @param  Request $request
     * @return Response
     */
    public function store(SistemaRequest $request)
    {

        $this->sistema->fill($request->all() + ['sisstatus'=>'A']);
        $this->sistema->sanitize();
        $this->sistema->setNull();
        $this->sistema->save();
//
        return redirect()->route($this->prefixo.'.sistema.index')->with('success', 'Sistema salvo com sucesso');
    }

    /**
     * Show the specified resource.
     * @return Response
     */
    public function show($id)
    {
//        $perfil = Perfil::find($id);
//        return view('Perfil.show', ['perfil' => $perfil, 'prefixo' => $this->prefixo]);
    }

    /**
     * Show the form for editing the specified resource.
     * @return Response
     */
    public function edit($id)
    {
        $sistema = Sistema::find($id);
        $prefixo = $this->prefixo;
        return view('seguranca::sistema.editar', compact('sistema'), compact('prefixo'));
    }

    /**
     * Update the specified resource in storage.
     * @param  Request $request
     * @return Response
     */
    public function update(SistemaRequest $request, $id)
    {
        $this->sistema = Sistema::where('sisid',$id)->first()->fill($request->all());
        $this->sistema->sanitize();
        $this->sistema->update();
        return redirect()->route($this->prefixo . '.sistema.index')->with('success', 'Sistema atualizado com sucesso');
    }

    /**
     * Remove the specified resource from storage.
     * @return Response
     */
    public function destroy($id)
    {
//        Sistema::find($id)->update(['sisstatus' => 'I']);
//
//        return redirect()->route($this->prefixo . '.sistema.index')->with('success', 'Sistema excluido com sucesso');
    }
    

    
    /**
     * Retorna os perfis por sisid.
     * @return Response
     */
    public function getList(Request $request)
    {
//        if ($request->ajax()) {

            $sistema = Sistema::where([])->get();
//            dd($sistema);
            $datatables = Datatables::of($sistema)
                ->addColumn('action', function ($sistema) {
                    $url_edit = route($this->prefixo . ".sistema.edit", $sistema->sisid);
//                    $url_destroy = route($this->prefixo . ".sistema.destroy", $sistema->sisid);
                    return <<<HTML
                        <div class="text-center">
                            <a href="{$url_edit}" class="btn btn-sm btn-warning"><i class="fa fa-pencil"></i></a>
                            <!--<a href="{}" class="btn btn-sm btn-danger act_bt_delete"><i class="fa fa-trash"></i></a>-->
                        </div>
HTML;
                });
                
            return $datatables->make(true);
        }
//    }
}
