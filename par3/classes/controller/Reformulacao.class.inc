<?php
class Par3_Controller_Reformulacao extends Controle
{
    protected $model;
    
    protected $arPermissao;
    
    protected $esdid_atual;
    
    public function __construct()
    {
        $this->model = new Par3_Model_Reformulacao();
    }
    
    public function pegaReformulacaoDocumento( $dotid, $refid = '', $tirid = PAR3_REFORMULACAO_INICIATIVA )
    {
        $filtro = " and refstatus = 'A' ";
        if( $refid ){
            $filtro = " and refid = $refid ";
        }
        $sql = "SELECT refid, refidpai, dotid, proid, tirid, docid, refdata, usucpf_solicitacao, refdata_solicitada, refdata_aprovada,
                    refjustificativa, refconsideracaoent, refconsideracaoprop, refconsideracaoproj, refconsideracaoobj, refconsideracaojus,
                    refconsideracaoval, refoutrasconsideracoes, reftextoparecer, refempenhocomplementar, refvalorentidade, refrafmecfnde, 
                    refacoestomadas, refoutrasinformacoes, refconsideracoes, refvalorsuficienteexecucao, refsituacao
                FROM par3.reformulacao_documento where dotid = $dotid and tirid = $tirid and refidpai is not null $filtro ORDER BY refid DESC";
        
        return $this->model->pegaLinha($sql);
    }
    
    public function listaAccordionIniciativa( $dotid, $proid )
    {
        $sql = "SELECT distinct ON(ip.inpid, a.anaano)
                    (ip.inpid||' - '||id.inddsc || ' - ' || ee.etadsc||' - '||a.anaano) as descricao,
                    ip.inpid as id, ip.inuid, a.anaano, pp.anaid, ini.intaid, dt.dotstatus
                FROM par3.documentotermo dt
                	INNER JOIN par3.processo p ON p.proid = dt.proid AND p.prostatus = 'A'
                	INNER JOIN par3.termocomposicao te ON te.dotid = dt.dotid AND te.tecstatus = 'A'
                    INNER JOIN  par3.processoparcomposicao pp ON pp.proid = p.proid AND pp.inpid = te.inpid AND pp.ppcano = te.tecano AND pp.ppcstatus = 'A'
                	INNER JOIN par3.analise a ON a.anaid = pp.anaid and a.inpid = te.inpid AND a.anastatus = 'A'
                	INNER JOIN par3.iniciativa_planejamento ip ON ip.inpid = te.inpid AND ip.inpstatus = 'A'
                	INNER JOIN par3.iniciativa ini ON ini.iniid = ip.iniid AND ini.inistatus = 'A'
                	INNER JOIN par3.iniciativa_descricao id ON id.indid = ini.indid AND id.indstatus = 'A'
                	INNER JOIN par3.ensino_etapa ee ON ee.etaid = ip.etaid
                WHERE dt.dotid = $dotid
                	--AND dt.dotstatus = 'A'";
        
        $arrDados =  $this->model->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        $controllerIniciativaPlanejamentoItemComposicao = new Par3_Controller_IniciativaPlanejamentoItemComposicao();
        
        $arReformulacao = $this->pegaReformulacaoDocumento($dotid);
        $refid = $arReformulacao['refid'];
        $refidpai = $arReformulacao['refidpai'];
        $arEstado = $this->carregaEsdidAtual($arReformulacao['docid']);
        
        $modeloIniciativaPlanejamentoItemComposicao = new Par3_Model_IniciativaPlanejamentoItemComposicao();
        
        $html = '<div class="panel-group" id="accordion">';
        foreach ($arrDados as $v) {
            $modelIniciativaTiposAtendimento = new Par3_Model_IniciativaTiposAtendimento();
            $condicaoPorEscola = 0;
            if( $v['intaid'] == 2){
                $condicaoPorEscola = 1;
            }
            $attribs = array('maxlengh' => 4000, 'readonly' => 'readonly');
            $arParam = array('inpid' => $v['id'],
                'inuid' => $v['inuid'],
                'iniano' => $v['anaano'],
                'anaid' => $v['anaid'],
                'reformulacao' => 'S',
                'esdid' => $arEstado['esdid'],
                'refid' => $refid,
                'refidpai' => $refidpai,
                'dotid' => $dotid,
                'dotstatus' => $v['dotstatus'],
                'condicaoporescola' => $condicaoPorEscola);
            
            $arDadosItens    = $modeloIniciativaPlanejamentoItemComposicao->dadosComboItens($v['id']);
            $rsDadosItens    = $modeloIniciativaPlanejamentoItemComposicao->carregar($arDadosItens);
            $arrDadosItens   = simec_preparar_array(!empty($rsDadosItens)?$rsDadosItens:array());
            
            
            //'.$controllerIniciativaPlanejamentoItemComposicao->getPlanilhaAnalise($arParam).'
            //<input type="hidden" name="inpid[]" value='.$v['id'].'>
            $html.= '<div class="panel panel-default" style="width: 100% !important;">
                        <span  tabindex="1000" data-toggle="tooltip" title="Clique para abrir os dados da iniciativa">
                        	<div class="panel-heading" role="tab" id="heading2">
                        		<h4 class="panel-title">
                                        <a role="button" class="collapsed" data-toggle="collapse" data-content="teste" data-trigger="hover" data-parent="#accordion" href="#iniciativa_'.$v['id'].'_'.$v['anaano'].'" aria-expanded="false" aria-controls="collapse2" style="display:block">'.$v['descricao'].'</a>
                                </h4>
                        	</div>
                        </span>
                    	<div id="iniciativa_'.$v['id'].'_'.$v['anaano'].'" class="panel-collapse collapse">
                    		<div class="panel-body">
                                '.$this->cabecalhoAnalise($dotid, $v['id'], $v['anaano'], $v['anaid']);
                    
                    if( in_array($arEstado['esdid'], array(PAR3_REFORMULACAO_ESDID_EM_CADASTRAMENTO, PAR3_REFORMULACAO_ESDID_EM_DILIGENCIA) ) ){
                        $html .= '<div class = "col-lg-offset-3 col-lg-6">
                                    <div class = "input-group">
                                        <select name="iigid_ref" id="iigid_'.$v['id'].'_'.$v['anaano'].'" class="chosen-select form-control" style="text-align: center;" data-placeholder="Selecione Item" required>
                                            <option></option>';
                                            foreach ($arrDadosItens as $codigo => $valor):
                                                $html.= '<option value="'.$codigo.'">'.$valor.'</option>';
                                            endforeach;
                                $html.= '</select>
                                        <span class = "input-group-btn">
                                            <button class="btn btn-success" type="button" id="btn-incluir-item-itens-composicao" onclick="incluirItemComposicao('.$v['id'].', '.$refid.', '.$refidpai.', '.$v['anaid'].', '.$v['anaano'].', '.$v['inuid'].', '.$condicaoPorEscola.');">Adicionar <i class="fa fa-plus"></i></button>
                                       </span>
                                    </div><!-- /input-group -->
                                </div><!-- /.col-lg-6 -->';
                    }
                        $html .= '<div class="ibox-content">
                                    <div class="row">
                                         <input type="hidden" name="anaano['.$v['anaid'].']" value="'.$v['anaano'].'">
                                        '.$controllerIniciativaPlanejamentoItemComposicao->getPlanilhaAnalise($arParam).'
                                    </div>
                                </div>
                            </div>
                        </div>
                  	</div>';
        }
        $html .= '</div>';
        echo $html;
    }
    
    public function cabecalhoAnalise( $dotid, $inpid, $ano, $anaid )
    {
        $sql = "SELECT tc.inpid, tc.anaid, tc.tecano AS ano, ini.iniid||' - '||id.inddsc AS iniciativa,
                	d.dimcod||' - '||d.dimdsc AS dimensao,
                	ci.cicdsc AS ciclo,
                	ito.intodsc AS objeto,
                	ta.intadsc AS atendimento,
                	CASE WHEN iu.itrid = 1 THEN 'Estadual' ELSE 'Municipal' END AS esfera,
                	array_to_string(array(SELECT DISTINCT a.iardsc FROM par3.iniciativa_iniciativas_areas ia
                							INNER JOIN par3.iniciativa_areas a ON a.iarid = ia.iarid AND a.iarstatus = 'A'
                						WHERE iniid = ip.iniid AND ia.iiarstatus = 'A'), ', ') AS areas,
                	array_to_string(array(SELECT DISTINCT p.prgdsc FROM par3.iniciativa_iniciativas_programas ipr
                							INNER JOIN par3.programa p ON p.prgid = ipr.prgid AND p.prgstatus = 'A'
                						WHERE ipr.iniid = ip.iniid AND ipr.iiprgstatus = 'A'), ', ') AS programas,
                	array_to_string(array(SELECT DISTINCT p.prodsc FROM par3.iniciativa_iniciativas_projetos ipr
                							INNER JOIN par3.projeto p ON p.proid = ipr.proid AND p.prostatus = 'A'
                						WHERE ipr.iniid = ip.iniid AND ipr.iiprostatus = 'A'), ', ') AS projetos,
                	'EducaçãoBásica' AS nivel,
                	ee.etadsc AS etapa,
                	mo.moddsc AS modalidade,
                	array_to_string(array(SELECT DISTINCT ide.desdsc FROM par3.iniciativa_modalidades im
                							INNER JOIN par3.iniciativa_desdobramento ide ON ide.desid = im.desid AND ide.desstatus = 'A'
                						WHERE im.imostatus = 'A' AND im.iniid = ip.iniid), ', ') AS desdobramento
                FROM par3.termocomposicao tc
                    INNER JOIN par3.documentotermo dt ON dt.dotid = tc.dotid AND dt.dotstatus = 'A'
                	INNER JOIN  par3.processoparcomposicao pp ON pp.proid = dt.proid AND pp.inpid = tc.inpid AND pp.ppcano = tc.tecano AND pp.ppcstatus = 'A'
                	INNER JOIN par3.analise a ON a.anaid = pp.anaid and a.inpid = tc.inpid AND a.anastatus = 'A'
                	INNER JOIN par3.iniciativa_planejamento ip ON ip.inpid = tc.inpid AND ip.inpstatus = 'A'
                	INNER JOIN par3.instrumentounidade iu ON iu.inuid = ip.inuid AND iu.inustatus = 'A'
                	INNER JOIN par3.iniciativa ini ON ini.iniid = ip.iniid AND ini.inistatus = 'A'
                	INNER JOIN par3.iniciativa_descricao id ON id.indid = ini.indid AND id.indstatus = 'A'
                	INNER JOIN par3.dimensao d ON d.dimid = ip.dimid AND d.dimstatus = 'A'
                	INNER JOIN par3.ciclo_par ci ON ci.cicid = ini.cicid AND ci.cicstatus = 'A'
                	INNER JOIN par3.iniciativa_tipos_objeto ito ON ito.intoid = ini.intoid AND ito.intostatus = 'A'
                	INNER JOIN par3.iniciativa_tipos_atendimento ta ON ta.intaid = ini.intaid AND ta.intastatus = 'A'
                	INNER JOIN par3.ensino_etapa ee ON ee.etaid = ip.etaid
                	INNER JOIN par3.modalidade mo ON mo.modid = ip.modid AND mo.modstatus = 'A'
                WHERE tc.dotid = $dotid
                	AND tc.inpid = $inpid
                	AND tc.tecano = $ano
                    and a.anaid = $anaid
                	AND tc.tecstatus = 'A'";
        
        $arrDados =  $this->model->pegaLinha($sql);
        $html = '
        <div class="ibox-content">
            <div class="row">
                <div class="col-md-12">
                    <h2 class="center" style="font-weight: bold;">Proposta Reprogramação</h2>
                </div>
            </div>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="form-group">
                    <div class="col-md-2" style="text-align: justify;"><h4>Dimensão:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['dimensao'].'</p></div>
                    <div class="col-md-2" style="text-align: justify;"><h4>Áreas Relacionadas:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['areas'].'</p></div>
                </div>
            </div>
            <div class="row">
                <div class="form-group">
                    <div class="col-md-2" style="text-align: justify;"><h4>Iniciativas:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['iniciativa'].'</p></div>
                    <div class="col-md-2" style="text-align: justif;"><h4>Programa:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['programas'].'</p></div>
                </div>
            </div>
            <div class="row">
                <div class="form-group">
                    <div class="col-md-2" style="text-align: justify;"><h4>Ciclo:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['ciclo'].'</p></div>
                    <div class="col-md-2" style="text-align: justify;"><h4>Projeto:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['projeto'].'</p></div>
                </div>
            </div>
            <div class="row">
                <div class="form-group">
                    <div class="col-md-2" style="text-align: justify;"><h4>Anos:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['ano'].'</p></div>
                    <div class="col-md-2" style="text-align: justify;"><h4>Nível:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['nivel'].'</p></div>
                </div>
            </div>
            <div class="row">
                <div class="form-group">
                    <div class="col-md-2" style="text-align: justify;"><h4>Tipo de Objeto:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['objeto'].'</p></div>
                    <div class="col-md-2" style="text-align: justify;"><h4>Etapa:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['etapa'].'</p></div>
                </div>
            </div>
            <div class="row">
                <div class="form-group">
                    <div class="col-md-2" style="text-align: justify;"><h4>Tipo de Atendimento:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['atendimento'].'</p></div>
                    <div class="col-md-2" style="text-align: justify;"><h4>Modalidade:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['modalidade'].'</p></div>
                </div>
            </div>
            <div class="row">
                <div class="form-group">
                    <div class="col-md-2" style="text-align: justify;"><h4>Esfera:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['esfera'].'</p></div>
                    <div class="col-md-2" style="text-align: justify;"><h4>Desdobramento:</h4></div>
                    <div class="col-md-4"><p style="text-align: justify;">'.$arrDados['desdobramento'].'</p></div>
                </div>
            </div>
    	</div>';
        return $html;
    }
    
    public function salvarSolicitacao( $arrDados = array() )
    {
        
        $refid = $this->salvarReformulacaoDocumento($arrDados);
        
        if( $arrDados['escola'] == 'S' ){
            $arrReformularItem = array();
            $arrReformularItem['arChavePrimaria'] 	= 'reiid';
            $arrReformularItem['stNomeTabela'] 		= 'par3.reformulacao_itemcomposicao';
            $arrReformularItem['arAtributosTabela'] = array('reiid', 'ipiid', 'iigid', 'inpid', 'refid', 'refidpai', 'reiquantidade', 'reivalor', 'reiano', 'usucpf', 'reidata', 'reistatus');
            
            $arrReformularItem['reiid']         = $arrDados['reiid'];
            $arrReformularItem['refid']         = $refid;
            $arrReformularItem['refidpai']      = $arrDados['refidpai'];
            if( !empty($arrDados['ipiid']) ){
                $arrReformularItem['ipiid']         = $arrDados['ipiid'];
            }
            $arrReformularItem['iigid']         = $arrDados['iigid'];
            $arrReformularItem['inpid']         = $arrDados['inpid'];
            $arrReformularItem['reiano']        = $arrDados['iniano'];
            $arrReformularItem['reiquantidade'] = 0;
            $arrReformularItem['reivalor']      = 0;
            $arrReformularItem['usucpf']        = $_SESSION['usucpf'];
            
            $reiid = trata_salvar($arrReformularItem);
            
            return array('refid' => $refid, 'reiid' => $reiid);
        } else {
            if( $arrDados['iigid'] ){
                foreach ($arrDados['iigid'] as $inpid => $arAno) {
                    foreach ($arAno as $ano => $arItem) {
                        foreach ($arItem as $iigid) {
                            $arrReformularItem = array();
                            $arrReformularItem['arChavePrimaria'] 	= 'reiid';
                            $arrReformularItem['stNomeTabela'] 		= 'par3.reformulacao_itemcomposicao';
                            $arrReformularItem['arAtributosTabela'] = array('reiid', 'ipiid', 'iigid', 'inpid', 'refid', 'refidpai', 'reiquantidade', 'reivalor', 'reiano', 'usucpf', 'reidata', 'reistatus');
                            
                            $reiid         = $arrDados['reiid'][$inpid][$ano][$iigid];
                            $ipiid         = $arrDados['ipiid'][$inpid][$ano][$iigid];
                            $reivalor      = $arrDados['valor_referencia'][$inpid][$ano][$iigid];
                            $reiquantidade = $arrDados['qtd-item-solicitada'][$inpid][$ano][$iigid];
                            
                            if( (int)$reiquantidade == (int)0 ){
                                $sql = "UPDATE par3.reformulacao_item_composicao_escola SET riequantidade = 0 WHERE refid = {$refid} AND reiid = {$reiid} AND iigid = {$iigid}";
                                $this->model->executar($sql);
                            }
                            
                            $arrReformularItem['reiid']         = $reiid;
                            $arrReformularItem['refid']         = $refid;
                            if( !empty($ipiid) ) {
                                $arrReformularItem['ipiid']         = $ipiid;
                            }
                            $arrReformularItem['iigid']         = $iigid;
                            $arrReformularItem['reiano']        = $ano;
                            $arrReformularItem['inpid']         = $inpid;
                            $arrReformularItem['refidpai']      = $arrDados['refidpai'];
                            $arrReformularItem['reiquantidade'] = ($reiquantidade ? $reiquantidade : '0');
                            if( $reivalor > 0 )
                            $arrReformularItem['reivalor']      = $reivalor;
                            $arrReformularItem['usucpf']        = $_SESSION['usucpf'];
                            
                            trata_salvar($arrReformularItem);
                        }
                    }
                }
            }
        }
        return $refid;
    }
    
    public function salvarReformulacaoDocumento($arrDados = array())
    {
        $arrReformular = array();
        $arrReformular['arChavePrimaria'] 	= 'refid';
        $arrReformular['stNomeTabela'] 		= 'par3.reformulacao_documento';
        $arrReformular['arAtributosTabela'] = array('refid', 'refidpai', 'dotid', 'proid', 'tirid', 'docid', 'refdata', 'usucpf_solicitacao', 'refdata_solicitada', 'refdata_aprovada',
                                                    'refstatus', 'refjustificativa', 'refconsideracaoent', 'refconsideracaoprop', 'refconsideracaoproj', 'refconsideracaoobj', 'refconsideracaojus',
                                                    'refconsideracaoval', 'refoutrasconsideracoes', 'reftextoparecer', 'refempenhocomplementar', 'refvalorentidade', 'refrafmecfnde', 'refdata_solicitada', 
                                                    'refjustificativa', 'refacoestomadas', 'refoutrasinformacoes', 'refconsideracoes', 'refvalorsuficienteexecucao', 'refsituacao');
        
        $arrDados['docid']                      = $this->pegaIdWorkFlow($arrDados['refid'], '', $arrDados['dotid'], $arrDados['tirid'], 352);
        $arrReformular['refid']                 = $arrDados['refid'];
        $arrReformular['dotid']                 = $arrDados['dotid'];
        $arrReformular['inuid']                 = $arrDados['inuid'];
        $arrReformular['proid']                 = $arrDados['proid'];
        $arrReformular['tirid']                 = $arrDados['tirid'];
        $arrReformular['docid']                 = $arrDados['docid'];
        
        //        $arrReformular['refempenhocomplementar']= str_replace(array(".",","), array("","."), $arrDados['refempenhocomplementar']);
        $arrReformular['refvalorentidade']      = ($arrDados['refvalorentidade'] ? str_replace(array(".",","), array("","."), $arrDados['refvalorentidade']) : 'null');
        $arrReformular['refrafmecfnde']         = ($arrDados['refrafmecfnde'] ? str_replace(array(".",","), array("","."), $arrDados['refrafmecfnde']) : 'null');
        
        $arrReformular['usucpf_solicitacao']    = $_SESSION['usucpf'];
        $arrReformular['refdata_solicitada']    = 'null';
        $arrReformular['refjustificativa']      = $arrDados['refjustificativa'];
        $arrReformular['refdata_solicitada']    = ($arrDados['refdata_solicitada'] ? formata_data_sql($arrDados['refdata_solicitada']) : 'null');
        $arrReformular['refacoestomadas']       = $arrDados['refacoestomadas'];
        $arrReformular['refoutrasinformacoes']  = $arrDados['refoutrasinformacoes'];
        if( $arrDados['refconsideracoes'] )
        $arrReformular['refconsideracoes']      = $arrDados['refconsideracoes'];
        $arrReformular['refvalorsuficienteexecucao']      = $arrDados['refvalorsuficienteexecucao'];
        $refid = trata_salvar($arrReformular);
        
        return $refid;
    }
    
    public function salvarReformulacaoPrazoDocumentoAnalise($arrDados = array())
    {
        
        $arrReformular = array();
        $arrReformular['arChavePrimaria'] 	= 'refid';
        $arrReformular['stNomeTabela'] 		= 'par3.reformulacao_documento';
        $arrReformular['arAtributosTabela'] = array('refid', 'refidpai', 'dotid', 'proid', 'tirid', 'docid', 'refdata', 'usucpf_solicitacao', 'refdata_solicitada', 'refdata_aprovada',
                                                    'refstatus', 'refjustificativa', 'refconsideracaoent', 'refconsideracaoprop', 'refconsideracaoproj', 'refconsideracaoobj', 'refconsideracaojus',
                                                    'refconsideracaoval', 'refoutrasconsideracoes', 'reftextoparecer', 'refempenhocomplementar', 'refvalorentidade', 'refrafmecfnde', 'refdata_solicitada', 
                                                    'refjustificativa', 'refacoestomadas', 'refoutrasinformacoes', 'refconsideracoes', 'refvalorsuficienteexecucao', 'refsituacao');
        
        $arrReformular['refid']                 = $arrDados['refid'];
        $arrReformular['refconsideracoes']      = $arrDados['refconsideracoes'];
        $arrReformular['refsituacao']           = $arrDados['refsituacao'];
        $arrReformular['refdata_aprovada']      = ($arrDados['refdata_aprovada'] ? formata_data_sql($arrDados['refdata_aprovada']) : 'null');
        $refid = trata_salvar($arrReformular);
        
        return $refid;
    }
    
    public function salvarAnaliseReformulacao( $arrDados = array() )
    {
        //ver($arrDados,d);
        $arrReformular = array();
        $arrReformular['arChavePrimaria'] 	= 'refid';
        $arrReformular['stNomeTabela'] 		= 'par3.reformulacao_documento';
        $arrReformular['arAtributosTabela'] = array('refid', 'refidpai', 'dotid', 'proid', 'tirid', 'docid', 'refdata', 'usucpf_solicitacao', 'refdata_solicitada', 'refdata_aprovada',
            'refstatus', 'refjustificativa', 'refconsideracaoent', 'refconsideracaoprop', 'refconsideracaoproj', 'refconsideracaoobj', 'refconsideracaojus',
            'refconsideracaoval', 'refoutrasconsideracoes', 'reftextoparecer', 'refempenhocomplementar', 'refvalorentidade', 'refrafmecfnde');

        $arrDados['docid']                      = $this->pegaIdWorkFlow($arrDados['refid'], PAR3_REFORMULACAO_PRAZO_ESDID_EM_CADASTRAMENTO, $arrDados['dotid'], 2, 355);
        $arrReformular['refid']                 = $arrDados['refid'];
        $arrReformular['dotid']                 = $arrDados['dotid'];
        $arrReformular['inuid']                 = $arrDados['inuid'];
        $arrReformular['proid']                 = $arrDados['proid'];

        $refempenhocomplementar = str_replace(array(".",","), array("","."), $arrDados['refempenhocomplementar']);
        $refvalorentidade       = str_replace(array(".",","), array("","."), $arrDados['refvalorentidade']);
        $refrafmecfnde          = str_replace(array(".",","), array("","."), $arrDados['refrafmecfnde']);

        $arrReformular['refempenhocomplementar']=  $refempenhocomplementar != ''? $refempenhocomplementar : 0;
        $arrReformular['refvalorentidade']      =  $refvalorentidade       != ''? $refvalorentidade : 0;
        $arrReformular['refrafmecfnde']         =  $refrafmecfnde          != ''? $refrafmecfnde : 0;

        $arrReformular['refempenhocomplementar']= str_replace(array(".",","), array("","."), $arrDados['refempenhocomplementar']);
        $arrReformular['refconsideracaoent']    = ($arrDados['refconsideracaoent']      ? $arrDados['refconsideracaoent']       : 'null');
        $arrReformular['refconsideracaoprop']   = ($arrDados['refconsideracaoprop']     ? $arrDados['refconsideracaoprop']      : 'null');
        $arrReformular['refconsideracaoproj']   = ($arrDados['refconsideracaoproj']     ? $arrDados['refconsideracaoproj']      : 'null');
        $arrReformular['refconsideracaoobj']    = ($arrDados['refconsideracaoobj']      ? $arrDados['refconsideracaoobj']       : 'null');
        $arrReformular['refconsideracaojus']    = ($arrDados['refconsideracaojus']      ? $arrDados['refconsideracaojus']       : 'null');
        $arrReformular['refconsideracaoval']    = ($arrDados['refconsideracaoval']      ? $arrDados['refconsideracaoval']       : 'null');
        $arrReformular['refoutrasconsideracoes']= ($arrDados['refoutrasconsideracoes']  ? $arrDados['refoutrasconsideracoes']   : 'null');
        $arrReformular['reftextoparecer']       = ($arrDados['reftextoparecer']         ? trim($arrDados['reftextoparecer'])    : 'null');
//        ver($arrReformular,d);
        $refid = trata_salvar($arrReformular);
        
        if( $arrDados['iigid'] ){
            foreach ($arrDados['iigid'] as $inpid => $arAno) {
                foreach ($arAno as $ano => $arItem) {
                    foreach ($arItem as $iigid) {
                        
                        $arrReformularItem = array();
                        $arrReformularItem['arChavePrimaria'] 	= 'raiid';
                        $arrReformularItem['stNomeTabela'] 		= 'par3.reformulacao_analise_itemcomposicao';
                        $arrReformularItem['arAtributosTabela'] = array('raiid', 'inpid', 'aicid', 'ipiid', 'iigid', 'anaid', 'refid', 'refidpai', 'reiid', 'raiqtdaprovado', 'raivaloraprovado', 'usucpf', 'raidata', 'raiaprovado', 'raistatus');
                        
                        $raiid         = $arrDados['raiid'][$inpid][$ano][$iigid];
                        $anaid         = $arrDados['anaid'][$inpid][$ano][$iigid];
                        $reiid         = $arrDados['reiid'][$inpid][$ano][$iigid];
                        $ipiid         = $arrDados['ipiid'][$inpid][$ano][$iigid];
                        $reivalor      = $arrDados['valor_referencia'][$inpid][$ano][$iigid];
                        $reiquantidade = $arrDados['qtd-item-aprovado'][$inpid][$ano][$iigid];
                        
                        $arrReformularItem['raiid']         = $raiid;
                        $arrReformularItem['reiid']         = $reiid;
                        $arrReformularItem['inpid']         = $inpid;
                        $arrReformularItem['refid']         = $refid;
                        if( !empty($ipiid) )
                            $arrReformularItem['ipiid']     = $ipiid;
                        $arrReformularItem['iigid']         = $iigid;
                        $arrReformularItem['anaid']         = $anaid;
                        $arrReformularItem['reiano']        = $ano;
                        $arrReformularItem['inpid']         = $inpid;
                        $arrReformularItem['refidpai']      = $arrDados['refidpai'];
                        $arrReformularItem['raiqtdaprovado'] = ($reiquantidade ? $reiquantidade : 0);
                        $arrReformularItem['raivaloraprovado'] = $reivalor;
                        $arrReformularItem['usucpf']        = $_SESSION['usucpf'];
                        //ver($arrReformularItem);
                        trata_salvar($arrReformularItem);
                    }
                }
            }
        }
        $arEstado = $this->carregaEsdidAtual($arrDados['docid']);
        if( $arEstado['esdid'] == PAR3_REFORMULACAO_ESDID_AGUARDANDO_ANALISE ){
            wf_alterarEstado($arrDados['docid'], 5638, 'Fluxo de Reformulação do PAR3', array(
                'refid' => $arrDados['refid'],
                'dotid' => $arrDados['dotid'],
                'proid' => $arrDados['proid']
            ));
        }
        return $refid;
    }
    
    public function verificaQtdMonitoradaItem($ipiid, $iigid)
    {
        $filtro = '';
        if( $ipiid ){
            
            if( $iigid ) $filtro = " AND api.iigid = $iigid ";
            
            $sql = "SELECT sum(ec.eciqtditens) FROM par3.analise_itemcomposicao ai
                        INNER JOIN par3.iniciativa_planejamento_item_composicao api ON api.ipiid = ai.ipiid AND api.ipistatus = 'A'
                    	INNER JOIN par3.execucao_contratoitens ec ON ec.aicid = ai.aicid
                        INNER JOIN par3.execucao_contrato co ON co.ecoid = ec.ecoid AND co.ecostatus = 'A'
                    WHERE ai.ipiid = $ipiid $filtro";
            $qtd = $this->model->pegaUm($sql);
        } else {
            $qtd = 0;
        }
        
        return $qtd;
    }
    
    public function gerarBackup( $dotid, $proid, $tirid)
    {
        /*$this->model->executar("UPDATE par3.reformulacao_itemcomposicao SET reistatus = 'I' WHERE refid IN (SELECT refid FROM par3.reformulacao_documento WHERE dotid = $dotid AND proid = $proid AND tirid = $tirid);
                                UPDATE par3.reformulacao_item_composicao_escola SET riestatus = 'I' WHERE refid IN (SELECT refid FROM par3.reformulacao_documento WHERE dotid = $dotid AND proid = $proid AND tirid = $tirid);
                                UPDATE par3.reformulacao_analise_itemcomposicao SET raistatus = 'I' WHERE refid IN (SELECT refid FROM par3.reformulacao_documento WHERE dotid = $dotid AND proid = $proid AND tirid = $tirid);
                                UPDATE par3.reformulacao_documento SET refstatus = 'I' WHERE dotid = $dotid AND proid = $proid AND tirid = $tirid;");*/
        
        $sql = "INSERT INTO par3.reformulacao_documento(dotid, proid, tirid, docid, refdata, usucpf_solicitacao, refdata_solicitada, refdata_aprovada, refstatus)
                VALUES($dotid, $proid, $tirid, null, now(), '{$_SESSION['usucpf']}', null, null, 'A') returning refid";
        $refid_bk = $this->model->pegaUm($sql);
        
        if( $tirid == PAR3_REFORMULACAO_INICIATIVA ){
            $esdid = PAR3_REFORMULACAO_ESDID_EM_CADASTRAMENTO;
            $tpdid = 355;
        } else {
            $esdid = PAR3_REFORMULACAO_PRAZO_ESDID_EM_CADASTRAMENTO;
            $tpdid = 352;
        }
        
        $docid = $this->pegaIdWorkFlow('', $esdid, $dotid, $tirid, $tpdid);
        $sql = "INSERT INTO par3.reformulacao_documento(dotid, proid, tirid, docid, refidpai, refdata, usucpf_solicitacao, refdata_solicitada, refdata_aprovada, refstatus)
                VALUES($dotid, $proid, $tirid, $docid, $refid_bk, now(), '{$_SESSION['usucpf']}', null, null, 'A') returning refid";
        $refid = $this->model->pegaUm($sql);
        
        if( $tirid == PAR3_REFORMULACAO_INICIATIVA ){
            $sql = "SELECT tecid, dotid, inpid, tecano, tecstatus, obrid, anaid FROM par3.termocomposicao WHERE dotid = $dotid AND tecstatus = 'A'";
            $arrDados = $this->model->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            /*
             * Gera registro de backup
             * */
            $arrDados_bk = $arrDados;
            foreach ($arrDados_bk as $v) {
                
                $sql = "SELECT ipiid, iigid, ipiano, ipiquantidade, ipivalorreferencia FROM par3.iniciativa_planejamento_item_composicao WHERE inpid = {$v['inpid']} AND ipiano = '{$v['tecano']}' AND ipistatus = 'A'";
                $arrItens = $this->model->carregar($sql);
                $arrItens = $arrItens ? $arrItens : array();
                
                foreach ($arrItens as $item) {
                    $sql = "INSERT INTO par3.reformulacao_itemcomposicao(ipiid, iigid, refid, inpid, reiquantidade, reivalor, reiano, usucpf, reidata, reistatus)
                            VALUES({$item['ipiid']}, {$item['iigid']}, $refid_bk, {$v['inpid']}, {$item['ipiquantidade']}, {$item['ipivalorreferencia']}, {$item['ipiano']}, '{$_SESSION['usucpf']}', now(), 'A') returning reiid";
                    $reiid = $this->model->pegaUm($sql);
                    
                    /*
                     * DADOS ITEM ESCOLA
                     * */
                    $sql = "SELECT ipeid, ipiid, ipequantidade, ipestatus, co_entidade, escid FROM par3.iniciativa_planejamento_item_composicao_escola where ipiid = {$item['ipiid']} and ipestatus = 'A'";
                    $arrItensEscola = $this->model->carregar($sql);
                    $arrItensEscola = $arrItensEscola ? $arrItensEscola : array();
                    
                    foreach ($arrItensEscola as $itemEscola) {
                        $sql = "INSERT INTO par3.reformulacao_item_composicao_escola(ipiid, iigid, reiid, refid, ipeid, co_entidade, escid, riequantidade, riestatus)
                                VALUES({$item['ipiid']}, {$item['iigid']}, $reiid, $refid_bk, {$itemEscola['ipeid']}, 0, {$itemEscola['escid']}, {$itemEscola['ipequantidade']}, 'A')";
                        $this->model->executar($sql);
                    }//fim escola
                    
                    /*
                     * DADOS ITEM ANALISE
                     * */
                    $sql = "SELECT DISTINCT it.aicid, it.ipiid, it.anaid, it.aicqtdaprovado, it.aicvaloraprovado, it.usucpf, it.aicdata, it.aicaprovado, it.aicstatus
                            FROM par3.analise_itemcomposicao it
                            	INNER JOIN par3.analise a ON a.anaid = it.anaid AND a.anastatus = 'A'
                            WHERE it.aicstatus = 'A'
                            	AND a.inpid = {$v['inpid']} AND a.anaano = '{$v['tecano']}' AND ipiid = {$item['ipiid']}";
                    $arrItensAnalise = $this->model->carregar($sql);
                    $arrItensAnalise = $arrItensAnalise ? $arrItensAnalise : array();
                    
                    foreach ($arrItensAnalise as $analise) {
                        $sql = "INSERT INTO par3.reformulacao_analise_itemcomposicao(ipiid, iigid, inpid, aicid, anaid, refid, reiid, raiqtdaprovado, raivaloraprovado, usucpf, raidata, raiaprovado, raistatus)
                                VALUES({$item['ipiid']}, {$item['iigid']}, {$v['inpid']}, {$analise['aicid']}, {$analise['anaid']}, $refid_bk, $reiid, {$analise['aicqtdaprovado']}, {$analise['aicvaloraprovado']}, '{$analise['usucpf']}', '{$analise['aicdata']}', '{$analise['aicaprovado']}', 'A')";
                        $this->model->executar($sql);
                    }//fim analise item
                    
                }//fim item_composicao
            }
        
            /*
             * Gera registro de alteração
             * */
            foreach ($arrDados as $v) {
                
                $sql = "SELECT ipiid, iigid, ipiano, ipiquantidade, ipivalorreferencia FROM par3.iniciativa_planejamento_item_composicao WHERE inpid = {$v['inpid']} AND ipiano = '{$v['tecano']}' AND ipistatus = 'A'";
                $arrItens = $this->model->carregar($sql);
                $arrItens = $arrItens ? $arrItens : array();
                
                foreach ($arrItens as $item) {
                    $sql = "INSERT INTO par3.reformulacao_itemcomposicao(ipiid, iigid, refid, refidpai, inpid, reiquantidade, reivalor, reiano, usucpf, reidata, reistatus)
                            VALUES({$item['ipiid']}, {$item['iigid']}, $refid, $refid_bk, {$v['inpid']}, {$item['ipiquantidade']}, {$item['ipivalorreferencia']}, {$item['ipiano']}, '{$_SESSION['usucpf']}', now(), 'A') returning reiid";
                    $reiid = $this->model->pegaUm($sql);
                    
                    /*
                     * DADOS ITEM ESCOLA
                     * */
                    $sql = "SELECT ipeid, ipiid, ipequantidade, ipestatus, co_entidade, escid FROM par3.iniciativa_planejamento_item_composicao_escola where ipiid = {$item['ipiid']} and ipestatus = 'A'";
                    $arrItensEscola = $this->model->carregar($sql);
                    $arrItensEscola = $arrItensEscola ? $arrItensEscola : array();
                    
                    /*
                     * Foi deixado de gravar porque não tinha como saber qual era a quantidade pactuada de escola, o usuario deverar inserir manualmente*/
                    foreach ($arrItensEscola as $itemEscola) {
                        $sql = "INSERT INTO par3.reformulacao_item_composicao_escola(ipiid, iigid, reiid, refid, refidpai, ipeid, co_entidade, escid, riequantidade, riestatus)
                                VALUES({$item['ipiid']}, {$item['iigid']}, $reiid, $refid, $refid_bk, {$itemEscola['ipeid']}, 0, {$itemEscola['escid']}, {$itemEscola['ipequantidade']}, 'A')";
                        //$this->model->executar($sql);
                    }//fim escola
                    
                    /*
                     * DADOS ITEM ANALISE
                     * */
                    $sql = "SELECT DISTINCT it.aicid, it.ipiid, it.anaid, it.aicqtdaprovado, it.aicvaloraprovado, it.usucpf, it.aicdata, it.aicaprovado, it.aicstatus
                            FROM par3.analise_itemcomposicao it
                            	INNER JOIN par3.analise a ON a.anaid = it.anaid AND a.anastatus = 'A'
                            WHERE it.aicstatus = 'A'
                            	AND a.inpid = {$v['inpid']} AND a.anaano = '{$v['tecano']}' AND ipiid = {$item['ipiid']}";
                    $arrItensAnalise = $this->model->carregar($sql);
                    $arrItensAnalise = $arrItensAnalise ? $arrItensAnalise : array();
                    
                    foreach ($arrItensAnalise as $analise) {
                        $sql = "INSERT INTO par3.reformulacao_analise_itemcomposicao(ipiid, iigid, inpid, aicid, anaid, refid, refidpai, reiid, raiqtdaprovado, raivaloraprovado, usucpf, raidata, raiaprovado, raistatus)
                                VALUES({$item['ipiid']}, {$item['iigid']}, {$v['inpid']}, {$analise['aicid']}, {$analise['anaid']}, $refid, $refid_bk, $reiid, {$analise['aicqtdaprovado']}, {$analise['aicvaloraprovado']}, '{$analise['usucpf']}', '{$analise['aicdata']}', '{$analise['aicaprovado']}', 'A')";
                        $this->model->executar($sql);
                    }//fim analise item
                    
                }//fim item_composicao
            }
        }
        $this->model->commit();
        return $refid;
    }
    
    public function importaReformulacaoOriginal( $refid )
    {
        $sql = "UPDATE par3.iniciativa_planejamento_item_composicao ic1 SET ipiquantidade = foo.reiquantidade, ipivalorreferencia = foo.reivalor
                FROM(
                	SELECT ri.ipiid, ri.iigid, ri.inpid, reiquantidade, reivalor, ic.ipiquantidade, ic.ipivalorreferencia
                	FROM par3.reformulacao_itemcomposicao ri
                		INNER JOIN par3.iniciativa_planejamento_item_composicao ic ON ic.ipiid = ri.ipiid AND ic.iigid = ri.iigid AND ic.ipiano = ri.reiano AND ic.inpid = ri.inpid AND ic.ipistatus = 'A'
                	WHERE refid = $refid
                ) AS foo
                WHERE foo.ipiid = ic1.ipiid";
        $this->model->executar($sql);

        $sql = "UPDATE par3.iniciativa_planejamento_item_composicao_escola esc SET ipequantidade = foo.riequantidade
                FROM(
                	SELECT re.rieid, re.ipiid, re.iigid, re.reiid, re.refid, re.refidpai, re.ipeid, re.co_entidade, re.escid, re.riequantidade, re.riestatus
                	FROM par3.reformulacao_item_composicao_escola re
                		INNER JOIN par3.iniciativa_planejamento_item_composicao_escola ce ON ce.ipiid = re.ipiid AND ce.ipeid = re.ipeid AND ce.escid = re.escid AND ce.ipestatus = 'A'
                	WHERE refid = $refid
                ) AS foo
                WHERE foo.ipeid = esc.ipeid";
        $this->model->executar($sql);
        
        $sql = "UPDATE par3.analise_itemcomposicao ai SET aicqtdaprovado = foo.raiqtdaprovado, aicvaloraprovado = foo.raivaloraprovado
                FROM(
                	SELECT raiid, inpid, aicid, ipiid, iigid, anaid, refid, refidpai, reiid, raiqtdaprovado, raivaloraprovado, usucpf, raidata, raiaprovado, raistatus
                	FROM par3.reformulacao_analise_itemcomposicao re
                	WHERE refid = $refid AND aicid IS NOT NULL
                ) AS foo
                WHERE foo.aicid = ai.aicid";
        $this->model->executar($sql);
        
        $sql = "SELECT ri.reiid, ri.inpid, ri.reivalor, ri.reiquantidade, ri.reiano, ri.iigid FROM par3.reformulacao_itemcomposicao ri WHERE refid = $refid AND ipiid IS NULL";
        $arrItem = $this->model->carregar($sql);
        $arrItem = $arrItem ? $arrItem : array();
        
        foreach ($arrItem as $v) {
            $ipiid = $this->model->pegaUm("SELECT ipiid FROM par3.iniciativa_planejamento_item_composicao 
                                            WHERE inpid = {$v['inpid']} AND ipiano = {$v['reiano']} AND iigid = {$v['iigid']}");
            if( !empty($ipiid) ){
                $sql = "UPDATE par3.iniciativa_planejamento_item_composicao SET ipiquantidade = {$v['reiquantidade']} WHERE ipiid = {$ipiid}";
                $this->model->executar($sql);
            } else {
                $sql = "INSERT INTO par3.iniciativa_planejamento_item_composicao(inpid, ipivalorreferencia, ipiquantidade, ipiano, iigid, ipistatus, ipiquantidadeaprovada, ipiaprovado)
                        VALUES({$v['inpid']}, {$v['reivalor']}, {$v['reiquantidade']}, {$v['reiano']}, {$v['iigid']}, 'A', null, null) returning ipiid";
                $ipiid = $this->model->pegaUm($sql);
            }            
            
            $sql = "UPDATE par3.reformulacao_itemcomposicao SET ipiid = $ipiid WHERE reiid = {$v['reiid']}";
            $this->model->executar($sql);
            
            $sql = "SELECT rieid, ipiid, riequantidade, riestatus, co_entidade, escid FROM par3.reformulacao_item_composicao_escola WHERE refid = $refid AND reiid = {$v['reiid']}";
            $arrItemEscola = $this->model->carregar($sql);
            $arrItemEscola = $arrItemEscola ? $arrItemEscola : array();
            
            foreach ($arrItemEscola as $escola) {
                
                $ipeid = $this->model->pegaUm("SELECT ipeid FROM par3.iniciativa_planejamento_item_composicao_escola WHERE escid = {$escola['escid']} AND ipiid = $ipiid");
                if( !empty($ipeid) ){
                    $sql = "UPDATE par3.iniciativa_planejamento_item_composicao_escola SET ipequantidade = {$escola['riequantidade']} where ipeid = $ipeid";
                    $this->model->executar($sql);
                } else {
                    $sql = "INSERT INTO par3.iniciativa_planejamento_item_composicao_escola(ipiid, ipequantidade, ipestatus, co_entidade, escid)
                            VALUES($ipiid, {$escola['riequantidade']}, 'A', {$escola['co_entidade']}, {$escola['escid']}) returning ipeid";
                    $ipeid = $this->model->pegaUm($sql);
                }
                
                $sql = "UPDATE par3.reformulacao_item_composicao_escola SET ipeid = $ipeid WHERE rieid = {$escola['rieid']}";
                $this->model->executar($sql);
            }
            
            $sql = "SELECT raiid, inpid, aicid, ipiid, iigid, anaid, refid, refidpai, reiid, raiqtdaprovado, raivaloraprovado, usucpf, raidata, raiaprovado, raistatus
                	FROM par3.reformulacao_analise_itemcomposicao re
                	WHERE refid = $refid AND aicid IS NULL AND reiid = {$v['reiid']}";
            $arrItemAnalise = $this->model->carregar($sql);
            $arrItemAnalise = $arrItemAnalise ? $arrItemAnalise : array();
            
            foreach ($arrItemAnalise as $analise) {
                $aicid = $this->model->pegaUm("SELECT aicid FROM par3.analise_itemcomposicao WHERE ipiid = $ipiid AND anaid = {$analise['anaid']} AND aicstatus = 'A'");

                if( !empty($aicid) ){
                    $sql = "UPDATE par3.analise_itemcomposicao SET aicqtdaprovado = {$analise['raiqtdaprovado']}, aicvaloraprovado = {$analise['raivaloraprovado']} WHERE aicid = $aicid";
                    $this->model->executar($sql);
                } else {                
                    $sql = "INSERT INTO par3.analise_itemcomposicao(ipiid, anaid, aicqtdaprovado, aicvaloraprovado, usucpf, aicdata, aicaprovado, aicstatus)
                            VALUES($ipiid, {$analise['anaid']}, {$analise['raiqtdaprovado']}, {$analise['raivaloraprovado']}, '{$analise['usucpf']}', '{$analise['raidata']}', null, 'A') returning aicid";
                    $aicid = $this->model->pegaUm($sql);
                }
                
                $sql = "UPDATE par3.reformulacao_analise_itemcomposicao SET ipiid = $ipiid, aicid = $aicid WHERE raiid = {$analise['raiid']}";
                $this->model->executar($sql);
            }
        }
        return $this->model->commit();
    }
    
    public function pegaIdWorkFlow($refid = '', $esdid = '2363', $dotid = '', $tirid = PAR3_REFORMULACAO_INICIATIVA, $tpdid = 355)
    {
        require_once APPRAIZ . '/includes/workflow.php';
        
        if ($refid) {
            $arDados = $this->pegaReformulacaoDocumento($dotid, $refid, $tirid);
            $docid = $arDados['docid'];
        } else {
            // descrição do documento
            $docdsc = "Fluxo de Reformulação do PAR3: refid - " . $refid;
            // cria documento do WORKFLOW
            //2363 - Em cadastramento
            $docid = wf_cadastrarDocumento($tpdid, $docdsc, $esdid);
        }
        return $docid;
    }
    
    public function carregaEsdidAtual( $docid )
    {
        $sql = "select ed.esdid, ed.esddsc from workflow.documento d
					inner join workflow.estadodocumento ed on ed.esdid = d.esdid
				where docid = $docid";
        $arEstato = $this->model->pegaLinha($sql);
        return $arEstato;
    }
    
    public function listar($arrPost)
    {
        $where = '';
        if($arrPost['muncod'] && count($arrPost['muncod']) != 0 ){
            $arrMuncod = array_filter($arrPost['muncod'],function($val){return !empty($val);});
            if(count($arrMuncod) != 0){
                $where  .= " AND iu.muncod::integer in (".implode(',',$arrMuncod).")";
            }
        }
        
        $where  .= $arrPost['estuf'] ? " AND iu.estuf = '{$arrPost['estuf']}'":'';
        $where  .= $arrPost['proid'] ? " AND p.proid = '{$arrPost['proid']}'":'';
        $where  .= $arrPost['itrid'] && $arrPost['itrid'] != 0 ?($arrPost['itrid'] == 1 ? "AND iu.itrid in(1,3)" :"AND iu.itrid = {$arrPost['itrid']}"): "";
        $where  .= $arrPost['intoid'] ? " AND obj.intoid in (".implode(',', $arrPost['intoid']).")":'';
        $where  .= $arrPost['dotnumero'] ? " AND dt.dotnumero ilike '%{$arrPost['dotnumero']}%'":'';
        $where  .= $arrPost['dotnumero'] ? " AND dt.dotnumero ilike '%".trim(explode('-', $arrPost['dotnumero'])[0])."%'":'';
        
        if( $arrPost['iniano'] ) $where  .= " AND dt.ano = '{$arrPost['iniano']}'";
        
        if($arrPost['dataini'] != null ) {
            if($dataVigenciaInicio = DateTime::createFromFormat('d/m/Y',$arrPost['dataini'])) {
                $dtVigenciaIni = $dataVigenciaInicio->format('Y-m-d');
                $where .= " and dt.refdata >= '{$dtVigenciaIni}' ";
            }
        }
        
        if($arrPost['datafim'] != null ) {
            if ($dataVigenciaFim = DateTime::createFromFormat('d/m/Y', $arrPost['datafim'])) {
                $dtVigenciaFim = $dataVigenciaFim->format('Y-m-d');
                $where .= " and dt.refdata <= '{$dtVigenciaFim}' ";
            }
        }
        
        /*if( $arrPost['dataini'] && $arrPost['datafim'] ){
            $where .= " AND dt.refdata BETWEEN '".formata_data_sql($arrPost['dataini'])."' AND '".formata_data_sql($arrPost['datafim'])."' ";
        } else {
            if( $arrPost['dataini']) $where .= " AND dt.refdata = '".formata_data_sql($arrPost['dataini'])."'";
            if($arrPost['datafim']) $where .= " AND dt.refdata = '".formata_data_sql($arrPost['datafim'])."'";
        } */       
        if($arrPost['numeroprocesso']) {
            $arrPost['numeroprocesso'] = str_replace(".","", $arrPost['numeroprocesso']);
            $arrPost['numeroprocesso'] = str_replace("/","", $arrPost['numeroprocesso']);
            $arrPost['numeroprocesso'] = str_replace("-","", $arrPost['numeroprocesso']);
            $where .= " and p.pronumeroprocesso = '".$arrPost['numeroprocesso']."'";
        }        
        if( $arrPost['codigo'] ) $where .= " AND pp.inpid = '{$arrPost['codigo']}' ";
        if( $arrPost['tirid'] != '0' && !empty($arrPost['tirid']) ) $where .= " AND dt.tirid = {$arrPost['tirid']} ";
        if( $arrPost['esdid'] ) $where .= " AND dt.esdid = {$arrPost['esdid']} ";
        if( $arrPost['tipoassistencia'] ) $where .= " AND p.intaid = {$arrPost['tipoassistencia']} ";
        if( $arrPost['iniid'] ) $where .= " AND ini.iniid in (".implode(',', $arrPost['iniid']).")";
        if( $arrPost['iarid'] ) $where .= " AND ({$arrPost['iarid']}) in (SELECT DISTINCT
										                        iar.iarid
										                        FROM par3.iniciativa_iniciativas_areas iiar
										                        INNER JOIN par3.iniciativa_areas iar ON iar.iarid = iiar.iarid
										                        WHERE iniid = ini.iniid) ";
        if( $arrPost['tirid'] == 2 ){
            $sql = $this->model->sqlListaReformulacaoIniciativa($where, $arrPost);
            
            $arrayCabecalho = array('Número','Processo','UF', 'Entidade', 'Tipo de Objeto', 'Data Vigência', 'Valor Termo', 'Valor Empenho', 'Valor Pagamento', 'Assistência');
            $esconderColunas = array('proid', 'dotid', 'inuid', 'refid', 'tirid');
        } else {
            $sql = $this->model->sqlListaReformulacaoIniciativa($where, $arrPost);
            
           
            
            $arrayCabecalho = array('Número','Processo','UF', 'Entidade', 'Tipo de Objeto', 'Data Vigência', 'Valor Termo', 'Valor Empenho', 'Valor Pagamento', 'Assistência');
            $esconderColunas = array('proid', 'dotid', 'inuid', 'refid', 'tirid');
        }
        
        if( $arrPost['requisicao'] == 'excel' ){
            ob_clean();
            $listagemSimec = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
            $listagemSimec->setCabecalho( $arrayCabecalho );
            $listagemSimec->esconderColunas($esconderColunas);
            $listagemSimec->setQuery($sql);
            $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagemSimec->addCallbackDeCampo(['vlrempenho', 'dotvalortermo', 'vlrpago', 'vlr_solicitado', 'vlr_aprovado'], 'formataNumeroMonetarioSemSimbolo');
            $listagemSimec->addCallbackDeCampo('pronumeroprocesso', 'formata_numero_processo_sem_html');
            $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            die;
        } else {
            $listagemSimec = new Simec_Listagem();
            $listagemSimec->setCabecalho($arrayCabecalho);
            $listagemSimec->esconderColunas($esconderColunas);
            $listagemSimec->turnOnPesquisator();
            $listagemSimec->setQuery($sql);
            $listagemSimec->addAcao('plus', array('func' => 'detalharReformulacao', 'extra-params' => array('inuid', 'proid', 'tirid')));
            //$listagemSimec->addAcao('view', array('func' => 'visualizaReformulacao', 'extra-params' => array('inuid', 'proid', 'refid')));
            $listagemSimec->addCallbackDeCampo('pronumeroprocesso','formata_numero_processo');
            $listagemSimec->addCallbackDeCampo(['vlrempenho', 'dotvalortermo', 'vlrpago', 'vlr_solicitado', 'vlr_aprovado'], 'formataNumeroMonetarioSemSimbolo');
            //$listagemSimec->addCallbackDeCampo('docid', 'popOverSituacaoObra');
            $listagemSimec->setFormFiltros('form-pesquisa-reformulacao');
            $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagemSimec->setTamanhoPagina(50);
            $listagemSimec->setCampos($arrayCabecalho);
            
            $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }
    }
    
    public function listarProrrogacaoPrazo($arrPost)
    {
        $where = '';
        if($arrPost['muncod'] && count($arrPost['muncod']) != 0 ){
            $arrMuncod = array_filter($arrPost['muncod'],function($val){return !empty($val);});
            if(count($arrMuncod) != 0){
                $where  .= " AND iu.muncod::integer in (".implode(',',$arrMuncod).")";
            }
        }
        $where  .= $arrPost['estuf'] ? " AND iu.estuf = '{$arrPost['estuf']}'":'';
        $where  .= $arrPost['proid'] ? " AND p.proid = '{$arrPost['proid']}'":'';
        $where  .= $arrPost['itrid'] && $arrPost['itrid'] != 0 ?($arrPost['itrid'] == 1 ? "AND iu.itrid in(1,3)" :"AND iu.itrid = {$arrPost['itrid']}"): "";
        $where  .= $arrPost['intoid'] ? " AND obj.intoid in (".implode(',', $arrPost['intoid']).")":'';
        $where  .= $arrPost['obrid'] ? " AND obj.intoid = {$arrPost['intoid']}":'';
        $where  .= $arrPost['dotnumero'] ? " AND dt.dotnumero ilike '%".trim(explode('-', $arrPost['dotnumero'])[0])."%'":'';
        
        if( $arrPost['iniano'] ) $where  .= " AND dt.ano = '{$arrPost['iniano']}'";
        
        if($arrPost['datainiVigencia'] != null ) {
            if($dataVigenciaInicio = DateTime::createFromFormat('d/m/Y',$arrPost['datainiVigencia'])) {
                $dtVigenciaIni = $dataVigenciaInicio->format('Y-m-d');
                $where .= " and dt.data_vigencia >= '{$dtVigenciaIni}' ";
            }
        }
        
        if($arrPost['datafimVigencia'] != null ) {
            if ($dataVigenciaFim = DateTime::createFromFormat('d/m/Y', $arrPost['datafimVigencia'])) {
                $dtVigenciaFim = $dataVigenciaFim->format('Y-m-d');
                $where .= " and dt.data_vigencia <= '{$dtVigenciaFim}' ";
            }
        }
        
        if($arrPost['dataini'] != null ) {
            if($dataVigenciaInicio = DateTime::createFromFormat('d/m/Y',$arrPost['dataini'])) {
                $dtVigenciaIni = $dataVigenciaInicio->format('Y-m-d');
                $where .= " and dt.data_solicitacao >= '{$dtVigenciaIni}' ";
            }
        }
        
        if($arrPost['datafim'] != null ) {
            if ($dataVigenciaFim = DateTime::createFromFormat('d/m/Y', $arrPost['datafim'])) {
                $dtVigenciaFim = $dataVigenciaFim->format('Y-m-d');
                $where .= " and dt.data_solicitacao <= '{$dtVigenciaFim}' ";
            }
        }
        
        /*if( $arrPost['datainiVigencia'] && $arrPost['datafimVigencia'] ){
            $where .= " AND dt.data_vigencia BETWEEN '".formata_data_sql($arrPost['datainiVigencia'])."' AND '".formata_data_sql($arrPost['datafimVigencia'])."' ";
        } else {
            if( $arrPost['datainiVigencia']) $where .= " AND dt.data_vigencia = '".formata_data_sql($arrPost['datainiVigencia'])."'";
            if($arrPost['datafimVigencia']) $where .= " AND dt.data_vigencia = '".formata_data_sql($arrPost['datafimVigencia'])."'";
        }*/
        
        /*if( $arrPost['dataini'] && $arrPost['datafim'] ){
            $where .= " AND dt.data_solicitacao BETWEEN '".formata_data_sql($arrPost['dataini'])."' AND '".formata_data_sql($arrPost['datafim'])."' ";
        } else {
            if( $arrPost['dataini']) $where .= " AND dt.data_solicitacao = '".formata_data_sql($arrPost['dataini'])."'";
            if($arrPost['datafim']) $where .= " AND dt.data_solicitacao = '".formata_data_sql($arrPost['datafim'])."'";
        }*/
        
        if($arrPost['numeroprocesso']) {
            $arrPost['numeroprocesso'] = str_replace(".","", $arrPost['numeroprocesso']);
            $arrPost['numeroprocesso'] = str_replace("/","", $arrPost['numeroprocesso']);
            $arrPost['numeroprocesso'] = str_replace("-","", $arrPost['numeroprocesso']);
            $where .= " and p.pronumeroprocesso = '".$arrPost['numeroprocesso']."'";
        }        
        if( $arrPost['codigo'] ){
            $whereObra = " AND o.obrid = '{$arrPost['codigo']}' ";
            $wherePlan = " AND pp.inpid = '{$arrPost['codigo']}' ";
        }
        
        if( $arrPost['tirid'] != '0' && !empty($arrPost['tirid']) ) $where .= " AND dt.tirid = {$arrPost['tirid']} ";
        if( $arrPost['esdid'] ) $where .= " AND dt.esdid = {$arrPost['esdid']} ";
        if( $arrPost['tipoassistencia'] ) $where .= " AND p.intaid = {$arrPost['tipoassistencia']} ";
        if( $arrPost['iniid'] ) $where .= " AND ini.iniid in (".implode(', ', $arrPost['iniid']).") ";
        if( $arrPost['iarid'] ) $where .= " AND ({$arrPost['iarid']}) in (SELECT DISTINCT
										                        iar.iarid
										                        FROM par3.iniciativa_iniciativas_areas iiar
										                        INNER JOIN par3.iniciativa_areas iar ON iar.iarid = iiar.iarid
										                        WHERE iniid = ini.iniid) ";
        
        $sql = $this->model->sqlListaReprogramacaoPrazo($where);
        
       
        
        $arrayCabecalho = array('UF', 'Entidade','Processo', 'Documento', 'Tipo de Objeto', 'Data Vigência', 'Data Solicitação', 'Data Requisitada', 'Data Aprovada', 'Data Validação', 'Assistência', 'Situação', 'Justificativa');
        $esconderColunas = array('proid', 'dotid', 'inuid', 'refid', 'esdid', 'tirid', 'justificativa_completa', 'tipo');
        
        if( $arrPost['requisicao'] == 'excel' ){
            ob_clean();
            $listagemSimec = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
            $listagemSimec->setCabecalho( $arrayCabecalho );
            $listagemSimec->esconderColunas($esconderColunas);
            $listagemSimec->setQuery($sql);
            $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            //$listagemSimec->addCallbackDeCampo(['vlrempenho', 'dotvalortermo', 'vlrpago', 'vlr_solicitado', 'vlr_aprovado'], 'formataNumeroMonetarioSemSimbolo');
            $listagemSimec->addCallbackDeCampo('pronumeroprocesso', 'formata_numero_processo_sem_html');
            $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            die;
        } else {
            $listagemSimec = new Simec_Listagem();
            $listagemSimec->setCabecalho($arrayCabecalho);
            $listagemSimec->esconderColunas($esconderColunas);
            $listagemSimec->turnOnPesquisator();
            $listagemSimec->setQuery($sql);
            $listagemSimec->addAcao('plus', array('func' => 'detalharReformulacao', 'extra-params' => array('inuid', 'proid', 'refid', 'tirid', 'tipo')));
            //$listagemSimec->addAcao('view', array('func' => 'visualizaReformulacao', 'extra-params' => array('inuid', 'proid', 'refid')));
            $listagemSimec->addCallbackDeCampo('pronumeroprocesso','formata_numero_processo');
            //$listagemSimec->addCallbackDeCampo(['vlrempenho', 'dotvalortermo', 'vlrpago', 'vlr_solicitado', 'vlr_aprovado'], 'formataNumeroMonetarioSemSimbolo');
            //$listagemSimec->addCallbackDeCampo('docid', 'popOverSituacaoObra');
            $listagemSimec->addCallbackDeCampo('refjustificativa', 'popOverJustificativaReformulacao');
            $listagemSimec->setFormFiltros('form-pesquisa-reformulacao');
            $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagemSimec->setTamanhoPagina(50);
            $listagemSimec->setCampos($arrayCabecalho);
            
            $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
        }
    }
    
    public function verificaPermissaoTela( $esdid, $refid )
    {
        $arrPermissao = array();
        
        $arrPerfil = pegaPerfils($_SESSION['usucpf']);
        
        $arrPermissao['cadastro'] = array('salvar' => 'N', 'visualizar' => 'N');
        $arrPermissao['analise'] = array('salvar' => 'N', 'visualizar' => 'N');
        
        $arrPerfilSolicitar = array(PAR3_PERFIL_SECRETARIO_ESTADUAL, PAR3_PERFIL_PREFEITO, PAR3_PERFIL_DIRIGENTE_MUNICIPAL, PAR3_PERFIL_EQUIPE_MUNICIPAL, PAR3_PERFIL_EQUIPE_ESTADUAL);
        $arrPerfilVisualizar = array(PAR3_PERFIL_EQUIPE_MUNICIPAL, PAR3_PERFIL_EQUIPE_ESTADUAL, PAR3_PERFIL_CONSULTA_ESTADUAL, PAR3_PERFIL_CONSULTA_MUNICIPAL, PAR3_PERFIL_CONSULTA_GERAL );
        $arrPerfilAnalisar = array(PAR3_PERFIL_ADMINISTRADOR, PAR3_PERFIL_ANALISTA_DE_PROGRAMAS, PAR3_PERFIL_ANALISTA_PLANEJAMENTO);
        $arrPerfilPodeTudo = array(PAR3_PERFIL_SUPER_USUARIO, PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR);
        
        switch ($esdid) {
            case PAR3_REFORMULACAO_ESDID_EM_CADASTRAMENTO:
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_CADASTRAMENTO:
                if(checaPerfilValido($arrPerfilPodeTudo, $arrPerfil) == 'S'){
                    $arrPermissao['cadastro'] = array('salvar' => 'S');
                    $arrPermissao['analise'] = array('salvar' => 'N', 'visualizar' => 'N');
                } else {
                    if( checaPerfilValido($arrPerfilSolicitar, $arrPerfil) == 'S' ){
                        $arrPermissao['cadastro'] = array('salvar' => 'S');
                        $arrPermissao['analise'] = array('salvar' => 'N', 'visualizar' => 'N');
                    } else {
                        if( (checaPerfilValido($arrPerfilVisualizar, $arrPerfil) == 'S') || (checaPerfilValido($arrPerfilAnalisar, $arrPerfil) == 'S') ){
                            $arrPermissao['cadastro'] = array('salvar' => 'N');
                            $arrPermissao['analise'] = array('salvar' => 'N', 'visualizar' => 'N');
                        }
                    }
                }
            break;
            case PAR3_REFORMULACAO_ESDID_EM_DILIGENCIA:
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_DILIGENCIA:
                if(checaPerfilValido($arrPerfilPodeTudo, $arrPerfil) == 'S'){
                    $arrPermissao['cadastro'] = array('salvar' => 'S');
                    $arrPermissao['analise'] = array('salvar' => 'N', 'visualizar' => 'S');
                } else {
                    if( (checaPerfilValido($arrPerfilSolicitar, $arrPerfil) == 'S') ){
                        $arrPermissao['cadastro'] = array('salvar' => 'S');
                        $arrPermissao['analise'] = array('salvar' => 'N', 'visualizar' => 'S');
                    } else {
                        if( (checaPerfilValido($arrPerfilVisualizar, $arrPerfil) == 'S') || (checaPerfilValido($arrPerfilAnalisar, $arrPerfil) == 'S') ){
                            $arrPermissao['cadastro'] = array('salvar' => 'N');
                            $arrPermissao['analise'] = array('salvar' => 'N', 'visualizar' => 'S');
                        }
                    }
                }
            break;
            case PAR3_REFORMULACAO_ESDID_AGUARDANDO_ANALISE:
            case PAR3_REFORMULACAO_ESDID_EM_ANALISE:
            case PAR3_REFORMULACAO_ESDID_EM_ANALISE_RETORNO_DILIGENCIA:
                if(checaPerfilValido($arrPerfilPodeTudo, $arrPerfil) == 'S'){
                    $arrPermissao['cadastro'] = array('salvar' => 'N');
                    $arrPermissao['analise'] = array('salvar' => 'S', 'visualizar' => 'S');
                } else {
                    if( (checaPerfilValido($arrPerfilSolicitar, $arrPerfil) == 'S') ){
                        $arrPermissao['cadastro'] = array('salvar' => 'N');
                        $arrPermissao['analise'] = array('salvar' => 'N', 'visualizar' => 'N');
                    } else {
                        if( (checaPerfilValido($arrPerfilAnalisar, $arrPerfil) == 'S') || (checaPerfilValido($arrPerfilPodeTudo, $arrPerfil) == 'S') ){
                            $arrPermissao['cadastro'] = array('salvar' => 'N');
                            $arrPermissao['analise'] = array('salvar' => 'S', 'visualizar' => 'S');
                        }
                    }
                }
            break;
            default:
                $arrPermissao['cadastro'] = array('salvar' => 'N');
                $arrPermissao['analise'] = array('salvar' => 'N', 'visualizar' => 'S');
            break;
        }
        
        return $arrPermissao;
    }
    
    public function pegaPerfilAcaoWorkflow( $esdid, $aedid )
    {
        global $db;        
        $sql = "SELECT DISTINCT p.pflcod, p.pfldsc 
                FROM workflow.acaoestadodoc ae 
                	INNER JOIN workflow.estadodocumentoperfil ep ON ep.aedid = ae.aedid
                	INNER JOIN seguranca.perfil p ON p.pflcod = ep.pflcod
                WHERE ae.esdidorigem in ({$esdid}) and ae.aedid = $aedid";
        
        return $db->carregarColuna($sql, 'pflcod');
    }
    
    public function getPermissaoAttr($campo)
    {
        return $this->arPermissao[$this->esdid_atual][$campo]['attribs'];
    }

    public function getPermissaoConfig($campo)
    {
        return $this->arPermissao[$this->esdid_atual][$campo]['config'];        
    }
    
    public function verificaPermissaoTelaReprogramacaoPrazo($esdid, $refid )
    {
        $this->esdid_atual = $esdid;
        
        $arrPermissao = array();
        
        $arrPerfil = pegaPerfils($_SESSION['usucpf']);
        
        $this->arPermissao[$esdid] = array(
            'btnSalvar'             => array('attribs' => array('disabled' => 'disabled')),
            'btnAnexo'              => array('attribs' => array('disabled' => 'disabled')),
            'btnSalvarAnalise'      => array('attribs' => array('disabled' => 'disabled')),
            'refdata_solicitada'    => array('attribs' => array('autocomplete' => 'off', 'readonly' => 'readonly'), 'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true, 'pode-editar' => 'N')),
            'refdata_aprovada'      => array('attribs' => array('autocomplete' => 'off', 'readonly' => 'readonly'), 'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true, 'pode-editar' => 'N')),
            'refjustificativa'      => array('attribs' => array('maxlengh' => 4000, 'readonly' => 'readonly'),      'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true)),
            'refacoestomadas'       => array('attribs' => array('maxlengh' => 4000, 'readonly' => 'readonly'),      'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true)),
            'refoutrasinformacoes'  => array('attribs' => array('maxlengh' => 4000, 'readonly' => 'readonly'),      'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true)),
            'refconsideracoes'      => array('attribs' => array('maxlengh' => 4000, 'readonly' => 'readonly'),      'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true)),
            'refsituacao'           => array('attribs' => array('disabled' => 'disabled'),                          'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true)),
        );
        
        switch ($esdid) {
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_CADASTRAMENTO:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5772) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']  = array('disabled' => '');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']   = array('disabled' => '');
                    
                    $this->arPermissao[$esdid]['refdata_solicitada']    = array('attribs' => array('autocomplete' => 'off'),   'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refdata_aprovada']      = array('attribs' => array('autocomplete' => 'off'),   'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refjustificativa']      = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refacoestomadas']       = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refoutrasinformacoes']  = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refconsideracoes']      = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refsituacao']           = array('attribs' => array('disabled' => ''),          'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_DILIGENCIA:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5790) , $arrPerfil) == 'S'){                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']['disabled']  = '';
                    $this->arPermissao[$esdid]['btnAnexo']['attribs'] ['disabled']  = '';
                    $this->arPermissao[$esdid]['refdata_solicitada']                = array('attribs' => array('autocomplete' => 'off'), 'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refoutrasinformacoes']              = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true));
                    
                    $this->arPermissao[$esdid]['refdata_aprovada']      = array('attribs' => array('autocomplete' => 'off'),   'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refconsideracoes']      = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refsituacao']           = array('attribs' => array('disabled' => ''),          'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_DILIGENCIA:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5812) , $arrPerfil) == 'S'){
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']['disabled']  = '';
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']['disabled']   = '';
                    $this->arPermissao[$esdid]['refdata_solicitada']                = array('attribs' => array('autocomplete' => 'off'), 'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refoutrasinformacoes']              = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true));
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_AGUARDANDO_ANALISE:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5777) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => '');
                    
                    
                }
                if( checaPerfilValido( array(PAR3_PERFIL_PREFEITO, PAR3_PERFIL_SECRETARIO_ESTADUAL) , $arrPerfil) == 'S' ){
                    $this->arPermissao[$esdid]['refdata_aprovada']      = array('attribs' => array('autocomplete' => 'off'),   'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refconsideracoes']      = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refsituacao']           = array('attribs' => array('disabled' => ''),          'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5778) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => '');
                    
                    $this->arPermissao[$esdid]['refdata_aprovada']  = array('attribs' => array('autocomplete' => 'off'),       'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refconsideracoes']  = array('attribs' => array('autocomplete' => 'off'),       'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refsituacao']       = array('attribs' => array('autocomplete' => 'off'),       'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true));
                }
                if( checaPerfilValido( array(PAR3_PERFIL_PREFEITO, PAR3_PERFIL_SECRETARIO_ESTADUAL) , $arrPerfil) == 'S' ){
                    $this->arPermissao[$esdid]['refdata_aprovada']      = array('attribs' => array('autocomplete' => 'off'),   'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refconsideracoes']      = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refsituacao']           = array('attribs' => array('disabled' => ''),          'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                }
            break;            
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE_CGEST:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5783) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => 'disabled');
                }
                if( checaPerfilValido( array(PAR3_PERFIL_PREFEITO, PAR3_PERFIL_SECRETARIO_ESTADUAL) , $arrPerfil) == 'S' ){
                    $this->arPermissao[$esdid]['refdata_aprovada']      = array('attribs' => array('autocomplete' => 'off'),   'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refconsideracoes']      = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refsituacao']           = array('attribs' => array('disabled' => ''),          'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE_CGIMP:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5785) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => 'disabled');
                }
                if( checaPerfilValido( array(PAR3_PERFIL_PREFEITO, PAR3_PERFIL_SECRETARIO_ESTADUAL) , $arrPerfil) == 'S' ){
                    $this->arPermissao[$esdid]['refdata_aprovada']      = array('attribs' => array('autocomplete' => 'off'),   'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refconsideracoes']      = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refsituacao']           = array('attribs' => array('disabled' => ''),          'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE_CGPES:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5786) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => 'disabled');
                }
                if( checaPerfilValido( array(PAR3_PERFIL_PREFEITO, PAR3_PERFIL_SECRETARIO_ESTADUAL) , $arrPerfil) == 'S' ){
                    $this->arPermissao[$esdid]['refdata_aprovada']      = array('attribs' => array('autocomplete' => 'off'),   'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refconsideracoes']      = array('attribs' => array('maxlengh' => 4000),        'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => false));
                    $this->arPermissao[$esdid]['refsituacao']           = array('attribs' => array('disabled' => ''),          'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => false));
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_AGUARDANDO_VALIDACAO_COORDENADOR:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5778) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => '');
                    
                    $this->arPermissao[$esdid]['refdata_aprovada']  = array('attribs' => array('autocomplete' => 'off'),       'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refconsideracoes']  = array('attribs' => array('autocomplete' => 'off'),       'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refsituacao']       = array('attribs' => array('autocomplete' => 'off'),       'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true));
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_AGUARDANDO_ANALISE:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5799) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => 'disabled');
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE:                
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5805) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => '');
                    
                    $this->arPermissao[$esdid]['refdata_aprovada']  = array('attribs' => array('autocomplete' => 'off'),       'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refconsideracoes']  = array('attribs' => array('autocomplete' => 'off'),       'config' => array('input-size' => '9', 'label-size' => '2', 'visible' => true));
                    $this->arPermissao[$esdid]['refsituacao']       = array('attribs' => array('autocomplete' => 'off'),       'config' => array('input-size' => '2', 'label-size' => '2', 'visible' => true));
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE_CGIMP:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5809) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => 'disabled');
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE_CGPES:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5811) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => 'disabled');
                }
            break;
            case PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE_CGEST:
                if(checaPerfilValido( $this->pegaPerfilAcaoWorkflow($esdid, 5807) , $arrPerfil) == 'S'){
                    
                    $this->arPermissao[$esdid]['btnSalvar']['attribs']          = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnAnexo']['attribs']           = array('disabled' => 'disabled');
                    $this->arPermissao[$esdid]['btnSalvarAnalise']['attribs']   = array('disabled' => 'disabled');
                }
            break;
        }
        
//         return $this->arPermissao;
    }
    
    public function validaFormularioDocumentoReformulacao($arrPost = array())
    {
        $arrErro = array();
        if( empty($arrPost['refdata_solicitada']) ){
            $arrErro['refdata_solicitada'] = ['Campo Obrigatório'];
        }
        if( empty($arrPost['refjustificativa']) ){
            $arrErro['refjustificativa'] = ['Campo Obrigatório'];
        }
        if( empty($arrPost['refacoestomadas']) ){
            $arrErro['refacoestomadas'] = ['Campo Obrigatório'];
        }
        if( empty($arrPost['refoutrasinformacoes']) ){
            $arrErro['refoutrasinformacoes'] = ['Campo Obrigatório'];
        }
        //refconsideracoes
        return $arrErro;
    }
    
    public function salvarDocumentoReformulacao($dados)
    {
        
        $tamanhoMax = 10 * 1024 * 1024;
        $inpid = $_GET['inpid'];
        $ipaid = 0;
        
        if ($_FILES['arqid']['error'] == UPLOAD_ERR_NO_FILE) {
            echo 'emptyfile';
            die();
        }
        
        if ($_FILES['arqid']['name']) {
            if ($_FILES['arqid']['size'] > $tamanhoMax) {
                echo 'maxsize';
                die();
            }
            
            $_FILES['arqid']['name'] = remove_acentos($_FILES['arqid']['name']);
            
            $file = new FilesSimec("reformulacao_anexo", [], 'par3');
            
            $file->setUpload('PAR3 - Documentos Reformulação Termo', 'arqid', false);
            $arqid = $file->getIdArquivo();
            
            $valueDsc = '';
            if ($dados['axrdescricao']) {
                $dsc = ($dados['axrdescricao']);
                $valueDsc = "'{$dsc}'";
            }
            $sql = "INSERT INTO par3.reformulacao_anexo(refid, arqid, axrdescricao, axrstatus)
                    VALUES({$dados['refid']}, $arqid, {$valueDsc}, 'A') returning axrid";
            
            $axrid = $this->model->pegaUm($sql);
            $this->model->commit();
            
        }
        if ($axrid > 0) {
            $this->getListaArquivosReformulacao($dados['refid'], $dados['esdid']);
        } else {
            echo 'error';
        }
        exit;
    }
    
    public function getListaArquivosReformulacao($refid, $esdid)
    {        
        $sql = "SELECT r.axrid, r.refid, r.arqid, r.axrdescricao, to_char(a.arqdata, 'DD/MM/YYYY') AS arqdata, u.usunome 
                FROM par3.reformulacao_anexo r
                    INNER JOIN public.arquivo a ON a.arqid = r.arqid
                    INNER JOIN seguranca.usuario u ON u.usucpf = a.usucpf
                WHERE r.refid = {$refid} AND axrstatus = 'A'";
        
        $listagemSimec = new Simec_Listagem();
        $listagemSimec->setCabecalho(array('Nome', 'Descrição', 'Data', 'Usuário'));
        $listagemSimec->esconderColunas(array('axrid', 'refid'));
        $listagemSimec->setQuery($sql);
        if( $this->getPermissaoAttr('btnAnexo')['disabled'] == '' ){
            $listagemSimec->addAcao('delete', array('func' => 'deletarAnexo', 'extra-params' => array('axrid', 'arqid'), 'titulo' => 'Excluir Anexo'));
        }
        $listagemSimec->addCallbackDeCampo(['arqid'], 'download_arquivo');
        $listagemSimec->setCampos(array('Descrição'));
        $listagemSimec->setFormOff();
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
    
    public function carregaHistoricoReformulacao($dotid, $inuid, $proid, $tirid)
    {
        
        $sql = "SELECT distinct dt.dotid,
                    (SELECT max(dotid) FROM par3.documentotermo WHERE proid = p.proid AND dotstatus = 'A') AS dotid_id,
                    p.proid, p.inuid,
                    dt.refid,
                    to_char(dt.data_vigencia, 'DD/MM/YYYY') as data_vigencia,
                    dt.dotvalortermo,
                    to_char(dt.refdata, 'DD/MM/YYYY') as data_solicitada,
                    dt.vlr_solicitado, dt.vlr_aprovado,
                    e.vlrempenho as vlrempenho,
                    coalesce(e.vlr_pg_efetivado,0) as vlrpago,
                    dt.tirdescricao,
                    dt.docid,
                    dt.esdid,
                    coalesce(dt.dotid_ref,0) AS dotid_ref
                FROM par3.processo p
                	INNER JOIN par3.processoparcomposicao pp ON pp.proid = p.proid AND pp.ppcstatus = 'A'
                    inner join par3.v_saldo_empenho_do_processo e on e.processo = p.pronumeroprocesso
                    inner join par3.iniciativa_planejamento ip on ip.inpid = pp.inpid
                    INNER JOIN (
                		SELECT d.dotid, d.proid, d.dotdatafimvigencia AS data_vigencia, sum(d.dotvalortermo) AS dotvalortermo,
                			rd.refid, rq.vlr_solicitado, CASE WHEN esd.esdid in (".PAR3_REFORMULACAO_ESDID_REFORMULACAO_APROVADA.", ".PAR3_REFORMULACAO_ESDID_AGUARDANDO_COMPLEMENTACAO_EMPENHO.", ".PAR3_REFORMULACAO_ESDID_AGUARDANDO_COMPLEMENTACAO_EMPENHO.", ".PAR3_REFORMULACAO_ESDID_REPROGRAMACAO_FINALIZADA.") THEN rq.vlr_aprovado ELSE 0 END AS vlr_aprovado, doc.docid, esd.esdid, esd.esddsc, rd.refdata, rt.tirdescricao,
                            (SELECT max(dotid) FROM par3.documentotermo WHERE proid = rd.proid AND dotstatus = 'A' AND refid = rd.refid AND dotidpai IS NOT NULL) AS dotid_ref
                		FROM par3.reformulacao_documento rd
                           	INNER JOIN par3.documentotermo d ON rd.dotid = d.dotid /*AND rd.refstatus = 'A'*/ AND rd.refidpai IS NOT NULL
                           	inner join par3.modelodocumento md on md.mdoid = d.mdoid and md.mdostatus = 'A'
                           	inner JOIN workflow.documento doc ON doc.docid = rd.docid
                           	inner JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                           	left JOIN(
                           		SELECT ri.refid, sum(ri.reiquantidade * ri.reivalor) AS vlr_solicitado, sum(ra.raiqtdaprovado * ra.raivaloraprovado) AS vlr_aprovado
                           		FROM par3.reformulacao_itemcomposicao ri
                           			INNER JOIN par3.reformulacao_analise_itemcomposicao ra ON ra.reiid = ri.reiid --AND ra.raistatus = 'A'
                           		WHERE /*ri.reistatus = 'A' AND*/ ri.refidpai IS NOT NULL
                           		GROUP BY ri.refid
                           	) rq ON rq.refid = rd.refid
                           	INNER JOIN par3.reformulacao_tipos rt ON rt.tirid = rd.tirid
                        WHERE rd.tirid = {$tirid} --and d.dotstatus IN ('A', 'B')
                        GROUP BY d.dotid, d.proid, d.dotdatafimvigencia, rd.refid, rq.vlr_solicitado, rq.vlr_aprovado, esd.esddsc, rt.tirdescricao, doc.docid, esd.esdid
                    ) dt ON dt.proid = p.proid
                WHERE
                    ip.inpstatus = 'A'
                    and e.vlrempenho > 0
                    AND dt.proid = $proid
                GROUP BY dt.dotid, p.proid, p.inuid, p.pronumeroprocesso,
                    dt.data_vigencia, dt.dotvalortermo, e.vlrempenho, e.vlr_pg_efetivado, p.intaid, dt.refid, dt.vlr_solicitado, dt.vlr_aprovado, dt.esddsc, dt.refdata, 
                    dt.tirdescricao, dt.docid, dt.esdid, dt.refid, dt.dotid_ref
                order by dt.refid";
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Reformulação', 'Data Vigência', 'Valor Termo', 'Data da Solicitação', 'Valor Solicitado', 'Valor Aprovado', 'Valor Empenho', 'Valor Pagamento', 'Tipos de Reprogramação', 'Situação');
        $esconderColunas = array('proid', 'inuid', 'esdid', 'dotid_ref', 'dotid_id');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addAcao('info', array('func' => 'visualizaReformulacao', 'extra-params' => array('inuid', 'proid', 'refid')));
        $listagemSimec->addAcao('file', array('func' => 'gerarTermo', 'cor' => 'success', 'extra-params' => array('refid', 'inuid', 'proid', 'dotid_id')));
        $listagemSimec->addAcao('view', array('func' => 'visualizaTermo', 'extra-params' => array('dotid_ref')));
        $listagemSimec->addCallbackDeCampo(['vlrempenho', 'dotvalortermo', 'vlrpago', 'vlr_solicitado', 'vlr_aprovado'], 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addCallbackDeCampo('docid', 'popOverSituacaoObra');
        $listagemSimec->setAcaoComoCondicional('file', [['campo' => 'esdid', 'valor' => PAR3_REFORMULACAO_ESDID_AGUARDANDO_GERACAO_TERMO, 'op' => 'igual']]);
        $listagemSimec->setAcaoComoCondicional('view', [['campo' => 'dotid_ref', 'valor' => 0, 'op' => 'diferente']]);
        //$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function carregaHistoricoReformulacaoPrazo($dotid, $inuid, $proid)
    {
        
        $sql = "SELECT distinct dt.dotid,
                    (SELECT max(dotid) FROM par3.documentotermo WHERE proid = p.proid AND dotstatus = 'A') AS dotid_id,
                    p.proid, p.inuid,
                    dt.refid,
                    to_char(dt.data_vigencia, 'DD/MM/YYYY') as data_vigencia,
            	    to_char(dt.data_solicitacao, 'DD/MM/YYYY HH24:MI:SS') as data_solicitacao,
            	    to_char(dt.data_requisitada, 'DD/MM/YYYY') as data_requisitada,
            	    to_char(dt.data_aprovada, 'DD/MM/YYYY') as data_aprovada,
            	    dt.data_validacao,
            	    dt.refid,
                    substring(dt.refjustificativa,1,50)||'...' AS refjustificativa,
                    dt.refjustificativa as justificativa_completa, 
                    dt.docid,
                    dt.esdid,
                    coalesce(dt.dotid_ref,0) AS dotid_ref,
                    pp.tipo
                FROM par3.processo p
                	INNER JOIN(                		
						SELECT pp.proid, pp.inpid, 'P' as tipo FROM par3.processoparcomposicao pp WHERE pp.ppcstatus = 'A' AND pp.proid = $proid
						UNION ALL 
						SELECT pp.proid, o.inpid, 'O' as tipo FROM par3.processoobracomposicao pp 
							INNER JOIN par3.obra o ON o.obrid = pp.obrid AND o.obrstatus = 'A'
						WHERE pp.pocstatus = 'A' AND pp.proid = $proid
					) pp ON pp.proid = p.proid
                    inner join par3.v_saldo_empenho_do_processo e on e.processo = p.pronumeroprocesso
                    inner join par3.iniciativa_planejamento ip on ip.inpid = pp.inpid
                    INNER JOIN (
                		SELECT rd.refid, d.dotid, d.proid, (d.dotnumero || '-'::text) || d.intoid AS dotnumero, d.dotdatafimvigencia AS data_vigencia,
                	    		md.mdonome||'_'||ito.intodsc as termo, rt.tirdescricao, esd.esddsc, to_char(rd.refdata, 'YYYY') as ano, 
                	    		rd.refdata::date, rd.tirid, esd.esdid, rd.docid, rd.refdata_solicitada AS data_requisitada, rd.refdata_aprovada AS data_aprovada, CASE WHEN esd.esdid = 2462 THEN to_char(dtv.data_validacao, 'DD/MM/YYYY HH24:MI:SS') ELSE '-' END AS data_validacao,
                	    		(SELECT max(htddata) FROM workflow.historicodocumento WHERE docid = rd.docid AND aedid = 5772) AS data_solicitacao, rd.refjustificativa,
                	    		(SELECT max(dotid) FROM par3.documentotermo WHERE proid = rd.proid AND dotstatus = 'A' AND refid = rd.refid AND dotidpai IS NOT NULL) AS dotid_ref
                			FROM par3.documentotermo d
                				INNER JOIN par3.modelodocumento md on md.mdoid = d.mdoid and md.mdostatus = 'A'
                	           	INNER JOIN par3.iniciativa_tipos_objeto ito on ito.intoid = d.intoid
                	           	INNER JOIN par3.reformulacao_documento rd ON rd.dotid = d.dotid AND rd.refstatus = 'A' AND rd.refidpai IS NOT NULL
                                INNER JOIN workflow.documento doc ON doc.docid = rd.docid
                	           	INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                                LEFT JOIN(                	           	
                	           		SELECT max(dtvdatavalidacao) AS data_validacao, dotid FROM par3.documentotermovalidacao WHERE dtvstatus = 'A' GROUP BY dotid                	           		
                	           	) dtv ON dtv.dotid = d.dotid
                	           	LEFT JOIN(
                	           		SELECT ri.refid, sum(ri.reiquantidade * ri.reivalor) AS vlr_solicitado, sum(ra.raiqtdaprovado * ra.raivaloraprovado) AS vlr_aprovado
                	           		FROM par3.reformulacao_itemcomposicao ri
                	           			INNER JOIN par3.reformulacao_analise_itemcomposicao ra ON ra.reiid = ri.reiid AND ra.raistatus = 'A'
                	           		WHERE ri.reistatus = 'A' AND ri.refidpai IS NOT NULL
                	           		GROUP BY ri.refid
                	           	) rq ON rq.refid = rd.refid
                	           	INNER JOIN par3.reformulacao_tipos rt ON rt.tirid = rd.tirid
                	        WHERE rd.tirid = 1 AND d.proid = $proid 
                    ) dt ON dt.proid = p.proid
                WHERE
                    ip.inpstatus = 'A'
                    and e.vlrempenho > 0
                    AND dt.proid = $proid
                GROUP BY dt.dotid, p.proid, p.inuid, dt.dotnumero, p.pronumeroprocesso,  dt.data_vigencia, p.intaid, dt.termo, dt.refid, dt.docid, dt.esdid, 
                	dt.data_requisitada, dt.data_aprovada, dt.esddsc, dt.data_solicitacao, dt.refjustificativa, dt.dotid_ref, pp.tipo, dt.data_validacao
                order by dt.refid";
           
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Reformulação', 'Data Vigência', 'Data Solicitação', 'Data Requisitada', 'Data Aprovada', 'Data Validação', 'Justificativa', 'Situação');
        $esconderColunas = array('proid', 'dotid', 'inuid', 'esdid', 'tirid', 'dotid_ref', 'dotid_id', 'justificativa_completa', 'tipo');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addAcao('info', array('func' => 'visualizaReformulacao', 'extra-params' => array('inuid', 'proid', 'refid', 'tipo')));
        $listagemSimec->addAcao('file', array('func' => 'gerarTermo', 'cor' => 'success', 'extra-params' => array('refid', 'inuid', 'proid', 'dotid_id', 'tipo')));
        $listagemSimec->addAcao('view', array('func' => 'visualizaTermo', 'extra-params' => array('dotid_ref', 'tipo')));
        $listagemSimec->addCallbackDeCampo('docid', 'popOverSituacaoObra');
        $listagemSimec->addCallbackDeCampo('refjustificativa', 'popOverJustificativaReformulacao');
        $listagemSimec->setAcaoComoCondicional('file', [['campo' => 'esdid', 'valor' => PAR3_REFORMULACAO_PRAZO_ESDID_EM_GERACAO_TERMO, 'op' => 'igual']]);
        $listagemSimec->setAcaoComoCondicional('view', [['campo' => 'dotid_ref', 'valor' => 0, 'op' => 'diferente']]);
        //$listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function comboSituacaoReformulacao( $tirid = '' )
    {
        $tpdid = '355, 352';
        if( $tirid == 1 ){
            $tpdid = '352';
        }
        if( $tirid == 2 ){
            $tpdid = '355';
        }
        $sql = "SELECT
                	esdid as codigo,
                    esddsc as descricao,
                    t.tpddsc
                FROM workflow.estadodocumento d
                	INNER JOIN workflow.tipodocumento t ON t.tpdid = d.tpdid
                WHERE
                	d.tpdid in ($tpdid)
                    AND esdstatus = 'A'
                ORDER BY d.tpdid DESC, esdordem";
        $arrDados = $this->model->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        $arrRegistro = array();
        foreach ($arrDados as $v) {
            $arrRegistro[$v['tpddsc']][$v['codigo']] = $v['descricao'];
        }
        
        return $arrRegistro;
    }
    
    public function verificaTermoEmGeracaoReformulacao( $dotid, $proid, $esdid )
    {
        $sql = "SELECT
                	esdid as codigo,
                    esddsc as descricao,
                    t.tpddsc
                FROM workflow.estadodocumento d
                	INNER JOIN workflow.tipodocumento t ON t.tpdid = d.tpdid
                WHERE
                	d.tpdid in ($tpdid)
                    AND esdstatus = 'A'
                ORDER BY d.tpdid DESC, esdordem";
        $arrDados = $this->model->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        $arrRegistro = array();
        foreach ($arrDados as $v) {
            $arrRegistro[$v['tpddsc']][$v['codigo']] = $v['descricao'];
        }
        
        return $arrRegistro;
    }
    
    public function montarTelaTermoAditivo( $dotid_original, $refid, $inuid, $proid, $tirid = 2, $tipo = 'P')
    {
        global $simec;
        $obDocumento = new Par3_Controller_DocumentoTermo();
        
        $obProcesso = new Par3_Controller_Processo();
        $arrEntidade = $obProcesso->carregaDadosEntidadeProcesso($proid);
        
        if( $tirid == 1 ){
            $esdid = PAR3_REFORMULACAO_PRAZO_ESDID_EM_GERACAO_TERMO;
        } else {
            $esdid = PAR3_REFORMULACAO_ESDID_AGUARDANDO_GERACAO_TERMO;
        }
        
        $sql = "SELECT dt1.dotid, dt1.mdoid FROM par3.documentotermo dt1
					INNER JOIN par3.reformulacao_documento rd ON rd.refid = dt1.refid
					INNER JOIN workflow.documento d ON d.docid = rd.docid
				WHERE dt1.proid = $proid AND d.esdid = $esdid AND rd.refstatus = 'A' AND dt1.dotstatus in ('A', 'B')";
        $arTermo = $this->model->pegaLinha($sql);
        
        $dotid_gerado = $arTermo['dotid'];
        $mdoid = $arTermo['mdoid'];
        if( $dotid_gerado ){
            $obDocumento = new Par3_Controller_DocumentoTermo();
            $arrDocumentoTermo = $obDocumento->carregaDocumentoTermo($dotid_gerado);
            $mdoconteudo = $obDocumento->pegaTermoCompromissoArquivo($dotid_gerado, true);
        }
        $controleUnidade = new Par3_Controller_InstrumentoUnidade();
        ?>
        <div class="ibox">
        	<div class="row">
                <div class="col-md-12">
                    <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($arrEntidade['inuid']); ?></h5>
                </div>
            </div>
        	<div class="ibox-content">
                <div class="row">
                    <div class="col-md-12 orcamento">
                        <?php
                            $obProcesso = new Par3_Controller_Processo();
                            echo $obProcesso->carregaDadosOrcamentario($arrEntidade['inuid'], '', '', $proid);
                        ?>
                    </div>
                </div>
            </div>
            <div class="row">
                <div class="col-md-12">
                    <h2 class="center">Gerar Termos</h2>
                </div>
            </div>
            <div class="ibox-content">
                <div class="row">
                    <form name="form-gerar-termo" id="form-gerar-termo" class="form-horizontal" method="post">
                        <input type="hidden" name="requisicao" id="requisicao" value="">
                        <input type="hidden" name="proid" id="proid" value="<?php echo $proid; ?>">
                        <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid; ?>">
                        <input type="hidden" name="refid" id="refid" value="<?php echo $refid; ?>">
                        <input type="hidden" name="tipo_processo" id="tipo_processo" value="<?php echo $tipo; ?>">
                        <input type="hidden" name="dotid_original" id="dotid_original" value="<?php echo $dotid_original; ?>">
                        <input type="hidden" name="dotid" id="dotid" value="<?php echo $dotid_gerado; ?>">
                        <input type="hidden" name="mdoconteudo_t" id="mdoconteudo_t" value="<?=simec_htmlentities($mdoconteudo); ?>" />
                        <div id="div_mdoconteudo" style="display: none"><?=($mdoconteudo); ?></div>
                        <div class="form-group ">
                            <label for="" class="col-sm-2 col-md-2 col-lg-2 control-label">Tipo de Documento: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <select name="tpdcod" id="tpdcod" maxlength="255" readonly="readonly" class="form-control" data-placeholder="Selecione">
            						<option value="21" label="Termo Aditivo" selected="selected">Termo Aditivo</option>
        						</select>
        					</div>
                        </div>
                        <?php
                        $sql_modelo = "select
                            				mod.mdoid as codigo,
                            				mod.mdonome as descricao
                        				from par3.modelodocumento mod
                        		     	where mod.mdostatus = 'A' AND mod.mdovisivel <> 'N'
                        					and mod.mdoesfera = '{$arrEntidade['esfera']}'
                        					and mod.mdotipo = '{$tipo}'
                        		     		and mod.tpdcod = 21
                                            and mod.mdoex_oficio = 'N'
                            			order by mod.mdonome asc";

                        echo $simec->select('mdoid', 'Modelo de documento', $mdoid, $sql_modelo, array('maxlength' => '255', 'onchange' => 'carregarTermoAditivo(this.value)'), array('label-size' => '2', 'input-size' => '9')); ?>
                        
                        <div class="form-group" style="text-align: right !important;">
                            <label class="col-sm-2 col-md-2 col-lg-2 control-label  control-label">Iniciativas: </label>
                        </div>
                        <div class="form-group" style="text-align: right !important;">
                            <div class="col-sm-12 col-md-12 col-lg-12" style="text-align: center !important;">
                                <?php echo $obDocumento->listarIniciativasProcesso($proid, $arrEntidade['tipo_processo']);?>
                            </div>
                        </div>
                        <div class="form-group " style="text-align: right !important;">
                            <div class="col-sm-12 col-md-12 col-lg-12" style="text-align: center !important;">
                                <input type="button" id="btn-carregar-termo" class="btn btn-primary btn-carregar-termo" onclick="carregarTermoAditivo();" value="Carregar Termo">
                            </div>
                        </div>
                        <div style="clear:both; height: 10px;"></div>
                        <div class="form-group " style="text-align: right !important;">
                            <div class="col-sm-12 col-md-12 col-lg-12" style="text-align: center !important;">
                                <div id="div_conteudo">
                                    <textarea id="mdoconteudo" name="mdoconteudo" rows="30" cols="160" class="minutatinymce"></textarea>
                                </div>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
        <script>
        tinyMCE.init({
            // General options
            mode : "textareas",
            theme : "advanced",
            language: "pt",
            editor_selector : "minutatinymce",
            plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

            // Theme options
            theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
            theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
            theme_advanced_buttons3 : "tablecontrols, pagebreak", //",|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
            //theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
            theme_advanced_toolbar_location : "top",
            theme_advanced_toolbar_align : "left",
            theme_advanced_statusbar_location : "",
            theme_advanced_resizing : true,

            // Example content CSS (should be your site CSS)
            //content_css : "css/content.css",

            // Drop lists for link/image/media/template dialogs
            template_external_list_url : "lists/template_list.js",
            external_link_list_url : "lists/link_list.js",
            external_image_list_url : "lists/image_list.js",
            media_external_list_url : "lists/media_list.js",

            // Replace values for the template plugin
            template_replace_values : {
                username : "Some User",
                staffid : "991234"
            }
        });

        jQuery(document).ready(function(){
        	jQuery('#mdoconteudo_t').val( jQuery('#div_mdoconteudo').html() );
        	jQuery('#mdoconteudo').val(jQuery('#mdoconteudo_t').val());
        });
        </script>
        <?php
    }
    
    public function salvarCienciaNotificacao( $arrDados = array() )
    {
        
        //simec_redirecionar($_SERVER['REQUEST_URI'], 'success', 'Operação');
        $arDado = array();
        $arDado['arChavePrimaria'] = 'acuid';
        $arDado['stNomeTabela'] = 'par3.aviso_conteudo_usuario';
        $arDado['arAtributosTabela'] = array('acuid', 'actid', 'usucpf', 'acuciente', 'acuvisualizacao', 'acustatus', 'hstid', 'refid');
        
        $arDado['actid'] = $arrDados['actid'];
        $arDado['hstid'] = $arrDados['hstid'];
        $arDado['refid'] = $arrDados['refid'];
        $arDado['acuciente'] = true;
        $arDado['usucpf'] = $_SESSION['usucpf'];
        $arDado['acuciente'] = 't';
        
        $acuid = (int)trata_salvar($arDado);
        $this->model->commit();
        return $acuid;
    }
    
    public function verificaNotificacaoReformulacao( $inuid, $modal = false )
    {
        
        $arrPerfil = pegaPerfils($_SESSION['usucpf']);
        $arrPerfilValido = array( PAR3_PERFIL_PREFEITO, PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO );
        
        if( checaPerfilValido($arrPerfilValido, $arrPerfil) == 'S' && $inuid ){
            
            $sql = "SELECT rt.refid, dt.dotid, dt.dotnumero||'-'||dt.intoid AS termo, cd.cmddsc,
                        CASE WHEN ac.esdiddestino = ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_DILIGENCIA." THEN 'Diligência' ELSE 'Interposição de Recurso' END AS tipo,
                    	es.esddsc, u.usunome, to_char(h.htddata, 'DD/MM/YYYY HH24:MI:SS') as htddata, h.hstid, es.esdid, ac.esdiddestino
                    FROM workflow.documento d
                    	INNER JOIN workflow.historicodocumento h ON h.docid = d.docid AND h.hstid = d.hstid
                    	INNER JOIN workflow.acaoestadodoc ac ON ac.aedid = h.aedid
                    	INNER JOIN workflow.estadodocumento es ON es.esdid = ac.esdidorigem
                    	INNER JOIN workflow.estadodocumento es1 ON es1.esdid = d.esdid
                    	INNER JOIN seguranca.usuario u ON u.usucpf = h.usucpf
                        INNER JOIN par3.reformulacao_documento rt ON rt.docid = d.docid 
    	                INNER JOIN par3.documentotermo dt ON dt.dotid = rt.dotid AND rt.refidpai IS NOT NULL
                        LEFT JOIN workflow.comentariodocumento cd ON cd.hstid = h.hstid
                    WHERE d.docid IN (
                    			SELECT DISTINCT rd.docid FROM par3.processo p
                    				INNER JOIN par3.documentotermo dt ON dt.proid = p.proid AND dt.dotstatus = 'A'
                    				INNER JOIN par3.reformulacao_documento rd ON rd.dotid = dt.dotid AND rd.refstatus = 'A' AND rd.refidpai IS NOT NULL
                    			WHERE p.inuid = $inuid
                    	)
                        AND rt.refid NOT IN (SELECT refid FROM par3.aviso_conteudo_usuario WHERE usucpf = '{$_SESSION['usucpf']}')
                    	AND ac.esdiddestino IN (".PAR3_REFORMULACAO_PRAZO_ESDID_EM_DILIGENCIA.", 
                                                ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_DILIGENCIA.", 
                                                ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_REPROVADA.", 
                                                ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_FINALIZADA_REPROVADA.", 
                                                ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_VALIDACAO_GESTOR.")
                    ORDER BY h.hstid DESC";
        if( $modal == false ){
            $cabecalho = array('Tipo', 'Onde Estava', 'Quem fez', 'Data de Notificação');
            
            $listagem = new Simec_Listagem(Simec_Listagem::RETORNO_PADRAO);
            $listagem->setCabecalho($cabecalho);
            $listagem->esconderColunas('esdid', 'esdiddestino');
            $listagem->setQuery($sql);
            $listagem->turnOffForm();
            $listagem->render(); 
        } else {
            
            $arDados = $this->model->carregar($sql);
            
            if( $arDados ){
                $html = '';
                foreach ($arDados as $key => $v) {
                    if( $v['esdiddestino'] == PAR3_REFORMULACAO_PRAZO_ESDID_EM_DILIGENCIA ){
                        $sql = "SELECT ac.actid, actconteudo, au.acuciente, au.hstid, au.refid
                        FROM par3.aviso_conteudo ac
                        	LEFT JOIN par3.aviso_conteudo_usuario au ON au.actid = ac.actid
                        WHERE ac.actid = 6;";
                    } else {
                        $sql = "SELECT ac.actid, actconteudo, au.acuciente, au.hstid, au.refid
                        FROM par3.aviso_conteudo ac
                        	LEFT JOIN par3.aviso_conteudo_usuario au ON au.actid = ac.actid
                        WHERE ac.actid = 7;";
                    }
                    $arAvisoDiligencia = $this->model->pegaLinha($sql);
                    
                    $arAvisoDiligencia['actconteudo'] = str_ireplace("#num_termo#", '<b>'.$v['termo'].'</b>', $arAvisoDiligencia['actconteudo']);
                    $arAvisoDiligencia['actconteudo'] = str_ireplace("#comentario#", ($v['cmddsc']), $arAvisoDiligencia['actconteudo']);
                    $html .= '
                    <input type="hidden" name="refid[]" value="'.$v['refid'].'">
                    <div class="ibox float-e-margins animated modal conteudo" id="modal_conteudo_aviso_reforumulacao_'.$v['refid'].'" tabindex="-1" role="dialog" aria-hidden="true">
                    <div class="modal-dialog modal-lg" style="width: 60%">
                        <div class="modal-content">
                                <div class="ibox-content">
                            	<div class="row">
                                	 <div class="ibox-title">
                                        <h2 id="modal_titulo" align="center"><b>Notificação</b></h2>
                                     </div>
                                </div>
                                <div class="row">
                                    <div class="ibox-content">
                                    	<div id="termo-modal">
                                    	'.$arAvisoDiligencia['actconteudo'].'
                                    	</div>
                                    </div>
                                </div>
                            </div>
                            <div class="ibox-footer notprint" style="text-align: center;">
            					<button type="button" id="bnt-ciente-reformulacao" class="btn btn-primary" data-toggle="popover" data-html="true" data-trigger="hover" data-placement="top" 
            						data-content="Clique para não aparecer mais este aviso!" onclick="salvarCienciaReformulacao('.$v['refid'].', '.$arAvisoDiligencia['actid'].', '.$v['hstid'].')"> <i class="fa fa-check-square"></i> Ciente</button>
                            </div>
                        </div>
                    </div>
                </div>';
                }
                echo $html;
            
                if( $modal ){
                    ?>
                    <script type="text/javascript">
                    	jQuery(document).ready(function() {
                    		jQuery('[name="refid[]"]').each(function(){
                    			var refid = jQuery(this).val();
								
                    			jQuery('#modal_conteudo_aviso_reforumulacao_'+refid).show();
                    		});
                    		
                    		jQuery('.loading-dialog-par3').show();
                    		jQuery('.loading-dialog-par3').css('z-index', 10);
                    		jQuery('.modal-dialog').css('padding-top', '150px');
                    	});

                    	function salvarCienciaReformulacao(refid, actid, hstid){
                        	
                    		jQuery('.loading-dialog-par3').show();
                    		jQuery('.loading-dialog-par3').css('z-index', 10000);
                    		var caminho = window.location.href;
                            var action = '&requisicao=salvar-ciente-reformulaca&refid='+refid+'&hstid='+hstid+'&actid='+actid;
                            jQuery.ajax({
                                type: "POST",
                                url: caminho,
                                data: action,
                                async: true,
                                success: function (resp) {
                                    window.location.href = window.location;
                                }
                            });
                    	}
                   		</script>
                    <?php 
                    }
                }
            } 
        }
    }
    
    function gravaParecerTecnicoPrazo( $arrParr )
    {
        global $db;
        
        $parecerTecnico = $db->pegaUm("SELECT refconsideracoes FROM par3.reformulacao_documento WHERE refid = {$arrParr['refid']}");
        
        $sql = "SELECT hd.hstid, hd.docid FROM workflow.acaoestadodoc ae
            	INNER JOIN workflow.historicodocumento hd ON hd.aedid = ae.aedid
            WHERE ae.esdidorigem = ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE."
                AND ae.esdiddestino = ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_AGUARDANDO_VALIDACAO_COORDENADOR."
            ORDER BY hd.hstid DESC;";
        $arDados = $db->pegaLinha($sql);
        
        if( $arDados ){
            $sql = "INSERT INTO workflow.comentariodocumento(docid, cmddsc, cmdstatus, cmddata, hstid)
                VALUES({$arDados['docid']}, '{$parecerTecnico}', 'A', now(), {$arDados['hstid']});";
            $db->executar($sql);
            $db->commit();
        }
        
        return true;
    }
}











