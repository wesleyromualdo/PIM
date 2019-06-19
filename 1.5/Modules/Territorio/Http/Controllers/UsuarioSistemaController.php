<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Illuminate\Routing\Router;
use Modules\Seguranca\Entities\Menu;
use Modules\Seguranca\Entities\Sistema;
use Illuminate\Http\Request;
use Modules\Seguranca\Http\Requests\UsuarioSistemaRequest;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Yajra\Datatables\Datatables;
use Modules\Seguranca\Entities\Perfil;
use Modules\Seguranca\Entities\PerfilUsuario;
use Modules\Seguranca\Entities\UsuarioSistema;
use Modules\Seguranca\Entities\TipoResponsabilidade;
use Modules\Seguranca\Entities\HistoricoUsuario;




class UsuarioSistemaController extends Controller
{
    private $usuarioSistema;
    private $prefixo;
    
    public function __construct()
    {
        parent::__construct();
        $this->usuarioSistema = new UsuarioSistema();
//        $this->sisid = $_SESSION['sisid'];
//        $this->usucpf = $_SESSION['usucpf'];
        $this->prefixo = $_SESSION['prefixo'];
        $this->sistemas = Sistema::where('sisstatus', 'A')->orderBy('sisdsc', 'asc')->get();
    }

    public function index(Request $request)
    {
        $objContainer = new \stdClass();
//        $objContainer->perfis = PerfilUsuario::getListaPerfisUsuario($request->usucpf);
        $objContainer->usucpf = $request->usucpf;
        $objContainer->prefixo = $this->prefixo;
//        $tipoResponsabilidade = new TipoResponsabilidade();
//        $objContainer->tiposResponsabilidades = $tipoResponsabilidade->getTipoResponsabilidadeByPerfil($objContainer);
        return view('seguranca::usuariosistema.index',['objContainer'=>$objContainer, 'sistemas' => $this->sistemas] );

    }    
    
    public function edit(Request $request){
        $objContainer = new \stdClass();
        $objContainer->usucpf = $request->usucpf;
        $objContainer->prefixo = $this->prefixo;
        return view('seguranca::usuariosistema.index', ['objContainer'=>$objContainer, 'sistemas' => $this->sistemas])->render();
    }
    
    public function getList(Request $request) {
        $Sistema = new UsuarioSistema();
        return $Sistema->getList($request);
    }

    public function pegaPerfisSistemabyUsuario(Request $request) {
        $Sistema = new UsuarioSistema();
        return $Sistema->pegaPerfisSistemabyUsuario($request);
        //return Perfil::where(['sisid' => $request->sisid,])->pluck('pfldsc', 'pflcod');
    }            
    
    public function associarResponsabilidade(Request $request)
    {
        if ($request->ajax())
        {   
            $cpf = $request->usucpf;
            $trpcod = $request->trpcod;
            $responsabilidadecod  = $request->responsabilidadecod;
              
            $responsabilidade = UsuarioResponsabilidade::where(['status' => 'A', 'usucpf' => $cpf, 'trpcod' => $trpcod,'responsabilidadecod' => $responsabilidadecod])->get();
              
              if (!$responsabilidade->isEmpty()) {
                  $this->destroy($responsabilidade[0]->urcod);
                  //PerfilMenu::where(['usucpf' => $cpf, 'trpcod' => $trpcod,'responsabilidadecod' => $responsabilidadecod])->delete();                   
                  
              } else {
                  $usuarioResponsabilidade = new UsuarioResponsabilidade;
                  $usuarioResponsabilidade->trpcod = $trpcod;
                  $usuarioResponsabilidade->responsabilidadecod = $responsabilidadecod;
                  $usuarioResponsabilidade->usucpf = $cpf;
                  $usuarioResponsabilidade->status = 'A';
                  $usuarioResponsabilidade->save();
              }
        }
    }

//    public function create()
//    {
//        $sisid = $_SESSION['sisid'];
//        return view('seguranca::menu.criar', ['sisid' => $sisid]);
//    }
//
//
    public function store(UsuarioSistemaRequest $request)
    {
        $this->validate($request, $this->usuarioSistema->rules);
//        \DB::beginTransaction();

        $usuarioSistema = UsuarioSistema::firstOrNew([['sisid', '=', $request->sisid], ['usucpf', '=', $request->usucpf]]);
//        $test = Test::firstOrCreate('test_name', $request->test_name); 
//        $test->update($request->all());

        if ($usuarioSistema->suscod != $request->suscod) {
            $usuarioSistema->fill(['usucpf' => $request->usucpf, 'sisid' => $request->sisid, 'suscod' => $request->suscod]);
           if ($usuarioSistema->save()) {
               $resultHistorico = HistoricoUsuario::salvaHistorico($request);
           } else {
//               \DB::rollback();
               return [false, 'msg' => 'O status do usuário não pode ser atualizado corretamente, por gentileza tente novamente.'];
           }
        }

        $resultPerfilUsuario = $this->storePerfilUsuario ($request);        


//        \DB::commit();
        return [true, 'msg' => 'O vinculo do usuário ao sistema foi atualizado com sucesso.'];
        //return redirect()->route($this->prefixo . ".usuarioglobal.edit", ['id' => $request->usucpf, 'aba' => 'userSis'])->with('success', 'O vinculo do usuário ao sistema foi atualizado com sucesso.');
        
    }
    
    public function storePerfilUsuario ($request) {
//      $perfilUsuario = new PerfilUsuario();
        $result = PerfilUsuario::where(['usucpf' => $request->usucpf])
                ->whereIn('pflcod', function($query) use($request){
                    $query->select('pflcod')->from('seguranca.perfil')->where('sisid', $request->sisid);
            
        })->delete();

        foreach ($request->pflcod as $perfil) {
            $insert[] = ['usucpf' => $request->usucpf, 'pflcod' => $perfil];
        }

        return PerfilUsuario::insert($insert);
        
    }
    
    public function getHistorico(Request $request) {
        $result = HistoricoUsuario::getHistorico($request);
        return ($result);
    }
    
    public function renderView(Request $request) {
        $usuarioRespo = new UsuarioResponsabilidadeController();
        return $usuarioRespo->renderView($request);
    }


//
//
//    public function show($id)
//    {
//        $menu = Menu::find($id);
//        return view('Menu.show', ['menu' => $menu]);
//    }
//
//
//    public function edit($id)
//    {
//        $menu = Menu::find($id);
//        return view('seguranca::menu.editar', compact('menu'));
//    }
//
//
//    public function update(Request $request, $id)
//    {
//        $this->validate($request, $this->menu->rules);
//        Menu::find($id)->update($request->all());
//        
//        return redirect()->route('seguranca.menu.index')->with('success', 'Menu atualizado com sucesso.');
//    }


    public function destroy($id)
    {
        UsuarioResponsabilidade::find($id)->update(['status' => 'I']);
        //return redirect()->route('seguranca.usuariosistema.index')->with('success', "Responsabilidade excluida com sucesso.");
    }
}