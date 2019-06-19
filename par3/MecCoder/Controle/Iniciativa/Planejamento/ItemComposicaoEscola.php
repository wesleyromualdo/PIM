<?php
namespace Simec\Par3\Controle\Iniciativa\Planejamento;

class ItemComposicaoEscola extends \Simec\AbstractControle
{

    public function finish()
    {
        
    }

    public function index()
    {
        $this->verDefinicaoQuantidade();
    }

    public function init()
    {
        
    }

    public function verDefinicaoQuantidade()
    {
        $this->js('/zimec/public/temas/simec/js/funcoes.js');
        $this->js('/zimec/public/temas/simec/js/plugins/viewjs/view.js');
        $this->toJs('_POST', $this->post());
        $this->showHtml();
    }

    public function listarEscolas()
    {
        $this->showJson((new \Simec\Par3\Modelo\Iniciativa\Planejamento\ItemComposicaoEscola())->listarEscolas($this->get()));
    }
}
