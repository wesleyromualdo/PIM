<?php
namespace Simec\Par3\Dado\Iniciativa\Planejamento;

/**
 * Estrutura de dados referente à: par3.iniciativa_planejamento_desdobramentos
 * @description   
 * @tabela par3.iniciativa_planejamento_desdobramentos
 */
class Desdobramentos extends \Simec\AbstractDado
{

    /**
     * 
     * @campo ipdid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $ipdid;

    /**
     * 
     * @campo desid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $desid;

    /**
     * 
     * @campo inpid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inpid;

    /**
     * 
     * @campo ipddtinclusao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $ipddtinclusao;


}