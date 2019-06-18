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
 * Par3_Controller_Indicador
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-10-2015
 * @link     no link
 */
class Par3_Controller_Indicador extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}

    /**
     * Função salvarIndicadorGuia
     * - Salva o indicador no Guia
     *
     */
    public function salvarIndicadorGuia()
    {
        // Salvar Indicador
        $oIndicador = new Par3_Model_Indicador;
        $indid = $oIndicador->preparaSalvar();

        // Exclui as metas já vinculadas
        $oIndicadorMeta = new Par3_Model_IndicadorMeta;
        $metasSalvas = $oIndicadorMeta->recuperarDados($indid);
        if( is_array($metasSalvas) ){
            foreach( $metasSalvas as $metas ){
                $oIndicadorMeta->preparaExcluir($metas['inmid']);
            }
        }

        // Salvar as Metas do PNE vinculadas ao Indicador
        if( is_array($_POST['metas']) && $_POST['metas'][0] ){
            foreach( $_POST['metas'] as $meta ){
                $oIndicadorMeta = new Par3_Model_IndicadorMeta;
                $oIndicadorMeta->preparaSalvar($indid, $meta);
            }
        }

        return true;

    }//end salvarIndicadorGuia()


}//end Class
?>