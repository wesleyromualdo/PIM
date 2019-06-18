<?php
/**
 * Classe de mapeamento da entidade ide2016.tab10_mu
 *
 * @category Class
 * @package  A1
 * @author   FERNANDO DE SOUZA AIRES <fernando.aires@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 11-02-2016
 * @link     no link
 */



/**
 * Ide2016__ModelTab10_mu
 *
 * @category Class
 * @package  A1
 * @author   FERNANDO DE SOUZA AIRES <fernando.aires@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 11-02-2016
 * @link     no link
 */
class Par3_Model_Ide_Tab10Mu extends Modelo
{
	
	protected $tabelaArvore;
	protected $titulo;
	protected $dados;
	
	public function __construct($id = null)
	{
		parent::__construct();
		$this->tabelaArvore = new HTML_Table('table_tabela10_uf', 'table table-condensed table-bordered  table-responsive');
		$this->dados = array();
	}	
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'ide2016.tab10_mu';

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
        'c_em' => null,
        'c_gr' => null,
        'c_lic' => null,
        'c_nm' => null,
        'co_ibge' => null,
        'co_modalidade_etapa' => null,
        'co_rede' => null,
        'nu_ano' => null,
        's_em' => null,
        'total' => null,
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
            'c_em' => array( 'allowEmpty' => true ),
            'c_gr' => array( 'allowEmpty' => true ),
            'c_lic' => array( 'allowEmpty' => true ),
            'c_nm' => array( 'allowEmpty' => true ),
            'co_ibge' => array( 'allowEmpty' => true ),
            'co_modalidade_etapa' => array( 'allowEmpty' => true ),
            'co_rede' => array( 'allowEmpty' => true ),
            'nu_ano' => array( 'allowEmpty' => true ),
            's_em' => array( 'allowEmpty' => true ),
            'total' => array( 'allowEmpty' => true ),
            'data_atualizacao' => array( 'allowEmpty' => true ),
        );
    }//end getCamposValidacao($dados)

    
    public function getTitulo($tipo, $muncod)
    {
        $municipio = $this->pegaUm(" SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$muncod}'");
        if($tipo == 1){
            return "Tabela 10-A. Funções Docentes por Modalidade e Etapa de Ensino - Rede Estadual em {$municipio}";
        } elseif($tipo == 2){
            return "Tabela 10-B. Funções Docentes por Modalidade e Etapa de Ensino - Rede Municipal em {$municipio}";
        }
    }  
    
    protected function getSql($co_rede, $uf, $muncod)
    {
    	$sql = "
    	SELECT 			mod.descricao, nu_ano, c_lic || ';' || c_gr|| ';' ||c_em|| ';' ||c_nm || ';' || s_em || ';' || total AS grupo
    	FROM 			ide2016.tab10_mu tab10
    	LEFT JOIN 		territorios.municipio mun ON mun.muncod::integer = tab10.co_ibge
    	LEFT JOIN 		ide2016.tb_ide_modalidade_etapa mod ON mod.co_modalidade_etapa = tab10.co_modalidade_etapa
    	WHERE 			co_rede = {$co_rede} AND mun.muncod = '{$muncod}'";
    	
    	return $sql;
    }    

    public function getTabela($co_rede, $estuf, $muncod)
    {
    	$sql = $this->getSql($co_rede, $estuf, $muncod);
    	$dados = $this->carregar($sql);
    
    	if (!empty($dados)) {
    		$this->setHeader();
    		$this->organizarDados($dados);
    		$this->organizarDados2();
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
    	 
    	foreach ($this->dados as $serie => $dado) {
    		$this->tabelaArvore->addRow();
    		//add serie
    		$this->tabelaArvore->addCell($serie, $class, 'data', array('class' => 'text-center', 'rowspan' => $dado['cont'], 'style' => 'vertical-align:middle;'));
    
    		foreach ($dado as $ano => $indicadores) {
    
    			if (is_array($indicadores)) {
    				
    				$indicadores = array_filter($indicadores);
    				$countInd = count($indicadores);
    				
    				$this->tabelaArvore->addCell($ano, $class, 'data', array('class' => 'text-center', 'rowspan'=> $countInd, 'style' => 'vertical-align:middle;'));
    
    				if (empty($indicadores)) {
    					$this->tabelaArvore->addCell($ano, $class, 'data', array('rowspan' => ($countInd == 0 ? 1 : $countInd), 'class' => 'text-center', 'style' => 'vertical-align:middle;'));
    					$this->tabelaArvore->addCell('', $class, 'data', array('colspan' => 6));
    					$this->tabelaArvore->addRow();
    				} else {
    
    					foreach ($indicadores as $ind) {
    						$arrInd = explode(';', $ind);
    						for ($i = 0; $i <= 5; $i++) {
    							if (array_key_exists($i, $arrInd)) {
    								$this->tabelaArvore->addCell($arrInd[$i], $class, 'data', array('class' => 'text-center'));
    							}
    						}
    
    						$this->tabelaArvore->addRow();
    					}
    				}
    			}
    		}
    	}
    	
    	$nota = "<b>Legenda para Funções Docentes: C/Lic - com Licenciatura; C/Gr - com Graduação; C/EM - com Ensino Médio; C/NM - com Normal Médio; S/EM - sem Ensino Médio.</b>";
    	
    	$this->tabelaArvore->addRow();
    	$this->tabelaArvore->addCell($nota, $class, 'data', array('class' => 'text-left', 'colspan' => '8', 'style' => 'vertical-align:middle;'));
    }
    
    protected function getColunaCabecalho()
    {
    	return array(
    			'Modalidade/Etapa' => array('class' => 'text-center', 'rowspan' => 2, 'style' => 'width: 50%; vertical-align:middle'),
    			'Ano' => array('class' => 'text-center', 'rowspan' => 2, 'style' => 'vertical-align:middle'),
    			'Funções Docentes' => array('class' => 'text-center', 'colspan' => 6)
    	);
    }
    
    protected function organizarDados($dados)
    {
    	foreach ($dados as $key => $valor) {
    		$this->agrupaDados($valor, 'grupo', $valor['descricao']);
    	}
    	return $this->dados;
    }
    
    protected function setHeader()
    {
    
    	$camposDaTabela = $this->getColunaCabecalho();
    	$this->tabelaArvore->addTSection('thead');
    	$class = 'thead';
    	$this->tabelaArvore->addRow('alert alert-info');
    	 
    	foreach ($camposDaTabela as $campo => $atributos) {
    		$this->tabelaArvore->addCell($campo, '', 'header', $atributos);
    	}
    	 
    	$this->tabelaArvore->addRow();
    	 
    	$this->tabelaArvore->addCell('C/Lic', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('C/Gr', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('C/EM', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('C/NM', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('S/EM', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('Total', '', 'header', array('class' => 'text-center alert alert-success'));
    
    }
    
    protected function agrupaDados($dado, $campo_serie, $label)
    {
    	if (!empty($dado[$campo_serie])) {
    		$cont = $this->dados[$label]['cont'];
    		$this->dados[$label]['cont'] = (empty($cont) ? 1 : $cont + 1);
    	} else {
    		$this->dados[$label]['cont']++;
    	}
    	$this->dados[$label][$dado['nu_ano']][] = $dado[$campo_serie];
    }
    
    protected function organizarDados2()
    {
    	foreach ($this->dados as $indice => $infos01) {
    		$cont = 0;
    		if (is_array($infos01)) {
    			foreach ($infos01 as $infos02) {
    				if (is_array($infos02)) {
    					$infos02 = array_filter($infos02);
    					$countInfos02 = count($infos02);
    					$cont += (empty($countInfos02) ? 1 : $countInfos02);
    				}
    			}
    		}
    		$this->dados[$indice]['cont'] = $cont;
    	}
    }    
}//end Class
?>