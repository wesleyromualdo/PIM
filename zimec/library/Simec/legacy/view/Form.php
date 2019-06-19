<?php
/**
 * Criação de formulários html/bootstrap.
 *
 * $Id: Form.php 105253 2015-11-26 16:52:42Z maykelbraz $
 * @package Simec\View\Form
 */

/**
 * Classe de criação de formulários customizados da SPO, utilizando os componentes definidos
 * em funcoesspo_componentes.php.
 *
 * => Botões do tipo 'submit' verificam se existe a função onNomedoformSubmit está definida. Se estiver,
 * ela é executada antes do submit.
 * => Botão do tipo 'novo' executa a função onNomedoformNovo, se ela não estiver definida, um alerta é gerado.
 *
 * Importante: Os dados enviados pelo formulário são AUTOMATICAMENTE, agrupados dentro de um array
 * com o nome igual ao ID do formulário.
 *
 * Exemplos:
 * $form = new Simec_View_Form('periodoreferencia', Simec_View_Form::POST);
 * $form->addInputTexto('dados[prftitulo]', $dados['prftitulo'], 'prftitulo', 250, false, ['flabel' => 'Título'])
 *     ->addBotoes(['limpar', 'buscar'])
 *     ->addHidden('prfid', 3, 'prfid')
 *     ->addHiddens([
 *         ['nome' => 'prfid2', 'valor' => 3, 'id' => 'prfid2'],
 *         ['nome' => 'prfid24', 'valor' => 43, 'id' => 'prfid42']
 *     ])
 *     ->addSeparador()
 *     ->addSeparador('texto', 'h3', 'vermelho')
 *     ->addInputData(
 *         ['prfpreenchimentoinicio', 'prfpreenchimentofim'],
 *         [$dados['prfpreenchimentoinicio'], $dados['prfpreenchimentofim']],
 *         ['prfpreenchimentoinicio', 'prfpreenchimentofim'],
 *         ['flabel' => 'Período de preenchimento']
 *     )->addBotoes(['limpar', 'salvar', 'novo' => ['label' => 'Novo pedido'], 'buscar'])
 *     ->setRequisicao('salvar')
 *     ->render();
 *
 * @package Simec\View\Form
 * @see funcoesspo_componentes.php
 */
class Simec_View_Form
{
    const POST = 'POST';
    const GET = 'GET';

    /**
     * @var string ID HTML do formulário.
     */
    protected $id;
    protected $method;
    protected $action;
    protected $requisicao;
    protected $nomeRequisicao;
    protected $opcoes = array();
    protected $elementos = array();

    /**
     * @var mixed[] Lista de valores do formulário.
     */
    protected $dados = array();

    /**
     * @var bool Indica se algum dos novos métodos de adição de inputs já foi chamado.
     */
    protected $addInputChamado = false;

    protected $icones = array(
        'limpar' => 'remove-circle',
        'salvar' => 'ok',
        'buscar' => 'search',
        'importar' => 'upload',
        'novo' => 'plus',
        'visualizar' => 'eye-open',
        'copiar' => 'copy',
        'executar' => 'cog'
    );

    /**
     * @var string[] Armazena JSs associados aos botões.
     */
    protected $js = array();

    /**
     * @var string[] Lista com o nome dos campos que são obrigatórios.
     */
    protected $camposObrigatorios = array();

    /**
     * Cria uma nova instância de Simev_View_Form.
     *
     * Opções implementadas:
     * => enctype: Define o enctype do form - utilize quando o formulário contiver inputs de arquivo.
     * => name: Nome do formulário -  por padrão é utilizado o id do formulário.
     *
     * Importante: Ao utilizar o método GET, você deve, manualmente, adicionar os campos da URL como inputs HIDDEN.
     *
     * @param string $id Id do formulário
     * @param string $method Método de envio de dados do formulário, utilize: Simec_View_Form::POST ou Simec_View_Form::GET.
     * @param string $action URL para a qual o formulário será enviado - padrão é a mesma URL da página.
     * @param array $opcoes Opções diversas do formulário - veja descrição para opções já implementadas.
     *
     * @todo Definir automaticamente o enctype do form para multipart/data quando um campo de arquivo for incluso.
     * @todo Incluir input de arquivo (não esquecer de colocar opção para baixar o modelo do arquivo a ser enviado).
     * @todo Incluir automaticamente campos da URL no formulário qdo form utilizado com o MÉTODO GET.
     */
    public function __construct($id, $method = Simec_View_Form::POST, $action = '', array $opcoes = array())
    {
        $this->id = $id;
        $this->method = $method;

        if ($action) {
            $this->action = $action;
        }
        if (!empty($opcoes)) {
            $this->opcoes = $opcoes;
        }
    }

    /**
     * Gera um ID para um campo com base no id do formulário e o nome do campo.
     *
     * @param string $nome O nome do campo para criar o id do campo.
     * @return string
     * @uses self::$id
     */
    protected function getCampoId($nome)
    {
        return "{$this->id}_{$nome}";
    }

    /**
     * Define a requisição padrão do formulário.
     * Este campo pode ser acessada diretamente via javascript pelo id: idform_requisicao.
     *
     * @param type $string
     * @return \Simec_View_Form
     */
    public function setRequisicao($string, $nomeRequisicao = 'requisicao')
    {
        $this->requisicao = $string;
        $this->nomeRequisicao = $nomeRequisicao;
        return $this;
    }

    /**
     * Adiciona um input de texto ao formulário.
     *
     * O ID do input é criado segundo a seguinte fórmula: idform_nomeinput. Tem suporte
     * à carga de valores atribuídos pelo método Simec_View_Form::carregarDados().
     *
     * @param string $label Label do input de texto no formulário.
     * @param string $nome Nome do input de texto no formulário.
     * @param int $limite Numero máximo de caracteres aceitáveis no input.
     * @param mixed[] $opcoes Lista de opções de modificação do input.
     * @return \Simec_View_Form
     *
     * @uses Simec_View_Form::addInputTexto()
     */
    public function addTexto($label, $nome, $limite, array $opcoes = array())
    {
        $this->addInputChamado = true;

        $id = "{$this->id}_{$nome}";
        $opcoes['flabel'] = $label;

        return $this->addInputTexto($nome, $this->getValor($nome), $id, $limite, false, $opcoes);
    }

    public function addMoeda($label, $nome, array $opcoes = array())
    {
        $this->addInputChamado = true;

        $id = "{$this->id}_{$nome}";
        $opcoes['flabel'] = $label;

        $limite = isset($opcoes['masc'])
            ?strlen($opcoes['masc'])
            :strlen('###.###.###.###.###,##');

        return $this->addInputTexto($nome, $this->getValor($nome), $id, $limite, true, $opcoes);
    }

    public function addTextarea($label, $nome, $limite, array $opcoes = array())
    {
        $this->addInputChamado = true;
        $opcoes['flabel'] = $label;
        $id = "{$this->id}_{$nome}";

        return $this->addInputTextarea($nome, $this->getValor($nome), $id, $limite, $opcoes, true);
    }

    /**
     * Adiciona um select ao formulário.
     *
     * O ID do select é criado segundo a seguinte fórmula: idform_nomeselect. Tem suporte
     * à carga de valores atribuídos pelo método Simec_View_Form::carregarDados().
     *
     * @param string $label Label do select no formulário.
     * @param string $nome Nome do select no formulário.
     * @param string|array $dados Query ou array de dados com as opções do select.
     * @param array $opcoes Lista de opções de modificação do select.
     *
     * @uses Simec_View_Form::addInputCombo()
     */
    public function addCombo($label, $nome, $dados, array $opcoes = array())
    {
        $this->addInputChamado = true;

        $opcoes['flabel'] = $label;
        $id = "{$this->id}_{$nome}";

        return $this->addInputCombo($nome, $dados, $this->getValor($nome), $id, $opcoes);
    }

    public function addData($label, $nome, array $opcoes = array())
    {
        $this->addInputChamado = true;
        $opcoes['flabel'] = $label;
        $id = "{$this->id}_{$nome}";

        return $this->addInputData($nome, $this->getValor($nome), $id, $opcoes);
    }

    /**
     * Adiciona ao formulário um input do tipo slider, um seletor de valores.
     *
     * As seguintes opções de modificação estão disponíveis:
     * * marcos: Adiciona labels e pontos específicos de valores ao slider.
     * * max: O valor máximo do slider, se não for informado é 100;
     * * min: O valor mínimo do slider, se não for informado é 0;
     * * passo: Passo de incremento do slider, se não for informado é 1.
     *
     * @param string $label Label do slider.
     * @param string $nome Nome do input.
     * @param array $opcoes Opções de modificação do slider.
     * @return \Simec_View_Form
     *
     * @example
     * <code>
     * $form = new Simec_View_Form('id_do_form');
     * $form->addSlider(
     *     'Percentual %',
     *     'vlrpercentual',
     *     [
     *          'min' => -10,
     *          'max' => 150,
     *          'passo' => 2,
     *          'marcos' => [0 => '0 %', 100 => '100 %']
     *     ]);
     * </code>
     */
    public function addSlider($label, $nome, array $opcoes = array())
    {
        $this->elementos[] = ['func' => 'renderSlider', 'params' => func_get_args()];
        return $this;
    }

    /**
     * Cria uma lista de opções para seleção (input radio).
     *
     * Tem suporte à carga de valores atribuídos pelo método Simec_View_Form::carregarDados().
     *
     * @param string $label Label da lista de choices no formulário.
     * @param string $nome Nome dos itens da lista de choices no formulário.
     * @param array $opcoes Lista de opções. As chaves representam o texto das opções, os valores são os valores enviados no formulário.
     * @param array $config Lista de opções de modificação dos choices.
     * @return \Simec_View_Form
     *
     * @uses Simec_View_Form::addInputChoices()
     */
    public function addChoices($label, $nome, array $opcoes, array $config = array())
    {
        $this->addInputChamado = true;

        $config['flabel'] = $label;
        $prefixoId = "{$this->id}_{$nome}_";

        return $this->addInputChoices($nome, $opcoes, $this->getValor($nome), $prefixoId, $config);
    }

    /**
     * Adiciona um checkbox ao formulário.
     *
     * Tem suporte à carga de valores atribuídos pelo método Simec_View_Form::carregarDados().
     *
     * @param string $label Texto apresentado ao lado do checkbox.
     * @param string $nome Nome do checkbox no formulário.
     * @param array $opcoes Lista de opções de modificação do checkbox.
     * @return \Simec_View_Form
     *
     * @uses Simec_View_Form::addInputCheckbox()
     */
    public function addCheckbox($label, $nome, array $opcoes = array())
    {
        $this->addInputChamado = true;

        $opcoes['flabel'] = $label;
        $id = "{$this->id}_{$nome}";

        return $this->addInputCheckbox($nome, null, $id, $opcoes);
    }

    public function addHidden($nome, $valor, $id = '')
    {
        $this->elementos[] = array('func' => 'renderHidden', 'params' => array(
            'name' => $nome,
            'value' => (('' === $valor)?$this->getValor($nome):$valor),
            'id' => (empty($id)?"{$this->id}_{$nome}":$id)
        ));

        return $this;
    }

    public function addFile($label, $nome, array $opcoes = array())
    {
        $this->addInputChamado = true;
        $this->elementos[] = array('func' => 'renderFile', 'params' => func_get_args());

        // -- setando o enctype automaticamente
        if (!array_key_exists('enctype', $this->opcoes)) {
            $this->opcoes['enctype'] = 'multipart/form-data';
        }

        return $this;
    }

    /**
     * Cria uma input do tipo texto.
     *
     * Importante: Se definida a opção flabel, o input é criado com um label e ocupa uma linha do formulário.
     *
     * @param string $nome Nome do input.
     * @param mixed $valor Valor para o input.
     * @param string $id Id do input.
     * @param integer $limite Limite de caracteres do input.
     * @param bool $ehMonetario Indica se o input é para campos monetários, em caso positivo aplica máscara.
     * @param array $opcoes Opções adicionais de configuração do input - Veja a função inputTexto() para detalhamento.
     * @return \Simec_View_Form
     *
     * @uses inputTexto();
     * @deprecated Utilize Simec_View_Form::addTexto() e Simec_View_Form::addMoeda()
     */
    public function addInputTexto($nome, $valor, $id, $limite, $ehMonetario = true, array $opcoes = array())
    {
        $this->elementos[] = array('func' => 'inputTexto', 'params' => func_get_args());
        return $this;
    }

    /**
     * Cria um input do tipo select.
     *
     * Importante: Se definida a opção flabel, o input é criado com um label e ocupa uma linha do formulário.
     *
     * @param string $nome Nome do input.
     * @param string|array $dados Fonte de dados do input, um array ou uma query - Deve conter os campos: codigo e descricao.
     * @param mixed $valor Valor selecionado no input.
     * @param string $id Id do input.
     * @param array $opcoes Opções adicionais de configuração do input - Veja a função inputCombo() para detalhamento.
     * @return \Simec_View_Form
     *
     * @deprecated Utilize Simec_View_Form::addCombo()
     * @uses inputCombo()
     */
    public function addInputCombo($nome, $dados, $valor, $id, array $opcoes = array())
    {
        $this->elementos[] = array('func' => 'inputCombo', 'params' => func_get_args());
        return $this;
    }

    /**
     * Cria um textarea com contagem de caracteres.
     *
     * @param string $nome Nome do textarea.
     * @param mixed $valor Valor para o textarea.
     * @param string $id Id do textarea.
     * @param integer $limite Limite de caracteres do textarea.
     * @param array $opcoes Opções adicionais de configuração do textarea - Veja a função inputTextArea() para detalhamento.
     * @param type $bpClass
     * @return \Simec_View_Form
     *
     * @see inputTextArea()
     * @deprecated Utilize Simec_View_Form::addTextarea()
     */
    public function addInputTextarea($nome, $valor, $id, $limite, array $opcoes = array(), $bpClass = true)
    {
        $this->elementos[] = array('func' => 'inputTextArea', 'params' => func_get_args());
        return $this;
    }

    /**
     * Método ainda não validado, (deveria) cria(r) coleção de choices.
     *
     * @param type $nome
     * @param array $opcoes
     * @param type $valorMarcado
     * @param type $prefixoId
     * @param array $config
     * @return \Simec_View_Form
     *
     * @uses inputChoices()
     * @deprecated Utilize Simec_View_Form::addChoices()
     */
    public function addInputChoices($nome, array $opcoes, $valorMarcado, $prefixoId, array $config = null)
    {
        $this->elementos[] = array('func' => 'inputChoices', 'params' => func_get_args());
        return $this;
    }

    /**
     * Método ainda não validado, (deveria) cria(r) coleção de choices.
     *
     * @param type $nome
     * @param type $valor
     * @param type $id
     * @param array $opcoes
     * @return \Simec_View_Form
     *
     * @uses inputCheckbox()
     * @deprecated Utilize Simec_View_Form::addCheckbox()
     */
    public function addInputCheckbox($nome, $valor, $id, $opcoes = array())
    {
        $this->elementos[] = array('func' => 'inputCheckbox', 'params' => func_get_args());
        return $this;
    }

    /**
     * Criar inputs do tipo data, e faz a inicialização de inclusão dos javascripts necessários.
     *
     * Importante: Se definida a opção flabel, o input é criado com um label e ocupa uma linha do formulário.
     * Importante: Os parâmetros $nome, $valor e $id podem ser strings ou arrays. Se forem arrays, dois campos de data são criados (range).
     *
     * @param string|array $nome Nome(s) do(s) input(s).
     * @param string|array $valor Valor(es) do(s) input(s).
     * @param string|array $id Id(s) do(s) input(s).
     * @param array $opcoes Configurações adicionais do(s) input(s) - Veja a função inputTexto() para detalhamento.
     * @return \Simec_View_Form
     *
     * @see inputData()
     */
    public function addInputData($nome, $valor, $id, array $opcoes = array())
    {
        $this->elementos[] = array('func' => 'inputData', 'params' => func_get_args());
        return $this;
    }

    /**
     * Cria uma lista com cheboxes para ser utilizada em formulários. Segue o mesmo padrão de
     * renderização dos demais inputs.
     *
     * Campos obrigatórios: codigo e selecionado.
     * codigo: Deve ser o primeiro campo da query e será utilizado para criar os checkboxes, é o id da lista.
     * selecionado: Utilizado para dizer quais checkboxes já estão selecionados - deve ser booleano.
     *
     * @param string $nome Nome do atributo do formulário utilizado para os checkboxes.
     * @param string $sqlItens SQL que faz a consulta dos itens da lista.
     * @param array $opcoes Opções adicionais.
     */
    public function addInputLista($nome, $sqlItens, array $opcoes = array())
    {
        $this->elementos[] = array('func' => 'renderLista', 'params' => func_get_args());
    }

    /**
     *
     * @param type $config
     * @return \Simec_View_Form
     */
    public function addBotao($config)
    {
        $this->elementos[] = array('func' => 'renderBotao', 'params' => func_get_args());
        return $this;
    }

    public function addBotoes(array $botoes)
    {
        $this->addSeparador();

        foreach ($botoes as $key => $botao) {
            if (is_array($botao)) {
                $this->addBotao($key, $botao);
            } else {
                $this->addBotao($botao);
            }
        }
        return $this;
    }

    public function addSeparador($texto = '', $tag = '', $classes = '')
    {
        $this->elementos[] = array('func' => 'renderSeparador', 'params' => func_get_args());
        return $this;
    }

    public function addHiddens(array $hiddens)
    {
        foreach ($hiddens as $hidden) {
            $this->addHidden($hidden['nome'], $hidden['valor'], isset($hidden['id'])?$hidden['id']:'');
        }

        return $this;
    }

    /**
     *
     * @param bool $echo Indica se o html deve ser impresso ou retornado.
     * @return type
     */
    public function render($echo = true)
    {
        $formName = $this->opcoes['name']?$this->opcoes['name']:$this->id;
        $enctype = $this->opcoes['enctype']?" enctype=\"{$this->opcoes['enctype']}\"":'';
        $action = $this->action?" action=\"{$this->action}\"":'';

        $html = <<<DML
<div class="well col-md-12">
    <form id="{$this->id}" class="form-horizontal" role="form"
          method="{$this->method}" name="{$formName}"{$enctype}{$action}>
        %s
    </form>
</div>
DML;
        // -- incluindo os elementos
        $htmlInputs = <<<HTML
<input type="hidden" name="{$this->nomeRequisicao}" id="{$this->id}_requisicao" value="{$this->requisicao}" />
HTML;
        foreach ($this->elementos as $input) {
            // -- prefixo dos campos do formulario
            switch ($input['func']) {
                case 'inputCheckbox':
                case 'inputChoices':
                case 'inputTexto':
                case 'inputTextArea':
                case 'inputCombo':
                    $input['params'][0] = "{$this->id}[{$input['params'][0]}]";
                    break;
                case 'renderHidden':
                    $input['params']['name'] = "{$this->id}[{$input['params']['name']}]";
                    break;
                case 'inputData':
                    if (is_array($input['params'][0])) {
                        $input['params'][0][0] = "{$this->id}[{$input['params'][0][0]}]";
                        $input['params'][0][1] = "{$this->id}[{$input['params'][0][1]}]";
                    } else {
                        $input['params'][0] = "{$this->id}[{$input['params'][0]}]";
                    }
                    break;
                case 'renderFile':
                    break;
                default:
            }

            // -- renderização dos itens
            switch ($input['func']) {
                // @todo Estes não podem ser o default?
                case 'renderSeparador':
                case 'renderBotao':
                case 'renderHidden';
                case 'renderLista':
                case 'renderFile';
                case 'renderSlider':
                    $input['func'] = array($this, $input['func']);
                    break;
                case 'inputTextArea':
                    if (isset($input['params'][4])) {
                        $input['params'][4]['return'] = true;
                    } else {
                        $input['params'][4] = array('return' => true);
                    }
                    break;
                default:
                    $input['params'][count($input['params'])-1]['return'] = true;
            }

            $htmlInputs .= call_user_func_array($input['func'], $input['params']);
        }

        $html = sprintf($html, $htmlInputs);

        if (!empty($this->js)) {
            $js = implode('', $this->js);
            $html .= <<<JS
<script type="text/javascript">
$(function(){
    {$js}
});
</script>
JS;
        }

        if (!$echo) {
            return $html;
        }

        echo $html;
    }

    /**
     * Armazena os valores que os campos de input deverão carregar.
     *
     * @param mixed[]|null $dados Lista de valores a serem atribuídos aos inputs.
     */
    public function carregarDados($dados)
    {
        if (empty($dados)) {
            return $this;
        }

        if (!is_array($dados)) {
            throw new Exception('Este método deve, obrigatoriamente, receber um array como parâmetro.');
        }

        if ($this->addInputChamado) {
            throw new Exception('Simec_View_Form::carregarDados() deve ser chamado antes dos métodos de adição de inputs.');
        }

        $this->dados = $dados;
        return $this;
    }

    protected function getValor($nome)
    {
        return array_key_exists($nome, $this->dados)?$this->dados[$nome]:null;
    }

    protected function renderSeparador()
    {
        $args = func_get_args();
        if (empty($args)) {
            return '<hr />';
        } elseif (1 == count($args)) {
            return "<h1>{$args[0]}</h1>";
        } elseif (2 == count($args)) {
            return "<{$args[1]}>{$args[0]}</{$args[1]}>";
        } elseif (3 == count($args)) {
            return <<<HTML
<{$args[1]} class="{$args[2]}">{$args[0]}</{$args[1]}>
HTML;
        }
    }

    /**
     * @todo Extender a validação de label do tipo 'novo' para os demais botões
     * @return type
     */
    protected function renderBotao()
    {
        $args = func_get_args();
        $id = (is_array($args[1]) && isset($args[1]['id']))?" id=\"{$args[1]['id']}\"":'';
        $tipo = current($args);
        $icone = $this->icones[$tipo];
        $label = (is_array($args[1]) && isset($args[1]['label']))
                    ?$args[1]['label']
                    :ucfirst($tipo);
        $tmpID = ucfirst($this->id); // -- id do form

        switch ($tipo) {
            case 'limpar':
                $cor = 'warning';
                $role = 'reset';
                $this->js[] = <<<JS
$('.btn-{$args[0]}').click(function(){
    var url = window.location.href;
    window.location.assign(url);
});
JS;
                break;
            case 'salvar':
            case 'buscar':
            case 'importar':
            case 'executar':
                $cor = 'primary';
                $role = 'submit';

                $this->js[] = <<<JS
$('#{$this->id}').submit(function(e){
    if (typeof Function == typeof window['on{$tmpID}Submit']) {
        return window['on{$tmpID}Submit'](e);
    }
});
JS;
                break;
            case 'novo':
                $cor = 'success';
                $role = 'button';
                $this->js[] = <<<JS
$('#{$this->id} .btn-{$args[0]}').click(function(e){
    if (typeof Function == typeof window['on{$tmpID}Novo']) {
        return window['on{$tmpID}Novo'](e);
    }
    alert("A função 'on{$tmpID}Novo' não está definida.");
    return false;
});
JS;
                break;
            case 'visualizar':
                $cor = 'info';
                $role = 'button';
                break;
            case 'copiar':
                $cor = 'success';
                $role = 'submit';
                break;
            case 'avancado':
                $label = ''; $span = ''; $class = 'btn-default'; $extra = '';
                if(is_array($args[1])){
                    $label = isset($args[1]['label']) ? $args[1]['label'] : ucfirst($args[0]);
                    $span = isset($args[1]['span']) ? "<span class=\"{$args[1]['span']}\"></span>" : $span;
                    $class = isset($args[1]['class']) ? $args[1]['class'] : $class;
                    $extra = isset($args[1]['extra']) ? $args[1]['extra'] : $extra;
                }

                return <<<HTML
<button{$id} class="btn {$class} btn-{$args[0]}" {$extra} type="button">{$span} {$label}</button>&nbsp
HTML;
            default:
                throw new Exception('Botão não identificado');
        }

        return <<<HTML
<button{$id} class="btn btn-{$cor} btn-{$tipo}" type="{$role}">
    <span class="glyphicon glyphicon-{$icone}"></span> {$label}
</button>&nbsp
{$adicional}
HTML;
    }

    protected function renderHidden($name, $value, $id = '')
    {
        $id = $id?" id=\"{$id}\"":'';
        return <<<HTML
<input type="hidden" name="{$name}" value="{$value}" {$id} />
HTML;
    }

    protected function renderSlider($label, $nome, array $opcoes = array())
    {
        $id = $this->getCampoId($nome);
        $value = $this->getValor($nome);
        $nomeCompleto = "{$this->id}[{$nome}]";

        // -- Max
        if (!isset($opcoes['max'])) {
            $opcoes['max'] = 100;
        }
        if (!isset($opcoes['min'])) {
            $opcoes['min'] = 0;
        }
        if (!isset($opcoes['passo'])) {
            $opcoes['passo'] = 1;
        }
        if ('' === $value) {
            $value = 0;
        }

        $valorMarcos = $labelMarcos = '';
        if (isset($opcoes['marcos'])) {
            $valorMarcos = 'data-slider-ticks="[' . implode(', ', array_keys($opcoes['marcos'])) . ']"';
            $labelMarcos = "data-slider-ticks-labels='[" . implode(
                ', ',
                array_map(function($value){
                    return '"' . $value . '"';
                }, $opcoes['marcos'])) . "]'";
        }

        return $this->renderSliderJs() . <<<HTML
<div class="form-group control-group">
    <label class="col-lg-2 control-label pad-12">{$label}:</label>
    <div class="col-lg-10">
        <div style="padding-left:10px; padding-top:5px">
            <input type="text" id="{$id}" class="bt-slider" name="{$nomeCompleto}"
                   data-slider-id="slider_{$id}"
                   data-slider-max="{$opcoes['max']}"
                   data-slider-min="{$opcoes['min']}"
                   data-slider-step="{$opcoes['passo']}"
                   data-slider-value="{$value}"
                   {$valorMarcos}
                   {$labelMarcos} />
        </div>
    </div>
</div>
HTML;
    }

    /**
     * Verifica a necessidade de incluir o JS com a definição do bootstra-slider, evitando
     * requisições desnecessárias.
     *
     * @staticvar boolean $jsIncluso Indica se o JS já foi incluso alguma vez.
     * @return string
     */
    public function renderSliderJs()
    {
        static $jsIncluso = false;
        if (!$jsIncluso) {

            $jsIncluso = true;
            return <<<HTML
<link href="/library/bootstrap-slider/css/bootstrap-slider.min.css" rel="stylesheet">
<script type="text/javascript" lang="JavaScript">
jQuery(document).ready(function(){
    jQuery.getScript('/library/bootstrap-slider/bootstrap-slider.min.js', function(){
        $('.bt-slider').bootstrapSlider();
    });
});
</script>
HTML;
        }

        return '';
    }

    /**
     *
     * @param type $label
     * @param type $name
     * @param array $opcoes
     * @return type
     *
     * @todo Fazer o download do arquivo qdo ele tiver um arquivo carregado
     */
    protected function renderFile($label, $name, array $opcoes = array())
    {
        $id = $opcoes['id']?$opcoes['id']:"{$this->id}_{$name}";
        $nomeCompleto = "{$this->id}_{$name}";
        $titulo = $opcoes['titulo']?$opcoes['titulo']:'Selecione um arquivo...';

        $html = <<<HTML
<div class="form-group">
    <label for="{$id}" class="col-md-2 control-label">{$label}</label>
    <div class="col-md-10">
HTML;

        if ($valor = $this->getValor($name)) {
            $file = new FilesSimec();
            $arquivo = $file->getDadosArquivo($valor);

            $html .= <<<HTML
    <p class="form-control-static">
        <a href="{$_SERVER['REQUEST_URI']}&download=S&arquivo={$valor}">{$arquivo['arqdescricao']}<a>&nbsp;&nbsp;&nbsp;&nbsp;
HTML;
        if ($opcoes['button']) {
            $html.= <<<HTML
        <button id="{$opcoes['button']['id']}"
                class="{$opcoes['button']['class']}"
                type="button"
                data-arqid="{$valor}">{$opcoes['button']['texto']}</button>
    </p>
HTML;
                }
        } else {
            $html .= <<<HTML
                <input type="file" class="btn btn-primary" name="{$nomeCompleto}" id="{$id}" title="{$titulo}" />
HTML;
        }
        $html .= <<<HTML
    </div>
</div>
HTML;
        return $html;
    }

    protected function renderLista($nome, $sqlItens, array $opcoes = array())
    {
        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $list->turnOnPesquisator()
            ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::ADICIONAR)
            ->turnOnPrototipo()
            ->setFormOff()
            ->setCampos($opcoes['campos']);

        // -- Cabeçalho da listagem
        if (isset($opcoes['cabecalho'])) {
            $list->setCabecalho($opcoes['cabecalho']);
        }
        // -- Ações da listagem
        if (isset($opcoes['acoes'])) {
            foreach ($opcoes['acoes'] as $acao) {
                list($acao, $config) = $acao;
                $list->addAcao($acao, $config);
            }
        }
        // -- Callbacks
        if (isset($opcoes['callbacks'])) {
            foreach ($opcoes['callbacks'] as $callback) {
                list($campo, $config) = $callback;
                $list->addCallbackDeCampo($campo, $config);
            }
        }
        // -- id
        if (isset($opcoes['id'])) {
            $list->setId($opcoes['id']);
        }

        // -- totalizador
        if (isset($opcoes['totalizador'])) {
            $params = array(
                ('linhas' == $opcoes['totalizador'])
                    ?Simec_Listagem::TOTAL_QTD_REGISTROS
                    :Simec_Listagem::TOTAL_SOMATORIO_COLUNA
            );
            if (is_array($opcoes['totalizador'])) {
                $params[] = $opcoes['totalizador'];
            }
            call_user_func_array(array($list, 'setTotalizador'), $params);
        }

        $list->setQuery($sqlItens);
        $html = <<<HTML
<div class="form-group control-group">
    <label class="col-lg-2 control-label pad-12">{$opcoes['flabel']}:</label>
    <div class="col-lg-10">
HTML;
        $html .= $list->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        $html .= <<<HTML
    </div>
</div>
HTML;
        return $html;
    }

    /**
     * Define um, ou vários campos, como obrigatório.
     *
     * @param string $campo,... Campo ou campos obrigatórios do formulário.
     */
    public function setObrigatorio($campo)
    {
        $this->camposObrigatorios = array_merge(
            $this->camposObrigatorios,
            func_get_args()
        );
        return $this;
    }
}
