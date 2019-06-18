<?php

class Testes implements Iterator
{
    private $_testes;

    public function __construct()
    {
        $this->_testes = array();
    }

    public function addTeste(Teste $teste)
    {
        $this->_testes[] = $teste;
    }

    public function delTeste($pos)
    {
        if (isset($this->_testes[$pos])) {
            unset($this->_testes[$pos]);
        }
    }

    public function rewind()
    {
        reset($this->_testes);
    }

    public function current()
    {
        $var = current($this->_testes);
        return $var;
    }

    public function key()
    {
        $var = key($this->_testes);
        return $var;
    }

    public function next()
    {
        $var = next($this->_testes);
        return $var;
    }

    public function valid()
    {
        $var = $this->current() !== false;
        return $var;
    }

    public function resultado()
    {
        foreach ($this->_testes as $teste) {
            if (!$teste->status) {
                return false;
            }
        }

        return true;
    }
}

/**
 * Abstrai o conceito de teste atravs de nome, informação, status e msg de erro.
 *
 * @author Adonias Malosso <adonias@mesotec.com.br>
 * @example $t = new Teste("Permisso de escrita no diretrio", "/var/log", is_writeable("/var/log");
 * @version 1.0
 */
class Teste
{
    const IMGOK = "<img src='../imagens/valida1.gif' border='0' title='OK'/>";
    const IMGERRO = "<img src='../imagens/valida3.gif' border='0' title='Erro'/>";
    public $nome;
    public $info;
    public $status;
    public $msg;

    public function __construct($pnome, $pinfo, $pstatus, $pmsg = "")
    {
        $this->nome = $pnome;
        $this->info = $pinfo;
        $this->status = $pstatus;
        $this->msg = $pmsg;
    }

    public function getImagem()
    {
        switch ($this->status) {
            case 0:
                return self::IMGERRO;
                break;
            case 1:
                return self::IMGOK;
                break;
        }
    }
}
