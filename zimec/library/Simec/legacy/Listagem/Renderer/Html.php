<?php
/**
 * Renderizador de listagens HTML.
 *
 * @version $Id: Html.php 96707 2015-04-28 20:14:40Z lindalbertofilho $
 */

/**
 * Renderizador base.
 * @see Simec_Listagem_Renderer_Abstract
 */
require_once(dirname(__FILE__) . '/Abstract.php');

class Simec_Listagem_Renderer_Html extends Simec_Listagem_Renderer_Abstract
{
    /**
     * Conjunto de regras de formatação de uma linha da listagem.
     * @var array
     */
    protected $regrasDeLinha = array();

    /**
     * Armazena as ações que devem ser adicionadas à listagem.
     * $this->acoes = array(
     *     'plus' => 'detalharItem', // -- ações simples
     *     'send' => array( // -- ações avançadas
     *         'func' => 'detalharItem',
     *         'extra-params' => array('iditem', 'exercicio')
     *      )
     * );
     *
     * @var array
     * @see Simec_Listagem::addAcao()
     */
    protected $acoes = false;

    /**
     * Lista de ações e condições que devem ser atendidas para exibição da ação.
     * @var array
     * @see Simec_Listagem::setAcaoComoCondicional()
     */
    protected $acoesCondicionais = array();

    /**
     * Armazena uma lista de campos que são inclusos no formulário da listagem.
     * @var array
     */
    protected $formCampos = array();

    /**
     * Indica que deve ser exibido um botão importar no formulário.
     * @var type
     */
    protected $formImportar = false;

    /**
     * Action que será utilizada pelo formulário.
     * @var nome da action.
     */
    protected $formAction = '';

    public $renderizarForm = true;

    /**
     * Indica se o pesquisator - busca rápida deve estar ativo.
     *
     * @var bool|array
     */
    public $renderPesquisator = false;

    /**
     * Exibe ordenação
     */
    protected $ordenacao = false;

    /**
     * Exibe filtros de coluna
     */
    protected $filtros = false;

    /**
     * ID da tabela de listagem.
     * @var string
     */
    protected $id = 'tb_render';

    /**
     * Indica para o paginador qual é a página atualmente carregada.
     * @var int|string
     */
    protected $paginaAtual;

    /**
     * Armazena o ID do form com os filtros do relatório.
     * @var type
     */
    protected $formFiltros = '';

    /**
     * Armazena as configurações de criação de ID de linha da listagem.
     * <pre>
     * $configIdLinha = 'prefixo';
     * </pre>
     * @var string
     */
    protected $configIdLinha;

    /**
     * Armazena o array com as larguras das colunas da tabela.
     * @var array
     */
    protected $larguraColuna = array();

    /**
     * Para mais de uma coluna, passar um array no primeiro parâmetro, ignorando o segundo parâmetro.
     * Ex.: setLarguraColuna(array('campo' => '50%')) || setLarguraColuna('campo','50%').
     * @param string|array $campo (Coluna para sql ou chave para dados)
     * @param string $largura (Pode ser definida como desejar: X%, Ypx ...)
     */
    public function setLarguraColuna($campo, $largura = null){
        if(is_array($campo)){
            $this->larguraColuna = $campo;
            return;
        }
        $this->larguraColuna[$campo] = $largura;
    }

    /**
     * Liga e desliga a ordenação de colunas.
     * @param bool $ordenacao
     */
    public function setOrdenacao($ordenacao)
    {
        $this->ordenacao = $ordenacao;
    }

    /**
     * Retorna se a ordenação de colunas está ou não ligada.
     * @return bool
     */
    public function getOrdenacao()
    {
        return $this->ordenacao;
    }

    /**
     * Define as configurações de filtro.
     * @param bool|array $filtro
     */
    public function setFiltros($filtros)
    {
        if ((true === $filtros) && is_array($this->filtros)) {
            return;
        }

        $this->filtros = $filtros;
    }

    /**
     * Retorna as configurações de filtro.
     * @return mixed
     */
    public function getFiltros()
    {
        return $this->filtros;
    }

    /**
     * Define o ID da listagem.
     *
     * @param string $id
     * @return \Simec_Listagem_Renderer_Html
     * @throws Exception Não permite que seja setado um ID vazio.
     */
    public function setId($id)
    {
        if (empty($id)) {
            throw new Exception('O ID da listagem não pode ser definido como vazio.');
        }

        $this->id = $id;
    }

    /**
     * Retorna o ID da listagem.
     * @return string
     */
    public function getId()
    {
        return $this->id;
    }

    /**
     * Define a ação que será criada para o formulário.
     * @param string $formAction Ação do formulário
     */
    public function setFormAction($formAction)
    {
        $this->formAction = $formAction;
    }

    public function setPaginaAtual($pagina)
    {
        $this->paginaAtual = $pagina;
    }

    /**
     * Armazena o nome do form que tem os filtros da listagem.
     * @param string $formid
     */
    public function setFormFiltros($formid)
    {
        $this->formFiltros = $formid;
    }

    /**
     * Ativa a criação de ID de linha e também configura o prefixo, se necessário.
     * @param string $prefixo Prefixo da criação de ID da TR.
     */
    public function setIdLinha($prefixo = '')
    {
        $this->configIdLinha = $prefixo;
    }

    /**
     * Imprime o código HTML da listagem.
     */
    public function render()
    {
        // -- Incluíndo o arquivo de javascript de funções do relatório
        echo <<<HTML
<script type="text/javascript" lang="JavaScript">
jQuery(document).ready(function(){
    if (!(typeof Function == typeof window['listagem_js_carregado'])) {
        jQuery.getScript('/library/simec/js/listagem.js');
    }
});
</script>
HTML;
        // -- Título do relatório
        $this->renderTitulo();
        if ($this->renderizarForm) {
            echo <<<HTML
            <form method="post" class="form-listagem" action="{$this->formAction}" data-form-filtros="{$this->formFiltros}">
HTML;
        }
        // -- Inputs de controle da listagem
        // -- @todo Necessário apenas quando houver paginação
        echo <<<HTML
                <input type="hidden" name="listagem[p]" id="listagem-p" value="{$this->paginaAtual}" />
                <input type="hidden" name="listagem[ordenacao]" id="input_ordenacao" class="input_ordenacao" />
HTML;
        if ($this->formCampos) {
            foreach ($this->formCampos as $configCampo) {
                echo <<<HTML
<input type="{$configCampo['type']}" name="{$configCampo['name']}" id="{$configCampo['id']}" />
HTML;
            }
        }
        $qtdAcoes = count($this->acoes);

        // -- Renderizando o pesquisator
        if ($this->renderPesquisator) {
            echo $this->renderPesquisator();
        }

        echo <<<HTML
<table class="table table-striped table-bordered table-hover tabela-listagem" id="{$this->id}" data-qtd-acoes="{$qtdAcoes}">
HTML;
        echo $this->renderCabecalho();

        $this->renderDados();
        echo $this->renderRodape();
        echo '</table>';
        if ($this->formImportar) {
            echo <<<HTML
<button type="submit" class="btn btn-primary" id="import-data">Importar</button>
HTML;
        }
        if ($this->renderizarForm) {
            echo '</form>';
        }
    }

    /**
     * Verifica se é necessario incluir um título na listagem.
     * @see Simec_Listagem::render()
     */
    protected function renderTitulo()
    {
        if ($this->titulo) {
            echo <<<HTML
            <div class="page-header">
                <h4>{$this->titulo}</h4>
            </div>
HTML;
        }
    }

    protected function renderCabecalho()
    {
        // -- Se não houver um cabeçalho definido, pula a criação do cabeçaho
        if (is_null($this->cabecalho)) {
            return '';
        }

        if ('auto' == $this->cabecalho) {
            $todosCampos = array_keys(current($this->dados));
            $this->cabecalho = array_diff($todosCampos, $this->colunasOcultas);
        }

        // -- Verificando a quantidade de níveis do título
        $cabecalhoComDoisNiveis = false;
        foreach ($this->cabecalho as $titColuna) {
            if (is_array($titColuna)) {
                $cabecalhoComDoisNiveis = true;
                break;
            }
        }

        $htmlCabecalho = '';

        // -- Faz o processamento da coluna de ações do sistema
        $numAcoes = is_array($this->acoes)?count($this->acoes):0;
        if ($numAcoes) {
            $rowSpan = $cabecalhoComDoisNiveis?' rowspan="2"':'';
            $htmlCabecalho .= <<<HTML
<th colspan="{$numAcoes}"{$rowSpan}>&nbsp;</th>
HTML;
        }

        // -- Retornando o nome dos campos para associar à ordenação das colunas
        $campos = $this->camposDoCabecalho();
        // -- Indexador de campos da query
        $numCampo = 0;

        // -- Processando o primeiro nível do cabecalho
        foreach ($this->cabecalho as $key => $titColuna) {
            if (is_array($titColuna)) {
                $qtdColunas = count($titColuna);

                $htmlCabecalho .= <<<HTML
<th colspan="{$qtdColunas}">{$key}</th>
HTML;
                $numCampo += $qtdColunas;
            } else {
                $htmlCabecalho .= $this->renderCabecalhoFolha($titColuna, $campos[$numCampo], $cabecalhoComDoisNiveis, 1);
                $numCampo++;
            }
        }

        $cabecalho = "<tr>{$htmlCabecalho}</tr>";

        // -- Faz o processamento do segundo nível do cabecalho
        $htmlCabecalho = '';
        // -- Indexador de campos da query
        $numCampo = 0;
        foreach ($this->cabecalho as $agrupColunas => $titColuna) {
            if (is_array($titColuna)) {
                foreach ($titColuna as $titulo) {
                    $htmlCabecalho .= $this->renderCabecalhoFolha($titulo, $campos[$numCampo], $cabecalhoComDoisNiveis, 2);
                    $numCampo++;
                }
                continue;
            }
            $numCampo++;
        }

        $cabecalho .= "<tr>{$htmlCabecalho}</tr>"
                   . ($this->getFiltros()?$this->renderFiltro($campos, $numAcoes):'');

        $cabecalho = "<thead>{$cabecalho}</thead>";

        return $cabecalho;
    }

    /**
     * Renderiza uma coluna de título que representa um campo da query - pode ser filhos de agrupadores de coluna.<br />
     * No caso de uma coluna que não é agrupada, inclui os rowspans necessários para desenhar corretamente a coluna.
     *
     * @param string $titulo Título da coluna.
     * @param string $nomeCampo Nome da coluna na query.
     * @param bool $cabecalhoComDoisNiveis Indica se o cabeçalho tem dois níveis.
     * @param int $nivelAtual Indica o nível atual do processamento.
     * @return string
     */
    function renderCabecalhoFolha($titulo, $nomeCampo, $cabecalhoComDoisNiveis, $nivelAtual)
    {
        $rowspan = '';
        $width = '';
        if ($cabecalhoComDoisNiveis && 1 == $nivelAtual) {
            $rowspan = ' rowspan="2"';
        }
        if (array_key_exists($nomeCampo, $this->larguraColuna)){
            $width = " style=\"width: {$this->larguraColuna[$nomeCampo]};\"";
        }
        $html = <<<HTML
<th{$rowspan}{$width}>
    {$titulo}
HTML;
        if ($this->getOrdenacao()) {
            $html .= <<<HTML
    <br />
    <span class="glyphicon glyphicon-arrow-down campo_ordenacao" campo-ordenacao="{$nomeCampo}"></span>
    <span class="glyphicon glyphicon-arrow-up campo_ordenacao" campo-ordenacao="{$nomeCampo} DESC"></span>
HTML;
        }
        $html .= <<<HTML
</th>
HTML;
        return $html;
    }

    protected function renderFiltro($campos, $numAcoes)
    {
        $html = '<tr>';

        // -- Tratando as colunas de ação
        if ($numAcoes > 0) {
            $html .= <<<HTML
<th colspan="{$numAcoes}">&nbsp;</th>
HTML;
        }

        // -- Processando cada um dos campos presentes na query
        foreach ($campos as $campo) {
            $html .= <<<HTML
<th class="campo_filtro"><input name="filtro[{$campo}]" class="form-control" value="{$this->filtros[$campo]}" /></th>
HTML;
        }

        $html .= '</tr>';
        return $html;
    }

    /**
     * Retorna uma com os campos da query que são exibidos em cada coluna.
     * @return array
     */
    protected function camposDoCabecalho()
    {
        // -- Lista completa de campos retornados pela query
        $campos = array_keys(current($this->dados));

        // -- Se há alguma ação, remove a primeira coluna, pois ela foi usada como id para as ações
        if (count($this->acoes) > 0) {
            array_shift($campos);
        }

        // -- Elimina da lista de colunas, aquelas que estão ocultas
        $campos = array_diff($campos, $this->colunasOcultas);
        return array_values($campos);
    }

    /**
     * Faz a contagem de colunas da listagem, incluíndo colunas de ações (quando presentes).
     * @todo Precisa disso? Precisa ser assim??? oO
     * @return integer
     */
    protected function quantidadeDeColunasExibidas()
    {
        if(count($this->colunasOcultas) != 0){
            $qtdColunasExibidas = count(
                array_diff_key( // -- Criar um array temporário com os campos dados que não estão inclusos na listagem de colunas ocultas
                    $this->dados[0],
                    array_combine( // -- Cria um array temporário baseado nas colunas ocultas
                        $this->colunasOcultas,
                        array_fill(0, count($this->colunasOcultas), null)
                    )
                )
            );
        }
        // -- Ajuste da quantidade de colunas da query mediante contagem de ações
        if (count($this->acoes) > 1) {
            // -- -1 pq a coluna de ID já é contada em $qtdColunasExibidas
            $qtdColunasExibidas += count($this->acoes) - 1;
        }

        return $qtdColunasExibidas;
    }

    protected function renderDados()
    {
        // -- @todo Verificar se o tipo de listagem é de FORM, para incluir os inputs
        if (empty($this->dados)) {
            return;
        }
        echo <<<BODY
    <tbody>
BODY;
        foreach ($this->dados as $linha => $dadosLinha) {
            $classe = $this->getClasseLinha($dadosLinha);
            $id = !is_null($this->configIdLinha)?' id="' . $this->configIdLinha . current($dadosLinha) . '"':'';

            echo <<<TR
        <tr{$classe}{$id}>
TR;
            echo $this->parseAcao($dadosLinha, $linha);
            // -- @todo: Verificar o tipo da listagem do formulario
            if ($this->acoes) {
                $idLinha = array_shift($dadosLinha);
            } else {
                reset($dadosLinha);
                $idLinha = current($dadosLinha);
            }
            foreach ($dadosLinha as $key => $dado) {
                if (!in_array($key, $this->colunasOcultas)) {
                    // -- Verificação de totalizador de coluna
                    if (in_array($key, array_keys($this->colunasTotalizadas))) {
                        $this->somarColuna($key, $dado);
                    }
                    // -- Chamando função de callback registrada para o campo da listagem
                    if (array_key_exists($key, $this->callbacksDeCampo)) {
                        // QUANDO COLOCAR MAIS VARIAVEIS, ADICIONAR NO ARRAY
                        $dado = $this->callbacksDeCampo[$key]($dado, $dadosLinha, $idLinha, array('campo'=>$key) );
                    }
                    echo <<<TD
            <td>{$dado}</td>
TD;
                }
            }
            echo <<<TR
        </tr>
TR;
        }
        echo <<<BODY
    </tbody>
BODY;
    }

    /**
     * @param type $dados
     * @param type $linha
     * @return string
     */
    protected function parseAcao($dados, $linha)
    {
        $acoesHTML = '';
        if (!$this->acoes) {
            return $acoesHTML;
        }

        foreach ($this->acoes as $acao => $configAcao) {
            if (!is_array($configAcao)) {
                $configAcao = array('func' => $configAcao);
            }
            $configAcao['condicao'] = $this->acoesCondicionais[$acao];
            $objAcao = Simec_Listagem_FactoryAcao::getAcao($acao, $configAcao);
            $acoesHTML .= (string)$objAcao->setDados($dados);
        }

        return $acoesHTML;
    }

    protected function getClasseLinha($dados)
    {
        foreach ($this->regrasDeLinha as $regra) {
            $method = strtolower($regra['op']);
            if(is_array($regra['valor'])){
                foreach($regra['valor'] as $valor){
                    if (Simec_Operacoes::$method($dados[$regra['campo']], $valor)) {
                        return <<<HTML
                            class="{$regra['classe']}"
HTML;
                    }
                }
            }
            if (Simec_Operacoes::$method($dados[$regra['campo']], $regra['valor'])) {
                return <<<HTML
                    class="{$regra['classe']}"
HTML;
            }
        }
    }

    /**
     * Cria o rodapé da listagem.
     * @todo Implementar o totalizador de coluna.
     */
    protected function renderRodape()
    {
        if (Simec_Listagem::TOTAL_QTD_REGISTROS == $this->totalizador) {
            $spanDeColunas = (count($this->dados[0]) -1) + count($this->acoes);
            $numRegistros = count($this->dados);
            echo <<<HTML
                    <tfoot>
                        <tr>
                            <td style="text-align:right" colspan="{$spanDeColunas}"><strong>Total de registros:&nbsp; $numRegistros</strong></td>
                        </tr>
                    </tfoot>
HTML;
        } elseif (Simec_Listagem::TOTAL_SOMATORIO_COLUNA == $this->totalizador) {
            echo <<<HTML
                    <tfoot>
                        <tr>
HTML;
                            // -- Correção para quando tem mais de uma ação na listagem.
                            $dadosLinha = $this->dados[0];
                            if (is_array($this->acoes)) {
                                $qtdAcoes = count($this->acoes);
                                for ($i = 0; $i < $qtdAcoes; $i++) {
                                    array_unshift($dadosLinha, "--a{$i}");
                                }
                            }

                            foreach ($dadosLinha as $key => $dado) {
                                if (in_array($key, $this->colunasOcultas)) {
                                    continue;
                                }
                                if (0 === $key) { // -- oO ????
                                    continue;
                                }

                                if (key_exists($key, $this->colunasTotalizadas)) {
                                    if (key_exists($key, $this->callbacksDeCampo)) {
                                        $valor = $this->callbacksDeCampo[$key]($this->colunasTotalizadas[$key]);
                                    } else {
                                        $valor = $this->colunasTotalizadas[$key];
                                    }
                                    echo <<<HTML
                                            <td style="font-weight:bold">{$valor}</td>
HTML;
                                } else {
                                    echo <<<HTML
                                            <td>&nbsp;</td>
HTML;
                                }
                            }
                            echo <<<HTML
                        </tr>
                    </tfoot>
HTML;
        }
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
     * $listagem->addAcao('plus', 'detalharItem');
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
     * $listagem->addAcao('plus', array('func' => 'detalharItem','extra-params' => array('idLinha', 'exercicio')));
     * ...
     * $listagem->render();
     *
     * @param array $acoes Definições das ações que deverão ser encorporadas na listagem.
     * @see Simec_Listagem::acoesDisponiveis
     */
    public function addAcao($acao, $config)
    {
        $this->acoes[$acao] = $config;
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
     */
    public function setAcaoComoCondicional($acao, array $condicoes)
    {
        if (is_array($acao)) {
            foreach ($acao as $acao_) {
                $this->acoesCondicionais[$acao_] = $condicoes;
            }
        } else {
            $this->acoesCondicionais[$acao] = $condicoes;
        }
    }

    /**
     * Indica que o pesquisator - busca rápida - deve ser renderizado junto à listagem.
     */
    public function turnOnPesquisator()
    {
        $this->renderPesquisator = true;
    }

    /**
     * Renderiza o pesquisator - busca rápida.
     * @return string
     */
    protected function renderPesquisator()
    {
        return <<<HTML
<div class="form-group row form-horizontal pesquisator">
    <label class="control-label col-md-1" for="textFind" style="padding-top:2px!important">Pesquisa rápida: </label>
    <div class="col-md-11 input-group">
            <div class="input-group-addon" data-toggle="popover" style="cursor:pointer">
                <span class="glyphicon glyphicon-info-sign" style="color:#428bca"></span>
            </div>
            <input class="normal form-control busca-listagem" type="text" id="textFind"
                   placeholder="Digite o texto para busca" />
    </div>
</div>
HTML;
    }

    /**
     * Adiciona um novo campo no formulário da listagem.
     * @param array $campos Configuração do campo com: id, name e type.
     */
    public function addCampo(array $campos)
    {
        $this->formCampos[] = $campos;
    }

    public function mostrarImportar($mostrar = true)
    {
        $this->formImportar = $mostrar;
        return $this;
    }
}
