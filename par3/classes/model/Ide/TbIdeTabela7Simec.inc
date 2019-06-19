<?php
/**
 * Classe de mapeamento da entidade ide2016.tb_ide_tabela7_simec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */

/**
 * Tb_ide_tabela7_simec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */
class Par3_Model_Ide_TbIdeTabela7Simec extends Modelo
{
    protected $tabelaArvore;
    protected $titulo;
    protected $dados;

    public function __construct($id = null)
    {
        parent::__construct();
        $this->tabelaArvore = new HTML_Table('table_tabela11a_uf', 'table table-condensed table-bordered  table-responsive');
        $this->dados = array();
    }

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'ide2016.tb_ide_tabela7_simec';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array();
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array();

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'co_ibge' => null,
        'co_rede' => null,
        'co_area' => null,
        'nu_ano' => null,
        'nu_escolas' => null,
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
            'co_ibge' => array('allowEmpty' => true),
            'co_rede' => array('allowEmpty' => true),
            'co_area' => array('allowEmpty' => true),
            'nu_ano' => array('allowEmpty' => true),
            'nu_escolas' => array('allowEmpty' => true),
            'data_atualizacao' => array('allowEmpty' => true),
        );
    }//end getCamposValidacao($dados)

    public function getTitulo($rede, $muncod)
    {
        $municipio = $this->pegaUm(" SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$muncod}' ");

        if ($rede == 1) {
            return "Tabela 7-A. Número de Escolas Rurais em Áreas Específicas - Rede Estadual em {$municipio}";
        } elseif ($rede == 2) {
            return "Tabela 7-B. Número de Escolas Rurais em Áreas Específicas - Rede Municipal em {$municipio}";
        }
    }

    protected function getSql($co_rede, $uf, $muncod)
    {
        $sql = "
                SELECT nu_ano, area.descricao as area, nu_escolas
                FROM ide2016.tb_ide_tabela7_simec tab7
                LEFT JOIN territorios.municipio mun ON mun.muncod::integer = tab7.co_ibge
                LEFT JOIN ide2016.tb_ide_area area ON area.co_area = tab7.co_area
                WHERE --area.co_area NOT IN (2,3,4,6) AND
                      co_rede = {$co_rede} AND mun.estuf = '{$uf}' AND mun.muncod = '{$muncod} ' GROUP by nu_ano, area,nu_escolas ORDER  by nu_ano
        ";
        return $sql;
    }

    public function getTabela($rede, $estuf, $muncod)
    {
        $sql = $this->getSql($rede, $estuf, $muncod);
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
        $serie_ano_old = null;

        foreach ($this->dados as $serie_ano => $dado) {
            $this->tabelaArvore->addRow();
            //add serie
            $this->tabelaArvore->addCell($serie_ano, $class, 'data', array('class' => 'text-center', 'rowspan' => $dado['cont'], 'style' => 'vertical-align:middle;'));

            foreach ($dado as $ano => $indicadores) {

                if (is_array($indicadores)) {
                    $indicadores = array_filter($indicadores);
                    $countInd = count($indicadores);

                    if (empty($indicadores)) {
                        // add ano
                        $this->tabelaArvore->addCell($ano, $class, 'data', array('rowspan' => ($countInd == 0 ? 1 : $countInd), 'class' => 'text-center', 'style' => 'vertical-align:middle;'));
                        $this->tabelaArvore->addCell('', $class, 'data', array('colspan' => 3));
                        $this->tabelaArvore->addRow();
                    } else {
                        // add ano
                        $this->tabelaArvore->addCell($ano, $class, 'data', array('rowspan' => ($countInd == 0 ? 1 : $countInd), 'class' => 'text-center', 'style' => 'vertical-align:middle;'));

                        foreach ($indicadores as $ind) {
                            $this->tabelaArvore->addCell($ind, $class, 'data', array('class' => 'text-center'));
                            $this->tabelaArvore->addRow();
                        }
                    }
                }
            }
        }
    }

    protected function getColunaCabecalho()
    {
        return array(
            'Áreas' => array('class' => 'text-center', 'style' => 'width: 50%;'),
            'Ano' => array('class' => 'text-center', 'style' => 'width: 25%;'),
            'Número de Escolas' => array('class' => 'text-center', 'style' => 'width: 25%;'),
        );
    }

    protected function organizarDados($dados)
    {
        foreach ($dados as $key => $valor) {
            if($valor['area'] == "Rural"){
                $valor['area'] = "Escola do Campo";
            }
            $this->agrupaDados($valor, 'nu_escolas', $valor['area']);
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