<?php
/**
 *
 */

/**
 *
 */
class Simec_Listagem_Acao_Retornar extends Simec_Listagem_Acao
{
    protected $icone = 'share-alt';
    protected $titulo = 'Retornar';
    protected $cor = 'success';

    protected function renderGlyph()
    {
        $html = <<<HTML
        <style>
            .icon-flipped {
                transform: scaleX(-1);
                -moz-transform: scaleX(-1);
                -webkit-transform: scaleX(-1);
                -ms-transform: scaleX(-1);
            }
        </style>
        <span class="btn btn-%s btn-sm glyphicon glyphicon-%s icon-flipped"></span>
HTML;
        return sprintf($html, $this->cor, $this->icone);
    }
}
