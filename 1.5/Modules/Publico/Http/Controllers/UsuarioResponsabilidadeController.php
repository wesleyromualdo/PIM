<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Illuminate\Routing\Router;
use Modules\Seguranca\Entities\Menu;
use Modules\Seguranca\Entities\Sistema;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Yajra\Datatables\Datatables;
use Modules\Seguranca\Entities\Perfil;
use Modules\Seguranca\Entities\PerfilUsuario;
use Modules\Seguranca\Entities\UsuarioResponsabilidade;
use Modules\Seguranca\Entities\TipoResponsabilidade;




class UsuarioResponsabilidadeController extends Controller
{
    private $menu;
    private $prefixo;
    
    public function __construct()
    {
        parent::__construct();
        $this->menu = new Menu();
//        $this->sisid = $_SESSION['sisid'];
//        $this->usucpf = $_SESSION['usucpf'];
        $this->prefixo = $_SESSION['prefixo'];
    }

    public function index(Request $request)
    {
        $objContainer = new \stdClass();
        $objContainer->perfis = PerfilUsuario::getListaPerfisUsuario($request->usucpf);
        $objContainer->usucpf = $request->usucpf;
        $objContainer->prefixo = $this->prefixo;
        $tipoResponsabilidade = new TipoResponsabilidade();
        $objContainer->tiposResponsabilidades = $tipoResponsabilidade->getTipoResponsabilidadeByPerfil($objContainer);
        return view('seguranca::usuarioresponsabilidade.index',['objContainer'=>$objContainer]);

    }
    
    public function renderView(Request $request) {
        $objContainer = new \stdClass();
        $objContainer->perfis = PerfilUsuario::getListaPerfisUsuario($request->usucpf, $request->sisid);
        $objContainer->usucpf = $request->usucpf;
        $objContainer->prefixo = $this->prefixo;
        //$objContainer->request = $request;
        $objContainer->sisid = $request->sisid;
        $tipoResponsabilidade = new TipoResponsabilidade();
        $objContainer->tiposResponsabilidades = $tipoResponsabilidade->getTipoResponsabilidadeByPerfil($objContainer);
        return view('seguranca::usuarioresponsabilidade.modal-index',['objContainer'=>$objContainer])->render();
    }
    
    public function getList(Request $request) {
        $usrResponsabilidade = new UsuarioResponsabilidade();
        return $usrResponsabilidade->getList($request);
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
    
    public function geraTabelaResponsabilidadesUsuario(Request $request) {
        $tabela = '';
        $objContainer = new \stdClass();
        $objContainer->perfis = PerfilUsuario::getListaPerfisUsuario();
        $objContainer->usucpf = $request->usucpf;
        $tipoResponsabilidade = new TipoResponsabilidade();
        $objContainer->tiposResponsabilidades = $tipoResponsabilidade->getTipoResponsabilidadeByPerfil($objContainer);
        foreach ($objContainer->perfis as $key => $perfil) {
            $tabela[$key] = '<div class="panel-body">';
            foreach($objContainer->tiposResponsabilidades[$key] as $respKey => $responsabilidade ) {
                if (!is_null($responsabilidade->tprcod) && $responsabilidade->pflcod == $key) {
                    $tabela[$key] .= '<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
                        <tr>
                            <td colspan="3">'.$responsabilidade->tprdsc.'</td>
                        </tr>
                        <tr style="color:#000000;">
                            <td valign="top" width="12">&nbsp;</td>
                            <td valign="top">Código</td>
                            <td valign="top">Descrição</td>
                        </tr>';
                        foreach($objContainer->UsuarioResponsabilidades[$key][$responsabilidade->tprcod] as $usrRespKey => $usrResp ) {
                            $tabela[$key] .= '<tr onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'F7F7F7\';" bgcolor="F7F7F7">
                                <td valign="top" width="12">&nbsp;</td>
                                <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap>'.$usrResp->codigo.'</td>
                                <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;">'.$usrResp->descricao.'</td>
                            </tr>';
                        }
                    $tabela[$key] .= '<tr>
                            <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">
                              Total: ('.count($objContainer->UsuarioResponsabilidades[$key][$responsabilidade->tprcod]).')
                            </td>
                        </tr>
                    </table>';
                }
            }
            $tabela[$key] .= '</div>';
        }
        return $tabela;
    }

//    public function create()
//    {
//        $sisid = $_SESSION['sisid'];
//        return view('seguranca::menu.criar', ['sisid' => $sisid]);
//    }
//
//
//    public function store(Request $request)
//    {        
//        $this->validate($request, $this->menu->rules);
//        
//        $sisid = $_SESSION['sisid'];
//        Menu::create($request->all() + ['sisid' => $sisid, 'mnustatus' => 'A']);
//        
//        return redirect()->route('seguranca.menu.index')->with('success', 'Menu salvo com sucesso.');
//    }
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
        //return redirect()->route('seguranca.usuarioresponsabilidade.index')->with('success', "Responsabilidade excluida com sucesso.");
    }
}