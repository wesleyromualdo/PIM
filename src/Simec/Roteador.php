<?php
namespace Simec;

class Roteador extends \MecCoder\Router
{

    public function possuiRecurso($rota)
    {
        return $this->has($rota);
    }

    public function rotear($rota)
    {
        $this->execute($rota);
    }
}
