<?php
/**
 * $Id: Pluss.php 117020 2017-01-03 11:48:39Z gabrielalmeida $
 */

/**
 *
 */
class Simec_Listagem_Acao_Pluss extends Simec_Listagem_AcaoComID
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
}
