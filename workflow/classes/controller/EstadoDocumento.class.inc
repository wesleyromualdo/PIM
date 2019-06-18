<?php
/**
 * EstadoDocumento_Controller_EstadoDocumento
 *
 * @category Class
 * @package  A1
 * @author   Wesley Romualdo da Silva <wesleysilva@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25-01-2016
 * @link     no link
 */
class EstadoDocumento_Controller_EstadoDocumento
{
    public function recuperarEstadoDocumento($tpdid)
    {
        $where = $tpdid ? array(" tpdid::int in (" . implode(",", explode(',', $tpdid)) . ") ") : null;

        $modelEstado = new Workflow_Model_EstadoDocumento();
        $sql = $modelEstado->pegarSQLSelect($arrPost[$tpdid]);
        $retorno = $modelEstado->recuperarTodos($sql);
        return simec_json_encode($retorno);
    }
}
?>