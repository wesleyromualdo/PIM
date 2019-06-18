<?php
namespace mvc;

abstract class HelperVisao
{
    private $config;
    private $visao;
    
    abstract public function iniciarFluxo();
    
    final public function __construct(Visao $objetoVisao)
    {
        $this->visao = $objetoVisao;
        $pasta = str_replace("\\", "/", get_class($this));
        $this->config['caminhoVisaoHelper'] = APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $pasta . "/";        
        $this->config['arquivoVisaoHelper'] = end(explode("\\", get_class($this))).'.inc';
        
        $this->iniciarFluxo();
    }
        
    final public function setarCaminhoVisaoHelper($caminhoCompleto)
    {
        $this->config['caminhoVisaoHelper'] = $caminhoCompleto;
        
        return $this;
    }
    
    final public function setarArquivoVisaoHelper($arquivo)
    {
        $this->config['arquivoVisaoHelper'] = $arquivo;
        
        return $this;
    }
    
    final public function renderizarHelper($renderizarAgora=true)
    {
        $arquivo = $this->config['caminhoVisaoHelper'] . $this->config['arquivoVisaoHelper'];
        if (file_exists($arquivo)){
            if ($renderizarAgora == true) {
                include $arquivo;
            } else {
                ob_clean();
                include $arquivo;
                $html = ob_get_contents();
                ob_clean();
                
                return $html;
            }
        } else {
            $this->visao->setarErro('O Helper <b>'. get_class($this) .'</b> não possui a visão: ' . $arquivo);
        }
    }
}