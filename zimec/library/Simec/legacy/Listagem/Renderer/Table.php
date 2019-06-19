<?php
/**
 * Renderizador de listagens XLS.
 *
 * @version $Id: Xls.php 113369 2016-09-15 14:05:50Z tiagofreire $
 */
/**
 * Renderizador base.
 * @see Simec_Listagem_Renderer_Abstract
 */
require_once(dirname(__FILE__) . '/Abstract.php');

class Simec_Listagem_Renderer_Table extends Simec_Listagem_Renderer_Abstract {

    protected $linhasiniciais = [];

    /**
     * Imprime o código XLS da listagem.
     */
    public function render() {
        $html = "";
        $html .= '<table class="table" id="tb_render" >';
        $html .= $this->renderCabecalho();
        $html .= $this->renderDados();
        $html .= $this->renderRodape();
        $html .= '</table>';
        return $html;
    }

    public function renderTitulo() {

    }

    /**
     * Função que insere uma linha no topo da tabela
     * Esta função possui dois parâmetros:
     *     $valor - texto que será apresentado na linha
     *     $atributos - atributos da tag td, exemplos:
     *          'colspan' => 3
     *          'width' => 100
     *
     * @param string $valor
     * @param array $atributos
     */
    public function addLinhaInicial($colunas = array()){
        $this->linhasiniciais[] = $colunas;
        return $this;
    }

    /**
     * Função que reenderiza as colunas iniciais da tabela
     *
     */
    public function renderLinhasIniciais(){
        foreach ($this->linhasiniciais as $colunas) {
            echo <<<HTML
                <TR>
HTML;
            foreach ($colunas as $col) {
                if(empty($col['atributos']['class'])){
                    $col['atributos']['class'] = 'text-center';
                } else {
                    if(strpos($col['atributos']['class'], 'text-') === false){
                        $col['atributos']['class'] .= ' text-center';
                    }
                }
                $attrs = !empty($col['atributos']) ? $this->getLinhaAtributos($col['atributos']) : '';
                echo <<<HTML
                    <TD {$attrs}>
                        {$col['valor']}
                    </TD>
HTML;
            }
            echo <<<HTML
                </TR>
HTML;
        }
    }

    /**
     * Retorna os atributos da linha extra como string
     *
     * @return string
     */
    public function getLinhaAtributos($atributos){
        $attrs = "";
        if(is_array($atributos)) {
            foreach ($atributos as $attr => $val) {
                $attrs .= "{$attr}=\"{$val}\" ";
            }
        }
        return $attrs;
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

        $html = "";
        if (is_null($this->dados)) {
            return;
        }
        $html .= <<<BODY
    <tbody>
BODY;

        $this->renderLinhasIniciais();

        foreach ($this->dados as $linha => $dadosLinha) {
            $html .= <<<TR
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
                    $html .= <<<TD
            <td class="text-center" id="{$key}">{$dado}</td>
TD;
                }
            }
            $html .= <<<TR
        </tr>
TR;
            if (empty($linhaFilha) || $this->config->ignorarLinhasFilhasExportacao()) {
                continue;
            }

            // -- Renderização de linha filha
            $colspan = count($this->config->getCabecalho());
            $html .= "<tr class=\"tr_abre_e_fecha\"><td colspan=\"{$colspan}\">" . $this->renderRepeticaoTemplate($linhaFilha) . '</td></tr>';
        }
        $html .= <<<BODY
    </tbody>
BODY;
        return $html;
        ver($html, d);
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
            if (is_array($this->acoes)) {
                $qtdAcoes = $this->config->getNumeroAcoes();
                for ($i = 0; $i < $qtdAcoes; $i++) {
                    array_unshift($dadosLinha, "--a{$i}");
                }
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
