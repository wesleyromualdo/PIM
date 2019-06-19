<?php
namespace Simec\Corporativo\Workflow\Dado;

/**
 * Estrutura de dados referente à: workflow.comentariodocumento
 * @description   
 * @tabela workflow.comentariodocumento
 */
class ComentarioDocumento extends \Simec\AbstractDado
{

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo cmdid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $cmdid;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo docid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $docid;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo cmddata
     * @tipo data
     * @tamanho 4
     * @validador
     * @var string
     */
    public $cmddata;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo cmddsc
     * @tipo texto
     * @tamanho -5
     * @validador
     * @var string
     */
    public $cmddsc;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo cmdstatus
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $cmdstatus;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo hstid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $hstid;

}