<?php
/**
 * Classe de mapeamento da entidade spoemendas.solicitacaofinanceira.
 *
 * @version $Id$
 * @since   2017.04.13
 */

/**
 * Spoemendas_Model_Solicitacaofinanceira: sem descricao
 *
 * @package Spoemendas\Model
 * @uses    Simec\Db\Modelo
 * @author  Victor Eduardo Barreto <victor.barreto@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Solicitacaofinanceira($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Solicitacaofinanceira($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int    $gnd
 * @property int    $ptrid
 * @property string $exercicio
 * @property int    $emeid
 * @property int    $autid
 * @property string $unicod
 * @property int    $docid
 * @property int    $sfngrauprioridade
 * @property string $sfnobjetivo
 * @property int    $sfnmequipamento
 * @property string $sfnnumeroreferencia        - default: NULL::character varying
 * @property string $sfnnumeroreferenciaoutros  - default: NULL::character varying
 * @property string $sfnconveniosiafi           - default: NULL::character varying
 * @property string $sfnpropostasiconv          - default: NULL::character varying
 * @property string $sfncontratorepasse         - default: NULL::character varying
 * @property string $muncod
 * @property string $estuf
 * @property int    $sfnfontedetalhada
 * @property int    $sfncodvinculacao
 * @property int    $sfngrupodespesa
 * @property string $sfnnotaempenho
 * @property string $sfnugexecutora
 * @property int    $sfninteressado
 * @property int    $sfnid                      - default: nextval('spoemendas.solicitacaofinanceira_sfnid_seq'::regclass)
 */
class Spoemendas_Model_Solicitacaofinanceira extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.solicitacaofinanceira';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = [
        'sfnid',
    ];

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = [
        'ptrid'          => ['tabela' => 'monitora.ptres', 'pk' => 'ptrid'],
        'emeid'          => ['tabela' => 'emenda.emenda', 'pk' => 'emeid'],
        'sfninteressado' => ['tabela' => 'emenda.autor', 'pk' => 'autid'],
        'autid'          => ['tabela' => 'emenda.autor', 'pk' => 'autid'],
        'gnd'            => ['tabela' => 'gnd', 'pk' => 'gndcod'],
        'exercicio'      => ['tabela' => 'spoemendas.programacaoexercicio', 'pk' => 'prsano'],
        'docid'          => ['tabela' => 'workflow.documento', 'pk' => 'docid'],
        'muncod'         => ['tabela' => 'territorios.municipio', 'pk' => 'muncod'],
        'estuf'          => ['tabela' => 'territorios.estado', 'pk' => 'estuf'],
    ];

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = [
        'ptrid'                     => null,
        'exercicio'                 => null,
        'emeid'                     => null,
        'autid'                     => null,
        'unicod'                    => null,
        'docid'                     => null,
        'sfngrauprioridade'         => null,
        'sfnobjetivo'               => null,
        'sfnmequipamento'           => null,
        'sfnnumeroreferencia'       => null,
        'sfnnumeroreferenciaoutros' => null,
        'sfnconveniosiafi'          => null,
        'sfnpropostasiconv'         => null,
        'sfncontratorepasse'        => null,
        'muncod'                    => null,
        'estuf'                     => null,
        'sfnfontedetalhada'         => null,
        'sfncodvinculacao'          => null,
        'sfngrupodespesa'           => null,
        'sfnnotaempenho'            => null,
        'sfnugexecutora'            => null,
        'sfninteressado'            => null,
        'sfnid'                     => null,
        'sfnultimaedicao'           => null,
        'sfnstatus'                 => null
    ];

    /**
     * Validators.
     *
     * @param mixed[] $dados
     *
     * @return mixed[]
     */
    public function getCamposValidacao($dados = [])
    {
        return [
            'gnd'                       => ['Digits'],
            'ptrid'                     => ['Digits'],
            'exercicio'                 => [new Zend_Validate_StringLength(['max' => 4])],
            'emeid'                     => ['Digits'],
            'autid'                     => ['Digits'],
            'unicod'                    => [new Zend_Validate_StringLength(['max' => 5])],
            'docid'                     => ['allowEmpty' => true, 'Digits'],
            'sfngrauprioridade'         => ['allowEmpty' => true, 'Digits'],
            'sfnobjetivo'               => [new Zend_Validate_StringLength(['max' => 250])],
            'sfnmequipamento'           => ['allowEmpty' => true, 'Digits'],
            'sfnnumeroreferencia'       => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 150])],
            'sfnnumeroreferenciaoutros' => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 150])],
            'sfnconveniosiafi'          => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 150])],
            'sfnpropostasiconv'         => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 150])],
            'sfncontratorepasse'        => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 150])],
            'muncod'                    => [new Zend_Validate_StringLength(['max' => 7])],
            'estuf'                     => [new Zend_Validate_StringLength(['max' => 2])],
            'sfnfontedetalhada'         => ['Digits'],
            'sfncodvinculacao'          => ['Digits'],
            'sfngrupodespesa'           => ['Digits'],
            'sfnnotaempenho'            => [new Zend_Validate_StringLength(['max' => 10])],
            'sfnugexecutora'            => [new Zend_Validate_StringLength(['max' => 6])],
            'sfninteressado'            => ['allowEmpty' => true, 'Digits'],
            'sfnid'                     => ['Digits'],
        ];
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    /**
     * Método responsável por recuperar combo de unidade
     *
     * @author Victor Eduardo Barreto
     *
     * @param string $usucpf Identificador do Usuario
     *
     * @return mixed Dados para o combo
     */
    public function comboUnidade($usucpf = null, $json = true)
    {
        $sql = "
            SELECT unicod AS codigo, unicod || ' - ' || unidsc AS descricao
            FROM unidade
            WHERE orgcod = '26000'
            ORDER BY unicod";

        if ($usucpf && (in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral()) || in_array(PFL_UO_EQUIPE_FINANCEIRA, pegaPerfilGeral()))) {
            $sql = <<<DML
                SELECT DISTINCT 
                    uni.unicod AS codigo,
                    uni.unicod || ' - ' || uni.unidsc AS descricao
                FROM public.unidade uni
                INNER JOIN public.unidadegestora ung USING(unicod)
                INNER JOIN spoemendas.usuarioresponsabilidade rpu USING(ungcod)
                WHERE rpu.usucpf = '%s'
                AND rpu.rpustatus = 'A'
                AND uni.orgcod = '26000'
                ORDER BY uni.unicod;
DML;
            $sql = sprintf($sql, $usucpf);

            $ret = $this->carregar($sql);

            if (!$ret) {
                $ret = [];
            }

            if ($json) {
                $ret = simec_json_encode(
                    formatarCombo($ret, 'codigo', 'descricao'));
            }

            return $ret;
        }

        if ($json) {
            return simec_json_encode(
                formatarCombo($this->carregar($sql), 'codigo', 'descricao'));
        }

        return $sql;
    }

    /**
     * Método responsável por recuperar combo de autor
     *
     * @author Victor Eduardo Barreto
     *
     * @param int $unidade Identificador de unidade
     *
     * @return mixed Dados para o combo
     */
    public function comboAutor($unidade = null, $exercicio = null, $json = true)
    {
        $sql = "
            SELECT aut.autid AS codigo, aut.autnome || ' (' || par.parsigla || ') - Cód: ' || autcod AS descricao
            FROM emenda.autor aut
            JOIN emenda.partido par ON aut.parid = par.parid
            WHERE aut.autstatus = 'A'
            ORDER BY aut.autnome";

        if ($unidade && $exercicio) {
            $sql = "
                SELECT DISTINCT aut.autid AS codigo, aut.autnome || ' (' || par.parsigla || ')' AS descricao
                FROM emenda.emenda eme
                INNER JOIN emenda.autor aut using(autid)
                INNER JOIN emenda.partido par ON aut.parid = par.parid
                INNER JOIN monitora.acao using(acaid)
                WHERE emeano = '{$exercicio}'
                AND unicod = '{$unidade}'
                AND aut.autstatus = 'A'
                ORDER BY descricao";

            $ret = $this->carregar($sql);

            if ($json) {
                $ret = simec_json_encode(
                    formatarCombo($ret, 'codigo', 'descricao'));
            }

            return $ret;
        }

        return $sql;
    }

    /**
     * Método responsável por recuperar combo de autor
     *
     * @param int $unidade Identificador de unidade
     *
     * @return mixed Dados para o combo
     */
    public function comboNE($ptres, $gnd, $exercicio, $uge, $json = true)
    {
        if (empty($gnd)) {
            if ($json) {
                return '';
            } else {
                return [];
            }
        }

        $sql = "SELECT ne AS codigo, ne AS descricao
                FROM spoemendas.tesourogerencial tgs
                INNER JOIN monitora.ptres ptr USING (ptres)
                WHERE ptrid = '{$ptres}'
                AND gndcod = '{$gnd}'
                AND ne ~ '{$exercicio}'
                AND ungcod = '{$uge}'
                ORDER BY 2 ASC";

        if ($json) {
            return simec_json_encode(
                formatarCombo($this->carregar($sql), 'codigo', 'descricao'));
        }

        return $sql;
    }

    public function comboInteressado($unidade = null, $exercicio = null, $json = false)
    {
        $where = '';

        if (!is_null($exercicio)) {
            $where = <<<SQL
    AND EXISTS (SELECT 1
                  FROM emenda.emenda eme
                  WHERE eme.autid = aut.autid
                    AND emeano = '{$exercicio}')
SQL;
        }

        $sql = <<<DML
SELECT aut.autid AS codigo,
       aut.autnome || ' (' || par.parsigla || ')' AS descricao
  FROM emenda.autor aut
    JOIN emenda.partido par ON aut.parid = par.parid
  WHERE EXISTS (SELECT 1
                  FROM emenda.emenda eme
                    INNER JOIN monitora.acao USING(acaid)
                  WHERE eme.autid = aut.autid)
  AND aut.autstatus = 'A'
  {$where}
  AND (
      (aut.autnome !~* '^Relator')
      AND (aut.autnome !~* '^Com.')
      AND (aut.autnome !~* '^Bancada')
      AND (aut.autnome !~* '^Relat ')
      AND (aut.autnome !~* '^Partido')
  )
  ORDER BY descricao ASC
DML;
        if ($unidade) {
            $sql = "
                SELECT
                    aut.autid AS codigo,
                    aut.autnome || ' (' || par.parsigla || ')' AS descricao
                FROM emenda.autor aut
                JOIN emenda.partido par ON aut.parid = par.parid
                WHERE EXISTS (SELECT 1
                              FROM emenda.emenda eme
                              INNER JOIN monitora.acao USING(acaid)
                              WHERE eme.autid = aut.autid
                              AND unicod = '{$unidade}')
                 AND aut.autstatus = 'A'AND ((aut.autnome !~* '^Relator') and (aut.autnome !~* '^Com.') and (aut.autnome !~* '^Bancada'))
                 AND ((aut.autnome !~* '^Relator') and (aut.autnome !~* '^Com.') and (aut.autnome !~* '^Bancada'))
                 ORDER BY descricao ASC
            ";
        }

        if ($json || $unidade) {
            exit(simec_json_encode(
                formatarCombo($this->carregar($sql), 'codigo', 'descricao')));
        }

        return $sql;
    }

    /**
     * Recupera uma linha de registro pelo ID
     *
     * @param $id
     *
     * @return array|bool|mixed|NULL
     */
    public function recuperaLinha($id)
    {
        $sql = <<<DML
            SELECT
                sf.sfnid,
                tg.tgid,
                sf.sfninteressado,
--              COALESCE(sf.sfnugexecutora, tg.ungcod) AS sfnugexecutora,
                sf.sfnugexecutora,
                sf.sfnnotaempenho,
                sf.sfngrupodespesa,
                tg.tsgpo as po,
                COALESCE(sf.sfncodvinculacao,
                    CASE
                        WHEN tg.tgresulteof = 3 THEN
                            CASE
                                WHEN tg.tsgpo = 'EBPM' THEN 404
                                WHEN tg.tsgpo = 'EIND' THEN 411
                            END
                        ELSE tg.tgresulteof
                    END
                ) AS sfncodvinculacao,
                COALESCE(sf.sfnfontedetalhada, tg.tgfonterecursodetalhada) AS sfnfontedetalhada,
                COALESCE(sf.estuf, tg.regcod) AS estuf,
                COALESCE(sf.muncod, tg.mundsc) AS muncod,
                sf.sfncontratorepasse,
                sf.sfnpropostasiconv,
                COALESCE(sf.sfnconveniosiafi, tg.tgconvsiafi) AS sfnconveniosiafi,
                sf.sfnnumeroreferencia,
                sf.sfnmequipamento,
                sf.sfnobjetivo,
                sf.sfngrauprioridade,
                (SELECT SUM(sfp.sfpvalorpedido)
         FROM spoemendas.solicitacaofinanceirapedido sfp
         JOIN spoemendas.periodosolicitacao prs ON  prs.prsid = sfp.prsid
         AND prs.prsstatus
           AND now()::date between prsdatainicio and prsdatafim
        WHERE sfp.sfnid = sf.sfnid AND sfp.sfpstatus = 'A')  AS sfpvalorpedido,
                sf.docid,
                COALESCE(sf.unicod, tg.unicod) AS unicod,
                sf.autid,
                sf.emeid,
                COALESCE(sf.exercicio::INTEGER, tg.emeano::INTEGER) AS exercicio,
                sf.ptrid,
                sf.sfnnumeroreferenciaoutros,
                sf.sfnultimaedicao
            FROM {$this->stNomeTabela} sf
            LEFT JOIN monitora.ptres pt USING(ptrid)
            JOIN emenda.emenda em ON em.emeid = sf.emeid
            LEFT JOIN spo.vinculacaopagamento vin ON (vin.vnpcod = sf.sfncodvinculacao AND vin.vnpano::integer = em.emeano)
            LEFT JOIN spoemendas.tesourogerencial tg ON (
                                                            tg.unicod = sf.unicod
                                                            AND tg.ptres::INTEGER = pt.ptres::INTEGER
                                                            AND tg.emeano::INTEGER = em.emeano::INTEGER
                                                         )
            WHERE
                {$this->arChavePrimaria[0]} = {$id}
DML;
        $dados = $this->pegaLinha($sql);
        $dados['sfnfontedetalhada'] = trim($dados['sfnfontedetalhada']);

        // -- Calculando valor da vinculaçao com base no Resultado EOF, ou salvo no banco
        switch ($dados['sfncodvinculacao']) {
            case 2:
                $dados['sfncodvinculacao'] = 406; //'CUSTEIO/INVEST.-DEMAIS EMENDAS INDIVIDUAIS';
                break;
            case 3:
                switch ($dados['po']) {
                    case 'EBPM':
                        $dados['sfncodvinculacao'] = 404;
                        break;
                    case 'EIND':
                        $dados['sfncodvinculacao'] = 411; //'CUSTEIO/INVEST. PAC-EMENDAS';
                        break;
                }
                break;
            case 6:
                $dados['sfncodvinculacao'] = 405; //'CUSTEIO/INVEST.-EMENDAS INDIVIDUAIS';
                break;
            case 7:
                $dados['sfncodvinculacao'] = 410; //'CUST/INV-EMENDAS IMPOSITIVAS DE BANCADA RP7';
                break;
        };

        $dados['sfncodvinculacao_disabled'] = (new Spo_Model_Vinculacaopagamento)->carregarVinculacoes($dados['exercicio'])[$dados['sfncodvinculacao']];
        if (empty($dados['sfncodvinculacao_disabled'])) {
            $dados['sfncodvinculacao_disabled'] = $dados['sfncodvinculacao'];
        }

        return $dados;
    }

    public function gridSolicitacoesResumidas($prsid, $esdid)
    {
        $strSQL = <<<DML
            SELECT
                sf.exercicio,
                eme.emecod,
                sf.unicod || ' - ' || uni.unidsc AS unidade,
                aut.autnome || ' - ' || par.parsigla AS autor,
                sf.sfnnotaempenho,
                sf.ptrid,
                sf.estuf || ' - ' || mun.mundescricao AS estado,
                sf.sfngrupodespesa,
                sf.sfninteressado,
                sf.sfnfontedetalhada,
                sfp.sfpvalorpedido AS sfnvlrsolicitado
            FROM {$this->stNomeTabela} sf
            LEFT JOIN territorios.municipio mun ON (mun.muncod = sf.muncod)
            JOIN spoemendas.solicitacaofinanceirapedido sfp ON sfp.sfnid = sf.sfnid AND sfp.sfpstatus = 'A'
            JOIN emenda.emenda eme ON (eme.emeid = sf.emeid)
            JOIN public.unidade uni ON (uni.unicod = sf.unicod)
            JOIN emenda.autor aut ON (aut.autid = sf.autid)
            LEFT JOIN emenda.partido par ON (par.parid = aut.parid)
            JOIN workflow.documento d ON (d.docid = sf.docid)
            WHERE d.esdid = %d
            AND sf.sfnstatus = 'A'
            AND sfp.prsid = {$prsid}


DML;
        $stmt = sprintf($strSQL, $esdid, $_SESSION['exercicio']);

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho([
            'Ano',
            'Emenda',
            'Unidade<br>Orçamentária',
            'Autor',
            'Nota de Empenho',
            'PTRES',
            'UF - Município',
            'GND',
            'Solicitante',
            'Fonte',
            'Valor a Solicitar<br>R$'
        ]);
        $listagem->setQuery($stmt);
        $listagem->turnOnPesquisator();
        $listagem->addCallbackDeCampo('sfnvlrsolicitado', 'mascaraMoeda');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function buscaTotalPorEstadoPeriodo($esdid, $exercicio, $coluna, $query = false)
    {
        $strSQL = <<<DML
                SELECT
                    sfp.prsid || '-' || {$esdid},
                    TO_CHAR(ps.prsdatainicio, 'YYYY') AS exercicio,
                    ps.prsdescricao,
                    TO_CHAR(ps.prsdatainicio, 'DD/MM/YYYY') || ' até ' || TO_CHAR(ps.prsdatafim, 'DD/MM/YYYY') AS periodo,
                    COUNT(sfp.prsid) AS total
                FROM {$this->stNomeTabela} sf
                JOIN spoemendas.solicitacaofinanceirapedido sfp ON (sf.sfnid = sfp.sfnid)
                JOIN spoemendas.periodosolicitacao ps ON (ps.prsid = sfp.prsid)
                JOIN workflow.documento d ON (d.docid = sf.docid)
                WHERE d.esdid = %d
                AND sf.sfnstatus = 'A'
                AND sfp.sfpstatus = 'A'
                GROUP BY
                    sfp.prsid,
                    ps.prsdescricao,
                    ps.prsdatainicio,
                    ps.prsdatafim
                ORDER BY sfp.prsid
DML;
        $stmt = sprintf($strSQL, $esdid, $exercicio);
        if ($query) {
            return $stmt;
        }

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho(['Ano', 'Mês de Solicitação', 'Periodo de Solicitação', $coluna]);
        $listagem->setQuery($stmt);
        $listagem->setAcoes(['download' => 'xls']);
        $listagem->setAcoes(['info' => 'info']);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    /**
     * Retorna total de solicitações cadastradas
     *
     * @return bool|mixed|NULL|string
     */
    public function getTotal()
    {
        return $this->pegaUm("SELECT COUNT(*) AS total FROM {$this->stNomeTabela}");
    }

    /**
     * Retorna total de solicitações por estado
     *
     * @param $esdid
     *
     * @return bool|mixed|NULL|string
     */
    public function getTotalPorEstado($esdid)
    {
        $strSQL = <<<DML
            SELECT
                COUNT(sfp.prsid) AS total
            FROM {$this->stNomeTabela} sf
            JOIN spoemendas.solicitacaofinanceirapedido sfp ON (sf.sfnid = sfp.sfnid)
            JOIN spoemendas.periodosolicitacao ps ON (ps.prsid = sfp.prsid)
            JOIN workflow.documento d ON (d.docid = sf.docid)
            WHERE d.esdid = {$esdid}
DML;

        return $this->pegaUm($strSQL);
    }

    public function queryDeSolicitacoes($filtros, array $config = [])
    {

        $andmes = empty($prsmes) ? '' : "AND prsmes = {$prsmes}";

        $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
        $andUg = '';
        if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) || in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis)) {
            $andUg = sprintf(<<<SQL
               AND EXISTS (
                    SELECT 
                        1
                    FROM spoemendas.usuarioresponsabilidade
                    WHERE ungcod = sof.sfnugexecutora
                    AND usucpf = '%s'
               )
SQL
                , $_SESSION['usucpf']);
        }


        $AND = $this->geraFiltroSof($filtros, $config);
        $enviado = ESD_ENVIADO;
        $sql = <<<DML
                SELECT
                    sof.sfnid,
                    sof.exercicio,
                    eme.emecod,
                    sof.unicod,
                    uni.uniabrev,
                    uni.unidsc,
                    aut.autnome,
                    sol.autnome AS sfninteressado,
                    CASE
                        WHEN esd.esdid = %d THEN 'Sim'
                        ELSE 'Não'
                    END,
                    ene.sfnnotaempenho,
                    ptr.ptres,
                    mun.mundescricao,
                    sof.estuf,
                    sof.sfngrupodespesa,
                    sof.sfnfontedetalhada,
                    (
                        SELECT
                            SUM( sfp.sfpvalorpedido )
                        FROM
                            spoemendas.solicitacaofinanceirapedido sfp
                        JOIN spoemendas.periodosolicitacao prs ON
                            prs.prsid = sfp.prsid
                            AND prs.prsstatus
                            AND NOW()::DATE BETWEEN prsdatainicio AND prsdatafim
                        WHERE
                            sfp.sfnid = sof.sfnid
                            AND sfp.sfpstatus = 'A'
                    ) AS sfnvlrsolicitado,
                    (
                        SELECT
                            sfs.sfsdescricao
                        FROM
                            spoemendas.solicitacaofinanceirapedido sfp
                        JOIN spoemendas.solicitacaofinanceirasituacao sfs USING(sfsid)
                        WHERE
                            sfp.sfnid = sof.sfnid
                            AND sfp.sfpstatus = 'A'
                    ) AS situacaopedido,
                    (
                        SELECT
                            COUNT( 1 )
                        FROM
                            spoemendas.periodosolicitacao prs
                        WHERE
                            prs.prsstatus
                            AND NOW()::DATE BETWEEN prsdatainicio AND prsdatafim
                    ) AS periodoaberto,
                    'Deseja cadastrar novo pedido com os mesmos dados deste?' AS mensagem
                FROM %s sof
                INNER JOIN public.unidade uni USING(unicod)
                INNER JOIN emenda.autor aut USING(autid)
                LEFT JOIN workflow.documento doc USING(docid)
                LEFT JOIN workflow.estadodocumento esd USING(esdid)
                INNER JOIN emenda.emenda eme USING(emeid)
                INNER JOIN monitora.ptres ptr USING(ptrid)
                INNER JOIN spoemendas.solicitacaofinanceira ene USING(sfnid)
                LEFT JOIN emenda.autor sol ON (sof.sfninteressado = sol.autid)
                LEFT JOIN public.gnd gnd ON (gnd.gndcod = sof.sfngrupodespesa)
                LEFT JOIN territorios.municipio mun ON (mun.muncod = sof.muncod)
                WHERE
                    sof.sfnstatus = 'A' %s %s
                ORDER BY
                    sof.exercicio DESC,
                    uni.unicod,
                    eme.emecod,
                    ene.sfnnotaempenho
DML;

        $sql = sprintf($sql, $enviado, $this->stNomeTabela, $AND, $andUg);

        return $sql;
    }

    /**
     * Método responsável por recuperar combo de exercicio
     *
     * @author Victor Eduardo Barreto
     * @filesource
     * @throw  Mensagem de erro
     * @return array Dados para o combo
     */
    public function comboExercicio()
    {
        $sql = <<<DML
            SELECT prsano AS codigo, prsano AS descricao
            FROM spoemendas.programacaoexercicio
            WHERE prsexercicioaberto
            ORDER BY prsano
DML;

        return $sql;
    }

    /**
     * Método responsável por recuperar combo de emenda
     *
     * @author Victor Eduardo Barreto
     *
     * @param int  $autid  Identificação do autor da emenda
     * @param int  $ano    Ano do exercício
     * @param int  $unicod Identificador da unidade
     * @param bool $json   Flag para retornar em json
     *
     * @return mixed Dados para o combo
     */
    public function comboEmenda($autid, $ano, $unicod, $json = true)
    {
        if ($autid && $ano && $unicod) {
            $sql = "SELECT emeid AS codigo, emecod AS descricao
                    FROM emenda.emenda
                    JOIN monitora.acao USING(acaid)
                    WHERE autid = '{$autid}'
                    AND emeano = '{$ano}'
                    AND unicod = '{$unicod}'
                    ORDER BY emecod";

            $ret = $this->carregar($sql);

            if ($json) {
                $ret = simec_json_encode(
                    formatarCombo($ret, "codigo", "descricao"));
            }
        }

        return $ret;
    }

    /**
     * Método responsável por recuperar combo de GND
     *
     * @author Victor Eduardo Barreto
     * @filesource
     * @throw  Mensagem de erro
     *
     * @param int $emenda Identificador da emenda
     *
     * @return array|string Dados para o combo
     */
    public function comboGND($emenda = null, $json = true)
    {
        $sql = "
            SELECT gndcod AS codigo, gndcod || ' - ' || gnddsc AS descricao
            FROM gnd
            WHERE gndstatus = 'A'
            ORDER BY gndcod";

        if ($emenda) {
            $sql = "
                SELECT DISTINCT gndcod  AS codigo, gndcod || ' - ' || gnddsc AS descricao
                FROM gnd
                JOIN emenda.emendadetalhe eme USING(gndcod)
                WHERE eme.emeid = '{$emenda}'
                ORDER BY gndcod";

            $ret = $this->carregar($sql);

            if ($json) {
                $ret = simec_json_encode(
                    formatarCombo($ret, 'codigo', 'descricao'));
            }

            return $ret;
        }

        return $sql;
    }

    /**
     * Método responsável por recuperar combo de PTRES
     *
     * @author  Victor Eduardo Barreto
     * @package [subpackage]
     *
     * @param int $unidade Unidade Orçamentária
     * @param int $ano     Ano do exercício
     * @param int $emenda  Identificador da emenda
     *
     * @return object Dados para o combo
     */
    public function comboPTRES($unidade, $ano, $emenda, $json = true)
    {
        if ($unidade && $ano && $emenda) {
            $sql = "
                SELECT ptrid AS codigo, ptres AS descricao
                FROM emenda.emenda eme
                INNER JOIN monitora.acao aca USING(acaid)
                INNER JOIN monitora.ptres ptres USING(acaid)
                WHERE aca.unicod = '{$unidade}'
                AND eme.emeano = '{$ano}'
                AND eme.emeid = '{$emenda}'
                ORDER BY ptres.ptres ASC";

            $ret = $this->carregar($sql);

            if ($json) {
                $ret = simec_json_encode(
                    formatarCombo($ret, "codigo", "descricao"));
            }
        }

        return $ret;
    }

    public function detalheSolicitacao($sfnid)
    {
        $sql = <<<DML
            SELECT DISTINCT sof.exercicio,
                prs.prsdescricao,
                TO_CHAR(prs.prsdatainicio, 'DD/MM/YYYY') AS prsdatainicio,
                TO_CHAR(prs.prsdatafim, 'DD/MM/YYYY') AS prsdatafim,
                aut.autnome || ' (' || par.parsigla || ')' AS autnome,
                eme.emecod,
                sol.autnome || ' (' || par2.parsigla || ')'  AS sfninteressado,
                sof.sfnugexecutora,
                ptr.ptres,
                ptr.ptrid,
                sof.sfnfontedetalhada,
                sof.sfncontratorepasse,
                sof.sfnconveniosiafi,
                CASE WHEN 1 = sof.sfnmequipamento THEN 'SIM' ELSE 'NÃO' END AS sfnmequipamento,
                sof.sfnobjetivo,
                (SELECT SUM(sfp.sfpvalorpedido)
         FROM spoemendas.solicitacaofinanceirapedido sfp
         JOIN spoemendas.periodosolicitacao prs ON  prs.prsid = sfp.prsid
         AND prs.prsstatus
           AND now()::date between prsdatainicio and prsdatafim
        WHERE sfp.sfnid = sof.sfnid AND sfp.sfpstatus = 'A') sfnvlrsolicitado,
                sof.unicod,
                uni.unidsc,
                ene.ne,
                sof.sfngrupodespesa AS gndcod,
                sof.sfngrupodespesa|| ' - ' || gnd.gnddsc AS sfngrupodespesa,
                sof.sfncodvinculacao,
                sof.estuf,
                mun.mundescricao,
                sof.sfnpropostasiconv,
                sof.sfnnumeroreferencia,
                sof.docid,
                sof.sfnid,
                CASE WHEN 1 = sof.sfngrauprioridade THEN '1 - Alta'
                     WHEN 2 = sof.sfngrauprioridade THEN '2 - Média'
                     ELSE '3 - Baixa' END AS sfngrauprioridade,
                CASE WHEN 6 = emd.emdimpositiva THEN 'Sim'
                     ELSE 'Não' END AS emdimpositiva,
                sfnultimaedicao
            FROM spoemendas.solicitacaofinanceira sof
            LEFT JOIN spoemendas.solicitacaofinanceirapedido sfp USING (sfnid)
            LEFT JOIN spoemendas.periodosolicitacao prs USING(prsid)
            INNER JOIN public.unidade uni USING(unicod)
            INNER JOIN emenda.autor aut USING(autid)
            INNER JOIN emenda.partido par ON aut.parid = par.parid
            INNER JOIN workflow.documento doc USING(docid)
            INNER JOIN workflow.estadodocumento esd USING(esdid)
            INNER JOIN emenda.emenda eme USING(emeid)
            INNER JOIN monitora.ptres ptr USING(ptrid)
            LEFT JOIN emenda.emendadetalhe emd USING(emeid)
            LEFT JOIN territorios.municipio mun USING(muncod)
            LEFT JOIN spoemendas.emendane ene ON (ene.exercicio = sof.exercicio AND ene.emecod = eme.emecod)
            LEFT JOIN emenda.autor sol ON (sof.sfninteressado = sol.autid)
            LEFT JOIN emenda.partido par2 ON sol.parid = par2.parid
            LEFT JOIN public.gnd gnd ON (gnd.gndcod = sof.sfngrupodespesa)
            WHERE sfnid = {$sfnid}
DML;
        $dados = $this->pegaLinha($sql);
        $dados['sfnvlrsolicitado'] = mascaraMoeda($dados['sfnvlrsolicitado'], false);

        return $dados;
    }

    /**
     * Faz processamento e joga string CSV para download
     *
     * @param $esdid
     * @param $prsid
     */
    public function exportarCsv($esdid, $prsid)
    {
        $resultado = $this->getQueryReport($esdid, $prsid);
        $output = $this->buildArrayToCsv($resultado);

        //Gera um nome randomico para o arquivo
        $name = md5(uniqid() . microtime(true) . mt_rand()) . '_' . date('d-m-Y') . '.csv';

        //Lança a string de CSV para download pelo navegador
        header('Content-Type: text/csv');
        header('Content-Disposition: attachment; filename=' . $name);
        header('Pragma: no-cache');
        header("Expires: 0");
        echo $output;
    }

    public function getUf()
    {
        $sql = <<<DML
            SELECT estuf AS codigo, estuf AS descricao FROM territorios.estado ORDER BY estuf ASC
DML;

        return $this->carregar($sql);
    }

    public function getUnidadesPerfil($usucpf, $prfid)
    {
        $sql = "select unicod from spoemendas.usuarioresponsabilidade where usucpf = '{$usucpf}' AND rpustatus = 'A' AND prfid = $prfid";

        return $this->carregar($sql);
    }

    public function getMunicipio($uf, $json = false)
    {
        $sql = "SELECT muncod AS codigo, mundescricao AS descricao FROM territorios.municipio";
        $sql .= ($uf) ? " WHERE estuf = '{$uf}'" : '';
        $sql .= " ORDER BY descricao ASC";

        $ret = $this->carregar($sql);

        if ($json) {
            $ret = simec_json_encode(
                formatarCombo($ret, "codigo", "descricao"));
        }

        return $ret;
    }

    public function exportarXlsx($esdid, $prsid)
    {
        $resultado = $this->getQueryReportXlsx($esdid, $prsid);

        //Gera um nome randomico para o arquivo
        $name = md5(uniqid() . microtime(true) . mt_rand()) . '_' . date('d-m-Y') . '.xlsx';

        //Lança a string de XLSX para download pelo navegador
        header("Expires: 0");
        header("Last-Modified: " . date('d-m-Y'));
        header("Cache-Control: no-cache, must-revalidate");
        header("Content-Disposition: attachment; filename=\"{$name}\"");
        header("Content-Description: PHP Generated Data");
        header("Content-type: application/vnd.opencmlformats-officedocument.spreadsheetml.sheet");
        header("Pragma: no-cache");


        $output = $this->escreverXlsx($resultado);
        echo $output;
    }

    public function inputValorSolicitar($sfnid, $ne, $emeano = null)
    {
        $emeano = empty($emeano) ? $_SESSION['exercicio'] : $emeano;
        $sql = <<<SQL
      select CASE
       WHEN tsg.emeano != %d :: INTEGER THEN coalesce(tsg.tgrappagprocnproc, 0)
       WHEN tsg.emeano = %d :: INTEGER THEN coalesce(tsg.tgdespempenhadas, 0)
       END - (
           SELECT coalesce(sum (sfpvalorautorizado), 0)
           FROM spoemendas.solicitacaofinanceirapedido sfp
           WHERE
               sfp.sfnid = sof.sfnid
               AND sfp.sfpstatus = 'A'
       ) AS empenhado
FROM spoemendas.solicitacaofinanceira sof
    JOIN spoemendas.tesourogerencial tsg ON (sof.unicod = tsg.unicod AND sof.sfnugexecutora = tsg.ungcod)
WHERE
    sof.sfnid = %d AND tsg.ne IN ('%s');
SQL;
        $sql = sprintf($sql, $emeano, $emeano,  $sfnid, $ne);

        return $this->pegaUm($sql);
    }

    /**
     * Função que verifica se existe uma solicitação com a nota de empenho passada por parâmetro.
     *
     * @param null $ne
     *
     * @return bool
     */
    public function existsNE($ne = null, $ug = null)
    {
        $ne = empty($ne) ? $this->sfnnotaempenho : $ne;
        $ug = empty($ug) ? $this->sfnugexecutora : $ug;

        $sql = <<<SQL
          SELECT sfnid FROM spoemendas.solicitacaofinanceira WHERE sfnnotaempenho = '{$ne}' and sfnugexecutora = '{$ug}' and sfnstatus = 'A';
SQL;
        $sfnid = $this->pegaUm($sql);
        if (empty($this->sfnid)) {
            $this->carregarPorId($sfnid);
        }

        return !empty($sfnid) ? true : false;

    }

    public function copiarSolicitacao($sfnid = null)
    {
        if (!(new SpoEmendas_Model_Periodosolicitacao)->temPeriodoAberto()) {
            return false;
        }

        $sfnid = empty($sfnid) ? $_REQUEST['sfnid'] : $sfnid;
        $sfnAtual = new Spoemendas_Model_Solicitacaofinanceira($sfnid);
        $this->unicod = empty($sfnAtual->unicod) ? null : $sfnAtual->unicod;
        $this->autid = empty($sfnAtual->autid) ? null : $sfnAtual->autid;
        $this->exercicio = empty($sfnAtual->exercicio) ? null : $sfnAtual->exercicio;
        $this->emeid = empty($sfnAtual->emeid) ? null : $sfnAtual->emeid;
        $this->sfngrupodespesa = empty($sfnAtual->sfngrupodespesa) ? null : $sfnAtual->sfngrupodespesa;
        $this->ptrid = empty($sfnAtual->ptrid) ? null : $sfnAtual->ptrid;
        $this->sfnugexecutora = empty($sfnAtual->sfnugexecutora) ? null : $sfnAtual->sfnugexecutora;
        $this->sfncodvinculacao = empty($sfnAtual->sfncodvinculacao) ? null : $sfnAtual->sfncodvinculacao;
        $this->sfnfontedetalhada = empty($sfnAtual->sfnfontedetalhada) ? null : $sfnAtual->sfnfontedetalhada;
        $this->estuf = empty($sfnAtual->estuf) ? null : $sfnAtual->estuf;
        $this->sfninteressado = empty($sfnAtual->sfninteressado) ? null : $sfnAtual->sfninteressado;
        //$this->prsid = (new Spoemendas_Model_Periodosolicitacao)->retornaIdPeriodoAtual();

//        if (empty($this->prsid)) {
//            throw new Exception('Nenhum período selecionado.');
//        }

        $this->salvar(true, true, ['sfninteressado', 'sfncontratorepasse', 'sfnpropostasiconv', 'sfnconveniosiafi', 'sfnobjetivo', 'sfnnumeroreferenciaoutros', 'sfnvlrsolicitado', 'sfnnotaempenho']);
        $doc = new simec_Workflow();
        $doc->cadastraDocumento(WF_TPDID_SPOEMENDAS_SOLICITACOES_FINANCEIRAS, 'Nova solicitação de financeiro');
        $this->docid = $doc->getDocid();
        $this->salvar();
        if (!$this->commit()) {
            throw new Exception('Ocorreu um erro ao salvar a solicitação.');
        }

        return $this->sfnid;
    }

    /**
     * Apresenta listagem de histotico de alteração de solicitações
     *
     * @author Victor Eduardo Barreto
     * @filesource
     *
     * @param int $sfnid Identificador do registro
     *
     * @return string Dados para modal de apresentação
     */
    public function historicoAlteracao($sfnid)
    {
        (new Simec_Listagem())
            ->setCabecalho(['seq.', 'Data da Alteração', 'Responsável'])
            ->setDados((new Spoemendas_Model_Solicitacaofinanceirahistorico)
                ->dadosHistoricoSolicitacao($sfnid))
            ->addAcao('plus', 'detalheHistorico')
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    /**
     * Apresenta formulário de de-para de histórico de alteração de solicitação
     *
     * @author Victor Eduardo Barreto
     *
     * @param int $sfhid Identificador do histórico
     *
     * @return mixed Formulário de comparação
     */
    public function historicoDetalhe($sfhid)
    {
        return (new Spoemendas_Model_Solicitacaofinanceirahistorico($sfhid))
            ->montaComparacaoHistorico();
    }

    /**
     * Inativa o registro da solicitação financeira
     *
     * @author Jeferson carlos Araujo
     *
     * @param $sfnid
     *
     * @return resource
     */
    public function removeSolcitacao($sfnid)
    {
        $msg = new Simec_Helper_FlashMessage('financeiro/solicitar_financeiro');
        $SQL = <<<SQL
    UPDATE {$this->stNomeTabela} SET sfnstatus = 'I' WHERE sfnid = {$sfnid}
SQL;
        echo $SQL;
        try {
            $this->executar($SQL);
            $this->commit();
            $msg->addMensagem('Solicitação Removida com sucesso!', Simec_Helper_FlashMessage::SUCESSO);

            return true;
        } catch (Exceprion $ex) {
            throw $ex->getMessage();
            $msg->addMensagem('Erro ao tentar remover solicitação!', Simec_Helper_FlashMessage::ERRO);

        }
    }

    /**
     * Verifica se o perfil do usuario pode excluir a solicitação
     *
     * @author Jeferson Carlos Araujo
     *
     * @param $sfnid
     *
     * @return bool
     */
    public function podeRemoverSolicitacaoFinanceira($sfnid)
    {
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);

        $sql = <<<SQL
            SELECT
                docid
            FROM
                {$this->stNomeTabela}
            WHERE
                sfnid = {$sfnid}
            AND
                sfnstatus = 'A'
SQL;
        $docid = $this->pegaUm($sql);

        $sqlEstado = <<<SQL
            SELECT
                esdid
            FROM
                workflow.documento
            WHERE
                docid = {$docid};
SQL;
        $estadoAtual = $this->pegaUm($sqlEstado);


        if ($estadoAtual == ESD_NAO_ENVIADO && (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) || in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis))) {
            return true;
        }
        if (
            in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)
            || in_array(PFL_CGF_EQUIPE_FINANCEIRA, $perfis)
            || in_array(PFL_SUPER_USUARIO, $perfis)
        ) {
            return true;
        }

        return false;
    }

    public function isPrimeiroPeriodoAberto($sfnid = null)
    {
        $enviado = ESD_ENVIADO;
        if (is_null($sfnid)) {
            $sfnid = $this->sfnid;
        }

        return $this->pegaUm(<<<SQL
            SELECT
                NOW()::DATE BETWEEN prsdatainicio AND prsdatafim AS corrente
            FROM
                {$this->stNomeTabela} sf
            JOIN spoemendas.solicitacaofinanceirapedido sfp
                    USING(sfnid)
            JOIN spoemendas.periodosolicitacao prs
                    USING(prsid)
            JOIN workflow.documento
                    USING(docid)
            WHERE
                esdid = {$enviado}
                AND sf.sfnid = {$sfnid}
                AND sfp.sfpstatus = 'A'
            ORDER BY
                sfp.sfpid ASC LIMIT 1;
SQL
            ) == 't';
    }

    public function contarPedidos($sfnid)
    {
        return $this->pegaUm(
            <<<SQL
SELECT
     COUNT(1)
FROM {$this->stNomeTabela} sf
    JOIN spoemendas.solicitacaofinanceirapedido sfp USING (sfnid)
WHERE
    sf.sfnid = {$sfnid}
    AND sfp.sfpstatus = 'A'
SQL

        );
    }

    /**
     * Metodo responsavel por retornar os meses para listagem de filtro
     *
     * @author Jeferson Carlos Araujo <jeferson.araujo@castgroup.com.br>
     * @return array|mixed|NULL
     */
    public function mesReferencia()
    {
        $sql = <<<DML
            SELECT
                mescod AS codigo,
                mesdsc AS descricao
            FROM
                public.meses;
DML;

        return $this->carregar($sql);

    }

    /**
     * Metodo responsavel por retornar as situacoes dos pedidos para
     * filtro da listagem
     *
     * @author Jeferson Carlos Araujo <jeferson.araujo@castgroup.com.br>
     * @return array|mixed|NULL
     */
    public function situacaoPedido()
    {
        $sql = <<<DML
            SELECT
                sfsid AS codigo,
                sfsdescricao AS descricao
            FROM
                spoemendas.solicitacaofinanceirasituacao
            WHERE
                sfsid NOT IN(1);
DML;

        return $this->carregar($sql);

    }

    private function geraFiltroSof($filtros, $config)
    {
        $AND = [];
        $filtroMesReferencia = "(
							SELECT
								ps.prsmes
							FROM
								spoemendas.solicitacaofinanceira sof2
							LEFT JOIN spoemendas.solicitacaofinanceirapedido sfp ON
								sfp.sfnid = sof2.sfnid
							LEFT JOIN spoemendas.periodosolicitacao ps ON
								ps.prsid = sfp.prsid
							WHERE
								sfp.sfpstatus = 'A'
								AND sof2.sfnid = sof.sfnid) = ";
        foreach (is_array($filtros) ? $filtros : [] as $filtro => $valor) {
            if (empty($valor)) {
                continue;
            }
            switch ($filtro) {
                case 'exercicio':
                    if (1 == count($valor)) {
                        $AND[] = "sof.exercicio = '{$valor[0]}'";
                    } else {
                        $AND[] = "sof.exercicio IN('" . implode("', '", $valor) . "')";
                    }
                    break;
                case 'intid':
                    $AND[] = "sof.sfninteressado = {$valor}";
                    break;
                case 'emecod':
                    $AND[] = "eme.emecod = '{$valor}'";
                    break;
                case 'ptres':
                    $AND[] = "ptr.ptres = '{$valor}'";
                    break;
                case 'situacaopedido':
                    $AND[] = "
                    (
                        SELECT
                            sfs.sfsid
                        FROM
                            spoemendas.solicitacaofinanceirapedido sfp
                        LEFT JOIN spoemendas.solicitacaofinanceirasituacao sfs ON
                            sfs.sfsid = sfp.sfsid
                        WHERE
                            sfp.sfnid = sof.sfnid
                            AND sfp.sfpstatus = 'A'
                    ) = {$valor}";
                    break;
                case 'mesreferencia':
                    $AND[] = $filtroMesReferencia . $valor;
                    break;
                case 'enviado':
                    $AND[] = "esd.esdid = '{$valor}' ";
                    break;
                default:
                    $AND[] = "sof.{$filtro} = '{$valor}'";
            }
        }
        if (!empty($filtros['situacaopedido']) && empty($filtros['mesreferencia'])) {
            $AND[] = $filtroMesReferencia . date('m');
        }
        if (in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral()) || in_array(PFL_UO_EQUIPE_FINANCEIRA, pegaPerfilGeral())) {
            $AND[] = sprintf('
                    EXISTS (
		                SELECT DISTINCT 
		                    1
		                FROM public.unidade uni
		                INNER JOIN public.unidadegestora ung USING(unicod)
		                INNER JOIN spoemendas.usuarioresponsabilidade rpu USING(ungcod)
		                WHERE rpu.usucpf = \'%s\'
		                AND uni.unicod = sof.unicod
		                AND rpu.rpustatus = \'A\'                    
                    )', $_SESSION['usucpf']);
        }

        if (!empty($AND)) {
            $AND = 'AND ' . implode(' AND ', $AND);
        } else {
            $AND = '';
        }

        return $AND;
    }

    /**
     * Busca da base conjunto de dados para compor relatório de solicitação finenceira
     * Por estado workflow e por período referencia
     *
     * @param $esdid
     * @param $prsid
     *
     * @return array|mixed|NULL
     */
    private function getQueryReport($esdid, $prsid)
    {
        $sql = <<<SQL
            SELECT DISTINCT
                sof.sfnid,
                sof.unicod AS "UO",
                uni.unidsc AS "Descrição da UO",
                sof.sfnugexecutora AS "UG Executora",
                ug.ungabrev || ' - ' || ug.ungdsc AS "UG Executora - Descrição",
                ptr.ptres AS "Programa de Trabalho Resumido (PTRES)",
                sof.sfngrupodespesa AS "Grupo de Despesa",
                emd.emdimpositiva AS "RP",
                sof.sfnfontedetalhada AS "Fonte detalhada",
                aut.autnome AS "Autor",
                par.parsigla AS "Partido",
                sol.autnome AS "Interessado",
                sof.exercicio AS "Ano",
                sof.emeid AS "Nº da Emenda",
                sof.sfnnotaempenho AS "Nº Nota de Empenho",
                'MEC' AS "Gestor",
                ug.ungcod || ' - ' || ug.ungabrev || ' - ' || ug.ungdsc AS "UGE",
                sof.estuf AS "UF",
                mun.mundescricao AS "Municípios",
                sof.sfncontratorepasse AS "Contrato de Repasse",
                sof.sfnpropostasiconv AS "Proposta no SICONV",
                sof.sfnconveniosiafi AS "Convênio SIAFI",
                sof.sfnnumeroreferencia AS "Nº de Referência",
                CASE
                  WHEN sof.sfnmequipamento = 1 THEN 'Sim'
                  WHEN sof.sfnmequipamento = 2 THEN 'Nâo'
                END AS "Máquina/Equipamento",
                sof.sfnobjetivo AS "Objeto",
                sof.sfncodvinculacao AS "Vinculação",
                SUBSTRING(sof.sfnfontedetalhada, 0, 5) AS "Fonte",
                CASE
                  WHEN sof.sfngrauprioridade = 1 THEN '1 - Alta'
                  WHEN sof.sfngrauprioridade = 2 THEN '2 - Média'
                  WHEN sof.sfngrauprioridade = 3 THEN '3 - Baixa'
                END AS "Grau de Prioridade",
                (SELECT SUM(sfp.sfpvalorpedido)
         FROM spoemendas.solicitacaofinanceirapedido sfp
         JOIN spoemendas.periodosolicitacao prs ON  prs.prsid = sfp.prsid
         AND prs.prsstatus
           AND now()::DATE BETWEEN prsdatainicio AND prsdatafim
        WHERE sfp.sfnid = sof.sfnid AND sfp.sfpstatus = 'A') AS "Valor a Pagar"
            FROM spoemendas.solicitacaofinanceira sof
            JOIN spoemendas.solicitacaofinanceirapedido sfp ON (sof.sfnid = sfp.sfnid)
            INNER JOIN spoemendas.periodosolicitacao ps ON (ps.prsid = sfp.prsid)
            INNER JOIN public.unidade uni USING(unicod)
            LEFT JOIN public.unidadegestora ug ON (ug.ungcod = sof.sfnugexecutora)
            INNER JOIN emenda.autor aut USING(autid)
            INNER JOIN workflow.documento doc USING(docid)
            INNER JOIN workflow.estadodocumento esd USING(esdid)
            INNER JOIN emenda.emenda eme USING(emeid)
            INNER JOIN emenda.emendadetalhe emd USING(emeid)
            INNER JOIN monitora.ptres ptr USING(ptrid)
            LEFT JOIN territorios.municipio mun ON (mun.muncod = sof.muncod)
            LEFT JOIN spoemendas.emendane ene ON (ene.exercicio = sof.exercicio AND ene.emecod = eme.emecod)
            LEFT JOIN emenda.autor sol ON (sof.sfninteressado = sol.autid)
            LEFT JOIN emenda.partido par ON (par.parid = sol.parid)
            WHERE doc.esdid = %d AND ps.prsid = %d
SQL;

        return $this->carregar(sprintf($sql, $esdid, $prsid));
    }

    /**
     * Recebe Array e devolve string CSV
     *
     * @param array $results
     *
     * @return bool|string
     */
    private function buildArrayToCsv(array $results)
    {
        $totalSolicitacao = 0;
        $output = '';
        $linhasContabilizadas = [];

        $fonteDetalhada = 8;
        $fonte = 26;
        $notaEmpenho = 14;

        //Popular o cabeçalho do CSV
        foreach (array_keys($results[0]) as $item) {
            if ($item == 'sfnid') {
                continue;
            } //Exclui a primeira linha pk da tabela do cabeçalho csv
            $output .= "{$item};";
        }
        //Remover separador e adicionar quebra de linha
        $output = substr($output, 0, -1);
        $output .= "\n";

        //Popula as linhas de conteudo do CSV
        foreach (!is_array($results) ? [] : $results as $result) {

            //Usa a pk da tabela em conjunto com array
            //para nao contabilizar mais de uma vez as linhas que possuirem multiplas NE's
            if (!in_array($result['sfnid'], $linhasContabilizadas)) {
                array_push($linhasContabilizadas, $result['sfnid']);
                $totalSolicitacao += $result['Valor a Pagar'];
            }

            $tmpValue = array_map('trim', $result);

            // Rotina para saber se tem mais de uma NE
            $notaDeEmpenho = explode(',', $tmpValue['Nº Nota de Empenho']);

            // Para pedidos que tiverem mais de uma NE cadastrada, a linha deve ser
            // replicada, alterando apenas o valor da NE para cada linha da planilha
            if (count($notaDeEmpenho) > 1) {

                foreach ($notaDeEmpenho as $ne) {
                    $ne = trim($ne);
                    foreach (array_values($tmpValue) as $k => $value) {
                        if ($k == 0) {
                            continue;
                        }
                        if (($k == $fonteDetalhada) || $k == $fonte) {
                            $output .= "'{$value};";
                        } else {
                            if ($k == $notaEmpenho) {
                                $output .= "{$ne};";
                            } else {
                                $output .= "{$value};";
                            }
                        }
                    }
                    $output .= "\n";
                }

            } else {

                foreach (array_values($tmpValue) as $k => $value) {
                    if ($k == 0) {
                        continue;
                    }
                    if (($k == $fonteDetalhada) || $k == $fonte) {
                        $output .= "'{$value};";
                    } else {
                        $output .= "{$value};";
                    }
                }

            }

            $output = substr($output, 0, -1);
            $output .= "\n";
        }

        //Adicionar coluna com o somatorio das linhas de "valor a pagar"
        $linhaSomatorio = '';
        foreach (array_keys($results[0]) as $k => $row) {
            if ($k == 0) {
                continue;
            }
            if ($k < 28) {
                $linhaSomatorio .= ";";
            } else {
                $totalSolicitacao = number_format($totalSolicitacao, 2, ',', '.');
                $linhaSomatorio .= "{$totalSolicitacao}\n";
            }
        }

        $output = $linhaSomatorio . $output;

        return $output;
    }

    private function getQueryReportXlsx($esdid, $prsid)
    {
        if ($esdid == ESD_ENVIADO) {
            $and = "AND sfp.sfsid !=" . Spoemendas_Model_Solicitacaofinanceirasituacao::NAO_SOLICITADO;
        }
        $sql = <<<SQL
                  SELECT
                  "Mês de Referência",
                  "UO",
                  "Descrição da UO",
                  "UG Principal",
                  "UG Executora",
                  "UG Executora - Descrição",
                  "Programa de Trabalho Resumido (PTRES)",
                  "Grupo de Despesa",
                  "Indicador de Resultado EOF",
                  "Fonte detalhada",
                  "Autor",
                  "Partido",
                  "Parlamentar Interessado",
                  "Ano da Emenda",
                  "Nº da Emenda",
                  "Nota de Empenho",
                  "Gestor",
                  "UGE",
                  "UF",
                  "Municípios",
                  "Contrato de Repasse",
                  "Proposta no SICONV",
                  "Convênio SIAFI",
                  "Nº de Referência",
                  "Máquina/Equipamento",
                  "Objeto",
                  "Vinculação",
                  "Fonte",
                  "Grau de Prioridade",
                  "Valor a Pagar",
                  valorempenhado - valorautorizadosoma AS "Total Empenhado/ Inscrito no Exercício Corrente",
                  "Valor Pago no Exercício Corrente",
                  "VALOR AUTORIZADO",
                  "DATA DE AUTORIZAÇÃO"
                    FROM (SELECT
                    sof.sfnid,
                    TO_CHAR(TO_DATE(ps.prsmes ||'/'||ps.prsano, 'MM/YYYY'), 'MM/YYYY') AS "Mês de Referência",
                    sof.unicod AS "UO",
                    uni.unidsc AS "Descrição da UO",
                    tsg.ungcodprincipal AS "UG Principal",
                    sof.sfnugexecutora AS "UG Executora",
                    tsg.ungdsc AS "UG Executora - Descrição",
                    ptr.ptres AS "Programa de Trabalho Resumido (PTRES)",
                    sof.sfngrupodespesa AS "Grupo de Despesa",
                    tsg.tgresulteof AS "Indicador de Resultado EOF",
                    sof.sfnfontedetalhada AS "Fonte detalhada",
                    aut.autnome AS "Autor",
                    CASE
                      WHEN aut.autnome ~* '^Relator' THEN COALESCE(ptd.parsigla, 'Não identificado')
                      WHEN aut.autnome ~* '^Com.' THEN COALESCE(ptd.parsigla, 'Não identificado')
                      WHEN aut.autnome ~* '^Bancada' THEN COALESCE(ptd.parsigla, 'Não identificado')
                      WHEN aut.autnome ~* '^Relat ' THEN COALESCE(ptd.parsigla, 'Não identificado')
                      WHEN aut.autnome ~* '^Partido' THEN COALESCE(ptd.parsigla, 'Não identificado')
                      ELSE COALESCE(par.parsigla, ptd.parsigla, 'Não identificado')
                    END AS "Partido",
                    CASE
                      WHEN aut.autnome ~* '^Relator' THEN sol.autnome
                      WHEN aut.autnome ~* '^Com.' THEN sol.autnome
                      WHEN aut.autnome ~* '^Bancada' THEN sol.autnome
                      WHEN aut.autnome ~* '^Relat ' THEN sol.autnome
                      WHEN aut.autnome ~* '^Partido' THEN sol.autnome
                      ELSE 'N/A'
                    END AS "Parlamentar Interessado",
                    sof.exercicio AS "Ano da Emenda",
                    eme.emecod AS "Nº da Emenda",
                    sof.sfnnotaempenho AS "Nota de Empenho",
                    'MEC' AS "Gestor",
                    ug.ungcod || ' - ' || tsg.ungdsc AS "UGE",
                    sof.estuf AS "UF",
                    mun.mundescricao AS "Municípios",
                    sof.sfncontratorepasse AS "Contrato de Repasse",
                    sof.sfnpropostasiconv AS "Proposta no SICONV",
                    sof.sfnconveniosiafi AS "Convênio SIAFI",
                    CASE
                      WHEN sof.sfnnumeroreferencia = 'O' THEN sof.sfnnumeroreferenciaoutros
                      WHEN sof.sfnnumeroreferencia != 'O' THEN sof.sfnnumeroreferencia
                    END AS "Nº de Referência",
                    CASE
                      WHEN sof.sfnmequipamento = 1 THEN 'Sim'
                      WHEN sof.sfnmequipamento = 2 THEN 'Não'
                    END AS "Máquina/Equipamento",
                    sof.sfnobjetivo AS "Objeto",
                    sof.sfncodvinculacao AS "Vinculação",
                    SUBSTRING(sof.sfnfontedetalhada, 0, 5) AS "Fonte",
                    CASE
                      WHEN sof.sfngrauprioridade = 1 THEN '1 - Alta'
                      WHEN sof.sfngrauprioridade = 2 THEN '2 - Média'
                      WHEN sof.sfngrauprioridade = 3 THEN '3 - Baixa'
                    END AS "Grau de Prioridade",
                    sfp.sfpvalorpedido AS "Valor a Pagar",
                    CASE
                      WHEN tsg.emeano != to_char(now(),'YYYY')::INTEGER THEN tsg.tgrapprocnproc
                      WHEN tsg.emeano = to_char(now(),'YYYY')::INTEGER THEN tsg.tgdespempenhadas
                    END AS valorempenhado,
                    CASE
                      WHEN tsg.emeano != to_char(now(),'YYYY')::INTEGER THEN tsg.tgpagprocnproc
                      WHEN tsg.emeano = to_char(now(),'YYYY')::INTEGER THEN tsg.tgdesppagas
                    END AS "Valor Pago no Exercício Corrente",
                    '' AS "VALOR AUTORIZADO",
                    '' AS "DATA DE AUTORIZAÇÃO",
                    (
                            SELECT
                                SUM( COALESCE( sfpvalorautorizado, 0 ))
                            FROM
                                spoemendas.solicitacaofinanceirapedido sfp2
                            JOIN spoemendas.periodosolicitacao ps2
                                    USING(prsid)
                            WHERE
                                sfp2.sfpstatus = 'A'
                                AND sfp2.sfnid = sfp.sfnid
                                AND ps2.prsano = to_char(
                                    NOW(),
                                    'YYYY'
                                )::INTEGER
                        ) AS valorautorizadosoma
            FROM spoemendas.solicitacaofinanceira sof
            JOIN spoemendas.solicitacaofinanceirapedido sfp ON (sof.sfnid = sfp.sfnid)
            INNER JOIN spoemendas.periodosolicitacao ps ON (ps.prsid = sfp.prsid)
            INNER JOIN public.unidade uni USING(unicod)
            LEFT JOIN spo.vinculacaopagamento vin ON (vin.vnpcod = sof.sfncodvinculacao AND sof.exercicio = vin.vnpano)
            LEFT JOIN public.unidadegestora ug ON (ug.ungcod = sof.sfnugexecutora)
            LEFT JOIN emenda.autor aut USING(autid)
            LEFT JOIN emenda.autor sol ON (sof.sfninteressado = sol.autid)
            INNER JOIN workflow.documento doc USING(docid)
            INNER JOIN workflow.estadodocumento esd USING(esdid)
            INNER JOIN emenda.emenda eme USING(emeid)
            INNER JOIN monitora.ptres ptr USING(ptrid)
            LEFT JOIN territorios.municipio mun ON (mun.muncod = sof.muncod)
            LEFT JOIN spoemendas.emendane ene ON (ene.exercicio = sof.exercicio AND ene.emecod = eme.emecod AND sof.sfnnotaempenho = ene.ne)
            LEFT JOIN emenda.partido par ON (par.parid = aut.parid)
            LEFT JOIN emenda.partido ptd ON (ptd.parid = sol.parid)
            LEFT JOIN spoemendas.tesourogerencial tsg ON (
                                                                tsg.unicod = sof.unicod
                                                            AND tsg.emeano = eme.emeano
                                                            AND tsg.ptres = ptr.ptres
                                                            AND sof.sfngrupodespesa = tsg.gndcod
                                                            AND sof.sfnnotaempenho = tsg.ne
                                                          )
            WHERE
            doc.esdid = %d
            AND ps.prsid = %d
            AND sof.sfnstatus = 'A'
            AND sfp.sfpstatus = 'A'
            {$and}) AS tabela
SQL;
        $querySql = $this->carregar(sprintf($sql, $esdid, $prsid));
        $numRows = count($querySql);
        $fieldname = [];
        if (is_array($querySql[0])) {
            foreach ($querySql[0] as $key => $val) {
                if ($key != 'sfnid') {
                    array_push($fieldname, $key);
                }
            }
        }
        $numColumns = count($fieldname);
        $resultado = [$querySql, $numRows, $numColumns, $fieldname];

        return $resultado;
    }

    private function escreverXlsx($resultado)
    {
//       Criando o objeto PHPExcel
        $objPHPExcel = new PHPExcel();

//      $resultado = simec_utf8_decode_recursive($resultado);
        $query = $resultado[0];
        $numRows = $resultado[1];
        $numColumns = $resultado[2];
        $nmeColunas = $resultado[3];
        $lastColumn = 'A';

//      Descobre qual a ultima coluna de dados de acordo com a query
        for ($countCol = 0; $countCol < $numColumns; $countCol++) {
            $lastColumn++;
        }

//      imprimir cabeçalho
        $countCab = 0;
        for ($col = 'A'; $col != $lastColumn; $col++) {
            $objPHPExcel->setActiveSheetIndex(0)
                ->setCellValue($col . 2, ($nmeColunas[$countCab]));
            $countCab++;
        }

//      imprimir calculos
        $objPHPExcel->setActiveSheetIndex(0)
            ->setCellValue('AD1', '=SUM(AD3:AD' . ($numRows + 2) . ')')
            ->setCellValue('AE1', '=SUM(AE3:AE' . ($numRows + 2) . ')')
            ->setCellValue('AF1', '=SUM(AF3:AF' . ($numRows + 2) . ')')
            ->setCellValue('AG1', '=SUM(AG3:AG' . ($numRows + 2) . ')');

//       Configuracao de cores e bordas
        $transparente = [
            'borders'   => [
                'allborders' => [
                    'style' => PHPExcel_Style_Border::BORDER_THIN
                ]
            ],
            'alignment' => [
                'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
                'vertical'   => PHPExcel_Style_Alignment::VERTICAL_CENTER
            ]
        ];
        $azul = [
            'borders'   => [
                'allborders' => [
                    'style' => PHPExcel_Style_Border::BORDER_THIN
                ]
            ],
            'fill'      => [
                'type'  => PHPExcel_Style_Fill::FILL_SOLID,
                'color' => ['argb' => 'FF2F75B5']
            ],
            'font'      => [
                'bold'  => true,
                'color' => ['rgb' => 'FFFFFF']
            ],
            'alignment' => [
                'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
                'vertical'   => PHPExcel_Style_Alignment::VERTICAL_CENTER
            ]
        ];
        $amarelo = [
            'borders'   => [
                'allborders' => [
                    'style' => PHPExcel_Style_Border::BORDER_THIN
                ]
            ],
            'fill'      => [
                'type'  => PHPExcel_Style_Fill::FILL_SOLID,
                'color' => ['argb' => 'FFFFFF00']
            ],
            'alignment' => [
                'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
                'vertical'   => PHPExcel_Style_Alignment::VERTICAL_CENTER
            ]
        ];
        $laranja = [
            'borders'   => [
                'allborders' => [
                    'style' => PHPExcel_Style_Border::BORDER_THIN
                ]
            ],
            'fill'      => [
                'type'  => PHPExcel_Style_Fill::FILL_SOLID,
                'color' => ['argb' => 'FFFFC000']
            ],
            'alignment' => [
                'horizontal' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
                'vertical'   => PHPExcel_Style_Alignment::VERTICAL_CENTER
            ]
        ];
        $bordas = [
            'borders' => [
                'allborders' => [
                    'style' => PHPExcel_Style_Border::BORDER_THIN
                ]
            ]
        ];

//      imprimir dados
        for ($lin = 0; $lin < $numRows; $lin++) {
            $count = 0;
            for ($col = 'A'; $col != $lastColumn; $col++) {
                $valor = ($query[$lin][$nmeColunas[$count]]);
                $header = $objPHPExcel->getActiveSheet()->getCell($col . 2)->getValue();
                if ($header == 'Nota de Empenho' AND (strpos($valor, ', ') || strpos($valor, ','))) {
                    $valor = str_replace(",", "\n", $valor);
                }
                if ($header == "NÂº de ReferÃªncia") {
                    $valor = ' ' . $valor;
                }
                $objPHPExcel->setActiveSheetIndex(0)
                    ->setCellValue($col . ($lin + 3), $valor)
                    ->getStyle($col . ($lin + 3))->applyFromArray($bordas);
                $count++;
            }
        }

//       quebra de linha para os itens da coluna O
        $objPHPExcel->getActiveSheet()->getStyle('O3:O' . $objPHPExcel->getActiveSheet()->getHighestRow())
            ->getAlignment()->setWrapText(true);

//       Colorindo os titulos
        $objPHPExcel->getActiveSheet()->getStyle('AC1:AE1')->applyFromArray($transparente);
        unset($styleArray);
        $objPHPExcel->getActiveSheet()->getStyle('A2:J2')->applyFromArray($azul);
        unset($styleArray);
        $objPHPExcel->getActiveSheet()->getStyle('K2:L2')->applyFromArray($amarelo);
        unset($styleArray);
        $objPHPExcel->getActiveSheet()->getStyle('M2:M2')->applyFromArray($laranja);
        unset($styleArray);
        $objPHPExcel->getActiveSheet()->getStyle('N2:Q2')->applyFromArray($amarelo);
        unset($styleArray);
        $objPHPExcel->getActiveSheet()->getStyle('R2:R2')->applyFromArray($laranja);
        unset($styleArray);
        $objPHPExcel->getActiveSheet()->getStyle('S2:W2')->applyFromArray($amarelo);
        unset($styleArray);
        $objPHPExcel->getActiveSheet()->getStyle('X2:X2')->applyFromArray($laranja);
        unset($styleArray);
        $objPHPExcel->getActiveSheet()->getStyle('Y2:AA2')->applyFromArray($amarelo);
        unset($styleArray);
        $objPHPExcel->getActiveSheet()->getStyle('AB2:AC2')->applyFromArray($laranja);
        unset($styleArray);
        $objPHPExcel->getActiveSheet()->getStyle('AD2:AH2')->applyFromArray($amarelo);
        unset($styleArray);

//       Salvando no formato excel 2007
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
        $objWriter->save('php://output');
    }

    public function dadosGraficoSolicitacaoMensalGupoUnidade($emeano, $prsmes = null) {
        $andmes = empty($prsmes) ? '' : "AND prsmes = {$prsmes}";
        $colmes = empty($prsmes) ? '' : "prsmes,";
        $grumes = empty($prsmes) ? '' : ", prsmes";

        $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
        $andUg = '';
        if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) || in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis)) {
            $andUg = sprintf(<<<SQL
               AND EXISTS (
                    SELECT 
                        1
                    FROM spoemendas.usuarioresponsabilidade
                    WHERE ungcod = sfn.sfnugexecutora
                    AND usucpf = '%s'
               )
SQL
                , $_SESSION['usucpf']);
        }

        $sql = <<<SQL
            SELECT
              gundsc, %s
              COALESCE(SUM(valorlimite), 0.00) AS valorlimite,
              COALESCE(SUM(valorsolicitado), 0.00) AS valorsolicitado,
              COALESCE(SUM(valorautorizado), 0.00) AS valorautorizado
            FROM (
                   SELECT
                     gun.gundsc,
                     prs.prsmes,
                     (SELECT CASE
                             WHEN tgs.emeano :: CHAR(4) = to_char(now(), 'YYYY')
                               THEN tgdespempenhadas - tgdesppagas
                             ELSE tgrappagprocnproc
                             END valorlimite
                      FROM spoemendas.tesourogerencial tgs
                        INNER JOIN monitora.ptres ptr USING (ptres)
                      WHERE ptres = ptr.ptres
                            AND gndcod = gnd.gndcod
                            AND ne = sfn.sfnnotaempenho
                            AND ungcod = ung.ungcod
                            --AND ptr.ptrano = '%s'
                        LIMIT 1),
                     sfp.sfpvalorpedido AS valorsolicitado,
                     sfp.sfpvalorautorizado AS valorautorizado
                   FROM spoemendas.solicitacaofinanceira AS sfn
                     INNER JOIN spoemendas.solicitacaofinanceirapedido sfp USING (sfnid)
                     INNER JOIN spoemendas.periodosolicitacao prs USING (prsid)
                     INNER JOIN monitora.ptres ptr USING (ptrid)
                     LEFT JOIN public.gnd gnd ON (gnd.gndcod = sfn.sfngrupodespesa)
                     INNER JOIN public.unidade uni ON uni.unicod = sfn.unicod
                     INNER JOIN public.unidadegestora ung ON ung.unicod = uni.unicod
                     INNER JOIN public.grupounidade gun USING (gunid)
                     INNER JOIN emenda.emenda eme USING (emeid)
                   WHERE sfp.sfpstatus = 'A' AND sfn.sfnugexecutora = ung.ungcod %s
                   %s
                   ) tab
            GROUP BY gundsc %s
            ORDER BY gundsc
SQL;
        $sql = sprintf($sql, $colmes, $emeano, $andmes, $andUg, $grumes);

        $res = $this->carregar($sql);
        return empty($res) ? [] : $res;
    }

    public function dadosGraficoSolicitacaoMensalUnidade($emeano, $prsmes = null) {
        $andmes = empty($prsmes) ? '' : "AND prsmes = {$prsmes}";

        $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
        $andUg = '';
        if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) || in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis)) {
            $andUg = sprintf(<<<SQL
               AND EXISTS (
                    SELECT 
                        1
                    FROM spoemendas.usuarioresponsabilidade
                    WHERE ungcod = sfn.sfnugexecutora
                    AND usucpf = '%s'
               )
SQL
                , $_SESSION['usucpf']);
        }

        $sql = <<<SQL
            SELECT
              gundsc,
              uniabrev,
              prsmes,
              COALESCE(SUM(valorlimite), 0.00) AS valorlimite,
              COALESCE(SUM(valorsolicitado), 0.00) AS valorsolicitado,
              COALESCE(SUM(valorautorizado), 0.00) AS valorautorizado
            FROM (
                   SELECT
                     gun.gundsc,
                     uni.unicod,
                     uni.uniabrev,
                     prs.prsmes,
                     (SELECT CASE
                             WHEN tgs.emeano :: CHAR(4) = to_char(now(), 'YYYY')
                               THEN tgdespempenhadas - tgdesppagas
                             ELSE tgrappagprocnproc
                             END valorlimite
                      FROM spoemendas.tesourogerencial tgs
                        INNER JOIN monitora.ptres ptr USING (ptres)
                      WHERE ptres = ptr.ptres
                            AND gndcod = gnd.gndcod
                            AND ne = sfn.sfnnotaempenho
                            AND ungcod = ung.ungcod
                           -- AND ptr.ptrano = '%s'
                        LIMIT 1),
                     sfp.sfpvalorpedido AS valorsolicitado,
                     sfp.sfpvalorautorizado AS valorautorizado
                   FROM spoemendas.solicitacaofinanceira AS sfn
                     INNER JOIN spoemendas.solicitacaofinanceirapedido sfp USING (sfnid)
                     INNER JOIN spoemendas.periodosolicitacao prs USING (prsid)
                     INNER JOIN monitora.ptres ptr USING (ptrid)
                     LEFT JOIN public.gnd gnd ON (gnd.gndcod = sfn.sfngrupodespesa)
                     INNER JOIN public.unidade uni ON uni.unicod = sfn.unicod
                     INNER JOIN public.unidadegestora ung ON ung.unicod = uni.unicod
                     INNER JOIN public.grupounidade gun USING (gunid)
                     INNER JOIN emenda.emenda eme USING (emeid)
                   WHERE sfp.sfpstatus = 'A' AND sfn.sfnugexecutora = ung.ungcod %s
                   %s
                   ) tab
            GROUP BY gundsc, uniabrev, prsmes;
SQL;
        $sql = sprintf($sql, $emeano, $andmes, $andUg);

        $res = $this->carregar($sql);
        return empty($res) ? [] : $res;
    }

    public function dadosGraficoSolicitacaoMensalUnidadeGestora($emeano, $prsmes = null)
    {
        $andmes = empty($prsmes) ? '' : "AND prsmes = {$prsmes}";

        $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
        $andUg = '';
        if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) || in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis)) {
            $andUg = sprintf(<<<SQL
               AND EXISTS (
                    SELECT 
                        1
                    FROM spoemendas.usuarioresponsabilidade
                    WHERE ungcod = sfn.sfnugexecutora
                    AND usucpf = '%s'
               )
SQL
                , $_SESSION['usucpf']);
        }

        $sql = <<<SQL
            SELECT
              uniabrev,
              ungdsc,
              ungcod,
              prsmes,
              COALESCE(SUM(valorlimite), 0.00) AS valorlimite,
              COALESCE(SUM(valorsolicitado), 0.00) AS valorsolicitado,
              COALESCE(SUM(valorautorizado), 0.00) AS valorautorizado
            FROM (
                   SELECT
                     uni.uniabrev,
                     ung.ungdsc,
                     ung.ungcod,
                     prs.prsmes,
                     (SELECT CASE
                             WHEN tgs.emeano :: CHAR(4) = to_char(now(), 'YYYY')
                               THEN tgdespempenhadas - tgdesppagas
                             ELSE tgrappagprocnproc
                             END valorlimite
                      FROM spoemendas.tesourogerencial tgs
                        INNER JOIN monitora.ptres ptr USING (ptres)
                      WHERE ptres = ptr.ptres
                            AND gndcod = gnd.gndcod
                            AND ne = sfn.sfnnotaempenho
                            AND ungcod = ung.ungcod
                            --AND ptr.ptrano = '%s'
                        LIMIT 1),
                     sfp.sfpvalorpedido AS valorsolicitado,
                     sfp.sfpvalorautorizado AS valorautorizado
                   FROM spoemendas.solicitacaofinanceira AS sfn
                     INNER JOIN spoemendas.solicitacaofinanceirapedido sfp USING (sfnid)
                     INNER JOIN spoemendas.periodosolicitacao prs USING (prsid)
                     INNER JOIN monitora.ptres ptr USING (ptrid)
                     LEFT JOIN public.gnd gnd ON (gnd.gndcod = sfn.sfngrupodespesa)
                     INNER JOIN public.unidade uni ON uni.unicod = sfn.unicod
                     INNER JOIN public.unidadegestora ung ON ung.unicod = uni.unicod
                     INNER JOIN public.grupounidade gun USING (gunid)
                     INNER JOIN emenda.emenda eme USING (emeid)
                   WHERE sfp.sfpstatus = 'A' AND sfn.sfnugexecutora = ung.ungcod %s 
                   %s
                   ) tab
            GROUP BY uniabrev, ungdsc, ungcod, prsmes
            ORDER BY uniabrev;
SQL;
        $sql = sprintf($sql, $emeano, $andmes, $andUg);

        $res = $this->carregar($sql);
        return empty($res) ? [] : $res;
    }

    public function dadosGraficoSolicitacaoMensalEmenda($emeano, $prsmes = null) {
        $andmes = empty($prsmes) ? '' : "AND prsmes = {$prsmes}";

        $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
        $andUg = '';
        if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) || in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis)) {
            $andUg = sprintf(<<<SQL
               AND EXISTS (
                    SELECT 
                        1
                    FROM spoemendas.usuarioresponsabilidade
                    WHERE ungcod = sfn.sfnugexecutora
                    AND usucpf = '%s'
               )
SQL
                , $_SESSION['usucpf']);
        }

        $sql = <<<SQL
            SELECT
              ungcod,
              emecod,
              prsmes,
              COALESCE(SUM(valorlimite), 0.00) AS valorlimite,
              COALESCE(SUM(valorsolicitado), 0.00) AS valorsolicitado,
              COALESCE(SUM(valorautorizado), 0.00) AS valorautorizado
            FROM (
                   SELECT
                     gun.gundsc,
                     uni.uniabrev,
                     ung.ungcod,
                     eme.emecod,
                     prs.prsmes,
                     (SELECT CASE
                             WHEN tgs.emeano :: CHAR(4) = to_char(now(), 'YYYY')
                               THEN tgdespempenhadas - tgdesppagas
                             ELSE tgrappagprocnproc
                             END valorlimite
                      FROM spoemendas.tesourogerencial tgs
                        INNER JOIN monitora.ptres ptr USING (ptres)
                      WHERE ptres = ptr.ptres
                            AND gndcod = gnd.gndcod
                            AND ne = sfn.sfnnotaempenho
                            AND ungcod = ung.ungcod
                           -- AND ptr.ptrano = '%s'
                        LIMIT 1),
                      sfp.sfpvalorpedido AS valorsolicitado,
                      sfp.sfpvalorautorizado AS valorautorizado
                    FROM spoemendas.solicitacaofinanceira AS sfn
                    INNER JOIN spoemendas.solicitacaofinanceirapedido sfp USING (sfnid)
                    INNER JOIN spoemendas.periodosolicitacao prs USING (prsid)
                    INNER JOIN monitora.ptres ptr USING (ptrid)
                    LEFT JOIN public.gnd gnd ON (gnd.gndcod = sfn.sfngrupodespesa)
                    INNER JOIN public.unidade uni ON uni.unicod = sfn.unicod
                    INNER JOIN public.unidadegestora ung ON ung.unicod = uni.unicod
                    INNER JOIN public.grupounidade gun USING (gunid)
                    INNER JOIN emenda.emenda eme USING (emeid)
                    WHERE sfp.sfpstatus = 'A' AND sfn.sfnugexecutora = ung.ungcod %s
                    %s
                    ) tab
            GROUP BY ungcod, emecod, prsmes
            ORDER BY emecod
SQL;
        $sql = sprintf($sql, $emeano, $andmes, $andUg);

        $res = $this->carregar($sql);
        return empty($res) ? [] : $res;
    }

    public function dadosGraficoSolicitacaoMensalEmpenho($emeano, $prsmes = null) {
        $andmes = empty($prsmes) ? '' : "AND prsmes = {$prsmes}";

        $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
        $andUg = '';
        if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) || in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis)) {
            $andUg = sprintf(<<<SQL
               AND EXISTS (
                    SELECT 
                        1
                    FROM spoemendas.usuarioresponsabilidade
                    WHERE ungcod = sfn.sfnugexecutora
                    AND usucpf = '%s'
               )
SQL
                , $_SESSION['usucpf']);
        }

        $sql = <<<SQL
            SELECT 
              mesdsc,
              uniabrev,
              emeid,
              emecod,
              sfnnotaempenho,
              prsmes,
              prsdescricao,
              COALESCE(SUM(valorlimite), 0.00) AS valorlimite,
              COALESCE(SUM(valorsolicitado), 0.00) AS valorsolicitado,
              COALESCE(SUM(valorautorizado), 0.00) AS valorautorizado
            FROM public.meses mes LEFT JOIN (
                   SELECT distinct
                     uni.uniabrev,
                     eme.emeid,
                     eme.emecod,
                     sfn.sfnnotaempenho,
                     prs.prsmes,
                     prs.prsdescricao,
                     (SELECT CASE
                             WHEN tgs.emeano :: CHAR(4) = to_char(now(), 'YYYY')
                               THEN tgdespempenhadas - tgdesppagas
                             ELSE tgrappagprocnproc
                             END valorlimite
                      FROM spoemendas.tesourogerencial tgs
                        INNER JOIN monitora.ptres ptr USING (ptres)
                      WHERE ptres = ptr.ptres
                            AND gndcod = gnd.gndcod
                            AND ne = sfn.sfnnotaempenho
                          --  AND ungcod = ung.ungcod
                          --  AND ptr.ptrano = '%s'
                        LIMIT 1),
                     sfp.sfpvalorpedido AS valorsolicitado,
                     sfp.sfpvalorautorizado AS valorautorizado
                   FROM spoemendas.solicitacaofinanceira AS sfn
                     INNER JOIN spoemendas.solicitacaofinanceirapedido sfp USING (sfnid)
                     INNER JOIN spoemendas.periodosolicitacao prs USING (prsid)
                     INNER JOIN monitora.ptres ptr USING (ptrid)
                     LEFT JOIN public.gnd gnd ON (gnd.gndcod = sfn.sfngrupodespesa)
                     INNER JOIN public.unidade uni ON uni.unicod = sfn.unicod
                     INNER JOIN public.unidadegestora ung ON ung.unicod = uni.unicod
                     INNER JOIN public.grupounidade gun USING (gunid)
                     INNER JOIN emenda.emenda eme USING (emeid)
                   WHERE sfp.sfpstatus = 'A'  %s
                   %s
                   ) tab ON tab.prsmes = mes.mescod::INTEGER
              WHERE sfnnotaempenho IS NOT NULL
            GROUP BY mesdsc, uniabrev, emecod, emeid, sfnnotaempenho, prsmes, prsdescricao
            ORDER BY emecod;
SQL;
        $sql = sprintf($sql, $emeano, $andmes, $andUg);


        $res = $this->carregar($sql);
        return empty($res) ? [] : $res;
    }

    public function dadosGraficoSolicitacaoEmpenhoPorMes($emeano) {
        $sql = <<<SQL
            SELECT
              prsdescricao,
              COALESCE(SUM(valorlimite), 0.00) AS valorlimite,
              COALESCE(SUM(valorsolicitado), 0.00) AS valorsolicitado,
              COALESCE(SUM(valorautorizado), 0.00) AS valorautorizado
            FROM (
                   SELECT
                     uni.uniabrev,
                     eme.emeid,
                     eme.emecod,
                     sfn.sfnnotaempenho,
                     prs.prsmes,
                     prs.prsdescricao,
                     (SELECT CASE
                             WHEN tgs.emeano :: CHAR(4) = to_char(now(), 'YYYY')
                               THEN tgdespempenhadas - tgdesppagas
                             ELSE tgrappagprocnproc
                             END valorlimite
                      FROM spoemendas.tesourogerencial tgs
                        INNER JOIN monitora.ptres ptr USING (ptres)
                      WHERE ptres = ptr.ptres
                            AND gndcod = gnd.gndcod
                            AND ne = sfn.sfnnotaempenho
                       --     AND ptr.ptrano = '{$emeano}'),
                     sfp.sfpvalorpedido AS valorsolicitado,
                     sfp.sfpvalorautorizado AS valorautorizado
                   FROM spoemendas.solicitacaofinanceira AS sfn
                     INNER JOIN spoemendas.solicitacaofinanceirapedido sfp USING (sfnid)
                     INNER JOIN spoemendas.periodosolicitacao prs USING (prsid)
                     INNER JOIN monitora.ptres ptr USING (ptrid)
                 LEFT JOIN public.gnd gnd ON (gnd.gndcod = sfn.sfngrupodespesa)
                     INNER JOIN public.unidade uni ON uni.unicod = sfn.unicod
                     INNER JOIN public.grupounidade gun USING (gunid)
                     INNER JOIN emenda.emenda eme USING (emeid)
                   WHERE sfp.sfpstatus = 'A') tab
              WHERE sfnnotaempenho IS NOT NULL
            GROUP BY prsdescricao;
SQL;
        $res = $this->carregar($sql);
        return empty($res) ? [] : $res;
    }

    public function dadosGraficoSolicitacaoMes($emeano) {
        $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
        $andUg = '';
        if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis) || in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis)) {
            $andUg = sprintf(<<<SQL
               AND EXISTS (
                    SELECT 
                        1
                    FROM spoemendas.usuarioresponsabilidade
                    WHERE ungcod = sfn.sfnugexecutora
                    AND usucpf = '%s'
               )
SQL
            , $_SESSION['usucpf']);
        }

        $sql = <<<SQL
            SELECT
              mesdsc,
              COALESCE(SUM(valorlimite), 0.00) AS valorlimite,
              COALESCE(SUM(valorsolicitado), 0.00) AS valorsolicitado,
              COALESCE(SUM(valorautorizado), 0.00) AS valorautorizado
            FROM public.meses mes LEFT JOIN
            (
                   SELECT
                     prs.prsmes,
                     (SELECT CASE
                             WHEN tgs.emeano :: CHAR(4) = to_char(now(), 'YYYY')
                               THEN tgdespempenhadas - tgdesppagas
                             ELSE tgrappagprocnproc
                             END valorlimite
                      FROM spoemendas.tesourogerencial tgs
                        INNER JOIN monitora.ptres ptr USING (ptres)
                      WHERE ptres = ptr.ptres
                            AND gndcod = gnd.gndcod
                            AND ne = sfn.sfnnotaempenho
                       --     AND ptr.ptrano = '{$emeano}'
                        LIMIT 1),
                     sfp.sfpvalorpedido AS valorsolicitado,
                     sfp.sfpvalorautorizado AS valorautorizado
                   FROM spoemendas.solicitacaofinanceira AS sfn
                     INNER JOIN spoemendas.solicitacaofinanceirapedido sfp USING (sfnid)
                     INNER JOIN spoemendas.periodosolicitacao prs USING (prsid)
                     INNER JOIN monitora.ptres ptr USING (ptrid)
                     LEFT JOIN public.gnd gnd ON (gnd.gndcod = sfn.sfngrupodespesa)
                   WHERE sfp.sfpstatus = 'A'
                   {$andUg}
                   ) tab ON tab.prsmes = mes.mescod::INTEGER
            GROUP BY mescod
            ORDER BY mescod;
SQL;
        return $this->carregar($sql);
    }
}