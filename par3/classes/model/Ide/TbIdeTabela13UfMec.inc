<?php
/**
 * Classe de mapeamento da entidade ide2016.tb_ide_tabela13_uf_mec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */


/**
 * Tb_ide_tabela13_uf_mec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */
class Par3_Model_Ide_TbIdeTabela13UfMec extends Modelo
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
    protected $stNomeTabela = 'ide2016.tb_ide_tabela13_uf_mec';

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
        'co_area' => null,
        'co_rede' => null,
        'co_uf' => null,
        'nu_0s1a_aband_fund' => null,
        'nu_0s1a_apr_fund' => null,
        'nu_0s1a_repr_fund' => null,
        'nu_1s_aband_medio' => null,
        'nu_1s_apr_medio' => null,
        'nu_1s_repr_medio' => null,
        'nu_1s2a_aband_fund' => null,
        'nu_1s2a_apr_fund' => null,
        'nu_1s2a_repr_fund' => null,
        'nu_2s_aband_medio' => null,
        'nu_2s_apr_medio' => null,
        'nu_2s_repr_medio' => null,
        'nu_2s3a_aband_fund' => null,
        'nu_2s3a_apr_fund' => null,
        'nu_2s3a_repr_fund' => null,
        'nu_3s_aband_medio' => null,
        'nu_3s_apr_medio' => null,
        'nu_3s_repr_medio' => null,
        'nu_3s4a_aband_fund' => null,
        'nu_3s4a_apr_fund' => null,
        'nu_3s4a_repr_fund' => null,
        'nu_4s_aband_medio' => null,
        'nu_4s_apr_medio' => null,
        'nu_4s_repr_medio' => null,
        'nu_4s5a_aband_fund' => null,
        'nu_4s5a_apr_fund' => null,
        'nu_4s5a_repr_fund' => null,
        'nu_5s6a_aband_fund' => null,
        'nu_5s6a_apr_fund' => null,
        'nu_5s6a_repr_fund' => null,
        'nu_6s7a_aband_fund' => null,
        'nu_6s7a_apr_fund' => null,
        'nu_6s7a_repr_fund' => null,
        'nu_7s8a_aband_fund' => null,
        'nu_7s8a_apr_fund' => null,
        'nu_7s8a_repr_fund' => null,
        'nu_8s9a_aband_fund' => null,
        'nu_8s9a_apr_fund' => null,
        'nu_8s9a_repr_fund' => null,
        'nu_ano' => null,
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
            'co_area' => array('allowEmpty' => true),
            'co_rede' => array('allowEmpty' => true),
            'co_uf' => array('allowEmpty' => true),
            'nu_0s1a_aband_fund' => array('allowEmpty' => true),
            'nu_0s1a_apr_fund' => array('allowEmpty' => true),
            'nu_0s1a_repr_fund' => array('allowEmpty' => true),
            'nu_1s_aband_medio' => array('allowEmpty' => true),
            'nu_1s_apr_medio' => array('allowEmpty' => true),
            'nu_1s_repr_medio' => array('allowEmpty' => true),
            'nu_1s2a_aband_fund' => array('allowEmpty' => true),
            'nu_1s2a_apr_fund' => array('allowEmpty' => true),
            'nu_1s2a_repr_fund' => array('allowEmpty' => true),
            'nu_2s_aband_medio' => array('allowEmpty' => true),
            'nu_2s_apr_medio' => array('allowEmpty' => true),
            'nu_2s_repr_medio' => array('allowEmpty' => true),
            'nu_2s3a_aband_fund' => array('allowEmpty' => true),
            'nu_2s3a_apr_fund' => array('allowEmpty' => true),
            'nu_2s3a_repr_fund' => array('allowEmpty' => true),
            'nu_3s_aband_medio' => array('allowEmpty' => true),
            'nu_3s_apr_medio' => array('allowEmpty' => true),
            'nu_3s_repr_medio' => array('allowEmpty' => true),
            'nu_3s4a_aband_fund' => array('allowEmpty' => true),
            'nu_3s4a_apr_fund' => array('allowEmpty' => true),
            'nu_3s4a_repr_fund' => array('allowEmpty' => true),
            'nu_4s_aband_medio' => array('allowEmpty' => true),
            'nu_4s_apr_medio' => array('allowEmpty' => true),
            'nu_4s_repr_medio' => array('allowEmpty' => true),
            'nu_4s5a_aband_fund' => array('allowEmpty' => true),
            'nu_4s5a_apr_fund' => array('allowEmpty' => true),
            'nu_4s5a_repr_fund' => array('allowEmpty' => true),
            'nu_5s6a_aband_fund' => array('allowEmpty' => true),
            'nu_5s6a_apr_fund' => array('allowEmpty' => true),
            'nu_5s6a_repr_fund' => array('allowEmpty' => true),
            'nu_6s7a_aband_fund' => array('allowEmpty' => true),
            'nu_6s7a_apr_fund' => array('allowEmpty' => true),
            'nu_6s7a_repr_fund' => array('allowEmpty' => true),
            'nu_7s8a_aband_fund' => array('allowEmpty' => true),
            'nu_7s8a_apr_fund' => array('allowEmpty' => true),
            'nu_7s8a_repr_fund' => array('allowEmpty' => true),
            'nu_8s9a_aband_fund' => array('allowEmpty' => true),
            'nu_8s9a_apr_fund' => array('allowEmpty' => true),
            'nu_8s9a_repr_fund' => array('allowEmpty' => true),
            'nu_ano' => array('allowEmpty' => true),
            'data_atualizacao' => array('allowEmpty' => true),
        );
    }//end getCamposValidacao($dados)

    public function getTitulo($rede, $uf)
    {
        $estado = $this->pegaUm("SELECT estdescricao FROM territorios.estado WHERE ESTUF = '{$uf}' ");
        if ($rede == 1) {
            return "Tabela 13-A. Taxas de Rendimento - Rede Estadual do Estado do {$estado}";
        } elseif ($rede == 2) {
            return "Tabela 13-B. Taxas de Rendimento - Redes Municipais do Estado do {$estado}";
        }
    }

    protected function getSql($co_rede, $uf)
    {
        $sql = "
                SELECT area.co_area, area.descricao AS area, nu_ano,
                
                       nu_0s1a_apr_fund || ';' || nu_0s1a_repr_fund || ';' || nu_0s1a_aband_fund as serie_0_a_1,
                       nu_1s2a_apr_fund || ';' || nu_1s2a_repr_fund || ';' || nu_1s2a_aband_fund as serie_1_a_2,
                       nu_2s3a_apr_fund || ';' || nu_2s3a_repr_fund || ';' || nu_2s3a_aband_fund as serie_2_a_3,
                       nu_3s4a_apr_fund || ';' || nu_3s4a_repr_fund || ';' || nu_3s4a_aband_fund as serie_3_a_4,
                       nu_4s5a_apr_fund || ';' || nu_4s5a_repr_fund || ';' || nu_4s5a_aband_fund as serie_4_a_5,
                       nu_5s6a_apr_fund || ';' || nu_5s6a_repr_fund || ';' || nu_5s6a_aband_fund as serie_5_a_6,
                       nu_6s7a_apr_fund || ';' || nu_6s7a_repr_fund || ';' || nu_6s7a_aband_fund as serie_6_a_7,
                       nu_7s8a_apr_fund || ';' || nu_7s8a_repr_fund || ';' || nu_7s8a_aband_fund as serie_7_a_8,
                       nu_8s9a_apr_fund || ';' || nu_8s9a_repr_fund || ';' || nu_8s9a_aband_fund as serie_8_a_9,
                        nu_1s_apr_medio || ';' ||  nu_1s_repr_medio || ';' || nu_1s_aband_medio as medio_1,
                        nu_2s_apr_medio || ';' ||  nu_2s_repr_medio || ';' || nu_2s_aband_medio as medio_2,
                        nu_3s_apr_medio || ';' ||  nu_3s_repr_medio || ';' || nu_3s_aband_medio as medio_3,
                        nu_4s_apr_medio || ';' ||  nu_4s_repr_medio || ';' || nu_4s_aband_medio as medio_4
                FROM ide2016.tb_ide_tabela13_uf_mec tab13
                LEFT JOIN territorios.estado est ON est.estcod = tab13.co_uf::varchar(2)
                LEFT JOIN ide2016.tb_ide_area area ON area.co_area = tab13.co_area
               WHERE co_rede = {$co_rede} AND est.estuf = '{$uf}'group by area.co_area,area,nu_ano,serie_0_a_1,serie_1_a_2,serie_2_a_3,serie_3_a_4,serie_4_a_5,serie_5_a_6,serie_6_a_7,serie_7_a_8,serie_8_a_9,medio_1,medio_2,medio_3,medio_4 order by area
        ";
        return $sql;
    }

    public function getTabela($rede, $estuf)
    {
        $sql = $this->getSql($rede, $estuf);
        $dados = $this->carregar($sql);
     //   ver($sql);
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
            $this->tabelaArvore->addCell($serie_ano, $class, 'data', array('rowspan' => $dado['cont'], 'class' => 'text-center', 'style' => 'vertical-align:middle;'));


            foreach ($dado as $ano => $areas) {

                if (is_array($areas)) {

                    //add serie
                    $this->tabelaArvore->addCell($ano, $class, 'data', array('rowspan' => 2, 'class' => 'text-center', 'style' => 'vertical-align:middle;'));

                    foreach ($areas as $area => $indicadores) {

                        if (is_array($indicadores)) {
                            $indicadores = array_filter($indicadores);
                            $countInd = count($indicadores);

                            if (empty($indicadores)) {
                                // add ano
                                $this->tabelaArvore->addCell($area, $class, 'data', array('rowspan' => ($countInd == 0 ? 1 : $countInd), 'class' => 'text-center', 'style' => 'vertical-align:middle;'));
                                $this->tabelaArvore->addCell('', $class, 'data', array('colspan' => 3));
                                $this->tabelaArvore->addRow();
                            } else {
                                // add ano
                                $this->tabelaArvore->addCell($area, $class, 'data', array('rowspan' => ($countInd == 0 ? 1 : $countInd), 'class' => 'text-center', 'style' => 'vertical-align:middle;'));

                                foreach ($indicadores as $ind) {
                                    $arrInd = explode(';', $ind);
                                    for ($i = 0; $i <= 2; $i++) {
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
            }
        }
    }

    protected function getColunaCabecalho()
    {
        return array(
            'SÉRIE/ANO' => array('class' => 'text-center', 'style' => ''),
            'Ano' => array('class' => 'text-center', 'style' => ''),
            'Área' => array('class' => 'text-center', 'style' => ''),
            'Taxa Aprovação' => array('class' => 'text-center', 'style' => ''),
            'Taxa Reprovação' => array('class' => 'text-center', 'style' => ''),
            'Taxa Abandono' => array('class' => 'text-center', 'style' => ''),
        );
    }

    protected function organizarDados($dados)
    {
        foreach ($dados as $key => $valor) {
            $this->agrupaDados($valor, 'serie_0_a_1', '1ª série');
            $this->agrupaDados($valor, 'serie_1_a_2', '1ª série / 2º ano do EF');
            $this->agrupaDados($valor, 'serie_2_a_3', '2ª série / 3º ano do EF');
            $this->agrupaDados($valor, 'serie_3_a_4', '3ª série / 4º ano do EF');
            $this->agrupaDados($valor, 'serie_4_a_5', '4ª série / 5º ano do EF');
            $this->agrupaDados($valor, 'serie_5_a_6', '5ª série / 6º ano do EF');
            $this->agrupaDados($valor, 'serie_6_a_7', '6ª série / 7º ano do EF');
            $this->agrupaDados($valor, 'serie_7_a_8', '7ª série / 8º ano do EF');
            $this->agrupaDados($valor, 'serie_8_a_9', '8ª série / 9º ano do EF');
            $this->agrupaDados($valor, 'medio_1', '1º ano do EM');
            $this->agrupaDados($valor, 'medio_2', '2º ano do EM');
            $this->agrupaDados($valor, 'medio_3', '3º ano do EM');
            $this->agrupaDados($valor, 'medio_4', '4º ano do EM');
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
       // ver($dados)
    {
        if (!empty($dado[$campo_serie])) {
            $cont = $this->dados[$label]['cont'];
            $this->dados[$label]['cont'] = (empty($cont) ? 1 : $cont + 1);
        } else {
            $this->dados[$label]['cont']++;
        }
        $this->dados[$label][$dado['nu_ano']][$dado['area']][] = $dado[$campo_serie];
    }

    protected function organizarDados2()
    {
        foreach ($this->dados as $indice => $infos01) {
            $cont = 0;
            if (is_array($infos01)) {
                foreach ($infos01 as $infos02) {
                    if (is_array($infos02)) {
                        foreach ($infos02 as $infos03) {
                            if (is_array($infos03)) {
                                $infos03 = array_filter($infos03);
                                $countInfos03 = count($infos03);
                                $cont += (empty($countInfos03) ? 1 : $countInfos03);
                            }
                        }
                    }
                }
            }
            $this->dados[$indice]['cont'] = $cont;
        }
    }

}//end Class
?>