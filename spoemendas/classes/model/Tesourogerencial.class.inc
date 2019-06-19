<?php
/**
 * Classe de mapeamento da entidade spoemendas.tesourogerencial.
 *
 * @version $Id$
 * @since   2017.09.19
 */

/**
 * Spoemendas_Model_Tesourogerencial: Informações consolidadas e verificadas depois da carga
 *
 * @package Spoemendas\Model
 * @uses    Simec\Db\Modelo
 * @author  Jeferson Carlos AraÚjo <jeferson.araujo@castgroup.com.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Spoemendas_Model_Tesourogerencial($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Spoemendas_Model_Tesourogerencial($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int                    $tgid                    Identificador da tabela - default: nextval('spoemendas.tesourogerencial_tgid_seq'::regclass)
 * @property string                 $unicod                  indentificador da unidade orçamentaria
 * @property string                 $unidsc                  Descrição da Unidade Orçamentaria
 * @property string                 $orgcod                  Codigo indentificação orgão
 * @property string                 $orgdsc                  Descrição do orgão
 * @property string                 $ungcod                  Identificação Unidade gestora execução
 * @property string                 $ungdsc                  Descrição da ug executora
 * @property string                 $gestaocod               Numero indentificador gestão
 * @property string                 $ungcodprincipal         UG principal
 * @property string                 $uniabrev                Sigla da unidade orçamentaria
 * @property int                    $tgresulteof             Resultado EOF
 * @property string                 $tgdesceof               Descrição indentificação EOF
 * @property string                 $ptres                   Numero do PTRES
 * @property int                    $gndcod                  Grupo de natureza de despesa
 * @property string                 $gnddsc                  Descrição do grupo de despesa
 * @property int                    $emeano                  Ano da emenda
 * @property string                 $ne                      NE completa da carga tesouro gerencial
 * @property string                 $necompleta
 * @property string                 $tgfontesof              Fonte da SOF
 * @property string                 $tgfonterecursodetalhada Fonte de Recursos detalhada
 * @property string                 $tgdescfonterecurso      Descrição fonte de recursos detalhada
 * @property string                 $regcod                  UF da unidade beneficiada
 * @property string                 $descricaouf             Descrição da UF
 * @property string                 $mundsc                  Descrição do municipio beneficiado
 * @property string                 $tgnreferencia           Nº de Referencia
 * @property string                 $tgsisorigem             Sistema de origem
 * @property string                 $tgconvsiafi             Convenio SIAFI
 * @property double                 $tgdespempenhadas        Despesas Empenhadas
 * @property double                 $tgdespliquidas          Despesas Liquidas
 * @property double                 $tgdespliquidadaspagar   Despesas Liquidadas a pagar
 * @property double                 $tgdesppagas             Despesas Pagas
 * @property double                 $tgrapprocesspagar       RAP Processados a Pagar
 * @property double                 $tgrapnprocessliquipagar RAP não processado liquidados a pagar
 * @property double                 $tgrapprocnproc          RAP Inscritos (Processados e Não Processados)
 * @property double                 $tgpagprocnproc          RAP Pagos (Proessados e Não Processados)
 * @property double                 $tgrappagprocnproc       RAP a Pagar (Processados e Não Processados)
 * @property double                 $tgvalliquidopagar       Valores Liquidados a Pagar (Exercício + RAP)
 * @property string                 $usucpf                  Usuario responsavel pela importação
 * @property \Datetime(Y-m-d H:i:s) $tgdataprocessamento     Data do processamento - default: now()
 * @property int                    $tgidentcarga            Identificacao da carga
 * @property string                 $tsgpo                   Codigo PO originario da carga
 */
class Spoemendas_Model_Tesourogerencial extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'spoemendas.tesourogerencial';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = [
        'tgid',
    ];

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = [
        'usucpf' => ['tabela' => 'seguranca.usuario', 'pk' => 'usucpf'],
    ];

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = [
        'tgid'                    => null,
        'unicod'                  => null,
        'unidsc'                  => null,
        'orgcod'                  => null,
        'orgdsc'                  => null,
        'ungcod'                  => null,
        'ungdsc'                  => null,
        'gestaocod'               => null,
        'ungcodprincipal'         => null,
        'uniabrev'                => null,
        'tgresulteof'             => null,
        'tgdesceof'               => null,
        'ptres'                   => null,
        'gndcod'                  => null,
        'gnddsc'                  => null,
        'emeano'                  => null,
        'ne'                      => null,
        'necompleta'              => null,
        'tgfontesof'              => null,
        'tgfonterecursodetalhada' => null,
        'tgdescfonterecurso'      => null,
        'regcod'                  => null,
        'descricaouf'             => null,
        'mundsc'                  => null,
        'tgnreferencia'           => null,
        'tgsisorigem'             => null,
        'tgconvsiafi'             => null,
        'tgdespempenhadas'        => null,
        'tgdespliquidas'          => null,
        'tgdespliquidadaspagar'   => null,
        'tgdesppagas'             => null,
        'tgrapprocesspagar'       => null,
        'tgrapnprocessliquipagar' => null,
        'tgrapprocnproc'          => null,
        'tgpagprocnproc'          => null,
        'tgrappagprocnproc'       => null,
        'tgvalliquidopagar'       => null,
        'usucpf'                  => null,
        'tgdataprocessamento'     => null,
        'tgidentcarga'            => null,
        'tsgpo'                   => null
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
            'tgid'                    => ['Digits'],
            'unicod'                  => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 5])],
            'unidsc'                  => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 110])],
            'orgcod'                  => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 5])],
            'orgdsc'                  => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 110])],
            'ungcod'                  => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 6])],
            'ungdsc'                  => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 100])],
            'gestaocod'               => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 5])],
            'ungcodprincipal'         => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 6])],
            'uniabrev'                => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 50])],
            'tgresulteof'             => ['allowEmpty' => true, 'Digits'],
            'tgdesceof'               => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 100])],
            'ptres'                   => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 6])],
            'gndcod'                  => ['allowEmpty' => true],
            'gnddsc'                  => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 50])],
            'emeano'                  => ['allowEmpty' => true, 'Digits'],
            'ne'                      => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 40])],
            'necompleta'              => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 40])],
            'tgfontesof'              => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 4])],
            'tgfonterecursodetalhada' => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 10])],
            'tgdescfonterecurso'      => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 50])],
            'regcod'                  => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 2])],
            'descricaouf'             => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 45])],
            'mundsc'                  => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 50])],
            'tgnreferencia'           => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 50])],
            'tgsisorigem'             => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 30])],
            'tgconvsiafi'             => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 30])],
            'tgdespempenhadas'        => ['allowEmpty' => true],
            'tgdespliquidas'          => ['allowEmpty' => true],
            'tgdespliquidadaspagar'   => ['allowEmpty' => true],
            'tgdesppagas'             => ['allowEmpty' => true],
            'tgrapprocesspagar'       => ['allowEmpty' => true],
            'tgrapnprocessliquipagar' => ['allowEmpty' => true],
            'tgrapprocnproc'          => ['allowEmpty' => true],
            'tgpagprocnproc'          => ['allowEmpty' => true],
            'tgrappagprocnproc'       => ['allowEmpty' => true],
            'tgvalliquidopagar'       => ['allowEmpty' => true],
            'usucpf'                  => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 11])],
            'tgdataprocessamento'     => ['allowEmpty' => true],
            'tgidentcarga'            => ['allowEmpty' => true, 'Digits'],
            'tsgpo'                   => ['allowEmpty' => true, new Zend_Validate_StringLength(['max' => 4])],
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
     * @param int $idCarga
     *
     * @return int
     */
    public function saveOrUpdateTesouroGerencial ($idCarga)
    {
        $sqlUpd = <<<DML
    UPDATE spoemendas.tesourogerencial tg
    SET
        tgdespempenhadas        = ctg.ctgdespempenhadas,
        tgdespliquidas          = ctg.ctgdespliquidas,
        tgdespliquidadaspagar   = ctg.ctgdespliquidadaspagar,
        tgdesppagas             = ctg.ctgdesppagas,
        tgrapprocesspagar       = ctg.ctgrapprocesspagar,
        tgrapnprocessliquipagar = ctg.ctgrapnprocessliquipagar,
        tgrapprocnproc          = ctg.ctgrapprocnproc,
        tgpagprocnproc          = ctg.ctgpagprocnproc,
        tgrappagprocnproc       = ctg.ctgrappagprocnproc,
        tgvalliquidopagar       = ctg.ctgvalliquidopagar,
        tgidentcarga           = {$idCarga}
    FROM spoemendas.cargatesourogerencial ctg
    WHERE
        ctg.ctgidentcarga = {$idCarga} AND ctg.unicod = tg.unicod
        AND ctg.orgcod = tg.orgcod
        AND ctg.emeano = tg.emeano
        AND ctg.ne = tg.necompleta
        AND (ctgdespempenhadas != tg.tgdespempenhadas
             OR ctgdespliquidas != tg.tgdespliquidas
             OR ctgdespliquidadaspagar != tg.tgdespliquidadaspagar
             OR ctgdesppagas != tg.tgdesppagas
             OR ctgrapprocesspagar != tg.tgrapprocesspagar
             OR ctgrapnprocessliquipagar != tg.tgrapnprocessliquipagar
             OR ctgrapprocnproc != tg.tgrapprocnproc
             OR ctgpagprocnproc != tg.tgpagprocnproc
             OR ctgrappagprocnproc != tg.tgrappagprocnproc
             OR ctgvalliquidopagar != tg.tgvalliquidopagar
        );

DML;
        $resultUpd = $this->executar($sqlUpd);
        $UpdRowsAffected = pg_affected_rows($resultUpd);
        $sqlInser = <<<DML

         INSERT INTO spoemendas.tesourogerencial (
         unicod , unidsc , orgcod , orgdsc, ungcod , ungdsc , gestaocod
        , ungcodprincipal, uniabrev , tgresulteof , tgdesceof , ptres, gndcod
        , gnddsc, emeano , ne,necompleta, tgfontesof , tgfonterecursodetalhada, tgdescfonterecurso
        , regcod, descricaouf, mundsc, tgnreferencia, tgsisorigem, tgconvsiafi, tgdespempenhadas
        , tgdespliquidas, tgdespliquidadaspagar, tgdesppagas, tgrapprocesspagar, tgrapnprocessliquipagar, tgrapprocnproc
        , tgpagprocnproc, tgrappagprocnproc, tgvalliquidopagar, usucpf, tgidentcarga, tsgpo)
        (
            SELECT
               	  ctg.unicod
		        , ctg.unidsc
		        , ctg.orgcod
		        , ctg.orgdsc
		        , ctg.ungcod
		        , ctg.ungdsc
		        , ctg.gestaocod
		        , ctg.ungcodprincipal
		        , ctg.uniabrev
		        , ctg.ctgresulteof
		        , ctg.ctgdesceof
		        , ctg.ptres
		        , ctg.gndcod
		        , ctg.gnddsc
		        , ctg.emeano
		        , SUBSTR(ctg.ne,12)
		        , ctg.ne
		        , ctg.ctgfontesof
		        , ctg.ctgfonterecursodetalhada
		        , ctg.ctgdescfonterecurso
		        , ctg.regcod
		        , ctg.descricaouf
		        , ctg.mundsc
		        , ctgnreferencia
		        , ctgsisorigem
		        , ctgconvsiafi
                , ctgdespempenhadas - COALESCE (tgdespempenhadas, 0)               AS dctdespempenhadas
                , ctgdespliquidas - COALESCE (tgdespliquidas, 0)                   AS dctdespliquidas
                , ctgdespliquidadaspagar - COALESCE (tgdespliquidadaspagar, 0)     AS dctdespliquidadaspagar
                , ctgdesppagas - COALESCE (tgdesppagas, 0)                         AS dctdesppagas
                , ctgrapprocesspagar - COALESCE (tgrapprocesspagar, 0)             AS dctrapprocesspagar
                , ctgrapnprocessliquipagar - COALESCE (tgrapnprocessliquipagar, 0) AS dctrapnprocessliquipagar
                , ctgrapprocnproc - COALESCE (tgrapprocnproc, 0)                   AS dctrapprocnproc
                , ctgpagprocnproc - COALESCE (tgpagprocnproc, 0)                   AS dctpagprocnproc
                , ctgrappagprocnproc - COALESCE (tgrappagprocnproc, 0)             AS dctrappagprocnproc
                , ctgvalliquidopagar - COALESCE (tgvalliquidopagar, 0)             AS dctvalliquidopagar
                , ctg.usucpf
                , {$idCarga}
                ,ctg.ctgpo
            FROM spoemendas.cargatesourogerencial ctg
                LEFT JOIN spoemendas.tesourogerencial tg ON tg.unicod = ctg.unicod
               AND tg.orgcod = ctg.orgcod
               AND tg.emeano = ctg.emeano
               AND tg.gndcod = ctg.gndcod
               AND tg.necompleta = ctg.ne
            WHERE
                 ctg.ctgidentcarga = {$idCarga} AND tgid IS NULL
        );

DML;
//	    ver($sqlInser, $sqlUpd, d);
        $resultInsert = $this->executar($sqlInser);
        $InsertRowsAffected = pg_affected_rows($resultInsert);
        $this->commit();

        return $UpdRowsAffected + $InsertRowsAffected;
    }

    /**
     * Exporta o arquivo excel
     *
     * @param $tituloExport
     */
    public function exportExcel ($tituloExport)
    {

        //Gera um nome randomico para o arquivo
        $name = $tituloExport . md5(uniqid() . microtime(true) . mt_rand()) . '_' . date('d-m-Y') . '.xlsx';

        //Lança a string de XLSX para download pelo navegador
        header("Expires: 0");
        header("Last-Modified: " . date('d-m-Y'));
        header("Cache-Control: no-cache, must-revalidate");
        header("Content-Disposition: attachment; filename=\"{$name}\"");
        header("Content-Description: PHP Generated Data");
        header("Content-type: application/vnd.opencmlformats-officedocument.spreadsheetml.sheet");
        header('Content-type: text/csv; charset=ISO-8859-1');
        header("Pragma: no-cache");
        die;


    }

    /**
     * Escreve o excel os dados passados
     *
     * @param $dados
     * @param $cabecalho
     * @param $tituloExport
     */
    public function writeExcel ($dados, $cabecalho, $tituloExport)
    {
        $objPHPExcel = new PHPExcel();
        $lastColumn = 'AJ';
        $fieldname = [];
        foreach ($dados[0] as $key => $val)
        {
            array_push($fieldname, $key);
        }
        $nameCabecalho = [];
        foreach ($cabecalho as $key => $val)
        {
            array_push($nameCabecalho, ($val));
        }
        $countCab = 0;
        for ($column = 'A'; $column != $lastColumn; $column++)
        {
            $objPHPExcel->setActiveSheetIndex(0)
                ->setCellValue($column . 1, $nameCabecalho[$countCab]);
            $countCab++;
        }
        for ($linha = 2; $linha <= sizeof($dados) + 2; $linha++)
        {
            $count = 0;
            for ($column = 'A'; $column != $lastColumn; $column++)
            {
                $objPHPExcel->setActiveSheetIndex(0)
                    ->setCellValue($column . $linha, ($dados[$linha - 2][$fieldname[$count]]));
                $count++;
            }
        }
        $objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
        $objWriter->save('php://output');
        $this->exportExcel($tituloExport);
    }

    public function getDadosProcessados ($idCarga)
    {

        $sql = <<<DML
 SELECT
            unicod,
            unidsc,
            SUM(dctdespempenhadas)			AS dctdespempenhadas,
            SUM(dctdespempenhadasdif)			AS dctdespempenhadasdif,
            SUM(dctdespliquidas) 			AS dctdespliquidas,
            SUM(dctdespliquidasdif)			AS dctdespliquidasdif,
            SUM(dctdespliquidadaspagar) 		AS dctdespliquidadaspagar 	,
            SUM(dctdespliquidadaspagardif) 		AS dctdespliquidadaspagardif,
            SUM(dctdesppagas)				AS dctdesppagas	,
            SUM(dctdesppagasdif)			AS dctdesppagasdif,
            SUM(dctrapprocesspagar)  			AS dctrapprocesspagar,
            SUM(dctrapprocesspagardif)  		AS dctrapprocesspagardif ,
            SUM(dctrapnprocessliquipagar) 		AS dctrapnprocessliquipagar ,
            SUM(dctrapnprocessliquipagardif)  	        AS dctrapnprocessliquipagardif,
            SUM(dctrapprocnproc) 			AS dctrapprocnproc 	,
            SUM(dctrapprocnprocdif) 			AS dctrapprocnprocdif	,
            SUM(dctpagprocnproc ) 			AS dctpagprocnproc 	,
            SUM(dctpagprocnprocdif ) 			AS dctpagprocnprocdif ,
            SUM(dctrappagprocnproc ) 			AS dctrappagprocnproc ,
            SUM(dctrappagprocnprocdif) 			AS dctrappagprocnprocdif ,
            SUM(dctvalliquidopagar) 			AS dctvalliquidopagar ,
            SUM(dctvalliquidopagardif)                  AS dctvalliquidopagardif
        FROM spoemendas.diffcargatesourogerencial
        WHERE dctidentcarga = {$idCarga}
        GROUP BY
        unicod,
        unidsc
DML;

        return $this->carregar($sql);
    }

    /**
     * Recupera uma linha de registro pelo ID
     *
     * @param $sfnid
     *
     * @return array|bool|mixed|NULL
     */
    public function recuperaLinha ($sfnid)
    {
        $sql = <<<DML
			SELECT DISTINCT
				tg.tgresulteof,
				tg.regcod,
				tg.tgfontesof,
				tg.tgfonterecursodetalhada,
				tg.tgconvsiafi
-- 				tg.tgnreferencia
			FROM
				spoemendas.solicitacaofinanceira sol LEFT JOIN monitora.ptres pt
					using(ptrid) LEFT JOIN emenda.emenda em ON
				em.emeid = sol.emeid LEFT JOIN emenda.emendadetalhe ed ON
				ed.emeid = em.emeid LEFT JOIN spoemendas.tesourogerencial tg ON
				tg.unicod = sol.unicod
				AND tg.ptres::integer = pt.ptres::INTEGER
				AND tg.emeano::integer = em.emeano::INTEGER
				AND tg.gndcod = ed.gndcod
			WHERE
				sol.sfnid = {$sfnid}
DML;

        return $this->carregar($sql);
    }

    public function getUnidadesPerfil ($usucpf, $pflcod)
    {
        $pflcod = implode(",", $pflcod);
//        $sql = "SELECT DISTINCT unicod FROM spoemendas.usuarioresponsabilidade WHERE usucpf = '{$usucpf}' AND rpustatus = 'A' AND pflcod IN ({$pflcod})";
        $sql = <<<SQL
            WITH temp_unidade AS (
                SELECT 
                    unicod
                FROM spoemendas.usuarioresponsabilidade
                WHERE
                    usucpf = '%s'
                    AND rpustatus = 'A'
                    AND pflcod IN(%s)
                    AND unicod IS NOT NULL 
                
                UNION 
                
                SELECT 
                    ung.unicod
                FROM public.unidadegestora ung
                INNER JOIN spoemendas.usuarioresponsabilidade rpu USING(ungcod)
                WHERE 
                    usucpf = '%s'
                    AND rpustatus = 'A'
                    AND pflcod IN(%s)
            )
            
            SELECT DISTINCT * FROM temp_unidade;
SQL;
        $sql = sprintf($sql, $usucpf, $pflcod, $usucpf, $pflcod);

        return $this->carregar($sql);
    }

    public function getUg($unicod = null, $json = false)
    {
        $unicods = '';
        if (is_array($unicod)) {
            if (sizeof($unicod) > 1) {
                foreach ($unicod as $key => $val) {
                    $unicods .= '\'' . $val['unicod'] . '\'' . ",";
                }

                $unicods = rtrim($unicods, ',');
            } else {
                $unicods = '\'' . $unicod[0]['unicod'] . '\'';
            }
        } else {
            $unicods = '\''.$unicod.'\'';
        }

        $andUnicod = $unicods != "''" ? "AND uni.unicod IN ({$unicods}) " : '';
        $andUsucpf = "AND rpu.usucpf = '".$_SESSION['usucpf']."'";
        $join = "INNER JOIN spoemendas.usuarioresponsabilidade rpu USING(ungcod)";
        $perfis = pegaPerfilGeral($_SESSION['usucpf']);

        if (in_array(PFL_SUPER_USUARIO, $perfis)
            || in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)
            || in_array(PFL_CGF_EQUIPE_FINANCEIRA, $perfis)) {
            $andUsucpf = "";
            $join = "";
        }

        $sql = "SELECT DISTINCT ung.ungcod as codigo,
                ung.ungcod || 
                CASE uni.uniabrev WHEN 'SPO/MEC' THEN 'MEC' 
                ELSE COALESCE(' - ' || uni.uniabrev, '') END 
                || ' - ' || ung.ungdsc as descricao
                FROM public.unidadegestora ung
                {$join}
                INNER JOIN unidade uni ON ung.unicod = uni.unicod
                WHERE 1=1
                {$andUnicod} {$andUsucpf}
                ORDER BY codigo ASC";

        if ($json) {
            return simec_json_encode(
                formatarCombo($this->carregar($sql), 'codigo', 'descricao'));
        } else {
            $ret = $this->carregar($sql);
            if (empty($ret)) {
                $ret = ['codigo' => '', 'descricao' => ''];
            }
            return $ret;
        }
    }

    public function getUf ($usucpf, $pflcod)
    {
        $sql = <<<DML
            SELECT DISTINCT tgufbeneficiada AS codigo, tgufbeneficiada AS descricao FROM spoemendas.tesourogerencial
            WHERE unicod = (SELECT unicod
                    FROM spoemendas.usuarioresponsabilidade
                    WHERE usucpf = '{$usucpf}'
                    AND rpustatus = 'A'
                    AND pflcod = {$pflcod}
            )::integer
            ORDER BY tgufbeneficiada ASC
DML;

        return $this->carregar($sql);
    }

    public function getSiafi ($tgconvsiafi)
    {
        $sql = <<<DML
			SELECT DISTINCT sf.sfnconveniosiafi FROM spoemendas.tesourogerencial tg
			inner join spoemendas.solicitacaofinanceira sf on tg.unicod = sf.unicod
			WHERE sf.sfnconveniosiafi::integer = '{$tgconvsiafi}'
DML;

        $sfnconveniosiafi = $this->carregar($sql);

        return $sfnconveniosiafi[0]['sfnconveniosiafi'];
    }

    public function getRefSiafi ($usucpf, $pflcod)
    {
        $sql = <<<DML
			SELECT DISTINCT sfnnumeroreferencia FROM spoemendas.tesourogerencial tg
			inner join spoemendas.solicitacaofinanceira sf on tg.unicod = sf.unicod::integer
			WHERE tg.unicod = (SELECT unicod
                    FROM spoemendas.usuarioresponsabilidade
                    WHERE usucpf = '{$usucpf}'
                    AND rpustatus = 'A'
                    AND pflcod = {$pflcod}
            )::integer
			AND sfnconveniosiafi IS NOT NULL
DML;

        $sfnnumeroreferencia = $this->carregar($sql);

        return $sfnnumeroreferencia[0]['sfnnumeroreferencia'];
    }

    /**
     * Método responsável por recuperar combo de unidade
     *
     * @author Victor Eduardo Barreto
     * @filesource
     * @throw  Mensagem de erro
     * @return string Query
     */
    public function comboUnidade ()
    {
        $sql = <<<DML
			SELECT unicod AS codigo,
			       unicod || ' - ' || unidsc AS descricao
			  FROM unidade
			  WHERE orgcod = '26000'
			  ORDER BY unidsc
DML;

        return $sql;
    }

    public function detalheSolicitacao ($sfnid)
    {
        $sql = <<<DML
SELECT DISTINCT sof.exercicio,
                prs.prsdescricao,
                TO_CHAR(prs.prsdatainicio, 'DD/MM/YYYY') AS prsdatainicio,
                TO_CHAR(prs.prsdatafim, 'DD/MM/YYYY') AS prsdatafim,
                aut.autnome,
                eme.emecod,
                sol.autnome AS sfninteressado,
                sof.sfnugexecutora,
                ptr.ptres,
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
        WHERE sfp.sfnid = sof.sfnid AND sfp.sfpstatus = 'A')sfnvlrsolicitado,
                sof.unicod,
                uni.unidsc,
                ene.ne,
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
    LEFT JOIN spoemendas.solicitacaofinanceirapedido sfp USING(sfnid)
    LEFT JOIN spoemendas.periodosolicitacao prs USING(prsid)
    INNER JOIN public.unidade uni USING(unicod)
    INNER JOIN emenda.autor aut USING(autid)
    INNER JOIN workflow.documento doc USING(docid)
    INNER JOIN workflow.estadodocumento esd USING(esdid)
    INNER JOIN emenda.emenda eme USING(emeid)
    INNER JOIN monitora.ptres ptr USING(ptrid)
    LEFT JOIN emenda.emendadetalhe emd USING(emeid)
    LEFT JOIN territorios.municipio mun USING(muncod)
    LEFT JOIN spoemendas.emendane ene ON (ene.exercicio = sof.exercicio AND ene.emecod = eme.emecod)
    LEFT JOIN emenda.autor sol ON (sof.sfninteressado = sol.autid)
    LEFT JOIN public.gnd gnd ON (gnd.gndcod = sof.sfngrupodespesa)
  WHERE sfnid = {$sfnid}
DML;
        $dados = $this->pegaLinha($sql);
        $dados['sfnvlrsolicitado'] = mascaraMoeda($dados['sfnvlrsolicitado'], false);

        return $dados;
    }

    public function verificaQtdDiffTesouro ($carga)
    {
        $sql = "select dctid from spoemendas.diffcargatesourogerencial where dctidentcarga = {$carga}";

        return $this->carregar($sql);
    }

    /**
     * Recupera uma linha de registro pelo ID
     *
     * @param $sfnid
     *
     * @return array|bool|mixed|NULL
     */
    public function getNumReferencia ($sfnid)
    {
        $sql = <<<DML
			select DISTINCT
				tg.tgnreferencia as codigo,
				tg.tgnreferencia as descricao
			from
				spoemendas.solicitacaofinanceira sol
				left join monitora.ptres pt	using(ptrid)
				left join emenda.emenda em on em.emeid = sol.emeid
				left join emenda.emendadetalhe ed on ed.emeid = em.emeid
				left join spoemendas.tesourogerencial tg on	tg.unicod::integer = sol.unicod::integer
														and tg.ptres::integer = pt.ptres::INTEGER
														and tg.emeano::integer = em.emeano::INTEGER
														and tg.gndcod = ed.gndcod
			where sol.sfnid = {$sfnid}
DML;

        return $this->carregar($sql);
    }

    /**
     * Recupera uma linha de registro pelo ID
     *
     * @param $ptres
     * @param $emeid
     * @param $unicod
     *
     * @return array|mixed|NULL
     */
    public function getNumReferenciaDireta ($ptres, $emeid, $unicod)
    {
        $sql = <<<DML
			select DISTINCT
				tg.tgnreferencia as codigo,
				tg.tgnreferencia as descricao
			from monitora.ptres pt
				left join emenda.emenda em on em.emeid = {$emeid}
				left join emenda.emendadetalhe ed on ed.emeid = em.emeid
				left join spoemendas.tesourogerencial tg on	tg.unicod::integer = {$unicod}
														and tg.ptres::integer = pt.ptres::INTEGER
														and tg.emeano::integer = em.emeano::INTEGER
														and tg.gndcod = ed.gndcod
            WHERE pt.ptrid = {$ptres}
DML;

        return $this->carregar($sql);
    }

    /**
     * Função que retorna o código do munícipio (muncod) a partir do ID do tesouro gerencial (tgid).
     *
     * @param null|int $tgid
     * @param boolean  $mostraMsg
     *
     * @return bool|mixed|NULL|string
     * @throws Exception
     */
    public function getMuncod ($tgid = null, $mostraMsg = true)
    {
        $tgid = empty($tgid) ? $this->tgid : $tgid;
        if (empty($tgid) && $mostraMsg)
        {
            throw new Exception('A NE digitada não foi encontrada na base de dados. Em caso de dúvida, entrar em contato com a CGF.');
        }
        else
        {
            $sql = <<<SQL
                select
                  muncod
                from spoemendas.tesourogerencial tg
                inner join territorios.municipio mun on translate(lower(tg.mundsc), 'áàâãäåaaaÁÂÃÄÅAAAÀéèêëeeeeeEEEÉEEÈìíîïìiiiÌÍÎÏÌIIIóôõöoooòÒÓÔÕÖOOOùúûüuuuuÙÚÛÜUUUUçÇñÑýÝ','aaaaaaaaaAAAAAAAAAeeeeeeeeeEEEEEEEiiiiiiiiIIIIIIIIooooooooOOOOOOOOuuuuuuuuUUUUUUUUcCnNyY') = translate(lower(mun.mundescricao), 'áàâãäåaaaÁÂÃÄÅAAAÀéèêëeeeeeEEEÉEEÈìíîïìiiiÌÍÎÏÌIIIóôõöoooòÒÓÔÕÖOOOùúûüuuuuÙÚÛÜUUUUçÇñÑýÝ','aaaaaaaaaAAAAAAAAAeeeeeeeeeEEEEEEEiiiiiiiiIIIIIIIIooooooooOOOOOOOOuuuuuuuuUUUUUUUUcCnNyY')
                WHERE tgid = {$tgid}
SQL;
            $muncod = $this->pegaUm($sql, 'muncod');

            return $muncod;
        }
    }

    /**
     * Carrega o registro do tesouro gerencial de acordo com o PTRES, com o GND e com a Nota de Empenho
     *
     * @param $ptres
     * @param $gnd
     * @param $ne
     *
     * @return $this
     */
    public function carregarPorPtresGndNe ($ptres, $gnd, $ne)
    {
        $sql = <<<DML
            SELECT
                tgid
            FROM spoemendas.tesourogerencial tgs
            INNER JOIN monitora.ptres ptr USING (ptres)
            WHERE ptrid = '{$ptres}'
            AND gndcod = '{$gnd}'
            AND ne = '{$ne}';
DML;

        $res = $this->pegaUm($sql);

        if (!empty($res))
        {
            $this->carregarPorId($res);
        }

        return $this;
    }

    /**
     * @param null $resulteof
     * @param null $po
     *
     * @return int|null
     */
    public function tratarResulteof ($resulteof = null, $po = null)
    {
        if (is_null($resulteof))
        {
            $resulteof = $this->tgresulteof;
        }

        if (is_null($po))
        {
            $po = $this->tsgpo;
        }

        $codvinculacao = null;

        // -- Calculando valor da vinculaçao com base no Resultado EOF, ou salvo no banco
        switch ($resulteof)
        {
            case 2:
                $codvinculacao = 406; //'CUSTEIO/INVEST.-DEMAIS EMENDAS INDIVIDUAIS';
                break;
            case 3:
                switch ($po)
                {
                    case 'EBPM':
                        $codvinculacao = 404;
                        break;
                    case 'EIND':
                        $codvinculacao = 411; //'CUSTEIO/INVEST. PAC-EMENDAS';
                        break;
                }
                break;
            case 6:
                $codvinculacao = 405; //'CUSTEIO/INVEST.-EMENDAS INDIVIDUAIS';
                break;
            case 7:
                $codvinculacao = 410; //'CUST/INV-EMENDAS IMPOSITIVAS DE BANCADA RP7';
                break;
        };

        return $codvinculacao;
    }

    public function pegaDataUltimaCarga()
    {
        $strSQL = <<<DML
SELECT 
	TO_CHAR(tgdataprocessamento, 'DD/MM/YYYY') AS tgdataprocessamentof
FROM {$this->stNomeTabela}
GROUP BY tgdataprocessamento
ORDER BY tgdataprocessamento DESC LIMIT 1
DML;
        return $this->pegaUm($strSQL);
    }
}
