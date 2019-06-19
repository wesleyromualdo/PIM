<?php

class Spo_Job_Cargaug extends Simec_Job_Abstract {

    private $params;

    protected function init() {
        $this->params = $this->loadParams();
    }

    public function getName() {
        return 'Processando dados das unidades gestoras';
    }

    protected function echoOutput($mensagem){
        echo $mensagem."<br>";
        $this->salvarOutput();
    }

    protected function execute() {
        try {
            $dados = (new Spo_Model_Cargaug())->recuperarTodos('*');

            foreach ($dados as $dado) {
                $ug = new Public_Model_Unidadegestora();
                if ($ug->exists($dado['ungcod'])) {
                    $ug->carregarPorId($dado['ungcod']);
                    $ug->popularDadosObjeto($dado);
                    $ug->salvar();
                    if (!$ug->commit()) {
                        throw new Exception('Erro ao alterar a unidade gestora');
                    }
                    $this->echoOutput('Unidade Gestora "'.$dado['ungdsc'].'" foi alterada com sucesso.');
                } else {
                    $ug->popularDadosObjeto($dado);
                    $ug->inserir([], true);
                    if (!$ug->commit()) {
                        throw new Exception('Erro ao inserir a unidade gestora');
                    }
                    $this->echoOutput('Unidade Gestora "'.$dado['ungdsc'].'" foi incluída com sucesso!');
                }
            }
            $this->echoOutput('Carga Finalizada');


            $this->fm->addMensagem('Importação realizada com sucesso');

        } catch (Exception $e) {
            throw $e;
        }
    }

    protected function shutdown() {

    }
}