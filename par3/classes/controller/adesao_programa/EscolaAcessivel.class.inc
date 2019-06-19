<?php

class Par3_Controller_EscolaAcessivel extends Controle
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
//
//    public function liberarEnvio($objEscolasEscolaAcessivel, $objAdmEscolas, $adpid, $inuid, $adpano_ciclo, $aceite){
//        // Verifica se existem escolas no grupo 1 e se existem adesao
//        $arr['inuid'] = $inuid;
//        $arr['adpano_ciclo'] = $adpano_ciclo;
//
////        $arr['carga_horaria'] = '10';
//        $existeEscolaNoGrupo1 = $objEscolasEscolaAcessivel->existeEscolaNoGrupo($arr);
//
////        $arr['carga_horaria'] = '5';
//        $existeEscolaNoGrupo2 = $objEscolasEscolaAcessivel->existeEscolaNoGrupo($arr);
//
//        $restricaoMinimaGrupo1 = false;
//        if ($existeEscolaNoGrupo1) {
//            $existeAdesao1 = $objAdmEscolas->existeEscolas($inuid, $adpid);
//            if (!$existeAdesao1) {
//                $restricaoMinimaGrupo1 = true;
//            }
//        }
//
//        $restricaoMinimaGrupo2 = false;
//        if ($existeEscolaNoGrupo1) {
//            $existeAdesao2 = $objAdmEscolas->existeEscolas($inuid, $adpid);
//            if (!$existeAdesao2) {
//                $restricaoMinimaGrupo2 = true;
//            }
//        }
//
//        if (
//            ($aceite['adpresposta'] == "S") &&
//            //($objCoordenador->existeCoordenador($inuid, $adpid)) &&
//            ($objAdmEscolas->existeEscolas($inuid, $adpid)) &&
//            (!$restricaoMinimaGrupo1) && (!$restricaoMinimaGrupo2)
//        ) {
//            return true;
//        }
//        return false;
//    }
    

}