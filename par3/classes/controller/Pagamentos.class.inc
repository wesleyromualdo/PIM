<?php
class Par3_Controller_Pagamentos extends Controle
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_Pagamentos();
    }

    public function listar($arrPost = array())
    {

        /*if ($arrPost['tipo_processo'] == 'O') {
            $sql = $this->model->listarDocObras($arrPost);
        } else {
        }*/
        $sql = $this->model->listarDadosPagamento($arrPost);

        if ($_REQUEST['requisicao'] == 'excel') {
            return $sql;
        } elseif ($_REQUEST['requisicao'] == 'imprimir-listapagamento') {
            $arrayCabecalho = array('Processo','UF', 'Entidade', 'Tipo de Objeto', 'Valor Processo', 'Valor Empenho', 'Valor Pagamento','% Pago <br>Empenho', '% Pago <br> Processo', 'Assistência', 'Situação', 'Bloqueio PAR');
            $esconderColunas = array('tipo', 'inuid', 'proid', 'id');
            
            $listagemSimec = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagemSimec->setCabecalho($arrayCabecalho);
            $listagemSimec->esconderColunas($esconderColunas);
            $listagemSimec->setQuery($sql);
            $listagemSimec->setId('listagemPagamentos');
            $listagemSimec->addCallbackDeCampo(['vlrempenho', 'valor_processo', 'vlrpagamento'], 'formataNumeroMonetarioSemSimbolo');
            $listagemSimec->addCallbackDeCampo(['inuid_pendencia'], 'popOverPendenciarsPar');
            $listagemSimec->addCallbackDeCampo('porcentagem','hintPorcentagemPagamento');
            $listagemSimec->addCallbackDeCampo('processo','formata_numero_processo_component');
            $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            
            $listagemSimec->render();
        } else {
            $listagemSimec = new Simec_Listagem();
            $arrayCabecalho = array('Processo','UF', 'Entidade', 'Tipo de Objeto', 'Valor Processo', 'Valor Empenho', 'Valor Pagamento','% Pago <br>Empenho', '% Pago <br> Processo', 'Assistência', 'Situação', 'Saldo % de deferimento', 'Bloqueio PAR');
            $esconderColunas = array('tipo', 'inuid', 'id');
            $listagemSimec->setCabecalho($arrayCabecalho);
    
            $listagemSimec->esconderColunas($esconderColunas);
            $listagemSimec->turnOnPesquisator();
            $listagemSimec->setQuery($sql);
            $listagemSimec->esconderColunas('deferimento');
            $listagemSimec->setFormFiltros('form-pesquisa-pagamento');
            $listagemSimec->setId('listagemPagamentos');
            $listagemSimec->addAcao('plusoutline', array('func' => 'detalharPagamento', 'extra-params' => array('tipo')));
            $listagemSimec->addAcao('pagamentosuccess', array('func' => 'solicitarPagamento', 'extra-params' => array('tipo'), 'cor' => 'warning'));
            $listagemSimec->addAcao('gearoutline', array('func' => 'atualizaPendencias', 'titulo' => 'Pendências', 'extra-params' => array('tipo', 'inuid'), 'cor' => 'warning'));
            $listagemSimec->addCallbackDeCampo(['vlrempenho', 'valor_processo', 'vlrpagamento'], 'formataNumeroMonetarioSemSimbolo');
            $listagemSimec->addCallbackDeCampo(['inuid_pendencia'], 'popOverPendenciarsPar');
            $listagemSimec->addCallbackDeCampo('porcentagem','hintPorcentagemPagamento');
            $listagemSimec->addCallbackDeCampo('processo','formata_numero_processo_component');
            $listagemSimec->addCallbackDeCampo('saldo_desembolso','Par3_Controller_Pagamentos::formata_coluna_desembolso');
            $listagemSimec->addCallbackDeCampo('tipo_obrjeto','carregaHistoricoObra');
            $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagemSimec->setTamanhoPagina(50);
            $listagemSimec->setCampos($arrayCabecalho);
            
            $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }
    }

    public function carregarDadosEmpenho($arrPost = array())
    {
        global $db;
        if ($arrPost['tipo'] == 'par') {
            $tabela = 'v_saldo_empenho_composicao_iniciativa';
        }
        else if ($arrPost['tipo'] == 'obra') {
            $tabela = 'v_saldo_empenho_por_obra';
        }else {
            echo "<div class='alert alert-info' align='center'>Nenhum registro encontrado</div>";die;
        }
        
        $boErro = $arrPost['boErro'];
        
        $funcao = "selecionarEmpenhoPagamento('||ve.empid||');";
        
        $sql = "SELECT distinct
					ve.empid,
                    ve.empid as radio_empid,
				    ve.empenho_original,
				    ve.empfonterecurso,
				    sum(ve.saldo) as vlrempenho,
				    coalesce(sum(ve.vlr_solicitado),0) as vlr_solicitado,
				    coalesce(sum(ve.vlr_efetivado),0) as vlr_efetivado,
				    ve.empsituacao,
				    ve.empprotocolo,
				    formata_cpf_cnpj(ve.entcnpj) as cnpj,
				    ve.entnome
				FROM par3.$tabela ve
				WHERE
					ve.proid = {$arrPost['proid']}
					and ve.saldo > 0
                GROUP BY ve.empid, ve.empenho_original, ve.empfonterecurso, ve.empsituacao, ve.empprotocolo,
				    ve.entcnpj, ve.entnome";
        
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('','Nº Empenho','Fonte de Recurso', 'Valor Empenhado', 'Pagamento Solicitado', 'Pagamento Efetivado', 'Situação do Empenho', 'Protocolo', 'CNPJ', 'Entidade');
        $esconderColunas = array();
        $listagemSimec->setCabecalho($arrayCabecalho);
        
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['vlrempenho', 'vlr_solicitado', 'vlr_efetivado']);
        $listagemSimec->addCallbackDeCampo(['vlrempenho', 'vlr_solicitado', 'vlr_efetivado'], 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addAcao('plusoutline',array('func' => 'historicoPagamento', 'extra-params' => array('empid')));
        $listagemSimec->addCallbackDeCampo('radio_empid','radioEventoEmpenho');
        $listagemSimec->setTamanhoPagina(500);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function pegaValorJaPago($proid, $tipo)
    {
        global $db;
        if ($tipo == 'par') {
            $tabela = 'v_empenho_composicao_iniciativa';
        }
        else if ($tipo == 'obra') {
            $tabela = 'v_empenho_composicao_obra';
        }
        
        $sql = "SELECT sum(ve.vlrpago)
				FROM par3.$tabela ve
				WHERE
					ve.proid = {$proid}
					and ve.saldo > 0";
        return $this->model->pegaUm($sql);
    }
    
    public function verificaPendenciaPagamento($proid)
    {
        
        $arrErros = array();
        $arrErros = $this->verificaPendenciaTermo($proid);
        
        /*$proseqconta = $this->model->pegaUm("SELECT proseqconta FROM par3.processo WHERE proid = $proid");
        if( empty($proseqconta) ){
            $strErros = '- Não existe conta corrente solicitada para esse processo. Deseja solicitar? <a href="par3.php?modulo=principal/orcamento/administrarProcesso&acao=A&proid='.$proid.'"><b>Clique aqui!</b></a>';
            array_push($arrErros, $strErros);
        }*/
        
        
        return $arrErros;
    }
    
    public function verificaPendenciaTermo($proid)
    {
        $obProcesso = new Par3_Controller_Processo();
        $arrEntidade = $obProcesso->carregaDadosEntidadeProcesso($proid);
        
        $arrTermo = $this->model->pegaLinha("select distinct p.inuid, dp.termo, dp.numtermo, dp.mdoqtdvalidacao, dp.validado
											from par3.processo p
												LEFT JOIN(
													SELECT md.mdonome||'_'||ito.intodsc as termo, dp.mdoid, (dp.dotnumero||'-'|| dp.intoid)  as numtermo,
											    		md.mdoqtdvalidacao, dp.proid,
											    		(select count(d.dtvid) from par3.documentotermovalidacao d where d.dotid = dp.dotid and d.dtvstatus = 'A') as validado
													FROM par3.documentotermo dp
											    		inner join par3.modelodocumento md on md.mdoid = dp.mdoid and md.tpdcod in (21, 102, 103) and md.mdostatus = 'A'
											    		inner join par3.iniciativa_tipos_objeto ito on ito.intoid = dp.intoid
											    	WHERE dp.dotstatus = 'A'
												) dp ON dp.proid = p.proid
											where
											    p.proid = $proid
											    AND p.prostatus = 'A'");
        
        $arrErros = array();
        if (empty($arrTermo['termo'])) {
            $arrErros[] = '- Não existe termo de compromisso gerado para esse processo. Deseja gerar o Termo? <a href="par3.php?modulo=principal/orcamento/gerarTermo&acao=A&proid='.$proid.'"><b>Clique aqui!</b></a>';
        }
        if ($arrTermo['validado'] < $arrTermo['mdoqtdvalidacao']) {
            $arrErros[] = '- O termo de compromisso('.$arrTermo['numtermo'].') para esse processo não está validado. <a href="par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid='.$arrTermo['inuid'].'"><b>Clique aqui!</b></a>';
        }
                        
        return $arrErros;
    }
    
    public function carregarDadosHistoricoPagamento($empid, $pagina = '')
    {

        $sql = "SELECT
				    p.pagid,
				    e.empnumeroprocesso,
				    p.pagparcela,
				    p.pagmes,
				    p.paganoparcela,
				    p.pagnumseqob,
				    p.pagnumeroob,
                    e.empnumero as ne,
				    p.pagvalorparcela,
                    to_char(p.pagdatapagamento,'DD/MM/YYYY HH24:MI:SS'),
				    u.usucpf || ' - ' || u.usunome as usunome,
				    COALESCE(p.pagsituacaopagamento,'-') as situacao,
				    p.paganoexercicio,
				    l.lgwmsgretorno as lgwmsgretorno
				FROM
				    par3.pagamento p
				    INNER JOIN par3.empenho e ON e.empid = p.empid and empstatus = 'A'
				    LEFT JOIN seguranca.usuario u ON u.usucpf = p.usucpf
				    left join par3.logwssigef l on l.pagid = p.pagid and l.lgwdata in 
                        (select max(h1.lgwdata) as data from par3.logwssigef h1 where h1.pagid = p.pagid)
				WHERE
				    p.empid in ($empid)
				    and pagstatus='A'
				    and p.pagsituacaopagamento not ilike '%VALA CENTRO DE GESTÃO%'
				ORDER BY
				    pagparcela";
        
        $listagemSimec = new Simec_Listagem();
//        $listagemSimec->turnOnPesquisator();
        $arrayCabecalho = array('Parcela', 'Mês', 'Ano', 'Seq OB', 'Nº OB', 'NE', 'Valor(R$)', 'Data','Usuário', 'Situação pagamento', 'Log Retorno/Erro SIGEF');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $esconderColunas = array('pagid', 'empid', 'empnumeroprocesso', 'paganoexercicio');
        if ($pagina != 'lista') {
            $esconderColunas = array('empid', 'empnumeroprocesso', 'paganoexercicio');
            $listagemSimec->addAcao('view', 'visualizarDadosPagamento');
            $listagemSimec->addAcao('excluir', array('func' => 'cancelarPagamento', 'extra-params' => array('pagnumseqob'), 'titulo' => 'Cancelar Pagamento'));
            $listagemSimec->addAcao('refresh', array('func' => 'concultarPagamento', 'extra-params' => array('pagnumseqob')));
            $listagemSimec->addAcao('share', array('func' => 'reenviarPagamento', 'cor' => 'warning', 'titulo' => 'Reenviar Pagamento'));
            $listagemSimec->setAcaoComoCondicional('refresh', [['campo' => 'pagnumseqob', 'valor' => '', 'op' => 'diferente']]);
            $listagemSimec->setAcaoComoCondicional('share', [['campo' => 'pagnumseqob', 'valor' => '', 'op' => 'igual']]);
        }
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->addCallbackDeCampo('pagvalorparcela', 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, 'pagvalorparcela');
        $listagemSimec->setTamanhoPagina(500);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function carregarDadosParaPagamento($arrPost = array())
    {
        
        $proid = $arrPost['proid'];
        $empid = $arrPost['empid'];
        
        if ($arrPost['tipo'] == 'par') {
            $cabecalho = array('Ação', 'Iniciativa', 'Valor da Iniciativa', 'Valor Empenhado',  '% Empenhado', 'Valor Pago neste Empenho', '% Pago no Empenho', '% Saldo total deferido', '% Total pago processo', '% Pagamento empenho', 'Valor pagamento (R$)', '% Total Pago Processo', 'Parcela');
            $sql = $this->model->carregarDadosParaPagamentoPAR($empid);
        } else {
            $cabecalho = array('Ação', 'Obra', 'Valor da Obra', '% Físico Total', 'Valor Empenhado', '% Empenhado', 'Valor Pago neste Empenho', '% Pago no Empenho', '% Saldo total deferido', '% Total pago processo', '% Pagamento empenho', 'Valor pagamento (R$)', '% Pagamento obra', 'Parcela');
            $sql = $this->model->carregarDadosParaPagamentoOBRA($empid);
        }
        $arrDados = $this->model->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        echo getToobarListagem($sql, false);
        $html = '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="0">
				<thead>
					<tr>';
        foreach ($cabecalho as $v) {
            $html .= '<th valign="middle">'.$v.'</th>';
        }
        $html .= '</tr>
				</thead>
				<tbody>';
        
        $parcela = $this->model->pegaUm("SELECT COALESCE(MAX(p.pagparcela),0) as parcela FROM par3.pagamento p
                                         WHERE p.empid = ".$empid." AND p.pagstatus='A'
                                            and p.pagsituacaopagamento not ilike '%CANCELADO%'
                            			    and p.pagsituacaopagamento not ilike '%vala%'
                            			    and p.pagsituacaopagamento not ilike '%devolvido%'");
        
        $totalIniciativa = 0;
        $totalEmpenhado = 0;
        $totalPagoEmpenho = 0;
        $obPendencia = new Par3_Controller_Pendencia();
        $boPendenciaIniciativa = 'S';
        
        foreach ($arrDados as $v) {
            $totalIniciativa += $v['vlriniciativa'];
            $totalEmpenhado += $v['saldo'];
            $totalPagoEmpenho += $v['vlrpagamento'];
            
            $disabled = "";
            if ((float)$v['vlrpagamento'] >= (float)$v['saldo'] || ($v['percent_pago'] > 100 || $v['percent_pago'] == 100)) {
                $disabled = "disabled";
            }

            $checkbox = '<input type="checkbox" name="codigo[]" id="chk_'.$v['codigo'].'" value="'.$v['codigo'].'" class="chk_acao" '.$disabled.'>';
            $boPendenciaIniciativa = 'N';

            $v['checkbox'] = true;
            $colunaDesembolso = '<td>' . self::formata_coluna_desembolso($v['saldo_desembolso'], $v) . '</td>';

            if ($arrPost['tipo'] == 'obra') {
                $execucaoObra = '<td>'.simec_number_format($v['percent_obra']).'</td>';
                $tdAdicionalObra = '<td></td>';
                
                $arCampos['tipo_obrjeto'] = 'Obra';
                $arCampos['id'] = $proid;
                $v['descricaoitem'] = $v['descricaoitem'].' '.carregaHistoricoObra('', $arCampos);
            }

            $html .= '<tr>
						<td valign="top" style="text-align: center;"><div class="checkbox checkbox-success">
							<input type="hidden" name="valor_item_'.$v['codigo'].'" id="valor_item_'.$v['codigo'].'" value="'.$v['vlriniciativa'].'"/>
							<input type="hidden" name="valor_total_'.$v['codigo'].'" id="valor_total_'.$v['codigo'].'" value="'.$v['valortotal'].'"/>
							<input type="hidden" name="valor_empenho_'.$v['codigo'].'" id="valor_empenho_'.$v['codigo'].'" value="'.$v['saldo'].'"/>
							<input type="hidden" name="valor_pago_empenho_'.$v['codigo'].'" id="valor_pago_empenho_'.$v['codigo'].'" value="'.$v['vlrpagamento'].'"/>
                        	'.$checkbox.'
                        	<label for="chk_'.$v['codigo'].'">&nbsp;</label>
                        	</div></td>
						<td>'.$v['descricaoitem'].'</td>
						<td>'.simec_number_format($v['vlriniciativa']).'</td>
						' . $execucaoObra . '
						<td>'.simec_number_format($v['saldo']).'</td>
						<td>'.simec_number_format($v['percent_empenho'], '2', ',', '.').'%</td>
						<td>'.simec_number_format($v['vlrpagamento']).'</td>
						<td>'.simec_number_format($v['percent_pago'], '2', ',', '.').'%</td>
						' . $colunaDesembolso . '
						<td>'.simec_number_format($v['percent_total_pago_processo'], '2', ',', '.').'%</td>
						<td><input name="percent['.$v['codigo'].']" id="percent_'.$v['codigo'].'" type="text" value="" disabled class="form-control percent" onkeyup="this.value=mascaraglobal(\'###.##\',this.value); cacularValorPagamento('.$v['codigo'].', \'percent\');"></td>
						<td><input name="valorpagamento['.$v['codigo'].']" id="valorpagamento_'.$v['codigo'].'" type="text" value="" disabled class="form-control valorpagamento" onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value); cacularValorPagamento('.$v['codigo'].', \'valor\');"></td>
						<td><input name="total_pago['.$v['codigo'].']" id="total_pago_'.$v['codigo'].'" type="text" disabled data-max="'.(100-number_format_par3($v['percent_total_pago'])).'" class="form-control" onkeyup="this.value=mascaraglobal(\'###.##\',this.value); cacularValorPagamento('.$v['codigo'].', \'total\');"/>
                            <input name="percent_total_pago['.$v['codigo'].']" id="percent_total_pago_'.$v['codigo'].'" type="hidden" value="'.$v['percent_total_pago'].'">
                            <input name="vlr_total_pagamento['.$v['codigo'].']" id="vlr_total_pagamento_'.$v['codigo'].'" type="hidden" value="'.$v['vlr_total_pagamento'].'">
                        </td>
						<td>'.($parcela+1).'</td>
					</tr>';
        }
        $html .= '<tr>
					<td colspan="2"><b></b></td>
					<td style="text-align: center;"><b>'.simec_number_format($totalIniciativa, '2', ',', '.').'</b></td>
					' . $tdAdicionalObra . '
					<td style="text-align: center;"><b>'.simec_number_format($totalEmpenhado, '2', ',', '.').'</b></td>
					<td></td>
					<td style="text-align: center;"><b>'.simec_number_format($totalPagoEmpenho, '2', ',', '.').'</b></td>
					<td></td>
					<td></td>
					<td colspan="1" style="text-align: right;"><b>Total do Pagamento:</b></td>
					<td><input name="valor_total_pagamento" id="valor_total_pagamento" type="text" value="0,00" readonly class="form-control">
						<input name="pagparcela" id="pagparcela" type="hidden" value="'.($parcela+1).'"></td>
						<input name="pagid" id="pagid" type="hidden" value=""></td>
						<input name="pagnumseqob" id="pagnumseqob" type="hidden" value=""></td>
					<td></td>
                    <td></td>
				</tr>
				</tbody></table>';
        return array($html, $boPendenciaIniciativa);
    }
    
    public function excluirPagamento($arrPost = array())
    {
        $sql = "UPDATE par3.pagamento SET pagstatus = 'I', pagusucpfexclusao = '{$_SESSION['usucpf']}', pagdataexclusao =  now() WHERE pagid = {$_REQUEST['pagid']}";
        $this->model->executar($sql);
        
        if ($arrPost['tipo'] == 'par') {
            $sql = "UPDATE par3.pagamentoparcomposicao SET ppcstatus = 'I' WHERE pagid = {$_REQUEST['pagid']}";
            $this->model->executar($sql);
        } else {
            $sql = "UPDATE par3.pagamentoobracomposicao SET pmcstatus = 'I' WHERE pagid = {$_REQUEST['pagid']}";
            $this->model->executar($sql);
        }
        
        return $this->model->commit();
    }
    
    public function validaSolicitaPagamento($arrPost = array())
    {
        include_once APPRAIZ . "includes/classes/ProcessoFNDE.class.php";
        include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
        require_once APPRAIZ . 'par/classes/Habilita.class.inc';
        require_once APPRAIZ.'par3/classes/controller/WS_Servico_FNDE.class.inc';

//        ver($arrPost);




        $obHabilita = new Habilita();
        $obServicoFNDE = new Par3_Controller_WS_Servico_FNDE($arrPost['wsusuario'], $arrPost['wssenha']);
        $obProcesso = new Par3_Controller_Processo();
        
        $arrProcesso = $this->model->pegaLinha("select p.procnpj, p.seq_conta_corrente from par3.processo p where p.proid = {$arrPost['proid']}");
        /*if( empty($arrProcesso['seq_conta_corrente']) ){
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', 'Para solicitar o pagamento, é obrigatório o processo ter conta corrente ativa.');
        } else {*/
            //$habilitado = $obHabilita->consultaHabilitaEntidade($arrProcesso['procnpj']);
            $habilitado = 'Habilitado';
            
        if ($habilitado == 'Habilitado') {
            $res_acc = $obServicoFNDE->consultaContaCorrenteProcessoFNDE($arrPost['proid'], true);

            if ((string)$res_acc->seq_conta) {
                $obProcesso->atualizaContaCorrentePAR($arrPost['proid'], $res_acc);
                $res_se = $this->solicitarPagamento($arrPost);
            } else {
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', '<b>Consulta Conta Corrente:</b> '.$res_acc);
                //$obServicoFNDE->solicitarContaCorrente($dados);
            }
        } else {
            if ($habilitado = "Em diligência") {
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', "<b>Consulta Habilita:</b> A solicitação de pagamento não pode ser efetuada, pois o município não apresentou a documentação exigida para habilitação junto ao FNDE.");
            } else {
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', '<b>Consulta Habilita:</b> '.$habilitado);
            }
        }
        //}
    }

    public function solicitarPagamento($arrPost = array())
    {
        
        $dadosse = $this->model->pegaLinha("SELECT emp.empcnpj, pro.proseqconta, pro.seq_conta_corrente,
										  emp.empnumeroprocesso, emp.empprogramafnde,
										  emp.empnumerosistema, emp.empanooriginal,
										  emp.empnumero, pro.pronumeroprocesso, trim(procnpj) as procnpj, empcodigonatdespesa
								   FROM par3.empenho emp
								   INNER JOIN par3.processo pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A' and empstatus = 'A'
								   WHERE empid='".$arrPost['empid']."'");
        
        if ($arrPost['pagid']) {
            $arrRequest = $arrPost;
            $arrPost = $this->carregaPagamentoEnviado($arrPost['pagid']);
            
            $arrPost['requisicao'] = $arrRequest['requisicao'];
            $arrPost['wsusuario'] = $arrRequest['wsusuario'];
            $arrPost['wssenha'] = $arrRequest['wssenha'];
            $arrPost['requisicao'] = $arrRequest['requisicao'];
            $arrPost['requisicao'] = $arrRequest['requisicao'];
            $arrPost['requisicao'] = $arrRequest['requisicao'];
            $arrPost['empid'] = $arrRequest['empid'];
            
            if ($dadosse['empcodigonatdespesa'] == 33404100 || $dadosse['empcodigonatdespesa'] == 33304100) {
                $arrPost['vl_custeio'] = $arrPost['valor_total_pagamento'];
                $arrPost['vl_capital'] = "0";
            } else { // Capital
                $arrPost['vl_custeio'] = "0";
                $arrPost['vl_capital'] = $arrPost['valor_total_pagamento'];
            }
            $arrPost['valor'] = $arrPost['valor_total_pagamento'];
            
            $arrPost['an_referencia'] = $arrPost['paganoexercicio'];
        } else {
            if ($dadosse['empcodigonatdespesa'] == 33404100 || $dadosse['empcodigonatdespesa'] == 33304100) {
                $arrPost['vl_custeio'] = str_replace(array(".",","), array("","."), $arrPost['valor_total_pagamento']);
                $arrPost['vl_capital'] = "0";
            } else { // Capital
                $arrPost['vl_custeio'] = "0";
                $arrPost['vl_capital'] = str_replace(array(".",","), array("","."), $arrPost['valor_total_pagamento']);
            }
            $arrPost['valor'] = str_replace(array(".",","), array("","."), $arrPost['valor_total_pagamento']);
            
            $arrPost['an_referencia'] = date("Y");
        }
                
        $arrPost['nu_processo'] = $dadosse['empnumeroprocesso'];
        $arrPost['nu_documento_siafi_ne'] = substr($dadosse['empnumero'], strpos($dadosse['empnumero'], 'NE')+2);
        $arrPost['nu_cgc_favorecido'] = $dadosse['empcnpj'];
        $arrPost['nu_seq_conta_corrente_favorec'] = $dadosse['proseqconta'];
        
        $arrPost['sub_tipo_documento'] = "01";
        $arrPost['nu_sistema'] = $dadosse['empnumerosistema'];
        $arrPost['unidade_gestora'] = "153173";
        $arrPost['gestao'] = "15253";
        $arrPost['co_programa_fnde'] = $this->model->pegaUm("SELECT tipprogramafnde FROM execucaofinanceira.tipoprocesso WHERE tipid = 12");
        
        $dadosNE = explode("NE", $dadosse['empnumero']);
        $arrPost['an_exercicio'] = $dadosNE[0];
        if (!$arrPost['an_exercicio']) {
            $arrPost['an_exercicio'] = $this->model->pegaUm("SELECT to_char( empdata, 'YYYY' ) as ano FROM par3.empenho WHERE empid='".$arrPost['empid']."'");
        }
        $arrPost['nu_mes'] = sprintf("%02d", $arrPost['mes']);
        
        include_once APPRAIZ.'includes/classes/HistoricoSolicitacaoEmpenho.class.inc';
        $obRequest = new HistoricoSolicitacaoEmpenho();
        
        $obRequest->hsesistema = $arrPost['nu_sistema'];
        
        if ($arrPost['requisicao'] == 'visualiza_xml') {
            $arrPost['request_id'] = $obRequest->pegaRequest_ID();
        } else {
            $arrPost['request_id'] = $obRequest->gravarRequest_ID();
        }
        //$arrPost['request_id'] = $this->model->pegaUm("SELECT (COALESCE(MAX(pagid),0)+1000000) as r FROM par3.pagamento");
        
        $obServico = new Fnde_WebService_ServicoFNDE($arrPost);
        
        
        if ($arrPost['requisicao'] == 'solicitar' || $arrPost['requisicao'] == 'reenviar_pagamento') {
            $arrParam = array(
                    'mes'               => $arrPost['mes'],
                    'ano'               => $arrPost['an_referencia'],
                    'exercicio'         => $arrPost['an_exercicio'],
                    'pagparcela'        => $arrPost['pagparcela'],
                    'valor'             => $arrPost['valor'],
                    'empnumero'         => $dadosse['empnumero'],
                    'empid'             => $arrPost['empid'],
                    'request_id'        => $arrPost['request_id'],
                    'sistema'           => $arrPost['tipo'],
                    'arepcid'           => $arrPost['codigo'],
                    'arpercentual'      => $arrPost['percent'],
                    'arvalorpagamento'  => $arrPost['valorpagamento']
            );
            $id_pagamento = $arrPost['pagid'];

            if ($arrPost['requisicao'] == 'solicitar') {
                $id_pagamento = $this->salvarDadosPagamento($arrParam);

                $percent_total_processo = 0;
                foreach ($arrPost['total_pago'] as $t){
                    $percent_total_processo += $t;
                }

                if($arrPost['chkdef']) {
                    $this->model->vincularPagamentoDesembolso($arrPost['chkdef'], $arrPost['pagdef'], $id_pagamento,
                        $percent_total_processo, $arrPost['tipo']);
                }
            }
            
            $retorno = $obServico->solicitarPagamentoFNDE($arrPost);
            
            if ($retorno['erro'] == 'S') {
                $sql = "INSERT INTO par3.logwssigef(proid, empid, pagid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 		VALUES ('".$arrPost['proid']."', ".$arrPost['empid'].", $id_pagamento, 'solicitar', 'solicitarPagamento - erro', '".$retorno['url']."', '".str_replace("'", '"', $retorno['xmlenvio'])."', '".str_replace("'", '"', $retorno['xmlretorno'])."', true, '".$_SESSION['usucpf']."', '".str_replace("'", '', $retorno['mensagem'])."', NOW()) RETURNING lgwid";
                
                $lgwid = $this->model->pegaUm($sql);
                $this->model->commit();
                
                $obRequest->lgwid = $lgwid;
                $obRequest->empid = $arrPost['empid'];
                $obRequest->pagid = $id_pagamento;
                $obRequest->alterar($arrPost['request_id']);
                                
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', $retorno['mensagem']);
            } else {
                $xml = simplexml_load_string(stripslashes($retorno['xmlretorno']));
                $pagnumseqob = (string)$xml->body->nu_registro_ob;
                
                $sql = "UPDATE par3.pagamento SET pagnumseqob = ".(($pagnumseqob)?"'".$pagnumseqob."'":"NULL")." WHERE pagid = $id_pagamento";
                $this->model->executar($sql);
                $this->model->commit();
                
                $sql = "INSERT INTO par3.logwssigef(proid, empid, pagid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 		VALUES ('".$arrPost['proid']."', ".$arrPost['empid'].", $id_pagamento, 'solicitar', 'solicitarPagamento - sucesso', '".$retorno['url']."', '".str_replace("'", '"', $retorno['xmlenvio'])."', '".str_replace("'", '"', $retorno['xmlretorno'])."', false, '".$_SESSION['usucpf']."', '".str_replace("'", '', $retorno['mensagem'])."', NOW()) RETURNING lgwid";
                $lgwid = $this->model->pegaUm($sql);
                $this->model->commit();
                
                $obRequest->lgwid = $lgwid;
                $obRequest->empid = $arrPost['empid'];
                $obRequest->pagid = $id_pagamento;
                $obRequest->alterar($arrPost['request_id']);
                
                simec_redirecionar($_SERVER['HTTP_REFERER'], 'success', $retorno['mensagem']);
            }
        } else {
            if ($arrPost['requisicao']  == 'visualiza_xml') {
                $retorno = $obServico->solicitarPagamentoFNDE($arrPost);
                
                echo '<pre>';
                echo simec_htmlentities($retorno);
                exit;
            }
        }
    }
    
    public function carregaPagamentoEnviado($pagid)
    {
        $sql = "select
					e.empcnpj,
				    e.empnumeroprocesso, e.empprogramafnde,
				    e.empnumerosistema, e.empanooriginal,
				    e.empnumero, e.empcodigonatdespesa,
				    pro.pronumeroprocesso, pro.proid,
				    trim(pro.procnpj) as cnpj,
				    pro.proseqconta, pro.seq_conta_corrente,
				    p.pagid,
				    p.pagvalorparcela as valorpagamento,
				    p.pagvalorparcela as valor_total_pagamento,
				    e.empprogramafnde as co_programa_fnde,
				    p.pagparcela,
				    p.paganoexercicio,
				    p.paganoparcela as ano,
				    p.pagmes as mes
				from par3.pagamento p
					inner join par3.empenho e on e.empid = p.empid
				    inner join par3.processo pro on pro.pronumeroprocesso = e.empnumeroprocesso
				where
					p.pagid = $pagid
				    and p.pagstatus = 'A'
				    and e.empstatus = 'A'
				    and pro.prostatus = 'A'";
        return $this->model->pegaLinha($sql);
    }
    
    public function salvarDadosPagamento(array $arrParam = array())
    {
    
        $mes = $arrParam['mes'];
        $ano = $arrParam['ano'];
        $pagparcela = $arrParam['pagparcela'];
        $valor = $arrParam['valor'];
        $empnumero = $arrParam['empnumero'];
        $empid = $arrParam['empid'];
        $sistema = $arrParam['sistema'];
        $anosubacao = $arrParam['anosubacao'];
        $arepcid = $arrParam['arepcid'];
        $anosubacao = $arrParam['anosubacao'];
        $arpercentual = $arrParam['arpercentual'];
        $arvalorpagamento = $arrParam['arvalorpagamento'];
        $parnumseqob = ($arrParam['parnumseqob'] ? "'" . $arrParam['parnumseqob'] . "'" : 'null');
    
        $sql = "INSERT INTO par3.pagamento(pagparcela, pagmes, paganoparcela, pagvalorparcela, paganoexercicio, pagnumeroempenho, empid, usucpf, pagdatapagamento, pagnumseqob, pagsituacaopagamento)
				VALUES ({$pagparcela},
						{$mes},
						{$ano},
						{$valor},
						" . date('Y') . ",
						'{$empnumero}',
						{$empid},
						'{$_SESSION['usucpf']}',
						NOW(),
						{$parnumseqob},
						'Enviado ao SIGEF') RETURNING pagid";
    
        $pagid = $this->model->pegaUm($sql);
    
        if (is_array($arepcid) && !empty($arepcid[0])) {
            if ($sistema == 'par') {
                foreach ($arepcid as $epcid) {
                    $pobpercentualpag = str_replace(array(","), array("."), $arpercentual[$epcid]);
                    $ppcvalorpagamento = str_replace(array(".", ","), array("", "."), $arvalorpagamento[$epcid]);
    
                    $sql = "INSERT INTO par3.pagamentoparcomposicao(pagid, epcid, ppcpercentualpag, ppcvalorpagamento, ppcstatus)
    						VALUES ($pagid, $epcid, $pobpercentualpag, $ppcvalorpagamento, 'A')";
                    
                    $this->model->executar($sql);
                    $this->model->commit();
                }
            } else {
                foreach ($arepcid as $epcid) {
                    $pobpercentualpag = str_replace(array(","), array("."), $arpercentual[$epcid]);
                    $ppcvalorpagamento = str_replace(array(".", ","), array("", "."), $arvalorpagamento[$epcid]);
                
                    $sql = "INSERT INTO par3.pagamentoobracomposicao(pagid, eocid, pmcpercentualpag, pmcvalorpagamento, pmcstatus)
    						VALUES ($pagid, $epcid, $pobpercentualpag, $ppcvalorpagamento, 'A')";
                     
                    $this->model->executar($sql);
                    $this->model->commit();
                }
            }
        }
        $sql = "INSERT INTO par3.historicopagamento(pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
				VALUES ({$pagid}, NOW(), '" . $_SESSION['usucpf'] . "', " . $pagparcela . ", " . $valor . ", 'Enviado ao SIGEF')";
        $this->model->executar($sql);
        $this->model->commit();
    
        return $pagid;
    }
    
    public function consultarPagamento($arrPost = array())
    {
        
        $obServico = new Fnde_WebService_ServicoFNDE($arrPost);
        $retorno = $obServico->consultarPagamentoFNDE($arrPost['pagnumseqob']);
        		
        if ($retorno['erro'] == 'S') {
            $sql = "INSERT INTO par3.logwssigef(proid, empid, pagid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 		VALUES ('".$arrPost['proid']."', ".$arrPost['empid'].", {$arrPost['pagid']}, 'consultar', 'consultarPagamento - erro', '".$retorno['url']."', '".str_replace("'", '"', $retorno['xmlenvio'])."', '".str_replace("'", '"', $retorno['xmlretorno'])."', true, '".$_SESSION['usucpf']."', '".str_replace("'", '', $retorno['mensagem'])."', NOW())";
        
            $this->model->executar($sql);
            $this->model->commit();
        
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', $retorno['mensagem']);
        } else {
            $xml = simplexml_load_string(stripslashes($retorno['xmlretorno']));
            $nu_seq_ob          = (string) $xml->body->row->nu_seq_ob;
            $numero_documento   = (string) $xml->body->row->numero_documento;
            $nu_cnpj            = (string) $xml->body->row->nu_cnpj;
            $ds_problema        = (string) $xml->body->row->ds_problema;
            $data_documento     = (string) $xml->body->row->data_documento;
            $valor_ob           = (string) $xml->body->row->valor_ob;
            $situacao_documento = (string) $xml->body->row->situacao_documento;
            $processo           = (string) $xml->body->row->processo;
            $nu_favorecido      = (string) $xml->body->row->nu_favorecido;
            $nu_seq_ob          = (string) $xml->body->row->nu_seq_ob;
            $status             = (string) $xml->body->row->status;
            
            if ($data_documento) {
                $sql = "UPDATE par3.pagamento SET
							pagnumeroob = '".$numero_documento."',
							pagsituacaopagamento = '".iconv("UTF-8", "ISO-8859-1", $situacao_documento)."',
							pagdatapagamentosiafi = ".(trim($data_documento) ? "'".formata_data_sql(iconv("UTF-8", "ISO-8859-1", $data_documento))."'" : 'null')."
						WHERE pagid = ".$arrPost['pagid'];
            } else {
                $sql = "UPDATE par3.pagamento SET
							pagsituacaopagamento = '".iconv("UTF-8", "ISO-8859-1", $situacao_documento)."'
						WHERE pagid = ".$arrPost['pagid'];
            }
            $this->model->executar($sql);
            $this->model->commit();
        
            $sql = "INSERT INTO par3.logwssigef(proid, empid, pagid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 		VALUES ('".$arrPost['proid']."', ".$arrPost['empid'].", {$arrPost['pagid']}, 'consultar', 'consultarPagamento - sucesso', '".$retorno['url']."', '".str_replace("'", '"', $retorno['xmlenvio'])."', '".str_replace("'", '"', $retorno['xmlretorno'])."', false, '".$_SESSION['usucpf']."', '".str_replace("'", '', $retorno['mensagem'])."', NOW())";
            $this->model->executar($sql);
            $this->model->commit();
            
            $pagparcela = $this->model->pegaUm("SELECT hpgparcela FROM par3.historicopagamento WHERE pagid = {$arrPost['pagid']} ORDER BY hpgdata desc");
            
            $sql = "INSERT INTO par3.historicopagamento(pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
					VALUES ({$arrPost['pagid']}, NOW(), '" . $_SESSION['usucpf'] . "', {$pagparcela}, " . $valor_ob . ", '".iconv("UTF-8", "ISO-8859-1", $situacao_documento)."')";
            $this->model->executar($sql);
            $this->model->commit();
        
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'success', $retorno['mensagem']);
        }
    }
    
    public function cancelarPagamento($arrPost = array())
    {
        
        $obServico = new Fnde_WebService_ServicoFNDE($arrPost);
        $retorno = $obServico->cancelarPagamentoFNDE($arrPost['pagnumseqob']);
        
        if ($retorno['erro'] == 'S') {
            $sql = "INSERT INTO par3.logwssigef(proid, empid, pagid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 		VALUES ('".$arrPost['proid']."', ".$arrPost['empid'].", {$arrPost['pagid']}, 'cancelar', 'cancelarPagamento - erro', '".$retorno['url']."', '".str_replace("'", '"', $retorno['xmlenvio'])."', '".str_replace("'", '"', $retorno['xmlretorno'])."', true, '".$_SESSION['usucpf']."', '".str_replace("'", '', $retorno['mensagem'])."', NOW())";
        
            $this->model->executar($sql);
            $this->model->commit();
        
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error', $retorno['mensagem']);
        } else {
            $sql = "SELECT pagparcela, pagvalorparcela FROM par3.pagamento WHERE pagid = {$arrPost['pagid']}";
            $arPagamento = $this->model->pegaLinha($sql);
            
            $sql = "UPDATE par3.pagamento SET pagsituacaopagamento = '9 - CANCELADO' WHERE pagid = ".$arrPost['pagid'];
            $this->model->executar($sql);
            $this->model->commit();
        
            $sql = "INSERT INTO par3.logwssigef(proid, empid, pagid, lgwmethod, lgwservico, lgwurl, lgwrequest, lgwresponse, lgwerro, usucpf, lgwmsgretorno, lgwdata)
				 		VALUES ('".$arrPost['proid']."', ".$arrPost['empid'].", {$arrPost['pagid']}, 'cancelar', 'cancelarPagamento - sucesso', '".$retorno['url']."', '".str_replace("'", '"', $retorno['xmlenvio'])."', '".str_replace("'", '"', $retorno['xmlretorno'])."', false, '".$_SESSION['usucpf']."', '".str_replace("'", '', $retorno['mensagem'])."', NOW())";
            $this->model->executar($sql);
            $this->model->commit();
            
            $sql = "INSERT INTO par3.historicopagamento(pagid, hpgdata, usucpf, hpgparcela, hpgvalorparcela, hpgsituacaopagamento)
					VALUES ({$arrPost['pagid']}, NOW(), '" . $_SESSION['usucpf'] . "', {$arPagamento['pagparcela']}, ".$arPagamento['pagvalorparcela'].", '9 - CANCELADO')";
            $this->model->executar($sql);
            $this->model->commit();
        
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'success', $retorno['mensagem']);
        }
    }
    
    public function historicoPagamento($arrPost = array())
    {
        $sql = "SELECT
					u.usunome,
					to_char(hpgdata, 'dd/mm/YYYY HH24:MI:SS') as data,
					hpgsituacaopagamento,
					hpgparcela,
					hpgvalorparcela
				FROM
					par3.historicopagamento h
				LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf
				WHERE
					h.pagid = {$arrPost['pagid']}
					and h.hpgdata in (select max(h1.hpgdata) as data
	                                      from par3.historicopagamento h1
	                                      where
	                                          h1.pagid = h.pagid
	                                      group by h1.usucpf, h1.hpgsituacaopagamento)
				ORDER BY
					hpgdata desc";
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Usuário', 'Data', 'Situação', 'Parcela', 'Valor parcela');
        $esconderColunas = array();
        $listagemSimec->setTitulo('Histórico de Fase do Pagamentos');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo('hpgvalorparcela', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->setTamanhoPagina(500);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function iniciativasVinculadasPagamento($arrPost = array())
    {
        $sql = "SELECT
					ve.iniid,
				    ve.iniciativa,
				    ve.vlriniciativa,
				    ve.saldo,
				    sum(cast(((pac.ppcvalorpagamento/ve.saldo )*100) as numeric(20,2))) as percent,
				    sum(pac.ppcvalorpagamento) as vlrpago
				FROM par3.pagamento pag
				    inner join par3.pagamentoparcomposicao pac on pac.pagid = pag.pagid and pac.ppcstatus = 'A'
				    inner join par3.v_saldo_empenho_composicao_iniciativa ve on ve.empid = pag.empid and pac.epcid = ve.epcid
				WHERE pag.pagstatus = 'A'
					and pag.pagid = {$arrPost['pagid']}
				    and pag.pagsituacaopagamento not ilike '%CANCELADO%'
				    and pag.pagsituacaopagamento not ilike '%vala%'
				    and pag.pagsituacaopagamento not ilike '%devolvido%'
				    and pag.pagsituacaopagamento not ilike '%VALA CENTRO DE GESTÃO%'
				GROUP BY ve.iniid, ve.iniciativa, ve.vlriniciativa, ve.saldo";
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Codigo', 'Iniciativa', 'Valor Iniciativa', 'Valor Empenhado', '% Pago', 'Valor Pago');
        $esconderColunas = array();
        $listagemSimec->setTitulo('Lista de Iniciativas do Pagamento');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo('vlriniciativa', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->addCallbackDeCampo('vlrpago', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->addCallbackDeCampo('saldo', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->setTamanhoPagina(500);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function obrasVinculadasPagamento($arrPost = array())
    {
        $sql = "SELECT
					ve.obrid,
				    ve.obrdsc,
				    ve.valorobra,
				    ve.saldo,
				    cast(((pac.pmcvalorpagamento/ve.saldo )*100) as numeric(20,2)) as percent,
				    pac.pmcvalorpagamento as vlrpago,
				    ve.esddsc
				FROM par3.pagamento pag
				    inner join par3.pagamentoobracomposicao pac on pac.pagid = pag.pagid and pac.pmcstatus = 'A'
				    inner join par3.v_saldo_empenho_composicao_obra ve on ve.empid = pag.empid and pac.eocid = ve.eocid
				WHERE pag.pagstatus = 'A'
					and pag.pagid = {$arrPost['pagid']}
				    and pag.pagsituacaopagamento not ilike '%CANCELADO%'
				    and pag.pagsituacaopagamento not ilike '%vala%'
				    and pag.pagsituacaopagamento not ilike '%devolvido%'
				    and pag.pagsituacaopagamento not ilike '%VALA CENTRO DE GESTÃO%'";
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Codigo', 'Obra', 'Valor Obra', 'Valor Empenhado', '% Pago', 'Valor Pago', 'Situação');
        $esconderColunas = array();
        $listagemSimec->setTitulo('Lista de Obras do Pagamento');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo('valorobra', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->addCallbackDeCampo('saldo', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->addCallbackDeCampo('vlrpago', 'formataNumeroMonetarioComSimbolo');
        $listagemSimec->setTamanhoPagina(500);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function verificaAcompanhamentoEntidade($proid)
    {
        $sql = $this->model->sqlPegaAcompanhamentoEntidade($proid);
        
        return $this->model->pegaLinha($sql);
    }
    
    public function carregaAcompanhamentoEntidade($proid)
    {

        $sql = $this->model->sqlPegaAcompanhamentoEntidade($proid);
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Processo', 'UF', 'Entidade', 'Nº Termo', 'Qtd Contrato', 'Qtd Total Itens', 'Vlr Total Contrato', 'Valor do processo', 'Vlr Total Empenhado', 'Vlr Total Pago', '% Total pago');
        $esconderColunas = array('inuid');
        //$listagemSimec->setTitulo('Lista de Obras do Pagamento');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo(['totalempenhado','valortotalcontrato', 'totalpago'], 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addCallbackDeCampo(['valorprocesso'], 'formatavalorprocesso');
        $listagemSimec->setTamanhoPagina(500);
        $listagemSimec->setCampos($arrayCabecalho);
        //$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    public function carregaPendenciasPar($inuid)
    {
        $sql = "SELECT
                	CASE WHEN obras_par = 't' THEN 'Sim' ELSE 'Não' END AS obras_par,
                	CASE WHEN cacs = 't' THEN 'Sim' ELSE 'Não' END AS cacs,
                	CASE WHEN pne = 't' THEN 'Sim' ELSE 'Não' END AS pne,
                	CASE WHEN siope = 't' THEN 'Sim' ELSE 'Não' END AS siope,
                	CASE WHEN habilitacao = 't' THEN 'Sim' ELSE 'Não' END AS habilitacao
                FROM par3.vm_relatorio_quantitativo_pendencias
                WHERE inuid = $inuid";
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Obras', 'CACS', 'PNE', 'SIOPE', 'Habilita');
        $esconderColunas = array();
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->setTamanhoPagina(500);
        $listagemSimec->setCampos($arrayCabecalho);
        //$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    /**
     * retorna situações de pagamento
     * @return array
     */
    public function getPagamentoSituacoes()
    {
        return $this->model->getPagamentoSituacoes();
    }

    /**
     * @param $saldo -> porcentagem total de desembolso aprovado - porcentagem de desembolso paga
     * @param $campos -> deferimento: lista de cada desembolso, com porcentagem paga.
     * @return string
     */
    public static function formata_coluna_desembolso($saldo, $campos){

        $arrDef = explode(',', $campos['deferimento']);
        $deferimento = '';
        $totalDeferido = 0.00;
        if(count($campos['deferimento'])) {
            foreach ($arrDef as $def) {
                $arrValues = explode('/', $def);
                $id = trim($arrValues[0]);
                $perc_desembolso = trim($arrValues[1]);
                $perc_pag_def = trim($arrValues[2]);

                $totalDeferido = bcadd($totalDeferido, $perc_desembolso, 2);

                $campos['codigo'] = trim($campos['codigo']);

                if ($campos['checkbox']) {
                    if($perc_pag_def == '100.00') {
                        $deferimento .= "<tr>
                                            <td></td>
                                            <td>{$id}</td>
                                            <td>{$perc_desembolso}%</td>
                                            <td>{$perc_pag_def}%</td>
                                         </tr>";
                    }else{
                        $deferimento .= "<tr>
                                            <td>
                                              <div class='checkbox checkbox-success'>
                                                  <input disabled type='checkbox' name='chkdef[{$id}]' id='chkdef_{$id}' 
                                                                  value='{$perc_desembolso}' class='chkdef_{$campos['codigo']}'
                                                                  onchange='alteraValorDesembolso(this,{$campos['codigo']})' >
                                                  <label for='chkdef_{$id}'>&nbsp;</label>
                                                  <input type='hidden' name='pagdef[{$id}]' value='{$perc_pag_def}'>
                                                  
                                              </div>
                                            </td>
                                            <td>{$id}</td>
                                            <td>{$perc_desembolso}%</td>
                                            <td>{$perc_pag_def}%</td>
                                        </tr>";
                    }
                    $cabecalho = '<th style="vertical-align: bottom;">Ação</th>';
                    $tdAdicional = '<td></td>';
                } else {
                    $deferimento .= '<tr><td>' . $id . '</td><td>' . $perc_desembolso . '%</td><td>' . $perc_pag_def . '%</td></tr>';
                }
            }
        }

        $saldo = $saldo > 0 ? $saldo : '0.00';
        
        $html = '<div class="btn-group dropup dropup_deferido" >
                  <span type="button" class="btn btn-secondary dropdown-toggle" data-toggle="dropdown" title="Deferimento" aria-haspopup="true" aria-expanded="false">
                    ' . $saldo . '% <i style="color: #64a0e8;" class="fa fa-info-circle"></i>
                  </span>
                  <div class="dropdown-menu" style="padding: 4px 0px 8px 8px;width: 290px; z-index:10 !important;">
                    <h3 class="popover-header" style="margin: 4px 8px 0px 8px; padding: 4px 0px 4px 0px; border-bottom: 1px dotted #cecece;">Deferimento</h3>
                    <div style="padding: 8px 8px 0px 8px;">
                      <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" style="padding: 10px; margin: 0px !important;">
                        <thead><tr>'.$cabecalho.'<th>ID Solicitação</th><th>% Deferido</th><th>% Pago</th></tr></thead>
                   ';

        if($deferimento == ''){
            $html .= '<tbody><tr><td colspan="3"><div class="alert alert-info text-center nenhum-registro" id="tb_render">Nenhuma solicitação de desembolso encontrada</div></td></tr></tbody>';
        }else{
            $html .= '<tbody>' . $deferimento . '</tbody>
                      <tfoot>'.$tdAdicional.'<td colspan="2"><b>Total deferido:</b> '. $totalDeferido . '%</td><td></td></tfoot>';
        }
        $html .= '</table></div>
                  </div>
                </div>';

        return $html;
    }
}
