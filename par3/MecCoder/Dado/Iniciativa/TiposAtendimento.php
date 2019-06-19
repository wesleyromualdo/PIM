<?php
namespace Simec\Par3\Dado\Iniciativa;

/**
 * Estrutura de dados referente à: par3.iniciativa_tipos_atendimento
 * @description   
 * @tabela par3.iniciativa_tipos_atendimento
 */
class TiposAtendimento extends \Simec\AbstractDado
{

    /**
     * 
     * @campo intaid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $intaid;

    /**
     * 
     * @campo intacpfinativacao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $intacpfinativacao;

    /**
     * 
     * @campo intacpfinclusao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $intacpfinclusao;

    /**
     * 
     * @campo intadsc
     * @tipo texto
     * @obrigatorio true
     * @tamanho 255
     * @validador
     * @var string
     */
    public $intadsc;

    /**
     * 
     * @campo intadtinativacao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $intadtinativacao;

    /**
     * 
     * @campo intadtinclusao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $intadtinclusao;

    /**
     * 
     * @campo intasituacao
     * @tipo texto
     * @obrigatorio true
     * @tamanho 1
     * @validador
     * @var string
     */
    public $intasituacao;

    /**
     * 
     * @campo intastatus
     * @tipo texto
     * @obrigatorio true
     * @tamanho 1
     * @validador
     * @var string
     */
    public $intastatus;


}