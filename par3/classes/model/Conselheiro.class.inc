<?php
/**
 * Classe de mapeamento da entidade par3.cae_conselheiro.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade par3.cae_conselheiro.
 *
 * @see Modelo
 */
class Par3_Model_Conselheiro extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.cae_conselheiro';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'cacid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    	'caeid' => array('tabela' => 'par3.cae', 'pk' => 'caeid'),
    	'cctid' => array('tabela' => 'par3.cae_conselheiro_categoria', 'pk' => 'cctid'),
    	'cscid' => array('tabela' => 'par3.cae_conselheiro_subcategoria', 'pk' => 'cscid'),
    	'entid' => array('tabela' => 'par3.instrumentounidade_entidade', 'pk' => 'entid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'cacid' => null,
		'caeid' => null,
		'cctid' => null,
		'cscid' => null,
		'cacfuncao' => null,
		'cacmandato' => null,
		'cacocupacao' => null,
		'cacrenda' => null,
		'cacoutroconselho' => null,
		'cacregime' => null,
		'cacplanoacao' => null,
		'cacreuniao' => null,
		'caccapacitacao' => null,
		'cacorgaocapacitacao' => null,
		'cacorgaocapacitacaooutro' => null,
		'cacpopulacaoindigena' => null,
		'cacrepresentanteoindigena' => null,
    	'entid' => null,
    	'arqid' => null
    );
    

    /**
     * Função carregarConselheiroPorCPF
     * - retorna array de entidades do tipo conselheiro.
     *
     * @return array $arrDados.
     *
     */
    public function carregarConselheiroCPF($usucpf)
    {
    	$sql = "SELECT  *
		    	FROM 	par3.instrumentounidade_entidade
		    	WHERE 	entcpf = '{$usucpf}'
		    	AND	entstatus = 'A'
	    		AND	tenid IN (" . Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_ESTADUAL . "," . Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_MUNICIPAL . ")";
    
    	return $this->carregar($sql);
    }//end carregarConselheiroPorCPF()]
    
    
    /**
     * Função carregarConselheiros
     * - retorna array de entidades do tipo conselheiro.
     *
     * @return array $arrDados.
     *
     */
    public function carregarConselheiros($inuid)
    {
    	$sql = "SELECT  *
		    	FROM 	par3.instrumentounidade_entidade
		    	WHERE 	inuid = '{$inuid}'
		    	AND	entstatus = 'A'
		    	AND	tenid IN (" . self::CONSELHO_ESTADUAL . "," . self::CONSELHO_MUNICIPAL . ")";
    
    	return $this->carregar($sql);
    }//end carregarConselheiros()
    
    
    /**
     * Função existeConselheiro
     * - validar se o cpf já possui cadastro como conselheiro.
     *
     * @return void.
     *
     */
    public function existeConselheiro($usucpf, $inuid)
    {
    	$sql = "SELECT  entid
		    	FROM 	{$this->stNomeTabela}
		    	WHERE 	entcpf = '{$usucpf}'
		    	AND 	entstatus = 'A'
		    	AND 	inuid = '{$inuid}'
		    	AND	tenid IN (" . self::CONSELHO_ESTADUAL . "," . self::CONSELHO_MUNICIPAL . ")";
		    
    	return $this->carregar($sql);
    }//end existeConselheiro()
    
}//end Class
?>