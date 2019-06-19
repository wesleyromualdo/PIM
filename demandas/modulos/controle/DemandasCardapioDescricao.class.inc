<?php
require APPRAIZ . 'demandas/model/DemandasCardapioDescricao.class.inc';

class DemandasCardapioDescricao extends mvc\Controle
{
    public function iniciarFluxo()
    {
        switch ($_REQUEST['requisicao']) {
            case 'salvarCardapioDescricao':
                $this->acaoSalvar();
                $this->sucesso('demandas.php?modulo=demandasCardapioDescricao/listar&acao=A');
                die;
            case 'deletarCardapioDescricao':
                $this->acaoDeletar($_REQUEST);
                $this->sucesso('demandas.php?modulo=demandasCardapioDescricao/listar&acao=A');
                die;
                
        }
    }
    
    public function acaoListar()
    {
        
        $moDemandasCardapioDescricao = new DemandasCardapioDescricao_Model();
        
        $arDado = array();
        
        $_POST['returnsql'] = true;
        
        $arDado['sql_cardapio_descricao'] = $moDemandasCardapioDescricao->listaCardapioDescricao($_POST);
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('lista');
    }
    
    public function acaoCadastrar()
    {
        
        $moDemandasCardapioDescricao = new DemandasCardapioDescricao_Model();
        
        $arDado = array();
        
        if($_REQUEST['dcdid']) $arDado = $moDemandasCardapioDescricao->getCardapioDescricaoPorId($_REQUEST['dcdid']);
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho(' ');
        $this->visao->incluirVisao('cadastro');
    }
    
    
    public function acaoDeletar()
    {
        $moDemandasCardapioDescricao = new DemandasCardapioDescricao_Model();
        
        $_REQUEST['dcdstatus'] = 'I';
        $moDemandasCardapioDescricao->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasCardapioDescricao->commit();
        
    }
    
    public function acaoSalvar()
    {
        $moDemandasCardapioDescricao = new DemandasCardapioDescricao_Model();
        
        $moDemandasCardapioDescricao->popularDadosObjeto($_REQUEST)->salvar();
        
        $moDemandasCardapioDescricao->commit();
        
    }
    
    
}