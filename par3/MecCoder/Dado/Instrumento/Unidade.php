<?php
namespace Simec\Par3\Dado\Instrumento;

/**
 * Estrutura de dados referente à: par3.instrumentounidade
 * @description   
 * @tabela par3.instrumentounidade
 */
class Unidade extends \Simec\AbstractDado
{

    /**
     * chave primária do instrumento
     * @campo inuid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inuid;

    /**
     * se o instrumento for estado o campo tera a UF
     * @campo estuf
     * @tipo texto
     * @tamanho 2
     * @validador
     * @var string
     */
    public $estuf;

    /**
     * tipo de instrumento que ele pertence (estadual, municipal ou distrito federal)
     * @campo itrid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $itrid;

    /**
     * se o instrumento for município o campo tera o código do próprio
     * @campo muncod
     * @tipo texto
     * @tamanho 7
     * @validador
     * @var string
     */
    public $muncod;

    /**
     * 
     * @campo docid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $docid;

    /**
     * Nome do estado ou município
     * @campo inudescricao
     * @tipo texto
     * @tamanho 100
     * @validador
     * @var string
     */
    public $inudescricao;

    /**
     * 
     * @campo inusemconselhoeducacao
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inusemconselhoeducacao;

    /**
     * 
     * @campo inusiop
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $inusiop;

    /**
     * 
     * @campo inustatus
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $inustatus;


}