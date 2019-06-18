<?php
class Par3_Controller_NovoEnsinoMedio extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_EscolasNovoEnsinoMedio($_GET['agrid']);

        $this->modelProadesao                     = new Par3_Model_Proadesao();
        $this->modelAdesaoPrograma                = new Par3_Model_AdesaoPrograma();
        $this->modelInstrumentoUnidadeEntidade    = new Par3_Model_InstrumentoUnidadeEntidade();

        $this->modelInstrumentoUnidade            = new Par3_Model_InstrumentoUnidade($this->_GET['inuid']);
    }

    public function validaSeExisteAdesao($adpano_ciclo)
    {
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
    
    public function vincularEscolas($dados)
    {
        if($this->model->verificaEscolaVinculada($dados)){
            $this->model->excluirEscolaVinculada($dados);
        }else{
            $this->model->vincularEscola($dados);
        }
    }
    
    public function buscaQTDEscolasSelecionadas($dadosSobreEscola)
    {
        $dadosSobreEscola['selecionadas']   = 'S';
        $dadosSobreEscola['situacao']       = '2';
        $selecionadas = $this->model->buscaQTDEscolas($dadosSobreEscola);
        
        $dadosSobreEscola['selecionadas']   = 'N';
        $dadosSobreEscola['situacao']       = '1';
        $sorteadas = $this->model->buscaQTDEscolas($dadosSobreEscola);
        
        return count($selecionadas)+count($sorteadas);
    }
    
    public function enviarSelecao($dados)
    {
        $this->model->enviarEscolas($dados);
        
        $result = wf_alterarEstado($dados['docid'], WF_AEDID_PNEM_CADASTRAMENTO_ENVIADO, "Aceitou o termo", array(), array());
        
        simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid=' . $dados['inuid'] . "&prgid=" . $dados['prgid'], 'success', 'Escolas enviadas com sucesso');
    }
    
    public function listaEscolas($dados)
    {
        $dados['somenteLeitura'] = $dados['somenteLeitura'] === 'false' ? false : $dados['somenteLeitura'];
        
        $cabecalho = array();
        if(!$dados['somenteLeitura']) $cabecalho[] = "Ação";
            
        $cabecalho[] = "Estado";
        $cabecalho[] = "Código do Município";
        $cabecalho[] = "Município";
        $cabecalho[] = "Código INEP";
        $cabecalho[] = "Nome da Escola";
        
        switch($dados['situacao']){
            case 1:
                $cabecalho[] = "Tipo de Escola Sorteada";
                break;
            case 2:
                $cabecalho[] = "Carga Horária Anual";
                $cabecalho[] = "Total de Matrículas";
                $cabecalho[] = "INSE";
                $cabecalho[] = "Tipo da Escola";
                break;
            case 3:
                $cabecalho[] = "Motivo";
                break;
        }
        
        $sql = $this->model->buscaSQLListaEscolas($dados);

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->turnOffForm();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }
}