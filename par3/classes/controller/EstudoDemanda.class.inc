<?php
class Par3_Controller_EstudoDemanda extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}


    public function retornaCampoExterno($perid, $qrpid, $arrDados)
    {
        if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
            $arrEstudoDemandaEstimativa = array(645, 655, 718, 730, 680, 698, 542, 552, 615, 627, 577, 595);
            $arrEstudoDemandaOrigem     = array(646,719,681,616,578,543);
            $arrEstudoDemandaModelo     = array(647,720,682,617,579,544);
        }else{
            $arrEstudoDemandaEstimativa = array(5788,5913,5925,5937,5874,5892,5807,5817,5836,5848,5771,5789);
            $arrEstudoDemandaOrigem     = array(5904,5926,5875,5808,5837,5772);
            $arrEstudoDemandaModelo     = array(5905,5927,5876,5809,5838,5773);
        }

        if (in_array($perid, $arrEstudoDemandaOrigem)) {
            $htm = $this->retornaOrigemDemanda($perid, $qrpid);
        }

        if (in_array($perid, $arrEstudoDemandaEstimativa)) {
            $htm = $this->retornaPerguntaEstimativa($perid, $qrpid);
        }

        return $htm;
    }

    public function retornaOrigemDemanda($perid, $qrpid)
    {
        global $simec;

        $mdObra_x_Questionario = new Par3_Model_ObraXQuestionario();
        $obrid = $mdObra_x_Questionario->recuperarTodos('obrid', array("qrpid = $qrpid"));

        $mdObra = new Par3_Model_Obra($obrid);
        $mdUnidade = new Par3_Model_InstrumentoUnidade($mdObra->inuid);

        ob_start();//define onde o buffer deve começar a processar
        include_once APPRAIZ.'par3/modulos/principal/planoTrabalho/obras/estudo_demanda/origemDemanda.php';
        $htm = ob_get_contents();
        if($htm){
            ob_end_clean();//finaliza o buffer
        }

        return $htm;
    }

    public function retornaPerguntaEstimativa($perid, $qrpid)
    {
        $model = new Par3_Model_EstudoDemanda();
        $qtd = $model->buscaEstimativaAlunos($perid, $qrpid);
        $htm = "<h5>
                    Com base na quantidade de salas e na recomendação de atendimento do Ministério da Educação,
                    a quantidade estimada de alunos a serem atendidos com a nova construção é de: $qtd
                </h5>";

        return $htm;
    }

}//end Class
?>