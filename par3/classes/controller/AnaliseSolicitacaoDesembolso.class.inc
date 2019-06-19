<?php

class Par3_Controller_AnaliseSolicitacaoDesembolso extends Controle
{
    protected $model;

    public function __construct()
    {
        $this->model = new Par3_Model_AnaliseSolicitacaoDesembolso();
    }

    public function sqlSituacaoAnalise()
    {
        $controllerSolicitacaoDesembolso = new Par3_Controller_SolicitacaoDesembolso();
        return $controllerSolicitacaoDesembolso->sqlSituacaoAnalise();
    }

    public function listarSolicitacoesAnalise($filtros)
    {

        $sql = $this->model->montaSqlListaSolicitacoes($filtros);

        $cabecalho = array('UF', 'Município', 'Solicitação', 'Processo', 'Termo', 'Ano', 'Vigência do termo', 'Documento',
            'CNPJ', 'Valor do documento', 'Valor da solicitação', 'Valor pago Processo', 'Data solicitação', 'Situação');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setId('lista_analisedesembolso');
        $listagem->esconderColunas(['arqid', 'obrid', 'sldid', 'esdid', 'proid', 'tipo']);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo(['sdpvalor', 'saldo', 'valordocumento', 'pagoprocesso'], 'par3_mascaraMoeda');
        $listagem->addCallbackDeCampo('pronumeroprocesso', 'formata_numero_processo_component');
        $listagem->addCallbackDeCampo('cnpjfornecedor', 'formatar_cnpj');

        $listagem->addAcao('download', ['func' => 'acaoDownload', 'titulo' => 'Download do Documento', 'extra-params' => ['arqid'], 'cor' => 'success', 'outline' => 'btn-outline']);
        $listagem->setAcaoComoCondicional('download', array(array('campo' => 'sldid', 'valor' => '', 'op' => 'igual')));

        if($filtros['contratosemsolicitacao'] == 'N'){
            $listagem->addAcao('analise', ['func' => 'modalAnaliseDesembolso', 'titulo' => 'Análise']);
            $listagem->setAcaoComoCondicional('analise', array(array('campo' => 'sldid', 'valor' => '', 'op' => 'igual')));
        }

        $listagem->addAcao('pagamento', ['func' => 'redirecionarPagamentos', 'titulo' => 'Ir para Pagamentos', 'extra-params' => ['proid', 'tipo'], 'cor' => 'warning']);
        $listagem->setAcaoComoCondicional('pagamento', array(array('campo' => 'esdid', 'valor' => PAR3_DESEMBOLSO_APROVADO, 'op' => 'igual')));

        $listagem->turnOnPesquisator();
        $listagem->setFormFiltros('form_solicitacoes');
        $listagem->setTamanhoPagina(50);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public function modalAnaliseDesembolso($sdpid)
    {
        require_once APPRAIZ . '/includes/workflow.php';
        global $simec;

        $dadosAnalise = $this->model->recuperarAnaliseSolicitacao($sdpid);
        $modelSolicitacao = new Par3_Model_SolicitacaoDesembolso();
        $dadosSolicitacao = $modelSolicitacao->recuperarDadosSolicitacaoDesembolso($dadosAnalise['ecoid'], $dadosAnalise['edlid']);
        $controllerSolicitacao = new Par3_Controller_SolicitacaoDesembolso();

        $perfil = pegaPerfilGeral($_SESSION['usucpf']);
        $html = '';

        $saldoTermo = bcsub($dadosSolicitacao[0]['vlrtermo'], $dadosSolicitacao[0]['valorsolicitacoes'], 2);
        $saldoTermo = bcsub($saldoTermo, $dadosSolicitacao[0]['totalpago'], 2);
        $saldoTermoFormatado = par3_mascaraMoeda($saldoTermo, false, true);

        $estadosPermitidos = [PAR3_DESEMBOLSO_EMANALISE];
        if (!in_array(PAR3_PERFIL_SUPER_USUARIO, $perfil) && !in_array(PAR3_PERFIL_ADMINISTRADOR, $perfil) && !in_array(PAR3_PERFIL_ANALISTA_PLANEJAMENTO, $perfil)
            ||
            (!in_array($dadosAnalise['esdid'], $estadosPermitidos))
        ) {
            $attribus = array('maxlengh' => 4000, 'readonly' => 'readonly');
            $attribusCombo = array('maxlength' => '255', '', '', 'text-align' => 'left', 'disabled' => 'disabled');
            $readonly = 'readonly';
        } else {
            $attribusCombo = array('maxlength' => '255', '', '', 'text-align' => 'left');
            $attribus = array('maxlengh' => 4000);
        }

        if ($dadosSolicitacao) {
            $valorTotalSolicitacao = 0;
            foreach ($dadosSolicitacao as $sol) {
                if ($sol['esdid'] == PAR3_DESEMBOLSO_APROVADO) {
                    $valorTotalSolicitacao = bcadd($valorTotalSolicitacao, $sol['sdpvalor'], 2);
                }
            }

            $saldoContrato = bcsub($dadosSolicitacao[0]['ecovalor'], $valorTotalSolicitacao, 2);
        }

        ### Dados do Parecer
        $html .= "<table border=\"0\" class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
        <tr>
            <td style=\"width: 100%\">";
        $html .= "<div class=\"panel panel-default\" style='margin-left: 16px;'>
					    	<div class=\"panel-heading\" style=\"text-align: center; font-size: 16px;\">Dados de Processo</div>
					    	<div class=\"panel-body\">";

        $pagamentos = new Par3_Controller_Pagamentos();
        ob_start();
        $pagamentos->carregaAcompanhamentoEntidade($dadosSolicitacao[0]['proid']);
        $html .= ob_get_clean();

        $html .= "</div>
                </div>
                <div class=\"panel panel-default\" style='margin-left: 16px;'>
                    <div class=\"panel-heading\" style=\"text-align: center; font-size: 16px;\">Dados da Solicitação</div>
                    <div class=\"panel-body\">";

        $html .= $this->montaHtmlDadosSolicitacao($dadosSolicitacao[0], $saldoTermoFormatado, $saldoContrato, $readonly, $dadosAnalise);

        $html .= "</div>
                </div>";

        $html .= "<div class=\"row\">
                        <div class=\"ibox-content\" id=\"cic-div\">
                            <label class=\"col-md-offset-1 col-lg-offset-1 col-lg-2 control-label\"
                                   style=\"font-size: 18px; font-weight: bold;\">Parecer da Análise:</label>
                        </div>
                    </div>
                </div>
                <div class=\"ibox-content\" style='border: 0'>
                <form name=\"form_analise_desembolso_parecer\" id=\"form_analise_desembolso_parecer\"
                      class=\"form-horizontal\" method=\"post\">
                    <input type=\"hidden\" name=\"requisicao\" id=\"requisicao_analise\" value=\"salvarAnaliseDesembolso\">
                    <input type=\"hidden\" name=\"asdid\" id=\"asdid\" value=\"{$dadosAnalise['asdid']}\">
                    <input type=\"hidden\" name=\"sdpid\" id=\"sdpid\" value=\"{$dadosAnalise['sdpid']}\">
                    <input type=\"hidden\" name=\"docid\" id=\"docid\" value=\"{$dadosAnalise['docid']}\">
                    <input type=\"hidden\" name=\"readonly\" id=\"readonly\" value=\"{$readonly}\">
                    <input type=\"hidden\" name=\"esdid\" id=\"esdid\" value=\"{$dadosAnalise['esdid']}\">";

        $html .=
            "<div class=\"form-group\">
                        <label for=\"asdparecer\" class=\"col-sm-2 col-md-2 col-lg-2\">Texto do parecer *: </label>
                        <div class=\"col-sm-10 col-md-10 col-lg-10 \">
                            <textarea name=\"asdparecer\" id=\"asdparecer\" class=\"form-control\"
                                      maxlengh=\"4000\" required {$readonly}>{$dadosAnalise['asdparecer']}</textarea>
                        </div>
                        <div style=\"clear:both\"></div>
                    </div>
                </form></div>
            </td>
            <td style=\"text-align: center; padding: 15px;\" valign=\"center\">";

        if (!in_array(PAR3_PERFIL_SUPER_USUARIO, $perfil)) {
            $ocultar = ['historico' => true];
        }

        ob_start();
        wf_desenhaBarraNavegacaoBootstrap($dadosAnalise['docid'], array('sdpid' => $sdpid, 'valorSolicitado' => $dadosSolicitacao[0]['sdpvalor'],  'saldoTermo' => $saldoTermo), $ocultar, 'Fluxo', 'modalAnaliseDesembolso('.$dadosAnalise['sdpid'].')');
        $html .= ob_get_clean();
        $html .= "</td>
                    </tr>
                </table>";

        $html .= '<div class="row">
                        <div class="form-group" id="titulo-dados-solicitacao">
                            <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label"
                                   style="font-size: 16px; font-weight: bold;">Histórico de solicitações <i id="i-dados-solicitacao" class="fa fa-chevron-down"></i></label>
                        </div>
                    </div>
                    <div class="ibox-content" id="div-dados-solicitacao" style="display: none;">';
        ob_start();
        $controllerSolicitacao->listaHistoricoSolicitacoes($dadosSolicitacao);
        $html .= ob_get_clean();

        echo $html;
    }

    public function montaHtmlDadosSolicitacao($solicitacao, $saldoTermo, $saldoContrato, $readonly, $dadosAnalise)
    {

        $documento = '';
        $valor = ' - ';
        if ($solicitacao['ecoid'] > 0) {
            $documento = 'Contrato ' . $solicitacao['contrato'];
            $valor = par3_mascaraMoeda($solicitacao['ecovalor'], false, true);
            $saldoContrato = '<label class="col-sm-2 col-md-2 col-lg-2 control-label">Saldo do contrato: </label>
                                <div class="col-sm-2 col-md-2 col-lg-2" id="modalSaldoContrato"> '.par3_mascaraMoeda($saldoContrato, false, true).' </div>';
        } elseif ($solicitacao['edlid'] > 0) {
            $documento = $solicitacao['doadescricao'];
            $valor = par3_mascaraMoeda($solicitacao['edlvalordocumento'], false, true);
        }

        $html = '<div class="">
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label" >Solicitação: </label>
                    <div class="col-sm-2 col-md-2 col-lg-2 "> '.$solicitacao['sdpid'].' </div>
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Data da solicitação: </label>
                    <div class="col-sm-2 col-md-2 col-lg-2"> '. $solicitacao['datacriacao'] .' </div>
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Valor solicitado: </label>
                    <div class="col-sm-2 col-md-2 col-lg-2" id="modalSaldoContrato"> '.par3_mascaraMoeda($solicitacao['sdpvalor'], false, true).' </div>
                    <div style="clear:both"></div>
                    
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label" >Documento: </label>
                    <div class="col-sm-2 col-md-2 col-lg-2 " style="padding-bottom: 10px;"> ' . $documento . '
                                                             <br>
                                                             <a href="par3.php?modulo=principal/planoTrabalho/planejamento/analiseSolicitacaoDesembolso&acao=A&requisicao=downloadDocumento&arqid='.$solicitacao['arqid'].'">
                                                                <i class="glyphicon glyphicon-cloud-download btn btn-warning btn-xs"></i> ' . $solicitacao['arqnome'] . '.' . $solicitacao['arqextensao'] .
                                                            '</a>
                    </div>
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Data de criação do documento: </label>
                    <div class="col-sm-2 col-md-2 col-lg-2"> '. $solicitacao['datacriacaodocumento'] .' </div>
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Valor do documento: </label>
                    <div class="col-sm-2 col-md-2 col-lg-2"> '. $valor .' </div>
                    <div style="clear:both"></div>
                  ';

        if ($solicitacao['esdid'] == PAR3_DESEMBOLSO_APROVADO) {
            $html .= '<label class="col-sm-2 col-md-2 col-lg-2 control-label">Saldo do termo no momento da aprovação: </label>
                    <div class="col-sm-2 col-md-2 col-lg-2"> '. par3_mascaraMoeda($solicitacao['sdpsaldotermo'], false, true) .' </div>';
        }

        $html .= '<label class="col-sm-2 col-md-2 col-lg-2 control-label">Saldo atual do termo: </label>
                    <div class="col-sm-2 col-md-2 col-lg-2"> '. $saldoTermo .' </div>';

        $html .= '<div style="clear:both"></div>
                  <label class="col-sm-2 col-md-2 col-lg-2 control-label">Observação: </label>
                  <div class="col-sm-10 col-md-10 col-lg-10"> '. $solicitacao['sdpobservacao'] .' </div>';

        if ($solicitacao['esdid'] == PAR3_DESEMBOLSO_APROVADO) {
            $valor = par3_mascaraMoeda($dadosAnalise['asdvaloraprovado'], false);
            $porcentagem = ($dadosAnalise['asdvaloraprovado'] * 100) / $solicitacao['sdpvalor'];
            $html .= '<div style="clear:both"><br></div>
                    <div class="row">
                        <div class="col-sm-4 col-md-4 col-lg-4">
                            <label class="col-sm-6 col-md-6 col-lg-6  control-label" >Valor aprovado: </label>
                            <div class="col-sm-4 col-md-4 col-lg-4">
                                '. $valor .'
                            </div>
                        </div>
                        <div class="col-sm-6 col-md-6 col-lg-6">
                            <label class="col-sm-5 col-md-5 col-lg-5 control-label">Porcentagem aprovada: </label>
                            <div class="col-sm-6 col-md-6 col-lg-6">
                                '. $porcentagem .'%
                            </div>
                        </div>
                    </div>
                    <div style="clear:both"></div>';
        }else if($solicitacao['esdid'] != PAR3_DESEMBOLSO_AGUARDANDOANALISE){

            $porcentagem = '100';
            $valor = par3_mascaraMoeda($solicitacao['sdpvalor'], false);

            if($dadosAnalise['asdid'] && $dadosAnalise['asdvaloraprovado'] > 0){
                $valor = par3_mascaraMoeda($dadosAnalise['asdvaloraprovado'], false);
                $porcentagem = number_format_par3(($dadosAnalise['asdvaloraprovado'] * 100) / $solicitacao['sdpvalor']);
            }

            $html .= '<div style="clear:both"><br></div>
                    <div class="row">
                        <div class="col-sm-6 col-md-6 col-lg-6">
                            <label class="col-sm-5 col-md-5 col-lg-5  control-label" >Valor a ser aprovado: </label>
                            <div class="col-sm-6 col-md-6 col-lg-6">
                                <input type="text" class="form-control" name="asdvaloraprovado" id="asdvaloraprovado"
                                    value="'. $valor .' " '. $readonly .'
                                    onkeyup="this.value=mascaraglobal(\'###.###.###,##\',this.value); calculaPorcentagemPeloValor(this.value, \' ' . $solicitacao['sdpvalor'] . '\');">
                            </div>
                        </div>
                        <div class="col-sm-6 col-md-6 col-lg-6">
                            <label class="col-sm-6 col-md-6 col-lg-6 control-label">Porcentagem a ser aprovada: </label>
                            <div class="col-sm-6 col-md-6 col-lg-6">
                                <input type="text"  class="form-control" name="asdpercaprovado" id="asdpercaprovado"
                                    value="'. $porcentagem .' " '. $readonly .'
                                    onkeyup="this.value=mascaraglobal(\'###.##\',this.value); calculaValorPorPorcentagem(this.value, \' ' . $solicitacao['sdpvalor'] . '\');">
                            </div>
                        </div>
                    </div>
                    <div style="clear:both"></div>';
        }

        $html .= '</div>';

        return $html;
    }

    public function salvarAnaliseDesembolso($dados)
    {
        $dados['usucpf'] = $_SESSION['usucpf'];
        $dados['asdvaloraprovado'] = str_replace(array('.', ','), array('', '.'), $dados['asdvaloraprovado']);
        $this->model->popularDadosObjeto($dados);
        $this->model->salvar();

        if($this->model->commit()){
            $this->modalAnaliseDesembolso($dados['sdpid']);
        }else{
            echo 'erro';
        }

        /*if ($this->model->commit()) {
            simec_redirecionar('par3.php?modulo=principal/planoTrabalho/planejamento/analiseSolicitacaoDesembolso&acao=A', 'success');
        } else {
            simec_redirecionar('par3.php?modulo=principal/planoTrabalho/planejamento/analiseSolicitacaoDesembolso&acao=A', 'error');
        }*/
    }
}
