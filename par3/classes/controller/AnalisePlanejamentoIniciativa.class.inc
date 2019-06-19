<?php
require_once APPRAIZ . '/includes/workflow.php';

class Par3_Controller_AnalisePlanejamentoIniciativa extends Controle
{

    private $anaid;

    public function __construct($anaid = '')
    {
        $this->model = new Par3_Model_AnalisePlanejamentoIniciativa($anaid);
        $this->anaid = $anaid;
    }

    public function recuperar($arrPost)
    {
        return $this->model;
    }

    public function salvar($arrPost)
    {
        global $url;
        
        $anaid = $arrPost['anaid'];
        
        $docid = $this->pegaIdWorkFlow($anaid);
        $arrPost['docid'] = $docid;
        
        $arEstado = $this->carregaEsdidAtual($docid);
        
        try {
            $anaid = $this->model->salvar($arrPost);
            $url = $url . '&inpid=' . $_REQUEST['inpid'] . '&inuid=' . $_REQUEST['inuid'] . '&iniano=' . $_REQUEST['iniano'] . '&aba=' . $_REQUEST['aba'] . '&anaid=' . $anaid;
            
            if ($arEstado['esdid'] == PAR3_ESDID_PLANEJAMENTO_AGUARDANDO_ANALISE) {
                
                if (wf_alterarEstado($docid, 5152, 'Fluxo de Iniciativas do PAR3', array(
                    'inpid' => $_REQUEST['inpid'],
                    'anaid' => $anaid
                ))) {
                    simec_redirecionar($url, 'success');
                } else {
                    simec_redirecionar($url, 'error');
                }
            } else {
                simec_redirecionar($url, 'success');
            }
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($_SERVER['HTTP_REFERER'], 'error');
        }
        
        return $anaid;
    }

    public function verificaExisteAnalisePlanejamento($inpid, $ano)
    {
        $sql = "select a.anaid from par3.analise a where a.inpid = $inpid and a.anaano = $ano";
        $anaid = $this->model->pegaUm($sql);
        
        return $anaid;
    }

    public function executar_acao_workflow_analise($arrPost)
    {
        global $url;
        
        $anaid = $arrPost['anaid'];
        $docid = $this->pegaIdWorkFlow($anaid);
        $arrPost['docid'] = $docid;
        
        $aedid = $arrPost['aedid_analise'];
        
        $arAedid_Diligencia = array(
            5154,
            5156,
            5160
        );
        
        $esdid = $this->carregaEsdidAtual($docid)['esdid'];
        $boAcaoSalvar = array(
            PAR3_ESDID_ANALISE_DILIGENCIA,
            PAR3_ESDID_PLANEJAMENTO_ANALISE,
            PAR3_ESDID_PLANEJAMENTO_AGUARDANDO_ANALISE
        );
        
        if (in_array($esdid, $boAcaoSalvar)) {
            $anaid = $this->model->salvar($arrPost);
        }
        $arrParamWork = array(
            'inpid' => $_REQUEST['inpid'],
            'anaid' => $anaid
        );
        $url = $url . '&inpid=' . $_REQUEST['inpid'] . '&inuid=' . $_REQUEST['inuid'] . '&iniano=' . $_REQUEST['iniano'] . '&aba=' . $_REQUEST['aba'] . '&anaid=' . $anaid;
        
        if (wf_alterarEstado($docid, $aedid, 'Fluxo de Iniciativas do PAR3', $arrParamWork)) {
            simec_redirecionar($url, 'success');
        } else {
            simec_redirecionar($url, 'error');
        }
        
        return $anaid;
    }

    public function carregaEsdidAtual($docid)
    {
        if (empty($docid)) {
            $docid = $this->pegaIdWorkFlow($this->anaid, PAR3_ESDID_PLANEJAMENTO_AGUARDANDO_ANALISE); // Aguardando Analise
        }
        
        $arEstato = $this->model->carregaEsdidAtual($docid);
        
        return $arEstato;
    }

    public function carregarAnalisePlanejamento()
    {
        if ($this->anaid)
            return $this->model->carregarAnalisePlanejamento($this->anaid);
    }

    public function carregarAnalisePlanejamentoDiligencia($inpid, $boolEsdidDiligencia = true)
    {
        if ($inpid)
            return $this->model->carregarAnalisePlanejamentoDiligencia($inpid, $boolEsdidDiligencia);
    }

    public function pegaIdWorkFlow($anaid, $esdid = '')
    {
        if ($anaid) {
            $this->anaid = $anaid;
            $arDados = $this->carregarAnalisePlanejamento($anaid);
            
            $docid = $arDados['docid'];
        } else {
            // descrição do documento
            $docdsc = "Fluxo de Iniciativas do PAR3 - Análise Planejamento: " . $anaid;
            
            // cria documento do WORKFLOW
            $docid = wf_cadastrarDocumento(310, $docdsc, $esdid);
            $_REQUEST['docid'] = $docid;
            
            $anaid = $this->model->salvar($_REQUEST);
            return $anaid;
        }
        
        return $docid;
    }

    public function carregaBotaoAcaoWorkFlow()
    {
        $arAnalise = $this->carregarAnalisePlanejamento();
        $arEstado = $this->carregaEsdidAtual($arAnalise['docid']);
        
        $arWork = $this->model->carregaBotaoAcaoWorkFlow($arEstado['esdid']);
        // ver($arWork,d);
        $html = '';
        foreach ($arWork as $v) {
            $html .= '<button type="button" id="btn-acao-workflow" data-dismiss="modal" class="btn btn-primary" onclick="pegaAcaoWorkFlow(' . $v['aedid'] . ', ' . $arAnalise['docid'] . ');" data-loading-text="' . $v['aeddscrealizada'] . ', aguarde ..."><i class="fa fa-times-circle-o"></i> ' . $v['aeddscrealizar'] . '</button>&nbsp;&nbsp;';
        }
        
        return $html;
    }

    public function carregaAnaliseValidacaoCoordenador($arrPost = array())
    {
        $sql = $this->model->sqlCarregaAnaliseValidacaoCoordenador($arrPost);

        $cabecalho = array(
            'Selecionar Todos <input type="checkbox" id="selecionarTodos" onclick="javascript: marcarTodosChecks( \'selecionarTodos\', \'checklote\' );" />',
            'Análise',
            'Planejamento',
            'Ano',
            'UF',
            'Entidade',
            'Iniciativa',
            'Tipo de Objeto',
            'Tipo de assistência',
            'Valor',
            'Situação',
            'Técnico',
            'Data de Análise',
        );
        $esconderColunas = array(
            'iniid',
            'inuid',
            'anaid',
            'etapa',
            'programas'
        );
        
        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setFormFiltros('form-iniciativaplanejamento');
        //$listagem->setTamanhoPagina(30);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addCallbackDeCampo('vlr_composicao', 'formataNumeroMonetarioComSimbolo');
        $listagem->setCampos($cabecalho);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        
    }

    /**
     * Valida se as obras que irão para aprovação possuem o mesmo Estado Documento
     *
     * @param array $obras
     * @return bool
     */
    public function validarAprovacaoLote(array $analises)
    {
        include_once APPRAIZ . "includes/workflow.php";
        
        $arrAnaId = array();
        
        $arrEsd = array();
        $docid = 0;
        $arrEstDocs = array();
        $arrAnalise = array();
        
        $arrDocid = array();
        foreach ($analises as $anaid) {
            
            $docid = $this->pegaIdWorkFlow($anaid);
            
            $arrEsd = wf_pegarEstadoAtual($docid);
            
            if (count($arrEstDocs) > 0 && ! in_array($arrEsd, $arrEstDocs)) {
                
                return 0;
            }
            
            $arrProximosEstados = wf_pegarProximosEstados($docid);
            
            array_push($arrEstDocs, $arrEsd);
            
            array_push($arrDocid, array(
                'anaid' => $anaid,
                'docid' => $docid
            ));
            
            array_push($arrAnaId, $anaid);
        }
        
        require APPRAIZ . 'par3/modulos/principal/planoTrabalho/planejamento/aprovacaoLoteAnalise.inc';
    }

    public function verificarParametrosPosAcao($anaid)
    {
        $this->anaid = $anaid;
        $analisePlanejamento = $this->carregarAnalisePlanejamento();
        
        $arrParametros = array(
            'inpid' => $analisePlanejamento['inpid'],
            'anaid' => $anaid,
            'iniano' => $analisePlanejamento['anaano']
        );
        
        return $arrParametros;
    }

    /**
     * Realiza tramitação em lote para documentos que possuem o mesmo estadodocumento
     *
     * @param array $arrDocid
     * @param
     *            $aedid
     * @param string $comentario
     */
    public function tramitarEmLote(array $arrDocid, $aedid, $comentario = '')
    {
        include_once APPRAIZ . "includes/workflow.php";
        $retorno = array();
        
        foreach ($arrDocid as $arrDoc) {
            
            $arrParametros = $this->verificarParametrosPosAcao($arrDoc['anaid']);
            
            $retorno[] = wf_alterarEstado($arrDoc['docid'], $aedid, $comentario . (' (Tramitado em lote)'), $arrParametros);
        }
        
        if (in_array(0, $retorno)) {
            return 0;
        }
        return 1;
    }

    public function tramitaAnaliseLote($arrPost = array())
    {
        // $aedid = 5160; #Enviar para diligência
        foreach ($arrPost['checklote'] as $anaid) {
            $this->anaid = $anaid;
            $docid = $this->pegaIdWorkFlow($anaid);
            $aedid = 5161; // Enviar para planejamento aprovado
            
            wf_alterarEstado($docid, $aedid, 'Fluxo de Iniciativas do PAR3', array(
                'inpid' => '',
                'anaid' => $anaid
            ));
        }
        
        simec_redirecionar($_SERVER['HTTP_REFERER'], 'success');
    }

    public function modalHistorico($dados = [])
    {
        global $db;
        $sqlListagem = 'SELECT
                        anaconsideracaoent AS "Considerações sobre a Entidade Proponente", 
                        anaconsideracaoprop AS "Considerações sobre a Proposta", 
                        anaconsideracaoproj AS "Considerações sobre o Projeto", 
                        anaconsideracaoobj AS "Considerações sobre o Objetivo", 
                        anaconsideracaojus AS "Considerações sobre a Justificativa", 
                        anaconsideracaoval AS "Considerações sobre os Valores", 
                        anaoutrasconsideracoes AS "Outras Considerações Cabíveis", 
                        anatextoparecer AS "Texto do parecer", 
                        CASE WHEN anaparecer = \'A\' THEN \'Aprovado\'
                             WHEN anaparecer = \'R\' THEN \'Reprovado\'
                             WHEN anaparecer = \'D\' THEN \'Diligência\' END AS "Situação do Parecer"
                        FROM 
                          par3.analise_historico 
                        WHERE 
                          inpid = ' . $dados['inpid'] . '
                          AND anaano = \'' . $dados['anaano'] . '\';';
        
        $sqlEstado = "SELECT estadodocumento.esddsc 
                      FROM par3.analise analise
                      JOIN workflow.documento documento ON documento.docid = analise.docid
                      JOIN workflow.estadodocumento estadodocumento ON estadodocumento.esdid = documento.esdid
                      WHERE analise.inpid = {$dados['inpid']} AND analise.anaano = {$dados['anaano']}";
        $esddsc = $db->pegaUm($sqlEstado);
        
        // Listagem
        ob_start();
        echo "<div>Situação Atual: <b>{$esddsc}</b></div><br><br>";
        $listagem = new Simec_Listagem();
        $listagem->setCabecalho('auto');
        $listagem->setQuery($sqlListagem);
        $listagem->setId('anhid');
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        echo "</div>";
        $html = ob_get_clean();
        
        // Resposta json
        $response = simec_json_encode([
            'html' => $html
        ]);
        echo $response;
    }
}