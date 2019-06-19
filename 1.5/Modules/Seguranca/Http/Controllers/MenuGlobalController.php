<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Illuminate\Routing\Router;
use Modules\Seguranca\Entities\Menu;
use Modules\Seguranca\Entities\Sistema;
use Illuminate\Http\Request;
use Modules\Seguranca\Http\Requests\MenuGlobalRequest;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Yajra\Datatables\Datatables;

class MenuGlobalController extends Controller
{
    private $menu;
    private $sistema;
    private $prefixo;
    

    public function __construct()
    {
        parent::__construct();
        $this->menu = new Menu();
        //$this->sisid = $_SESSION['sisid'];
        $this->prefixo = $_SESSION['prefixo'];
        $this->sistema = Sistema::where('sisstatus', 'A')->orderBy('sisdsc', 'asc')->pluck('sisdsc', 'sisid');
        
    }

    public function index()
    {
        return view('seguranca::menuglobal.index', ['prefixo' => $this->prefixo, 'sistema' => $this->sistema ]);
    }

    public function create(Request $request)
    {
        $sisid = $request->sisid;
        $menus = $this->menu->where(['mnustatus' => 'A', 'mnusnsubmenu' => true, 'sisid' => $sisid ])->pluck('mnudsc', 'mnuid');//->get();
        $parametros = array('prefixo' => $this->prefixo, 'sisid' => $sisid, 'DropMenus' => $menus);
        return view('seguranca::menuglobal.criar', $parametros);
    }


    public function store(MenuGlobalRequest $request)
    {
        $sisid = $request->sisid;
        $mnuidpai = $request->mnuidpai;
        $menu = new Menu();
        $this->validate($request, $this->menu->rules);
        $menu->fill($request->all() + ['mnustatus' => 'A']);
        $menu->save();
        return view('seguranca::menuglobal.index', ['prefixo' => $this->prefixo, 'sistema' => $this->sistema, 'success' => 'Menu salvo com sucesso.' ]);
        //return redirect()->route($this->prefixo.'.menuglobal.index')->with('success', 'Menu salvo com sucesso.');
    }


//    public function show($id)
//    {
//        $menu = Menu::find($id);
//        return view('Menu.show', ['menu' => $menu, 'prefixo' => $this->prefixo]);
//    }


    public function edit($id)
    {
        $menu = Menu::find($id); 
        $prefixo = $this->prefixo;
        $menus = $this->menu->where(['mnustatus' => 'A', 'mnusnsubmenu' => true, 'sisid' => $menu->sisid, ['mnuid', '<>', $id] ])->pluck('mnudsc', 'mnuid');//->get();
        return view('seguranca::menuglobal.editar', ['menu' => $menu, 'prefixo' => $prefixo, 'sisid' => $menu->sisid, 'DropMenus' => $menus]);
    }


    public function update(MenuGlobalRequest $request, $id)
    {
        $this->validate($request, $this->menu->rules);
        $params = $request->all();
        if ($request->mnutipo < 2) {
            $params += ['mnuidpai' => null];
        }
        Menu::find($id)->update($params);
        
        return view('seguranca::menuglobal.index', ['prefixo' => $this->prefixo, 'sistema' => $this->sistema, 'success' => 'Menu atualizado com sucesso.' ]);
        //return redirect()->route($this->prefixo . '.menuglobal.index')->with('success', 'Menu atualizado com sucesso.');        
    }


    public function destroy($id)
    {
        Menu::find($id)->update(['mnustatus' => 'I']);
        return redirect()->route($this->prefixo . '.menuglobal.index')->with('success', 'Menu excluido com sucesso.');
    }

    
 public function getList(Request $request)
    {
        if ($request->ajax()) {
            $sisid = $request->sisid;
            $menus = Menu::where(['mnustatus' => 'A','sisid' => $sisid])->orderBy('mnucod', 'asc')->get();
                        
            $datatables = Datatables::of($menus)->addColumn('action', function ($menus) {
                    $url_edit = route($_SESSION['prefixo'].".menuglobal.edit", $menus->mnuid);
                    $url_destroy = route($_SESSION['prefixo'].".menuglobal.destroy", $menus->mnuid);
                    $mnutipo = ($menus->mnutipo < 4) ? $menus->mnutipo +1 : 4;
                    $url_add = route($_SESSION['prefixo'].".menuglobal.create", ['mnuidpai' => $menus->mnuid, 'mnutipo' => $mnutipo, 'sisid' =>$menus->sisid]);
                    $action = "<div class=\"text-center\">
                            <a href=\"{$url_edit}\" class=\"btn btn-sm btn-warning \" style=\"float:left; \"><i class=\"fa fa-pencil\"></i> </a>
                            <a href=\"{$url_destroy}\" class=\"btn btn-sm btn-danger act_bt_delete\" style=\"float:left; \"><i class=\"fa fa-trash\"></i> </a>";
                    if ($menus->mnusnsubmenu == true) {
                        $action .= "<a href=\"{$url_add}\" class=\"btn btn-sm btn-success\" style=\"float:left; \"><i class=\"fa fa-plus\"></i></a>";
                    }
                    $action .=  "</div>";
                    return $action;
                })->editColumn('mnushow', function (Menu $menu) {
                    if($menu->mnushow == 't'){
                        return 'Sim';
                    }else {
                        return 'Não';
                    }
                })->editColumn('mnudsc', function (Menu $menu) {
                    if($menu->mnuidpai){
                        return '&#8627;&nbsp;&nbsp;' . $menu->mnudsc;
                    }else{
                        return $menu->mnudsc;
                    }
                });
               
            return $datatables->make(true);
        }
    }
}
