<?php
/**
 * Classe de mapeamento da entidade ide2016.tb_ide_tabela11a_uf_mec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-12-2015
 * @link     no link
 */

include_once APPRAIZ . "includes/library/simec/view/html_table.class.php";


/**
 * Tb_ide_tabela11a_uf_mec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-12-2015
 * @link     no link
 */
class Par3_Model_Ide_TbIdeTabela11aUfMec extends Modelo
{
    protected $tabelaArvore;
    protected $titulo;
    protected $dados;

    public function __construct($id = null)
    {
        parent::__construct();
        $this->tabelaArvore = new HTML_Table('table_tabela11a_Uf', 'table table-condensed table-bordered  table-responsive');
        $this->dados  = array();
    }

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'ide2016.tb_ide_tabela11a_uf_mec';

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
        'co_uf' => null,
        'nu_ano' => null,
        'no_estado' => null,
        'co_rede' => null,
        'alunos_turma_ei_urbana' => null,
        'alunos_turma_ei_rural' => null,
        'alunos_turma_ai_ef_urbana' => null,
        'alunos_turma_ai_ef_rural' => null,
        'alunos_turma_af_ef_urbana' => null,
        'alunos_turma_af_ef_rural' => null,
        'alunos_turma_em_urbana' => null,
        'alunos_turma_em_rural' => null,
        'alunos_turma_eja_ef_urbana' => null,
        'alunos_turma_eja_ef_rural' => null,
        'horas_aula_ei_urbana' => null,
        'horas_aula_ei_rural' => null,
        'horas_aula_ai_ef_urbana' => null,
        'horas_aula_ai_ef_rural' => null,
        'horas_aula_af_ef_urbana' => null,
        'horas_aula_af_ef_rural' => null,
        'horas_aula_em_urbana' => null,
        'horas_aula_em_rural' => null,
        'horas_aula_eja_ef_urbana' => null,
        'horas_aula_eja_ef_rural' => null,
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
            'co_uf' => array('allowEmpty' => true),
            'nu_ano' => array('allowEmpty' => true),
            'no_estado' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 50))),
            'co_rede' => array('allowEmpty' => true),
            'alunos_turma_ei_urbana' => array('allowEmpty' => true),
            'alunos_turma_ei_rural' => array('allowEmpty' => true),
            'alunos_turma_ai_ef_urbana' => array('allowEmpty' => true),
            'alunos_turma_ai_ef_rural' => array('allowEmpty' => true),
            'alunos_turma_af_ef_urbana' => array('allowEmpty' => true),
            'alunos_turma_af_ef_rural' => array('allowEmpty' => true),
            'alunos_turma_em_urbana' => array('allowEmpty' => true),
            'alunos_turma_em_rural' => array('allowEmpty' => true),
            'alunos_turma_eja_ef_urbana' => array('allowEmpty' => true),
            'alunos_turma_eja_ef_rural' => array('allowEmpty' => true),
            'horas_aula_ei_urbana' => array('allowEmpty' => true),
            'horas_aula_ei_rural' => array('allowEmpty' => true),
            'horas_aula_ai_ef_urbana' => array('allowEmpty' => true),
            'horas_aula_ai_ef_rural' => array('allowEmpty' => true),
            'horas_aula_af_ef_urbana' => array('allowEmpty' => true),
            'horas_aula_af_ef_rural' => array('allowEmpty' => true),
            'horas_aula_em_urbana' => array('allowEmpty' => true),
            'horas_aula_em_rural' => array('allowEmpty' => true),
            'horas_aula_eja_ef_urbana' => array('allowEmpty' => true),
            'horas_aula_eja_ef_rural' => array('allowEmpty' => true),
            'data_atualizacao' => array('allowEmpty' => true),
        );
    }//end getCamposValidacao($dados)

    public function getTitulo($rede, $uf)
    {
        $estado = $this->pegaUm("SELECT estdescricao FROM territorios.estado WHERE ESTUF = '{$uf}' ");
        if($rede == 1){
            return "Tabela 11-A. Condições de Atendimento Diurno - Rede Estadual do Estado do {$estado}";
        }elseif($rede == 2){
            return "Tabela 11-B. Condições de Atendimento Diurno - Redes Municipais do Estado do {$estado}";
        }
    }

    protected function getSql($co_rede, $uf)
    {
        $sql = "
                SELECT nu_ano,
                       alunos_turma_ei_urbana || ';' || alunos_turma_ei_rural || ';' ||
                       alunos_turma_ai_ef_urbana || ';' || alunos_turma_ai_ef_rural || ';' ||
                       alunos_turma_af_ef_urbana || ';' || alunos_turma_af_ef_rural || ';' ||
                       alunos_turma_em_urbana || ';' || alunos_turma_em_rural || ';' ||
                       alunos_turma_eja_ef_urbana || ';' || alunos_turma_eja_ef_rural AS turma,

                      horas_aula_ei_urbana || ';' ||   horas_aula_ei_rural || ';' ||
                      horas_aula_ai_ef_urbana || ';' ||   horas_aula_ai_ef_rural || ';' ||
                      horas_aula_af_ef_urbana || ';' ||   horas_aula_af_ef_rural || ';' ||
                      horas_aula_em_urbana || ';' ||   horas_aula_em_rural || ';' ||
                      horas_aula_eja_ef_urbana || ';' ||  horas_aula_eja_ef_rural AS hora

                FROM ide2016.tb_ide_tabela11a_uf_mec tab11
                LEFT JOIN territorios.estado est ON est.estcod = tab11.co_uf::varchar(2)
               WHERE co_rede = {$co_rede} AND est.estuf = '{$uf}'group by nu_ano,turma,hora ORDER by nu_ano
            
        ";
//        ver($sql);
        return $sql;
    }

/*
    protected function getColunaCabecalho()
    {
        return array(
//            'Indicador' => array('rowspan' => 2, 'class' => 'text-center', 'style' => 'padding-top: 35px;'),
            'Ano' => array('rowspan' => 2, 'colspan' => 1, 'class' => 'text-center', 'style' => 'padding-top: 35px;'),
            'Educação Infantil' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
            'Anos Iniciais do Ensino Fundamental' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
            'Anos Finais do Ensino Fundamental' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
            'Ensino Médio' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
            'EJA - Anos Iniciais do Ensino Fundamental' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
        );
    }

    protected function getAtributosTabela()
    {
        return array(
//            'indicador' => array(),
            'nu_ano' => array(),
            'alunos_turma_ei_rural' => array('class' => 'text-center'),
            'alunos_turma_ei_urbana' => array('class' => 'text-center'),
            'alunos_turma_ai_ef_rural' => array('class' => 'text-center'),
            'alunos_turma_ai_ef_urbana' => array('class' => 'text-center'),
            'alunos_turma_af_ef_rural' => array('class' => 'text-center'),
            'alunos_turma_af_ef_urbana' => array('class' => 'text-center'),
            'alunos_turma_em_rural' => array('class' => 'text-center'),
            'alunos_turma_em_urbana' => array('class' => 'text-center'),
            'alunos_turma_eja_ef_rural' => array('class' => 'text-center'),
            'alunos_turma_eja_ef_urbana' => array('class' => 'text-center'),
        );
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
        for ($i = 0; $i <= 4; $i++) {
            $this->tabelaArvore->addCell('Rural', '', 'header', array('class' => 'text-center alert alert-success'));
            $this->tabelaArvore->addCell('Urbana', '', 'header', array('class' => 'text-center alert alert-success'));
        }
    }*/

    public function getTabela($rede, $estuf)
    {
        $sql = $this->getSql($rede, $estuf);
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
            $this->tabelaArvore->addCell($serie_ano, $class, 'data', array('rowspan' => $dado['cont'], 'class' => 'text-center', 'style' => 'vertical-align:middle;'));


            foreach ($dado as $ano => $areas) {

                if (is_array($areas)) {

                    //add serie
                    $this->tabelaArvore->addCell($ano, $class, 'data', array('class' => 'text-center', 'style' => 'vertical-align:middle;'));

                    foreach ($areas as $area => $indicadores) {

                        if (is_array($indicadores)) {
                            $indicadores = array_filter($indicadores);
                            $countInd = count($indicadores);

                            if (empty($indicadores)) {
                                // add ano
                                $this->tabelaArvore->addCell($area, $class, 'data', array('rowspan' => ($countInd == 0 ? 1 : $countInd), 'class' => 'text-center', 'style' => 'vertical-align:middle;'));
                                $this->tabelaArvore->addCell('', $class, 'data', array('colspan' => 10));
                                $this->tabelaArvore->addRow();
                            } else {
                                // add ano
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
            }
        }
    }

    protected function getColunaCabecalho()
    {
        return array(
            'Indicador' => array('rowspan' => 2, 'class' => 'text-center', 'style' => 'vertical-align:middle;'),
            'Ano' => array('rowspan' => 2, 'class' => 'text-center', 'style' => 'vertical-align:middle;'),
            'Educação Infantil' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
            'Anos Iniciais do Ensino Fundamental' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
            'Anos Finais do Ensino Fundamental' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
            'Ensino Médio' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
            'EJA - Anos Iniciais do Ensino Fundamental' => array('colspan' => 2, 'class' => 'text-center', 'style' => ''),
        );
    }

    protected function organizarDados($dados)
    {
        foreach ($dados as $key => $valor) {
            $this->agrupaDados($valor, 'turma', 'Média de alunos por turma');
            $this->agrupaDados($valor, 'hora', 'Média de horas-aula diárias');
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
        for ($i = 0; $i <= 4; $i++) {
            $this->tabelaArvore->addCell('Rural', '', 'header', array('class' => 'text-center alert alert-success'));
            $this->tabelaArvore->addCell('Urbana', '', 'header', array('class' => 'text-center alert alert-success'));
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