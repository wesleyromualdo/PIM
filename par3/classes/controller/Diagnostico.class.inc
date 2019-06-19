<?php

error_reporting(0);
/**
 * Par_Controle_Diagnostico
 *
 * @category Class
 * @package  A1
 * @author   Rafael Jose <rafaelgloria@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-09-2015
 * @link     no link
 */
class Par3_Controller_Diagnostico extends Controle
{

    public function __construct()
    {
        parent::__construct();
    }

    /**
     * Função lista
     * - monta a lista de instrumentos unidade.
     *
     * @return escreve a lista.
     *
     */
    public static function verificarPendencia($inuid)
    {
        $controllerEntidade = new Par3_Controller_InstrumentoUnidade();
        $testaPreenchimentoUnidade                  = $controllerEntidade->verificaPercentualPreenchimentoUnidade($inuid);
        $testaPreenchimentoQuestoesEstrategicas     = $controllerEntidade->verificaPreencimentoQuestoes($inuid);
        $testaPreenchimentoQuestoesEstrategicasPNE  = $controllerEntidade->verificaPreencimentoQuestoesPNE($inuid);

        $obPNE = new Par3_Controller_Pne();
        //$testaPreenchimentoPNE = $obPNE->verificaPreenchimentoPNE($inuid);
        //a pedido da Genilda todos os estados serão liberados sem o preenchimento do PNE 27/12/2016
        // para voltar a verificação basta excluir a linha abaixo e descomentar o código acima.
        $testaPreenchimentoPNE = 100;

        if($testaPreenchimentoUnidade >= 100 && $testaPreenchimentoPNE >= 100 && $testaPreenchimentoQuestoesEstrategicas >= 100  && $testaPreenchimentoQuestoesEstrategicasPNE >= 100){
            $existePendenciaGeral = FALSE;
        } else {
            $existePendenciaGeral = TRUE;
        }

        return $existePendenciaGeral;
    }//end lista()

}//end class

?>