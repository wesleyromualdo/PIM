<?php
namespace visao\Helper;

class Ibox extends \MVC\HelperVisao
{
    protected $configVisaoHelper;
    protected $dados;

    public function iniciarFluxo()
    {
        $this->configVisaoHelper['caixaExterna']['class']          = 'col-md-4';
        $this->configVisaoHelper['caixaExterna']['visivel']        = true;
        $this->configVisaoHelper['caixaBox']['class']              = 'ibox float-e-margins';
        $this->configVisaoHelper['caixaBox']['id']                 = '';

        $this->configVisaoHelper['caixaTitulo']['retirar']         = false;
        $this->configVisaoHelper['caixaTitulo']['class']           = 'ibox-title';
        $this->configVisaoHelper['caixaTitulo']['permitir-fechar'] = false;
        $this->configVisaoHelper['caixaTituloOpcao']['class']      = 'ibox-tools';
        $this->configVisaoHelper['caixaOpcoes']                    = array();
        $this->configVisaoHelper['caixaCorpo']['class']            = 'ibox-content';
        $this->configVisaoHelper['caixaCorpo']['max-height']       = '';
        $this->configVisaoHelper['caixaCorpo']['height']           = '';

        $this->configVisaoHelper['caixaRodape']['retirar']         = false;
        $this->configVisaoHelper['caixaRodape']['class']           = 'ibox-footer';
        
    }
    
    public function setarConteudoTitulo($conteudo)
    {
        $this->dados['conteudoTitulo'] = $conteudo;
        return $this;
    }
    
    public function setarConteudoCorpo($conteudo)
    {
        $this->dados['conteudoCorpo'] = $conteudo;
        return $this;
    }
    
    public function setarConteudoRodape($conteudo)
    {
        $this->dados['conteudoRodape'] = $conteudo;
        return $this;
    }
        
    public function addClassCaixaExterna($class)
    {
        $this->configVisaoHelper['caixaExterna']['class'] .= ' ' . $class;
        return $this;
    }
    
    public function limparClassCaixaExterna()
    {
        $this->configVisaoHelper['caixaExterna']['class'] = '';
        return $this;
    }
    
    public function addClassCaixaBox($class)
    {
        $this->configVisaoHelper['caixaBox']['class'] .= ' ' . $class;
        return $this;
    }
    
    public function limparClassCaixaBox()
    {
        $this->configVisaoHelper['caixaBox']['class'] = '';
        return $this;
    }
        
    public function addClassCaixaTitulo($class)
    {
        $this->configVisaoHelper['caixaTitulo']['class'] .= ' ' . $class;
        return $this;
    }
    
    public function limparClassCaixaTitulo()
    {
        $this->configVisaoHelper['caixaTitulo']['class'] = '';
        return $this;
    }
    
    public function retirarCaixaTitulo($boleano)
    {
        $this->configVisaoHelper['caixaTitulo']['retirar'] = ($boleano ? true : false);
        return $this;
    }
        
    public function addClassCaixaTituloOpcao($class)
    {
        $this->configVisaoHelper['caixaTituloOpcao']['class'] .= ' ' . $class;
        return $this;
    }
    
    public function limparClassCaixaTituloOpcao()
    {
        $this->configVisaoHelper['caixaTituloOpcao']['class'] = '';
        return $this;
    }
    
    public function setarMaxHeightCorpo($maxHeight)
    {
        $this->configVisaoHelper['caixaCorpo']['max-height'] = $maxHeight;
    }
    
    public function addClassCaixaCorpo($class)
    {
        $this->configVisaoHelper['caixaCorpo']['class'] .= ' ' . $class;
        return $this;
    }
    
    public function limparClassCaixaCorpo()
    {
        $this->configVisaoHelper['caixaCorpo']['class'] = '';
        return $this;
    }
         
    public function addClassCaixaRodape($class)
    {
        $this->configVisaoHelper['caixaRodape']['class'] .= ' ' . $class;
        return $this;
    }
    
    public function limparClassCaixaRodape()
    {
        $this->configVisaoHelper['caixaRodape']['class'] = '';
        return $this;
    }
    
    public function retirarCaixaRodape($boleano)
    {
        $this->configVisaoHelper['caixaRodape']['retirar'] = ($boleano ? true : false);
        return $this;
    }

    public function addCaixaToolBoxOpcoes($opcoes,$testaPermissao = true)
    {
        if($testaPermissao) {
            $opcoes->setParentId($this->getId());
            $this->configVisaoHelper['caixaOpcoes'][] = $opcoes;
        }
        return $this;
    }

    public function getListaToolBoxOpcoes()
    {
        return $this->configVisaoHelper['caixaOpcoes'];
    }

    public function permitirFecharIbox($permitir = false)
    {
        $this->configVisaoHelper['caixaTitulo']['permitir-fechar'] = $permitir;
        return $this;
    }

    public function setId($id)
    {
        $this->configVisaoHelper['caixaBox']['id'] = $id;
        return $this;
    }

    public function getId()
    {
        return $this->configVisaoHelper['caixaBox']['id'];
    }

    public function getConteudoTitulo()
    {
        return $this->dados['conteudoTitulo'];
    }

    public function setVisivel($visivel = true)
    {
        $this->configVisaoHelper['caixaExterna']['visivel'] = $visivel;
        return $this;
    }

    public function isVisivel()
    {
        return $this->configVisaoHelper['caixaExterna']['visivel'];
    }

    public function setHeight($height)
    {
        $this->configVisaoHelper['caixaCorpo']['height'] = $height;
        return $this;
    }
}
