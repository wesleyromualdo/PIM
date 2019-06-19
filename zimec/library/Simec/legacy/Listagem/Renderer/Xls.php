<?php

/**
 * Renderizador de listagens XLS.
 *
 * @version $Id: Xls.php 84305 2014-08-07 19:11:23Z werteralmeida $
 */
/**
 * Renderizador base.
 * @see Simec_Listagem_Renderer_Abstract
 */
require_once(dirname(__FILE__) . '/Abstract.php');

class Simec_Listagem_Renderer_Xls extends Simec_Listagem_Renderer_Abstract {

    /**
     * Imprime o código XLS da listagem.
     */
    public function render() {
        header("Content-Type:   application/vnd.ms-excel; charset=utf-8");
        header("Content-type:   application/x-msexcel; charset=utf-8");
        header("Content-Disposition: attachment; filename=extrato.xls");
        header("Expires: 0");
        header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
        header("Cache-Control: private", false);
        echo '<table class="table table-striped table-bordered table-hover" id="tb_render" >';
        echo $this->renderCabecalho();
        $this->renderDados();
        echo $this->renderRodape();
        echo '</table>';
    }

    protected function renderTitulo() {
        
    }

    protected function renderCabecalho() {
        // -- Se não houver um cabeçalho definido, pula a criação do cabeçaho
        if (is_null($this->cabecalho)) {
            return '';
        }

        // -- Cabecalho de um único nível
        if (1 == $nivelCabecalho) {
            $colunas = '';
            foreach ($this->cabecalho as $itemCabecalho) {
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
        foreach ($this->cabecalho as $key => $itemCabecalho) {
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
        foreach ($this->cabecalho as $key => $value) {
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

    /**
     * Faz a contagem de colunas da listagem, incluíndo colunas de ações (quando presentes).
     * @return integer
     */
    protected function quantidadeDeColunasExibidas() {
        if (count($this->colunasOcultas) != 0) {
            $qtdColunasExibidas = count(
                    array_diff_key(// -- Criar um array temporário com os campos dados que não estão inclusos na listagem de colunas ocultas
                            $this->dados[0], array_combine(// -- Cria um array temporário baseado nas colunas ocultas
                                    $this->colunasOcultas, array_fill(0, count($this->colunasOcultas), null)
                            )
                    )
            );
        }
        // -- Ajuste da quantidade de colunas da query mediante contagem de ações
        if (count($this->acoes) > 1) {
            // -- -1 pq a coluna de ID já é contada em $qtdColunasExibidas
            $qtdColunasExibidas += count($this->acoes) - 1;
        }

        return $qtdColunasExibidas;
    }

    protected function renderDados() {
        // -- @todo Verificar se o tipo de listagem é de FORM, para incluir os inputs
        if (empty($this->dados)) {
            return;
        }
        echo <<<BODY
    <tbody>
BODY;
        foreach ($this->dados as $linha => $dadosLinha) {
            $classe = "";

            echo <<<TR
        <tr{$classe}>
TR;

            // -- @todo: Verificar o tipo da listagem do formulario
            if ($this->acoes) {
                $idLinha = array_shift($dadosLinha);
            } else {
                reset($dadosLinha);
                $idLinha = current($dadosLinha);
            }
            foreach ($dadosLinha as $key => $dado) {
                if (!in_array($key, $this->colunasOcultas)) {
                    // -- Verificação de totalizador de coluna
                    if (in_array($key, array_keys($this->colunasTotalizadas))) {
                        $this->somarColuna($key, $dado);
                    }
                    // -- Chamando função de callback registrada para o campo da listagem
                    if (array_key_exists($key, $this->callbacksDeCampo)) {
                        $dado = $this->callbacksDeCampo[$key]($dado, $dadosLinha, $idLinha);
                    }
                    echo <<<TD
            <td class="text-center">{$dado}</td>
TD;
                }
            }
            echo <<<TR
        </tr>
TR;
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
        if (Simec_Listagem::TOTAL_QTD_REGISTROS == $this->totalizador) {
            $spanDeColunas = (count($this->dados[0]) - 1) + count($this->acoes);
            $numRegistros = count($this->dados);
            echo <<<HTML
                    <tfoot>
                        <tr>
                            <td style="text-align:right" colspan="{$spanDeColunas}"><strong>Total de registros:&nbsp; $numRegistros</strong></td>
                        </tr>
                    </tfoot>
HTML;
        } elseif (Simec_Listagem::TOTAL_SOMATORIO_COLUNA == $this->totalizador) {
            echo <<<HTML
                    <tfoot>
                        <tr>
HTML;
            // -- Correção para quando tem mais de uma ação na listagem.
            $dadosLinha = $this->dados[0];
            if (is_array($this->acoes)) {
                $qtdAcoes = count($this->acoes);
                for ($i = 0; $i < $qtdAcoes; $i++) {
                    array_unshift($dadosLinha, "--a{$i}");
                }
            }

            foreach ($dadosLinha as $key => $dado) {
                if (in_array($key, $this->colunasOcultas)) {
                    continue;
                }
                if (0 === $key) { // -- oO ????
                    continue;
                }

                if (key_exists($key, $this->colunasTotalizadas)) {
                    if (is_string($key) && key_exists($key, $this->callbacksDeCampo)) {
                        $valor = $this->callbacksDeCampo[$key]($this->colunasTotalizadas[$key]);
                    } else {
                        $valor = $this->colunasTotalizadas[$key];
                    }
                    echo <<<HTML
                                            <td style="text-align:center;font-weight:bold">{$valor}</td>
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
