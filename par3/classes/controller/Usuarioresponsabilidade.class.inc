<?php
/**
 * Classe de controle do  usuarioresponsabilidade
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   ELIAS LOURENÇO DE OLIVEIRA <elias.oliveira@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-02-2017
 * @link     no link
 */



/**
 * Par3_Controller_Usuarioresponsabilidade
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-02-2017
 * @link     no link
 */ 
class Par3_Controller_Usuarioresponsabilidade
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Usuarioresponsabilidade($_GET['rpuid']);
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Par3_Controller_Usuarioresponsabilidade();
        $this->model->popularDadosObjeto();
        $url .= '&rpuid=' . $this->model->rpuid;

        try{
            $sucesso = $this->model->salvar();
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if($sucesso){
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['rpuid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&rpuid={$id}";
        $usuarioresponsabilidade = new Par3_Model_Usuarioresponsabilidade($id);
        try{
             $usuarioresponsabilidade->Xstatus = 'I';
             $usuarioresponsabilidade->Xdtinativacao = date('Y-m-d H:i:s');
             $usuarioresponsabilidade->Xusucpfinativacao = $_SESSION['usucpf'];

             $usuarioresponsabilidade->salvar();
             $usuarioresponsabilidade->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }
	
    /**
     * Função para atualizar os dados relativos ao usuário responsabilidade do PAR 3
     *
     *
     */
    public function atualizaUsuarioResponsabilidadeRotina($dados){
    	
    	/**INATIVA OS ANTERIORES DO CARGO */
    	$inativou = $this->model->inativaUsuarioResponsabilidadeRotina($dados);
    	
    	/**CASO TENHA INATIVADO CORRETAMENTE INSERE O REGISTRO ATUAL*/
    	if( $inativou )
    	{
    		if( $this->model->insereUsuarioResponsabilidadeRotina($dados) )
    		{
    			return $this->model->commit();
    		}
    		else 
    		{
    			return false;
    		}
    	}
    	else
    	{
    		return false;
    	}
    }

}
?>