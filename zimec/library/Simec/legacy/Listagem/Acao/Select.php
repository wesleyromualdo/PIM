<?php
/**
 * Implementação do renderizador da ação Select, utilizada na listagem.
 *
 * @version $Id: Select.php 94533 2015-02-26 20:19:25Z fellipesantos $
 * @see Simec_Listagem
 */

/**
 * Esta ação se parece com um checkbox, que pode ser marcado e desmarcado. Sempre que acontece a<br />
 * transição de estado, uma callback registrada no momento da criação da ação é chamada. É possível<br />
 * definir o estado inicial de todos os itens da lista, além de definir uma condição para marcação, ou não.<br />
 * Exemplo 1: Uso básico, os itens aparecem marcados.<br />
 * <pre>
 * $list = new Simec_Listagem(...);
 * ...
 * $list->addAcao('select', 'selecionarLinha');
 * ...
 * </pre>
 * Exemplo 2: Todos os itens aparecem desmarcados.<br />
 * <pre>
 * ...
 * $config = array();
 * $config['func'] = 'selecionarLinha';
 * $config['desmarcado'] = true;
 * $list->addAcao('select', $config);
 * ...
 * </pre>
 * Exemplo 3: Os itens são marcados condicionalmente.<br />
 * <pre>
 * ...
 * $config = array();
 * $config['func'] = 'selecionarLinha';
 * $config['verifica'] = array('campo' => 'diferenca', 'valor' => 0.00, 'op' => 'igual');
 * $list->addAcao('select', $config);
 * ...
 * </pre>
 * Para maiores detalhes da callback executada, veja o arquivo listagem.js e a função delegateAcaoSelect().
 * @see listagem.js
 */
class Simec_Listagem_Acao_Select extends Simec_Listagem_AcaoComID
{
    protected $icone = 'ok-circle';
    protected $cor = 'green';
    protected $titulo = 'Selecionar item';

    protected $icone2 = 'remove-circle';
    protected $cor2 = 'gray';
    protected $titulo2 = 'Remover item';

    protected function renderAcao()
    {    
        // -- Status inicial da ação: marcado ou desmarcado.
        $marcado = true;
        if (isset($this->config['desmarcado']) || $this->config['desmarcado']) {
            $marcado = false;
        }

        // -- Verificação de status da ação com base no valor de um campo
        if (isset($this->config['verifica'])) {
            $func = 'checa' . ucfirst($this->config['verifica']['op']?$this->config['verifica']['op']:'igual');
            $marcado = $this->$func(
                $this->dados[$this->config['verifica']['campo']],
                $this->config['verifica']['valor']
            );
        }

        $acao = <<<HTML
<span title="%s" class="glyphicon glyphicon-%s" style="cursor:pointer;color:%s" data-id="%s" data-cb="%s" data-exp="%s"></span>
HTML;
        return sprintf(
            $acao,
            $marcado?$this->titulo:$this->titulo2,
            $marcado?$this->icone:$this->icone2,
            $marcado?$this->cor:$this->cor2,
            $this->getAcaoID(),
            $this->callbackJS,
            str_replace('"', "'", simec_json_encode($this->paramsAsArray()))
        );
    }

    protected function paramsAsArray()
    {
        $parametros = array();
        foreach ($this->parametrosExternos as $param) {
            $parametros[$param] = $this->dados[$param];
        }
        foreach ($this->parametrosExtra as $param) {
            $parametros[$param] = $this->dados[$param];
        }
        return $parametros;
    }
}
