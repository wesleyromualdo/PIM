<?php
namespace Simec\Corporativo\Usuario;

/**
 * Description of UsuarioSessao
 *
 * @author calixto
 */
class UsuarioSessao
{

    /**
     * Identificador do usuário
     * @var integer
     */
    public $id;

    /**
     * Nome do usuário
     * @var string
     */
    public $nome;

    /**
     * Email do usuário
     * @var string
     */
    public $email;

    /**
     * Número do cadastro de pessoa física
     * @var integer
     */
    public $cpf;

    public function __construct()
    {
        if (!isset($_SESSION['sisid']) || !$_SESSION['sisid']) {
            throw new \Simec\Exception('O Usuário não está logado no sistema.');
        }
        $this->id = $_SESSION['sisid'];
        $this->nome = $_SESSION['usunome'];
        $this->email = $_SESSION['usuemail'];
        $this->cpf = $_SESSION['usucpf'];
    }
}
