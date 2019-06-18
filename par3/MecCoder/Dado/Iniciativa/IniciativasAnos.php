<?php
namespace Simec\Par3\Dado\Iniciativa;

/**
 * Estrutura de dados referente à: par3.iniciativa_iniciativas_anos
 * @description   
 * @tabela par3.iniciativa_iniciativas_anos
 */
class IniciativasAnos extends \Simec\AbstractDado
{

    /**
     * Chave Primária
     * @campo iiano
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iiano;

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
     * Status do ano
     * @campo iianostatus
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $iianostatus;

    /**
     * Ano da iniciativa
     * @campo iniano
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iniano;


}