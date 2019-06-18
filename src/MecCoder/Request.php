<?php
namespace MecCoder;

/**
 * Classe definidora da requisição
 */
class Request
{

    /**
     * Conteúdo da requisição $_GET
     * @var array
     */
    protected $get;

    /**
     * Conteúdo da requisição $_POST
     * @var array
     */
    protected $post;

    /**
     * Conteúdo da requisição $_FILES
     * @var array
     */
    protected $file;

    /**
     * Método inicializador da requisição
     * - Define os dados que foram requisitados na tela $_GET, $_POST e demais
     */
    public function __construct()
    {
        $this->post = $_POST ? $_POST : [];
        $this->get = $_GET ? $_GET : [];
        $this->files = $_FILES ? $_FILES : [];
    }

    /**
     * Retorna dados da postagem recebida
     * @param string $var nome da variável
     * @return mixed
     */
    public function post($var = null)
    {
        if ($var !== null) {
            if(isset($_POST[$var])){
                return $_POST[$var];
            }
            return false;
        }
        return $_POST;
    }

    /**
     * Retorna dados da url requisitada
     * @param string $var nome da variável
     * @return mixed
     */
    public function get($var = null)
    {
        if ($var !== null) {
            if(isset($_GET[$var])) {
                return $_GET[$var];
            }
            return false;
        }
        return $_GET;
    }

    /**
     * Retorna dados dos arquivos recebidos
     * @param string $var nome da variável
     * @return array
     */
    public function files($var = null)
    {
        if ($var !== null) {
            if(isset($_FILES[$var])) {
                return $_FILES[$var];
            }
            return false;
        }
        return $_FILES;
    }
    
    /**
     * Retorna se a requisição foi executada via ajax
     * @return boolean
     */
    public static function isAjax()
    {
        return isset($_SERVER['HTTP_X_REQUESTED_WITH']) && $_SERVER['HTTP_X_REQUESTED_WITH'] === 'XMLHttpRequest';
    }
    
    /**
     * Retorna se a requisição foi executada via GET
     * @return boolean
     */
    public static function isGet(){
        return $_SERVER['REQUEST_METHOD'] === 'GET';
    }
    
    /**
     * Retorna se a requisição foi executada via POST
     * @return boolean
     */
    public static function isPost(){
        return $_SERVER['REQUEST_METHOD'] === 'POST';
    }
    
}
