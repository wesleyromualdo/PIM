<?php
namespace Simec\Exemplo\Dado\PessoaTeste;

/**
 * Estrutura de dados referente à: exemplo.pessoa_teste
 * @description   
 * @tabela exemplo.pessoa_teste
 */
class PessoaTeste extends \Simec\AbstractDado
{

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo petid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $petid;

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
     * @campo petidade
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $petidade;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo petnome
     * @tipo texto
     * @obrigatorio true
     * @tamanho 100
     * @validador
     * @var string
     */
    public $petnome;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo petsexo
     * @tipo texto
     * @obrigatorio true
     * @tamanho 1
     * @validador
     * @var string
     */
    public $petsexo;

    /**
     * 
     * @nome 
     * @nomeAbreviado
     * @campo petstatus
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $petstatus;


}
