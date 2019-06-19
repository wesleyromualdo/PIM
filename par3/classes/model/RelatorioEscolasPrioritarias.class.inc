<?php

class Par3_Model_RelatorioEscolasPrioritarias extends Par3_Model_Relatorio_Abstract
{
    public function __construct()
    {
        parent::__construct();
    }

    public function buildDataGrid($xlsDownload = FALSE)
    {
        $tipoRelatorio = ($xlsDownload)
            ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;

        $tratamentoListaVazia = ($xlsDownload)
            ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

        $this->_listagem = new Simec_Listagem($tipoRelatorio);
        $this->_listagem->setCabecalho($this->getHeader());
        $this->_listagem->setQuery($this->buildQuery());
        $this->_listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $this->_listagem->turnOnOrdenacao('estuf');
        $this->_listagem->esconderColunas(['tp_dependencia', 'ibge']);
        if (!$xlsDownload && count($this->getFilters())) {
            $this->addInputFilters();
        }
        $this->_listagem->render($tratamentoListaVazia);
    }

    private function addInputFilters()
    {
        foreach ($this->getFilters() as $k => $filter) {
            $this->_listagem->addCampo([
                'id' => $k.'-fields',
                'name' => 'cols[]',
                'type' => 'hidden',
                'value' => $filter
            ]);
        }

        if (count($_POST['sfiltro']) > 0) {
            foreach ($_POST['sfiltro'] as $key => $filtro) {
                $this->_listagem->addCampo([
                    'id' => $k.'-filter',
                    'name' => "sfiltro[{$key}]",
                    'type' => 'hidden',
                    'value' => $filtro
                ]);
            }
        }
        $this->_listagem->setFormFiltros('filtroRelatorio');
    }

    protected function _getQuery()
    {
        $strSQL = <<<DML
        SELECT %s FROM (
            SELECT DISTINCT
                iu.inuid,
                ep.pepescola as escola, ep.co_entidade as codescola,
                ep.estuf,
                iu.inudescricao,
                iu.muncod as ibge,
                ep.tp_dependencia ,
                CASE WHEN ep.tp_dependencia = 3 THEN 'Municipal' ELSE 'Estadual' END  AS esfera,
                CASE WHEN pa.adpresposta = '' OR pa.adpresposta IS NULL THEN 'Pendente' ELSE pa.adpresposta END AS "aceitouadesao"
            FROM par3.instrumentounidade iu
            LEFT JOIN par3.prodesaoprograma pa INNER JOIN par3.proadesao p ON (pa.pfaid = p.pfaid AND p.pfaid = 4) ON (iu.inuid = pa.inuid )
            INNER JOIN par3.proadesao_lista_escolas_prioritarias ep ON
                (ep.muncod = iu.muncod AND ep.tp_dependencia = 3 AND itrid = 2)  OR (ep.estuf = iu.estuf AND ep.tp_dependencia = 2 AND itrid in (1,3))
        ) AS vTable
        %s
DML;
        return $strSQL;
    }

    protected $_listagem;
    public $optionsColumns = [
        'vTable.escola'          => 'Escolas',
        'vTable.codescola'       => 'Código da Entidade',
        'vTable.estuf'           => 'UF',
        'vTable.inudescricao'    => 'Descrição',
        'vTable.esfera'          => 'Município',
        'vTable.aceitouadesao'   => 'Aceite adesão',
        'vTable.ibge'            => '',
    ];
}