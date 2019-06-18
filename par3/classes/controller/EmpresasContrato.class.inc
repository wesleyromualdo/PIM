<?php
/**
 * Classe de controle do  empresas_contrato
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-03-2017
 * @link     no link
 */


/**
 * Par3_Controller_Empresas_contrato
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-03-2017
 * @link     no link
 */
class Par3_Controller_EmpresasContrato extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_EmpresasContrato($_GET['ecoid']);
    }

    /**
     * Função crudeEmpresas
     * - mostra crude em lista das empresas
     *
     */
    public function crudEmpresas($arrPost = null)
    {
        $model = new Par3_Model_EmpresasContrato();
        $mContratos = new Par3_Model_Contratos();

        $empresaContrato = array('economeempresa' => '', 'ecocnpjempresa' => '', 'conid' => null);
        if ($arrPost['ecoid']) {
            $empresaContrato = $mContratos->pegaLinha("SELECT * FROM par3.empresas_contrato WHERE ecoid = {$arrPost['ecoid']}");
            $mEmpresaContrato = new Par3_Model_EmpresasContrato();
        }

        $rsContratosSQL = $mContratos->pegarSQLSelectCombo($arrPost);

        $disabled = "readonly";
        $instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();

        global $simec;
        echo '<input type="hidden" name="ecoid" value="' . $empresaContrato['ecoid'] . '" />';
        echo '<div class="col-md-4">' . $simec->input('economeempresa', 'Nome Empresa', $empresaContrato['economeempresa'], array('maxlength' => '255', true, 'required')) . '</div>';
        echo '<div class="col-md-4">' . $simec->cnpj('ecocnpjempresa', 'CNPJ', $empresaContrato['ecocnpjempresa'], array('class' => 'cnpj', 'required')) . '</div>';
        echo '<div class="col-md-3">' . $simec->select('conid', 'Contrato', $empresaContrato['conid'], $rsContratosSQL, Array('required')) . '</div>';

    }

    public function listaEmpresas($arrPost)
    {
        global $disabled;
        $mEmpresaContrato = new Par3_Model_EmpresasContrato();
        $sql = $mEmpresaContrato->montarSQLSimplesEmpresas($arrPost);
        $cabecalho = array('Nome Empresa', 'CNPJ Empresa', 'Nº Contrato');

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas(['id']);
        $listagem->addCallbackDeCampo('ecocnpjempresa', 'formatar_cnpj'); // função php para formatar cpf
        if ($disabled == '') {
            $listagem->addAcao('remove', 'inativarEmpresa');
            $listagem->addAcao('edit', 'editarEmpresa');
        }
        $listagem->turnOnOrdenacao();
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }//end listaEquipe()

    /**
     * Função gravar
     * - grava os dados
     *
     */

    public function salvar($arrPost)
    {
        global $url;
//        $this->model = new Par3_Controller_Empresas_contrato();
        $arrPost['usucpf'] = $_SESSION['usucpf'];
        $arrPost['economeempresa'] = ($arrPost['economeempresa']);
        $arrPost['ecocnpjempresa'] = str_replace(array('.', '-', '/'), '', $arrPost['ecocnpjempresa']);
        $arrPost['ecostatus'] = 'A';
        $arrPost['ecodtcricao'] = date('Y-m-d H:m:s');
        $this->model->popularDadosObjeto($arrPost);
        $url = "par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid=" . $arrPost['inuid'];
        $url .= '&ecoid=' . $this->model->ecoid;

        try {
            $sucesso = $this->model->salvar($arrPost);
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            return false;
        }
        if ($sucesso) {
            return true;
        }
        return false;
    }


    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($arrPost)
    {
        global $url;
        $id = $arrPost['ecoid'];
        $url = "par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid=" . $arrPost['inuid'];
        $empresas_contrato = new Par3_Model_EmpresasContrato($id);
        try {
            $empresas_contrato->ecostatus = 'I';

            $empresas_contrato->salvar();
            $empresas_contrato->commit();
            return true;
//            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            return false;
//            simec_redirecionar($url, 'error');
        }
    }


}

?>