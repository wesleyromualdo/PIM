<?php
/**
 * Classe de controle do  IniciativaPlanejamentoItemComposicao
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   RENNER NASCENTES TANIZAKI <rennertanizaki@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */


/**
 * Par3_Controller_IniciativaPlanejamentoItemComposicao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */
class Par3_Controller_IniciativaPlanejamentoItemComposicao
{
    private $model;

    public function __construct()
    {
        $this->model = new Par3_Model_IniciativaPlanejamentoItemComposicao($_GET['ipiid']);
        $this->modelHistorico = new Par3_Model_IniciativaPlanejamentoItemComposicaoHistorico();
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($dados)
    {

        $inpid = $dados['inpid'];
        $arConsulta[1] = 'inpid = ' . $inpid;

//       if ($dados['condicao-por-escola']) {
//            return false;
//        }

        if (is_array($dados['qtd-item'])) {
            foreach ($dados['qtd-item'] as $iigid => $arValor) {
                $arConsulta[2] = 'iigid = ' . $iigid;
                $arConsulta[3] = "ipistatus = 'A'";
                foreach ($arValor as $ano => $qtd) {
                    $arConsulta[4] = 'ipiano = ' . $ano;
                    $arIpic = $this->model->recuperarTodos('ipiid', $arConsulta);

                    if (isset($arIpic[0]['ipiid']) && $arIpic[0]['ipiid']) {
                        $arUpdate = array();
                        $arUpdate['ipiid'] = $arIpic[0]['ipiid'];
                        if (isset($qtd) && $qtd != '') {
                            $arUpdate['ipiquantidade'] = $qtd;
                            $arUpdate['ipistatus'] = 'A';
                        } else {
                            $arUpdate['ipistatus'] = 'I';
                        }

                        $this->model->popularDadosObjeto($arUpdate);
                        $this->model->salvar();
                        $this->model->commit();
                        //$this->model->atualizaValorPlanejadoPlanejamentoIniciativa($inpid);
                        $modelSalvo = new Par3_Model_IniciativaPlanejamentoItemComposicao($arIpic[0]['ipiid']);
                        $this->modelHistorico->gravarHistorico($modelSalvo, Par3_Model_IniciativaIniciativasAreasHistorico::UPDATE);
                    } else {
                        //ck_ipiquantidade_positivo (ipiquantidade >= 0)
                        if (isset($qtd) && $qtd != '' && $qtd >= 0) {
                            $arInsert = array();
                            $arInsert['inpid'] = $inpid;
                            $arInsert['ipivalorreferencia'] = $dados['referencia'][$iigid];
                            $arInsert['ipiquantidade'] = (int)$qtd;
                            $arInsert['ipiano'] = $ano;
                            $arInsert['iigid'] = $iigid;
                            $arInsert['ipistatus'] = 'A';

                            $this->model->popularDadosObjeto($arInsert);
                            $this->model->ipiid = null;
                            $id = $this->model->salvar();

                            $this->model->commit();
                           // $this->model->atualizaValorPlanejadoPlanejamentoIniciativa($inpid);
                            $modelSalvo = new Par3_Model_IniciativaPlanejamentoItemComposicao($id);

                            $this->modelHistorico->gravarHistorico($modelSalvo, Par3_Model_IniciativaIniciativasAreasHistorico::CREATE);
                        }
                    }
                }
            }
        }
    }





    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['ipiid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&ipiid={$id}";
        $IniciativaPlanejamentoItemComposicao = new Par3_Model_IniciativaPlanejamentoItemComposicao($id);
        try {
            $IniciativaPlanejamentoItemComposicao->Xstatus = 'I';
            $IniciativaPlanejamentoItemComposicao->Xdtinativacao = date('Y-m-d H:i:s');
            $IniciativaPlanejamentoItemComposicao->Xusucpfinativacao = $_SESSION['usucpf'];

            $IniciativaPlanejamentoItemComposicao->salvar();
            $IniciativaPlanejamentoItemComposicao->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    public function getPlanilha($inpid, $inuid, $condicaoPorEscola)
    {

        $anosPlanejamento = $this->model->anosPlanejamento($inpid);

        $mdUnidade      = new Par3_Model_InstrumentoUnidade($inuid);
        $mdWFUnidade    = new Workflow_Model_Documento($mdUnidade->docid);

        $mdPlanejamento = new Par3_Model_IniciativaPlanejamento($inpid);
        $mdWFPlan       = new Workflow_Model_Documento($mdPlanejamento->docid);

        $habilitaExclusao = false;
        if ($mdWFUnidade->esdid == PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO &&
            $mdWFPlan->esdid == PAR3_ESDID_CADASTRAMENTO
        ) {
            $habilitaExclusao = true;
        }

        ($habilitaExclusao) ? ($colspanAdicional = 5) : ($colspanAdicional = 4);
        $colspan = count($anosPlanejamento);
        $linhaAno = '';
        if (is_array($anosPlanejamento)) {
            foreach ($anosPlanejamento as $ano) {
                $linhaAno .= "<th> {$ano['iniano']}</th>";
            }
        }

        $itensCadastrados = $this->model->getItensCadastrados($inpid);

        $linha = '';
        if (is_array($itensCadastrados)) {
            $arConsulta['inpid'] = $inpid;
            $arConsulta['inuid'] = $inuid;
            $arConsulta['habilitaExclusao'] = $habilitaExclusao;
            $arConsulta['condicao-por-escola'] = $condicaoPorEscola;
            $arConsulta['obrid'] = $mdPlanejamento->obrid;

            foreach ($itensCadastrados as $item) {
                $arConsulta['iigid'] = $item['iigid'];
                $linha .= $this->buscaLinha($arConsulta);
            }
            if($mdPlanejamento->obrid > 0){
                $tabela .= '<input type="hidden" id="iigidpai" name="iigidpai" value="'.$dados['iigidpai'].'">';
            }
        }



        $tabela .= '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="dados-itens-composicao">';
        $tabela .= '<thead>';
        $tabela .= '<tr>';
        ($habilitaExclusao) ? $tabela .= '<th> </th>' : null;
        $tabela .= '<th> </th>';
        $tabela .= '<th> </th>';
        $tabela .= '<th> </th>';
        $tabela .= '<th colspan="' . $colspan . '" style="text-align: center"> Quantidade</th>';
        $tabela .= '<th colspan="' . $colspan . '" style="text-align: center"> Valor</th>';
        $tabela .= '<th colspan="2" style="text-align: center"> Total</th>';
        $tabela .= '</tr>';
        $tabela .= '<tr>';
        ($habilitaExclusao) ? $tabela .= '<th> Ação</th>' : null;
        $tabela .= '<th> Item</th>';
        $tabela .= '<th> Unidade de Medida</th>';
        $tabela .= '<th> Valor Referência</th>';
        $tabela .= $linhaAno;
        $tabela .= $linhaAno;
        $tabela .= '<th> Itens</th>';
        $tabela .= '<th> Valor</th>';
        $tabela .= '</tr>';
        $tabela .= '</thead>';
        $tabela .= '<tbody id="dados-itens-composicao-tbody">';
        $tabela .= $linha;
        $tabela .= '</tbody>';
        $tabela .= '<tfoot>';
        $tabela .= '<tr>';
        $tabela .= '<td style="text-align: right" colspan="' . (($colspan * 2) + $colspanAdicional) . '" > <b>Total</b></td>';
        $tabela .= '<td id="dados-itens-composicao-total"> - </td>';
        $tabela .= '</tr>';
        $tabela .= '</tfoot>';
        $tabela .= '</table>';
        return $tabela;
    }
    
    public function getPlanilhaImpressao($inpid, $inuid, $condicaoPorEscola)
    {

        $anosPlanejamento = $this->model->anosPlanejamento($inpid);

        $mdUnidade      = new Par3_Model_InstrumentoUnidade($inuid);
        $mdWFUnidade    = new Workflow_Model_Documento($mdUnidade->docid);

        $mdPlanejamento = new Par3_Model_IniciativaPlanejamento($inpid);
        $mdWFPlan       = new Workflow_Model_Documento($mdPlanejamento->docid);

        $colspanAdicional = 4;
        $colspan = count($anosPlanejamento);
        $linhaAno = '';
        if (is_array($anosPlanejamento)) {
            foreach ($anosPlanejamento as $ano) {
                $linhaAno .= "<th> {$ano['iniano']}</th>";
            }
        }

        $itensCadastrados = $this->model->getItensCadastrados($inpid);

        $linha = '';
        if (is_array($itensCadastrados)) {
            $arConsulta['inpid'] = $inpid;
            $arConsulta['inuid'] = $inuid;
            $arConsulta['habilitaExclusao'] = $habilitaExclusao;
            $arConsulta['condicao-por-escola'] = $condicaoPorEscola;

            foreach ($itensCadastrados as $item) {
                $arConsulta['iigid'] = $item['iigid'];
                $dadoLinha = $this->buscaLinhaImpressao($arConsulta);
                $linha .= $dadoLinha['tabela'];
                $valorTotal = $valorTotal + $dadoLinha['valor'];
            }
        }

        $tabela  = '<table class="table" id="dados-itens-composicao">';
        $tabela .= '<thead>';
        $tabela .= '<tr>';
        $tabela .= '<th> </th>';
        $tabela .= '<th> </th>';
        $tabela .= '<th> </th>';
        $tabela .= '<th colspan="' . $colspan . '" style="text-align: center"> Quantidade</th>';
        $tabela .= '<th colspan="' . $colspan . '" style="text-align: center"> Valor</th>';
        $tabela .= '<th colspan="2" style="text-align: center"> Total</th>';
        $tabela .= '</tr>';
        $tabela .= '<tr>';
        $tabela .= '<th> Item</th>';
        $tabela .= '<th> Unidade de Medida</th>';
        $tabela .= '<th> Valor Referência</th>';
        $tabela .= $linhaAno;
        $tabela .= $linhaAno;
        $tabela .= '<th> Itens</th>';
        $tabela .= '<th> Valor</th>';
        $tabela .= '</tr>';
        $tabela .= '</thead>';
        $tabela .= '<tbody id="dados-itens-composicao-tbody">';
        $tabela .= $linha;
    //    $tabela .= '</tbody>';
    //    $tabela .= '<tfoot>';
        $tabela .= '<tr>';
        $tabela .= '<td style="text-align: right" colspan="' . (($colspan * 2) + $colspanAdicional) . '" > <b>Total</b></td>';
        $tabela .= '<td id="dados-itens-composicao-total">'. ($valorTotal>0 ? 'R$ '.formata_valor($valorTotal) : '-' ) .'</td>';
        $tabela .= '</tr>';
    //    $tabela .= '</tfoot>';
        $tabela .= '</tbody>';    // ADICIONEI
        $tabela .= '</table>';
        return $tabela;
    }

    /**
     * Iniciativas de obras do proinfância listarão todos os itens dentro do kit
     */
    public function buscaLinhasProinfancia($dados){
        $itens = $this->model->recuperarItensKitProinfancia($dados['iigid']);

        $dados['iigidpai'] = $dados['iigid'];
        $html = '';
        if(!empty($itens)) {
            foreach ($itens as $item) {
                $dados['iigid'] = $item['iigid'];
                $dados['itcid'] = $item['itcid'];
                $html .= $this->buscaLinha($dados);
            }
        }else{
            $html .= '<script>swal("", "O kit selecionado não possui itens vinculados! A vinculação é feita nas tabelas de apoio.", "warning");</script>;';
        }

        $htmlGrupoPai = '<input type="hidden" id="iigidpai" name="iigidpai" value="'.$dados['iigidpai'].'">';
        $html .= $htmlGrupoPai;

        return $html;
    }


    public function buscaLinha($dados)
    {

        //recupera estado do planejamento e iniciativa, para desabilitar os campos quantidade caso o planejamento ou a iniciatva estejam finalizados.
        $mInp = new Par3_Controller_IniciativaPlanejamento();
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $modelDocumento = new Workflow_Model_Documento($modelInstrumentoUnidade->docid);
        $inp = $mInp->recuperar();
        $mdDocPl = new Workflow_Model_Documento($inp->docid);

        global $simec;
        $modelIniciativaPlanejamentoItemComposicaoEscola = new Par3_Model_IniciativaPlanejamentoItemComposicaoEscola();

        $mdUnidade      = new Par3_Model_InstrumentoUnidade($dados['inuid']);
        $mdWFUnidade    = new Workflow_Model_Documento($mdUnidade->docid);

        $mdPlanejamento = new Par3_Model_IniciativaPlanejamento($dados['inpid']);
        $mdWFPlan       = new Workflow_Model_Documento($mdPlanejamento->docid);

        $habilitaExclusao = false;
        if ($mdWFUnidade->esdid == PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO &&
            $mdWFPlan->esdid == PAR3_ESDID_CADASTRAMENTO
        ) {
            $habilitaExclusao = true;
        }

        $sql = "select itcid, igrid from par3.iniciativa_itenscomposicao_grupo WHERE iigid = '{$dados['iigid']}' --AND iigsituacao = 'A'";
        $iicg = $this->model->pegaLinha($sql);
        if ($iicg['itcid']) {
            $sql = "SELECT iicg.iigid, iia.iniano, itcdsc, unidsc, (SELECT max(itdvalor)
                            FROM par3.itenscomposicao_detalhamento id
                            WHERE id.itcid = iicg.itcid
                            AND itdstatus = 'A'
                            AND itdsituacao = 'A'
                            AND (now() BETWEEN itdperiodoinicio AND itdperiodofim  OR itdperiodofim < now())
                            AND exists (SELECT 1 FROM par3.itenscomposicao_detalhamento_estado ice
                                    WHERE ice.itdid = id.itdid
                                    AND estuf in (select estuf from par3.instrumentounidade WHERE inuid = {$dados['inuid']}))
                            GROUP BY itdperiodofim
                            ORDER BY itdperiodofim DESC limit 1) valorreferencia
                    FROM par3.iniciativa_planejamento ip
                    JOIN par3.iniciativa i USING (iniid)
                    JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                    JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                    JOIN par3.itenscomposicao ic USING (itcid)
                    JOIN par3.unidade_medida um USING (uniid)
                    WHERE ip.inpid = {$dados['inpid']} AND iia.iianostatus = 'A' AND iicg.iigid = {$dados['iigid']}
                    ORDER BY iia.iniano;";
        } elseif ($iicg['igrid']) {
            $sql = "SELECT iicg.iigid, iia.iniano, igrnome as itcdsc, '-' as unidsc, sum(icgi.gitqtd * (SELECT max(itdvalor)
                            FROM par3.itenscomposicao_detalhamento id
                            WHERE icgi.itcid = id.itcid
                            AND itdstatus = 'A'
                            AND itdsituacao = 'A'
                            AND (now() between itdperiodoinicio AND itdperiodofim OR itdperiodofim < now())
                            AND exists (SELECT 1 FROM par3.itenscomposicao_detalhamento_estado ice
                                    WHERE ice.itdid = id.itdid
                                    AND estuf in (select estuf from par3.instrumentounidade WHERE inuid = {$dados['inuid']}))
                            GROUP BY itdperiodofim
                            ORDER BY itdperiodofim DESC limit 1)) valorreferencia
                    FROM par3.iniciativa_planejamento ip
                    JOIN par3.iniciativa i USING (iniid)
                    JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                    JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                    JOIN par3.itenscomposicao_grupos icg USING (igrid)
                    JOIN par3.itenscomposicao_grupos_itens icgi USING (igrid)
                    WHERE ip.inpid = {$dados['inpid']} AND iia.iianostatus = 'A' AND iicg.iigid = {$dados['iigid']}
                    GROUP BY iicg.iigid, iia.iniano, igrnome
                    ORDER BY iia.iniano;";
        }

        $dadosLinha = $this->model->carregar($sql);
        
        $arconsulta[1] = 'iigid = ' . $dados['iigid'];
        $arconsulta[2] = 'inpid = ' . $dados['inpid'];
        $arconsulta[3] = "ipistatus = 'A'";
        $valoresCadastrador = $this->model->recuperarTodos('MAX(ipivalorreferencia) as ipivalorreferencia', $arconsulta);
    
        if ($dadosLinha) {
            if ($valoresCadastrador[0]['ipivalorreferencia']) {
                $referenciaValor = $valoresCadastrador[0]['ipivalorreferencia'];
            } else {
                $referenciaValor = $dadosLinha[0]['valorreferencia'];
            }

            $acao = "<td id='acao-{$dadosLinha[0]['iigid']}'>$excluir</td>";
            $nome = "<td id='nome-{$dadosLinha[0]['iigid']}'> {$dadosLinha[0]['itcdsc']}</td>";
            $unidade = "<td> {$dadosLinha[0]['unidsc']}
                            <input type=\"hidden\" name=\"referencia[{$dadosLinha[0]['iigid']}]\" value='{$referenciaValor}'>
                            <input type=\"hidden\" name=\"iigid[]\" value='{$dadosLinha[0]['iigid']}'>
                        </td>";
            $referencia = "<td>"."R$ ".formata_valor($referenciaValor)."<div style='display:none;' id='vl-referencia-{$dadosLinha[0]['iigid']}'>".formata_valor($referenciaValor)."</div></td>";
            $item = "<td id='qtd-total-{$dadosLinha[0]['iigid']}'> - </td>";
            $valor = "<td id='vl-total-{$dadosLinha[0]['iigid']}'> - </td>";
        }
        $anosEditaveis = $anosEstaticos = '';
        if (is_array($dadosLinha)) {
            foreach ($dadosLinha as $itemAno) {
                if ($referenciaValor) {
                    $arconsulta[4] = 'ipiano = ' . $itemAno['iniano'];
                    if ($dados['condicao-por-escola']) {
                        $valoresCadastradorAno = $modelIniciativaPlanejamentoItemComposicaoEscola->getDadosTotais($arconsulta);
                    } else {
                        $valoresCadastradorAno = $this->model->recuperarTodos('ipiquantidade', $arconsulta);
                    }
                    $qtd = $valoresCadastradorAno[0]['ipiquantidade'];

                    //Itens do kit proinfancia devem aparecer com os valores pré cadastrados
                    $vlr = '-';

                    if(empty($valoresCadastradorAno) && $dados['obrid']){
                        $it = $this->model->recuperaQtdItemProinfancia($iicg);
                        $qtd = $it['gitqtd'];
                        $vlr = par3_mascaraMoeda($qtd * $it['itdvalor']);
                    }
                }
                
                $sql = "select d.esdid, a.anaano from par3.analise a
							inner join workflow.documento d on d.docid = a.docid
						where a.inpid = {$dados['inpid']} and a.anaano = {$itemAno['iniano']} AND anastatus = 'A'";
                $dadosAnalise = $this->model->pegaLinha($sql);

                $esdidBloqIniciativa = array(PAR3_ESDID_EM_DILIGENCIA, PAR3_ESDID_EM_ANALISE, PAR3_ESDID_AGUARDANDO_ANALISE);

                //se for do proinfancia (obrid), so deve ser habilitado apra o ano vigente
                if($dados['obrid'] > 0 && $itemAno['iniano'] != date('Y')){
                    $config['readonly'] = 'readonly';
                    $qtd = '';
                    $vlr = '-';
                }else{
                    unset($config['readonly']);
                }
                
                if (in_array($mdDocPl->esdid, $esdidBloqIniciativa)) {
                    $config['readonly'] = 'readonly';
                }

                if ($dados['condicao-por-escola']) {
                    $config['readonly'] = 'readonly';
                    $config['onclick'] = "abreModalEscolas({$dados['inpid']},{$itemAno['iniano']},{$itemAno['iigid']})";
                    $config['style'] = 'cursor:pointer;text-align:center;';
                    $config['title'] = 'Clique no campo para adicionar a Quantidade de Equipamentos para as Escolas';
                    $config['help'] = '<small style="font: 8px">Clique acima para adicionar Item</small>';
                } else {
                    $config['onchange'] = 'atualizaListagemItensComposicao()';
                }

                $config['maxlength'] = 7;
                $config['class'] = 'inteiro';
                $config['type'] = 'number';
                $config['onchange'] = 'atualizaListagemItensComposicao()';
                $config['id'] = 'qtd-item-' . $itemAno['iigid'] . '-' . $itemAno['iniano'];
                
                $esdidBloqIniciativa = array(PAR3_ESDID_EM_DILIGENCIA, PAR3_ESDID_CADASTRAMENTO);
                $esdidBloqUnidade = array(PAR3_ESDID_PLANEJAMENTO_NAO_INICIADO, PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO);
                
                //disabilita os campos de quantidade caso a unidade ou a iniciativa estejam finalizados.
                //if (($modelDocumento->esdid != PAR3_ESDID_PLANEJAMENTO_NAO_INICIADO || $modelDocumento->esdid != PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO) && $mdDocPl->esdid != PAR3_ESDID_CADASTRAMENTO) $config['disabled'] = 'disabled';
//                if( !in_array($modelDocumento->esdid, $esdidBloqUnidade) || !in_array($mdDocPl->esdid, $esdidBloqIniciativa) ){
//                  $config['disabled'] = 'disabled';
//                }
                
                if ($dadosAnalise['esdid'] == PAR3_ESDID_ANALISE_DILIGENCIA) {
                    $config['readonly'] = '';
                    $config['disabled'] = '';
                    unset($config['readonly']);
                    unset($config['disabled']);
                }

                $anosEditaveis .= "<td class='col-lg-2 col-md-2 col-sm-2'>
                                <input type=\"hidden\" name=\"ano-{$itemAno['iigid']}\" value='{$itemAno['iniano']}'>
                                " . $simec->input('qtd-item[' . $itemAno['iigid'] . '][' . $itemAno['iniano'] . ']', null, $qtd, $config, array('input-size' => '12')) . "
                                </td>";
                $anosEstaticos .= "<td id='vl-item-{$itemAno['iigid']}-{$itemAno['iniano']}' class='vl-item-{$itemAno['iigid']}'> {$vlr} </td>";
                if ($habilitaExclusao) {
                    $linkBotao = '<td class="text-center" style="width:33px">
                                <a title="Remover">
                                    <span data-id="'. $itemAno['iigid'] .'" class="btn btn-danger btn-sm glyphicon glyphicon-trash removerItem"></span>
                                </a>
                            </td>';
                }
            }
        }
        $tabela = '<tr>';
        (!empty($linkBotao)) ? $tabela .= $linkBotao : null ;
        $tabela .= $nome;
        $tabela .= $unidade;
        $tabela .= $referencia;
        $tabela .= $anosEditaveis;
        $tabela .= $anosEstaticos;
        $tabela .= $item;
        $tabela .= $valor;
        $tabela .= '</tr>';
        return $tabela;
    }
    
    public function buscaLinhaImpressao($dados)
    {

        //recupera estado do planejamento e iniciativa, para desabilitar os campos quantidade caso o planejamento ou a iniciatva estejam finalizados.
        $mInp = new Par3_Controller_IniciativaPlanejamento();
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $modelDocumento = new Workflow_Model_Documento($modelInstrumentoUnidade->docid);
        $inp = $mInp->recuperar();
        $mdDocPl = new Workflow_Model_Documento($inp->docid);
        $qtdTotal = 0;


        global $simec;
        $modelIniciativaPlanejamentoItemComposicaoEscola = new Par3_Model_IniciativaPlanejamentoItemComposicaoEscola();

        $mdUnidade      = new Par3_Model_InstrumentoUnidade($dados['inuid']);
        $mdWFUnidade    = new Workflow_Model_Documento($mdUnidade->docid);

        $mdPlanejamento = new Par3_Model_IniciativaPlanejamento($dados['inpid']);
        $mdWFPlan       = new Workflow_Model_Documento($mdPlanejamento->docid);

        $sql = "select itcid, igrid from par3.iniciativa_itenscomposicao_grupo WHERE iigid = '{$dados['iigid']}' --AND iigsituacao = 'A'";
        $iicg = $this->model->pegaLinha($sql);
        if ($iicg['itcid']) {
            $sql = "SELECT iicg.iigid, iia.iniano, itcdsc, unidsc, (SELECT max(itdvalor)
                            FROM par3.itenscomposicao_detalhamento id
                            WHERE id.itcid = iicg.itcid
                            AND itdstatus = 'A'
                            AND itdsituacao = 'A'
                            AND (now() BETWEEN itdperiodoinicio AND itdperiodofim  OR itdperiodofim < now())
                            AND exists (SELECT 1 FROM par3.itenscomposicao_detalhamento_estado ice
                                    WHERE ice.itdid = id.itdid
                                    AND estuf in (select estuf from par3.instrumentounidade WHERE inuid = {$dados['inuid']}))
                            GROUP BY itdperiodofim
                            ORDER BY itdperiodofim DESC limit 1) valorreferencia
                    FROM par3.iniciativa_planejamento ip
                    JOIN par3.iniciativa i USING (iniid)
                    JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                    JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                    JOIN par3.itenscomposicao ic USING (itcid)
                    JOIN par3.unidade_medida um USING (uniid)
                    WHERE ip.inpid = {$dados['inpid']} AND iia.iianostatus = 'A' AND iicg.iigid = {$dados['iigid']}
                    ORDER BY iia.iniano;";
        } elseif ($iicg['igrid']) {
            $sql = "SELECT iicg.iigid, iia.iniano, igrnome as itcdsc, '-' as unidsc, sum(icgi.gitqtd * (SELECT max(itdvalor)
                            FROM par3.itenscomposicao_detalhamento id
                            WHERE icgi.itcid = id.itcid
                            AND itdstatus = 'A'
                            AND itdsituacao = 'A'
                            AND (now() between itdperiodoinicio AND itdperiodofim OR itdperiodofim < now())
                            AND exists (SELECT 1 FROM par3.itenscomposicao_detalhamento_estado ice
                                    WHERE ice.itdid = id.itdid
                                    AND estuf in (select estuf from par3.instrumentounidade WHERE inuid = {$dados['inuid']}))
                            GROUP BY itdperiodofim
                            ORDER BY itdperiodofim DESC limit 1)) valorreferencia
                    FROM par3.iniciativa_planejamento ip
                    JOIN par3.iniciativa i USING (iniid)
                    JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                    JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                    JOIN par3.itenscomposicao_grupos icg USING (igrid)
                    JOIN par3.itenscomposicao_grupos_itens icgi USING (igrid)
                    WHERE ip.inpid = {$dados['inpid']} AND iia.iianostatus = 'A' AND iicg.iigid = {$dados['iigid']}
                    GROUP BY iicg.iigid, iia.iniano, igrnome
                    ORDER BY iia.iniano;";
        }

        $dadosLinha = $this->model->carregar($sql);
        
        $arconsulta[1] = 'iigid = ' . $dados['iigid'];
        $arconsulta[2] = 'inpid = ' . $dados['inpid'];
        $arconsulta[3] = "ipistatus = 'A'";
        $valoresCadastrador = $this->model->recuperarTodos('MAX(ipivalorreferencia) as ipivalorreferencia', $arconsulta);
    
        if ($dadosLinha) {
            if ($valoresCadastrador[0]['ipivalorreferencia']) {
                $referenciaValor = $valoresCadastrador[0]['ipivalorreferencia'];
            } else {
                $referenciaValor = $dadosLinha[0]['valorreferencia'];
            }


            $nome = "<td id='nome-{$dadosLinha[0]['iigid']}'> {$dadosLinha[0]['itcdsc']}</td>";
            $unidade = "<td> {$dadosLinha[0]['unidsc']}
                            <input type=\"hidden\" name=\"referencia[{$dadosLinha[0]['iigid']}]\" value='{$referenciaValor}'>
                            <input type=\"hidden\" name=\"iigid[]\" value='{$dadosLinha[0]['iigid']}'>
                        </td>";
            $referencia = "<td>"."R$ ".formata_valor($referenciaValor)."<div style='display:none;' id='vl-referencia-{$dadosLinha[0]['iigid']}'>".formata_valor($referenciaValor)."</div></td>";
            $item = "<td id='qtd-total-{$dadosLinha[0]['iigid']}'> - </td>";
            $valor = "<td id='vl-total-{$dadosLinha[0]['iigid']}'> - </td>";
        }
        $anosEditaveis = $anosEstaticos = '';
        if (is_array($dadosLinha)) {
            foreach ($dadosLinha as $itemAno) {
                if ($valoresCadastrador[0]['ipivalorreferencia']) {
                    $arconsulta[4] = 'ipiano = ' . $itemAno['iniano'];
                    if ($dados['condicao-por-escola']) {
                        $valoresCadastradorAno = $modelIniciativaPlanejamentoItemComposicaoEscola->getDadosTotais($arconsulta);
                    } else {
                        $valoresCadastradorAno = $this->model->recuperarTodos('ipiquantidade', $arconsulta);
                    }
                    $qtd = $valoresCadastradorAno[0]['ipiquantidade'];
                }
                
                $sql = "select d.esdid, a.anaano from par3.analise a
							inner join workflow.documento d on d.docid = a.docid
						where a.inpid = {$dados['inpid']} and a.anaano = {$itemAno['iniano']} AND anastatus = 'A'";
                $dadosAnalise = $this->model->pegaLinha($sql);

                $esdidBloqIniciativa = array(PAR3_ESDID_EM_DILIGENCIA, PAR3_ESDID_EM_ANALISE, PAR3_ESDID_AGUARDANDO_ANALISE);
                
                $config['readonly'] = 'readonly';

                if ($dados['condicao-por-escola']) {
                    $config['readonly'] = 'readonly';
                    $config['onclick'] = "abreModalEscolas({$dados['inpid']},{$itemAno['iniano']},{$itemAno['iigid']})";
                    $config['style'] = 'cursor:pointer;text-align:center;';
                    $config['title'] = 'Clique no campo para adicionar a Quantidade de Equipamentos para as Escolas';
                    $config['help'] = '<small style="font: 8px">Clique acima para adicionar Item</small>';
                }

                $config['maxlength'] = 7;
                $config['class'] = 'inteiro';
                $config['type'] = 'number';
                $config['onchange'] = 'atualizaListagemItensComposicao()';
                $config['id'] = 'qtd-item-' . $itemAno['iigid'] . '-' . $itemAno['iniano'];
                
                $esdidBloqIniciativa = array(PAR3_ESDID_EM_DILIGENCIA, PAR3_ESDID_CADASTRAMENTO);
                $esdidBloqUnidade = array(PAR3_ESDID_PLANEJAMENTO_NAO_INICIADO, PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO);
                
                //disabilita os campos de quantidade caso a unidade ou a iniciativa estejam finalizados.
                //if (($modelDocumento->esdid != PAR3_ESDID_PLANEJAMENTO_NAO_INICIADO || $modelDocumento->esdid != PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO) && $mdDocPl->esdid != PAR3_ESDID_CADASTRAMENTO) $config['disabled'] = 'disabled';
//                if( !in_array($modelDocumento->esdid, $esdidBloqUnidade) || !in_array($mdDocPl->esdid, $esdidBloqIniciativa) ){
//                  $config['disabled'] = 'disabled';
//                }
                
                if ($dadosAnalise['esdid'] == PAR3_ESDID_ANALISE_DILIGENCIA) {
                    $config['readonly'] = '';
                    $config['disabled'] = '';
                    unset($config['readonly']);
                    unset($config['disabled']);
                }

                if( $qtd > 0 ){
                    $qtdTotal = $qtdTotal + $qtd;
                    $anosEditaveis .= "<td class='col-lg-2 col-md-2 col-sm-2' >{$qtd}</td>";
                    $total = $referenciaValor * $qtd;
                    $vlTotal = $vlTotal + $total;
                    $anosEstaticos .= "<td class='col-lg-2 col-md-2 col-sm-2' >R$ ".formata_valor($total)."</td>";
                } else {
                    $anosEditaveis .= "<td class='col-lg-2 col-md-2 col-sm-2'>-</td>";
                    $anosEstaticos .= "<td class='col-lg-2 col-md-2 col-sm-2'>-</td>";;
                }
            }
        }
        $tabela = '<tr>';
        $tabela .= $nome;
        $tabela .= $unidade;
        $tabela .= $referencia;
        $tabela .= $anosEditaveis;
        $tabela .= $anosEstaticos;
        $tabela .= '<td>'.$qtdTotal.'</td>';
        $tabela .= '<td>'.($vlTotal>0 ? 'R$ '.formata_valor($vlTotal) : '-').'</td>';
        $tabela .= '</tr>';
        $arRetorno = array('tabela' => $tabela, 'valor' => $vlTotal);
        return $arRetorno;
    }
    
    public function getPlanilhaAnalise($arParam = array())
    {
        $inpid = $arParam['inpid'];
        $inuid = $arParam['inuid'];
        $iniano = $arParam['iniano'];
        $condicaoPorEscola = $arParam['condicaoporescola'];
        $esdid = $arParam['esdid'];
        $anaid = $arParam['anaid'];
        
        $sql = "select iigid from par3.iniciativa_planejamento_item_composicao where inpid = {$inpid} and ipiano = $iniano and ipistatus = 'A' group by iigid;";
        $itensCadastrados = $this->model->carregarColuna($sql);
        
        if ($itensCadastrados) {
            $iigid = implode(', ', $itensCadastrados);
            
            $tabela = '';
            if (is_array($itensCadastrados)) {
                $arConsulta['inpid'] = $inpid;
                $arConsulta['inuid'] = $inuid;
                $arConsulta['iniano'] = $iniano;
                $arConsulta['esdid'] = $esdid;
                $arConsulta['anaid'] = $anaid;
                $arConsulta['reformulacao'] = ($arParam['reformulacao'] ? $arParam['reformulacao'] : 'N');
                $arConsulta['condicao-por-escola'] = $condicaoPorEscola;
                
                $arConsulta['iigid'] = $iigid;
                if( $arConsulta['reformulacao'] == 'S' ){
                    $arConsulta['refid'] = $arParam['refid'];
                    $arConsulta['pega_linha'] = $arParam['pega_linha'];
                    $arConsulta['iigid_ref'] = $arParam['iigid'];
                    $arConsulta['refidpai'] = $arParam['refidpai'];
                    $arConsulta['dotid'] = $arParam['dotid'];
                    $arConsulta['dotstatus'] = $arParam['dotstatus'];
                    $tabela = $this->buscaLinhaItemAnaliseReformulacao($arConsulta);
                } else {
                    $arConsulta['pega_linha'] = 'N';
                    $tabela = $this->buscaLinhaItemAnalise($arConsulta);
                }
            }
        }
        
        return $tabela;
    }
    
    public function buscaLinhaItemAnalise($dados)
    {
        global $simec;
        
        $modelIniciativaPlanejamentoItemComposicaoEscola = new Par3_Model_IniciativaPlanejamentoItemComposicaoEscola();
        $inuid = $dados['inuid'];
                
        $sql = "select distinct iic.iigid, iig.itcid, iig.igrid from par3.iniciativa_planejamento_item_composicao  iic
					inner join par3.iniciativa_itenscomposicao_grupo iig on iig.iigid = iic.iigid --and iig.iigsituacao = 'A'
				where iic.inpid = {$dados['inpid']} and iic.iigid in ({$dados['iigid']}) and iic.ipistatus = 'A'";
       
        $iicg = $this->model->pegaLinha($sql);
        
        $sql = "SELECT count(eic.ieiid)
                FROM par3.iniciativa_emenda_item_composicao eic
                	INNER JOIN par3.iniciativa_emenda ie ON ie.ineid = eic.ineid
                    INNER JOIN par3.analise a ON a.edeid = ie.edeid AND a.inpid = ie.inpid AND a.anastatus = 'A'
                WHERE eic.ieistatus = 'A' AND ie.inestatus = 'A' AND ie.inpid = {$dados['inpid']} AND a.anaid = {$dados['anaid']}";
        $boEmenda = $this->model->pegaUm($sql);
        
        $sql = $this->montaSqlItemAnalise($iicg, $boEmenda, $dados);
        
        if ($_SESSION['usucpf'] == '') {
            echo getToobarListagem($sql, false);
        }
        $dadosLinha = $this->model->carregar($sql);
        
        $arrRegistros = array();
        
        $anosEditaveis = $anosEstaticos = '';
        if (is_array($dadosLinha)) {
            $coluna = '';
            if ((int)$boEmenda > (int)0) {
                $coluna = '<th style="vertical-align: middle;">QTD Indicada (Emenda)</th>
        						<th style="vertical-align: middle;">Total(R$)</th>';
            }
            
            $tabela = '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="dados-itens-composicao" data-qtd-acoes="4">
        				<thead>
        					<tr>
        						<th style="vertical-align: middle;">Item</th>
        						<th style="vertical-align: middle;">Unidade de Medida</th>
        						<th style="vertical-align: middle;">Valor Referência(R$)</th>
        						<th style="vertical-align: middle; width: 160px;">Quantidade</th>
        						<th style="vertical-align: middle;">Total(R$)</th>
        						'.$coluna.'
        						<th style="vertical-align: middle; width: 160px;">QTD Aprovado</th>
        						<th style="vertical-align: middle;">Valor Aprovado(R$)</th>
        						<th style="vertical-align: middle;">QTD Total Aprovada</th>
        						<th style="vertical-align: middle;">Vlr Total Aprovado(R$)</th>
        					</tr>
        				</thead>
        				<tbody>';
            
            $totalItemGeral = 0;
            $totalItemEmendaGeral = 0;
            $vlrTotalItensGeral = 0;
            $qtdTotalAprovado = 0;
            $vlrTotalAprovado = 0;
            foreach ($dadosLinha as $itemAno) {
                $arconsulta = array();
                
                $arconsulta[0] = 'iigid = ' . $itemAno['iigid'];
                $arconsulta[1] = 'inpid = ' . $dados['inpid'];
                $arconsulta[2] = "ipistatus = 'A'";
                
                $valoresCadastrador = $this->model->recuperarTodos('MAX(ipivalorreferencia) as ipivalorreferencia', $arconsulta);
                
                if ($valoresCadastrador[0]['ipivalorreferencia']) {
                    $itemAno['valorreferencia'] = $valoresCadastrador[0]['ipivalorreferencia'];
                }
                
                if ($valoresCadastrador[0]['ipivalorreferencia']) {
                    $arconsulta[3] = 'ipiano = ' . $itemAno['iniano'];
                    
                    if ($dados['condicao-por-escola']) {
                        $valoresCadastradorAno = $modelIniciativaPlanejamentoItemComposicaoEscola->getDadosTotais($arconsulta);
                    } else {
                        $valoresCadastradorAno = $this->model->recuperarTodos('ipiquantidade', $arconsulta);
                    }
                    $qtd = $valoresCadastradorAno[0]['ipiquantidade'];
                }
                
                $qtd = ($itemAno['ipiquantidade'] ? $itemAno['ipiquantidade'] : $qtd);
                
                $attribs['readonly'] = 'readonly';
                if ($dados['condicao-por-escola']) {
                    $attribs['onclick'] = "abreModalEscolas({$dados['inpid']},{$itemAno['iniano']},{$itemAno['iigid']})";
                    $attribs['style'] = 'cursor:pointer;text-align:center;';
                    $attribs['title'] = 'Clique no campo para adicionar a Quantidade de Equipamentos para as Escolas';
                    $attribs['help'] = '<small style="font: 8px">Clique acima para visualizar Escolas</small>';
                }
                $attribs['maxlength'] = 7;
                $attribs['size'] = 10;
                $attribs['class'] = 'inteiro';
                $attribs['type'] = 'number';
                $attribs['id'] = 'qtd-item-' . $itemAno['iigid'] . '-' . $itemAno['iniano'];
                
                //$radioItem = array('N' => 'Não', 'S' => 'Sim', 'T' => 'Sim com Saldo');
                $totalItem = ((float)$itemAno['valorreferencia'] * (int)$qtd);
                $totalItemEmenda = ((float)$itemAno['valorreferencia'] * (int)$itemAno['quantidade_emenda']);
                
                $estadoBloqueio = array(PAR3_ESDID_AGUARDANDO_VALIDACAO_COORDENADOR, PAR3_ESDID_EM_VALIDACAO_COORDENADOR, PAR3_ESDID_ANALISE_PLANEJAMENTO_APROVADO);
                $itemAno['aicqtdaprovado'] = str_replace(".", "", $itemAno['aicqtdaprovado']);
                
                $attribuQTDAprovado = array();
                if (!in_array($dados['esdid'], $estadoBloqueio) ) {
                    if (!empty(trim($itemAno['aicaprovado']))) {
                        $attribuQTDAprovado = array(
                            'onchange' => 'atualizaValoresItens('.$itemAno['iigid'].', '.$itemAno['iniano'].')',
                            'onkeyup' => 'this.value=mascaraglobal(\'###############\',this.value)',
                            'onblur' => 'this.value=mascaraglobal(\'###############\',this.value); atualizaValoresItens('.$itemAno['iigid'].', '.$itemAno['iniano'].')'
                        );
                        $vlrTotalItens = ((int)$itemAno['aicqtdaprovado'] * (float)$itemAno['valorreferencia']);
                        if ($itemAno['aicaprovado'] == 'N' || $itemAno['aicaprovado'] == 'S') {
                            $attribuQTDAprovado =
                            array(
                                'onchange' => 'atualizaValoresItens('.$itemAno['iigid'].', '.$itemAno['iniano'].')',
                                'onkeyup' => 'this.value=mascaraglobal(\'###############\',this.value)',
                                'onblur' => 'this.value=mascaraglobal(\'###############\',this.value), atualizaValoresItens('.$itemAno['iigid'].', '.$itemAno['iniano'].')'
                            );
                        }
                    } else {
                        $attribuQTDAprovado =
                        array(
                            'onchange' => 'atualizaValoresItens('.$itemAno['iigid'].', '.$itemAno['iniano'].')',
                            'onkeyup' => 'this.value=mascaraglobal(\'###############\',this.value)',
                            'onblur' => 'this.value=mascaraglobal(\'###############\',this.value)'
                        );
                        $vlrTotalItens = '0';
                    }
                    //$attribusCheck = array('onclick' => 'checarItem('.$itemAno['iigid'].', '.$itemAno['iniano'].')');
                } else {
                    $vlrTotalItens = ((int)$itemAno['aicqtdaprovado'] * (float)$itemAno['valorreferencia']);
                    $attribuQTDAprovado = array(
                        'onkeyup' => 'this.value=mascaraglobal(\'###############\',this.value)',
                        'onblur' => 'this.value=mascaraglobal(\'###############\',this.value)'
                    );
                    //$attribusCheck = array('onclick' => 'checarItem('.$itemAno['iigid'].', '.$itemAno['iniano'].')', 'disabled' => 'disabled');
                }
                
                $campos = "";
                $camosEmenda = "";
                if ((int)$boEmenda > (int)0) {
                    
                    $campos = "<td>".$simec->input('qtd-item-emenda[' . $itemAno['iigid'] . '][' . $itemAno['iniano'] . ']', null, $itemAno['quantidade_emenda'], $attribs, array('input-size' => '12'))."</td>
                				<td>".formata_valor($totalItemEmenda)."</td>";
                    $totalItemEmendaGeral = ($totalItemEmendaGeral + $totalItemEmenda);
                    
                } else {
                    $camosEmenda = '<input name="qtd-item-emenda['.$itemAno['iigid'].']['.$itemAno['iniano'].']" id="qtd-item-emenda-'.$itemAno['iigid'].'-'.$itemAno['iniano'].'" type="hidden" value="0" class="form-control inteiro">';
                }
                
                $tabela .= "<tr>
                				<td id='nome-{$itemAno['iigid']}'>".$itemAno['itcdsc']."
                						<input type=\"hidden\" name=\"iigid[{$itemAno['iigid']}]\" value='{$itemAno['iigid']}'>
                						<input type=\"hidden\" name=\"inpid[]\" value='{$dados['inpid']}'>
                						<input type=\"hidden\" name=\"iniano[{$itemAno['iigid']}]\" value='{$itemAno['iniano']}'>
                						<input type=\"hidden\" name=\"ipiid[{$itemAno['iigid']}]\" value='{$itemAno['ipiid']}'>
                						<input type=\"hidden\" name=\"inuid[]\" value='{$inuid}'>
                                        <input type=\"hidden\" name=\"valor_referencia[".$itemAno['iigid']."][".$itemAno['iniano']."]\" value='".$itemAno['valorreferencia']."'>
                                        <input type=\"hidden\" name=\"qtd_total_aprovada[".$itemAno['iigid']."][".$itemAno['iniano']."]\" value='".$itemAno['qtd_total_aprovada']."'>
                                        <input type=\"hidden\" name=\"qtd-item-aprovado-s[".$itemAno['iigid']."][".$itemAno['iniano']."]\" value='".($itemAno['aicqtdaprovado'] ? $itemAno['aicqtdaprovado'] : '0')."'>
                                        $camosEmenda
                						</td>
                				<td style='width: 150px;'>".$itemAno['unidsc']."</td>
                				<td>".formata_valor($itemAno['valorreferencia'])."</td>
                				<td><input type=\"hidden\" name=\"ano-{$itemAno['iigid']}\" value='{$itemAno['iniano']}'>".$simec->input('qtd-item[' . $itemAno['iigid'] . '][' . $itemAno['iniano'] . ']', null, $qtd, $attribs, array('input-size' => '12'))."</td>
                				<td>".formata_valor($totalItem)."</td>
                                ".$campos."
                				<td>".$simec->input('qtd-item-aprovado['.$itemAno['iigid'].']['.$itemAno['iniano'].']', null, ($itemAno['aicqtdaprovado'] ? $itemAno['aicqtdaprovado'] : '0'), $attribuQTDAprovado, array('input-size' => '12'))."</td>
                				<td><div id='div_vlr_aprovado_".$itemAno['iigid']."_".$itemAno['iniano']."'>".formata_valor($vlrTotalItens)."</div></td>
                				<td>".$itemAno['qtd_total_aprovada']."</td>
                				<td>".formata_valor($itemAno['total_aprovado'])."</td>
                			</tr>";

                $qtdTotalAprovado = ($qtdTotalAprovado + (int)$itemAno['qtd_total_aprovada']);
                $vlrTotalAprovado = ($vlrTotalAprovado + (int)$itemAno['total_aprovado']);
                $totalItemGeral = ($totalItemGeral + $totalItem);
                $vlrTotalItensGeral = ($vlrTotalItensGeral + $vlrTotalItens);
            }
            $tabela .= '<tr>
            				<td colspan=3><b>Totais:</b></td>
            				<td>&nbsp;</td>
            				<td>'.formata_valor($totalItemGeral).'</td>';
            if ((int)$boEmenda > (int)0) {
                $tabela .= '<td>&nbsp;</td>
            				<td>'.formata_valor($totalItemEmendaGeral).'</td>';
            }
            
            $tabela .= '    <td>&nbsp;</td>
            				<td>'.formata_valor($vlrTotalItensGeral).'</td>
            				<td>'.formata_valor($qtdTotalAprovado).'</td>
            				<td>'.formata_valor($vlrTotalAprovado).'</td>
            			</tr>';
        }
        $tabela .= '</tbody></table>';
        
        return $tabela;
    }
    
    public function buscaLinhaItemAnaliseReformulacao($dados)
    {
        global $simec;
        
        $modelIniciativaPlanejamentoItemComposicaoEscola = new Par3_Model_IniciativaPlanejamentoItemComposicaoEscola();
        $inuid = $dados['inuid'];
                
        $sql = "select distinct iic.iigid, iig.itcid, iig.igrid from par3.iniciativa_planejamento_item_composicao  iic
					inner join par3.iniciativa_itenscomposicao_grupo iig on iig.iigid = iic.iigid --and iig.iigsituacao = 'A'
				where iic.inpid = {$dados['inpid']} and iic.iigid in ({$dados['iigid']}) and iic.ipistatus = 'A'";
       
        $iicg = $this->model->pegaLinha($sql);
        $boEmenda = 0;
        
        $sql = $this->montaSqlItemAnalise($iicg, $boEmenda, $dados);
        
        $tabela = '';
        $tabelaLinha = '';
        if ($_SESSION['usucpf'] == '') {
            $tabela .= getToobarListagem($sql, false);
        }
        
        $dadosLinha = $this->model->carregar($sql);
        
        $arrRegistros = array();
        
        $style = "";
        $styleN = "";
        if( in_array($dados['esdid'], array(PAR3_REFORMULACAO_ESDID_EM_CADASTRAMENTO) ) ){
            $style = ' style="display: none"; ';
            $styleN = ' display: none; ';
        }
        $obReformulacao = new Par3_Controller_Reformulacao();
        $arrPermissao = $obReformulacao->verificaPermissaoTela($dados['esdid'], $dados['refid']);
        
        $anosEditaveis = $anosEstaticos = '';
        if (is_array($dadosLinha)) {
            $tabela .= '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="dados-itens-composicao" data-qtd-acoes="4">
        				<thead>
        					<tr>
        						<th style="vertical-align: middle;"></th>
        						<th style="vertical-align: middle;">Item</th>
        						<th style="vertical-align: middle;">Unidade de Medida</th>
        						<th style="vertical-align: middle;">Valor Referência(R$)</th>
        						<th style="vertical-align: middle; width: 160px;">Quantidade Planejada</th>
        						<th style="vertical-align: middle;">Total(R$)</th>
        						<th style="vertical-align: middle; width: 160px;">Quantidade Pactuada</th>
        						<th style="vertical-align: middle;">Total Pactuado(R$)</th>
        						<th style="vertical-align: middle; width: 150px;">Qtd Solicitada</th>
        						<th style="vertical-align: middle;">Total Solicitado(R$)</th>';
            if( $arrPermissao['analise']['visualizar'] == 'S' ){
                $tabela .= '    <th style="vertical-align: middle; '.$styleN.'">QTD Aprovado</th>
                                <th style="vertical-align: middle; '.$styleN.'">Valor Aprovado(R$)</th>';
            }
            $tabela .= '
        					</tr>
        				</thead>
                        <tbody id="dados-itens-composicao-tbody_'.$dados['inpid'].'_'.$dados['iniano'].'">';
            
            $totalItemGeral = 0;
            $totalItemSolicitado = 0;
            $totalItemEmendaGeral = 0;
            $totalItemPactuadoGeral = 0;
            $vlrTotalItensGeral = 0;
            $qtdTotalAprovado = 0;
            $vlrTotalAprovado = 0;
            foreach ($dadosLinha as $itemAno) {
                
                $reiquantidade  = '';
                $reivalor       = '';
                $editavel       = 'N';
                
                if( $dados['pega_linha'] != 'S' ){
                    $sql = "SELECT reiid, reiquantidade, reivalor FROM par3.reformulacao_itemcomposicao ri
                            	INNER JOIN par3.reformulacao_documento rd ON rd.refid = ri.refid AND rd.refstatus = 'A'
                            WHERE ri.inpid = {$dados['inpid']} AND ri.reiano = {$itemAno['iniano']} AND ri.iigid = {$itemAno['iigid']} AND ri.refid = {$dados['refidpai']} AND rd.refidpai IS NULL";
                    $arItemref_bk = $this->model->pegaLinha($sql);
                    
                    $reiquantidade_bk = $arItemref_bk['reiquantidade'];
                    $reivalor_bk      = $arItemref_bk['reivalor'];
                    $reiid_bk         = $arItemref_bk['reiid'];
                    
                    $sql = "SELECT reiid, reiquantidade, reivalor FROM par3.reformulacao_itemcomposicao ri
                            	INNER JOIN par3.reformulacao_documento rd ON rd.refid = ri.refid AND rd.refstatus = 'A'
                            WHERE ri.inpid = {$dados['inpid']} AND ri.reiano = {$itemAno['iniano']} AND ri.iigid = {$itemAno['iigid']} AND ri.refid = {$dados['refid']} AND rd.refidpai IS NOT NULL";
                    $arItemref = $this->model->pegaLinha($sql);
                    
                    $reiquantidade = ($arItemref['reiquantidade']);
                    $reivalor      = ($arItemref['reivalor']);
                    $reiid         = ($arItemref['reiid']);
                    
                    $qtd = $reiquantidade_bk;
                    $itemAno['valorreferencia'] = ($reivalor_bk ? $reivalor_bk : $reivalor);
                } else {
                    $arconsulta = array();
                    $arconsulta[0] = 'iigid = ' . $itemAno['iigid'];
                    $arconsulta[1] = 'inpid = ' . $dados['inpid'];
                    $arconsulta[2] = "ipistatus = 'A'";
                    
                    $valoresCadastrador = $this->model->recuperarTodos('MAX(ipivalorreferencia) as ipivalorreferencia', $arconsulta);
                    
                    if ($valoresCadastrador[0]['ipivalorreferencia']) {
                        $itemAno['valorreferencia'] = $valoresCadastrador[0]['ipivalorreferencia'];
                    }
                    
                    if( $dados['pega_linha'] == 'S' ){
                        if( $itemAno['valorreferencia'] <> $itemAno['vlr_detalhamento'] ){
                            $itemAno['valorreferencia'] = $itemAno['vlr_detalhamento'];
                        }
                    }
                    
                    if ($valoresCadastrador[0]['ipivalorreferencia']) {
                        $arconsulta[3] = 'ipiano = ' . $itemAno['iniano'];
                        
                        if ($dados['condicao-por-escola']) {
                            $valoresCadastradorAno = $modelIniciativaPlanejamentoItemComposicaoEscola->getDadosTotais($arconsulta);
                        } else {
                            $valoresCadastradorAno = $this->model->recuperarTodos('ipiquantidade', $arconsulta);
                        }
                        $qtd = $valoresCadastradorAno[0]['ipiquantidade'];
                    }
                    $qtd = ($itemAno['ipiquantidade'] ? $itemAno['ipiquantidade'] : ($qtd ? $qtd : 0));
                }
                
                $attribs = array();
                $attribRefor = array();
                
                if(!empty($itemAno['ipiid'])) $filtroIpiid = " AND va.ipiid = {$itemAno['ipiid']} ";
                $sql = "SELECT va.ipiquantidadeaprovada
                    FROM par3.termocomposicao t
                    	INNER JOIN par3.analise a ON a.anaid = t.anaid
                    	INNER JOIN par3.v_analise_planejamento_item_composicao va ON va.anaid = a.anaid
                    WHERE dotid = {$dados['dotid']}
                    	AND va.iigid = {$itemAno['iigid']}
                    	$filtroIpiid
                    	AND t.inpid = {$dados['inpid']}
                    	AND t.tecano = {$itemAno['iniano']}";
            	$vlrPactuado = $this->model->pegaUm($sql);
            	$vlrPactuado = ($vlrPactuado ? $vlrPactuado : 0);
            	
            	$temVlrPactuado = 'N';
            	if( $vlrPactuado > 0 ) $temVlrPactuado = 'S';
                
                if ($dados['condicao-por-escola']) {
                    if( $arrPermissao['cadastro']['salvar'] == 'S' ){
                        $editavel = 'S';
                    }
                    $reiquantidade          = 0;
                    $attribs['onclick']     = "abreModalEscolasReformulacao({$dados['inpid']},{$itemAno['iniano']},{$itemAno['iigid']}, 'N', 'S', '')";
                    $attribs['style']       = 'cursor:pointer;text-align:center;';
                    $attribs['title']       = 'Clique no campo para adicionar a Quantidade de Equipamentos para as Escolas';
                    $attribs['help']        = '<small style="font: 8px">Clique acima para visualizar Escolas</small>';
                    $attribs['readonly']    = 'readonly';
                    
                    $attribRefor['onclick']     = "abreModalEscolasReformulacao({$dados['inpid']},{$itemAno['iniano']},{$itemAno['iigid']}, '$editavel', 'N', '$temVlrPactuado')";
                    $attribRefor['style']       = 'cursor:pointer;text-align:center;';
                    $attribRefor['title']       = 'Clique no campo para adicionar a Quantidade de Equipamentos para as Escolas';
                    $attribRefor['help']        = '<small style="font: 8px">Clique acima para visualizar Escolas</small>';
                    $attribRefor['readonly']    = 'readonly';
                }
                $attribs['maxlength']   = 7;
                $attribs['size']        = 10;
                $attribs['class']       = 'inteiro';
                $attribs['type']        = 'number';
                $attribs['id']          = 'qtd-item-' . $itemAno['iigid'];
                
                $attribRefor['maxlength']   = 7;
                $attribRefor['size']        = 10;
                $attribRefor['class']       = 'inteiro';
                $attribRefor['type']        = 'number';
                $attribRefor['id']          = 'qtd-item-solicitada-' . $itemAno['iigid'];
                
                $totalItem       = ((float)$itemAno['valorreferencia'] * (int)$qtd);
                $totalItemSolic  = ((float)$reivalor * (int)($reiquantidade?$reiquantidade:0));
                $totalItemEmenda = ((float)$itemAno['valorreferencia'] * (int)$itemAno['quantidade_emenda']);
                
                $itemAno['aicqtdaprovado'] = str_replace(".", "", $itemAno['aicqtdaprovado']);
                
                $vlrTotalItens = ((int)$itemAno['aicqtdaprovado'] * (float)$itemAno['valorreferencia']);
                
                $campos = "";
                $camosEmenda = "";
                $camosEmenda = '<input name="qtd-item-emenda['.$dados['inpid'].']['.$itemAno['iniano'].']['.$itemAno['iigid'].']" id="qtd-item-emenda-'.$itemAno['inpid'].'-'.$itemAno['iigid'].'-'.$itemAno['iniano'].'" type="hidden" value="0" class="form-control inteiro">';
                $attribs['readonly'] = 'readonly';
                
                $attribsPactuado = array(
                    'readonly' => 'readonly',
                    'onkeyup' => 'this.value=mascaraglobal(\'###############\',this.value)'
                );
                
                $attribRefor['onchange'] = 'atualizaValoresItens('.$dados['inpid'].", ".$itemAno['iniano'].", ".$itemAno['iigid'].')';
                $attribRefor['onkeyup'] = 'this.value=mascaraglobal(\'###############\',this.value)';
                $attribRefor['onblur'] = 'this.value=mascaraglobal(\'###############\',this.value)';
                
                $attribsAnalise = array(
                    'readonly' => 'readonly',
                    'onkeyup' => 'this.value=mascaraglobal(\'###############\',this.value)'
                );
                
                if( $arrPermissao['analise']['salvar'] == 'S' ){
                    unset($attribsAnalise['readonly']);
                    $attribsAnalise['onchange'] = 'atualizaValoresItensAnalise('.$dados['inpid'].", ".$itemAno['iniano'].", ".$itemAno['iigid'].')';
                    $attribsAnalise['onblur'] = 'this.value=mascaraglobal(\'###############\',this.value), atualizaValoresItensAnalise('.$dados['inpid'].", ".$itemAno['iniano'].", ".$itemAno['iigid'].')';
                }
                
                if( $arrPermissao['cadastro']['salvar'] == 'S' && !$dados['condicao-por-escola'] && $dados['dotstatus'] == 'A' ){
                    unset($attribRefor['readonly']);
                } else {
                    $attribRefor['readonly'] = 'readonly';
                }
                
                $totalItemPactuado = ((float)$itemAno['valorreferencia'] * (int)$vlrPactuado);
                
                $qtdMonitorada = $obReformulacao->verificaQtdMonitoradaItem($itemAno['ipiid'], $itemAno['iigid']);                
                $attribRefor['title'] = 'Quantidade Monitorada: '.$qtdMonitorada;
                
                $styleNome = "";
                if( $qtd > 0 && $reiquantidade == 0){
                    $styleNome = 'style="text-decoration: line-through"';
                }
                
                $tabelaLinha .= "<tr>
                                       <input type=\"hidden\" name=\"ano-{$itemAno['iigid']}\" value='{$itemAno['iniano']}'>
                                       <input type=\"hidden\" name=\"iigid[{$dados['inpid']}][".$itemAno['iniano']."][]\" value='{$itemAno['iigid']}'>
                                       <input type=\"hidden\" name=\"ipiid[".$dados['inpid']."][".$itemAno['iniano']."][{$itemAno['iigid']}]\" value='{$itemAno['ipiid']}'>
                                       <input type=\"hidden\" name=\"reiid[".$dados['inpid']."][".$itemAno['iniano']."][".$itemAno['iigid']."]\" value='".$reiid."'>
                                       <input type=\"hidden\" name=\"valor_referencia[".$dados['inpid']."][".$itemAno['iniano']."][".$itemAno['iigid']."]\" value='".$itemAno['valorreferencia']."'>
                                       <input type=\"hidden\" name=\"vlr_detalhamento[".$dados['inpid']."][".$itemAno['iniano']."][".$itemAno['iigid']."]\" value='".$itemAno['vlr_detalhamento']."'>
                                       <input type=\"hidden\" name=\"qtd_total_aprovada[".$dados['inpid']."][".$itemAno['iniano']."][".$itemAno['iigid']."]\" value='".$itemAno['qtd_total_aprovada']."'>
                            			<input type=\"hidden\" name=\"ano[".$dados['inpid']."][".$itemAno['iniano']."][".$itemAno['iigid']."]\" value='".$itemAno['iniano']."'>
                            			<input type=\"hidden\" name=\"raiid[".$dados['inpid']."][".$itemAno['iniano']."][".$itemAno['iigid']."]\" value='".$itemAno['raiid']."'>
                            			<input type=\"hidden\" name=\"anaid[".$dados['inpid']."][".$itemAno['iniano']."][".$itemAno['iigid']."]\" value='".$dados['anaid']."'>
                            			<input type=\"hidden\" name=\"qtd_monitorada[".$dados['inpid']."][".$itemAno['iniano']."][".$itemAno['iigid']."]\" value='".$qtdMonitorada."'>
                                            $camosEmenda
                    				<td><span class=\"btn btn-danger btn-sm glyphicon glyphicon-remove\" onclick=\"excluirItemReformulacao('".$itemAno['iigid']."', '".$dados['inpid']."', '".$itemAno['iniano']."');\"></span></td>
                    				<td id='nomereformular-{$itemAno['iigid']}' ".$styleNome.">".$itemAno['itcdsc']."</td>
                    				<td style='width: 150px;' id='nome-{$dados['inpid']}-{$itemAno['iniano']}-{$itemAno['iigid']}'>".$itemAno['unidsc']."</td>
                    				<td>".formata_valor($itemAno['valorreferencia'])."</td>
                    				<td>".$simec->input('qtd-item['.$dados['inpid']."][".$itemAno['iniano']."][".$itemAno['iigid'].']', null, $qtd, $attribs, array('input-size' => '12'))."</td>
                    				<td>".formata_valor($totalItem)."</td>
                    				<td>".$simec->input('qtd-item-pactuado['.$dados['inpid']."][".$itemAno['iniano']."][".$itemAno['iigid'].']', null, $vlrPactuado, $attribsPactuado, array('input-size' => '12'))."</td>
                    				<td>".formata_valor($totalItemPactuado)."</td>
                    				<td>".$simec->input('qtd-item-solicitada[' . $dados['inpid'].']['.$itemAno['iniano'].']['.$itemAno['iigid'].']', null, $reiquantidade, $attribRefor, array('input-size' => '12'))."</td>
                    				<td><div id='div_vlr_total_solic_".$dados['inpid'].'_'.$itemAno['iniano'].'_'.$itemAno['iigid']."' class='class_vlr_total_solic_".$dados['inpid'].'_'.$itemAno['iniano']."'>".formata_valor($totalItemSolic)."</div></td>
                                    ".$campos;
                if( $arrPermissao['analise']['visualizar'] == 'S' ){
                    $tabelaLinha .="<td ".$style.">".$simec->input('qtd-item-aprovado['.$dados['inpid'].']['.$itemAno['iniano'].']['.$itemAno['iigid'].']', null, ($itemAno['aicqtdaprovado'] ? $itemAno['aicqtdaprovado'] : '0'), $attribsAnalise, array('input-size' => '12'))."</td>
                                    <td ".$style."><div id='div_vlr_aprovado_".$dados['inpid'].'_'.$itemAno['iniano'].'_'.$itemAno['iigid']."'>".formata_valor($vlrTotalItens)."</div></td>";
                } else {
                    $tabelaLinha .='<input name="qtd-item-aprovado['.$dados['inpid'].']['.$itemAno['iniano'].']['.$itemAno['iigid'].']" id="qtd-item-aprovado['.$dados['inpid'].']['.$itemAno['iniano'].']['.$itemAno['iigid'].']" type="hidden" value="0" class="form-control"';
                }
                $tabelaLinha .="
                    			</tr>";

                $totalItemGeral = ($totalItemGeral + $totalItem);
                $totalItemPactuadoGeral = ($totalItemPactuadoGeral + $totalItemPactuado);
                $totalItemSolicitado = ($totalItemSolicitado + $totalItemSolic);
                $vlrTotalItensGeral = ($vlrTotalItensGeral + $vlrTotalItens);
            }
            if( $dados['pega_linha'] == 'S' ){
                return $tabelaLinha;
            }
            $tabela .= $tabelaLinha;
            $tabela .= '</tbody>
                        <tfoot><tr>
            				<td colspan=4><b>Totais:</b></td>
            				<td>&nbsp;</td>
            				<td>'.formata_valor($totalItemGeral).'</td>
            				<td>&nbsp;</td>
            				<td>'.formata_valor($totalItemPactuadoGeral).'</td>
                            <td>&nbsp;</td>
            				<td><div id="div_total_solicitado_'.$dados['inpid'].'_'.$dados['iniano'].'">'.formata_valor($totalItemSolicitado).'</div></td>';
            if ((int)$boEmenda > (int)0) {
                $tabela .= '<td '.$style.'>&nbsp;</td>
            				<td '.$style.'>'.formata_valor($totalItemEmendaGeral).'</td>';
            }
            if( $arrPermissao['analise']['visualizar'] == 'S' ){
            $tabela .= '    <td '.$style.'>&nbsp;</td>
            				<td '.$style.'>'.formata_valor($vlrTotalItensGeral).'</td>';
            }
            $tabela .= '</tr>';
        }
        $tabela .= '</tfoot>
                </table>';
        
        return $tabela;
    }
    
    public function montaSqlItemAnalise($iicg, $boEmenda, $dados)
    {
        $filtroEmenda = " AND a.intaid = 1 ";
        if ((int)$boEmenda > (int)0) {
            $filtroEmenda = " AND a.intaid = 2 ";
        }
        
        $inner = " inner join ";
        $filtro = "";
        if( $dados['pega_linha'] == 'S' ){
            $inner = " left join ";
            $filtro = " AND iicg.iigid = ".$dados['iigid_ref'];
        }
        
        if ($iicg['itcid']) {
            if( $dados['reformulacao'] == 'S' && $dados['pega_linha'] != 'S' ){
                $sql = "SELECT distinct ipc.ipiid, iicg.iigid, iicg.itcid, iicg.iigid, iia.iniano, itcdsc, unidsc, aic.aicqtdaprovado, ipc.reiquantidade as ipiquantidade,
                    	   ipc.reivalor as valorreferencia,
                            (SELECT max(itdvalor)
                            FROM par3.itenscomposicao_detalhamento id
                            WHERE 
                                id.itcid = ic.itcid
                                AND itdstatus = 'A'
                                AND itdsituacao = 'A'
                                AND (now() between itdperiodoinicio AND itdperiodofim)) vlr_detalhamento,
                            coalesce(iec.quantidade_emenda,0) as quantidade_emenda,
                        aic.qtd_total_aprovada, aic.total_aprovado, aic.raiid
                    FROM par3.iniciativa_planejamento ip
                    JOIN par3.iniciativa i USING (iniid)
                    JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                    JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                    inner join  par3.reformulacao_itemcomposicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.reistatus = 'A' and ipc.reiano = iia.iniano and ipc.refidpai = {$dados['refidpai']}
                    LEFT JOIN (
                    	SELECT DISTINCT a.anaid, a.anaano, a.inpid, ai.raiid, ai.reiid, ai.ipiid, ai.raiqtdaprovado as aicqtdaprovado, ai.raivaloraprovado as aicvaloraprovado, toa.qtd_total_aprovada, toa.total_aprovado
                        FROM par3.analise a
                        	INNER JOIN par3.reformulacao_analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.raistatus = 'A'
                        	LEFT JOIN(
                        		SELECT reiid, aicqtdaprovado AS qtd_total_aprovada, (aicqtdaprovado * aicvaloraprovado) AS total_aprovado FROM(
                        			SELECT it.reiid, sum(it.raiqtdaprovado) AS aicqtdaprovado, it.raivaloraprovado as aicvaloraprovado
                                    FROM par3.reformulacao_analise_itemcomposicao it
                                        INNER JOIN par3.analise a ON a.anaid = it.anaid AND a.anastatus = 'A'
                        			WHERE it.raistatus = 'A' $filtroEmenda  AND a.anaano = {$dados['iniano']} AND a.inpid = {$dados['inpid']} AND it.refid = {$dados['refid']}
                        			GROUP BY it.raivaloraprovado, it.reiid
                        		) AS foo
                        	) toa ON toa.reiid = ai.reiid
						WHERE a.anastatus = 'A' AND a.inpid = {$dados['inpid']} AND a.anaid = {$dados['anaid']} AND ai.refid = {$dados['refid']}
                    ) aic ON aic.reiid = ipc.reiid
                    LEFT JOIN(
                    	SELECT inpid, ipiid, sum(ieiquantidade) AS quantidade_emenda FROM (
                        	SELECT distinct ie.inpid, eic.ipiid, eic.ieiquantidade
                        	FROM par3.iniciativa_emenda ie
                        		INNER JOIN par3.iniciativa_emenda_item_composicao eic ON eic.ineid = ie.ineid
                        		INNER JOIN par3.analise a ON a.edeid = ie.edeid AND a.anastatus = 'A'
                        	WHERE ie.inestatus = 'A' AND eic.ieistatus = 'A'
                        		AND ie.inpid = {$dados['inpid']} AND a.anaid = {$dados['anaid']}
                        ) AS foo
                        GROUP BY inpid, ipiid
                    ) iec ON iec.inpid = ip.inpid AND ipc.ipiid = iec.ipiid
                    JOIN par3.itenscomposicao ic USING (itcid)
                    JOIN par3.unidade_medida um USING (uniid)
                    WHERE ip.inpid = {$dados['inpid']} and iia.iniano = '{$dados['iniano']}' AND iia.iianostatus = 'A' $filtro
                    ORDER BY itcdsc;";
            } else {
                $sql = "SELECT distinct ipc.ipiid, iicg.iigid, iicg.itcid, iicg.iigid, iia.iniano, itcdsc, unidsc, aic.aicaprovado, aic.aicqtdaprovado, ipc.ipiquantidade,
            			case when ipc.ipivalorreferencia is not null then ipc.ipivalorreferencia
							ELSE (SELECT max(itdvalor)
                            FROM par3.itenscomposicao_detalhamento id
                            WHERE id.itcid = ic.itcid
                            AND itdstatus = 'A'
                            AND itdsituacao = 'A'
                            AND (now() between itdperiodoinicio AND itdperiodofim OR itdperiodofim < now())
                            ) END as valorreferencia,
                        (SELECT max(itdvalor)
                            FROM par3.itenscomposicao_detalhamento id
                            WHERE id.itcid = ic.itcid
                                AND itdstatus = 'A'
                                AND itdsituacao = 'A'
                                AND (now() between itdperiodoinicio AND itdperiodofim)) vlr_detalhamento,
                        coalesce(iec.quantidade_emenda,0) as quantidade_emenda,
                        aic.qtd_total_aprovada, aic.total_aprovado
                    FROM par3.iniciativa_planejamento ip
                    JOIN par3.iniciativa i USING (iniid)
                    JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                    JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                    $inner par3.iniciativa_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = iia.iniano
                    LEFT JOIN (
                    	SELECT a.anaid, a.anaano, a.inpid, ai.ipiid, ai.aicqtdaprovado, ai.aicvaloraprovado, ai.aicaprovado, toa.qtd_total_aprovada, toa.total_aprovado
                        FROM par3.analise a
                        	INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.aicstatus = 'A'
                        	INNER JOIN(
                        		SELECT ipiid, aicqtdaprovado AS qtd_total_aprovada, (aicqtdaprovado * aicvaloraprovado) AS total_aprovado FROM(
                        			SELECT it.ipiid, sum(it.aicqtdaprovado) AS aicqtdaprovado, it.aicvaloraprovado
                                    FROM par3.analise_itemcomposicao it
                                        INNER JOIN par3.analise a ON a.anaid = it.anaid AND a.anastatus = 'A'
                        			WHERE it.aicstatus = 'A' $filtroEmenda AND a.anaano = {$dados['iniano']} GROUP BY it.aicvaloraprovado, it.ipiid
                        		) AS foo
                        	) toa ON toa.ipiid = ai.ipiid
						WHERE a.anastatus = 'A' AND a.inpid = {$dados['inpid']} AND a.anaid = {$dados['anaid']}
                    ) aic ON aic.ipiid = ipc.ipiid
                    LEFT JOIN(
                    	SELECT inpid, ipiid, sum(ieiquantidade) AS quantidade_emenda FROM (
                        	SELECT distinct ie.inpid, eic.ipiid, eic.ieiquantidade
                        	FROM par3.iniciativa_emenda ie
                        		INNER JOIN par3.iniciativa_emenda_item_composicao eic ON eic.ineid = ie.ineid
                        		INNER JOIN par3.analise a ON a.edeid = ie.edeid AND a.anastatus = 'A'
                        	WHERE ie.inestatus = 'A' AND eic.ieistatus = 'A'
                        		AND ie.inpid = {$dados['inpid']} AND a.anaid = {$dados['anaid']}
                        ) AS foo
                        GROUP BY inpid, ipiid
                    ) iec ON iec.inpid = ip.inpid AND ipc.ipiid = iec.ipiid
                    JOIN par3.itenscomposicao ic USING (itcid)
                    JOIN par3.unidade_medida um USING (uniid)
                    WHERE ip.inpid = {$dados['inpid']} and iia.iniano = '{$dados['iniano']}' AND iia.iianostatus = 'A' $filtro
                    ORDER BY itcdsc;";
            }
        } elseif ($iicg['igrid']) {
            if( $dados['reformulacao'] == 'S' && $dados['pega_linha'] != 'S' ){
                $sql = "SELECT distinct ipc.ipiid, iicg.iigid, iicg.itcid, iicg.igrid, iia.iniano, igrnome as itcdsc, '-' as unidsc, aic.aicqtdaprovado, ipc.reiquantidade as ipiquantidade,
						ipc.reivalor as valorreferencia,
                        sum(icgi.gitqtd * (SELECT max(itdvalor)
										    FROM par3.itenscomposicao_detalhamento id
										    WHERE icgi.itcid = id.itcid
										    AND itdstatus = 'A'
										    AND itdsituacao = 'A'
										    AND (now() between itdperiodoinicio AND itdperiodofim))) vlr_detalhamento,
                        coalesce(iec.quantidade_emenda,0) as quantidade_emenda,
                        aic.qtd_total_aprovada, aic.total_aprovado, aic.raiid
                    FROM par3.iniciativa_planejamento ip
                    JOIN par3.iniciativa i USING (iniid)
                    JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                    JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                    JOIN par3.itenscomposicao_grupos icg USING (igrid)
                    JOIN par3.itenscomposicao_grupos_itens icgi USING (igrid)
                    inner join par3.reformulacao_itemcomposicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.reistatus = 'A' and ipc.reiano = iia.iniano and ipc.refidpai = {$dados['refidpai']}
                    LEFT JOIN (
                    	SELECT DISTINCT a.anaid, a.anaano, a.inpid, ai.raiid, ai.reiid, ai.ipiid, ai.raiqtdaprovado as aicqtdaprovado, ai.raivaloraprovado as aicvaloraprovado, toa.qtd_total_aprovada, toa.total_aprovado
                        FROM par3.analise a
                        	INNER JOIN par3.reformulacao_analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.raistatus = 'A'
                        	LEFT JOIN(
                        		SELECT reiid, aicqtdaprovado AS qtd_total_aprovada, (aicqtdaprovado * aicvaloraprovado) AS total_aprovado FROM(
                        			SELECT it.reiid, sum(it.raiqtdaprovado) AS aicqtdaprovado, it.raivaloraprovado as aicvaloraprovado
                                    FROM par3.reformulacao_analise_itemcomposicao it
                                        INNER JOIN par3.analise a ON a.anaid = it.anaid AND a.anastatus = 'A'
                        			WHERE it.raistatus = 'A' $filtroEmenda  AND a.anaano = {$dados['iniano']} AND a.inpid = {$dados['inpid']} AND it.refid = {$dados['refid']}
                        			GROUP BY it.raivaloraprovado, it.reiid
                        		) AS foo
                        	) toa ON toa.reiid = ai.reiid
						WHERE a.anastatus = 'A' AND a.inpid = {$dados['inpid']} AND a.anaid = {$dados['anaid']} AND ai.refid = {$dados['refid']}
                    ) aic ON aic.reiid = ipc.reiid
                    LEFT JOIN(
                    	SELECT inpid, ipiid, sum(ieiquantidade) AS quantidade_emenda FROM (
                        	SELECT distinct ie.inpid, eic.ipiid, eic.ieiquantidade
                        	FROM par3.iniciativa_emenda ie
                        		INNER JOIN par3.iniciativa_emenda_item_composicao eic ON eic.ineid = ie.ineid
                        		INNER JOIN par3.analise a ON a.edeid = ie.edeid AND a.anastatus = 'A'
                        	WHERE ie.inestatus = 'A' AND eic.ieistatus = 'A'
                        		AND ie.inpid = {$dados['inpid']} AND a.anaid = {$dados['anaid']}
                        ) AS foo
                        GROUP BY inpid, ipiid
                    ) iec ON iec.inpid = ip.inpid AND ipc.ipiid = iec.ipiid
                    WHERE ip.inpid = {$dados['inpid']} and iia.iniano = '{$dados['iniano']}' AND iia.iianostatus = 'A' $filtro
                    GROUP BY ipc.ipiid, iicg.iigid, iia.iniano, aic.raiid, igrnome, ipc.reivalor, ipc.reiquantidade, aic.aicqtdaprovado, iec.quantidade_emenda, aic.qtd_total_aprovada, aic.total_aprovado
                    ORDER BY igrnome;";
            } else {
                $sql = "SELECT distinct ipc.ipiid, iicg.iigid, iicg.itcid, iicg.igrid, iia.iniano, igrnome as itcdsc, '-' as unidsc, aic.aicaprovado, aic.aicqtdaprovado, ipc.ipiquantidade,
						case when ipc.ipivalorreferencia is not null then ipc.ipivalorreferencia
							else
								sum(icgi.gitqtd * (SELECT max(itdvalor)
										    FROM par3.itenscomposicao_detalhamento id
										    WHERE icgi.itcid = id.itcid
										    AND itdstatus = 'A'
										    AND itdsituacao = 'A'
										    AND (now() between itdperiodoinicio AND itdperiodofim OR itdperiodofim < now()))) end valorreferencia,
                        (SELECT max(itdvalor)
                            FROM par3.itenscomposicao_detalhamento id
                            WHERE id.itcid = iicg.itcid
                                AND itdstatus = 'A'
                                AND itdsituacao = 'A'
                                AND (now() between itdperiodoinicio AND itdperiodofim)) vlr_detalhamento,
                        coalesce(iec.quantidade_emenda,0) as quantidade_emenda,
                        aic.qtd_total_aprovada, aic.total_aprovado
                    FROM par3.iniciativa_planejamento ip
                    JOIN par3.iniciativa i USING (iniid)
                    JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                    JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                    JOIN par3.itenscomposicao_grupos icg USING (igrid)
                    JOIN par3.itenscomposicao_grupos_itens icgi USING (igrid)
                    $inner par3.iniciativa_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = iia.iniano
                    LEFT JOIN (
                    	SELECT a.anaid, a.anaano, a.inpid, ai.ipiid, ai.aicqtdaprovado, ai.aicvaloraprovado, ai.aicaprovado, toa.qtd_total_aprovada, toa.total_aprovado
                        FROM par3.analise a
                        	INNER JOIN par3.analise_itemcomposicao ai ON ai.anaid = a.anaid AND ai.aicstatus = 'A'
                        	INNER JOIN(
                        		SELECT ipiid, aicqtdaprovado AS qtd_total_aprovada, (aicqtdaprovado * aicvaloraprovado) AS total_aprovado FROM(
                        			SELECT it.ipiid, sum(it.aicqtdaprovado) AS aicqtdaprovado, it.aicvaloraprovado
                                    FROM par3.analise_itemcomposicao it
                                        INNER JOIN par3.analise a ON a.anaid = it.anaid AND a.anastatus = 'A'
                        			WHERE it.aicstatus = 'A' $filtroEmenda AND a.anaano = {$dados['iniano']} GROUP BY it.aicvaloraprovado, it.ipiid
                        		) AS foo
                        	) toa ON toa.ipiid = ai.ipiid
						WHERE a.anastatus = 'A' AND a.inpid = {$dados['inpid']} AND a.anaid = {$dados['anaid']}
                    ) aic ON aic.ipiid = ipc.ipiid
                    LEFT JOIN(
                    	SELECT inpid, ipiid, sum(ieiquantidade) AS quantidade_emenda FROM (
                        	SELECT distinct ie.inpid, eic.ipiid, eic.ieiquantidade
                        	FROM par3.iniciativa_emenda ie
                        		INNER JOIN par3.iniciativa_emenda_item_composicao eic ON eic.ineid = ie.ineid
                        		INNER JOIN par3.analise a ON a.edeid = ie.edeid AND a.anastatus = 'A'
                        	WHERE ie.inestatus = 'A' AND eic.ieistatus = 'A'
                        		AND ie.inpid = {$dados['inpid']} AND a.anaid = {$dados['anaid']}
                        ) AS foo
                        GROUP BY inpid, ipiid
                    ) iec ON iec.inpid = ip.inpid AND ipc.ipiid = iec.ipiid
                    WHERE ip.inpid = {$dados['inpid']} and iia.iniano = '{$dados['iniano']}' AND iia.iianostatus = 'A' $filtro
                    GROUP BY ipc.ipiid, iicg.iigid, iia.iniano, igrnome, ipc.ipivalorreferencia, aic.aicaprovado, aic.aicqtdaprovado, iec.quantidade_emenda, aic.qtd_total_aprovada, aic.total_aprovado
                    ORDER BY igrnome;";
            }
        }
        
        return $sql;
    }
    
    public function buscaObjeto($inpid)
    {

        global $db;
        $result = pg_fetch_array(($db->executar("SELECT DISTINCT inito.intoid
           FROM par3.iniciativa as ini
           INNER JOIN   par3.iniciativa_planejamento inp ON  inp.iniid = ini.iniid
           LEFT JOIN par3.iniciativa_tipos_objeto as inito ON inito.intoid = ini.intoid
           WHERE ini.inistatus = 'A' AND inpid = $inpid")));

        return $result[0];
    }

    public function excluirItem()
    {
        global $db;

        $sql  = "UPDATE par3.iniciativa_planejamento_item_composicao SET ipistatus = 'I' where inpid = ".$_REQUEST['inpid']." and iigid = ".$_REQUEST['iigid']."";

        $resultItem = $db->executar($sql);
        $db->commit();

        return $resultItem;
    }
}
