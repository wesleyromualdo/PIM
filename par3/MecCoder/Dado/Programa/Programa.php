<?php
namespace Simec\Par3\Dado\Programa;

/**
 * Estrutura de dados referente à: par3.programa
 * @description Programa relacioando a portaria  
 * @tabela par3.programa
 */
class Programa extends \Simec\AbstractDado
{

    /**
     * 
     * @campo prgid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $prgid;

    /**
     * 
     * @campo arqid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $arqid;

    /**
     * 
     * @campo pgoid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $pgoid;

    /**
     * 
     * @campo prgabreviatura
     * @tipo texto
     * @tamanho 100
     * @validador
     * @var string
     */
    public $prgabreviatura;

    /**
     * 
     * @campo prganoreferencia
     * @tipo texto
     * @tamanho 4
     * @validador
     * @var string
     */
    public $prganoreferencia;

    /**
     * 
     * @campo prgarquivo
     * @tipo texto
     * @obrigatorio true
     * @tamanho 100
     * @validador
     * @var string
     */
    public $prgarquivo;

    /**
     * 
     * @campo prgatd_extraordinario
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $prgatd_extraordinario;

    /**
     * 
     * @campo prgcpfinativacao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $prgcpfinativacao;

    /**
     * 
     * @campo prgcpfinclusao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $prgcpfinclusao;

    /**
     * 
     * @campo prgdsc
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $prgdsc;

    /**
     * 
     * @campo prgdtinativacao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $prgdtinativacao;

    /**
     * 
     * @campo prgdtinclusao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $prgdtinclusao;

    /**
     * 
     * @campo prgesfera
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $prgesfera;

    /**
     * 
     * @campo prglink
     * @tipo texto
     * @tamanho 100
     * @validador
     * @var string
     */
    public $prglink;

    /**
     * 
     * @campo prgperiodofim
     * @tipo data
     * @tamanho 4
     * @validador
     * @var string
     */
    public $prgperiodofim;

    /**
     * 
     * @campo prgperiodoinicio
     * @tipo data
     * @tamanho 4
     * @validador
     * @var string
     */
    public $prgperiodoinicio;

    /**
     * O campo informa se o programa vai ser restrito a estado e municipio selecionados/escolhidos valores S para sim N para não
     * @campo prgrestrito
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $prgrestrito;

    /**
     * 
     * @campo prgresumo
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $prgresumo;

    /**
     * 
     * @campo prgsituacao
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $prgsituacao;

    /**
     * 
     * @campo prgstatus
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $prgstatus;

    /**
     * 
     * @campo prgtipoprograma
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $prgtipoprograma;


}