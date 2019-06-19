<?php

class Seguranca_Model_AcessoRapido_Perfil extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.acessorapido_perfil';
    
    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'arpid'
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
        'arpid' => null,
        'pflcod' => null,
        'acrid' => null
    );
    
    public function pegarPflcodPorAcrid($acrid)
    {       
        $sql = "SELECT
                    pflcod
                FROM
                    seguranca.acessorapido_perfil 
                WHERE
                    acrid = {$acrid}";
        $dados = $this->carregarColuna($sql);        
    
        return ($dados ? $dados : array());
    }
    
    public function apagarPorAcrid($acrid)
    {
        $sql = "DELETE FROM seguranca.acessorapido_perfil WHERE acrid = {$acrid}";
        $this->executar($sql);
    }
}
