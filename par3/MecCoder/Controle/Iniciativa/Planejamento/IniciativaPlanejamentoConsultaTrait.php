<?php

/**
 * Trait PlanejamentoIniciativaConsultaTrait
 * @package Simec\Par3\Controle
 * Especificação da aba de Consulta do Planejamento de Iniciativas do PAR3
 */
namespace Simec\Par3\Controle\Iniciativa\Planejamento;

// Modelos
use Simec\Par3\Modelo\Dimensao as modeloDimensao;
use Simec\Par3\Modelo\Instrumento\Unidade as modeloInstrumentoUnidade;
use Simec\Par3\Modelo\Iniciativa\Planejamento\Ciclo as modeloCiclo;
use Simec\Par3\Modelo\Iniciativa\Planejamento\Planejamento as modeloIniciativaPlanejamento;

trait IniciativaPlanejamentoConsultaTrait
{

    public function consulta()
    {
        $modeloInstrumentoUnidade = new modeloInstrumentoUnidade($_REQUEST['inuid']);

        $permissoes = $modeloInstrumentoUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

        $modeloCiclo = new modeloCiclo();
        $caminho = 'par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=' . $_REQUEST['inuid'];
        $controleInstrumentoUnidade = new \Par3_Controller_InstrumentoUnidade();

        if (!$permissoes || !$permissoes['booVisualizar']) {
            simec_redirecionar('par3.php?modulo=inicio&acao=C', 'error', 'Acesso negado.');
        }

        require_once APPRAIZ . '/includes/workflow.php';
        $arrEsdDiag = $modeloInstrumentoUnidade->retornaEstadosPrePlanejamento();
        $modeloWorkflowDocumento = new \Workflow_Model_Documento($modeloInstrumentoUnidade->dados->docid);


        if (in_array($modeloWorkflowDocumento->esdid, $arrEsdDiag) || $modeloWorkflowDocumento->esdid == '') {
            $caminho = "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid={$_REQUEST['inuid']}&resumo=1";
            simec_redirecionar($caminho, 'success', 'Para iniciar o planejamento primeiro <b>FINALIZE O DIAGNÓSTICO.</b>');
            die();
        }

        if ($_REQUEST['aba'] != 'analisarPlanejamento') {

            if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
                $this->simec = $this->importSimec();
            }
        }

        $itrid = $controleInstrumentoUnidade->pegarItrid($_GET['inuid']);

//        $this->js('/zimec/public/temas/simec/js/plugins/viewjs/js/bootstrap-tour.min.js');
        $this->js('js/tour/indicadoresQualitativos.js');
        $this->js('/zimec/public/temas/simec/js/plugins/viewjs/view.js');

        $this->toJs('ciclo', (new modeloCiclo())->selectCiclos());
        $this->toJs('situacao', (new modeloIniciativaPlanejamento())->getEstadosIniciativas());
        $this->toJs('dimensao', (new modeloDimensao())->listaDimensoes(array('itrid' => $itrid)));

        $this->toView('caminho', $caminho);
        $this->toView('controleIniciativaPlanejamento', $this);
        $this->toView('controleInstrumentoUnidade', $controleInstrumentoUnidade);
        $this->toView('modeloDimensao', $modeloDimensao);
        $this->toView('ciclos', $modeloCiclo->pegarSQLSelectCombo());
        $this->toView('insereAbas', $this->gerarAbas());
        $this->toView('controleUnidadeNome', $controleInstrumentoUnidade->pegarNomeEntidade($_REQUEST['inuid']));
        $this->toView('simec', $this->simec);

        $this->showHtml();
    }

    public function pesquisar()
    {
        echo $this->listar($_REQUEST);
        die;
    }

    public function xls()
    {
        $controleInstrumentoUnidade = new \Par3_Controller_InstrumentoUnidade();
        $nomeInstrumento = $controleInstrumentoUnidade->pegarNomeEntidade($_REQUEST['inuid']);
        $this->listar($_REQUEST);
        header("Content-Disposition: attachment; filename=Relatorio_Planejamento_Iniciativas_" . $nomeInstrumento . ".xls");
        die;
    }

    public function imprimir()
    {
        $this->listar($_REQUEST);
        die();
    }

    public function remover()
    {
        echo $this->remover($_REQUEST);
        die;
    }


}