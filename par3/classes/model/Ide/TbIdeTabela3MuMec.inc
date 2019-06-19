<?php
/**
 * Classe de mapeamento da entidade ide2016.tb_ide_tabela3_mu_mec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */


/**
 * Tb_ide_tabela3_mu_mec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */
class Par3_Model_Ide_TbIdeTabela3MuMec extends Modelo
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
    protected $stNomeTabela = 'ide2016.tb_ide_tabela3_mu_mec';

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
        'nu_ano' => null,
        'no_uf' => null,
        'no_municipio' => null,
        'nu_ideb_inicial' => null,
        'nu_projecao_inicial' => null,
        'nu_ideb_final' => null,
        'nu_projecao_final' => null,
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
            'nu_ano' => array('allowEmpty' => true),
            'no_uf' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 30))),
            'no_municipio' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 150))),
            'nu_ideb_inicial' => array('allowEmpty' => true),
            'nu_projecao_inicial' => array('allowEmpty' => true),
            'nu_ideb_final' => array('allowEmpty' => true),
            'nu_projecao_final' => array('allowEmpty' => true),
            'data_atualizacao' => array('allowEmpty' => true),
        );
    }//end getCamposValidacao($dados)

    public function getTitulo($rede, $muncod)
    {
        return "Tabela 3. Índice de Desenvolvimento da Educação Básica - IDEB";
    }

    protected function getSql($co_rede, $uf, $muncod)
    {
        $sql = "
                SELECT no_uf, nu_ano,
                   nu_ideb_inicial || ';' || nu_projecao_inicial || ';' || nu_ideb_final || ';' || nu_projecao_final /*|| ';' || nu_ideb_em || ';' || nu_projecao_em*/ AS indicador
                FROM ide2016.tb_ide_tabela3_mu_mec tab3
                LEFT JOIN territorios.municipio mun ON mun.muncod::integer = tab3.co_ibge
                WHERE mun.muncod = '{$muncod}'
        ";
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
                        $this->tabelaArvore->addCell('', $class, 'data', array('colspan' => 6));
                        $this->tabelaArvore->addRow();
                    } else {
                        // add ano
                        $this->tabelaArvore->addCell($ano, $class, 'data', array('rowspan' => ($countInd == 0 ? 1 : $countInd), 'class' => 'text-center', 'style' => 'vertical-align:middle;'));

                        foreach ($indicadores as $ind) {
                            $arrInd = explode(';', $ind);
                            foreach($arrInd as $val){
                                $this->tabelaArvore->addCell($val, $class, 'data', array('class' => 'text-center'));
                            }
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
            ' ' => array('rowspan' => 2, 'class' => 'text-center', 'style' => ''),
            'Ano' => array('rowspan' => 2, 'class' => 'text-center', 'style' => 'vertical-align:middle;'),
            'Anos Iniciais do Ensino Fundamental' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
            'Anos Finais do Ensino Fundamental' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
            'Ensino Médio' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
        );
    }

    protected function organizarDados($dados)
    {
        foreach ($dados as $key => $valor) {
            $this->agrupaDados($valor, 'indicador', $valor['no_uf']);
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
        for ($i = 0; $i <=2; $i++) {
            $this->tabelaArvore->addCell('IDEB Observado', '', 'header', array('class' => 'text-center alert alert-success'));
            $this->tabelaArvore->addCell('Metas', '', 'header', array('class' => 'text-center alert alert-success'));
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