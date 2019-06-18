<?php
/**
 * Classe de controle do  ResponsavelContrato
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-03-2017
 * @link     no link
 */



/**
 * Par3_Controller_ResponsavelContrato
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-03-2017
 * @link     no link
 */
class Par3_Controller_ResponsavelContrato
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ResponsavelContrato($_POST['rcoid']);
    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        $arrDados = array();
        $arrDados['conid']      = $_REQUEST['conid'];
        $arrDados['rconome']    = str_replace(array("'"), "", $_REQUEST['rconome']);
        $arrDados['rcocpf']     = str_replace(array(".", "-"), "", $_REQUEST['rcocpf']);
        $arrDados['rcocargo']   = str_replace(array("'"), "", $_REQUEST['rcocargo']);
        $arrDados['rcostatus']  = 'A';
        $arrDados['usucpf']     = $_SESSION['usucpf'];

        $this->model->popularDadosObjeto($arrDados);

        try{
            $sucesso = $this->model->salvar();
            $this->model->commit();
            return true;
        } catch (Simec_Db_Exception $e) {
            return false;
        }
    }


   /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_POST['rcoid'];
        $ResponsavelContrato = new Par3_Model_ResponsavelContrato($id);
        try{
            $ResponsavelContrato->rcostatus = 'I';

            $ResponsavelContrato->salvar();
            $ResponsavelContrato->commit();

            return true;
        } catch (Simec_Db_Exception $e) {
            return false;
        }
    }

    /**
     * Função crudResponsavel
     * - monda form de cadastro e edição de Responsáveis
     *
     */
    public function crudResponsavel($arrPost = null)
    {
        $mContratos = new Par3_Model_Contratos();

        $rsContratosSQL = $mContratos->pegarSQLSelectCombo($arrPost);

//         $disabled = "readonly";
        $instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();

        global $simec;
        echo '<form method="POST" id="formResponsavel" action="" class="form form-horizontal">';
        echo '<div class="row">';

        echo '<div class="col-md-2"><label>Contrato:</label><br>'.$simec->select('conid', '',$this->model->conid, $rsContratosSQL, Array('required')).'</div>';

        echo '<div class="col-md-3">';
        echo '<input type="hidden" name="req" value="salvarResponsavel"/>';
        echo '<input type="hidden" name="rcoid" value="'.$this->model->getPK().'" />';
		echo '<input type="hidden" name="rcocpf_old" value="'.formatar_cpf($objPessoaFisica->rcocpf).'"/>';
		echo '<label>CPF:</label><br>';
		echo $simec->cpf('rcocpf', '', $this->model->rcocpf, array('class' => 'cpf', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{"#rconome": "no_pessoa_rf"}', 'required'));
		echo '</div>';

		echo '<div class="col-md-3">';
		echo '<label>Nome:</label><br>';
        echo $simec->input('rconome', '', $this->model->rconome, array('maxlength' => '255', 'required', 'readonly'));
		echo '</div>';

		echo '<div class="col-md-3">';
		echo '<label>Cargo:</label><br>';
        echo $simec->input('rcocargo', '', $this->model->rcocargo, array('maxlength' => '255', $disabled, 'required'));
		echo '</div>';

		echo '<div class="col-md-1"><br>';
        echo '<button type="button" class="btn btn-success salvar_responsavel" title="Cadastrar Responsável"><i class="fa fa-save"></i></button>';
		echo '</div>';

        echo '</div>';
        echo '</form>';
    }//end crudResponsavel()

    /**
     * Função listaResponsaveis
     * - lista responsáveis por contrato
     *
     */
    public function listaResponsaveis($arrPost)
    {
        global $disabled;
        $sql = $this->model->montarSQLSimples($arrPost);
        $cabecalho = array('Nº Contrato', 'Nome', 'CPF', 'Cargo');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas(['id']);
        $listagem->addCallbackDeCampo('rcocpf','formatar_cpf'); // função php para formatar cpf
        if($disabled == ''){
            $listagem->addAcao('remove', 'inativarResponsavel');
            $listagem->addAcao('edit', 'editarResponsavel');
        }
        $listagem->turnOnOrdenacao();
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }//end listaResponsaveis()

}
?>