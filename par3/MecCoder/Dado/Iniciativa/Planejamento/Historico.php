<?php
namespace Simec\Par3\Dado\Iniciativa\Planejamento;

/**
 * Estrutura de dados referente à: par3.iniciativa_planejamento_historico
 * @description   
 * @tabela par3.iniciativa_planejamento_historico
 */
class Historico extends \Simec\AbstractDado
{

    /**
     * 
     * @campo hinid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $hinid;

    /**
     * 
     * @campo dimid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $dimid;

    /**
     * 
     * @campo docid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $docid;

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
     * @campo hinacao
     * @tipo texto
     * @tamanho 6
     * @validador
     * @var string
     */
    public $hinacao;

    /**
     * 
     * @campo hincpf
     * @tipo texto
     * @obrigatorio true
     * @tamanho 11
     * @validador
     * @var string
     */
    public $hincpf;

    /**
     * 
     * @campo hindtcriacao
     * @tipo data
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $hindtcriacao;

    /**
     * 
     * @campo iniid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iniid;

    /**
     * 
     * @campo inpcronogramaano
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inpcronogramaano;

    /**
     * 
     * @campo inpcronogramamesfinal
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inpcronogramamesfinal;

    /**
     * 
     * @campo inpcronogramamesinicial
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inpcronogramamesinicial;

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
     * @campo inpitemcomposicaodetalhamento
     * @tipo texto
     * @tamanho 10000
     * @validador
     * @var string
     */
    public $inpitemcomposicaodetalhamento;

    /**
     * 
     * @campo inpsituacaocadastro
     * @tipo texto
     * @obrigatorio true
     * @tamanho 1
     * @validador
     * @var string
     */
    public $inpsituacaocadastro;

    /**
     * 
     * @campo inpstatus
     * @tipo texto
     * @obrigatorio true
     * @tamanho 1
     * @validador
     * @var string
     */
    public $inpstatus;

    /**
     * 
     * @campo inuid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inuid;

    /**
     * 
     * @campo modid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $modid;

    /**
     * 
     * @campo nivid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $nivid;


}