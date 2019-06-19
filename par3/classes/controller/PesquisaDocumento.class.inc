<?php

class Par3_Controller_PesquisaDocumento extends Controle
{

    private $model;
    public function __construct()
    {
        $this->model = new Par3_Model_ExecucaoAnaliseDocumentoLicitacao();
    }

    public function salvarAnalise($arrPost)
    {
        parse_str($arrPost['dados'], $dados);

        $this->model->popularDadosObjeto($dados);
        $this->model->salvar();
        $sucesso = $this->model->commit();

        if ($dados['eadparecer'] != '' && $dados['esdid'] == PAR3_ESD_DOCUMENTO_AGUARDANDO_ANALISE) {
            require_once APPRAIZ . '/includes/workflow.php';
            $aedid = wf_pegarAcao(PAR3_ESD_DOCUMENTO_AGUARDANDO_ANALISE, PAR3_ESD_DOCUMENTO_EMANALISE);
            wf_alterarEstado($dados['docid'], $aedid['aedid'], '', array('edlid'=>$dados['edlid']));
        }

        if ($sucesso) {
//            simec_redirecionar('par3.php?modulo=principal/planoTrabalho/planejamento/pesquisarDocumento&acao=A', 'success');
            wf_desenhaBarraNavegacaoBootstrap($dados['docid'], array('edlid' => $dados['edlid']), null, 'Fluxo');
        } else {
            echo 'falha';
//            simec_redirecionar('par3.php?modulo=principal/planoTrabalho/planejamento/pesquisarDocumento&acao=A', 'error');
        }
    }

    public function listarDocumentosAnalise($arrPost)
    {
        $sql = $this->model->montarSQLSimples($arrPost);

        if ($sql != '') {
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
            $cabecalho = array('Análise','Processo', 'Número do Termo', 'UF', 'Entidade', 'Tipo de documento', 'Descrição', 'Iniciativa', 'Data de Inclusão', 'Quantidade de Itens', 'Valor do Documento', 'Situação Análise');
            $listagem->setCabecalho($cabecalho);
            $listagem->setQuery($sql);
            $listagem->esconderColunas('arqid', 'esdid');
            $listagem->setAcaoComoCondicional('remove', array(array('campo' => 'esdid', 'valor' => PAR3_ESD_DOCUMENTO_AGUARDANDO_ANALISE, 'operacao' => 'igual')));
            $listagem->addCallbackDeCampo('valor', 'par3_mascaraMoeda');
            $listagem->addCallbackDeCampo('pronumeroprocesso', 'formata_numero_processo_component');
            $listagem->addAcao('download', ['func' => 'acaoDownload', 'titulo' => 'Download', 'extra-params' => ['arqid'], 'cor' => 'success']);
//            $listagem->addAcao('analise', ['func' => 'acaoDownload', 'titulo' => 'Download', 'extra-params' => ['arqid']]);
            $listagem->turnOnPesquisator();
            $listagem->setFormFiltros('form_documentos');
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagem->setTamanhoPagina(30);
            $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        } else {
            $html = "<div class='row'>";
            $html .= "<div class='col-md-4'></div>";
            $html .= "<div class='col-md-4'><div class=\"alert alert-info\" role=\"alert\">Nenhum registro encontrado!</div></div>";
            $html .= "<div class='col-md-4'></div>";
            $html .= "</div>";
            echo $html;
        }
    }

    public function sqlSituacaoAnalise()
    {
        $sql = "SELECT codigo, descricao FROM(
				SELECT DISTINCT 
						e.esdid AS codigo,
					    e.esddsc AS descricao,
					    e.esdordem
					FROM workflow.estadodocumento e 
					WHERE e.tpdid = " . PAR3_FLUXO_VALIDACAO_DOCUMENTOS . "
					ORDER BY e.esddsc ASC
				) AS foo
				ORDER BY descricao ASC";
        return $sql;
    }

    public function downloadDocumento($arqid)
    {
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec();
        try {
            $file->getDownloadArquivo($arqid);
        } catch (Exception $e) {
            simec_redirecionar('par3.php?modulo=principal/planoTrabalho/planejamento/pesquisarDocumento&acao=A');
        }
    }

    public function modalAnaliseDocumento($dados)
    {

        require_once APPRAIZ . '/includes/workflow.php';
        global $simec;
        $dadosAnalise = $this->model->recuperarAnaliseDocumento($dados['edlid']);
        $estadosPermitidos = [PAR3_ESD_DOCUMENTO_AGUARDANDO_ANALISE, PAR3_ESD_DOCUMENTO_EMANALISE];
        $perfil = pegaPerfilGeral($_SESSION['usucpf']);
        $html = '';

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

        ### Lista itens composição
        $arrItens = $this->model->carregaItensComposicao($dados['edlid']);
        if ($arrItens) {
            $html .= "<div class=\"float-e-margins\" id=\"itens-composicao\">
                        <div class=\"ibox-title\" id=\"btn-itens-composicao\">
                            <div class=\"ibox-tools\"><a>Opções Avançadas <i id=\"i-itens-composicao\" class=\"fa fa-chevron-down\"></i></a>
                            </div>
                            <h3 class=\"\">Itens de Composição</h3>
                        </div>
                        <div class=\"ibox-content\" id=\"div-itens-composicao\" style=\"display: none;\">";

            $cabecalho = array('Descrição do Item', 'Ano', 'Unidade de Medida', 'Quantidade Aprovada', 'Valor de Referência', 'Valor Total');
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
            $listagem->setCabecalho($cabecalho);
            $listagem->setDados($arrItens);
            $listagem->addCallbackDeCampo(['ipivalorreferencia','valortotal'], 'par3_mascaraMoeda');
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagem->setFormOff();
            $listagem->turnOnPesquisator();
            ob_start();
            $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
            $html .= ob_get_clean();
            $html .= "</div></div>";
        }

        ### Dados do Parecer

        $html .= "<table border=\"0\" class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
        <tr>
            <td style=\"width: 100%\">
                <div class=\"ibox-content\" style='border:0;'>
                    <div class=\"row\">
                        <div class=\"form-group\" id=\"cic-div\">
                            <label class=\"col-md-offset-1 col-lg-offset-1 col-lg-2 control-label\"
                                   style=\"font-size: 18px; font-weight: bold;\">Parecer da Análise:</label>
                        </div>
                    </div>
                </div>
                <form name=\"form_analise_documentos_parecer\" id=\"form_analise_documentos_parecer\"
                      class=\"form-horizontal\" method=\"post\">
                    <input type=\"hidden\" name=\"requisicao\" id=\"requisicao_analise\" value=\"salvaranalise\">
                    <input type=\"hidden\" name=\"edlid\" id=\"edlid\" value=\"{$dadosAnalise['edlid']}\">
                    <input type=\"hidden\" name=\"eadid\" id=\"eadid\" value=\"{$dadosAnalise['eadid']}\">
                    <input type=\"hidden\" name=\"docid\" id=\"docid\" value=\"{$dadosAnalise['docid']}\">
                    <input type=\"hidden\" name=\"esdid\" id=\"esdid\" value=\"{$dadosAnalise['esdid']}\">";

                    $html .= $simec->textarea('eadproponente', 'Considerações sobre a Entidade Proponente', $dadosAnalise['eadproponente'], $attribus, array('label-size' => '2'));
                    $html .= $simec->textarea('eadproposta', 'Considerações sobre a Proposta', $dadosAnalise['eadproposta'], $attribus, array('label-size' => '2'));
                    $html .= $simec->textarea('eadprojeto', 'Considerações sobre o Projeto', $dadosAnalise['eadprojeto'], $attribus, array('label-size' => '2'));
                    $html .= $simec->textarea('eadobjetivo', 'Considerações sobre o Objetivo', $dadosAnalise['eadobjetivo'], $attribus, array('label-size' => '2'));
                    $html .= $simec->textarea('eadjustificativa', 'Considerações sobre a Justificativa', $dadosAnalise['eadjustificativa'], $attribus, array('label-size' => '2'));
                    $html .= $simec->textarea('eadvalores', 'Considerações sobre os Valores', $dadosAnalise['eadvalores'], $attribus, array('label-size' => '2'));
                    $html .= $simec->textarea('eadcabiveis', 'Outras Considerações Cabíveis', $dadosAnalise['eadcabiveis'], $attribus, array('label-size' => '2'));

                    /*$arParecer = array('A' => 'Aprovado',
                        'R' => 'Reprovado',
                        'D' => 'Diligência'
                    );

                    $html .= $simec->select('eadsituacao', 'Situação do Parecer', $dadosAnalise['eadsituacao'], $arParecer, $attribusCombo, array('label-size' => '2'));*/
                    $html .=
                    "<div class=\"form-group \">
                        <label for=\"eadparecer\" class=\"col-sm-2 col-md-2 col-lg-2 control-label\">Texto do parecer *: </label>
                        <div class=\"col-sm-10 col-md-10 col-lg-10 \">
                            <textarea name=\"eadparecer\" id=\"eadparecer\" class=\"form-control\"
                                      maxlengh=\"4000\" required {$readonly}>{$dadosAnalise['eadparecer']}</textarea>
                        </div>
                        <div style=\"clear:both\"></div>
                    </div>
                </form>
            </td>
            <td style=\"text-align: center; padding: 15px;\" valign=\"center\" id='barranavegacaoworkflow'>";

        if (!in_array(PAR3_PERFIL_SUPER_USUARIO, $perfil)) {
            $ocultar = ['historico' => true];
        }

            ob_start();
            wf_desenhaBarraNavegacaoBootstrap($dadosAnalise['docid'], array('edlid' => $dados['edlid']), $ocultar, 'Fluxo', 'modalAnaliseDocumento(' . $dados['edlid'] . ')');
            $html .= ob_get_clean();
            $html .= "</td>
                    </tr>
                </table>";

            echo $html;
    }

    public function sqlFormListaIniciativas(){
        return $this->model->sqlFormListaIniciativas();
    }

    public function sqlFormListaTipoObjeto(){
        return $this->model->sqlFormListaTipoObjeto();
    }
}
