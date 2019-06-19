<?php
namespace Simec\Par3\Dado\Ensino;

/**
 * Estrutura de dados referente à: par3.ensino_etapa
 * @description   
 * @tabela par3.ensino_etapa
 */
class Etapa extends \Simec\AbstractDado
{

    /**
     * Chave Primária
     * @campo etaid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $etaid;

    /**
     * Chave estrangeira que faz referência a tabela par3.ensino_nivel
     * @campo nivid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $nivid;

    /**
     * Descrição da etapa do nível de Ensino
     * @campo etadsc
     * @tipo texto
     * @tamanho 255
     * @validador
     * @var string
     */
    public $etadsc;


}