<?php
/**
 * Classe de controle do  itenscomposicao_detalhamento
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-06-2017
 * @link     no link
 */



/**
 * Par3_Controller_Itenscomposicao_detalhamento
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-06-2017
 * @link     no link
 */
class Par3_Controller_ItensComposicaoDetalhamento extends Controle
{
    private $model;
    private $modelHistorico;

    public function __construct()
    {
        $itdid = validarInteiro($_GET['itdid'])? $_GET['itdid']:'';
        $this->model          = new Par3_Model_ItensComposicaoDetalhamento($itdid);
        $this->modelHistorico = new Par3_Model_ItensComposicaoDetalhamentoHistorico();
    }

    public function salvar($arrPost)
    {

        $valor = trim($arrPost['itdvalor']);
        $valor = str_replace(".", "", $valor);
        $valor = str_replace(",", ".", $valor);

        //define acão para guardar em histórico
        $acao = Par3_Model_ItensComposicaoDetalhamentoHistorico::CREATE;
        if($arrPost['itdid']){
            $arrItemDetalhamento['itdid'] = trim($arrPost['itdid']);
            $acao = Par3_Model_ItensComposicaoDetalhamentoHistorico::UPDATE;
        }
        
        $insert = $acao == Par3_Model_ItensComposicaoDetalhamentoHistorico::CREATE;
        
        $arrItemDetalhamento['itdid']              = $arrPost['itdid'];
        $arrItemDetalhamento['itcid']              = $arrPost['itcid'];
        $arrItemDetalhamento['sprid']              = $arrPost['sprid'] ? $arrPost['sprid'] : null;//id sigarppregao
        $arrItemDetalhamento['atpid']              = $arrPost['atpid'] ? $arrPost['atpid'] : null;
        $arrItemDetalhamento['itdpregao']          = trim($arrPost['itdpregao']);
        $arrItemDetalhamento['itdpregaopublicado'] = trim($arrPost['itdpregaopublicado']);
        $arrItemDetalhamento['itdvalor']           = $valor;
        $arrItemDetalhamento['itdperiodoinicio']   = formata_data_sql($arrPost['itdperiodoinicio']);
        $arrItemDetalhamento['itdperiodofim']      = formata_data_sql($arrPost['itdperiodofim']);
        $arrItemDetalhamento['itdstatus']          = trim($arrPost['itdstatus']);
        $arrItemDetalhamento['itdsituacao']        = trim($arrPost['itdsituacao']);
        $arrItemDetalhamento['estuf']              = prepararArraySelectMultiSelecao($arrPost['estuf']);

        $arrItemDetalhamento['itddtinclusao']      = date('Y-m-d H:m:s');
        $arrItemDetalhamento['itdstatus']          = 'A';
        $arrItemDetalhamento['itdcpfinclusao']     = $_SESSION['usucpf'];
        $arrItemDetalhamento['acao']               = $acao;

        if(empty($arrPost['itdid'])){$arrItemDetalhamento['itddtinclusao']  = date('Y-m-d H:m:s');}
        //Valida campos
        $erro = $this->model->validarInput($arrItemDetalhamento);
        if($erro){return $erro;}

        try {
            $this->model->popularDadosObjeto($arrItemDetalhamento);
            $novoItemDetalhamento = $this->model->salvar(true,true,array('sprid','atpid'));
            $this->model->commit();

            $arrItemDetalhamentoEstuf = array();
            $arrItemDetalhamentoEstuf = prepararArraySelectMultiSelecao($arrPost['estuf']);
            $mDetalhamentoEstado = new Par3_Model_ItensComposicaoDetalhamentoEstado();
            $mDetalhamentoEstado->salvarItemDetalhamentoEstado($novoItemDetalhamento,$arrItemDetalhamentoEstuf, $insert);

            $modelItemDetalhamento = new Par3_Model_ItensComposicaoDetalhamento($novoItemDetalhamento);//recupera Item de Detalhamento
            $modelHistorico = new Par3_Model_ItensComposicaoDetalhamentoHistorico();
            $modelHistorico->gravarHistorico($modelItemDetalhamento,$acao);//grava em hitórico
            $modelHistorico->salvar();
            $modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {
            return $e->getMessage();
        }
        return $novoItemDetalhamento;
    }

    public function remover($arrPost)
    {
        $arrItemDetalhamento['itdid']            = $arrPost['itdid'];
        $arrItemDetalhamento['itdstatus']        = 'I';
        $arrItemDetalhamento['itddtinativacao']  = date('Y-m-d H:m:s');
        $arrItemDetalhamento['itdcpfinativacao'] = $_SESSION['usucpf'];

        try {
            $this->model->popularDadosObjeto($arrItemDetalhamento);
            $sucesso = $this->model->salvar();
            $this->model->commit();

            $mItemDetalhamento = new Par3_Model_ItensComposicaoDetalhamento($arrPost['itdid']);

            $this->modelHistorico->gravarHistorico($mItemDetalhamento,Par3_Model_ItensComposicaoDetalhamentoHistorico::DELETE);
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

    public function listarPregao()
    {
        global $disabled;
        $sql = <<<DML
                SELECT
                sprid as id,
                sprnuseqpregao as codigo,
                sprnupregao as pregao,
                TO_CHAR(sprdtinicio, 'dd/mm/YYYY') as dtinicio,
                TO_CHAR(sprdtfim, 'dd/mm/YYYY') as dtfim,
                TO_CHAR(sprdtinclusao, 'dd/mm/YYYY') as dtinclusao
                FROM
                par.sigarppregao
                ORDER BY
                sprnuseqpregao       
DML;

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        $tratamentoListaVazia = ($arrPost['req'] == '' ) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
        $cabecalho = array('Código','Pregão','Data Início','Data Fim','Data de Inclusão');
        $esconderColunas = array('id');
        $listagem = new Simec_Listagem(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('checkfa', 'adicionarPregao');
        $listagem->addAcao('plus', 'carregarDadosPregao');
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }

    public function listarDadosPregao($sprid)
    {
        global $db;
        $cabecalho = array("Descrição do Item","UF","Valor");
        $sql = "SELECT
                sit.sitdsc as item,
                spd.spduf as uf,
                spd.spdvalor as valor
                FROM
                par.sigarppregaodados spd
                INNER JOIN par.sigarpitem sit ON sit.sitid = spd.sitid
                WHERE
                spd.sprid = ".$sprid;

        $tratamentoListaVazia = ($arrPost['req'] == '' ) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
        $cabecalho = array('Item','UF','Valor');
        $listagem = new Simec_Listagem(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->render($tratamentoListaVazia);
    }

    public function listarAta()
    {
        global $disabled;
        $sql = "SELECT atpid as id,atpid as codigo,atpnumeroata FROM par3.itenscomposicao_ata_pregao WHERE atpsituacao = 'A' AND atpstatus='A'";

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        $tratamentoListaVazia = ($arrPost['req'] == '' ) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $cabecalho = array('Código','Nº Ata');
        $esconderColunas = array('id');
        $listagem = new Simec_Listagem(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('checkfa', 'adicionarAta');
//        $listagem->setTamanhoPagina(10);
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
    /**
     * ordenação
     * id =1,codigo = 2,detalhamento = 3,atapregao = 4,estados = 5,periodoinicio = 6,periodofim = 7,situacao = 8
     */
    public function listar($arrPost)
    {
        global $disabled;
        $sql = $this->model->montarSQLDataGrid($arrPost);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        $tratamentoListaVazia = ($arrPost['req'] == '' ) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $cabecalho = array('Código','Ata Pregão','Estados','Valor','Período Início','Período Fim','Situação');
        $esconderColunas = array('id');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('remove', 'removerItemDetalhamento');
        $listagem->addAcao('edit', 'editarItemDetalhamento');
        $listagem->addCallbackDeCampo('situacao','formata_status');
        $listagem->addCallbackDeCampo('periodoinicio','formata_data');
        $listagem->addCallbackDeCampo('periodofim','formata_data');
        $listagem->setTamanhoPagina(10);
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
}
?>