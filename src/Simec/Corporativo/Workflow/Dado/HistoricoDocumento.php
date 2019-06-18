<?php
namespace Simec\Corporativo\Workflow\Dado;

/**
 * Estrutura de dados referente à: workflow.historicodocumento
 * @description   
 * @tabela workflow.historicodocumento
 */
class HistoricoDocumento extends \Simec\AbstractDado
{

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo hstid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $hstid;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo aedid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $aedid;

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
     * @campo usucpf
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $usucpf;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo htddata
     * @tipo data
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $htddata;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo pflcod
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $pflcod;
}