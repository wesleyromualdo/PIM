<?php
/**
 * Classe de controle do  itenscomposicao_grupo_itens
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   DANIEL DA ROCHA FIUZA <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-06-2017
 * @link     no link
 */



/**
 * Par3_Controller_Itenscomposicao_grupo_itens
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 16-06-2017
 * @link     no link
 */
class Par3_Controller_ItensComposicaoGruposItens extends Controle
{
    private $model;
    private $modelHistorico;

    public function __construct()
    {
        $this->model = new Par3_Model_ItensComposicaoGruposItens($_GET['gitid']);
        $this->modelHistorico = new Par3_Model_ItensComposicaoGruposItensHistorico();
    }

    public function recuperar($arrPost)
    {
        return $this->model->recuperarItemVinculado($arrPost);
    }

    public function salvar($arrPost)
    {
        //Valida campos
        $erro = $this->model->validarInput($arrPost);
        if($erro){return $erro;}
        return $this->model->vincularItemGrupo($arrPost);
    }

    public function getMediaTotal($igrid)
    {
        return $this->model->getMediaTotal($igrid);
    }

    public function desvincular($arrPost)
    {
       return $this->model->desvincularItemGrupo($arrPost['gitid']);
    }

    public function listarItens($arrPost)
    {
        global $disabled;
        $sql = $this->model->sqlListagemSelecionarGrupoItem($arrPost);
        $disabled = 'disabled';
        $cabecalho = array('Vinculado','Código','Quantidade','Descrição');
        $esconderColunas = array('id');
        $listagem = new Simec_Listagem(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('vincularitenstable');
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
       $listagem->setTamanhoPagina(10000000);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function listar($arrPost)
    {
        //sqlDataGrid
        global $disabled;
        $sql = $this->model->sqlDataGrid($arrPost);
        $disabled = 'disabled';
        /**
         *Cabeçalho
         * id,codigo,itemcomposicao,quantidade,
         */
        $cabecalho = array('Item','Nome','Descrição','Quantidade','Media Valor Unitário','Média Valor Total');
        $esconderColunas = array('id');
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setId('grupositenstable');
        $listagem->setQuery($sql);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('unlinkfa', 'removerGrupoItem');
//      $listagem->setTamanhoPagina(10);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS,'id');
        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
}
?>


