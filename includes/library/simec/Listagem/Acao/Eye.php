<?php

/**
 * classe implementada usando os ícones da biblioteca Font Awesome, os mesmos usados com o template inspinia
 * @link https://wrapbootstrap.com/theme/inspinia-responsive-admin-theme-WB0R5L90S
 * @link http://fontawesome.io/icons/
 */
class Simec_Listagem_Acao_Eye extends Simec_Listagem_Acao
{
    protected $icone = 'eye';
    protected $titulo = 'Visualizar';
    protected $cor = 'info';

    protected function renderGlyph()
    {
        $html = <<<HTML
            <span class="btn btn-%s btn-sm fa fa-eye"></span>
HTML;
        return sprintf($html, $this->cor);
    }
}