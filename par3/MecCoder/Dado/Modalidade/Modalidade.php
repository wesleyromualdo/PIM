<?php
namespace Simec\Par3\Dado\Modalidade;

/**
 * Estrutura de dados referente à: par3.modalidade
 * @description   
 * @tabela par3.modalidade
 */
class Modalidade extends \Simec\AbstractDado
{

    /**
     * 
     * @campo modid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $modid;

    /**
     * 
     * @campo etaid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $etaid;

    /**
     * 
     * @campo nivid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $nivid;

    /**
     * 
     * @campo modcpfinativacao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $modcpfinativacao;

    /**
     * 
     * @campo modcpfinclusao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $modcpfinclusao;

    /**
     * 
     * @campo moddsc
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $moddsc;

    /**
     * 
     * @campo moddtinativacao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $moddtinativacao;

    /**
     * 
     * @campo moddtinclusao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $moddtinclusao;

    /**
     * 
     * @campo modsituacao
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $modsituacao;

    /**
     * 
     * @campo modstatus
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $modstatus;


}