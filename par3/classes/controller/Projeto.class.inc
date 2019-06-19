<?php
/**
 * Classe de controle do  projeto
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-04-2017
 * @link     no link
 */



/**
 * Par3_Controller_Projeto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-04-2017
 * @link     no link
 */
class Par3_Controller_Projeto extends Controle
{
    private $model;

    public function __construct()
    {
        $proid = validarInteiro($_REQUEST['proid'])? $_REQUEST['proid']:'';
        $this->model = new Par3_Model_Projeto($proid);
        $this->modelHistorico = new Par3_Model_ProjetoHistorico();
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */

    public function form($arrPost)
    {
        global $simec;
        echo '<input type="hidden" name="proid" value="'.$this->model->proid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->proid.'" class="form-control"></div>';
        echo '</div>';
        echo $simec->input('prosigla', 'Sigla', $this->model->prosigla, array('maxlength' => '100', 'required', 'title' => 'Insira a descrição do projeto'));
        echo $simec->input('prodsc', 'Descrição', $this->model->prodsc, array('maxlength' => '255','required','title' => 'Insira a sigla do projeto'));
        echo $simec->input('prolink', 'Link', $this->model->prolink, array('maxlength' => '255','title' => 'Insira o link do projeto.'));
        echo $simec->textarea('proresumo', 'Resumo', $this->model->proresumo, array('required','title' => 'Insira o resumo do projeto.'));
        $vigencia =  array('VIGENTE' => 'Vigente', 'FECHADO' => 'Fechado','NAO_INICIADO' =>'Não Iniciado');
//        if ($arrPost['proid']){
        echo $simec->radio('prosituacao', 'Situação',(!($this->model->prosituacao)?'A':$this->model->prosituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
//        }
    }
    public function salvar($arrPost)
    {
        $acao = Par3_Model_IniciativaDescricaoHistorico::CREATE;
        if($arrPost['indid']){
            $acao = Par3_Model_IniciativaDescricaoHistorico::UPDATE;
        }

        $arrayProjeto['proid']           = $arrPost['proid'];
        $arrayProjeto['prosituacao']     = $arrPost['prosituacao'];
        $arrayProjeto['prostatus']       = 'A';
        $arrayProjeto['prodsc']          = trim(($arrPost['prodsc']));
        $arrayProjeto['proresumo']       = trim(($arrPost['proresumo']));
        $arrayProjeto['prolink']         = trim(($arrPost['prolink']));
        $arrayProjeto['prosigla']        = trim(($arrPost['prosigla']));

        $arrayProjeto['procpfinclusao'] = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);

        if(!$arrPost['proid']){
            $arrayProjeto['prodtinclusao']  = date('Y-m-d H:m:s');
            $arrayProjeto['proid']          = $arrPost['proid'];
        }

        //Valida campos
        $erro = $this->model->validarInput($arrayProjeto);
        if($erro){return $erro;}

        try {
            $this->model->popularDadosObjeto($arrayProjeto);
            $id =  $this->model->salvar();
            $this->model->commit();

            $modelProjeto = new Par3_Model_Projeto($id);

            $this->modelHistorico->gravarHistorico($modelProjeto,$acao);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {return 'erro';}
        if ($id) {return $id;}
        return 'erro';
    }

    public function desvincular($arrPost)
    {
        $arrayProjeto['proid']           = $arrPost['proid'];
        $arrayProjeto['prosituacao']     = 'I';
        $arrayProjeto['prodtinativacao'] = date('Y-m-d H:m:s');

        try {
            $this->model->popularDadosObjeto($arrayProjeto);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelProjeto = new Par3_Controller_Projeto($sucesso);

            $this->modelHistorico->gravarHistorico($modelProjeto,Par3_Model_Projeto_historico::DELETE);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {return 'erro';}
        if ($sucesso) {return $sucesso;}
        return 'erro';
    }

    public function remover($arrPost)
    {
        $arrayProjeto['proid']             = $arrPost['proid'];
        $arrayProjeto['prostatus']         = 'I';
        $arrayProjeto['prodtinativacao']   = date('Y-m-d H:m:s');
        $arrayProjeto['procpfinativacao']  = $_SESSION['usucpf'];

        try {
            $this->model->popularDadosObjeto($arrayProjeto);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelProjeto = new Par3_Model_Projeto($arrPost['proid']);

            $this->modelHistorico->gravarHistorico($modelProjeto,Par3_Model_CicloParHistorico::DELETE);
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

    public function listar($arrPost)
    {
        global $disabled;
        $modelProjeto = new Par3_Model_Projeto();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'proid';
                break;
            case 'sigla':
                $arrQuery['ordenacao']['campo'] = 'prosigla';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'prodsc';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }

        $sql = $modelProjeto->montarSQLSimples($arrQuery);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,sigla,descrição,situção
        $cabecalho = array('Código', 'Sigla','Descrição','Situação');
        $esconderColunas = array('id','qtd');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            $esconderColunas = array('id');
            array_unshift($cabecalho,'QTD');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }
        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('projeto');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('prosituacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'js_verificaRemoverProjeto');
            $listagem->addAcao('edit', 'editarProjeto');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }


    public function getIniciativaProjetoById($proid){
        $result = $this->model->getIniciativaProjetoById($proid);
        return $result;
    }

    public function verificaLigacaoPlanejamentoById($proid){
        return $this->model->verificaLigacaoPlanejamentoById($proid);

    }

}
?>