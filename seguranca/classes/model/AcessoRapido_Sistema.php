<?php

class Seguranca_Model_AcessoRapido_Sistema extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.acessorapido_sistema';
    
    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'arsid'
    );
    
    /**
     * @var mixed[] Chaves estrangeiras.
     */
//     protected $arChaveEstrangeira = array(
//         'modid' => array('tabela' => 'modulo', 'pk' => 'modid'),
//     );
    
    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'arsid' => null,
        'sisid' => null,
        'acrid' => null
    );
    
    public function pegarSisidPorAcrid($acrid)
    {
        $sql = "SELECT
                    sisid
                FROM
                    seguranca.acessorapido_sistema
                WHERE
                    acrid = {$acrid}";
        $dados = $this->carregarColuna($sql);
        
        return ($dados ? $dados : array());
    }
    
    public function apagarPorAcrid($acrid)
    {
        $sql = "DELETE FROM seguranca.acessorapido_sistema WHERE acrid = {$acrid}";
        $this->executar($sql);
    }
}
