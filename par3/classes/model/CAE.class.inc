<?php
/**
 * Classe de mapeamento da entidade par3.cae.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade par3.cae.
 *
 * @see Modelo
 */
class Par3_Model_CAE extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.cae';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = [
        'caeid',
    ];

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = [
        'entid' => ['tabela' => 'par3.instrumentounidade_entidade', 'pk' => 'entid'],
    ];

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = [
        'caeid' => null,
        'entid' => null,
        'caestatus' => null,
        'caenomeacao' => null,
        'caenomeacaooutro' => null,
        'caedocumento' => null,
        'caedtinicio' => null,
        'caedttermino' => null,
        'caedtexclusao' => null,
        'arqid' => null
    ];

    /**
     * @param $inuid
     * @return object
     */
    public function carregarDadosCAE($inuid)
    {
        $sql = sprintf('
            SELECT 
              *, 
              i.entid
            FROM 
              ' . $this->stNomeTabela . ' c
              LEFT JOIN par3.instrumentounidade_entidade i 
                ON i.entid = c.entid
              LEFT JOIN par3.endereco e 
                ON e.endid = i.endid
            WHERE
              i.inuid = %d
              AND c.caestatus = \'A\'
              AND i.tenid =  %d',
            $inuid,
            Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CAE
        );

        $arDados = $this->pegaLinha($sql, 0, 30);
        return (object) $arDados;
    }

    /**
     * @param $inuid
     * @return object
     */
    public function carregarDadosPresidenteCAE($inuid)
    {
        $sql = sprintf('
            SELECT 
              *, 
              i.entid
            FROM 
              par3.cae_presidente c
              LEFT JOIN par3.instrumentounidade_entidade i ON i.entid = c.entid
              LEFT JOIN par3.endereco e ON e.endid = i.endid
            WHERE
              i.inuid = %d
              AND i.entstatus = \'A\'
              AND i.tenid = %d',
            $inuid,
            Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_PRESIDENTE_CAE
        );

        $arDados = $this->pegaLinha($sql, 0, 30);
        return (object) $arDados;
    }

    /**
     * @param $inuid
     * @return array|mixed|NULL
     */
    public function carregarConselheirosCAE($inuid)
    {
        $sql = sprintf('
            SELECT
              ent.*,
              MAX(arqid) AS arqid
            FROM
              par3.instrumentounidade_entidade ent
              LEFT JOIN par3.cae_conselheiro con ON con.entid = ent.entid
            WHERE
              ent.inuid = %d
              AND ent.tenid = %d
              AND ent.entstatus = \'A\'
            GROUP BY
              ent.entid
            ',
            $inuid,
            Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CONSELHEIRO_CAE
        );

        $arDados = $this->carregar($sql, 0, 30);
        return $arDados;
    }

    /**
     * @param $caeid
     * @throws Exception
     */
    public function inativaCAE($caeid)
    {
        $sql = sprintf('
            UPDATE 
              ' . $this->stNomeTabela . ' 
            SET 
              caestatus = \'I\' 
            WHERE 
              caeid = %d',
            $caeid
        );
        $this->executar($sql);
    }

    /**
     * @param $usucpf
     * @return bool|object
     */
    public function carregarConselheiroCPF($usucpf)
    {
        $sql = sprintf('
            SELECT
              c.caeid, 
              c.caestatus, 
              c.caenomeacao, 
              c.caenomeacaooutro, 
              c.caedocumento,
              cc.*,
              i.*
            FROM 
              par3.cae c
              LEFT JOIN par3.cae_conselheiro cc ON cc.caeid = c.caeid
              LEFT JOIN par3.instrumentounidade_entidade i ON i.entid = cc.entid
              LEFT JOIN par3.endereco e ON e.endid = i.endid
            WHERE 
              i.entcpf = \'%s\'
              AND i.entstatus = \'A\'
              AND i.tenid = %d',
            $usucpf,
            Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CONSELHEIRO_CAE
        );

        $retorno = false;
        $arDados = $this->pegaLinha($sql);

        if ($arDados) {
            $retorno = (object) $arDados;
        }

        return $retorno;
    }

    /**
     * @todo refatorar código para desacoplar métodos de listagem e de detalhes pois esta query está sendo utilizada para ambos e pode ser otimizada para seu fim específico
     *
     * @param array $dados
     * @param null $entid
     * @return string
     */
    public function getSqlListaCae(array $dados, $entid = null)
    {
        $where = $this->getWhereLista($dados, $entid);

        $whereStr = '';

        if(count($where) > 0){
            $whereStr = ' AND ' . implode(' AND  ', $where);
        }

        $campos = 'ent.entid as id, ent.entid, entcpf, entnome, entemail, sub.arqid, \'Em Análise\'';

        if(!is_null($entid)){
            $campos = 'ent.*, c.*, cc.*, sub.arqid, \'Em Análise\'';
        }

        $sql = sprintf('
            SELECT 
              %s
            FROM 
              par3.instrumentounidade_entidade  ent
              INNER JOIN par3.cae_conselheiro cc ON cc.entid = ent.entid
              INNER JOIN par3.cae c ON c.caeid = cc.caeid
              INNER JOIN par3.instrumentounidade  inu ON inu.inuid  = ent.inuid
              LEFT JOIN (
                SELECT 
                  caeid, 
                  entid, 
                  max(arqid) AS arqid 
                FROM 
                  par3.cae_conselheiro conselheiro 
                GROUP BY 
                  caeid, 
                  entid, 
                  arqid 
              ) AS sub ON sub.entid = ent.entid
            WHERE
              ent.tenid = %d
              AND ent.entstatus = \'A\'
              %s
            ',
            $campos,
            Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CONSELHEIRO_CAE,
            $whereStr
        );
        return $sql;
    }

    /**
     * @param array $arrPost
     * @param null $entid
     * @return array
     */
    private function getWhereLista(array $arrPost = [], $entid = null)
    {
        $where = [];

        if (!is_null($entid)) {
            $where[] = 'ent.entid = ' . $entid;
        }

        if (array_key_exists('entnome', $arrPost) && !empty($arrPost['entnome'])) {
            $where[] = 'ent.entnome ILIKE \'%'. $arrPost['entnome'] . '%\'';
        }

        if (array_key_exists('entemail', $arrPost) && !empty($arrPost['entemail'])) {
            $where[] = 'ent.entemail ILIKE \'%' . $arrPost['entemail'] . '%\'';
        }

        if (array_key_exists('entcpf', $arrPost) && !empty($arrPost['entcpf'])) {
            $where[] = 'entcpf = \'' . $arrPost['entcpf'] . '\'';
        }

        if (array_key_exists('entcpf', $arrPost) && !empty($arrPost['entcpf'])) {
            $where[] = 'ent.entcpf = \'' . $arrPost['entcpf'] . '\'';
        }

        if (array_key_exists('entdtnascimento', $arrPost) && !empty($arrPost['entdtnascimento'])) {
            $where[] = 'ent.entdtnascimento = \'' . formata_data_sql($arrPost['entdtnascimento']) . '\'';
        }

        if (array_key_exists('enttelefonecomercial', $arrPost) && !empty($arrPost['enttelefonecomercial'])) {
            $where[] = 'ent.enttelefonecomercial = \'' . $arrPost['enttelefonecomercial'] . '\'';
        }

        if (array_key_exists('entsexo', $arrPost) && !empty($arrPost['entsexo'])) {
            $where[] = 'ent.entsexo = \'' . $arrPost['entsexo'] . '\'';
        }

        if (array_key_exists('cctid', $arrPost) && !empty($arrPost['cctid'])) {
            $where[] = 'ent.cctid = \'' . $arrPost['cctid'] . '\'';
        }

        if (array_key_exists('cscid', $arrPost) && !empty($arrPost['cscid'])) {
            $where[] = 'ent.cscid = \'' . $arrPost['cscid'] . '\'';
        }

        if (array_key_exists('entnivelensino', $arrPost) && !empty($arrPost['entnivelensino'])) {
            $where[] = 'ent.entnivelensino = \'' . $arrPost['entnivelensino'] . '\'';
        }

        if (array_key_exists('cacfuncao', $arrPost) && !empty($arrPost['cacfuncao'])) {
            $where[] = 'cc.cacfuncao = \'' . $arrPost['cacfuncao'] . '\'';
        }

        if (array_key_exists('cacmandato', $arrPost) && !empty($arrPost['cacmandato'])) {
            $where[] = 'cc.cacmandato = \''. $arrPost['cacmandato'] . '\'';
        }

        if (array_key_exists('cacoutroconselho', $arrPost) && !empty($arrPost['cacoutroconselho'])) {
            $where[] = 'cc.cacoutroconselho = \'' . $arrPost['cacoutroconselho'] . '\'';
        }

        if (array_key_exists('cacocupacao', $arrPost) && !empty($arrPost['cacocupacao'])) {
            $where[] = 'cc.cacocupacao = \'' . $arrPost['cacocupacao'] . '\'';
        }

        if (array_key_exists('caedocumento', $arrPost) && !empty($arrPost['caedocumento'])) {
            $where[] = 'c.caedocumento = \'' . $arrPost['caedocumento'] . '\'';
        }

        if (array_key_exists('caenomeacao', $arrPost) && !empty($arrPost['caenomeacao'])) {
            $where[] = 'c.caenomeacao = \'' . $arrPost['caenomeacao'] . '\'';
        }

        if (array_key_exists('caestatus', $arrPost) && !empty($arrPost['caestatus'])) {
            $where[] = 'c.caestatus = \'' . $arrPost['caestatus'] . '\'';
        }

        if (array_key_exists('anexo', $arrPost)) {
            if ($arrPost['anexo'] == 'S') {
                $where[] = 'sub.arqid IS NULL';
            } elseif ($arrPost['anexo'] == 'N') {
                $where[] = 'sub.arqid IS NOT NULL';
            }
        }

        if(array_key_exists('listaMunicipio', $arrPost) && !empty($arrPost['listaMunicipio'])) {
            $stringmuncod = '';

            foreach($arrPost['listaMunicipio'] as $key => $value) {
                $stringmuncod .= '\'' . $value . '\'';

                if(!(count($arrPost['listaMunicipio']) - 1 == $key)) {
                    $stringmuncod .= ',';
                }
            }

            if($arrPost['estuf']) {
                $where[] = '(inu.estuf = \'' . $arrPost['estuf'] . '\' OR inu.muncod in (' . $stringmuncod . '))';
            } else {
                $where[] = 'inu.muncod in (' . $stringmuncod . ')';
            }
        } elseif( $arrPost['estuf']) {
            $where[] = 'inu.estuf = \'' . $arrPost['estuf'] . '\'';
        }

        return $where;
    }

    /**
     * @param $caeid
     * @return bool|mixed|NULL|string
     * @throws Exception
     */
    public function criarDocidCae($caeid)
    {
        $docdsc = 'Fluxo Validação de Cadastros CAE - caeid' . $caeid;
        $docid = wf_cadastrarDocumento(TPDID_VALIDACAO_CAE, $docdsc);
        $sql = sprintf('UPDATE par3.cae SET docid= %d WHERE dopid = %d', $docid, $caeid);
        $this->executar($sql);
        $this->commit();
        return $docid;
    }

    /**
     * @param $dopid
     * @return array|bool|mixed|NULL
     */
    public function getDocIdCae($dopid)
    {
        $sql = sprintf('SELECT docid FROM par.documentopar WHERE dopid = %d', $dopid);
        $docid = $this->pegaLinha($sql);

        if ($docid) {
            $docid = $docid['docid'];
        }

        return $docid;
    }
}