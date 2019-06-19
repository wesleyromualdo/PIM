<?php

/**
 * Par_Controle_Instrumentounidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-09-2015
 * @link     no link
 */
class Par3_Controller_Inicio extends Controle
{

    public function __construct()
    {
        parent::__construct();
    }

    public function salvar($dados)
    {
        try {

            $inicio = new Par3_Model_Inicio();
            $inicio->excluirVarios( "1=1" );
            $inicio->commit();

            foreach ($dados['inicio'] as $key => $id) {

                if(empty($id))
                {
                    $id = 16681;
                }
                unset($inicio);
                $inicio = new Par3_Model_Inicio();
                $inicio->popularDadosObjeto(array("pflcod" => $key, "mnuid" => $id));
                $inicio->salvar();
                $inicio->commit();
            }
            simec_redirecionar('par3.php?modulo=sistema/perfil/parInicio&acao=A', 'success','Operação Realizada com Sucesso');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar('par3.php?modulo=sistema/perfil/parInicio&acao=A', 'error');
        }
    }

}//end class

?>