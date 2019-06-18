<?php
/**
 * $Id: AcaoComID.php 81167 2014-06-04 12:24:26Z maykelbraz $
 */

/**
 * 
 */
abstract class Simec_Listagem_AcaoComID extends Simec_Listagem_Acao
{
    protected function renderAcao()
    {
        $acao = <<<HTML
<a href="javascript:%s(%s);" title="%s" id="arow-%s">
    <span class="glyphicon glyphicon-%s"></span>
</a>
HTML;
        return sprintf(
            $acao,
            $this->callbackJS,
            $this->getCallbackParams(),
            $this->titulo,
            $this->getAcaoID(),
            $this->icone
        );
    }
}
