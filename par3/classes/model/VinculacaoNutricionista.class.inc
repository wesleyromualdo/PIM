<?php

/**
 * Classe de mapeamento da entidade par3.vinculacaonutricionista
 *
 * @category Class
 * @package  A1
 * @author   MARCUS VINÍCIUS LOUVEM ROCHA <marcus.rocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-12-2015
 * @link     no link
 */



/**
 * Vinculacaonutricionista
 *
 * @category Class
 * @package  A1
 * @author   MARCUS VINÍCIUS LOUVEM ROCHA <marcus.rocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-12-2015
 * @link     no link
 */
class Par3_Model_VinculacaoNutricionista extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.vinculacaonutricionista';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'vnid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
        'tenid' => array('tabela' => 'par3.tipoentidade', 'pk' => 'tenid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'vnid' => null,
        'vncpf' => null,
        'inuid' => null,
        'vndatavinculacao' => null,
        'vndatadesvinculacao' => null,
        'vnmotivodesvinculacao' => null,
        'vntermodesvinculacao' => null,
        'vnassinaturadesvinculacao' => null,
        'vndataassinaturadesvinculacao' => null,
        'vnstatus' => null,
        'vncargahorariasemanal' => null,
        'snid' => null,
        'tenid' => null,
        'dntvid' => null,
        'snaceito' => null,
        'vnatuacaoexclusivaei' => null,
        'usucpfalteracao' => null,
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'vnid' => array(  'Digits'  ),
            'vncpf' => array(  new Zend_Validate_StringLength(array('max' => 11))  ),
            'inuid' => array( 'allowEmpty' => true, 'Digits'  ),
            'vndatavinculacao' => array( 'allowEmpty' => true ),
            'vndatadesvinculacao' => array( 'allowEmpty' => true ),
            'vnmotivodesvinculacao' => array( 'allowEmpty' => true ),
            'vntermodesvinculacao' => array( 'allowEmpty' => true ),
            'vnassinaturadesvinculacao' => array( 'allowEmpty' => true ),
            'vndataassinaturadesvinculacao' => array( 'allowEmpty' => true ),
            'vnstatus' => array(  new Zend_Validate_StringLength(array('max' => 1))  ),
            'vncargahorariasemanal' => array( 'allowEmpty' => true, 'Digits'  ),
            'snid' => array( 'allowEmpty' => true, 'Digits'  ),
            'tenid' => array( 'allowEmpty' => true, 'Digits'  ),
            'dntvid' => array( 'allowEmpty' => true, 'Digits'  ),
            'snaceito' => array( 'allowEmpty' => true ),
            'vnatuacaoexclusivaei' => array( 'allowEmpty' => true ),
            'usucpfalteracao' => array( 'allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))  ),
        );
    }//end getCamposValidacao($dados)


    public function montarFiltroSQLListaVinculos($arrPost = array())
    {
        $where = Array();

        if($arrPost['dancrnuf'] != ''){
    	    $where[] = "dancrnuf = '{$arrPost['dancrnuf']}'";
        }

        if($arrPost['dancpf'] != ''){
    	    $where[] = "dancpf = '{$arrPost['dancpf']}'";
        }

        if($arrPost['nome'] != ''){
    	    $where[] = "usu.usunome ilike '%{$arrPost['nome']}%'";
        }

        if($arrPost['usucpf'] != ''){
            $usucpf = str_replace(array('.', '-', '/', ' '), '',$arrPost['usucpf']);
            $where[] = "usu.usucpf = '{$usucpf}'";
        }
        if($arrPost['vncpf'] != ''){
          $where[] = "vn.vncpf= '{$arrPost['vncpf']}'";
        }

        if($arrPost['entcpf'] != '') {
            $where[] = "iue.entcpf= '{$arrPost['entcpf']}'";

        }

        if($arrPost['snid'] != ''){
            if (is_array($arrPost['snid'])) {
                $where[] = "vn.snid IN (".  implode($arrPost['snid'], ',').")";
            } else {
                $where[] = "vn.snid = '{$arrPost['snid']}'";
            }
        }

        if($arrPost['vnstatus'] != ''){
    	    $where[] = "vn.vnstatus = '{$arrPost['vnstatus']}'";
        }

        if($arrPost['validacao'] != ''){

            $not = $arrPost['validacao'] == 'n' ? 'NOT' : '';

    	    $where[] = "inu.inuid $not IN (SELECT
                                    	inuid
                                      FROM
                                    	par3.vinculacaonutricionista
                                      WHERE
                                    	tenid = 7
                                    	AND snid = 1)";
        }

        if($arrPost['tenid'] != ''){
            $arrTenid = array_filter($arrPost['tenid']);
            if(count($arrTenid) != 0){
                $arrTenid = implode(',',$arrTenid);
                $where[] = "vn.tenid in ({$arrTenid})";
            }
        }



        return $where;
    }//end montarSQLListaVinculos();


    public function montarSQLListaVinculos($arrPost = array())
    {

        $where = $this->montarFiltroSQLListaVinculos($arrPost);
        // Caso tenha municípios no filtro
        $where[] = $arrPost['itrid'] && $arrPost['itrid'] != 0 ?($arrPost['itrid'] == 1 ? " inu.itrid in(1,3)" :" inu.itrid = {$arrPost['itrid']}"): '';

        if($arrPost['muncod'] && count($arrPost['muncod']) != 0 ){
            $arrMuncod = tratarArrayParaMultiSelect($arrPost['muncod']);
            if(count($arrMuncod) != 0) {
                $where[] = " inu.muncod::integer in (".implode(',',$arrMuncod).")";
            }
        }

        if($arrPost['estuf'] && count($arrPost['estuf']) != 0 ) {
            $arrEstuf = implode(',',array_map(function($estuf){return (string)"'".$estuf."'";},$arrPost['estuf']));
            $where[] = $arrPost['estuf'] ? " inu.estuf in ({$arrEstuf})":'';
        }

        $innerdntvid = '';
        if($arrPost['dntvid']){
            $innerdntvid = 'inner join par3.dadosnutricionistatipovinculo dnt  on vn.dntvid = dnt.dntvid';
        }
        //retira os índices sem valor e reseta os índices do array
        $where  = array_values(array_filter($where));
        $sql = "SELECT
                	CASE
                		WHEN sn.snid IN (1, 4, 5) THEN TRUE
                		ELSE FALSE
                	END as acao,
                	inu.estuf,
                	CASE
                		WHEN inu.itrid IN (1, 3) THEN 'Estadual'
                		ELSE 'Municipal'
                	END as esfera,
                	mun.mundescricao,
                	mun.muncod,
                	vn.vncpf,
                	usu.usunome as usunome,
                	dan.dancrn,
                	dan.dancrnuf,
                	dan.dancrn||'/'||dan.dancrnuf as dan_crn_uf,
                	CASE
                		WHEN vnstatus = 'A' THEN 'Vinculado'
                		ELSE 'Desvinculado'
                	END as vinculacao,
                	sn.sndescricao,
                	ten.tendsc,
                    vn.vnstatus,
                    vn.snaceito,
                    vn.vnid,
                    vn.snid,
                    vn.vndatavinculacao,
                    vn.vndataassinaturadesvinculacao,
                    dan.danemailprincipal,
                    dan.danemailalternativo,
                    dan.dantelefonefixo,
                    dan.dantelefonecelular,
                    en.endcep,
                    en.endlogradouro,
                    en.endcomplemento,
                    en.endnumero,
                    en.endbairro,
                    CASE
                        WHEN vnatuacaoexclusivaei IS NULL THEN
        					'<center>-</center>'
        				WHEN vnatuacaoexclusivaei = 'f' THEN
        					'<center>Não</center>'
        				WHEN vnatuacaoexclusivaei = 't' THEN
        					'<center>Sim</center>'
    				END as infantil,
                    dan.danid
                FROM
                	par3.vinculacaonutricionista vn
                JOIN par3.instrumentounidade_entidade    iue ON (vn.tenid = iue.tenid AND vn.vncpf = iue.entcpf AND vn.inuid = iue.inuid AND iue.entstatus = 'A')
                JOIN par3.situacaonutricionista  	      sn ON sn.snid = vn.snid
                JOIN par3.tipoentidade  		         ten ON ten.tenid = vn.tenid
                JOIN par3.instrumentounidade 	         inu ON inu.inuid = vn.inuid AND inu.inustatus = 'A'
                LEFT JOIN territorios.municipio 	     mun ON mun.muncod = inu.muncod
                LEFT JOIN par3.dadosnutricionista  	     dan ON dan.dancpf = vn.vncpf
                left join par3.endereco					 en  on en.endid = dan.endid
                JOIN seguranca.usuario                   usu ON usu.usucpf = iue.entcpf
                ".($where[0] != '' ? "WHERE ".implode(' AND ', $where) : '').
                " ORDER BY vn.vndatavinculacao";

        return $sql;
    }//end montarSQLListaVinculos();


    public function recuperarDadosListaVinculos($arrPost = array())
    {
        $sql = $this->montarSQLListaVinculos($arrPost);
        return $this->carregar($sql);
    }//end recuperarDadosListaVinculos();


    public function montarSQLXlsUnidadesVinculo($arrPost = array())
    {
    	switch($arrPost['tipo']){
    		case 'mcc':
    		    $sql = "SELECT
                                DISTINCT mun.mundescricao, inu.estuf
                            FROM par3.instrumentounidade inu
    		            INNER JOIN territorios.municipio        mun ON mun.muncod = inu.muncod
                            INNER JOIN par3.vinculacaonutricionista vn  ON vn.inuid   = inu.inuid AND vn.vnstatus = 'A'
                            INNER JOIN par3.dadosnutricionista      dan ON dan.dancpf = vn.vncpf
                            WHERE inu.itrid = 2";
    		    break;
    		case 'msc':
    		    $sql = "SELECT
                                DISTINCT mun.mundescricao, inu.estuf
                            FROM par3.instrumentounidade inu
                            INNER JOIN territorios.municipio mun ON mun.muncod = inu.muncod
                            WHERE inu.itrid = 2 and inu.inuid NOT IN (
                                SELECT
                                    inu.inuid
                                FROM par3.instrumentounidade inu
                                INNER JOIN par3.vinculacaonutricionista vn ON vn.inuid = inu.inuid AND vn.vnstatus = 'A'
                                INNER JOIN par3.dadosnutricionista dan ON dan.dancpf = vn.vncpf
                                WHERE inu.itrid = 2
                            )";
    		    break;
    		case 'ecc':
    		    $sql = "SELECT
                                DISTINCT 'SEDUC-'||inu.estuf as seduc, est.estdescricao
                            FROM par3.instrumentounidade inu
                            INNER JOIN territorios.estado est ON est.estuf  = inu.estuf
                            INNER JOIN par3.vinculacaonutricionista vn ON vn.inuid = inu.inuid AND vn.vnstatus = 'A'
                            INNER JOIN par3.dadosnutricionista dan ON dan.dancpf = vn.vncpf
                            WHERE inu.itrid <> 2";
    		    break;
    		case 'esc':
    		    $sql = "SELECT
                                DISTINCT 'SEDUC-'||inu.estuf as seduc, est.estdescricao
                            FROM par3.instrumentounidade inu
                            INNER JOIN territorios.estado est ON est.estuf  = inu.estuf
                            WHERE inu.itrid <> 2 AND inu.inuid NOT IN (
                                SELECT
                                    inu.inuid
                                FROM par3.instrumentounidade inu
                                INNER JOIN par3.vinculacaonutricionista vn ON vn.inuid = inu.inuid AND vn.vnstatus = 'A'
                                INNER JOIN par3.dadosnutricionista dan ON dan.dancpf = vn.vncpf
                                WHERE inu.itrid <> 2
                            )";
    		    break;
    	}

        return $sql;
    }//end montarSQLXlsUnidadesVinculo();


    public function recuperarDadosXlsUnidadesVinculo($arrPost = array())
    {
        $sql = $this->montarSQLXlsUnidadesVinculo($arrPost);
        return $this->carregar($sql);
    }//end recuperarDadosXlsUnidadesVinculo();


    public function recuperarDadosQuadroResumo()
    {
        $sql = "SELECT
                    (
                        SELECT
                            count(DISTINCT inu.inuid)
                        FROM par3.instrumentounidade inu
                        INNER JOIN par3.vinculacaonutricionista vn ON vn.inuid = inu.inuid AND vn.vnstatus = 'A'
                        INNER JOIN par3.dadosnutricionista dan ON dan.dancpf = vn.vncpf
                        WHERE inu.itrid = 2
                    ) as qtd_mun_com_cadastro,
                    (
                        SELECT
                            count(DISTINCT inu.inuid)
                        FROM par3.instrumentounidade inu
                        WHERE inu.itrid = 2 and inu.inuid NOT IN (SELECT
                            inu.inuid
                        FROM par3.instrumentounidade inu
                        INNER JOIN par3.vinculacaonutricionista vn ON vn.inuid = inu.inuid AND vn.vnstatus = 'A'
                        INNER JOIN par3.dadosnutricionista dan ON dan.dancpf = vn.vncpf
                        WHERE inu.itrid = 2)
                    ) as qtd_mun_sem_cadastro,
                    (
                        SELECT
                            count(DISTINCT inu.inuid)
                	FROM par3.instrumentounidade inu
                	INNER JOIN par3.vinculacaonutricionista vn ON vn.inuid = inu.inuid AND vn.vnstatus = 'A'
                        INNER JOIN par3.dadosnutricionista dan ON dan.dancpf = vn.vncpf
                	WHERE inu.itrid <> 2
                    ) as qtd_est_com_cadastro,
                    (
                        SELECT
                            count(DISTINCT inu.inuid)
                        FROM par3.instrumentounidade inu
                        WHERE inu.itrid <> 2 AND inu.inuid NOT IN (SELECT
                            inu.inuid
                        FROM par3.instrumentounidade inu
                        INNER JOIN par3.vinculacaonutricionista vn ON vn.inuid = inu.inuid AND vn.vnstatus = 'A'
                        INNER JOIN par3.dadosnutricionista dan ON dan.dancpf = vn.vncpf
                        WHERE inu.itrid <> 2)
                    ) as qtd_est_sem_cadastro";

        return $this->pegaLinha($sql);
    }//end recuperarDadosQuadroResumo();


    public function getNutriPorVinculacao($vnid)
    {

        $sql = " SELECT
                vn.inuid, entnome, ie.tenid  from par3.vinculacaonutricionista  vn
                INNER JOIN par3.instrumentounidade_entidade ie ON ie.entcpf = vn.vncpf
                WHERE
                vnid = {$vnid}";
        return $this->pegaLinha($sql);

    }

    public function carregarVinculacaoPorInstrumentoUnidade($inuid)
    {

        $sql = " SELECT vn.*
                   FROM par3.vinculacaonutricionista vn
             INNER JOIN par3.instrumentounidade_entidade ie ON ie.entcpf = vn.vncpf
                  WHERE inuid = {$inuid}";

        return $this->pegaLinha($sql);
    }

    public function carregarVinculacaoNutricionistaPorUnidade($usucpf, $inuid)
    {

        $sql = "SELECT vnid
                FROM par3.vinculacaonutricionista
                WHERE
                    vncpf = '$usucpf'
                    AND inuid = $inuid
                    AND vnstatus = 'A'";

        return $this->pegaUm($sql);
    }

    public function carregarVinculacaoNutricionistaPorVnidInuid($usucpf, $inuid,$arCampos = null,$buscarInativos = false)
    {
        $arCampos = array_filter($arCampos);
        $campos = '';
        if(count($arCampos) > 0){
            $campos = ','.implode(',',$arCampos);
        }
        $vnstatus = $buscarInativos?'':"AND vnstatus = 'A'";

        $sql = "SELECT vnid{$campos}
                FROM par3.vinculacaonutricionista
                WHERE
                    vncpf = '$usucpf'
                    AND inuid = $inuid
                    {$vnstatus}
        ";
        return $this->recuperar($sql);
    }

    public function invativarVinculoNutricionista($entid)
    {

        $entidade = new Par3_Model_InstrumentoUnidadeEntidade($entid);

        $sql = "UPDATE par3.vinculacaonutricionista
                SET
                    vnstatus = 'I',
                    usucpfalteracao = '{$_SESSION['usucpf']}'
                WHERE
                    vncpf = '{$entidade->entcpf}'
                    AND inuid = {$entidade->inuid}
                    AND tenid = {$entidade->tenid}";

        $this->executar($sql);
        $this->commit();
    }

    public function desvincularNutricionista($arrPost)
    {
        $entidade = new Par3_Model_InstrumentoUnidadeEntidade($arrPost['entid']);
        $sql = "UPDATE par3.vinculacaonutricionista SET vnstatus = 'I',usucpfalteracao = '%s',vnmotivodesvinculacao = '%s',vndatadesvinculacao = '%s' WHERE vncpf = '%s' AND inuid = %d AND tenid = %d";
        $sql = sprintf($sql,$_SESSION['usucpf'],$arrPost['vnmotivodesvinculacao'],$arrPost['vndatadesvinculacao'],$entidade->entcpf,$entidade->inuid,$entidade->tenid);
        $this->executar($sql);
    }

    public function nomearRT($arrPost)
    {

        $entidade = new Par3_Model_InstrumentoUnidadeEntidade($arrPost['entid']);

        $entidade->tenid = Par3_Model_InstrumentoUnidadeEntidade::NUTRICIONISTA_RESPONSAVEL;

        $vinid = $this->carregarVinculacaoNutricionistaPorUnidade($entidade->entcpf, $entidade->inuid);

        $this->carregarPorId($vinid);
        $this->tenid = Par3_Model_InstrumentoUnidadeEntidade::NUTRICIONISTA_RESPONSAVEL;

        $entidade->salvar();
        $this->salvar();
        $this->commit();
    }

    public function recuperaDadosVinculacao()
    {
        $arrCondicao = array("
                        vncpf = '{$_SESSION['usucpf']}'
                        AND (
                            ( snid = 3 AND vnstatus = 'A')
                        OR
                            (
                            /* Caso não esteja na situação 'pendente de validacao'
                            nem 'Não validao pelo nutricionista' deve verificar a
                            existencia dos campos*/
                            CASE
                                WHEN snid in (1,6,4,5) THEN
                                    --(vncargahorariasemanal IS NULL OR vndatavinculacao IS NULL OR dntvid IS NULL OR vnatuacaoexclusivaei IS NULL)
                                    NOT(vncargahorariasemanal IS NULL OR vndatavinculacao IS NULL OR dntvid IS NULL )
                                ELSE
                                    FALSE
                            END
                            )
                        )");

        return $this->recuperarTodos('*', $arrCondicao);
    }

    public function finalizarCadastro()
    {
        $sql = "UPDATE $this->stNomeTabela SET
                    snid = 1
                WHERE
                    vncpf = '{$_SESSION['usucpf']}'
                    AND vnstatus = 'A'";

        $this->executar($sql);
        $this->commit();
    }

    public function gerarTermoDesvinculacao($arrDados)
    {
        $usuario = new Seguranca_Model_Usuario();
        $arrDadosUsuario = $usuario->recuperarPorCPF($this->vncpf);

        $dadosNutricionista = new Par3_Model_DadosNutricionista();
        $dadosNutricionista->carregarDadosPorCPF($this->vncpf);

        $arrDados['nome']                = $arrDadosUsuario['usunome'];
        $arrDados['cpf']                 = $this->vncpf;
        $arrDados['crn']                 = $dadosNutricionista->dancrn;
        $arrDados['crnuf']               = $dadosNutricionista->dancrnuf;
        $arrDados['datadesvinculacao']   = $arrDados['vndatadesvinculacao'];
        $arrDados['motivodesvinculacao'] = $arrDados['vnmotivodesvinculacao'];
        $arrDados['unidade']             = $arrDados['unidade'];

        return $this->retornaTextoTermoDesvinculacao($arrDados);
    }//end gerarTermoDesvinculacao()

    public function retornaTextoTermoDesvinculacao($arrDados)
    {
        $termo = "<p style='font-size: 15' ><center><b>DECLARAÇÃO DE DESVINCULAÇÃO</b></center></p>
                <p style='text-align: justify;' >Eu, <b>{$arrDados['nome']}</b>,
                CPF <b>".formatar_cpf($arrDados['cpf'])."</b>, CRN Nº <b>{$arrDados['crn']}</b>,
                Região <b>{$arrDados['crnuf']}</b> declaro que, a partir
                da data de <b>{$arrDados['datadesvinculacao']}</b>, por motivo(s)
                <b>{$arrDados['motivodesvinculacao']}</b>, me desligo das incumbências
                a mim atribuídas como nutricionista do Programa Nacional de
                Alimentação Escolar (PNAE) em <b>".($arrDados['unidade'])."</b>.</p>

                Em, <b>".date('d')." de ".ucfirst(strtolower(mes_extenso(date('m'))))." de ".date('Y')."</b>";

        return $termo;
    }//end retornaTextoTermoDesvinculacao()

    public function recuperarTermoDesviculacao()
    {
        $usuario = new Seguranca_Model_Usuario();
        $arrDadosUsuario = $usuario->recuperarPorCPF($this->vncpf);
        $dadosNutricionista = new Par3_Model_DadosNutricionista();
        $dadosNutricionista->carregarDadosPorCPF($this->vncpf);

        $arrDados = array();

        $arrDados['nome']                = $arrDadosUsuario['usunome'];
        $arrDados['cpf']                 = $this->vncpf;
        $arrDados['crn']                 = $dadosNutricionista->dancrn;
        $arrDados['crnuf']               = $dadosNutricionista->dancrnuf;
        $arrDados['datadesvinculacao']   = formata_data($this->vndatadesvinculacao);
        $arrDados['motivodesvinculacao'] = $this->vnmotivodesvinculacao;
        $controleUnidade                 = new Par3_Controller_InstrumentoUnidade();
        $arrDados['unidade']             = ($controleUnidade->pegarNomeEntidade($this->inuid));

        return $this->retornaTextoTermoDesvinculacao($arrDados);
    }//end gerarTermoDesvinculacao()

    public function enviarEmailDesvinculacao($vnid)
    {
        if($this->vncpf == ''){
            $this->carregarPorId($vnid);
        }

        $unidade = new Par3_Model_InstrumentoUnidade($this->inuid);
        $email   = $unidade->recuperarEmailEsponsaveis($this->inuid);

        $usuario = new Seguranca_Model_Usuario();
        $arrDadosUsuario = $usuario->recuperarPorCPF($this->vncpf);

        $esfera = $unidade->itrid == '2' ? 'municipio' : 'estado';

        $strAssunto = "Aviso de desvinculação de nutricionista";

        $remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");

        $strMensagem = "
        <pre align=\"center\" style=\"text-align: justify;\"  >
        Informamos que o(a) nutricionista  {$arrDadosUsuario['usunome']} assinou o Termo de Desvinculação do Programa Nacional de Alimentação Escolar nesta data.
        Observe se o número de nutricionistas vinculados ao seu {$esfera} está de acordo com os parâmetros numéricos definidos na Resolução CFN nº 465/2010.

        http://simec.mec.gov.br

        Atenciosamente,
        Equipe SIMEC/PAR.
        </pre>";

        if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" ){
            echo $strAssunto."<br>";
            echo implode('; ', $email)."<br>";
            echo $strMensagem."<br>";
        }elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br"){
            $strEmailTo = array('andreneto@mec.gov.br', 'Murilo.Martins@mec.gov.br', 'EduardoNeto@mec.gov.br');
            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
        }else{
            $strEmailTo = $email;
            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
        }
    }//end enviarEmailDesvinculacao()

    public function countTipoNutricionistas()
    {
        $sql = "
            SELECT ten.tenid as id,ten.tendsc as descricao, count(*) as valor
            FROM par3.vinculacaonutricionista vn
                JOIN par3.instrumentounidade_entidade    iue ON (vn.tenid = iue.tenid AND vn.vncpf = iue.entcpf AND vn.inuid = iue.inuid AND iue.entstatus = 'A')
                JOIN par3.situacaonutricionista  	      sn ON sn.snid = vn.snid
                JOIN par3.tipoentidade  		         ten ON ten.tenid = vn.tenid
                JOIN par3.instrumentounidade 	         inu ON inu.inuid = vn.inuid AND inu.inustatus = 'A'
                LEFT JOIN territorios.municipio 	     mun ON mun.muncod = inu.muncod
                LEFT JOIN par3.dadosnutricionista  	     dan ON dan.dancpf = vn.vncpf
                JOIN seguranca.usuario                   usu ON usu.usucpf = iue.entcpf
            WHERE vn.tenid in (7,8,16)
            and iue.entstatus = 'A'
            and vn.vnstatus = 'A'
            and vn.vndatadesvinculacao IS NULL
            group by ten.tendsc, ten.tenid
                        ";
        return $sql;
    }

    public function countSituacaoNutricionistas()
    {
        $arTenId = [
            Par3_Model_InstrumentoUnidadeEntidade::NUTRICIONISTA_QUADRO,
            Par3_Model_InstrumentoUnidadeEntidade::NUTRICIONISTA_RESPONSAVEL,
            Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_CONSELHEIRO_INFANTIL,
        ];
        $stTenId = implode(',',$arTenId);
        $sql = "
        select
            sn.sndescricao as descricao,
            count(*) as valor
        from
        par3.vinculacaonutricionista vn
        JOIN par3.instrumentounidade_entidade    iue ON (vn.tenid = iue.tenid AND vn.vncpf = iue.entcpf AND vn.inuid = iue.inuid AND iue.entstatus = 'A')
        JOIN par3.situacaonutricionista  	      sn ON sn.snid = vn.snid
        JOIN par3.tipoentidade  		         ten ON ten.tenid = vn.tenid
        JOIN par3.instrumentounidade 	         inu ON inu.inuid = vn.inuid AND inu.inustatus = 'A'
        LEFT JOIN territorios.municipio 	     mun ON mun.muncod = inu.muncod
        LEFT JOIN par3.dadosnutricionista  	     dan ON dan.dancpf = vn.vncpf
        JOIN seguranca.usuario                   usu ON usu.usucpf = iue.entcpf
        where vn.vnstatus   = 'A'
        and   iue.tenid IN ({$stTenId})
        and   vn.vndatadesvinculacao IS null
        group by sn.sndescricao
        ";
        return $sql;
    }

    public function countSituacaoEntidades()
    {
        $sql = "
        select
                (
            case when situacao.descricao is null then 
                'Sem cadastro' 
            else situacao.descricao 
            end
        )as descricao,
        count(inu.inuid) as valor
        FROM par3.instrumentounidade inu
            left join
            (
                SELECT
                (case
                    when sn.snid in(1,4,6) then 'Cadastro Validado'
                    when sn.snid in(2,3,5) then 'Com pendência no cadastro'
                    else sn.snid::varchar
                end) as descricao,
                count(*) as valor,
                ine.inuid
                FROM par3.instrumentounidade             ine
                INNER JOIN par3.instrumentounidade_entidade int ON int.inuid  = ine.inuid
                INNER JOIN par3.tipoentidade                ten ON ten.tenid  = int.tenid
                INNER JOIN seguranca.usuario                usu ON int.entcpf = usu.usucpf
                INNER JOIN par3.vinculacaonutricionista     vn  ON vn.tenid   = int.tenid AND vn.vncpf = int.entcpf AND vn.inuid = int.inuid AND vn.vnstatus = 'A'
                INNER JOIN par3.situacaonutricionista       sn  ON sn.snid    = vn.snid
                WHERE
                int.tenid IN (8,7,16)
            AND int.entstatus = 'A'
                group by ine.inuid,descricao
        ) as situacao on situacao.inuid = inu.inuid
        group by situacao.descricao
        ";
        return $sql;
    }

    public function carregarRelatorioEntidadesPorSituacao($arrsnid = array())
    {
        $where     = '';
        if($arrsnid){
            $arrsnTratado = array_map(function($val){ return "'".$val."'";},$arrsnid);
            $arrsnTratado = array_filter($arrsnTratado);
            if(count($arrsnTratado) == 0){
                return array();
            }
            $where = "where situacao.descricao in (".implode(',',$arrsnTratado).")";
        }
        $sql = "
            select * from
            (
                    select
                    (case when inu.itrid <> 2 then 'Estadual' else 'Municipal' end) as esfera,
                    inu.itrid,
                    inu.inuid,
                    inu.estuf,
                    inu.muncod,
                    inu.inudescricao,
                    (
                     case when situacao.descricao is null then 
                            'Sem cadastro' 
                        else situacao.descricao 
                        end
                    )as descricao,
                    situacao.count_validados,
                    situacao.count_nao_validado,
                    situacao.count_pendentes,
                    situacao.count_aprovados,
                    situacao.count_reprovados,
                    situacao.count_vinculos_validados
                    FROM par3.instrumentounidade inu
                    left join(
                        SELECT
                        (case
                            when sn.snid in(1,4,6) then 'Cadastro Validado'
                            when sn.snid in(2,3,5) then 'Com pendência no cadastro'
                            else sn.snid::varchar
                        end) as descricao,
                        count(case when sn.snid = 1 then 1 end) as count_validados,
                        count(case when sn.snid = 2 then 1 end) as count_nao_validado,
                        count(case when sn.snid = 3 then 1 end) as count_pendentes,
                        count(case when sn.snid = 4 then 1 end) as count_aprovados,
                        count(case when sn.snid = 5 then 1 end) as count_reprovados,
                        count(case when sn.snid = 5 then 1 end) as count_vinculos_validados,
                        ine.inuid             
                        FROM par3.instrumentounidade             ine
                        INNER JOIN par3.instrumentounidade_entidade int ON int.inuid  = ine.inuid
                        INNER JOIN par3.tipoentidade                ten ON ten.tenid  = int.tenid
                        INNER JOIN seguranca.usuario                usu ON int.entcpf = usu.usucpf
                        INNER JOIN par3.vinculacaonutricionista     vn  ON vn.tenid   = int.tenid AND vn.vncpf = int.entcpf AND vn.inuid = int.inuid AND vn.vnstatus = 'A'
                        INNER JOIN par3.situacaonutricionista       sn  ON sn.snid    = vn.snid
                        WHERE
                        int.tenid IN (8,7,16)
                    AND int.entstatus = 'A'
                        group by ine.inuid,descricao
                ) as situacao on situacao.inuid = inu.inuid
            )as situacao
            {$where}
            group by 
            itrid,
            estuf,
            inuid,
            esfera,
            muncod,
            descricao,
            inudescricao,
            count_validados,
            count_nao_validado,
            count_pendentes,
            count_aprovados,
            count_reprovados,
            count_vinculos_validados
            order by estuf,itrid,inudescricao
        ";
        return $this->carregar($sql);
    }
}//end Class
?>