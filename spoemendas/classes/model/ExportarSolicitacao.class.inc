<?php

/**
 * Classe responsável por roteamento e regras de negocio
 * @package classes/model
 * @filesource
 */
class Spoemendas_Model_ExportarSolicitacao
{

    /**
     * Método responsável por gerar listagem de emendas liberadas ou pendentes
     * @author Victor Eduardo Barreto
     * @filesource
     * @throw Mensagem de erro
     * @param string $aba Aba que está sendo mostrada
     * @return mixed Listagem de emendas liberadas ou pendentes
     */
    public function listagem($aba)
    {
        $sql = 'QUERY';

            // $stmt = sprintf($sql, $_SESSION['exercicio'], $_SESSION['sisid']);

            // mock
            $stmt = [
            ['id' => 1, 'ano' => '2017', 'mes' => 'janeiro', 'dataini' => '10/04/2017', 'pendente' => '3'],
            ['id' => 1, 'ano' => '2017', 'mes' => 'janeiro', 'dataini' => '10/04/2017', 'pendente' => '3'],
            ['id' => 1, 'ano' => '2017', 'mes' => 'janeiro', 'dataini' => '10/04/2017', 'pendente' => '3'],
            ['id' => 1, 'ano' => '2017', 'mes' => 'janeiro', 'dataini' => '10/04/2017', 'pendente' => '3'],
            ['id' => 1, 'ano' => '2017', 'mes' => 'janeiro', 'dataini' => '10/04/2017', 'pendente' => '3'],
            ];

        // $cabecalho = ['Ano', 'Mês de Solicitação', 'Periodo de Solicitação'];
            $coluna = 'Recebidos';

        if ($aba == 'ped') {
            $coluna = 'Pendentes de Envio';
            // $coluna = ['Ano', 'Mês de Solicitação', 'Periodo de Solicitação', 'Pendentes de Envio'];
        }

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho([['Ano', 'Mês de Solicitação', 'Periodo de Solicitação', $coluna]]);
            // $listagem->setQuery($stmt);
            $listagem->setDados($stmt);
        $listagem->setAcoes(['view' => 'xls']);
        $listagem->setAcoes(['info' => 'info']);
                // ->addCallbackDeCampo('esdid', 'formatarEstadoLote')
                // ->addCallbackDeCampo('loteid', 'formatarNumeroLote')
                // ->esconderColuna('esddsc');
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        return $listagem->render();
    }
}
