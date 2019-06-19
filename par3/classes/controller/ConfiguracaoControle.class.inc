<?php
/**
 * Comentário do arquivo
 */

/**
 * Par3_Controller_ConfiguracaoControle()
 *
 * @author eduardoneto
 *
 */
class Par3_Controller_ConfiguracaoControle extends Controle
{

    public function __construct()
    {
        parent::__construct();
    }

    public static function recuperarIntrumentosGuia($itrid = null)
    {
        $oInstrumento = new Par3_Model_Instrumento();
        return $oInstrumento->recuperarIntrumentosGuia($itrid);
    }

    public static function recuperarDimensoesGuia($itrid, $dimid = null)
    {
        $oDimensao = new Par3_Model_Dimensao();
        return $oDimensao->recuperarDimensoesGuia($itrid, $dimid);
    }

    public static function recuperarAreasGuia($dimid)
    {
        $oArea = new Par3_Model_Area();
        return $oArea->recuperarAreasGuia($dimid);

    }

    public static function recuperarIndicadoresGuia($areid)
    {
        $oIndicador = new Par3_Model_Indicador();
        return $oIndicador->recuperarIndicadoresGuia($areid);
    }

    public static function recuperarCriteriosGuia($indid)
    {
        $oCriterio = new Par3_Model_Criterio();
        return $oCriterio->recuperarCriteriosGuia($indid);
    }

    public static function recuperarIniciativaGuia($indid)
    {
        $oIniciativa = new Par3_Model_PropostaIniciativa();
        return $oIniciativa->recuperarIniciativaGuia($indid);
    }

    /**
     * recuperaDadosFormGuiaDimensao()
     *
     * @param unknown $codigo
     * @param unknown $boDimensao
     * @param unknown $stTitulo
     * @param unknown $itrdsc
     * @param unknown $itrid
     * @param string $dimid
     * @param string $dimdsc
     * @param unknown $ordcod
     * @param unknown $arCampos
     */
    public static function recuperaDadosFormGuiaDimensao($codigo, &$boDimensao, &$stTitulo, &$itrdsc, &$itrid, &$dimid = null, &$dimdsc = null, &$ordcod, &$arCampos)
    {
        $oInstrumento 	= new Par3_Model_Instrumento();
        $oDimensao 		= new Par3_Model_Dimensao();

        if($_REQUEST['acaoGuia'] == 'editar'){
            $oDimensao->carregarPorId($codigo);
            $oInstrumento->carregarPorId($oDimensao->itrid);
            $dimid	= $oDimensao->dimid;
            $dimdsc = $oDimensao->dimdsc;
            $ordcod = $oDimensao->dimcod;
        }else{
            $oInstrumento->carregarPorId($codigo);
            $oDimensao->recuperarOrdemDimensaoPorItrid($codigo);
            $ordcod 	= $oDimensao->stOrdem;
        }

        $boDimensao	= '';
        $stTitulo 	= 'da dimensão';
        $itrdsc 	= $oInstrumento->itrdsc;
        $itrid 		= $oInstrumento->itrid;
        $arCampos 	= array("instrumento", "dimensao");
    }

    public static function recuperaDadosFormGuiaArea($codigo, &$boArea, &$stTitulo, &$itrdsc, &$dimdsc, &$dimid, &$areid = null, &$aredsc = null, &$ordcod, &$arCampos)
    {
        $oInstrumento 	= new Par3_Model_Instrumento();
        $oDimensao 		= new Par3_Model_Dimensao();
        $oArea 			= new Par3_Model_Area();

        if($_REQUEST['acaoGuia'] == 'editar'){
            $oArea->carregarPorId($codigo);
            $oDimensao->carregarPorId($oArea->dimid);
            $oInstrumento->carregarPorId($oDimensao->itrid);
            $areid = $oArea->areid;
            $aredsc = $oArea->aredsc;
            $ordcod	= $oArea->arecod;
        }else{
            $oArea->recuperarOrdemAreaPorDimid($codigo);
            $oDimensao->carregarPorId($codigo);
            $oInstrumento->carregarPorId($oDimensao->itrid);
            $ordcod	= $oArea->stOrdem;
        }

        $boArea 	= '';
        $stTitulo 	= 'da área';
        $itrdsc 	= $oInstrumento->itrdsc;
        $dimdsc 	= $oDimensao->dimdsc;
        $dimid 		= $oDimensao->dimid;

        $arCampos 	= array("instrumento", "dimensao", "area");
    }

    public static function recuperaDadosFormGuiaIndicador($codigo, &$boIndicador, &$stTitulo, &$itrdsc, &$dimdsc, &$aredsc, &$areid, &$indid = null, &$inddsc = null, &$indlei = null, &$indajuda = null, &$indrequerido = null, &$metas = null, &$ordcod, &$arCampos)
    {
        $oInstrumento 	= new Par3_Model_Instrumento();
        $oDimensao 		= new Par3_Model_Dimensao();
        $oArea 			= new Par3_Model_Area();
        $oIndicador 	= new Par3_Model_Indicador();
        $oIndicadorMeta	= new Par3_Model_IndicadorMeta();

        if($_REQUEST['acaoGuia'] == 'editar'){
            $oIndicador->carregarPorId($codigo);
            $oArea->carregarPorId($oIndicador->areid);
            $oDimensao->carregarPorId($oArea->dimid);
            $oInstrumento->carregarPorId($oDimensao->itrid);

            $metas   		= $oIndicadorMeta->recuperarDados($codigo);
            $metas    		= $metas ? $metas : array();
            $indid	  		= $oIndicador->indid;
            $inddsc	  		= $oIndicador->inddsc;
            $indlei	  		= $oIndicador->indlei;
            $indajuda 		= $oIndicador->indajuda;
            $indrequerido	= $oIndicador->indrequerido;
            $ordcod	  		= $oIndicador->indcod;
        }else{
            $oIndicador->recuperarOrdemIndicadorPorAreid($codigo);
            $oArea->carregarPorId($codigo);
            $oDimensao->carregarPorId($oArea->dimid);
            $oInstrumento->carregarPorId($oDimensao->itrid);

            $ordcod	= $oIndicador->stOrdem;
        }

        $boIndicador	= '';
        $stTitulo 		= 'do indicador';
        $itrdsc 		= $oInstrumento->itrdsc;
        $dimdsc 		= $oDimensao->dimdsc;
        $aredsc 		= $oArea->aredsc;
        $areid	 		= $oArea->areid;
        $arCampos 		= array("instrumento", "dimensao", "area", "indicador");
    }


    /**
     * recuperaDadosFormGuiaCriterio()
     * Recupera lalala
     *
     * @param integer $trocentos
     *
     * @return
     *
     */
    public static function recuperaDadosFormGuiaCriterio($codigo, &$boCriterio, &$stTitulo, &$itrdsc, &$dimdsc, &$aredsc, &$inddsc, &$indajuda, &$metas, &$indid, &$crtid = null, &$crtdsc = null, &$crtpontuacao = null, &$ordcod =null, &$arCampos)
    {
        $oInstrumento 	= new Par3_Model_Instrumento();
        $oDimensao 		= new Par3_Model_Dimensao();
        $oArea 			= new Par3_Model_Area();
        $oIndicador 	= new Par3_Model_Indicador();
        $oCriterio		= new Par3_Model_Criterio();
        $oIndicadorMeta	= new Par3_Model_IndicadorMeta();

        if($_REQUEST['acaoGuia'] == 'editar'){
            $oCriterio->carregarPorId($codigo);
            $oIndicador->carregarPorId($oCriterio->indid);
            $oArea->carregarPorId($oIndicador->areid);
            $oDimensao->carregarPorId($oArea->dimid);
            $oInstrumento->carregarPorId($oDimensao->itrid);
            $crtid = $oCriterio->crtid;
            $crtdsc = $oCriterio->crtdsc;
            $crtpontuacao = $oCriterio->crtpontuacao == 1 ? 'checked' : '';
            $ordcod	= $oCriterio->crtcod;
        }else{
            $oIndicador->carregarPorId($codigo);
            $oArea->carregarPorId($oIndicador->areid);
            $oDimensao->carregarPorId($oArea->dimid);
            $oInstrumento->carregarPorId($oDimensao->itrid);
            $oCriterio->recuperarOrdemCriterioPorIndid($codigo);
            $ordcod	= $oCriterio->stOrdem;
        }

        $boCriterio 	= '';
        $stTitulo 		= 'dos componentes do indicador';
        $itrdsc 		= $oInstrumento->itrdsc;
        $dimdsc 		= $oDimensao->dimdsc;
        $aredsc 		= $oArea->aredsc;
        $inddsc			= $oIndicador->inddsc;
        $indajuda		= $oIndicador->indajuda;
        $indid			= $oIndicador->indid;
        $metas          = $oIndicadorMeta->recuperarDados($indid);
        $arCampos		= array("instrumento", "dimensao", "area", "indicador", "criterio");
    }//end recuperaDadosFormGuiaCriterio();


}//endClass
?>