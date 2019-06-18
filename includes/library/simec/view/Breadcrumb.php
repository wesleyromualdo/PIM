<?php
/**
 * Implementação da classe de criação de breadcrumbs html/bootstrap.
 *
 * $Id: Breadcrumb.php 99994 2015-07-10 17:46:12Z maykelbraz $
 *
 * @filesource
 */

/**
 * Cria um breadcrumb no estilo bootstrap.
 */
class Simec_View_Breadcrumb
{
    protected $path = array();

    public function __construct($tipo = 'C')
    {
        $descricao = $_SESSION['sisdsc'];
        $link = "{$_SESSION['sisdiretorio']}.php?modulo=inicio&acao={$tipo}";
        $this->add($descricao, $link);
    }

    public function add($descricao, $link = null)
    {
        if (empty($descricao)) {
            throw new Exception('A descrição do novo item não pode ser vazia.');
        }

        $this->path[] = array('desc' => $descricao, 'link' => $link);
        return $this;
    }

    public function render()
    {
        if (empty($this->path)) {
            throw new Exception('Nenhum item foi adicionado para renderização.');
        }

        $ultimo = count($this->path) - 1;
        $html = <<<HTML
    <ol class="breadcrumb">
HTML;
        foreach ($this->path as $key => $path) {
            if ($key == $ultimo) {
                $html .= <<<HTML
        <li class="active">{$path['desc']}</li>
HTML;
                continue;
            }

            if (!empty($path['link'])) {
                $path['desc'] = <<<HTML
            <a href="{$path['link']}">{$path['desc']}</a>
HTML;
            }

            $html .= <<<HTML
            <li>{$path['desc']}</li>
HTML;
        }
        $html .= <<<HTML
    </ol>
HTML;
        echo $html;
    }
}
