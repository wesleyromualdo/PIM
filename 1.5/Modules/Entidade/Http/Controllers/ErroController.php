<?php

namespace Modules\Seguranca\Http\Controllers;

use DB;
use Modules\Seguranca\Entities\Erro;
use Modules\Seguranca\Entities\Usuario;
use Modules\Seguranca\Entities\Sistema;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Yajra\Datatables\Datatables;
use Carbon\Carbon as Carbon;



class ErroController extends Controller
{
    private $erro;
    private $prefixo;

    public function __construct()
    {
        parent::__construct();
        $this->erro = new Erro();
    }

    public function index()
    {
        return view('seguranca::erro.index');
    }

    public function create()
    { 
        $sisid = $_SESSION['sisid'];
        return view('seguranca::erro.criar', ['sisid' => $sisid, 'prefixo' => $this->prefixo]);
    }

    public function store(Request $request)
    {
        $this->validate($request, $this->erro->rules);
        
        $sisid = $_SESSION['sisid'];
        Erro::create($request->all() + ['sisid' => $sisid, 'pflstatus' => 'A', 'pflresponsabilidade' => 'N']);
        
        return redirect()->route($this->prefixo.'.erro.index')->with('success', 'Erro salvo com sucesso');
    }

    public function edit($id)
    {
        $erro = Erro::find($id);
        $prefixo = $this->prefixo;
        return view('seguranca::erro.editar', compact('erro'), compact('prefixo'));
    }

    public function update(Request $request, $id)
    {
        $this->validate($request, $this->erro->rules);
        Erro::find($id)->update($request->all());
        
        return redirect()->route($this->prefixo . '.erro.index')->with('success', 'Erro atualizado com sucesso');
    }

    public function destroy($id)
    {
        Erro::find($id)->update(['pflstatus' => 'I']);
        
        return redirect()->route($this->prefixo . '.erro.index')->with('success', 'Erro excluido com sucesso');
    }
    public function show($id)
    {
        $erro = Erro::find($id);
        $erro->errdescricaocompleta = stripslashes(html_entity_decode($erro->errdescricaocompleta, ENT_QUOTES, "ISO-8859-1"));
        $usuario = Usuario::find(str_replace("'", "", $erro->usucpf));
        $sistema = Sistema::find($erro->sisid);

        return view('seguranca::erro.editar', compact('erro','usuario','sistema'));
    }

    public function getList(Request $request)
    {
//        if ($request->ajax())
//        {
            $anoAtual = (int) strftime("%Y", time());
            $arrAnos = [($anoAtual-1), $anoAtual];
//            $anos = ($anoAtual-1).','.$anoAtual;
//            $qb = \DB::connection()->getDoctrineConnection()->createQueryBuilder();
//            $qb->select('count(er.errid) as qtderros, MAX(errid) as errid, er.errdescricao, sisdsc, er.errlinha ')
//                ->from('seguranca.erro', 'er')
//                ->leftJoin('er', 'seguranca.sistema', 'sis', "er.sisid = sis.sisid")
//                ->where('extract(YEAR FROM er.errdata) IN ('.$anos.') AND er.errdescricaocompleta IS NOT NULL AND errdata >= (now() - INTERVAL \'' .$request->horas. ' hours\')')
//                ->groupBy( 'errdescricao, sisdsc, errlinha')
//                ->orderBy('sisdsc');
//            $erros =  $qb->execute()->fetchAll(\PDO::FETCH_ASSOC);
            
            $erros = Erro::selectRaw('count(er.errid) as qtderros, MAX(errid) as errid, er.errdescricao, sisdsc, er.errlinha')
                    ->from('seguranca.erro AS er')
                    ->leftjoin('seguranca.sistema AS sis', 'er.sisid', '=', 'sis.sisid')
                    ->whereIn(\DB::raw('extract(YEAR FROM er.errdata)'),  $arrAnos)
                    ->whereNotNull('er.errdescricaocompleta')
                    ->where('errdata', '>=', \DB::raw('(now() - INTERVAL \'' .$request->horas. ' hours\')'))
                    ->groupBy(\DB::raw('errdescricao, sisdsc, errlinha'))
                    ->orderBy('sisdsc')->get()->toArray();
            
            //dd($erros, $result->toSql(), $result->getBindings(), $qb->getsql());
//                $erros =  $qb->getsql();
//    dd($erros);
            $arrErros = array() ;
            foreach ($erros as $key => $value) {
                $objErros = new \stdClass();
                $objErros->errid = $value['errid'];
                $objErros->sisdsc = $value['sisdsc'];
                $objErros->errdescricao = $value['errdescricao'];
                $objErros->qtderros = $value['qtderros'];
    //                $objErros->errtipo = $value['errtipo'];
              array_push($arrErros,$objErros);

            }

            $collectionErros = collect($arrErros);

            $datatables = Datatables::of($collectionErros)
                ->addColumn('action', function ($erro) {
                    $url_show = route("seguranca.logErro.show", $erro->errid);
                    return <<<HTML
                        <div class="text-center">                       
                            <a href="{$url_show}" class="btn btn-sm btn-warning"><i class="fa fa-eye"></i></a>
                        </div>
HTML;
                });

            return $datatables->make(true);

        }
//    }

}
