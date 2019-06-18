<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );
require APPRAIZ . 'obras2/includes/principal/cadObraRecursos/ctrl.php';

echo cabecalhoObra($obrid);
echo "<br>";
//monta_titulo( 'Recursos da Obra', '' );
monta_titulo_obras( 'Recursos da Obra', '' );

$tipoOrigemObra = new TipoOrigemObra();

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<form method="post" name="formRecursos" id="formRecursos">
	<input type="hidden" name="req" id="req" value=""/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
                    <td style="width:20%" class="SubTituloDireita">Tipo de Instrumento</td>
                    <td>
                        <?php
                            $tipoFormaRepasseRecursos = new TipoFormaRepasseRecursos();
                            $sql                      = $tipoFormaRepasseRecursos->listaCombo($frpid);
                            $db->monta_combo('frpid', $sql, $habilita, "Selecione...", 'abreCamposRecursos', '', '', '150', 'S', 'frpid');
                        ?>
                    </td>
		</tr>
        <tr>
            <td style="width:20%" class="SubTituloDireita">Situação do Instrumento</td>
            <td>
                <?php
                $tipoSituacaoInstrumento = new SituacaoInstrumento();
                $sql                      = $tipoSituacaoInstrumento->listaCombo();
                $db->monta_combo('stiid', $sql, $habilita, "Selecione...", '', '', '', '150', 'S', 'stiid');
                ?>
            </td>
        </tr>
        <tr>
            <td style="width:20%" class="SubTituloDireita">Data de Vigência</td>
            <td>
                <?php echo campo_data2( 'dtvigencia', 'N', 'S', '', 'S' );
                ?>
            </td>


        </tr>
        <tr>
            <td class="SubTituloDireita">Medidas de Exceção</td>
            <td>
                <input type="radio" name="medidasexcecao" <?=($habilita == 'N') ? "disabled" : ""?> id="" value="S" <?= ( $medidasexcecao == 't' ? "checked='checked'" : "" ) ?>/> Sim
                <input type="radio" name="medidasexcecao" <?=($habilita == 'N') ? "disabled" : ""?> id="" value="N" <?= ( $medidasexcecao == 'f' ? "checked='checked'" : "" ) ?>/> Não
            </td>
        </tr>
		<tr>
                    <td class="SubTituloDireita">Fonte:</td>
                    <td>
                        <?php
                            $sql = $tipoOrigemObra->listaComboRecursos($obras->preid);
                            $db->monta_combo('tooid',$sql, 'N','Selecione...','','','',200,'N', 'tooid', '', '', '');
                        ?>
                    </td>
		</tr>
        <?php
        $sql = getSqlConvenio($_SESSION["obras2"]["obrid"]);

        $rsConvenio = $db->pegaLinha($sql);
        ?>
<!-- aqui -->
		<tr id="convenio" style="<? if( !empty($obras->preid) || !empty($obras->obrid_par3) ) echo 'display:none'?>">
			<td colspan="2">


				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
					<tr>
						<td>Convênio
                            <? if (!$rsConvenio['dcoid']): ?>
                                <span style="color: red">(dados bancários não encontrados)</span>
                            <? endif; ?>
                        </td>
					</tr>
					<tr>
                        <td class="SubTituloDireita">Número do Convênio</td>
                        <td>
                            <?
                                $covnumero = $rsConvenio['dcoconvenio']; //$dados_convenio["covnumero"];
                                echo campo_texto( 'covnumero', 'N', 'N', '', 22, 20, '', '', 'left', '', 0, 'id="covnumero"');
                                if($habilitado == 'N'){
                                    echo '<!--
                                          <input type="button" name="busca_convenio" value="Pesquisar" style="cursor: pointer;" onclick="window.open( \'?modulo=principal/inserir_convenio&acao=A\', \'blank\', \'height=600, width=800, status=no, toolbar=no, menubar=no, scrollbars=yes, location=no, resizable=no\');"/>
                                          <input type="button" name="limpa_convenio" value="Remover" style="cursor: pointer;" onclick="removeConvenio();"/>
                                          -->';
                                }
                            ?>
                                <input type="hidden" name="covid" id="covid" value="<?php if($covid){ echo $covid; } ?>"/>

                            <?php
                                $sql = getSqlObrasComMesmoConvenio($numconvenio,  $obranoconvenio);

                                $rsObrasComMesmoConvenio = $db->pegaUm($sql);
                                if($rsObrasComMesmoConvenio > 1):
                            ?>
                                    <script type="text/javascript">
                                        function popupConvenioObras(covnumero){
                                            var popUp = window.open('?modulo=principal/cadObraRecursos&acao=A&req=exibeObrasConvenio&covnumero='+covnumero, 'popupConvenioObras', 'height=600,width=800,scrollbars=yes,top=0,left=0');
                                            popUp.focus();
                                        }
                                    </script>
                                    <a href="javascript:void(0)" onclick="popupConvenioObras('<?=$numconvenio; ?>')">
                                            &nbsp;Obras com o mesmo convênio
                                    </a>
                            <?php endif; ?>
                        </td>
					</tr>
					<tr>
                        <td class="SubTituloDireita">Ano</td>
                        <td>
                            <?php
                                $covano = $rsConvenio['dcoano']; //$dados_convenio["covano"];
                                echo campo_texto( 'covano', 'N', 'N', '', 4, 5, '', '', 'left', '', 0, 'id="covano"');
                            ?>
                        </td>
					</tr>
					<tr>
                        <td class="SubTituloDireita">Processo</td>
                        <td>
                            <?php
                                $covprocesso = $rsConvenio['dcoprocesso']; //$dados_convenio["covprocesso"];
                                echo campo_texto( 'covprocesso', 'N', 'N', '', 22, 20, '', '', 'left', '', 0, 'id="covprocesso"');
                            ?>
                        </td>
					</tr>
					<tr>
                        <td class="SubTituloDireita">Concedente</td>
                        <td>
                            <?php
                                $covvlrconcedente = $rsConvenio['dcovalorconcedente']; //$dados_convenio["covvlrconcedente"];
                                $covvlrconcedente = number_format($covvlrconcedente,2,',','.');
                                echo campo_texto( 'covvlrconcedente', 'N', 'N', '', 22, 17, '###.###.###,##', '', 'left', '', 0, 'id="covvlrconcedente"');
                            ?>
                        </td>
					</tr>
					<tr>
                        <td class="SubTituloDireita">Convenente</td>
                        <td>
                            <?php
                                $covvlrconvenente = $rsConvenio['dcovalorcontrapartida']; //$dados_convenio["covvlrconvenente"];
                                $covvlrconvenente = number_format($covvlrconvenente,2,',','.');
                                echo campo_texto( 'covvlrconvenente', 'N', 'N', '', 22, 17, '###.###.###,##', '', 'left', '', 0, 'id="covvlrconvenente"');
                            ?>
                        </td>
					</tr>
					<tr>
                        <td class="SubTituloDireita">Valor R$</td>
                        <td>
                            <?php
                                $covvalor = $rsConvenio['dcovalorcontrapartida']+$rsConvenio['dcovalorconcedente']; //$dados_convenio["covvalor"];
                                $covvalor = number_format($covvalor,2,',','.');
                                echo campo_texto( 'covvalor', 'N', 'N', '', 22, 17, '###.###.###,##',  '', 'left', '', 0, 'id="covvalor"');
                            ?>
                        </td>
					</tr>
					<tr>
                        <td class="SubTituloDireita">Início</td>
                        <td>
                            <?php
                                $covdtinicio = $rsConvenio['dcodatainicio']; //$dados_convenio["covdtinicio"];
                                echo campo_data2( 'covdtinicio', 'N', 'N', '', 'S' );
                            ?>
                        </td>
					</tr>
					<tr>
                        <td class="SubTituloDireita">Fim</td>
                        <td>
                            <?php
                                $covdtfinal = $rsConvenio['dcodatafim']; //$dados_convenio["covdtfinal"];
                                echo campo_data( 'covdtfinal', 'N', 'N', '', 'S' );
                            ?>
                        </td>
					</tr>
					<tr>
						<td colspan="2">Saldos Bancários</td>
					</tr>
					<tr>
                        <td colspan="2">
                            <div id="covaditivo">
                            <?php
                                $sqlSaldoConta = getSQLSaldoConta($rsConvenio['dcoprocesso']);

                                $cabecalho = array('Mês Ref.','Saldo Conta','Saldo Aplicações','Total Conta');
                                $db->monta_lista_simples( $sqlSaldoConta, $cabecalho, 1000, 10, 'N', '90%', '' );
                             ?>
                            </div>
                        </td>
					</tr>
					<tr>
                        <td colspan="2">Repasses</td>
					</tr>
					<tr>
                        <td colspan="2">
                            <div id="covaditivo">
                            <?php
                                $sqlDadosBancarios = getSqlDadosBancarios($rsConvenio['dcoprocesso']);
                                
                                $cabecalho = array('Data do Repasse','OB','Valor Repassado','Dados Bancários');
                                $db->monta_lista_simples( $sqlDadosBancarios, $cabecalho, 1000, 10, 'N', '90%', '' );
                             ?>
                            </div>
                        </td>
					</tr>

				</table>
			</td>
		</tr>
<?php

if($obras->preid || $obras->obrid_par3) {
    $preid = $obras->preid;
    $obridPar3 = $obras->obrid_par3;

     if ($tooid == 1 || $tooid == 11 || $tooid == 4){ ?>
        <tr>
            <th colspan="2">Pagamentos</th>
        </tr>

        <tr>
            <td colspan="2">
                <?php $dadosPagemento = $obras->getPagamentoPar($obras->obrid); ?>
                <?php if ($dadosPagemento){ ?>
                    <?php
                    $cabecalho = array('Nº do Termo', 'Número processo', 'Número do Empenho', 'Valor do Pagamento', 'Percentual pagamento', 'Parcela', 'Data pagamento', 'Situação', 'Valor FNDE', 'Valor Contrapartida', 'Valor Total da Obra');
                    $db->monta_lista_simples($dadosPagemento, $cabecalho, 50, 5, array('4' => '4', '3' => '3'), '100%', 'S', false, false, false, false);
                    ?>
                <?php }else{ ?>
                    Nenhum pagamento efetuado.
                <?php }//endif; ?>
            </td>
        </tr>
    <?php }// endif; ?>
    <tr>
        <th colspan="2">Financeiro</th>
    </tr>
    <tr>
        <td colspan="2">
            <table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                <tr>
                    <td class="SubTituloDireita" valign="top"><b>Termo:</b></td>
                    <td valign="top">
                        <?php
                        /* Trecho que preenche a coluna termo */
                        $sql =  getSQLTermoObra($preid, $obridPar3 );

                        $termoObra = $db->pegaLinha($sql);

                        if ($termoObra) {
                            if ($termoObra['terassinado'] == "f") {
                                echo "Gerado (" . ($termoObra['terdatainclusao']) . ")";
                            } elseif ($termoObra['terassinado'] == "t") {
                                echo "Assinado";
                            }
                            if ($termoObra['arqid']) {
                                echo " <a href=\"/par/par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&download=S&arqid=" . $termoObra['arqid'] . "\" ><img align=absmiddle src=../imagens/anexo.gif border=0></a>";
                            }
                        } else {
                            echo "Não gerado";
                        }
                        /* FIM - Trecho que preenche a coluna termo */
                        ?>
                    </td>
                    <td class="SubTituloDireita" valign="top"><b>Empenho:</b></td>
                    <td valign="top">
                        <?php
                        /* Trecho que preenche a coluna empenho */
                        $sql =  getSQLEmpenhoObra($preid, $obridPar3 );

                        $empenhoObra = $db->pegaLinha($sql);

                        if ($empenhoObra) {
                            if ($empenhoObra['empnumero']) {
                                echo "Gerado (R$" . number_format($empenhoObra['empvalorempenho'], 2, ",", ".") . " - " . $empenhoObra['empnumero'] . ")";
                            } else {
                                echo "Não gerado";
                            }
                        } else {
                            echo "Não gerado";
                        }
                        /* FIM - Trecho que preenche a coluna empenho */
                        ?>
                    </td>
                    <td class="SubTituloDireita" valign="top"><b>Pagamento:</b></td>
                    <td valign="top">
                        <?php

                        $sql  =  getSQLPagamentoObra($preid, $obridPar3);

                        $pagamentoObra = $db->pegaLinha($sql);

                        if ($pagamentoObra) {
                            echo "Pago<br>
                                      Valor pagamento(R$): " . number_format($pagamentoObra['pagvalorparcela'], 2, ",", ".") . "<br>
                                      Nº da Ordem Bancária: " . $pagamentoObra['pagnumeroob'] . "<br>
                                      Data de Solicitação do pagamento: " . $pagamentoObra['pagsolicdatapagamento'] . "<br>
                                      Data do pagamento: " . $pagamentoObra['pagdatapagamento'] . "<br>
                                      Banco: " . $pagamentoObra['probanco'] . ", Agência: " . $pagamentoObra['proagencia'] . "
                                      ";
                        } else {
                            echo "Não pago";
                        }
                        /* FIM - Trecho que preenche a coluna pagamento */
                        ?>
                    </td>
                </tr>
            </table>
        </td>
    </tr>
    <?php
}
            if( $empenhoObra['empnumero'] ){

                $dadosEmpenho = $db->pegaLinha(getSqlDadosEmpenho($empenhoObra['empnumero']));

                $processo = $dadosEmpenho['empnumeroprocesso'];
            if( $processo ){?>
				<tr>
					<!--<th colspan="2">Saldos</th>-->
					<th colspan="2">Extrato</th>
				</tr>
				<tr>
                    <td colspan="2">

                        <script type="text/javascript" src="../../includes/remedial.js"></script>
                        <script type="text/javascript" src="../../includes/superTitle.js"></script>
                        <link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
                         <?php

                        $cnpj = $dadosEmpenho['empcnpj'];

                        $sql = getSqlDadosFinanceiros($processo, $cnpj );

                        $cabecalho = array('Processo', 'CNPJ', 'Razão Social', 'Banco', 'Agência', 'Conta', 'Saldo Conta', 'Saldo Fundos', 'Saldo Poupança', 'Saldo CDB','Mês/Ano Referência');
                        $db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', 'S', false, false, false, false);
                        
                        ?>
                    </td>
                </tr>
		<?php }
		} ?>
		<tr class="divTituloPadrao">
			<td></td>
			<td>
				<div style="float: left;">
					<!--
					<input type="hidden" name="frrid" value="<?=$frpid?>">
					-->
					<input type="hidden" name="obrid" value="<?=$_SESSION["obras2"]["obrid"];?>">
					<input type="button" id="salvar" value="Salvar" style="cursor: pointer" <?php if( $habilita == "N") { echo "disabled"; } ?>>
					<input type="button" value="Voltar" id="btnVoltar" style="cursor: pointer" onclick="history.back(-1);">
				</div>
			</td>
		</tr>
	</table>
</form>

<?php

    require APPRAIZ . 'obras2/includes/principal/cadObraRecursos/js.php';

    $objObras = new Obras($obrid);
    $blockEdicao = $objObras->verificaObraVinculada();
    if($blockEdicao){
        echo '<script type="text/javascript">';
        echo " setTimeout(bloqueiaForm('formRecursos'), 500);
               function bloqueiaForm(idForm){
                  jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                  jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                  jQuery('#btnVoltar').attr('disabled', false);
               }
             ";
        echo '</script>';
    }
?>
