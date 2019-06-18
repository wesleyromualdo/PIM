<?php
namespace Simec\Par3\Dado\Iniciativa\Planejamento;

/**
 * Estrutura de dados referente à: par3.iniciativa_planejamento
 * @description   
 * @tabela par3.iniciativa_planejamento
 */
class Planejamento extends \Simec\AbstractDado
{

    /**
     * 
     * @campo inpid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inpid;

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
     * @campo inpcronogramaanoinicial
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inpcronogramaanoinicial;

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
     * @campo inpvalor_planejado_total
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inpvalor_planejado_total;

    /**
     * Obras do PROINFANCIA, próprias do par3
     * @campo obrid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $obrid;

    /**
     * 
     * @campo obrid_par3
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $obrid_par3;


}