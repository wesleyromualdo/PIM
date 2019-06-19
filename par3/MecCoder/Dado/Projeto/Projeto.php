<?php
namespace Simec\Par3\Dado\Projeto;

/**
 * Estrutura de dados referente à: par3.projeto
 * @description   
 * @tabela par3.projeto
 */
class Projeto extends \Simec\AbstractDado
{

    /**
     * Chave Primária
     * @campo proid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $proid;

    /**
     * cpf do responsável pela inativação do projeto
     * @campo procpfinativacao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $procpfinativacao;

    /**
     * cpf do responsável pela inclusão do projeto
     * @campo procpfinclusao
     * @tipo texto
     * @tamanho 11
     * @validador
     * @var string
     */
    public $procpfinclusao;

    /**
     * Descrição do projeto
     * @campo prodsc
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $prodsc;

    /**
     * data de inativação do projeto
     * @campo prodtinativacao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $prodtinativacao;

    /**
     * data de inclusão do projeto
     * @campo prodtinclusao
     * @tipo data
     * @tamanho 8
     * @validador
     * @var string
     */
    public $prodtinclusao;

    /**
     * Link do projeto
     * @campo prolink
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $prolink;

    /**
     * resumo do projeto
     * @campo proresumo
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $proresumo;

    /**
     * sigla do projeto
     * @campo prosigla
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $prosigla;

    /**
     * situação do projeto: indidica se o projeto está ativo/inativo
     * @campo prosituacao
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $prosituacao;

    /**
     * status do projeto: indidica se o projeto foi removido
     * @campo prostatus
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $prostatus;


}