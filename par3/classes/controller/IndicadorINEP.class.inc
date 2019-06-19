<?php
/**
 * Classe de controle do Indicador
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_IndicadorINEP
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-10-2015
 * @link     no link
 */
class Par3_Controller_IndicadorINEP extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}

    /**
     * Função carregarIndicadores
     * - Listar dados do indicador
     *
     */
    public function carregarIndicadores($filtros = array())
    {
    	$modelIndicadorINEP = new Par3_Model_IndicadorINEP();
    	
 		$sql = $modelIndicadorINEP->pegarSQLLista($filtros);

 		return $modelIndicadorINEP->pegaLinha($sql);
    }//end carregarIndicadores()


}//end Class
?>