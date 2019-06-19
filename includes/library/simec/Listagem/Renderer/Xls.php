<?php
/**
 * Renderizador de listagens XLS.
 *
 * @version $Id: Xls.php 145663 2018-11-07 12:33:49Z calixtomodesto $
 */
/**
 * Renderizador base.
 * @see Simec_Listagem_Renderer_Abstract
 */
require_once(dirname(__FILE__) . '/Abstract.php');

class Simec_Listagem_Renderer_Xls extends Simec_Listagem_Renderer_Abstract {
    public static $filename = 'extrato.xls';

    /**
     * Imprime o código XLS da listagem.
     */
    public function render() {
        $filename = self::$filename;
        header("Content-Type:   application/vnd.ms-excel; charset=utf-8");
        header("Content-type:   application/x-msexcel; charset=utf-8");
        header("Content-Disposition: attachment; filename=$filename");
        header("Expires: 0");
        header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
        header("Cache-Control: private", false);
        echo '<table class="table table-striped table-bordered table-hover" id="tb_render" border=1 >';
        echo $this->renderCabecalho();
        $this->renderDados();
        echo $this->renderRodape();
        echo '</table>';
    }

    public function renderTitulo() {

    }

    protected function renderCabecalho() {

        // -- Se não houver um cabeçalho definido, pula a criação do cabeçaho
        if (is_null($this->getCabecalho('xls'))) {
            return '';
        }

        if (array('auto') == $this->getCabecalho('xls')) {
            $todosCampos = array_keys(current($this->dados));
            $this->config->setCabecalho(
                array_diff($todosCampos, $this->config->getColunasOcultas())
            );
        }

        // -- Cabecalho de um único nível
        if (1 == $nivelCabecalho) {
            $colunas = '';
            foreach ($this->getCabecalho('xls') as $itemCabecalho) {
                if ('&nbsp' == $itemCabecalho) {
                    $colunas .= '<th class="text-center" colspan="' . $spanPrimeiraColuna . '">' . $itemCabecalho . '</th>';
                } else {
                    $colunas .= '<th class="text-center">' . $itemCabecalho . '</th>';
                }
            }
            return <<<HEADER
                                <thead>
                                    <tr>{$colunas}</tr>
                                </thead>
HEADER;
        }
        // -- Dois níveis de cabecalho
        $cabecalho = '<thead><tr>';

        // -- Primeiro nível
        foreach ($this->getCabecalho('xls') as $key => $itemCabecalho) {
            if (is_array($itemCabecalho)) {
                $colspan = count($itemCabecalho);
                $cabecalho .= <<<HTML
                                <th class="text-center" colspan="{$colspan}">{$key}</th>
HTML;
            } else {
                if ('&nbsp' == $itemCabecalho) {
                    if (!$this->renderPrimeiroItem) {
                        continue;
                    }
                    $cabecalho .= <<<HTML
                                <th class="text-center" rowspan="2" colspan="{$spanPrimeiraColuna}">{$itemCabecalho}</th>
HTML;
                    $this->renderPrimeiroItem = false;
                } else {
                    $cabecalho .= <<<HTML
                                <th class="text-center" rowspan="2">{$itemCabecalho}</th>
HTML;
                }
            }
        }
        $cabecalho .= '</tr><tr>';
        // -- Segundo nível
        foreach ($this->getCabecalho('xls') as $key => $value) {
            if (!is_array($value)) {
                continue;
            }
            foreach ($value as $colunaFilho) {
                $cabecalho .= <<<HTML
                                <th class="text-center">{$colunaFilho}</th>
HTML;
            }
        }
        $cabecalho .= '</tr></thead>';
        return $cabecalho;
    }

    protected function renderDados() {

        if (is_null($this->dados)) {
            return;
        }
        echo <<<BODY
    <tbody>
BODY;
        foreach ($this->dados as $linha => $dadosLinha) {
            echo <<<TR
        <tr>
TR;
            if ($this->config->getNumeroAcoes()) {
                $idLinha = array_shift($dadosLinha);
            } else {
                reset($dadosLinha);
                $idLinha = current($dadosLinha);
            }

            $linhaFilha = array();

            if ($colunaAgrupada = $this->config->getColunaAgrupada() && isset($dadosLinha[$colunaAgrupada]) && key_exists($colunaAgrupada, $dadosLinha)) {
                $linhaFilha = $dadosLinha[$colunaAgrupada];
                unset($dadosLinha[$colunaAgrupada]);
            }

            foreach ($dadosLinha as $key => $dado) {
                if (is_array($dado)) {
                    $linhaFilha = $dado;
                    continue;
                }

                if (!$this->config->colunaEstaOculta($key)) {
                    // -- Verificação de totalizador de coluna
                    $this->somarColuna($key, $dado);

                    // -- Chamando função de callback registrada para o campo da listagem
                    $dado = $this->aplicarCallback($key, $dado, array($dadosLinha, $idLinha, array(
                        'campo' => $key
                    )));
                    echo <<<TD
            <td class="text-center">{$dado}</td>
TD;
                }
            }
            echo <<<TR
        </tr>
TR;
            if (empty($linhaFilha) || $this->config->ignorarLinhasFilhasExportacao()) {
                continue;
            }

            // -- Renderização de linha filha
            $colspan = count($this->config->getCabecalho('xls'));
            echo "<tr class=\"tr_abre_e_fecha\"><td colspan=\"{$colspan}\">" . $this->renderRepeticaoTemplate($linhaFilha) . '</td></tr>';
        }
        echo <<<BODY
    </tbody>
BODY;
    }

    /**
     * Cria o rodapé da listagem.
     * @todo Implementar o totalizador de coluna.
     */
    protected function renderRodape() {
        if (Simec_Listagem::TOTAL_QTD_REGISTROS == $this->config->getTotalizador()) {
            $spanDeColunas = (count($this->dados[0]) - 1) + count($this->acoes);
            $numRegistros = count($this->dados);
            echo <<<HTML
                    <tfoot>
                        <tr>
                            <td style="text-align:right" colspan="{$spanDeColunas}"><strong>Total de registros:&nbsp; $numRegistros</strong></td>
                        </tr>
                    </tfoot>
HTML;
        } elseif (Simec_Listagem::TOTAL_SOMATORIO_COLUNA == $this->config->getTotalizador()) {
            echo <<<HTML
                    <tfoot>
                        <tr>
HTML;
            // -- Correção para quando tem mais de uma ação na listagem.
            $dadosLinha = $this->dados[0];
            if ($this->config->getNumeroAcoes() > 0) {
                //removendo a chave pk das ações, por default é sempre a primeira
                $chaveId = key($dadosLinha);
                unset($dadosLinha[$chaveId]);
            }

            foreach ($dadosLinha as $key => $dado) {
                if ($this->config->colunaEstaOculta($key)) {
                    continue;
                }
                if (0 === $key) { // -- oO ????
                    continue;
                }

                if ($this->config->colunaEhTotalizada($key)) {
                    $valor = $this->aplicarCallback($key, $this->getTotalColuna($key));
                    echo <<<HTML
                                            <td style="text-align:right;font-weight:bold">{$valor}</td>
HTML;
                } else {
                    echo <<<HTML
                                            <td>&nbsp;</td>
HTML;
                }
            }
            echo <<<HTML
                        </tr>
                    </tfoot>
HTML;
        }
    }

}
