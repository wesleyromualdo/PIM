<?php
/**
 * Classe de mapeamento da entidade ide2016.tab08s_uf
 *
 * @category Class
 * @package  A1
 * @author   FERNANDO DE SOUZA AIRES <fernando.aires@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-02-2016
 * @link     no link
 */



/**
 * Ide2016__ModelTab08s_uf
 *
 * @category Class
 * @package  A1
 * @author   FERNANDO DE SOUZA AIRES <fernando.aires@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 12-02-2016
 * @link     no link
 */
class Par3_Model_Ide_Tab08sUf extends Modelo
{
	
	protected $tabelaArvore;
	protected $titulo;
	protected $dados;
	
	public function __construct($id = null)
	{
		parent::__construct();
		$this->tabelaArvore = new HTML_Table('table_tabela08s_uf', 'table table-condensed table-bordered  table-responsive');
		$this->dados = array();
	}	
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'ide2016.tab08s_uf';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'co_rede' => null,
        'co_uf' => null,
        'nu_ano' => null,
        'nu_salas_existen' => null,
        'nu_salas_utiliza' => null,
        'data_atualizacao' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'co_rede' => array( 'allowEmpty' => true ),
            'co_uf' => array( 'allowEmpty' => true ),
            'nu_ano' => array( 'allowEmpty' => true ),
            'nu_salas_existen' => array( 'allowEmpty' => true ),
            'nu_salas_utiliza' => array( 'allowEmpty' => true ),
            'data_atualizacao' => array( 'allowEmpty' => true ),
        );
    }//end getCamposValidacao($dados)


    protected function getSql($co_rede, $uf)
    {
    	$sql = "SELECT 		nu_ano, nu_salas_existen, nu_salas_utiliza
		    	FROM 		ide2016.tab08s_uf tab08
		    	LEFT JOIN 	territorios.estado est ON est.estcod = tab08.co_uf::varchar(2)
			   	WHERE 		co_rede = {$co_rede} AND est.estuf = '{$uf}'";

    	return $sql;
    }    
    
    public function getTabela($co_rede, $estuf)
    {
    	$sql = $this->getSql($co_rede, $estuf);
    	$dados = $this->carregar($sql);
    
    	if (!empty($dados)) {
    		$this->setHeader($co_rede);
    		$this->organizarDados($dados);
    		$this->setBody();
    		echo $this->tabelaArvore->display();
    	} else {
    		echo '<div class="alert alert-warning">
                   Não possui resultados para esta pesquisa.
                </div>';
    	}
    }
    
    protected function setBody()
    {
    	$this->tabelaArvore->addTSection('tbody');
    	 
    	foreach ($this->dados as $indice => $dado) {
    
    		$this->tabelaArvore->addRow();
    		$this->tabelaArvore->addCell($indice, $class, 'data', array('class' => 'text-center', 'rowspan' => count($dado), 'style' => 'vertical-align:middle;'));
    
    		foreach ($dado as $ano => $valor) {
    			$this->tabelaArvore->addCell($ano, $class, 'data', array('class' => 'text-center', 'style' => 'vertical-align:middle;'));
    			$this->tabelaArvore->addCell($valor, $class, 'data', array('class' => 'text-center'));
    			$this->tabelaArvore->addRow();
    		}
    	}
    	 
    	$nota = "<p>Nota:</p>
                 <p>As informações de cada escola podem ser obtidas no sistema Data Escola Brasil, do Inep / MEC. <a href=\"http://www.dataescolabrasil.inep.gov.br/dataEscolaBrasil\" target=\"_blank\" >Acesse aqui</a>";
    	 
    	$this->tabelaArvore->addRow();
    	$this->tabelaArvore->addCell($nota, $class, 'data', array('class' => 'text-left', 'colspan' => '3', 'style' => 'vertical-align:middle;'));
    	 
    }
    
    protected function getColunaCabecalho($co_rede)
    {
    	 
    	if($co_rede == 1){
    		$texto = "Salas de aula da rede estadual no municipio";
    	} elseif ($co_rede == 2) {
    		$texto = "Salas de aula da rede municipal no municipio";
    	}
    
    	return array(
    			$texto => array('class' => 'text-center', 'colspan' => 3, 'style' => 'vertical-align:middle')
    	);
    }    
    
    
    protected function organizarDados($dados)
    {
    	 
    	foreach ($dados as $valor) {
    		$this->dados['Existentes'][$valor['nu_ano']] = $valor['nu_salas_existen'];
    		$this->dados['Utilizadas'][$valor['nu_ano']] = $valor['nu_salas_utiliza'];
    	}
    	 
    }
    
    protected function setHeader($co_rede)
    {
    
    	$camposDaTabela = $this->getColunaCabecalho($co_rede);
    	$this->tabelaArvore->addTSection('thead');
    	$class = 'thead';
    	$this->tabelaArvore->addRow('alert alert-info');
    
    	foreach ($camposDaTabela as $campo => $atributos) {
    		$this->tabelaArvore->addCell($campo, '', 'header', $atributos);
    	}
    
    	$this->tabelaArvore->addRow();
    }
    

}//end Class
?>