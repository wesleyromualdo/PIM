<?php
/**
 * Classe de mapeamento da entidade ide2016.tb_ide_tabela6_simec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */



/**
 * Tb_ide_tabela6_simec
 *
 * @category Class
 * @package  A1
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */
class Par3_Model_Ide_TbIdeTabela6Simec extends Modelo
{
    protected $tabelaArvore;
    protected $titulo;

    public function __construct($id = null)
    {
        parent::__construct();
        $this->tabelaArvore = new HTML_Table('table_tabela11a_uf', 'table table-condensed table-bordered  table-responsive');
    }

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'ide2016.tb_ide_tabela6_simec';

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
        'nu_ano' => null,
        'nu_educacao_infantil_rural' => null,
        'nu_educacao_infantil_urbana' => null,
        'nu_ensino_fundamental_rural' => null,
        'nu_ensino_fundamental_urbana' => null,
        'nu_ensino_medio_rural' => null,
        'nu_ensino_medio_urbana' => null,
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
            'nu_ano' => array( 'allowEmpty' => true ),
            'nu_educacao_infantil_rural' => array( 'allowEmpty' => true ),
            'nu_educacao_infantil_urbana' => array( 'allowEmpty' => true ),
            'nu_ensino_fundamental_rural' => array( 'allowEmpty' => true ),
            'nu_ensino_fundamental_urbana' => array( 'allowEmpty' => true ),
            'nu_ensino_medio_rural' => array( 'allowEmpty' => true ),
            'nu_ensino_medio_urbana' => array( 'allowEmpty' => true ),
            'data_atualizacao' => array( 'allowEmpty' => true ),
        );
    }//end getCamposValidacao($dados)

    public function getTitulo($rede, $muncod)
    {
        $municipio = $this->pegaUm(" SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$muncod}' ");

        if($rede == 1){
            return "Tabela 6-A. Número de Escolas por Etapa de Ensino - Rede Estadual em {$municipio}";
        }elseif($rede == 2){
            return "Tabela 6-B. Número de Escolas por Etapa de Ensino - Rede Municipal em {$municipio}";
        }
    }

    protected function getSql($co_rede, $uf, $muncod)
    {
        $sql = "
                SELECT nu_ano,
                       nu_educacao_infantil_rural, nu_educacao_infantil_urbana, (nu_educacao_infantil_rural+nu_educacao_infantil_urbana) as soma_educacao_infantil,
                       nu_ensino_fundamental_rural, nu_ensino_fundamental_urbana, (nu_ensino_fundamental_rural + nu_ensino_fundamental_urbana) as soma_ensino_fundamental,
                       nu_ensino_medio_rural, nu_ensino_medio_urbana, (nu_ensino_medio_rural + nu_ensino_medio_urbana) as soma_ensino_medio
                FROM ide2016.tb_ide_tabela6_simec tab6
                LEFT JOIN territorios.municipio mun ON mun.muncod::integer = tab6.co_ibge
                WHERE co_rede = {$co_rede} AND mun.estuf = '{$uf}' AND mun.muncod = '{$muncod}'
        ";
        return $sql;
    }

    public function getTabela($rede, $estuf, $muncod)
    {
        $sql = $this->getSql($rede, $estuf, $muncod);
        $dados = $this->carregar($sql);

        if (!empty($dados)) {
            $this->setHeader();
            $this->setBody($dados);
            echo $this->tabelaArvore->display();
        } else {
            echo '<div class="alert alert-warning">
                   Não possui resultados para esta pesquisa.
                </div>';
        }
    }

    protected function setBody($dados)
    {
        $atributos = $this->getAtributosTabela();
        $this->tabelaArvore->addTSection('tbody');

        foreach ($dados as $key => $dado) {
            $this->tabelaArvore->addRow();
            foreach ($atributos as $campo => $atributo) {
                $valor = $dado[$campo];

                $this->tabelaArvore->addCell($valor, $class, 'data', $atributo);
            }
        }
    }

    protected function getColunaCabecalho()
    {
        return array(
//            'Indicador' => array('rowspan' => 2, 'class' => 'text-center', 'style' => 'padding-top: 35px;'),
            'Ano' => array('rowspan' => 2, 'colspan' => 1, 'class' => 'text-center', 'style' => 'vertical-align:middle;'),
            'Educação Infantil' => array('colspan' => 3, 'class' => 'text-center', 'style' => ''),
            'Ensino Fundamental' => array('colspan' => 3, 'class' => 'text-center', 'style' => ''),
            'Ensino Médio' => array('colspan' => 3, 'class' => 'text-center', 'style' => ''),
        );
    }

    protected function getAtributosTabela()
    {
        return array(
//            'indicador' => array(),
            'nu_ano' => array('class' => 'text-center'),
            'nu_educacao_infantil_rural' => array('class' => 'text-center'),
            'nu_educacao_infantil_urbana' => array('class' => 'text-center'),
            'soma_educacao_infantil' => array('class' => 'text-center'),
            'nu_ensino_fundamental_rural' => array('class' => 'text-center'),
            'nu_ensino_fundamental_urbana' => array('class' => 'text-center'),
            'soma_ensino_fundamental' => array('class' => 'text-center'),
            'nu_ensino_medio_rural' => array('class' => 'text-center'),
            'nu_ensino_medio_urbana' => array('class' => 'text-center'),
            'soma_ensino_medio' => array('class' => 'text-center'),
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
        for ($i = 0; $i <=2; $i++) {
            $this->tabelaArvore->addCell('Rural', '', 'header', array('class' => 'text-center alert alert-success'));
            $this->tabelaArvore->addCell('Urbana', '', 'header', array('class' => 'text-center alert alert-success'));
            $this->tabelaArvore->addCell('Total', '', 'header', array('class' => 'text-center alert alert-success'));
        }
    }

}//end Class
?>