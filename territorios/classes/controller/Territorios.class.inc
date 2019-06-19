<?php
/**
 * Territorios_Controller_Territorios
 *
 * @category Class
 * @package  A1
 * @author   Orion Teles de Mesquita <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 28-12-2015
 * @link     no link
 */
class Territorios_Controller_Territorios
{
    public function recuperarUfsPorRegiao($regcod)
    {
        $where = $regcod ? array(" regcod::int in (" . implode(",", explode(',', $regcod)) . ") ") : null;

        $modelEstado = new Territorios_Model_Estado();
        $retorno = $modelEstado->recuperarTodos(" estuf, estuf || ' - ' || estdescricao as estdescricao ", $where, 'estuf');
        return simec_json_encode($retorno);
    }

    public function recuperarMunicipiosPorUf($uf)
    {
        $where = array(" estuf in ('" . implode("', '", explode(',', $uf)) . "') ");

        $modelMunicipio = new Territorios_Model_Municipio();
        $retorno = $modelMunicipio->recuperarTodos(" muncod, estuf || ' - ' || mundescricao as mundescricao ", $where, 'mundescricao');
        return simec_json_encode($retorno);
    }
}
?>