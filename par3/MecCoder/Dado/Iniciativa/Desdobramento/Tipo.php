<?php
namespace Simec\Par3\Dado\Iniciativa\Desdobramento;

/**
 * Estrutura de dados referente à: par3.iniciativa_desdobramento_tipo
 * @description   
 * @tabela par3.iniciativa_desdobramento_tipo
 */
class Tipo extends \Simec\AbstractDado
{

    /**
     * 
     * @campo tipid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $tipid;

    /**
     * 
     * @campo tipcpfinativacao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $tipcpfinativacao;

    /**
     * 
     * @campo tipcpfinclusao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $tipcpfinclusao;

    /**
     * 
     * @campo tipdsc
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $tipdsc;

    /**
     * 
     * @campo tipdtinativacao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $tipdtinativacao;

    /**
     * 
     * @campo tipdtinclusao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $tipdtinclusao;

    /**
     * 
     * @campo tipimutavel
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $tipimutavel;

    /**
     * 
     * @campo tipsituacao
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $tipsituacao;

    /**
     * 
     * @campo tipstatus
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $tipstatus;


}