<?php
require APPRAIZ . 'demandas/model/DemandasCardapioItensDescricao.class.inc';

class DemandasCardapioItensDescricao extends mvc\Controle
{
    public function iniciarFluxo()
    {
        switch ($_REQUEST['requisicao']) {
            case 'salvarCardapioItemDescricao':
                $this->acaoSalvar();
                $this->sucesso('demandas.php?modulo=demandasCardapioItensDescricao/listar&acao=A');
                die;
            case 'deletarCardapioItemDescricao':
                $this->acaoDeletar($_REQUEST);
                $this->sucesso('demandas.php?modulo=demandasCardapioItensDescricao/listar&acao=A');
                die;
                
        }
    }
    
    public function acaoListar()
    {
        
        $moDemandasCardapioItensDescricao = new DemandasCardapioItensDescricao_Model();
        
        $arDado = array();
        
        $_POST['returnsql'] = true;
        
        $arDado['sql_cardapio_itens_descricao'] = $moDemandasCardapioItensDescricao->listaCardapioItensDescricao($_POST);
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('lista');
    }
    
    public function acaoCadastrar()
    {
        
        $moDemandasCardapioItensDescricao = new DemandasCardapioItensDescricao_Model();
        
        $arDado = array();
        
        if($_REQUEST['cidid']) $arDado = $moDemandasCardapioItensDescricao->getCardapioItensDescricaoPorId($_REQUEST['cidid']);
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('cadastro');
    }
    
    
    public function acaoDeletar()
    {
        $moDemandasCardapioItensDescricao = new DemandasCardapioItensDescricao_Model();
        
        $_REQUEST['cidstatus'] = 'I';
        $moDemandasCardapioItensDescricao->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasCardapioItensDescricao->commit();
        
    }
    
    public function acaoSalvar()
    {
        $moDemandasCardapioItensDescricao = new DemandasCardapioItensDescricao_Model();
        
        $moDemandasCardapioItensDescricao->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasCardapioItensDescricao->commit();
        
    }
    
    
}