<?php
/**
 * Classe de controle do  unidade_medida
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-05-2017
 * @link     no link
 */



/**
 * Par3_Controller_Unidade_medida
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-05-2017
 * @link     no link
 */
class Par3_Controller_UnidadeMedidaClasse extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_UnidadeMedidaClasse($_GET['unciid']);
    }


    public function marcarTodos($arrPost)
    {
        $uniid = $arrPost['uniid'];
        $desmarcar = false;
        if($arrPost['desmarcar'] == 'F'){
            $desmarcar = true;
        }
        return $this->model->marcarTodosToggle($uniid,$desmarcar);
    }

    public function salvar($arrPost)
    {
        $cniid = $arrPost['cniid'];
        $uniid = $arrPost['uniid'];

        $rsUnc = $this->model->recuperar("SELECT * FROM par3.unidade_medida_classe WHERE cniid = {$cniid} AND uniid = {$uniid}");
        $rsUnc['uncstatus'] = $rsUnc['uncstatus'] == 'A'? 'I' :'A';
        if(!$rsUnc['uncid']){
            $rsUnc['cniid']     = $cniid;
            $rsUnc['uniid']     = $uniid;
            $rsUnc['uncstatus'] = 'A';
        }

        try {
            $this->model->popularDadosObjeto($rsUnc);
            $novoUnidadeMedida =  $this->model->salvar();//grava Unidade de Medida Classe
            $this->model->commit();

        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($novoUnidadeMedida) {
            return $novoUnidadeMedida;
        }
        return 'erro';
    }
}
?>