<?php
require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraContratacao/ctrl.php';
require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraContratacao/css.php';

if ( empty($dados['crtid']) ):
	$licitacao = new Licitacao();
	$licDados = $licitacao->pegaDadosPorObra( $obrid );

?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td width="100%" class="subtitulocentro" style="color: red;" valign="top">
			<?php
			if ( count( $licDados ) ):
			?>
			Esta obra não está vinculada a nenhum contrato.
			<br>

			<?php
			else:
			?>
			Para vincular esta obra a um contrato, primeiramente ela deve ser vinculada a uma licitação.
			<?php
			endif;
			?>
		</td>


	</tr>
</table>
<?php
else:
    monta_titulo( 'Contrato (Obra:'.$obrid.')', '' );
    $idObraOriginal = $obrid;
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" border="0">
	<tr>
            <td height="1">&nbsp;</td>
            <td>&nbsp;</td>

	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Licitação:</td>
		<td>
			<?php
				$licitacao = new Licitacao();
				$dadoLic   = $licitacao->pegaDadosPorObra( $obrid );
				echo $dadoLic['licnumero'] . ' - ' . $dadoLic['moldsc'];
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Empresa Contratada</td>
		<td>
		  <span id="entnomeempresa">
              <span id="entnomeempresa_<?= $obrid; ?>">
          <?php
          echo $entnomeempresa;

          if (($estadoObra == ESDID_OBJ_CONCLUIDO || $estadoObra == ESDID_OBJ_CANCELADO || $estadoObra == ESDID_OBJ_INACABADA || $estadoObra == ESDID_OBJ_PARALISADO) &&
              (!$execucaoFinanceiraFinalizada) && ($dados['entidempresa'] != '') ) {
              if ($habilitaPesquisaCNPJ == 'S') {

                  echo "<input type='button' name='pesquisar_entidade' value='Pesquisar' style='cursor: pointer;' onclick='inserirEmpresa($entidempresa,\"\",$obrid);'>";
              }
          }
          ?>
              </span>
          </span>
            <?php
              if ( ($estadoObra == ESDID_OBJ_CONCLUIDO || $estadoObra == ESDID_OBJ_CANCELADO || $estadoObra == ESDID_OBJ_INACABADA) &&
              (!$execucaoFinanceiraFinalizada ) && ($dados['entidempresa'] != '')){
              echo "&nbsp;<img src='../imagens/alterar.gif' title='Inserir aditivo de valor' width='16' style=cursor:pointer; 
                        onclick='inserirAditivoExtra(".$obrid.",".$entidempresa.")'>&nbsp;";
              }
              ?>
		  <input type="hidden" name="entidempresa" id="entidempresa" value="<?php echo $entidempresa; ?>">


        </td>
	</tr>
	<tr>
		<td class="subtitulodireita">Data de Assinatura do Contrato:</td>
		<td><?= $dados['crtdtassinatura'];?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Prazo de Vigência do Contrato (dias):</td>
		<td><?= $dados['crtprazovigencia'];?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Data de término do contrato:</td>
		<td><?= $dados['crtdttermino'];?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor do Contrato (R$):</td>
		<td><?= $dados['crtvalorexecucao'];?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Valor Previsto da Obra (R$):</td>
		<td><?=$obrvalorprevisto;?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Percentual BDI:</td>
		<td><?= $dados['crtpercentualdbi'];?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
	</tr>
    <tr>
		<td class="SubTituloDireita">Data de Inserção:</td>
		<td><?= $dados['dt_cadastro'];?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"> Contrato digitalizado em PDF:</td>
		<td>
            <?php if($dados['arqidcontrato']){
                $arquivo = new Arquivo($dados['arqidcontrato']); ?>
                <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                <?php
                echo $arquivo->arqnome . "." . $arquivo->arqextensao;
            }?>
		</td>
	</tr>
    <?
    if(obraMi($obrid)):
        $itenscomposicao = new ItensComposicaoObras();
    ?>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Soma do Valor das Etapas do Cronograma da Edificação (R$):</td>
        <td>
            <?= number_format( $itenscomposicao->getValorTotalItens($obrid, 'D'), 2, ',', '.'); ?>
        </td>
    </tr>

    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Soma do Valor das Etapas dos Serviços Externos (R$): </td>
        <td>
            <?= number_format( $itenscomposicao->getValorTotalItens($obrid, 'F'), 2, ',', '.'); ?>
        </td>
    </tr>
    <? endif;
    if ( $dados['ttaid'] ):
    ?>
    <tr>
        <td width="265" class="subtitulodireita">&nbsp;</td>
        <td>
            Esta obra foi aditivada, para visualizar o histórico de aditivos
            <a href="javascript:janela('?modulo=principal/historicoAditivo&acao=E&crtid=<?php echo $dados['crtid']; ?>',600,800,'historicoAditivo')">clique aqui.</a>
    </td>
    </tr>
    <tr>
        <td width="265" class="subtitulodireita">Tipo de Aditivo:</td>
        <td>
            <?php
            $tipoTermo = new TipoTermoAditivo( $dados['ttaid'] );
            echo $tipoTermo->ttadsc;
            ?>
        </td>
    </tr>
    <tr>
        <td width="265" class="subtitulodireita">Denominação:</td>
        <td>
            <?php
            echo $dados['crtdenominacao'];
            ?>
        </td>
    </tr>
	<tr>
		<td width="265" class="subtitulodireita">Data de Assinatura do Aditivo:</td>
		<td>
			<?php
			echo formata_data( $dados['crtdtassinaturaaditivo'] );
			?>
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Aditivo de Supressão:</td>
		<td>
			<?php
			echo ($dados['crtsupressao'] == 't' ? 'Sim' : 'Não');
			?>
		</td>
	</tr>
	<tr>
		<td width="265" class="subtitulodireita">Justificativa:</td>
		<td>
			<?php
			echo nl2br( $dados['crtjustificativa'] );
			?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Anexo Assinado digitalizado em PDF:</td>
		<td>
            <?php if($dados['arqid']){
                $arquivo = new Arquivo($dados['arqid']); ?>
                <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                <?php
                echo $arquivo->arqnome . "." . $arquivo->arqextensao;
            }?>
		</td>
	</tr>
    <?php
    endif;

    if( !obraMi($obrid) ):?>

	<tr bgcolor="#FFFFFF" id="tr_obra_contrato" style="display: <?=$dados['licid'] ? '' : 'none' ?>;">
            <td colspan="3" valign="top" id="td_obra_contrato">
                <div style="margin-bottom: 7px;">
                        Vínculo da obra no contrato
                </div>
                <?php
                if ( $dados['crtid'] ):
                        $obraContrato = new ObrasContrato();
                        $dadosObraContrato = $obraContrato->listaByContrato( $dados['crtid'], $obrid );
                        foreach ( $dadosObraContrato as $dados ):
                                $umdid 			  		= $dados['umdid'];
                                $ocrqtdconstrucao     = $dados['ocrqtdconstrucao'];
                                $ocrdtordemservico    = formata_data( $dados['ocrdtordemservico'] );
                                $ocrdtinicioexecucao  = formata_data( $dados['ocrdtinicioexecucao'] );
                                $ocrprazoexecucao     = $dados['ocrprazoexecucao'];
                                $ocrdtterminoexecucao = formata_data(  $dados['ocrdtterminoexecucao'] );
                                $ocrvalorexecucao     = number_format( $dados['ocrvalorexecucao'], 2, ',', '.');
                                $ocrcustounitario     = number_format( $dados['ocrcustounitario'], 2, ',', '.');
                                $ocrpercentualdbi     = number_format( $dados['ocrpercentualdbi'], 2, ',', '.');
                                $obrid                = $dados['obrid'];
                                $obrnome              = $dados['obrnome'];
                                $arqidos              = $dados['arqidos'];
                                $arqidcusto           = $dados['arqidcusto'];
                ?>
                <table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem" id="table_obra_<?php echo $obrid ?>" style="margin-bottom: 10px;">
                    <tr>
                        <td class="subtitulodireita" width="20%">
                                Obra:
                                <input name="obrid[]" id="obrid" value="<?php echo $obrid ?>" type="hidden">
                        </td>
                        <td id="txt_obra" width="35%" colspan="3">
                                <?php echo $obrid . ' - ' . $obrnome ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtitulodireita">Data de assinatura da ordem de serviço:</td>
                        <td><?=$ocrdtordemservico?></td>
                        <td class="SubTituloDireita">Total da planilha contratada (R$):</td>
                        <td><?=$ocrvalorexecucao?></td>
                    </tr>
                    <tr>
                        <td class="subtitulodireita">Início da Execução:</td>
                        <td><?=$ocrdtinicioexecucao?></td>
                        <td class="SubTituloDireita">Área/Quantidade a ser Construída:</td>
                        <td>
                                <?php
                                $unidadeMedida = new UnidadeMedida( $umdid );
                                echo $ocrqtdconstrucao;
                                echo '&nbsp;&nbsp;Unidade de Medida:&nbsp;' . $unidadeMedida->umdeesc;
                                ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtitulodireita"></td>
                        <td></td>
                        <td class="SubTituloDireita">Custo Unitário R$:</td>
                        <td><?=$ocrcustounitario?> (R$ / Unidade de Medida)</td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita"></td>
                        <td></td>
                        <td class="SubTituloDireita">Percentual BDI:</td>
                        <td><?=$ocrpercentualdbi?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
                    </tr>
                    <tr>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top;">Ordem de Serviço assinada e em PDF:</td>
                        <td>
                            <?php
                            if ( $obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C ){
                                    $ordemServico = new OrdemServicoMI();
                                    $ordemServico->carregarPorObrid( $obrid );

                                    if($arquivo->arqid){
                                        $arquivo = new Arquivo($arquivo->arqid); ?>
                                        <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                                        <?php
                                        echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                                    }
                            }else{
                                    if($arqidos){
                                        $arquivo = new Arquivo($arqidos); ?>
                                        <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                                        <?php
                                        echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                                    }
                            }
                            ?>
                        </td>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top;">Planilha de Custo Contratada assinada e em PDF:</td>
                        <td>
                            <?php if($arqidcusto){
                                $arquivo = new Arquivo($arqidcusto); ?>
                                <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                                <?php
                                echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                            }?>
                        </td>
                    </tr>
                </table>

                <?php
                        endforeach;
                endif;
                unset( $umdid, $ocrqtdconstrucao, $ocrdtordemservico, $ocrdtinicioexecucao, $ocrprazoexecucao, $ocrdtterminoexecucao, $ocrvalorexecucao, $ocrcustounitario, $ocrpercentualdbi, $obrid, $obrnome );
                ?>

            </td>
	</tr>
    <? endif;

    $execFinanceira = new ExecucaoFinanceira();
    $crtidOriginal = $execFinanceira->pegaContratoOriginal($idObraOriginal);
    $arrAditivoExtra = $execFinanceira->pegaDadosAditivoExtra($crtidOriginal);

    if(count($arrAditivoExtra)){
    ?>
    <tr bgcolor="#FFFFFF" ">
        <td colspan="3" valign="top" id="td_obra_contrato">
            <div style="margin-bottom: 7px;">
                Aditivos Adicionais do Contrato
            </div>
            <?php foreach ($arrAditivoExtra as $aditivoExtra){?>
                <table width="90%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem"
                       style="margin-bottom: 10px;">
                    <tr>
                        <td width="265" class="subtitulodireita">Tipo de Aditivo:</td>
                        <td>
                            <?php
                            $tipoTermo = new TipoTermoAditivo($aditivoExtra['ttaid']);
                            echo $tipoTermo->ttadsc;
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td width="265" class="subtitulodireita">Denominação:</td>
                        <td>
                            <?php
                            echo $aditivoExtra['crtdenominacao'];
                            if ( ($estadoObra == ESDID_OBJ_CONCLUIDO || $estadoObra == ESDID_OBJ_CANCELADO || $estadoObra == ESDID_OBJ_INACABADA ) &&
                                (!$execucaoFinanceiraFinalizada)) {
                                echo "&nbsp;<img src='../imagens/alterar.gif' title='Editar Aditivo' width='16' style=cursor:pointer; 
                                onclick='editarAditivoExtra(" .$idObraOriginal.",".$entidempresa. ",".$aditivoExtra['crtid'].")'>&nbsp;
                                <img src='../imagens/excluir.gif' title='Excluir Aditivo' width='16' style=cursor:pointer; 
                                onclick='excluirAditivoExtra(" . $aditivoExtra['crtid'] . ")'>&nbsp;";
                            }
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td width="265" class="subtitulodireita">Valor do Aditivo:</td>
                        <td>
                            <?php
                            echo formata_numero_monetario($aditivoExtra['crtvalorexecucao']);
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td width="265" class="subtitulodireita">Data de Assinatura do Aditivo:</td>
                        <td>
                            <?php
                            echo formata_data($aditivoExtra['crtdtassinatura']);
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td width="265" class="subtitulodireita">Justificativa:</td>
                        <td>
                            <?php
                            echo nl2br($aditivoExtra['crtjustificativa']);
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Anexo Assinado
                            digitalizado em PDF:
                        </td>
                        <td>
                            <?php
                            if($aditivoExtra['arqid']){
                                $arquivo = new Arquivo($aditivoExtra['arqid']); ?>
                                <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                                <?php
                                echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                            }
                            ?>
                        </td>
                    </tr>
                </table>
            <?php } ?>
        </td>
    </tr>
    <?php } ?>


</table>

<?php
endif;
?>
        <?php

        foreach ($arrObridVinculado as $obra => $obrid) {
        $idObra = $obrid;
        $obraVinculada = new Obras($obrid);
        $crtidVinculado = $obraVinculada->pegaContratoPorObra($obrid);
        $contratoVinculado = new Contrato($crtidVinculado);
        $dadosVinculados = $contratoVinculado->getDados();


        if ($dadosVinculados['crtid']){
        $dadosVinculados['crtdtassinatura'] = formata_data($dadosVinculados['crtdtassinatura']);
        $dadosVinculados['dt_cadastro'] = formata_data($dadosVinculados['dt_cadastro']);
        $dadosVinculados['crtdttermino'] = formata_data($dadosVinculados['crtdttermino']);
        $dadosVinculados['crtvalorexecucao'] = number_format($dadosVinculados['crtvalorexecucao'], 2, ',', '.');
        $dadosVinculados['crtpercentualdbi'] = number_format($dadosVinculados['crtpercentualdbi'], 2, ',', '.');


        $obrvalorprevistoVinculada = number_format($obraVinculada->obrvalorprevisto, 2, ',', '.');

        $empresa = new Entidade($dadosVinculados['entidempresa']);

            $habilitaPesquisaCNPJVinculado = 'S';
            if($empresa->entnumcpfcnpj != '' && $empresa->entnome != ''){
                $habilitaPesquisaCNPJVinculado = 'N';
            }
            $entnomeempresa = "(". mascaraglobal($empresa->entnumcpfcnpj,"##.###.###/####-##") .") ".$empresa->entnome;

        $entidempresa = $empresa->getPrimaryKey();


        monta_titulo('Contrato - Obra Vinculada (Obra:'.$obrid.')', '');
        ?>
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" border="0">
    <tr>
        <td>
        <table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class=""
               style="margin-bottom: 10px;">
            <tr>
                <td width="265" class="subtitulodireita">Licitação:</td>
                <td>
                    <?php
                    $licitacao = new Licitacao();
                    $dadoLic = $licitacao->pegaDadosPorObra($obrid);
                    echo $dadoLic['licnumero'] . ' - ' . $dadoLic['moldsc'];
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Empresa Contratada</td>
                <td>
                    <span id="entnomeempresa">
                    <span id="entnomeempresa_<?=$obrid;?>">
                    <?php
                    echo $entnomeempresa;
                    if ( ($estadoObra == ESDID_OBJ_CONCLUIDO || $estadoObra == ESDID_OBJ_CANCELADO || $estadoObra == ESDID_OBJ_INACABADA || $estadoObra == ESDID_OBJ_PARALISADO) &&
                        (!$execucaoFinanceiraFinalizada) && ($dadosVinculados['entidempresa'] != '')) {
                        if ($habilitaPesquisaCNPJVinculado == 'S') {

                            echo "<input type='button' name='pesquisar_entidade' value='Pesquisar' style='cursor: pointer;' onclick='inserirEmpresa($entidempresa,\"\",$obrid);'>";
                        }
                    }
                    ?>
                    </span>
                    </span>
                    <?php
                    if ( ($estadoObra == ESDID_OBJ_CONCLUIDO || $estadoObra == ESDID_OBJ_CANCELADO || $estadoObra == ESDID_OBJ_INACABADA) &&
                        (!$execucaoFinanceiraFinalizada) && ($dados['entidempresa'] != '')){
                        echo "&nbsp;<img src='../imagens/alterar.gif' title='Inserir aditivo de valor' width='16' style=cursor:pointer; 
                        onclick='inserirAditivoExtra(".$obrid.",".$entidempresa.")'>&nbsp;";
                    }
                    ?>

                    <input type="hidden" name="entidempresa" id="entidempresa" value="<?php echo $entidempresa; ?>">
                </td>
            </tr>
            <tr>
                <td class="subtitulodireita">Data de Assinatura do Contrato:</td>
                <td><?= $dadosVinculados['crtdtassinatura']; ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita">Prazo de Vigência do Contrato (dias):</td>
                <td><?= $dadosVinculados['crtprazovigencia']; ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita">Data de término do contrato:</td>
                <td><?= $dadosVinculados['crtdttermino']; ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Valor do Contrato (R$):</td>
                <td><?= $dadosVinculados['crtvalorexecucao']; ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Valor Previsto da Obra (R$):</td>
                <td><?= $obrvalorprevistoVinculada; ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Percentual BDI:</td>
                <td><?= $dadosVinculados['crtpercentualdbi']; ?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Data de Inserção:</td>
                <td><?= $dadosVinculados['dt_cadastro']; ?></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"> Contrato digitalizado
                    em PDF:
                </td>
                <td>
                    <?php if($dadosVinculados['arqidcontrato']){
                        $arquivo = new Arquivo($dadosVinculados['arqidcontrato']); ?>
                        <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                        <?php
                        echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                    }?>
                </td>
            </tr>
            <?
            if (obraMi($obrid)):
                $itenscomposicao = new ItensComposicaoObras();
                ?>
                <tr>
                    <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Soma do Valor das
                        Etapas do Cronograma da Edificação (R$):
                    </td>
                    <td>
                        <?= number_format($itenscomposicao->getValorTotalItens($obrid, 'D'), 2, ',', '.'); ?>
                    </td>
                </tr>

                <tr>
                    <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Soma do Valor das
                        Etapas dos Serviços Externos (R$):
                    </td>
                    <td>
                        <?= number_format($itenscomposicao->getValorTotalItens($obrid, 'F'), 2, ',', '.'); ?>
                    </td>
                </tr>
            <? endif; ?>
            <?php
            if ($dadosVinculados['ttaid']):
            ?>
            <tr>
                <td width="265" class="subtitulodireita">&nbsp;</td>
                <td>

                    Esta obra foi aditivada, para visualizar o histórico de aditivos
                    <a href="javascript:janela('?modulo=principal/historicoAditivo&acao=E&crtid=<?php echo $dadosVinculados['crtid']; ?>',600,800,'historicoAditivo')">clique
                        aqui.</a>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Tipo de Aditivo:</td>
                <td>
                    <?php
                    $tipoTermo = new TipoTermoAditivo($dadosVinculados['ttaid']);
                    echo $tipoTermo->ttadsc;
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Denominação:</td>
                <td>
                    <?php
                    echo $dadosVinculados['crtdenominacao'];
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Data de Assinatura do Aditivo:</td>
                <td>
                    <?php
                    echo formata_data($dadosVinculados['crtdtassinaturaaditivo']);
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Aditivo de Supressão:</td>
                <td>
                    <?php
                    echo($dadosVinculados['crtsupressao'] == 't' ? 'Sim' : 'Não');
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Justificativa:</td>
                <td>
                    <?php
                    echo nl2br($dadosVinculados['crtjustificativa']);
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Anexo Assinado
                    digitalizado em PDF:
                </td>
                <td>
                    <?php
                    if($dadosVinculados['arqid']){
                        $arquivo = new Arquivo($dadosVinculados['arqid']); ?>
                        <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                        <?php
                        echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                    }
                    ?>
                </td>
            </tr>
            <?php endif;?>
        </table>
        </td>
    </tr>
    <?php


    if( !obraMi($obrid) ): ?>


        <?php
        if ( $dadosVinculados['crtid'] ):
        $obraContrato = new ObrasContrato();
        $dadosObraContrato = $obraContrato->listaByContrato( $crtid, $obrid );

        if(count($dadosObraContrato) > 0){
        ?>
    <tr bgcolor="#FFFFFF" id="tr_obra_contrato" style="display: <?= $dadosVinculados['licid'] ? '' : 'none' ?>;">
        <td colspan="3" valign="top" id="td_obra_contrato">
            <div style="margin-bottom: 7px;">
                Vínculo da obra no contrato adicional
            </div>
            <?php

            foreach ($dadosObraContrato as $dados):
                $umdid = $dados['umdid'];
                $ocrqtdconstrucao = $dados['ocrqtdconstrucao'];
                $ocrdtordemservico = formata_data($dados['ocrdtordemservico']);
                $ocrdtinicioexecucao = formata_data($dados['ocrdtinicioexecucao']);
                $ocrprazoexecucao = $dados['ocrprazoexecucao'];
                $ocrdtterminoexecucao = formata_data($dados['ocrdtterminoexecucao']);
                $ocrvalorexecucao = number_format($dados['ocrvalorexecucao'], 2, ',', '.');
                $ocrcustounitario = number_format($dados['ocrcustounitario'], 2, ',', '.');
                $ocrpercentualdbi = number_format($dados['ocrpercentualdbi'], 2, ',', '.');
                $obrid = $dados['obrid'];
                $obrnome = $dados['obrnome'];
                $arqidos = $dados['arqidos'];
                $arqidcusto = $dados['arqidcusto'];
                ?>
                <table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem"
                       id="table_obra_<?php echo $obrid ?>" style="margin-bottom: 10px;">
                    <tr>
                        <td class="subtitulodireita" width="20%">
                            Obra:
                            <input name="obrid[]" id="obrid" value="<?php echo $obrid ?>" type="hidden">
                        </td>
                        <td id="txt_obra" width="35%" colspan="3">
                            <?php echo $obrid . ' - ' . $obrnome ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtitulodireita">Data de assinatura da ordem de serviço:</td>
                        <td><?= $ocrdtordemservico ?></td>
                        <td class="SubTituloDireita">Total da planilha contratada (R$):</td>
                        <td><?= $ocrvalorexecucao ?></td>
                    </tr>
                    <tr>
                        <td class="subtitulodireita">Início da Execução:</td>
                        <td><?= $ocrdtinicioexecucao ?></td>
                        <td class="SubTituloDireita">Área/Quantidade a ser Construída:</td>
                        <td>
                            <?php
                            $unidadeMedida = new UnidadeMedida($umdid);
                            echo $ocrqtdconstrucao;
                            echo '&nbsp;&nbsp;Unidade de Medida:&nbsp;' . $unidadeMedida->umdeesc;
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="subtitulodireita"></td>
                        <td></td>
                        <td class="SubTituloDireita">Custo Unitário R$:</td>
                        <td><?= $ocrcustounitario ?> (R$ / Unidade de Medida)</td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita"></td>
                        <td></td>
                        <td class="SubTituloDireita">Percentual BDI:</td>
                        <td><?= $ocrpercentualdbi ?> (Administração, taxas, emolumentos, impostos e lucro.)</td>
                    </tr>
                    <tr>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top;">Ordem de Serviço assinada e em PDF:
                        </td>
                        <td>
                            <?php
                            if ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) {
                                $ordemServico = new OrdemServicoMI();
                                $ordemServico->carregarPorObrid($obrid);

                                if($ordemServico->arqid){
                                    $arquivo = new Arquivo($ordemServico->arqid); ?>
                                    <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                                    <?php
                                    echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                                }                             } else {
                                 if($arqidos){
                                    $arquivo = new Arquivo($arqidos); ?>
                                    <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                                    <?php
                                    echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                                }
                            }
                            ?>
                        </td>
                        <td align='right' class="SubTituloDireita" style="vertical-align:top;">Planilha de Custo Contratada assinada
                            e em PDF:
                        </td>
                        <td>
                            <?php
                            $arquivo = new Arquivo($arqidcusto);
                            if ($arquivo->arqid) {
                                echo "<a href='?modulo=principal/exibeContrato&acao=" . $_REQUEST['acao'] . "&op=download&arqid={$arquivo->arqid}'>
                                    <img src='/imagens/salvar.png' border='0'>
                                        " . $arquivo->arqnome . "." . $arquivo->arqextensao . "
                                    </a>";
                            }
                            ?>
                        </td>
                    </tr>

                </table><br>
                <?php
            endforeach; ?>
        </td>
    </tr>
<?php }
endif;

endif;
}
$execFinanceira = new ExecucaoFinanceira();
$crtidOriginal = $execFinanceira->pegaContratoOriginal($idObra);
$arrAditivoExtra = $execFinanceira->pegaDadosAditivoExtra($crtidOriginal); 

if(count($arrAditivoExtra) > 0){
    ?>
    <tr bgcolor="#FFFFFF">
    <td colspan="3" valign="top" id="td_obra_contrato">
        <div style="margin-bottom: 7px;">
            Aditivos Adicionais do Contrato
        </div>
        <?php foreach ($arrAditivoExtra as $aditivoExtra){?>
        <table width="90%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem"
               style="margin-bottom: 10px;">
            <tr>
                <td width="265" class="subtitulodireita">Tipo de Aditivo:</td>
                <td>
                    <?php
                    $tipoTermo = new TipoTermoAditivo($aditivoExtra['ttaid']);
                    echo $tipoTermo->ttadsc;
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Denominação:</td>
                <td>
                    <?php
                    echo $aditivoExtra['crtdenominacao'];
                    if ( ($estadoObra == ESDID_OBJ_CONCLUIDO || $estadoObra == ESDID_OBJ_CANCELADO || $estadoObra == ESDID_OBJ_INACABADA) &&
                        (!$execucaoFinanceiraFinalizada)) {
                        echo "&nbsp;<img src='../imagens/alterar.gif' title='Editar Aditivo' width='16' style=cursor:pointer; 
                                onclick='editarAditivoExtra(" .$idObraOriginal.",".$entidempresa. ",".$aditivoExtra['crtid'].")'>&nbsp;
                                <img src='../imagens/excluir.gif' title='Excluir Aditivo' width='16' style=cursor:pointer; 
                                onclick='excluirAditivoExtra(" . $aditivoExtra['crtid'] . ")'>&nbsp;";
                    }
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Valor do Aditivo:</td>
                <td>
                    <?php
                    echo formata_numero_monetario($aditivoExtra['crtvalorexecucao']);
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Data de Assinatura do Aditivo:</td>
                <td>
                    <?php
                    echo formata_data($aditivoExtra['crtdtassinatura']);
                    ?>
                </td>
            </tr>
            <tr>
                <td width="265" class="subtitulodireita">Justificativa:</td>
                <td>
                    <?php
                    echo nl2br($aditivoExtra['crtjustificativa']);
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Anexo Assinado
                    digitalizado em PDF:
                </td>
                <td>
                    <?php
                    if($aditivoExtra['arqid']){
                        $arquivo = new Arquivo($aditivoExtra['arqid']); ?>
                        <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                        <?php
                        echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                    }
                    ?>
                </td>
            </tr>
        </table>
        <?php } ?>
        </td>
    </tr>
    <?php
    }

}
?>
    <tr bgcolor="#DEDEDE">
        <td colspan="3" align="center" style="font-size: 15px">
            <?php
            if ( ($estadoObra == ESDID_OBJ_CONCLUIDO || $estadoObra == ESDID_OBJ_CANCELADO || $estadoObra == ESDID_OBJ_INACABADA) && (!$execucaoFinanceiraFinalizada)) {
                ?>
                Deseja incluir outra empresa contratada? <input type="button" id="addConstrutora" onclick="incluirContrato()"
                                                                value="Sim" style="cursor: pointer"/>
                <?php
            }

            ?>
        </td>
    </tr>

    <tr>
        <td colspan="3" valign="top" id="td_obra_contrato"><br><br>
            <?php
            foreach ($arrConstrutoraExtra as $construtora) {
                $contratoOriginalExtra = $execucaoFinanceira->buscaContratoOriginalExtras($construtora['cexid']);


                $imgEditarConstrutora = "<img src='../imagens/alterar.gif' title='Editar Construtora / Inserir aditivo de valor' width='16' style=cursor:pointer; onclick='editarConstrutora(".$construtora['cexid'].")'>&nbsp;";
                $imgExcluirConstrutora = "<img src='../imagens/excluir.gif' title='Excluir Construtora' width='16' style=cursor:pointer; onclick='excluirConstrutora(".$construtora['cexid'].")'>&nbsp;";
                $imgPendenciaConstrutora = "";

                if($construtora['ccedataassinatura'] != ""){
                    $numContrato = $contratoOriginalExtra['ccenumero'];
                }
                else{
                    $imgPendenciaConstrutora = "<img src='../imagens/atencao.png' title='Pendência de Preenchimento Construtora' width='16' style=cursor:pointer; onclick='javascript:return false;'>&nbsp;";
                    $numContrato = "Construtora não possui contrato assinado";
                }

                monta_titulo('Contrato Adicional', '');
                ?>

                <table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem"
                       style="margin-bottom: 10px;">
                    <tr>
                        <td width="265" class="subtitulodireita">Licitação:</td>
                        <td>
                            <?php
                            echo $construtora['lienumero'] . " - " . $construtora['moldsc'];
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Empresa Contratada:</td>
                        <td>
                    <span id="entnomeempresa"><?php echo "(".formatar_cnpj($construtora['cexnumcnpj']).") " .$construtora['cexrazsocialconstrutora'] . "&nbsp;&nbsp;";
                        if(!$execucaoFinanceiraFinalizada){
                            echo $imgEditarConstrutora . $imgExcluirConstrutora;
                        }
                        echo $imgPendenciaConstrutora;
                        ?></span>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Número do Contrato:</td>
                        <td>
                            <span><?php echo $numContrato; ?></span>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data de Assinatura do Contrato:</td>
                        <td>
                            <span><?php echo formata_data($contratoOriginalExtra['ccedataassinatura']); ?></span>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Valor do Contrato (R$):</td>
                        <td>
                            <span><?php echo formata_numero_monetario($contratoOriginalExtra['ccevalor']); ?></span>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Contrato digitalizado em PDF:</td>
                        <td>
                            <?php if($contratoOriginalExtra['arqid']){
                                $arquivo = new Arquivo($contratoOriginalExtra['arqid']); ?>
                                <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                                <?php
                                echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                            }?>
                        </td>
                    </tr>
                    <tr>
                        <?php
                        $arrAditivoExtra = $execucaoFinanceira->buscaAditivoExtra($construtora['cexid'], $contratoOriginalExtra['cceid']);
                        if(count($arrAditivoExtra) > 0){
                            ?>
                            <td colspan="3" valign="top" align="left">
                                <div style="margin-bottom: 7px;">
                                    Aditivos do Contrato Adicional Número: <?php echo $contratoOriginalExtra['ccenumero']; ?>
                                </div>
                                <?php foreach ($arrAditivoExtra as $aditivo){ ?>
                                    <table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem"
                                           style="margin-bottom: 10px;">
                                        <tr>
                                            <td class="SubTituloDireita" width="20%">Tipo de Aditivo:</td>
                                            <td>
                                                <span><?php echo $aditivo['ttadsc'];?></span>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="20%">Número do Aditivo:</td>
                                            <td>
                                                <span><?php echo $aditivo['ccenumero'];?></span>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="20%">Data de Assinatura do Aditivo:</td>
                                            <td>
                                                <span><?php echo formata_data($aditivo['ccedataassinatura']);?></span>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="20%">Valor do Aditivo:</td>
                                            <td>
                                                <span><?php echo formata_numero_monetario($aditivo['ccevalor']);?></span>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="20%">Aditivo digitalizado em PDF:</td>
                                            <td>
                                                <?php if($aditivo['arqid']){
                                                    $arquivo = new Arquivo($aditivo['arqid']); ?>
                                                    <img src="../imagens/salvar.png" style=cursor:pointer; onclick="downloadArquivo(<?= $arquivo->arqid;?>)">
                                                    <?php
                                                    echo $arquivo->arqnome . "." . $arquivo->arqextensao;
                                                }?>
                                            </td>
                                        </tr>
                                    </table>
                                <?php }?>
                            </td>
                        <?php }?>
                    </tr>
                </table>
                <?php
            }
            ?>
        </td>
    </tr>
</table>


<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>

<?php  
    require APPRAIZ . 'obras2/includes/principal/listaExecFinanceiraContratacao/js.php';
?>


