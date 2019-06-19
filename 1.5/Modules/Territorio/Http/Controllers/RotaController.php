<?php

namespace Modules\Seguranca\Http\Controllers;

use Modules\Seguranca\Entities\Perfil;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use App\Http\Controllers\Controller;
use Modules\Seguranca\Entities\Rota;
use Modules\Seguranca\Entities\Sistema;
use Modules\Seguranca\Http\Requests\MenuRequest;
use Modules\Seguranca\Http\Requests\PerfilRequest;
use Yajra\Datatables\Datatables;

class RotaController extends Controller
{
    private $rota;
    private $prefixo;

    public function __construct()
    {
        parent::__construct();
        $this->rota = new Rota();
        $this->prefixo = $_SESSION['prefixo'];
    }

    /**
     * Display a listing of the resource.
     * @return Response
     */
    public function index()
    {
        $sistemas = Sistema::recuperaSistemasCombo();
        return view('seguranca::rota.index', ['prefixo' => $this->prefixo], compact('sistemas'));
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
        $this->rota->fill($request->all() + ['sisid' => $sisid, 'pflstatus' => 'A', 'pflresponsabilidade' => 'N']);
        $this->rota->save();

        return redirect()->route($this->prefixo . '.rota.index')->with('success', 'Rota salvo com sucesso');
    }

    /**
     * Show the specified resource.
     * @return Response
     */
    public function show($id)
    {
        $rota = Rota::find($id);
        return view('Perfil.show', ['perfil' => $rota, 'prefixo' => $this->prefixo]);
    }

    /**
     * Show the form for editing the specified resource.
     * @return Response
     */
    public function edit($id)
    {
        $rota = Rota::find($id);
        $prefixo = $this->prefixo;
        return view('seguranca::rota.editar', compact('rota'), compact('prefixo'));
    }

    /**
     * Update the specified resource in storage.
     * @param  Request $request
     * @return Response
     */
    public function update(PerfilRequest $request, $id)
    {
        Perfil::find($id)->update($request->all());
        return redirect()->route($this->prefixo . '.perfil.index')->with('success', 'Perfil atualizado com sucesso');
    }

    /**
     * Remove the specified resource from storage.
     * @return Response
     */
    public function destroy($id)
    {
        Rota::find($id)->update(['pflstatus' => 'I']);

        return redirect()->route($this->prefixo . '.rota.index')->with('success', 'Rota excluido com sucesso');
    }


    /**
     * Retorna as rotas do sistema por sisid.
     * @return Response
     */
    public function getList(Request $request)
    {

        if ($request->ajax()) {
            $sisid = $request->sisid;
            $rotasAplicacao = $this->preparaArrayColetion(Rota::getRotasSistema($sisid));

            $rotas = Rota::getRotasSistemaDB($sisid);

            $datatables = Datatables::of($rotasAplicacao)
                ->addColumn('action', function ($rotasAplicacao) use ($rotas) {

                    $bool = $this->verificaRotasCoencidem($rotasAplicacao['ds_rota'], $rotas);

                    $checked = $bool ? "checked='checked'" : "";
                    return <<<HTML
                       <div class="text-center">
                            <input type="checkbox"  class="chkRotaSis" ds_rota="{$rotasAplicacao['ds_rota']}" {$checked} id="{$rotasAplicacao['ds_rota']}"  name="ds_rota" />
                        </div>
HTML;
                });
            return $datatables->make(true);

        }
    }


    //-----------------------------------------------------------------------------------

    /**
     * Retorna as rotas do Banco por sisid.
     * @return Response
     */
    public function getListRotasBanco(Request $request)
    {

        if ($request->ajax()) {
            $sisid = $request->sisid;
            $rotasAplicacao = $this->preparaArrayColetion(Rota::getRotasSistema($sisid));

            $rotas = Rota::getRotasSistemaDB($sisid);

            $datatables = Datatables::of($rotas)
                ->addColumn('action', function ($rotas) {

                    $bool = $this->verificaRotasHabilitadas($rotas);

                    $checked = $bool ? "checked='checked'" : "";
                    return <<<HTML
                       <div class="text-center">
                            <input type="checkbox"  class="chkRotaDb" co_rota="{$rotas['co_rota']}" {$checked} id="{$rotas['co_rota']}"  name="co_rota" />
                        </div>
HTML;
                });
            return $datatables->make(true);

        }
    }

    //-----------------------------------------------------------------------------------

    /**
     * @param array $dados
     * @return \Illuminate\Support\Collection
     */
    private function preparaArrayColetion($dados = array())
    {
//        dd($dados);
        $arrRotas = [];
        if (is_array($dados)) {
            foreach ($dados as $dado) {
                $arrRotas[] = ['ds_rota' => $dado, 'ds_rota_descricao' => 'Pagina ' . $dado];
            }
        }
        return collect($arrRotas);
    }

    /**
     * Verifica se a $rota esta presente no banco de dados
     * @param $rota
     * @param $arrRotaDB
     * @return bool
     */
    private function verificaRotasCoencidem($rota, $arrRotaDB)
    {
        if (is_object($arrRotaDB)) {
            foreach ($arrRotaDB as $rotaDB) {
                if ($rota == $rotaDB->ds_rota) {
                    return true;
                }
            }
        }
        return false;
    }


    private function verificaRotasHabilitadas($arrRotaDB)
    {
        if (is_object($arrRotaDB)) {

            $rotaBanco = Rota::find($arrRotaDB->co_rota);

            if (!$rotaBanco->st_exclusao) {
                return true;
            } else {
                return false;
            }
        }

    }


    /**
     * Salva a rota no banco caso exista apenas no sistema.
     * Habilita ou desabilita uma rota caso ela exista no banco
     * @param Request $request
     */
    public function atualizaRota(Request $request)
    {
        if ($request->ajax()) {
            $sisid = $request->sisid;
            $dsRota = $request->ds_rota;
            $chekecd = $request->checked;
            $checkedAll = $request->checkedAll;

            if ($dsRota == null) {
                $rotasSis = Rota::getRotasSistema($sisid);
                foreach ($rotasSis as $rotaSis) {
                    $rota = Rota::getRotaDescricao($rotaSis, $sisid);
                    $this->atualizaRotaDb($rota, $checkedAll, $sisid, $rotaSis); /*Rotasis e a descricao da rota */
                }
            } else {
                $rota = Rota::getRotaDescricao($dsRota, $sisid);
                /*a rota existe, so atualizar o campo st_habilitado e dt_exclusao*/
                $this->atualizaRotaDb($rota, $chekecd, $sisid, $dsRota);
            }

        }
    }

    /**
     * @param $rota
     * @param $status
     * @param $sisid
     * @param $dsRota
     */
    private function atualizaRotaDb($rota, $status, $sisid, $dsRota)
    {
        if ($rota != null) {
            $rota->st_habilitado = $status ? true : false;
            $rota->update();
            //alterar no perfil_rota_
            $perfilRotaController = new PerfilRotaController();
            $perfilRotaController->alteraStatusPerfilRota($status, $rota->co_rota);
        } else {
            if ($status) {
//                var_dump('oi');
                $this->rota = new Rota();
                $this->rota->ds_rota = $dsRota;
                $this->rota->ds_rota_descricao = 'Pagina ' . $dsRota;
                $this->rota->st_habilitado = true;
                $this->rota->dt_cadastro = date('Y-m-d h:m:s');
                $this->rota->sisid = $sisid;
//                var_dump($this->rota);
                $this->rota->save();
            }

        }
    }





    /**
     * Atualiza o Status da rota no banco.
     * Habilita ou desabilita uma rota caso ela exista no banco
     * @param Request $request
     */
    public function atualizaStatusRotaBanco(Request $request)
    {

        if ($request->ajax()) {
            $sisid = $request->sisid;
            $co_Rota = $request->co_rota;
            $checkedAll = $request->checked;

            if (($checkedAll == 0)&&($co_Rota == null)) {
                echo "Desativar todos -> ".$checkedAll;
                $rotasDB = Rota::getRotasSistemaDB($sisid);
                foreach ($rotasDB as $rotaBanco) {
                    $rotaBanco->st_habilitado = false;
                    $rotaBanco->dt_exclusao = date('Y-m-d H:i:s');
                    $rotaBanco->st_exclusao = true;
                    $rotaBanco->update();
                }
            } elseif (($checkedAll == 1)&&($co_Rota == null)) {
                echo "Ativar todos -> ".$checkedAll;
                $rotasDB = Rota::getRotasSistemaDB($sisid);
                foreach ($rotasDB as $rotaBanco) {
                    $rotaBanco->st_habilitado = true;
                    $rotaBanco->dt_exclusao = null;
                    $rotaBanco->st_exclusao = false;
                    $rotaBanco->update();
                }

            } else {
                $rotaBanco = Rota::find($co_Rota);
                if ($rotaBanco->st_habilitado) {
                    echo "Desativar";
                    $rotaBanco->st_habilitado = false;
                    $rotaBanco->dt_exclusao = date('Y-m-d H:i:s');
                    $rotaBanco->st_exclusao = true;
                    $rotaBanco->update();

                } else {
                    echo "Ativar";
                    $rotaBanco->st_habilitado = true;
                    $rotaBanco->dt_exclusao = null;
                    $rotaBanco->st_exclusao = false;
                    $rotaBanco->update();
                }
            }


        }

    }




}