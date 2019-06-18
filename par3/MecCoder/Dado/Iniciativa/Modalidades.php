<?php
namespace Simec\Par3\Dado\Iniciativa;

/**
 * Estrutura de dados referente à: par3.iniciativa_modalidades
 * @description Tabela utilizada para cadastro da Iniciativa Detalhe - Modalidades (3° Aba).
Story #11025  
 * @tabela par3.iniciativa_modalidades
 */
class Modalidades extends \Simec\AbstractDado
{

    /**
     * Chave Primária
     * @campo imoid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $imoid;

    /**
     * FK da Desdobramento (par3.iniciativa_desdobramento)
     * @campo desid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $desid;

    /**
     * FK da Etapa (par3.ensino_etapa)
     * @campo etaid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $etaid;

    /**
     * FK da Iniciativa (par3.iniciativa)
     * @campo iniid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $iniid;

    /**
     * FK da Modalidade (par3.modalidade)
     * @campo modid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $modid;

    /**
     * FK da Nível (par3.ensino_nivel)
     * @campo nivid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $nivid;

    /**
     * Status do registro
     * @campo imostatus
     * @tipo texto
     * @obrigatorio true
     * @tamanho 1
     * @validador
     * @var string
     */
    public $imostatus;


}