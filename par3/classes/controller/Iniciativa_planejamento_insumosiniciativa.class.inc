<?php
/**
 * Classe de controle do  iniciativa_planejamento_insumosiniciativa
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   HEMERSON EXPEDITO DORNELAS MORAIS <hemerson.moraiss@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */

include_once APPRAIZ . 'par3/classes/model/Iniciativa_planejamento_insumosiniciativa.class.inc';
include_once APPRAIZ . 'par3/classes/model/Iniciativa_planejamento_insumosiniciativaHistorico.class.inc';

/**
 * Par3_Controller_Iniciativa_planejamento_insumosiniciativa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */
class Par3_Controller_Iniciativa_planejamento_insumosiniciativa
{
    private $model;

    public function __construct()
    {
        $ipiinid = $_POST['ipiinid'];
        $this->model = new Par3_Model_Iniciativa_planejamento_insumosiniciativa($ipiinid);
        $this->modelHistorico = new Par3_Model_Iniciativa_planejamento_insumosiniciativaHistorico();
    }


    public function formInsumosIniciativa($arrPost)
    {

        global $simec;
        echo '<input type="hidden" name="inpid" value="' . $arrPost["inpid"] . '"/>';
        echo '<input type="hidden" name="ipiinid" value="' . $this->model->ipiinid . '"/>';
        echo '<div class="form-group"><label class="col-md-3 control-label">Código:</label>';
        echo '<div class="col-md-2"><input type="text" disabled="" value="' . $this->model->ipiinid . '" class="form-control"></div>';
        echo '<div style="clear:both"></div>';
        echo '</div>';
        echo $simec->input('ipiindescricao', 'Descrição', $this->model->ipiindescricao, array('type' => 'text', 'maxlength' => '255'));
        echo $simec->input('ipiinunidadeinsumo1', 'Und Insumo 1', $this->model->ipiinunidadeinsumo1, array('type' => 'text', 'maxlength' => '255'));
        echo $simec->input('ipiinqtdinsumo1', 'Qtd Insumo 1', $this->model->ipiinqtdinsumo1, array('type' => 'number', 'maxlength' => '12'));
        echo $simec->input('ipiinunidadeinsumo2', 'Und Insumo 2', $this->model->ipiinunidadeinsumo2, array('type' => 'text', 'maxlength' => '255'));
        echo $simec->input('ipiinqtdinsumo2', 'Qtd Insumo 2', $this->model->ipiinqtdinsumo2, array('type' => 'number', 'maxlength' => '12'));
        echo $simec->input('ipiinqtd', 'Quantidade', $this->model->ipiinqtd, array('type' => 'number', 'readonly' => 'true'));
        echo $simec->input(
            'ipiinvalunit',
            'Valor Unitário',
            $this->model->ipiinvalunit,
            array(
                    'maxlength' => '12'
            )
        );
        echo $simec->input('ipiinvaltotal', 'Valor Total', $this->model->ipiinvaltotal, array('readonly' => 'true'));

        echo '<script>
                                jQuery(function ($) {
                                    "use strict";
                                    $("#ipiinvalunit").inputmask(\'decimal\', {
                                        \'alias\': \'numeric\',
                                        \'groupSeparator\': \'.\',
                                        \'autoGroup\': true,
                                        \'digits\': 2,
                                        \'min\': 0,
                                        \'max\': 999999999999,
                                        \'radixPoint\': ",",
                                        \'digitsOptional\': false,
                                        \'allowMinus\': false,
                                        \'allowPlus\': false,
                            //        \'prefix\': \'R$ \',
                                        \'placeholder\': \'\'
                                    });

                                    $("#ipiinvaltotal").inputmask(\'decimal\', {
                                        \'alias\': \'decimal\',
                                        \'groupSeparator\': \'.\',
                                        \'autoGroup\': true,
                                        \'digits\': 2,
                                        \'min\': 0,
                                        \'allowPlus\': false,
                                        \'allowMinus\': false,
                                        \'max\': 999999999999,
                                        \'radixPoint\': ",",
                                        \'digitsOptional\': false,
                            //        \'prefix\': \'R$ \',
                                        \'placeholder\': \'\',
                                        \'autoUnmask\': true
                                    });
                                });

                                $("#ipiinqtdinsumo1,#ipiinqtdinsumo2 ,#ipiinvalunit").change(function (evt) {
                        
                                evt.preventDefault();
                        
                                var insumo1 = document.getElementById(\'ipiinqtdinsumo1\').value;
                                
                                   if(insumo1 == 0 || insumo1 == ""){
                                   insumo1 = 0;
                                }
                           
                                var insumo2 = document.getElementById(\'ipiinqtdinsumo2\').value;
                                      if(insumo2 == 0 || insumo2 == ""){
                                   insumo2 = 0;
                                }
                          
                                var valunit = document.getElementById(\'ipiinvalunit\').value.replace(\'.\',\'\').replace(\',\',\'.\');
                                console.log(valunit);
                         if(valunit == 0 || valunit == ""){
                                  valunit = 1;
                              }
                             
                       
                                document.getElementById(\'ipiinqtd\').value = parseInt(insumo1) + parseInt(insumo2);
                                var ipiinvaltotal = (parseFloat(insumo1) + parseFloat(insumo2))*valunit;
                                console.log(ipiinvaltotal);
                                $(\'#ipiinvaltotal\').val(ipiinvaltotal);
                              

                                
                          });
                                
                         $(function () {
                                    $(\'#ipiinqtdinsumo1\').bind(\'keydown\', soNums); //
                                    $(\'#ipiinqtdinsumo2\').bind(\'keydown\', soNums); //
                                    $(\'#ipiinqtd\').bind(\'keydown\', soNums); //
                                    $(\'#ipiinvalunit\').bind(\'keydown\', soNums); //
                                    $(\'#ipiinvaltotal\').bind(\'keydown\', soNums); //
                            
                            
                         });

                         function soNums(e) {
                        
                                //teclas adicionais permitidas (tab,delete,backspace,setas direita e esquerda)
                                keyCodesPermitidos = new Array(8, 9, 37, 39, 46);
                        
                                //numeros e 0 a 9 do teclado alfanumerico
                                for (x = 48; x <= 57; x++) {
                                    keyCodesPermitidos.push(x);
                                }
                       
                                //numeros e 0 a 9 do teclado numerico
                                for (x = 96; x <= 105; x++) {
                                    keyCodesPermitidos.push(x);
                                }
                        
                                //Pega a tecla digitada
                                keyCode = e.which;
                        
                                //Verifica se a tecla digitada é permitida
                                if ($.inArray(keyCode, keyCodesPermitidos) != -1) {
                                    return true;
                                    }
                            return false;
                         }

               
                        </script>';

    }


    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        global $url;
        $this->model->popularDadosObjeto();
        $url .= '&ipiinid=' . $this->model->ipiinid;

        $arrayInsumo['ipiindescricao'] = trim(($arrPost['ipiindescricao']));
        $arrayInsumo['ipiinunidadeinsumo1'] = trim(($arrPost['ipiinunidadeinsumo1']));
        $arrayInsumo['ipiinqtdinsumo1'] = $arrPost['ipiinqtdinsumo1'];
        $arrayInsumo['ipiinunidadeinsumo2'] = trim(($arrPost['ipiinunidadeinsumo2']));
        $arrayInsumo['ipiinqtdinsumo2'] = $arrPost['ipiinqtdinsumo2'];
        $arrayInsumo['ipiinqtd'] = $arrPost['ipiinqtd'];

        $ipiinvalunit = str_replace('.','',$arrPost['ipiinvalunit']);
        $arrayInsumo['ipiinvalunit'] = str_replace(',','.',$ipiinvalunit);

        $ipiinvaltotal = str_replace('.','',$arrPost['ipiinvaltotal']);
        $arrayInsumo['ipiinvaltotal'] = str_replace(',','.',$ipiinvaltotal);
        try {
            $this->model->popularDadosObjeto($arrayInsumo);
            $sucesso = $this->model->salvar();
            $this->model->commit();
            $this->modelHistorico->gravarHistorico($this->model, 'CREATE');
            $this->modelHistorico->commit();

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if ($sucesso) {
            return (int)$sucesso;
        }
        return 'erro';
    }

    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($arrPost)
    {
        global $url;
        $id = $arrPost['ipiinid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&ipiinid={$id}";
        $iniciativa_planejamento_insumosiniciativa = new Par3_Model_Iniciativa_planejamento_insumosiniciativa($id);
        try {
            $iniciativa_planejamento_insumosiniciativa->ipiinstatus = 'I';
            $iniciativa_planejamento_insumosiniciativa->ipiindtinativacao = date('Y-m-d H:i:s');
            $iniciativa_planejamento_insumosiniciativa->ipiinusucpfinativacao = $_SESSION['usucpf'];

            $sucesso = $iniciativa_planejamento_insumosiniciativa->salvar();
            $iniciativa_planejamento_insumosiniciativa->commit();


            $this->modelHistorico->gravarHistorico($iniciativa_planejamento_insumosiniciativa, 'DELETE');


        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        if ($sucesso) {
            $arrPost['inpid'];
            return (int)$sucesso;
        }
        return 'erro';
    }

    public function listaInsumosIniciativa($inpid, $buff = false)
    {
        $sql = "SELECT 
                  ipiin.ipiinid ,
                  ipiin.inpid ,
                  ipiin.ipiindescricao ,
                  ipiin.ipiinunidadeinsumo1 ,
                  ipiin.ipiinqtdinsumo1,
                  ipiin.ipiinunidadeinsumo2,
                  ipiin.ipiinqtdinsumo2,
                  ipiin.ipiinqtd,
                  ipiin.ipiinvalunit,
                  ipiin.ipiinvaltotal,
                  ipiin.ipiinstatus
FROM par3.iniciativa_planejamento_insumosiniciativa ipiin
INNER JOIN par3.iniciativa_planejamento ip ON ip.inpid = ipiin.inpid
WHERE ipiin.inpid = $inpid AND   ipiin.ipiinstatus = 'A' 
ORDER BY ipiin.ipiinid";

        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Descrição', 'Unidade Insumo 1', 'Qtd Insumo 1', 'Unidade Insumo 2', 'Qtd Insumo 2', 'Quantidade', 'Valor Unitário', 'Valor Total');
        $esconderColunas = array('ipiinid', 'inpid', 'ipiinstatus');
        if($buff){
            
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
        }else{
            
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        }
        $listagem->esconderColunas($esconderColunas);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addAcao('remove', 'removerInsumo');
        $listagem->addAcao('edit', 'editarInsumosIniciativa');
        $listagem->addCallbackDeCampo('ipiinvalunit', 'par3_mascaraMoeda');
        $listagem->addCallbackDeCampo('ipiinvaltotal', 'par3_mascaraMoeda');
        //$listagem->turnOnOrdenacao();
        return $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        //$listagem->render();
    }

}

?>