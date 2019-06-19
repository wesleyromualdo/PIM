<?php
/**
 * Classe de controle do  itenscomposicao_grupos
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-06-2017
 * @link     no link
 */



/**
 * Par3_Controller_Itenscomposicao_grupos
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-06-2017
 * @link     no link
 */
class Par3_Controller_ItensComposicaoGrupos extends Controle
{
    private $model;
    private $modelHistorico;

    public function __construct()
    {
        $igrid = validarInteiro($_GET['igrid'])? $_GET['igrid']:'';
        $this->model          = new Par3_Model_ItensComposicaoGrupos($igrid);
        $this->modelHistorico = new Par3_Model_ItensComposicaoGruposHistorico();
    }

    public function salvar($arrPost)
    {
        $acao = Par3_Model_ItenscomposicaoGruposHistorico::CREATE;
        if($arrPost['igrid']){
            $arrayItemGrupo['igrid'] = $arrPost['igrid'];
            $acao = Par3_Model_ItenscomposicaoGruposHistorico::UPDATE;
        }

        $arrayItemGrupo['igrsituacao']    = $arrPost['igrsituacao'];
        $arrayItemGrupo['igrstatus']      = 'A';
        $arrayItemGrupo['igrnome']        = trim(($arrPost['igrnome']));
        $arrayItemGrupo['igrdsc']         = trim(($arrPost['igrdsc']));
        $arrayItemGrupo['intoid']         = $arrPost['intoid'];
        $arrayItemGrupo['igridsigarp']    = $arrPost['igridsigarp'] ?: null;
        $arrayItemGrupo['igrcpfinclusao'] = str_replace(array('.', '-', '/', ' '), '',$_SESSION['usucpf']);

        if(!$arrPost['igrid']){$arrayItemGrupo['igrdtinclusao'] = date('Y-m-d H:m:s');}
        //Valida campos
        $erro = $this->model->validarInput($arrayItemGrupo);
        if($erro){return $erro;}

        try {
            $this->model->popularDadosObjeto($arrayItemGrupo);
            $id =  $this->model->salvar();
            $this->model->commit();

            //Salvar Ata Pregão Estados
            $mItensGruposTiposObj = new Par3_Model_ItensComposicaoGruposTiposObjeto();
            $mItensGruposTiposObj->salvarItensGruposTiposObjeto($id,prepararArraySelectMultiSelecao($arrayItemGrupo['intoid']));

            $mItemGrupo = new $this->model($id);

            $this->modelHistorico->gravarHistorico($mItemGrupo,$acao);
            $this->modelHistorico->commit();
        } catch (Simec_Db_Exception $e) {return 'erro';}
        if ($id) {return $id;}
        return 'erro';
    }

    public function remover($arrPost)
    {
        $arrayItemGrupo['igrid']             = $arrPost['igrid'];
        $arrayItemGrupo['igrstatus']         = 'I';
        $arrayItemGrupo['igrdtinativacao']   = date('Y-m-d H:m:s');
        $arrayItemGrupo['igrcpfinativacao']  = $_SESSION['usucpf'];

        try {
            $this->model->popularDadosObjeto($arrayItemGrupo);
            $sucesso = $this->model->salvar();
            $this->model->commit();
            $modelItem = new Par3_Model_ItenscomposicaoGrupos($arrPost['igrid']);
            $this->modelHistorico->gravarHistorico($modelItem,Par3_Model_ItenscomposicaoGruposHistorico::DELETE);
        } catch (Simec_Db_Exception $e) {
            return 'erro';
        }
        if ($sucesso) {return $sucesso;}
        return 'erro';
    }

    public function recuperar()
    {
        return $this->model;
    }

    /** id =1,qtd = 2,codigo = 3,nome = 4,descricao = 5,tiposobjetos= 6,datainclusao = 7,situacao= 8
     */
    public function listar($arrPost)
    {
        global $disabled;
        $mItemGrupos = new Par3_Model_ItensComposicaoGrupos();

        $campo   = $arrPost['listagem']['campo'];
        $sentido = $arrPost['listagem']['sentido'] == 'ASC'? 'ASC':'DESC';

        $arrQuery = $arrPost;
        $arrQuery['ordenacao']['sentido'] = $sentido;
        /** id =1,qtd = 2,codigo = 3,nome = 4,descricao = 5,tiposobjetos= 6,datainclusao = 7,situacao= 8
         */
        switch($campo){
            case 'codigo':       $arrQuery['ordenacao']['campo'] = '3';break;
            case 'nome':         $arrQuery['ordenacao']['campo'] = '4';break;
            case 'descricao':    $arrQuery['ordenacao']['campo'] = '5';break;
            case 'tiposobjetos': $arrQuery['ordenacao']['campo'] = '6';break;
            case 'datainclusao': $arrQuery['ordenacao']['campo'] = '7';break;
            case 'situacao':     $arrQuery['ordenacao']['campo'] = '8';break;
            default:             $arrQuery['ordenacao'] = '';break;
        }

        $sql = $mItemGrupos->montarSQLDataGrid($arrQuery);
        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';
        //Cabeçalho: código,descrição,situção
        $cabecalho       = array();
        $esconderColunas = array();
        /** id =1,qtd = 2,codigo = 3,nome = 4,descricao = 5,tiposobjetos= 6,datainclusao = 7,situacao= 8
         */
        $cabecalho = array('Código','Nome','Descrição','Tipos de Objeto','Data de Inclusão','Situação');
        $esconderColunas = array('id','qtd');

        if($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir'){
            $esconderColunas = array();
            array_unshift($cabecalho,'QTD');
            $disabled = true;
        }

        $tipoRelatorio = ($arrPost['requisicao'] == 'xls') ? Simec_Listagem::RELATORIO_XLS : ( ($arrPost['requisicao'] == 'imprimir') ? $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO : Simec_Listagem::RELATORIO_PAGINADO );
        $tratamentoListaVazia = ($arrPost['requisicao'] == 'xls' || $arrPost['requisicao'] == 'imprimir') ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('itenscomposicaogrupostable');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('situacao','formata_status');
        if($disabled == ''){
            $listagem->addAcao('remove', 'removerItemComposicaoGrupo');
            $listagem->addAcao('edit', 'editarItemComposicaoGrupo');
        }
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
}
?>