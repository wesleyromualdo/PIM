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
class Par3_Controller_EventoTipo
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
            
            if (isset($dados['eventotipo']) && is_array($dados['eventotipo'])) {
                foreach ($dados['eventotipo'] as $eventotipo) {
                    $modelEventoTipo = new Par3_Model_EventoTipo($eventotipo['evtid']);
                    $modelEventoTipo->popularDadosObjeto($eventotipo);
                    $modelEventoTipo->salvar();
                }
                $modelEventoTipo->commit();
            }

            simec_redirecionar('par3.php?modulo=principal/calendario&acao=A', 'success');
        } catch (Exception $e){
            $modelEventoTipo->rollback();
            simec_redirecionar('par3.php?modulo=principal/calendario&acao=A', 'error');
        }
    }

    public function montarFormulario($dados)
    {
        global $simec;

        include_once APPRAIZ . 'par3/modulos/principal/eventotipo/formulario.php';
    }
}
?>