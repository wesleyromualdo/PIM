<?php

class Par3_Controller_MaisAlfabetizacao extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Agrupador($_GET['agrid']);

        $this->modelProadesao                     = new Par3_Model_Proadesao();
        $this->modelAdesaoPrograma                = new Par3_Model_AdesaoPrograma();
        $this->modelInstrumentoUnidadeEntidade    = new Par3_Model_InstrumentoUnidadeEntidade();

        $this->modelInstrumentoUnidade            = new Par3_Model_InstrumentoUnidade($this->_GET['inuid']);
        $this->modalEscolasPrioritarias           = new Par3_Model_EscolasPrioritarias();
        $this->modalInstrumentoUnidade            = new Par3_Model_InstrumentoUnidade($this->_GET['inuid']);
    }

    public function validaSeExisteAdesao($adpano_ciclo){
        $proadesaoDados = $this->modelProadesao->pegarAdesaoPorPrograma($_GET['prgid'], $adpano_ciclo);
        $pfaid = $proadesaoDados['pfaid'];

        if (!$pfaid) {
            $_SESSION['par3']['adpano_ciclo'] = '2018';
            echo "
        <script>
            alert('Adesão não cadastrada. Contate o Administrador do Sistema.');
            window.location.href = 'par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=$inuid';
        </script>";
            die;
        }
    }

    public function salvarCoordenador($objCoordenador, $inuid, $adpid, $docidInserido, $esdid, $prgid){
        // REcebe variáveis do coordenador direto do form
        $cmaid = $_POST['cmaid'];
        $cmacpf = str_replace(array('.', '-', '/'), '', $_REQUEST['cmacpf']);
        $cmanome = $_POST['cmanome'];
        $cmaemail = $_POST['cmaemail'];
        $cmatelefone = str_replace(array('(', '-', ')', ' '), '', $_REQUEST['cmatelefone']);;

        // Verifica mensagem de erro
        $msgAlert = "";
        // Trata nome (exceção para erro no webservice)
        if ($cmanome == "") {
            $msgAlert = "O campo 'Nome' do Coordenador é obrigatório, caso não esteja sendo carregado verifique o campo CPF";
            simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid=' . $inuid . "&prgid=" . $prgid . "&abaatual=3", 'error', $msgAlert);
            die();
        }
        // Tra email
        if (!$objCoordenador->validaEmailCoordenador($cmaemail)) {
            $msgAlert = "Email do Coordenador inválido.";
            simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid=' . $inuid . "&prgid=" . $prgid . "&abaatual=3", 'error', $msgAlert);
            die();
        }
        // Verifica se é edição ou inserção
        if ($cmaid) {
            $dadosCoordenador['cmaid'] = $cmaid;
        }
        // Carrega variáveis para salvar
        $dadosCoordenador['inuid'] = "{$inuid}";
        $dadosCoordenador['adpid'] = "{$adpid}";
        $dadosCoordenador['cmacpf'] = "{$cmacpf}";
        $dadosCoordenador['cmanome'] = "{$cmanome}";
        $dadosCoordenador['cmaemail'] = "{$cmaemail}";
        $dadosCoordenador['cmatelefone'] = "{$cmatelefone}";
        $dadosCoordenador['cmastatus'] = "A";

        // Salva requisição
        $objCoordenador->popularDadosObjeto($dadosCoordenador);
        $objCoordenador->salvar();

        if ($objCoordenador->commit()) {
            if ($docidInserido) {
                if (($esdid == WF_ESDID_NAO_INICIADO_MAISALFABETIZACAO) || (!$esdid)) {
                    wf_alterarEstado($docidInserido, WF_AEDID_MAISALFABETIZACAO_NAOINICIADO_CADASTRAMENTO, "Aceitou o termo", array(), array());
                }
            }
            $msgAlert = "Coordenador Salvo com Sucesso";
            simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid=' . $inuid . "&prgid=" . $prgid . "&abaatual=3", 'success', $msgAlert);
            die();
        }
    }

    public function liberarEnvio($objEscolasMaisAlfabetizacao,$objCoordenador, $objAdmEscolas, $adpid, $inuid, $adpano_ciclo, $aceite){
        // Verifica se existem escolas no grupo 1 e se existem adesao
        $arr['inuid'] = $inuid;
        $arr['adpano_ciclo'] = $adpano_ciclo;

        $arr['carga_horaria'] = '10';
        $existeEscolaNoGrupo1 = $objEscolasMaisAlfabetizacao->existeEscolaNoGrupo($arr);

        $arr['carga_horaria'] = '5';
        $existeEscolaNoGrupo2 = $objEscolasMaisAlfabetizacao->existeEscolaNoGrupo($arr);

        $restricaoMinimaGrupo1 = false;
        if ($existeEscolaNoGrupo1) {
            $existeAdesao1 = $objAdmEscolas->existeEscolas($inuid, $adpid, '10');
            if (!$existeAdesao1) {
                $restricaoMinimaGrupo1 = true;
            }
        }

        $restricaoMinimaGrupo2 = false;
        if ($existeEscolaNoGrupo1) {
            $existeAdesao2 = $objAdmEscolas->existeEscolas($inuid, $adpid, '5');
            if (!$existeAdesao2) {
                $restricaoMinimaGrupo2 = true;
            }
        }

        if (
            ($aceite['adpresposta'] == "S") &&
            ($objCoordenador->existeCoordenador($inuid, $adpid)) &&
            ($objAdmEscolas->existeEscolas($inuid, $adpid)) &&
            (!$restricaoMinimaGrupo1) && (!$restricaoMinimaGrupo2)
        ) {
            return true;
        }
        return false;
    }

}