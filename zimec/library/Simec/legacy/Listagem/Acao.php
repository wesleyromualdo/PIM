<?php
/**
 * Implementa a classe base de criação de ações.
 * @version $Id: Acao.php 96257 2015-04-09 19:32:53Z maykelbraz $
 */

/**
 * Implementação base de todas as ações da listagem. Faz o processamento de condições e parâmetros adicionais
 * da ação. Também faz o parse das ações para o padrão bootstrap e retorna a string HTML da ação (dentro de uma TD).
 *
 * @todo Transformar ações em singletons
 */
abstract class Simec_Listagem_Acao
{
    /**
     * Indica que o parâmetro adicionado é um parametro extra e faz parte da linha de dados do banco.
     */
    const PARAM_EXTRA = 1;
    /**
     * Indica que o parâmetro adicionado é um parâmetro externo e seu valor é inserido diretamente na callback.
     */
    const PARAM_EXTERNO = 2;

    protected $icone;
    protected $titulo;
    protected $callbackJS;
    protected $dados;

    protected $condicoes = array();

    protected $parametrosExtra = array();
    protected $parametrosExternos = array();

    protected $partesID = array();

    /**
     * Lista de configuração adicionais da ação - estas configurações são utilizadas na classe da ação.
     * @var array
     */
    protected $config = array();

    public function __construct()
    {
        if (!isset($this->icone)) {
            throw new Exception('Esta ação não tem um ícone definido para ela.');
        }
    }

    public function setTitulo($titulo)
    {
        if (!empty($titulo)) {
            $this->titulo = $titulo;
        }
        return $this;
    }

    public function setCallback($callback)
    {
        if (empty($callback)) {
            throw new Exception('A callback da ação não pode ser vazia.');
        }
        $this->callbackJS = $callback;
        return $this;
    }

    public function setDados(array $dados)
    {
        $this->dados = $dados;
        return $this;
    }

    public function addCondicao(array $condicao)
    {
        $this->condicoes[] = $condicao;
        return $this;
    }

    public function addParams($tipo, $params)
    {
        switch ($tipo) {
            case self::PARAM_EXTRA:
                $tpParam = 'parametrosExtra';
                break;
            case self::PARAM_EXTERNO:
                $tpParam = 'parametrosExternos';
                break;
        }

        if (is_array($params)) {
            foreach ($params as $key => $param) {
                $this->{$tpParam}[$key] = $param;
            }
        } else {
            $this->{$tpParam}[$key] = $param;
        }
        return $this;
    }

    public function setPartesID($partesID)
    {
        if (!is_array($partesID) && !empty($partesID)) {
            $partesID = array($partesID);
        }

        foreach ($partesID as $parteID) {
            if (!key_exists($parteID, $this->dados)) {
                throw new Exception("A parte do ID '{$parteID}' não existe no conjunto de dados da listagem.");
            }

            $this->partesID[] = $parteID;
        }

        return $this;
    }

    /**
     * Armazena as configurações adicionais de cada ação.
     *
     * @param array $config Lista de configurações especiais da ação.
     * @return \Simec_Listagem_Acao
     */
    public function setConfig(array $config)
    {
        if (empty($config)) {
            return;
        }

        $this->config = $config;
        return $this;
    }

    /**
     * Retorna o HTML da ação.
     * @return string
     */
    public function render()
    {
        if (empty($this->dados)) {
            trigger_error('Não há dados associados a esta ação.', E_USER_ERROR);
        }

        if (empty($this->callbackJS)) {
            trigger_error('Não há uma callback JS associada a esta ação.', E_USER_ERROR);
        }

        $html = <<<HTML
            <td class="text-center" style="width:33px">%s</td>
HTML;

        // -- Ação não atende condição de exibição
        if (!$this->exibirAcao()) {
            return sprintf($html, '-');
        }

        return sprintf($html, $this->renderAcao());
    }

    protected function renderAcao()
    {
        $acao = <<<HTML
<a href="javascript:%s(%s);" title="%s">
    <span class="glyphicon glyphicon-%s"></span>
</a>
HTML;
        return sprintf(
            $acao,
            $this->callbackJS,
            $this->getCallbackParams(),
            $this->titulo,
            $this->icone
        );
    }

    /**
     * Cria a lista de parametros que será utilizada na chamada da callback.
     * Existem 3 categorias de parâmetros:<br />
     * <ul><li><b>id</b>: O primeiro parâmetro é o id da linha. O valor do id da linha é o valor<br />
     * do primeiro campo da linha de dados;</li>
     * <li><b>parâmetros extra</b>: Ao definir a ação, o usuário pode indicar um conjunto de valores adicionais<br />
     * que devem ser passados para a callback. Eles vem logo depois do id.</li>
     * <li><b>parâmetros externos</b>: Também na definição da função, o usuário pode indicar um conjunto de valores<br />
     * externos (não inclusos no conjunto de dados da linha) que são inclusos na lista de parâmetros log depois dos<br />
     * parâmetros extra.</li></ul>
     *
     * @param bool $paramsComoArray Indica que os parâmetros da callback devem ser retornados como um array.
     * @return string
     * @throws Exception Se algum parâmetro que não existe nos dados da linha for informado na lista de parâmetros extras, é gerada uma exceção.
     * @todo Implementar passagem do nome do parametro como chave do array
     */
    protected function getCallbackParams($paramsComoArray = false)
    {
        $params = array();
        $params[] = "'" . current($this->dados) . "'"; // -- Informando o primeiro parâmetro sempre como string

        // -- parametros extras
        foreach ($this->parametrosExtra as $param) {
            if (!key_exists($param, $this->dados)) {
                trigger_error("O parâmetro '{$param}' não existe no conjunto de dados da listagem.", E_USER_ERROR);
            }
            $params[$param] = "'{$this->dados[$param]}'";
        }
        // -- parametros extras
        foreach ($this->parametrosExternos as $key => $param) {
            $params[$key] = "'{$param}'";
        }

        // -- Transformar a lista de parâmetros em um array
        if ($paramsComoArray) {
            $params = '[' . implode(', ', $params) . ']';
            if (!is_null($parametroinicial)) {
                $params = "'{$parametroinicial}', {$params}";
            }
        } else {
            if (!is_null($parametroinicial)) {
                array_unshift($params, "'{$parametroinicial}'");
            }
            $params = implode(', ', $params);
        }

        return $params;
    }

    protected function getAcaoID()
    {
        if (empty($this->partesID)) {
            reset($this->dados);
            return current($this->dados);
        }

        $partes = array_intersect_key(
            $this->dados,
            array_combine(
                $this->partesID,
                array_fill(0, count($this->partesID), null)
            )
        );
        return implode('_', $partes);
    }

    public function __toString()
    {
        return $this->render();
    }

    protected function exibirAcao()
    {
        if (empty($this->condicoes)) {
            return true;
        }

        // -- Se houver uma entrada para a coluna no array de condicionais, as condições devem ser avaliadas
        foreach ($this->condicoes as &$condicao) {
            if (empty($condicao['op'])) {
                $condicao['op'] = 'igual';
            }
            $method = 'checa' . ucfirst($condicao['op']);
            // -- Se ao menos uma das condições falhar, a ação não deve ser exibida
            if (!$this->$method($this->dados[$condicao['campo']], $condicao['valor'])) {
                return false;
            }
        }

        return true;
    }

    protected function checaIgual($val1, $val2)
    {
        return $val1 == $val2;
    }

    protected function checaDiferente($val1, $val2)
    {
        return $val1 != $val2;
    }

    protected function checaContido($val1, $val2)
    {
        return in_array($val1, $val2);
    }
}
