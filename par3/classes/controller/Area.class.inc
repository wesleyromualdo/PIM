<?php
/**
 * Classe de controle da Area
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_Area
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-10-2015
 * @link     no link
 */
class Par3_Controller_Area extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}


    /**
     * Função recuperarDadosListaVinculacao
     * - Retorn os critérios irmãos do critério passado para a vinculação das regras.
     *
     */
    public function pontuacaoArea($areid, $inuid){

        $oArea = new Par3_Model_Area();
        $dados = $oArea->pontuacaoArea($areid, $inuid);

        $percent['percent'] = $dados['indicadores'] > 0 ? ($dados['pontuados']*100)/$dados['indicadores'] : 0;

        return array_merge($dados, $percent);
    }

}//end Class
?>