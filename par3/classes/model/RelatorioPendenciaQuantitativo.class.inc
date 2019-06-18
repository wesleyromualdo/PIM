<?php
class Par3_Model_RelatorioPendenciaQuantitativo extends Par3_Model_Relatorio_Abstract
{
    public $optionsColumns = [
        'estuf'                  => 'UF',
        'mundescricao'           => 'Município',
        'esddsc'                 => 'Situação',
        'habilitacao'            => 'Habilitação',
        'obras_par'              => 'Pendência de Obras do PAR',
        'cacs'                   => 'Cacs',
        'cae'                    => 'CAE',
        'contas'                 => 'Prestação de Contas',
        'monitoramento_par'      => 'Monitoramento PAR',
        'pne'                    => 'PME/PEE',
        'siope'                  => 'SIOPE'

    ];

    public function __construct()
    {
        parent::__construct();
    }

    public function buildDataGrid($xlsDownload = FALSE) {
        $tipoRelatorio = ($xlsDownload) ? Simec_Listagem::RELATORIO_XLS : Simec_Listagem::RELATORIO_PAGINADO;

        $tratamentoListaVazia = ($xlsDownload) ?
            Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
        $arrCabecalho = $this->getHeader();

        if($_REQUEST['sfiltro']['itrid'] == '1') {
            unset($arrCabecalho[1]);
        }
        
        $sql = $this->buildQuery();
        $this->_listagem = new Simec_Listagem($tipoRelatorio);
        $this->_listagem->setCabecalho($arrCabecalho);
        $this->_listagem->setQuery($sql);
        $this->_listagem->setTamanhoPagina(100000);
        $this->_listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $this->_listagem->turnOnOrdenacao('estuf');
        if($_REQUEST['sfiltro']['itrid'] == '1') {
            $this->_listagem->esconderColuna('mundescricao');
        }
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

        if (count($_REQUEST['sfiltro']) > 0) {
            foreach ($_REQUEST['sfiltro'] as $key => $filtro) {
                $this->_listagem->addCampo([
                    'id' => $k.'-filter',
                    'name' => "sfiltro[{$key}]",
                    'type' => 'hidden',
                    'value' => $filtro
                ]);
            }
        }
        $this->_listagem->setFormFiltros('filtroRelatorioPendencias');
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
    
    public function setWhere(array $fromPost)
    {
        if(!empty($fromPost['itrid']) && $fromPost['itrid'] != 0) {
            if($fromPost['itrid'] == 1) {
                $this->_where[] = "vTable.itrid IN (1,3)";
            }else {
                $this->_where[] = "vTable.itrid = 2";
            }
            unset($fromPost['itrid']);
        }

        if($fromPost['esdid'] && count($fromPost['esdid']) != 0 ) {
            $arrEsdid = $fromPost['esdid'];
            $where = " vTable.esdid in (".implode(',',$arrEsdid).")";
            $naoIniciado = array_filter($arrEsdid,function ($esdid) {
                    return $esdid == 0;
            });
            //valida se foi marcada a opção de diagnóstico não iniciado
            if($naoIniciado) {
                $arrEsdid = array_filter($arrEsdid);
                $where = "(vTable.esdid in (".implode(',',$arrEsdid).") OR vTable.docid IS NULL) ";
            }
            $this->_where[] = $where ;
        }
        unset($fromPost['esdid']);

        if(isset($fromPost['pendencia']) && !empty($fromPost['pendencia'])) {
            foreach($fromPost['pendencia'] as $key => $value) {
                if (!empty($value)) {
                    //                     $value = ($value);
                    $this->_where[] = "vTable.{$key} = '{$value}'";
                }
            }
            unset($fromPost['pendencia']);
        }

        if($fromPost['mundescricao'] && count($fromPost['mundescricao']) != 0 ) {
            $arrMuncod = tratarArrayParaMultiSelect($fromPost['mundescricao']);
            if(count($arrMuncod) != 0) {
                $this->_where[] = " vTable.muncod::integer in (".implode(',',$arrMuncod).")";
            }
        }
        unset($fromPost['mundescricao']);

        if($fromPost['estuf'] && count($fromPost['estuf']) != 0 ) {
            $arrEstuf = implode(',',array_map(function($estuf){return (string)"'".$estuf."'";},$fromPost['estuf']));
            $this->_where[] = $fromPost['estuf'] ? " vTable.estuf in ({$arrEstuf})":'';
        }
        unset($fromPost['estuf']);

        foreach ($fromPost as $key => $value) {
            if (!empty($value)) {
                $this->_where[] = "vTable.{$key} = '{$value}'";
            }
        }
    }

    protected function _getQuery(){
        $strSQL = <<<DML
            SELECT
                %s
            FROM (
                    SELECT  ine.itrid,
                            ine.docid,
                            doc.esdid,
                            ine.estuf,
                            (case when ine.itrid = 1 then ' - ' else ine.inudescricao end) as mundescricao,
                            esd.esddsc,
                            ine.muncod,
                            CASE WHEN
							    pendencias.habilitacao = 'f'
                                THEN
                                    'Sem Pendência'
                                ELSE
                                    'Com Pendência'
                                END as habilitacao,
                            CASE WHEN
							    pendencias.obras_par = 'f'
                                THEN
                                    'Sem Pendência'
                                ELSE
                                    'Com Pendência'
                            END as obras_par,
                            CASE WHEN
							    pendencias.cacs = 'f'
                                THEN
                                    'Sem Pendência'
                                ELSE
                                    'Com Pendência'
                            END as cacs,
                            CASE WHEN
							    pendencias.cae = 'f'
                                THEN
                                    'Sem Pendência'
                                ELSE
                                    'Com Pendência'
                            END as cae,
                            CASE WHEN
							    pendencias.contas = 'f'
                                THEN
                                    'Sem Pendência'
                                ELSE
                                    'Com Pendência'
                            END as contas,
                            CASE WHEN
							    pendencias.monitoramento_par = 'f'
                                THEN
                                    'Sem Pendência'
                                ELSE
                                    'Com Pendência'
                            END as monitoramento_par,
                            CASE WHEN
							    pendencias.pne = 'f'
                                THEN
                                    'Sem Pendência'
                                ELSE
                                    'Com Pendência'
                            END as pne,
                            CASE WHEN
							    pendencias.siope = 'f'
                                THEN
                                    'Sem Pendência'
                                ELSE
                                    'Com Pendência'
                            END as siope
                    FROM par3.instrumentounidade ine 
                    LEFT  JOIN par3.vm_relatorio_quantitativo_pendencias pendencias ON pendencias.inuid = ine.inuid
                    LEFT  JOIN workflow.documento       doc ON doc.docid = ine.docid
                    LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid

            ) AS vTable
            %s
            ORDER BY vTable.estuf, vTable.mundescricao

DML;
        return $strSQL;
    }

    protected $_listagem;

}

