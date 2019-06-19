<?php
namespace Simec\Par3\Dado\Iniciativa;

/**
 * Estrutura de dados referente à: par3.iniciativa
 * @description Representa uma classificação de um conjunto de bens e serviços - especificações - ofertados pelo Responsável com utilização dos recursos orçamentário para a criação do PTA  
 * @tabela par3.iniciativa
 */
class Iniciativa extends \Simec\AbstractDado
{

    /**
     * Chave Primária
     * @campo iniid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iniid;

    /**
     * FK do Ciclo (par3.ciclo_par)
     * @campo cicid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $cicid;

    /**
     * FK do Descrição Iniciativa (par3.iniciativa_descricao)
     * @campo indid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $indid;

    /**
     * FK do Tipo de Atendimento (par3.iniciativa_tipos_atendimento)
     * @campo intaid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $intaid;

    /**
     * FK do Tipo de Objeto (par3.iniciativa_tipos_objeto)
     * @campo intoid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $intoid;

    /**
     * Esfera da iniciativa. "M" => Municipal, "E" => Estadual, "T" => Todos
     * @campo iniesfera
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $iniesfera;

    /**
     * Diz se a iniciativa é específica para solicitação de mobiliário de obras do Proinfância.
     * @campo iniobraproinfancia
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iniobraproinfancia;

    /**
     * Situação da iniciativa
     * @campo inisituacao
     * @tipo "char"
     * @obrigatorio true
     * @tamanho 1
     * @validador
     * @var string
     */
    public $inisituacao;

    /**
     * Status da iniciativa
     * @campo inistatus
     * @tipo "char"
     * @tamanho 1
     * @validador
     * @var string
     */
    public $inistatus;

    /**
     * S ou N. Indica se possui todos os estados
     * @campo initodos_estuf
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $initodos_estuf;

    /**
     * S ou N. Indica se possui todos os municípios
     * @campo initodos_muncod
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $initodos_muncod;

    /**
     * 
     * @campo inivalidapendencia
     * @tipo texto
     * @tamanho 1
     * @validador
     * @var string
     */
    public $inivalidapendencia;


}