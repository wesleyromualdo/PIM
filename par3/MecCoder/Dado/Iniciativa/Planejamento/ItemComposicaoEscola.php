<?php
namespace Simec\Par3\Dado\Iniciativa\Planejamento;

/**
 * Estrutura de dados referente à: par3.iniciativa_planejamento_item_composicao_escola
 * @description Tabela utilizada para cadastro dos Itens de composição das Iniciativas por escola  
 * @tabela par3.iniciativa_planejamento_item_composicao_escola
 */
class ItemComposicaoEscola extends \Simec\AbstractDado
{

    /**
     * Chave Primária
     * @campo ipeid
     * @tipo numerico
     * @chave true
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $ipeid;

    /**
     * FK iniciativa_planejamento_item_composicao
     * @campo ipiid
     * @tipo numerico
     * @obrigatorio true
     * @tamanho 8
     * @validador
     * @var string
     */
    public $ipiid;

    /**
     * 
     * @campo co_entidade
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $co_entidade;

    /**
     * 
     * @campo escid
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $escid;

    /**
     * Quantidade de itens
     * @campo ipequantidade
     * @tipo numerico
     * @tamanho 8
     * @validador
     * @var string
     */
    public $ipequantidade;

    /**
     * status A- ativo, I - inativo
     * @campo ipestatus
     * @tipo texto
     * @obrigatorio true
     * @tamanho 1
     * @validador
     * @var string
     */
    public $ipestatus;


}