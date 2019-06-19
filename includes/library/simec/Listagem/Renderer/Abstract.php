<?php
/**
 * Interface de definição de Renderers de Simec_Listagem.
 *
 * @version $Id: Abstract.php 130256 2017-09-11 13:16:39Z saulocorreia $
 */

/**
 * Classe de encapsulamento das configurações da lista.
 * @see Simec_Listagem_Config
 */
require_once dirname(__FILE__) . '/../Config.php';

require_once dirname(__FILE__) . '/../Datasource.php';

/**
 *
 */
require_once dirname(__FILE__) . '/../TemplateParser.php';

/**
 * Classe abstrata base para renderizadores.
 *
 * Renderizadores são os responsáveis por gerar o conteúdo dos relatórios
 * conforme a saída necessitada pelo usuário. Extenda esse classe para criar
 * o seu próprio renderizador. Para exemplos de implementação, verifique a
 * classe Simec_Listagem_Renderer_Html.
 *
 * @package Simec\View\Listagem\Renderer
 * @see Simec_Listagem_Renderer_Html
 */
abstract class Simec_Listagem_Renderer_Abstract
{
    /**
     * @var Simec_Listagem_Config Configurações de exibição da listagem.
     */
    protected $config;

    /**
     * @var Simec_Listagem_Datasource Fonte de dados da listagem.
     */
    protected $datasource;

    /**
     * @var array Armazena os dados utilizados para montar a listagem.
     */
    protected $dados;

    /**
     * @var string Armazena o título do relatório, se estiver vazio, nenhum título é exibido.
     */
    protected $titulo = '';

    /**
     * @var array Lista de colunas totalizadas com a soma dos valores processados até o momento.
     */
    protected $totalColunas = array();

    /**
     * @var bool Atributo de ajuda para renderização do titulo.
     * @todo Remover a utilização deste campo.
     */
    protected $renderPrimeiroItem = true;

    /**
     * @var \Simec_Listagem_TemplateParser
     */
    protected $templateParser;

    /**
     * @var int|string Indica para o paginador qual é a página atualmente carregada.
     */
    protected $paginaAtual;
    
    /**
     * Construtor do renderizador.
     *
     * @param Simec_Listagem_Config $config Configuração das lista.
     */
    public function __construct(Simec_Listagem_Config $config)
    {
        $this->config = $config;
    }

    /**
     * Retorna a configuração do cabecalho da listagem.
     *
     * @param string $formato Define em qual datasource será retornado
     * @return string|string[]
     * @uses Simec_Listagem_Config::getCabecalho()
     */
    protected function getCabecalho($formato = null)
    {
        return $this->config->getCabecalho($formato);
    }

    /**
     * Define a configuração do cabecalho da listagem.
     *
     * @param string[] $cabecalho Nome das colunas da lista.
     * @param string $formato Define em qual formato o datasource sera utilizado
     * @uses Simec_Listagem_Config::setCabecalho()
     */
    protected function setCabecalho($cabecalho, $formato = null)
    {
        $this->config->setCabecalho($cabecalho, $formato);
    }

    /**
     * Retorna uma referência ao objeto de configuração de exibição da listagem.
     *
     * @return \Simec_Listagem_Config
     */
    public function getConfig()
    {
        return $this->config;
    }

    /**
     * Atribui ao renderer uma referência da fonte de dados.
     *
     * @param Simec_Listagem_Datasource $ds Fonte de dados da listagem.
     * @return \Simec_Listagem_Renderer_Abstract
     */
    public function setDatasource(Simec_Listagem_Datasource $ds)
    {
        $this->datasource = $ds;
        $this->dados = $this->datasource->getDados();
        return $this;
    }

    /**
     * Retorna uma referência ao total das colunas totalizadas.
     *
     * @return array
     * @uses \Simec_Listagem_Config::getColunasTotalizadas()
     */
    public function getColunasTotalizadas()
    {
        if (empty($this->totalColunas) && 0 !== $this->config->getColunasTotalizadas()) {
            $this->totalColunas = array_combine(
                $this->config->getColunasTotalizadas(),
                array_fill(0, count($this->config->getColunasTotalizadas()), 0)
            );
        }
        return $this->totalColunas;
    }

    /**
     * Carrega no objeto os dados que serão utilizados para criar a listagem.
     *
     * @param array $dados Dados para criação da listagem.
     * @uses self::$dados
     * @uses self::$renderPrimeiroItem
     */
    public function setDados($dados)
    {
        if (!is_array($dados)) {
            return false;
        }

        $this->dados = $dados;

        // -- Limpando indicador do primeiro campo a ser renderizado
        $this->renderPrimeiroItem = true;

        return $this;
    }

    /**
     * Adiciona um valor ao valor atual de uma coluna totalizada.
     *
     * @param string $nomeColuna Nome da coluna a ter o valor atualizado.
     * @param int|float $valor Valor a ser adicionado ao total da coluna.
     * @return \Simec_Listagem_Renderer_Abstract
     */
    public function somarColuna($nomeColuna, $valor)
    {
        if ($this->config->colunaEhTotalizada($nomeColuna)) {
            if (strpos($valor, '.')) {
                $valor = (double)$valor;
            } else {
                $valor = (int)$valor;
            }
            $this->totalColunas[$nomeColuna] += $valor;
        }

        return $this;
    }

    /**
     * Retorna o somatório de uma coluna.
     *
     * @param string $nomeColuna Nome da coluna.
     * @return int|float
     */
    public function getTotalColuna($nomeColuna)
    {
        return $this->totalColunas[$nomeColuna];
    }

    public function semDados()
    {
        return empty($this->dados);
    }

    /**
     * Define um título para o relatório.
     * @param string $titulo Título a ser exibido acima do relatório.
     */
    public function setTitulo($titulo)
    {
        if (!empty($titulo)) {
            $this->titulo = $titulo;
        }
    }

    /**
     * Verifica se uma coluna precisa de callback e, se necessário, aplica a função de callback associada.
     *
     * @param string $nomeColuna Nome da coluna para verificação.
     * @param mixed $valor Valor da coluna, parâmetro principal da callback.
     * @param mixed[] $parametros Parâmetros adicionais da callback: dados da linha, id da linha, array variado.
     * @return mixed
     */
    protected function aplicarCallback($nomeColuna, $valor, array $parametros = array())
    {
        if ($this->config->colunaTemCallback($nomeColuna)) {
            array_unshift($parametros, $valor);
            $valor = call_user_func_array(
                $this->config->getCallback($nomeColuna),
                $parametros
            );
        }
        return $valor;
    }


    /**
     * Faz a contagem de colunas da listagem, incluíndo colunas de ações (quando presentes).
     * @todo Precisa disso? Precisa ser assim??? oO
     * @return integer
     */
    protected function quantidadeDeColunasExibidas()
    {
        $numColunasOcultas = $this->config->getNumeroColunasOcultas();

        if ($numColunasOcultas != 0) {
            $qtdColunasExibidas = count(
                array_diff_key( // -- Criar um array temporário com os campos dados que não estão inclusos na listagem de colunas ocultas
                    $this->dados[0],
                    array_combine( // -- Cria um array temporário baseado nas colunas ocultas
                        $this->config->getColunasOcultas(),
                        array_fill(0, $numColunasOcultas, null)
                    )
                )
            );
        }
        // -- Ajuste da quantidade de colunas da query mediante contagem de ações
        $numAcoes = $this->config->getNumeroAcoes();
        if ($numAcoes > 1) {
            // -- -1 pq a coluna de ID já é contada em $qtdColunasExibidas
            $qtdColunasExibidas += $numAcoes - 1;
        }

        return $qtdColunasExibidas;
    }

    /**
     * Retorna uma instância do parser de template.
     *
     * @return Simec_Listagem_TemplateParser
     */
    public function getTemplateParser()
    {
        if (is_null($this->templateParser)) {
            $this->templateParser = new Simec_Listagem_TemplateParser(
                $this->config->getTemplate()
            );
        }

        return $this->templateParser;
    }

    /**
     * Renderiza a sessão de repetição do template.
     *
     * @param mixed[] $dados Dados para substituição no template.
     * @return type
     */
    public function renderRepeticaoTemplate($dados)
    {
        $repeticao = '';
        foreach ($dados as $linha) {
            $repeticao .= $this->getTemplateParser()->renderRepeticao($linha);
        }

        return $this->getTemplateParser()->replace($repeticao);
    }


    public function setPaginaAtual($pagina)
    {
        $this->paginaAtual = $pagina;
    }

    public function getPaginaAtual()
    {
        return $this->paginaAtual;
    }


    /**
     * Executa a criação da listagem de acordo com o Delegate implementado.
     */
    abstract public function render();

    /**
     * Renderiza um título para a listagem.
     */
    abstract public function renderTitulo();

    abstract protected function renderCabecalho();

    abstract protected function renderRodape();
}