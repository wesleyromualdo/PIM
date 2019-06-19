<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Modules\Seguranca\Entities\ExportarSistema;
use Modules\Seguranca\Entities\Usuario;
use Modules\Seguranca\Entities\Sistema;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Yajra\Datatables\Datatables;
use Carbon\Carbon as Carbon;



class ExportarSistemaController extends Controller
{
    private $prefixo;

    public function __construct()
    {
        parent::__construct();
    }

    public function index()
    {
        $sistemas = Sistema::orderBy('sisdsc', 'ASC')->pluck('sisdsc','sisid');
//        dd($sistemas);
        return view('seguranca::exportarsistema.index',compact('sistemas'));
    }

    public function create()
    { 
        $sisid = $_SESSION['sisid'];
        return view('seguranca::ExportarSistema.criar', ['sisid' => $sisid, 'prefixo' => $this->prefixo]);
    }

    public function store(Request $request)
    {
        $this->validate($request, $this->ExportarSistema->rules);
        
        $sisid = $_SESSION['sisid'];
        Erro::create($request->all() + ['sisid' => $sisid, 'pflstatus' => 'A', 'pflresponsabilidade' => 'N']);
        
        return redirect()->route($this->prefixo.'.ExportarSistema.index')->with('success', 'Erro salvo com sucesso');
    }

    public function edit($id)
    {

    }

    public function update(Request $request, $id)
    {
        $this->validate($request, $this->ExportarSistema->rules);
        Erro::find($id)->update($request->all());
        
        return redirect()->route($this->prefixo . '.ExportarSistema.index')->with('success', 'Erro atualizado com sucesso');
    }

    public function destroy($id)
    {
        Erro::find($id)->update(['pflstatus' => 'I']);
        
        return redirect()->route($this->prefixo . '.ExportarSistema.index')->with('success', 'Erro excluido com sucesso');
    }
    public function show($id)
    {

    }
    public function exportarSistema(Request $request){
//        dd($request);
        if($request->sisid)
        {
            $sql = '<BR/><BR/><BR/>--INSERE SISTEMA <BR/><BR/><BR/>';
            $sql .= ExportarSistema::montaInsertSegurancaSistema($request->sisid);

            if($request->segurancaperfil =='true') {
                $sql .= '<BR/><BR/><BR/>--INSERE PERFIL<BR/><BR/><BR/>';
                $sql .= ExportarSistema::montaInsertSegurancaPerfil($request->sisid);
            }
            if($request->segurancamenu =='true') {
                $sql .= '<BR/><BR/><BR/>--INSERE MENU<BR/><BR/><BR/>';
                $sql .= ExportarSistema::montaInsertSegurancaMenu($request->sisid);
            }
            if($request->perfilmenu =='true') {
                $sql .= '<BR/><BR/><BR/>--INSERE PERFIL MENU<BR/><BR/><BR/>';
                $sql .= ExportarSistema::montaInsertPerfilMenu($request->sisid);
            }
            if($request->perfilusuario =='true') {
                $sql .='<BR/><BR/><BR/>--INSERE PERFIL USUÁRIO<BR/><BR/><BR/>';
                $sql .= ExportarSistema::montaInsertPerfilUsuario($request->sisid);
            }
            if($request->tipodocumento =='true') {
                $sql .= '<BR/><BR/><BR/>--INSERE WORKFLOW TIPO DOCUMENTO<BR/><BR/><BR/>';
                $sql .= ExportarSistema::montaInsertTipoDocumento($request->sisid);
            }
            if($request->estadodocumento =='true') {
                $sql .= '<BR/><BR/><BR/>--INSERE WORKFLOW ESTADO DOCUMENTO<BR/><BR/><BR/>';
                $sql .= ExportarSistema::montaInsertEstadoDocumento($request->sisid);
            }
            if($request->acaoestadoducomento =='true') {
                $sql .= '<BR/><BR/><BR/>--INSERE WORKFLOW AÇÃO ESTADO DOCUMENTO<BR/><BR/><BR/>';
                $sql .= ExportarSistema::montaInsertAcaoEstadoDocumento($request->sisid);
            }
            if($request->estadodocumentoperfil =='true') {
                $sql .= '<BR/><BR/><BR/>--INSERE WORKFLOW AÇÃO ESTADO DOCUMENTO PERFIL<BR/><BR/><BR/>';
                $sql .= ExportarSistema::montaInsertEstadoDocumentoPerfil($request->sisid);
            }
            if($request->usuarioresponsabilidade =='true') {
                $sql .= '<BR/><BR/><BR/>--INSERE RESPONSABILIDADES<BR/><BR/><BR/>';
                $sql .= ExportarSistema::montaInsertUsuarioResponsabilidade($request->sisid);
            }
        }
        return response($sql, 200);
    }
    public function getList(Request $request)
    {

    }

}
