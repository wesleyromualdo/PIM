<?php
namespace Simec\Par3\Dado\Iniciativa;

/**
 * Estrutura de dados referente à: par3.iniciativa_iniciativas_areas
 * @description   
 * @tabela par3.iniciativa_iniciativas_areas
 */
class IniciativasAreas extends \Simec\AbstractDado
{

    /**
     * Chave Primária
     * @campo iiarid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iiarid;

    /**
     * FK do código da área (par3.iniciativa_areas)
     * @campo iarid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iarid;

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
     * Status
     * @campo iiarstatus
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $iiarstatus;


}