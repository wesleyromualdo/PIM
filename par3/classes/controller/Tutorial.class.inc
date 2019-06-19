<?php
/**
 * Classe de controle do Endereço
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2015
 * @link     no link
 */

/**
 * Par3_Controller_Evento
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2015
 * @link     no link
 */
class Par3_Controller_Tutorial
{
    /**
     * Função salvar
     * - retorna o evento da prefeitura.
     *
     * @return retorna um objeto evento prefeitura.
     *
     */
    public function salvar($dados)
    {
        
        try{
            
            $modelTutorial = new Par3_Model_Tutorial($tutorial['tutdescricao']);
            $modelTutorial->popularDadosObjeto($Tutorial);
            $modelTutorial->salvar();
            $modelTutorial->commit();

            simec_redirecionar('par3.php?modulo=principal/atendimento/tutoriais&acao=A', 'success');
        } catch (Exception $e){
            $modelTutorial->rollback();
            simec_redirecionar('par3.php?modulo=principal/atendimento/tutoriais&acao=A', 'error');
        }
    }

}
?>