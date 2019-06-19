<?php
namespace Simec\Par3\Controle\Iniciativa\Planejamento;

use Simec\Par3\Modelo\Modalidade\Modalidade as modeloModalidade;
use Simec\Par3\Modelo\Ensino\Etapa as modeloEnsinoEtapa;
use Simec\Par3\Modelo\Iniciativa\Iniciativa as modeloIniciativa;
use Simec\Par3\Modelo\Iniciativa\Modalidades as modeloIniciativaModalidades;
use Simec\Par3\Modelo\Iniciativa\IniciativasAnos as modeloIniciativaIniciativasAnos;
use Simec\Par3\Modelo\Iniciativa\IniciativasAreas as modeloIniciativaIniciativasAreas;
use Simec\Par3\Modelo\Iniciativa\IniciativasProgramas as modeloIniciativaIniciativasProgramas;
use Simec\Par3\Modelo\Iniciativa\IniciativasProjetos as modeloIniciativaIniciativasProjetos;
use Simec\Par3\Modelo\Iniciativa\TiposAtendimento as modeloIniciativaTiposAtendimento;
use Simec\Par3\Modelo\Iniciativa\TiposObjeto as modeloIniciativaTiposObjeto;
use Simec\Par3\Modelo\Iniciativa\Areas\Areas as modeloIniciativaAreas;
use Simec\Par3\Modelo\Iniciativa\Desdobramento\Desdobramento as modeloIniciativaDesdobramento;
use Simec\Par3\Modelo\Iniciativa\Desdobramento\Tipo as modeloIniciativaDesdobramentoTipo;
use Simec\Par3\Modelo\Iniciativa\Planejamento\Planejamento as modeloIniciativaPlanejamento;
use Simec\Par3\Modelo\Iniciativa\Planejamento\Historico as modeloIniciativaPlanejamentoHistorico;
use Simec\Par3\Modelo\Iniciativa\Planejamento\Ciclo as modeloCiclo;
use Simec\Par3\Modelo\Iniciativa\Planejamento\Desdobramentos as modeloIniciativaPlanejamentoDesdobramentos;
use Simec\Par3\Modelo\Instrumento\Unidade as modeloInstrumentoUnidade;
use Simec\Par3\Modelo\Programa\Programa as modeloPrograma;
use Simec\Par3\Modelo\Projeto\Projeto as modeloProjeto;

class IniciativaPlanejamento extends \Simec\AbstractControle
{

    use \Simec\Par3\Controle\Iniciativa\Planejamento\IniciativaPlanejamentoConsultaTrait;

    protected $model;

    protected $modelHistorico;

    public function init()
    {
        $inpid = validarInteiro($_REQUEST['inpid']) ? $_REQUEST['inpid'] : '';
        $this->model = new modeloIniciativaPlanejamento($inpid);
        $this->modelHistorico = new modeloIniciativaPlanejamentoHistorico();
    }

    public function finish()
    {
    }

    public function index()
    {
        $this->consulta();
    }

    private function gerarAbas()
    {
        if (!$_REQUEST['aba']) $_REQUEST['aba'] = 'consulta';

        $url = 'par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=' . $_REQUEST['inuid'];
        $abasGuias = array();
        $abasGuias[] = array("descricao" => "Consulta", "link" => $url . '&aba=consulta');
        $abasGuias[] = array("descricao" => "Cadastro", "link" => $url . '&aba=cadastro');
        $abasGuias[] = array("descricao" => "Síntese  Planejamento", "link" => $url . '&aba=sintese');

        return ($_REQUEST['aba'] == 'analisarPlanejamento') ? false : $this->simec->tab($abasGuias, $url . '&aba=' . $_REQUEST['aba']);

    }

    public function recuperar()
    {
        return $this->model;
    }

    public function recuperaIniciativaProinfancia($obrid){
        return $this->model->recuperaIniciativaProinfancia($obrid);
    }

    public function retornaCategoriaIniciativa()
    {
        return ($this->model->inpid != '') ? $this->model->retornaCategoriaIniciativa() : null;
    }

    private function selectEsfera($arrPost) {
        $uniid = new modeloInstrumentoUnidade($arrPost['inuid']);

        $esfera = array('estuf' => $uniid->estuf, 'dimid' => $arrPost['dimid']);
        $esfera_especifica = ($uniid->itrid == 2) ? array( 'iniesfera' => 'M', 'muncod' => $uniid->muncod) : array('iniesfera' => 'E');

        return array_merge($esfera, $esfera_especifica);;
    }

    public function getIniciativa($arrPost, $consulta = false)
    {
        $iniid = new modeloIniciativa();
        $iniesfera = $this->selectEsfera($arrPost);

        return $iniid->listarSelectDimensoesIniciativas($iniesfera, $consulta);
    }

    public function getIniciativaPorInpAtivo($arrPost, $consulta = false)
    {
        $iniid = new modeloIniciativa();
        $iniesfera = $this->selectEsfera($arrPost);

        return $iniid->listarSelectDimensoesIniciativasInp($iniesfera, $consulta);
    }

    public function getIniciativaDados($arrPost)
    {
        $mIni = new modeloIniciativa();
        $mIni->carregarPorId($arrPost['iniid']);
        $mIniiar = new modeloIniciativaIniciativasAreas();

        $mCic = new modeloCiclo($mIni->cicid);
        $mInta = new modeloIniciativaTiposAtendimento($mIni->intaid);
        $mInto = new modeloIniciativaTiposObjeto($mIni->intoid);

        $mIiprg = new modeloIniciativaIniciativasProgramas();
        $mIiPro = new modeloIniciativaIniciativasProjetos();
        $mImoid = new modeloIniciativaModalidades();
        $mUnidade = new modeloInstrumentoUnidade($arrPost['inuid']);

        $rsIniiar = $mIniiar->recuperarPorIniciativa($mIni->iniid);

        // Recupera as áreas cadastradas para a iniciativa
        $arrAreasIniciativa = array();
        foreach ($rsIniiar as $iniiar) {
            $mIar = new modeloIniciativaAreas($iniiar['iarid']);
            $arrAreasIniciativa[] = $mIar->iarsigla . ' - ' . $mIar->iardsc;
        }

        // Recuperar Programas da Iniciativa
        $arrProgramas = array();
        $rsIiprg = $mIiprg->recuperarPorIniciativa($mIni->iniid);
        foreach ($rsIiprg as $iprg) {
            $mPro = new modeloPrograma($iprg['prgid']);
            $arrProgramas[] = $mPro->prgabreviatura . ' - ' . $mPro->prgdsc;
        }

        // Recuperar Projetos da Iniciativa
        $arrProjetos = array();
        $rsIipro = $mIiPro->recuperarPorIniciativa($mIni->iniid);
        foreach ($rsIipro as $ipro) {
            $mPro = new modeloProjeto($ipro['proid']);
            $sigla = $mPro->prosigla ? $mPro->prosigla . ' - ' : '';
            $arrProjetos[] = $sigla . $mPro->prodsc;
        }

        // Recupera os Anos Selecionados Para a Iniciativa
        $arrAnos = array();
        $mIiano = new modeloIniciativaIniciativasAnos();
        $rsIniiar = $mIiano->recuperarPorIniciativa($mIni->iniid);

        foreach ($rsIniiar as $iniiar) {
            $mIiano = new modeloIniciativaIniciativasAnos($iniiar['iiano']);
            $arrAnos[$mIiano->iiano] = $mIiano->iniano;
        }

        $rsImoid = $mImoid->recuperarPorIniciativa($mIni->iniid);

        $arrEta = array();
        foreach ($rsImoid as $imoid) {
            $mEta = new modeloEnsinoEtapa($imoid['etaid']);
            if (
                $mEta->nivid != 1
                ||
                (
                    ($mUnidade->itrid == '2' && $mEta->etaid == '3') ||
                    ($mUnidade->itrid == '1' && $mEta->etaid == '1')
                )
            ) { // Utilizar somente Educação Básica, de acordo a Andréia Couto Ribeiro - FNDE
                continue;
            }
            $arrEta[$mEta->etaid] = $mEta->etadsc;
        }

        $arrDadosIniciativa = array(
            'cic' => '',
            'anos' => '',
            'into' => '',
            'inta' => '',
            'esfera' => '',
            'iar' => '',
            'prg' => '',
            'pro' => '',
            'eta' => ''
        );
        $arrDadosIniciativa['cic'] = $mCic->cicdsc;
        $arrDadosIniciativa['anos'] = implode(',', $arrAnos);
        $arrDadosIniciativa['into'] = $mInto->intodsc;
        $arrDadosIniciativa['inta'] = $mInta->intadsc;
        $arrDadosIniciativa['esfera'] = $mUnidade->itrid == '2' ? 'Municipal' : 'Estadual';
        $arrDadosIniciativa['iar'] = implode(' , ', $arrAreasIniciativa);
        $arrDadosIniciativa['prg'] = trim(implode(' , ', $arrProgramas));
        $arrDadosIniciativa['pro'] = implode(' , ', $arrProjetos);
        $arrDadosIniciativa['eta'] = $arrEta;
        $arrDadosIniciativa['intaid'] = $mIni->intaid;

        return $arrDadosIniciativa;
    }

    public function getIniciativaModalidades($arrPost, $valor = false)
    {
        $mImoid = new modeloIniciativaModalidades();
        $rsImoid = $mImoid->recuperarPorIniciativa($arrPost['iniid']);

        $arrMod = array();
        foreach ($rsImoid as $imoid) {
            $mMod = new modeloModalidade($imoid['modid']);
            if ($imoid['etaid'] != $arrPost['etaid']) {
                continue;
            }
            $arrMod[$mMod->modid] = $mMod->moddsc;
        }
        if ($valor) {
            return $mMod->moddsc;
        } else {
            return $arrMod;
        }
    }

    /**
     * CONSULTA DESDOBRAMENTOS NA PAGINA DE CONSULTA
     */
    public function getIniciativaDesdobramentoConsulta($arrPost, $multiselect = false)
    {
        $mImoid = new modeloIniciativaModalidades();
        $rsImoid = $mImoid->recuperarPorIniciativa($arrPost['iniid']);
        $arrDes = array();
        foreach ($rsImoid as $imoid) {
            if (($imoid['etaid'] != $arrPost['etaid'] && $arrPost['etaid'] != '') && ($imoid['modid'] != $arrPost['modid'] && $arrPost['modid'] != '')) {
                continue;
            }
            $mDes = new modeloIniciativaDesdobramento($imoid['desid']);
            $mTip = new modeloIniciativaDesdobramentoTipo($mDes->tipid);

            if ($multiselect == true) {
                $arrDes[] = array(
                    'codigo' => $mDes->desid,
                    'descricao' => $mDes->desdsc . ' - ' . $mTip->tipdsc
                );
                continue;
            }
            $arrDes[$mDes->desid] = $mDes->desdsc . ' - ' . $mTip->tipdsc;
        }
        return $arrDes;
    }

    public function getIniciativaDesdobramentos($arrPost, $multiselect = false)
    {
        $mImoid = new modeloIniciativaModalidades();
        $rsImoid = $mImoid->recuperarPorIniciativaModalidade($arrPost);
        $arrDes = array();
        foreach ($rsImoid as $imoid) {
            if (($imoid['etaid'] != $arrPost['etaid'] && $arrPost['etaid'] != '') && ($imoid['modid'] != $arrPost['modid'] && $arrPost['modid'] != '')) {
                continue;
            }
            $mDes = new modeloIniciativaDesdobramento($imoid['desid']);
            $mTip = new modeloIniciativaDesdobramentoTipo($mDes->tipid);
            if ($mDes->desstatus == 'A' && $mDes->dessituacao == 'A') {
                if ($multiselect == true) {
                    $arrDes[] = array(
                        'codigo' => $mDes->desid,
                        'descricao' => $mDes->desdsc . ' - ' . $mTip->tipdsc
                    );
                    continue;
                }
                $arrDes[$mDes->desid] = $mDes->desdsc . ' - ' . $mTip->tipdsc;
            }
        }
        return $arrDes;
    }

    /**
     * Finaliza Planejamento
     * Função que realiza a mudança do estado do doc de planejamento e
     * atualiza o estado documento das iniciativas vinculadas ao planejamento
     *
     * @param array $arrPost
     */
    public function finalizarPanejamento($arrPost)
    {
        $modelSegurancaUsuario = new \Seguranca_Model_Usuario();
        $controllerlInstrumentoUnidade = new \Par3_Controller_InstrumentoUnidade();

        try {
            $mInu = new modeloInstrumentoUnidade($arrPost['inuid']);
            $mDoc = new \Workflow_Model_Documento($mInu->docid);
            $erro = 'erro';


            // Tramita o Instrumento Unidade
            if ($mDoc->esdid == PAR3_ESDID_ELABORACAO) {
                $erro = wf_alterarEstado($mInu->docid, PAR3_AEDID_ENVIA_ANALISE, 'Tramitação Sistema', array(), array()); // Altera status olanejamento

                if ($erro) {
                    $arrIniciativas = $this->model->recuperarTodosEmCadastramento($arrPost['inuid']); //recupera docid iniciativas em cadastramento
                    // Tramita tods as Iniciativas para Enviar Para cancelada
                    foreach ($arrIniciativas as $dociniciativa) { //Em Cadastramento para Canceladas
                        $erro = wf_alterarEstado($dociniciativa['docid'], PAR3_AEDID_CANCELADA, 'Cancelar', array());
                    }
                }
            }

            $usuario = $modelSegurancaUsuario->recuperarPorCPF($_SESSION['usucpf']);
            $nomeunidade = $controllerlInstrumentoUnidade->pegarNomeEntidade($arrPost['inuid']);

            $dadosEmail = array(
                'usunome' => $usuario['usunome'],
                'usucpf' => $usuario['usucpf'],
                'usuemail' => $usuario['usuemail'],
                'assunto' => "SIMEC - PAR Planejamento Finalizado",
                'unidade' => $nomeunidade,
                'data' => date("d/m/Y"),
                'hora' => date("H:i:s"),
            );


            $retornoEmail = $this->enviarEmailPlanejamentoFinalizado($dadosEmail);
            return 1;
        } catch (Simec_Db_Exception $e) {
            return $e;
        }

        return 1;
    }

    /**
     * enviarPlano
     * Função que realiza a mudança do estado do doc de planejamento e
     * atualiza o estado documento das iniciativas vinculadas ao planejamento
     *
     * @param array $arrPost
     */
    public function enviarPlano($arrPost)
    {

        require_once APPRAIZ.'/includes/workflow.php';
        try {
            $mInu = new modeloInstrumentoUnidade($arrPost['inuid']);
            $mDoc = new \Workflow_Model_Documento($mInu->docid);

            $erro = 'erro';

            // Tramita o Instrumento Unidade
            // if($mDoc->esdid == PAR3_ESDID_ELABORACAO){
            // $erro = wf_alterarEstado($mInu->docid,PAR3_AEDID_ENVIA_ANALISE,'Tramitação Sistema', array(),array());
            // }
            // Tramita tods as Iniciativas para Enviar Para Análise de Mérito
            $arrIniciativas = $this->model->recuperarTodos('docid', [
                "inuid = {$arrPost['inuid']}",
                "inpid = {$arrPost['inpid']}"
            ]);

            $sql = "select ed.esdid, ed.esddsc from workflow.documento d
            			inner join workflow.estadodocumento ed on ed.esdid = d.esdid
            		where docid = ".$this->model->docid;
            $arEstato = $this->model->pegaLinha($sql);

            if ($arEstato['esdid'] == PAR3_ESDID_EM_DILIGENCIA) {
                $aedid = 5168;
            } else {
                $aedid = PAR3_AEDID_AGUARDANDO_ANALISE;
            }

            //ver($this->model->docid,'d');
            //  foreach ($arrIniciativas as $dociniciativa) {
            $erro = wf_alterarEstado($this->model->docid, $aedid, 'Aguardando Análise', array('inpid' => $arrPost['inpid'] ));

            $sql = "SELECT ipiano, sum(ipiquantidade) AS ipiquantidade,
                    	(SELECT count(anaid) FROM par3.analise WHERE inpid = ipi.inpid AND anaano = ipi.ipiano AND anastatus = 'A' AND intaid = 1) AS tem_analise
                    FROM par3.iniciativa_planejamento_item_composicao ipi
                    WHERE inpid = {$arrPost['inpid']} AND ipistatus = 'A' GROUP BY ipiano, ipi.inpid";
            $arAnalise = $this->model->carregar($sql);
            $arAnalise = $arAnalise ? $arAnalise : array();

            foreach ($arAnalise as $v) {
                if ((int)$v['ipiquantidade'] > (int)0 && (int)$v['tem_analise'] == (int)0) {
                    $docdsc = "Fluxo de Iniciativas do PAR3 - Análise Planejamento ";
                    $docid_analise = wf_cadastrarDocumento(310, $docdsc, '2070');

                    $sql = "INSERT INTO par3.analise(anaano, docid, anadatacriacao, anastatus, intaid, inpid)
                            VALUES('".$v['ipiano']."',  $docid_analise, now(), 'A', 1, {$arrPost['inpid']})";
                    $this->model->executar($sql);
                    $this->model->commit();
                }
            }

            //  }

//            $arrObras = $this->model->recuperarObrasEmCadastramento($arrPost['inuid'],$arrPost['inpid']);
//
//            foreach ($arrObras as $dociniciativa) { //Em Cadastramento para Canceladas
//                $erro = wf_alterarEstado($dociniciativa['docid'], PAR3_AEDID_CANCELADA, 'Cancelar', array());
//
//            }


            return 1;
        } catch (Simec_Db_Exception $e) {
            return $e;
        }
        return 1;
    }

    public function getDesdobramentos($inpid)
    {
        $mDes = new modeloIniciativaPlanejamentoDesdobramentos();
        $arrDesdobramentos = $mDes->recuperarDesdobramentos($inpid);
        return $arrDesdobramentos;
    }

    public function getDesdobramentosSelecionado($inpid)
    {
        $mDes = new modeloIniciativaPlanejamentoDesdobramentos();
        $arrDesdobramentos = $mDes->recuperarDesdobramentosEscolhida($inpid);
        return $arrDesdobramentos;
    }

    public function salvar($arrPost)
    {
        $arrPlanejamento['inpstatus'] = 'A';
        $arrPlanejamento['dimid'] = $arrPost['dimid'];
        $arrPlanejamento['inpid'] = $arrPost['inpid'];
        $arrPlanejamento['iniid'] = $arrPost['iniid'];
        $arrPlanejamento['inuid'] = $arrPost['inuid'];
        $arrPlanejamento['inpcronogramamesinicial'] = $arrPost['inpcronogramamesinicial'] ? $arrPost['inpcronogramamesinicial'] : null;
        $arrPlanejamento['inpcronogramamesfinal'] = $arrPost['inpcronogramamesfinal'] ? $arrPost['inpcronogramamesfinal'] : null;
        $arrPlanejamento['inpcronogramaano'] = $arrPost['inpcronogramaano'] ? $arrPost['inpcronogramaano'] : null;
        $arrPlanejamento['inpitemcomposicaodetalhamento'] = ($arrPost['inpitemcomposicaodetalhamento'] ? ($arrPost['inpitemcomposicaodetalhamento']) : ' ');
        $arrPlanejamento['nivid'] = 1;
        $arrPlanejamento['etaid'] = $arrPost['etaid'];
        $arrPlanejamento['modid'] = $arrPost['modid'];
        $arrPlanejamento['desid'] = $arrPost['desid'];
        $arrPlanejamento['inpcronogramaanoinicial'] = $arrPost['inpcronogramaanoinicial'] ? $arrPost['inpcronogramaanoinicial'] : null;
        $arrPlanejamento['obrid'] = $arrPost['obrid'] ? $arrPost['obrid'] : null;
        $arrPlanejamento['obrid_par3'] = $arrPost['obrid_par3'] ? $arrPost['obrid_par3'] : null;

        // Valida campos
        $erro = $this->model->validarInput($arrPlanejamento);
        if ($erro) {
            return $erro;
        }

        $mInu = new modeloInstrumentoUnidade($arrPost['inuid']);
        $mDoc = new \Workflow_Model_Documento($mInu->docid);

        if ($mDoc->esdid == 1874) {
            wf_alterarEstado($mInu->docid, 4923, 'Tramitação Sistema', array(), array());
        }

        $acao = IniciativaPlanejamentoHistorico::CREATE;
        // $arrTeste = $this->model->recuperarTodos('inpid', array("inuid = {$arrPost['inuid']}", "dimid = {$arrPost['dimid']}", "iniid = {$arrPost['iniid']}", "etaid = {$arrPost['etaid']}", "modid = {$arrPost['modid']}"));
        // $arrPost['inpid'] = $arrPost['inpid'] ? $arrPost['inpid'] : $arrTeste[0]['inpid'];
        // if($arrPost['inpid']){
        // $arrPlanejamento['inpid'] = $arrPost['inpid'];
        $acao = IniciativaPlanejamentoHistorico::UPDATE;
        // }

        $imod = new modeloIniciativaModalidades();

        // $imoid = $imod->recuperarIniciativaModalidade($arrPost);
        // $arrPlanejamento['imoid'] = $imoid;

        try {
            $arrPlanejamento['docid'] = $this->model->recuperaDocumento();
            $this->model->popularDadosObjeto($arrPlanejamento);
            if ($arrPost['inpid']) {
                $inp = new IniciativaPlanejamento($arrPost['inpid']);
                $arrPlanejamento['etaid'] = $this->model->etaid;
                $arrPlanejamento['modid'] = $this->model->modid;
                $arrPlanejamento['dimid'] = $this->model->dimid;
                $arrPlanejamento['iniid'] = $this->model->iniid;
            }
            $id = $this->model->salvar();
            $this->model->commit();

            if (!$arrPost['inpid']) {
                $mIpd = new modeloIniciativaPlanejamentoDesdobramentos();
                $mIpd->salvarDesdobramentos($id, prepararArraySelectMultiSelecao($arrPlanejamento['desid']));
            }

            $model = new IniciativaPlanejamento($id);

            $this->modelHistorico->gravarHistorico($model, $acao);
            $this->modelHistorico->commit();

            $obModelUnidade = new modeloInstrumentoUnidade($arrPost['inuid']);
            $arrEsdDiag = $obModelUnidade->retornaEstadosPrePlanejamento();
            $mdWorkdlow = new \Workflow_Model_Documento($obModelUnidade->docid);

            if ($mdWorkdlow->esdid == 1874) {
                require_once APPRAIZ . '/includes/workflow.php';
                wf_alterarEstado($obModelUnidade->docid, 5013, 'Tramitação Sistema', array(), array());
            }

            return $id;
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
    }

    public function remover($arrPost)
    {

        $situacao = $this->model->verificarSituacaoIniciativa($arrPost);
        if ($situacao == "sucesso") {
            $arrInp['inpid'] = $arrPost['inpid'];
            $arrInp['inpstatus'] = 'I';
            try {
                $this->model->popularDadosObjeto($arrInp);
                $sucesso = $this->model->salvar();
                $this->model->commit();
                $mInp = new IniciativaPlanejamento($arrPost['inpid']);
                $this->modelHistorico->gravarHistorico($mInp, IniciativaPlanejamentoHistorico::DELETE);
                return $sucesso;
            } catch (Simec_Db_Exception $e) {
                return 'erro';
            }
        } else {
            return $situacao;
        }
    }


    public function listar($arrPost)
    {
        global $disabled;
        $campo = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC' ? 'ASC' : 'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        /**
         * ordenação
         * id =1,qtd = 2,codigo = 3,descricao = 4,tiposobjetos = 5,unidademedida = 6,categoriadespesa = 7,situacao = 8
         */
        switch ($campo) {
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = '3';
                break;
            case 'dimensao':
                $arrQuery['ordenacao']['campo'] = '4';
                break;
            case 'iniciativa':
                $arrQuery['ordenacao']['campo'] = '5';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = '6';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }
        /**
         * ordenação
         * id =1,qtd = 2,codigo = 3,descricao = 4,tiposobjetos = 5,unidademedida = 6,categoriadespesa = 7,situacao = 8
         */
        $sql = $this->model->montarSQLDataGrid($arrQuery); // #
        $disabled = '';
        $mostrarOpcoes = true;
        if (\Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) {
            $disabled = '';
        }
        // Cabeçalho: código,descrição,situção
        $cabecalho = array();
        $esconderColunas = array();
        // $cabecalho = array('Código','Dimensão','Código da Iniciativa','Iniciativa','Situação');
        $cabecalho = array(
            'Código',
            'Dimensão',
            'Iniciativa',
            'Programa',
            'Tipo de Objeto',
            'Etapa',
            'Modalidade',
            'Desdobramentos',
            'Valor',
            'Situação'
        );
        // $esconderColunas = array('id','qtd','nivel','etapa','modalidade','anos','ciclo','tipoobjeto','tipoatendimento','areas','projetos','programas','desdobramentos','cronoinicio','cronofim','cronoano');
        $esconderColunas = array(
            'id',
            'qtd',
            'nivel',
            'anos',
            'ciclo',
            'tipoatendimento',
            'areas',
            'projetos',
            'cronoinicio',
            'cronofim',
            'cronoano',
            'descricao','esdid'
        );

        if ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') {
            // id,qtd,codigo,numeroata,atpobjeto,ano,arquivo,descricaoanexo,estados,vigencia,situação,
            // $esconderColunas = array('id','nivel','etapa','modalidade','anos','ciclo','tipoobjeto','tipoatendimento','areas','projetos','programas','desdobramentos','cronoinicio','cronofim','cronoano');
            // $cabecalho = array('id','Código','Dimensão','Código da Iniciativa','Iniciativa','Situação');
            $cabecalho = array(
                'Código',
                'Dimensão',
                'Iniciativa',
                'Programa',
                'Tipo de Objeto',
                'Etapa',
                'Modalidade',
                'Desdobramentos',
                'Valor',
                'Situação'
            );
            $esconderColunas = array(
                'id',
                'qtd',
                'nivel',
                'anos',
                'ciclo',
                'tipoatendimento',
                'areas',
                'projetos',
                'cronoinicio',
                'cronofim',
                'cronoano',
                'descricao',
                'esdid'
            );

            /*
             * if($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir'){ $esconderColunas = array('id'); $cabecalho = array('QTD','Código','Dimensão', 'Código da Iniciativa','Iniciativa', 'Nível','Etapa','Modalidade','Anos', 'Ciclo','Tipo de Objeto','Tipo de Atendimento', 'Areas Relacionadas','Projetos','Programas', 'Desdobramentos','Cronograma Mês Início','Cronograma Mês Fim','Cronograma Ano','Situação' ); }
             */

            $tipoRelatorio = \Simec_Listagem::RELATORIO_CORRIDO;
            $mostrarOpcoes = false;
        }

        $tipoRelatorio = ($arrPost['requisicao'] == 'xls') ? \Simec_Listagem::RELATORIO_XLS : \Simec_Listagem::RELATORIO_PAGINADO;
        if ($arrPost['requisicao'] == 'imprimir') {
            $tipoRelatorio = \Simec_Listagem::RELATORIO_CORRIDO;
            $mostrarOpcoes = false;
        } // desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') ? \Simec_Listagem::SEM_REGISTROS_MENSAGEM : \Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
        //ver($sql); die;
        $listagem = new \Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('iniciativaplanejamentotable');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->turnOnPesquisator();
        $listagem->setTotalizador(\Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->setTamanhoPagina(50);
        $listagem->addCallbackDeCampo('situacao', 'formata_status');
        $listagem->addCallbackDeCampo('valor', 'formataNumeroMonetarioComSimbolo');
        if ($mostrarOpcoes) {
            // Funcionalidade criada para análise da iniciativa...
            //$listagem->addAcao('list', 'listarAnalise');
            $listagem->addAcao('remove', 'removerIniciativaPlanejamento');
            $listagem->setAcaoComoCondicional('remove', [['campo' => 'esdid', 'valor' => PAR3_ESDID_CADASTRAMENTO, 'op' => 'igual']]);
            $listagem->addAcao('edit', 'editarIniciativaPlanejamento');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }



    public function carregaDadosDimensao($dimcod, $arrDados)
    {

        global $db;

        $cabecalho = array('Código','Ano', 'Iniciativa', 'Tipo de Objeto', 'Programa', 'Situação da Iniciativa', 'Valor Total');
        $esconderColunas = array('codigo');

        $sql = $this->model->montarSqlIniciativasDimensao($dimcod, $arrDados); // #


        $tratamentoListaVazia = \Simec_Listagem::SEM_REGISTROS_MENSAGEM;

        $listagem = new \Simec_Listagem(\Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        $listagem->setCabecalho($cabecalho);
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->addAcao('view', 'vizualizarIniciativa');
        $listagem->addCallbackDeCampo('valor', 'formataNumeroMonetarioComSimbolo');
        $listagem->setTotalizador(\Simec_Listagem::TOTAL_SOMATORIO_COLUNA, 'valor');
        $listagem->render($tratamentoListaVazia);
    }


    public function listarDimensoes($arrPost)
    {


        global $disabled;
        $campo = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC' ? 'ASC' : 'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;

        $sql = $this->model->montarSqlDimensao($arrQuery); // #

        $cabecalho = array('Dimensão', 'Quantidade de Inciativas Planejadas', 'Quantidade de Inciativas Finalizadas', 'Valor Total Finalizado da Dimensão');

        if ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') {
            $sql = $this->model->montarSqlDimensaoImpressao($arrQuery); // #
            $cabecalho = array('Dimensão', 'Quantidade de Inciativas Planejadas', 'Quantidade de Inciativas Finalizadass', 'Valor Total da Dimensão',
                'Iniciativas da Dimensão');
        }

        $disabled = '';
        $mostrarOpcoes = true;
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) {
            $disabled = '';
        }



        $esconderColunas = array('dimcod');

        if ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') {
            $tipoRelatorio = \Simec_Listagem::RELATORIO_CORRIDO;
            $mostrarOpcoes = false;
        }

        $tipoRelatorio = ($arrPost['requisicao'] == 'xls') ? \Simec_Listagem::RELATORIO_XLS : \Simec_Listagem::RELATORIO_PAGINADO;
        if ($arrPost['requisicao'] == 'imprimir') {
            $tipoRelatorio = \Simec_Listagem::RELATORIO_CORRIDO;
            $mostrarOpcoes = false;
        } // desabilitar opções para a impressão

        $tratamentoListaVazia = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') ? \Simec_Listagem::SEM_REGISTROS_MENSAGEM : \Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new \Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('iniciativaplanejamentotable');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(\Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->setTamanhoPagina(50);
        $listagem->addCallbackDeCampo('situacao', 'formata_status');
        $listagem->addCallbackDeCampo('valortitaldimensao', 'formataNumeroMonetarioComSimbolo');
        //  $listagem->addAcao('plus', array('func' => 'carregaDadosDimensao', 'extra-params' => array('dimcod','dimensao')));
        $listagem->addAcao('plus', 'carregaDadosDimensao');

        $listagem->render($tratamentoListaVazia);
    }

    /**
     * ription <esta função é utilizada para validar se uma Iniciativa Descrição já está ligada ao planejamento, pois quando estiver ligada, não poderá ser excluída>
     *
     * @author Leo Kenzley <leo.oliveira@cast.com.br>
     * @param $indid
     * @return array mixed NULL
     */
    public function verificaIniciativaDescricaoEmPlanejamentoById($indid)
    {
        return $this->model->getIniciativaPlanejamentoByIdIniciativaDescricao($indid);
    }

    /**
     * ription <Esta função está sendo utilizada para retringir a duplicadão de dados Dimensão, Iniciativa, Etapa, Modalidade e Desdobramento>
     *
     * @author Leo Kenzley <leo.oliveira@cast.com.br>
     * @param array $array
     * @return int (true = já existe, 0 = não existe)
     */
    public function verificaExistenciaParaSalvar(array $array)
    {
        if (is_array($array)) {
            return $this->model->verificaExistenciaParaSalvar($array);
        }
    }

    /**
     * ription <Esta função está sendo utilizada para verificar a existência de iniciativas compedências de preenchimento da Unidade>
     *
     * @author Hemerson Morais <hemerson.morais@castgroup.com.br>
     * @param array $array
     * @return int quantidade de iniciativa pedentes
     */
    public function verificaExistenciaPendenciasPreenchimento(array $array)
    {
        if (is_array($array)) {
            return $this->model->verificaExistenciaPendenciasPreenchimento($array);
        }
    }

    public function existeIniciativaJaCadastrada($inuid)
    {
        return $this->model->existeIniciativaJaCadastrada($inuid);
    }

    /**
     * ription Retorna lista de situações das obras no par
     *
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @return string
     */
    public function getSqlSituacaoObraPlanejamento()
    {
        return $this->model->getSqlSituacaoObraPlanejamento();
    }

    /**
     * ription Retorna valor total Planejamento
     *
     * @author Hemerson Morais <hemerson.morais@castgroup.com.br>
     * @return float
     */
    public function getValorTotalPlanejamento($arrPost)
    {
        return $this->model->getValorTotalPlanejamento($arrPost);
    }


    /**
     * Envia e-mail para destino após o planejamento se finalizado
     *
     * @author Hemerson Morais <hemerson.morais@castgroup.com.br>
     * @return string
     */
    public function enviarEmailPlanejamentoFinalizado($dadosEmail)
    {
        $strAssunto = $dadosEmail['assunto'];
        $strEmailTo = $dadosEmail['usuemail'];

        $strMensagem =
            "
                <pre align=\"center\" style=\"text-align: justify;\"  >

               Prezado(a) Sr(a). {$dadosEmail['usunome']},
               
               O Planejamento do Município: {$dadosEmail['unidade']} foi finalizado por: {$dadosEmail['usucpf']} - {$dadosEmail['usunome']} no dia {$dadosEmail['data']} ás {$dadosEmail['hora']}. O mesmo encontra-se em aguardo da Análise de Mérito do FNDE.

                Esta é uma mensagem automática e não é necessária nenhuma resposta ou confirmação.

                <b> Ministério da Educação </b>

                </pre>";

        $remetente = array("nome" => "SIMEC", "email" => "noreply@mec.gov.br");

        $strMensagem = html_entity_decode($strMensagem);

        $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);

        return $retorno;
    }

    public function pegaTipoAtendimentoAnalise($inpid)
    {
        $sql = "SELECT distinct i.intaid, ta.intadsc
		    	FROM par3.iniciativa_planejamento ip
			    	inner join par3.iniciativa i on i.iniid = ip.iniid
			    	inner join par3.iniciativa_tipos_atendimento ta on ta.intaid = i.intaid
		    	WHERE ip.inpid = $inpid";

        $arTipo = $this->model->pegaLinha($sql);
        return $arTipo;
    }

    public function pegaTipoObjetoAnalise($inpid)
    {
        $sql = "SELECT distinct ito.intoid, ito.intodsc
		    	FROM par3.iniciativa_planejamento ip
			    	inner join par3.iniciativa i on i.iniid = ip.iniid
                    inner join par3.iniciativa_tipos_objeto ito on ito.intoid = i.intoid
		    	WHERE ip.inpid = $inpid";

        $arTipo = $this->model->pegaLinha($sql);
        return $arTipo;
    }

    public function getEstadosIniciativas()
    {
//        ver($this->model->getEstadosIniciativas()); die;
        return $this->model->getEstadosIniciativas();
    }

    public function getListaArquivosPlanejamento($inpid)
    {
        $quantidadeMaxima = 5;
        $arrPermissao = array(PAR3_ESDID_CADASTRAMENTO, PAR3_ESDID_EM_DILIGENCIA);

        $arquivos = $this->model->sqlListaDocumentoPlanejamento($inpid);

        if ($_REQUEST['aba'] == 'cadastro') {
            if (!in_array($arquivos[0]['esdid'], $arrPermissao)) {
                $btnAnexarDocumento = '<div style="width: 150px;" data-toggle="popover" data-content="Planejamento em processo de análise" 
                                         data-placement="right" data-trigger="hover" data-html="true">
                                     <a class="btn btn-success btn-large" disabled>Anexar Documento</a>
                                   </div>';
            } else {
                if (count($arquivos) >= $quantidadeMaxima) {
                    $btnAnexarDocumento = '<div style="width: 150px;" data-toggle="popover" data-content="Quantidade máxima de arquivos atingida" 
                                         data-placement="right" data-trigger="hover" data-html="true">
                                     <a class="btn btn-success btn-large" disabled>Anexar Documento</a>
                                   </div>';
                } else {
                    $btnAnexarDocumento = '<a class="btn btn-success btn-large" onclick="anexarDocumentoPlanejamento()">Anexar Documento</a>';
                }
            }

            $btnAnexarDocumento .= '<div style="clear:both"></div>';

            echo $btnAnexarDocumento;
        }

        // $arquivos sempre vai ter um resultado, por causa do esdid do planejamento.
        // Quando não houver arquivos anexados, deverá ser um array vazio para que a lista seja apresentada corretamente.
        if (!$arquivos[0]['arqid']) {
            $arquivos = array();
        }

        $cabecalho = array('Arquivo', 'Descrição', 'Data');

        $listagem = new \Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setDados($arquivos);
        $listagem->esconderColunas('esdid');
        $listagem->addCallbackDeCampo('situacao', 'formata_status');
        $listagem->addCallbackDeCampo('valor', 'formataNumeroMonetarioComSimbolo');
        $listagem->addAcao('download', ['func' => 'downloadDocumentoPlanejamento', 'titulo' => 'Download documento','cor' => 'success']);
        if ($_REQUEST['aba'] == 'cadastro') {
            $listagem->addAcao('remove', 'removerDocumentoPlanejamento');
        }
        $listagem->setAcaoComoCondicional('remove', [[
            'campo' => 'esdid', 'valor' => $arrPermissao, 'op' => 'contido'
        ]]);
        $listagem->render(\Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    /**
     * @param $dados
     * @return string 'emptyfile' -> erro de arquivo vazio
     *         string 'maxsize' -> arquivo ultrapassa 5mb
     *         string 'error' -> erro ao salvar arquivo
     *         no caso de sucesso, a função retorna a lista de arquivos para ser atualizada pelo javascript
     */
    public function salvarDocumentoPlanejamento($dados)
    {
        $tamanhoMax = 5 * 1024 * 1024;
        $inpid = $_GET['inpid'];
        $ipaid = 0;

        if ($_FILES['documentoPlanejamento']['error'] == UPLOAD_ERR_NO_FILE) {
            echo 'emptyfile';
            die();
        }

        if ($_FILES['documentoPlanejamento']['name']) {
            if ($_FILES['documentoPlanejamento']['size'] > $tamanhoMax) {
                echo 'maxsize';
                die();
            }

            $_FILES['documentoPlanejamento']['name'] = remove_acentos($_FILES['documentoPlanejamento']['name']);

            $file = new FilesSimec("iniciativa_planejamento_arquivos", [], 'par3');

            $file->setUpload('PAR3 - Documentos Iniciativa Planejamento', 'documentoPlanejamento', false);
            $arqid = $file->getIdArquivo();

            $ipaid = $this->model->salvarDocumentoPlanejamento($inpid, $arqid, $dados['ipadsc']);
        }
        if ($ipaid > 0) {
            $this->getListaArquivosPlanejamento($inpid);
        } else {
            echo 'error';
        }
        exit;
    }

    public function removerDocumentoPlanejamento($arqid)
    {

        $inpid = $_GET['inpid'];

        try {
            $this->model->removerDocumentoPlanejamento($arqid);

            //atualiza lista de documentos
            $this->getListaArquivosPlanejamento($inpid);
        } catch (Simec_Db_Exception $e) {
            echo 'error';
        }
        exit;
    }

    public function getIniciativaPesquisaIndicadorSelect($iniid = null)
    {
        if($iniid){
            $andIniid = " AND iniid = {$iniid} ";
        }
        $sql = "
                SELECT DISTINCT ini.iniid as codigo,
                        ini.iniid ||' - '|| idesc.inddsc as descricao
                        
                FROM par3.iniciativa_planejamento ip
                    INNER JOIN par3.analise a ON a.inpid = ip.inpid   
                        AND a.anastatus = 'A'
                    INNER JOIN par3.instrumentounidade iu ON iu.inuid = ip.inuid	
                    INNER JOIN par3.iniciativa ini ON ini.iniid = ip.iniid
                    INNER JOIN par3.iniciativa_descricao idesc ON idesc.indid = ini.indid			
                WHERE ip.inpstatus = 'A'::bpchar
                {$andIniid}
                GROUP BY
                    idesc.inddsc, ini.iniid
                ORDER BY ini.iniid ";
        return $sql;
    }

    public function getDimensaoIniciativas($inuid)
    {

        return $this->model->getDimensaoIniciativas($inuid);
    }

    public function getDimensaoIniciativasPlanejamento($inuid, $dimid)
    {
        return $this->model->getDimensaoIniciativasPlanejamento($inuid, $dimid);
    }

    public function getDadosObraProinfancia($obrid){
        include_once APPRAIZ . 'obras2/classes/modelo/Obras.class.inc';
        require_once APPRAIZ . 'includes/library/simec/AcessoRapido/core/Filtro.php';
        require_once APPRAIZ . 'includes/library/simec/AcessoRapido/filtro/obras2/obra/controle/Obra.php';

        $obras = new Obras();
        $cabecalho = array(
            "ID",
            "ID Pré-obra",
            "Nº Processo",
            "Nº / Ano do termo / convênio",
            "Obra",
            "Unidade implantadora",
            "Município / UF",
            "Data de início da execução",
            "Situação da Obra",
            "Última vistoria instituição",
            "% executado empresa",
            "Tipologia"
        );

        $dados = $obras->acessoRapidoPegarListaDetalheObra(['obrid' => $obrid]);
        $listagem = new \Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($dados);
        $listagem->addCallbackDeCampo('situacao', 'AcessoRapido\filtro\obras2\obra\controle\Obra::tratarSituacaoObra');
        $listagem->addCallbackDeCampo('dtvistoria', '\AcessoRapido\filtro\obras2\obra\controle\Obra::tratarDataUltimaVistoria');
        $listagem->setId('listaDetalheDaObra');
        $listagem->esconderColunas(['ultima_atualizacao', 'strid']);
        $listagem->render(\Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }
}
