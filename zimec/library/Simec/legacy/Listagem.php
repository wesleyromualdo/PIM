<?php
/**
 * Classe para criação de listagens.
 *
 * @version $Id: Listagem.php 95795 2015-03-25 14:57:46Z werteralmeida $
 */

/**
 * Construtor de ações.
 * @see Simec_Listagem_FactoryAcao
 * @see Simec_Listagem_Acao
 * @see Simec_Listagem_AcaoComID
 */
require_once dirname(__FILE__) . '/Listagem/FactoryAcao.php';
/**
 * Renderizador HTML.
 * @see Simec_Listagem_Renderer
 */
require_once dirname(__FILE__) . '/Listagem/Renderer/Html.php';
/**
 * Renderizador HTML.
 * @see Simec_Listagem_Renderer
 */
require_once dirname(__FILE__) . '/Listagem/Renderer/Xls.php';
/**
 * Classe com operações matemáticas para a renderização da listagem e ações.
 * @see Simec_Operacoes
 */
require_once dirname(__FILE__) . '/Operacoes.php';

/**
 * Classe de criação de listagens.
 */
class Simec_Listagem
{
    const RELATORIO_PAGINADO = 1;
    const RELATORIO_CORRIDO = 2;
//    const RELATORIO_IMPRESSAO = 3;
//    const RELATORIO_CSV = 4;
    const RELATORIO_XLS = 5;
//    const RELATORIO_PDF = 6;

    /**
     * Indica que a saída do relatório será no momento de sua execução.
     */
    const RETORNO_PADRAO = false; // -- SAIDA_PRINT
    /**
     * Indica que a saída do relatório será bufferizada e retornada.
     */
    const RETORNO_BUFFERIZADO = true; // -- SAIDA_RETORNO

    /**
     * Não inclui um rodapé na listagem.
     */
    const TOTAL_SEM_TOTALIZADOR = 1;
    /**
     * Inclui um totalizador na tabela onde é mostrada a quantidade de registros.
     */
    const TOTAL_QTD_REGISTROS = 2;
    /**
     * Inclui um totalizador na tabela onde é exibido o total de uma coluna.
     * @see Simec_Listagem::setColunaSomatorio();
     * @todo implementar Simec_Listagem::setColunaSomatorio();
     */
    const TOTAL_SOMATORIO_COLUNA = 3;

    /**
     * Identifica que a query deve ser retornada com um SELECT COUNT(1) externo.
     */
    const QUERY_COUNT = 1;
    /**
     * Identifica que a query deve ser retornada sem alterações.
     */
    const QUERY_NORMAL = 2;

    /**
     * Indica ao renderizador que deve imprimir mensagem de aviso se não houver registros.
     */
    const SEM_REGISTROS_MENSAGEM = true;
    /**
     * Indica ao renderizador que deve retornar false se não houver registros.
     */
    const SEM_REGISTROS_RETORNO = false;

    /**
     * Indica o tipo de saída do relatório.
     * @var integer
     * @see Simec_Listagem::RELATORIO_PAGINADO
     * @see Simec_Listagem::RELATORIO_CORRIDO
     */
    protected $tipoRelatorio;

    /**
     * Armazena o tipo de saída do relatório.
     * @var bool
     * @see Simec_Listagem::RETORNO_PADRAO
     * @see Simec_Listagem::RETORNO_BUFFERIZADO
     */
    protected $bufferizarRetorno;

    /**
     *  Instância do renderer responsável por criar/formatar o conteúdo do relatório.
     * @var Simec_Listagem_Renderer_Abstract
     * @see Simec_Listagem_Renderer_Abstract
     */
    protected $renderer;

    /**
     * Armazena a query que será executada para carregar os dados da listagem.
     * @var string
     * @see Simec_Listagem::setQuery()
     */
    protected $query;

    /**
     * Define tempo de armazenamento em cache da query.
     * Valor 0 armazena a query para sempre e o valor definido não deve ser <br />
     * superior a 2592000 (30 dias) que este limite. A unidade de tempo é segundos.
     *
     * @var integer
     * @see cls_banco::carregar()
     * @see Simec_Listagem::setQuery()
     * @see classes_simec.inc
     */
    protected $queryTimeout = null;

    /**
     * Conjunto de dados completos da listagem. Utilizado apenas quando são setados dados diretamente no objeto.
     * @var array
     */
    protected $dados;

    /**
     * Indica o tipo de totalizador do relatório.
     *
     * @var int
     * @see Simec_Listagem::setTotalizador()
     * @see Simec_Listagem::totalizarColunas()
     * @see Simec_Listagem::TOTAL_SEM_TOTALIZADOR
     * @see Simec_Listagem::TOTAL_QTD_REGISTROS
     * @see Simec_Listagem::TOTAL_SOMATORIO_COLUNA
     */
    protected $totalizador = Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

    /**
     * Configurações de legenda do relatório.
     * @var string
     * @todo Implementar a criação de legendas no relatório.
     */
    protected $legenda;

    /**
     * Quantidade de registros por página.
     * @var int
     */
    protected $tamanhoPagina = 100;

    /**
     * O número da página que deverá ser exibida.
     * @var int
     */
    protected $paginaAtual = 1;

    /**
     * Número máximo de páginas que serão exibidas no seletor de páginas.
     * @var int
     */
    protected $numPaginasSeletor = 7;

    /**
     * Total de registros da listagem.
     * @var int
     */
    protected $totalRegistros = null;

    /**
     * Número de páginas no relatório.
     * @var int
     */
    protected $numPaginas;

    /**
     * Criar uma nova listagem com configuração de tipo de relatório (paginado ou não) e o tipo de <br />
     * retorno (padrão ou bufferizado).
     *
     * @param integer $tipoRelatorio Tipo de listagem que será criada.
     * @param int $tipoRetorno
     *      Indica se a saída da listagem deve ser retornada em uma variável (self::RETORNO_BUFFERIZADO) ou
     *      deve ser exibida diretamente na tela (self::RETORNO_PADRAO).
     */
    public function __construct($tipoRelatorio = self::RELATORIO_PAGINADO, $tipoRetorno = self::RETORNO_PADRAO)
    {
        $this->setTipoRelatorio($tipoRelatorio);
        $this->setTipoRetorno($tipoRetorno);

        switch ($tipoRelatorio) {
            case Simec_Listagem::RELATORIO_XLS:
                $this->renderer = new Simec_Listagem_Renderer_Xls();
                break;
            default:
                $this->renderer = new Simec_Listagem_Renderer_Html();
        }

        // -- Carrega os dados externos enviados à listagem (paginação, filtros e ordenação)
        $this->carregarDadosExternos();
    }

    /**
     * Define um novo renderizador para o compomente de listagem de dados
     * @param Simec_Listagem_Renderer_Abstract $renderer
     * @return $this
     */
    public function setRenderer(Simec_Listagem_Renderer_Abstract $renderer)
    {
        $this->renderer = $renderer;
        return $this;
    }

    /**
     * Define uma nova quantidade de registros para compor uma página da listagem (número de registros por página).
     * @param int $tamanhoPagina Novo tamanho da página.
     * @return Simec_Listagem
     */
    public function setTamanhoPagina($tamanhoPagina)
    {
        $this->tamanhoPagina = $tamanhoPagina;
        return $this;
    }

    /**
     * Retorna a quantidade de registros de uma pagina.
     * @return int
     */
    public function getTamanhoPagina()
    {
        return $this->tamanhoPagina;
    }

    public function setFormOff()
    {
        $this->renderer->renderizarForm = false;
        return $this;
    }

    /**
     * Transfere as chamadas de métodos não definidos para o renderizador.
     *
     * @param string $name O Nome do método chamado.
     * @param string $arguments Lista de argumentos do método chamado.
     * @return mixed|Simec_Listagem
     * @throws Exception Se o método não está implementado no renderizador, lança uma exceção.
     */
    public function __call($name, $arguments)
    {
        // -- Verifica se a função solicitada está disponível no renderer
        if (!is_callable(array($this->renderer, $name)) || !method_exists($this->renderer, $name)) {
            $rendererClass = get_class($this->renderer);
            throw new Exception("O método '{$name}' não está implementado no renderizador '{$rendererClass}'.");
        }
        $retorno = call_user_func_array(array($this->renderer, $name), $arguments);

        if (is_null($retorno)) {
            return $this;
        }
    }

    protected function setTipoRelatorio($tipoRelatorio)
    {
        if ($tipoRelatorio != self::RELATORIO_PAGINADO
            && $tipoRelatorio != self::RELATORIO_CORRIDO
            && $tipoRelatorio != self::RELATORIO_XLS
                ) {
            throw new Exception(
                'Tipo de relatório inválido. Tipos válidos: Simec_Listagem:RELATORIO_PAGINADO ou Simec_Listagem::RELATORIO_CORRIDO.'
            );
        }

        $this->tipoRelatorio = $tipoRelatorio;
        return $this;
    }

    public function getPaginaAtual()
    {
        return $this->paginaAtual;
    }

    protected function setPaginaAtual($novaPaginaAtual)
    {
        $this->paginaAtual = $novaPaginaAtual;
        return $this;
    }

    /**
     * Carrega informações de páginação, filtros e ordenação (todos considerando o tipo do relatório).
     */
    protected function carregarDadosExternos()
    {
        if (Simec_Listagem::RELATORIO_PAGINADO == $this->tipoRelatorio) {
            $novaPagina = $_POST['listagem']['p'];
            if ('0' == $novaPagina || !$novaPagina) {
                $novaPagina = 1;
            }

            if ('all' == $novaPagina) {
                $this->paginaAtual = 'all';
            } else {
                $this->paginaAtual = (int)$novaPagina;
            }
        }

        // -- Carregando filtros, ordenação e número de página para o relatório renderizado em HTML
        if ($this->renderer instanceof Simec_Listagem_Renderer_Html) {
            $this->renderer->setFiltros($_POST['filtro']);
            $this->renderer->setOrdenacao($_POST['campo_ordenacao']);
            $this->renderer->setPaginaAtual($novaPagina);
        }
    }

    /**
     * Troca o tipo de saída do relatório.
     * Por padrão, o relatório é impresso na tela, mas a saída pode ser mudada para retorno do HTML.
     *
     * @param int $tipoRetorno Um dos tipos válidos de saída de relatório.
     * @throws Exception Lança exceção quando tipo informado é inválido.
     * @return \Simec_Listagem
     * @see Simec_Listagem::RETORNO_PADRAO
     * @see Simec_Listagem::RETORNO_BUFFERIZADO
     */
    public function setTipoRetorno($tipoRetorno)
    {
        if ($tipoRetorno != self::RETORNO_PADRAO && $tipoRetorno != self::RETORNO_BUFFERIZADO) {
            throw new Exception(
                'Tipo de saída inválido. Tipos válidos: Simec_Listagem:RETORNO_PADRAO ou Simec_Listagem::RETORNO_BUFFERIZADO.'
            );
        }
        $this->bufferizarRetorno = $tipoRetorno;
        return $this;
    }

    /**
     * Carrega no objeto os dados que serão utilizados para criar a listagem.
     * Não inclui o cabecalho da tabela. Esta função é uma alternativa a Simec_Listagem::setQuery().
     *
     * @param array $dados Dados para criação da listagem.
     * @return \Simec_Listagem
     * @see Simec_Listagem::setQuery()
     */
    public function setDados($dados)
    {
        
        if(is_array($dados)){
        $this->dados = $dados;
            return $this;
        } else {
            $this->setQuery("SELECT NULL WHERE false");
        }
    }

    /**
     * Carrega no objeto a query responsável por recuperar os dados que serão listados.
     * Esta função é uma alternativa a Simec_Listagem::setDados().
     *
     * @param string $query String SQL para carregar os dados da listagem.
     * @param int $queryTimeout Número de segundos que a query deve ficar armazenada em cache. O valor 0 faz com que seja armazenado para sempre.
     * @return \Simec_Listagem
     * @see Simec_Listagem::setDados()
     */
    public function setQuery($query, $queryTimeout = null)
    {
        $query = trim($query);
        if (!empty($query) && is_string($query)) {
            if (';' == substr($query, -1)) {
                $query = substr($query, 0, -1);
            }
            $this->query = $query;
        }

        // -- Definindo timeout de cache da query
        if (!is_null($queryTimeout)) {
            if ($queryTimeout > 2592000) {
                throw new Exception('O tempo de cache da query não deve esceder 2592000.');
            }
            $this->queryTimeout = $queryTimeout;
        }
        return $this;
    }

    /**
     * Retorna a query utilizada pelo relatório. Se o relatório for paginado, retorna a query para paginação.
     * @return string
     */
    protected function getQuery($formatoQuery = null)
    {
        if (empty($this->query)) {
            throw new Exception('Nenhuma query foi definida para a listagem.');
        }
        if (self::QUERY_COUNT == $formatoQuery) {
            return <<<DML
SELECT COUNT(1) FROM ({$this->query}) lst
DML;
        }
        if (self::QUERY_NORMAL == $formatoQuery || 'all' == $this->getPaginaAtual()) {
            return $this->query;
        }

        // -- Relatório sem paginação
        if ($this->tipoRelatorio != Simec_Listagem::RELATORIO_PAGINADO) {
            return $this->query;
        }

        // -- Relatório paginado
        return $this->query . ' OFFSET ' . $this->calculaOffset() . " LIMIT {$this->tamanhoPagina}";
    }

    /**
     * Calcula o offset da consulta com base na pagina selecionada atualmente.
     * @return int
     */
    protected function calculaOffset()
    {
        return (($this->paginaAtual - 1) * $this->tamanhoPagina);
    }

    /**
     * Define um título para o relatório.
     * @param string $titulo Título a ser exibido acima do relatório.
     */
    public function setTitulo($titulo)
    {
        $this->renderer->setTitulo($titulo);
        return $this;
    }

    /**
     * Lista de títulos das colunas do relatório. Também cria títulos de duas colunas,
     * para isso, passe o nome da coluna principal como chave do array e as colunas filhas como
     * itens deste array.
     * Exemplo cabecalho simples:
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Coluna 1', 'Coluna 2'));
     * Exemplo cabecalho de dois níveis:
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Grupo de colunas' => array('Coluna 1', 'Coluna 2'));
     *
     * @param mixed $cabecalho
     * @return \Simec_Listagem
     */
    public function setCabecalho($cabecalho)
    {
        $this->renderer->setCabecalho($cabecalho);
        return $this;
    }

    /**
     * Configura o tipo de totalizador da listagem. Adicionalmente, pode
     * definir quais colunas serão totalizadas.
     *
     * @param int $totalizador Define o tipo de totalizador
     * @param string|array $colunas Coluna (ou lista de colunas) que serão totalizadas.
     * @return \Simec_Listagem
     * @see Simec_Listagem::totalizarColunas();
     * @see Simec_Listagem::TOTAL_SOMATORIO_COLUNA;
     * @see Simec_Listagem::TOTAL_QTD_REGISTROS;
     */
    public function setTotalizador($totalizador, $colunas = null)
    {
        $this->renderer->setTotalizador($totalizador, $colunas);
        return $this;
    }

    /**
     * Define quais colunas serão totalizadas.
     *
     * @param string|array $campos
     * @return \Simec_Listagem
     */
    public function totalizarColunas($campos)
    {
        $this->renderer->totalizarColunas($campos);
        return $this;
    }

    /**
     * Indica que um, ou mais, campo(s) da query não será(ão) exibido(s).
     *
     * @param string|array $nomeColuna Nome do campo da query que não será exibida na listagem.
     * @return \Simec_Listagem
     */
    public function esconderColunas($nomeColuna)
    {
        $this->renderer->esconderColunas($nomeColuna);
        return $this;
    }

    /**
     * Define as ações que estarão disponíveis no relatório. As ações são exibidas nas
     * primeiras colunas da listagem. As ações podem ser de dois tipos, simples e avançadas.
     *
     * Ações simples são definidas com o nome da ação (veja lista disponível de ações)
     * como chave do array e o nome da callback js que deverá ser invocada. O valor
     * passado para todas ações simples é o da primeira coluna da listagem e esta coluna
     * deixa de ser exibida na listagem.
     * Ex:
     * $listagem = new Simec_Listagem();
     * ...
     * $listagem->setAcoes(
     *      array('plus' => 'detalharItem')
     * );
     * ...
     * $listagem->render();
     *
     * Ações avançadas são definidas com o nome da ação (veja lista disponível de ações)
     * como chave do array e um array de configuração que inclui o nome da callback js e
     * os parâmetros extras que a ação pode receber. A ação PLUS, é uma ação especial e
     * adiciona um identificador para o ícone e também para a sua linha da listagem;
     * Ex:
     * $listagem = new Simec_Listagem();
     * ...
     * $listagem->setAcoes(
     *     array('plus' => array(
     *         'func' => 'detalharItem',
     *         'extra-params' => array('idLinha', 'exercicio')
     *     )
     * );
     * ...
     * $listagem->render();
     *
     * @param array $acoes Definições das ações que deverão ser encorporadas na listagem.
     * @see Simec_Listagem::acoesDisponiveis
     * @return \Simec_Listagem
     */
    public function addAcao($acao, $config)
    {

        // -- @todo: Verificar se o tipo de renderer suportar ações

        $this->renderer->addAcao($acao, $config);
        return $this;
    }

    /**
     * Define condições para que uma ação seja exibida em uma listagem. A ação em questão só será exibida se atender a<br />
     * todas as condições forem atendidas. A condição é criada verificando valores do conjunto de dados da listagem.<br />
     * Se mais de uma condição for definida para a ação, esta só será exibida se todas as condições forem atendidas.<br />
     * Exemplo de utilização:<pre>
     * $dados = array(array('valor' => 3.00), array('valor' => 0.00));
     *
     * $listagem = new Simec_Listagem();
     * $listagem->setDados($dados);
     * $listagem->setCabecalho(array('Valor'));
     * $listagem->addAcao('edit', 'editarValor');
     * $listagem->setAcaoComoCondicional('edit', array(array('campo' => 'valor', 'valor' => 0.00, 'op' => 'diferente')));
     * $listagem->render();</pre>
     * Desta forma, a ação de edição só será exibida se o valor do campo 'valor' for igual a 0.00.
     *
     * @param string|array $acao Nome da ação, ou ações, que serão exibidas de acordo com a condição definida.
     * @param array $condicoes Array de configuração da(s) condição(ões) de exibição da ação.
     * @return \Simec_Listagem
     */
    public function setAcaoComoCondicional($acao, array $condicoes)
    {

        // -- @todo: Verificar se o tipo de renderer suportar ações

        $this->renderer->setAcaoComoCondicional($acao, $condicoes);
        return $this;
    }

    /**
     * Adiciona uma nova regra de formatação de linha.
     * A nova regra deve atender ao formato armazenado em self::$regrasDeLinha:
     *
     * @param array $regra
     * @todo validar a estrutura da nova regra a ser adicionada
     * @see Simec_Listagem::$regrasDeLinha
     */
    public function addRegraDeLinha(array $regra)
    {
        $this->renderer->addRegraDeLinha($regra);
        return $this;
    }

    /**
     * Adiciona uma função callback de processamento de conteúdo de campo.
     * Uma ação comum que pode ser executada com este método, é a aplicação de máscara em um campo de CPF.
     *
     * Exemplo de utilização:<pre>
     * function mascaraReal($valor)
     * {
     * &nbsp;&nbsp;&nbsp;&nbsp;return "R$ {$valor}";
     * }
     * [...]
     * $listagem = new Simec_Listagem();
     * $listagem->setQuery("SELECT '3.00' AS valor");
     * $listagem->setCabecalho(array('Valor'));
     * $listagem->addCallbackDeCampo('valor', 'mascaraReal');
     * $listagem->render();</pre>
     *
     * @param string|array $nomeCampo Nome(s) do(s) campo(s) que receberá(ão) o tratamento.
     * @param string $nomeCallback Nome da função de processamento do campo. Ela deve retornar sempre uma string.
     * @return \Simec_Listagem
     * @throws Exception Gerada quando o nome da callback ou a própria função é inválida.
     */
    public function addCallbackDeCampo($nomeCampo, $nomeCallback)
    {
        $this->renderer->addCallbackDeCampo($nomeCampo, $nomeCallback);
        return $this;
    }

    /**
     * Se o renderer suportar, ativa o pesquisator - busca rápida, na listagem.
     *
     * @return \Simec_Listagem
     */
    public function turnOnPesquisator()
    {
        if (!method_exists($this->renderer, 'turnOnPesquisator')) {
            throw new Exception('O renderer atual não suporta o pesquisator.');
        }
        $this->renderer->turnOnPesquisator();
        return $this;
    }

    /**
     * @param bool $mostrarMensagem
     * @return bool|string
     * @throws Exception
     * @todo Bufferizar mensagem
     * @todo Bufferizar Paginador
     */
    public function render($mostrarMensagem = Simec_Listagem::SEM_REGISTROS_RETORNO)
    {
        if (!isset($this->dados) && !isset($this->query)) {
            throw new Exception('A listagem não pode ser renderizada sem dados. Utilize Simec_Listagem::setDados() ou Simec_Listagem::setQuery() para carregar os dados da listagem.');
        }

        // -- Armazenando a saída em um buffer do relatório
        if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
            ob_start();
        }

        // -- Verificando se há registros para serem listados.
        if (0 == $this->getTotalRegistros()) {
            // -- Tipo de retorno se não houver registros
            if (Simec_Listagem::SEM_REGISTROS_MENSAGEM == $mostrarMensagem) {
                echo <<<HTML
<div style="margin-top:20px;" class="alert alert-info col-md-4 col-md-offset-4 text-center">Nenhum registro encontrado</div>
<br style="clear:both" />
<br />
HTML;
            }
            if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
                $listagem = ob_get_contents();
                ob_end_clean();

                return $listagem;
            } else {
                return false;
            }
        }

        // -- Verifica se a página atualmente solicitada é válida, senão, joga para a primeira página
        if ((Simec_Listagem::RELATORIO_PAGINADO == $this->tipoRelatorio)
             && ($this->getNumPaginas() < $this->getPaginaAtual())
             && ($this->getPaginaAtual() != 'all')) {
            $this->setPaginaAtual(1);
        }

        $this->renderer->setDados($this->getPaginaAtualDeDados());

        // -- Tratar a paginação aqui, fornecendo para o renderer apenas os dados para renderização
        $this->renderer->render();

        if (Simec_Listagem::RELATORIO_PAGINADO == $this->tipoRelatorio) {
            // -- Inclui a seleção de páginas no final da listagem
            $this->renderPaginador();
        }

        // -- Armazenando a saída em um buffer do relatório
        if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
            $listagem = ob_get_contents();
            ob_end_clean();

            return $listagem;
        }
    }

    /**
     * Retorna o conjunto de dados referente à página atual de dados. Retorna a página atual
     * de dados independente se for um array ou uma query.
     * Faz um ajuste na página solicitada transformando em página 1 se a página requisitada for inválida.
     *
     */
    protected function getPaginaAtualDeDados()
    {
        // -- Ajuste de página solicitada inválida
        if (Simec_Listagem::RELATORIO_PAGINADO == $this->tipoRelatorio) {
            if (($this->getPaginaAtual() > $this->getNumPaginas())
                && ($this->getPaginaAtual() != 'all')) {
                $this->setPaginaAtual(1);
            }
        }

        if (isset($this->dados)) {
            if ((Simec_Listagem::RELATORIO_PAGINADO == $this->tipoRelatorio)
                 && ('all' != $this->getPaginaAtual())) {
                return array_slice($this->dados, $this->calculaOffset(), $this->tamanhoPagina);
            } else {
                return $this->dados;
            }
        } elseif (isset($this->query)) {
            if (Simec_Listagem::RELATORIO_PAGINADO == $this->tipoRelatorio) {
                return $this->queryDatabase($this->getQuery());
            } else {
                return $this->queryDatabase($this->getQuery(Simec_Listagem::QUERY_NORMAL));
            }
        }
    }

    /**
     * @todo Mover para o renderer??? Abstract de paginador???
     * @return type
     */
    protected function renderPaginador()
    {
        // -- Se não foi preciso paginar, não exibe o paginador
        if ($this->getTotalRegistros() < $this->tamanhoPagina) {
            return;
        }

        echo <<<HTML
        <div class="row container-listing">
            <div class="col-lg-12" style="padding-bottom:20px;text-align:center">
HTML;

        if ('all' == $this->paginaAtual) {
            echo <<<HTML
                <ul class="pagination">
                    <li class="pgd-item" data-pagina="1">
                        <a href="javascript:void(0)">Paginar</a>
                    </li>
                </ul>
HTML;
        } else {

            $paginaAnterior = ((int)$this->paginaAtual - 1);
            $desabilitarAnterior = '';
            if ($paginaAnterior <= 0) {
                $desabilitarAnterior = ' disabled';
            }
            echo <<<HTML
                    <ul class="pagination">
                        <li class="pgd-item{$desabilitarAnterior}" data-pagina="{$paginaAnterior}">
                            <a href="javascript:void(0);">&laquo;</a>
                        </li>
HTML;
            if ((int)$paginaAnterior >= 2) {
                echo <<<HTML
                        <li class="pgd-item" data-pagina="1">
                            <a href="javascript:void(0);">&laquo; 1</a>
                        </li>
HTML;
            }
            $listaPaginas = $this->gerarListaPaginas();

            // -- Imprimindo as páginas do seletor
            foreach ($listaPaginas as $numPagina) {
                $paginaAtualCSS = '';
                if ($this->paginaAtual == $numPagina) {
                    $paginaAtualCSS = ' active';
                }
                echo <<<HTML
                        <li class="pgd-item{$paginaAtualCSS}" data-pagina="{$numPagina}">
                            <a href="javascript:void(0)">{$numPagina} </a>
                        </li>
HTML;
            }
            $ultimaPagina = $this->getNumPaginas();
            if (!in_array($ultimaPagina, $listaPaginas)) {
                echo <<<HTML
                        <li class="pgd-item" data-pagina="{$ultimaPagina}">
                            <a href="javascript:void(0)">{$ultimaPagina} &raquo;</a>
                        </li>
HTML;
            }
            $desabilitarProxima = '';
            if ($this->paginaAtual == $ultimaPagina) {
                $desabilitarProxima = ' disabled';
            }
            $proximaPagina = $this->paginaAtual + 1;
            echo <<<HTML
                    <li class="pgd-item{$desabilitarProxima}" data-pagina="{$proximaPagina}">
                        <a href="javascript:void(0)">&raquo;</a>
                    </li>
                </ul>
                <ul class="pagination">
                    <li class="pgd-item" data-pagina="all">
                        <a href="javascript:void(0)">Mostrar todos</a>
                    </li>
                </ul>
HTML;
        }
        echo <<<HTML
            </div>
        </div>
HTML;
    }

    protected function gerarListaPaginas()
    {
        $metadeDasPaginas = floor($this->numPaginasSeletor / 2);
        $qtdPaginasAnteriores = -1 * $metadeDasPaginas;
        $qtdPaginasPosteriores = $metadeDasPaginas;
        $listaPaginas = array();

        // -- A lista de páginas que devem ser exibidas
        for ($qtdPaginasAnteriores; $qtdPaginasAnteriores <= 0; $qtdPaginasAnteriores++) {
            // -- Se a página for menor que zero, não exibe a página e cria uma nova página posterior
            if ($this->paginaAtual + $qtdPaginasAnteriores <= 0) {
                $qtdPaginasPosteriores++;
                continue;
            }
            $listaPaginas[] = $this->paginaAtual + $qtdPaginasAnteriores;
        }

        for ($i = 1; $i < $qtdPaginasPosteriores + ($this->numPaginasSeletor % 2); $i++) {
            if ($this->paginaAtual +$i > $this->getNumPaginas()) {
                break;
            }
            $listaPaginas[] = $this->paginaAtual + $i;
        }

        return $listaPaginas;
    }

    protected function getTotalRegistros()
    {
        // -- Se o total de registros já estiver carregando, retorna-o
        if (!is_null($this->totalRegistros)) {
            return $this->totalRegistros;
        }
        // -- Carregando ophp.t otal de registros
        if (empty($this->query)) { // -- à partir de um array de dados
            $this->totalRegistros = count($this->dados);
            return $this->totalRegistros;
        }

        return $this->totalRegistros = current(
            current(
                $this->queryDatabase($this->getQuery(Simec_Listagem::QUERY_COUNT))
            )
        );
    }

    protected function queryDatabase($query)
    {
        global $db;

        // -- Executando a query
        if ($db) {
            return $db->carregar($query, null, $this->queryTimeout);
        }
        // -- Zend
        if (class_exists(('Zend_Db_Table'))) {
            return Zend_Db_Table::getDefaultAdapter()->query($query)->fetchAll();
        }

        throw new Exception('Não foi possível estabelecer uma conexão com a base de dados.');
    }

    /**
     * Retorna a quantidade de páginas da listagem.
     * @return int
     */
    protected function getNumPaginas()
    {
        if (!isset($this->numPaginas)) {
            $this->numPaginas = ceil($this->getTotalRegistros() / $this->tamanhoPagina);
        }
        return $this->numPaginas;
    }

    /**
     * Adiciona um novo campo no formulário da listagem.
     * @param array $campos Configuração do campo com: id, name e type.
     */
    public function addCampo(array $campos)
    {
        $this->renderer->addCampo($campos);
        return $this;
    }

    public function mostrarImportar($mostrar = true)
    {
        $this->renderer->mostrarImportar($mostrar);
        return $this;
    }

    // -- Métodos depreciados ------------------------------------------------------------------------------------------

    /**
     * Utilizar Simec_Listagem:setTotalizador() ou Simec_Listagem::totalizarColunas()
     * para definir quais colunas do relatório serão totalizadas.
     *
     * @deprecated
     * @param string $nomeCampo
     * @return \Simec_Listagem
     */
    public function totalizarColuna($nomeCampo)
    {
        return $this->totalizarColunas($nomeCampo);
    }

    /**
     * Indica que um campo da query não será exibido.
     * Utilizar Simec_Listagem::esconderColunas() para esconder uma,
     * ou mais colunas.
     *
     * @deprecated
     * @param string $nomeCampo Nome do campo da query que não será exibida na listagem.
     * @return \Simec_Listagem
     */
    public function esconderColuna($nomeCampo)
    {
        return $this->esconderColunas($nomeCampo);
    }

    /**
     * Use Simec_Listagem::addAcao no lugar de setAcoes.
     *
     * @deprecated
     * @param type $acoes
     * @return \Simec_Listagem
     */
    public function setAcoes($acoes)
    {
        if (empty($acoes)) {
            return $this;
        }
        foreach ($acoes as $acao => $config) {
            $this->addAcao($acao, $config);
        }
        return $this;
    }

    /**
     * Utilize Simec_Listagem::setTipoRetorno();
     * @deprecated
     */
    public function trocaTipoSaida($tipoRetorno)
    {
        return $this->setTipoRetorno($tipoRetorno);
    }
}
