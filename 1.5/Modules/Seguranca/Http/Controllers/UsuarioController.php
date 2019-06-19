<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Illuminate\Routing\Router;
use Modules\Seguranca\Entities\Usuario;
use Modules\Seguranca\Entities\Sistema;

use Modules\Seguranca\Http\Requests\UsuarioRequest;
use Modules\Territorio\Entities\Municipio;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Yajra\Datatables\Datatables;
use \Modules\Publico\Entities\UnidadeGestora;
use \Modules\Publico\Entities\UnidadeOrcamentaria;
use \Modules\Publico\Entities\UnidadeFederativa;
use \Modules\Seguranca\Entities\Perfil;
use \Modules\Publico\Entities\Cargo;
use \Modules\Publico\Entities\TipoOrgao;


class UsuarioController extends Controller
{
    private $usuario;
    private $prefixo;

    public function __construct()
    {
        parent::__construct();
        $this->usuario = new Usuario();
        $this->prefixo = $_SESSION['prefixo'];
    }
    
    public function index()
    {  

        $unidadesFederativas = UnidadeFederativa::whereNotNull('codigoibgeuf')->pluck('descricaouf', 'regcod');
        $unidadesOrcamentarias = UnidadeOrcamentaria::where('unistatus','=','A')->orderBy('unicod')->orderBy('unidsc', 'ASC')->pluck('unidsc', 'unicod');
        
        $sistema = $_SESSION['sisdsc'];
        
        $perfis = Perfil::where('sisid', '=', $_SESSION['sisid'])->orderBy('pfldsc', 'ASC')->pluck('pfldsc', 'pflcod');
        $prefixo = $this->prefixo;
        return view('seguranca::usuario.index', compact('unidadesFederativas','sistema', 'perfis', 'unidadesOrcamentarias', 'cargos', 'prefixo'));
    }
    
    
    public function create()
    {
        $sisid = $_SESSION['sisid'];
        return view('seguranca::usuario.criar', ['sisid' => $sisid, 'prefixo' => $this->prefixo]);
    }
    
    public function store(Request $request)
    {
        $sisid = $_SESSION['sisid'];
        Usuario::create($request->all() + ['sisid' => $sisid, 'pflstatus' => 'A', 'pflresponsabilidade' => 'N']);
        
        return redirect()->route($this->prefixo . '.usuario.index')->with('success', 'Usuario salvo com sucesso');
    }
    
    public function show($id)
    {
        $perfil = Usuario::find($id);        
        return view('Usuario.editar', ['perfil' => $perfil, 'prefixo' => $this->prefixo]);
    }
    
    public function edit($id)
    {
        $usuario = Usuario::find($id);

        $unidadesFederativas = UnidadeFederativa::whereNotNull('codigoibgeuf')->pluck('descricaouf', 'regcod');

        $municpiosPorUnidade = Municipio::where('estuf','=',$usuario->regcod)->pluck('mundescricao','muncod');
        
        $unidadesOrcamentarias = UnidadeOrcamentaria::where('unistatus','=','A')->orderBy('unicod')->orderBy('unidsc', 'ASC')->pluck('unidsc', 'unicod');
        
        $unidadesGestoras = array(); //UnidadeGestora::where('ungstatus','=','A')->where('muncod','=',$usuario->muncod)->orderBy('ungcod')->orderBy('ungdsc', 'ASC')->pluck('ungdsc', 'ungcod');

        $sistema =  $_SESSION['sisdsc'];
        
        $perfis = Perfil::where('sisid', '=', $_SESSION['sisid'])->orderBy('pfldsc', 'ASC')->pluck('pfldsc', 'pflcod');
        
        $cargos = array(); //Cargo::pluck('cardsc','carid');
        
        $tipoOrgao = TipoOrgao::where('tpostatus','=','A')->orderBy('tpodsc', 'ASC')->pluck('tpodsc', 'tpocod');
        
        $prefixo = $this->prefixo;

        return view('seguranca::usuario.editar', compact('unidadesFederativas','sistema', 'perfis', 'unidadesOrcamentarias', 'cargos', 'usuario','unidadesGestoras','tipoOrgao','municpiosPorUnidade', 'prefixo'));
        
    }

    public function update(UsuarioRequest $request, $usucpf)
    {

        $usr = Usuario::find($usucpf);
        $usr->usucpf = $usucpf;
        $usr->usunome = $request->usunome;
        $usr->usunomeguerra = $request->usunomeguerra;
        $usr->ususexo = $request->ususexo;
        $usr->usudatanascimento = $request->usudatanascimento;        
        $usr->regcod =  $request->unidadeFederativa;
        $usr->muncod = $request->municipio;        
        $usr->usufoneddd = $request->ddd;
        $usr->usufonenum = $request->usufonenum;
        $usr->usuemail = $request->email;
        $usr->carid = $request->cargo;
        $usr->ungcod = $request->unidadeGestora;
        $usr->unicod = $request->unidadeOrcamentaria;
        $usr->tpocod = $request->tipoOrgao;        
        
        if($request->alterarSenhaPadrao)
        {
            $usr->usuchaveativacao =  false;
            $usr->ususenha = $usr->criptografarSenha(SENHA_PADRAO, '');
        }
        
        $usr->save();

       
        return redirect()->route('usuario.index')->with('success', 'Usuario atualizado com sucesso');
    }
    
    public function destroy($id)
    {
        Usuario::find($id)->update(['pflstatus' => 'I']);        
        return redirect()->route('usuario.index')->with('success', 'Usuario excluido com sucesso');
    }    
    
    public function getList(Request $request)
    {
        
        if ($request->ajax())
        {
            $sisid = $_SESSION['sisid'];
            $cpf = str_replace('-', '', str_replace('.', '', $request->usucpf));
            $pflcod = $request->perfil ? $request->perfil : 0;

            $usuarios = Usuario::where('usuario.usunome', 'like', '%' . $request->usunome . '%');

            if($request->usucpf)
            {
                $usuarios->where('usuario.usucpf', 'like', '%' . $cpf . '%');
            }
            
            if($request->unidadeFederativa)
            {
                $usuarios->where('usuario.regcod', '=', $request->unidadeFederativa);
            }
            
            if($pflcod)
            {
            $usuarios->where('usuario.pflcod', '=', (integer) $pflcod);
            }
            
            if($request->unidadeOrcamentaria)
            {
                $usuarios->where('usuario.unicod', '=', (integer) $request->unidadeOrcamentaria);
            }
            
            if($request->suscod)
            {
                $usuarios->where('usuario.suscod', '=', $request->suscod);
            }            

            if ($request->usuchaveativacao)
            {
                $usuarios->where('usuario.usuchaveativacao', '=', $request->usuchaveativacao);
            }            
            
          
            $usuarios->leftJoin('public.cargo', 'seguranca.usuario.carid', '=', 'public.cargo.carid');
            $usuarios->leftJoin('public.uf', 'seguranca.usuario.regcod', '=', 'public.uf.regcod');
            $usuarios->leftJoin('territorios.municipio', 'seguranca.usuario.muncod', '=', 'territorios.municipio.muncod');
            

            if ($request->sitperfil)
            {
                if ($request->sitperfil == "D") {
                    $usuarios->leftJoin('seguranca.usuario_sistema', function($join) use ($pflcod) {
                        $join->on('seguranca.usuario_sistema.pflcod', '=', 'seguranca.usuario.pflcod')->where('seguranca.usuario.pflcod', '=', $pflcod);
                    });
                } else if ($request->sitperfil == "A") {
                    $usuarios->leftJoin('seguranca.perfil', function($join) use ($pflcod) {
                        $join->on('seguranca.perfil.pflcod', '=', 'seguranca.usuario.pflcod')->where('seguranca.usuario.pflcod', '=', $pflcod);
                        ;
                    });
                } else if ($request->sitperfil == "V") {
                    $usuarios->leftJoin('seguranca.perfil', function($join) use ($pflcod) {
                        $join->on('seguranca.perfil.pflcod', '=', 'seguranca.usuario.pflcod')->where('seguranca.usuario.pflcod', '=', $pflcod);
//                                //->orWhere('seguranca.usuario_sistema.pflcod', '=', $pflcod);
                    });
                }
            }                    
            
            $usuarios->limit(3000)->get();

            $datatables = Datatables::of($usuarios)
                    ->addColumn('action', function ($usuario) {
                        $url_edit = route("usuario.edit", $usuario->usucpf);
                        return <<<HTML
                        <div class="text-center">
                            <a href="{$url_edit}" class="btn btn-sm btn-warning"><i class="fa fa-pencil"></i></a>                           
                        </div>
HTML;
                    })
                    ->editColumn('telefone', function (Usuario $usuario) {
                        if ($usuario->usufoneddd && $usuario->usufonenum) {
                            return "(" . $usuario->usufoneddd . ") " . $usuario->usufonenum;
                        }
                    })
                    ->editColumn('unidade', function (Usuario $usuario) {
                        return $usuario->descricaouf;
                    })
                    ->editColumn('cargofuncao', function (Usuario $usuario) {
                        return $usuario->cardsc;
                    })
                    ->editColumn('orgao', function (Usuario $usuario) {
                        return $usuario->orgao;
                    })
                    ->editColumn('uf', function (Usuario $usuario) {
                        return $usuario->regcod;
                    })
                    ->editColumn('municipio', function (Usuario $usuario) {
                        return $usuario->mundescricao;
                    })
                    ->editColumn('usudataatualizacao', function (Usuario $usuario) {
                return \Carbon\Carbon::parse($usuario->usudataatualizacao)->format('d/m/y H:m');
            });            
            
            return $datatables->make(true);
        }
    }
    
    public function GeraTabelaSimularUsuario(Request $request) {
                
        if ($request->ajax()) {
            $Usuario = new Usuario();
            $usuarios = $Usuario->GetUsuariosSimular($request);
            $collection = collect($usuarios);
            
            $datatables = Datatables::of($collection)->addColumn('action', function ($collection) {
                
                $checked = $collection->action == false ? "" : "checked disabled";
                
                return '<input class="chkSimulaUsuario" id="chkSimulaUsuario" type="radio" value="'.$collection->codigo.'" '.$checked.'/>';
            });

            return $datatables->make(true);

        }
        
    }
    
    public function SimulaUsuario(Request $request){
        if ($request->ajax()) {
            $Usuario = new Usuario();
            $usuarios = $Usuario->VerificaUsuarioSimular($request);
//            return $usuarios;
            if (count($usuarios) > 0) {
                $_SESSION['usucpf'] = $request->usucpf;
                $_SESSION['dadosusuario'][$_SESSION['usucpf']] = $usuarios;
            }
        }
        
    }
    
    public function VoltaUsuario(){
        $_SESSION['usucpf'] = $_SESSION['usucpforigem'];
    }
}