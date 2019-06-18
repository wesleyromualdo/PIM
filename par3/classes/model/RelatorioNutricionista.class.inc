<?php
class Par3_Model_RelatorioNutricionista extends Par3_Model_Relatorio_Abstract
{
    public $optionsColumns = [
        'estuf'                  => 'UF',
        'mundescricao'           => 'Municípios',
        'inudescricao'           => 'Descrição',
        'entcpf'                 => 'CPF',
        'entnome'                => 'Nome',
        'dancrn'                 => 'CRN',
        'dancrnuf'               => 'CRN-UF',
        'vnid'                   => 'Vinculação',
        'vndatavinculacao'       => 'Data da Vinculação',
        'sndescricao'            => 'Situação',
        'danemailprincipal'      => 'E-mail Principal',
        'danemailalternativo'    => 'E-mail Alternativo',
        'dantelefonefixo'        => 'Tel. Comercial',
        'dantelefonecelular'     => 'Tel. Celular',
        'esfera'                 => 'Esfera',
        'tendsc'                 => 'Cargo/função',
        'dntvdescricao'          => 'Vínculo',
        'vndatadesvinculacao'    => 'Data da Desvinculação'
    ];

    public function __construct()
    {
        parent::__construct();
    }

    public function buildDataGrid($xlsDownload = FALSE) {
        $tipoRelatorio = ($xlsDownload) ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;

        $tratamentoListaVazia = ($xlsDownload) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;

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

    public function setWhere(array $fromPost)
    {
        if(!empty($fromPost['itrid'])) {
            $this->_where[] = "vTable.itrid IN ({$fromPost['itrid']})";
            unset($fromPost['itrid']);
        }

        foreach ($fromPost as $key => $value) {
            if (!empty($value)) {
                $this->_where[] = "vTable.{$key} = '{$value}'";
            }
        }
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

    protected function _getQuery(){
        $strSQL = <<<DML
            SELECT DISTINCT
                %s
            FROM (
                    SELECT  ine.estuf,
                            vn.tenid,
                            mun.mundescricao,
                            ine.inudescricao,
                            int.entcpf,
                            ine.itrid,
                            int.entnome,
                            dan.dancrn,
                            dan.dancrnuf,
                            vn.vnid,
                            vn.vnstatus,
                            vn.vndatavinculacao,
                            vn.vndatadesvinculacao,
                            sn.sndescricao,
                            dan.danemailprincipal,
                            dan.danemailalternativo,
                            dan.dantelefonefixo,
                            dan.dantelefonecelular,
                            CASE WHEN ine.itrid = 2
                                THEN 'Municipal'
                                ELSE 'Estadual'
                            END AS esfera,
                            tipo.tendsc,
                            dntvdescricao

                    FROM par3.instrumentounidade_entidade int

                    INNER JOIN seguranca.usuario usu ON int.entcpf = usu.usucpf
                    INNER JOIN par3.instrumentounidade ine ON int.inuid = ine.inuid
                    INNER JOIN par3.vinculacaonutricionista vn ON vn.tenid = int.tenid AND vn.vncpf = int.entcpf AND vn.inuid = int.inuid AND int.entstatus = 'A'
                    LEFT  JOIN par3.dadosnutricionistatipovinculo dnv ON dnv.dntvid = vn.dntvid
                    INNER JOIN par3.situacaonutricionista sn ON sn.snid = vn.snid
                    INNER JOIN par3.tipoentidade tipo ON tipo.tenid = int.tenid
                    LEFT  JOIN territorios.municipio mun ON mun.muncod = ine.muncod
                    LEFT  JOIN par3.dadosnutricionista dan ON dan.dancpf = vn.vncpf

                    WHERE int.entstatus = 'A' AND int.tenid IN (7,8,16)
            ) AS vTable
            %s
DML;
        return $strSQL;
    }

    protected $_listagem;

}