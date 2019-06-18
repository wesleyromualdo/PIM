<?php
/**
 * Classe de controle do  abasacesso
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   JEFERSON CARLOS ARAÚJO <jeferson.araujo@castgroup.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-06-2017
 * @link     no link
 */
class Spoemendas_Controller_Abasacesso
{
    private $model;
    private $dados;

    public function __construct($dados = NULL)
    {
        $this->dados= $dados;
        $this->model = new Spoemendas_Model_Abaacesso();
        
    }

    /**
     * @param $apaid
     */
    public function salvar($apaid)
    {
        global $url;
        unset($this->dados['pflcod'], $this->dados['etoid'],$this->dados['apaid']);
        $this->model->removerAcessoAbas($apaid);
        foreach ($this->dados as $dado) {
            if ($dado) {
                $aba['apaid'] = $apaid;
                $aba['aacaba'] = $dado;
                try {
                    $this->model->salvarAbas($aba);
                } catch (Simec_Db_Exception $e) {
                    simec_redirecionar($url, 'error');
                }
            }
        }
        $this->model->commit();
    }

}
?>