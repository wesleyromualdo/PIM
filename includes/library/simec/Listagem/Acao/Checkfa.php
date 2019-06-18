<?php

/**
 * User: danielfiuza
 * Date: 24/02/17
 * Time: 15:33
 * classe implementada usando os ícones da biblioteca Font Awesome, os mesmos usados com o template inspinia
 * @link https://wrapbootstrap.com/theme/inspinia-responsive-admin-theme-WB0R5L90S
 * @link http://fontawesome.io/icons/
 */
class Simec_Listagem_Acao_Checkfa extends Simec_Listagem_Acao
{
    protected $icone = 'checkfa';
    protected $titulo = 'Selecionar';
    protected $cor = 'success';

    protected function renderGlyph()
    {
        $html = <<<HTML
            <span class="btn btn-%s btn-sm fa fa-check"></span>
HTML;
        return sprintf($html, $this->cor);
    }
}