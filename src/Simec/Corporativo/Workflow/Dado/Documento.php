<?php
namespace Simec\Corporativo\Workflow\Dado;

/**
 * Estrutura de dados referente à: workflow.documento
 * @description
 * @tabela workflow.documento
 */
class Documento extends \Simec\AbstractDado
{
    
    /**
     *
     * @campo docid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $docid;
    
    /**
     *
     * @campo esdid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $esdid;
    
    /**
     *
     * @campo tpdid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $tpdid;
    
    /**
     *
     * @campo docdatainclusao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $docdatainclusao;
    
    /**
     *
     * @campo docdsc
     * @tipo texto
     * @tamanho 500
     * @validador
     * @var string
     */
    public $docdsc;
    
    /**
     *
     * @campo hstid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $hstid;
    
    /**
     *
     * @campo unicod
     * @tipo texto
     * @tamanho 5
     * @validador
     * @var string
     */
    public $unicod;
    
    
}