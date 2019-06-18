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
class Par3_Controller_IniciativaProgramasTermoAdesao extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaProgramasTermoAdesao($_REQUEST['terid']);
        $this->modelHistorico = new Par3_Model_IniciativaProgramasTermoAdesaoHistorico();
    }

    public function form($arrPost)
    {
        $anos = array();
        $anoAtual = '2017';
        $anoFim   = $anoAtual + 15;
        for($anoAtual;$anoAtual < $anoFim;$anoAtual++)
        {
            $anos += array($anoAtual => $anoAtual);
        }
//        select * from workflow.tipodocumento;
        $sql = 'select tpdid as codigo,tpddsc as descricao from workflow.tipodocumento';
        global $simec;
        echo '<input type="hidden" name="terid" value="'.$this->model->terid.'"/>';
        echo '<input type="hidden" name="inprgid" value="'.$_REQUEST['inprgid'].'"/>';
        echo '<input type="hidden" name="tpdid" value="'.$this->model->tpdid.'"/>';
        echo $simec->textarea('terpacto', 'Texto do Pacto',  $this->model->terpacto, array('class' => 'summernote','required'));
        echo $simec->textarea('tertermo', 'Texto do Termo de Adesão',  $this->model->tertermo, array('class' => 'summernote','required'));

        echo '<div class="ibox float-e-margins">';
        echo '<div class="ibox-title "><div class="ibox-tools pull-left"><a id="btnMensagensTermo"> Mensagens <i id="chevronTermo" class="fa fa-chevron-down"></i></a></div></div>';
        echo '<div class="ibox-content" id="mensagensTermo" style="display: none;">';
        echo $simec->textarea('teraceitepacto', 'Mensagem de Aceite - Pacto',  $this->model->teraceitepacto);
        echo $simec->textarea('ternaoaceitepacto', 'Mensagem de Não Aceite - Pacto',  $this->model->ternaoaceitepacto);
        echo $simec->textarea('teraceitetermo', 'Mensagem de Aceite - Termo',  $this->model->teraceitetermo);
        echo $simec->textarea('ternaoaceitetermo', 'Mensagem de Não Aceite - Termo',  $this->model->ternaoaceitetermo);
        echo $simec->textarea('teremailpreanalise', 'Mensagem de E-mail -Após envio para Pré-Análise',  $this->model->teremailpreanalise);
        echo '</div></div>';

        echo '<div class="ibox float-e-margins">';
        echo '<div class="ibox-title"><div class="ibox-tools pull-left"><a id="btnOutrosTermo"> Outros <i id="chevronOutrosTermo" class="fa fa-chevron-down"></i></a></div></div>';
        echo '<div class="ibox-content" id="outrosTermo" style="display: none;">';
        echo $simec->select('teranoreferencia', 'Ano de Referência', $this->model->teranoreferencia ,$anos, array('maxlength' => '255','required'));

        echo $simec->select('teresfera', 'Esfera', $this->model->teresfera ,array(1 =>'Estadual',2 => 'Municipal'), array('maxlength' => '255','required'));
        echo $simec->select('tpdid', 'Fluxo Workflow', $this->model->tpdid ,$sql, array('maxlength' => '255'));
        echo $simec->input('tercabecalhoadesao', 'Título Cabeçalho de Adesão',  $this->model->tercabecalhoadesao, array('maxlength' => '255','required'));
        echo $simec->textarea('terinstrucoes', 'Instruções',  $this->model->terinstrucoes);
        echo $simec->textarea('termsgbemvindo', 'Mensagens de Bem Vindo',  $this->model->termsgbemvindo);
        echo $simec->input('terredirecionamentoadesao', 'Redirecionamento após Adesão',  $this->model->terredirecionamentoadesao, array('maxlength' => '255'));
        echo '</div></div>';
        echo $simec->radio('tersituacao', 'Situação',(!($this->model->tersituacao)?'A':$this->model->tersituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));

    }

    public function salvar($arrPost)
    {
        //define acão para guardar em histórico
        $acao = Par3_Model_IniciativaProgramasTermoAdesaoHistorico::CREATE;
        if($arrPost['terid']){
            $arrTermoAdesao['terid'] = trim($arrPost['terid']);
            $acao = Par3_Model_IniciativaProgramasTermoAdesaoHistorico::UPDATE;
        }

        $arrTermoAdesao['terpacto']                  = trim($arrPost['terpacto']);
        $arrTermoAdesao['tertermo']                  = trim($arrPost['tertermo']);
        $arrTermoAdesao['teraceitepacto']            = trim($arrPost['teraceitepacto']);
        $arrTermoAdesao['ternaoaceitepacto']         = trim($arrPost['ternaoaceitepacto']);
        $arrTermoAdesao['teraceitetermo']            = trim($arrPost['teraceitetermo']);
        $arrTermoAdesao['ternaoaceitetermo']         = trim($arrPost['ternaoaceitetermo']);
        $arrTermoAdesao['teremailpreanalise']        = trim($arrPost['teremailpreanalise']);
        $arrTermoAdesao['teranoreferencia']          = trim($arrPost['teranoreferencia']);
        $arrTermoAdesao['teresfera']                 = trim($arrPost['teresfera']);
        $arrTermoAdesao['tercabecalhoadesao']        = trim($arrPost['tercabecalhoadesao']);
        $arrTermoAdesao['terinstrucoes']             = trim($arrPost['terinstrucoes']);
        $arrTermoAdesao['termsgbemvindo']            = trim($arrPost['termsgbemvindo']);
        $arrTermoAdesao['terredirecionamentoadesao'] = trim($arrPost['terredirecionamentoadesao']);
        $arrTermoAdesao['tersituacao']               = trim($arrPost['tersituacao']);
        $arrTermoAdesao['terdtinclusao']             = trim($arrPost['terdtinclusao']);

        $arrTermoAdesao['inprgid']                   = $arrPost['inprgid'];
        $arrTermoAdesao['terdtinclusao']             = date('Y-m-d H:m:s');
        $arrTermoAdesao['terstatus']                 = 'A';
        $arrTermoAdesao['tercpfinclusao']            = $_SESSION['usucpf'];
        $arrTermoAdesao['terpacto']                  = ($arrPost['terpacto']);
        if(empty($arrPost['terid'])){$arrTermoAdesao['terdtinclusao']  = date('Y-m-d H:m:s');}//só grava data de inclusão ao criar
        //Valida campos
        $erro = $this->model->validarInput($arrTermoAdesao);
        if($erro){return $erro;}

        try {
            $this->model->popularDadosObjeto($arrTermoAdesao);
            $novoTermo = $this->model->salvar();
            $this->model->commit();

            $modelTermo = new Par3_Model_IniciativaProgramasTermoAdesao($novoTermo);//recupera Termo de adesão
            $modelHistorico = new Par3_Model_IniciativaProgramasTermoAdesaoHistorico();
            $modelHistorico->gravarHistorico($modelTermo,$acao);//grava em hitórico
            $modelHistorico->salvar();
            $modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return $e->getMessage();
        }
        return $novoTermo;
    }

    public function remover($arrPost)
    {
        $arrTermoAdesao['terid']             = $arrPost['terid'];
        $arrTermoAdesao['terstatus']         = 'I';
        $arrTermoAdesao['terdtinativacao']   = date('Y-m-d H:m:s');
        $arrTermoAdesao['tercpfinativacao']  = $_SESSION['usucpf'];

        try {
            $this->model->popularDadosObjeto($arrTermoAdesao);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $modelTermoAdesao = new Par3_Model_IniciativaProgramasTermoAdesao($arrTermoAdesao['terid']);

            $modelHistorico = new Par3_Model_IniciativaProgramasTermoAdesao();
            $modelHistorico->gravarHistorico($modelTermoAdesao,Par3_Model_IniciativaProgramasTermoAdesaoHistorico::DELETE);
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
        $sql = $this->model->montarSQLSimples($arrPost);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        $tratamentoListaVazia = ($arrPost['req'] == '' ) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $cabecalho = array('Ano de Referência', 'Esfera','Situação');
        $esconderColunas = array('id','codigo');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('remove', 'removerTermoAdesao');
        $listagem->addAcao('edit', 'editarTermoAdesao');
        $listagem->addCallbackDeCampo('terstatus','formata_status');
        $listagem->addCallbackDeCampo('teresfera','formata_esfera');
        $listagem->setTamanhoPagina(10);
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
}
