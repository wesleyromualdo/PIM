<?php
require APPRAIZ . 'demandas/model/DemandasCardapio.class.inc';

class DemandasCardapio extends mvc\Controle
{
    public function iniciarFluxo()
    {
        switch ($_REQUEST['requisicao']) {
            case 'salvarCardapio':
                $this->acaoSalvar();
                $this->sucesso('demandas.php?modulo=demandasCardapio/listar&acao=A');
                die;
            case 'deletarCardapioItemDescricao':
                $this->acaoDeletar($_REQUEST);
                $this->sucesso('demandas.php?modulo=demandasCardapioItensDescricao/listar&acao=A');
                die;
                
        }
    }
    
    public function acaoListar()
    {
        
        $moDemandasCardapio = new DemandasCardapio_Model();
        
        $arDado = array();
        
        $_POST['returnsql'] = true;
        
        $arDado['sql_cardapio'] = $moDemandasCardapio->listaCardapio($_POST);
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('lista');
    }
    
    
    
    public function acaoCadastrar()
    {
        global $arDado;
        
        $moDemandasCardapio = new DemandasCardapio_Model();
        
        $arDado = array();
        
        if($_REQUEST['dcoid']) {
            
            $arDado = $moDemandasCardapio->getCardapioPorId($_REQUEST['dcoid']);
            
            require APPRAIZ . 'demandas/model/DemandasCardapioItens.class.inc';
            
            $moDemandasCardapioItens = new DemandasCardapioItens_Model();
            
            $arDado['cardapioItens'] = $moDemandasCardapioItens->listaCardapioItens($_REQUEST);
            
            $arDado['htmlItens'] = $this->visao->capturarHtmlVisaoAcessoria('cadastroitens');
            
        }
        
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

        $moDemandasCardapio = new DemandasCardapio_Model();
        
        $moDemandasCardapio->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasCardapio->commit();
        
    }
    
    
}