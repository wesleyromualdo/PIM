<?php
/**
 * $Id: Plusoutline.php 142567 2018-08-09 22:00:57Z danielfiuza $
 */

/**
 *
 */
class Simec_Listagem_Acao_Plusoutline extends Simec_Listagem_AcaoComID
{
    protected $icone = 'plus';
    protected $titulo = 'Detalhar';
    protected $cor = 'info';

    protected function renderAcao()
    {
        $acao = <<<HTML
<a href="javascript:expandirLinha('%s', %s, '%s');" title="%s" id="arow-%s">%s</a>
HTML;
        return sprintf(
            $acao,
            $this->callbackJS,
            $this->getCallbackParams(true),
            $this->getAcaoID(),
            $this->titulo,
            $this->getAcaoID(),
            $this->renderGlyph()
        );
    }

    protected function renderGlyph()
    {
        $html = <<<HTML
            <span class="btn btn-%s btn-sm  btn-outline glyphicon glyphicon-plus"></span>
HTML;
        return sprintf($html, $this->cor);
    }
}
