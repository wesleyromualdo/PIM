<?php
namespace Simec\Par3\Dado\Iniciativa\Areas;

/**
 * Estrutura de dados referente à: par3.iniciativa_areas
 * @description   
 * @tabela par3.iniciativa_areas
 */
class Areas extends \Simec\AbstractDado
{

    /**
     * 
     * @campo iarid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iarid;

    /**
     * 
     * @campo iarid_pai
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iarid_pai;

    /**
     * 
     * @campo iuoid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iuoid;

    /**
     * 
     * @campo co_unidade_org
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $co_unidade_org;

    /**
     * 
     * @campo iardsc
     * @tipo texto
     * @obrigatorio true
     * @tamanho 500
     * @validador
     * @var string
     */
    public $iardsc;

    /**
     * 
     * @campo iarsigla
     * @tipo texto
     * @obrigatorio true
     * @tamanho 255
     * @validador
     * @var string
     */
    public $iarsigla;

    /**
     * 
     * @campo iarsituacao
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $iarsituacao;

    /**
     * 
     * @campo iarstatus
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $iarstatus;

    /**
     * 
     * @campo sg_unidade_org
     * @tipo texto
     * @tamanho 100
     * @validador
     * @var string
     */
    public $sg_unidade_org;


}