<?php
namespace Simec\Par3\Dado\Iniciativa;

/**
 * Estrutura de dados referente à: par3.iniciativa_iniciativas_programas
 * @description   
 * @tabela par3.iniciativa_iniciativas_programas
 */
class IniciativasProgramas extends \Simec\AbstractDado
{

    /**
     * Chave Primária
     * @campo iiprgid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iiprgid;

    /**
     * FK do código da iniciativa (par3.iniciativa)
     * @campo iniid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iniid;

    /**
     * FK do código do programa (par3.programa)
     * @campo prgid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $prgid;

    /**
     * Status
     * @campo iiprgstatus
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $iiprgstatus;


}