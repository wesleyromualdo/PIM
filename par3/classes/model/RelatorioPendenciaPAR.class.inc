<?php
class Par3_Model_RelatorioPendenciaPAR extends Par3_Model_Relatorio_Abstract
{
    public $optionsColumns = array('estuf' => 'UF', 'municipio' => 'Município', 'habilitacao' => 'Habilitação');
    public $relatorio = '';
    public $sql = '';

    public function __construct()
    {
        parent::__construct();
    }

    public function buildDataGrid($xlsDownload = FALSE)
    {
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
        if ($_REQUEST['relatorio'] == 'monitoramentoPAR2011'){
            $this->_listagem->addAcao('plus', array('func' => 'detalhesPendenciasMonitoramento')); 
        }
        $this->_listagem->render($tratamentoListaVazia);
    }

    private function addInputFilters()
    {
        foreach ($this->getFilters() as $k => $filter) {
            $this->_listagem->addCampo([
                'id' => $k . '-fields',
                'name' => 'cols[]',
                'type' => 'hidden',
                'value' => $filter
            ]);
        }

        if (count($_POST['sfiltro']) > 0) {
            foreach ($_POST['sfiltro'] as $key => $filtro) {
                $this->_listagem->addCampo([
                    'id' => $k . '-filter',
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
    }

    public function getInuid($arrPost = array()){

    }

    private function addCallbackDeCampo()
    {
    }

    protected function _getQuery()
    {
        $strSQL = $this->sql;
        return $strSQL;
    }

    public function setRelatorio($arrPost)
    {
        $this->relatorio = $arrPost['relatorio'];

        switch ($this->relatorio) {
            case 'obrasPar':
                $this->optionsColumns = array('estuf' => 'UF', 'descricao' => 'Município', 'esddsc' => 'Situação', 'obrid' => 'ID Obra', 'predescricao' => 'Nome da Obra', 'situacao' => 'Pendência');
                $_POST['cols'] = array('estuf', 'descricao', 'esddsc', 'obrid', 'predescricao', 'situacao');
                $this->sql = $this->montaSQLObrasPar($arrPost);
                $this->__construct();
                break;
            case 'habilitacao':
                $this->optionsColumns = array('estuf' => 'UF', 'municipio' => 'Município', 'esddsc' => 'Situação', 'habilitacao' => 'Habilitação');
                $_POST['cols'] = array('estuf', 'municipio', 'esddsc', 'habilitacao');
                $this->sql = $this->montaSQLHabilitacao($arrPost);
                $this->__construct();
                break;
            case 'cae': //Conselho de Alimentação Escolar
                $this->optionsColumns = array('estuf' => 'UF', 'municipio' => 'Município', 'esddsc' => 'Situação', 'cae' => 'Conselho de Alimentação Escolar');
                $_POST['cols'] = array('estuf', 'municipio', 'esddsc', 'cae');
                $this->sql = $this->montaSQLCAE($arrPost);
                $this->__construct();
                break;
            case 'cacs': //Conselho de Alimentação Escolar
                $this->optionsColumns = array('estuf' => 'UF', 'municipio' => 'Município', 'esddsc' => 'Situação', 'cacs' => 'Conselho de Acompanhamento de Controle Social');
                $_POST['cols'] = array('estuf', 'municipio', 'esddsc', 'cacs');
                $this->sql = $this->montaSQLCACS($arrPost);
                $this->__construct();
                break;
            case 'prestacaoContas':
                $this->optionsColumns = array('estuf' => 'UF', 'municipio' => 'Município', 'esddsc' => 'Situação', 'prestacaoContas' => 'Prestação de Contas');
                $_POST['cols'] = array('estuf', 'municipio', 'esddsc', 'prestacaoContas');
                $this->sql = $this->montaSQLprestacaoContas($arrPost);
                $this->__construct();

                break;
            case 'pme':
                $legendaInstrumento = $arrPost['esfera'] == 'E'?'Estadual':'Municipal';
                $this->optionsColumns = array('estuf' => 'UF', 'municipio' => 'Município', 'esddsc' => 'Situação', 'pme' => "Plano {$legendaInstrumento} de Educação");
                $_POST['cols'] = array('estuf', 'municipio', 'esddsc', 'pme');
                $this->sql = $this->montaSQLPNE($arrPost);
                $this->__construct();
                break;
            case 'siope':
                $this->optionsColumns = array('estuf' => 'UF', 'municipio' => 'Município', 'esddsc' => 'Situação', 'siope' => "SIOPE");
                $_POST['cols'] = array('estuf', 'municipio', 'esddsc', 'siope');
                $this->sql = $this->montaSQLSIOPE($arrPost);
                $this->__construct();
                break;
        }
    }

    public function montaSQLObrasPar($arrPost)
    {
        $where = array('1=1');

        if($arrPost['esfera']) $where[] = "pen.empesfera = '{$arrPost['esfera']}'";
        if($arrPost['estuf'])  $where[] = "pen.estuf = '{$arrPost['estuf']}'";
        if($arrPost['muncod']) $where[] = "pen.muncod = '{$arrPost['muncod']}'";
        
        if($arrPost['esdid'] != '' && $arrPost['esdid'] != '0'){
            $where[]        = "doc.esdid = '{$arrPost['esdid']}'";
            $innerDoc       = " INNER JOIN workflow.documento       doc ON doc.docid = inu.docid
                                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
            $colunaSituacao = "esd.esddsc,";
        }else{
            if($arrPost['esdid'] != '') $where[] = "inu.docid IS NULL";
            $colunaSituacao = "coalesce(esd.esddsc, 'Diagnóstico Não Iniciado') as esddsc,";
            $innerDoc       = " LEFT  JOIN workflow.documento       doc ON doc.docid = inu.docid
                                LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
        }

// pre.obrid as btn
        $sql = "SELECT 
                    pen.estuf,
                    pen.descricao,
                    $colunaSituacao
                    pre.obrid,
                    pre.predescricao,
                    pen.situacao
                FROM par3.instrumentounidade  inu
                LEFT JOIN obras2.vm_total_pendencias pen 
                    INNER JOIN obras.preobra            pre ON pre.preid = pen.preid
                    ON inu.estuf = pen.estuf AND inu.muncod = pen.muncod
                $innerDoc
                WHERE ".implode(' AND ', $where)."
                ORDER BY 1,2,3,5";
//         ver($sql);
        return $sql;
    }

    public function montaSQLHabilitacao($arrPost)
    { 
        $where = array('1=1');
        if ($arrPost['esfera'] == 'E')      $where[] = "inu.itrid IN (1,3)";
        if ($arrPost['esfera'] == 'M')      $where[] = "inu.itrid IN (2)";
        if ($arrPost['pendencia'] == 'P')   $where[] = "vm.habilitacao IS TRUE";
        if ($arrPost['pendencia'] == 'N')   $where[] = "vm.habilitacao IS NOT TRUE";
        if ($arrPost['estuf'])              $where[] = "inu.estuf = '{$arrPost['estuf']}'";
        if ($arrPost['muncod'])             $where[] = "inu.muncod = '{$arrPost['muncod']}'";
        
        if($arrPost['esdid'] != '' && $arrPost['esdid'] != '0'){
            $where[]        = "doc.esdid = '{$arrPost['esdid']}'";
            $innerDoc       = " INNER JOIN workflow.documento       doc ON doc.docid = inu.docid
                                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
            $colunaSituacao = "esd.esddsc,";
        }else{
            if($arrPost['esdid'] != '') $where[] = "inu.docid IS NULL";
            $colunaSituacao = "coalesce(esd.esddsc, 'Diagnóstico Não Iniciado') as esddsc,";
            $innerDoc       = " LEFT  JOIN workflow.documento       doc ON doc.docid = inu.docid
                                LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
        }

        $sql = "SELECT 
                	inu.estuf,
                	CASE WHEN inu.itrid = 2
                		THEN inu.inudescricao
                		ELSE ' - '
                	END as municipio,
                    $colunaSituacao
                	CASE WHEN vm.habilitacao
                		THEN 'Entidade Não Habilitada'
                		ELSE 'Entidade Habilitada'
                	END as habilitacao
                FROM par3.instrumentounidade  inu
                LEFT JOIN par3.vm_relatorio_quantitativo_pendencias vm ON inu.inuid = vm.inuid
                $innerDoc
                WHERE
                    " . implode(' AND ', $where) . "
                ORDER BY
                    estuf,
                    municipio";

        return $sql;
    }

    public function montaSQLCAE($arrPost)
    {
        $where = array('1=1');
        if ($arrPost['esfera'] == 'E')      $where[] = "inu.itrid IN (1,3)";
        if ($arrPost['esfera'] == 'M')      $where[] = "inu.itrid IN (2)";
        if ($arrPost['pendencia'] == 'P')   $where[] = "vm.cae IS TRUE";
        if ($arrPost['pendencia'] == 'N')   $where[] = "vm.cae IS NOT TRUE";
        if ($arrPost['estuf'])              $where[] = "inu.estuf = '{$arrPost['estuf']}'";
        if ($arrPost['muncod'])             $where[] = "inu.muncod = '{$arrPost['muncod']}'";
        
        if($arrPost['esdid'] != '' && $arrPost['esdid'] != '0'){
            $where[]        = "doc.esdid = '{$arrPost['esdid']}'";
            $innerDoc       = " INNER JOIN workflow.documento       doc ON doc.docid = inu.docid
                                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
            $colunaSituacao = "esd.esddsc,";
        }else{
            if($arrPost['esdid'] != '') $where[] = "inu.docid IS NULL";
            $colunaSituacao = "coalesce(esd.esddsc, 'Diagnóstico Não Iniciado') as esddsc,";
            $innerDoc       = " LEFT  JOIN workflow.documento       doc ON doc.docid = inu.docid
                                LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
        }

        $sql = "SELECT
                	inu.estuf,
                	CASE WHEN inu.itrid = 2
                		THEN inu.inudescricao
                		ELSE ' - '
                	END as municipio,
                    $colunaSituacao
                	CASE WHEN vm.cae
                		THEN 'A etapa de preenchimento do CAE NÃO encontra-se preenchida.'
                		ELSE 'A etapa de preenchimento do CAE encontra-se preenchida.'
                	END as habilitacao
                FROM par3.instrumentounidade  inu
                LEFT JOIN par3.vm_relatorio_quantitativo_pendencias vm ON inu.inuid = vm.inuid
                $innerDoc
                WHERE
                    " . implode(' AND ', $where) . "
                ORDER BY
                    estuf,
                    municipio";

        return $sql;
    }

    public function montaSQLprestacaoContas($arrPost)
    {
        $where = array('1=1');
        if ($arrPost['esfera'] == 'E')      $where[] = "inu.itrid IN (1,3)";
        if ($arrPost['esfera'] == 'M')      $where[] = "inu.itrid IN (2)";
        if ($arrPost['pendencia'] == 'P')   $where[] = "vm.contas IS TRUE";
        if ($arrPost['pendencia'] == 'N')   $where[] = "vm.contas IS NOT TRUE";
        if ($arrPost['estuf'])              $where[] = "inu.estuf = '{$arrPost['estuf']}'";
        if ($arrPost['muncod'])             $where[] = "inu.muncod = '{$arrPost['muncod']}'";
        
        if($arrPost['esdid'] != '' && $arrPost['esdid'] != '0'){
            $where[]        = "doc.esdid = '{$arrPost['esdid']}'";
            $innerDoc       = " INNER JOIN workflow.documento       doc ON doc.docid = inu.docid
                                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
            $colunaSituacao = "esd.esddsc,";
        }else{
            if($arrPost['esdid'] != '') $where[] = "inu.docid IS NULL";
            $colunaSituacao = "coalesce(esd.esddsc, 'Diagnóstico Não Iniciado') as esddsc,";
            $innerDoc       = " LEFT  JOIN workflow.documento       doc ON doc.docid = inu.docid
                                LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
        }

        $sql = "SELECT
                	inu.estuf,
                	CASE WHEN inu.itrid = 2
                		THEN inu.inudescricao
                		ELSE ' - '
                	END as municipio,
                    $colunaSituacao
                	CASE WHEN vm.contas
                		THEN 'A etapa de preenchimento de Prestação de Contas NÃO encontra-se preenchida.'
                		ELSE 'A etapa de preenchimento de Prestação de Contas encontra-se preenchida.'
                	END as habilitacao
                FROM par3.instrumentounidade  inu
                LEFT JOIN par3.vm_relatorio_quantitativo_pendencias vm ON inu.inuid = vm.inuid
                $innerDoc
                WHERE
                    " . implode(' AND ', $where) . "
                ORDER BY
                    estuf,
                    municipio";
        return $sql;
    }

    public function montaSQLCACS($arrPost)
    {
        $where = array('1=1');
        if ($arrPost['esfera'] == 'E')      $where[] = "inu.itrid IN (1,3)";
        if ($arrPost['esfera'] == 'M')      $where[] = "inu.itrid IN (2)";
        if ($arrPost['pendencia'] == 'P')   $where[] = "vm.cacs IS TRUE";
        if ($arrPost['pendencia'] == 'N')   $where[] = "vm.cacs IS NOT TRUE";
        if ($arrPost['estuf'])              $where[] = "inu.estuf = '{$arrPost['estuf']}'";
        if ($arrPost['muncod'])             $where[] = "inu.muncod = '{$arrPost['muncod']}'";
        
        if($arrPost['esdid'] != '' && $arrPost['esdid'] != '0'){
            $where[]        = "doc.esdid = '{$arrPost['esdid']}'";
            $innerDoc       = " INNER JOIN workflow.documento       doc ON doc.docid = inu.docid
                                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
            $colunaSituacao = "esd.esddsc,";
        }else{
            if($arrPost['esdid'] != '') $where[] = "inu.docid IS NULL";
            $colunaSituacao = "coalesce(esd.esddsc, 'Diagnóstico Não Iniciado') as esddsc,";
            $innerDoc       = " LEFT  JOIN workflow.documento       doc ON doc.docid = inu.docid
                                LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
        }

        $sql = "SELECT
                	inu.estuf,
                	CASE WHEN inu.itrid = 2
                		THEN inu.inudescricao
                		ELSE ' - '
                	END as municipio,
                    $colunaSituacao
                	CASE WHEN vm.cacs
                		THEN 'A etapa de preenchimento do Conselho de Acompanhamento de Controle Social NÃO encontra-se preenchida.'
                		ELSE 'A etapa de preenchimento do Conselho de Acompanhamento de Controle Social encontra-se preenchida.'
                	END as habilitacao
                FROM par3.instrumentounidade  inu
                LEFT JOIN par3.vm_relatorio_quantitativo_pendencias vm ON inu.inuid = vm.inuid
                $innerDoc
                WHERE
                    " . implode(' AND ', $where) . "
                ORDER BY
                    estuf,
                    municipio";
        return $sql;
    }

    public function montaSQLPNE($arrPost)
    {
        $where = array('1=1');
        if ($arrPost['esfera'] == 'E')      $where[] = "inu.itrid IN (1,3)";
        if ($arrPost['esfera'] == 'M')      $where[] = "inu.itrid IN (2)";
        if ($arrPost['pendencia'] == 'P')   $where[] = "vm.pne IS TRUE";
        if ($arrPost['pendencia'] == 'N')   $where[] = "vm.pne IS NOT TRUE";
        if ($arrPost['estuf'])              $where[] = "inu.estuf = '{$arrPost['estuf']}'";
        if ($arrPost['muncod'])             $where[] = "inu.muncod = '{$arrPost['muncod']}'";
        
        if($arrPost['esdid'] != '' && $arrPost['esdid'] != '0'){
            $where[]        = "doc.esdid = '{$arrPost['esdid']}'";
            $innerDoc       = " INNER JOIN workflow.documento       doc ON doc.docid = inu.docid
                                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
            $colunaSituacao = "esd.esddsc,";
        }else{
            if($arrPost['esdid'] != '') $where[] = "inu.docid IS NULL";
            $colunaSituacao = "coalesce(esd.esddsc, 'Diagnóstico Não Iniciado') as esddsc,";
            $innerDoc       = " LEFT  JOIN workflow.documento       doc ON doc.docid = inu.docid
                                LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
        }

        $legenda = $arrPost['esfera'] == 'E'? 'Estadual':'Municipal';
        $sql = "SELECT
                	inu.estuf,
                	CASE WHEN inu.itrid = 2
                		THEN inu.inudescricao
                		ELSE ' - '
                	END as municipio,
                    $colunaSituacao
                	CASE WHEN vm.pne
                		THEN 'A etapa de preenchimento do Plano {$legenda} de educação NÃO encontra-se preenchida.'
                		ELSE 'A etapa de preenchimento do Plano {$legenda} de educação encontra-se preenchida.'
                	END as habilitacao
                FROM par3.instrumentounidade  inu
                LEFT JOIN par3.vm_relatorio_quantitativo_pendencias vm ON inu.inuid = vm.inuid
                $innerDoc
                WHERE
                    " . implode(' AND ', $where) . "
                ORDER BY
                    estuf,
                    municipio";
        return $sql;
    }

    public function montaSQLSIOPE($arrPost)
    {
        $where = array('1=1');
        if($arrPost['esfera'] == 'E')      $where[] = "inu.itrid IN (1,3)";
        if($arrPost['esfera'] == 'M')      $where[] = "inu.itrid IN (2)";
        if($arrPost['pendencia'] == 'P')   $where[] = "vm.siope IS TRUE";
        if($arrPost['pendencia'] == 'N')   $where[] = "vm.siope IS NOT TRUE";
        if($arrPost['estuf'])              $where[] = "inu.estuf = '{$arrPost['estuf']}'";
        if($arrPost['muncod'])             $where[] = "inu.muncod = '{$arrPost['muncod']}'";
        
        if($arrPost['esdid'] != '' && $arrPost['esdid'] != '0'){
            $where[]        = "doc.esdid = '{$arrPost['esdid']}'";
            $innerDoc       = " INNER JOIN workflow.documento       doc ON doc.docid = inu.docid
                                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
            $colunaSituacao = "esd.esddsc,";
        }else{
            if($arrPost['esdid'] != '') $where[] = "inu.docid IS NULL";
            $colunaSituacao = "coalesce(esd.esddsc, 'Diagnóstico Não Iniciado') as esddsc,";
            $innerDoc       = " LEFT  JOIN workflow.documento       doc ON doc.docid = inu.docid
                                LEFT  JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid";
        }

        $sql = "SELECT
                	inu.estuf,
                	CASE WHEN inu.itrid = 2
                		THEN inu.inudescricao
                		ELSE ' - '
                	END as municipio,
                    $colunaSituacao
                	UPPER(vm.siope_dsc) as siope
                FROM par3.instrumentounidade  inu
                LEFT JOIN par3.vm_relatorio_quantitativo_pendencias vm ON inu.inuid = vm.inuid
                $innerDoc
                WHERE
                    " . implode(' AND ', $where) . "
                ORDER BY
                    estuf,
                    municipio";
        return $sql;
    }

     

    protected $_listagem;

}