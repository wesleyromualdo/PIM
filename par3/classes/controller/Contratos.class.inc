<?php
/**
 * Classe de controle do  contratos
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-03-2017
 * @link     no link
 */


/**
 * Par3_Controller_Contratos
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 02-03-2017
 * @link     no link
 */
class Par3_Controller_Contratos extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Contratos($_GET['conid']);
    }

    /**
     * Função formNovoContrato
     * - grava os dados
     *
     */
    public function formNovoContrato()
    {
        global $simec;

        $sqlAnos = "SELECT
                    	ano as codigo,
                    	ano as descricao
                    FROM public.anos
                    WHERE ano::integer <= (to_char(now()::date, 'YYYY'))::integer";

        echo $simec->select('conano', 'Ano do Contrato', date('Y'), $sqlAnos, array($disabled, 'required'));
        echo $simec->input('connumero', 'Número Contrato', '', array('maxlength' => '255',"type"=>'number',$disabled,'required'));
    }


    public function listarContratos($arrPost)
    {
        global $disabled;
        $mContrato = new Par3_Model_Contratos();
        $sql = $mContrato->pegarSQLSelectListagem($arrPost);
        $cabecalho = array('N° Contrato');
        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas(['id']);
//        if ($disabled == '') {
//            $listagem->addAcao('remove', 'inativarVeiculo');
//            $listagem->addAcao('edit', 'editarVeiculo');
//        }
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

        $this->model->popularDadosObjeto($arrPost);

        try {
            $sucesso = $this->model->salvar();
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            return $e;
        }

        if ($sucesso) {
            return 'sucesso';
        }

        return  'erro';
    }


    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['conid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&conid={$id}";
        $contratos = new Par3_Model_Contratos($id);
        try {
            $contratos->Xstatus = 'I';
            $contratos->Xdtinativacao = date('Y-m-d H:i:s');
            $contratos->Xusucpfinativacao = $_SESSION['usucpf'];

            $contratos->salvar();
            $contratos->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }


}

?>