<?php
/**
 * Classe de mapeamento da entidade spoemendas.solicitacaofinanceirapedido.
 *
 * @version $Id$
 * @since   2017.09.11
 */

/**
 * Spoemendas_Model_Solicitacaofinanceirapedido: tabela com os pedidos de solicitacao financeira por mes
 *
 * @package Spoemendas\Model
 * @uses    Simec\Db\Modelo
 * @author  Saulo Araújo Correia <saulo.correia@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Solicitacaofinanceirapedido($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Solicitacaofinanceirapedido($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int                    $sfpid              - default: nextval('spoemendas.solicitacaofinanceirapedido_sfpid_seq'::regclass)
 * @property int                    $sfnid              chave do spoemendas.solicitacaofinanceira
 * @property int                    $prsid              chave do spoemendas.periodosolicitacao
 * @property numeric                $sfpvalorpedido     valor pedido
 * @property numeric                $sfpvalorautorizado valor autorizado
 * @property \Datetime(Y-m-d H:i:s) $sfpdatapedido      data do pedido - default: now()
 * @property \Datetime(Y-m-d H:i:s) $sfpdataautorizacao data da autorizacao
 * @property int                    $sfsid              chave do spoemendas.solicitacaofinanceirasituacao
 * @property string                 $usucpf
 * @property string                 $sfpstatus          status ativo-inativo  - default: 'A'::bpchar
 */
class Spoemendas_Model_Solicitacaofinanceirapedido extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.solicitacaofinanceirapedido';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = [
        'sfpid',
    ];

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = [
        'sfsid' => ['tabela' => 'spoemendas.solicitacaofinanceirasituacao', 'pk' => 'sfsid'],
        'prsid' => ['tabela' => 'spoemendas.periodosolicitacao', 'pk' => 'prsid'],
        'sfnid' => ['tabela' => 'spoemendas.solicitacaofinanceira', 'pk' => 'sfnid'],
    ];

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = [
        'sfpid'              => null,
        'sfnid'              => null,
        'prsid'              => null,
        'sfpvalorpedido'     => null,
        'sfpvalorautorizado' => null,
        'sfpdatapedido'      => null,
        'sfpdataautorizacao' => null,
        'sfsid'              => null,
        'usucpf'             => null,
        'sfpstatus'          => null,
    ];

    /**
     * Validators.
     *
     * @param mixed[] $dados
     *
     * @return mixed[]
     */
    public function getCamposValidacao ($dados = [])
    {
        return [
            'sfpid'              => ['Digits'],
            'sfnid'              => ['Digits'],
            'prsid'              => ['Digits'],
            'sfpvalorpedido'     => [],
            'sfpvalorautorizado' => ['allowEmpty' => true],
            'sfpdatapedido'      => [],
            'sfpdataautorizacao' => ['allowEmpty' => true],
            'sfsid'              => ['Digits'],
            'usucpf'             => [new Zend_Validate_StringLength(['max' => 11])],
            'sfpstatus'          => [new Zend_Validate_StringLength(['max' => 1])],
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
    public function antesSalvar ()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    /**
     * buscar por SNFID (spoemendas.solicitacaofinanceira)
     *
     * @param      $sfnid
     * @param bool $retornarQuery
     *
     * @return array|NULL|string
     */
    public function buscarPorSfnid ($sfnid, $retornarQuery = false)
    {
        $sql = <<<SQL
SELECT
    sfpid
    , to_date (prsmes || '/' || prsano, 'MM/YYYY') AS mes
    , sfpvalorpedido
    , sfpvalorautorizado
    , COALESCE(TO_CHAR(sfpdataautorizacao, 'DD/MM/YYYY'), '-') sfpdataautorizacao
    , sit.sfsdescricao
    , CASE
     WHEN sfpdataautorizacao IS NOT NULL THEN 0
     WHEN NOW()::DATE BETWEEN prs.prsdatainicio AND prs.prsdatafim THEN 1 ELSE  0 END periodoatual
FROM {$this->stNomeTabela} sfp
JOIN spoemendas.solicitacaofinanceirasituacao sit USING (sfsid)
JOIN spoemendas.periodosolicitacao prs using (prsid)
WHERE sfnid = {$sfnid}
AND sfp.sfpstatus = 'A'
ORDER BY prsano desc, prsmes ASC
SQL;

        if ($retornarQuery)
        {
            return $sql;
        }

        return $this->carregar($sql);
    }

    public function inativarTodos ($sfnid)
    {
        $sql = <<<SQL
UPDATE {$this->stNomeTabela}
   SET sfpstatus = 'I'
 WHERE sfnid = {$sfnid}
SQL;
        return $this->executar($sql);
    }

    public function inativar ($sfpid)
    {
        $sql = <<<SQL
UPDATE {$this->stNomeTabela}
   SET sfpstatus = 'I'
 WHERE sfpid = {$sfpid}
SQL;
        return $this->executar($sql);
    }

    public function atualizaValorPedido($dados)
    {
        $dados['valorsolicitacaop'] = desformata_valor($dados['valorsolicitacaop']);
        $dados['situacao'] = $this->verificaValorSituacao($dados['valorsolicitacaop']);

        $SQL = <<<SQL
            SELECT
                *
            FROM
                {$this->stNomeTabela}
            WHERE
                sfpid = {$dados['sfpid']}
SQL;
        $result = $this->pegaLinha($SQL);
        $this->persisteValorSolicitado($result,$dados);
    }

    private function persisteValorSolicitado($result, $dados)
    {
        $result['sfpvalorpedido'] = floatval($dados['valorsolicitacaop']);
        $result['sfsid'] = $dados['situacao'];

        try {
            $this->inativar($dados['sfpid']);

            unset($result['sfpid']);
            $this->popularDadosObjeto($result);
            $this->salvar();
            $this->commit();

            // atualiza historico.
            (new Spoemendas_Model_Solicitacaofinanceirahistorico())->historicoEditarPedido($result);

            (new Simec_Helper_FlashMessage('solicitacao'))->addMensagem('Solicitação atualizada com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
        } catch (Exception $ex) {
            (new Simec_Helper_FlashMessage('solicitacao'))->addMensagem('Erro ao atualizar Solicitação!', Simec_Helper_FlashMessage::ERRO);
        }
    }
    private function verificaValorSituacao($val){
        if($val == '0,00' || $val == '0'){
                return Spoemendas_Model_Solicitacaofinanceirasituacao::NAO_SOLICITADO;
        }
        return Spoemendas_Model_Solicitacaofinanceirasituacao::PENDENTE_DE_AUTORIZACAO;
    }

    public function verificaPeriodoReferenciaNotaEmpenho($carga, $pedido)
    {
        $referencia = explode('/', $carga->casmesdereferencia);
        $casmesdereferencia = (int)$referencia[0];
        $casanodereferencia = (int)$referencia[1];

        if ($pedido->prsmes == $casmesdereferencia
            && $pedido->prsano == $casanodereferencia
            && $pedido->sfnnotaempenho == $carga->casne) {
            return true;
        }

        return false;
    }

    public function getPedidoCarga($carga)
    {
        $periodo = explode('/',$carga->casmesdereferencia);

        $sql = <<<DML
            SELECT
                    sfp.sfpid,
                    --sf.sfnid,
                    sfp.sfpvalorpedido,
                    sfp.sfpvalorautorizado,
                    sfp.sfpdataautorizacao,
                    sf.sfnnotaempenho,
                    prs.prsmes,
                    prs.prsano,
                    sf.unicod
            FROM spoemendas.solicitacaofinanceirapedido sfp
            INNER JOIN spoemendas.solicitacaofinanceira sf ON (sf.sfnid = sfp.sfnid AND sf.sfnstatus = 'A')
            INNER JOIN spoemendas.periodosolicitacao prs USING (prsid)
            WHERE
                    sf.sfnnotaempenho = '{$carga->casne}'
                    AND sf.emeid IN (SELECT emeid FROM emenda.emenda WHERE emecod = '{$carga->emecod}' AND emeano = '{$carga->emeano}')
                    AND sf.ptrid IN (SELECT ptrid FROM monitora.ptres WHERE ptrstatus = 'A' AND ptres = '{$carga->ptres}')
                    AND sf.unicod = '{$carga->unicod}'
                    AND sfp.sfpstatus = 'A'
                    AND sfp.sfpdataautorizacao IS NULL
                    AND prs.prsmes = {$periodo[0]}
                    AND prs.prsano = {$periodo[1]}
DML;

        return $this->pegaLinha($sql);
    }

    public function atualizaPedido ($idPedido, $carga)
    {
        $this->carregarPorId($idPedido);

        if (!empty($this->sfpdataautorizacao))
        {
            return false;
        }

        $this->sfpvalorautorizado = $carga->casvlrautorizado;
        $this->sfpdataautorizacao = $carga->casdtaautorizacao;

        if ($this->sfpvalorpedido == $carga->casvlrautorizado)
        {
            $this->sfsid = Spoemendas_Model_Solicitacaofinanceirasituacao::AUTORIZADO;
        }
        elseif ($this->sfpvalorpedido > $carga->casvlrautorizado && $carga->casvlrautorizado > 0)
        {
            $this->sfsid = Spoemendas_Model_Solicitacaofinanceirasituacao::AUTORIZADO_PARCIALMENTE;
        }
        elseif (floatval($carga->casvlrautorizado) == 0 || $carga->casvlrautorizado == '')
        {
            $this->sfsid = Spoemendas_Model_Solicitacaofinanceirasituacao::NAO_AUTORIZADO;
        }

        $this->salvar();
        $this->commit();

        (new Spoemendas_Model_Cargasegov())->alteraStatus($carga->casid);
        return true;
    }

    public function criarNovaLinhaNaoSolicitado($prsidAtual, $prsidNovo)
    {
        $sfsid = Spoemendas_Model_Solicitacaofinanceirasituacao::NAO_SOLICITADO;
        $esdEnviado = ESD_ENVIADO;
        $sql = <<<SQL
            SELECT
                sfp.*
            FROM
                spoemendas.solicitacaofinanceira sf
            JOIN spoemendas.solicitacaofinanceirapedido sfp USING(sfnid)
            JOIN workflow.documento wd USING (docid)
            WHERE
                sfsid = {$sfsid}
                AND sfpstatus = 'A'
                AND esdid = {$esdEnviado}
                AND prsid = {$prsidAtual}
                AND NOT EXISTS (
                    SELECT 1
                    FROM spoemendas.solicitacaofinanceirapedido sfp2
                    WHERE sfp.sfnid = sfp2.sfnid
                    AND sfp2.sfpstatus = 'A'
                    AND sfp2.prsid = {$prsidNovo}
                );
SQL;
        $results = $this->carregar($sql);

        if (empty($results))
        {
            return;
        }

        $modelsfp = new Spoemendas_Model_Solicitacaofinanceirapedido();
        foreach($results as $result){
            $modelsfp->popularDadosObjeto($result);

            $modelsfp->sfpid = null;
            $modelsfp->sfpdatapedido = null;
            $modelsfp->prsid = $prsidNovo;
            $modelsfp->usucpf = $_SESSION['usucpf'];
            $modelsfp->salvar();
        }

        $modelsfp->commit();
    }

    /**
     * Retorna total de pedidos de solicitação financeira, pedido ativos independente da situação
     * @param null $situacao              situação do pedido financeiro
     * @param null $esdid                 status do workflow
     * @return array|mixed|NULL
     */
    public function retornaTotalPedidos($situacao = null, $esdid = null)
    {
        $naoEnviado = ESD_ENVIADO;

        $strSQL = <<<DML
SELECT
    sof.sfnid,
    sof.exercicio,
    eme.emecod,
    sof.unicod,
    uni.uniabrev,
    uni.unidsc,
    aut.autnome,
    CASE
        WHEN esd.esdid = {$naoEnviado} THEN 'Sim'
        ELSE 'Não'
    END,
    ene.sfnnotaempenho,
    ptr.ptres,
    mun.mundescricao,
    sof.estuf,
    sof.sfngrupodespesa,
    sol.autnome AS sfninteressado,
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
        JOIN spoemendas.periodosolicitacao prs ON
            prs.prsid = sfp.prsid
            AND prs.prsstatus
            AND NOW()::DATE BETWEEN prsdatainicio AND prsdatafim
        LEFT JOIN spoemendas.solicitacaofinanceirasituacao sfs ON
            sfs.sfsid = sfp.sfsid
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
    ) AS periodoaberto
FROM
    spoemendas.solicitacaofinanceira sof
INNER JOIN public.unidade uni
        USING(unicod)
INNER JOIN emenda.autor aut
        USING(autid)
LEFT JOIN workflow.documento doc
        USING(docid)
LEFT JOIN workflow.estadodocumento esd
        USING(esdid)
INNER JOIN emenda.emenda eme
        USING(emeid)
INNER JOIN monitora.ptres ptr
        USING(ptrid)
INNER JOIN spoemendas.solicitacaofinanceira ene
        USING(sfnid)
LEFT JOIN emenda.autor sol ON
    (
        sof.sfninteressado = sol.autid
    )
LEFT JOIN public.gnd gnd ON
    (
        gnd.gndcod = sof.sfngrupodespesa
    )
LEFT JOIN territorios.municipio mun ON
    (
        mun.muncod = sof.muncod
    )
WHERE
    sof.sfnstatus = 'A'
    AND (
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
                AND sof2.sfnid = sof.sfnid) = (SELECT EXTRACT(month FROM CURRENT_DATE))
    %s
ORDER BY
    sof.exercicio DESC,
    uni.unicod,
    eme.emecod,
    ene.sfnnotaempenho
DML;

        $andWhere = '';
        if (null !== $situacao) {
            $andWhere.= " AND (
                        SELECT
                            sfs.sfsid
                        FROM
                            spoemendas.solicitacaofinanceirapedido sfp
                        LEFT JOIN spoemendas.solicitacaofinanceirasituacao sfs ON
                            sfs.sfsid = sfp.sfsid
                        WHERE
                            sfp.sfnid = sof.sfnid
                            AND sfp.sfpstatus = 'A'
                    ) = {$situacao} ";
        }

        if (null !== $esdid) {
            $andWhere.= " AND (esd.esdid = {$esdid}) ";
        }

        $result = $this->carregar(sprintf($strSQL, $andWhere));
        return (!$result) ? 0 : count($result);
    }
}
