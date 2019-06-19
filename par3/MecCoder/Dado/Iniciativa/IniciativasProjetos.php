<?php
namespace Simec\Par3\Dado\Iniciativa;

/**
 * Estrutura de dados referente à: par3.iniciativa_iniciativas_projetos
 * @description   
 * @tabela par3.iniciativa_iniciativas_projetos
 */
class IniciativasProjetos extends \Simec\AbstractDado
{

    /**
     * Chave Primária
     * @campo iiproid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iiproid;

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
     * FK do código do projeto (par3.projeto)
     * @campo proid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $proid;

    /**
     * Status
     * @campo iiprostatus
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $iiprostatus;


}