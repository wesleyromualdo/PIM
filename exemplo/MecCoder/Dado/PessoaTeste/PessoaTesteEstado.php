<?php
namespace Simec\Exemplo\Dado\PessoaTeste;

/**
 * Estrutura de dados referente à: exemplo.pessoa_teste_estado
 * @description   
 * @tabela exemplo.pessoa_teste_estado
 */
class PessoaTesteEstado extends \Simec\AbstractDado
{

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo pteid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $pteid;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo estuf
     * @tipo texto
     * @obrigatorio true
     * @tamanho 2
     * @validador
     * @var string
     */
    public $estuf;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo petid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $petid;


}