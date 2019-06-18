<?php
require APPRAIZ . '\obras2\includes\principal\popUpPagamentoExecFinanceira\ctrl.php';
?>

<body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="js/obras2.js?v=2"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<?php 
	require APPRAIZ . '\obras2\includes\principal\popUpPagamentoExecFinanceira\css.php'; 
?>

<form name=formulario id=formulario method=post enctype="multipart/form-data">
	<input type="hidden" name="requisicao" id="requisicao" value="salvar">
	<input type="hidden" id=pgtid   name=pgtid value="<?=$pgtid ?>">
	<input type="hidden" id=tem_anexos  name=tem_anexos value="<?=$temDocumentos ?>">
	<input type="hidden" name=edicao_arquivo id=edicao_arquivo value="<?=$edicao ?>">
    <!-- FORMULÁRIO DE INCLUSÃO DE MEDIÇÕES -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTitulocentro" colspan="2"><?php echo ($edicao)  ? "Editar Pagamento" : "Incluir Pagamento";?></td>
        </tr>

        <tr>
            <td>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">
					
					 <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
							Data da Transação Bancária:
                        </td>
                        <td> 
	
                            <?= campo_data2('pgtdtpagamento', 'N', 'S', '', '', '','', $dadosEdicao['pgtdtpagamento']); ?>
                            <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
					<tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
							Nº da Transação Bancária:
                        </td>
                        <td>
                            <input type="text" style="text-align:left;" name="pgtnumtransacao"
                                   id="pgtnumtransacao" size="25" maxlength="25" value="<?php echo $dadosEdicao['pgtnumtransacao'];  ?>"
                                   onKeyUp="this.value = this.value.toUpperCase();"
                                   onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();"
                                   onmouseout="MouseOut(this);" onblur="MouseBlur(this);"
                                   title="Número da transação bancária." class="obrigatorio normal"
                            />
                            <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'/>
                        
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                          Tipo de Pagamento:
                        </td>
                        <td>
						<?php 
							//tptid
							//obras2.tipopagamentotransacao
							//@todo
							
							$sql = "SELECT DISTINCT
										tpt.tptid AS codigo,
										tpt.tptdesc AS descricao
									FROM
										obras2.tipopagamentotransacao tpt
									ORDER BY tptid ASC";
							
							$db->monta_combo("tptid", $sql, 'S', "Selecione...", '', '', '', '', 'S', "tptid", '', $dadosEdicao['tptid']);
							
						?>
                        
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                          Valor da Transação Bancária:
                        </td>
                        <td>
                        		<?php 
                        		if($edicao)
                        		{
                        			$valor = number_format($dadosEdicao['pgtvalortransacao'], 2, ",", ".");
                        		}
                        		else
                        		{
                        			$valor =  "0,00";
                        		}
                        		?>
                                <input
                                        type="text"
                                        style="text-align:left;"
                                        name="pgtvalortransacao"
                                        id="pgtvalortransacao"
                                        onmouseout="MouseOut(this);"
                                        onfocus="MouseClick(this);this.select();"
                                        onmouseover="MouseOver(this);"
                                        onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                                        value="<?php echo $valor;?>" <?php // @todo tratar depois?>
                                        maxlength="18"
                                        size="17"
                                        class="obrigatorio normal"
                                />
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Transação Bancária:
                        </td>
                        <td>
                        
                            <?php
                            if($edicao){
                            	if($dadosEdicao['arqid'])
                            	{
                            		$arquivo = recuperaDadosArquivo($dadosEdicao['arqid']);
                            			
                            
                            ?>
                            <div class="divArquivoDownload">
                                <a class="link" onclick="excluirArquivo('<?=$dadosEdicao['arqid']?>');">
                                    <img src="/imagens/excluir.gif" title="Excluir arquivo <?=$arquivo['arqnome']?>">
                                </a>
                                <a class="link" onclick="downloadArquivo(<?=$arquivo['arqid']; ?>)" alt='Efetuar download do anexo do Contrato' title='Efetuar download do anexo do Contrato'>
                                    <img src='/imagens/salvar.png' border='0'>
                                    <?= $arquivo['arqnome'] .'.'. $arquivo['arqextensao'] ?>
                                </a><br>
                            </div>
                           <?php }		
                            
                            ?>
                            <div class="divArquivoUpload" style="display: none">
                                <input type="hidden" name="arqidOld" id="arqidOld" value=""/>
                                <input type="file" name="arquivo" id="arquivo"/>
                                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                <font face="Verdana" size="1" color="red">Somente arquivo no formato PDF.</font>
                            </div>
                            <?php } else {?>
                                 <div class="divArquivoUpload">
                                    <input type="hidden" name="arq_contrato_??" id="arq_contrato_??" value=""/>
                                    <input type="file" name="arquivo" id="arquivo"/>
                                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><br/>
                                    <span style="color: #FF0000">Somente arquivo no formato PDF.</span>
                                </div>
                            <?php }  ?>
                                
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                           Favorecido:
                        </td>
                        <td>
                             
                             <?php
                            		montaComboEmpresaContratada($obrid,$empresa);
                             ?>
                        </td>
                    </tr>
                     
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO DE INCLUSÃO DE MEDIÇÕES. -->

    <!-- FORMULÁRIO DE CONTRATO/ADITIVO DE VALOR. -->
    <table width="95%" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTitulocentro" colspan="2">Documentos</td>
        </tr>

        <tr>
            <td>
                <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"
                       align="center">

                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
							Contrato ou Aditivo de Valor:
                        </td>
                        <td>
                            <?php 
                            /*
                             * crtid
                             * cceid
                             * @todo aqui apenas os de valor por parte crtid
                             * @TODO TROCAR POR id desta tabela
                             * */
                            if ($medid) {
                            	if ($tipoEmpresaContratada === "ent") {
                            		echo montaComboContratosAditivos($obrid, $idEmpresaContratada, null, $medid);
                            	}
                            
                            	if ($tipoEmpresaContratada === "cex") {
                            		echo montaComboContratosAditivos($obrid, null, $idEmpresaContratada, $medid);
                            	}
                            } else {
                            	?>
								<select name="contratos" id="contratos" onchange="montaNumeroDocumento(this.value)" >
									<option value="">Selecione...</option>
                                </select>
                                                            <?php
							}
                            ?>
                            
                            <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'/>
                        </td>
                    </tr>
					 <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                           Tipo de Documento
                        </td>
                        <td>
                          <?php 
                          /*
                           * tdtid
                           * //@todo aqui é fixo o tipo "Nota Fiscal"
                           * 
                           * */
                          
                          ?>
                          <select name="tdtid" id="tdtid" disabled=disabled>
                          	<option disabled=disabled value="1" selected=selected >
                          		Nota Fiscal
                          	</option>
                          </select>
                           <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Nº do Documento:
                        </td>
                        <td>
                         	<?php 
                         	/*
                         	 * //@todo duvida este a listagem de "Notas fiscais"
                         	 * @todo implementar a questão quando escolher o contrato
                         	 * @todo adicionar o campo ntfid da documentotransacao
                         	*/
                         	?>
                         	 <select name="ntfid" id="ntfid" onchange="retornaValorNota(this.value)" > 
	                          	<option  value="">
	                          		Selecione...
	                          	</option>
	                          	
                          </select>
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                    
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor do Documento:
                        </td>
                        <td style="background-color: #DCDCDC">
                           		<span id="valor_nota"></span>
                           		<input name="dotvalordocumento" id="dotvalordocumento" type="hidden">
                              <?php 
                              /*
                               *dotvalordocumento // 
                               * @todo add de acordo com o que selecionar no campo acima
                               * */
                               ?>
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%">
                            Valor Pago na Transação:
                        </td>
                        <td style="background-color: #DCDCDC">
                           
                             <?php 
                        		
                        		$valor =  "0,00";
                        		
                        		?>
                                <input
                                        type="text"
                                        style="text-align:left;"
                                        name="dotvalorpagodoc"
                                        id="dotvalorpagodoc"
                                        onmouseout="MouseOut(this);"
                                        onfocus="MouseClick(this);this.select();"
                                        onmouseover="MouseOver(this);"
                                        onkeyup="this.value=mascaraglobal('[.###],##',this.value);"
                                        value="<?php echo $valor;?>" <?php // @todo tratar depois?>
                                        maxlength="18"
                                        size="17"
                                        class="obrigatorio normal"
                                />
                                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."/>
                        </td>
                    </tr>
                    
                    <tr>
	                    <td colspan="2">
	                    	<center>
								<input type="button" value="Adicionar Pagamento" onclick="atualizarListaDocumentos()"/>
	                    	</center>
	                   	<td>
                    </tr>
                    
                </table>
            </td>
        </tr>
    </table>
    <!-- FIM DO FORMULÁRIO  DE INCLUSÃO DE MEDIÇÕES. -->

    <br/>
               		<div id="lista_excluidas_edicao">
               			
               		</div>
   
        <div id="acompanhamento" style="display: show">

            <!-- INÍCIO DA TABELA DE ACOMPANHAMENTO. -->
            <table width="95%" align="center" border="1" style="border-collapse: collapse" id="tbAcompanhamento">
                <thead>
               <tr>
                    <td class="tbAcompanhamentoTitulo">Ações</td>
                    <td class="tbAcompanhamentoTitulo">Número do Documento</td>
                    <td class="tbAcompanhamentoTitulo">Tipo de Documento</td>
                    <td class="tbAcompanhamentoTitulo">Valor do Documento</td>
                    <td class="tbAcompanhamentoTitulo">Valor Pago na Transação</td>     
                </tr>
                </thead>
                <tbody>
                <?php 

                if(  $edicao )
                {
                	echo "<input type='hidden' name='edicao[]'  id='edicao' value='1' >";
                }

                if( $temDocumentos && $edicao )
                {
                	
                	$arrDocumentos = ($arrDocumentos) ? $arrDocumentos : array();
                	if(count($arrDocumentos) > 0 )
                	{
                		echo "<input type='hidden' id=total_aditivos value={$totalDocumentos}>";
                		
	                	foreach( $arrDocumentos as $key => $value )
	                	{
	                		$valorAtual		= formata_numero_monetario($value['valor_documento']);
	                		$valorPago		= formata_numero_monetario($value['dotvalorpagodoc']);

	                		echo "<tr>"
			                  . "<td> 
    								<input name='dotid[]' value='".$value['dotid']."' type='hidden'> 
    								<input id='listaNtfId' name='listaNtfId[]' value='".$value['ntfid']."-".$value['contrato']."' type='hidden'> 
    								<span class='acaoTbAcompanhamento' alt='Remover Nota Fiscal' title='Remover Nota Fiscal' onclick='removerAditivoEdicao(this, 
    								" . $idControleObjDocumento . ", ".$value['dotid']." , ".$value['ntfid'].", \"" . $value['contrato'] . "\")'>
    								<img src='/imagens/excluir.gif' alt='Excluir Aditivo'/></span>
    								<span class='acaoTbAcompanhamento' alt='Editar Nota Fiscal' title='Editar Nota Fiscal' 
    								onclick='editarAditivoEdicao(this, " . $idControleObjDocumento . ", ".$value['dotid'].", ".$value['numero_documento'].",
    								".$value['valor_documento'].", ".$value['dotvalorpagodoc'].",  ".$value['ntfid'].", \"" . $value['contrato'] . "\")'>
    								<img src='/imagens/alterar.gif' alt='Editar Nota Fiscal'/></span>
								</td>"
								. "<td>" . $value['numero_documento'] . "</td>"
								. "<td>" . $value['tipo_documento'] . "</td>"
			                  	. "<td>" . $valorAtual . "</td>"
			                  	. "<td>" . $valorPago . "</td>"
			                  . "</tr>";
							  $idControleObjDocumento++;
	                	}
                	}
                }
                
                ?>
                
                </tbody>
            </table>
            <!-- FIM DA TABELA DE ACOMPANHAMENTO. -->

            <br/>

            <div style="text-align: center">
            
            	<center>
               	<div id="loading" style="text-align: center; display: none"><img src="../imagens/wait.gif" />&nbsp;Aguarde.</div>
               </center>
            </div>
            
        </div>
        <center>
        		<input type="button" id="botaoSalvar" value="Salvar" onclick="salvarDocumento();"> 
               	<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
         </center>
      
</form>

<?php 
	require APPRAIZ . '\obras2\includes\principal\popUpPagamentoExecFinanceira\js.php'; 
?>

</body>

