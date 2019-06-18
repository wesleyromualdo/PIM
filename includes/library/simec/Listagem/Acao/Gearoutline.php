<?php
/**
 * $Id: Edit.php 97554 2015-05-19 21:13:18Z maykelbraz $
 */

/**
 *
 */
class Simec_Listagem_Acao_Gearoutline extends Simec_Listagem_Acao
{
    protected $icone = 'cog';
    protected $titulo = 'Configurações';
    protected $cor = 'warning';

    protected function renderGlyph()
    {
        $html = <<<HTML
            <span class="btn btn-%s btn-sm  btn-outline glyphicon glyphicon-cog"></span>
HTML;
        return sprintf($html, $this->cor);
    }
}
