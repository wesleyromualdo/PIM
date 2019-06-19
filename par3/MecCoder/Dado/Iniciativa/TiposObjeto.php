<?php
namespace Simec\Par3\Dado\Iniciativa;

/**
 * Estrutura de dados referente à: par3.iniciativa_tipos_objeto
 * @description   
 * @tabela par3.iniciativa_tipos_objeto
 */
class TiposObjeto extends \Simec\AbstractDado
{

    /**
     * 
     * @campo intoid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $intoid;

    /**
     * 
     * @campo intocpfinativacao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $intocpfinativacao;

    /**
     * 
     * @campo intocpfinclusao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $intocpfinclusao;

    /**
     * 
     * @campo intodsc
     * @tipo texto
     * @obrigatorio true
     * @tamanho 255
     * @validador
     * @var string
     */
    public $intodsc;

    /**
     * 
     * @campo intodtinativacao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $intodtinativacao;

    /**
     * 
     * @campo intodtinclusao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $intodtinclusao;

    /**
     * 
     * @campo intosituacao
     * @tipo texto
     * @obrigatorio true
     * @tamanho 1
     * @validador
     * @var string
     */
    public $intosituacao;

    /**
     * 
     * @campo intostatus
     * @tipo texto
     * @obrigatorio true
     * @tamanho 1
     * @validador
     * @var string
     */
    public $intostatus;


}