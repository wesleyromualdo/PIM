<?php
/**
 * Created by PhpStorm.
 * User: lucasgomes
 * Date: 14/07/2017
 * Time: 17:59
 */

class Simec_Abas_Renderer_Vertical extends Simec_Abas_Renderer_Abstract implements Simec_Abas_Renderer_Interface
{
    public function render()
    {
        if ($this->config === null) {
            throw new Exception('Variável membro "$this->config" precisa ser configurada');
        }

        echo '<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">';
        $url = $this->config['url'] ? $this->config['url'] : $_SERVER['REQUEST_URI'];

        if (is_array($this->config['listaAbas'])) {
            $rs = $this->config['listaAbas'];
        } else {
            global $db;
            $rs = $db->carregar($this->config['listaAbas']);
        }

        $menu = <<<HTML
    <div class="list-group">
HTML;

        foreach ($rs as $tab) {
            if (is_array($tab)) {
                $cssClassActive = ($tab['link'] == $url)? ' active' : '';
                $menu .= <<<HTML
    <a href="{$tab['link']}" class="list-group-item {$cssClassActive}">{$tab['descricao']}</a>
HTML;
            }
        }

        $menu .= <<<HTML
    </div>
HTML;

        echo $menu;
    }
}