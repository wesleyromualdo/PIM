<?php
/**
 * Renderizador de listagens XLS.
 *
 * @version $Id: Pdf.php 120677 2017-03-23 16:43:17Z marcelorodovalho $
 */
/**
 * Renderizador base.
 * @see Simec_Listagem_Renderer_Abstract
 */
require_once(dirname(__FILE__) . '/Abstract.php');
include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";

/**
 * Class Simec_Listagem_Renderer_Pdf
 * @package Simec\View\Listagem\Renderer\Pdf
 * @author Marcelo Rodovalho <marcelorodovalho@mec.gov.br>
 */
class Simec_Listagem_Renderer_Pdf extends Simec_Listagem_Renderer_Abstract {

    /**
     * Imprime o código XLS da listagem.
     */
    public function render() {
        ini_set("memory_limit","500M");
        set_time_limit(0);
        header('Content-Transfer-Encoding: binary');
        header('Content-Encoding: none');
        header("Content-Type:   application/pdf");
        header("Content-Disposition: attachment; filename=extrato.pdf");
        header("Expires: 0");
        header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
        header("Cache-Control: private", false);

        $html = $this->renderEstilo() .
            '<table class="table table-striped table-bordered table-hover" id="tb_render" >' .
            $this->renderCabecalho() .
            $this->renderDados() .
            $this->renderRodape() .
            '</table>';

        $dompdf = new DOMPDF();
        $dompdf->load_html($html);
        $dompdf->set_paper('A4', 'landscape');
        $dompdf->render();
        echo $dompdf->output();
    }

    public function renderTitulo() {

    }

    protected function renderCabecalho() {
        // -- Se não houver um cabeçalho definido, pula a criação do cabeçaho
        if (is_null($this->getCabecalho())) {
            return '';
        }

        // -- Cabecalho de um único nível
        if (1 == $nivelCabecalho) {
            $colunas = '';
            foreach ($this->getCabecalho() as $itemCabecalho) {
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
        foreach ($this->getCabecalho() as $key => $itemCabecalho) {
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
        foreach ($this->getCabecalho() as $key => $value) {
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
        $dados = '';
        if (is_null($this->dados)) {
            return;
        }
        $dados .= <<<BODY
    <tbody>
BODY;
        foreach ($this->dados as $linha => $dadosLinha) {
            $dados .= <<<TR
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
                    $dados .= <<<TD
            <td class="text-center">{$dado}</td>
TD;
                }
            }
            $dados .= <<<TR
        </tr>
TR;
            if (empty($linhaFilha) || $this->config->ignorarLinhasFilhasExportacao()) {
                continue;
            }

            // -- Renderização de linha filha
            $colspan = count($this->config->getCabecalho());
            $dados .= "<tr class=\"tr_abre_e_fecha\"><td colspan=\"{$colspan}\">" . $this->renderRepeticaoTemplate($linhaFilha) . '</td></tr>';
        }
        $dados .= <<<BODY
    </tbody>
BODY;
        return $dados;
    }

    /**
     * Cria o rodapé da listagem.
     * @todo Implementar o totalizador de coluna.
     */
    protected function renderRodape() {
        $rodape = '';
        if (Simec_Listagem::TOTAL_QTD_REGISTROS == $this->config->getTotalizador()) {
            $spanDeColunas = (count($this->dados[0]) /*- 1*/) + count($this->acoes);
            $numRegistros = count($this->dados);
            $rodape .= <<<HTML
                    <tfoot>
                        <tr>
                            <td style="text-align:right" colspan="{$spanDeColunas}"><strong>Total de registros:&nbsp; $numRegistros</strong></td>
                        </tr>
                    </tfoot>
HTML;
        } elseif (Simec_Listagem::TOTAL_SOMATORIO_COLUNA == $this->config->getTotalizador()) {
            $rodape .= <<<HTML
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
                    $rodape .= <<<HTML
                                            <td style="text-align:right;font-weight:bold">{$valor}</td>
HTML;
                } else {
                    $rodape .= <<<HTML
                                            <td>&nbsp;</td>
HTML;
                }
            }
            $rodape .= <<<HTML
                        </tr>
                    </tfoot>
HTML;
        }
        return $rodape;
    }

    public function renderEstilo()
    {
        $estilo = <<<HTML
<style>
    <!--
    body {
        font-family: Verdana, Arial, Helvetica, sans-serif;
        font-size: 9pt;
    }
    table {
        font-family:"Trebuchet MS", Arial, Helvetica, sans-serif;
        width:100%;
        border-collapse:collapse;
    }
    table td {
        font-size:9pt;
        border:1px solid #d4d4d4;
        padding:3px 7px 2px 7px;
    }
    table thead tr {
        background-color: #89abea;
    }
    -->
</style>

HTML;
        return $estilo;

    }

}
