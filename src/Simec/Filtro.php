<?php
namespace Simec;

class Filtro
{
    const igual = 1;
    const diferente = 2;
    const nulo = 3;
    const menorQue = 4;
    const menorOuIgual = 5;
    const maiorQue = 6;
    const maiorOuIgual = 7;
    const entre = 8;
    const dominio = 9;
    
    protected static $operadores = [
        1 => '=',
        2 => '<>',
        3 => 'is null',
        4 => '<',
        5 => '<=',
        6 => '>',
        7 => '>=',
        8 => 'between',
        8 => 'in',
    ];
    
    protected $conjuncao = 'and';
    protected $operador;
    protected $valor1;
    protected $valor2;
    
    public function conjuncao(){
        return $this->conjuncao;
    }
    
    public function operador(){
        return $this->operador;
    }
    
    public function simbolo(){
        return self::$operadores[$this->operador];
    }
    
    public function valor(){
        return $this->valor1;
    }
    
    public function valor1(){
        return $this->valor1;
    }
    
    public function valor2(){
        return $this->valor2;
    }

    /**
     * Retorna um filtro do operador igual
     * @param mixed $valor
     * @param string $conjuncao [and,or]
     * @return \Simec\Filtro
     */
    public static function igual($valor, $conjuncao = 'and')
    {
        $filtro = new Filtro();
        $filtro->operador = self::igual;
        $filtro->valor1 = $valor;
        $filtro->conjuncao = $conjuncao;
        return $filtro;
    }

    /**
     * Retorna um filtro do operador diferente
     * @param mixed $valor
     * @return \Simec\Filtro
     */
    public static function diferente($valor, $conjuncao = 'and')
    {
        $filtro = new Filtro();
        $filtro->operador = self::diferente;
        $filtro->valor1 = $valor;
        $filtro->conjuncao = $conjuncao;
        return $filtro;
    }

    /**
     * Retorna um filtro do operador nulo
     * @param mixed $valor
     * @return \Simec\Filtro
     */
    public static function nulo($valor, $conjuncao = 'and')
    {
        $filtro = new Filtro();
        $filtro->operador = self::nulo;
        $filtro->valor1 = $valor;
        $filtro->conjuncao = $conjuncao;
        return $filtro;
    }

    /**
     * Retorna um filtro do operador menor que o valor passado
     * @param mixed $valor
     * @return \Simec\Filtro
     */
    public static function menorQue($valor, $conjuncao = 'and')
    {
        $filtro = new Filtro();
        $filtro->operador = self::menorQue;
        $filtro->valor1 = $valor;
        $filtro->conjuncao = $conjuncao;
        return $filtro;
    }

    /**
     * Retorna um filtro do operador menor ou igual ao valor passado
     * @param mixed $valor
     * @return \Simec\Filtro
     */
    public static function menorOuIgual($valor, $conjuncao = 'and')
    {
        $filtro = new Filtro();
        $filtro->operador = self::menorOuIgual;
        $filtro->valor1 = $valor;
        $filtro->conjuncao = $conjuncao;
        return $filtro;
    }

    /**
     * Retorna um filtro do operador maior que o valor passado
     * @param mixed $valor
     * @return \Simec\Filtro
     */
    public static function maiorQue($valor, $conjuncao = 'and')
    {
        $filtro = new Filtro();
        $filtro->operador = self::maiorQue;
        $filtro->valor1 = $valor;
        $filtro->conjuncao = $conjuncao;
        return $filtro;
    }

    /**
     * Retorna um filtro do operador maior ou igual ao valor passado
     * @param mixed $valor
     * @return \Simec\Filtro
     */
    public static function maiorOuIgual($valor, $conjuncao = 'and')
    {
        $filtro = new Filtro();
        $filtro->operador = self::maiorOuIgual;
        $filtro->valor1 = $valor;
        $filtro->conjuncao = $conjuncao;
        return $filtro;
    }

    /**
     * Retorna um filtro do operador entre os valores passados
     * @param mixed $valor1
     * @param mixed $valor2
     * @return \Simec\Filtro
     */
    public static function entre($valor1, $valor2, $conjuncao = 'and')
    {
        $filtro = new Filtro();
        $filtro->operador = self::entre;
        $filtro->valor1 = $valor1;
        $filtro->valor2 = $valor2;
        $filtro->conjuncao = $conjuncao;
        return $filtro;
    }

    /**
     * Retorna um filtro do operador entre os valores passados
     * @param mixed $valores
     * @return \Simec\Filtro
     */
    public static function dominio(array $valores, $conjuncao = 'and')
    {
        $filtro = new Filtro();
        $filtro->operador = self::dominio;
        $filtro->valor1 = $valores;
        $filtro->conjuncao = $conjuncao;
        return $filtro;
    }
    
}
