<?php

/**
 * Controle da funcionalidade itenscomposicao
 *
 * @author DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license GNU simec.mec.gov.br
 * @version $Id$
 */
class Par3_Controller_ItensComposicao extends Controle
{
    /**
     * Variável para instância do modelo.
     *
     * @var object
     */
    private $model;
    private $modelHistorico;

    /**
     * Variável para guardar a url da funcionalidade.
     *
     * @var string
     */
    protected $_url;

    public function __construct()
    {
        $itcid = validarInteiro($_GET['itcid'])? $_GET['itcid']:'';
        $this->model          = new Par3_Model_ItensComposicao($itcid);
        $this->modelHistorico = new Par3_Model_ItensComposicaoHistorico();
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_ItensComposicaoHistorico::CREATE;
        if($arrPost['itcid']){
            $arrayItem['itcid'] = $arrPost['itcid'];
            $acao = Par3_Model_ItensComposicaoHistorico::UPDATE;
        }
		
        $arrayItem['itcsituacao']      = $arrPost['itcsituacao'];
        $arrayItem['itcstatus']        = 'A';
        $arrayItem['itcdsc']           = trim(($arrPost['itcdsc']));
        $arrayItem['itcespecificacao'] = trim(($arrPost['itcespecificacao']));
        $arrayItem['ctdid']            = trim(($arrPost['ctdid']));
        $arrayItem['uniid']            = trim(($arrPost['uniid']));
        $arrayItem['intoid']           = $arrPost['intoid'];
        if( $arrPost['itcidsigarp'] ) $arrayItem['itcidsigarp'] = $arrPost['itcidsigarp'];
        $arrayItem['itccpfinclusao']   = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);
//        ver($arrayModalidade,d);
        if(!$arrPost['atpid']){$arrayItem['atpdtinclusao'] = date('Y-m-d H:m:s');}

        //Valida campos
        $erro = $this->model->validarInput($arrayItem);
        if($erro){return $erro;}

        try {
            $this->model->popularDadosObjeto($arrayItem);
            $id =  $this->model->salvar();
            $this->model->commit();
            
            if($arrayItem['itcsituacao'] == 'I'){
                $this->model->removerDetalhes($arrPost['itcid'], 'A', 'I');
            }

            //Salvar Ata Pregão Estados
            $mItensTiposObj = new Par3_Model_ItensComposicaoTiposObjeto();
            $mItensTiposObj->salvarItensTiposObjeto($id,prepararArraySelectMultiSelecao($arrayItem['intoid']));

            $mItem = new $this->model($id);

            $this->modelHistorico->gravarHistorico($mItem,$acao);
            $this->modelHistorico->commit();

            //$this->model->atualizaValorPlanejadoPlanejamentoIniciativa();

        } catch (Simec_Db_Exception $e) {return 'erro';}
        if ($id) {return $id;}
        return 'erro';
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function remover($arrPost)
    {
        $arrayItem['itcid']             = $arrPost['itcid'];
        $arrayItem['itcstatus']         = 'I';
        $arrayItem['itcsituacao']       = 'I';
        $arrayItem['itcdtinativacao']   = date('Y-m-d H:m:s');
        $arrayItem['itccpfinativacao']  = $_SESSION['usucpf'];

        try {
            $this->model->popularDadosObjeto($arrayItem);
            $sucesso = $this->model->salvar();
            $this->model->commit();
            
            $this->model->removerDetalhes($arrPost['itcid'], 'A', 'I');

            $modelItem = new Par3_Model_ItensComposicao($arrPost['itcid']);

            $this->modelHistorico->gravarHistorico($modelItem,Par3_Model_ItensComposicaoHistorico::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {
            return $sucesso;
        }
        return 'erro';
    }

    /**ordenação
     * id =1,qtd = 2,codigo = 3,descricao = 4,tiposobjetos = 5,unidademedida = 6,categoriadespesa = 7,situacao = 8
     */
    public function listar($arrPost)
    {
        global $disabled;
        $mItem = new Par3_Model_ItensComposicao();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        /**ordenação
         * id =1,qtd = 2,codigo = 3,descricao = 4,tiposobjetos = 5,unidademedida = 6,categoriadespesa = 7,situacao = 8
         */
        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = '3';
                break;
            case 'descricao':
                $arrQuery['ordenacao']['campo'] = '4';
                break;
            case 'tiposobjetos':
                $arrQuery['ordenacao']['campo'] = '5';
                break;
            case 'unidademedida':
                $arrQuery['ordenacao']['campo'] = '6';
                break;
            case 'categoriadespesa':
                $arrQuery['ordenacao']['campo'] = '7';
                break;
            case 'situacao':
                $arrQuery['ordenacao']['campo'] = '8';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }
        /**ordenação
         * id =1,qtd = 2,codigo = 3,descricao = 4,tiposobjetos = 5,unidademedida = 6,categoriadespesa = 7,situacao = 8
         */
        $sql = $mItem->montarSQLDataGrid($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,descrição,situção
        $cabecalho       = array();
        $esconderColunas = array();
        $cabecalho = array('Código','Item','Tipos de Objeto','Unidade de Medida','Categoria Despesa','Situação', 'SIGARP');
        $esconderColunas = array('id','qtd', 'especificacao');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            //id,qtd,codigo,numeroata,atpobjeto,ano,arquivo,descricaoanexo,estados,vigencia,situação,
            $esconderColunas = array('id');
            $cabecalho = array('QTD','Código','Item','Tipos de Objeto','Unidade de Medida','Categoria Despesa','Situação', 'Especificação Técnica');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('itenscomposicaotable');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
//        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('situacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'removerItemComposicao');
            $listagem->addAcao('edit', 'editarItemComposicao');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
}

