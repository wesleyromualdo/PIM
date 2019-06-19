<?php
/**
 *
 */

/**
 * Gerenciador de abas para o simec layout bootstrap.
 *
 * @param string $baseUrl A URL principal de acesso à tela que contém as abas.
 * @param string $requestParam O parâmetro em que virá a aba selecionada.
 * @param bool $incluirCssAdicional Indica se o CSS do bootstrap deve ser incluso - use apenas se tiver problemas de compatibilidade com o layout das abas.
 * @throws Exception
 *
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
class Simec_Abas {
    protected $abas = array();
    protected $regras = array();
    protected $requestParam;
    protected $baseUrl;
    protected $abaDefault;
    protected $abaAtiva;
    protected $incluirCssAdicional = false;

    protected $renderer;
    protected $config;

    const ABA_HORIZONTAL = 1;
    const ABA_VERTICAL = 2;

    public function __construct($baseUrl, $requestParam = 'aba',
                                $incluirCssAdicional = false, $tipoAba = self::ABA_HORIZONTAL)
    {
        if (empty($baseUrl)) {
            throw new Exception('A URL base das abas não pode ser deixada em branco.');
        }
        $this->requestParam = $requestParam;
        $this->baseUrl = "{$baseUrl}&{$this->requestParam}=";
        $this->incluirCssAdicional = $incluirCssAdicional;
        $this->setDefaultRenderer($tipoAba);
    }

    /**
     * @param $tipoAba
     */
    protected function setDefaultRenderer($tipoAba)
    {
        switch($tipoAba) {
            case self::ABA_VERTICAL:
                $this->renderer = new Simec_Abas_Renderer_Vertical();
                break;
            case self::ABA_HORIZONTAL:
            default:
                $this->renderer = new Simec_Abas_Renderer_Horizontal();
                break;
        }
    }

    /**
     *
     * @param bool $permiteInvalida Geralmente utilizado quando uma aba é criada apenas quando ela é solicitada.
     * @return type
     */
    public function getAbaAtiva($permiteInvalida = false)
    {
        if (!empty($this->abaAtiva)) {
            return $this->abaAtiva;
        }

        $this->setAbaAtiva(null, $permiteInvalida);
        return $this->abaAtiva;
    }

    /**
     *
     * @param type $abaAtiva
     * @param bool $permiteInvalida Geralmente utilizado quando uma aba é criada apenas quando ela é solicitada.
     * @return \Simec_Abas
     */
    public function setAbaAtiva($abaAtiva = null, $permiteInvalida = false)
    {
        if (!is_null($abaAtiva) && key_exists($abaAtiva, $this->abas)) {
            $this->abaAtiva = $abaAtiva;

            return $this;
        }

        $abaRequest = $_REQUEST[$this->requestParam];
        if (is_null($abaAtiva)
            && !empty($abaRequest)
            && (key_exists($abaRequest, $this->abas) || $permiteInvalida)
        ) {
            $this->abaAtiva = $_REQUEST[$this->requestParam];

            return $this;
        }

        $this->abaAtiva = $this->abaDefault;
    }

    /**
     * Adiciona uma nova abao ao gerenciador de abas.
     *
     * @param string $nomeAba Identificador da aba. Será utilizado como parâmetro na requisição para indicar a aba selecionada.
     * @param string $titulo Texto exibido na aba.
     * @param string $require
     * @param string $glyphicon Glyphicon do bootstrap. Utilize apenas a classe que define a imagem.
     * @param array $params
     * @return \Simec_Abas
     * @throws Exception
     */
    public function adicionarAba($nomeAba, $titulo, $require, $glyphicon = null, array $params = array())
    {
        if (key_exists($nomeAba, $this->abas)) {
            throw new Exception("Já foi criada uma aba com o nome '{$nomeAba}'.");
        }

        if (!empty($require) && !is_file($require)) {
            throw new Exception("O arquivo '{$require}' não existe. O caminho está correto?");
        }

        $this->abas[$nomeAba] = array(
            'titulo' => $titulo,
            'require' => $require,
            'icon' => $glyphicon,
            'params' => $params
        );

        return $this;
    }

    /**
     *
     * @param type $nomeAba
     * @param type $regra
     * @return \Simec_Abas
     * @todo Fazer uma classe de condição que pode ser utilizada pelas demais, afim de ficar tudo igual
     */
    public function adicionarCondicaoAba($nomeAba, $regra)
    {
        $this->regras[$nomeAba] = $regra;
//        if (is_string($regra) && is_callable($regra)) { // -- função callback
//        } elseif (is_array($regra)) {
//        } elseif (is_callable($regra)) {
//        }
        //array(array('var' => 'valor', 'valor' => 0.00, 'op' => 'diferente'));
        // -- pode ser array, ou pode ser function
        return $this;
    }

    protected function verificarCondicao($nomeAba)
    {
        if (!isset($this->regras[$nomeAba])) {
            return true;
        }

        foreach ($this->regras[$nomeAba] as $regra) {
            $method = $regra['op'];
            if (!Simec_Operacoes::$method($regra['op1'], $regra['op2'])) {
                return false;
            }
        }

        return true;
    }

    public function definirAbaDefault($abaDefault)
    {
        if (!key_exists($abaDefault, $this->abas)) {
            throw new Exception("A aba '{$abaDefault}' não existe na atual lista de abas.");
        }

        $this->abaDefault = $abaDefault;

        return $this;
    }

    /**
     * Faz a renderização das abas na tela, e já faz o require do arquivo de conteúdo da aba.
     * Importante: Quando a aba é inclusa pela função render(), o escopo do arquivo não é o global
     * então as variáveis globais não podem ser acessadas no arquivo, a menos que ocorra a declaração
     * de variáveis globais (global $var). Para evitar isso, utilize render(true) e dê um require
     * no retorno da função render(true);
     *
     * @param true $retornaCaminhoAba Indica que o arquivo de conteúdo da aba não deve ser incluído e sim retornado (veja observações).
     * @return null|string
     */
    public function render($retornaCaminhoAba = false)
    {
        $this->setAbaAtiva();

        $listaAbas = array();
        $i = 0;

        foreach ($this->abas as $aba => $config) {

            if (!$this->verificarCondicao($aba)) {
                continue;
            }

            // -- criando o título da aba
            $titulo = '';
            if ($config['icon']) {
                $glyphicon = '<span class="glyphicon glyphicon-%s"></span> ';
                $titulo = sprintf($glyphicon, $config['icon']);
            }
            $titulo .= $config['titulo'];
            $listaAbas[] = array(
                'id' => ++$i,
                'descricao' => $titulo,
                'link' => "{$this->baseUrl}{$aba}" . $this->montaParametros()
            );
        }

        $this->config['listaAbas'] = $listaAbas;
        $this->config['url'] = "{$this->baseUrl}{$this->abaAtiva}" . $this->montaParametros();
        $this->config['incluirCssAdicional'] = $this->incluirCssAdicional;

        $this->renderer->setConfig($this->config);

        $this->renderer->render();

        // -- Fazendo o include da aba
        if ($this->abaAtiva && $this->renderer instanceof Simec_Abas_Renderer_Horizontal) {
            if ($retornaCaminhoAba) {
                return $this->abas[$this->abaAtiva]['require'];
            }

            require $this->abas[$this->abaAtiva]['require'];
        }
    }

    protected function montaParametros()
    {
        $params = $this->abas[$this->abaAtiva]['params'];
        if (!empty($params)) {
            return '&' . http_build_query($params);
        }

        return '';
    }

    public function getAbas()
    {
        return $this->abas;
    }
}
