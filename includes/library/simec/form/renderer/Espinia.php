<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 17/11/2016
 * Time: 09:05
 * 
 */

require_once(dirname(__FILE__) . '/../Form_Abstract.php');

require_once(dirname(__FILE__) . '/../../view/Helper.php');

class Simec_Renderer_Form_Espinia extends Simec_Form_Abstract
{
    protected $botoes = [];

    /**
     * Função que adiciona um campo no formulário
     *
     * @param $func - função do campo. Ex: 'input', 'checkbox', 'select', etc...
     * @param $name - nome do campo
     * @param null $label - label docampo
     * @param null $value - valor do campo
     * @param array $attribs - atributos do campo. Ex: 'class', 'style', etc...
     * @param array $opcoes - este campo é utilizado para passar as opções de um 'select', por exemplo. Existem campos que
     * não possuem este parâmetro
     * @return $this
     */
    private function addCampo($func, $name, $label = null, $value = null, $attribs = [], $opcoes = [])
    {
        $attribs['id'] = !empty($attribs['id']) ? $attribs['id'] : $this->getCampoId($name);
        $this->campos[] = [
        'func' => $func,
        'param' => [
        'name' => $name,
        'label' => $label,
        'value' => $value,
        'attribs' => $attribs,
        'opcoes' => $opcoes
        ]
        ];
        return $this;
    }

    /**
     * Função que adiciona um 'input' no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $attribs
     * @return $this
     */
    public function addInput($name, $label = null, $value = null, $attribs = [], $config = [])
    {
        $this->addCampo('input', $name, $label, $value, $attribs, $config);
        return $this;
    }

    /**
     * Função que adiciona um campo de CEP no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $attribs
     * @return $this
     */
    public function addCep($name, $label = null, $value = null, $attribs = [])
    {
        $this->addCampo('cep', $name, $label, $value, $attribs);
        return $this;
    }

    /**
     * Função que adiciona um campo CPF no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $attribs
     * @return $this
     */
    public function addCpf($name, $label = null, $value = null, $attribs = [])
    {
        $this->addCampo('cpf', $name, $label, $value, $attribs);
        return $this;
    }

    /**
     * Função que adiciona um campo CNPJ no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $attribs
     * @return $this
     */
    public function addCnpj($name, $label = null, $value = null, $attribs = [])
    {
        $this->addCampo('cnpj', $name, $label, $value, $attribs);
        return $this;
    }

    /**
     * Função que adiciona um campo EMAIL no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $attribs
     * @return $this
     */
    public function addEmail($name, $label = null, $value = null, $attribs = [])
    {
        $this->addCampo('email', $name, $label, $value, $attribs);
        return $this;
    }

    /**
     * Função que adiciona um campo 'Telefone' no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $attribs
     * @return $this
     */
    public function addTelefone($name, $label = null, $value = null, $attribs = [])
    {
        $this->addCampo('telefone', $name, $label, $value, $attribs);
        return $this;
    }

    /**
     * Função que adiciona um campo de data no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $attribs
     * @return $this
     */
    public function addData($name, $label = null, $value = null, $attribs = [])
    {
        $this->addCampo('data', $name, $label, $value, $attribs);
        return $this;
    }

    /**
     * Função que adiciona um campo de período no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $attribs
     * @return $this
     */
    public function addPeriodo($name, $label = null, $value = null, $attribs = [])
    {
        $this->addCampo('periodo', $name, $label, $value, $attribs);
        return $this;
    }

    /**
     * Função que adiciona uma combobox, select, no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes
     * @param array $attribs
     * @return $this
     */
    public function addSelect($name, $label = null, $value = null, $opcoes = [], $attribs = [])
    {
        $this->addCampo('select', $name, $label, $value, $attribs, $opcoes);
        return $this;
    }

    public function addSelectOutros($name, $label = null, $value = null, $opcoes = [], $attribs = [], $attribsOutros = [])
    {
        if (!in_array('O', $opcoes)) {
            $opcoes = array_merge($opcoes, ['O' => 'Outros']);
        }
        $id = $this->getCampoId($name);
        $idOutros = $this->getCampoId('outros_'.$name);
        $this->addSelect($name, $label, $value, $opcoes, $attribs);
        $this->addInput('outros_'.$name, 'Outros', null, $attribsOutros, ['visible' => false]);
        $script = <<<SCRIPT
            $(document).ready(function(){
                $('#{$id}').change(function(){
                    if($(this).val() == 'O'){
                        $('#{$idOutros}').parent("div").parent(".form-group").removeClass('hidden');
                    } else {
                        $('#{$idOutros}').parent("div").parent(".form-group").addClass('hidden');
                        $('#{$idOutros}').val('');
                    }
                });
            });
SCRIPT;
        $this->addScript($script);
        return $this;
    }

    /**
     * Função que adiciona um textarea no formulário
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $attribs
     * @return $this
     */
    public function addTextarea($name, $label = null, $value = null, $attribs = [])
    {
        $this->addCampo('textarea', $name, $label, $value, $attribs);
        return $this;
    }

    /**
     * Função que adiciona campos 'radio' no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes - opções do grupo de botões. Ex: ['t' => 'Sim', 'n' => 'Não']
     * @param array $attribs
     * @return $this
     */
    public function addRadio($name, $label = null, $value = null, $opcoes = [], $attribs = [])
    {
        $this->addCampo('radio', $name, $label, $value, $attribs, $opcoes);
        return $this;
    }

    public function addRadioOutros($name, $label = null, $value = null, $opcoes = [], $attribs = [], $attribsOutros = [])
    {
        if (!in_array('O', $opcoes)) {
            $opcoes = array_merge($opcoes, ['Outros'=>'Outros']);
        }
        $id = $this->getCampoId($name);
        $idOutros = $this->getCampoId('outros_'.$name);
        $this->addRadio($name, $label, $value, $opcoes, $attribs);
        $this->addInput('outros_'.$name, 'Outros', null, $attribsOutros, ['visible' => false]);
        $script = <<<SCRIPT
            $(document).ready(function(){
                $('input[id^={$id}]').parent().on('click', function(){
                    if($(this).find('input').val() === 'Outros'){
                       $('#{$idOutros}').parent("div").parent(".form-group").removeClass('hidden');
                    } else {
                       $('#{$idOutros}').parent("div").parent(".form-group").addClass('hidden');
                       $('#{$idOutros}').val('');
                    }
                });
            });
SCRIPT;
        //ver($this);
        $this->addScript($script);
        return $this;
    }

    /**
     * Função que adiciona campos 'checkbox' no formulário.
     *
     * @param $name
     * @param null $label
     * @param null $value
     * @param array $opcoes - campos adicionados. Ex: ['A' => 'Opção A', 'B' => 'Opção B', 'C' => 'Opção C']
     * @param array $attribs
     * @return $this
     */
    public function addCheckbox($name, $label = null, $value = null, $opcoes = [], $attribs = [])
    {
        $this->addCampo('checkbox', $name, $label, $value, $attribs, $opcoes);
        return $this;
    }

    /**
     * Função que adiciona campos 'file' no formulário
     *
     * @param $name
     * @param $titulo
     * @param null $accept
     * @param array $attribs
     * @return $this
     */
    public function addFile($name, $titulo, $accept = null, $attribs = [])
    {
        $id = $this->getCampoId($name);
        $dados = $this->dados;
        $html = <<<HTML
            <div class="form-group">
                <label for="{$name}" class="col-sm-3 col-md-3 col-lg-3 control-label">{$titulo}</label>
                <div class="col-sm-9 col-md-9 col-lg-9">
HTML;

        if (!isset($dados[$name])) {
            $html .= <<<HTML
                <input
                    type="file"
                    class="btn btn-success nao_confirma"
                    accept="application/pdf"
                    id="{$id}"
                    name="{$name}"
                    title="{$titulo}"
                >
HTML;
        } else {
            $file = new FilesSimec();
            $arquivo = $file->getNomeArquivo($dados[$name]);

            $html .= <<<HTML
                <button
                    type="button"
                    class="btn btn-success"
                    id="btn-{$name}-download"
                    title="Download do arquivo {$arquivo}"
                >
                    <i class="fa fa-download" aria-hidden="true"></i>&nbsp;{$arquivo}
                </button>
                <button
                    type="button"
                    class="btn btn-danger"
                    id="btn-{$name}-delete"
                    title="Apaga o arquivo {$arquivo}"
                >
                    <i class="fa fa-trash-o" aria-hidden="true"></i>
                </button>
HTML;
        }
        $html .= <<<HTML
                     </div>
            </div>
HTML;
        $this->addCampoPersonalizado($name, $html);
        return $this;
    }

    /**
     * Função que adiciona campos 'hidden' no formulário
     *
     * @param $name
     * @param null $value
     * @param array $attribs
     * @return $this
     */
    public function addHidden($name, $value = null, $attribs = [])
    {
        $id = $this->getCampoId($name);
        $valor = "";
        $atributos = $this->trataAtributos($attribs);
        if (!empty($dados[$name])) {
            $valor = $dados[$name];
        } else {
            if (!empty($value)) {
                $valor =  $value;
            }
        }

        $html = <<<HTML
            <input type="hidden" name="{$name}" id="{$id}" value="{$valor}" {$atributos}>
HTML;
        $this->addCampoPersonalizado($name, $html);
        return $this;
    }

    public function trataParametros($params = [])
    {
        $parametros = "";
        if (!empty($params)) {
            foreach ($params as $param) {
                $parametros .= !empty($parametros) ? ', ' : '';
                if (is_string($param)) {
                    $parametros .= "'{$param}'";
                } else {
                    $parametros .= $param;
                }
            }
        }
        return $parametros;
    }

    /**
     * Função que adiciona botões ao formulário
     *
     * @param $classe
     * @param $value
     * @param string $type
     * @param null $func
     * @param array $params
     */
    public function addBotao($classe, $value, $type = 'button', $func = null, $params = [])
    {
        $funcao = "";
        $parametros = "";
        if (!empty($func)) {
            if (!empty($params)) {
                $parametros = $this->trataParametros($params);
            }
            $funcao = <<<HTML
                onclick="{$func}({$parametros})"
HTML;
        }
        $this->botoes[] = <<<HTML
            <input type="{$type}" class="btn {$classe}" value="{$value}" {$funcao}>
HTML;
    }

    /**
     * Adiciona um botão com a classe btn-success ao formulário
     *
     * @param $value
     * @param null $func
     * @param array $params
     * @param string $type
     */
    public function addBotaoSuccess($value, $func = null, $params = [], $type = 'button')
    {
        $this->addBotao('btn-success', $value, $type, $func, $params);
    }

    /**
     * Adiciona um botão com a classe btn-warning ao formulário
     *
     * @param $value
     * @param null $func
     * @param array $params
     * @param string $type
     */
    public function addBotaoWarning($value, $func = null, $params = [], $type = 'button')
    {
        $this->addBotao('btn-warning', $value, $type, $func, $params);
    }

    /**
     * Adiciona um botão com a classe btn-danger ao formulário
     *
     * @param $value
     * @param null $func
     * @param array $params
     * @param string $type
     */
    public function addBotaoDanger($value, $func = null, $params = [], $type = 'button')
    {
        $this->addBotao('btn-danger', $value, $type, $func, $params);
    }

    /**
     * Adiciona um botão com a classe btn-info ao formulário
     *
     * @param $value
     * @param null $func
     * @param array $params
     * @param string $type
     */
    public function addBotaoInfo($value, $func = null, $params = [], $type = 'button')
    {
        $this->addBotao('btn-info', $value, $type, $func, $params);
    }


    /**
     * Adiciona um botão com a classe btn-default ao formulário
     *
     * @param $value
     * @param null $func
     * @param array $params
     * @param string $type
     */
    public function addBotaoDefault($value, $func = null, $params = [], $type = 'button')
    {
        $this->addBotao('btn-default', $value, $type, $func, $params);
    }

    /**
     * Adiciona um botão 'reset' ao formulário
     *
     * @param $value
     */
    public function addBotaoReset($value)
    {
        $this->addBotao('btn-default', $value, 'reset', 'resetar', ["{$this->id}"]);
    }

    /**
     * Trata os botões, montando o html a partir do array de botões
     *
     * @return string
     */
    public function trataBotoes()
    {
        $botoes = "";
        if (!empty($this->botoes)) {
            $botoes = implode(" ", $this->botoes);
        } else {
            $botoes = <<<HTML
                <input type="submit" class="btn btn-success" value="Salvar">
HTML;
        }
        return $botoes;
    }

    /**
     * Função que trata os campos e retorna o HTML completo dos mesmos.
     *
     * @return string
     */
    public function trataCampos()
    {
        $helper = new Simec_View_Helper();
        $dados = $this->dados;
        $campos = "";
        foreach ($this->campos as $campo) {
            if (!isset($campo['html'])) {
                switch ($campo['func']) {
                    case 'select':
                    case 'radio':
                    case 'checkbox':
                    $campos .= $helper->$campo['func']($campo['param']['name'], $campo['param']['label'], $dados[$campo['param']['name']], $campo['param']['opcoes'], $campo['param']['attribs']);
                    break;
                    default:
                    $campos .= $helper->$campo['func']($campo['param']['name'], $campo['param']['label'], $dados[$campo['param']['name']], $campo['param']['attribs'], $campo['param']['opcoes']);
                    break;
                }
            } else {
                $campos .= $campo['html'];
                $campos .= <<<HTML
                <script >
                    $('#{$campo['name']}').val('{$dados[$campo['name']]}')
                </script>
HTML;
            }
        }
        return $campos;
    }

    /**
     * Renderiza o formulário.
     *
     * @return $this
     */
    public function render()
    {
        $helper = new Simec_View_Helper();
        $html = "";

        $campos = $this->trataCampos();
        $botoes = $this->trataBotoes();

        // verifica se o botão de limpar foi inserido
        foreach ($this->botoes as $botao) {
            if (strpos($botao, 'type="reset"')) {
                $limpar = true;
            }
        }

        // adiciona script para o botão limpar
        if ($limpar) {
            $script = <<<HTML
	/**
	 * Função responsável por limpar os campos
	 * @param string form Nome do formulário a ser limpo
	 */
	function resetar(form) {
		let \$form = $('#'+form);

		$(":text", \$form).each(function () {
			$(this).val("");
		});

		$(":radio", \$form).each(function () {
			$(this).prop({ checked: false }).parent().removeClass('active')
		});

		$("select", \$form).each(function () {
			$(this).val("").trigger('chosen:updated');
		});

		$(':checkbox', \$form).each(function() {
			$(this).attr('checked', false).parent().removeClass('active')
		});
	}
HTML;
            $this->addScript($script);
        }

        if ($this->formOff) {
            echo $campos;
            echo $this->renderScript();
            return $this;
        }

        $html .= <<<HTML
            <form name="{$this->id}" id="{$this->id}" action="{$this->action}" method="{$this->method}" class="form form-horizontal">
                <input type="hidden" name="acao" id="acao" value="{$this->acao}">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>{$this->titulo}</h5>
                    </div>

                    <div class="ibox-content">
                        {$campos}
                    </div>

                    <div class="ibox-footer">
                        {$botoes}
                    </div>
                </div>
            </form>
HTML;

        echo $html;
        echo $this->renderScript();
        return $this;
    }
}
