<?php
/**
 * Trait para salvar os dados do serviço no banco de dados.
 * @version $Id: Salvarservico.php 136637 2018-01-15 23:58:56Z maykelbraz $
 */

trait Spo_Job_Trait_Salvarservico
{
    /**
     * @param $params
     *
     * @throws Exception
     */
    protected function carregarDados()
    {
        if (empty($this->params['servicos'])) {
            throw new Exception('Nenhum serviço selecionado para importação.');
        }

        try {
            foreach ($this->params['servicos'] as $servico) {
                $this->persistirDados(
                    $this->obterDados($servico),
                    $servico
                );
            }
        } catch (Exception $e) {
            $this->rollback();
            throw $e;
        }
    }

    /**
     * Deleta informações da tabela temporaria e inseri novos registros
     *
     * @param $resultados
     * @param $servico
     */
    private function persistirDados($resultados, $servico)
    {
        if ($resultados) {
            $this->setTable($servico['tabela']);
            $count = $this->jobInsert($resultados, true);

            echo "{$count} {$servico['nome']} importados(as)...<br />";
            $this->salvarOutput();

            return;
        }

        echo "0 {$servico['nome']} importados(as)...<br />";
    }

    /**
     * @param $informacoes
     *
     * @return mixed
     * @throws Exception
     */
    private function obterDados($servico)
    {
        try {

            $retornoSoap = $this->wsqualitativo->$servico['servico'](
                $this->parametrosWs($servico)
            );

            if ($retornoSoap instanceof SoapFault) {
                throw new Exception($retornoSoap);
            }

            $retornoSoap = $retornoSoap->return;

            if ($retornoSoap->mensagensErro) {
                throw new Exception($retornoSoap->mensagensErro);
            }

        } catch (Exception $ex) {
            throw $this->gerarXml(
                $servico['metodo'],
                $retornoSoap,
                $this->params['log'],
                $ex
            );
        }

        return $retornoSoap->$servico['dto'];
    }
}
