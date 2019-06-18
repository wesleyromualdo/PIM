<?php
/**
 * $Id: Delete.php 97554 2015-05-19 21:13:18Z maykelbraz $
 */

/**
 *
 */
class Simec_Listagem_Acao_Menos extends Simec_Listagem_Acao
{
    protected $icone = 'minus';
    protected $titulo = 'Menos';
    protected $cor =  "warning";

    protected function renderAcao()
    {
        //separando e removendo aspas dos parametros
        $params = $this->getCallbackParams();
        $params = explode(',', $params);
        $params = str_replace("'", '',$params);
        $cancelado = new Ted_Model_Cancelamentovalororcamentario();
        $possui = $cancelado->possuiValorCancelado($params[0]);
        if ($possui > 0) {
            return $this->cor = "<a href=\"javascript:cancelarValorOrcamentario('" . $params[0] . "', '" . $params[1] . "', '" . $params[2] . "');\" title=\"Cancelamento de Valor Orçamentário\"><span class=\"btn btn-success btn-sm glyphicon glyphicon-minus\"></span></a>";
        } else {
            return $this->cor = "<a href=\"javascript:cancelarValorOrcamentario('" . $params[0] . "', '" . $params[1] . "', '" . $params[2] . "');\" title=\"Cancelamento de Valor Orçamentário\"><span class=\"btn btn-warning btn-sm glyphicon glyphicon-minus\"></span></a>";

        }
    }
}