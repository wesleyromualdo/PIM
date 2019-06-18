<?php

/**
 * Par3_Controller_Iniciativa_programas_termo_adesao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 26-04-2017
 * @link     no link
 */
class Par3_Controller_ProgramaAdesao extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Proadesao($_REQUEST['pfaid']);
        $this->modelHistorico = new Par3_Model_ProAdesaoHistorico();
    }

    public function form($arrPost)
    {
        if ($arrPost['prgid']) {
            $this->model->prgid = $arrPost['prgid'];
        }

        global $simec;

        $disabled = false;
        $radioStatus = array('A' => 'Sim', 'I' => 'Não');
        $selectEsfera = array('T' => 'Todos', 'E' => 'Estadual', 'M' => 'Municipal');
        $selectUFs = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY 2";
        $sql = "SELECT
                                        prgid as codigo,
                                        prgdsc as descricao
                                    FROM par3.programa
                                   ";
        echo '<input type="hidden" name="pfaid" value="' . $this->model->pfaid . '"/>';
        echo $simec->select("prgid", 'Programa', $this->model->prgid, $sql, array("required"));
        echo $simec->input('pfaano', 'Ano', $this->model->pfaano, array('maxlength' => '4', 'required'));
        echo $simec->data('pfadatainicial', 'Data de início', $this->model->pfadatainicial, array($disabled, 'required'));
        echo $simec->data('pfadatafinal', 'Data final', $this->model->pfadatafinal, array($disabled, 'required'));
        echo $simec->input('pfaicone', 'Nome do ícone', $this->model->pfaicone, array('maxlength' => '255'));
        echo $simec->input('pfafundocor', 'Cor de fundo', $this->model->pfafundocor, array('maxlength' => '255'));
        echo $simec->input('pfabackground', 'Imagem de fundo', $this->model->pfabackground, array('maxlength' => '255'));
        echo $simec->select("pfaesfera", 'Esfera', $this->model->pfaesfera, $selectEsfera, array("required"));
        echo $simec->select("ufs", 'UFs', $this->model->recuperaUFs($this->model->pfaid), $selectUFs, array("multiple"));
        echo $simec->radio('pfastatus', 'Situação', (!($this->model->pfastatus) ? 'A' : $this->model->pfastatus), $radioStatus, array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
        echo '<div class="form-group ">
                                        <label for="pfaicone" class="col-sm-3 col-md-3 col-lg-3 control-label">Variáveis possíveis nos termos de Adesão: </label>
                                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                                #nome_entidade#<br>
                                                #data_atual#<br>
                                                #dirigente#<br>
                                                #dirigente_cpf#<br>
                                                #escolas_prioritarias#
                                            </div>
                                        </div>';
        echo $simec->textarea('pfatermomunicipio', 'Termo de Adesão do Município', $this->model->pfatermomunicipio, array('class' => 'summernote'));
        echo $simec->textarea('pfatermoestado', 'Termo de Adesão do Estado', $this->model->pfatermoestado, array('class' => 'summernote'));
        echo $simec->textarea('pfamsgboasvindas', 'Mensagem de Boas vindas da adesão', $this->model->pfamsgboasvindas, array('class' => 'summernote'));
        echo $simec->textarea('pfamsgdeaceitacao', 'Mensagem que será exibida depois da aceitação da entidade', $this->model->pfamsgdeaceitacao, array('class' => 'summernote'));
        echo $simec->textarea('pfamsgdenaoaceitacao', 'Mensagem que será exebida caso a entidade não aceite o termo', $this->model->pfamsgdenaoaceitacao, array('class' => 'summernote'));
        echo $simec->textarea('pfasobre', 'Texto sobre a adesão do programa', $this->model->pfasobre, array('class' => 'summernote'));


    }

    public function salvar($arrPost)
    {
//ver($arrPost, d);
        //define acão para guardar em histórico
        $acao = Par3_Model_ProAdesaoHistorico::CREATE;
        if ($arrPost['pfaid']) {
            $arrTermoAdesao['pfaid'] = trim($arrPost['pfaid']);
            $acao = Par3_Model_ProAdesaoHistorico::UPDATE;
        }


        $arrTermoAdesao['prgid'] = $arrPost['prgid'];
        $arrTermoAdesao['pfaano'] = trim($arrPost['pfaano']);
        $arrTermoAdesao['pfaicone'] = trim($arrPost['pfaicone']);
        $arrTermoAdesao['pfaesfera'] = trim($arrPost['pfaesfera']);
        $arrTermoAdesao['pfastatus'] = trim($arrPost['pfastatus']);
        $arrTermoAdesao['pfatermoestado'] = trim(($arrPost['pfatermoestado']));
        $arrTermoAdesao['pfatermomunicipio'] = trim(($arrPost['pfatermomunicipio']));
        $arrTermoAdesao['pfamsgboasvindas'] = trim(($arrPost['pfamsgboasvindas']));
        $arrTermoAdesao['pfamsgdeaceitacao'] = trim(($arrPost['pfamsgdeaceitacao']));
        $arrTermoAdesao['pfamsgdenaoaceitacao'] = trim(($arrPost['pfamsgdenaoaceitacao']));
        $arrTermoAdesao['pfafundocor'] = trim($arrPost['pfafundocor']);
        $arrTermoAdesao['pfabackground'] = trim($arrPost['pfabackground']);
        $arrTermoAdesao['pfasobre'] = trim($arrPost['pfasobre']);
        $arrTermoAdesao['pfadatafinal'] = $arrPost['pfadatafinal'];
        $arrTermoAdesao['pfadatainicial'] = $arrPost['pfadatainicial'];


        $erro = $this->model->validarInput($arrTermoAdesao);
        if ($erro) {
            return $erro;
        }

        $arrTermoAdesao['pfadatafinal'] = formata_data_sql($arrTermoAdesao['pfadatafinal']);
        $arrTermoAdesao['pfadatainicial'] = formata_data_sql($arrTermoAdesao['pfadatainicial']);

        try {
            $this->model->popularDadosObjeto($arrTermoAdesao);
            $novoTermo = $this->model->salvar();
            $this->model->commit();
            
            $this->model->salvarUFs($arrPost);

            $modelTermo = new Par3_Model_Proadesao($novoTermo);//recupera Termo de adesão
            $modelHistorico = new Par3_Model_ProAdesaoHistorico();
            $modelHistorico->gravarHistorico($modelTermo, $acao);//grava em hitórico
            $modelHistorico->salvar();
            $modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return $e->getMessage();
        }
        return $novoTermo;
    }

    public function remover($arrPost)
    {
        $arrTermoAdesao['pfaid'] = $arrPost['pfaid'];
        $arrTermoAdesao['pfastatus'] = 'I';


        try {
            $this->model->popularDadosObjeto($arrTermoAdesao);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelTermoAdesao = new Par3_Model_Proadesao($arrTermoAdesao['pfaid']);

            $modelHistorico = new Par3_Model_ProAdesaoHistorico();
            $modelHistorico->gravarHistorico($modelTermoAdesao, Par3_Model_ProAdesaoHistorico::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    function listar($arrPost)
    {
        global $disabled;
        $proadesaoL = new Par3_Model_Proadesao();
        $sql = $proadesaoL->montarSQLSimples($arrPost);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        $tratamentoListaVazia = ($arrPost['req'] == '') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $cabecalho = array('Ano de Referência', 'Data de início', 'Data de fim', 'Situação');
        $esconderColunas = array('id', 'codigo');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('remove', 'removerTermoAdesao');
        $listagem->addAcao('edit', 'editarTermoAdesao');
        $listagem->addCallbackDeCampo('pfastatus', 'formata_status');
        $listagem->setTamanhoPagina(10);
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

}
