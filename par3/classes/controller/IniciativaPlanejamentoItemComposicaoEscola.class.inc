<?php
/**
 * Classe de controle do  iniciativa_planejamento_item_composicao_escola
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RENNER NASCENTES TANIZAKI <rennertanizaki@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-07-2017
 * @link     no link
 */


/**
 * Par3_Controller_Iniciativa_planejamento_item_composicao_escola
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 17-07-2017
 * @link     no link
 */
class Par3_Controller_IniciativaPlanejamentoItemComposicaoEscola
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaPlanejamentoItemComposicaoEscola($_GET['ipeid']);
        $this->modelHistorico = new Par3_Model_IniciativaPlanejamentoItemComposicaoEscolaHistorico();
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($dados)
    {
        $modelIniciativaPlanejamentoItemComposicao = new Par3_Model_IniciativaPlanejamentoItemComposicao();
        $ipiid = $modelIniciativaPlanejamentoItemComposicao->getIdIniciativaPlanejamentoItemComposicao($dados);
        if(!$ipiid) {
            return false;
        }
        
        $arClausula = [
            "ipiid = " . $ipiid, 
            "ipestatus = 'A'", 
            "escid IS NOT NULL"
        ];
        $arDadosItemComposicaoEscola = $this->model->recuperarTodos('ipeid, escid', $arClausula);
        foreach ($arDadosItemComposicaoEscola as $dadosItemComposicaoEscola) {
            $arIpeid[$dadosItemComposicaoEscola['escid']] = $dadosItemComposicaoEscola['ipeid'];
        }
        
        $dados['qtd-item-escola'] = ($dados['qtd-item-escola'] ? $dados['qtd-item-escola'] : array());
        foreach ($dados['qtd-item-escola'] as $escid => $qtd) {
            $qtd = (trim($qtd) ? $qtd : 0);
            // update
            if ($arIpeid[$escid]) {
                $arDadosModel['ipeid'] = $arIpeid[$escid];
            // insert    
            } else {
                $arDadosModel['ipiid'] = $ipiid;
                $arDadosModel['escid'] = $escid;
            }
            $arDadosModel['ipequantidade']  = $qtd;
            $arDadosModel['co_entidade']    = 0;
            
            $this->model->popularDadosObjeto($arDadosModel)
                        ->salvar();
            $this->model->clearDados();
        }
        
        $arClausula = ['ipiid = ' . $ipiid];
        $total = $this->model->getDadosTotais($arClausula);
        $modelIniciativaPlanejamentoItemComposicao->popularDadosObjeto(['ipiid'=>$ipiid, 'ipiquantidade'=>$total[0]['ipiquantidade']])
                                                  ->salvar();
        $this->model->commit();
                                                  
        return $total[0]['ipiquantidade'];
    }
    
    public function salvarItemEscolaReformulacao($dados)
    {
        $total = 0;
        //ver($dados,d);
        $obReformulacao = new Par3_Controller_Reformulacao();
        $dados['escola'] = 'S';
        $dados['tirid'] = '2';
        $arRetorno = $obReformulacao->salvarSolicitacao($dados);
        $refid = $arRetorno['refid'];
        $reiid = $arRetorno['reiid'];
        
        if( is_array($dados['escid']) ){
            
            $arrItemEscola = array();
            $arrItemEscola['arChavePrimaria'] 	= 'rieid';
            $arrItemEscola['stNomeTabela'] 		= 'par3.reformulacao_item_composicao_escola';
            $arrItemEscola['arAtributosTabela'] = array('rieid', 'ipiid', 'iigid', 'reiid', 'refid', 'refidpai', 'ipeid', 'co_entidade', 'escid', 'riequantidade', 'riestatus');
            
            foreach ($dados['escid'] as $iigid => $arEscola) {
                
                foreach ($arEscola as $escid) {
                    $quantidade = $dados['qtd-item-escola-solicitada'][$iigid][$escid];
                    $rieid      = $dados['rieid'][$iigid][$escid];
                    
                    if( (int)$quantidade > (int)0 ){
                        $arrItemEscola['rieid']         = $rieid;
                        $arrItemEscola['reiid']         = $reiid;
                        $arrItemEscola['refid']         = $refid;
                        $arrItemEscola['iigid']         = $iigid;
                        $arrItemEscola['refidpai']      = $dados['refidpai'];
                        if( $dados['ipiid_escola'] ) $arrItemEscola['ipiid'] = $dados['ipiid'];
                        $arrItemEscola['co_entidade']   = 0;
                        $arrItemEscola['escid']         = $escid;
                        $arrItemEscola['riequantidade'] = $quantidade;
                        $arrItemEscola['riestatus']     = 'A';
                            
                        trata_salvar($arrItemEscola);
                    }
                }
            }
        }
        
        $this->model->commit();
        
        $sql = "SELECT SUM(riequantidade) as quantidade
                  FROM par3.reformulacao_item_composicao_escola
                  WHERE riestatus = 'A' and escid is not null /*AND ipiid = {$dados['ipiid_escola']} AND*/ and refid = $refid AND reiid = $reiid and refidpai is not null";
        
        $total = $this->model->pegaUm($sql);
        return json_encode(array('refid' => $refid, 'reiid' => $reiid, 'total' => $total));
    }
    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['ipeid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&ipeid={$id}";
        $iniciativa_planejamento_item_composicao_escola = new Par3_Model_Iniciativa_planejamento_item_composicao_escola($id);
        try {
            $iniciativa_planejamento_item_composicao_escola->Xstatus = 'I';
            $iniciativa_planejamento_item_composicao_escola->Xdtinativacao = date('Y-m-d H:i:s');
            $iniciativa_planejamento_item_composicao_escola->Xusucpfinativacao = $_SESSION['usucpf'];

            $iniciativa_planejamento_item_composicao_escola->salvar();
            $iniciativa_planejamento_item_composicao_escola->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    /**
     * @return Par3_Model_Iniciativa_planejamento_item_composicao_escola
     */
    public function getFormEscolas($dados)
    {
        
        //recupera estado do planejamento e iniciativa, para desabilitar os campos quantidade caso o planejamento ou a iniciatva estejam finalizados.
        $mInp = new Par3_Controller_IniciativaPlanejamento();
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $modelDocumento = new Workflow_Model_Documento($modelInstrumentoUnidade->docid);
        $inp = $mInp->recuperar();
        $mdDocPl = new Workflow_Model_Documento($inp->docid);
        
        $sql = "SELECT count(eic.ieiid)
                FROM par3.iniciativa_emenda_item_composicao eic
                	INNER JOIN par3.iniciativa_emenda ie ON ie.ineid = eic.ineid
                WHERE eic.ieistatus = 'A' AND ie.inestatus = 'A' AND ie.inpid = {$dados['inpid']}";
        $boEmenda = $this->model->pegaUm($sql);

        global $simec;
        $config['maxlength'] = 10;
        $config['class']     = 'inteiro qtd-escola-item-composicao';
        $config['type']      = 'number';
        $config['onblur']    = 'this.value=mascaraglobal("[#]",this.value);';
        $config['onkeyup']   = 'this.value=mascaraglobal("[#]",this.value);';
        $config['style']     = 'text-align:center;';

        $sql = "select d.esdid, a.anaano from par3.analise a
				    inner join workflow.documento d on d.docid = a.docid
                where a.inpid = {$dados['inpid']} and a.anaano = {$dados['iniano']} and intaid = 1 AND anastatus = 'A'";
        $dadosAnalise = $this->model->pegaLinha($sql);

        if( $dados['anaid'] ){
        	$sql = "select
		        		ie.ineid
		        	from
		        		par3.analise a
		        	join emenda.emendadetalheentidade ede on a.edeid = ede.edeid and
		        											ede.edestatus = 'A'
		        	join par3.iniciativa_emenda ie on ie.edeid = ede.edeid and
										        		ie.inpid = {$dados['inpid']} and
										        		ie.inestatus = 'A'
		        	where
		        		a.anastatus = 'A' and
		        		a.anaid = {$dados['anaid']}";
        	$ineid = $this->model->pegaUm($sql);
        	$dados['ineid'] = $ineid;
        }
       
        $arConsultaEscola = $this->model->getDadosEscolasPAR3($dados);
        if(empty($arConsultaEscola)){
            return '';
        }
        $tabela = '<input type="hidden" name="inuid" value="' . $dados['inuid'] . '"/>';
        $tabela .= '<input type="hidden" name="inpid" value="' . $dados['inpid'] . '"/>';
        $tabela .= '<input type="hidden" name="iniano" value="' . $dados['iniano'] . '"/>';
        $tabela .= '<input type="hidden" name="iigid" value="' . $dados['iigid'] . '"/>';
        $tabela .= '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="areas" data-qtd-acoes="0">';
        $tabela .= '<thead>';
        $tabela .= '<tr>';
        $tabela .= '<th>Município</th>';
        $tabela .= '<th>Escola</th>';
        $tabela .= '<th>Código INEP</th>';
        $tabela .= '<th>Localização</th>';
        $tabela .= '<th>Esfera</th>';

        if($dados['aba'] == 'analisarPlanejamento' ||
           ($dados['aba'] == 'cadastro' && ($dadosAnalise['esdid'] != 2074 || $dadosAnalise['esdid'] == null ) &&
           ($mdDocPl->esdid == PAR3_ESDID_EM_DILIGENCIA || $mdDocPl->esdid != PAR3_ESDID_CADASTRAMENTO))
        ) {
	        $tabela .= '<th>Quantidade</th>';
	        $config['disabled']     = 'disabled';
        } else {
        	$tabela .= '<th>Quantidade<br> ' . $simec->input('qtd-item-escola-total', NULL, '', array('maxlength'=>'10', 'type'=>'number','style'=>'text-align:center;','onblur'=>'this.value=mascaraglobal("[#]",this.value);', 'onkeyup'=>'this.value=mascaraglobal("[#]",this.value);', 'onchange'=>'atualizaTodasEscolas()'), array('input-size' => '8')) . '</th>';
        }
        if( (int)$boEmenda > (int)0 && $dados['ineid'] ) $tabela .= '<th>Quantidade(Emendas)</th>';
        
        $tabela .= '<th>Quantidade de Salas</th>';
        $tabela .= '<th>Quantidade de Alunos</th>';
        $tabela .= '</tr>';
        $tabela .= '</thead>';
        $tabela .= '<tbody>';
        foreach ($arConsultaEscola as $escola) {
            $tabela .= "<tr>";
            $tabela .= "<td>{$escola['mundescricao']}</td>";
            $tabela .= "<td>{$escola['escnome']}</td>";
            $tabela .= "<td>{$escola['esccodinep']}</td>";
            $tabela .= "<td>{$escola['escno_localizacao']}</td>";
            $tabela .= "<td>{$escola['escno_esfera']}</td>";
            $tabela .= "<td>" . $simec->input('qtd-item-escola[' . $escola['escid'] . ']', NULL, $escola['ipequantidade'], $config, array('input-size' => '8')) . "</td>";
            if( (int)$boEmenda > (int)0 && $dados['ineid'] ) $tabela .= "<td>" . $simec->input('qtd-item-escola-emenda[' . $escola['escid'] . ']', NULL, $escola['qtd_emenda'], $config, array('input-size' => '8')) . "</td>";
            $tabela .= "<td>{$escola['nu_salas_existentes']}</td>";
            $tabela .= "<td>{$escola['qtd_matricula']}</td>";
            $tabela .= "</tr>";
        }
        $tabela .= "</tbody>";
        $tabela .= "<tfoot>";
        $tabela .= "<tr>";
        $tabela .= "<td style=\"text-align:right\" colspan=\"10\"><strong>Total de Registros: ".count($arConsultaEscola)."</strong></td>";
        $tabela .= "</tr>";
        $tabela .= "</tfoot>";
        $tabela .= "</table>";
        return $tabela;
    }
    
    public function getFormEscolasReformulacao($dados)
    {
        //recupera estado do planejamento e iniciativa, para desabilitar os campos quantidade caso o planejamento ou a iniciatva estejam finalizados.
        $mInp = new Par3_Controller_IniciativaPlanejamento();
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $modelDocumento = new Workflow_Model_Documento($modelInstrumentoUnidade->docid);
        $inp = $mInp->recuperar();
        $mdDocPl = new Workflow_Model_Documento($inp->docid);
        
        global $simec;
        $config['maxlength'] = 4;
        $config['class']     = 'inteiro qtd-escola-item-composicao';
        $config['type']      = 'number';
        $config['onblur']    = 'this.value=mascaraglobal("[#]",this.value);';
        $config['onkeyup']   = 'this.value=mascaraglobal("[#]",this.value);';
        $config['style']     = 'text-align:center;';
                
        $arConsultaEscola = $this->model->getDadosEscolasPAR3($dados);
        if(empty($arConsultaEscola)){
            return '<div style="margin-top:20px;" class="alert alert-info col-md-4 col-md-offset-4 text-center nenhum-registro" id="tb_render">Não há registro de alunos para esta etapa de ensino.</div>';
        }
        
        if( $dados['editavel'] == 'N' ){
            $config['disabled'] = 'disabled';
        }
        
        $tabela = '<input type="hidden" name="inuid" value="' . $dados['inuid'] . '"/>';
        $tabela .= '<input type="hidden" name="inpid" value="' . $dados['inpid'] . '"/>';
        $tabela .= '<input type="hidden" name="iniano" value="' . $dados['iniano'] . '"/>';
        $tabela .= '<input type="hidden" name="iigid" value="' . $dados['iigid'] . '"/>';
        $tabela .= '<input type="hidden" name="ipiid" value="' . $dados['ipiid'] . '"/>';
        $tabela .= '<input type="hidden" name="reiid" value="' . $dados['reiid'] . '"/>';
        $tabela .= '<input type="hidden" name="refid" value="' . $dados['refid'] . '"/>';
        $tabela .= '<input type="hidden" name="refidpai" value="' . $dados['refidpai'] . '"/>';
        $tabela .= '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="areas" data-qtd-acoes="0">';
        $tabela .= '<thead>';
        $tabela .= '<tr>';
        $tabela .= '<th>Município</th>';
        $tabela .= '<th>Escola</th>';
        $tabela .= '<th>Código INEP</th>';
        $tabela .= '<th>Localização</th>';
        $tabela .= '<th>Esfera</th>';
        $tabela .= '<th>Quantidade</th>';
        if( $dados['editavel'] == 'N' ){
            $tabela .= '<th>Quantidade Solicitada</th>';
        } else {
            $tabela .= '<th>Quantidade Solicitada<br> '.$simec->input('qtd-item-escola-total', NULL, '', array('maxlength' => '4', 'type' => 'number','style'=>'text-align:center;','onblur' => 'this.value=mascaraglobal("[#]",this.value);', 'onkeyup' => 'this.value=mascaraglobal("[#]",this.value);', 'onchange' => 'atualizaTodasEscolas()'), array('input-size' => '8')).'</th>';
        }
        $tabela .= '<th>Quantidade de Salas</th>';
        $tabela .= '<th>Quantidade de Alunos</th>';
        $tabela .= '</tr>';
        $tabela .= '</thead>';
        $tabela .= '<tbody>';
        
        $filtroIpiid = "";
        if( $dados['ipiid'] ) $filtroIpiid .= " AND ipiid = {$dados['ipiid']} ";
        if( $dados['reiid'] ) $filtroReiid .= " AND reiid = {$dados['reiid']} ";
        
        foreach ($arConsultaEscola as $escola) {
            
            
            //if( $dados['dados_pai'] == 'S' ){
                $sql = "SELECT rieid, riequantidade FROM par3.reformulacao_item_composicao_escola
                        WHERE iigid = {$dados['iigid']} $filtroIpiid
                             and refid = {$dados['refidpai']} AND escid = {$escola['escid']} AND riestatus = 'A'";
                $arEscola_bk = $this->model->pegaLinha($sql);
                
                $rieid          = $arEscola_bk['rieid'];
                $qtd_solicitada_bk = $arEscola_bk['riequantidade'];
            //} else {            
                $sql = "SELECT rieid, riequantidade FROM par3.reformulacao_item_composicao_escola
                        WHERE refid = {$dados['refid']} AND iigid = {$dados['iigid']} $filtroIpiid
                            $filtroReiid AND escid = {$escola['escid']} AND riestatus = 'A'";
                $arEscola = $this->model->pegaLinha($sql);
                
                $rieid          = $arEscola['rieid'];
                $qtd_solicitada = $arEscola['riequantidade'];
           // }
            
            //if( $dados['mostra_valor'] == 'N' ) $qtd_solicitada = 0;
            
            $tabela .= "<tr>";
            $tabela .= "<td><input type='hidden' name='escid[".$dados['iigid']."][]' value='".$escola['escid']."'/>
                            <input type='hidden' name='rieid[".$dados['iigid']."][".$escola['escid']."]' value='".$rieid."'/>
                    {$escola['mundescricao']}</td>";
            $tabela .= "<td>{$escola['escnome']}</td>";
            $tabela .= "<td>{$escola['esccodinep']}</td>";
            $tabela .= "<td>{$escola['escno_localizacao']}</td>";
            $tabela .= "<td>{$escola['escno_esfera']}</td>";
            $tabela .= "<td>".$simec->input('qtd-item-escola[' . $escola['escid'] . ']', NULL, $qtd_solicitada_bk, array('disabled' => 'disabled'), array('input-size' => '8')) . "</td>";
            $tabela .= "<td>".$simec->input('qtd-item-escola-solicitada['.$dados['iigid'].'][' . $escola['escid'] . ']', NULL, $qtd_solicitada, $config, array('input-size' => '8')) . "</td>";
            $tabela .= "<td>{$escola['nu_salas_existentes']}</td>";
            $tabela .= "<td>{$escola['qtd_matricula']}</td>";
            $tabela .= "</tr>";
        }
        $tabela .= "</tbody>";
        $tabela .= "<tfoot>";
        $tabela .= "<tr>";
        $tabela .= "<td style=\"text-align:right\" colspan=\"10\"><strong>Total de Registros: ".count($arConsultaEscola)."</strong></td>";
        $tabela .= "</tr>";
        $tabela .= "</tfoot>";
        $tabela .= "</table>";
        return $tabela;
    }
    
    
    /**
     * @return Par3_Model_Iniciativa_planejamento_item_composicao_escola
     */
    public function getFormItemComposicaoEscolasComVinculoParlamentar($dados)
    {
        //recupera estado do planejamento e iniciativa, para desabilitar os campos quantidade caso o planejamento ou a iniciatva estejam finalizados.
        $mInp = new Par3_Controller_IniciativaPlanejamento();
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $modelDocumento = new Workflow_Model_Documento($modelInstrumentoUnidade->docid);
        $inp = $mInp->recuperar();
        $mdDocPl = new Workflow_Model_Documento($inp->docid);

        global $simec;
        $config['maxlength'] = 4;
        $config['class']     = 'inteiro qtd-escola-item-composicao';
        $config['type']      = 'number';
        $config['onblur']    = 'this.value=mascaraglobal("[#]",this.value);';
        $config['onkeyup']   = 'this.value=mascaraglobal("[#]",this.value);';
        $config['style']     = 'text-align:right;';
        $config['disabled']  = 'disabled';
        
        $configItemIndicado = $config;
        
        $sql = "SELECT
		        	d.docid, ed.esdid, ed.esddsc
		        FROM
		        	par3.iniciativa_emenda e
		        INNER JOIN workflow.documento d ON d.docid = e.docid
		        INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
		        WHERE
		        	e.inestatus = 'A' and
		        	e.ineid = {$dados['ineid']}";
        $workflowIniciativaEmenda = $this->model->pegaLinha($sql);
        
        $boLibera = false;
        if ($workflowIniciativaEmenda['esdid'] == PAR3_ESD_EMENDA_ANALISE_ENTIDADE &&
            possuiPerfil(
                array(
                    PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR,
            		PAR3_PERFIL_SUPER_USUARIO,
            		PAR3_PERFIL_PREFEITO,
            		PAR3_PERFIL_SECRETARIO_ESTADUAL
                )
            )) {
        		$boLibera = true;
        }
        
        if ( $boLibera ){
	        unset($configItemIndicado['disabled']);
        }
        
        $sql = "select d.esdid, a.anaano from par3.analise a
							inner join workflow.documento d on d.docid = a.docid
						where a.inpid = {$dados['inpid']} and a.anaano = {$dados['iniano']}";
        $dadosAnalise = $this->model->pegaLinha($sql);
		
        $arConsultaEscola = $this->model->getDadosEscolasPAR3ComVinculoParlamentar($dados);
        if(empty($arConsultaEscola)){
            return '';
        }
//         $tabela = '<input type="hidden" name="inuid" value="' . $dados['inuid'] . '"/>';
        $tabela .= '<input type="hidden" name="inpid" value="' . $dados['inpid'] . '"/>';
//         $tabela .= '<input type="hidden" name="iniano" value="' . $dados['iniano'] . '"/>';
//         $tabela .= '<input type="hidden" name="iigid" value="' . $dados['iigid'] . '"/>';
        $tabela .= '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="areas" data-qtd-acoes="0">';
        $tabela .= '<thead>';
        $tabela .= '<tr>';
        $tabela .= '<th>Município</th>';
        $tabela .= '<th>Escola</th>';
        $tabela .= '<th>Código INEP</th>';
        $tabela .= '<th>Localização</th>';
        $tabela .= '<th>Esfera</th>';

//         if($dados['aba'] == 'analisarPlanejamento' || ($dados['aba'] == 'cadastro' && ($dadosAnalise['esdid'] != 2074 || $dadosAnalise['esdid'] == null ) && ($mdDocPl->esdid == PAR3_ESDID_EM_DILIGENCIA || $mdDocPl->esdid != PAR3_ESDID_CADASTRAMENTO))){
	        $tabela .= '<th>Quantidade</th>';
	        $tabela .= '<th>Quantidade Indicada</th>';
// 	        $config['disabled']     = 'disabled';
//         } else {
//         	$tabela .= '<th>Quantidade<br> ' . $simec->input('qtd-item-escola-total', NULL, '', array('maxlength' => '4', 'type' => 'number','style'=>'text-align:center;','onblur' => 'this.value=mascaraglobal("[#]",this.value);', 'onkeyup' => 'this.value=mascaraglobal("[#]",this.value);', 'onchange' => 'atualizaTodasEscolas()'), array('input-size' => '14')) . '</th>';
//         }
        $tabela .= '<th>Quantidade de Salas</th>';
        $tabela .= '<th>Quantidade de Alunos</th>';
        $tabela .= '</tr>';
        $tabela .= '</thead>';
        $tabela .= '<tbody>';
        foreach ($arConsultaEscola as $escola) {
        	if (($dados['ineid'] /*&& $escola['ipeid']*/ && $dados['inuid'] && $dados['inuid']) == false){
        		$configItemIndicado['disabled']  = 'disabled';
        	}
        	
            $tabela .= "<tr>";
            $tabela .= "<td>{$escola['mundescricao']}</td>";
            $tabela .= "<td>{$escola['escnome']}</td>";
            $tabela .= "<td>{$escola['esccodinep']}</td>";
            $tabela .= "<td>{$escola['escno_localizacao']}</td>";
            $tabela .= "<td>{$escola['escno_esfera']}</td>";
            $tabela .= "<td>" . $simec->input('qtd-item-escola[' . $escola['escid'] . ']', NULL, $escola['ipequantidade'], $config, array('input-size' => '12')) . "</td>";
            $tabela .= "<td>" . $simec->input('qtd-item-escola-indicada_'.$dados['ineid'].'_'.$escola['ipeid'].'_'.$dados['ipiid'].'_'.$dados['inuid'].'_'.$escola['escid'], NULL, $escola['ieequantidade'], $configItemIndicado, array('input-size' => '12')) . "</td>";
            $tabela .= "<td style=\"text-align:right\">{$escola['nu_salas_existentes']}</td>";
            $tabela .= "<td style=\"text-align:right\">{$escola['qtd_matricula']}</td>";
            $tabela .= "</tr>";
        }
        $tabela .= "</tbody>";
        $tabela .= "<tfoot>";
        $tabela .= "<tr>";
        $tabela .= "<td style=\"text-align:right\" colspan=\"9\"><strong>Total de Registros: ".count($arConsultaEscola)."</strong></td>";
        $tabela .= "</tr>";
        $tabela .= "</tfoot>";
        $tabela .= "</table>";
        
        $tabela .= <<<HTML
        			<script type="text/javascript">
					    jQuery('[name*=qtd-item-escola-indicada_]').change(function ()
                        {
                            var obCalc = new Calculo();
                    		if (obCalc.comparar(jQuery(this).val(), 0, '<')) {
                    			jQuery(this).val(0);
                    		}

					    	var name = jQuery(this).attr('name');
					    	name = name.split('_');
					    	var ineid = name[1];
					    	var ipeid = name[2];
					    	var ipiid = name[3];
					    	var inuid = name[4];
					    	var escid = name[5];
        		
					    	var caminho = window.location.href;
					    	var dado	= new Object();
					    	dado.requisicao 	= 'SalvarDetalheItensComposicaoEscola';
					    	dado.ieequantidade 	= jQuery(this).val();
					    	dado.ineid 			= ineid;
					    	dado.ipeid 			= ipeid;
					    	dado.ipiid 			= ipiid;
					    	dado.escid 			= escid;
							dado.inpid 			= jQuery('[name=inpid]').val();
        		
        					var ieiquantidade = 0;
        					var obCalc = new Calculo();
        					jQuery('[name*=qtd-item-escola-indicada_]').each(function(){
    							ieiquantidade = obCalc.operacao(ieiquantidade, jQuery(this).val(), '+');
    						});
        					dado.ieiquantidade = ieiquantidade;

					    	$.ajax({
						        type: 	"POST",
						        url: 	caminho,
						        data: 	dado,
						        async: 	false,
						        success: function (resp) {
						        	if (resp != 'okok'){
							        	swal({title: "Erro ao salvar o item de composição por escola da Iniciativa", text: "", type: "error"},
										   function(){
												window.opener.location.href = window.opener.location.href;
										   }
										);
									}
						    	},
						    	error: function(){
							        	swal({title: "Erro ao salvar o item de composição por escola da Iniciativa", text: "", type: "error"},
										   function(){
												window.opener.location.href = window.opener.location.href;
										   }
										);
    							}
						    });
					    	
						});
        		</script>
HTML;
        
        return $tabela;
    }

    public function atualizarValorItemComposicao($arrPost){
        $arrPost = array_filter($arrPost);
        $valorTotal = $arrPost['valorTotal'];
        if($arrPost['valorTotal'] == '' || empty($arrPost['valorTotal'])) {
            $valorTotal = 0.00;
        }
        global $db;

        $inpid = $arrPost['inpid'];
        $sql = "UPDATE par3.iniciativa_planejamento set inpvalor_planejado_total = {$valorTotal} where inpid = {$inpid}";
        $db->executar($sql);
        $db->commit();

    }
}