<?php

/**
 * Classe de mapeamento da entidade par.solicitacaoprorrogacaoprazoobra
 *
 * @category Class
 * @package  A1
 * @author   JAIR FERREIRA FORO SANTOS <jair.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */

/**
 * SolicitacaoProrrogacaoPrazoObra
 *
 * @category Class
 * @package  A1
 * @author   JAIR FERREIRA FORO SANTOS <jair.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 05-01-2016
 * @link     no link
 */
class SolicitacaoProrrogacaoPrazoObra extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par.solicitacaoprorrogacaoprazoobra';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'sppid'
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'terid',
        'docid',
        'dopid',
        'proid'
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sppid' => null,
        'sppusucpfinclusao' => null,
        'sppdtfimsolicitado' => null,
        'sppdtvigenciaanterior' => null,
        'sppdtinclusao' => null,
        'sppdtfimaprovada' => null,
        'sppusucpfvalidador' => null,
        'sppdtvalidacao' => null,
        'docid' => null,
        'dopid' => null,
        'terid' => null,
        'sppjustificativa' => null,
        'sppacaotomada' => null,
        'sppoutrainformacao' => null,
        'sppvlrsuficientefnde' => null,
        'proid' => null
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'sppid' => array('Digits'),
            'sppusucpfinclusao' => array('allowEmpty' => false),
            'sppdtfimsolicitado' => array('allowEmpty' => false),
            'sppdtfimaprovada' => array('allowEmpty' => true),
            'sppusucpfvalidador' => array('allowEmpty' => true),
            'sppdtvalidacao' => array('allowEmpty' => true),
            'docid' => array('allowEmpty' => false, 'Digits'),
            'dopid' => array('allowEmpty' => true, 'Digits'),
            'terid' => array('allowEmpty' => true, 'Digits'),
            'proid' => array('allowEmpty' => true, 'Digits'),
            'sppjustificativa' => array('allowEmpty' => false),
            'sppacaotomada' => array('allowEmpty' => false),
            'sppoutrainformacao' => array('allowEmpty' => true),
            'sppvlrsuficientefnde' => array('allowEmpty' => false),
            'sppanexodeclaracao' => array('allowEmpty' => true),
            'sppanexocronograma' => array('allowEmpty' => false),
        );
    }

    /**
     * Retorna a Lista de Solicitações por processo Historico.
     * @param type $arrDados
     * @return type
     */
    public function getHistoricoSolicitacao($arrDados)
    {

        $tipoDocumento = ($arrDados['tipo'] == 'OBRAPAR') ? 'dopid' : 'terid';
        $proid = (int)$arrDados['proid'];

        $sql = "SELECT
                    TO_CHAR(sppdtfimsolicitado, 'DD/MM/YYYY') AS sppdtfimsolicitado,
                    COALESCE(TO_CHAR(sppdtfimaprovada, 'DD/MM/YYYY'), '<center>-</center>') AS sppdtfimaprovada ,
                    COALESCE(TO_CHAR(sppdtvalidacao, 'DD/MM/YYYY'), '<center>-</center>') AS sppdtvalidacao  ,
                    esd.esddsc,
                    COALESCE(TO_CHAR(sppdtvigenciaanterior, 'DD/MM/YYYY'), '<center>-</center>') AS sppdtvigenciaanterior,
                    doc.docid
                FROM $this->stNomeTabela spp
                INNER JOIN workflow.documento doc ON doc.docid = spp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE spp.$tipoDocumento IS NOT NULL AND proid = {$proid}
                ORDER BY sppid DESC";

        return ($this->carregar($sql)) ? $this->carregar($sql) : array();
    }

    public function inserirSolicitacao($arrDados)
    {
        try {
            if ($this->validarCampos($arrDados)) {
                // Trata o f5, evitando várias solicitações de uma vez só
                $resultadoExiste = $this->verificarSolicitacaoReformulacao($arrDados);
                if ($resultadoExiste) {
                    return "ja_inserido";
                }

                #Vigencia Anterior.
                if (!empty($arrDados['sppdtvigenciaanterior'])) {
                    if (strlen($arrDados['sppdtvigenciaanterior']) == 10) {
                        $arrDados['sppdtvigenciaanterior'] = formata_data_sql($arrDados['sppdtvigenciaanterior']);
                    }
                } else {
                    $arrDados['sppdtvigenciaanterior'] = null;
                }

                $arrDados['dopid'] = ($arrDados['tipo'] == 'OBRAPAR') ? $arrDados['id'] : null;
                $arrDados['terid'] = ($arrDados['tipo'] == 'OBRAPAC') ? $arrDados['id'] : null;
                $arrDados['sppvlrsuficientefnde'] = $arrDados['sppvlrsuficientefnde'] == 'S' ? 'true' : 'false';
                $arrDados['sppusucpfinclusao'] = $_SESSION['usucpf'];
                $arrDados['sppdtinclusao'] = 'NOW()';
                if ($arrDados['sppjustificativa'] != '') {
                    $arrDados['sppjustificativa'] = substr($arrDados['sppjustificativa'], 0, 1000);
                }
                if ($arrDados['sppacaotomada'] != '') {
                    $arrDados['sppacaotomada'] = substr($arrDados['sppacaotomada'], 0, 1000);
                }

                include_once APPRAIZ . "includes/workflow.php";
                if (empty($arrDados['docid'])) {
                    $arrDados['docid'] = wf_cadastrarDocumento(
                        TPDID_SOLICITACAO_PRORROGACAO_OBRA_TERMO,
                        'Solicitacão de Prorrogação de Prazo Obras',
                        ESDID_AGUARDANDO_PRORROGACAO_PRAZO
                    ); // Criar docid
                }

                #Inserir Solicitacao Reprogramacao Prazo Obras
                $this->popularDadosObjeto($arrDados);
                $sppid = $this->salvar();

                #Upload
                include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

                #Insere Anexo Declaracao
                if ($arrDados['sppvlrsuficientefnde'] == 'false') {
                    #Insere Declaracao
                    $campos = array(
                        "sppid" => $sppid,
                        'asptipo' => "'D'",
                        'aspstatus' => "'A'",
                    );

                    $file = new FilesSimec("anexosolicitacaoprorrogacaoprazoobra", $campos, "par");
                    $file->setUpload('Anexo Declaração', 'sppanexodeclaracao', true, 'aspid');
                    $file->getCampoRetorno();
                }

                #Insere Anexo Cronograma
                $campos = array(
                    "sppid" => $sppid,
                    'asptipo' => "'C'",
                    'aspstatus' => "'A'",
                );

                $file = new FilesSimec("anexosolicitacaoprorrogacaoprazoobra", $campos, "par");
                $file->setUpload('Anexo Cronograma', 'sppanexocronograma', true, 'aspid');
                $file->getCampoRetorno();

                $this->commit();
                return $sppid;
            }
        } catch (Exception $exc) {
            echo $exc->getTraceAsString();
            $this->rollback();
        }
    }

    /*
     *
     *
     * */
    public function retornaArrayEmail($dadosUnidade = array())
    {

        $esfera = $dadosUnidade['tipo_entidade']; // Se estadual ou municipal
        $muncod = $dadosUnidade['muncod'];
        $estuf = $dadosUnidade['estuf'];

        if (($esfera == 'E') && ($estuf != '')) {
            $sqlEmail = "
    		SELECT
    			distinct ent.entemail as email
    		FROM
    			par.entidade ent
    		INNER JOIN par.entidade ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
    		INNER JOIN territorios.estado est on est.estuf = ent2.estuf
    	
    		WHERE
    			ent.entstatus='A'
    		AND
    			ent.dutid in( 9, 10 )
    		AND
    			ent2.estuf in ( '$estuf' )
    		AND
    			ent.entemail <> ''
    		AND
    			ent.entemail is not null
    		";
        } else {
            if ($esfera == 'M' && ($muncod != '')) {
                $sqlEmail = "SELECT
    			distinct ent.entemail as email
			FROM
    			par.entidade ent
			INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
    		INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
    		WHERE
    			ent.dutid in( 2, 7, 6, 8 )
    			and ent.entstatus = 'A'
			AND
    			mun.muncod in ( '{$muncod}' )
			AND
				ent.entemail <> ''
			AND
				ent.entemail is not null
    			";
            }
        }

        $resultEmail = $this->carregar($sqlEmail);

        $resultEmail = ($resultEmail) ? $resultEmail : array();
        $arrEmail = array();

        if (count($resultEmail) > 0) {
            foreach ($resultEmail as $k => $v) {
                $arrEmail[] = $v['email'];
            }
        }
        return $arrEmail;
    }

    /*
     * $acao = 'A' = Aceitar; 'R' = Recusar
     * */
    public function retornaNumeroTermo($arrDados)
    {
        $tipo = $arrDados['tipo'];
        $id = $arrDados['id'];
        $numTermo = $arrDados['num_termo'];
        if ($tipo == "OBRAPAR" && $id) {
            $sql = "SELECT dopnumerodocumento FROM par.documentopar  where dopid = {$id}";
            return $this->pegaUm($sql);
        } else {
            if ($tipo == "OBRAPAC" && $id) {
                $sql = "SELECT ternumero FROM par.termocompromissopac  where terid = {$id}";
                return $this->pegaUm($sql);
            } else {
                return '';
            }
        }
    }

    /*
     * $acao = 'A' = Aceitar; 'R' = Recusar
     * */
    public function enviaEmailTramitacao($arrDados, $acao = 'A', $interposicao = false)
    {

        // Carrega variáveis
        $tipo = $arrDados['tipo']; /* 'OBRAPAR' ou 'OBRAPAC'*/
        $id = $arrDados['id'];
        $justificativa = $arrDados['txConsideracao'];
        $numTermo = $arrDados['num_termo'];

        if ($tipo == "OBRAPAR") {
            $sqlEntidade =
                "SELECT
				CASE WHEN itrid = 1 THEN 'E' ELSE 'M' END as tipo_entidade, inu.muncod, inu.estuf
			FROM 
    			par.documentopar dop
			INNER JOIN  par.processoobraspar p ON p.proid = dop.proid
			INNER JOIN  par.instrumentounidade inu ON inu.inuid = p.inuid
			WHERE dopid 
    				= {$id}";
            $dadosUnidade = $this->pegaLinha($sqlEntidade);
        } else {
            if ($tipo == "OBRAPAC") {
                $sqlEntidade = "
    		SELECT
    			CASE WHEN pro.muncod is NULL THEN 'E' ELSE 'M' END as tipo_entidade, pro.muncod, pro.estuf 
    		FROM 
    			par.termocompromissopac tp
			INNER JOIN  par.processoobra  pro ON pro.proid = tp.proid
			where 
    			terid = {$id}";
                $dadosUnidade = $this->pegaLinha($sqlEntidade);
            }
        }
        //
        if ($interposicao) {
            if ($acao == 'A') { // Deferido
                $strAssunto = "Deferimento Recurso Prorrogação de Prazo Termo de Compromisso - {$numTermo}";
                $strMensagem = "
<p>Senhor Gestor,</p>
<p style=\"text-align: justify;\">O recurso impetrado em " . date('d/m/Y') . ", referente ao Termo de Compromisso {$numTermo}, foi deferido.</p>
<p style=\"text-align: justify;\">Solicita-se aguardar os trâmites de finalização da prorrogação.</p>
<p style=\"text-align: justify;\">Para dirimir eventuais dúvidas que porventura se apresentem, favor entrar em contato pelo e-mail coven@fnde.gov.br.</p>
<p style=\"text-align: justify;\">Atenciosamente,</p>

<pre>
<b>Coordenação de Convênios - COVEN</b>
Fundo Nacional de Desenvolvimento da Educação - FNDE
SBS Qd 02 bl. F Ed. FNDE. CEP 70.070-929 - Brasília-DF
e-mail: coven@fnde.gov.br
Telefone Institucional: 0800 61 61 61
www.fnde.gov.br
</pre>
";
            } else // Indeferido
            {
                $strAssunto = "Indeferimento Recurso Prorrogação de Prazo Termo de Compromisso - {$numTermo}";
                $strMensagem = "
<p>Senhor Gestor,</p>
<p style=\"text-align: justify;\">O recurso impetrado em " . date('d/m/Y') . ", referente ao Termo de Compromisso {$numTermo}, foi indeferido, de acordo com a seguinte justificativa:</p>
<p style=\"text-align: justify;\">{$justificativa} </p>
<p style=\"text-align: justify;\">Para dirimir eventuais dúvidas que porventura se apresentem, favor entrar em contato pelo e-mail coven@fnde.gov.br.</p>
<p style=\"text-align: justify;\">Atenciosamente,</p>
<pre>
<b>Coordenação de Convênios - COVEN</b>
Fundo Nacional de Desenvolvimento da Educação - FNDE
SBS Qd 02 bl. F Ed. FNDE. CEP 70.070-929 - Brasília-DF
e-mail: coven@fnde.gov.br
Telefone Institucional: 0800 61 61 61 
www.fnde.gov.br
</pre>
";
            }
        } else {
            if ($acao == 'R') { // Indeferido
                $strAssunto = "Indeferimento Prorrogação de Prazo Termo de Compromisso - {$numTermo}";
                $strMensagem = "
<p>Senhor Gestor, </p>
<p style=\'text-align: justify\'>
 	Informa-se que a solicitação de prorrogação do Termo de Compromisso-TC nº {$numTermo}, foi indeferido, de acordo com a seguinte justificativa:</p>
<p style=\"text-align: justify;\">{$justificativa}</p>
<p style=\"text-align: justify;\">Desta forma, faz-se necessário providenciar a devida prestação de contas. Caso julgue necessário, o município tem prazo <b>até o termino da vigência do TC</b> para interposição de recurso, dentro do SIMEC.</p>
<p style=\"text-align: justify;\">Se as obras estiverem em execução e pendentes de atualizações no SIMEC, as solicitações de prorrogação deverão, necessariamente, atender os seguintes tópicos:</p>
<p style=\"text-align: justify;\">a.	Inserção de solicitação de ajuste no sistema, através da opção: SIMEC-OBRAS 2.0/Lista de Opções/ Solicitação para Correção de Inconsistência no sistema.</p>
<p style=\"text-align: justify;\">b.	Inserção, na aba documentos, de relatório fotográfico, contendo percentual já construído e fotos do estágio de execução.</p>
<p style=\"text-align: justify;\">Alerta-se que, conforme legislação, os recursos em conta podem ser utilizados para pagamento de despesas empenhadas, desde que haja autorização da área gestora. Assim, é necessário encaminhar Ofício à Coordenação Geral de Infraestrutura Educacional - CGEST, no seguinte endereço: SBS, Quadra 02, Bloco \"F\", Ed. FNDE, 14º andar - CEP: 70.070-929 - BRASÍLIA/DF. A correspondência deverá ser enviada aos cuidados do/a Sr. (Srª) Coordenador (a) Geral.
Para esclarecimentos quanto à prestação de contas, favor utilizar um dos canais abaixo:</p>
<ul>
<li><b>Fale Conosco:</b></li>
&nbsp;&nbsp;	o	contasonline.projetos@fnde.gov.br;<br>
&nbsp;&nbsp;	o	assessoria.cgcap@fnde.gov.br.
<li>	<b>Central de Atendimento ao Cidadão (ligação gratuita):</b></li> 
&nbsp;&nbsp;	o	0800-616161 de segunda a sexta-feira, das 8h às 20h.
<li>	<b>Atendimento Institucional (presencial):</b></li>
&nbsp;&nbsp;	o	Setor Bancário Sul, Quadra 2, Bloco F, Edifício FNDE  Térreo, Sala 1, Brasília/DF, de segunda a sexta-feira, das 8h às 12h e das 14h às 18h.
</ul>

<p style=\"text-align: justify;\">Permanecemos à disposição. </p>


<p>Atenciosamente,</p>
<pre>
<b>Coordenação de Convênios - COVEN</b>
Fundo Nacional de Desenvolvimento da Educação - FNDE
SBS Qd 02 bl. F Ed. FNDE. CEP 70.070-929 - Brasília-DF
e-mail: coven@fnde.gov.br
Telefone Institucional: 0800 61 61 61
www.fnde.gov.br
</pre>    			
";
            } else // Deferido
            {
                return true;
            }
        }

        $strAssunto = "=?ISO-8859-1?B?" . base64_encode($strAssunto) . "?=";

        $arrEmail = $this->retornaArrayEmail($dadosUnidade);

        $remetente = array("nome" => "SIMEC", "email" => "noreply@mec.gov.br");

        if ($_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost") {
            /*$arrEmail[] = "coven@fnde.gov.br";
            $arrEmail = implode($arrEmail, ', ');
            print_r(
                "
                    Para: {$arrEmail}<br>
                    Assunto: {$strAssunto}<br><Br>
                    Mensagem: {$strMensagem}

                "
            );
            die('foi');*/
        } else {
            if ($_SERVER['HTTP_HOST'] == "dsv-simec" || $_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br") {
                $strEmailTo = array('elias.oliveira@mec.gov.br', 'andressa.klosovski@fnde.gov.br');
                $emailsProducao = implode($arrEmail, ';');
                $strMensagem .= " <br>Emails serão enviados para: {$emailsProducao}";
                return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
            } else {
                $arrEmail[] = "coven@fnde.gov.br";
                $strEmailTo = $arrEmail;
                return enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
            }
        }
        return true;
    }

    public function validarSolicitacao($arrDados, $booRecusar = false)
    {
        try {
            $numTermo = $this->retornaNumeroTermo($arrDados);
            $arrDados['num_termo'] = $numTermo;

            if ($this->validarCampos($arrDados)) {
                #Vigencia Anterior.
                if (strlen($arrDados['sppdtvigenciaanterior']) == 7) {
                    $arrDados['sppdtvigenciaanterior'] = '01/' . $arrDados['sppdtvigenciaanterior'];
                }

                $arrDados['sppusucpfvalidador'] = $_SESSION['usucpf'];
                $arrDados['sppdtvalidacao'] = 'NOW()';

                if (!empty($arrDados['docid'])) {
                    include_once APPRAIZ . "includes/workflow.php";
                    if (!$booRecusar) {
                        if (empty($arrDados['sppdtfimaprovada'])) { #Aprovar
                            $arrDados['sppdtfimaprovada'] = formata_data_sql($arrDados['sppdtfimsolicitado']);

                            $resAlterarEstado = wf_alterarEstado(
                                $arrDados['docid'],
                                4051,
                                $arrDados['txConsideracao'],
                                array('cache_estado_atual' => 0),
                                array('commit' => false)
                            ); #Aprovado
                            if ($resAlterarEstado) {
                                if ($arrDados['tipo'] == 'OBRAPAC') {
                                    # Tramita direto para obra finalizada e atualiza o documento.
                                    (new TermoCompromissoPac())->gerarTermoEx(
                                        $arrDados['id'],
                                        $arrDados['sppdtfimsolicitado'],
                                        'ST'
                                    );
                                    wf_alterarEstado(
                                        $arrDados['docid'],
                                        4205,
                                        $arrDados['txConsideracao'],
                                        array('cache_estado_atual' => 0),
                                        array('commit' => false)
                                    ); #Prorrogacao d ePrazo Finalizada
                                } else {
                                    wf_alterarEstado(
                                        $arrDados['docid'],
                                        4061,
                                        $arrDados['txConsideracao'],
                                        array('cache_estado_atual' => 0),
                                        array('commit' => false)
                                    ); #Aguardando geracao de termo
                                }
                            } else {
                                return false;
                            }
                        } else { #Aprovar com Ressalva
                            $resAlterarEstado = wf_alterarEstado(
                                $arrDados['docid'],
                                4053,
                                $arrDados['txConsideracao'],
                                array('cache_estado_atual' => 0),
                                array('commit' => false)
                            ); #aprovado com ressalva
                            if ($resAlterarEstado) {
                                if ($arrDados['tipo'] == 'OBRAPAC') {
                                    # Tramita direto para obra finalizada e atualiza o documento.
                                    (new TermoCompromissoPac())->gerarTermoEx(
                                        $arrDados['id'],
                                        $arrDados['sppdtfimaprovada'],
                                        'ST'
                                    );
                                    wf_alterarEstado(
                                        $arrDados['docid'],
                                        4206,
                                        $arrDados['txConsideracao'],
                                        array('cache_estado_atual' => 0),
                                        array('commit' => false)
                                    ); #Prorrogacao d ePrazo Finalizada
                                } else {
                                    wf_alterarEstado(
                                        $arrDados['docid'],
                                        4120,
                                        $arrDados['txConsideracao'],
                                        array('cache_estado_atual' => 0),
                                        array('commit' => false)
                                    ); #Aguardando geracao de termo
                                }
                                $arrDados['sppdtfimaprovada'] = formata_data_sql($arrDados['sppdtfimaprovada']);
                            } else {
                                return false;
                            }
                        }
                    } else {
                        wf_alterarEstado(
                            $arrDados['docid'],
                            4052,
                            $arrDados['txConsideracao'],
                            array('commit' => true)
                        ); #Reprovado.
                        $this->enviaEmailTramitacao($arrDados, 'R');
                    }
                } else {
                    $this->rollback();
                    return false;
                }

                #Inserir Solicitacao Reprogramacao Prazo Obras
                $this->popularDadosObjeto($arrDados);
                $sppid = $this->salvar();

                $this->commit();
                return $sppid;
            }
        } catch (Exception $exc) {
            echo $exc->getTraceAsString();
            $this->rollback();
        }
    }

    public function validarSolicitacaoRecurso($arrDados, $booRecusar = false)
    {

        try {
            $numTermo = $this->retornaNumeroTermo($arrDados);
            $arrDados['num_termo'] = $numTermo;
            if ($this->validarCampos($arrDados)) {
                #Vigencia Anterior.
                if (strlen($arrDados['sppdtvigenciaanterior']) == 7) {
                    $arrDados['sppdtvigenciaanterior'] = '01/' . $arrDados['sppdtvigenciaanterior'];
                }

                $arrDados['sppusucpfvalidador'] = $_SESSION['usucpf'];
                $arrDados['sppdtvalidacao'] = 'NOW()';

                if (!empty($arrDados['docid'])) {
                    include_once APPRAIZ . "includes/workflow.php";
                    if (!$booRecusar) {
                        if (empty($arrDados['sppdtfimaprovada'])) { #Aprovar
                            $arrDados['sppdtfimaprovada_ori'] = $arrDados['sppdtfimsolicitado'];
                            $arrDados['sppdtfimaprovada'] = formata_data_sql($arrDados['sppdtfimsolicitado']);
                        } else #Aprovar com Ressalva
                        {
                            $arrDados['sppdtfimaprovada_ori'] = $arrDados['sppdtfimaprovada'];
                            $arrDados['sppdtfimaprovada'] = formata_data_sql($arrDados['sppdtfimaprovada']);
                        }

                        wf_alterarEstado(
                            $arrDados['docid'],
                            (string)AEDID_DEFERIDO_PELA_DIGAP,
                            $arrDados['txConsideracao'],
                            array('cache_estado_atual' => 0),
                            array('commit' => false)
                        ); #Aprovado
                        if ($arrDados['tipo'] == 'OBRAPAC') {
                            (new TermoCompromissoPac())->gerarTermoEx(
                                $arrDados['id'],
                                $arrDados['sppdtfimaprovada_ori'],
                                'ST'
                            );
                            wf_alterarEstado(
                                $arrDados['docid'],
                                4205,
                                $arrDados['txConsideracao'],
                                array('cache_estado_atual' => 0),
                                array('commit' => false)
                            ); #Prorrogacao d ePrazo Finalizada
                        } else {
                            wf_alterarEstado(
                                $arrDados['docid'],
                                4061,
                                $arrDados['txConsideracao'],
                                array('cache_estado_atual' => 0),
                                array('commit' => false)
                            ); #Aguardando geracao de termo
                        }
                        $this->commit();
                        $this->enviaEmailTramitacao($arrDados, 'A', true);
                    } else {
                        wf_alterarEstado(
                            $arrDados['docid'],
                            (string)AEDID_INDEFERIDO_PELA_DIGAP,
                            $arrDados['txConsideracao'],
                            array('commit' => true)
                        ); #Reprovado. // ok
                        $this->enviaEmailTramitacao($arrDados, 'R', true);
                    }
                } else {
                    $this->rollback();
                    return false;
                }

                #Inserir Solicitacao Reprogramacao Prazo Obras
                $this->popularDadosObjeto($arrDados);
                $sppid = $this->salvar();

                $this->commit();
                return $sppid;
            }
        } catch (Exception $exc) {
            echo $exc->getTraceAsString();
            $this->rollback();
        }
    }

    public function listarAguardandoValidacao($arrDados)
    {

        $perfil = pegaArrayPerfil($_SESSION['usucpf']);
        $arrWhere = array('1=1');
        $arrWherePac = array('1=1');
        #Filtro UF
        if (!empty($arrDados['estuf'])) {
            array_push($arrWhere, "(pop.estuf = '{$arrDados['estuf']}' OR mun.estuf = '{$arrDados['estuf']}')");
            array_push($arrWherePac, "(po.estuf = '{$arrDados['estuf']}' OR mun.estuf = '{$arrDados['estuf']}')");
        }

        #Filtro Municipio
        if (!empty($arrDados['muncod'])) {
            array_push($arrWhere, "(pop.muncod = '{$arrDados['muncod']}' OR mun.muncod = '{$arrDados['muncod']}')");
            array_push($arrWherePac, "(po.muncod = '{$arrDados['muncod']}' OR mun.muncod = '{$arrDados['muncod']}')");
        }

        #Filtro Esfera
        if (!empty($arrDados['esfera'])) {
            if ($arrDados['esfera'] == 'M') {
                array_push($arrWhere, "pop.muncod IS NOT NULL");
                array_push($arrWherePac, "po.muncod IS NOT NULL");
            } else {
                array_push($arrWhere, "pop.muncod IS NULL");
                array_push($arrWherePac, "po.muncod IS NULL");
            }
        }

        #Filtro Esfera
        if (!empty($arrDados['esdid'])) {
            array_push($arrWhere, "esd.esdid = {$arrDados['esdid']}");
            array_push($arrWherePac, "esd.esdid = {$arrDados['esdid']}");
        }

        #Filtro Obrid
        if (!empty($arrDados['obrid'])) {
            array_push($arrWhere, "pre.obrid = {$arrDados['obrid']}");
            array_push($arrWherePac, "pre.obrid = {$arrDados['obrid']}");
        }

        #Filtro Preid
        if (!empty($arrDados['preid'])) {
            array_push($arrWhere, "pre.preid = {$arrDados['preid']}");
            array_push($arrWherePac, "pre.preid = {$arrDados['preid']}");
        }

        #Filtro Processo
        if (!empty($arrDados['pronumeroprocesso'])) {
            $arrDados['pronumeroprocesso'] = str_replace(array('.', '/', '-'), '', $arrDados['pronumeroprocesso']);
            array_push($arrWhere, "pronumeroprocesso = '{$arrDados['pronumeroprocesso']}'");
            array_push($arrWherePac, "pronumeroprocesso = '{$arrDados['pronumeroprocesso']}'");
        }

        #Filtro Documento
        if (!empty($arrDados['documento'])) {
            //retira tudo que não for numero
            $arrDados['documento'] = preg_replace('![^0-9]!', '', $arrDados['documento']);

            array_push($arrWhere, "dop.dopnumerodocumento = {$arrDados['documento']}");
            array_push($arrWherePac, "ter.ternumero = {$arrDados['documento']}");
        }

        //Filtro por data
        if (!empty($arrDados['dt_fim_vigencia_inicio']) && !empty($arrDados['dt_fim_vigencia_fim'])) {
            array_push(
                $arrWhere,
                " to_date(par.retornavigenciaobrapar(dop.proid),'DD/MM/YYYY') >= '{$arrDados['dt_fim_vigencia_inicio']}'::date " .
                " AND to_date(par.retornavigenciaobrapar(dop.proid),'DD/MM/YYYY') <= '{$arrDados['dt_fim_vigencia_fim']}'::date "
            );

            array_push(
                $arrWherePac,
                " to_date( to_char(ter.terdatafimvigencia,'DD/MM/YYYY'),'DD/MM/YYYY') >= '{$arrDados['dt_fim_vigencia_inicio']}'::date " .
                " AND to_date(to_char(ter.terdatafimvigencia,'DD/MM/YYYY'),'DD/MM/YYYY') <= '{$arrDados['dt_fim_vigencia_fim']}'::date "
            );
        }

        #Acao Exportar XLS
        $imgMais = "<span style=\"font-size:18px; cursor:pointer;\" title=\"mais\" id=\"image_'||pop.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||pop.pronumeroprocesso||'\', '||pop.proid||', \'\', this, \'PAR\');\"></span>";
        $imgGerarTermo = "' || CASE WHEN esd.esdid = 1676 THEN '<span style=\"font-size:18px; cursor:pointer;\" title=\"Gerar Termo PAR\" proid=\"'|| pop.proid ||'\" sppid=\"'|| spp.sppid ||'\" class=\"glyphicon glyphicon-refresh gerarTermoPar\"></span>' ELSE '' END || '";
        $acaoPAR = "'<center>{$imgMais}{$imgGerarTermo}&nbsp;'||
        					case when esd.esdid in (1669, 1671, 1672, 1673, 1674, 1675, 1676, 1906, 1908, 1909) then
        						'<span style=\"font-size:18px; cursor:pointer;\" title=\"Reprogramação de Prazo\" class=\"glyphicon glyphicon-calendar\" onclick=\"abreSolicitacaoReprogramacao(\''||dop.dopid||'\', \''||dop.proid||'\', \'OBRAPAR\', \''||spp.sppid||'\');\"></span>'
        					else '' end as acao,";

        if (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)) {
            $acaoPAR = "'<center>{$imgMais}{$imgGerarTermo}&nbsp;'||
	        	case when esd.esdid in (1669, 1671, 1672, 1673, 1674, 1676, 1670, 1675, 1677, 1906, 1908, 1909, 1910) then
	        	'<span style=\"font-size:18px; cursor:pointer;\" title=\"Reprogramação de Prazo\" class=\"glyphicon glyphicon-calendar\" onclick=\"abreSolicitacaoReprogramacao(\''||dop.dopid||'\', \''||dop.proid||'\', \'OBRAPAR\', \''||spp.sppid||'\');\"></span>'
	        	else '' end as acao,";
        }

        $imgMaisPAC = "<span style=\"font-size:18px; cursor:pointer;\" title=\"mais\" id=\"image_'||po.proid||'\" class=\"glyphicon glyphicon-download\" onclick=\"carregarListaObras(\''||po.pronumeroprocesso||'\', '||po.proid||', \'\', this, \'PAC\');\"></span>";
        /* $imgGerarTermoPAC = "' || CASE WHEN esd.esdid = 1676 THEN '<span style=\"font-size:18px; cursor:pointer;\" title=\"Gerar Termo PAC\" proid=\"'|| (SELECT DISTINCT
          CASE WHEN ter.muncod IS NOT NULL
          THEN ti.proid || '_' || po.protipo || '_' || ti.muncod || '_null' || '_' || ti.terid
          ELSE ti.proid || '_' || po.protipo || '_' || '_null' || ti.estuf || '_' || ti.terid
          END
          FROM par.termocompromissopac ti
          INNER JOIN par.processoobra poi ON poi.proid = ti.proid
          WHERE ti.proid = ter.proid AND ti.terstatus = 'A' LIMIT 1) ||'\" class=\"glyphicon glyphicon-refresh gerarTermoPac\"></span>' ELSE '' END || '"; */
        $imgGerarTermoPAC = "''";

        //'<input type=\"button\" value=\"Validar\" onclick=\"abreSolicitacaoReprogramacao(\''||ter.terid||'\', \''||ter.proid||'\', \'OBRAPAC\', \''||spp.sppid||'\')\" /></center>'
        /*
1669 - Aguardando Prorrogação de Prazo
1671- Em Análise CGIMP
1672 - Em Análise CGEST
1674 - Prorrogação de Prazo Reprovada
1676 - Aguardando Geração de Termo
         * */
        $acaoPAC = "'<center>{$imgMaisPAC}{$imgGerarTermoPAC}&nbsp;'||
        				case when esd.esdid in (1669, 1671, 1672, 1673, 1674, 1675, 1676, 1906, 1908, 1909) then
        					'<span style=\"font-size:18px; cursor:pointer;\" title=\"Reprogramação de Prazo\" class=\"glyphicon glyphicon-calendar\" onclick=\"abreSolicitacaoReprogramacao(\''||ter.terid||'\', \''||ter.proid||'\', \'OBRAPAC\', \''||spp.sppid||'\');\"></span>'
                        else '' end as acao,";

        if (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)) {
            $acaoPAC = "'<center>{$imgMaisPAC}{$imgGerarTermoPAC}&nbsp;'||
        				case when esd.esdid in (1669, 1671, 1672, 1673, 1674, 1675, 1676, 1670, 1677, 1906, 1908, 1909, 1910) then
        					'<span style=\"font-size:18px; cursor:pointer;\" title=\"Reprogramação de Prazo\" class=\"glyphicon glyphicon-calendar\" onclick=\"abreSolicitacaoReprogramacao(\''||ter.terid||'\', \''||ter.proid||'\', \'OBRAPAC\', \''||spp.sppid||'\');\"></span>'
                        else '' end as acao,";
        }
        $acaoOrdenacao = "'<input type=\"hidden\" value=\"' || sppdtinclusao || '\">' || ";
        if ($arrDados['requisicao'] == 'xls') {
            $acaoPAR = "";
            $acaoPAC = "";
            $acaoOrdenacao = "";
        }

        #Formatacao Processo
        $processoPAR = "'<span class=\"processo_detalhe\" >' || to_char(pop.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>'";
        $processoPAC = "'<span class=\"processo_detalhe\" >' || to_char(po.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>'";
        if ($arrDados['requisicao'] == 'xls') {
            $processoPAR = "(TO_CHAR(pop.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00'))";
            $processoPAC = "(TO_CHAR(po.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00'))";
        }

        #Formatar Anexos PAR
        $anexoCronPAR = "'<a href=\"par.php?modulo=principal/acompanhamento/ferramentas/validarSolicitacaoProrrogacaoPrazo&acao=A&requisicao=downloadArquivo&arqid='||(SELECT MAX(arqid) FROM par.anexosolicitacaoprorrogacaoprazoobra WHERE sppid = spp.sppid AND aspstatus = 'A' AND asptipo = 'C')|| '\" /> Cronograma</a>'";
        $anexoDeclPAR = "'<a href=\"par.php?modulo=principal/acompanhamento/ferramentas/validarSolicitacaoProrrogacaoPrazo&acao=A&requisicao=downloadArquivo&arqid='||(SELECT MAX(arqid) FROM par.anexosolicitacaoprorrogacaoprazoobra WHERE sppid = spp.sppid AND aspstatus = 'A' AND asptipo = 'D')|| '\" /> Declaracao</a>'";
        if ($arrDados['requisicao'] == 'xls') {
            $anexoCronPAR = "CASE WHEN (SELECT MAX(arqid) FROM par.anexosolicitacaoprorrogacaoprazoobra WHERE sppid = spp.sppid AND aspstatus = 'A' AND asptipo = 'C') > 0 THEN 'Sim' ELSE 'Não' END";
            $anexoDeclPAR = "CASE WHEN (SELECT MAX(arqid) FROM par.anexosolicitacaoprorrogacaoprazoobra WHERE sppid = spp.sppid AND aspstatus = 'A' AND asptipo = 'D') > 0 THEN 'Sim' ELSE 'Não' END";
        }

        #Formatar Anexos PAC
        $anexoCronPAC = "'<a href=\"par.php?modulo=principal/acompanhamento/ferramentas/validarSolicitacaoProrrogacaoPrazo&acao=A&requisicao=downloadArquivo&arqid='||(SELECT MAX(arqid) FROM par.anexosolicitacaoprorrogacaoprazoobra WHERE sppid = spp.sppid AND aspstatus = 'A' AND asptipo = 'C')|| '\" /> Cronograma</a>'";
        $anexoDeclPAC = "'<a href=\"par.php?modulo=principal/acompanhamento/ferramentas/validarSolicitacaoProrrogacaoPrazo&acao=A&requisicao=downloadArquivo&arqid='||(SELECT MAX(arqid) FROM par.anexosolicitacaoprorrogacaoprazoobra WHERE sppid = spp.sppid AND aspstatus = 'A' AND asptipo = 'D')|| '\" /> Declaracao</a>'";
        if ($arrDados['requisicao'] == 'xls') {
            $anexoCronPAC = "CASE WHEN (SELECT MAX(arqid) FROM par.anexosolicitacaoprorrogacaoprazoobra WHERE sppid = spp.sppid AND aspstatus = 'A' AND asptipo = 'C') > 0 THEN 'Sim' ELSE 'Não' END";
            $anexoDeclPAC = "CASE WHEN (SELECT MAX(arqid) FROM par.anexosolicitacaoprorrogacaoprazoobra WHERE sppid = spp.sppid AND aspstatus = 'A' AND asptipo = 'D') > 0 THEN 'Sim' ELSE 'Não' END";
        }

        $sqlPAR = "SELECT DISTINCT
                        $acaoPAR
                        CASE WHEN pop.estuf IS NOT NULL THEN pop.estuf  ELSE mun.estuf END AS uf,
                        CASE WHEN mun.mundescricao IS NOT NULL THEN mun.mundescricao ELSE '-' END AS municipio,
                        CASE WHEN pop.muncod IS NOT NULL THEN 'Municipal'  ELSE 'Estadual' END AS esfera,
                        $processoPAR as processo,
                        pop.proid,
                        dop.dopnumerodocumento  as numero_termo,
                        {$acaoOrdenacao} TO_CHAR(sppdtinclusao, 'DD/MM/YYYY') as sppdtinclusao,
                        TO_CHAR(sppdtfimsolicitado, 'DD/MM/YYYY') as prazo_requisitado,
                        TO_CHAR(sppdtfimaprovada, 'DD/MM/YYYY') as sppdtfimaprovada,
                        COALESCE(TO_CHAR(sppdtvalidacao, 'DD/MM/YYYY'), '<center> - </center>') as dt_tramitacao,
                        esd.esddsc as situacao_pedido,
                        sppjustificativa,
                        sppacaotomada,
                        sppoutrainformacao,
                        TO_CHAR(to_date(par.retornavigenciaobrapar(dop.proid),'DD/MM/YYYY'), 'DD/MM/YYYY') as dt_vigencia,
                        CASE WHEN sppvlrsuficientefnde = TRUE
                            THEN '<center> Sim </center>'
                            ELSE '<center> Não </center>'
                        END AS sppvlrsuficientefnde,
                        $anexoCronPAR as anexo_cronograma,
                        $anexoDeclPAR as anexo_declaracao
                    FROM par.solicitacaoprorrogacaoprazoobra spp
                    INNER JOIN par.documentopar dop ON dop.dopid = spp.dopid
                    INNER JOIN par.processoobraspar pop ON pop.proid = dop.proid AND prostatus = 'A'
                    INNER JOIN par.processoobrasparcomposicao popc ON popc.proid = pop.proid AND pocstatus = 'A'
                    INNER JOIN obras.preobra pre ON pre.preid = popc.preid
                    LEFT JOIN territorios.municipio mun ON mun.muncod = pop.muncod
                    INNER JOIN workflow.documento doc ON doc.docid = spp.docid
                    INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                    WHERE " . implode(' AND ', $arrWhere);

        $sqlPAC = "SELECT DISTINCT
                        $acaoPAC
                        CASE WHEN po.estuf IS NOT NULL THEN po.estuf  ELSE mun.estuf END AS uf,
                        CASE WHEN mun.mundescricao IS NOT NULL THEN mun.mundescricao ELSE '-' END AS municipio,
                        CASE WHEN po.muncod IS NOT NULL THEN 'Municipal'  ELSE 'Estadual' END AS esfera,
                        $processoPAC as processo,
                        po.proid,
                        ter.ternumero  as numero_termo,
                        {$acaoOrdenacao} TO_CHAR(sppdtinclusao, 'DD/MM/YYYY') as sppdtinclusao,
                        TO_CHAR(sppdtfimsolicitado, 'DD/MM/YYYY') as prazo_requisitado,
                        TO_CHAR(sppdtfimaprovada, 'DD/MM/YYYY') as sppdtfimaprovada,
                        COALESCE(TO_CHAR(sppdtvalidacao, 'DD/MM/YYYY'), '<center> - </center>') as dt_tramitacao,
                        esd.esddsc as situacao_pedido,
                        sppjustificativa,
                        sppacaotomada,
                        sppoutrainformacao,
                        TO_CHAR(ter.terdatafimvigencia,'DD/MM/YYYY') as dt_vigencia,
                        CASE WHEN sppvlrsuficientefnde = TRUE
                            THEN '<center> Sim </center>'
                            ELSE '<center> Não </center>'
                        END AS sppvlrsuficientefnde,
                        $anexoCronPAC as anexo_cronograma,
                        $anexoDeclPAC as anexo_declaracao
                    FROM par.solicitacaoprorrogacaoprazoobra spp
                    INNER JOIN par.termocompromissopac ter ON ter.terid = spp.terid and ter.terstatus = 'A'
                    INNER JOIN par.processoobra po ON po.proid = ter.proid AND prostatus = 'A'
                    INNER JOIN par.processoobraspaccomposicao popc ON popc.proid = po.proid AND popc.pocstatus = 'A'
                    INNER JOIN obras.preobra pre ON pre.preid = popc.preid
                    LEFT JOIN territorios.municipio mun ON mun.muncod = po.muncod
                    INNER JOIN workflow.documento doc ON doc.docid = spp.docid
                    INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                    WHERE " . implode(' AND ', $arrWherePac);

        $orderPadrao = "";
        if (!$_REQUEST['ordemlista']) {
            $orderPadrao = " ORDER BY sppdtinclusao DESC ";
        }

        #Filtro Fonte Preoobra
        $sql = "";
        if (!empty($arrDados['fonte_preobra'])) {
            if ($arrDados['fonte_preobra'] == 'PAR') {
                $sql = $sqlPAR . $orderPadrao;
            } else {
                $sql = $sqlPAC . $orderPadrao;
            }
        } else {
            $sql = "(" . $sqlPAR . $orderPadrao . ") UNION ALL (" . $sqlPAC . $orderPadrao . ")";
        }

        return ($this->carregar($sql)) ? $this->carregar($sql) : array();
    }

    public function carregarListaObras($request, $tipo)
    {
        global $db;
        if ($tipo == "PAR") {
            $acoes = "'<center><span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObrasPar('||p.preid||', '||p.preano||')\"></span></center>' as acoes,";

            $sql = "select
			$acoes
			p.preid,
			p.obrid,
			p.predescricao,
			cast(p.prevalorobra as numeric(20,2)) as valorobra,
			vlo.totalpago as valorpagoobra,
			pt.ptodescricao,
			res.res_estado,
			str.strdsc as estado2,
			p.preano,
			(SELECT popqtddiassolicitado FROM obras.preobraprorrogacao WHERE  preid = p.preid order by popid desc limit 1) as diasvalidado,
			((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0))::numeric(20,2) as porcentagem_execucao,
			(select count(pp.popid) from obras.preobraprorrogacao pp where pp.preid = p.preid) as totalPro
			from
			par.v_saldo_obra_por_empenho v
			inner join obras.preobra p on p.preid = v.preid and p.prestatus = 'A'
			left join obras.pretipoobra pt on pt.ptoid = p.ptoid and pt.ptostatus = 'A'
			left join par.v_pagamento_total_por_obra vlo ON vlo.preid = p.preid
			LEFT JOIN obras2.obras o ON o.preid = p.preid AND o.obridpai IS NULL AND o.obrstatus = 'A'
			left join obras2.situacao_registro str on str.strid = o.strid and str.strstatus = 'A'
			left join( select
			case when count(r.rstid) > 0 then
			case when d.esdid in (1142, 1143) then 'Não' else 'Sim' end
			else 'Não' end as res_estado,
			count(r.rstid), p1.obrid, d.esdid
			from
			obras.preobra p1
			left join obras2.restricao r
			inner join workflow.documento d on d.docid = r.docid
			on r.obrid = p1.obrid and r.tprid = 17 and r.rstitem = 'R' and r.rststatus = 'A'
			group by p1.obrid, d.esdid
			) res ON res.obrid = p.obrid
			$join
			where
			v.tipo = 'PAR'
			and v.processo = '{$request['processo']}'
			group by p.prevalorobra, p.predescricao, p.preano, p.preid, pt.ptodescricao, p.obrid, porcentagem_execucao, str.strdsc, vlo.totalpago, res.res_estado, v.processo";

            $arrDados = $db->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();

            $arrRegistro = array();
            foreach ($arrDados as $v) {
                array_push($arrRegistro, array(
                        'acoes' => $v['acoes'],
                        'preid' => $v['preid'],
                        'obrid' => $v['obrid'],
                        'restricao' => $v['res_estado'],
                        'predescricao' => $v['predescricao'],
                        'ptodescricao' => $v['ptodescricao'],
                        'estado2' => $v['estado2'],
                        'valorobra' => $v['valorobra'],
                        'valorpagoobra' => $v['valorpagoobra'],
                        'porcentagem_execucao' => $v['porcentagem_execucao'],
                        'tempo' => $v['diasvalidado'],
                    ));
            }

            $cabecalho = array(
                '&nbsp;&nbsp;Ações&nbsp;&nbsp;',
                'preid',
                'obrid',
                'Restrições tipo Diligência do Obras 2',
                'Descrição',
                'Tipo da Obra',
                'Situação Obras 2.0',
                'Valor da Obra',
                'Valor pago da Obra',
                '% de Execução da Obra',
                'Tempo de Prorrogação'
            );
            $db->monta_lista_simples($arrRegistro, $cabecalho, 100, 5, 'N', '100%', 'S', true, false, false, false);

            exit();
        } else {
            $acoes = "'<center><span style=\"font-size:18px; cursor:pointer;\" title=\"Abrir Obra\" class=\"glyphicon glyphicon-question-sign\" onclick=\"abreObras('||p.preid||', '||p.preano||')\"></span>
								</center>' as acoes,";
            $sql = "select
						$acoes
						p.preid,
						p.obrid,
						p.predescricao,
						cast(p.prevalorobra as numeric(20,2)) as valorobra,
						vlo.totalpago as valorpagoobra,
						pt.ptodescricao,
						CASE WHEN res.res_estado = 1 THEN 'Sim' ELSE 'Não' END res_estado,
						str.strdsc as estado2,
						p.preano,
						(SELECT popqtddiassolicitado FROM obras.preobraprorrogacao WHERE  preid = p.preid order by popid desc limit 1) as diasvalidado,
						((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0))::numeric(20,2) as porcentagem_execucao,
						(select count(pp.popid) from obras.preobraprorrogacao pp where pp.preid = p.preid) as totalPro
						from
						par.v_saldo_obra_por_empenho v
						inner join obras.preobra p on p.preid = v.preid and p.prestatus = 'A'
						left join par.v_pagamento_total_por_obra vlo ON vlo.preid = p.preid
						LEFT JOIN obras2.obras o ON o.preid = p.preid AND o.obridpai IS NULL AND o.obrstatus = 'A'
						left join obras2.situacao_registro str on str.strid = o.strid and str.strstatus = 'A'
						left join obras.pretipoobra pt on pt.ptoid = p.ptoid and pt.ptostatus = 'A'
						left join( select
						1 AS res_estado,
						count(r.rstid), p1.obrid, d.esdid
						from
						obras.preobra p1
						left join obras2.restricao r
						inner join workflow.documento d on d.docid = r.docid
						on r.obrid = p1.obrid and r.tprid = 17 and r.rstitem = 'R' and r.rststatus = 'A'
						group by p1.obrid, d.esdid
						) res ON res.obrid = p.obrid AND res.esdid NOT IN (1142, 1143)
						$join
						where
						v.tipo = 'PAC'
						and v.processo = '{$_REQUEST['processo']}'
						and v.saldo > 0
						$filtro

						group by p.prevalorobra, p.predescricao, p.preano, p.preid, pt.ptodescricao,str.strdsc, p.obrid, vlo.totalpago, porcentagem_execucao, res.res_estado, v.processo";

            $arrDados = $db->carregar($sql); //ver($sql, d);
            $arrDados = $arrDados ? $arrDados : array();

            $arrRegistro = array();
            foreach ($arrDados as $v) {
                array_push($arrRegistro, array(
                        'acoes' => $v['acoes'],
                        'preid' => $v['preid'],
                        'obrid' => $v['obrid'],
                        'restricao' => $v['res_estado'],
                        'predescricao' => $v['predescricao'],
                        'ptodescricao' => $v['ptodescricao'],
                        'estado2' => $v['estado2'],
                        'valorobra' => $v['valorobra'],
                        'valorpagoobra' => $v['valorpagoobra'],
                        'porcentagem_execucao' => $v['porcentagem_execucao'],
                        'tempo' => $v['diasvalidado'],
                    ));
            }

            $cabecalho = array(
                '&nbsp;&nbsp;Ações&nbsp;&nbsp;',
                'preid',
                'obrid',
                'Restrições tipo Diligência do Obras 2',
                'Descrição',
                'Tipo da Obra',
                'Situação Obras 2.0',
                'Valor da Obra',
                'Valor pago da Obra',
                '% de Execução da Obra',
                'Tempo de Prorrogação'
            );
            $db->monta_lista_simples($arrRegistro, $cabecalho, 100, 5, 'N', '100%', 'S', true, false, false, false);

            exit();
        }
    }

    public function verificarSolicitacaoReformulacao($arrDados)
    {
        $tipoDocumento = ($arrDados['tipo'] == 'OBRAPAR') ? 'dopid' : 'terid';

        if ($arrDados['requisicao'] == 'analisar') {
            return false;
        }

        $sql = "SELECT
                    sppid
                FROM par.solicitacaoprorrogacaoprazoobra  spp
                INNER JOIN workflow.documento doc ON doc.docid = spp.docid
                WHERE spp.$tipoDocumento = {$arrDados['id']}
                AND doc.esdid NOT IN (" . ESDID_PRORROGACAO_PRAZO_FINALIZADA . ", " . ESDID_PRORROGACAO_PRAZO_REPROVADA . ")
                ORDER BY sppid DESC";
        return ($this->pegaUm($sql)) ? true : false;
    }

    public function buscarSituacaoProrrogacaoTermo($proid, $tipo, $sppid = null)
    {

        if ($sppid) {
            $where = "AND spp.sppid = " . $sppid;
        }

        #@TODO colocar na classe de Documento
        $id = '';
        if ($tipo == 'OBRAS') {
            $sqlDocumento = "SELECT
	                            spp.dopid
	                        FROM par.documentopar dop
	                        INNER JOIN par.solicitacaoprorrogacaoprazoobra spp ON spp.dopid = dop.dopid
	                        INNER JOIN workflow.documento doc ON doc.docid = spp.docid
	                        WHERE dop.proid = {$proid} {$where}
	                        ORDER BY spp.dopid DESC";
            $id = $this->pegaUm($sqlDocumento);
            $strOrigem = 'dopid';
        } else {
            $sqlDocumentoPac = "SELECT
                            spp.terid
                        FROM par.termocompromissopac ter
                        INNER JOIN par.solicitacaoprorrogacaoprazoobra spp ON spp.terid = ter.terid
                        INNER JOIN workflow.documento doc ON doc.docid = spp.docid
                        WHERE ter.proid = {$proid}
                        ORDER BY spp.terid DESC";
            $terid = $this->pegaUm($sqlDocumentoPac);
            $strOrigem = 'terid';
            $id = $terid;
        }

        $sql = "SELECT
                    esd.esddsc,
                    esd.esdid,
                    doc.docid,
                    TO_CHAR(sppdtfimaprovada, 'DD/MM/YYYY') as sppdtfimaprovada
                FROM par.solicitacaoprorrogacaoprazoobra  spp
                INNER JOIN workflow.documento doc ON doc.docid = spp.docid
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE spp.$strOrigem = {$id}
                AND doc.esdid NOT IN (" . ESDID_PRORROGACAO_PRAZO_FINALIZADA . ", " . ESDID_PRORROGACAO_PRAZO_REPROVADA . ")
                ORDER BY sppid DESC";

        if ($id) {
            return ($this->pegaLinha($sql)) ? $this->pegaLinha($sql) : false;
        } else {
            return false;
        }
    }

    public function buscarVigenciaAtual($arrDados)
    {
        $dataFimVigenciaAtual = '';
        if ($arrDados['tipo'] == 'OBRAPAR') {
            $objDocumentoPar = new DocumentoParModelo();
            $proid = reset($objDocumentoPar->recuperarTodos('proid', array("dopid = {$_REQUEST['id']}")));
            $dataFimVigenciaAtual = (new DocumentoParModelo())->dataFinalVigencia($proid['proid']);
        } else {
            $objTermoCompromissoPac = new TermoCompromissoPac();
            $proid = reset($objTermoCompromissoPac->recuperarTodos('proid', array("terid = {$_REQUEST['id']}")));
            $dataFimVigenciaAtual = $objTermoCompromissoPac->dataFinalVigencia($proid['proid']);
        }
        return $dataFimVigenciaAtual;
    }

    /***
     * Caso o termo já tenha sido reprogramado, porém ainda não validado a sua data vai estar do termo anterior o que até então permitia uma nova solicitação
     * O objetivo  deste método  é retornar bool TRUE, caso o termo atual tenha sido resultado de uma reprogramação de prazo anterior e não tenha validação ainda
     * */
    public function buscaValidacaoPARReprogramacaoAnterior($arrDados)
    {
        if( $arrDados['tipo'] == 'OBRAPAR' ) {
            
            // Busco o dopidpai que originou o termo
            $dopid = $arrDados['id'];
            if( $dopid ) {
                 
                // Primeiro vejo se o termo está validado, caso esteja já esteja validado, não entra nas demais regras e está ok.
                $validacao = $this->pegaUm("select count(dpvid) from par.documentoparvalidacao where dopid = {$dopid}");
                $validacao = ($validacao == 0) ? FALSE : TRUE;
                // CAso não possua validação
                if($validacao == FALSE) {
                    // PEgo o id do pai e verifico se veio de uma reprogramação
                    $sqlDopidPai = $this->pegaUm("select dopidpai from par.documentopar   where dopid = {$dopid}");
                
                    if( $sqlDopidPai != "" && ($sqlDopidPai) ) {
                        // Verifico se o dopid que originou o termo foi reprogramado e se possui a data fim
                        $sppid = $this->pegaUm("select sppid from par.solicitacaoprorrogacaoprazoobra   where dopid = {$sqlDopidPai} AND sppdtfimaprovada IS NOT NULL");
                        $sppid = ($sppid) ? TRUE: FALSE;
                        if($sppid) {
                            RETURN TRUE;
                        } else {
                             return false;
                        }
                        
                    }else {
                        return false;
                    }
                } else { 
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
        
    }
    public function buscarSolicitacao($arrDados)
    {

        $strWhere = "";
        $tipoDocumento = ($arrDados['tipo'] == 'OBRAPAR') ? 'dopid' : 'terid';
        $strWhere .= (!empty($arrDados['sppid'])) ? " AND spp.sppid = {$arrDados['sppid']} " : "";
        $strWhere .= ($arrDados['visao'] == 'entidade') ? " AND doc.esdid NOT IN (" . ESDID_PRORROGACAO_PRAZO_FINALIZADA . ", " . ESDID_PRORROGACAO_PRAZO_REPROVADA . ")" : "";

        $sql = "SELECT
                    doc.esdid,
                    sppid,
                    TO_CHAR(sppdtfimsolicitado, 'DD/MM/YYYY') as sppdtfimsolicitado,
                    TO_CHAR(sppdtfimaprovada, 'DD/MM/YYYY') as sppdtfimaprovada,
                    sppjustificativa,
                    sppacaotomada,
                    sppoutrainformacao,
                    sppvlrsuficientefnde,
                    doc.docid,
                    terid,
                    dopid,
                    (SELECT MAX(arqid) FROM par.anexosolicitacaoprorrogacaoprazoobra WHERE sppid = spp.sppid AND aspstatus = 'A' AND asptipo = 'C') as sppanexocronograma,
                    (SELECT MAX(arqid) FROM par.anexosolicitacaoprorrogacaoprazoobra WHERE sppid = spp.sppid AND aspstatus = 'A' AND asptipo = 'D') as sppanexodeclaracao
                FROM $this->stNomeTabela spp
                INNER JOIN workflow.documento doc ON doc.docid = spp.docid
                WHERE $tipoDocumento = {$arrDados['id']} $strWhere
                ORDER BY sppid DESC";
        return ($this->pegaLinha($sql)) ? $this->pegaLinha($sql) : array();
    }
}

//end Class
