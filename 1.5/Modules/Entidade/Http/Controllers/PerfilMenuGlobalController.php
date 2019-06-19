<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Illuminate\Routing\Router;
use Modules\Seguranca\Entities\Menu;
use Modules\Seguranca\Entities\PerfilMenu;
use Modules\Seguranca\Entities\Sistema;
use Modules\Seguranca\Entities\Perfil;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Yajra\Datatables\Datatables;

class PerfilMenuGlobalController extends Controller
{
    private $menu;
    private $prefixo;

    public function __construct()
    {
        parent::__construct();
        $this->menu = new PerfilMenu();
        $this->prefixo = $_SESSION['prefixo'];
        $this->sistema = Sistema::where('sisstatus', 'A')->orderBy('sisdsc', 'asc')->pluck('sisdsc', 'sisid');
        
    }

    public function index()
    {
        return view('seguranca::perfilmenuglobal.index', ['prefixo' => $this->prefixo, 'sistema' => $this->sistema]);
    }

    public function ListarPerfilBySistema(Request $request) {
        if ($request->ajax()) {
            $perfis = Perfil::where(['pflstatus' => 'A','sisid' => $request->sisid])->orderBy('pfldsc')->get();
            return $perfis;
        }
    }
    
    public function create()
    {
        $sisid = $_SESSION['sisid'];
        return view('seguranca::perfilmenuglobal.criar', ['sisid' => $sisid, 'prefixo' => $this->prefixo]);
    }


    public function store(Request $request)
    {        
        $this->validate($request, $this->menu->rules);
        
        $sisid = $_SESSION['sisid'];
        Menu::create($request->all() + ['sisid' => $sisid, 'mnustatus' => 'A']);
        
        return redirect()->route($this->prefixo . '.perfilmenuglobal.index')->with('success', 'Menu salvo com sucesso.');
    }


    public function show($id)
    {
        $menu = Menu::find($id);      
        return view('Menu.perfilshow', ['menu' => $menu]);        
    }


    public function edit($id)
    {   
        $menu = Menu::find($id);
        $prefixo = $this->prefixo;
        return view('seguranca::perfilmenuglobal.editar', compact('menu'), compact('prefixo'));
    }


    public function update(Request $request, $id)
    {
        $this->validate($request, $this->menu->rules);
        Menu::find($id)->update($request->all());
        
        return redirect()->route($this->prefixo . '.perfilmenuglobal.index')->with('success', 'Menu atualizado com sucesso.');
    }


    public function destroy($id)
    {
        Menu::find($id)->update(['mnustatus' => 'I']);
        return redirect()->route($this->prefixo . '.perfilmenuglobal.index')->with('success', 'Menu excluido com sucesso.');
    }
    

    public function associarMenu(Request $request)
    {
        //dd($request);
        if ($request->ajax())
        {
            $pflcod = $request->pflcod;
            $mnuid  = $request->mnuid;
              
            $perfilMenu = PerfilMenu::where(['pflcod' => $pflcod,'mnuid' => $mnuid])->get();
              
              if(!$perfilMenu->isEmpty()){
                  
                  PerfilMenu::where(['pflcod' => $pflcod,'mnuid' => $mnuid])->delete();                   
                  
              }else{
                  
                  $perfilMenu = new PerfilMenu;
                  $perfilMenu->pflcod = $pflcod;
                  $perfilMenu->mnuid = $mnuid;
                  $perfilMenu->pmnstatus = 'A';
                  $perfilMenu->save();                  
              }
        }
    }
    
    
    public function getList(Request $request) {
        
        if ($request->ajax()) 
        {
            
            $sisid = $request->sisid;
            $pflcod = $request->pflcod != "" ? $request->pflcod : 0;
            
            $tbPerfilMenu = with(new PerfilMenu())->getTable();
            $tbMenu = with(new Menu())->getTable();
            
            DB::enableQueryLog();
            
            $menus = Menu::leftJoin($tbPerfilMenu, function($join) use ($pflcod, $tbPerfilMenu, $tbMenu){
                
                $join->on($tbPerfilMenu.'.mnuid', '=', $tbMenu.'.mnuid')->where($tbPerfilMenu.'.pflcod', '=', (int)$pflcod, 'and');
                
            })->select('menu.mnucod', 'menu.mnudsc', 'menu.mnutransacao', 'menu.mnushow', 'menu.mnuid','menu.sisid', 'perfilmenu.mnuid as mnuidp', 'perfilmenu.pflcod', 'perfilmenu.pmnstatus')       
                ->where('menu.mnustatus', '=', 'A')
                ->where('menu.sisid', '=', $sisid)
                ->orderBy('menu.mnucod')
                ->get(); 
            
            //dd(DB::getQueryLog());
            
            $datatables = Datatables::of($menus)->addColumn('action', function ($menus) {
                
            $checked = $menus->mnuidp == "" ? "" : "checked"; 
                
                return <<<HTML
                            <input class="chkPerfil" id="chkPerfil" type="checkbox" mnuid="$menus->mnuid" value="$menus->mnucod" $checked/>
HTML;
                    })->editColumn('mnushow', function (Menu $menu) {
                        if ($menu->mnushow == 't') {
                            return 'Sim';
                        } else {
                            return 'Não';
                        }
                    })->editColumn('mnudsc', function (Menu $menu) {

                if ($menu->mnuidpai) {
                    return '&#8627;&nbsp;&nbsp;' . $menu->mnudsc;
                } else {
                    return $menu->mnudsc;
                }
                
            })->addColumn('mnutransacao', function (Menu $menu) {
                return $menu->mnutransacao;
            });

            return $datatables->make(true);
        }
    }

}
