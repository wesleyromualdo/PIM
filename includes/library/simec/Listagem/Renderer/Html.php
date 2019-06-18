<?php
/**
 * Renderizador de listagens HTML.
 *
 * @version $Id: Html.php 141842 2018-07-18 13:08:08Z victormachado $
 * @filesource
 */

/**
 * Renderizador da barra de ferramentas da listagem.
 * @see Simec_Listagem_Html_Toolbar
 */
require_once(dirname(__FILE__) . '/Html/Toolbar.php');

/**
 * Renderizador base.
 *
 * @see Simec_Listagem_Renderer_Abstract
 */
require_once(dirname(__FILE__) . '/Abstract.php');
require_once dirname(__FILE__) . '/../Datasource.php';

/**
 * Classe de renderização de relatórios no formato HTML.
 *
 * Os métodos públicos desta classe são acessados diretamente por chamadas na classe Simec_Listagem.
 *
 * @package Simec\View\Listagem\Renderer
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
class Simec_Listagem_Renderer_Html extends Simec_Listagem_Renderer_Abstract
{
    /**
     * @var Simec_Listagem_Renderer_Html_Toolbar Toolbar da listagem.
     */
    protected $toolbar;

    /**
     * @var array Guarda os valores do último agrupamento realiado para uma ação durante a renderização das mesmas.
     * @see Simec_Listagem_Renderer_Html::renderAcoes()
     */
    protected $acoesAgrupadasUltimoValor = array();

    /**
     * @var array Armazena uma lista de campos que são inclusos no formulário da listagem.
     */
    protected $formCampos = array();

    /**
     * @var bool Indica que deve ser exibido um botão importar no formulário.
     */
    protected $formImportar = false;

    /**
     * @var string Action que será utilizada pelo formulário.
     */
    protected $formAction = '';

    public $renderizarForm = true;

    /**
     * @var bool Indica se deve ser renderizado um prototipo de linha da listagem.
     */
    protected $renderPrototipo = false;

    /**
     * @var bool Exibe ordenação.
     */
    protected $ordenacao = false;

    /**
     * @var bool Exibe filtros de coluna;
     */
    protected $filtros = false;

    /**
     * @var string Armazena o ID do form com os filtros do relatório.
     */
    protected $formFiltros = '';

    /**
     * @var string Armazena as configurações de criação de ID de linha da listagem.
     */
    protected $configIdLinha;

    /**
     * @var array Armazena o array com as larguras das colunas da tabela.
     */
    protected $larguraColuna = array();

    /**
     * @var bool Indica se o código JS da listagem já foi incluído.
     */
    static protected $jsIncluido = false;

    public function __construct(Simec_Listagem_Config $config, $monitorarExport = true)
    {
        parent::__construct($config);

        // -- Passa o config para a toolbar
        $this->getToolbar()->setConfig($config);

        // -- Se a exportação estiver ativada, adiciona o botão de exportar XLS
        if($monitorarExport){
            $this->getToolbar()->add(Simec_Listagem_Renderer_Html_Toolbar::EXPORTAR_XLS);
            $this->getToolbar()->add(Simec_Listagem_Renderer_Html_Toolbar::EXPORTAR_PDF);
        }
    }

    public function setDados($dados)
    {
        if (is_null($dados)) {
            $dados = array();
        }

        return parent::setDados($dados);
    }

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
     * @param bool|array $filtros
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
     * Define a ação que será criada para o formulário.
     * @param string $formAction Ação do formulário
     */
    public function setFormAction($formAction)
    {
        $this->formAction = $formAction;
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
     *
     * @uses Simec_Listagem_Renderer_Html::$jsIncluido
     */
    public function render()
    {
        // -- Incluíndo o arquivo de javascript de funções do relatório
        if (!self::$jsIncluido) {
            self::$jsIncluido = true;

            echo <<<HTML
<script type="text/javascript" lang="JavaScript">
jQuery(document).ready(function(){
    if (!(typeof Function == typeof window['listagem_js_carregado'])) {
        jQuery.getScript('/library/simec/js/listagem.js');
    }
});
</script>
HTML;
        }
        // -- Título do relatório
        $this->renderTitulo();
        if ($this->renderizarForm) {
            echo <<<HTML
            <form method="post" class="form-listagem" action="{$this->formAction}" data-form-filtros="{$this->formFiltros}">
HTML;
        }
        // -- Inputs de controle da listagem
        // -- @todo Necessário apenas quando houver paginação
        $infoOrdenacao = $this->config->getCampoOrdenado();
        echo <<<HTML
                <input type="hidden" name="listagem[p]" class="listagem-p" value="{$this->paginaAtual}" />
                <input type="hidden" name="listagem[campo]" id="listagem-campo-ordenacao" value="{$infoOrdenacao[0]}" />
                <input type="hidden" name="listagem[sentido]" id="listagem-campo-sentido" value="{$infoOrdenacao[1]}" />
HTML;
        if ($this->formCampos) {
            foreach ($this->formCampos as $configCampo) {
                if (array_key_exists('value', $configCampo) && !empty($configCampo['value'])) {
                    $configValue = "value = \"{$configCampo['value']}\"";
                } else {
                    $configValue = null;
                }
                echo <<<HTML
<input type="{$configCampo['type']}" name="{$configCampo['name']}" id="{$configCampo['id']}" {$configValue} />
HTML;
            }
        }
        $qtdAcoes = $this->config->getNumeroAcoes();

        // -- Barra de ferramentas da listagem
        echo $this->renderToolbar();

        // -- Prototipo de linha
        $prototipo = $this->renderPrototipo();
        $idTabela = $this->getConfig()->getId();

        echo <<<HTML
<div class="table-responsive">
    <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem  "
           id="{$idTabela}" data-qtd-acoes="{$qtdAcoes}" {$prototipo}>
HTML;
        echo $this->renderCabecalho();

        $this->renderDados();
        echo $this->renderRodape();
        echo <<<HTML
    </table>
</div>
HTML;

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
    public function renderTitulo()
    {
        if ($this->titulo) {
            echo <<<HTML
            <div class="page-header" style="margin-bottom:2px;padding-bottom:1px">
                <h4>{$this->titulo}</h4>
            </div>
HTML;
        }
    }

    protected function renderCabecalho()
    {
        // -- Se não houver um cabeçalho definido, pula a criação do cabeçaho
        if (is_null($this->getCabecalho())) {
            return '';
        }

        if ('auto' == $this->getCabecalho()) {
            $todosCampos = array_keys(current($this->dados));
            $this->config->setCabecalho(
                array_diff($todosCampos, $this->config->getColunasOcultas())
            );
        }

        // -- Verificando a quantidade de níveis do título
        $cabecalhoComDoisNiveis = false;
        foreach ($this->getCabecalho() as $titColuna) {
            if (is_array($titColuna)) {
                $cabecalhoComDoisNiveis = true;
                break;
            }
        }

        $htmlCabecalho = '';

        // -- Faz o processamento da coluna de ações do sistema
        $numAcoes = $this->config->getNumeroAcoes();
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
        foreach ($this->getCabecalho() as $key => $titColuna) {
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
        foreach ($this->getCabecalho() as $agrupColunas => $titColuna) {
            if (is_array($titColuna)) {
                foreach ($titColuna as $titulo) {
                    $htmlCabecalho .= $this->renderCabecalhoFolha($titulo, $campos[$numCampo], $cabecalhoComDoisNiveis, 2);
                    $numCampo++;
                }
                continue;
            }
            $numCampo++;
        }

        $cabecalho .= (!empty($htmlCabecalho)?"<tr>{$htmlCabecalho}</tr>":'')
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
        $rowspan = $width = $ordenacao = $icones = $wrap = $campo = '';
        if ($cabecalhoComDoisNiveis && 1 == $nivelAtual) {
            $rowspan = ' rowspan="2"';
        }
        if (array_key_exists($nomeCampo, $this->larguraColuna)){
            $width = " style=\"width: {$this->larguraColuna[$nomeCampo]};\"";
        }

        if ($ordenacao = $this->renderOrdenacao($nomeCampo)) {
            $campo = ' data-campo-ordenacao="' . $nomeCampo . '"';

            $iconeOrd = <<<HTML
<div class="ordenadores"><span class="icone icone_ordenado glyphicon glyphicon-chevron-up"></span></div>
HTML;
            $iconeRev = <<<HTML
<div class="ordenadores"><span class="icone icone_reverso glyphicon glyphicon-chevron-down"></span></div>
HTML;
        }

        $html = <<<HTML
<th{$rowspan}{$width} class="{$ordenacao}" {$campo}>{$iconeOrd}{$iconeRev}{$titulo}</th>
HTML;
        return $html;
    }

    protected function renderOrdenacao($nomeCampo)
    {
        if (!$this->config->ordenar()) {
            return '';
        }

        $clsOrdenacao = array('ordenavel');
        $infoOrdenacao = $this->config->getCampoOrdenado();
        if ($infoOrdenacao[0] == $nomeCampo) {
            switch ($infoOrdenacao[1]) {
                case 'ASC':
                    $clsOrdenacao[] = 'ordenado';
                    break;
                case 'DESC':
                    $clsOrdenacao[] = 'ordenado_reverso';
                    break;
                default:
            }
        }

        return implode(' ', $clsOrdenacao);
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
     * Retorna uma lista com os campos da query que são exibidos em cada coluna.
     * @return array
     */
    protected function camposDoCabecalho()
    {
        // -- Lista completa de campos retornados pela query
        if (!empty($this->dados)) {
            // -- Lista completa de campos retornados pela query
            $campos = array_keys(current($this->dados));
        } else { // -- Utilizando qdo a listagem está com o tratamento de vazios Simec_Listagem::SEM_REGISTRO_LISTA_VAZIA
            $campos = $this->config->getListaColunas();
        }

        // -- Se há alguma ação, remove a primeira coluna, pois ela foi usada como id para as ações
        if ($this->config->getNumeroAcoes() > 0) {
            array_shift($campos);
        }

        // -- Elimina da lista de colunas, aquelas que estão ocultas
        $campos = array_diff($campos, $this->config->getColunasOcultas());
        return array_values($campos);
    }

    protected function renderDados($dados = null)
    {
        if (is_null($dados)) {
            $dados = $this->dados;
        }

        // -- @todo Verificar se o tipo de listagem é de FORM, para incluir os inputs
        if (empty($dados)) {
            $colspan = $this->config->getNumeroAcoes() + count($this->config->getListaColunas());
            echo <<<HTML
    <tbody>
        <tr class="lista-vazia">
            <td colspan="{$colspan}">
            <div class="alert alert-info col-lg-12" role="alert">Nenhum registro encontrado</div>
            </td>
        <tr>
    <tbody>
HTML;
            return;
        }
        echo <<<BODY
    <tbody>
BODY;
        foreach ($dados as $linha => $dadosLinha) {
            $this->renderLinha($linha, $dadosLinha);
        }
        echo <<<BODY
    </tbody>
BODY;
    }

    protected function renderLinha($linha, $dadosLinha)
    {
        $linhaFilha = array();
        if ($colunaAgrupada = $this->config->getColunaAgrupada() && key_exists($colunaAgrupada, $dadosLinha)) {
            $linhaFilha = $dadosLinha[$colunaAgrupada];
            unset($dadosLinha[$colunaAgrupada]);
        }

        $classe = $this->getClasseLinha($dadosLinha, current($dadosLinha));
        $id = !is_null($this->configIdLinha)?' id="' . $this->configIdLinha . current($dadosLinha) . '"':'';

        echo <<<TR
        <tr{$classe}{$id}>
TR;
        echo $this->renderAcoes($dadosLinha, $linha);

        // -- @todo: Verificar o tipo da listagem do formulario
        if ($this->config->getNumeroAcoes()) {
            $idLinha = array_shift($dadosLinha);
        } else {
            reset($dadosLinha);
            $idLinha = current($dadosLinha);
        }

        foreach ($dadosLinha as $key => $dado) {
            if (is_array($dado)) {
                $linhaFilha = $dado;
                continue;
            }

            if (!$this->config->colunaEstaOculta($key)) {
                // -- Verificação de totalizador de coluna
                $this->somarColuna($key, $dado);

                // -- Chamando função de callback registrada para o campo da listagem
                $dado = $this->aplicarCallback($key, $dado, array($dadosLinha, $idLinha, array(
                    // -- Adicione novos parâmetros neste array
                    'campo' => $key,
                )));
                echo <<<TD
            <td>{$dado}</td>
TD;
            }
        }

        // -- renderizando as colunas virtuais
        foreach ($this->config->getColunasVirtuais() as $coluna) {
            $retorno = $coluna($dadosLinha, $idLinha);
            echo <<<TD
            <td>{$retorno}</td>
TD;
        }

        echo <<<TR
        </tr>
TR;
        if (empty($linhaFilha)) {
            return;
        }

        // -- Renderização de linha filha
        $colspan = count($this->config->getCabecalho());
        echo "<tr class=\"tr_abre_e_fecha\"><td colspan=\"{$colspan}\">" . $this->renderRepeticaoTemplate($linhaFilha) . '</td></tr>';
    }

    /**
     * @param type $dados
     * @param type $linha
     * @return string
     */
    protected function renderAcoes($dados, $linha)
    {
        $acoesHTML = '';
        if (0 === $this->config->getNumeroAcoes()) {
            return $acoesHTML;
        }

        foreach ($this->config->getAcoes() as $acao => $configAcao) {
            if (!is_array($configAcao)) {
                $configAcao = array('func' => $configAcao);
            }

            if (!$this->config->acaoEhAgrupada($acao)) { // -- acao não agrupada
                $html = <<<HTML
<td class="text-center" style="width:33px">%s</td>
HTML;
                $acoesHTML .= sprintf(
                    $html,
                    $this->renderAcao($acao, $dados, $configAcao)
                );
            } elseif (!$this->acaoJahAgrupada($acao, $dados)) { // -- Agrupando o primeiro item da ação
                // -- Guardando os valores do novo agrupamento
                $this->salvarAgrupamento($acao, $dados);
                // -- Renderiza com rowspan
                $html = <<<HTML
<td class="text-center" style="width:33px" rowspan="%d">%s</td>
HTML;
                $acoesHTML .= sprintf(
                    $html,
                    $this->calculaRowspan($acao),
                    $this->renderAcao($acao, $dados, $configAcao)
                );
            } else { // -- Tratamento de ações já agrupadas
                // -- não faz nada
            }
        }
        return $acoesHTML;
    }

    protected function renderAcao($acao, $dados, $configAcao)
    {
        $configAcao['condicao'] = $this->config->getCondicaoAcao($acao);
        $configAcao['regra'] = $this->config->getRegraAcao($acao);
        $objAcao = Simec_Listagem_FactoryAcao::getAcao($acao, $configAcao);
        return (string)$objAcao->setDados($dados);
    }

    protected function acaoJahAgrupada($acao, $dados)
    {
        if (empty($this->acoesAgrupadasUltimoValor[$acao])) {
            $this->acoesAgrupadasUltimoValor[$acao] = array('dummy' => 2);
        }

        return $this->acoesAgrupadasUltimoValor[$acao] == array_intersect_assoc(
                $this->acoesAgrupadasUltimoValor[$acao],
                $dados
            );
    }

    protected function salvarAgrupamento($acao, $dados)
    {
        $this->acoesAgrupadasUltimoValor[$acao] = array_intersect_key(
            $dados,
            array_fill_keys($this->config->getAgrupamentoAcao($acao), null)
        );
    }

    protected function calculaRowspan($acao)
    {
        $ultimoAgrupamento = $this->acoesAgrupadasUltimoValor[$acao];
        $rowspan = 0;
        $achou = false;
        foreach ($this->dados as $linha) {
            $igual = ($ultimoAgrupamento == array_intersect_assoc($ultimoAgrupamento, $linha));
            // -- Se já encontrou um elemento igual anteriormente (achou == true) e o elemento
            // -- atualmente analisado não é igual, finaliza a comparação - os itens sempre devem
            // -- estar ordenado de acordo com o agrupamento - pequena otimização da busca evitando
            // -- a análise do restante da lista uma vez que o conjunto já foi encontrado.
            $achou = (!$achou && $igual);
            if ($achou && !$igual) {
                break;
            }
            $rowspan += (int)$igual;
        }
        return $rowspan;
    }

    /**
     * TODO: fazer o tratamento de valores considerando outros campos, quando o valor comparado for um array
     * @param $dados
     * @param $idLinha
     * @return string
     */
    protected function getClasseLinha($dados, $idLinha)
    {
        foreach ($this->config->getRegrasDeLinha() as $regra) {
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

            $valor = null;
            if ($regra['valorComoCampo'] && 'id' == $regra['valor']) {
                $valor = $idLinha;
            } else if ($regra['valorComoCampo']) {
                $valor = $dados[$regra['valor']];
            } else {
                $valor = $regra['valor'];
            }

            if (Simec_Operacoes::$method($dados[$regra['campo']], $valor)) {
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
        $qtdAcoes = $this->config->getNumeroAcoes();

        if (Simec_Listagem::TOTAL_QTD_REGISTROS == $this->config->getTotalizador()) {
            $subraiColunas = (int) $qtdAcoes==0 ? 0 : -1;
            $c = ( !empty($this->dados[0]) ? $this->dados[0] : array() ) ;
            $spanDeColunas = (count($c) + $subraiColunas) + $qtdAcoes;

            $totalRegistros = $this->datasource->getTotalRegistros();

            if($totalRegistros > 0){
                if ($this->datasource->paginarDados()) {
                    list($regInicial, $regFinal) = $this->datasource->registrosExibidos();
                    $htmlContadorRegistros = "<strong>Exibindo registros {$regInicial} a {$regFinal}, de {$totalRegistros}</strong>";
                } else {
                    $htmlContadorRegistros = "<strong>Total de registros:&nbsp; {$totalRegistros}</strong>";
                }
                echo <<<HTML
                    <tfoot>
                        <tr>
                            <td style="text-align:right" colspan="{$spanDeColunas}">{$htmlContadorRegistros}</td>
                        </tr>
                    </tfoot>
HTML;
            }

        } elseif (Simec_Listagem::TOTAL_SOMATORIO_COLUNA == $this->config->getTotalizador() && !empty($this->dados)) {
            echo <<<HTML
                    <tfoot>
                        <tr>
HTML;
                            // -- Correção para quando tem mais de uma ação na listagem.
                            $dadosLinha = $this->dados[0];
                            for ($i = 0; $i < $qtdAcoes; $i++) {
                                array_unshift($dadosLinha, "--a{$i}");
                            }

                            foreach ($dadosLinha as $key => $dado) {
                                if ($this->config->colunaEstaOculta($key)) {
                                    continue;
                                }
                                if (0 === $key) { // -- oO ????
                                    continue;
                                }

                                if ($this->config->colunaEhTotalizada($key)) {
                                    $valor = $this->aplicarCallback($key, $this->getTotalColuna($key));
                                    echo <<<HTML
                                            <td style="font-weight:bold;color:#428bca">{$valor}</td>
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

    protected function renderPrototipo()
    {
        if (!$this->renderPrototipo) {
            return '';
        }

        function criaParametro(&$item)
        {
            $item = "%{$item}%";
        }

        // -- Encontrando a lista de colunas da listagem
        $colunas = array();
        if (!empty($this->dados)) {
            $colunas = array_keys(current($this->dados));
        } elseif ($this->config->getListaColunas()) {
            $colunas = array_values($this->config->getListaColunas());
        } else {
            throw new Exception('Não é possível criar o protótipo de linha sem dados, ou sem a lista de campos.');
        }

        $arrayParametros = range(1, count($colunas));
        array_walk($arrayParametros, 'criaParametro');
        $arrayParametros = array_combine($colunas, $arrayParametros);

        // -- Gerando o HTML da lista
        ob_start();
        $this->renderLinha(0, $arrayParametros);
        return 'data-prototipo="' . htmlspecialchars(trim(ob_get_clean()), ENT_QUOTES) . '"';
    }

    /**
     * Cria, se necessário, uma instância da toolbar e a retorna.
     *
     * @return Simec_Listagem_Renderer_Html_Toolbar
     * @uses Simec_Listagem_Renderer_Html_Toolbar
     */
    public function getToolbar()
    {
        if (!isset($this->toolbar)) {
            $this->toolbar = new Simec_Listagem_Renderer_Html_Toolbar($this->config);
        }

        return $this->toolbar;
    }

    /**
     * Inclui o pesquisator na barra de ferramentas da listagem.
     *
     * @uses Simec_Listagem_Renderer_Html_Toolbar::PESQUISATOR
     */
    public function turnOnPesquisator()
    {
        $this->config->showToolbarPesquisator(true);
        $this->getToolbar()->add(Simec_Listagem_Renderer_Html_Toolbar::PESQUISATOR);
    }

    /**
     * Adiciona uma funcionalidade na barra de ferramentas da listagem.
     *
     * @param int,... $item Uma das funcionalidades definidas em Simec_Listagem_Renderer_Html_Toolbar.
     * @uses Simec_Listagem_Renderer_Html_Toolbar::ADICIONAR Funcionalidade de adicionar novo item.
     * @uses Simec_Listagem_Renderer_Html_Toolbar::INVERTER Funcionalidade de inverter a seleção.
     */
    public function addToolbarItem($item)
    {
        $this->getToolbar()->add(func_get_args());
    }

    /**
     * Indica que um prototipo de linha da listagem deve ser incluso no atributo data-prototipo.
     */
    public function turnOnPrototipo()
    {
        $this->renderPrototipo = true;
    }

    /**
     * Renderiza o pesquisator - busca rápida.
     * @return string
     */
    protected function renderToolbar()
    {
        if (isset($this->toolbar)) {
            return (string)$this->toolbar;
        }
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

    public function setDatasource(\Simec_Listagem_Datasource $ds)
    {
        parent::setDatasource($ds);

        if (!$_SESSION['superuser']) {
            return $this;
        }

        if ($query = $ds->getQuery()) {
            $this->config->showToolbarQuery(true);
            $this->getToolbar()->add(Simec_Listagem_Renderer_Html_Toolbar::QUERY)
                ->setQuery($query);
        }

        return $this;
    }
}
