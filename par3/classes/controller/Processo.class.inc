<?php
class Par3_Controller_Processo extends Controle
{
    protected $model;
    public function __construct()
    {
        $this->model = new Par3_Model_Processo();
    }
    
    public function carregaProcessosPorTipoObjeto($arrPost = array())
    {
        $intoid = $arrPost['intoid'];
        $inuid = $arrPost['inuid'];
        $inpid = $arrPost['inpid'];
        $iniano = $arrPost['iniano'];
        $anaid = $arrPost['anaid'];
        $intaid = $arrPost['intaid'];
        
        $sql = $this->model->sqlProcessosPorTipoObjeto($intoid, $inuid, $intaid);
        
        $tratamentoListaVazia = ($arrProcesso == '' ) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
        $cabecalho = array('Processo','Agência', 'Banco', 'CNPJ', 'Tipo Objeto', 'Tipo Assistência', 'Valor do Processo');
        //$esconderColunas = array('iniid', 'inuid', 'anaid');
        $listagem = new Simec_Listagem(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        $listagem->setCabecalho($cabecalho);
        $listagem->turnOnPesquisator();
        $listagem->setQuery($sql);
        //$listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('plus', 'detalharProcessoViculado');
        $listagem->addAcao('edit', array('func' => 'vinculaProcessoExistente', 'extra-params' => array('processo')));
        $listagem->addCallbackDeCampo('vlr_processo', 'formataNumeroMonetarioSemSimbolo');
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
        ?>
        <form method="post" name="form-vincula-processo" id="form-vincula-processo" class="form form-horizontal">
            <input type="hidden" name="inuid" id="inuid" value="<?php  echo $inuid; ?>">
            <input type="hidden" name="inpid" id="inpid" value="<?php  echo $inpid; ?>">
            <input type="hidden" name="iniano" id="iniano" value="<?php  echo $iniano; ?>">
            <input type="hidden" name="anaid" id="anaid" value="<?php  echo $anaid; ?>">
            <input type="hidden" name="intoid" id="intoid" value="<?php  echo $intoid; ?>">
            <input type="hidden" name="proid" id="proid" value="">
            <input type="hidden" name="requisicao_analise" id="requisicao_analise" value="">
        </form>
        <?php
    }
    
    public function carregaAnaliseVinculadaProcesso($proid)
    {
        $sql = "SELECT a.anaid, a.anaano, a.inpid, sum(it.aicqtdaprovado * it.aicvaloraprovado) AS valor FROM par3.analise a
                	INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid and pp.ppcstatus = 'A'
                	INNER JOIN par3.analise_itemcomposicao it ON it.anaid = a.anaid
                WHERE a.anastatus = 'A'
                	AND pp.proid = $proid
                GROUP BY a.anaid, a.anaano, a.inpid";
        
        $tratamentoListaVazia = ($arrProcesso == '' ) ? Simec_Listagem::SEM_REGISTROS_MENSAGEM : Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
        $cabecalho = array('Código Análise','Ano', 'Código Planejamento', 'Valor Aprovado');
        //$esconderColunas = array('iniid', 'inuid', 'anaid');
        $listagem = new Simec_Listagem(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        $listagem->setCabecalho($cabecalho);
        //$listagem->turnOnPesquisator();
        $listagem->setQuery($sql);
        //$listagem->esconderColunas($esconderColunas);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['valor']);
        $listagem->addCallbackDeCampo('valor', 'formataNumeroMonetarioSemSimbolo');
        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);
    }
    
    public function pegaOptionEntidadeExecutora($arrPost = array())
    {
        include_once APPRAIZ . 'includes/library/simec/view/Helper.php';
        
        $simec = new Simec_View_Helper();
        
        $sql = "SELECT ungid AS codigo, ungrazao_social AS descricao FROM par3.unidade_executora WHERE ungstatus = 'A' and inuid = {$arrPost['inuid']}";
        $arrEntidade = $this->model->carregar($sql);
        
        if( $arrEntidade[0] ){
            return $simec->select('ungid_p', '', $_REQUEST['ungid'], $sql, array('maxlength' => '255', 'onchange' => 'carregaMSG()'), array());
        } else {
            return 'N';
        }
    }
    
    public function vinculaIniciativaProcessoExistente($arrPost = array())
    {
        global $url;
        
        $url = $url.'&inpid='.$arrPost['inpid'].'&inuid='.$arrPost['inuid'].'&iniano='.$arrPost['iniano'].'&aba='.$arrPost['aba'].'&anaid='.$arrPost['anaid'];
        
        $sql = "SELECT count(ppcid) FROM par3.processoparcomposicao
				WHERE inpid = {$arrPost['inpid']}
					and ppcano = {$arrPost['iniano']} and anaid = {$arrPost['anaid']} and ppcstatus = 'A'";
        $boTem = $this->model->pegaUm($sql);
        
        if ((int)$boTem > 0) {
            simec_redirecionar($url, 'error', 'Este planejamento já está vinculada ao processo');
        } else {
            $sql = $this->model->vinculaIniciativaProcessoExistente($arrPost);
            $this->model->executar($sql);
            
            if ($this->model->commit()) {
                simec_redirecionar($url, 'success', 'Processo vinculado com sucesso');
            } else {
                simec_redirecionar($url, 'error', 'Erro ao vincular o processo');
            }
        }
    }
    
    public function autenticacaoModalProcesso($arrPost = array())
    {
        $intoid = $arrPost['intoid'];
        $inuid = $arrPost['inuid'];
        $inpid = $arrPost['inpid'];
        $iniano = $arrPost['iniano'];
        $anaid = $arrPost['anaid'];
        $intaid = $arrPost['intaid'];
                
        $html = '
        			<div style="text-align: center;">
	        			<h1 class="center" style="font-weight: bold;">Autenticação</h1>
					</div>
        		<div class="ibox-content">
        			<form method="post" name="form-autentica-processo" id="form-autentica-processo" class="form form-horizontal">
        			<div class="row">
	        			<input type="hidden" name="inuid" id="inuid" value="'.$inuid.'">
						<input type="hidden" name="inpid" id="inpid" value="'.$inpid.'">
						<input type="hidden" name="iniano" id="iniano" value="'.$iniano.'">
						<input type="hidden" name="anaid" id="anaid" value="'.$anaid.'">
						<input type="hidden" name="intoid" id="intoid" value="'.$intoid.'">
						<input type="hidden" name="intaid" id="intoid" value="'.$intaid.'">
						<input type="hidden" name="proid" id="proid" value="">
						<input type="hidden" name="requisicao_analise" id="requisicao_analise" value="">
		        		<div class="form-group ">
		                    <label for="codana" class="col-md-3 control-label">Usuário: </label>
		                    <div class="col-md-7 ">
		                        <input name="ws_usuario" id="ws_usuario" type="text" value="" class="form-control">
		                    </div>
		                </div>
					</div>
					<div class="row">
		                <div class="form-group ">
		                    <label for="codana" class="col-md-3 control-label">Senha: </label>
		                    <div class="col-md-7 ">
		                        <input name="ws_senha" id="ws_senha" type="password" value="" class="form-control">
		                    </div>
		                    <div style="clear:both"></div>
		                </div>
					</div>
                    <div class="row" id="div_main_entidade">
		                <div class="form-group ">
		                    <label for="codana" class="col-md-3 control-label">Entidade Executora: </label>
		                    <div class="col-md-7" id="div_entidade_executora">
		                        
		                    </div>
		                    <label for="codana" class="col-md-3 control-label"> </label>
		                    <div class="col-md-7" id="div_entidade_executora_msg" style="color: red; display: none">
		                        A entidade selecionada será responsável pela execução dos recursos recebidos e prestação de contas do termo gerado para este processo. Certifique-se que a entidade selecionada está correta
		                    </div>
		                    <div style="clear:both"></div>
		                </div>
					</div>
        		</form></div>';
        return $html;
    }
    
    public function gerarProcesoFNDE($arrPost = array())
    {
        global $url;
        include_once APPRAIZ.'includes/classes/ProcessoFNDE.class.php';
        include_once APPRAIZ.'par3/classes/controller/WS_Servico_FNDE.class.inc';
                
        if ($arrPost['tipo_processo'] == 'obra') {
            $prptipo = 'P';
            $protipoexecucao = '';
            $boTem = 0;
        } elseif ($arrPost['tipo_processo'] == 'par') {
            $prptipo = 'O';
            $protipoexecucao = 'T';
            $boTem = 0;
        } else {
            $prptipo = 'O';
            $protipoexecucao = 'T';
            $sql = "SELECT count(ppcid) FROM par3.processoparcomposicao WHERE inpid = {$arrPost['inpid']} and ppcano = {$arrPost['iniano']} and anaid = {$arrPost['anaid']} and ppcstatus = 'A'";
            $boTem = $this->model->pegaUm($sql);
        }
        
        if ((int)$boTem > 0) {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Este planejamento já está vinculada ao processo');
        } else {            
            $sql = "select itrid, muncod from par3.instrumentounidade where inuid = {$arrPost['inuid']}";
            $arInstrumento = $this->model->pegaLinha($sql);
            $itrid = $arInstrumento['itrid'];
            $muncod = $arInstrumento['muncod'];
            
            if ((int)$itrid == 1 || (int)$itrid == 3) { #estadual
                $muncod = 'null';
                $entcnpj = $this->model->pegaUm("select entcnpj from par3.instrumentounidade_entidade e where e.inuid = {$arrPost['inuid']} and tenid = 3 and entstatus = 'A' and entcnpj is not null");
            } else { #municipal
                $entcnpj = $this->model->pegaUm("select entcnpj from par3.instrumentounidade_entidade e where e.inuid = {$arrPost['inuid']} and tenid = 1 and entstatus = 'A' and entcnpj is not null");
            }
            
            if($arrPost['ungid_p']){
                $sql = "SELECT inuid, ungcnpj FROM par3.unidade_executora WHERE ungstatus = 'A' AND ungid = {$arrPost['ungid_p']}";
                $arEntidade = $this->model->pegaLinha($sql);
                $entcnpj = $arEntidade['ungcnpj'];
            }
            
            $arrPost['ungid_p'] = ($arrPost['ungid_p'] ? $arrPost['ungid_p'] : 'null');
            
            $arrParamProcesso = array(
                                'nu_cpf'    => $_SESSION['usucpf'],
                                'ds_resumo' => 'Manutenção e desenvolvimento da educação',
                                'co_assunto'=> '100',
                                'nu_cnpj'   => $entcnpj
                            );
            
            //ver($arrParamProcesso, $arrPost, "select entcnpj from par3.instrumentounidade_entidade e where e.inuid = {$arrPost['inuid']} and tenid = 3 and entstatus = 'A'",$itrid,d);
            $obProcesso = new ProcessoFNDE($arrPost['ws_usuario'], $arrPost['ws_senha'], false);
            $arrProcesso = $obProcesso->gerarProcessoFNDE($arrParamProcesso);
            
            if (!is_array($arrProcesso)) {
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', $arrProcesso);
                die;
            }
                        
            if ($arrProcesso) {
                $numeroProcesso = $arrProcesso['processo'];
                $nu_banco       = $arrProcesso['banco'];
                $nu_agencia     = $arrProcesso['agencia'];

                $arrParam = array('banco' => $nu_banco, 'agencia' => $nu_agencia);
                                
                $sql = "INSERT INTO par3.processo(pronumeroprocesso, muncod, probanco, proagencia, usucpf, protipo, inuid, protipoexecucao, prodocumenta, procnpj, prodatainclusao, prostatus, ungid)
						VALUES ('{$numeroProcesso}', {$muncod}, '{$nu_banco}', '{$nu_agencia}', '{$_SESSION['usucpf']}', '$prptipo', {$arrPost['inuid']}, '{$protipoexecucao}', null, '{$entcnpj}', now(), 'A', {$arrPost['ungid_p']}) returning proid";
                $proid = $this->model->pegaUm($sql);

                $obProcesso->WSHistoricoProcesso($arrProcesso['arqxml'], $arrProcesso['xmlretorno'], 'GERAPROC - GerarProcesso - Sucesso'.($arrPost['anaid'] ? ' - analise: '.$arrPost['anaid'] : '' ));

                if ($proid) {
                    if ($arrPost['tipo_processo'] == 'obra') {
                        $intaid = 1;
                        foreach ($arrPost['check_acao'] as $inpid => $arObrid) {
                            foreach ($arObrid as $obrid) {
                                $sql = "SELECT count(ie.ineid)
                                        FROM par3.iniciativa_emenda ie
                                        	INNER JOIN par3.iniciativa_emenda_obra eo ON eo.ineid = ie.ineid AND eo.ieostatus = 'A'
                                        WHERE ie.inestatus = 'A' AND eo.obrid = $obrid";
                                $boTem = $this->model->pegaUm($sql);
                                if ((int)$boTem > (int)0) {
                                    $intaid = 2;
                                }
                                
                                $sql = "INSERT INTO par3.processoobracomposicao(proid, obrid, pocstatus, intoid)
										VALUES ($proid, $obrid, 'A', '1')";
                                $this->model->executar($sql);
                            }
                        }
                        $this->model->executar("UPDATE par3.processo SET intaid = $intaid WHERE proid = $proid");
                    } elseif ($arrPost['tipo_processo'] == 'par') {
                        foreach ($arrPost['check_acao'] as $inpid => $arObrid) {
                            $anaid_par = (is_array($arrPost['anaid']) ? $arrPost['anaid'][$inpid] : $arrPost['anaid']);
                            
                            $sql = "SELECT distinct ito.intoid
                    		    	FROM par3.iniciativa_planejamento ip
                    			    	inner join par3.iniciativa i on i.iniid = ip.iniid
                                        inner join par3.iniciativa_tipos_objeto ito on ito.intoid = i.intoid
                    		    	WHERE ip.inpid = $inpid";
                            $intoid = $this->model->pegaUm($sql);
                            
                            $intaid = $this->model->pegaUm("SELECT intaid FROM par3.analise WHERE anaid = {$anaid_par}");
                            $this->model->executar("UPDATE par3.processo SET intaid = $intaid WHERE proid = $proid");
                            
                            foreach ($arObrid as $ano) {
                                $sql = "INSERT INTO par3.processoparcomposicao(proid, inpid, ppcano, anaid, intoid, ppcstatus)
						                  VALUES ($proid, {$inpid}, '{$ano}', {$anaid_par}, {$intoid}, 'A')";
                                $this->model->executar($sql);
                            }
                        }
                    } else {
                        $intaid = $this->model->pegaUm("SELECT intaid FROM par3.analise WHERE anaid = {$arrPost['anaid']}");
                        $this->model->executar("UPDATE par3.processo SET intaid = $intaid WHERE proid = $proid");
                        
                        $sql = "INSERT INTO par3.processoparcomposicao(proid, inpid, ppcano, anaid, intoid, ppcstatus)
								VALUES ($proid, {$arrPost['inpid']}, '{$arrPost['iniano']}', {$arrPost['anaid']}, {$arrPost['intoid']}, 'A')";
                        $this->model->executar($sql);
                    }
                                        
                    $sql = "SELECT sisid, tprid, tipprogramafnde, tipnumerosistemasigef, tiptipoprocessosigef FROM execucaofinanceira.tipoprocesso where tipid = 12 and tipstatus = 'A'";
                    $arTipProcesso = $this->model->pegaLinha($sql);
                    
                    $arrParamProcesso = array(
                                    'an_processo'           => date("Y"),
                                    'nu_processo'           => $numeroProcesso,
                                    'tp_processo'           => $arTipProcesso['tiptipoprocessosigef'],
                                    'co_programa_fnde'      => $arTipProcesso['tipprogramafnde'],
                                    'nu_sistema'            => $arTipProcesso['tipnumerosistemasigef'],
                                    'nu_cnpj_favorecido'    => $entcnpj,
                                    'proid'                 => $proid,
                                    'ws_usuario'            => $arrPost['ws_usuario'],
                                    'ws_senha'              => $arrPost['ws_senha'],
                                    'nu_banco'              => $nu_banco,
                                    'nu_agencia'            => $nu_agencia,
                                );

                    $res_sp = $obProcesso->solicitarProcesso($arrParamProcesso);
                    if ($res_sp) {
                        $this->model->commit();
                        $obServicoFnde = new Par3_Controller_WS_Servico_FNDE($arrParamProcesso['ws_usuario'], $arrParamProcesso['ws_senha']);
                        $arrConta = $obServicoFnde->consultaContaCorrenteProcessoFNDE($proid);

                        if (!is_array($arrConta) && !is_object($arrConta)) {
                            $retornoCC = $obServicoFnde->solicitaAberturaContaCorrente($proid);
                            if (is_array($retornoCC) || is_object($retornoCC)) {
                                $this->atualizaContaCorrentePAR($proid, (object)$retornoCC);
                            } else {
                                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', $retornoCC);
                            }
                        } else {
                            $this->atualizaContaCorrentePAR($proid, $arrConta);
                        }
                    } else {
                        simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Criando Projeto', 'Não foi possível solicitar o processo.');
                    }
                } else {
                    simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Criando Projeto', 'Não foi possível inserir o processo.');
                }
            }
        }
        $msgAlert = "Processo gerado com sucesso: ". formata_numero_processo_sem_html((string) $numeroProcesso);
        simec_redirecionar($_SERVER['HTTP_REFERER'], 'success', $msgAlert);
    }
    
    public function atualizaContaCorrentePAR($proid, $arrConta)
    {
        global $db;
        
        $seq_solic_cr       = !empty($arrConta->seq_solic_cr) ? (int)$arrConta->seq_solic_cr : 'null';
        $seq_conta          = !empty($arrConta->seq_conta) ? (int)$arrConta->seq_conta : 'null';
        $dt_movimento       = !empty($arrConta->dt_movimento) ? (int)$arrConta->dt_movimento : 'null';
        $nu_banco           = !empty($arrConta->nu_banco) ? "'".$arrConta->nu_banco."'" : 'null';
        $nu_agencia         = !empty($arrConta->nu_agencia) ? "'".$arrConta->nu_agencia."'" : 'null';
        $nu_conta_corrente  = !empty($arrConta->nu_conta_corrente) ? "'".$arrConta->nu_conta_corrente."'" : 'null';
        $fase_solicitacao   = !empty($arrConta->fase_solicitacao) ? "'".$arrConta->fase_solicitacao."'" : 'null';
        $co_situacao_conta  = !empty($arrConta->co_situacao_conta) ? (int)$arrConta->co_situacao_conta : 'null';
        $situacao_conta     = !empty($arrConta->situacao_conta) ? "'".$arrConta->situacao_conta."'" : 'null';
        $nu_processo        = !empty($arrConta->nu_processo) ? "'".$arrConta->nu_processo."'" : 'null';
        $nu_identificador   = !empty($arrConta->nu_identificador) ? "'".$arrConta->nu_identificador."'" : 'null';
        $ds_razao_social    = !empty($arrConta->ds_razao_social) ? "'".$arrConta->ds_razao_social."'" : 'null';

        if (!empty($seq_solic_cr)) {
            $sql = "UPDATE par3.processo SET
						proseqconta = $seq_conta,
						seq_conta_corrente = $seq_solic_cr,
						nu_conta_corrente = $nu_conta_corrente,
						dt_movimento = $dt_movimento,
						fase_solicitacao = $fase_solicitacao,
						co_situacao_conta = $co_situacao_conta,
						situacao_conta = $situacao_conta,
						ds_razao_social = $ds_razao_social
					WHERE
					  	proid = $proid";
            
            $this->model->executar($sql);
            $this->model->commit();
        }
        return true;
    }

    function hintPorcentagemPagamento($porcentagem)
    {
        return '<span title="% de pagamento baseado no valor do termo">' . $porcentagem . '</span>';

    }

    public function listar($arrPost)
    {
        
        $sql = $this->model->montarSQLProcesso($arrPost);
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Processo','UF', 'Entidade', 'Tipo de Objeto', 'Data Vigência', 'Valor Processo', 'Valor Termo', 'Valor Empenho', 'Valor Pagamento','% Pago','Assistência');
        $esconderColunas = array('tipo', 'inuid', 'intoid', 'intaid');
        $listagemSimec->setCabecalho($arrayCabecalho);

        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addAcao('plus', array('func' => 'detalharProcesso', 'extra-params' => array('tipo')));
        $listagemSimec->addAcao('edit', array('func' => 'vincularObra', 'extra-params' => array('tipo', 'inuid', 'intoid', 'intaid')));
        $listagemSimec->addAcao('remove', array('func' => 'removeProcesso', 'extra-params' => array('tipo')));
        $listagemSimec->addAcao('ContaCorrente', 'solicitarConta');
        $listagemSimec->addCallbackDeCampo('pronumeroprocesso', 'formata_numero_processo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setAcaoComoCondicional('remove', [['campo' => 'vlrempenho', 'valor' => '0', 'op' => 'igual']]);
        $listagemSimec->addCallbackDeCampo(['valor_termo','vlrpago','valor_processo','vlrempenho'], 'par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('porcent_pago','Par3_Controller_Processo::hintPorcentagemPagamento');
        $listagemSimec->setFormFiltros('form-administra-processo');
        $listagemSimec->setTamanhoPagina(100);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function listaComposicao($proid)
    {
        $sql = "SELECT ppc.ppcid, ppc.ppcano, ip.inpid, a.anaid, ini.iniid||' - '||ind.inddsc as descricao,
                	coalesce(u.usunome, '-') as usunome,
                    coalesce(to_char(a.anadatacriacao, 'DD/MM/YYYY'), '-') as dataanalise,
                    sum((ai.aicqtdaprovado * ai.aicvaloraprovado)) as vlr_composicao,
                    'par' as tipo,
                     ve.vlr_empenho as empenhos_ativos,
            	    (select COUNT(1) from par3.termocomposicao tc inner join par3.documentotermovalidacao dtv ON dtv.dotid = tc.dotid where dtv.dtvstatus = 'A' and tc.tecstatus = 'A' AND  ip.inpid =  tc.inpid ) as temos_validados
                FROM par3.processoparcomposicao ppc
                	INNER JOIN par3.analise a ON a.anaid = ppc.anaid AND a.anastatus = 'A'
                	INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.aicstatus = 'A'
                	INNER JOIN par3.iniciativa_planejamento ip ON ip.inpid = a.inpid
                	INNER JOIN par3.iniciativa ini on ini.iniid = ip.iniid
                	INNER JOIN par3.iniciativa_descricao ind on ind.indid = ini.indid
                	INNER JOIN seguranca.usuario u on u.usucpf = a.cpftecnico
                	LEFT JOIN (
                		SELECT ve.anaid, ve.inpid, ve.ppcid, ve.proid, sum(ve.saldo) AS vlr_empenho FROM par3.v_empenho_composicao_iniciativa ve WHERE proid = $proid
                		GROUP BY ve.anaid, ve.inpid, ve.ppcid, ve.proid
                	) ve ON ve.anaid = a.anaid AND ve.inpid = ppc.inpid AND ve.proid = ppc.proid AND ve.ppcid = ppc.ppcid
                WHERE ppc.proid = $proid AND ppc.ppcstatus = 'A'
                GROUP BY a.anaid, ip.inpid, ppc.ppcano, ind.inddsc, u.usunome, a.anadatacriacao, ini.iniid, ppc.ppcid, ve.vlr_empenho";
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Ano', 'Planejamento', 'Análise', 'Iniciativa', 'Técnico', 'Data da Análise', 'Valor');
        $esconderColunas = array('', 'temos_validados', 'empenhos_ativos', 'tipo');
        $listagemSimec->setCabecalho($arrayCabecalho);

        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->addAcao('remove', array('func' => 'desvincularProcesso', 'extra-params' => array('tipo')));
        $listagemSimec->setAcaoComoCondicional('remove', array(array('campo' => 'temos_validados', 'valor' => '0', 'op' => 'igual'), array('campo' => 'empenhos_ativos', 'valor' => '0', 'op' => 'igual')));
        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo('vlr_composicao', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(100);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    public function listaComposicaoObra($proid)
    {
        $sql = "SELECT distinct
                    ppc.proid,
				    ini.iniid||' - '||ind.inddsc as iniciativa,
				    o.obrid||' - '||o.obrdsc as obra,
				    o.obrano,
				    oct.octdsc AS categoria,
				    o.obrvalor,
                    'obra' as tipo,
                    ve.vlr_empenho as empenhos_ativos,
            	    (select COUNT(1) from par3.termocomposicao tc inner join par3.documentotermovalidacao dtv ON dtv.dotid = tc.dotid where dtv.dtvstatus = 'A' and tc.tecstatus = 'A' AND  ip.inpid =  tc.inpid ) as temos_validados
				FROM par3.iniciativa_planejamento ip
				    inner join par3.instrumentounidade iu on iu.inuid = ip.inuid and iu.inustatus = 'A'
				    inner join par3.iniciativa ini on ini.iniid = ip.iniid
				    inner join par3.iniciativa_descricao ind on ind.indid = ini.indid
				    inner join par3.obra o on o.inpid = ip.inpid and o.inuid = iu.inuid and o.obrstatus = 'A'
				    inner join par3.obra_tipo otp ON otp.otpid = o.otpid
				    inner join par3.obra_categoria oct ON oct.octid = otp.octid
				    inner join par3.processoobracomposicao ppc on ppc.obrid = o.obrid and ppc.pocstatus = 'A'
                    LEFT JOIN (                		
				    	SELECT ve.obrid, ve.pocid, ve.proid, sum(ve.saldo) AS vlr_empenho FROM par3.v_empenho_composicao_obra ve WHERE proid = $proid
                		GROUP BY ve.obrid, ve.pocid, ve.proid                		
                	) ve ON ve.obrid = o.obrid AND ve.proid = ppc.proid AND ve.pocid = ppc.pocid
				WHERE ppc.proid = $proid";

        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Iniciativa', 'Obra', 'Ano', 'Categoria', 'Valor');
        $esconderColunas = array('', 'temos_validados', 'empenhos_ativos', 'tipo');

        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->addAcao('remove', array('func' => 'desvincularProcesso', 'extra-params' => array('tipo')));
        $listagemSimec->setAcaoComoCondicional('remove', array(array('campo' => 'temos_validados', 'valor' => '0', 'op' => 'igual'), array('campo' => 'empenhos_ativos', 'valor' => '0', 'op' => 'igual')));
        $listagemSimec->esconderColunas($esconderColunas);

        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo('obrvalor', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->setTamanhoPagina(100);

        $listagemSimec->render();
    }

    public function listaObrasSemProcesso($proid, $inuid, $intoid)
    {
        $sql = "SELECT distinct
					'<input type=\"checkbox\" id=\"check_acao[]\" name=\"check_acao[]\" value=\"'||o.obrid||'\" class=\"check_acao\">' as acao,
				    ini.iniid||' - '||ind.inddsc as iniciativa,
				    o.obrid||' - '||o.obrdsc as obra,
				    o.obrano,
				    oct.octdsc AS categoria,
				    o.obrvalor
				FROM par3.iniciativa_planejamento ip
				    inner join par3.instrumentounidade iu on iu.inuid = ip.inuid and iu.inustatus = 'A'
				    inner join par3.iniciativa ini on ini.iniid = ip.iniid
				    inner join par3.iniciativa_descricao ind on ind.indid = ini.indid
				    inner join par3.obra o on o.inpid = ip.inpid and o.inuid = iu.inuid and o.obrstatus = 'A'
				    inner join par3.obra_tipo otp ON otp.otpid = o.otpid
				    inner join par3.obra_categoria oct ON oct.octid = otp.octid
				    inner join workflow.documento d on d.docid = o.docid
				WHERE ip.inuid = $inuid
					and ini.intoid in ($intoid)
					and d.esdid IN (2050, 2055)
					and o.obrid not in (select obrid from par3.processoobracomposicao where pocstatus = 'A')";

        echo '<div class="ibox">
			    <div class="ibox-title">
			        <div class="row">
			            <div class="col-md-12">
			                <h2 class="center">Lista de Obras sem vinculo ao processo</h2>
			            </div>
			        </div>
			    </div>
		<form name="form-vincula-processo" id="form-vincula-processo" class="form-horizontal" method="post">
			<input type="hidden" name="proid" id="proid" value="'.$proid.'">
			<input type="hidden" name="inuid" id="inuid" value="'.$inuid.'">
			<input type="hidden" name="intoid" id="intoid" value="'.$intoid.'">
			<input type="hidden" name="tipo_processo" id="tipo_processo" value="obra">
			<input type="hidden" name="requisicao" id="requisicao" value="">
		';
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('', 'Iniciativa', 'Obra', 'Ano', 'Categoria', 'Valor');
        $esconderColunas = array();
        $listagemSimec->setCabecalho($arrayCabecalho);

        $listagemSimec->esconderColunas($esconderColunas);
        //$listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo('obrvalor', 'formataNumeroMonetarioComSimbolo');
        //$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(100);

        $listagemSimec->render();
        echo '</form></div>';
    }

    public function listaIniciativaPlanejamentoSemProcesso($proid, $inuid, $intoid, $intaid)
    {

        $sql = "SELECT DISTINCT
				    '<input type=\"checkbox\" id=\"check_acao[]\" name=\"check_acao[]\" value=\"'||inp.inpid||'_'||a.anaano||'_'||obj.intoid||'_'||a.anaid||'\" class=\"check_acao\">' as acao,
                    inp.inpid,
                    a.anaid,
				    (inp.iniid||' - '||ind.inddsc) as ds_iniciativa,
				    obj.intodsc as tipoobjeto,
				    inta.intadsc as tipoatendimento,
				    coalesce(u.usunome, '-') as usunome,
				    coalesce(to_char(a.anadatacriacao, 'DD/MM/YYYY'), '-') as dataanalise,
				    sum((COALESCE(ipc.ipiquantidadeaprovada * ipc.ipivalorreferencia,0))) as valor
				FROM par3.iniciativa_planejamento as inp
				    inner join par3.analise a on a.inpid = inp.inpid and a.anastatus = 'A'
				    inner join par3.v_analise_planejamento_item_composicao ipc on ipc.inpid = inp.inpid and ipc.ipiano = a.anaano and ipc.ipistatus = 'A'
				    inner join par3.instrumentounidade iu on iu.inuid = inp.inuid and iu.inustatus = 'A'
				    INNER JOIN workflow.documento doc ON doc.docid = a.docid
				    INNER JOIN par3.iniciativa as ini ON ini.iniid = inp.iniid
				    INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
				    inner join seguranca.usuario u on u.usucpf = a.cpftecnico
				    INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
				    INNER JOIN par3.iniciativa_tipos_atendimento inta ON inta.intaid = ini.intaid
				WHERE inp.inpstatus = 'A'
				     AND doc.esdid = ".PAR3_ESDID_ANALISE_PLANEJAMENTO_APROVADO."
				     and inp.inuid = $inuid
				     and obj.intoid in ($intoid)
				     and a.intaid = $intaid
				     --and inp.inpid not in (select p.inpid from par3.processoparcomposicao p where p.inpid = inp.inpid and p.ppcano = a.anaano and p.ppcstatus = 'A')
                     AND a.anaid NOT IN (select p.anaid from par3.processoparcomposicao p where p.inpid = inp.inpid and p.ppcano = a.anaano and p.ppcstatus = 'A')
                     AND 0 = (SELECT count(dotid) FROM par3.documentotermo WHERE dotstatus = 'A' and proid = $proid )
				group by inp.inpid, a.anaano, inp.iniid, ind.inddsc, obj.intodsc, inta.intadsc, u.usunome, a.anadatacriacao, a.anaid, obj.intoid";

        echo '<div class="ibox">
			    <div class="ibox-title">
			        <div class="row">
			            <div class="col-md-12">
			                <h2 class="center">Lista de Iniciativas sem Vinculo ao Processo</h2>
			            </div>
			        </div>
			    </div>
		<form name="form-vincula-processo" id="form-vincula-processo" class="form-horizontal" method="post">
			<input type="hidden" name="proid" id="proid" value="'.$proid.'">
			<input type="hidden" name="inuid" id="inuid" value="'.$inuid.'">
			<input type="hidden" name="tipo_processo" id="tipo_processo" value="par">
			<input type="hidden" name="requisicao" id="requisicao" value="">
		';
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('', 'Planejamento', 'Análise', 'Iniciativa', 'Tipo Objeto', 'Tipo Atendimento', 'Técnico', 'Data Análise', 'Valor');
        $esconderColunas = array();
        $listagemSimec->setCabecalho($arrayCabecalho);

        $listagemSimec->esconderColunas($esconderColunas);
        //$listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo('valor', 'formataNumeroMonetarioComSimbolo');
        //$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(100);

        $listagemSimec->render();
        echo '</form></div>';
    }
    
    public function listaIniciativaSemProcesso($arParam)
    {

        $where = '';

        $where .= $arParam['iniciativa'] ? " AND inp.inpid = '{$arParam['iniciativa']}' ":'';

        $where .= $arParam['itrid'] && $arParam['itrid'] != 0 ?
            ($arParam['itrid'] == 1 ? " AND iu.itrid in(1,3)" :"AND iu.itrid = {$arParam['itrid']} "): '';

        if($arParam['muncod'] && count($arParam['muncod']) != 0 ) {
            $arrMuncod = tratarArrayParaMultiSelect($arParam['muncod']);
            if(count($arrMuncod) != 0) {
                $where .= " AND iu.muncod::integer in (".implode(',',$arrMuncod).") ";
            }
        }

        if($arParam['estuf'] && count($arParam['estuf']) != 0 ) {
            $arrEstuf = implode(',',array_map(function($estuf){return (string)"'".$estuf."'";},$arParam['estuf']));
            $where .= $arParam['estuf'] ? " AND  iu.estuf in ({$arrEstuf}) ":'';
        }
        
        if ($arParam['iniano']) {
            $where .= " AND ipc.ipiano = {$arParam['iniano']} ";
        }
        
        $sql = "SELECT DISTINCT
				    '<input type=\"checkbox\" id=\"check_acao[]\" name=\"check_acao['||inp.inpid||'][]\" value=\"'||a.anaano||'\" class=\"check_acao\">
						<input type=\"hidden\" name=\"inuid\" id=\"inuid\" value=\"'||inp.inuid||'\">
						<input type=\"hidden\" name=\"anaid['||inp.inpid||']\" id=\"anaid\" value=\"'||a.anaid||'\">
				    	<input type=\"hidden\" name=\"tipo_processo\" id=\"tipo_processo\" value=\"par\">
						' as acao,
				    inp.inpid,
				    a.anaano,
				    iu.inuid,
				    case when iu.muncod is null then est.estuf else mun.estuf end as uf,
				    case when iu.muncod is null then est.estdescricao else mun.mundescricao end as entidade,
				    (inp.iniid||' - '||ind.inddsc) as ds_iniciativa,
				    (SELECT array_to_string(array(
				        SELECT DISTINCT
				        prg.prgdsc
				        FROM par3.iniciativa_iniciativas_programas iiprg
				        INNER JOIN par3.programa prg ON prg.prgid = iiprg.prgid
				        WHERE iniid = ini.iniid
				    ),',')
				    ) as programas,
				    obj.intodsc as tipoobjeto,

				    coalesce(u.usunome, '-') as usunome,
				    case  when a.intaid = 1
					then 'PAR'
					else 'Emendas'
				end as Tipo_assistencia,
				    coalesce(to_char(a.anadatacriacao, 'DD/MM/YYYY'), '-') as dataanalise,
				    va.vlr_aprovado as valor
				FROM par3.iniciativa_planejamento as inp
				    inner join par3.analise a on a.inpid = inp.inpid and a.anastatus = 'A'
				    inner join par3.v_analise_planejamento_item_composicao ipc on ipc.inpid = inp.inpid and ipc.ipiano = a.anaano and ipc.ipistatus = 'A'
				    inner join par3.instrumentounidade iu on iu.inuid = inp.inuid and iu.inustatus = 'A'
				    INNER JOIN workflow.documento doc ON doc.docid = a.docid
				    INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
				    INNER JOIN par3.iniciativa as ini ON ini.iniid = inp.iniid
				    INNER JOIN par3.iniciativa_descricao as ind ON ind.indid = ini.indid
					inner JOIN par3.iniciativa_tipos_assistencia it ON it.intaid = a.intaid
					INNER JOIN(
								SELECT anaid, sum(vlr_aprovado) AS vlr_aprovado FROM(
									SELECT ai.anaid, (ai.aicqtdaprovado * ai.aicvaloraprovado) AS vlr_aprovado FROM par3.analise_itemcomposicao ai WHERE ai.aicstatus = 'A' AND ai.aicqtdaprovado > 0
								) AS foo GROUP BY anaid
							) va ON va.anaid = a.anaid
				    inner join seguranca.usuario u on u.usucpf = a.cpftecnico
				    INNER JOIN par3.modalidade   mod ON mod.modid = inp.modid
				    INNER JOIN par3.iniciativa_tipos_objeto      obj  ON obj.intoid  = ini.intoid
				    INNER JOIN par3.iniciativa_tipos_atendimento inta ON inta.intaid = ini.intaid
				    LEFT JOIN territorios.municipio mun on mun.muncod = iu.muncod
				    LEFT JOIN territorios.estado est on est.estuf = iu.estuf
				WHERE inp.inpstatus = 'A'
				     AND esd.esdid = ".PAR3_ESDID_ANALISE_PLANEJAMENTO_APROVADO."
				     and a.anaid not in (select p.anaid from par3.processoparcomposicao p where p.inpid = inp.inpid and p.ppcano = a.anaano and p.ppcstatus = 'A')
				     $where
				group by
					inp.inpid, inp.iniid, ini.iniid, a.anaano, iu.inuid, iu.muncod, est.estuf, mun.estuf, est.estdescricao, mun.mundescricao, ind.inddsc,
				    obj.intodsc, mod.moddsc, inta.intadsc, u.usunome, a.anadatacriacao, a.anaid, va.vlr_aprovado,a.intaid
				ORDER BY inp.inpid DESC";
        
        echo '<form name="form-gerar-processo" id="form-gerar-processo" class="form-horizontal" method="post">
            	<input type="hidden" name="requisicao_processo" id="requisicao_processo" value="">
            	<input type="hidden" name="ws_usuario" id="ws_usuario" value="">
            	<input type="hidden" name="ws_senha" id="ws_senha" value="">
            	<input type="hidden" name="ungid" id="ungid" value="">';
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Ação', 'Planejamento','Ano','UF', 'Entidade', 'Iniciativa', 'Programas', 'Tipo Objeto', 'Técnico', 'Tipo de assitência', 'Data da Análise', 'Valor');
        $esconderColunas = array('iniid', 'inuid', 'modalidade', 'tipoatendimento');
        $listagemSimec->setCabecalho($arrayCabecalho);

        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo('valor', 'par3_mascaraMoeda');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(100);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        echo '</form>';
    }
    
    public function listaIniciativaObrasSemProcesso($arParam)
    {
        $where = '';

        $where .= $arParam['itrid'] && $arParam['itrid'] != 0 ?
            ($arParam['itrid'] == 1 ? " AND inu.itrid in(1,3)" :" AND inu.itrid = {$arParam['itrid']} "): '';

        if($arParam['muncod'] && count($arParam['muncod']) != 0 ){
            $arrMuncod = tratarArrayParaMultiSelect($arParam['muncod']);
            if(count($arParam) != 0) {
                $where .= " AND inu.muncod::integer in (".implode(',',$arrMuncod).") ";
            }
        }

        if($arParam['estuf'] && count($arParam['estuf']) != 0 ) {
            $arrEstuf = implode(',',array_map(function($estuf){return (string)"'".$estuf."'";},$arParam['estuf']));
            $where.= $arParam['estuf'] ? " AND  inu.estuf in ({$arrEstuf}) ":'';
        }

        $where  .= $arParam['iniciativa'] ? " AND obr.obrid = '{$arParam['iniciativa']}' ":'';

        if ($arParam['iniano']) {
            $where  .= " AND obr.obrano = {$arParam['iniano']}";
        }
        
        $sql = "SELECT DISTINCT
					'<input type=\"checkbox\" id=\"check_acao[]\" name=\"check_acao['||inp.inpid||'][]\" value=\"'||obr.obrid||'\" class=\"check_acao\">
						<input type=\"hidden\" name=\"iniano['||inp.inpid||']['||obr.obrid||']\" id=\"iniano[]\" value=\"'||obr.obrano||'\">
						<input type=\"hidden\" name=\"inuid\" id=\"inuid\" value=\"'||inp.inuid||'\">
				    	<input type=\"hidden\" name=\"tipo_processo\" id=\"tipo_processo\" value=\"obra\">
						' as acao,
				    inp.inpid,
				    obr.obrid,
				    obr.obrdsc,
				    inu.estuf AS uf,
				    CASE inu.itrid WHEN 1 THEN '-' ELSE inu.inudescricao END AS municipio,
				    inp.iniid||' - '||ind.inddsc AS iniciativa,
				    est.esddsc AS situacao,
				    obr.obrano AS ano,
				    oct.octdsc AS categoria,
				    CASE
				        WHEN otp.otpid IN (1, 2, 19, 20, 21, 23) THEN 'Creche'
				        WHEN otp.otpid IN (13, 14, 25) THEN 'Cobertura'
				        WHEN otp.otpid IN (4, 5, 6, 7, 8, 9, 10, 11, 15, 16, 17, 18, 22) THEN 'Escola'
				        WHEN otp.otpid IN (12, 24) THEN 'Quadra'
				        ELSE '-'
				        END AS local,
				    COALESCE(obr.obrvalor, 0) AS valor_planejado,
				    (SELECT COALESCE(SUM(valor_total_empenhado), 0)
				        FROM par3.empenho emp
				        JOIN par3.empenhoobracomposicao eoc ON eoc.empid = emp.empid AND eoc.eocstatus = 'A'
				        JOIN par3.processoobracomposicao poc ON poc.obrid = obr.obrid AND poc.pocid = eoc.pocid AND poc.pocstatus = 'A'
				        ) AS valor_empenhado
				FROM par3.iniciativa_planejamento AS inp
				    JOIN par3.iniciativa AS ini ON ini.iniid = inp.iniid AND ini.intoid IN(1)
				    JOIN par3.dimensao dim ON inp.dimid = dim.dimid
				    JOIN par3.iniciativa_descricao AS ind ON ind.indid = ini.indid
				    JOIN par3.instrumentounidade inu ON inu.inuid = inp.inuid
				        JOIN par3.obra obr ON obr.inpid = inp.inpid
				    JOIN par3.obra_tipo otp ON otp.otpid = obr.otpid
				    JOIN par3.obra_categoria oct ON oct.octid = otp.octid
				    JOIN workflow.documento doc ON doc.docid = obr.docid AND doc.esdid IN(2050, 2055)
				    JOIN workflow.estadodocumento est ON doc.esdid=est.esdid
				    LEFT JOIN territorios.municipio mun on mun.muncod = inu.muncod
				    LEFT JOIN territorios.estado et on et.estuf = inu.estuf
				WHERE inpstatus = 'A'
					$where
					and obr.obrid not in (select obrid from par3.processoobracomposicao where pocstatus = 'A')
				ORDER BY uf, municipio";
        
        //AND doc.esdid = ".PAR3_ESDID_ANALISE_PLANEJAMENTO_APROVADO.""
        
        echo '<form name="form-gerar-processo" id="form-gerar-processo" class="form-horizontal" method="post">
            	<input type="hidden" name="requisicao_processo" id="requisicao_processo" value="">
            	<input type="hidden" name="ws_usuario" id="ws_usuario" value="">
            	<input type="hidden" name="ws_senha" id="ws_senha" value="">';
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array( 'Ação', 'Obrid', 'Obra', 'UF', 'Entidade', 'Iniciativa', 'Ano', 'Categoria', 'Local', 'Valor Planejado', 'Valor Empenhado');
        $esconderColunas = array('inpid', 'situacao');
        $listagemSimec->setCabecalho($arrayCabecalho);

        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo('valor_planejado', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->addCallbackDeCampo('valor_empenhado', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(100);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        
        echo '</form>';
    }
    
    public function vinculaObraProcessoExistente($arrPost = array())
    {
        foreach ($arrPost['check_acao'] as $codigo) {
            $sql = "INSERT INTO par3.processoobracomposicao(proid, obrid, intoid, pocstatus) VALUES ({$arrPost['proid']}, $codigo, 1, 'A');";
            $this->model->executar($sql);
        }
            
        if ($this->model->commit()) {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'success', 'Processo vinculado com sucesso');
        } else {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Erro ao vincular o processo');
        }
    }
    
    public function pegaTipoProcesso( $proid )
    {
        $sql = "SELECT 'par' AS tipo FROM par3.processoparcomposicao WHERE proid = $proid and ppcstatus = 'A'
                UNION ALL
                SELECT 'obra' AS tipo FROM par3.processoobracomposicao WHERE proid = $proid and pocstatus = 'A'";
        return $this->model->pegaUm($sql);
    }
    
    public function verificaProcessoEmenda( $proid )
    {
        $boEmenda = $this->model->pegaUm("SELECT intaid FROM par3.processo WHERE proid = {$proid}");
        if($boEmenda == '2'){
            return 'S';
        } else {
            return 'N';
        }
    }
    
    public function vinculaIniciativaPlanejadaProcessoExistente($arrPost = array())
    {
        
        foreach ($arrPost['check_acao'] as $codigo) {
            $arrDados = explode('_', $codigo);
            $inpid = $arrDados[0];
            $ano = $arrDados[1];
            $intoid = $arrDados[2];
            $anaid = $arrDados[3];
            
            $sql = "INSERT INTO par3.processoparcomposicao(proid, inpid, ppcano, intoid, anaid, ppcstatus)
					VALUES ({$arrPost['proid']}, {$inpid}, {$ano}, $intoid, $anaid, 'A')";
            
            $this->model->executar($sql);
        }
            
        if ($this->model->commit()) {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'success', 'Processo vinculado com sucesso');
        } else {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Erro ao vincular o processo');
        }
    }
    public function carregaDadosEntidadeProcesso($proid)
    {
        $sql = "select
					formata_num_processo(p.pronumeroprocesso) as processo,
				    case when iu.muncod is null then e.estuf else m.estuf end as uf,
				    case when iu.muncod is null then e.estdescricao else m.mundescricao end as entidade,
				    (case when iu.itrid = 2 then 'M' else 'E' end) as esfera,
		            (case when (select count(ppcid) from par3.processoparcomposicao s where proid = p.proid and ppcstatus = 'A') > 0 then 'P' else 'O' end) as tipo_processo,
		            p.inuid, dt.dotid, p.intaid
				from par3.processo p
					inner join par3.instrumentounidade iu on iu.inuid = p.inuid and iu.inustatus = 'A'
				    left join territorios.municipio m on m.muncod = iu.muncod
				    left join territorios.estado e on e.estuf = iu.estuf
				    LEFT JOIN par3.documentotermo dt ON dt.proid = p.proid AND dt.dotstatus = 'A'
				where p.proid = {$proid}";
        
        return $this->model->pegaLinha($sql);
    }
    
    public function pegaTipoObjetoPorProcesso($proid)
    {
        $sql = "SELECT distinct obj.intoid, obj.intodsc
				FROM par3.processoparcomposicao pp
					inner join par3.iniciativa_planejamento ip on ip.inpid = pp.inpid
				    inner join par3.iniciativa i on i.iniid = ip.iniid
					inner join par3.iniciativa_tipos_objeto obj ON obj.intoid = i.intoid
				WHERE pp.proid = {$proid}";
        
        return $this->model->pegaLinha($sql);
    }
    
    public function carregaDadosOrcamentario($inuid, $inpid, $anaid = '', $proid = '')
    {
        $filtro .= "";
        $filtroP .= "";
        if (!empty($anaid)) {
            $filtro .= " and pp.anaid = $anaid ";
        }
        
        if( !empty($proid) ){
            $filtroP .= " and p.proid = $proid ";
        } else {
            $filtro .= " and pp.inpid = $inpid ";
        }
        
        if( !empty($filtro) ){
            $join = "INNER JOIN(
						SELECT pp.inpid, pp.proid, pp.anaid FROM par3.processoparcomposicao pp WHERE pp.ppcstatus = 'A'
						UNION ALL
						SELECT o.inpid, pp.proid,null AS anaid FROM par3.processoobracomposicao pp
							INNER JOIN par3.obra o ON o.obrid = pp.obrid AND o.obrstatus = 'A'
						WHERE pp.pocstatus = 'A'
					) pp ON pp.proid = p.proid $filtro";
        }
        
        $sql = "select distinct p.processo,
					p.probanco,
				    p.proagencia,
				    p.nu_conta_corrente,
				    p.numero_termo,
				    to_char(p.data_vigencia, 'DD/MM/YYYY') as data_vigencia,
				    p.saldo_processo,
					p.vlrempenho,
				    p.valor_termo,
				    p.vlrpago
				from par3.v_dados_processo p
					$join
				where p.inuid = $inuid $filtroP";

        $boDados = $this->model->pegaLinha($sql);
        if (is_array($boDados)) {
            $listagemSimec = new Simec_Listagem();
            $esconderColunas = array();
            $arrayCabecalho = array('Processo', 'Banco', 'Agência', 'Conta Corrente', 'Nº do Termo', 'Data Vigência', 'Saldo Conta', 'Valor Empenhado', 'Valor Termo', 'Valor Pago');
            $listagemSimec->setCabecalho($arrayCabecalho);
            
            $listagemSimec->esconderColunas($esconderColunas);
            $listagemSimec->setQuery($sql);
            $listagemSimec->addCallbackDeCampo('saldo_processo', 'formataNumeroMonetarioComSimbolo');
            $listagemSimec->addCallbackDeCampo('vlrempenho', 'formataNumeroMonetarioComSimbolo');
            $listagemSimec->addCallbackDeCampo('valor_termo', 'formataNumeroMonetarioComSimbolo');
            $listagemSimec->addCallbackDeCampo('vlrpago', 'formataNumeroMonetarioComSimbolo');
            $listagemSimec->addCallbackDeCampo('processo', 'formata_numero_processo');
            //$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagemSimec->setTamanhoPagina(50);
            $listagemSimec->setCampos($arrayCabecalho);
            
            $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }
    }

    /**
     * Função que lista os processo
     * @param array $arrFiltros
     * @return array
     */
    public function listarProcessosJSON($arrFiltros = [])
    {
        $sql =$this->model->listarProcessos($arrFiltros);
        return $this->model->carregar($sql);
    }

    /**
     * Função que persiste a edição do processo
     * @param array $arrDados
     * @return bool
     * @throws Exception
     */
    public function editarProcesso($arrDados = [])
    {
        $sql =$this->model->editarProcesso($arrDados);
        $this->model->executar($sql);
        $this->model->commit();
        $this->salvarHistorico(Par3_Model_Processo::class, Par3_Model_ProcessoHistorico::class, $arrDados['proid']);
        return true;
    }

    /**
     * Função que lista composição Par ou Obra, de acordo com o parâmetro informado
     * @param array $arrFiltros
     * @return array
     */
    public function listarComposicao($arrFiltros = [])
    {
        $sql =$this->model->listarComposicaoPar($arrFiltros);
        if ($arrFiltros['tipo']) {
            if ($arrFiltros['tipo'] == 'O') {
                $sql =$this->model->listarComposicaoObra($arrFiltros);
            }
        }

        return $this->model->carregar($sql);
    }

    /**
     * Função que monta a grid interna do processo
     * @param $ArrPro
     */
    public function montarListaInterna($ArrPro)
    {
        require APPRAIZ.'par3/modulos/sistema/tabelaapoio/errows/composicaoProcesso.inc';
    }

    /**
     * Função que lista a analise Par ou Obra do processo, de acordo com o parâmetro informado
     * @param array $arrFiltros
     * @return array
     */
    public function listarAnalise($arrFiltros = [])
    {
        $sql =$this->model->listarAnalisePar($arrFiltros);
        if ($arrFiltros['tipo']) {
            if ($arrFiltros['tipo'] == 'O') {
                $sql =$this->model->listarAnaliseObra($arrFiltros);
            }
        }

        return $this->model->carregar($sql);
    }

    /**
     * Função que lista documentos do processo
     * @param array $arrFiltros
     * @return array
     */
    public function listarDocumento($arrFiltros = [])
    {
        $modelDocumento = new Par3_Model_DocumentoTermo();
        $sql = $modelDocumento->listarDocumentosProcesso($arrFiltros);

        return $modelDocumento->carregar($sql);
    }

    /**
     * Função que lista o empenho
     * @param array $arrFiltros
     * @return array
     */
    public function listarEmpenho($arrFiltros = [])
    {
        $objProcesso = $this->obterProcesso($arrFiltros['proid']);
        $sql =$this->model->listarEmpenho($objProcesso->pronumeroprocesso);

        return $this->model->carregar($sql);
    }

    /**
     * Função que lista o empenho Par ou Obra de acordo com o parâmetro informado
     * @param array $arrFiltros
     * @return array
     */
    public function listarEmpenhoComposicao($arrFiltros = [])
    {
        $objProcesso = $this->obterProcesso($arrFiltros['proid']);

        $sql =$this->model->listarEmpenhoPar($objProcesso->pronumeroprocesso);
        if ($arrFiltros['tipo']) {
            if ($arrFiltros['tipo'] == 'O') {
                $sql =$this->model->listarEmpenhoObra($objProcesso->pronumeroprocesso);
            }
        }

        return $this->model->carregar($sql);
    }

    /**
     * Função que lista o pagamento
     * @param array $arrFiltros
     * @return array
     */
    public function listarPagamento($arrFiltros = [])
    {
        $objProcesso = $this->obterProcesso($arrFiltros['proid']);
        $sql =$this->model->listarPagamento($objProcesso->pronumeroprocesso);
        return $this->model->carregar($sql);
    }

    /**
     * Função que lista pagamento composição PAR ou Obra de acordo com o parâmetro informado
     * @param array $arrFiltros
     * @return array
     */
    public function listarPagamentoComposicao($arrFiltros = [])
    {
        $objProcesso = $this->obterProcesso($arrFiltros['proid']);
        $sqlPagamentoComposicao = $this->model->listarPagamentoParComposicao($objProcesso->pronumeroprocesso);
        if ($arrFiltros['proid']) {
            if ($arrFiltros['tipo'] == 'O') {
                $sqlPagamentoComposicao =$this->model->listarPagamentoObraComposicao($objProcesso->pronumeroprocesso);
            }
        }

        return $this->model->carregar($sqlPagamentoComposicao);
    }


    /**
     * Função que busca o processo de acordo com o id(proid) informado
     * @param $id
     * @return Par3_Model_Processo
     */
    private function obterProcesso($id)
    {
        return $this->model->carregarPorId($id);
    }

    /**
     * Função que persiste a edição do ComposicaoPar
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarComposicaoPar($arrData)
    {
        $sql =$this->model->editarComposicaoPar($arrData);
        $this->model->executar($sql);
        return $this->model->commit();
    }

    /**
     * Função que persiste a edição do Composição Obra
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarComposicaoObra($arrData)
    {
        $sql =$this->model->editarComposicaoObra($arrData);
        $this->model->executar($sql);
        $this->salvarHistorico(Par3_Model_Obra::class, Par3_Model_ObraHistorico::class, $arrData['obrid']);
        return $this->model->commit();
    }

    /**
     * Função que persiste a edição do Analise Par
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarAnalisePar($arrData)
    {
        $sql =$this->model->editarAnalisePar($arrData);
        $this->model->executar($sql);
        $this->model->commit();
        $this->salvarHistorico(Par3_Model_AnalisePlanejamentoIniciativa::class, Par3_Model_AnalisePlanejamentoIniciativaHistorico::class, $arrData['anaid']);
        return true;
    }

    /**
     * Função que persiste a edição do Analise Obra
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarAnaliseObra($arrData)
    {
        $sql =$this->model->editarAnaliseObra($arrData);
        $this->model->executar($sql);
        $this->model->commit();
        $this->salvarHistorico(Par3_Model_Obra::class, Par3_Model_ObraHistorico::class, $arrData['obrid']);

        return true;
    }

    /**
     * Função que persiste a edição do documento
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarDocumento($arrData)
    {
        $arrData = array_map(function($data) {
            return empty($data)?null:$data;
        },$arrData);
        $sql =$this->model->editarDocumento($arrData);
        $this->model->executar($sql);
        $this->model->commit();
        $this->salvarHistorico(Par3_Model_DocumentoTermo::class, Par3_Model_DocumentoTermoHistorico::class, $arrData['dotid']);
        return true;
    }

    /**
     * Função que persiste a edição do empenho
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarEmpenho($arrData)
    {
        $sql =$this->model->editarEmpenho($arrData);
        $this->model->executar($sql);
        $this->model->commit();
        $this->salvarHistorico(Par3_Model_Empenho::class, Par3_Model_EmpenhoHistorico::class, $arrData['empid']);
        return true;
    }

    /**
     * Função que persiste a edição do Empenho Par
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarEmpenhoPar($arrData)
    {
        $sql =$this->model->editarEmpenhoPar($arrData);
        $this->model->executar($sql);
        $this->model->commit();
        $this->salvarHistorico(Par3_Model_EmpenhoParComposicao::class, Par3_Model_EmpenhoParComposicaoHistorico::class, $arrData['epcid']);
        return true;
    }

    /**
     * Função que persiste a edição do Empenho Obra
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarEmpenhoObra($arrData)
    {
        $sql =$this->model->editarEmpenhoObra($arrData);
        $this->model->executar($sql);
        $this->model->commit();
        $this->salvarHistorico(Par3_Model_EmpenhoObraComposicao::class, Par3_Model_EmpenhoObraComposicaoHistorico::class, $arrData['eocid']);
        return true;
    }

    /**
     * Função que persiste a edição do Pagamento
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarPagamento($arrData)
    {
        $sql =$this->model->editarPagamento($arrData);
        $this->model->executar($sql);
        $this->model->commit();
        $this->salvarHistorico(Par3_Model_Pagamento::class, Par3_Model_PagamentoHistorico::class, $arrData['pagid']);
        return true;
    }

    /**
     * Função que persiste a edição do Pagamento Par
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarPagamentoComposicaoPar($arrData)
    {
        $sql =$this->model->editarPagamentoPar($arrData);
        $this->model->executar($sql);
        $this->model->commit();
        $this->salvarHistorico(Par3_Model_PagamentoParComposicao::class, Par3_Model_PagamentoParComposicaoHistorico::class, $arrData['ppcid']);
        return true;
    }

    /**
     * Função que persiste a edição do Pagamento Obra
     * @param $arrData
     * @return bool
     * @throws Exception
     */
    public function editarPagamentoComposicaoObra($arrData)
    {
        $sql =$this->model->editarPagamentoObra($arrData);
        $this->model->executar($sql);
        $this->model->commit();
        $this->salvarHistorico(Par3_Model_PagamentoObraComposicao::class, Par3_Model_PagamentoObraComposicaoHistorico::class, $arrData['pmcid']);
        return true;
    }


    /**
     * Salva o histótico na tabela de referência conforme passado por parâmetro
     * @param $modelOrigem
     * @param $modelHistorico
     * @param $idModel
     * @return bool
     */
    private function salvarHistorico($modelOrigem, $modelHistorico, $idModel)
    {

        $objModelOrigem = new $modelOrigem($idModel);
        $objModelOrigem = $objModelOrigem->carregarPorId($idModel);
        $mHistorico = new $modelHistorico();
        $mHistorico->gravarHistorico($objModelOrigem, $modelHistorico::UPDATE);
        return true;
    }
}
