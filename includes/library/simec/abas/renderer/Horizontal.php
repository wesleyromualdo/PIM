<?php
/**
 *
 */
class Simec_Abas_Renderer_Horizontal extends Simec_Abas_Renderer_Abstract implements Simec_Abas_Renderer_Interface
{
    public function render()
    {
        if ($this->config === null) {
            throw new Exception('Variável membro "$this->config" precisa ser configurada');
        }

        // -- Desenhando as abas
        echo montarAbasArray(
            $this->config['listaAbas'],
            "{$this->config['url']}",
            false,
            $this->config['incluirCssAdicional']
        );
    }
}