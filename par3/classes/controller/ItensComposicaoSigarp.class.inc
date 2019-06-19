<?php


/**
 * Classe de controle do  itenscomposicao_categoria_sigarp
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   HEMERSON MORAIS <hemersonmorais@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-06-2017
 * @link     no link
 */



/**
 * Par3_Controller_ItensComposicaoPregaoSicarp
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-06-2017
 * @link     no link
 */
class Par3_Controller_ItensComposicaoSigarp extends Controle
{
    protected $model;
    protected $modelHistorico;
    protected $_url;

    public function __construct()
    {
        $this->model = new Par3_Model_ItensComposicaoSigarp($_GET['sprid']);
        //$this->modelHistorico = new Par3_ItensComposicaoPregaoSicarp();
    }


    public function listar($arrPost)
    {

        global $disabled;

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;

        switch($campo){
            case 'codigo':
                $arrQuery['ordenacao']['campo'] = '1';
                break;
            case 'numero':
                $arrQuery['ordenacao']['campo'] = '2';
                break;
            case 'datavigenciainicio':
                $arrQuery['ordenacao']['campo'] = '3';
                break;
            case 'datavigenciafim':
                $arrQuery['ordenacao']['campo'] = '3';
                break;
            case 'datainclusao':
                $arrQuery['ordenacao']['campo'] = '3';
                break;
            default:
                $arrQuery['ordenacao'] = '';
                break;
        }

        $sql = $this->model->sqlDataGrid($arrPost);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,descrição,situção
        $cabecalho       = array();
        $esconderColunas = array();
        $cabecalho = array('Código','Descrição','Data Inclusão','Código Categoria','Nome Categoria','Data Inclusão Categoria');
        $esconderColunas = array('id');

        if($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir'){
            //id,qtd,codigo,numeroata,atpobjeto,ano,arquivo,descricaoanexo,estados,vigencia,situação,
            $esconderColunas = array('id');
            $cabecalho = array('Código','Descição','Data Inclusão','Código Categoria','Nome Categoria','Data Inclusão Categoria');
            $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;
        }

        $tipoRelatorio = ($arrPost['req'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;
        if($arrPost['req'] == 'imprimir'){$tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;$disabled = true;}//desabilitar opções para a impressão
        $tratamentoListaVazia = ($arrPost['req'] == 'xls' || $arrPost['req'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        // $listagem->setId('ataPregaotable');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('situacao','formata_status');
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);


    }
}
?>


