<?php
/**
 * $Id: Delete.php 97554 2015-05-19 21:13:18Z maykelbraz $
 */

/**
 *
 */
class Simec_Listagem_Acao_Remove extends Simec_Listagem_Acao
{
    protected $icone = 'trash';
    protected $titulo = 'Remover';
    protected $cor = 'danger';

    /**
     * @return string
     */
    protected function renderGlyph()
    {
        $html = <<<HTML
        <span class="btn btn-%s btn-sm glyphicon glyphicon-%s %s"></span>
HTML;

        return sprintf($html, $this->cor, $this->icone, $this->config['outline']);
    }

}