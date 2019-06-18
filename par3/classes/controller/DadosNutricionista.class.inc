<?php
/**
 * Classe de controle da DadosNutricionista
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-01-2016
 * @link     no link
 */

/**
 * Par3_Controller_DadosNutricionista
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-01-2016
 * @link     no link
 */
class Par3_Controller_DadosNutricionista extends Controle
{

    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Função visualisarNutricionista
     * - monta o formulario de quadro resumo
     *
     * @return string quadro resumo.
     *
     */
    public function visualisarNutricionista($danid, $tipoId = 'danid')
    {
    	if($tipoId == 'danid'){
    		$dados    = new Par3_Model_DadosNutricionista($danid);
    	}

        $usuario  = new Seguranca_Model_Usuario($dados->dancpf);
        $endereco = new Par3_Model_Endereco($dados->endid);

        require_once APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/visualizarNutricionista.php';
    }//end visualisarNutricionista()

}//end class

?>