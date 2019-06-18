<?php
$inuid = $_REQUEST['inuid'];
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
require_once APPRAIZ . '/includes/workflow.php';

$obReformulacao = new Par3_Controller_Reformulacao();

switch ($_REQUEST['requisicao']) {
    case 'buscaModalEscolas':
        ob_clean();
        $inpice = new Par3_Controller_IniciativaPlanejamentoItemComposicaoEscola();
        //if( $_REQUEST['reformulacao'] == 'S' ){
            $resp = $inpice->getFormEscolasReformulacao($_REQUEST);
        /*} else {
            $_REQUEST['aba'] = 'analisarPlanejamento';
            $resp = $inpice->getFormEscolas($_REQUEST);
        }*/
        echo $resp;
    die;
    break;
    case 'salvarDocumento':
        ob_clean();
        $obReformulacao->salvarDocumentoReformulacao($_POST);
        die;
    break;
    case 'buscaLinhaItemComposicao':
        ob_clean();
        
        $_REQUEST['pega_linha'] = 'S';
        $_REQUEST['reformulacao'] = 'S';
        $_REQUEST['dotstatus'] = 'A';
        $inpic = new Par3_Controller_IniciativaPlanejamentoItemComposicao();
        $resp = $inpic->getPlanilhaAnalise($_REQUEST);
        
        echo $resp;
    die;
    break;
    case 'download':
        ob_clean();
        $file = new FilesSimec();
        $arqid = $_REQUEST['arqid'];
        $arquivo = $file->getDownloadArquivo($arqid);
        echo '<script type="text/javascript">
                window.location.href="par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid='.$_REQUEST['dotid'].'&inuid='.$_REQUEST['inuid'].'&proid='.$_REQUEST['proid'].'&refid='.$_REQUEST['refid'].'";
                </script>';
        exit;
    die;
    break;
    case 'delarquivo':
        ob_clean();
        $sql = "UPDATE par3.reformulacao_anexo SET axrstatus = 'I' where axrid = ".$_REQUEST['axrid'];
        $db->executar($sql);
        $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid = ".$_REQUEST['arqid'];
        $db->executar($sql);
        $db->commit();
        
        $file = new FilesSimec();
        $file->excluiArquivoFisico($_REQUEST['arqid']);
        
        echo '<script type="text/javascript">
        		alert("Operação realizada com sucesso!");
        		window.location.href="par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid='.$_REQUEST['dotid'].'&inuid='.$_REQUEST['inuid'].'&proid='.$_REQUEST['proid'].'&refid='.$_REQUEST['refid'].'";
        	  </script>';
    die;
    break;
    case 'salvar-reformulacao':
        ob_clean();
        if( in_array($_REQUEST['esdid'], array(PAR3_REFORMULACAO_ESDID_EM_ANALISE, PAR3_REFORMULACAO_ESDID_AGUARDANDO_ANALISE, PAR3_REFORMULACAO_ESDID_EM_ANALISE_RETORNO_DILIGENCIA)) ){
            $refid = $obReformulacao->salvarAnaliseReformulacao($_REQUEST);
            $msg = "Análise da Reprogramação salva com Sucesso!";
        } else {
            $refid = $obReformulacao->salvarSolicitacao($_REQUEST);
            $msg = "Reprogramação de iniciativa salva com sucesso! É necessário clicar em Enviar para análise para que a solicitação seja analisada pelo FNDE.";
        }
        
        simec_redirecionar('par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid='.$_REQUEST['dotid'].'&inuid='.$_REQUEST['inuid'].'&proid='.$_REQUEST['proid'].'&refid='.$_REQUEST['refid'], 'success', $msg);
    die;
    break;
    case 'qtd-monitorada-item':
        ob_clean();
        echo $obReformulacao->verificaQtdMonitoradaItem($_REQUEST['ipiid'], $_REQUEST['iigid']);
    die;
    break;
    case 'salvarItemComposicaoEscolas':
        ob_clean();
        
        $inpice = new Par3_Controller_IniciativaPlanejamentoItemComposicaoEscola();
        $resp = $inpice->salvarItemEscolaReformulacao($_REQUEST);
        echo $resp;
    die;
    break;
    case 'atualizarValorItemComposicao':
        ob_clean();
        $controlleINC = new Par3_Controller_IniciativaPlanejamentoItemComposicaoEscola();
        echo $controlleINC->atualizarValorItemComposicao($_REQUEST);
    die;
    break;
}

require APPRAIZ.'includes/cabecalho.inc';

$arrReformulacao = $obReformulacao->pegaReformulacaoDocumento($_REQUEST['dotid'], $_REQUEST['refid'], PAR3_REFORMULACAO_INICIATIVA);

if( $_REQUEST['gerar'] == 'S'){
    if( empty($arrReformulacao['refid']) ){
        $refid = $obReformulacao->gerarBackup($_REQUEST['dotid'], $_REQUEST['proid'], PAR3_REFORMULACAO_INICIATIVA);
        $arrReformulacao = $obReformulacao->pegaReformulacaoDocumento($_REQUEST['dotid'], $refid, PAR3_REFORMULACAO_INICIATIVA);
        $arEstado = $obReformulacao->carregaEsdidAtual($arrReformulacao['docid']);
    } else {
        $arEstado = $obReformulacao->carregaEsdidAtual($arrReformulacao['docid']);
        if( in_array($arEstado['esdid'], array(PAR3_REFORMULACAO_ESDID_REPROGRAMACAO_FINALIZADA, PAR3_REFORMULACAO_ESDID_REFORMULACAO_CANCELADA, PAR3_REFORMULACAO_ESDID_REFORMULACAO_REPROVADA)) ){
            $refid = $obReformulacao->gerarBackup($_REQUEST['dotid'], $_REQUEST['proid'], PAR3_REFORMULACAO_INICIATIVA);
            $arrReformulacao = $obReformulacao->pegaReformulacaoDocumento($_REQUEST['dotid'], $refid, PAR3_REFORMULACAO_INICIATIVA);
            $arEstado = $obReformulacao->carregaEsdidAtual($arrReformulacao['docid']);
        } else {
            $arrReformulacao = $obReformulacao->pegaReformulacaoDocumento($_REQUEST['dotid'], $refid, PAR3_REFORMULACAO_INICIATIVA);
            $arEstado = $obReformulacao->carregaEsdidAtual($arrReformulacao['docid']);
        }
    }
} else {
    $arrReformulacao = $obReformulacao->pegaReformulacaoDocumento($_REQUEST['dotid'], $_REQUEST['refid'], PAR3_REFORMULACAO_INICIATIVA);
    $arEstado = $obReformulacao->carregaEsdidAtual($arrReformulacao['docid']);
}

$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);

$arrPermissao = $obReformulacao->verificaPermissaoTela($arEstado['esdid'], $arrReformulacao['refid']);

//getDadosUnidade
$entidade = $modelInstrumentoUnidade->inudescricao;
$estuf = $modelInstrumentoUnidade->estuf;
$muncod = $modelInstrumentoUnidade->muncod;
$itrid = $modelInstrumentoUnidade->itrid;
?>
<style>
.panel-heading {
    background-color: #eee;
}

.panel-heading:hover {
    background-color: #ccc;
}

.collapsed:after {
    content: '\02795'; /* Unicode character for "plus" sign (+) */
    font-size: 13px;
    color: #777;
    float: right;
    margin-left: 5px;
}
.active_coll:after {
    content: "\2796"; /* Unicode character for "minus" sign (-) */
    font-size: 13px;
    color: #777;
    float: right;
    margin-left: 5px;
}


</style>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
            </div>
        </div>
    </div>
    <div class="ibox-content">
    <?php
        if (!$_REQUEST['requisicao']&&!$_REQUEST['req']) {
            $controleUnidade->cabecalhoUnidade();
        }?>
	</div>
</div>
<?php
echo '<br>';
if( $_REQUEST['refid'] ){
    $url = "par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid={$_REQUEST['dotid']}&inuid={$_REQUEST['inuid']}&proid={$_REQUEST['proid']}&refid={$_REQUEST['refid']}";
} else {
    $url = "par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid={$_REQUEST['dotid']}&inuid={$_REQUEST['inuid']}&proid={$_REQUEST['proid']}";
}
echo $simec->tab ( criaAbaReformulacao($_REQUEST['inuid'], 2, $_REQUEST['refid']), $url );

$obProcesso = new Par3_Controller_Processo();

?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="form-group" style="text-align: center;">
				<label class="col-md-12 control-label" style="font-size: 18px; font-weight: bold; text-align: center;">Solicitar Reprogramação</label>
			</div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="col-md-12">
                <?php
                $emecod = 'N';
                if ( $obProcesso->verificaProcessoEmenda($_REQUEST['proid']) == 'S') {
                    $edeid = $db->pegaUm("SELECT a.edeid FROM par3.analise a
        						INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = a.inpid AND pp.ppcstatus = 'A'
        					WHERE a.anastatus = 'A'
        						AND pp.proid = {$_REQUEST['proid']}");
                    $obEmenda = new Par3_Controller_EmendasParlamentares();
                    echo $obEmenda->pegaBeneficiarioSelecionado($edeid, $_REQUEST['inpid'], $_REQUEST['inuid']);
                    $emecod = 'S';
                } ?>
            </div>
        </div>
        <div class="row">
            <div class="col-md-12">
                <?php
                if ( $_REQUEST['proid'] ) {
                    echo $obProcesso->carregaDadosOrcamentario($_REQUEST['inuid'], $_REQUEST['inpid'], $_REQUEST['anaid'], $_REQUEST['proid']);
                } ?>
            </div>
        </div>
        <form name="form-solicita-reformulacao" id="form-solicita-reformulacao" class="form-horizontal" method="post" enctype="multipart/form-data">
			<input type="hidden" name="requisicao" id="requisicao" value="">
        	<input type="hidden" name="emecod" value="<?php echo $emecod; ?>"/>
        	<input type="hidden" name="refid" value="<?php echo $arrReformulacao['refid']; ?>"/>
        	<input type="hidden" name="esdid" value="<?php echo $arEstado['esdid']; ?>"/>
        	<input type="hidden" name="refidpai" value="<?php echo $arrReformulacao['refidpai']; ?>"/>
        	<input type="hidden" name="inuid" value="<?php echo $_REQUEST['inuid']; ?>"/>
        	<input type="hidden" name="dotid" value="<?php echo $_REQUEST['dotid']; ?>"/>
        	<input type="hidden" name="proid" value="<?php echo $_REQUEST['proid']; ?>"/>
        	<input type="hidden" name="tirid" value="2"/>
            <div class="row">
            	<div class="col-md-12">
        		<?php echo $obReformulacao->listaAccordionIniciativa($_REQUEST['dotid'], $_REQUEST['proid']); ?>
        		</div>
        	</div>
            <div class="ibox-content">
                <div class="row">
                	<div class="form-group">
                        <label class="col-lg-2 control-label">Arquivo (pdf)</label>
<?php   if( $arrPermissao['cadastro']['salvar'] == 'S' ){?>
                        <div class="col-lg-10">
                            <button type="button" id="add_field" class="btn btn-success"><i class="fa fa-plus-square-o"></i> Adicionar Anexo</button>
                        </div>
<?php   }?>
                        <!-- Mostrar arquivo de contrato anexado ao editar (preenchido via javascript) -->
                        <div class="row">
                        	<label class="col-lg-2 control-label">&nbsp;</label>
                        	<div class="col-lg-6">
                            	<div id="div-arquivos">
                                <?php
                                    echo $obReformulacao->getListaArquivosReformulacao($arrReformulacao['refid'], $arEstado['esdid']);
                                ?>
                                </div>
                        	</div>
                        </div>
                    </div>
                </div>
                <div class="row">
                <?php
                if( $arrPermissao['cadastro']['salvar'] == 'S' ){
                    $attribusJust = array('maxlengh' => 4000);
                } else {
                    $attribusJust = array('maxlengh' => 4000, 'disabled' => 'disabled');
                }
                
                ?>
                <?php echo $simec->textarea('refjustificativa', 'Justificativa da Reprogramação*', $arrReformulacao['refjustificativa'], $attribusJust, array('label-size' => '2')); ?>
    			</div>
            </div>
            <table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%">
            <tr>
            <td style="width: 100%">
            <?php
            $valorTermo = $db->pegaUm("SELECT dotvalortermo FROM par3.documentotermo WHERE dotid = {$_REQUEST['dotid']}");
            $valorReformulado = $db->pegaUm("SELECT sum(ri.reiquantidade * ri.reivalor)
                                                            FROM par3.reformulacao_documento rd
                                                            	INNER JOIN par3.reformulacao_itemcomposicao ri ON ri.refid = rd.refid AND ri.reistatus = 'A'
                                                            WHERE rd.dotid = {$_REQUEST['dotid']}
                                                            	AND ri.refid = {$arrReformulacao['refid']}
                                                            	AND rd.refstatus = 'A'");
            
            if( $arrPermissao['analise']['visualizar'] == 'S' ){
                $sql = "SELECT sum(ai.raiqtdaprovado * ai.raivaloraprovado)
                                                FROM par3.reformulacao_documento rd
                                                	INNER JOIN par3.reformulacao_analise_itemcomposicao ai ON ai.refid = rd.refid AND ai.raistatus = 'A'
                                                WHERE rd.dotid = {$_REQUEST['dotid']}
                                                	AND ai.refid = {$arrReformulacao['refid']}
                                                	AND rd.refstatus = 'A'";

                $valorArovado = $db->pegaUm($sql);
            } else {
                $valorArovado = 0;
            }
            ?>
           <div class="ibox-content">
                <div class="row">
                	<div class="col-md-12">
                        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem" id="tb_render" data-qtd-acoes="0">
                        <tbody>
                        	<tr>
                        		<td style="width: 40%;">Valor do termo:</td>
                        		<td style="text-align: left;">R$ <?php echo number_format($valorTermo, 2, ",", "."); ?>
                        		<input type="hidden" name="valor_termo" value="<?php echo $valorTermo;?>">
                        		<input type="hidden" name="valor_reformulado" value="<?php echo $valorReformulado;?>">
                        		</td>
                        	</tr>
                        	<tr>
                        		<td>Valor reprogramado:</td>
                        		<td style="text-align: left;"><div id="div_valor_reprogramado">R$ <?php echo number_format($valorReformulado, 2, ",", "."); ?></div></td>
                        	</tr>
                        	<tr>
                        		<td>Complemento da solicitação:</td>
                        		<td style="text-align: left;">
                                    <div id="div_complementacao_solicitacao">R$ <?php
                            		if( $valorReformulado < $valorTermo ) {
                            		    echo number_format(0, 2, ",", ".");
                            		} else {
                            		    $vlrComplementacaoSolicitacao = $valorReformulado - $valorTermo;
                            		    echo number_format(($vlrComplementacaoSolicitacao), 2, ",", ".");
                            		    if($vlrComplementacaoSolicitacao > 0) {

                                        }
                            		}?>
                                    </div>
                                    <span  style="display: <?= ($vlrComplementacaoSolicitacao == 0? 'none':'')?>;" class="danger" size="2"><strong><sup> <i> * Haverá contrapartida da entidade.</i></sup></strong></span>
                                </td>
                        	</tr>
                        <?php if( $arrPermissao['analise']['visualizar'] == 'S' ){?>
                            <tr>
                        		<td>Complementação Aprovação:</td>
                        		<td style="text-align: left;"><div id="div_complementacao_aprovacao">R$ <?php
                        		   if( $valorArovado < $valorTermo ){
                            		    echo number_format(0, 2, ",", ".");
                            		} else {
                            		    echo number_format(($valorArovado - $valorTermo), 2, ",", ".");
                            		}?></div></td>
                        	</tr>
                        <?php }?>
                        </tbody>
                        </table>
                    </div>
                </div>
			</div>
			<?php
			$disabledEntidade = 'disabled';
			$disabledRaf      = 'disabled';
			$disabledComple   = 'disabled';
			
			if( $arrPermissao['cadastro']['salvar'] == 'S' || $arrPermissao['analise']['salvar'] == 'S' ){
			    $disabledEntidade = '';
			    $disabledRaf      = '';
			}
			if( $arrPermissao['analise']['salvar'] == 'S' ){
			    $disabledComple = '';
			}
			
			if( $valorArovado > 0 ){
			     $empenhoComplementar = ($arrReformulacao['refempenhocomplementar'] > 0 ? $arrReformulacao['refempenhocomplementar'] : (($valorArovado - $valorTermo) - ($arrReformulacao['refvalorentidade'] + $arrReformulacao['refrafmecfnde'])) );
			} else {
			    $empenhoComplementar = ($arrReformulacao['refempenhocomplementar'] > 0 ? $arrReformulacao['refempenhocomplementar'] : 0);
			}
			?>
           <div class="ibox-content">
                <div class="row">
                	<div class="col-md-12">
                        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem" id="tb_render" data-qtd-acoes="0">
                        <thead>
                        	<tr>
                        		<th style="width: 40%">Origem</th>
                        		<th style="width: 60%">Valor Complementar</th>
                        	</tr>
                        </thead>
                        <tbody>
<?php                   if( $arrPermissao['analise']['visualizar'] == 'S' ){?>
                        	<tr>
                        		<td>Empenho complementar (MEC/FNDE):</td>
                        		<td><input name="refempenhocomplementar" id="refempenhocomplementar" type="text" value="<?php echo number_format($empenhoComplementar, 2, ",", "."); ?>" <?php echo $disabledComple;?> class="form-control monetario-origem-complementar" style="width: 300px"></td>
                        	</tr>
<?php                  } else {?>
						<input name="refempenhocomplementar" id="refempenhocomplementar" type="hidden" value="0" class="form-control">
<?php                  }?>
                        	<tr>
                        		<td><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?>:</td>
                        		<td><input name="refvalorentidade" id="refvalorentidade" type="text" value="<?php echo number_format($arrReformulacao['refvalorentidade'], 2, ",", "."); ?>" <?php echo $disabledEntidade;?> class="form-control monetario-origem-complementar" style="width: 300px">
                        		</td>
                        	</tr>
                        	<tr>
                        		<td>RAF (MEC/FNDE): <span style="color:#428bca;cursor:pointer" class="glyphicon glyphicon-info-sign" title="Verifique a existência de aplicações da conta corrente específica do Termo de Compromisso"></span></td>
                        		<td><input name="refrafmecfnde" id="refrafmecfnde" type="text" value="<?php echo number_format($arrReformulacao['refrafmecfnde'], 2, ",", "."); ?>" <?php echo $disabledRaf;?> class="form-control monetario-origem-complementar" style="width: 300px"></td>
                        	</tr>
                        	<tr>
                        		<td>Total:</td>
                        		<td style="text-align: left;"><div id="total_div"><?php echo number_format(($arrReformulacao['refempenhocomplementar'] + $arrReformulacao['refrafmecfnde'] + $arrReformulacao['refvalorentidade']), 2, ",", "."); ?></div></td>
                        	</tr>
                        </tbody>
                        </table>
                    </div>
                </div>
			</div>
<?php   if( $arrPermissao['analise']['visualizar'] == 'S' ){?>
    	<div class="ibox-content">
            <div class="row">
                <div class="form-group" style="text-align: center;">
                    <label class="col-md-12 control-label" style="font-size: 18px; font-weight: bold; text-align: center;">Parecer da Reprogramação</label>
                </div>
            </div>
        </div>
    	<div class="row">
    		<div class="col-md-12">
            <?php

            if( $arrPermissao['analise']['visualizar'] == 'S' && $arrPermissao['analise']['salvar'] == 'N' ){
                $attribus = array('maxlengh' => 4000, 'readonly' => 'readonly');
                $disabledText = 'disabled';
            } else {
                $attribus = array('maxlengh' => 4000);
                $disabledText = '';
            }
            
            echo $simec->textarea('refconsideracaoent', 'Considerações sobre a Entidade Proponente', $arrReformulacao['refconsideracaoent'], $attribus, array('label-size' => '2'));
            echo $simec->textarea('refconsideracaoprop', 'Considerações sobre a Proposta', $arrReformulacao['refconsideracaoprop'], $attribus, array('label-size' => '2'));
            echo $simec->textarea('refconsideracaoproj', 'Considerações sobre o Projeto', $arrReformulacao['refconsideracaoproj'], $attribus, array('label-size' => '2'));
            echo $simec->textarea('refconsideracaoobj', 'Considerações sobre o Objetivo', $arrReformulacao['refconsideracaoobj'], $attribus, array('label-size' => '2'));
            echo $simec->textarea('refconsideracaojus', 'Considerações sobre a Justificativa', $arrReformulacao['refconsideracaojus'], $attribus, array('label-size' => '2'));
            echo $simec->textarea('refconsideracaoval', 'Considerações sobre os Valores', $arrReformulacao['refconsideracaoval'], $attribus, array('label-size' => '2'));
            echo $simec->textarea('refoutrasconsideracoes', 'Outras Considerações Cabíveis', $arrReformulacao['refoutrasconsideracoes'], $attribus, array('label-size' => '2'));
            ?>
            <div class="form-group ">
                <label for="" class="col-sm-2 col-md-2 col-lg-2 control-label">Texto do parecer*: </label>
                <div class="col-sm-10 col-md-10 col-lg-10 ">
                    <textarea name="reftextoparecer" id="reftextoparecer" type="text" class="form-control" maxlengh="4000" <?php echo $disabledText; ?> label-for="reftextoparecer"
                              placeholder="Considerando o exposto na Lei nº 12.695/2012, que dispõe sobre o apoio técnico ou financeiro da União no âmbito do Plano de Ações Articuladas, após análise da solicitação, esta Coordenação posiciona-se favorável a pactuação por Termo de Compromisso e ao repasse de recursos, aprovando a iniciativa proposta pelo ente governamental.">
                              <?=trim($arrReformulacao['reftextoparecer']); ?></textarea>
                </div>
                <div style="clear:both"></div>
            </div>
            <br>
        	</div>
		</div>
<?php   } ?>
		<div class="row">
    		<div style="color: red;">Os campos com (*) são de preenchimento obrigatório</div>
    	</div>
		<div class="ibox float-e-margins"></div>
    <div class="ibox-content" >
<?php if( $arrPermissao['cadastro']['salvar'] == 'S' || $arrPermissao['analise']['salvar'] == 'S'  ){?>
    	<div class="row" style="text-align: center;">
        	<button type="button" id="btn-salvar-reformulacao" data-dismiss="modal" class="btn btn-success"><i class="fa fa-save""></i> Salvar</button>
        </div>
<?php }?>
    </div>
    <div class="ibox float-e-margins"> </div>
    </form>
    </td>
        <?php
        /**
         * situação ?Reprogramação aprovada?:
         * caso o valor ?Empenho complementar (MEC/FNDE)? seja maior que zero,
         * o analista seja obrigado a tramitar para ?Enviar para complementação de empenho?.
         */
        if($empenhoComplementar > 0 && wf_pegarEstadoAtual($arrReformulacao['docid'])['esdid'] == PAR3_REFORMULACAO_ESDID_REFORMULACAO_APROVADA) {
                //$ocultar['estados'] = array(PAR3_REFORMULACAO_ESDID_AGUARDANDO_GERACAO_TERMO);
            }
        ?>
        <td style="text-align: center;" valign="center"><?php
        echo wf_desenhaBarraNavegacaoBootstrap(
                $arrReformulacao['docid'],
                array('refid' => $arrReformulacao['refid'],
                'dotid' => $_REQUEST['dotid'],
                'proid' => $_REQUEST['proid']),
                $ocultar,
                'Fluxo'
        );
        ?></td>
    </tr>
	</table>
</div>

<div id="debug"></div>

<div class="ibox float-e-margins animated modal" id="modal_escolas" tabdesex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%;">
        <form method="post" name="form-escolas" id="form-escolas" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Itens de Composição por Escola - <span id="itemnomemodal"></span></h3>
                </div>
                <div class="ibox-content">
                    <div id="dadosbuscaescola" class="col-lg-offset-3">
                        <input type="hidden" name="inuid" value="<?php echo $_GET['inuid']?>"/>
                        <input type="hidden" name="inpid" id="inpid" value="<?php echo $_GET['inpid']?>"/>
                        <input type="hidden" name="iniano" id="iniano" value=""/>
                        <input type="hidden" name="iigid" id="iigid" value=""/>
                        <input type="hidden" name="ipiid" id="ipiid" value=""/>
                        <input type="hidden" name="reiid" id="reiid" value=""/>
                        <input type="hidden" name="reiid" id="reiid" value=""/>
                        <input type="hidden" name="editavel_escola" id="editavel_escola" value=""/>
                        <input type="hidden" name="mostra_valor" id="mostra_valor" value=""/>
                        <input type="hidden" name="editavel" id="editavel" value=""/>
                        <input type="hidden" name="dados_pai" id="dados_pai" value=""/>
                        <?php
                        if($itrid == 1){
                            $modelMunicipio = new Par3_Model_Municipio();
                            $sqlMunicipiosUF = $modelMunicipio->montaSQLComboMunicipiosUF($_GET['inuid']);
                            echo $simec->select('escmuncod', '','', $sqlMunicipiosUF, array('maxlength' => '255','data-placeholder' => 'Selecione o Município'), array('input-size' => '8'));
                        }if($itrid == 2){
                            echo '<input type="hidden" name="escmuncod" id="escmuncod" value="'.$muncod.'"/>';
                        }
                        ?>
                    </div>
                    <div id="conteudo-modal" style="height: 500px; width: 100%; overflow: auto;">

                    </div>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
<?php if( $arrPermissao['cadastro']['salvar'] == 'S' ){?>
                    	<button type="button" id="btn-salvar-escolas" class="btn btn-success"><i class="fa fa-plus-square-o"></i> Salvar</button>
<?php }?>
                        <button type="button" id="cancelarEscolas" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Cancelar</button>
                    </div>
                </div>
            </div>
            <input type="hidden"  id="valorTotalHidden"  />
        </form>
    </div>
</div>

<div class="ibox float-e-margins animated modal" id="modal_anexo_arquivos" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                <h3 id="modal_titulo">Documentos Anexo - Incluir</h3>
            </div>
            <div class="ibox-content" id="conteudo-modal">
                <?php
                    echo $simec->input('arqid', 'Anexo', null, array('type' => 'file', 'accept' => 'application/pdf'));
                    echo $simec->input('axrdescricao', 'Descrição', null, array('maxlength'=>100));
                ?>
            </div>
            <div class="ibox-footer">
                <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                    <button type="button" id="cancelar" data-dismiss="modal"
                            class="btn btn-default">
                        <i class="fa fa-times-circle-o"></i> Cancelar
                    </button>
                    <button type="button" id="salvarArquivo"
                            class="btn btn-success" onclick="salvarDocumentoReformulacao()">
                        <i class="fa fa-plus-square-o"></i> Salvar
                    </button>
                </div>
            </div>
        </div>
    </div>
</div>

<br>
<br>
<script>
jQuery(document).ready(function() {
	calculaTotalReformulacao();
	
    var campos_max          = 10;   //max de 10 campos
    var x = 1; // campos iniciais
    $('#add_field').click (function(e) {
            /*e.preventDefault();     //prevenir novos clicks
            if (x < campos_max) {
                    $('#listas').append('<div>\
                    		<input type="file" name="anexo_reformulacao[]" class="form-control" accept=\'application/pdf\'>\
                            </div>');
                    x++;
            }*/
    	$("#modal_anexo_arquivos").modal("show");
    });

    // Remover o div anterior
    $('#listas').on("click",".remover_campo",function(e) {
            e.preventDefault();
            $(this).parent('div').remove();
            x--;
    });

    jQuery('.collapsed').click(function(){
        if( jQuery(this).hasClass('active_coll') ){
        	jQuery(this).removeClass("active_coll");
		} else {
			jQuery(this).addClass("active_coll");
		}
    });

    jQuery('[name="refvalorentidade"], [name="refrafmecfnde"]').focusin(function(){
		if( jQuery(this).val() == '0,00' ){
			jQuery(this).val('');
		}
	});
	
    jQuery('[name="refvalorentidade"], [name="refrafmecfnde"]').focusout(function(){
		if( jQuery(this).val() == '' ){
			jQuery(this).val('0,00');
		}
	});
});

function excluirItemReformulacao(iigid, inpid, iniano){
	//console.log(iigid+', '+inpid+', '+iniano);
	//var qtd-item = jQuery('[name="qtd-item[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();
	jQuery('[name="qtd-item-solicitada[' + inpid + '][' + iniano + '][' + iigid + ']"]').val('0');
	jQuery('#nomereformular-'+iigid).css('text-decoration','line-through');
	atualizaValoresItens(inpid, iniano, iigid);
}

function calculaValorComplementar() {
	var refempenhocomplementar = retiraPontos(jQuery('[name="refempenhocomplementar"]').val());
	var refvalorentidade = retiraPontos(jQuery('[name="refvalorentidade"]').val());
	var refrafmecfnde = retiraPontos(jQuery('[name="refrafmecfnde"]').val());

	refempenhocomplementar = (refempenhocomplementar ? refempenhocomplementar : 0);
	refvalorentidade = (refvalorentidade ? refvalorentidade : 0);
	refrafmecfnde = (refrafmecfnde ? refrafmecfnde : 0);
	
	var total = parseFloat(refempenhocomplementar) + parseFloat(refvalorentidade) + parseFloat(refrafmecfnde);
	jQuery('#total_div').html( number_format(total, 2, ",", ".") );
}

function calculaTotalReformulacao(){
	var valor_termo = jQuery('[name="valor_termo"]').val();
	var valor_reformulado = jQuery('[name="valor_reformulado"]').val();

	var refempenhocomplementar = retiraPontos(jQuery('[name="refempenhocomplementar"]').val());
	var refvalorentidade = retiraPontos(jQuery('[name="refvalorentidade"]').val());
	var refrafmecfnde = retiraPontos(jQuery('[name="refrafmecfnde"]').val());
	
	var totalSolicitado = 0;
	var totalAprovado = 0;
	
	jQuery('[name^="qtd-item-solicitada["]').each(function(){
		var nome 	= jQuery(this).attr('name');
		var arNome 	= nome.split('[');
		var inpid 	= arNome[1].substring(0, ( (arNome[1].length)-1 ));
		var iniano 	= arNome[2].substring(0, ( (arNome[2].length)-1 ));
		var iigid 	= arNome[3].substring(0, ( (arNome[3].length)-1 ));

		
		var qtd_solicitado	 = jQuery('[name="qtd-item-solicitada[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();
		var valor_referencia = jQuery('[name="valor_referencia[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();
		var qtd_aprovado 	 = jQuery('[name="qtd-item-aprovado[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();

		
		
		//console.log(totalAprovado+' - '+qtd_aprovado+' - '+valor_referencia);
		totalSolicitado = parseFloat(totalSolicitado) + (parseInt(qtd_solicitado) * parseFloat(valor_referencia));
		totalAprovado   = parseFloat(totalAprovado) + (parseInt(qtd_aprovado) * parseFloat(valor_referencia));
	});

	jQuery('#div_valor_reprogramado').html('R$ '+number_format(totalSolicitado, 2, ",", "."));
	jQuery('[name="valor_reformulado"]').val(totalSolicitado);
	
	if( parseFloat(totalSolicitado) < parseFloat(valor_termo) ){
		totalSolicitado = 0;
	} else {
		totalSolicitado = parseFloat(totalSolicitado) - parseFloat(valor_termo);
	}
	if( parseFloat(totalAprovado) < parseFloat(valor_termo) ){
		totalAprovado = 0;
	} else {
		totalAprovado = parseFloat(totalAprovado) - parseFloat(valor_termo);
	}
	
    jQuery('[name="refempenhocomplementar"]').val( number_format( 0, 2, ",", ".") );
    jQuery('[name="refempenhocomplementar"]').attr('readonly',true);
    var valorLiberadoAprovacao = ( parseFloat(totalAprovado) - ( parseFloat(refvalorentidade) + parseFloat(refrafmecfnde) ));
    //console.log(valorLiberadoAprovacao);
    if( parseFloat(valorLiberadoAprovacao) >= 0) {
        jQuery('[name="refempenhocomplementar"]').attr('readonly',false);
        jQuery('[name="refempenhocomplementar"]').val( number_format(valorLiberadoAprovacao, 2, ",", "."));
	}

	jQuery('#div_complementacao_solicitacao').html('R$ '+number_format(totalSolicitado, 2, ",", "."));
	jQuery('#div_complementacao_aprovacao').html('R$ '+number_format(totalAprovado, 2, ",", "."));

	calculaValorComplementar();
}

function retiraPontos(v){
    if( v != 0 ){
        var valor = v.replace(/\./gi,"");
        valor = valor.replace(/\,/gi,".");
    } else {
        var valor = v;
    }
    
    return valor;
}

function download_arquivo( arqid ){
	window.location.href = window.location.href+'&requisicao=download&arqid='+arqid;
}
function deletarAnexo( axrid, arqid ){
	window.location.href = window.location.href+'&requisicao=delarquivo&axrid='+axrid+'&arqid='+arqid;
}

function incluirItemComposicao( inpid, refid, refidpai, anaid, ano, inuid, escola ){
	var cpe, inuid, iigid;
    iigid = jQuery('#form-solicita-reformulacao').find('#iigid_'+inpid+'_'+ano).val();
    esdid = jQuery('#form-solicita-reformulacao').find('[name="esdid"]').val();

    var valor_referencia = jQuery('[name="valor_referencia[' + inpid + '][' + ano + '][' + iigid + ']"]').val();
	var vlr_detalhamento = jQuery('[name="vlr_detalhamento[' + inpid + '][' + ano + '][' + iigid + ']"]').val();

    if(iigid == ''){
        alert('Selecione o Item.');
        return false;
    }

        
    if( (jQuery('#nome-'+inpid+'-'+ano+'-'+iigid).val() != undefined) /*&& (parseFloat(valor_referencia) == parseFloat(vlr_detalhamento))*/  ){
        alert('Esse item ja está na lista.');
        return false;
    }

    var caminho = window.location.href;
    var action  = '&requisicao=buscaLinhaItemComposicao&iigid='+iigid+'&inuid='+inuid+'&inpid='+inpid+'&condicaoporescola='+escola+'&iniano='+ano+'&refid='+refid+'&anaid='+anaid+'&refidpai='+refidpai+'&esdid='+esdid;
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            //jQuery('#debug').html(resp);
            if(jQuery('#dados-itens-composicao-tbody_'+inpid+'_'+ano+' tr:last').length == 0){
            	jQuery('#dados-itens-composicao-tbody_'+inpid+'_'+ano).append(resp);
            }else{
            	jQuery('#dados-itens-composicao-tbody_'+inpid+'_'+ano+' tr:last').after(resp);
            }
        }
    });
}

function abreModalEscolasReformulacao(inpid, iniano, iigid, editavel, dados_pai, mostra_valor) {
    
    var label = jQuery(document).find('#nomereformular-'+iigid).html();
    
    jQuery(document).find('#itemnomemodal').html(label);
    var refid = jQuery('#form-solicita-reformulacao').find('[name="refid"]').val();
    var refidpai = jQuery('#form-solicita-reformulacao').find('[name="refidpai"]').val();
    var ipiid = jQuery('[name="ipiid['+inpid+']['+iniano+']['+iigid+']"]').val();
    var reiid = jQuery('[name="reiid['+inpid+']['+iniano+']['+iigid+']"]').val();
    
    jQuery('#btn-salvar-escolas').show();

    if( editavel == 'N' ){
    	jQuery('#btn-salvar-escolas').hide();
	} else {
		jQuery('#btn-salvar-escolas').show();
	}

    jQuery('#dadosbuscaescola').find('[name=reiid]').val(reiid);
    jQuery('#dadosbuscaescola').find('[name=inpid]').val(inpid);
    jQuery('#dadosbuscaescola').find('[name=iniano]').val(iniano);
    jQuery('#dadosbuscaescola').find('[name=iigid]').val(iigid);
    jQuery('#dadosbuscaescola').find('[name=ipiid]').val(ipiid);
    jQuery('#dadosbuscaescola').find('[name=mostra_valor]').val(mostra_valor);
    jQuery('#dadosbuscaescola').find('[name=editavel]').val(editavel);
    jQuery('#dadosbuscaescola').find('[name=dados_pai]').val(dados_pai);
    
    <?php if($itrid == 2):?>
    var caminho = window.location.href;
    var inpid  = jQuery('#dadosbuscaescola').find('[name=inpid]').val();
    var iniano = jQuery('#dadosbuscaescola').find('[name=iniano]').val();
    var inuid  = jQuery('#dadosbuscaescola').find('[name=inuid]').val();
    var muncod = jQuery('#dadosbuscaescola').find('[name=escmuncod]').val();
    //var iigid  = jQuery('#dadosbuscaescola').find('[name=iigid]').val();
    var action  = '&requisicao=buscaModalEscolas&inpid='+inpid+'&iniano='+iniano+'&ipiid='+ipiid+'&inuid='+inuid+'&muncod='+muncod+'&reformulacao=S&refid='+refid+'&refidpai='+refidpai+'&iigid='+iigid+'&reiid='+reiid+'&editavel='+editavel+'&dados_pai='+dados_pai+'&mostra_valor='+mostra_valor;
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
        	jQuery("#conteudo-modal").html(resp);
        }
    });
    <?php elseif($itrid == 3):?>
    var caminho = window.location.href;
    var inpid  = jQuery('#dadosbuscaescola').find('[name=inpid]').val();
    var iniano = jQuery('#dadosbuscaescola').find('[name=iniano]').val();
    //var ipiid  = jQuery('#dadosbuscaescola').find('[name=ipiid]').val();
    var iigid  = jQuery('#dadosbuscaescola').find('[name=iigid]').val();
    var inuid  = jQuery('#dadosbuscaescola').find('[name=inuid]').val();
    var action  = '&requisicao=buscaModalEscolas&inpid='+inpid+'&iniano='+iniano+'&ipiid='+ipiid+'&inuid='+inuid+'&reformulacao=S&refid='+refid+'&refidpai='+refidpai+'&iigid='+iigid+'&reiid='+reiid+'&editavel='+editavel+'&mostra_valor='+mostra_valor;
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
        	jQuery("#conteudo-modal").html(resp);
        }
    });
    <?php else:?>
    jQuery('#dadosbuscaescola').find('[name=escmuncod]').val('').trigger('chosen:updated');
    jQuery('#dadosbuscaescola').find('[name=editavel_escola]').val(editavel);
    jQuery("#conteudo-modal").hide();
    jQuery("#conteudo-modal").html('');
<?php endif;?>
jQuery("#modal_escolas").modal("show");
}

jQuery(document).on('change','[name=escmuncod]',function(){
    var caminho = window.location.href;
    var muncod = jQuery(this).val();
    var inpid  = jQuery('#dadosbuscaescola').find('[name=inpid]').val();

    var refid = jQuery('#form-solicita-reformulacao').find('[name="refid"]').val();
    var refidpai = jQuery('#form-solicita-reformulacao').find('[name="refidpai"]').val();
    var reiid = jQuery('#dadosbuscaescola').find('[name=reiid]').val();

    var editavel_escola = jQuery('#dadosbuscaescola').find('[name=editavel_escola]').val();
    
    var iniano = jQuery('#dadosbuscaescola').find('[name=iniano]').val();
    var ipiid  = jQuery('#dadosbuscaescola').find('[name=ipiid]').val();
    var iigid  = jQuery('#dadosbuscaescola').find('[name=iigid]').val();
    var inuid  = jQuery('#dadosbuscaescola').find('[name=inuid]').val();

    var mostra_valor = jQuery('#dadosbuscaescola').find('[name=mostra_valor]').val();
    var editavel = jQuery('#dadosbuscaescola').find('[name=editavel]').val();
    var dados_pai = jQuery('#dadosbuscaescola').find('[name=dados_pai]').val();
    
    var action  = '&requisicao=buscaModalEscolas&inpid='+inpid+'&iniano='+iniano+'&ipiid='+ipiid+'&iigid='+iigid+'&inuid='+inuid+'&muncod='+muncod+'&reformulacao=S&refid='+refid+'&refidpai='+refidpai+'&reiid='+reiid+'&editavel_escola='+editavel_escola+'&editavel='+editavel+'&dados_pai='+dados_pai+'&mostra_valor='+mostra_valor;
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
        	jQuery("#conteudo-modal").html(resp);
        }
    });
    jQuery("#conteudo-modal").slideDown();
    return false;
});

function atualizaTodasEscolas(){
    var qtd = jQuery('#qtd-item-escola-total').val();
    if(parseFloat(qtd) < 0){''
        qtd = '0';
        jQuery('#qtd-item-escola-total').val('0');
    }
    jQuery('.qtd-escola-item-composicao').val(qtd);
}

jQuery('#btn-salvar-reformulacao').click(function(){
	if( jQuery('[name="esdid"]').val() == 2365 || jQuery('[name="esdid"]').val() == 2364 ){
		if( trim(jQuery('[name="reftextoparecer"]').val()) == '' ){
			alert('O Texto do parecer é preenchimento obrigatório!');
			return false;
		}		
	}
	jQuery('[name="requisicao"]').val('salvar-reformulacao');
	jQuery('[name="form-solicita-reformulacao"]').submit();
});

jQuery('#btn-salvar-escolas').click(function(){
    var form   = jQuery('#form-escolas').serialize();
    var iniano = jQuery('#form-escolas').find('[name=iniano]').val();
    var ipiid = jQuery('#form-escolas').find('[name=ipiid]').val();
    var iigid = jQuery('#form-escolas').find('[name=iigid]').val();
    var reiid = jQuery('#form-escolas').find('[name=reiid]').val();
    var refid = jQuery('#form-escolas').find('[name=refid]').val();
    var inpid = jQuery('#form-escolas').find('[name=inpid]').val();
    
    var dotid = jQuery('#form-solicita-reformulacao').find('[name=dotid]').val();
    var proid = jQuery('#form-solicita-reformulacao').find('[name=proid]').val();
    //var vlreferencia = jQuery('#form-itens-composicao').find('[name="referencia['+iigid+']"]').val();
    
    var caminho = 'par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&iniano='+iniano+'&ipiid_escola='+ipiid+'&reiid='+reiid+'&refid='+refid+'&dotid='+dotid+'&proid='+proid+'&iigid='+iigid;
    var action  = '&requisicao=salvarItemComposicaoEscolas&'+form;
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
        	///jQuery('#debug').html(resp);
            var obj = JSON.parse(resp);
            
        	jQuery('[name="refid"]').val( obj.refid );
        	jQuery('[name="reiid[' + inpid + '][' + iniano + '][' + iigid + ']"]').val( obj.reiid );
        	jQuery('[name="qtd-item-solicitada[' + inpid + '][' + iniano + '][' + iigid + ']"]').val( obj.total );
        	jQuery("#modal_escolas").modal("hide");
            //atualizaListagemItensComposicao();
            atualizaValoresItens(inpid, iniano, iigid);
            return true;
        }
    });
    return false;
});

function pegaQtdMonitoradaItem(ipiid, iigid){
	var retorno = '0';
	var caminho = 'par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A';
    var action  = '&requisicao=qtd-monitorada-item&ipiid='+ipiid;
    jQuery.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
        	retorno = resp;
        }
    });
    return retorno;
}

function atualizaValoresItens(inpid, iniano, iigid) {

	var qtd_item_solicitado = jQuery('[name="qtd-item-solicitada[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();
	var qtd_item			= jQuery('[name="qtd-item[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();
	var valor_referencia 	= jQuery('[name="valor_referencia[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();
	var vlr_detalhamento 	= jQuery('[name="vlr_detalhamento[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();
	var ipiid 				= jQuery('[name="ipiid['+inpid+']['+iniano+']['+iigid+']"]').val();
//	var qtdMonitorada 		= pegaQtdMonitoradaItem(ipiid, iigid);
	var qtdMonitorada 		= jQuery('[name="qtd_monitorada['+inpid+']['+iniano+']['+iigid+']"]').val();

	if( qtdMonitorada != '' && parseFloat(qtdMonitorada) > parseFloat(0) ){
		//verifica se quantidade monitora dos itens e maior que a quantidade solicitada
		/*if(( parseFloat(valor_referencia) != parseFloat(vlr_detalhamento)) && (parseFloat(qtd_item_solicitado) > parseFloat(qtd_item)) ){
			swal("Ação não Permitida!", "A quantidade solicitada deve ser igual a quantidade monitorada dos itens:\n*Quantidade Solicitada: "+qtd_item_solicitado+"\n*Quantidade Monitorada: "+qtdMonitorada , "error");
			jQuery('[name="qtd-item-solicitada[' + inpid + '][' + iniano + '][' + iigid + ']"]').val(qtdMonitorada);
			return false;
		} else {*/
			if( parseFloat(qtd_item_solicitado) < parseFloat(qtdMonitorada) ){
				swal("Ação não Permitida!", "A quantidade solicitada deve ser igual ou maior que a quantidade monitorada dos itens:\n*Quantidade Solicitada: "+qtd_item_solicitado+"\n*Quantidade Monitorada: "+qtdMonitorada , "error");
				jQuery('[name="qtd-item-solicitada[' + inpid + '][' + iniano + '][' + iigid + ']"]').val(qtdMonitorada);
				return false;
			}
		//}
	}
	
	var total = (parseFloat(qtd_item_solicitado) * parseFloat(valor_referencia));
	jQuery('#div_vlr_total_solic_'+inpid+'_'+iniano+'_'+iigid).html(number_format(total, 2, ",", "."));

	var valor_total = 0;
	jQuery('.class_vlr_total_solic_'+inpid+'_'+iniano).each(function(){
		var valorTot = retiraPontos( jQuery(this).html() );
		valor_total = parseFloat(valor_total) + parseFloat(valorTot);
	});

	jQuery('#div_total_solicitado_'+inpid+'_'+iniano).html(number_format(valor_total, 2, ",", "."));

	calculaTotalReformulacao();
}

function atualizaValoresItensAnalise(inpid, iniano, iigid) {

	var qtd_item_solicitado = jQuery('[name="qtd-item-solicitada[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();
	var qtd_item_aprovado = jQuery('[name="qtd-item-aprovado[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();
	var valor_referencia = jQuery('[name="valor_referencia[' + inpid + '][' + iniano + '][' + iigid + ']"]').val();
	var ipiid = jQuery('[name="ipiid['+inpid+']['+iniano+']['+iigid+']"]').val();

	//verifica se quantidade monitora dos itens e maior que a quantidade solicitada
	if( parseFloat(qtd_item_solicitado) < parseFloat(qtd_item_aprovado) ){
		swal("Ação não Permitida!", "A quantidade aprovada deve ser igual ou menor que a quantidade solicitadas dos itens:\n*Quantidade Solicitada: "+qtd_item_solicitado+"\n*Quantidade Aprovada: "+qtd_item_aprovado , "error");
		jQuery('[name="qtd-item-aprovado[' + inpid + '][' + iniano + '][' + iigid + ']"]').val(qtd_item_solicitado);
		return false;
	}
	
	var total = (parseFloat(qtd_item_aprovado) * parseFloat(valor_referencia));
	jQuery('#div_vlr_aprovado_'+inpid+'_'+iniano+'_'+iigid).html(number_format(total, 2, ",", "."));

	calculaTotalReformulacao();
}

function formatReal(numero) {
    var tmp = numero + '';
    var neg = false;

    // Condição retirada por aumentar o valor numerico na exibição
    /*if (tmp - (Math.round(numero)) == 0) {
        tmp = tmp + '00';
    }*/

    if (tmp.indexOf(".")) {
        tmp = tmp.replace(".", "");
    }

    if (tmp.indexOf("-") == 0) {
        neg = true;
        tmp = tmp.replace("-", "");
    }

    if (tmp.length == 1) tmp = "0" + tmp

    tmp = tmp.replace(/([0-9]{2})$/g, ",$1");
    if (tmp.length > 6)
        tmp = tmp.replace(/([0-9]{3}),([0-9]{2}$)/g, ".$1,$2");

    if (tmp.length > 9)
        tmp = tmp.replace(/([0-9]{3}).([0-9]{3}),([0-9]{2}$)/g, ".$1.$2,$3");

    if (tmp.length = 12)
        tmp = tmp.replace(/([0-9]{3}).([0-9]{3}).([0-9]{3}),([0-9]{2}$)/g, ".$1.$2.$3,$4");

    if (tmp.length > 12)
        tmp = tmp.replace(/([0-9]{3}).([0-9]{3}).([0-9]{3}).([0-9]{3}),([0-9]{2}$)/g, ".$1.$2.$3.$4,$5");

    if (tmp.indexOf(".") == 0) tmp = tmp.replace(".", "");
    if (tmp.indexOf(",") == 0) tmp = tmp.replace(",", "0,");

    return "R$"+(neg ? '-' + tmp : tmp);
}

function salvarDocumentoReformulacao() {

    var tamanhoMaximo = 10 * 1024 * 1024;
    var axrdescricao = $('#axrdescricao').val();

    if($('#arqid').val() == '') {
        alert('Adicione um arquivo');
        return false;
    }

    var file = $("#arqid")[0].files[0];

    if(file.size > tamanhoMaximo){
        alert("O arquivo não pode ser maior que 10MB!");
        return false;
    }

    var formData = new FormData();
    formData.append('arqid', file);
    formData.append('refid', jQuery('#form-solicita-reformulacao').find('[name="refid"]').val() );
    formData.append('esdid', jQuery('#form-solicita-reformulacao').find('[name="esdid"]').val() );
    formData.append('axrdescricao', axrdescricao);
    formData.append('requisicao', 'salvarDocumento');

    jQuery.ajax({
        url: window.location.href,
        data: formData,
        cache: false,
        contentType: false,
        processData: false,
        method: 'POST',
        type: 'POST', // For jQuery < 1.9
        success: function (data) {
            if(trim(data) === 'emptyfile'){
                swal("Erro", "Adicione um arquivo!", "error");
            }else if(trim(data) === 'maxsize'){
                swal("Erro", "Por favor, adicione um arquivo de no máximo 5MB.", "error");
            }else if(trim(data) === 'error'){
                swal("Erro", "Erro ao salvar arquivo, tente novamente ou contate o administrador do sistema.", "error");
            }else{
                $("#modal_anexo_arquivos").modal('hide');
                $('#arqid').val('');
                $('#axrdescricao').val('');
                $('#div-arquivos').html(data);
                swal("Sucesso", "Arquivo salvo com sucesso!", "success");
            }
        }
    });
}

$(document).find('.monetario-origem-complementar').on('keyup blur',function() {
    var strId = $(this).prop('id');
    var id = $("#"+strId);
    var val = id.val() === ''? 0:id.val();

    id.val(mascaraglobal('[.###],##',val));
    
    calculaValorComplementar();
    // if(strId != 'refempenhocomplementar'){
    //     calculaTotalReformulacao();
    // }
});

</script>
