<?php
/**
 * Classe de mapeamento da entidade ide2016.tab09_uf
 *
 * @category Class
 * @package  A1
 * @author   FERNANDO DE SOUZA AIRES <fernando.aires@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-02-2016
 * @link     no link
 */



/**
 * Ide2016__ModelTab09_uf
 *
 * @category Class
 * @package  A1
 * @author   FERNANDO DE SOUZA AIRES <fernando.aires@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-02-2016
 * @link     no link
 */
class Par3_Model_Ide_Tab09Uf extends Modelo
{
	
	protected $tabelaArvore;
	protected $titulo;
	protected $dados;
	
	public function __construct($id = null)
	{
		parent::__construct();
		$this->tabelaArvore = new HTML_Table('table_tabela09_uf', 'table table-condensed table-bordered  table-responsive');
		$this->dados = array();
	}	
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'ide2016.tab09_uf';

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
        'co_modalidade' => null,
        'co_modalidade_etapa' => null,
        'co_rede' => null,
        'co_uf' => null,
        'nu_ano' => null,
        'nu_rural_maior_igual_4h_d' => null,
        'nu_rural_maior_igual_4h_n' => null,
        'nu_rural_menor_4h_diurno' => null,
        'nu_rural_menor_4h_noturno' => null,
        'nu_rural_total' => null,
        'nu_total' => null,
        'nu_urbana_maior_igual_4h_' => null,
        'nu_urbana_maior_igual_4h1' => null,
        'nu_urbana_menor_4h_diurno' => null,
        'nu_urbana_menor_4h_noturn' => null,
        'nu_urbana_total' => null,
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
            'co_modalidade' => array( 'allowEmpty' => true ),
            'co_modalidade_etapa' => array( 'allowEmpty' => true ),
            'co_rede' => array( 'allowEmpty' => true ),
            'co_uf' => array( 'allowEmpty' => true ),
            'nu_ano' => array( 'allowEmpty' => true ),
            'nu_rural_maior_igual_4h_d' => array( 'allowEmpty' => true ),
            'nu_rural_maior_igual_4h_n' => array( 'allowEmpty' => true ),
            'nu_rural_menor_4h_diurno' => array( 'allowEmpty' => true ),
            'nu_rural_menor_4h_noturno' => array( 'allowEmpty' => true ),
            'nu_rural_total' => array( 'allowEmpty' => true ),
            'nu_total' => array( 'allowEmpty' => true ),
            'nu_urbana_maior_igual_4h_' => array( 'allowEmpty' => true ),
            'nu_urbana_maior_igual_4h1' => array( 'allowEmpty' => true ),
            'nu_urbana_menor_4h_diurno' => array( 'allowEmpty' => true ),
            'nu_urbana_menor_4h_noturn' => array( 'allowEmpty' => true ),
            'nu_urbana_total' => array( 'allowEmpty' => true ),
            'data_atualizacao' => array( 'allowEmpty' => true ),
        );
    }//end getCamposValidacao($dados)

    public function getTitulo($rede, $uf)
    {
    	$estado = $this->pegaUm("SELECT estdescricao FROM territorios.estado WHERE ESTUF = '{$uf}' ");
    	if ($rede == 1) {
    		return "Tabela 9-A. Matrículas por Modalidade, Etapa e Turno - Rede Estadual do Estado de {$estado}";
    	} elseif ($rede == 2) {
    		return "Tabela 9-B. Matrículas por Modalidade, Etapa e Turno - Redes Municipais do Estado de {$estado}";
    	}
    }
    
    protected function getSql($co_rede, $uf)
    {
    	$sql = "
    	SELECT 			mod.descricao, nu_ano, 
    					nu_rural_menor_4h_diurno || ';' || nu_rural_maior_igual_4h_d || ';' || nu_rural_menor_4h_noturno || ';' || nu_rural_maior_igual_4h_n || ';' ||  nu_rural_total || ';' ||
    					nu_urbana_menor_4h_diurno || ';' || nu_urbana_maior_igual_4h_ || ';' || nu_urbana_menor_4h_noturn || ';' || nu_urbana_maior_igual_4h1 || ';' ||  nu_urbana_total || ';' || nu_total AS grupo
    	FROM 		ide2016.tab09_uf tab09
    	LEFT JOIN 	territorios.estado est ON est.estcod = tab09.co_uf::varchar(2)
    	LEFT JOIN 	ide2016.tb_ide_modalidade_etapa mod ON mod.co_modalidade_etapa = tab09.co_modalidade_etapa
    	WHERE 		co_rede = {$co_rede} AND est.estuf = '{$uf}'";
    	 
    	return $sql;
    }
    
    public function getTabela($rede, $estuf)
    {
    	$sql = $this->getSql($rede, $estuf);
    	$dados = $this->carregar($sql);
    
    	if (!empty($dados)) {
    		$this->setHeader();
    		$this->organizarDados($dados);
    		$this->setBody();
    		echo $this->tabelaArvore->display();
    	} else {
    		echo '<div class="alert alert-warning">
                   Não possui resultados para esta pesquisa.
                </div>';
    	}
    }

    
    protected function getColunaCabecalho()
    {
    	return array(
    			'Modalidade/Etapa' => array('class' => 'text-center', 'rowspan' => 3, 'style' => 'width: 50%; vertical-align:middle'),
    			'Ano' => array('class' => 'text-center', 'rowspan' => 3, 'style' => 'vertical-align:middle'),
    			'Matrículas por Ano' => array('class' => 'text-center', 'colspan' => 10),
    			'Total' => array('class' => 'text-center', 'rowspan' => 3, 'style' => 'vertical-align:middle'),
    	);
    }
    
    protected function setBody()
    {
    	$this->tabelaArvore->addTSection('tbody');
    
    	foreach ($this->dados as $etapa => $dado) {
    		$this->tabelaArvore->addRow();
    		$this->tabelaArvore->addCell($etapa, $class, 'data', array('class' => 'text-center', 'rowspan' => $dado['cont'], 'style' => 'vertical-align:middle;'));
    
    		foreach ($dado as $ano => $indicadores) {
    
    			if (is_array($indicadores)) {
    
    				$indicadores = array_filter($indicadores);
    				$countInd = count($indicadores);
    
    				$this->tabelaArvore->addCell($ano, $class, 'data', array('class' => 'text-center', 'rowspan'=> $countInd,  'style' => 'vertical-align:middle;'));
    
    				if (empty($indicadores)) {
    					$this->tabelaArvore->addCell($ano, $class, 'data', array('rowspan' => ($countInd == 0 ? 1 : $countInd), 'class' => 'text-center', 'style' => 'vertical-align:middle;'));
    					$this->tabelaArvore->addCell('', $class, 'data', array('colspan' => 11));
    					$this->tabelaArvore->addRow();
    				} else {
    
    					foreach ($indicadores as $ind) {
    						$arrInd = explode(';', $ind);
    						for ($i = 0; $i <= 10; $i++) {
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
    	
    	$nota = "<b>Legenda para matrículas por turno:</b></br>
				 D-4: Diurno (Início das aulas antes das 17h) - Menos de 4h/aula/dia</br>
				 D+4: Diurno (Início das aulas antes das 17h) - 4h/aula/dia ou mais</br>
				 N-4: Noturno (Início das aulas a partir das 17h) - Menos de 4h/aula/dia</br>
				 N+4: Noturno (Início das aulas a partir das 17h) - 4h/aula/dia ou mais
				 T: Total";
    	 
    	$this->tabelaArvore->addRow();
    	$this->tabelaArvore->addCell($nota, $class, 'data', array('class' => 'text-left', 'colspan' => '8', 'style' => 'vertical-align:middle;'));
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
    	 
    	$this->tabelaArvore->addCell('Rural', '', 'header', array('class' => 'text-center alert alert-success', 'colspan' => 5));
    	$this->tabelaArvore->addCell('Urbana', '', 'header', array('class' => 'text-center alert alert-success', 'colspan' => 5));
    	 
    	$this->tabelaArvore->addRow();
    	//Urbana
    	$this->tabelaArvore->addCell('D-4', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('D+4', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('N-4', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('N+4', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('T', '', 'header', array('class' => 'text-center alert alert-success'));
    	//Rural
    	$this->tabelaArvore->addCell('D-4', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('D+4', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('N-4', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('N+4', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('T', '', 'header', array('class' => 'text-center alert alert-success'));
    
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
    
}//end Class
?>