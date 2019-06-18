<?php

class Par3_Controller_ItensComposicaoAtaPregao extends Controle
{
    /**
     * Variável para instância do modelo.
     *
     * @var object
     */
    protected $model;
    protected $modelHistorico;
    /**
     * Variável para guardar a url da funcionalidade.
     *
     * @var string
     */
    protected $_url;

    public function __construct()
    {
        $this->model = new Par3_Model_ItensComposicaoAtaPregao($_GET['atpid']);
        $this->modelHistorico = new Par3_Model_ItensComposicaoAtaPregaoHistorico();
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_ItensComposicaoAtaPregaoHistorico::CREATE;
        if($arrPost['atpid']){
            $arrayAtaPregao['atpid'] = $arrPost['atpid'];
            $acao = Par3_Model_ItensComposicaoAtaPregaoHistorico::UPDATE;
        }

        $arrayAtaPregao['atpsituacao']       = $arrPost['atpsituacao'];
        $arrayAtaPregao['atpstatus']         = 'A';
        $arrayAtaPregao['atpnumeroata']      = trim(($arrPost['atpnumeroata']));
        $arrayAtaPregao['atpanovigencia']    = trim(($arrPost['atpanovigencia']));
        $arrayAtaPregao['atpanovigencia']    = trim(($arrPost['atpanovigencia']));
        $arrayAtaPregao['atpdescricaoanexo'] = trim(($arrPost['atpdescricaoanexo']));
        $arrayAtaPregao['atpobjeto']         = trim(($arrPost['atpobjeto']));
        $arrayAtaPregao['atpvigencia']       = trim(($arrPost['atpvigencia']));

        $arrayAtaPregao['atpdtinicio']       = formata_data_sql($arrPost['atpperiodoinicio']);
        $arrayAtaPregao['atpdtfim']          = formata_data_sql($arrPost['atpperiodofim']);
        $arrayAtaPregao['atpcpfinclusao']    = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
        $arrayAtaPregao['estuf']             = prepararArraySelectMultiSelecao($arrPost['estuf']);
        $arrayAtaPregao['arqid']             = $arrPost['arqid'];
//        ver($arrayModalidade,d);
        if(!$arrPost['atpid']){$arrayAtaPregao['atpdtinclusao'] = date('Y-m-d H:m:s');}

        //Valida campos
        $erro = $this->model->validarInput($arrayAtaPregao);
        if($erro){return $erro;}

        try {
            $this->model->popularDadosObjeto($arrayAtaPregao);
            $id =  $this->model->salvar();
            $this->model->commit();

            //Salvar Ata Pregão Estados
            $mAtaPregaoEstado = new Par3_Model_ItensComposicaoAtaPregaoEstado();
            $mAtaPregaoEstado->salvarAtaPregaoEstado($id,$arrayAtaPregao['estuf']);

            $mAtaPregao = new Par3_Model_ItensComposicaoAtaPregao($id);

            $this->modelHistorico->gravarHistorico($mAtaPregao,$acao);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {return 'erro';}
        if ($id) {return $id;}
        return 'erro';
    }

    public function recuperar()
    {
        return $this->model;
    }

    public function remover($arrPost)
    {
        $arrayAtaPregao['atpid']            = $arrPost['atpid'];
        $arrayAtaPregao['atpstatus']        = 'I';
        $arrayAtaPregao['atpsituacao']      = 'I';
        $arrayAtaPregao['atpdtinativacao']  = date('Y-m-d H:m:s');
        $arrayAtaPregao['atpcpfinativacao'] = $_SESSION['usucpf'];

        try {
            $this->model->popularDadosObjeto($arrayAtaPregao);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $mAtaPregao = new Par3_Model_ItensComposicaoAtaPregao($arrPost['atpid']);

            $this->modelHistorico->gravarHistorico($mAtaPregao,Par3_Model_ItensComposicaoAtaPregaoHistorico::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    //id,qtd,codigo,ano,numeroata,atpobjeto,arquivo,descricaoanexo,estados,vigencia,situação,
    public function listar($arrPost)
    {
        global $disabled;
        $mAtaPregao = new Par3_Model_ItensComposicaoAtaPregao();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        /**
         * ordenação
         * id =1,qtd = 2,codigo = 3,numerodata = 4,atpobjeto = 5,ano = 6,arquivo,7,descricaoanexo = 8,estados = 9,vigencia = 11,situacao = 12
         */
        switch($campo){
            case 'numeroata':
                $arrQuery['ordenacao']['campo'] = '3';
                break;
            case 'arquivo':
                $arrQuery['ordenacao']['campo'] = '6';
                break;
            case 'atpobjeto':
                $arrQuery['ordenacao']['campo'] = '5';
                break;
            case 'estados':
                $arrQuery['ordenacao']['campo'] = '9';
                break;
            case 'vigencia':
                $arrQuery['ordenacao']['campo'] = '11';
                break;
            case 'situacao':
                $arrQuery['ordenacao']['campo'] = '12';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }

        $sql = $mAtaPregao->montarSQLSimples($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,descrição,situção
        $cabecalho       = array();
        $esconderColunas = array();
        $cabecalho = array('Código','Nº Ata','Objeto','Arquivo','Estado(s)','Vigência','Situação');
        $esconderColunas = array('id','qtd','ano','descricaoanexo');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            //id,qtd,codigo,numeroata,atpobjeto,ano,arquivo,descricaoanexo,estados,vigencia,situação,
            $esconderColunas = array('id');
            $cabecalho = array('QTD','Código','Nº Ata','Objeto','Ano','Arquivo','Descrição do Arquivo','Estado(s)','Vigência','Situação');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('ataPregaotable');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('situacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'removerAtaPregao');
            $listagem->addAcao('edit', 'editarAtaPregao');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
}

