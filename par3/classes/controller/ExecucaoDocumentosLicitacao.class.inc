<?php

/**
 * Class Par3_Controller_ExecucaoDocumentosLicitacao
 */


class Par3_Controller_ExecucaoDocumentosLicitacao extends Controle
{

    public $model;

    public function __construct()
    {
        $this->model = new Par3_Model_ExecucaoDocumentosLicitacao();
    }

    public function controlarRequisicoes($requisicao, $proid)
    {
        $retorno = '';
        switch ($requisicao) {
            case 'salvarDocumento':
                $retorno = $this->salvarDocumento($_REQUEST);
                break;
            case 'modalDocumento':
                $retorno = $this->modalDocumento($_REQUEST, $proid);
                break;
            case 'modalAnalise':
                $retorno = $this->modalAnalise($_REQUEST['edlid']);
                break;
            case 'salvarAnalise':
                $retorno = $this->salvarAnalise($_REQUEST, $proid);
                break;
            case 'atualizarListagensDocumento':
                $retorno = $this->atualizarListagensDocumento($proid);
                break;
            case 'dadosAdesao':
                $retorno = $this->dadosAdesao($_REQUEST);
                break;
            case 'visualizarDocumentosItem':
                $retorno = $this->visualizarDocumentosItem($_REQUEST['aicid']);
                break;
            case 'desativarDocumentoLicitacao':
                $retorno = $this->desativarDocumentoLicitacao($_REQUEST['edlid'], $proid);
                break;
        }

        return $retorno;
    }

    /**
     * Lista todos os documentos de licitação salvos para este processo
     * @param $proid
     */
    public function listarDocumentos($proid)
    {

        $sql = "SELECT DISTINCT ON(edl.edlid)
                    edl.edlid,
                    edl.arqid,
                    doadescricao,
                    edldsc as descricao,
                    '<span style=\'color:blue; cursor:pointer;\' onclick=\'downloadArqLicitacao('||edl.edlid||','||arq.arqid||')\'>' || arqnome || '.' || arqextensao || '</span>' as anexo,
                    to_char(edl.edldtainclusao, 'dd/mm/YYYY') as data,
                    COALESCE(qtd::text, '-') as qtditens,
                    COALESCE(edlvalordocumento::text, '-') as valor,
                    CASE WHEN ultimaSituacao is not null
                      THEN ultimaSituacao
                      ELSE '-'
                    END AS situacaodesembolso,
                    CASE WHEN doaanalise = TRUE
                      THEN esd.esddsc
                      ELSE '-'
                    END AS situacao,
                    CASE WHEN doaanalise = TRUE
                      THEN '<span style=\'display:block; cursor:pointer;\' onclick=\'modalAnalise('||edl.edlid||')\'><i class=\'fa fa-clipboard btn btn-lg\'></i></span>'
                      ELSE '-'
                    END as analise,
                    esd.esdid,
                    doadesembolso
                FROM par3.execucao_documentolicitacao edl
                INNER JOIN arquivo arq ON arq.arqid = edl.arqid
                LEFT JOIN par3.tipo_documento_anexo doa ON doa.doaid = edl.doaid
                LEFT JOIN par3.execucao_documentolicitacao_itens edi ON edi.edlid = edl.edlid
                LEFT JOIN par3.execucao_analise_documentolicitacao ead ON ead.edlid = edl.edlid AND eadstatus = 'A'
                LEFT JOIN workflow.documento doc on doc.docid = edl.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                LEFT JOIN LATERAL (SELECT sum(t.aicqtdaprovado * t.aicvaloraprovado) as valordoc, sum(aicqtdaprovado) as qtd
                          FROM par3.analise_itemcomposicao t
                          INNER JOIN par3.execucao_documentolicitacao_itens t2 ON t2.aicid = t.aicid
                          WHERE t2.edlid = edl.edlid
                          ) v on true
              LEFT JOIN LATERAL (SELECT esddsc as ultimaSituacao FROM par3.solicitacao_desembolso_par sdp
                         INNER JOIN workflow.documento doc ON sdp.docid = doc.docid
                         INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                         WHERE sdp.edlid = edl.edlid AND sdpstatus = 'A'
                         ORDER BY sdpid DESC LIMIT 1) v2 on true
                WHERE
                edlstatus = 'A'
                AND edl.proid = '{$proid}'
                ORDER BY edl.edlid DESC
         ";

        $cabecalho = array('Tipo de documento', 'Descrição', 'Anexo', 'Data de Inclusão', 'Quantidade de Itens', 'Valor do Documento', 'Situação Desembolso', 'Situação Análise', 'Análise');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas('arqid', 'esdid', 'doadesembolso');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addAcao('edit', ['func' => 'modalDocumento', 'titulo' => 'Editar Documento']);
        $listagem->addAcao('download', ['func' => 'downloadArqLicitacao', 'titulo' => 'Download', 'extra-params' => ['arqid'], 'cor' => 'success']);
        $listagem->addAcao('remove', ['func' => 'removerDcumentoLicitacao', 'titulo' => 'Remover', 'extra-params' => ['arqid']]);
        $listagem->addAcao('pagamento', ['func' => 'modalSolicitacaoDesembolsoDocumentos', 'titulo' => 'Solicitar Desembolso', 'cor' => 'warning']);
        $listagem->setAcaoComoCondicional('pagamento', array(array('campo' => 'doadesembolso', 'valor' => 't', 'op' => 'igual')));
        $listagem->setAcaoComoCondicional('remove', array(
            array('campo' => 'esdid', 'valor' => array(PAR3_ESD_DOCUMENTO_AGUARDANDO_ANALISE, PAR3_ESD_DOCUMENTO_EMDILIGENCIA), 'op' => 'contido')
        ));
        $listagem->addCallbackDeCampo('valor', 'par3_mascaraMoeda');
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    /**
     * Lista todos os itens deste termo de compromisso, por iniciativa (ação de clicar no accordion da iniciativa)
     * @param $inpid - id da iniciativa
     */
    public function listarItensComposicaoPorIniciativa($inpid)
    {

        $visualizarDocumentos = <<<HTML
<button class=" visualizar_item btn btn-success btn-sm" title="Visualizar Documentos" type="button" onclick="visualizarDocumentosItem('||aic.aicid||')"><i class="fa fa-search"></i></button>
HTML;

        $sql = "SELECT
                    itcdsc,
                    ppcano,
                    (SELECT count(e.aicid)
                      FROM par3.execucao_documentolicitacao_itens e
                      INNER JOIN par3.execucao_documentolicitacao d ON d.edlid = e.edlid
                      WHERE e.aicid = aic.aicid
                      AND edlstatus = 'A')
                    as qtddocumentos,
                    CASE WHEN ics.iasid IS NULL AND (SELECT count(e.aicid)
                                                      FROM par3.execucao_documentolicitacao_itens e
                                                      INNER JOIN par3.execucao_documentolicitacao d ON d.edlid = e.edlid
                                                      WHERE e.aicid = aic.aicid AND edlstatus = 'A') > 0
                      THEN '{$visualizarDocumentos}'
                    END AS documentos,
                    unidsc,
                    aicqtdaprovado,
                    aic.aicvaloraprovado as valorreferencia,
                    aic.aicvaloraprovado * aic.aicqtdaprovado as valortotal,
                    CASE WHEN ics.iasid IS NULL
                      THEN '-'
                      ELSE '<span onclick=\"modalAdesao(' || ics.iasid || ', ' || aic.aicid || ')\"><i class=\"glyphicon glyphicon-search\"></i> ' || ics.nu_seq_solicitacao_adesao || '</span>'
                    END AS adesao
                FROM par3.iniciativa_planejamento inp
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON inp.inpid = ipi.inpid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
                INNER JOIN par3.unidade_medida uniid ON itc.uniid = uniid.uniid
                INNER JOIN par3.processo pro ON inp.inuid = pro.inuid
                LEFT JOIN par3.execucao_documentolicitacao_itens edi ON edi.aicid = aic.aicid
                INNER JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid AND ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                LEFT JOIN par3.itemadesaosigarp ics ON itc.itcidsigarp = ics.item AND ics.proid = pro.proid and adestatus = 'A'
                WHERE
                ipi.ipistatus = 'A'
                AND aicstatus = 'A'
                AND inp.inpid = {$inpid}
                AND aicqtdaprovado > 0
                and aic.aicstatus = 'A'
                GROUP BY aic.aicid, itcdsc , itc.itcespecificacao, unidsc, ppcano, ics.iasid, ipi.ipiquantidadeaprovada, ipi.ipivalorreferencia,
                aicvaloraprovado, aicqtdaprovado, aic.aicid
                ORDER BY itcdsc
        ";

        $cabecalho = array('Descrição do Item', 'Ano', 'Documentos do item', 'Visualizar documentos', 'Unidade de Medida', 'Quantidade Aprovada', 'Valor de Referência', 'Valor Total', 'Adesão');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->turnOnPesquisator();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo(['valorreferencia','valortotal'], 'par3_mascaraMoeda');
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }


    /**
     * Ação de editar documento já anexado.
     * Os checkbox vem todos checados. Se o usuário quiser exluir, deve desmarcar os itens e salvar.
     * @param $inpid - id da iniciativa
     * @param bool $analise
     */
    public function listaItensDocumentoEdicao($inpid, $analise = false)
    {

        if ($analise) {
            $cabecalho = array('Descrição do Item', 'Ano', 'Unidade de Medida', 'Quantidade Aprovada', 'Valor de Referência', 'Valor Total');
            $INNER = "INNER ";
        } else {
            $INNER = "LEFT ";
            $campoCheckboxChecked = '<label class=\"form-check-label\" style=\"width: 100%;height: 100%;cursor: pointer;margin:0;padding:4px;\">
                           <input class=\"form-check-input\" checked type=\"checkbox\" name=\"aicid[]\"
                           value=\"\'||aic.aicid||\'\"
                           data-total=\"\'||aic.aicvaloraprovado * aic.aicqtdaprovado||\'\">
                          </label>';
            $campoCheckbox = '<label class=\"form-check-label\" style=\"width: 100%;height: 100%;cursor: pointer;margin:0;padding:4px;\">
                           <input class=\"form-check-input\"  type=\"checkbox\" name=\"aicid[]\"
                           value=\"\'||aic.aicid||\'\"
                           data-total=\"\'||aic.aicvaloraprovado * aic.aicqtdaprovado||\'\">
                          </label>';
            $sqlcheckbox = "
                            CASE WHEN ediid IS NOT NULL
                                THEN '{$campoCheckboxChecked}'
                                ELSE '{$campoCheckbox}'
                            END as checkbox,
                            ";
            $cabecalho = array('', 'Descrição do Item', 'Ano', 'Unidade de Medida', 'Quantidade Aprovada', 'Valor de Referência', 'Valor Total');
            $adesao = 'AND ics.iasid IS NULL';
        }

        $sql = "SELECT
                    {$sqlcheckbox}
                    itcdsc,
                    ppcano,
                    unidsc,
                    aic.aicqtdaprovado,
                    aic.aicvaloraprovado,
                    aic.aicvaloraprovado * aic.aicqtdaprovado as valortotal
                FROM par3.iniciativa_planejamento inp
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON inp.inpid = ipi.inpid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
                INNER JOIN par3.unidade_medida uniid ON itc.uniid = uniid.uniid
                INNER JOIN par3.processo pro ON inp.inuid = pro.inuid
                {$INNER} JOIN par3.execucao_documentolicitacao_itens edi ON edi.aicid = aic.aicid AND edi.edlid = {$_SESSION['edlid']}
                INNER JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid AND ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                LEFT JOIN par3.itemadesaosigarp ics ON itc.itcidsigarp = ics.item AND ics.proid = pro.proid and adestatus = 'A'
                WHERE
                aic.aicstatus = 'A'
                AND inp.inpid = {$inpid}
                AND aic.aicqtdaprovado > 0
                {$adesao}
                GROUP BY aic.aicid, itcdsc , itc.itcespecificacao, unidsc, ppcano, ics.iasid, ediid, ipiquantidadeaprovada, ipi.ipivalorreferencia
                ORDER BY itcdsc
         ";

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo(['ipivalorreferencia','valortotal'], 'par3_mascaraMoeda');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['ipiquantidadeaprovada','valortotal']);
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }


    /**
     * Ao selecionar um tipo de documento que depende de itens, a lista deve conter checkbox.
     * [modalAnexarDocumento]
     * @param $inpid - id da iniciativa
     */
    public function listaItensDocumentoInclusao($inpid)
    {

        //Comparando o tempo de carregamento da lista passando o html para o sql e usando a função addCallbackDeCampo, os resultados foram iguais.
        //Por enquanto.
        $campoCheckbox = '<label class=\"form-check-label\" style=\"width: 100%;height: 100%;cursor: pointer;margin:0;padding:4px;\">
                           <input class=\"form-check-input\" type=\"checkbox\" name=\"aicid[]\"
                           value=\"\'||aic.aicid||\'\"
                           data-total=\"\'||aic.aicvaloraprovado * aic.aicqtdaprovado ||\'\">
                          </label>';

        $sql = "SELECT
                    '{$campoCheckbox}' as checkbox,
                    itcdsc,
                    ppcano,
                    unidsc,
                    aic.aicqtdaprovado,
                    aic.aicvaloraprovado,
                    aic.aicvaloraprovado * aic.aicqtdaprovado as valortotal
                FROM par3.iniciativa_planejamento inp
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON inp.inpid = ipi.inpid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.iigid = ipi.iigid
                INNER JOIN par3.itenscomposicao itc ON itc.itcid = iig.itcid
                INNER JOIN par3.unidade_medida uniid ON itc.uniid = uniid.uniid
                INNER JOIN par3.processo pro ON inp.inuid = pro.inuid
                LEFT JOIN par3.execucao_documentolicitacao_itens edi ON edi.aicid = aic.aicid
                INNER JOIN par3.processoparcomposicao ppc ON pro.proid = ppc.proid AND ppc.inpid = inp.inpid AND ipi.ipiano = ppc.ppcano
                LEFT JOIN par3.itemadesaosigarp ics ON itc.itcidsigarp = ics.item AND ics.proid = pro.proid and adestatus = 'A'
                WHERE
                aic.aicstatus = 'A'
                AND inp.inpid = {$inpid}
                AND aic.aicqtdaprovado > 0
                AND ics.iasid IS NULL
                GROUP BY aic.aicid, itcdsc , itc.itcespecificacao, unidsc, ppcano, ics.iasid, aic.aicqtdaprovado, aic.aicvaloraprovado
                ORDER BY itcdsc
        ";
        $cabecalho = array('','Descrição do Item', 'Ano', 'Unidade de Medida', 'Quantidade Aprovada', 'Valor de Referência', 'Valor Total');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->turnOnPesquisator();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo(['ipivalorreferencia','valortotal'], 'par3_mascaraMoeda');
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    /**
     * Carrega dados da modal de anexo de documento e de ediçao
     * @param $dados
     * @param $proid
     */
    public function modalDocumento($dados, $proid)
    {
        global $simec;

        $modalTipoDocumento = new Par3_Model_TipoDocumentoAnexo();

        $html = '';

        ob_start();

        // - Accordion
            //edição
        if ($dados['edlid']) {
            $edicao = 'true';
            $_SESSION['edlid'] = $dados['edlid'];
            $dadosDocumento = $this->model->recuperaDocumentoLicitacao($dados['edlid']);

            // - Controle de edição
            $arrPermissoes = [PAR3_ESD_DOCUMENTO_AGUARDANDO_ANALISE, PAR3_ESD_DOCUMENTO_EMDILIGENCIA];

            if ($dadosDocumento['esdid'] > 0 && !in_array($dadosDocumento['esdid'], $arrPermissoes)) {
                $disabled = 'disabled';
            }
            // - fim controle edição

            if ($dadosDocumento['ediid'] > 0) {
                $detalharItens = 'true';
                if ($disabled) { //sem checkbox no accordion
                    $arrIniciativas = $this->model->recuperarIniciativasPorDocumentoLicitacao($dados['edlid']);
                    if ($arrIniciativas) {
                        $_SESSION['edlid'] = $dados['edlid'];
                        Par3_Helper_AccordionHelper::listagemAccordion($arrIniciativas, ['recarregarDiv' => false, 'req' => 'listaItensAnalise']);
                    }
                } else {
                    $arrIniciativas = $this->model->recuperarIniciativas($proid);
                    Par3_Helper_AccordionHelper::listagemAccordion($arrIniciativas, ['checkbox' => true, 'recarregarDiv' => false, 'req' => 'listaItensDocumentoEdicao']);
                }
            }
            $doaid = $dadosDocumento['doaid'];
            $tipoDocumento = $modalTipoDocumento->recuperarPorId($doaid);
        } else {
            $doaid = $dados['doaid'];
            $tipoDocumento = $modalTipoDocumento->recuperarPorId($doaid);

            if ($tipoDocumento['doadetalharitem'] == 't') {
                $detalharItens = 'true';
                $arrIniciativas = $this->model->recuperarIniciativas($proid);
                Par3_Helper_AccordionHelper::listagemAccordion($arrIniciativas, ['checkbox' => true, 'recarregarDiv' => false, 'req' => 'listaItensDocumentoInclusao']);
            }
        }
        $html .= ob_get_clean();
        // - Fim accordion

        $desembolso = $tipoDocumento['doadesembolso'] === 't' ? 'true' : 'false';

        $html .= "<input type='hidden' name='edicao' id='edicaoDocumento' value='{$edicao}'>";
        $html .= "<input type='hidden' name='detalharitens' id='detalharitens' value='{$detalharItens}'>";
        $html .= "<input type='hidden' name='desembolso' id='desembolso' value='{$desembolso}'>";
        if($edicao){
            $html .= "<input type='hidden' name='doaidmodal' id='doaidmodal' value='{$dadosDocumento['doaid']}'>";
        }

        $html .= "
                <div class='form-group'>
                    <label class='col-md-offset-1 col-lg-offset-1 col-sm-2 control-label'>
                        Arquivo: (pdf) *
                    </label>
                    <div class='col-md-8  col-md-8 col-lg-8'>
                        <input type='file' name='anexo' id='anexo' required='required' class='form-control' {$disabled} style='height: auto !important;'/>";

        if ($tipoDocumento['extensoes']) {
            $html .= "<div class='col-lg-12' style='padding-bottom: 10px'>Extensões: {$tipoDocumento['extensoes']}</div>";
        }

        $html .= "     <div id='listaAnexo'></div>
                    </div>
                 </div>";

        if ($dados['edlid']) {
            $html .= "
                   <div id=\"listaAnexoLicitacao\">
                      <input type='hidden' id='arqid_licitacao' value='{$dadosDocumento['arqid']}'>
                      <div class='col-lg-12 col-lg-offset-3' style='padding-bottom: 10px' id='arquivo_{$dadosDocumento['arqid']}'>
                        <div class='MultiFile-label'></div>
                        <a class='MultiFile-remove' href='#anexo'>";
            if (!$disabled) {
                $html .= "<a onclick='excluirAnexoLicitacao({$dadosDocumento['arqid']})' class='btn btn-danger btn-xs' title='Excluir'
                            aria-label='Close'><span aria-hidden='true'>×</span> </a> </a>";
            }

            $html .= "         <a title='Baixar' onclick='downloadArqLicitacao({$dados['edlid']},{$dadosDocumento['arqid']});'>
                                <i class='glyphicon glyphicon-cloud-download btn btn-warning btn-xs'></i></a>
                            <span disabled>
                                <span class='MultiFile-label' title='Selecionado: {$dadosDocumento['arqnome']}.{$dadosDocumento['arqextensao']}'>
                                    <span class='MultiFile-title'>
                                            {$dadosDocumento['arqnome']}.{$dadosDocumento['arqextensao']}
                                    </span>
                                </span>
                            </span>
                       </div>
                   </div>";
        }

        if($tipoDocumento['doadesembolso'] === 't'){
            $html .= $simec->input('edlcnpjfornecedor', 'CNPJ Fornecedor', formatar_cnpj($dadosDocumento['edlcnpjfornecedor']), array($disabled, 'onkeyup'=>"this.value=mascaraglobal('##.###.###/####-##',this.value);"), array('label-size' => '3', 'input-size' => '8'));

            $disabledEdlvalordocumento = $disabled;
            if ($tipoDocumento['doadetalharitem'] == 't') { //válido apenas para o campo edlvalordocumento
                $disabledEdlvalordocumento = 'disabled';
            }
            $html .= $simec->input('edlvalordocumento', 'Valor do documento', par3_mascaraMoeda($dadosDocumento['edlvalordocumento'], false, false), array('required', $disabledEdlvalordocumento, 'onkeyup'=>"this.value=mascaraglobal('###.###.###.###,##',this.value);"), array('label-size' => '3', 'input-size' => '8'));
        }

        $html .= $simec->textarea('descricao', 'Descrição', $dadosDocumento['edldsc'], array('required', 'rows' => 4, 'maxlength' => 500, $disabled), array('label-size' => '3', 'input-size' => '8'));

        $retorno['html'] = $html;
        $retorno['nomeDocumento'] = $tipoDocumento['doadescricao'];

        echo simec_json_encode($retorno);
    }

    /**
     * Carrega os dados para modal de análise do documento selecionado
     * @param $dados - request
     */
    public function modalAnalise($edlid)
    {

        require_once APPRAIZ . '/includes/workflow.php';
        global $simec;

        if ($edlid > 0) {
            $dadosAnalise = $this->model->recuperarDadosAnalise($edlid);
            $dadosDocumento = $this->model->recuperaDocumentoLicitacao($edlid);
        }

        $html = '<div class="row"><div class="col-sm-10 col-md-10 col-lg-10">';

        $arrIniciativas = $this->model->recuperarIniciativasPorDocumentoLicitacao($edlid);
        if ($arrIniciativas) {
            $_SESSION['edlid'] = $edlid;
            ob_start();
            Par3_Helper_AccordionHelper::listagemAccordion($arrIniciativas, ['recarregarDiv' => false, 'req' => 'listaItensAnalise']);
            $html .= ob_get_clean();
        }
        $perfil = pegaPerfilGeral($_SESSION['usucpf']);

        $podeeditar = true;
        $attribus = array('maxlengh' => 4000, 'onkeyup'=>"ajustarTamanhoTextarea(this);");
        $readonly = '';
        $msg = "<p>* Lembre-se de salvar a análise antes de tramitar seu estado</p>";
        $estadosPermitidos = [PAR3_ESD_DOCUMENTO_AGUARDANDO_ANALISE, PAR3_ESD_DOCUMENTO_EMDILIGENCIA];

        if (!in_array(PAR3_PERFIL_SUPER_USUARIO, $perfil) && !in_array(PAR3_PERFIL_ADMINISTRADOR, $perfil) && !in_array(PAR3_PERFIL_ANALISTA_PLANEJAMENTO, $perfil)) {
            $podeeditar = false;
        } else {
            $podeeditar = true;
        }

        if (!in_array($dadosDocumento['esdid'], $estadosPermitidos)) {
            $podeeditar = false;
        }

        if (!$podeeditar) {
            $readonly = 'readonly';
            $attribus = array('maxlengh' => 4000, 'readonly' => 'readonly');
            $msg = "";
            $html .= "<input type='hidden' name='edit' id='edit' value='false'>";
        }

        $html .= "<input type='hidden' name='edlid' id='edlid' value='{$edlid}'>
                  <input type='hidden' name='eadid' id='eadid' value='{$dadosAnalise['eadid']}'>
                  <input type='hidden' name='esdid' id='esdid' value='{$dadosDocumento['esdid']}'>
                  <input type='hidden' name='docid' id='docid' value='{$dadosDocumento['docid']}'>";

        $html .= $simec->textarea('eadproponente', 'Considerações sobre a Entidade Proponente', $dadosAnalise['eadproponente'], $attribus, array('label-size' => '2', 'onkeyup' => "ajustarTamanhoTextarea(this)"));
        $html .= $simec->textarea('eadproposta', 'Considerações sobre a Proposta', $dadosAnalise['eadproposta'], $attribus, array('label-size' => '2'));
        $html .= $simec->textarea('eadprojeto', 'Considerações sobre o Projeto', $dadosAnalise['eadprojeto'], $attribus, array('label-size' => '2'));
        $html .= $simec->textarea('eadobjetivo', 'Considerações sobre o Objetivo', $dadosAnalise['eadobjetivo'], $attribus, array('label-size' => '2'));
        $html .= $simec->textarea('eadjustificativa', 'Considerações sobre a Justificativa', $dadosAnalise['eadjustificativa'], $attribus, array('label-size' => '2'));
        $html .= $simec->textarea('eadvalores', 'Considerações sobre os Valores', $dadosAnalise['eadvalores'], $attribus, array('label-size' => '2'));
        $html .= $simec->textarea('eadcabiveis', 'Outras Considerações Cabíveis', $dadosAnalise['eadcabiveis'], $attribus, array('label-size' => '2'));
        $html .= $simec->textarea('eadparecer', 'Parecer', $dadosAnalise['eadparecer'], array('required'=>'required', $readonly), array('label-size' => '2'));

        $html .= "</div>
              <div class='col-sm-2 col-md-2 col-lg-2'>";

        ob_start();
        wf_desenhaBarraNavegacaoBootstrap($dadosDocumento['docid'], array('edlid' => $edlid));
        $html .= ob_get_clean();

        $html .= "</div></div>";

        echo $html;
    }

    public function salvarDocumento($dados)
    {
        $edicao = $dados['edlid'] > 0 ? true : false;
        $modalTipoDocumento = new Par3_Model_TipoDocumentoAnexo();
        $dadosTipoDocumento = $modalTipoDocumento->recuperarPorId($dados['doaid']);

        $campos = [
            'edldsc' => ($dados['descricao']),
            'proid' => $dados['proid'],
            'doaid' => $dados['doaid'],
            'usucpfinclusao' =>  $_SESSION['usucpf'],
            'edlcnpjfornecedor' => $dados['edlcnpjfornecedor'],
        ];

        if($dados['edlvalordocumento'] > 0){
            $campos['edlvalordocumento'] = $dados['edlvalordocumento'];
        }

        if ($edicao) {
            $campos['edlid'] = $dados['edlid'];
        }

        if (count($_FILES['file']['type']) > 0) {
            if ($dadosTipoDocumento['extensoes']) {
                $allowed = explode(',', str_replace(' ', '', $dadosTipoDocumento['extensoes']));
                $filename = $_FILES['file']['name'];
                $ext = pathinfo($filename, PATHINFO_EXTENSION);
                if (!in_array(trim($ext), $allowed)) {
                    echo 'filetype';
                    exit();
                }
            }

            //Se o usuário anexar outro arquivo na edição do documento, exclui o arquivo físico anterior
            if ($edicao) {
                $file = new FilesSimec('execucao_documentolicitacao', null, 'par3');
                $file->excluiArquivoFisico($dados['arqid']);
            }

            $_FILES['file']['name'] = ($_FILES['file']['name']); // Necessário devido ao 'charset:false' do ajax, que é necessário para mandar o arquivo no próprio ajax.
            $file = new FilesSimec('execucao_documentolicitacao', null, 'par3');
            $file->setUpload('PAR3 - Documentos Licitação ', null, false);
            $campos['arqid'] = $file->getIdArquivo();
        }

        if ($dadosTipoDocumento['doaanalise'] == 't') {
            require_once APPRAIZ . '/includes/workflow.php';

            if ($edicao) { //para atualizar docids dos documentos já existentes. pode ser retirado depois.
                $model = new Par3_Model_ExecucaoDocumentosLicitacao($dados['edlid']);
                if ($model->docid < 1) {
                    $docid = wf_cadastrarDocumento(PAR3_FLUXO_VALIDACAO_DOCUMENTOS, 'Validaçao de Documentos');
                }
            } else {
                $docid = wf_cadastrarDocumento(PAR3_FLUXO_VALIDACAO_DOCUMENTOS, 'Validaçao de Documentos');
            }

            if ($docid) {
                $campos['docid'] = $docid;
            }
        }

        //cálculo para inserir o valor total dos itens.
        /*if ($dados['ipiid'] && $dadosTipoDocumento['doadetalharitem'] == 't') {
            $arrValorItens = $this->model->buscaValorItens($dados['ipiid']);
            $totalValorItens = 0;
            foreach ($arrValorItens as $itens){
                $totalValorItens = bcadd($totalValorItens, $itens['valorItem']);
            }
            $campos['edlvalordocumento'] = $totalValorItens;
        }*/

        $this->model->popularDadosObjeto($campos);
        $idDoc = $this->model->salvar();
        $this->model->commit();

        //após se ter o id do documento, insere os itens na tabela associativa
        if ($dados['aicid'] && $dadosTipoDocumento['doadetalharitem'] == 't') {
            $arrValorItens = $this->model->buscaValorItens($dados['aicid']);
            $this->model->inserirTabelaAssociativa($idDoc, $arrValorItens);
        }


        if ($idDoc > 0) {
            echo 'sucesso';
        } else {
            echo 'falha';
        }
    }

    public function salvarAnalise($dados, $proid)
    {
        $dadosObjeto['edlid'] = $dados['edlid'];
        $dadosObjeto['eadparecer'] = ($dados['eadparecer']);
        $dadosObjeto['eadproponente'] = ($dados['eadproponente']);
        $dadosObjeto['eadproposta'] = ($dados['eadproposta']);
        $dadosObjeto['eadprojeto'] = ($dados['eadprojeto']);
        $dadosObjeto['eadobjetivo'] = ($dados['eadobjetivo']);
        $dadosObjeto['eadjustificativa'] = ($dados['eadjustificativa']);
        $dadosObjeto['eadvalores'] = ($dados['eadvalores']);
        $dadosObjeto['eadcabiveis'] = ($dados['eadcabiveis']);
        $dadosObjeto['usucpfinclusao'] = $_SESSION['usucpf'];
        $dadosObjeto['eadid'] = $dados['eadid'];

        //inativa análises anteriores
/*        $sql = "UPDATE par3.execucao_analise_documentolicitacao SET eadstatus = 'I' WHERE edlid = {$dadosObjeto['edlid']}";
        $this->model->executar($sql);
        $this->model->commit();
*/
        $modelAnaliseTermoReferencia = new Par3_Model_ExecucaoAnaliseDocumentoLicitacao();
        $modelAnaliseTermoReferencia->popularDadosObjeto($dadosObjeto);
        $modelAnaliseTermoReferencia->salvar();

        if ($dadosObjeto['eadparecer'] != '' && $dados['esdid'] == PAR3_ESD_DOCUMENTO_AGUARDANDO_ANALISE) {
            require_once APPRAIZ . '/includes/workflow.php';
            $aedid = wf_pegarAcao(PAR3_ESD_DOCUMENTO_AGUARDANDO_ANALISE, PAR3_ESD_DOCUMENTO_EMANALISE);
            wf_alterarEstado($dados['docid'], $aedid['aedid'], '', array('edlid'=> $dados['edlid'], 'cache_estado_atual' => false), array('cache_estado_atual' => false));
        }

        if ($modelAnaliseTermoReferencia->commit()) {
            $result['result'] = 'sucesso';
        } else {
            $result['result'] = 'falha';
        }

//        ob_start();
//        $this->modalAnalise($dados['edlid']);
//        $result['modalanalise'] = ob_get_clean();
        ob_start();
        $this->atualizarListagensDocumento($proid);
        $result['listaDocumentos'] = ob_get_clean();
        $result['listaDocumentos'] = simec_json_decode($result['listaDocumentos']);

        echo simec_json_encode($result);
    }

    /**
     * Atualiza lista de itens e lista de documentos depois de salvar
     * (necessário se feito por ajax)
     * @param $proid
     */
    public function atualizarListagensDocumento($proid)
    {
        ob_start();

        $arrIniciativas = $this->model->recuperarIniciativas($proid);
        if ($arrIniciativas) {
            Par3_Helper_AccordionHelper::listagemAccordion($arrIniciativas, ['recarregarDiv' => false]);
        } else {
            echo '<div id="vazio" style="margin:20px auto; float:none;"
                         class="alert alert-info col-md-4 col-md-offset-6 text-center nenhum-registro">Nenhum item foi encontrado
                  </div>
                  ';
        }

        $lista['iniciativas'] = ob_get_clean();

        ob_start();
        $this->listarDocumentos($_REQUEST['proid']);
        $lista['documentos'] = ob_get_clean();

        echo simec_json_encode($lista);
    }

    /**
     * Carrega dados de adesão do item
     * @param array $post - iasid (par3.itemadesaosigarp) e ipiid
     */
    public function dadosAdesao($post)
    {
        $sql = "SELECT
                       nu_seq_solicitacao_adesao,
                       ips.nu_seq_pregao,
                       replace(to_char(cnpj::bigint, '00:000:000/0000-00'), ':','.') as cnpj,
                       entrazaosocial as razaosocial,
                       itcdsc as itemnome,
                       aicqtdaprovado,
                       to_char(ipivalorreferencia, 'L9G999G990D99')
                FROM par3.itemadesaosigarp ias
                INNER JOIN par3.itenscomposicao itc ON itc.itcidsigarp = ias.item -- ON itc.itcidsigarp = ics.item AND ics.proid = pro.proid
                INNER JOIN par3.processo pro ON pro.proid = ias.proid
                INNER JOIN par3.iniciativa_itenscomposicao_grupo iig ON iig.itcid = itc.itcid
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.iigid = iig.iigid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.ipiid = ipi.ipiid
                INNER JOIN par3.instrumentounidade_entidade ent ON ent.entcnpj = ias.cnpj
                LEFT JOIN par3.itempregaosigarp ips ON ips.itcid = itc.itcid and pro.proid = ips.proid
                WHERE
                entstatus = 'A'
                and adestatus = 'A'
                AND iasid = {$post['iasid']}
                AND aic.aicid = {$post['aicid']}
                ";

        $dadosAdesao = $this->model->pegaLinha($sql);

        if ($dadosAdesao) {

            $cnpj = pega_numero($dadosAdesao['cnpj']);
            $adesao = $dadosAdesao['nu_seq_solicitacao_adesao'];
            unset($dadosAdesao['cnpj']);

            $chaves = array_keys($dadosAdesao); //chaves numéricas

            //Mesma ordem dos itens recuperados no sql
            $tituloDados = ['Adesão:', 'Pregão:', 'Razão Social:', 'Item Nome:', 'Quantidade de Itens Solicitados:', 'Valor do item Solicitado:'];

            $html = '<div class=\'container\'>
                    <h4>Visualizar Adesão</h4>';
            foreach ($tituloDados as $key => $titulo) {
                $html .= "
                    <div class='row' style='padding: 4px 0;'>
                        <div class=\"col-lg-4\" style='text-align: right;'>{$titulo}</div>
                        <div class=\"col-lg-8\" style='padding-left: 0px;'>{$dadosAdesao[$chaves[$key]]}</div>
                    </div>";
            }
            $html .= "<div class='row' style='padding: 4px 0;'>
                        <a href='http://www.fnde.gov.br/sigarpweb/index.php/consultas/solicitacao-cgcom2/view/cnpj/{$cnpj}/nu_seq_solicitacao_adesao/{$adesao}'
                        target='_blank' class=\"col-lg-6\" style='text-align: right;font-weight: 600;'>Acessar adesão no SIGARP</a>
                        
                    </div>";
            $html .= "</div>";

            $dados['dados'] = $html;
            $dados['item'] = $dadosAdesao['itemnome'];
        } else {
            $dados['dados'] = 'Não foram encontrados dados de adesão para este item.';
            $dados['item'] = '';
        }

        echo simec_json_encode($dados);
    }

    public function visualizarDocumentosItem($aicid)
    {
        $sql = "SELECT DISTINCT
                        edl.edlid,
                        doadescricao,
                        edldsc,
                        qtd,
                        valordoc
                FROM par3.execucao_documentolicitacao edl
                INNER JOIN par3.execucao_documentolicitacao_itens edi ON edl.edlid = edi.edlid
                INNER JOIN par3.analise_itemcomposicao aic ON aic.aicid = edi.aicid
                INNER JOIN par3.iniciativa_planejamento_item_composicao ipi ON ipi.ipiid = aic.ipiid
                INNER JOIN par3.tipo_documento_anexo doa ON doa.doaid = edl.doaid
                LEFT JOIN LATERAL (SELECT sum(t.aicqtdaprovado * t.aicvaloraprovado) as valordoc, sum(aicqtdaprovado) as qtd
                          FROM par3.analise_itemcomposicao t
                          INNER JOIN par3.execucao_documentolicitacao_itens t2 ON t2.aicid = t.aicid
                          WHERE t2.edlid = edl.edlid
                          ) v on true
                WHERE aic.aicid = {$aicid}
                ORDER BY 1
                ";

        $cabecalho = array('Tipo de Documento', 'Descrição', 'Quantidade item', 'Valor');

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addAcao('edit', 'editarDocModal');
        $listagem->addCallbackDeCampo('valordoc', 'par3_mascaraMoeda');
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function desativarDocumentoLicitacao($edlid, $proid = null)
    {

        $this->model->edlid = $edlid;
        $this->model->edlstatus = 'I';

        try {
            $this->model->salvar();
            $this->model->commit();

            $this->atualizarListagensDocumento($proid);
        } catch (Simec_Db_Exception $e) {
            echo 'falha';
        }
    }
}
