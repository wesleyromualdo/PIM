<?php
/**
 * Classe de controle do  modalidade
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-04-2017
 * @link     no link
 */



/**
 * Par3_Controller_Modalidade
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-04-2017
 * @link     no link
 */
require_once APPRAIZ . "includes/funcoesspo_componentes.php";

class Par3_Controller_Modalidade extends Controle
{
    private $model;
    private $modelEnsinoModalidade;

    public function __construct()
    {
        $modid = validarInteiro($_REQUEST['modid'])? $_REQUEST['modid']:'';
        $this->model = new Par3_Model_Modalidade($modid);
        $this->modelHistorico = new Par3_Model_ModalidadeHistorico();
        $this->modelEnsinoModalidade = new Par3_Model_EnsinoModalidade();
    }


    public function form($arrPost)
    {
        $modelNivel        = new Par3_Model_EnsinoNivel();
        $modelEtapa        = new Par3_Model_EnsinoEtapa();
        $modelEnsinoModelo = new Par3_Model_EnsinoModalidade();
        $sqlNivel   = $modelNivel->pegarSQLSelectCombo();
        $sqlEtapa   = array();
        if($arrPost['modid']){
            $arrModalidadeNiveisEtapas = $modelEnsinoModelo->getModalidadeNiveisEtapas($arrPost['modid']);
            $arrNiveisSalvos = postgres_to_php_array($arrModalidadeNiveisEtapas['niveis']);
            $arrEtapasSalvas = postgres_to_php_array($arrModalidadeNiveisEtapas['etapas']);
            $sqlEtapa   = $modelEtapa->pegarSQLSelectCombo($arrNiveisSalvos);
        }
        global $simec;
        echo '<input type="hidden" name="modid" value="'.$this->model->modid.'"/>';
        echo '<div class="form-group"><label class="col-lg-3 control-label">Código:</label>';
        echo '<div class="col-lg-2"><input type="text" disabled="" value="'.$this->model->modid.'" class="form-control"></div>';
        echo '</div>';

//        echo '<div class="form-group"><label class="col-lg-3 control-label">Nível:</label><div class="col-sm-9 col-md-9 col-lg-9 ">';
//        echo inputCombo("nivid[]", $sqlNivel,$arrNiveisSalvos,'nivid', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
//        echo '</div></div>';
        echo $simec->select('nivid[]', 'Nível', $arrNiveisSalvos, $sqlNivel, array('maxlength' => '255','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '9'));

//        echo '<div class="form-group"><label class="col-lg-3 control-label">Etapa:</label><div class="col-sm-9 col-md-9 col-lg-9 ">';
//        echo inputCombo("etaid[]", $sqlEtapa,$arrEtapasSalvas,'etaid', array('multiple'=> 'multiple', 'titulo'=>'Selecione'));
//        echo '</div></div>';
        echo $simec->select('etaid[]', 'Etapa', $arrEtapasSalvas, $sqlEtapa, array('maxlength' => '255','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '9'));

        echo $simec->input('moddsc', 'Descrição', $this->model->moddsc, array('maxlength' => '255','required','title' => 'Insira a descrição da modalidade'));
        echo $simec->radio('modsituacao', 'Situação',(!($this->model->modsituacao)?'A':$this->model->modsituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_ModalidadeHistorico::CREATE;
        if($arrPost['modid']){
        $arrayModalidade['modid'] = $arrPost['modid'];
            $acao = Par3_Model_ModalidadeHistorico::UPDATE;
        }

        $arrayModalidade['modsituacao']    = $arrPost['modsituacao'];
        $arrayModalidade['modstatus']      = 'A';
        $arrayModalidade['moddsc']         = trim(($arrPost['moddsc']));
        $arrayModalidade['modcpfinclusao'] = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
        $arrayModalidade['nivid']          = prepararArraySelectMultiSelecao($arrPost['nivid']);
        $arrayModalidade['etaid']          = prepararArraySelectMultiSelecao($arrPost['etaid']);

        if(!$arrPost['modid']){$arrayModalidade['moddtinclusao']  = date('Y-m-d H:m:s');}

        //Valida campos
        $erro = $this->model->validarInput($arrayModalidade);
        if($erro){return $erro;}

        try {
            $this->model->popularDadosObjeto($arrayModalidade);
            $id =  $this->model->salvar();
            $this->model->commit();

            //Salvar Ensino Modalidades
            $mEnsinoModalidade = new Par3_Model_EnsinoModalidade();
            $mEnsinoModalidade->salvarEnsinoModalidades($id,$arrayModalidade['etaid']);

            $modelModalidade= new Par3_Model_Modalidade($id);

            $this->modelHistorico->gravarHistorico($modelModalidade,$acao);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {return 'erro';}
        if ($id) {return $id;}
        return 'erro';
    }

    public function desvincular($arrPost)
    {
        $arrayModalidade['modid']           = $arrPost['modid'];
        $arrayModalidade['modsituacao']     = 'I';
        $arrayModalidade['moddtinativacao'] = date('Y-m-d H:m:s');

        try {
            $this->model->popularDadosObjeto($arrayModalidade);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelModalidade = new Par3_Controller_Modalidade($sucesso);

            $this->modelHistorico->gravarHistorico($modelModalidade,Par3_Model_Modalidade_historico::DELETE);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {return 'erro';}
        if ($sucesso) {return $sucesso;}
        return 'erro';
    }

    public function remover($arrPost)
    {
        $arrayModalidade['modid']             = $arrPost['modid'];
        $arrayModalidade['modstatus']         = 'I';
        $arrayModalidade['moddtinativacao']   = date('Y-m-d H:m:s');
        $arrayModalidade['modcpfinativacao']  = $_SESSION['usucpf'];

        try {
            $this->model->popularDadosObjeto($arrayModalidade);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelModalidade = new Par3_Model_Modalidade($arrPost['modid']);

            $this->modelHistorico->gravarHistorico($modelModalidade,Par3_Model_CicloParHistorico::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    public function sqlEnsinoNiveis()
    {
        $modelNivel = new Par3_Model_EnsinoNivel();
        return $modelNivel->pegarSQLSelectCombo();
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function listar($arrPost)
    {
        $arrPost['nivid'] = $arrPost['nivel'];
        $arrPost['etaid'] = $arrPost['etapa'];
        global $disabled;
        $modelModalidade = new Par3_Model_Modalidade();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = 'codigo';
                break;
            case 'etapas':
                $arrQuery['ordenacao']['campo'] = 'etapas';
                break;
            case 'niveis':
                $arrQuery['ordenacao']['campo'] = 'niveis';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = 'descricao';
                break;
            case 'modsituacao':
                $arrQuery['ordenacao']['campo'] = 'modsituacao';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }

        $sql = $modelModalidade->montarSQLSimples($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,descrição,situção
        $cabecalho = array('Código','Modalidade','Nível','Etapa','Situação');

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
        $listagem->setId('modalidade');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('modsituacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'js_verificaRemoverModalidade');
            $listagem->addAcao('edit', 'editarModalidade');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    public function getModalidadeById($modid){
        $result = $this->model->getModalidadeById($modid);
        return $result;
    }

    public function verificaLigacaoPlanejamento($modid){
        $result = $this->model->verificaLigacaoPlanejamento($modid);
        return $result;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     *
     */
    public function getFormListModalidade($modid = null, $etaid = null, $nivid = null){
        return $this->model->getFormListModalidade($modid, $etaid, $nivid);
    }

    public function getFormListModalidadeByEnsinoEtapa(array $array){
        return $this->modelEnsinoModalidade->getFormListModalidadeByEnsinoEtapa($array);
    }
}
?>