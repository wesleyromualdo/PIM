<?php
/**
 * $Id: Plus.php 81167 2014-06-04 12:24:26Z maykelbraz $
 */

/**
 *
 */
class Simec_Listagem_Acao_Plus extends Simec_Listagem_AcaoComID
{
    protected $icone = 'plus';
    protected $titulo = 'Detalhar';

    protected function renderAcao()
    {
        $acao = <<<HTML
<a href="javascript:expandirLinha('%s', %s, '%s');" title="%s" id="arow-%s">
    <span class="glyphicon glyphicon-%s"></span>
</a>
HTML;
        return sprintf(
            $acao,
            $this->callbackJS,
            $this->getCallbackParams(true),
            $this->getAcaoID(),
            $this->titulo,
            $this->getAcaoID(),
            $this->icone
        );
    }
}
