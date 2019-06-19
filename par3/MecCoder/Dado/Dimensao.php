<?php
namespace Simec\Par3\Dado;

/**
 * Estrutura de dados referente à: par3.dimensao
 * @description   
 * @tabela par3.dimensao
 */
class Dimensao extends \Simec\AbstractDado
{

    /**
     * chave primária da dimensão
     * @campo dimid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $dimid;

    /**
     * Código da dimensão
     * @campo dimcod
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $dimcod;

    /**
     * Descrição da dimensão
     * @campo dimdsc
     * @tipo texto
     * @obrigatorio true
     * @tamanho 500
     * @validador
     * @var string
     */
    public $dimdsc;

    /**
     * Status da dimensão
     * @campo dimstatus
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $dimstatus;

    /**
     * FK da tabela intrumento
     * @campo itrid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $itrid;


}