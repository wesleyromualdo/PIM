<?php
	require APPRAIZ . 'obras2/includes/principal/solicitacoes/ctrl.php';

	require APPRAIZ . 'obras2/includes/principal/solicitacoes/css.php';


	foreach(capturaSolicitacoesObra($obrid) as $solicitacao):?>
	    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
	        <tr>
	            <td colspan="3" align="center">
	                <b>Solicitação N° <?=$solicitacao['slcid']?></b>
	            </td>
	        </tr>
	        <tr>
	            <td colspan='2'></td>
	            <td align="right">
	                <? if($habilitado_super && ($solicitacao['esdid'] == ESDID_SOLICITACOES_INDEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACOES_DEFERIDO) && $solicitacao['aprovado'] == 'N'):?>
	                <button type="button" class='' onclick='aprovaSolicitacao(<?=$solicitacao['slcid']?>);'>APROVAR</button>
	                <? endif;?>
	            </td>
	        </tr>
	        <tr>
	            <td class="SubTituloDireita" width="20%">Solicitação</td>
	            <td><span onclick="abreSolicitacao(<?=$obrid?>,<?=$solicitacao['tslid']?>,<?=$solicitacao['slcid']?>);" style="cursor:pointer;" title="Acessar Solicitação"><img src="../imagens/alterar.gif" style="border:0;"> <?=$solicitacao['tsldescricao']?></span></td>
	        </tr>
	        <tr>
	            <td class="SubTituloDireita" width="20%">Situação</td>
	            <? if($solicitacao['aprovado'] == 'S'): ?>
	                <td>
	                <?
	                if($solicitacao['esdid'] == ESDID_SOLICITACOES_INDEFERIDO) {
	                    echo '<b style="color:red;">'.$solicitacao['esddsc'].'</b>';
	                } else if($solicitacao['esdid'] == ESDID_SOLICITACOES_DEFERIDO) {
	                    echo'<b style="color:green;">'.$solicitacao['esddsc'].'</b>';
	                } else {
	                    echo $solicitacao['esddsc'];
	                }
	                ?>
	                </td>
	            <? else: ?>
	            <td>
	                <?
	                if($solicitacao['aprovado'] == 'N'){
	                    if($solicitacao['esdid'] == ESDID_SOLICITACOES_CADASTRAMENTO){
	                        echo '<b>Em Cadastramento</b>';
	                    } else if($solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA) {
	                        echo '<b>Diligência</b>';
	                    } else if($solicitacao['esdid'] == ESDID_SOLICITACOES_CANCELADO) {
	                        echo'<b>Cancelado</b>';
	                    } else {
	                        echo'<b>Aguardando Análise</b>';
	                    }
	                }else{
	                    echo '<b style="color:green;">' . retornaTextoEstadoSolicitacao($solicitacao['esdid']) . '</b>';
	                }
	                ?>
	               </td>
	            <? endif; ?>
	        </tr>
	        <tr>
	            <td class="SubTituloDireita" width="20%">Justificativa</td>
	            <td><?=$solicitacao['slcjustificativa']?></td>
	        </tr>
	        <tr>
	            <td class="SubTituloDireita" width="20%">Inserido por</td>
	            <td><?=$solicitacao['criador']?></td>
	        </tr>
	        <? if(($solicitacao['esdid'] != ESDID_SOLICITACOES_CADASTRAMENTO && $solicitacao ['esdid'] != ESDID_SOLICITACOES_AGUARDANDO_ANALISE && $solicitacao['esdid'] != ESDID_SOLICITACOES_RETORNADO && $solicitacao['aprovado'] == 'S') || ($solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA)): ?>
	        <tr>
	            <td class="SubTituloDireita" width="20%">Análise FNDE</td>
	            <td>
	                <?
	                $analise = capturaAnaliseSolicitacao($solicitacao['docid']);
	                ?>
	                <table style="width: 100%">
	                    <tr>
	                        <td class="SubTituloDireita" style="width: 100px">Data da Análise</td>
	                        <td><?=$analise['htddata']?></td>
	                    </tr> 
	                                 
	                  <?php if($solicitacao['esdid'] != ESDID_SOLICITACOES_DEFERIDO  ) : ?>
		                    <tr>
		                        <td class="SubTituloDireita"  style="width: 100px">Analisado por</td>
		                        <td><?=$analise['analista']?></td>
		                    </tr>
		                 <?php 
		                   elseif ($solicitacao['esdid'] == ESDID_SOLICITACOES_DEFERIDO && $solicitacao['aprovado'] == 'S'):
		                 ?>
		                    
		                    <tr>
		                        <td class="SubTituloDireita"  style="width: 100px">Analisado por</td>
		                        <td><?=$solicitacao['usuaprovado']?></td>
		                    </tr>
	                  <?php endif; ?>
	                  
	                    <? if($solicitacao['esdid'] == ESDID_SOLICITACOES_INDEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA || $solicitacao['esdid'] == ESDID_SOLICITACOES_DEFERIDO): ?>
	                        <tr>
	                            <td class="SubTituloDireita"  style="width: 100px">Observações</td>
	                            <td>
	                                <?=capturaAnaliseSolicitacaoObservacoes($solicitacao['docid'])?>
	                            </td>
	                        </tr>
	                    <? endif; ?>
	                    <? if(($solicitacao['aprovado'] == 'S' || $solicitacao['esdid'] == ESDID_SOLICITACOES_DILIGENCIA) and ($solicitacao['tslid'] == 1 || $solicitacao['tslid'] == 2 || $solicitacao['tslid'] == 3 || $solicitacao['tslid'] == 4  ||$solicitacao['tslid'] == 5 )): ?>
	                        <tr>
	                            <td class="SubTituloDireita"  style="width: 100px">Ver Análise</td>
	                            <td> <a onclick='printQuestionario(<?=$obrid?>,<?=simec_json_encode($solicitacao['qrpid'])?>,<?=$solicitacao['slcid']?>,<?=simec_json_encode($solicitacao['queid'])?>);'> <img src='/imagens/print.png' > </a></td>
	                        </tr>
	                    <? endif; ?>
	                </table>
	            </td>
	            <? endif;?>
	        </tr>
	
	    </table>
	<?
	endforeach;


	require APPRAIZ . 'obras2/includes/principal/solicitacoes/js.php';