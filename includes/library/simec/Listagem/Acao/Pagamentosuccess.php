<?php
/**
 * Implementação da classe de ação de pagamento.
 *
 * @version $Id: Pagamentosuccess.php 142567 2018-08-09 22:00:57Z danielfiuza $
 */

/**
 * Ação de pagamento.
 *
 * @link http://getbootstrap.com/components/#glyphicons glyphicon glyphicon-usd
 * @package Simec\View\Listagem\Acao
 */
class Simec_Listagem_Acao_Pagamentosuccess extends Simec_Listagem_Acao
{
    protected $icone = 'usd';
    protected $titulo = 'Pagamento';

    protected function renderGlyph()
    {
        $html = <<<HTML
            <span class="btn btn-primary btn-sm  btn glyphicon glyphicon-usd"></span>
HTML;
        return sprintf($html, $this->cor);
    }
}
