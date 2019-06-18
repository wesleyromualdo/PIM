<?php

class Par3_Model_RelatorioCAE extends Par3_Model_Relatorio_Abstract
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
        $this->_listagem->esconderColunas(['tenid']);
        if (!$xlsDownload && count($this->getFilters())) {
            $this->addInputFilters();
            $this->addCallbackDeCampo();
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

    private function addCallbackDeCampo()
    {
        if (array_search('vTable.enttelefonecomercial', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('enttelefonecomercial', 'formatar_telefone');
        }

        if (array_search('vTable.enttelefonecelular', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('enttelefonecelular', 'formatar_telefone');
        }
    }

    protected function _getQuery()
    {
        $strSQL = <<<DML
            SELECT
                %s
            FROM (
                (
                    (
                        SELECT DISTINCT
                            inu.estuf, municipio.mundescricao, inu.inudescricao,
                            ent.entnome, ent.entemail, ent.enttelefonecomercial, ent.enttelefonecelular,
                            inu.itrid,
                            tipo.tendsc,
                            tipo.tenid
                        FROM par3.instrumentounidade_entidade  ent
                        INNER JOIN par3.instrumentounidade     inu ON inu.inuid  = ent.inuid
                        JOIN par3.tipoentidade                tipo ON (tipo.tenid = ent.tenid)
                        LEFT JOIN territorios.municipio  municipio ON (municipio.muncod = inu.muncod)
                        LEFT JOIN (SELECT caeid, entid, MAX(arqid) AS arqid
                                FROM par3.cae_conselheiro conselheiro GROUP BY caeid, entid, arqid
                            ) AS sub ON (sub.entid = ent.entid)
                        WHERE
                            ent.tenid in (13, 14)
                            AND ent.entstatus = 'A'
                    )
                    UNION ALL
                    (
                        SELECT DISTINCT
                            inu.estuf, municipio.mundescricao, inu.inudescricao,
                            ent.entnome, ent.entemail, ent.enttelefonecomercial, ent.enttelefonecelular,
                            inu.itrid,
                            tipo.tendsc,
                            tipo.tenid
                        FROM par3.instrumentounidade_entidade   ent
                        INNER JOIN par3.instrumentounidade      inu ON inu.inuid  = ent.inuid
                        JOIN par3.tipoentidade                 tipo ON (tipo.tenid = ent.tenid)
                        LEFT JOIN territorios.municipio   municipio ON (municipio.muncod = inu.muncod)
                        LEFT JOIN (SELECT caeid, entid, MAX(arqid) AS arqid
                                FROM par3.cae_conselheiro conselheiro GROUP BY caeid, entid, arqid
                            ) AS sub ON (sub.entid = ent.entid)
                        WHERE
                            ent.tenid in (13, 14)
                            AND ent.entstatus = 'A'
                    )
                )
            ) AS vTable
            %s
DML;
        return $strSQL;
    }

    protected $_listagem;
    public $optionsColumns = [
        'vTable.estuf'                  => 'UF',
        'vTable.mundescricao'           => 'Municípios',
        'vTable.inudescricao'           => 'Descrição',
        'vTable.entnome'                => 'Nome',
        'vTable.entemail'               => 'E-mail',
        'vTable.enttelefonecomercial'   => 'Tel. Comercial',
        'vTable.enttelefonecelular'     => 'Cel. Comercial',
        "CASE WHEN (vTable.itrid = 2) THEN 'Municipal' ELSE 'Estadual' END AS itrid"           => 'Esfera',
        'vTable.tendsc'                 => 'Cargo/função',
        'vTable.tenid'                  => ''
    ];
}