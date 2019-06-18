<?php
/**
 * Classe de mapeamento da entidade par.registrorelatorioobras.
 *
 * @version $Id$
 * @since 2018.06.14
 */
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par_Model_Registrorelatorioobras: sem descricao
 *
 * @package Par\Model
 * @uses Simec\Db\Modelo
 * @author Elias Lourenço De Oliveira <elias.oliveira@mec.gov.br>
 *        
 * @example <code>
 *          // -- Consultando registros
 *          $model = new Par_Model_Registrorelatorioobras($valorID);
 *          var_dump($model->getDados());
 *         
 *          // -- Alterando registros
 *          $valores = ['campo' => 'valor'];
 *          $model = new Par_Model_Registrorelatorioobras($valorID);
 *          $model->popularDadosObjeto($valores);
 *          $model->salvar(); // -- retorna true ou false
 *          $model->commit();
 *          </code>
 *         
 * @property int $arqid
 * @property string $rrostatus
 * @property \Datetime(Y-m-d H:i:s) $rrodata - default: now()
 * @property string $rrotipo
 * @property int $rroid - default: nextval('par.registrorelatorioobras_rroid_seq'::regclass)
 */
class Par_Model_Registrorelatorioobras extends Modelo
{

    /**
     *
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par.registrorelatorioobras';

    /**
     *
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'rroid'
    );

    /**
     *
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'arqid' => array(
            'tabela' => 'arquivo',
            'pk' => 'arqid'
        )
    );

    /**
     *
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'arqid' => null,
        'rrostatus' => null,
        'rrodata' => null,
        'rrotipo' => null,
        'rroid' => null
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados            
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'arqid' => array(
                'allowEmpty' => true,
                'Digits'
            ),
            'rrostatus' => array(
                'allowEmpty' => true,
                new Zend_Validate_StringLength(array(
                    'max' => 1
                ))
            ),
            'rrodata' => array(
                'allowEmpty' => true
            ),
            'rrotipo' => array(
                'allowEmpty' => true,
                new Zend_Validate_StringLength(array(
                    'max' => 3
                ))
            ),
            'rroid' => array(
                'Digits'
            )
        );
    }

    function getRegistro($rroid = null, $tipo = false)
    {
        if ($rroid == null && $tipo) {
            $nomeArquivo = $this->pegaUm("
                SELECT 
                    arqnome || '.' || arqextensao 
                FROM 
                    par.registrorelatorioobras rro
                INNER JOIN public.arquivo arq ON arq.arqid = rro.arqid
                WHERE 
                    rrotipo ='{$tipo}' 
                ORDER BY rroid DESC LIMIT 1");
        } elseif ($rroid != null) {
            $nomeArquivo = $this->pegaUm("
                SELECT
                arqnome || '.' || arqextensao
                FROM
                par.registrorelatorioobras rro
                INNER JOIN public.arquivo arq ON arq.arqid = rro.arqid
                WHERE
                    rroid ='{$rroid}'
                ORDER BY rroid DESC LIMIT 1");
        } else {
            return false;
        }
        
        if ($nomeArquivo != '') {
            return $nomeArquivo;
        } else {
            return false;
        }
    }

    function getNomePorArqid($arqid)
    {
        if ($arqid) {
            $nomeArquivo = $this->pegaUm("
                SELECT
                    arqnome
                FROM
                    public.arquivo arq
                WHERE
                arqid ='{$arqid}'");
        } else {
            return '';
        }
        
        if ($nomeArquivo != '') 
        {    
            return $nomeArquivo;
        } else {
            return '';
        }
    }
    
    function getDiretorioPorArqid($arqid)
    {
        if ($arqid) {
            $nomeArquivo = $this->pegaUm("
                SELECT
                    arqnome || '.' || arqextensao
                FROM
                    public.arquivo arq
                WHERE
                arqid ='{$arqid}'");
        } else {
            return '';
        }
        
        if ($nomeArquivo != '') {
            
            $diretorio = APPRAIZ . "arquivos/par/relatorioacompanhamentoobras/{$nomeArquivo}";
            
            return $diretorio;
        } else {
            return '';
        }
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
     *
     * @param $tipo =
     *            parâmetro string com dois valores possíveis 'PAR' OU 'PAC'
     * @return bool;
     *
     */
    function verificaSeExiste($tipo)
    {
        $tresHorasMenos = date("H") - 3;
        $tresHorasMenos = ($tresHorasMenos < 0) ? '00' : $tresHorasMenos;
        $dataMenosTres = date("Y-m-d {$tresHorasMenos}:i:s");
        $sql = "select rroid from par.registrorelatorioobras where rrodata > '{$dataMenosTres}' AND rrotipo = '{$tipo}' limit 1";
        
        $resultado = $this->pegaUm($sql);
        return ($resultado) ? TRUE : FALSE;
    }

    function retornaArrayRegistrosPAR()
    {
        $sql = "
			            SELECT  DISTINCT
			                    pre.preid as \"Preid\",
			                    obr.obrid as \"Obrid\",
			                    pre.predescricao as \"Nome da Obra\",
			                    pto.ptodescricao as \"Tipo da Obra\",
			                    mun.mundescricao as \"Município\",
			                    mun.estuf as \"Estado\",
			                    s.sbadsc as \"Descrição da Subação\",
			                    esd.esddsc as \"Situação\" ,
			                    str.strdsc as \"Situação Obras 2\",
			                    (SELECT
						usunome
						FROM
							workflow.historicodocumento hst
						INNER JOIN seguranca.usuario usu ON hst.usucpf = usu.usucpf
						WHERE hst.hstid = doc.hstid)  as \"Usuário\",
					    to_char(hst.htddata,'DD/MM/YYYY HH24:MI:SS') as \"Data da Situação\",
			                    sbo.sobano as \"Ano da Obra\",
			                    (select usunome from (select distinct
			                            max(hd1.htddata) as data,
			                            us1.usunome,
			                            hd1.docid,
			                            ed1.esddsc
			                    from workflow.historicodocumento hd1
			                            inner join workflow.acaoestadodoc ac1 on ac1.aedid = hd1.aedid
			                            inner join workflow.estadodocumento ed1 on ed1.esdid = ac1.esdidorigem /*AND ed1.tpdid = 45*/
			                            inner join seguranca.usuario us1 on us1.usucpf = hd1.usucpf
			                            left join workflow.comentariodocumento cd1 on cd1.hstid = hd1.hstid
			                    where
			                            ac1.esdiddestino in (328,324,325,329)
			                    and
			                            hd1.docid = pre.docid
			                    group by us1.usunome, hd1.docid, ed1.esddsc, hd1.htddata
			                    order by data desc limit 1) as foo) as \"Analista\",
								CASE WHEN pre.prevalorcontrapartida IS NOT NULL THEN COALESCE(pre.prevalorcontrapartida,0)
									 ELSE (CASE WHEN sfo.sfocontrapartidainformada IS NOT NULL THEN COALESCE(sfo.sfocontrapartidainformada, 0) ELSE COALESCE(por.pervalorcontrapartida,0) END) END AS valor_contrapartida,
			                    CASE WHEN pre.prevalorcontrapartida IS NOT NULL THEN (pre.prevalorobra - COALESCE(pre.prevalorcontrapartida,0))::numeric(20,2)
									 ELSE (pre.prevalorobra - (CASE WHEN sfo.sfocontrapartidainformada IS NOT NULL THEN COALESCE(sfo.sfocontrapartidainformada, 0) ELSE COALESCE(por.pervalorcontrapartida,0) END))::numeric(20,2)  END AS valor_mec_fnde,
			                    COALESCE(TRUNC(pre.prevalorobra,2), 0.00) as \"Valor da Obra\",
			                    to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as processo
			            FROM obras.preobra pre
				            inner JOIN workflow.documento doc ON doc.docid = pre.docid
				            inner JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esd.tpdid = 45
				            left JOIN workflow.historicodocumento hst on doc.hstid=hst.hstid
				            left JOIN territorios.municipio mun ON mun.muncod = pre.muncod
				            LEFT JOIN par.solicitacaoreformulacaoobras sfo ON sfo.preid = pre.preid AND sfo.sfostatus = 'A'
				            LEFT JOIN   (SELECT 	pr.perid,
													pr.preid,
													pr.pervalorcontrapartida
									     FROM 		obras.preobrareformulacao pr
									     WHERE 		pr.perdata = (select max(perdata) from obras.preobrareformulacao where preid = pr.preid)) por ON por.preid = pre.preid
							LEFT JOIN par.vm_saldo_empenho_por_obra se ON se.preid = pre.preid
				            left join par.subacaoobra sbo
						    	inner JOIN par.subacao s ON s.sbaid = sbo.sbaid AND s.sbastatus = 'A'
						    	INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
						    	INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
						    	INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
				            on sbo.preid = pre.preid
				            LEFT  JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND pto.ptostatus = 'A' AND pto.tooid <> 1
				            LEFT  JOIN par.processoobrasparcomposicao ppc ON ppc.preid = pre.preid and ppc.pocstatus = 'A'
			                LEFT  JOIN par.processoobraspar poc ON poc.proid = ppc.proid and poc.prostatus = 'A'
				            LEFT  JOIN obras2.obras obr
				           		INNER JOIN workflow.documento doc2 ON doc2.docid = obr.docid /*AND doc2.tpdid = 105*/
				                INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
								LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = doc2.esdid
								LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid
				            ON obr.obrid = pre.obrid AND obridpai IS NULL AND obr.obrstatus = 'A'
			     
			            WHERE pre.prestatus = 'A' AND pre.preidpai IS NULL AND pre.tooid <> 1
			            and esd.esdid not in (628, 338, 335, 332)
				    /*and pre.preid in (49511,
                                        49512,
                                        41333,
                                        33185,
                                        52405,
                                        28763,
                                        52396,
                                        52399,
                                        38656,
                                        37808,
                                        24891,
                                        45752,
                                        38760,
                                        26701,
                                        26551)*/
              /*@todo apagar*/
			            ORDER BY mun.estuf, mun.mundescricao, pre.predescricao
			       ";
        $arrRetorno = $this->carregar($sql);
        $arrRetorno = (is_array($arrRetorno)) ? $arrRetorno : Array();
        return $arrRetorno;
    }

    function geraArquivo($htmlTabela, $tipo)
    {
        if (($htmlTabela != '') && ($tipo)) {
            ob_clean();
            
            // $htmlTabela = $htmlTabela ? simec_htmlspecialchars($htmlTabela) : 'null';
            
            $nomeArquivo = "listaObras_" . $tipo . "_" . date('YmdHis');
            $diretorio = APPRAIZ . 'arquivos/par/relatorioacompanhamentoobras';
            $diretorioArquivo = APPRAIZ . 'arquivos/par/relatorioacompanhamentoobras/' . $nomeArquivo . '.txt';
            
            if (! is_dir($diretorio)) {
                mkdir($diretorio, 0777);
            }
            
            $fp = fopen($diretorioArquivo, "w");
            if ($fp) {
                stream_set_write_buffer($fp, 0);
                fwrite($fp, $htmlTabela);
                fclose($fp);
            }
            
            $sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
    			VALUES( '" . $nomeArquivo . "',
    					'txt',
    					'" . $nomeArquivo . "',
    					'text/plain',
    					'" . filesize($diretorioArquivo) . "',
    					'" . date('Y-m-d') . "',
    					'" . date('H:i:s') . "',
    					'" . $_SESSION["usucpf"] . "',
    					" . ($_SESSION['sisid'] ? $_SESSION['sisid'] : 'null') . ",
    					'A') RETURNING arqid";
            $arqid = $this->pegaUm($sql);
            
            if ($arqid) {
                $this->commit();
                return $arqid;
            } else {
                return '';
            }
        }
    }

    function verificaArquivosADeletar($tipo)
    {
        $sql = "select 
                    distinct rroid,
                    arqnome || '.' || arqextensao as nome_arquivo
                FROM 
                    par.registrorelatorioobras rro
                    INNER JOIN public.arquivo arq ON arq.arqid = rro.arqid
                WHERE 
                    rrotipo = '{$tipo}' 
                ORDER BY 
                    rroid DESC OFFSET 10";
        
        $registros = $this->carregar($sql);
        
        $registros = (is_array($registros)) ? $registros : Array();
        
        if (count($registros) > 0) {
            foreach ($registros as $k => $v) {
                $nomeArquivo = $v["nome_arquivo"];
                $arquivo = APPRAIZ . "arquivos/par/relatorioacompanhamentoobras/{$nomeArquivo}";
                echo $arquivo;
                if (is_file($arquivo)) {
                    unlink($arquivo);
                }
                $sqlDelete = "DELETE FROM par.registrorelatorioobras WHERE rroid = {$v['rroid']}";
                $this->executar($sqlDelete);
                $this->commit();
            }
        }
        return true;
    }

    function salvarRegistro($arqid, $tipo)
    {
        if ($tipo && $arqid && $_SESSION["usucpf"]) {
            
            $this->verificaArquivosADeletar($tipo);
            
            $this->executar("
                INSERT INTO par.registroRelatorioObras
                    (rrotipo, arqid, usucpf)
                VALUES
                    ('{$tipo}', {$arqid}, '{$_SESSION["usucpf"]}')
                ");
            return $this->commit();
        } else {
            return false;
        }
    }

    function listarUltimosRegistros($tipo = false)
    {
        if ($tipo) {
            
            $acoes = "'<center><img src=\"../imagens/icone_lupa.png\" style=\"cursor:pointer;\" title=\"Visualizar Pesquisa\" onclick=\"downloadXLS(\''||arq.arqid||'\');\" border=\"0\"></center>'";
            
            $sql = "SELECT
                       {$acoes}, arq.arqnome, rrotipo, to_char(rrodata, 'DD/MM/YYYY HH:MM') as data_geracao, usu.usunome
                    FROM
                        par.registrorelatorioobras rro
                    INNER JOIN public.arquivo arq ON arq.arqid = rro.arqid
                    INNER JOIN seguranca.usuario usu ON usu.usucpf = rro.usucpf
                    WHERE
                        rrotipo = '{$tipo}'
                        AND rroid in (select rro1.rroid from par.registrorelatorioobras rro1 where rro1.rrotipo = '{$tipo}' ORDER BY rroid DESC LIMIT 10) 
                    ORDER BY rroid DESC";
            
            $cabecalho = Array(
                'Arquivo',
                'Nome',
                'Tipo',
                'Data da geração do arquivo',
                'Usuário responsável'
            );
            $this->monta_lista($sql, $cabecalho, 100, 5, 'N', '95%', '', '', $tamanho, '', '');
            return true;
        }
        return false;
    }

    function retornaArrayRegistrosPAC()
    {
        $sql = "
						WITH par_entidade_entidade AS (
						SELECT e.entcodent, e.entnome FROM entidade.entidade e
						INNER JOIN obras.preobra pre ON pre.entcodent = e.entcodent
						WHERE entstatus = 'A' AND pre.prestatus = 'A' AND pre.entcodent IS NOT NULL AND e.entcodent != ''
								 AND pre.tooid = 1  AND pre.presistema = 23
						),
						par_municipio AS (
						SELECT DISTINCT mun.muncod, mun.mundescricao, mun.estuf
						FROM territorios.municipio mun
						INNER JOIN territorios.muntipomunicipio mtpm ON mtpm.muncod = mun.muncod
						INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtpm.tpmid AND tpmstatus = 'A' AND gtmid = 7
						INNER JOIN obras.preobra pre ON pre.muncod  = mun.muncod AND pre.prestatus = 'A'
						WHERE 1 = 1  AND pre.tooid = 1  AND pre.presistema = 23
						)
    	
			            SELECT  DISTINCT
			                    pre.preid as \"Preid\",
			                     oi.obrid as \"Obrid\",
			                    pre.predescricao as \"Nome da obra\",
			                    pto.ptodescricao as \"Tipo da obra\",
			                    mun.mundescricao as \"Município\",
			                    pre.estuf as \"UF\",
			                    esd.esddsc  as \"Situação\",
			                    str.strdsc as \"Situação - Obras 2.0\",
			                    usu.usunome as \"Usuário\",
			                    to_char(hst.htddata,'DD/MM/YYYY HH24:MI:SS') as \"Data da Situação\",
			                    (select usunome from (select distinct
			                                            max(hd1.htddata) as data,
			                                            us1.usunome,
			                                            hd1.docid,
			                                            ed1.esddsc
			                                    from workflow.historicodocumento hd1
			                                            inner join workflow.acaoestadodoc ac1 on
			                                                    ac1.aedid = hd1.aedid
			                                            inner join workflow.estadodocumento ed1 on
			                                                    ed1.esdid = ac1.esdidorigem AND ed1.tpdid = 37
			                                            inner join seguranca.usuario us1 on
			                                                    us1.usucpf = hd1.usucpf
			                                            left join workflow.comentariodocumento cd1 on
			                                                    cd1.hstid = hd1.hstid
			                                    where
			                                            ac1.esdiddestino in (210,211,212)
			                                    and
			                                            hd1.docid = pre.docid
			                                    group by us1.usunome, hd1.docid, ed1.esddsc, hd1.htddata
			                                    order by data desc limit 1) as foo) as \"Analista\",
			                    resnumero AS \"Resolução/Carga\",
    	
		                    	COALESCE(
		                            (
		                            CASE WHEN contra.valor_contrapartida IS NOT NULL
		                                THEN coalesce(contra.valor_contrapartida, 0)
		                                ELSE contra.prevalorcontrapartida
		                            END), 0) AS valor_contrapartida,
		                        (pre.prevalorobra -
		                    	COALESCE(
		                            (
		                            CASE WHEN contra.valor_contrapartida IS NOT NULL
		                                THEN coalesce(contra.valor_contrapartida, 0)
		                                ELSE contra.prevalorcontrapartida
		                            END), 0)
		                        )::numeric(20,2)   AS valor_fnde,
    	
			                    COALESCE(TRUNC(pre.prevalorobra,2), 0.00) as \"Valor da Obra\",
			                    to_char(po.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as \"Processo\"
    	
			            FROM obras.preobra pre
		                LEFT  JOIN par.solicitacaoreformulacaoobras    sfo ON sfo.preid = pre.preid AND sfo.sfostatus = 'A'
			            LEFT  JOIN par_entidade_entidade               et ON et.entcodent = pre.entcodent
			            INNER JOIN workflow.documento                  doc ON doc.docid   = pre.docid
			            INNER JOIN workflow.estadodocumento            esd ON esd.esdid = doc.esdid AND esd.tpdid = 37
			            left JOIN workflow.historicodocumento         hst on doc.hstid=hst.hstid
			            LEFT  JOIN seguranca.usuario                   usu ON hst.usucpf = usu.usucpf
			            LEFT  JOIN obras.pretipoobra                   pto ON pre.ptoid = pto.ptoid
			            LEFT  JOIN par_municipio                       mun ON pre.muncod  = mun.muncod
			            LEFT  JOIN par.resolucao                       res ON res.resid = pre.resid
		                 LEFT  JOIN (
		                            SELECT  poap.preid,
		                                    poap.poausucpfinclusao,
		                                    usup.usunome as nomeanalista
		                            FROM obras.preobraanalise poap
		                            LEFT JOIN seguranca.usuario usup ON usup.usucpf = poap.poausucpfinclusao
		                            ) poa ON poa.preid = pre.preid
		                LEFT  JOIN obras2.obras oi
		                    INNER JOIN workflow.documento                 doc2 ON doc2.docid = oi.docid AND doc2.tpdid = 105 AND oi.obrstatus = 'A' AND oi.obridpai IS NULL
		                    INNER JOIN workflow.estadodocumento           esd2 ON esd2.esdid = doc2.esdid
		                    LEFT JOIN obras2.situacao_registro_documento  srd  ON srd.esdid = doc2.esdid
		            		LEFT JOIN obras2.situacao_registro            str  ON str.strid = srd.strid
		                    ON oi.preid = pre.preid AND obridpai IS NULL AND oi.obrstatus = 'A'
		                LEFT JOIN par.processoobraspaccomposicao popc on popc.preid = pre.preid and popc.pocstatus = 'A'
		                LEFT JOIN par.processoobra po on po.proid = popc.proid and po.prostatus = 'A'
    	
		                LEFT  JOIN
		                    (
		                    SELECT
		                		p.preid,
		                		(
		                		CASE WHEN sr.sfocontrapartidainformada IS NOT NULL
		                            THEN coalesce(sr.sfocontrapartidainformada, 0)
		                            ELSE por.pervalorcontrapartida
		                        END) AS valor_contrapartida,
		                		p.prevalorcontrapartida
		                	FROM
		                		obras.preobra p
		                	LEFT JOIN par.solicitacaoreformulacaoobras sr ON sr.preid = p.preid and sr.sfostatus = 'A'
		                	LEFT JOIN(
		                    		SELECT pr.perid, pr.preid, pr.pervalorcontrapartida FROM obras.preobrareformulacao pr
		                    		WHERE pr.perdata = (SELECT MAX(perdata) FROM obras.preobrareformulacao WHERE preid = pr.preid)
		                    	) por ON por.preid = p.preid
			                ) contra ON contra.preid = pre.preid
    	
			     
			     
		                WHERE pre.prestatus = 'A' AND pre.preidpai IS NULL
		                AND esd.esdid NOT IN (368, 229, 213, 221)
						AND pre.tooid = 1  AND presistema = 23
			            /*AND pre.preid in (19312,
19314,
19313,
4933,
4191,
3619,
1227,
3087,
2500,
2501,
2503,
2505,
3765,
1284,
1884)*/
             /*@todo apagar*/
						";
        
        $arrRetorno = $this->carregar($sql);
        $arrRetorno = (is_array($arrRetorno)) ? $arrRetorno : Array();
        return $arrRetorno;
    }
}
