<?php
namespace Simec\Exemplo\Dado;

/**
 * Estrutura de dados referente �: exemplo.dependente
 * @description Dependentes das Pessoas de exemplo  
 * @tabela exemplo.dependente
 */
class Dependente extends \Simec\AbstractDado
{

    /**
     * Chave da identifica��o do dependente
     * @nome 
     * @nomeAbreviado
     * @campo id
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $id;

    /**
     * Nome do dependente
     * @nome 
     * @nomeAbreviado
     * @campo nome
     * @tipo texto
     * @tamanho 100
     * @validador
     * @var string
     */
    public $nome;

    /**
     * Peso em kilos do dependente
     * @nome 
     * @nomeAbreviado
     * @campo peso
     * @tipo numerico
     * @tamanho 3
     * @validador
     * @var string
     */
    public $peso;

    /**
     * Refer�ncia da pessoa respons�vel
     * @nome 
     * @nomeAbreviado
     * @campo pessoa_id
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $idPessoa;


}