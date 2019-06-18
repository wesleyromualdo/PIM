<?php
class Painel extends mvc\Controle
{
    protected $arIbox = [];
    public function getConfiguracoes()
    {
        $arFiltros = (new Par3_Model_PainelFiltros())->carregarConfiguracoes($_SESSION['usucpf']);
        return json_decode($arFiltros['filtros']);
    }

    public function setConfiguracoes($arParams)
    {
//        $arParams = simec_json_encode($arParams);
        $mFiltros = new Par3_Model_PainelFiltros();
        $mFiltros->salvarFiltros($arParams);
    }

    public function resetConfiguracoes()
    {
//        $arParams = simec_json_encode($arParams);
        $mFiltros = new Par3_Model_PainelFiltros();
        $mFiltros->resetConfiguracoes($_SESSION['usucpf']);
    }

    public function setListaIbox($arIbox)
    {
        $this->arIbox = $arIbox;
    }

    public function iniciarFluxo()
    {
        $this->visao->setCabecalho('par3/modulos/visao/Cabecalho/bootstrap_painel_v1.inc', true);
        $this->visao->setRodape('par3/modulos/visao/Rodape/bootstrap_painel_v1.inc', true);
        $this->visao->setarIncludeJS('/library/jquery/jquery-ui-1.10.3/jquery.ui.sortable.min.js', true);
        $this->visao->setarIncludeJS('/library/jquery/jquery-ui-1.10.3/jquery.ui.touch-punch.min.js', false);
        $this->visao->setarIncludeCSS('/zimec/public/temas/simec/css/plugins/datapicker/datepicker3.css', true);
        $this->visao->setarIncludeCSS('/zimec/public/temas/simec/css/plugins/awesome-bootstrap-checkbox/awesome-bootstrap-checkbox.css', true);
        $this->visao->setarIncludeCSS('/zimec/public/temas/simec/css/plugins/materialize/materialize.min.css', true);
        $this->visao->setarIncludeCSS('/libs_painel/toastr/toastr.css', true);
        $this->visao->setarIncludeCSS('/libs_painel/select2/select2.css', true);
        $this->visao->setarIncludeJS('/includes/Highcharts-4.0.3/js/highcharts.js', false);
        $this->visao->setarIncludeJS('/zimec/public/temas/simec/js/plugins/flot/jquery.flot.js', false);
        $this->visao->setarIncludeJS('/zimec/public/temas/simec/js/plugins/flot/jquery.flot.resize.js', false);
        $this->visao->setarIncludeJS('/zimec/public/temas/simec/js/plugins/flot/jquery.flot.tooltip.min.js', false);
        $this->visao->setarIncludeJS('/zimec/public/temas/simec/js/plugins/flot/curvedLines.js', false);
        $this->visao->setarIncludeJS('/zimec/public/temas/simec/js/plugins/flot/jquery.flot.orderBars.js', false);
        $this->visao->setarIncludeJS('/zimec/public/temas/simec/js/plugins/flot/jquery.flot.symbol.js', false);
        $this->visao->setarIncludeJS('/zimec/public/temas/simec/js/plugins/flot/jquery.flot.time.js', false);
        $this->visao->setarIncludeJS('/zimec/public/temas/simec/js/plugins/materialize/materialize.min.js', true);
        $this->visao->setarIncludeJS('/libs_painel/toastr/toastr.js', false);
        $this->visao->setarIncludeJS('/zimec/public/temas/simec/js/plugins/chartjs/Chart.bundle.min.js', true);
        $this->visao->setarIncludeJS('/libs_painel/select2/select2.min.js', false);
    }

    public function getListaIbox()
    {
        $arrFiltros = $this->getConfiguracoes();
        $arDado   = array();
        $arDado[] = $this->iboxEvolucoesPagamento($arrFiltros);
        $arDado[] = $this->iboxSituacoesPar($arrFiltros);
        $arDado[] = $this->iboxTermo($arrFiltros);
        $arDado[] = $this->iboxPagamentos($arrFiltros);
        $arDado[] = $this->iboxPendencias($arrFiltros);
        $arDado[] = $this->iboxEmpenho($arrFiltros);
        $arDado[] = $this->iboxRegioes($arrFiltros);
//        $arDado[] = $this->iboxVigenciaTermos($arrFiltros);

        $arIbox = [];
        $arIbox2 =[];
        if(is_array($arrFiltros->ibox)) {
            foreach($arDado as $ibox) {
                if(in_array($ibox->getId(),$arrFiltros->ibox)) {
                    $key = array_search($ibox->getId(), $arrFiltros->ibox);
                    $arIbox[$key] = $ibox;
                    continue;
                }
            }
            foreach($arDado as $ibox) {
                if(!in_array($ibox->getId(),$arrFiltros->ibox)) {
                    $arIbox[] = $ibox;
                    continue;
                }
            }
            $arIbox = $arIbox + $arIbox2;
        }else {
            foreach($arDado as $ibox) {
                $arIbox[] = $ibox;
            }
        }
        return $arIbox;
    }

    public function acaoIndex()
    {
        $arIbox = [];
        $arIbox['quadro'] = $this->getListaIbox();
        $this->visao->carregarDados($arIbox);
        $this->visao->incluirVisao('inicial');
    }

    public function iboxPendencias($arrFiltros = null)
    {
        $id  = 'iboxpendencias';
        $tps = new Par3_Model_TiposPendencias();
        $arrPendencias = $tps->selectTiposPendencias();
        $this->visao->carregarDados(["pendencia"=>$arrPendencias]);
        $inu = new Par3_Model_Pendencias();
        $sql = $inu->sqlPendenciasGrafico();
        $toolBoxSql = '';

        $permiteVisualizar = $this->db->testa_superuser();
        $toolBoxSql = $this->visao
            ->helper('ToolBoxIcon\SqlToolBox')
            ->setSql($sql)
            ->setId('iboxsqlpendencias');

        $toolBoxCheckBoxFilter = $this->visao
            ->helper('ToolBoxIcon\CheckBoxFilterToolBox')
            ->setId('checkboxpendencias')
            ->setTitle('Pendências')
            ->setIdDivAtualizar('situacaoEntidades')
            ->setName('tipid')
            ->setValues($arrFiltros)
            ->setUrl('/par3/par3.php?modulo=painel/filtrarpendencia&acao=A')
            ->setParams($arrPendencias);

        return $this->visao
            ->helper('Ibox')
            ->limparClassCaixaExterna()
            ->setId('iboxpendencias')
            ->setVisivel($this->getVisibilidadeIbox($arrFiltros,'iboxpendencias'))
            ->permitirFecharIbox(true)
            ->addClassCaixaExterna('col-md-4')
            ->setarConteudoTitulo('Pendências')
            ->setarConteudoCorpo($this->graficoPendencias((array)$arrFiltros->{$id}))
            ->addCaixaToolBoxOpcoes($toolBoxSql,$permiteVisualizar)
            ->addCaixaToolBoxOpcoes($toolBoxCheckBoxFilter)
            ->setarConteudoRodape('<span class="text-success">* Clique no gráfico para mais detalhes.</span>');
    }

    public function graficoPendencias($arrPost = null)
    {
        $id = "situacaoEntidades";
        $inu = new Par3_Model_Pendencias();
        $sql = $inu->sqlPendenciasGrafico($arrPost);
        ob_start();
        require_once APPRAIZ . "includes/funcoesspo_componentes.php";
        require APPRAIZ.'includes/Agrupador.php';
        require_once APPRAIZ . '/includes/library/simec/Grafico.php';
        $clsGraficoQuantidadeObras = new Grafico(Grafico::K_TIPO_PIZZA, false);
        $clsGraficoQuantidadeObras
            ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
            ->setId($id)
//            ->setHeight('400px')
            ->setPointEvent(array('click' => "detalharEntidade('" . $id . "',this);"))
            ->setColors("'#008000','#FF6A6A','#FFD700'")
            ->gerarGrafico($sql);
        $output = ob_get_clean();
        return $output;
    }

    public function iboxTermo($arrFiltros = null)
    {
        $idIbox  = 'iboxtermos';
        $grafico = $this->graficoSituacaoTermo((array)$arrFiltros->{$idIbox});
        $id      = $grafico['id'];
        $sql     = $grafico['sql'];
        $permiteVisualizar = $this->db->testa_superuser();
        $toolBoxSql = $this->visao
            ->helper('ToolBoxIcon\SqlToolBox')
            ->setSql($sql)
            ->setId('iboxsqltermos');

        $arInta = ['Atendimento' =>[1 => 'PAR',2 => 'Emenda']];
        $toolBoxCheckBoxFilter = $this->visao
            ->helper('ToolBoxIcon\CheckBoxFilterToolBox')
            ->setId('checkboxintatermo')
            ->setTitle('Filtros')
            ->setIdDivAtualizar('situacaotermo')
            ->setName('intaidtermo')
            ->setValues($arrFiltros)
            ->agrupar(true)
            ->setUrl('/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=filtrartermo')
            ->setParams($arInta);
        return $this->visao
            ->helper('Ibox')
            ->limparClassCaixaExterna()
            ->setId($idIbox)
            ->setVisivel($this->getVisibilidadeIbox($arrFiltros,$idIbox))
            ->permitirFecharIbox(true)
            ->addClassCaixaExterna('col-md-4')
            ->setarConteudoTitulo('Situação dos Termos')
            ->addCaixaToolBoxOpcoes($toolBoxSql,$permiteVisualizar)
            ->addCaixaToolBoxOpcoes($toolBoxCheckBoxFilter)
            ->setarConteudoCorpo($grafico['render'])
            ->setarConteudoRodape('<span class="text-success">* Clique no gráfico para mais detalhes.</span>');
    }

    public function graficoSituacaoTermo($arrParams = null)
    {
        $id = "situacaotermo";
        $mTermo = new Par3_Model_DocumentoTermo();
        $sql = $mTermo->sqlGrafico($arrParams);
        $arDadosgrafico = $mTermo->pegaLinha($sql);

        $arGrafico = [
            [
                'descricao'       => 'Documento Validado',
                'valor'           => $arDadosgrafico['validado'],
                'valor_monetario' => $arDadosgrafico['valor_validado']
            ],
            [
                'descricao'       => 'Documento não Validado',
                'valor'           => $arDadosgrafico['nao_validado'],
                'valor_monetario' => $arDadosgrafico['valor_nao_validado']
            ],
            [
                'descricao'       => 'Bloqueado',
                'valor'           => $arDadosgrafico['bloqueado'],
                'valor_monetario' => $arDadosgrafico['valor_bloqueado']
            ],
            [
                'descricao'       => 'Documento Rejeitado Contrapartida',
                'valor'           => $arDadosgrafico['rejeitado'],
                'valor_monetario' => $arDadosgrafico['valor_rejeitado']
            ],
        ];
        ob_start();
        $clsGraficoQuantidadeObras = new Grafico(Grafico::K_TIPO_PIZZA, false);
        $clsGraficoQuantidadeObras
            ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_VALOR_MONETARIO)
            ->setPointEvent(array('click' => "detalharTermos('" . $id . "',this);"))
            ->setId($id)
            ->setAgrupadores(array('name' => 'descricao', 'valor' => 'valor','valor_monetario' => 'valor_monetario'))
            ->setColors("'#55BF3B','#FFD700', '#FF6A6A','#CD0000'")
            ->gerarGrafico($arGrafico);
        $output = ob_get_clean();
        return ['id' => $id,'render' => $output,'sql' => $sql];
    }

    public function iboxEvolucoesPagamento($arrFiltros = null)
    {
        $id = 'iboxevolucoespagamento';
        $mTermo = new Par3_Model_DocumentoTermo();
        $tps = new Par3_Model_Pagamento();
        $sqlTermos     = $tps->sqlEvolucaoPagamentos(1);
        $sqlPagamentos = $tps->sqlEvolucaoPagamentos(2);

        $permiteVisualizar = $mTermo->testa_superuser();
        $toolBoxSql = $this->visao
            ->helper('ToolBoxIcon\SqlToolBox')
            ->setSql('<h2>Termos</h2>'.$sqlTermos.'<hr><h2>Pagamento</h2>'.$sqlPagamentos)
            ->setId('iboxsqlevolucoespagamentos');

        $this->visao->carregarDados(["filtro"=>(array)$arrFiltros->{$id}]);
        return $this->visao
            ->helper('Ibox')
            ->limparClassCaixaExterna()
            ->permitirFecharIbox(true)
            ->setId($id)
            ->setVisivel($this->getVisibilidadeIbox($arrFiltros,$id))
            ->addClassCaixaExterna('col-md-12')
            ->addCaixaToolBoxOpcoes($toolBoxSql,$permiteVisualizar)
            ->setarConteudoTitulo('Evolução de Pagamentos')
            ->setarConteudoCorpo($this->visao->capturarHtmlVisaoAcessoria('grafico_evolucao_pagamentos'))
            ->setarConteudoRodape('<span class="text-success"><br></span>');
    }

    public function iboxEmpenho($arrFiltros = null)
    {
        $id = 'iboxempenho';
        $grafico = $this->graficoEmpenhos((array)$arrFiltros->{$id});
        $permiteVisualizar = $this->db->testa_superuser();

        $toolBoxSql = $this->visao
            ->helper('ToolBoxIcon\SqlToolBox')
            ->setSql($grafico['sql'])
            ->setId('iboxsqlempenhos');

        $arInta = ['Atendimento' =>[1 => 'PAR',2 => 'Emenda']];
        $toolBoxCheckBoxFilter = $this->visao
            ->helper('ToolBoxIcon\CheckBoxFilterToolBox')
            ->setId('checkboxintaempenho')
            ->setTitle('Filtros')
            ->setValues($arrFiltros)
            ->setIdDivAtualizar($grafico['id'])
            ->setName('intaidempenho')
            ->agrupar(true)
            ->setUrl('/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=filtrarempenho')
            ->setParams($arInta);

        return $this->visao
            ->helper('Ibox')
            ->limparClassCaixaExterna()
            ->setId($id)
            ->setVisivel($this->getVisibilidadeIbox($arrFiltros,$id))
            ->permitirFecharIbox(true)
            ->addClassCaixaExterna('col-md-4')
            ->setarConteudoTitulo('Situação dos Empenhos')
            ->setarConteudoCorpo($grafico['render'])
            ->addCaixaToolBoxOpcoes($toolBoxSql,$permiteVisualizar)
            ->addCaixaToolBoxOpcoes($toolBoxCheckBoxFilter)
            ->setarConteudoRodape('<span class="text-success"><br></span>');
    }

    public function iboxPagamentos($arrFiltros = null)
    {
        $grafico = $this->graficoSituacaoPagamentos((array)$arrFiltros);
        $sql = $grafico['sql'];

        $permiteVisualizar = $this->db->testa_superuser();
        $toolBoxSql = $this->visao
            ->helper('ToolBoxIcon\SqlToolBox')
            ->setSql($sql)
            ->setId('iboxsqlpagamentos');

        $arInta = ['Atendimento' =>[1 => 'PAR',2 => 'Emenda']];
        $toolBoxCheckBoxFilter = $this->visao
            ->helper('ToolBoxIcon\CheckBoxFilterToolBox')
            ->setId('checkboxintapagamento')
            ->setTitle('Filtros')
            ->setValues($arrFiltros)
            ->setIdDivAtualizar($grafico['id'])
            ->setName('intaidpagamento')
            ->agrupar(true)
            ->setUrl('/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=filtrarpagamento')
            ->setParams($arInta);

        return $this->visao
            ->helper('Ibox')
            ->limparClassCaixaExterna()
            ->setId('iboxpagamentos')
            ->setVisivel($this->getVisibilidadeIbox($arrFiltros,'iboxpagamentos'))
            ->permitirFecharIbox(true)
            ->addClassCaixaExterna('col-md-4')
            ->setarConteudoTitulo('Situação dos Pagamentos')
            ->setarConteudoCorpo($grafico['render'])
            ->addCaixaToolBoxOpcoes($toolBoxSql,$permiteVisualizar)
            ->addCaixaToolBoxOpcoes($toolBoxCheckBoxFilter)
            ->setarConteudoRodape('<span class="text-success"><br></span>');
    }

    public function iboxVigenciaTermos($arrFiltros = null)
    {
        $grafico = $this->graficoVigenciaTermos((array)$arrFiltros);
        $sql = $grafico['sql'];

        $permiteVisualizar = $this->db->testa_superuser();
        $toolBoxSql = $this->visao
            ->helper('ToolBoxIcon\SqlToolBox')
            ->setSql($sql)
            ->setId('iboxsqlvigenciatermos');

        return $this->visao
            ->helper('Ibox')
            ->limparClassCaixaExterna()
            ->setId('iboxvigenciatermos')
            ->setVisivel($this->getVisibilidadeIbox($arrFiltros,'iboxvigenciatermos'))
            ->permitirFecharIbox(true)
            ->addClassCaixaExterna('col-md-4')
            ->setarConteudoTitulo('Vigência dos Termos')
            ->setarConteudoCorpo($grafico['render'])
            ->addCaixaToolBoxOpcoes($toolBoxSql,$permiteVisualizar)
            ->setarConteudoRodape('<span class="text-success"><br></span>');
    }

    public function graficoSituacaoPagamentos($arrParams = null)
    {
        $id = "situacaoPagamentos";
        $mPagamento = new Par3_Model_Pagamento();
        $sql = $mPagamento->sqlSituacaoGrafico($arrParams);
        ob_start();
        $clsGraficoQuantidadeObras = new Grafico(Grafico::K_TIPO_PIZZA, false);
        $clsGraficoQuantidadeObras
            ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_VALOR_MONETARIO)
            ->setId($id)
            ->setAgrupadores(array('name' => 'descricao', 'valor' => 'valor','valor_monetario' => 'valor_monetario'))
//            ->setColors("'Tomato','SteelBlue', 'LightSeaGreen','Gold','cyan'")
            ->gerarGrafico($sql);
        $output = ob_get_clean();
        return ['id' => $id,'render' => $output,'sql' => $sql];
    }

    public function graficoVigenciaTermos($arrParams = null)
    {
        $id = "vigenciatermos";
        $mDot = new Par3_Model_DocumentoTermo();
        $sql = $mDot->sqlPainelPeriodoVigencia();

        ob_start();
        $mGrafico = new Grafico(Grafico::K_TIPO_PIZZA, false);
        $mGrafico
            ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
            ->setId($id)
            ->setAgrupadores(array('name' => 'descricao', 'valor' => 'valor','id' => 'id'))
            ->setPointEvent(array('click' => "detalharVigenciaTermos('" . $id . "',this);"))
            ->gerarGrafico($sql);
        $output = ob_get_clean();
        return ['id' => $id,'render' => $output,'sql' => $sql];
    }

    public function graficoEmpenhos($arrParams = null)
    {
        $id = "situacaoEmpenho";
        $mEmpenho = new Par3_Model_Empenho();
        $sql = $mEmpenho->sqlGrafico($arrParams);
        ob_start();
        $clsGraficoQuantidadeObras = new Grafico(Grafico::K_TIPO_PIZZA, false);
        $clsGraficoQuantidadeObras
            ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_VALOR_MONETARIO)
            ->setId($id)
            ->setAgrupadores(array('name' => 'descricao', 'valor' => 'valor','valor_monetario' => 'valor_monetario'))
            ->setColors("'#55BF3B','#FFD700', '#FF6A6A','#CD0000'")
            ->gerarGrafico($sql);
        $output = ob_get_clean();
        return ['id' => $id,'render' => $output,'sql' => $sql];
    }

    public function graficoAreaRegioes($arrParams = null)
    {
        $id = "regiaoareas";
        $mEmpenho = new Par3_Model_Pagamento();
        $sql = $mEmpenho->sqlPagamentoPorRegiao($arrParams);
        ob_start();
        $clsGraficoQuantidadeObras = new Grafico(Grafico::K_TIPO_AREA, false);
        $clsGraficoQuantidadeObras
            ->setId($id)
//            ->setColors("'Tomato','SteelBlue', 'LightSeaGreen','Gold','cyan'")
            ->gerarGrafico($sql);
        $output = ob_get_clean();
        return ['id' => $id,'render' => $output,'sql' => $sql];
    }

    public function iboxSituacoesPar($arrFiltros = null)
    {
        $id = 'iboxsituacoespar';
        $mPendencias = new Par3_Model_Pendencias();
        $sql = $mPendencias->sqlSituacoesPar($this->arValoresSalvos->esdid);
        $arEsdObras   = simec_preparar_array($mPendencias->listarSituacoesPorTpdid(304));
        $arEsdAnalise = simec_preparar_array($mPendencias->listarSituacoesPorTpdid(310));

        $arEstadosDocumento = ['Obra' => $arEsdObras,'Análise' => $arEsdAnalise];
        $permiteVisualizar = $this->db->testa_superuser();

        $toolBoxSql = $this->visao
                ->helper('ToolBoxIcon\SqlToolBox')
                ->setSql($sql)
                ->setId('iboxsqlsituacaopar');

        $toolBoxCheckBoxFilter = $this->visao
            ->helper('ToolBoxIcon\CheckBoxFilterToolBox')
            ->setId('checkboxtipossituacoes')
            ->setTitle('Situações')
            ->setIdDivAtualizar('situacaopar')
            ->setName('esdid')
            ->setAditionalSubmitNames(['intaidsituacoes'])
            ->setValues($arrFiltros)
            ->setUrl('/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=filtrarlistasituacoes')
            ->agrupar(true)
            ->setParams($arEstadosDocumento);

        $arInta = ['Atendimento' =>[1 => 'PAR',2 => 'Emenda']];
        $toolBoxCheckBoxFilterInta = $this->visao
            ->helper('ToolBoxIcon\CheckBoxFilterToolBox')
            ->setId('checkboxintasituacoes')
            ->setTitle('Atendimento')
            ->setValues((array)$arrFiltros)
            ->setIdDivAtualizar('situacaopar')
            ->setName('intaidsituacoes')
            ->setAditionalSubmitNames(['esdid'])
            ->agrupar(true)
            ->setUrl('/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=filtrarlistasituacoes')
            ->setParams($arInta);

        return $this->visao
            ->helper('Ibox')
            ->limparClassCaixaExterna()
            ->setId($id)
            ->setVisivel($this->getVisibilidadeIbox($arrFiltros,$id))
            ->permitirFecharIbox(true)
            ->addClassCaixaExterna('col-md-4')
            ->setarConteudoTitulo('Situações PAR')
            ->setarConteudoCorpo($this->listaSituacoesPar($arrFiltros->{$id}))
            ->addCaixaToolBoxOpcoes($toolBoxSql,$permiteVisualizar)
            ->addCaixaToolBoxOpcoes($toolBoxCheckBoxFilterInta)
            ->addCaixaToolBoxOpcoes($toolBoxCheckBoxFilter)
            ->setarConteudoRodape('<span class="text-success"><br></span>');
    }

    public function listaSituacoesPar($arRequest = null)
    {
        $id = "situacaopar";
        $mPendencia = new Par3_Model_Pendencias();
        $sql = $mPendencia->sqlSituacoesPar((array)$arRequest);
        $arrParams = $mPendencia->carregar($sql);
        $this->visao->carregarDados(['arSituacao' => $arrParams,'id_div' => $id]);
        return $this->visao->capturarHtmlVisaoAcessoria('lista_situacao_par');
    }

    public function iboxRegioes($arrFiltros = null)
    {
        $sql = (new Par3_Model_Pagamento())->sqlPagamentoPorRegiao();
        $toolBoxSql = $this->visao
            ->helper('ToolBoxIcon\SqlToolBox')
            ->setSql($sql)
            ->setId('iboxsqlregioes');
        return $this->visao
            ->helper('Ibox')
            ->limparClassCaixaExterna()
            ->setId('iboxregioes')
            ->setVisivel($this->getVisibilidadeIbox($arrFiltros,'iboxregioes'))
            ->permitirFecharIbox(true)
            ->addClassCaixaExterna('col-md-4')
            ->setarConteudoTitulo('Distribuição por Região')
            ->addCaixaToolBoxOpcoes($toolBoxSql)
            ->setHeight('336px')
            ->setarConteudoCorpo($this->visao->capturarHtmlVisaoAcessoria('template_radar_regioes'))
            ->setarConteudoRodape('<span class="text-success">* Clique nos pontos do gráfico para mais detalhes.</span>');
    }

    public function ibox()
    {
        $id = "situacaoPagamentos";
        $mPagamento = new Par3_Model_Pagamento();
        $sql = $mPagamento->sqlSituacaoGrafico();
        ob_start();
        $clsGraficoQuantidadeObras = new Grafico(Grafico::K_TIPO_PIZZA, false);
        $clsGraficoQuantidadeObras
            ->setFormatoTooltip(Grafico::K_TOOLTIP_PIE_DECIMAL_0)
            ->setId($id)
            ->gerarGrafico($sql);
        $output = ob_get_clean();
        return $this->visao
            ->helper('Ibox')
            ->limparClassCaixaExterna()
            ->addClassCaixaExterna('col-md-4')
            ->setarConteudoTitulo('Situação dos Pagamentos')
            ->setarConteudoCorpo($output);
//            ->setarConteudoRodape('linha de rodapé');
    }

    public function acaoFiltrarpendencia()
    {
        ob_clean();
        echo $this->graficoPendencias($_GET);
        die;
    }

    public function acaoTermodetalhe()
    {
        $arrParams = $this->acaoListaTermos();
        $arrParams['arEstados'] = simec_preparar_array($this->db->carregar(Territorios_Model_Estado::pegarSQLSelect($_POST)));
        $controllerIniciativaTiposAssistencia = new Par3_Controller_IniciativaTiposAssistenciaController();
        $mInta = $controllerIniciativaTiposAssistencia->recuperar();
        $arrTiposAssistencia = simec_preparar_array($mInta->recuperarTodosFormatoInput('intadsc'));
        $arrTiposAssistencia[0] = 'Todos';
        $arrParams['arTiposAssistencia'] = $arrTiposAssistencia;
        $this->visao->carregarDados($arrParams);
        $this->visao->incluirVisao('template_detalhar_termos',false);
    }

    public function acaoDetalharpendencias()
    {
        $arrParams = $this->acaoListaPendencias();
        $arrParams['arEstados'] = simec_preparar_array($this->db->carregar(Territorios_Model_Estado::pegarSQLSelect($_POST)));
        $this->visao->carregarDados($arrParams);
        $this->visao->incluirVisao('template_detalhar_pendencias_entidade',false);
    }

    public function acaoListapendencias()
    {
        $type = $_GET['datatype']?:'';
        $arrPost['offset']       = $_GET['offset']?:'0';
        $arrPost['limit']        = $_GET['limit']?:'50';
        $arrPost['descricao']    = trim(($_GET['descricao']));
        $arrPost['inudescricao'] = trim(($_GET['inudescricao']));
        $arrPost['itrid']        = $_GET['itrid'];
        $arrPost['detalhar']     = true;
        $arrPost['tipid']        = $_GET['tipid'];
        $arrPost['inuid']        = $_GET['inuid']?:null;
        $arrPost['itrid']        = $_GET['itrid'];
        $arrPost['muncod']       = $_GET['muncod'] ? array_filter($_GET['muncod']):null;
        $arrPost['estuf']        = $_GET['estuf'] ? array_filter($_GET['estuf']):null;
        $mPendencia   = new Par3_Model_Pendencias();
        $mTermo       = new Par3_Model_DocumentoTermo();
        $superuser    = $mTermo->testa_superuser();
        $arPendencias = $mPendencia->sqlPendenciasGrafico($arrPost);
        $arrParams = [
            'tipid'        => $arrPost['tipid'],
            'arPendencias' => $arPendencias['resultset'],
            'sql'          => $arPendencias['sql'],
            'descricao'    => $arrPost['descricao'],
            'superuser'    => $superuser,
            'limit'        => $arrPost['limit'],
            'offset'       => $arrPost['offset']
        ];
        if($type == 'json') {
            ob_clean();
            echo simec_json_encode($arrParams);die;
        }
        return $arrParams;
    }

    public function acaoListatermos()
    {
        $type = $_GET['datatype']?:'';
        $arrPost['intaidtermo']  = $_GET['intaidtermo'];
        $arrPost['offset']       = $_GET['offset']?:'0';
        $arrPost['limit']        = $_GET['limit']?:'50';
        $arrPost['itrid']        = $_GET['itrid'];
        $arrPost['descricao']    = trim(($_GET['descricao']));
        $arrPost['inudescricao'] = trim(($_GET['inudescricao']));

        $arrPost['muncod'] = $_GET['muncod'] ? array_filter($_GET['muncod']):null;
        $arrPost['estuf']  = $_GET['estuf'] ? array_filter($_GET['estuf']):null;
        $mTermo = new Par3_Model_DocumentoTermo();
        $arTermos = $mTermo->detalharDocumentoTermoGrafico($arrPost);
        $superuser = $mTermo->testa_superuser();
        $arrParams = [
            'intaidtermo' => $arrPost['intaidtermo'],
            'arTermos'    => $arTermos['resultset'],
            'full_count'  => $arTermos['resultset'][0]['full_count'],
            'sql'         => $arTermos['sql'],
            'descricao'   => $arrPost['descricao'],
            'superuser'   => $superuser,
            'limit'       => $arrPost['limit'],
            'offset'      => $arrPost['offset']
        ];
        if($type == 'json') {
            ob_clean();
            echo simec_json_encode($arrParams);die;
        }
        return $arrParams;
    }

    public function acaoXlspendencias()
    {
        $arrPost['descricao'] = ($_GET['descricao-pendencias']);
        $arrPost['tipid']     = $_GET['tipid'];
        $arrPost['muncod']    = $_GET['muncod'] ? array_filter($_GET['muncod']):null;
        $arrPost['estuf']     = $_GET['estuf']  ? array_filter($_GET['estuf']):null;
        $arrPost['itrid']     = $_GET['itrid'];
        $arrPost['detalhar']  = true;

        ob_clean();
        $mPendencia   = new Par3_Model_Pendencias();
        $arPendencias = $mPendencia->sqlPendenciasGrafico($arrPost);
        header("Content-Disposition: attachment; filename=Pendencias.xls");
        $arrayCabecalho = array(
            'Situação',
            'Entidade',
            'CAE',
            'CACS',
            'Habilitação',
            'Monitoramento',
            'Obras PAR',
            'PNE',
            'Contas',
            'SIOPE'
        );
        $esconderColunas = array('full_count','inudescricao','itrid','muncod','estuf','inuid');
        $listagemSimec = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($arPendencias['sql']);
        $listagemSimec->setId('xlsPendencias','inuid');
        $listagemSimec->addCallbackDeCampo('cae', 'formataPendencia');
        $listagemSimec->addCallbackDeCampo('cacs', 'formataPendencia');
        $listagemSimec->addCallbackDeCampo('habilitacao', 'formataPendencia');
        $listagemSimec->addCallbackDeCampo('monitoramento_par', 'formataPendencia');
        $listagemSimec->addCallbackDeCampo('obras_par', 'formataPendencia');
        $listagemSimec->addCallbackDeCampo('pne', 'formataPendencia');
        $listagemSimec->addCallbackDeCampo('contas', 'formataPendencia');
        $listagemSimec->addCallbackDeCampo('siope', 'formataPendencia');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        die;
    }

    public function acaoXlstermos()
    {
        $arrPost['descricao']    = ($_REQUEST['descricaotermo']);
        $arrPost['muncod']    = $_GET['muncod'] ? array_filter($_GET['muncod']):null;
        $arrPost['estuf']     = $_GET['estuf']  ? array_filter($_GET['estuf']):null;
        $arrPost['itrid']     = $_GET['itrid'];
        $arrPost['inudescricao'] = $_REQUEST['inudescricao'];
        $arrPost['itrid']        = $_REQUEST['itrid'];
        $arrPost['intaidtermo']  = $_REQUEST['intaidtermo'];
        $arrPost['detalhar']     = true;
        ob_clean();
//        ver($_REQUEST,d);
        $mTermo = new Par3_Model_DocumentoTermo();
        $arTermos = $mTermo->detalharDocumentoTermoGrafico($arrPost,true);
        header("Content-Disposition: attachment; filename=Termos.xls");
        $arrayCabecalho = array(
            'Número do Processo',
            'Situação',
            'Documento',
            'Entidade',
            'Atendimento'
        );
        $esconderColunas = array('full_count','inudescricao','itrid','intaid','estuf','muncod');
        $listagemSimec = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($arTermos['sql']);
        $listagemSimec->setId('xlsPendencias');
        $listagemSimec->addCallbackDeCampo('pronumeroprocesso', 'formata_numero_processo_sem_html');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        die;
    }

    public function acaoListaevolucoes($arParams = null)
    {
        $tps = new Par3_Model_Pagamento();
        $arrTermos    = $tps->carregar($tps->sqlEvolucaoPagamentos(1,$arParams));
        $arrEmpenho   = $tps->carregar($tps->sqlEvolucaoPagamentos(2,$arParams));
        $arrPagamento = $tps->carregar($tps->sqlEvolucaoPagamentos(3,$arParams));
        return [
            'agrupado'  => (is_array($arrTermos)?array_group_by($arrTermos,'tipo_label'):null),
            'empenho'   => $arrEmpenho,
            'pagamento' => $arrPagamento
        ];
    }

    public function acaoListaPagamentosPorRegiao($arParam = null,$tipo = null)
    {
        $mPagamento = new Par3_Model_Pagamento();
        $sql = $mPagamento->sqlPagamentoPorRegiao($arParam,$tipo);
        $arDados = $mPagamento->carregar($sql);
        return ['arDados' => $arDados,'sql' => $sql];
    }

    public function listarDistribuicaoPorEstado($arParam = null)
    {
        $permiteVisualizar = $this->db->testa_superuser();
        $arGroup = [];
        $arDados          = $this->acaoListaPagamentosPorRegiao($arParam,1);
        $arDadosAgrupados = $this->acaoListaPagamentosPorRegiao($arParam,2);
        $arGroup = array_group_by($arDados['arDados'],'regcod');
        $this->visao->carregarDados([
             'arEstados' => $arGroup,
             'sql' => $arDados['sql'],
             'superuser' =>$permiteVisualizar,
             'arRegioes' =>$arDadosAgrupados['arDados']
            ]);
        return $this->visao->capturarHtmlVisaoAcessoria('template_lista_estados');
    }

    public function painelConfiguracoes($arrPost = null)
    {
        $listaIbox = [];
        $listaIbox = $this->getListaIbox();
        $this->visao->carregarDados(['quadro' => $listaIbox,'confs' => $this->getConfiguracoes()]);
        return $this->visao->capturarHtmlVisaoAcessoria('template_configuracoes_painel');
    }

    public function detalharSituacoesPar($arParam)
    {
        $esdid = $arParam['esdid'];
        if (!filter_var($esdid, FILTER_VALIDATE_INT)) {
            return array();
        }
        $mPendencia = new Par3_Model_Pendencias();
        $sqlSituacoes = $mPendencia->sqlDetalharSituacoesPar($esdid);
        $arIniciativa = $mPendencia->carregar($sqlSituacoes);
        $this->visao->carregarDados(['arIniciativa' => $arIniciativa]);
        return $this->visao->capturarHtmlVisaoAcessoria('template_detalhe_situacoes_par');
    }

    public function detalharVigenciaTermos($get)
    {
        if (!filter_var($get['id'], FILTER_VALIDATE_INT)) {
            return array();
        }
        $permiteVisualizar = $this->db->testa_superuser();
        $sql = (new Par3_Model_DocumentoTermo())->sqlPainelPeriodoVigencia(true,$get['id']);
        $this->visao->carregarDados(['descricao' => $get['descricao'],'id' => $get['id'],'sql' => $sql,'superuser' =>$permiteVisualizar]);
        return $this->visao->capturarHtmlVisaoAcessoria('template_vigencia_termos');
    }

    public function listarVigenciatermosJson($arrPost)
    {
        $arDados['order']        = $arrPost['order'][0]['column'];
        $arDados['dir']          = $arrPost['order'][0]['dir'];
        $arDados['limit']        = $arrPost['length'];
        $arDados['offset']       = $arrPost['start'];
        $arDados['inudescricao'] = ($arrPost['search']['value']);

        $res = (new Par3_Model_DocumentoTermo())->sqlPainelPeriodoVigencia(true,$arrPost['descricao'],$arDados);
        $data  = is_array($res)?$res:array();
        $response = [
            "draw"            => intval($_POST['draw']),
            "recordsTotal"    => intval($data[0]['full_count']),
            "data"            => array_map(function($arr){ return array_values($arr);},$data),
            "recordsFiltered" => intval($data[0]['full_count']),
            "start"           => $arrPost['start'],
            "length"          => $arrPost['length']
        ];
        return json_encode(simec_utf8_encode_recursive($response));
    }

    public function painelPermissoes()
    {
        $listaIbox = [];
        $listaIbox = $this->getListaIbox();
        $mResp = new Par3_Model_UsuarioResponsabilidade();
        $this->visao->carregarDados(['quadro' => $listaIbox,'sqlperfis' => $mResp->listaSqlPerfis()]);
        return $this->visao->capturarHtmlVisaoAcessoria('template_permissoes_painel');
    }

    public function acaoRotas()
    {
        switch ($_REQUEST['requisicao']) {
            case 'listaevolucoes':
                ob_clean();
                $arParam = null;
                if($_GET['data-inicio'] && $_GET['data-fim']) {
                    $arParam['data-inicio'] = $_GET['data-inicio'];
                    $arParam['data-fim']    = $_GET['data-fim'];
                }
                $arParam['limit']       = $_GET['limit'];
                echo simec_json_encode(simec_utf8_encode_recursive($this->acaoListaevolucoes($arParam)));
                die;
            break;
            case 'listapagamentosregiao':
                ob_clean();
                $arParam = null;
                echo simec_json_encode(simec_utf8_encode_recursive($this->acaoListaPagamentosPorRegiao($arParam,2)['arDados']));
                die;
            break;
            case 'listapagamentosregiaoestado':
                ob_clean();
                echo $this->listarDistribuicaoPorEstado();
                die;
            break;
            case 'filtrarlistasituacoes':
                ob_clean();
                $arParam['esdid'] = $_GET['esdid'];
                $arParam['intaidsituacoes'] = $_GET['intaidsituacoes'];
                echo $this->listaSituacoesPar($arParam);die;
            break;
            case 'detalharsituacaopar':
                ob_clean();
                $arParam['esdid'] = $_GET['esdid'];
                echo $this->detalharSituacoesPar($arParam);die;
            break;
            case 'loadconfigs':
                ob_clean();
                $params = simec_json_encode(simec_utf8_encode_recursive($_POST));
                echo $this->painelConfiguracoes($_POST);die;
            break;
            case 'salvarconfs':
                ob_clean();
                $params = simec_json_encode(simec_utf8_encode_recursive($_POST));
                echo $this->setConfiguracoes($params);die;
            break;
            case 'resetarconfs':
                ob_clean();
                echo $this->resetConfiguracoes();die;
            break;
            case 'filtrartermo':
                ob_clean(); echo $this->graficoSituacaoTermo($_GET)['render'];die;
            break;
            case 'filtrarpagamento':
                ob_clean(); echo $this->graficoSituacaoPagamentos($_GET)['render'];die;
            break;
            case 'filtrarempenho':
                ob_clean(); echo $this->graficoEmpenhos($_GET)['render'];die;
            break;
            case 'permissoespainel':
                ob_clean(); echo $this->painelPermissoes();die;
            break;
            case 'detalharvigenciatermos':
                ob_clean(); echo $this->detalharVigenciaTermos($_GET);die;
            break;
            case 'listarvigenciatermosjson':
                ob_clean(); echo $this->listarVigenciatermosJson($_GET);die;
            break;
            case 'carregarMunicipiosAnalise':
                //lista todos os municípios relacionados ao Estado(estuf) informado
                ob_clean();
                $municipio = new Territorios_Model_Municipio();//instancia a classe município
                echo simec_json_encode($municipio->carregar($municipio->pegarSQLSelect($_GET['estuf'])));
                die;
                break;
            case 'carregarTodosMunicipios':
                //lista todos os municípios relacionados ao Estado(estuf) informado
                ob_clean();
                $municipio = new Territorios_Model_Municipio();//instancia a classe município
                $arrMun = $municipio->recuperarListaMunicipios();
                echo simec_json_encode($arrMun);
                die;
                break;
        }
    }

    public function getVisibilidadeIbox($filtro = null,$idIbox)
    {
        if($filtro) {
            if(isset($filtro->{$idIbox}->visivel)) {
                return $filtro->{$idIbox}->visivel == 'true'? true : false;
            }
            return true;
        }
        return true;
    }
}