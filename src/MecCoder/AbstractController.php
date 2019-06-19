<?php
namespace MecCoder;

/**
 * Definição da classe de controle
 *
 * @author calixto
 */
abstract class AbstractController
{

    /**
     * Requisição do controle
     * @var Request
     */
    private $request;

    /**
     * Sessão ativa
     * @var Session
     */
    private $session;

    /**
     * Visualização
     * @var View
     */
    private $view;

    /**
     * Método construtor da classse
     */
    public final function __construct()
    {
        $this->request = new Request();
        $this->session = new Session();
        $this->view = new View();
        $reflection = new \ReflectionClass($this);
        foreach($reflection->getMethods() as $method){
            if($method->isProtected() && preg_match('/^initTrait(.+)/', $method->getName(),$matches)){
                $method->setAccessible (true);
                $method->invoke($this);
            }
        }
    }
    
    /**
     * Valida o uso do controle
     */
    public final function validateUse($method = null){
        
    }

    /**
     * Método inicial do controller chamado em todas as requisições
     */
    public abstract function init();

    /**
     * Método finalizador do controller chamado em todas as requisições
     */
    public abstract function finish();

    /**
     * Retorna um boleano informando se a requisição é ajax
     * @return boolean
     */
    protected function isAjax()
    {
        return Request::isAjax();
    }

    /**
     * Retorna dados da postagem recebida
     * @param string $var nome da variável
     * @return mixed
     */
    protected function post($var = null)
    {
        return $this->request->post($var);
    }

    /**
     * Retorna dados da url requisitada
     * @param string $var nome da variável
     * @return mixed
     */
    protected function get($var = null)
    {
        return $this->request->get($var);
    }

    /**
     * Retorna dados dos arquivos recebidos
     * @param string $var nome da variável
     * @return array
     */
    protected function files($var = null)
    {
        return $this->request->files($var);
    }

    /**
     * Retorna da sessão ou armazena dados na sessão
     * @param string $var nome da variável
     * @param mixed $val valor para armazenamento
     * @return mixed valor armazenado
     */
    protected function session($var, $val = null)
    {
        return $this->session->session($var, $val);
    }

    /**
     * Método de publicação de uma view
     * @global cls_banco $db
     * @param string $view
     */
    protected function showHtml($view = null)
    {
        return $this->view->showHtml($view);
    }

    /**
     * Método de publicação de uma partial
     * @global cls_banco $db
     * @param string $view
     */
    protected function showPartial($view = null)
    {
        return $this->view->showPartial($view);
    }


    /**
     * Armazena o valor para a apresentação na visualização
     * @param string $var nome da variável a ser utilizada da visualização
     * @param mixed $values valor da variável
     * @return $this
     */
    protected function toView($var, $values)
    {
        return $this->view->toView($var, $values);
    }

    /**
     * Armazena o valor para registrar no script da vizualização
     * @param string $var nome da variável a ser utilizada no namespace "_JS" do script
     * (Exemplo: a variável "bla" será acessível no script como "_JS.bla")
     * @param mixed $values valor da variável
     * @return $this
     */
    protected function toJs($var, $values)
    {
        return $this->view->toJs($var, $values);
    }

    /**
     * Armazena a url para registrar na visualização
     * @param string $urlPath
     * @return $this
     */
    protected function css($urlPath)
    {
        return $this->view->css($urlPath);
    }

    /**
     * Armazena a url para registrar na visualização
     * @param type $urlPath
     * @return $this
     */
    protected function js($urlPath)
    {
        return $this->view->js($urlPath);
    }

    /**
     * Método de publicação de um JSON
     * @param mixed $values
     */
    protected function showJson($values)
    {
        return $this->view->showJson($values);
    }

    /**
     * Método de publicação de um arquivo para Excel
     * @param resource/array $lines pilha de linhas para a impressão
     * @param string $name nome do arquivo de saída
     */
    protected function showExcel($lines, $name = null)
    {
        return $this->view->showExcel($lines, $name);
    }

    /**
     * Método de publicação de um arquivo para PDF
     * @param resource/array $lines pilha de linhas para a impressão
     * @param string $name nome do arquivo de saída
     */
    protected function showPdf($lines, $name = null)
    {
        return $this->view->showExcel($lines, $name);
    }
}
