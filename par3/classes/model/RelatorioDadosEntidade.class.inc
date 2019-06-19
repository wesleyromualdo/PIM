<?php

class Par3_Model_RelatorioDadosEntidade extends Par3_Model_Relatorio_Abstract
{
    public function __construct()
    {
        parent::__construct();
    }

    public function setWhere(array $fromPost)
    {
        foreach ($fromPost as $key => $value) {
            $namespace = '';
            foreach ($this->optionsColumnsAlias as $k => $v) {
                $tmpkey = explode('.', $k);
                if ($key == $tmpkey[1]) {
                    $namespace = $tmpkey[0];
                    break;
                }
            }
            if (!empty($value) && !empty($namespace)) {
                $this->_where[] = "{$namespace}.{$key} = '{$value}'";
            }
        }
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
        if (array_search('prefeito.entcpf', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('entcpf', 'formatar_cpf');
        }

        if (array_search('prefeitura.entcnpj', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('entcnpj', 'formatar_cnpj');
        }

        if (array_search('prefeito.enttelefonecomercial', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('telprefeito', 'formatar_telefone');
        }

        if (array_search('prefeito.enttelefonecelular', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('celprefeito', 'formatar_telefone');
        }

        if (array_search('prefeitura.enttelefonecomercial', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('telprefeitura', 'formatar_telefone');
        }

        if (array_search('prefeitura.enttelefonecelular', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('celprefeitura', 'formatar_telefone');
        }

        if (array_search('prefeito.endcep', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('cepprefeito', 'formatar_cep');
        }

        if (array_search('prefeitura.endcep', $this->getFilters())) {
            $this->_listagem->addCallbackDeCampo('cepprefeitura', 'formatar_cep');
        }
    }

    protected function _getQuery()
    {
        $strSQL = <<<DML
            WITH q_pref AS (SELECT entid,
                                   inuid,
                                   UPPER(entnome) AS entnome,
                                   entcpf,
                                   entemail,
                                   enttelefonecomercial,
                                   enttelefonecelular,
                                   endid,
                                   entsexo
                              FROM par3.instrumentounidade_entidade
                              WHERE tenid = 2 AND entstatus = 'A'),
                 q_enti AS (SELECT entid,
                                   inuid,
                                   UPPER(entnome) AS entnome,
                                   entcnpj,
                                   entemail,
                                   enttelefonecomercial,
                                   enttelefonecelular,
                                   endid
                              FROM par3.instrumentounidade_entidade
                              WHERE tenid = 1 AND entstatus = 'A'),
                 q_ender AS (SELECT endid,
                                    endcep,
                                    endlogradouro,
                                    endcomplemento,
                                    endnumero,
                                    endbairro,
                                    entid
                                FROM par3.endereco),
                 q_instr AS (SELECT i.inuid,
                                    i.itrid,
                                    i.estuf,
                                    i.muncod,
                                    t.mundescricao,
                                    i.inustatus
                          FROM par3.instrumentounidade i
                          INNER JOIN territorios.municipio t ON (t.muncod = i.muncod)
                          WHERE i.inustatus = 'A')
               SELECT
                    %s
                    FROM (SELECT *
                        FROM (SELECT max(entid) AS entid,
                               entcpf
                          FROM q_pref
                          GROUP BY entcpf) prefeito_por_cpf
                        INNER JOIN q_pref USING(entid, entcpf)
                        INNER JOIN q_ender USING(endid, entid)
                        INNER JOIN q_instr USING(inuid)
                        ORDER BY entnome) prefeito
                    INNER JOIN (SELECT *
                      FROM (SELECT max(entid) AS entid,
                                       inuid
                                  FROM q_enti
                                  GROUP BY inuid) prefeitura_por_cnpj
                            INNER JOIN q_enti USING(entid, inuid)
                            INNER JOIN q_ender USING(endid, entid)
                            INNER JOIN q_instr USING(inuid)
                          ORDER BY entnome) prefeitura USING(inuid)
                %s
DML;
        return $strSQL;
    }

    protected $_listagem;
    public $optionsColumns = [
        'prefeito.entnome'                  => 'Nome Prefeito',
        'prefeito.entsexo'                  => 'Sexo',
        'prefeito.entcpf'                   => 'CPF',
        'prefeito.entemail'                 => 'E-mail',
        'prefeito.enttelefonecomercial'     => 'Tel. Comercial',
        'prefeito.enttelefonecelular'       => 'Tel. Celular',
        'prefeito.endcep'                   => 'CEP',
        'prefeito.endlogradouro'            => 'Logradouro',
        'prefeito.endcomplemento'           => 'Complemento',
        'prefeito.endnumero'                => 'Número',
        'prefeito.endbairro'                => 'Bairro',

        'prefeitura.entnome'                => 'Prefeitura',
        'prefeitura.entcnpj'                => 'CNPJ',
        'prefeitura.entemail'               => 'E-mail',
        'prefeitura.enttelefonecomercial'   => 'Tel. Comercial',
        'prefeitura.enttelefonecelular'     => 'Cel. Comercial',
        'prefeitura.endcep'                 => 'CEP',
        'prefeitura.endlogradouro'          => 'Logradouro',
        'prefeitura.endcomplemento'         => 'Complemento',
        'prefeitura.endnumero'              => 'Número',
        'prefeitura.endbairro'              => 'Bairro',
        'prefeitura.estuf'                  => 'UF',
        'prefeitura.mundescricao'           => 'Município'
    ];

    public $optionsColumnsAlias = [
        'prefeito.entnome'                  => 'nomeprefeito',
        'prefeito.entsexo',
        'prefeito.entcpf',
        'prefeito.entemail'                 => 'emailprefeito',
        'prefeito.enttelefonecomercial'     => 'telprefeito',
        'prefeito.enttelefonecelular'       => 'celprefeito',
        'prefeito.endcep'                   => 'cepprefeito',
        'prefeito.endlogradouro'            => 'logprefeito',
        'prefeito.endcomplemento'           => 'complprefeito',
        'prefeito.endnumero'                => 'numprefeito',
        'prefeito.endbairro'                => 'bairroprefeito',

        'prefeitura.entnome'                => 'prefeitura',
        'prefeitura.entcnpj',
        'prefeitura.entemail'               => 'emailprefeitura',
        'prefeitura.enttelefonecomercial'   => 'telprefeitura',
        'prefeitura.enttelefonecelular'     => 'celprefeitura',
        'prefeitura.endcep'                 => 'cepprefeitura',
        'prefeitura.endlogradouro'          => 'logprefeitura',
        'prefeitura.endcomplemento'         => 'complprefeitura',
        'prefeitura.endnumero'              => 'numprefeitura',
        'prefeitura.endbairro'              => 'bairroprefeitura',
        'prefeitura.estuf'                  => 'estufprefeitura',
        'prefeitura.mundescricao'           => 'mundescricaoprefeitura'
    ];
}