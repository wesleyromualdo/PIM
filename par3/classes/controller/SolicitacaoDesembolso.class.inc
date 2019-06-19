<?php

/**
 * Created by PhpStorm.
 * User: danielmarconi
 * Date: 04/05/2018
 * Time: 17:45
 */
class Par3_Controller_SolicitacaoDesembolso extends Controle
{

    protected $model;

    public function __construct()
    {
        $this->model = new Par3_Model_SolicitacaoDesembolso();
    }

    public function controlarRequisicoes($requisicao)
    {
        switch ($requisicao) {
            case 'modalSolicitacaoDesembolso':
                $this->modalSolicitacaoDesembolso($_POST['ecoid'], null, $_POST['edlid']);
                break;
            case 'salvarSolicitacaoDesembolso':
                $this->salvarSolicitacaoDesembolso($_POST);
                break;
            case 'removerSolicitacaoDesembolso':
                $this->removerSolicitacaoDesembolso($_POST);
                break;
        }
    }

    /**
     * Tanto na lista de contratos quanto na de documentos de licitação, a modal deve mostrar apenas a última solicitação para editar (carrega com id do contrato ou documento).
     * Caso a última tenha sido aprovada, reprovada ou cancelada, a modal deve limpar os campos para salvar uma nova.
     * A lista geral de solicitações (aba Solicitação Desembolso) utiliza a mesma modal. Porém carrega os dados pelo id da própria solicitação, e não do documento.
     * @param null $ecoid (solicitação de contrato)
     * @param null $edlid (solicitação de documentos de licitação)
     * @param null $sdpid (tratamento para abrir a modal na lista de solicitações, onde se manda o id da solicitação e não do documento)
     * @return void, json -> array('form', 'botaoSalvar', 'botaoExcluir', 'botaoFechar')
     */
    public function modalSolicitacaoDesembolso($ecoid = null, $sdpid = null, $edlid = null)
    {
        require_once APPRAIZ . '/includes/workflow.php';
        $btnExcluir = false;
        $disabled = 'disabled';
        $msgDisabled = '* Campos indisponíveis pois o documento está sendo analisado.';
        $estadosDocConcluido = array(PAR3_DESEMBOLSO_APROVADO, PAR3_DESEMBOLSO_REPROVADO, PAR3_DESEMBOLSO_CANCELADO);
        $novaSolicitacao = false;

        $dadosSolicitacao = $this->model->recuperarDadosSolicitacaoDesembolso($ecoid, $edlid);

        if ($sdpid) {
            $indice = array_search($sdpid, array_column($dadosSolicitacao, 'sdpid'));
        } else {
            $indice = 0;
        }

        //config de variaveis e valores
        $valorTotalSolicitacao = 0;
        $saldoContrato = $dadosSolicitacao[$indice]['ecovalor'];
        $saldoDocumento = $dadosSolicitacao[$indice]['edlvalordocumento'];
        $dadosSolicitacao[$indice]['totalpago'] = $dadosSolicitacao[$indice]['totalpago'] ?: 0.00;
        if ($dadosSolicitacao) {
            foreach ($dadosSolicitacao as $sol) {
                if ($sol['esdid'] == PAR3_DESEMBOLSO_APROVADO) {
                    $valorTotalSolicitacao = bcadd($valorTotalSolicitacao, $sol['sdpvalor'], 2);
                }
            }

            if ($ecoid > 0) {
                $saldoContrato = bcsub($dadosSolicitacao[$indice]['ecovalor'], $valorTotalSolicitacao, 2);
            } elseif ($edlid > 0) {
                $saldoDocumento = bcsub($dadosSolicitacao[$indice]['edlvalordocumento'], $valorTotalSolicitacao, 2);
            }
        }

        $saldoTermo = bcsub($dadosSolicitacao[$indice]['vlrtermo'], $dadosSolicitacao[$indice]['totalpago'], 2);
        $saldoTermo = bcsub($saldoTermo, $dadosSolicitacao[$indice]['valorsolicitacoes']);

        //saldo será o valor padrão do input
        //deve ser o menor entre valor do contrato/documento e valor do termo
        $saldo = $ecoid > 0 ? $saldoContrato : $saldoDocumento;
        if ($saldoTermo < $saldo) {
            $saldo = $saldoTermo;
        }

        if (!$dadosSolicitacao[$indice]['sdpid']) {
            $disabled = '';
            $novaSolicitacao = true;
        } elseif ($dadosSolicitacao[$indice]['esdid'] == PAR3_DESEMBOLSO_AGUARDANDOANALISE) {
            $btnExcluir = true;
            $disabled = '';
        } elseif ($dadosSolicitacao[$indice]['esdid'] == PAR3_DESEMBOLSO_DILIGENCIA) {
            $disabled = '';
        } elseif (in_array($dadosSolicitacao[$indice]['esdid'], $estadosDocConcluido) && !$sdpid) {
            $novaSolicitacao = true;
            $disabled = '';
        }

        if ($ecoid > 0 && ($dadosSolicitacao[$indice]['ecostatus'] == 'I')) {
            $disabled = 'disabled';
//            $msgDisabled = '* Campos indisponíveis pois o contrato foi inativado';
            $msgDisabled = '<div class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro" id="tb_render">Contrato inativo</div>';
            $valorSolicitado = '';
        } elseif ($edlid > 0 && $dadosSolicitacao[$indice]['edlstatus'] == 'I') {
            $disabled = 'disabled';
//            $msgDisabled = '* Campos indisponíveis pois o documento foi inativado';
            $msgDisabled = '<div class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro" id="tb_render">Documento inativo</div>';
            $valorSolicitado = '';
        }else if($saldoTermo <= 0){
            $disabled = 'disabled';
            $valorSolicitado = '';
            $msgDisabled = '<div class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro" id="tb_render">O valor do processo já foi totalmente pago</div>';
        }else if ($saldo <= 0) {
            $disabled = 'disabled';
            $valorSolicitado = '';
            $msgDisabled = '<div class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro" id="tb_render">O saldo total total do documento já foi usado.</div>';
        } else {
            $valorSolicitado = $saldo;
        }

        $valorObservacao = $novaSolicitacao ? '' : $dadosSolicitacao[$indice]['sdpobservacao'];
        if (!$novaSolicitacao) {
            $valorSolicitado = $dadosSolicitacao[$indice]['sdpvalor'];
        }

        $html = '<div class="row"><div class="col-sm-10 col-md-10 col-lg-10">';
        $html .= '<input type="hidden" id="proidsolicitacao" value="'.$dadosSolicitacao[$indice]['proid'].'">';

        $html .= "<div class=\"panel panel-default\">
					    	<div class=\"panel-heading\" style=\"text-align: center; font-size: 16px;\"><b>Dados de Processo</b></div>
					    	<div class=\"panel-body\">";

        $pagamentos = new Par3_Controller_Pagamentos();
        ob_start();
        $pagamentos->carregaAcompanhamentoEntidade($dadosSolicitacao[0]['proid']);
        $html .= ob_get_clean();

        $html .= "</div>
                </div>";

        if($saldo > 0 && ($dadosSolicitacao[0]['ecostatus'] == 'A' || $dadosSolicitacao[0]['edlstatus'] == 'A')) {
            if ($ecoid > 0) {
                $html .= $this->montaHtmlFormModalContrato($dadosSolicitacao[$indice], $novaSolicitacao,
                    $valorObservacao, $disabled, $saldoContrato, $saldoTermo, $valorSolicitado);
            } elseif ($edlid > 0) {
                $html .= $this->montaHtmlFormModalDocumentos($dadosSolicitacao[$indice], $novaSolicitacao,
                    $valorObservacao, $disabled, $saldoTermo, $valorSolicitado);
            }
        }

        if ($disabled == 'disabled') {
            $html .= '<span class="col-sm-offset-2" style="color:red;">' . $msgDisabled . '</span>';
        }

        $html .= "</div>
              <div class='col-sm-2 col-md-2 col-lg-2'>";

        $valorDocid = $novaSolicitacao ? '' : $dadosSolicitacao[$indice]['docid'];
        $valorSdpid = $novaSolicitacao ? '' : $dadosSolicitacao[$indice]['sdpid'];

        if($sdpid && ($ecoid || $edlid)){
            if($ecoid) $argumentos = $ecoid . ',' . $sdpid;
            if($edlid) $argumentos = 'null,' . $sdpid . ',' . $edlid;
            $funcao = 'modalSolicitacaoDesembolso';
        }else if($ecoid){
            $argumentos = $ecoid;
            $funcao = 'modalSolicitacaoDesembolsoContrato';
        }else if($edlid){
            $argumentos = 'null, null, ' . $edlid;
            $funcao = 'modalSolicitacaoDesembolsoDocumentos';
        }
        ob_start();
        wf_desenhaBarraNavegacaoBootstrap($valorDocid, array('sdpid' => $valorSdpid, 'valorSolicitado' => $dadosSolicitacao[$indice]['sdpvalor'],  'saldoTermo' => $saldoTermo), null, null, $funcao.'('.$argumentos.')');
        $html .= ob_get_clean();

        $html .= "</div></div>";

        $html .= '<div class="ibox-title">
                <div class="ibox-tools"  style="float:left;">
                    <a id="btn-historicosolicitacoes">
                        Histórico de Solicitações <i id="chevron" class="fa fa-chevron-down"></i>
                    </a>
                  </div>
                </div>
                <div id="historicosolicitacoes" style="display: none;">
              ';

        ob_start();
        $this->listaHistoricoSolicitacoes($dadosSolicitacao);
        $html .= ob_get_clean();
        $html .= '</div>';


        /* Configuração dos botões da modal */
        if ($ecoid > 0) {
            $htmlBotaoSalvar = ' <button type="button" id="btnSalvarModalContrato" class="btn btn-success center" '.
                'onclick="salvarSolicitacaoDesembolsoContrato('.$ecoid.', '.$dadosSolicitacao[$indice]['proid'].')"> '.
                '<i class="fa fa-save"></i> Salvar </button> ';
        } elseif ($edlid > 0) {
            $htmlBotaoSalvar = ' <button type="button" id="btnSalvarModalContrato" class="btn btn-success center" '.
                'onclick="salvarSolicitacaoDesembolsoDocumento('.$edlid.', '.$dadosSolicitacao[$indice]['proid'].')"> '.
                '<i class="fa fa-save"></i> Salvar </button> ';
        }

        $htmlBotaoExcluir = '';
        $htmlBotaoFechar = '<button type="submit" data-dismiss="modal" id="btnFecharModalContrato" class="btn btn-default center"><i class="fa fa-times-circle-o"></i> Fechar</button>';

        if ($btnExcluir) {
            if( $sdpid > 0){
                $htmlBotaoExcluir = '<button type="button" id="btnExcluirDesembolso" class="btn btn-danger center" '.
                    'onclick="removerSolicitacaoDesembolso(' . $dadosSolicitacao[$indice]['sdpid'] . ')"> '.
                    '<i class="fa fa-times-circle-o"></i> Excluir Solicitação </button>';
            }else if ($ecoid > 0) {
                $htmlBotaoExcluir = '<button type="button" id="btnExcluirDesembolso" class="btn btn-danger center" '.
                    'onclick="removerSolicitacaoDesembolsoContrato(' . $dadosSolicitacao[$indice]['sdpid'] . ',' . $ecoid . ')"> '.
                    '<i class="fa fa-times-circle-o"></i> Excluir Solicitação </button>';
            } else if ($edlid > 0) {
                $htmlBotaoExcluir = '<button type="button" id="btnExcluirDesembolso" class="btn btn-danger center" '.
                    'onclick="removerSolicitacaoDesembolsoDocumento(' . $dadosSolicitacao[$indice]['sdpid'] . ', ' . $edlid . ')"> '.
                    '<i class="fa fa-times-circle-o"></i> Excluir Solicitação </button>';
            }
        }
        if ($disabled) {
            $htmlBotaoSalvar = '';
            $htmlBotaoExcluir = '';
        }

        $result['form'] = $html;
        $result['botaoSalvar'] = $htmlBotaoSalvar;
        $result['botaoExcluir'] = $htmlBotaoExcluir;
        $result['botaoFechar'] = $htmlBotaoFechar;

        $result = simec_json_encode($result);

        echo $result;
    }

    public function montaHtmlFormModalContrato($dadosSolicitacao, $novaSolicitacao, $observacao, $disabled, $saldoContrato, $saldoTermo, $valorSolicitado)
    {
        global $simec;
        $html = '';
        $html .= '
                <div class="form-group ">
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label" >Vigência do termo: </label>
                    <div class="col-sm-10 col-md-10 col-lg-10 "> '.$dadosSolicitacao['dotdatafimvigencia'].' </div>
                    <div style="clear:both"></div>
                </div>
                <div class="form-group ">
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label" >Fornecedor: </label>
                    <div class="col-sm-10 col-md-10 col-lg-10 "> '. formatar_cnpj($dadosSolicitacao['cnpjfornecedor']) . ' - ' . $dadosSolicitacao['razaosocial'] . ' </div>
                    <div style="clear:both"></div>
                </div>
                <div class="form-group ">
                        <label class="col-sm-2 col-md-2 col-lg-2 control-label" >Contrato: </label>
                        <div class="col-sm-10 col-md-10 col-lg-10 "> '.$dadosSolicitacao['contrato'].' </div>
                        <div style="clear:both"></div>
                      </div>';

        if (!$novaSolicitacao) {
            $html .= '<input type="hidden" id="sdpid" value="'.$dadosSolicitacao['sdpid'].'">';
            $html .= '<div class="form-group ">
                        <label class="col-sm-2 col-md-2 col-lg-2 control-label" >Código da Solicitação: </label>
                        <div class="col-sm-10 col-md-10 col-lg-10 "> '.$dadosSolicitacao['sdpid'].' </div>
                        <div style="clear:both"></div>
                      </div>';
        }

        $html .= $simec->textarea('sdpobservacao', 'Observação', $observacao, array('maxlengh' => 4000, $disabled), array('label-size' => '2'));

        $html .= '<div class="form-group ">
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Valor do contrato: </label>
                    <div class="col-sm-4 col-md-4 col-lg-4"> '.par3_mascaraMoeda($dadosSolicitacao['ecovalor'], false, true).' </div>
                    <div style="clear:both"></div>
                  </div>
                  ';

        $html .= '<div class="form-group ">                     
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Saldo do contrato: </label>
                    <div class="col-sm-3 col-md-3 col-lg-3" id="modalSaldoContrato"> '.par3_mascaraMoeda($saldoContrato, false, true).' </div>
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Saldo do termo: </label>
                    <div class="col-sm-3 col-md-3 col-lg-3" id="modalSaldoTermo"> '.par3_mascaraMoeda($saldoTermo, false, true).' </div>
                    <div style="clear:both"></div>
                  </div>
                  ';

        $html .= '<div class="form-group ">
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Valor solicitado: </label>
                    <div class="col-sm-3 col-md-3 col-lg-3"> 
                        <input name="sdpvalor" id="sdpvalor" type="text" ' . $disabled . ' value="' . par3_mascaraMoeda($valorSolicitado, false, false) . '" class="form-control" maxlength="15" 
                            onkeyup="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);"
                            onblur="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);validarValorSolicitadoContrato(this);"> 
                            <div class="erroValorSolicitado"></div>
                    </div>
                    <div style="clear:both"></div>
                  </div>';
        return $html;
    }

    public function montaHtmlFormModalDocumentos($dadosSolicitacao, $novaSolicitacao, $observacao, $disabled, $saldoTermo, $valorSolicitado)
    {
        global $simec;
        $html = '';
        $html .= '
                <div class="form-group ">
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label" >Vigência do termo: </label>
                    <div class="col-sm-10 col-md-10 col-lg-10 "> '.$dadosSolicitacao['dotdatafimvigencia'].' </div>
                    <div style="clear:both"></div>
                </div>
                <div class="form-group ">
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label" >Fornecedor: </label>
                    <div class="col-sm-10 col-md-10 col-lg-10 "> '. formatar_cnpj($dadosSolicitacao['cnpjfornecedor']) . '  ' . $dadosSolicitacao['razaosocial'] . ' </div>
                    <div style="clear:both"></div>
                </div>
                <div class="form-group ">
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label" >Documento: </label>
                    <div class="col-sm-10 col-md-10 col-lg-10 "> '. $dadosSolicitacao['doadescricao'] . ' - ' . $dadosSolicitacao['edldsc'] . ' </div>
                    <div style="clear:both"></div>
                </div>';

        if (!$novaSolicitacao) {
            $html .= '<input type="hidden" id="sdpid" value="'.$dadosSolicitacao['sdpid'].'">';
            $html .= '<div class="form-group ">
                        <label class="col-sm-2 col-md-2 col-lg-2 control-label" >Código da Solicitação: </label>
                        <div class="col-sm-10 col-md-10 col-lg-10 "> '.$dadosSolicitacao['sdpid'].' </div>
                        <div style="clear:both"></div>
                      </div>';
        }

        $html .= $simec->textarea('sdpobservacao', 'Observação', $observacao, array('maxlengh' => 4000, $disabled), array('label-size' => '2'));

        $html .= '<div class="form-group ">                     
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Valor do documento: </label>
                    <div class="col-sm-3 col-md-3 col-lg-3" id="modalValordocumento"> '.par3_mascaraMoeda($dadosSolicitacao['edlvalordocumento'], false, true).' </div>
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Saldo do termo: </label>
                    <div class="col-sm-3 col-md-3 col-lg-3" id="modalSaldoTermo"> '.par3_mascaraMoeda($saldoTermo, false, true).' </div>
                    <div style="clear:both"></div>
                  </div>
                  ';

        /*if($dadosSolicitacao['edlvalordocumento'] < $valorSolicitado){
            $valorSolicitado = $dadosSolicitacao['edlvalordocumento'];
        }*/

        $html .= '<div class="form-group ">
                    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Valor solicitado: </label>
                    <div class="col-sm-3 col-md-3 col-lg-3"> 
                        <input name="sdpvalor" id="sdpvalor" type="text" ' . $disabled . ' value="' . par3_mascaraMoeda($valorSolicitado, false, false) . '" class="form-control" maxlength="15" 
                            onkeyup="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);"
                            onblur="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);validarValorSolicitadoDocumento(this);"> 
                            <div class="erroValorSolicitado"></div>
                    </div>
                    <div style="clear:both"></div>
                  </div>';
        return $html;
    }

    public function listaHistoricoSolicitacoes($dados)
    {

        if (!$dados[0]['sdpid']) {
            $dados = array();
        }

        $cabecalho = array('ID', 'Observação', 'Valor Solicitado', 'Usuário', 'Data de criação', 'Estado');

        $dadosLista = array_map(function ($value) {
                                    return array($value['sdpid'],$value['sdpobservacao'], par3_mascaraMoeda($value['sdpvalor'], false, true), $value['usunome'], $value['datacriacao'], $value['esddsc']);
        }, $dados);

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setDados($dadosLista);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->turnOffForm();
        $listagem->turnOnPesquisator();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    /**
     * @param $dados
     */
    public function salvarSolicitacaoDesembolso($dados)
    {
        require_once APPRAIZ . '/includes/workflow.php';
        if (!$dados['sdpid']) {
            $docid = wf_cadastrarDocumento(PAR3_FLUXO_SOLICITACAO_DESEMBOLSO, 'PAR3 - Solicitação Desembolso');
        }

        $dados['sdpobservacao'] = ($dados['sdpobservacao']);
        $dados['usucpf'] = $_SESSION['usucpf'];
        $dados['docid'] = $docid;
        $this->model->popularDadosObjeto($dados);
        $this->model->salvar();
        $desembolsoSalvo = $this->model->commit();

        if ($desembolsoSalvo) {
            ob_start();
            $this->modalSolicitacaoDesembolso($dados['ecoid'], $dados['sdpid'], $dados['edlid']);
            $result = ob_get_clean();
            $result = json_decode($result, true);
            $result = simec_utf8_decode_recursive($result);
            if ($dados['ecoid'] > 0) {
                $contrato = new Par3_Controller_ExecucaoContrato();
                ob_start();
                $contrato->listaContratos($dados['proid']);
                $result['listacontratos'] = ob_get_clean();
            } elseif ($dados['edlid'] > 0) {
                $documento = new Par3_Controller_ExecucaoDocumentosLicitacao();
                ob_start();
                $documento->listarDocumentos($dados['proid']);
                $result['listadocumentos'] = ob_get_clean();
            }
            echo simec_json_encode($result);
        } else {
            echo 'falha';
        }
    }

    public function removerSolicitacaoDesembolso($dados)
    {
        $proid = $dados['proid'];
        $campos['sdpid'] = $dados['sdpid'];
        $campos['sdpstatus'] = 'I';
        $this->model->popularDadosObjeto($campos);
        $this->model->salvar();
        if ($this->model->commit()) {
            //ob_start();
            //$this->modalSolicitacaoDesembolso($dados['ecoid'], $dados['sdpid'], $dados['edlid']);
            //$result = ob_get_clean();
            //$result = json_decode($result,true);
            //$result = simec_utf8_decode_recursive($result);
            if ($proid && $dados['ecoid'] > 0) {
                $contrato = new Par3_Controller_ExecucaoContrato();
                ob_start();
                $contrato->listaContratos($proid);
                $result['listacontratos'] = ob_get_clean();
            } elseif ($proid && $dados['edlid'] > 0) {
                $documento = new Par3_Controller_ExecucaoDocumentosLicitacao();
                ob_start();
                $documento->listarDocumentos($proid);
                $result['listadocumentos'] = ob_get_clean();
            }
            echo simec_json_encode($result);
        } else {
            echo 'falha';
        }
    }

    /**
     * Aba Solicitação de Desembolso (lista geral)
     * @param $filtros
     */
    public function listarSolicitacoes($filtros)
    {
        /*if($filtros['tipo'] == 'OBRA'){
            $sql = $this->model->montaSqlListaSolicitacoesObra($filtros);
            $cabecalho = array('Solicitação', 'Processo', 'Termo', 'Ano', 'Vigência do termo',
                'CNPJ', 'Percentual Solicitado ', 'Data solicitação', 'Situação');
        }else{*/
        $sql = $this->model->montaSqlListaSolicitacoes($filtros);
        $cabecalho = array('Processo', 'Termo', 'Ano', 'Vigência do termo', 'Documento',
            'CNPJ', 'Valor do documento', 'Valor da solicitação', 'Saldo do contrato', 'Valor Termo', 'Data solicitação', 'Situação');

        $perfis = pegaPerfilGeral($_SESSION['usucpf']);
        $arrPerfisPagamento = [PAR3_PERFIL_SUPER_USUARIO, PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR, PAR3_PERFIL_ADMINISTRADOR];

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas('arqid', 'edlid', 'ecoid', 'sdpid', 'vlr_pago', 'per_sol', 'vlr_emp', 'per_emp', 'vlr_processo', 'per_processo', 'tipo', 'proid', 'esdid');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo(['valordocumento', 'valorsolicitacao', 'saldo'], 'par3_mascaraMoeda');
        $listagem->addCallbackDeCampo('pronumeroprocesso', 'formata_numero_processo_sem_html');
        $listagem->addCallbackDeCampo('cnpj', 'formatar_cnpj');
        $listagem->addCallbackDeCampo('dotvalortermo', 'Par3_Controller_SolicitacaoDesembolso::formata_valor_html_pagamentos');
        $listagem->addAcao('download', ['func' => 'acaoDownload', 'titulo' => 'Download do Documento', 'extra-params' => ['arqid'], 'cor' => 'success']);
        $listagem->addAcao('edit', ['func' => 'modalSolicitacaoDesembolso', 'titulo' => 'Editar / Visualizar Solicitação', 'extra-params' => ['sdpid','edlid']]);
        if(in_array(PAR3_PERFIL_SUPER_USUARIO, $perfis) ||
           in_array(PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR, $perfis) ||
           in_array(PAR3_PERFIL_ADMINISTRADOR, $perfis)){
            $listagem->addAcao('pagamento', ['func' => 'redirecionarPagamentos', 'titulo' => 'Ir para Pagamentos', 'extra-params' => ['proid', 'tipo'], 'cor' => 'warning']);
            $listagem->setAcaoComoCondicional('pagamento', array(array('campo' => 'esdid', 'valor' => PAR3_DESEMBOLSO_APROVADO, 'op' => 'igual')));
        }
        $listagem->setAcaoComoCondicional('edit', array(array('campo' => 'sdpid', 'valor' => '', 'op' => 'diferente')));
        $listagem->setAcaoComoCondicional('download', array(array('campo' => 'sdpid', 'valor' => '', 'op' => 'diferente')));
        $listagem->turnOnPesquisator();
        $listagem->setFormFiltros('form_solicitacoes');
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }

    public static function formata_valor_html_pagamentos($str, $dados)
    {
        $str = par3_mascaraMoeda($str, false, true);

        $arrCampos = ['per_sol', 'vlr_pago', 'vlr_emp', 'per_emp', 'vlr_processo', 'per_processo'];

        $html = '<table class="table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive">';
        $html .= '<thead><th>% Aprovado</th><th>Valor Pagamento</th><th>Valor Empenho</th><th>% Pago Empenhado</th><th>Valor Processo</th><th>% Pago Processo</th></thead><tbody><tr>';

        foreach ($dados as $coluna => $valor) {
            if (in_array($coluna, $arrCampos)) {
                $valor = par3_mascaraMoeda($valor);
                $html .= "<td>{$valor}</td>";
            }
        }

        $html .= '</tr></tbody></table>';

        return <<<HTML
    <div class="btn-group dropup" >
      <span type="button" class="btn btn-secondary dropdown-toggle" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
        {$str}<br><i style="color: #64a0e8;" class="fa fa-info-circle"></i>
      </span>
      <div class="dropdown-menu" style="padding: 8px">
        {$html}
      </div>
    </div>
HTML;

        /*return <<<HTML
        <div
        data-toggle="popover"
        data-trigger="hover"
        data-placement="top"
        data-html="true"
        data-content='{$html}'
        >{$str}<i style="color: #64a0e8;" class="fa fa-question-circle fa-2x"></i></div>
    HTML;*/
    }

    public function sqlSituacaoAnalise()
    {
        return $this->model->pegaSqlSituacaoAnalise();
    }

}
