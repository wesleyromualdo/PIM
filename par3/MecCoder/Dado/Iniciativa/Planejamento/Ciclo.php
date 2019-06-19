<?php
namespace Simec\Par3\Dado\Iniciativa\Planejamento;

/**
 * Estrutura de dados referente à: par3.ciclo_par
 * @description   
 * @tabela par3.ciclo_par
 */
class Ciclo extends \Simec\AbstractDado
{

    /**
     * 
     * @campo cicid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $cicid;

    /**
     * 
     * @campo cicanos
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $cicanos;

    /**
     * 
     * @campo ciccpfinativacao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $ciccpfinativacao;

    /**
     * 
     * @campo ciccpfinclusao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $ciccpfinclusao;

    /**
     * 
     * @campo cicdsc
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $cicdsc;

    /**
     * 
     * @campo cicdtfim
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $cicdtfim;

    /**
     * 
     * @campo cicdtinativacao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $cicdtinativacao;

    /**
     * 
     * @campo cicdtinclusao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $cicdtinclusao;

    /**
     * 
     * @campo cicdtinicio
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $cicdtinicio;

    /**
     * 
     * @campo cicduracao
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $cicduracao;

    /**
     * 
     * @campo cicsituacao
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $cicsituacao;

    /**
     * 
     * @campo cicstatus
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $cicstatus;

    /**
     * 
     * @campo cicvigencia
     * @tipo texto
     * @tamanho 12
     * @validador
     * @var string
     */
    public $cicvigencia;


}