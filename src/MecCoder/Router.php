<?php
namespace MecCoder;

/**
 * Classe que define as rotas válidas
 */
class Router
{
    protected $route;
    protected $path;
    protected $file;
    protected $controller;
    protected $action;
    protected $type;

    public function __construct()
    {
        $this->validate();
    }

    public function getPath()
    {
        
    }

    public function getFile()
    {
        
    }

    public function getController()
    {
        
    }

    public function getAction()
    {
        
    }

    /**
     * Valida a rota requisitada
     */
    protected final function validate()
    {
        //throw new Exception('Rota inválida');
    }

    /**
     * Realiza a chamada da ação pela variável $_GET[$nmReq],
     * caso não exista, executa a chamada da primeira rota definida
     * - Este método deverá ser substituido ao se implantar o boot
     * @param \MecCoder\AbstractController $controller
     * @param type $routes
     */
    public function callAction(AbstractController $controller)
    {
        $action = \ucfirst(array_pop($this->getArrayFromRoute($this->route)));
//        $requisicao = function() use ($this) {
//            
//            $nmReq = 'requisicao';
//            if ($_GET[$nmReq]) {
//                if (is_array($_GET[$nmReq])) {
//                    foreach ($_GET[$nmReq] as $requisicao) {
//                        
//                    }
//                    return $requisicao;
//                } else {
//                    return $_GET[$nmReq];
//                }
//            } else {
//                return 'index';
//            }
//        };
//        $action = $action();
        $class = new \ReflectionClass($controller);
        if ($class->hasMethod($action) && $class->getMethod($action)->isPublic()) {
            $controller->{$action}();
        } else {
            die("a requisicao {$action} não é publica ou não está definida.");
        }
    }

    protected function getArrayFromRoute($route)
    {
        $arRoute = explode(((strpos($route, '\\') === false) ? '/' : '\\'), $route);
        return array_map(['\MecCoder\Util\String', 'dashesToUpperCamelCase'], $arRoute);
    }

    protected function getClassPath($route)
    {
        $arPath = $this->getArrayFromRoute($route);
        $arPath[0] = strtolower($arPath[0]);
        array_unshift($arPath, strtolower($arPath[0]));
        $arPath[1] = 'MecCoder';
        array_pop($arPath);
        return APPRAIZ . implode('/', $arPath) . '.php';
    }

    protected function getClassName($route)
    {
        $ar = $this->getArrayFromRoute($route);
        array_pop($ar);
        return '\Simec\\' . implode('\\', $ar);
    }

    public function has($route)
    {
        return is_file($this->getClassPath($route));
    }

    public function execute($route)
    {
        $this->route = $route;
        if ($this->has($route)) {
            $class = $this->getClassName($route);
            $controller = new $class();
            if ($controller instanceof AbstractController) {
                $controller->validateUse($route);
                $controller->init();
                $this->callAction($controller);
                $controller->finish();
            } else {
                throw new Exception\RouterException('Tipo inválido de controlador.');
            }
        }
    }

    protected function createController($class)
    {
        
    }
}
