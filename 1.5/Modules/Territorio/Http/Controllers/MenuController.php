<?php

namespace Modules\Seguranca\Http\Controllers;

use Modules\Seguranca\Entities\Menu;
use Illuminate\Http\Request;
use App\Http\Controllers\Controller;
use Modules\Seguranca\Http\Requests\MenuRequest;
use Yajra\Datatables\Datatables;

class MenuController extends Controller
{
    private $menu;
    private $prefixo;
    

    public function __construct()
    {
        parent::__construct();
        $this->menu = new Menu();
        $this->sisid = $_SESSION['sisid'];
        $this->prefixo = $_SESSION['prefixo'];
    }

    public function index()
    {
        return view('seguranca::menu.index', ['prefixo' => $this->prefixo]);
    }

    public function create(Request $request)
    {
        $sisid = $_SESSION['sisid'];
        $menus = $this->menu->where(['mnustatus' => 'A', 'mnusnsubmenu' => true, 'sisid' => $sisid ])->pluck('mnudsc', 'mnuid');
        $parametros = array('prefixo' => $this->prefixo, 'sisid' => $sisid, 'DropMenus' => $menus);
        if(!empty($request->mnuidpai)){
            $menu = new \stdClass();
            $menu->mnuidpai = $request->mnuidpai;
            if (!empty($request->mnutipopai)) {                
                $mnutipo = ($request->mnutipopai < 4) ? $request->mnutipopai +1 : 4;
            } else {
                $mnutipo = 0;
            }
            $menu->mnutipo = $mnutipo;
            $parametros = array('prefixo' => $this->prefixo, 'sisid' => $sisid, 'DropMenus' => $menus, 'menu' =>$menu);
        }
        return view('seguranca::menu.criar', $parametros);
        //return view('seguranca::menu.criar', ['prefixo' => $this->prefixo, 'sisid' => $sisid]);
    }

    public function store(MenuRequest $request)
    {
        $sisid = $_SESSION['sisid'];
        $this->validate($request, $this->menu->rules);
        $menu = new Menu();
        $menu->fill($request->all() + ['sisid' => $sisid, 'mnustatus' => 'A']);
        $menu->save();

        return redirect()->route($this->prefixo.'.menu.index')->with('success', 'Menu salvo com sucesso.');
    }

    public function show($id)
    {
        $menu = Menu::find($id);      
        return view('Menu.show', ['menu' => $menu, 'prefixo' => $this->prefixo]);        
    }

    public function edit($id)
    {
        $menu = Menu::find($id); 
        $prefixo = $this->prefixo;
        $menus = $this->menu->where(['mnustatus' => 'A', 'mnusnsubmenu' => true, 'sisid' => $menu->sisid, ['mnuid', '<>', $id] ])->pluck('mnudsc', 'mnuid');
        return view('seguranca::menu.editar', ['menu' => $menu, 'prefixo' => $prefixo, 'sisid' => $menu->sisid, 'DropMenus' => $menus]);
    }


    public function update(MenuRequest $request, $id)
    {
        $this->validate($request, $this->menu->rules);
        $params = $request->all();
        if ($request->mnutipo < 2) {
            $params += ['mnuidpai' => null];
        }
        Menu::find($id)->update($params);

        return redirect()->route($this->prefixo . '.menu.index')->with('success', 'Menu atualizado com sucesso.');
    }


    public function destroy($id)
    {
        Menu::find($id)->update(['mnustatus' => 'I']);        
        return redirect()->route($this->prefixo . '.menu.index')->with('success', 'Menu excluido com sucesso.');
    }

    
 public function getList(Request $request)
    {
        if ($request->ajax()) {
            
            $sisid = $_SESSION['sisid'];
            $menus = Menu::where(['mnustatus' => 'A','sisid' => $sisid])->get();

            $datatables = Datatables::of($menus)->addColumn('action', function ($menus) {
                    $url_edit = route($_SESSION['prefixo'].".menu.edit", $menus->mnuid);
                    $url_destroy = route($_SESSION['prefixo'].".menu.destroy", $menus->mnuid);
                    $mnutipo = ($menus->mnutipo < 4) ? $menus->mnutipo +1 : 4;
                    $url_add = route($_SESSION['prefixo'].".menu.create", ['mnuidpai' => $menus->mnuid, 'mnutipo' => $mnutipo]);
                    $action = "<div class=\"text-center\">
                            <a href=\"{$url_edit}\" class=\"btn btn-sm btn-warning\" style=\"float:left; \"><i class=\"fa fa-pencil\"></i></a>
                            <a href=\"{$url_destroy}\" class=\"btn btn-sm btn-danger act_bt_delete\" style=\"float:left; \"><i class=\"fa fa-trash\"></i></a>";
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
