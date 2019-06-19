<?php
/**
 * Interface de edição dos dados do beneficiário.
 *
 * @version $Id: iniciativas.inc 112447 2016-07-26 14:01:33Z werteralmeida $
 */
$fm = new Simec_Helper_FlashMessage('editaremenda');
global $db,$alterandoEmenda;
$alterandoEmenda = true;
require_once dirname(__FILE__) . '/../../emendas/editaremenda/iniciativas/iniciativas.inc';
