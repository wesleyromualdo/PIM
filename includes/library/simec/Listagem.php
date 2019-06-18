<?php
/**
 * Implementação da classe de criação de listagens.
 *
 * @version $Id: Listagem.php 143340 2018-08-29 18:08:26Z juniosantos $
 * @filesource
 */

/**
 * Construtor de ações.
 *
 * @see Simec_Listagem_FactoryAcao
 */
require_once dirname(__FILE__) . '/Listagem/FactoryAcao.php';
/**
 * Renderizador HTML.
 *
 * @see Simec_Listagem_Renderer
 */
require_once dirname(__FILE__) . '/Listagem/Renderer/Html.php';
/**
 * Renderizador XLS.
 *
 * @see Simec_Listagem_Renderer
 */
require_once dirname(__FILE__) . '/Listagem/Renderer/Xls.php';
/**
 * Renderizador PDF.
 *
 * @see Simec_Listagem_Renderer
 */
//require_once dirname(__FILE__) . '/Listagem/Renderer/Pdf.php';
/**
 * Renderizador JSON.
 *
 * @see Simec_Listagem_Renderer
 */
if (file_exists(dirname(__FILE__) . '/Listagem/Renderer/Json.php')) {
    include_once(dirname(__FILE__) . '/Listagem/Renderer/Json.php');
}
/**
 * Classe com operações matemáticas para a renderização da listagem e ações.
 *
 * @see Simec_Operacoes
 */
require_once dirname(__FILE__) . '/Operacoes.php';
/**
 * Classe de encapsulamento de dados utilizados na listagem.
 *
 * @see Simec_Listagem_Datasource
 */
require_once dirname(__FILE__) . '/Listagem/Datasource.php';
/**
 * Armazenamento de configurações da listagem.
 *
 * @see Simec_Listagem_Config
 */
require_once dirname(__FILE__) . '/Listagem/Config.php';

require_once dirname(__FILE__) . '/Listagem/Datasource.php';
require_once dirname(__FILE__) . '/Listagem/Datasource/Array.php';
require_once dirname(__FILE__) . '/Listagem/Datasource/Query.php';

/**
 * Classe de criação de relatórios.
 *
 * *Importante*: Algumas opções avançadas são disponibilizadas pelos renderizadores. Para maiores informações,
 * ou opções extras, veja a documentação de cada um deles.
 *
 * Lista de funcionalidades
 *
 * * Saída: HTML, XLS;
 * * Suporte a arrays de dados e queries.
 * * Suporte a totalizadores e somatórios;
 * * Ações;
 * * Ações condicionais;
 * * Funções de callback para tratamento de dados do relatório (máscara, formatação, etc);
 * * Formatação condicional de linhas;
 * * Retorno bufferizado;
 * * Paginação;
 * * Suporte à customização de paginação;
 * * Exportação automática de XLSs;
 * * Debug de query dependente de nível de usuário;
 * * Suporte a implementação de novos renderizadores e fontes de dados;
 * * Integração com o workflow.
 *
 * @package Simec\View
 * @example
 * <code>
 * // -- Usando a lista com um array de dados e utilizando uma acao condicional
 * $dados = array(
 *     array('id' => 1, 'valor' => 3.00),
 *     array('id' => 2, 'valor' => 0.00),
 * );
 * $listagem = new Simec_Listagem();
 * $listagem->setDados($dados);
 * $listagem->setCabecalho(array('Valor'));
 * $listagem->addAcao('edit', 'editarValor'); // -- função javascript
 * $listagem->setAcaoComoCondicional('edit', array(
 *     array('campo' => 'valor', 'valor' => 0.00, 'op' => 'diferente'))
 * );
 * $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM); // -- html eh escrito na tela
 * </code>
 *
 * @example
 * <code>
 * // -- Usando a lista com retorno bufferizado e funcao de callback
 * $query = <<<DML
 * SELECT usucpf,
 *        usunome,
 *        usucpf AS cpfusuario
 *   FROM seguranca.usuario
 *   LIMIT 10
 * DML;
 * $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO)
 * $list->setQuery($query)
 *     ->setCabecalho(array('Nome', 'CPF'))
 *     ->addAcao('edit', 'editarUsuario') // -- função javascript
 *     ->addCallbackDeCampo('cpfusuario', 'formatarCpf'); // -- funcao PHP que retorne uma string
 *
 * $htmlLista = $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
 * </code>
 *
 * @author  Maykel S. Braz <maykelbraz@mec.gov.br>
 * @todo    Implementação de renderizadores PDF, CSV e IMPRESSÃO.
 * @method \Simec_Listagem setFormFiltros(string $ifFormFiltros) Associa um formulário de filtros à listagem - use em conjunto com a paginação para não perder os filtros atuais da listagem. Apena HTML.
 * @method \Simec_Listagem setOrdenacao(bool $exibirOrdenacao) Liga e desliga a interface de ordenação da lista através dos títulos das colunas. Apenas HTML.
 */
class Simec_Listagem
{
//    const RELATORIO_PDF = 6;
//    const RELATORIO_IMPRESSAO = 3;
//    const RELATORIO_CSV = 4;

    /**
     * Indica que o relatório HTML deve ser paginado.
     */
    const RELATORIO_PAGINADO = 1;
    /**
     * Indica que o relatório HTML não deve ser paginado.
     */
    const RELATORIO_CORRIDO = 2;
    /**
     * Indica que o relatório deve ser renderizado no formato XLS.
     */
    const RELATORIO_XLS = 5;
    /**
     * Indica que o relatório deve ser renderizado no formato PDF.
     */
    const RELATORIO_PDF = 6;

    /**
     * Indica que a chamada de self::render() irá imprimir o conteúdo do relatório.
     */
    const RETORNO_PADRAO = false; // -- SAIDA_PRINT
    /**
     * Indica que a chamada de self::render() irá retornar uma string com o conteúdo do relatório.
     */
    const RETORNO_BUFFERIZADO = true; // -- SAIDA_RETORNO

    /**
     * Não inclui um rodapé na listagem.
     */
    const TOTAL_SEM_TOTALIZADOR = 1;
    /**
     * Inclui um rodapé no relatório com a quantidade de registros.
     */
    const TOTAL_QTD_REGISTROS = 2;
    /**
     * Inclui um rodapé no relatório com o somatório das colunas indicadas.
     */
    const TOTAL_SOMATORIO_COLUNA = 3;

    /**
     * Indica ao renderizador que deve imprimir mensagem de aviso se não houver registros.
     */
    const SEM_REGISTROS_MENSAGEM = 1;
    /**
     * Indica ao renderizador que deve retornar false se não houver registros.
     */
    const SEM_REGISTROS_RETORNO = 2;
    /**
     * Indica ao renderizador que a tabela principal deve ser mostrada, com uma mensagem interna.
     * Utilizado para listas dinâmicas, que receberão elementos posteriormente.
     */
    const SEM_REGISTROS_LISTA_VAZIA = 3;
    protected static $monitorarExport = false;
    protected static $namespaceExport;
    /**
     * @var integer Indica o tipo de saída do relatório.
     * @uses Simec_Listagem::RELATORIO_PAGINADO Relatório HTML paginado.
     * @uses Simec_Listagem::RELATORIO_CORRIDO Relatório HTML sem paginação.
     * @uses Simec_Listagem::RELATORIO_XLS Relatório XLS.
     * @todo Precisa disso? Não dá pra fazer pelo renderer?
     */
    protected $tipoRelatorio = self::RELATORIO_PAGINADO;
    /**
     * @var bool Armazena o tipo de saída do relatório.
     * @uses Simec_Listagem::RETORNO_PADRAO
     * @uses Simec_Listagem::RETORNO_BUFFERIZADO
     */
    protected $bufferizarRetorno;
    /**
     * @var Simec_Listagem_Renderer_Abstract Instância do renderer responsável por criar/formatar o conteúdo do relatório.
     */
    protected $renderer;
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
     *
     * @var string
     * @todo Implementar a criação de legendas no relatório.
     */
    protected $legenda;
    /**
     * Número máximo de páginas que serão exibidas no seletor de páginas.
     *
     * @var int
     */
    protected $numPaginasSeletor = 7;
    /**
     * @var \Simec_Listagem_Config Configurações da listagem.
     */
    protected $config;
    /**
     * @var \Simec_Listagem_Datasource Fonte de dados da listagem.
     */
    protected $datasource;

    /**
     * Criar uma nova listagem com configuração de tipo de relatório (paginado ou não) e o tipo de <br />
     * retorno (padrão ou bufferizado).
     *
     * @param integer               $tipoRelatorio
     *      Tipo de listagem que será criada. Em listas HTML estão disponíveis as opções de
     *      RELATORIO_PAGINADO e RELATORIO_CORRIDO, também pode ser um RELATORIO_XML.
     * @param int                   $tipoRetorno
     *      Indica se a saída da listagem deve ser retornada em uma variável (self::RETORNO_BUFFERIZADO) ou
     *      deve ser exibida diretamente na tela (self::RETORNO_PADRAO).
     * @param Simec_Listagem_Config $config
     *      Conjunto de configurações pré-definidas para a Simec_Listagem. Essa opção geralmente é utilizada,
     *      quando a lista é criada automaticamente (nos casos de exportação e paginação, por exemplo)
     *
     * @uses Simec_Listagem_Config
     * @uses Simec_Listagem::RETORNO_PADRAO
     * @uses Simec_Listagem::RETORNO_BUFFERIZADO
     */
    public function __construct(
        $tipoRelatorio = self::RELATORIO_PAGINADO,
        $tipoRetorno = self::RETORNO_PADRAO,
        Simec_Listagem_Config $config = null
    )
    {
        $this->setTipoRetorno($tipoRetorno);
        if (is_null($config)) {
            $config = new Simec_Listagem_Config();
            $config->paginarLista($tipoRelatorio == self::RELATORIO_PAGINADO);
        }

        $this->config = $config;

        switch ($tipoRelatorio) {
            case Simec_Listagem::RELATORIO_XLS:
                $this->renderer = new Simec_Listagem_Renderer_Xls($this->config);
                break;
            case Simec_Listagem::RELATORIO_PDF:
                $this->renderer = new Simec_Listagem_Renderer_Pdf($this->config);
                break;
            default:
                $this->renderer = new Simec_Listagem_Renderer_Html($this->config, self::$monitorarExport);
        }

        // -- Carrega os dados externos enviados à listagem (paginação, filtros e ordenação)
        $this->carregarDadosExternos();
    }

    /**
     * Inicia o monitoramento da Simec_Listagem.
     *
     * A lista intercepta requisições com a assinatura $_POST['listagem']. Todas estas requisições são
     * tratadas automaticamente pela lista e seu retorno é enviado para a saída. As seguintes funcionalidades
     * utilizam esse monitoramento:
     * * Ordenação: Realizada através de uma chamada POST via AJAX.
     * * Exportação de XLS: Realizada através de uma chamada POST comum.
     *
     * IMPORTANTE: Esta chamada deve ser incluída acima da chamada do arquivo controleAcesso.inc no arquivo
     * principal do módulo.
     *
     * @param string $namespace Identificador onde serão armazenadas as configurações da listagem.
     * @param array  $config
     */
    public static function iniciarMonitoramento($namespace, array $config = [])
    {
        self::$monitorarExport = true;
        self::$namespaceExport = $namespace;

        self::exportarXLS();
    }

    /**
     * Inicia o monitoramento de requisições da listagem.
     *
     * @param string $namespace Identificador onde serão armazenadas as configurações da listagem.
     *
     * @deprecated Utilizar Simec_Listagem::iniciarMonitoramento()
     */
    public static function monitorarExport($namespace)
    {
        self::iniciarMonitoramento($namespace);
    }

    /**
     * @todo Verificar a necessidade de chamar naoPaginar() diretamente, ao setar o tipo como XLS, já deveria não paginar
     */
    protected static function exportarXLS()
    {
        if (isset($_REQUEST['listagem']['requisicao'])) {

            switch ($_REQUEST['listagem']['requisicao']) {
                case 'exportar-xls':
                    $config = Simec_Listagem_Config::carregar(self::$namespaceExport, $_REQUEST['listagem']['id-lista']);
                    $list = new self(self::RELATORIO_XLS, self::RETORNO_PADRAO, $config);
                    // -- Forçando a não paginação mesmo sendo um XLS
                    $config->getDatasource('xls')->naoPaginar();
                    $list->render(Simec_Listagem::SEM_REGISTROS_RETORNO, 'xls');
                    break;
//                case 'exportar-pdf':
//                    $config = Simec_Listagem_Config::carregar(self::$namespaceExport, $_REQUEST['listagem']['id-lista']);
//                    $list = new self(self::RELATORIO_PDF, self::RETORNO_PADRAO, $config);
//                    // -- Forçando a não paginação mesmo sendo um PDF
//                    $config->getDatasource()->naoPaginar();
//                    $list->render();
//                    break;
                case 'ordenar':
                    $config = Simec_Listagem_Config::carregar(self::$namespaceExport, $_REQUEST['listagem']['id-lista']);
                    $list = new self(null, null, $config);
                    $list->setCampoOrdenado($_REQUEST['listagem']['campo'], $_REQUEST['listagem']['sentido'])
                        ->render();
                    break;
                default:
                    ver($_REQUEST, d);
            }
            die();
        }
    }

    /**
     * Define um novo ID para a lista.
     *
     * Geralmente é utilizado quando é necessário colocar duas ou mais listas na mesma página.
     *
     * @param string $id Novo id da lista
     *
     * @return \Simec_Listagem
     */
    public function setId($id)
    {
        $this->config->setId($id);

        return $this;
    }

    /**
     * Retorna o id atual da lista.
     *
     * @return string
     */
    public function getId()
    {
        return $this->config->getId();
    }

    /**
     * Define um novo className para a lista.
     *
     * Geralmente é utilizado quando é necessário colocar duas ou mais listas na mesma página.
     *
     * @param string $className Novo className da lista
     *
     * @return \Simec_Listagem
     */
    public function setClassName($className)
    {
        $this->config->setClassName($className);

        return $this;
    }

    /**
     * Retorna o className atual da lista.
     *
     * @return string
     */
    public function getClassName()
    {
        return $this->config->getClassName();
    }

    /**
     * Função de criação de conteúdo para a coluna da listagem.
     *
     * **Importante**: Não utilize closures, pois elas são incompatíaveis com
     * o export automático da lista.
     *
     * @param string|function $callback
     */
    public function addColunaVirtual($callback)
    {
        $this->config->addColunaVirtual($callback);
    }

    /**
     * Define uma configuração de linha composta para o relatório.
     *
     * Uma linha composta, é uma linha com outra linha filha com conteúdo que detalha a linha principal.
     * Quando acionado, é acrescentado à toolbar um novo botão de expansão/retração desses detalhes.
     *
     * Esse método recebe uma array com os seguintes campos:
     * * campo: Nome do campo que tem os dados agrupados da segunda linha.
     * * template: Template HTML do conteúdo da linha de detalhamento.
     *
     * @param mixed[] $configLinha
     *
     * @return \Simec_Listagem
     * @example
     * <code>
     * $list->setConfigLinhaComposta([
     *        'campo' => 'entrega',
     *       'template' => <<<HTML
     * <table class="table table-bordered table-condensed tabela-listagem tabela-entrega">
     *     <thead>
     *         <tr>
     *             <th class="text-center th_entrega">Descrição</th>
     *             <th class="text-center th_entrega">Dt. Inclusão</th>
     *             <th class="text-center th_entrega">Tipo</th>
     *             <th class="text-center th_entrega">Relatório de Ocorrência</th>
     *             <th class="text-center th_entrega">Estado</th>
     *             <th class="text-center th_entrega">Data Alteração Estado</th>
     *         </tr>
     *     </thead>
     *     <tbody>
     *         {{repetir}}
     *             <tr>
     *                 <td>%%dmedsc%%</td>
     *                 <td>%%dmedtinclusao%%</td>
     *                 <td>%%dmetipo%%</td>
     *                 <td>%%dmerelocorrencia%%</td>
     *                 <td>%%esddsc_entrega%%</td>
     *                 <td>%%data_estado_entrega%%</td>
     *             </tr>
     *         {{fim-repetir}}
     *     </tbody>
     * </table>
     * HTML
     * ])->render();
     * </code>
     */
    public function setConfigLinhaComposta($configLinha)
    {
        $this->config->setConfigLinhaComposta($configLinha);
        $this->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::ABRE_FECHA);
        $this->setTipoRelatorio(self::RELATORIO_CORRIDO);

        return $this;
    }

    /**
     * Define um novo renderizador para o compomente de listagem de dados.
     *
     * @param Simec_Listagem_Renderer_Abstract $renderer
     *      Define um novo renderizador de dados da lista.
     *
     * @return \Simec_Listagem
     */
    public function setRenderer(Simec_Listagem_Renderer_Abstract $renderer)
    {
        $this->renderer = $renderer;

        return $this;
    }

    /**
     * Define uma nova quantidade de registros para compor uma página da listagem (número de registros por página).
     *
     * O padrão da lista é de 100 registros por página.
     *
     * @param int $tamanhoPagina Novo tamanho da página.
     *
     * @return Simec_Listagem
     */
    public function setTamanhoPagina($tamanhoPagina)
    {
        $this->config->setTamanhoPagina($tamanhoPagina);

        return $this;
    }

    /**
     * Transfere as chamadas de métodos não definidos para o renderizador.
     *
     * @param string $name      O Nome do método chamado.
     * @param string $arguments Lista de argumentos do método chamado.
     *
     * @return mixed|Simec_Listagem
     * @throws Exception Se o método não está implementado no renderizador, lança uma exceção.
     */
    public function __call($name, $arguments)
    {
        // -- Verifica se a função solicitada está disponível no renderer
        if (!is_callable([$this->renderer, $name]) || !method_exists($this->renderer, $name)) {
            $rendererClass = get_class($this->renderer);
            throw new Exception("O método '{$name}' não está implementado no renderizador '{$rendererClass}'.");
        }
        $retorno = call_user_func_array([$this->renderer, $name], $arguments);

        if (is_null($retorno)) {
            return $this;
        }
    }

    /**
     *
     * @return type
     *
     * @todo Não dá pra utilizar o do config?
     */
    public function getPaginaAtual()
    {
        return $this->paginaAtual;
    }

    /**
     * Troca o tipo de saída do relatório.
     * Por padrão, o relatório é impresso na tela, mas a saída pode ser mudada para retorno do HTML.
     *
     * @param int $tipoRetorno Um dos tipos válidos de saída de relatório.
     *
     * @throws Exception Lança exceção quando tipo informado é inválido.
     * @return \Simec_Listagem
     * @uses Simec_Listagem::RETORNO_PADRAO
     * @uses Simec_Listagem::RETORNO_BUFFERIZADO
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
     * Carrega na listagem o conjunto de dados para exibição.
     *
     * Geralmente o array irá conter apenas a lista de linhas do relatório,
     * no entanto, em casos especiais é possível adicionar chaves especiais
     * neste array de forma a mudar o seu comportamente:
     * **dados**: Ao utilizar um array do tipo configuração, as linhas de dados
     * devem vir entro desta chave. Sua existência é que muda o funcionamento
     * do datasource.
     * query: Armazena a query utilizada para a criação do array de dados, é
     * a única chave com impacto fora do datasource e serve para ser exibida na toolbar
     * da listagem.
     *
     * @param array  $dados   Array de dados/configuração do datasource do tipo array.
     * @param string $formato Define em qual formato o datasource sera utilizado
     *
     * @return \Simec_Listagem
     * @uses \Simec\Listagem\Simec_Listagem_Datasource_Array
     * @uses \Simec_Listagem::showQuery()
     */
    public function setDados($dados, $formato = null)
    {
        if (!is_array($dados)) {
            $dados = [];
        }

        if (array_key_exists('pagina', $dados)) {
            $this->paginaAtual = $dados['pagina'];
        }

        $datasource = new Simec_Listagem_Datasource_Array($this->config->paginarLista());
        $datasource->setSource($dados);

        if ($this->config->paginarLista()) {
            $datasource->setPaginaAtual($this->paginaAtual);
        } else {
            // -- @todo Desfazer a necessidade dessa chamada, fazendo o parametro de construção do array prevalecer
            $datasource->setPaginaAtual('all');
        }

        // -- Salvando para execução posterior automática, ex: xls
        $this->config->setDatasource($datasource, $formato);

        return $this;
    }

    /**
     * Carrega no objeto a query responsável por recuperar os dados que serão listados.
     * Esta função é uma alternativa a Simec_Listagem::setDados().
     *
     * @param string $query        String SQL para carregar os dados da listagem.
     * @param int    $queryTimeout Número de segundos que a query deve ficar armazenada em cache. O valor 0 faz com que seja armazenado para sempre.
     * @param string $formato      Define em qual formato o datasource sera utilizado
     *
     * @return \Simec_Listagem
     * @see Simec_Listagem::setDados()
     */
    public function setQuery($query, $queryTimeout = null, $formato = null)
    {
        $datasource = new Simec_Listagem_Datasource_Query($this->config->paginarLista());
        $datasource->setSource($query, ['timeout' => $queryTimeout]);

        $datasource->setPaginaAtual($this->paginaAtual);

        // -- Salvando para execução posterior automática, ex: xls
        $this->config->setDatasource($datasource, $formato);

        return $this;
    }

    /**
     * Define um título para o relatório.
     *
     * Exemplo de utilização:
     * $list = new Simec_Listagem();
     * $list->setTitulo('Relatório de movimentação');
     *
     * @param string $titulo Título a ser exibido acima do relatório.
     *
     * @return \Simec_Listagem
     */
    public function setTitulo($titulo)
    {
        $this->renderer->setTitulo($titulo);

        return $this;
    }

    /**
     * Lista de títulos das colunas do relatório.
     *
     * O título pode ter elementos em dois níveis, para isso, passe o nome da coluna
     * principal como chave do array e as colunas filhas como itens deste array.
     *
     * @param mixed $cabecalho
     *
     * @return \Simec_Listagem
     *
     * @example Cabeçalho simples
     * <code>
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Coluna 1', 'Coluna 2'));
     * </code>
     * @example Cabecalho de dois níveis
     * <code>
     * $list = new Simec_Listagem();
     * $list->setCabecalho(array('Grupo de colunas' => array('Coluna 1', 'Coluna 2'));
     * </code>
     */
    public function setCabecalho($cabecalho, $formato = null)
    {
        $this->config->setCabecalho($cabecalho, $formato);

        return $this;
    }

    /**
     * Configura o tipo de totalizador da listagem, adicionalmente, informa quais colunas serão totalizadas.
     *
     * @param int               $totalizador Tipo de totalizador da listagem (Simec_Listagem::TOTAL_SOMATORIO_COLUNA e Simec_Listagem::TOTAL_QTD_REGISTROS)
     * @param string|null|array $colunas     Lista de colunas que serão totalizadas.
     *
     * @return \Simec_Listagem
     *
     * @uses Simec_Listagem::TOTAL_SOMATORIO_COLUNA;
     * @uses Simec_Listagem::TOTAL_QTD_REGISTROS;
     */
    public function setTotalizador($totalizador, $colunas = null)
    {
        $this->config->setTotalizador($totalizador, $colunas);

        return $this;
    }

    /**
     * Define a lista de colunas que a listagem receberá do conunto de dados.
     *
     * Este método é utilizado em conjunto com a opção Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA, geralmente
     * utilizado quando a funcionalidade de adicionar novas linhas à listagem está ativa.
     *
     * @param string[] $listaColunas Lista com as colunas da listagem.
     *
     * @return \Simec_Listagem
     * @uses \Simec_Listagem_Config::setListaColunas()
     */
    public function setListaColunas(array $listaColunas)
    {
        $this->config->setListaColunas($listaColunas);

        return $this;
    }

    /**
     * Define a lista de campos que a listagem irá receber da consulta ou no array de dados.
     *
     * Este método é utilizado em conjunto com a opção Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA, geralmente
     * utilizado quando a funcionalidade de adicionar novas linhas à listagem está ativa.
     *
     * @param string[] $listaColunas Lista com as colunas da listagem.
     *
     * @return \Simec_Listagem
     * @deprecated Utilize \Simec_Listagem::setListaColunas();
     */
    public function setCampos(array $listaColunas)
    {
        $this->setListaColunas($listaColunas);

        return $this;
    }

    /**
     * Indica que um, ou mais, campo(s) da query não será(ão) exibido(s) na listagem.
     *
     * @param string|string[]|string,... $nomeColuna Nome do campo da query que não será exibida na listagem.
     *
     * @return \Simec_Listagem
     */
    public function esconderColunas($nomeColuna)
    {
        foreach (func_get_args() as $arg) {
            $this->config->ocultarColunas($arg);
        }

        return $this;
    }

    /**
     * Adiciona ações às linhas da listagem.
     *
     * Ao adicionar uma ação, você pode utilizar o formato simples ou, para maior controle,
     * o formato avançado.
     *
     * O formato simples é composto pelo nome da ação e o nome da callback js que será
     * invocada. O valor passado para todas ações simples é o da primeira coluna da listagem
     * e esta coluna deixa de ser exibida na listagem. Ex:<pre>
     * $listagem = new Simec_Listagem();
     * $listagem->addAcao('plus', 'detalharItem');
     * ...
     * $listagem->render();</pre>
     *
     * O formato avançado é composto pelo nome da ação e um array de configuração, onde,
     * os pares chave/valor definem as opção daquela ação. Ex:<pre>
     * $listagem = new Simec_Listagem();
     * $listagem->addAcao('plus', array('func' => 'detalharItem', 'extra-params' => array('idLinha', 'exercicio')));
     * ...
     * $listagem->render();</pre>
     *
     * Para uma lista completa de configurações da ação, veja a class Simec_Listagem_Acao.
     * Para uma lista completa das ações disponíveis, veja as classes filhas de Simec_Listagem_Acao.
     *
     * @param string         $acao   Identificador de uma ação
     * @param string|mixed[] $config Nome de função callback js, ou array de configuração da ação.
     *
     * @return \Simec_Listagem
     * @see  \Simec_Listagem_Acao
     * @todo Verificar se o tipo de renderer suportar ações
     */
    public function addAcao($acao, $config)
    {
        $this->config->addAcao($acao, $config);

        return $this;
    }

    /**
     * Define condições para que uma ação seja exibida em uma listagem. Ao definir uma condição, mais de uma ação pode
     * ser informada.
     *
     * A ação só será exibida se atender a todas as condições atribuídas. A condição é criada verificando valores do
     * conjunto de dados da listagem. Se mais de uma condição for definida para a ação, esta só será exibida se todas
     * as condições forem atendidas. Exemplo de utilização:<pre>
     *
     * $dados = array(array('valor' => 3.00), array('valor' => 0.00));
     * $listagem = new Simec_Listagem();
     * $listagem->setDados($dados);
     * $listagem->setCabecalho(array('Valor'));
     * $listagem->addAcao('edit', 'editarValor');
     * $listagem->setAcaoComoCondicional('edit', array(array('campo' => 'valor', 'valor' => 0.00, 'op' => 'diferente')));
     * $listagem->render();</pre>
     *
     * Desta forma, a ação de edição só será exibida se o valor do campo 'valor' for igual a 0.00.
     *
     * A condição aceita o nome de um campo retornado pelo SQL passado para a lista. Exemplo:
     *
     * $listagem = new Simec_Listagem();
     * $listagem->setDados($dados);
     * $listagem->setCabecalho(array('Valor'));
     * $listagem->addAcao('edit', 'editarValor');
     * $listagem->setAcaoComoCondicional('edit', array(array('campo' => 'valor', 'valor' => 'maximo', 'op' => 'igual')));
     * $listagem->render();</pre>
     *
     * Desta forma, a ação de edição só será exibida se o valor do campo 'valor' for igual ao valor do campo 'maximo'.
     *
     * @param string|string[] $acao      Nome da ação, ou ações, que serão exibidas de acordo com a condição definida.
     * @param array           $condicoes Array de configuração da(s) condição(ões) de exibição da ação.
     *
     * @return \Simec_Listagem
     */
    public function setAcaoComoCondicional($acao, array $condicoes)
    {
        $this->config->setAcaoComoCondicional($acao, $condicoes);

        return $this;
    }

    /**
     * Define uma regra, do tipo Simec_Regra_Agrupador, como condicional para a apresentação da ação.
     *
     * A ação só será exibida se atender todas as regras contidas no agrupador. Exemplo de utilização:<pre>
     *
     * $dados = array(array('valor' => 3.00), array('valor' => 0.00));
     * $listagem = new Simec_Listagem();
     * $listagem->setDados($dados);
     * $listagem->setCabecalho(array('Valor'));
     * $listagem->addAcao('edit', 'editarValor');
     * $listagem->setRegraAcaoCondicional('edit',
     *      (new Sistema_Regra_Agregador('VALIDA_VALOR')),
     *      ['valor']
     * );
     * $listagem->render();</pre>
     *
     * Desta forma, a ação de edição só será exibida caso as regras contidas no agregador 'VALIDA_VALOR', da classe Sistema_Regra_Agregador, sejam atendidas.
     *
     * @param $acao
     * @param Simec_Regra_Agregador $regra
     * @param $campo
     * @param array $opcoes
     * @return $this
     */
    public function setRegraAcaoCondicional($acao, Simec_Regra_Agregador $regra, $campo, $opcoes = []){
        $this->config->setRegraAcaoCondicional($acao, $regra, $campo, $opcoes);
        return $this;
    }

    /**
     * Agrupa uma ação entre linhas da listagem.
     *
     * É importante que a ordenação dos dados seja compatível com o agrupamento solicitado.
     *
     * @param string   $acao   O nome de uma ação adicionada à listagem.
     * @param string[] $campos Lista de colunas que serão consideradas para fazer o agrupamento.
     *
     * @return \Simec_Listagem
     * @todo Verificar se o tipo de renderer suportar ações
     */
    public function setAcaoComoAgrupada($acao, array $campos)
    {
        $this->config->setAcaoComoAgrupada($acao, $campos);

        return $this;
    }

    /**
     * Adiciona uma nova regra de formatação de linha.
     * A nova regra deve atender ao formato armazenado em self::$regrasDeLinha:
     *
     * @param array $regra
     *
     * @todo validar a estrutura da nova regra a ser adicionada
     */
    public function addRegraDeLinha(array $regra)
    {
        $this->config->addRegraDeLinha($regra);

        return $this;
    }

    /**
     * Adiciona uma função callback de processamento de conteúdo de campo.
     *
     * Uma ação comumente realizada com este método é a aplicação de máscara em um campo de CPF ou monetário.
     * Mais de um campo pode ser informado na mesma chamada, basta utilizar um array com a lista de campos, ao
     * invés do nome de um único campo.
     *
     * Também é possível utilizar uma função anônima para executar a formatação, basta substituir o
     * nome da função pela declaração da função anônima.
     *
     * @param string|array $nomeCampo    Nome(s) do(s) campo(s) que receberá(ão) o tratamento.
     * @param string       $nomeCallback Nome da função de processamento do campo. Ela deve retornar sempre uma string.
     *
     * @return \Simec_Listagem
     * @throws Exception Gerada quando o nome da callback ou a própria função é inválida.
     * @example
     * <code>
     * function mascaraReal($valor) { return "R$ {$valor}"; }
     * ...
     * $listagem = new Simec_Listagem();
     * $listagem->setQuery("SELECT '3.00' AS valor");
     * $listagem->addCallbackDeCampo('valor', 'mascaraReal');
     * $listagem->render();
     * </code>
     */
    public function addCallbackDeCampo($nomeCampo, $nomeCallback)
    {
        $this->config->addCallbackDeCampo($nomeCampo, $nomeCallback);

        return $this;
    }

    /**
     * Se o renderer suportar, ativa o pesquisator - busca rápida, na listagem.
     *
     * @return \Simec_Listagem
     */
    public function turnOnPesquisator()
    {
        if (method_exists($this->renderer, 'turnOnPesquisator')) {
            $this->renderer->turnOnPesquisator();
        }

        return $this;
    }

    /**
     * Adiciona ações à toolbar da lista. Veja a descrição das ações em Simec_Listagem_Renderer_Html_Toolbar.
     *
     * Disponível apenas para o renderizador HTML.
     *
     * @param int $tipo ID do botão que será adicionado.
     *
     * @return \Simec_Listagem
     * @uses \Simec_Listagem_Renderer_Html_Toolbar
     */
    public function addToolbarItem($tipo)
    {
        $this->renderer->addToolbarItem($tipo);

        return $this;
    }

    /**
     * Se o renderizador suportar, ativa o protótipo de linha - cria um atributo na
     * listagem com o conteúdo de uma linha, para criação de novas linhas com js.
     *
     * @return \Simec_Listagem
     */
    public function turnOnPrototipo()
    {
        if (!method_exists($this->renderer, 'turnOnPrototipo')) {
            $renderizador = get_class($this->renderer);
            throw new Exception("O renderizador atual ({$renderizador}) não suporta protótipo de linha.");
        }
        $this->renderer->turnOnPrototipo();

        return $this;
    }

    /**
     * Desliga o formulário da listagem - geralmente utilizado qdo é preciso inserir a listagem dentro de um form.
     *
     * @return \Simec_Listagem
     */
    public function turnOffForm()
    {
        $this->renderer->renderizarForm = false;

        return $this;
    }

    /**
     * Liga a ordenação da lista.
     * IMPORTANTE: Não se esqueça de seguir as instruções definidas em Simec_Listagem::iniciarMonitoramento();
     *
     * @param string|function $callback Função externa de ordenação da listagem.
     *
     * @return \Simec_Listagem
     */
    public function turnOnOrdenacao($callback = null)
    {
        $this->config->setFuncaoOrdenacao($callback);

        return $this;
    }

    /**
     * Cria o output da listagem.
     *
     * O formato gerado dependerá do renderer que foi definido para a listagem.
     *
     * @param int $tratamentoListaVazia Indica qual será o comportamente da lista caso nenhum registro seja retornado.
     *
     * @return bool|string
     * @throws Exception
     * @todo Refatorar este método.
     * @todo renderizar lista vazia deve ser feito pelo renderer e não pela listagem
     */
    public function render($tratamentoListaVazia = Simec_Listagem::SEM_REGISTROS_RETORNO, $formato = null)
    {
        if (is_null($this->config->getDatasource($formato))) {
            throw new Exception('A listagem não pode ser renderizada sem dados. Utilize Simec_Listagem::setDados(), Simec_Listagem::setQuery() ou Simec_Listagem::setDatasource() para carregar os dados da listagem.');
        }

        if ((Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA == $tratamentoListaVazia) && !$this->config->getListaColunas()) {
            throw new Exception('Para usar "Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA" como opção de '
                . 'retorno, é necessário chamar Simec_Listagem::setCampos() informando a lista de campos. '
                . 'Caso esteja utilizando Simec_View_Form::addInputLista(), inclua a lista de campos no '
                . 'parâmetro de opções utilizando a chave "campos".');
        }

        // -- Armazenando a saída em um buffer do relatório
        if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
            ob_start();
        }

        // -- Mensagens de debug da exportação de XLS
        if (self::$monitorarExport && !IS_PRODUCAO && $this->renderer instanceof Simec_Listagem_Renderer_Html) {
            foreach ($this->config->getMensagensCallback() as $mensagem) {
                echo <<<HTML
<div class="col-md-8 col-md-offset-2 listagem-info">
    <div class="alert alert-info" role="alert"><b>Simec_Listagem Info:</b> {$mensagem}</div>
</div>
<br style="clear:both" />
HTML;
            }
        }

        // -- Validações de lista vazia
        // -- @todo Migrar pra dentro do renderer
        if ($this->config->getDatasource($formato)->estaVazio()) {
            switch ($tratamentoListaVazia) {
                case Simec_Listagem::SEM_REGISTROS_MENSAGEM:
                    $idListaVazia = $this->config->getId();
                    echo $this->renderer->renderTitulo();
                    echo <<<HTML
<div style="margin-top:20px;" class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro"
    id="{$idListaVazia}">Nenhum registro encontrado</div>
<br style="clear:both" />
<br />
HTML;
                    if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
                        return ob_get_clean();
                    }

                    return;
                case Simec_Listagem::SEM_REGISTROS_RETORNO:
                    if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
                        ob_end_clean();
                    }

                    return false;
                case Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA:
                    // -- Neste caso, a lista precisa continuar sendo renderizada, mesmo vazia
                    break;
            }
        }

        // -- Verifica se a página atualmente solicitada é válida, senão, joga para a primeira página
        if ((Simec_Listagem::RELATORIO_PAGINADO == $this->config->getDatasource($formato)->paginarDados())
            && ($this->config->getDatasource($formato)->getTotalPaginas() < $this->getPaginaAtual())
            && ($this->getPaginaAtual() != 'all')) {
            $this->setPaginaAtual(1);
        }

        // -- Passando para o renderizador a fonte de dados
        $this->renderer->setDatasource($this->config->getDatasource($formato));

        // -- Tratar a paginação aqui, fornecendo para o renderer apenas os dados para renderização
        $this->renderer->render();

        if (Simec_Listagem::RELATORIO_PAGINADO == $this->config->getDatasource($formato)->paginarDados()) {
            // -- Inclui a seleção de páginas no final da listagem
            $this->renderPaginador();
        }

        // -- @todo passar isso pra dentro do renderer ou do próprio config
        if ((self::$monitorarExport) && ($this->renderer instanceof Simec_Listagem_Renderer_Html)) {
            $this->config->salvar(self::$namespaceExport, $this->config->getId());
        }

        // -- Armazenando a saída em um buffer do relatório
        if (self::RETORNO_BUFFERIZADO == $this->bufferizarRetorno) {
            $listagem = ob_get_contents();
            ob_end_clean();

            return $listagem;
        }
    }

    /**
     * Adiciona um input à listagem.
     *
     * Este campo não está associado particularmente a nenhuma linha da lista. Deve ser
     * utilizado quando é necessário algum campo de controle, ou um campo associado a
     * todos os elementos da linha (uma chave estrangeira geral, por exemplo).
     *
     * O campo deve conter os valores de id, name e type. E o valor deve ser setado
     * através de javascript.
     *
     * @param mixed[] $campos Configurações do input: id, name e type.
     *
     * @example
     * <code>
     * $list = new Simec_Listagem();
     * $list->addCampo([
     *     'id' => 'listagem_usucpf',
     *     'name' => 'perfil[usucpf]',
     *     'type' => 'hidden'
     * ]);
     * </code>
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

    public function setCampoOrdenado($campo, $sentido)
    {
        $this->config->setCampoOrdenado($campo, $sentido);

        return $this;
    }

    /**
     * Utilizar Simec_Listagem:setTotalizador() ou Simec_Listagem::totalizarColunas()
     * para definir quais colunas do relatório serão totalizadas.
     *
     * @deprecated
     *
     * @param string $nomeCampo
     *
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
     *
     * @param string $nomeCampo Nome do campo da query que não será exibida na listagem.
     *
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
     *
     * @param type $acoes
     *
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



    // -- Métodos depreciados ------------------------------------------------------------------------------------------

    /**
     * Utilize Simec_Listagem::setTipoRetorno();
     *
     * @deprecated
     */
    public function trocaTipoSaida($tipoRetorno)
    {
        return $this->setTipoRetorno($tipoRetorno);
    }

    /**
     * Utilize Simec_Listagem::turnOffForm();
     *
     * @return \Simec_Listagem
     * @deprecated
     */
    public function setFormOff()
    {
        return $this->turnOffForm();
    }

    /**
     * Define quais colunas serão totalizadas.
     *
     * Atualmente utilizada em conjunto com Simec_Listagem::setTotalizador(). Prefira a utilização por lá.
     *
     * @param string|string[] $campos Campo ou lista de campos para totalização.
     *
     * @return \Simec_Listagem
     * @deprecated
     */
    public function totalizarColunas($campos)
    {
        $this->config->totalizarColunas($campos);

        return $this;
    }

    /**
     * Carrega informações de páginação, filtros e ordenação (todos considerando o tipo do relatório).
     *
     * @return \Simec_Listagem
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
                $this->paginaAtual = (int) $novaPagina;
            }
        }

        // -- Ordenação da listagem
        $this->config->setCampoOrdenado($_POST['listagem']['campo'], $_POST['listagem']['sentido']);

        // -- Carregando filtros, ordenação e número de página para o relatório renderizado em HTML
        if ($this->renderer instanceof Simec_Listagem_Renderer_Html) {
            $this->renderer->setFiltros($_POST['filtro']);
            $this->renderer->setPaginaAtual($novaPagina);
        }

        return $this;
    }

    /**
     * @todo Mover para o renderer??? Abstract de paginador???
     * @return type
     */
    protected function renderPaginador()
    {
        // -- Se não foi preciso paginar, não exibe o paginador
        if (!$this->config->getDatasource()->paginar()) {
            return;
        }

        echo <<<HTML
        <div class="row container-listing">
            <div class="col-lg-12" style="padding-bottom:20px;text-align:center">
HTML;

        if ('all' == $this->config->getDatasource()->getPaginaAtual()) {
            echo <<<HTML
                <ul class="pagination">
                    <li class="pgd-item" data-pagina="1">
                        <a href="javascript:void(0)">Paginar</a>
                    </li>
                </ul>
HTML;
        } else {

            $paginaAtual = (int) $this->config->getDatasource()->getPaginaAtual();
            $paginaAnterior = ($paginaAtual - 1);
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
            if ((int) $paginaAnterior > 3) {
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
                if ($paginaAtual == $numPagina) {
                    $paginaAtualCSS = ' active';
                }
                echo <<<HTML
                        <li class="pgd-item{$paginaAtualCSS}" data-pagina="{$numPagina}">
                            <a href="javascript:void(0)">{$numPagina} </a>
                        </li>
HTML;
            }
            $ultimaPagina = $this->config->getDatasource()->getTotalPaginas();
            if (!in_array($ultimaPagina, $listaPaginas)) {
                echo <<<HTML
                        <li class="pgd-item" data-pagina="{$ultimaPagina}">
                            <a href="javascript:void(0)">{$ultimaPagina} &raquo;</a>
                        </li>
HTML;
            }
            $desabilitarProxima = '';
            if ($paginaAtual == $ultimaPagina) {
                $desabilitarProxima = ' disabled';
            }
            $proximaPagina = $paginaAtual + 1;
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
        $listaPaginas = [];
        $paginaAtual = $this->config->getDatasource()->getPaginaAtual();

        // -- A lista de páginas que devem ser exibidas
        for ($qtdPaginasAnteriores; $qtdPaginasAnteriores <= 0; $qtdPaginasAnteriores++) {
            // -- Se a página for menor que zero, não exibe a página e cria uma nova página posterior
            if ($paginaAtual + $qtdPaginasAnteriores <= 0) {
                $qtdPaginasPosteriores++;
                continue;
            }
            $listaPaginas[] = $paginaAtual + $qtdPaginasAnteriores;
        }

        for ($i = 1; $i < $qtdPaginasPosteriores + ($this->numPaginasSeletor % 2); $i++) {
            if ($paginaAtual + $i > $this->config->getDatasource()->getTotalPaginas()) {
                break;
            }
            $listaPaginas[] = $paginaAtual + $i;
        }

        return $listaPaginas;
    }

    protected function setTipoRelatorio($tipoRelatorio)
    {
        $tiposValidosRelatorio = [
            self::RELATORIO_PAGINADO,
            self::RELATORIO_CORRIDO,
            self::RELATORIO_XLS
        ];

        if (!in_array($tipoRelatorio, $tiposValidosRelatorio)) {
            throw new Exception(
                'Tipo de relatório inválido. Tipos válidos: Simec_Listagem:RELATORIO_PAGINADO ou Simec_Listagem::RELATORIO_CORRIDO.'
            );
        }

        $this->tipoRelatorio = $tipoRelatorio;

        return $this;
    }
}
