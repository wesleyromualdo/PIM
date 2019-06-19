<?php
/**
 * Classe de controle da VinculacaoNutricionista
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-01-2016
 * @link     no link
 */

/**
 * Par3_Controller_VinculacaoNutricionista
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 06-01-2016
 * @link     no link
 */
class Par3_Controller_VinculacaoNutricionista extends Controle
{

    private $model;
    public function __construct()
    {
        $this->model = new Par3_Model_VinculacaoNutricionista();
        parent::__construct();
    }


    /**
     * Função quadroResumo
     * - monta o formulario de quadro resumo

     * @return string quadro resumo.
     *
     */
    public function quadroResumo($arrPost)
    {
        $vinculo  = new Par3_Model_VinculacaoNutricionista();
        $arrDados = $vinculo->recuperarDadosQuadroResumo();

        require_once APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/quadroResumoNutricionista.php';
    }//end quadroResumo()


    /**
     * Função listaVinculos
     * - monta a lista de vinculos
     *
     * @return string lista de vinculos.
     *
     */
    public function listaVinculos($arrPost, $readonly = false)
    {
        $vinculo  = new Par3_Model_VinculacaoNutricionista();
        $arrDados = $vinculo->recuperarDadosListaVinculos($arrPost);
        $arrDados = is_array($arrDados) ? $arrDados : array();

        include_once APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/listaVinculosNutricionista.php';
    }//end listaVinculos()


    /**
     * Função listaVinculos
     * - monta a lista de vinculos
     *
     * @return string lista de vinculos.
     *
     */
    public function imprimirXlsUnidadesVinculo($arrPost)
    {
        $vinculo  = new Par3_Model_VinculacaoNutricionista();
        $arrDados = $vinculo->recuperarDadosXlsUnidadesVinculo($arrPost);

        include_once APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/imprimirXlsUnidadesVinculo.php';
    }//end listaVinculos()


    /**
     * Função gerarTermoRT
     * - monta o termo RT
     *
     * @return string termoRT.
     *
     */
    public function gerarTermoRT($arrPost)
    {
        global $simec;

        $vinculo   = new Par3_Model_VinculacaoNutricionista($arrPost['vnid']);
        $vinculo->vndatavinculacao      = $arrPost['vndatavinculacao'];
        $vinculo->vncargahorariasemanal = $arrPost['vncargahorariasemanalo'];
        $vinculo->vnatuacaoexclusivaei  = "{$arrPost['vnatuacaoexclusivaei']}";
        $vinculo->dntvid                = $arrPost['dntvido'];
        $vinculo->salvar();
        $vinculo->commit();
        $unidade   = new Par3_Model_InstrumentoUnidade($vinculo->inuid);
        $municipio = new Territorios_Model_Municipio($unidade->muncod);
        $uf        = new Territorios_Model_Estado($unidade->estuf);

        include_once APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/termoRT.php';
    }//end gerarTermoRT()

    public function desenhaGrafico($tipo)
    {
        $sql = '';
        $id = '';
        switch ($tipo) {
            case 1:
                $sql = $this->model->countTipoNutricionistas();
                $id = 'countTipoNutricionistas';
                break;
            case 2:
                $sql = $this->model->countSituacaoNutricionistas();
                $id = 'countSituacaoNutricionistas';
                break;
            case 3:
                $sql = $this->model->countSituacaoEntidades();
                $id = 'countSituacaoEntidades';
                break;
            default:
                break;
        }
        if ($sql) {
            $clsGraficoQuantidadeObras = new Grafico(Grafico::K_TIPO_PIZZA, false);
            $clsGraficoQuantidadeObras
                ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
                ->setId($id)
                ->setPointEvent(array('legendItemClick' => "teste('" . $id . "',event.target);"))
                ->gerarGrafico($sql);
        }
    }

    public function carregarGraficos()
    {
        include_once APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/graficos-panel.php';
    }

    public function gerarRelatorioXlsEntidades($arrDados = array())
    {
        $arrEntidade = $this->model->carregarRelatorioEntidadesPorSituacao($arrDados);
        if (!$arrEntidade) {
            simec_redirecionar(
                'par3.php?modulo=principal/acompanhamento/analisarNutricionista&acao=A',
                'error',
                'O relatório não trouxe nenhum resultado'
            );
        }
        $arrEntidade = array_filter($arrEntidade);
        if (count($arrEntidade) == 0) {
            simec_redirecionar(
                'par3.php?modulo=principal/acompanhamento/analisarNutricionista&acao=A',
                'error',
                'O relatório não trouxe nenhum resultado'
            );
        }
//        ver($arrEntidade,d);
        include_once APPRAIZ.'par3/modulos/principal/acompanhamento/nutricionista/relatorioEntidade.php';
    }

    public function gerarCsv($arrPost)
    {
        header('Content-Encoding: ISO-8859-1');
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Cache-Control: no-cache, must-revalidate");
        header("Pragma: no-cache");
        header('Content-Type: text/csv;charset= ISO-8859-1');
        header('Content-Disposition: attachment; filename=exportacao-cfn'.date('Ymdhms').'.csv');

        $output = fopen('php://output', 'w');

        $arrColunas = array(
            'Estado',
            'Esfera',
            'Código IBGE',
            'Unidade',
            'CPF',
            'Nome',
            'CRN',
            'CRN-UF',
            'Cargo/Função',
            'Vínculo',
            'Situação',
            'Código da Situação',
            'Validação CFN',
            'Modelo de Validação',
            'Registro/CFN',
            'Situação Problema'
        );
        fputcsv($output, $arrColunas, ';');
        $arrDados = $this->model->recuperarDadosListaVinculos($arrPost);
        $arrDados = is_array($arrDados) ? $arrDados : array();
        foreach ($arrDados as $row) {
            $arrRow = [
                $row['estuf'],
                ($row['muncod']?'Municipal':'Estadual'),
                $row['muncod'],
                trim($row['mundescricao']),
                $row['vncpf'],
                trim($row['usunome']),
                $row['dancrn'],
                $row['dancrnuf'],
                trim($row['tendsc']),
                trim($row['dntvdescricao']),
                trim($row['sndescricao']),
                $row['snid'],
                '',
                '',
                '',
                ''
            ];
            fputcsv($output, $arrRow, ';');
            unset($arrRow);
        }
    }
}
