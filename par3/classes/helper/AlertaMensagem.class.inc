<?php

/**
 * Class Par3_Helper_AlertaMensagem
 *
 * @category Class
 * @package  Helper
 * @author   <lucas.oliveira>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 *
 * @method addDanger
 * @method addWarning
 * @method addInfo
 * @method addSuccess
 */
class Par3_Helper_AlertaMensagem
{
    /**
     * Use para criar uma mensagem de sucesso. Balão verde.
     * Este é o tipo de mensagem padrão.
     */
    const SUCESSO = 'success';
    /**
     * Use para criar uma mensagem de informação. Balão azul.
     */
    const INFO = 'info';
    /**
     * Use para criar uma mensagem de aviso. Balão amarelo.
     */
    const AVISO = 'warning';
    /**
     * Use para criar uma mensagem de erro. Balão vermelho.
     */
    const ERRO = 'danger';

    /**
     * Titulo padrão da janela modal, caso nenhum exista
     */
    const TITULO_PADRAO = 'Alerta';

    /**
     * Titulo costumizado para a janela modal
     * @var string
     */
    protected $_modalTitulo;

    /**
     * Estrutura da janela modal
     * @var
     */
    protected $_templateModal;

    /**
     * Estrutura do aviso de mensagem
     * @var
     */
    protected $_templateAlert;

    /**
     * Fila de mensagens para modal
     * @var array
     */
    protected $_mensagens = [];

    /**
     * @var array
     */
    protected $_methodAllowed = ['addDanger', 'addWarning', 'addInfo', 'addSuccess', 'addMensagem'];

    /**
     * Inicializa objeto
     * @param string $modalTitulo
     */
    public function __construct($modalTitulo = self::TITULO_PADRAO)
    {
        $this->_modalTitulo = $modalTitulo;
        $this->initTemplateModal();
        $this->initTemplateAlert();
    }

    /**
     * Renderiza mensagens agrupadas
     * @return string
     */
    public function render()
    {
        if (count($this->_mensagens)) {
            $html = '';
            foreach ($this->_mensagens as $tupla) {
                extract($tupla);

                $extraButtons = '';
                if ($btnAcao instanceof Par3_Helper_BotaoAcao) {
                    $extraButtons .= $btnAcao->render();
                }

                $html .= sprintf($this->_getTemplateAlert(), $tipo, $titulo, $mensagem, $extraButtons);
            }

            return sprintf($this->_getTemplateModal(), $this->_modalTitulo, $html);
        }
    }

    /**
     * Alias para metodo de addMensagem
     *
     * @param $name
     * @param $arguments
     * @throws Exception
     */
    public function __call($name, $args)
    {
        if (!in_array($name, $this->_methodAllowed)) {
            throw new Exception('O metodo {'.$name.'} chamado não existe na classe');
        }

        $btnAcao = null;
        if (count($args) < 2) {
            throw new Exception('A funcão exige dois argumentos "título" e "mensagem"');
        } elseif (count($args) === 3) {
            $btnAcao = $args[2];
        }

        $this->addMensagem($args[0], $args[1], $this->getTypeAlert($name), $btnAcao);

        return $this;
    }

    /**
     * Adiciona uma nova mensagem ao modal
     *
     * @param $titulo
     * @param $mensagem
     * @param string $tipo
     * @return $this
     * @throws Exception
     */
    public function addMensagem($titulo, $mensagem, $tipo = self::INFO, Par3_Helper_BotaoAcao $btnAcao = null)
    {
        if (!strlen($titulo)) {
            throw new Exception('O titulo não pode ser vazio');
        }

        if (!strlen($mensagem)) {
            throw new Exception('A mensagem não pode ser vazio');
        }

        if (!in_array($tipo, [self::SUCESSO, self::INFO, self::AVISO, self::ERRO])) {
            throw new Exception('O tipo da mensagem escolhida é inválida');
        }

        array_push($this->_mensagens, [
            'titulo' => $titulo, 'mensagem' => $mensagem, 'tipo' => $tipo, 'btnAcao' => $btnAcao
        ]);

        return $this;
    }

    /**
     * Pega o tipo de mensagem adicionada
     *
     * @param $name
     * @return string
     */
    private function getTypeAlert($name)
    {
        switch ($name) {
            case 'addDanger':
                return self::ERRO;
                break;
            case 'addWarning':
                return self::AVISO;
                break;
            case 'addSuccess':
                return self::SUCESSO;
                break;
            case 'addInfo':
            case 'addMensagem':
            default:
                return self::INFO;
                break;
        }
    }

    /**
     * Recupera HTML da modal
     * @return mixed
     */
    protected function _getTemplateModal()
    {
        return (strlen($this->_templateModal)) ? $this->_templateModal : null;
    }

    /**
     * Recupera HTML do template
     * @return mixed
     */
    protected function _getTemplateAlert()
    {
        return (strlen($this->_templateAlert)) ? $this->_templateAlert : null;
    }

    /**
     * HTML do modal
     * @return void
     */
    private function initTemplateModal()
    {
        $this->_templateModal = <<<TPL
            <div class="modal fade" id="_modal-alerta" tabindex="-1" role="dialog">
                <div class="modal-dialog">
                    <div class="modal-content">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                                <span aria-hidden="true">&times;</span>
                            </button>
                            <h1 class="modal-title">%s</h1>
                        </div>
                        <div class="modal-body">
                            %s
                        </div>
                        <div class="modal-footer">
                            <div class="col-md-12 text-center">
                                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <script type="text/javascript">$(function(){ $("#_modal-alerta").modal({backdrop:"static"}); });</script>
TPL;
    }

    /**
     * HTML template do alerta
     * @return void
     */
    private function initTemplateAlert()
    {
        $this->_templateAlert = <<<TPL
            <div class="alert alert-%s alert-dismissible fade in" role="alert">
                <button type="button" class="close" data-dismiss="alert" aria-label="Close">
                    <span aria-hidden="true">×</span>
                </button>
                <h2>%s</h2>
                <p class="text-justify" style="font-size:14px;margin-bottom:20px;">%s</p>
                <p class="text-right">
                    %s
                </p>
            </div>
TPL;
    }
}

/**
 * Class Par3_Helper_addBotaoAcao
 * @category Class
 * @package  Helper
 * @author   <lucas.oliveira>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
Class Par3_Helper_BotaoAcao
{
    /**
     * @var array
     */
    protected $_btnAcao = [];

    /**
     * @param array $options
     *
     */
    public function __construct(array $options = [])
    {
        if (array_key_exists('label', $options) &&
            array_key_exists('func', $options) && array_key_exists('type', $options))
        {
            extract($options);
            $this->_btnAcao[] = ['label' => $label, 'func' => $func, 'type' => $type];
        }

        return $this;
    }

    /**
     * Checa se existe alguma configuração para montar botão
     * @return bool
     */
    private function getBtnAcao()
    {
        return (count($this->_btnAcao)) ? $this->_btnAcao : false;
    }

    /**
     * Retorna template HTML usado para construit o botão
     * @return string
     */
    private function getTemplateBotao()
    {
        return <<<TPL
            <button type="button" class="btn btn-%s" onClick="%s">%s</button>
TPL;
    }

    /**
     * Adiciona dados para compor o botão
     *
     * @param $label
     * @param $func
     * @param $type
     * @return $this
     */
    public function add($label, $func, $type)
    {
        $this->_btnAcao[] = ['label' => $label, 'func' => $func, 'type' => $type];
        return $this;
    }

    /**
     * Renderizza HTML do botão
     * @return string
     */
    public function render()
    {
        $output = '';
        if ($this->getBtnAcao()) {
            foreach ($this->getBtnAcao() as $groupBtn) {
                extract($groupBtn);
                $output .= sprintf($this->getTemplateBotao(), $type, "$func();", $label);
            }
        }
        return $output;
    }
}