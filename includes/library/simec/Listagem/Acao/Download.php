<?php
/**
 * Ação de download relacionada a um item da listagem.
 *
 * @package SiMEC
 * @subpackage listagem
 * @version $Id: Download.php 145432 2018-10-30 13:04:24Z danielmarconi $
 */

/**
 * Ação de download relacionada a um item da listagem.
 *
 * Criação ações de dois tipo, a primeira é uma ação normal, como as demais ações da listagem.<br />
 * O segundo é uma ação avançada, que possibilita o download de extensões diferentes. Neste<br />
 * segundo caso, é aberta uma janela para o usuário selecionar o tipo de extensão que ele<br />
 * precisa.<br />
 * Exemplo de utilização do modo avançado:<pre>
 * $listagem = new Simec_Listagem();
 * ...
 * $configAcao = array('func' => 'nomeCallback', 'formatos' => array('xls', 'pdf'));
 * $listagem->addAcao('download', $configAcao);
 * ...
 * $listagem->render();</pre>
 * A assinatura do callback js deve suportar como primeiro parâmetro um array e uma string como o segundo.<br />
 * O primeiro parâmetro é um array com todos os parâmetros informados durante a configuração do médoto, normalmente,<br />
 * é apena o ID da linha. O segundo parâmetro é o tipo de arquivo selecionado pelo usuário.
 *
 * @see Simec_Listagem
 * @see Simec_Listagem_Acao
 */
class Simec_Listagem_Acao_Download extends Simec_Listagem_Acao
{
    protected $icone = 'download-alt';
    protected $titulo = 'Download do arquivo';

    /**
     * Renderiza o ícone de ação.
     *
     * Se Simec_Listagem_Acao_Download::$config['formatos'] for preenchido, a renderização é feita<br />
     * utilizando o formato avançado da ação. Veja a forma de utilizar o modo avançado na documentação desta<br />
     * classe.<br />
     * Se Simec_Listagem_Acao_Download::$config['formatos'] não for preenchido, a rederização é feita<br />
     * da mesma forma que as demais ações básicas.
     *
     * @return string
     */
    protected function renderAcao()
    {
        // -- Ação básica
        if (!isset($this->config['formatos'])) {
            return parent::renderAcao();
        }

        // -- Ação avançada
        $acao = <<<HTML
<a href="#" class="multi-download" data-cb="%s" data-types="%s" data-params="%s">%s</a>
HTML;
        return sprintf(
            $acao,
            $this->callbackJS,
            str_replace('"', "'", simec_json_encode($this->config['formatos'])),
            $this->getCallbackParams(true),
            $this->renderGlyph()
        );
    }

    protected function renderGlyph()
    {
        $html = <<<HTML
<span class="btn btn-%s btn-sm glyphicon glyphicon-%s %s"></span>
HTML;

        return sprintf($html, $this->cor, $this->icone, $this->config['outline']);
    }
}
