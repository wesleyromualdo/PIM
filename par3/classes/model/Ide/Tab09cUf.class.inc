<?php
/**
 * Classe de mapeamento da entidade ide2016.tab09c_uf
 *
 * @category Class
 * @package  A1
 * @author   FERNANDO DE SOUZA AIRES <fernando.aires@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-02-2016
 * @link     no link
 */



/**
 * Ide2016__ModelTab09c_uf
 *
 * @category Class
 * @package  A1
 * @author   FERNANDO DE SOUZA AIRES <fernando.aires@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 15-02-2016
 * @link     no link
 */
class Par3_Model_Ide_Tab09cUf extends Modelo
{
	
	protected $tabelaArvore;
	protected $titulo;
	protected $dados;
	
	public function __construct($id = null)
	{
		parent::__construct();
		$this->tabelaArvore = new HTML_Table('table_tabela09c_uf', 'table table-condensed table-bordered  table-responsive');
		$this->dados = array();
	}	
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'ide2016.tab09c_uf';

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
        'co_ibge' => null,
        'co_modalidade' => null,
        'co_rede' => null,
        'co_uf' => null,
        'nu_ano' => null,
        'nu_eja_int' => null,
        'nu_eja_prof' => null,
        'nu_emejap' => null,
        'nu_emint' => null,
        'nu_emnm' => null,
        'nu_emr' => null,
        'nu_ept_c' => null,
        'nu_ept_s' => null,
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
            'co_ibge' => array( 'allowEmpty' => true ),
            'co_modalidade' => array( 'allowEmpty' => true ),
            'co_rede' => array( 'allowEmpty' => true ),
            'co_uf' => array( 'allowEmpty' => true ),
            'nu_ano' => array( 'allowEmpty' => true ),
            'nu_eja_int' => array( 'allowEmpty' => true ),
            'nu_eja_prof' => array( 'allowEmpty' => true ),
            'nu_emejap' => array( 'allowEmpty' => true ),
            'nu_emint' => array( 'allowEmpty' => true ),
            'nu_emnm' => array( 'allowEmpty' => true ),
            'nu_emr' => array( 'allowEmpty' => true ),
            'nu_ept_c' => array( 'allowEmpty' => true ),
            'nu_ept_s' => array( 'allowEmpty' => true ),
            'data_atualizacao' => array( 'allowEmpty' => true ),
        );
    }//end getCamposValidacao($dados)

    public function getTitulo($rede, $uf)
    {
    	$estado = $this->pegaUm("SELECT estdescricao FROM territorios.estado WHERE ESTUF = '{$uf}' ");
    	if ($rede == 1) {
    		return "Tabela 9-C. Distribuição de Matrículas do Ensino Médio por Modalidade - Rede Estadual do Estado de {$estado}";
    	} elseif ($rede == 2) {
    		return "Tabela 9-C. Distribuição de Matrículas do Ensino Médio por Modalidade - Rede Municipal do Estado de {$estado}";
    	} elseif ($rede == 3) {
    		return "Tabela 9-C. Distribuição de Matrículas do Ensino Médio por Modalidade - Rede Federal do Estado de {$estado}";
    	} elseif ($rede == 4) {
    		return "Tabela 9-C. Distribuição de Matrículas do Ensino Médio por Modalidade - Rede Privada do Estado de {$estado}";
    	}
    }
    
    protected function getSql($co_rede, $uf)
    {
    	$sql = "

    				SELECT nu_ano,sum(nu_emr)  || ';' || sum(nu_emint) || ';' || sum(nu_emnm)|| ';' || sum(nu_ept_c) || ';' || sum(nu_ept_s) || ';' || sum(nu_emejap) || ';' || sum(nu_eja_prof) || ';' || sum(nu_eja_int)||';'|| (sum(nu_emr) +sum(nu_emint)+sum(nu_emnm)+sum(nu_ept_c)+sum(nu_ept_s) +sum(nu_emejap)+sum(nu_eja_prof)+sum(nu_eja_int))  as grupo
    	FROM 		ide2016.tab09c_uf tab09c
    	LEFT JOIN 	territorios.estado est ON est.estcod = tab09c.co_uf::varchar(2)
    	LEFT JOIN 	ide2016.tb_ide_modalidade_ensino mod ON mod.co_modalidade = tab09c.co_modalidade
    	WHERE 		co_rede = {$co_rede} AND est.estuf = '{$uf}'GROUP by nu_ano ORDER by nu_ano";

    	return $sql;
    }
    
    public function getTabela($rede, $estuf)
    {
    	$sql = $this->getSql($rede, $estuf);
    	$dados = $this->carregar($sql);
   // ver($sql);exit;
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
    
    protected function getColunaCabecalho()
    {
    	return array(
    			'Ano' => array('class' => 'text-center', 'rowspan' => 2, 'style' => 'width: 50%; vertical-align:middle'),
    			'Ensino Médio' => array('class' => 'text-center', 'colspan' => 3),
    			'Educação Profissional de Nível Técnico' => array('class' => 'text-center', 'colspan' => 2),
    			'EJA' => array('class' => 'text-center', 'colspan' => 3),
    			'Total' => array('class' => 'text-center', 'rowspan' => 2, 'style' => 'vertical-align:middle')
    	);
    }
        
    protected function setBody()
    {
    	$this->tabelaArvore->addTSection('tbody');
    	
    	foreach ($this->dados as $etapa => $dado) {
    		$this->tabelaArvore->addRow();
    		
    		foreach ($dado as $ano => $indicadores) {
    
    			if (is_array($indicadores)) {
    
    				$indicadores = array_filter($indicadores);
    				$countInd = count($indicadores);
    
    				$this->tabelaArvore->addCell($ano, $class, 'data', array('class' => 'text-center', 'rowspan'=> $countInd,  'style' => 'vertical-align:middle;'));
    
    				if (empty($indicadores)) {
    					$this->tabelaArvore->addCell($ano, $class, 'data', array('rowspan' => ($countInd == 0 ? 1 : $countInd), 'class' => 'text-center', 'style' => 'vertical-align:middle;'));
    					$this->tabelaArvore->addCell('', $class, 'data', array('colspan' => 10));
    					$this->tabelaArvore->addRow();
    				} else {
    
    					foreach ($indicadores as $ind) {
    						$arrInd = explode(';', $ind);
    						for ($i = 0; $i <= 9; $i++) {
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
    	 
    	$nota = "<b>NOTA:</b></br>
    			 (1) Integrado: alunos que estão cursando o ensino médio articulado com a educação profissional técnica, com matrícula única.</br>
    			 (2) Concomitante: alunos com matrículas distintas no ensino médio e na educação profissional.</br>
    			 (3) Subsequente: alunos que já concluíram o ensino médio (matrícula somente na educação profissional).";
    
    	$this->tabelaArvore->addRow();
    	$this->tabelaArvore->addCell($nota, $class, 'data', array('class' => 'text-left', 'colspan' => '10', 'style' => 'vertical-align:middle;'));
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
    	//Ensino Médio
    	$this->tabelaArvore->addCell('Regular', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('Integrado(1)', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('Normal Magistério', '', 'header', array('class' => 'text-center alert alert-success'));
    	//Educação
    	$this->tabelaArvore->addCell('Concomitante(2)', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('Subsequente(3)', '', 'header', array('class' => 'text-center alert alert-success'));
    	//EJA
    	$this->tabelaArvore->addCell('Presencial', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('Semipresencial', '', 'header', array('class' => 'text-center alert alert-success'));
    	$this->tabelaArvore->addCell('integrado', '', 'header', array('class' => 'text-center alert alert-success'));    
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