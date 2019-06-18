<?php
namespace Simec\Par3\Dado\Iniciativa\Desdobramento;

/**
 * Estrutura de dados referente à: par3.iniciativa_desdobramento
 * @description   
 * @tabela par3.iniciativa_desdobramento
 */
class Desdobramento extends \Simec\AbstractDado
{

    /**
     * 
     * @campo desid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $desid;

    /**
     * 
     * @campo tipid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $tipid;

    /**
     * 
     * @campo descpfinativacao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $descpfinativacao;

    /**
     * 
     * @campo descpfinclusao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $descpfinclusao;

    /**
     * 
     * @campo desdsc
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $desdsc;

    /**
     * 
     * @campo desdtinativacao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $desdtinativacao;

    /**
     * 
     * @campo desdtinclusao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $desdtinclusao;

    /**
     * 
     * @campo desimutavel
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $desimutavel;

    /**
     * 
     * @campo dessituacao
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $dessituacao;

    /**
     * 
     * @campo desstatus
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $desstatus;


}