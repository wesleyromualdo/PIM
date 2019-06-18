<?php
namespace Simec\Exemplo\Dado;

/**
 * Estrutura de dados referente à: exemplo.pessoa
 * @description   
 * @tabela exemplo.pessoa
 */
class Pessoa extends \Simec\AbstractDado
{

    /**
     * Chave da identificação da pessoa
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
     * Número do cadastro de pessoa física
     * @nome 
     * @nomeAbreviado
     * @campo cpf
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $cpf;

    /**
     * Data de nascimento da pessoa
     * @nome 
     * @nomeAbreviado
     * @campo nascimento
     * @tipo data
     * @tamanho 4
     * @validador
     * @var string
     */
    public $nascimento;

    /**
     * Sigla da UF de nascimento da pessoa
     * @nome 
     * @nomeAbreviado
     * @campo naturalidade
     * @tipo texto
     * @tamanho 2
     * @validador
     * @var string
     */
    public $naturalidade;

    /**
     * Nome da pessoa
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
     * Peso em kilos da pessoa
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
     * Telefone da pessoa
     * @nome 
     * @nomeAbreviado
     * @campo telefone
     * @tipo texto
     * @tamanho 20
     * @validador
     * @var string
     */
    public $telefone;

    public function enviarBanco()
    {
        $this->cpf = \Simec\Corporativo\Utilitario\Cpf::desmascarar($this->cpf);
        $this->nascimento = \Simec\Corporativo\Utilitario\Data::germanicoParaIso($this->nascimento);
        return $this;
    }

    public function enviarVisao()
    {
        $this->cpf = \Simec\Corporativo\Utilitario\Cpf::mascarar($this->cpf);
        $this->nascimento = \Simec\Corporativo\Utilitario\Data::isoParaGermanico($this->nascimento);
        return $this;
    }
}
