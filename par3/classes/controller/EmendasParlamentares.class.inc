<?php
class Par3_Controller_EmendasParlamentares extends Controle
{
    //public $arChavePrimaria, $stNomeTabela, $arAtributos;

    public function __construct()
    {
        $this->model = new Par3_Model_EmendasParlamentares();
    }
    
    public function listar($arrPost = array())
    {
        
        $sql = $this->model->montaSQLLista($arrPost);
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Emenda','Ano', 'Parlamentar', 'Beneficiário', 'UF', 'Situação', 'Valor Total da Emenda', 'Valor Designado', 'Valor Empenhado', 'Valor Total Pago');
        $esconderColunas = array('edeid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addAcao('edit', array('func' => 'vincularEmendaIniciativa', 'extra-params' => array('edeid', 'emecod')));
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        /*$listagemSimec->setAcaoComoCondicional('remove', [['campo' => 'vlrempenho', 'valor' => '0', 'op' => 'igual']]);*/
        $listagemSimec->addCallbackDeCampo(['emevalor', 'edevalor', 'valor_empenho', 'valor_pago'], 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addCallbackDeCampo(['autor', 'enbnome', 'situacao', 'iniciativa'], 'formata_Tipo_Texto_Tabela');
        $listagemSimec->setFormFiltros('form-lista-emenda');
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function listarVisaoMunicipio($arrPost = array())
    {
        
        $sql = $this->model->montaSQLListaVisaoMunicipio($arrPost);
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Emenda','Ano', 'Parlamentar', 'Beneficiário', "UF", 'Situação', 'Valor Total da Emenda', 'Valor Designado', 'Valor Empenhado', 'Valor Total Pago');
        $esconderColunas = array('edeid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->addAcao('edit', array('func' => 'detalharEmendaIniciativa', 'extra-params' => array('edeid', 'emecod')));
        $listagemSimec->setAcaoComoCondicional('edit', [['campo' => 'situacao', 'valor' => 'Em cadastramento', 'op' => 'diferente'],['campo' => 'situacao', 'valor' => '', 'op' => 'diferente']]);
//         $listagemSimec->addAcao('view', array('func' => 'removeProcesso', 'extra-params' => array( 'edeid', 'emecod')) );
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->addCallbackDeCampo(['emevalor', 'edevalor', 'valor_empenho', 'valor_pago'], 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addCallbackDeCampo(['autor', 'enbnome', 'situacao', 'iniciativa'], 'formata_Tipo_Texto_Tabela');
        $listagemSimec->setFormFiltros('form-lista-emenda');
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function listarItemComposicaoIniciativaBeneficiarioComVinculoParlamentar($arrPost = array())
    {
        global $simec;
        
        //      $inuid = $dados['inuid'];
        $ineid = $arrPost['dados'][0];
        $inpid = $arrPost['dados'][1];
        $inuid = $arrPost['dados'][2];
        
        $arrPerfilRetorno = $this->verificaPerfilEmendasEntidade($arrPost['edeid']);
        
        $sql = "SELECT
    				d.docid, ed.esdid, ed.esddsc
		    	FROM
		    		par3.iniciativa_emenda e
		    	INNER JOIN workflow.documento d ON d.docid = e.docid
		    	INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
		    	WHERE
    				e.inestatus = 'A' and
    				e.ineid = $ineid";
        $dadoWorkFlow = $this->model->pegaLinha($sql);
        
        $mInu = new Par3_Model_InstrumentoUnidade($inuid);
         
        $sql = "select
					intaid
				from
					par3.iniciativa_planejamento ip
				join par3.iniciativa i on i.iniid = ip.iniid
				where
					inpid = {$inpid}";
         
        $intaid = $this->model->pegaUm($sql);
        
        $modelIniciativaTiposAtendimento = new Par3_Model_IniciativaTiposAtendimento();
//      echo $intaid. " == " . $modelIniciativaTiposAtendimento::POR_ESCOLA;
        $condicaoPorEscola = 0;
        if ($intaid == $modelIniciativaTiposAtendimento::POR_ESCOLA) {
            $condicaoPorEscola = 1;
        }
        
//      $boLibera = false;
//      if ($dadoWorkFlow['esdid'] == PAR3_ESD_EMENDA_ANALISE_ENTIDADE &&
//          possuiPerfil(array(PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR,
//              PAR3_PERFIL_SUPER_USUARIO,
//              PAR3_PERFIL_PREFEITO,
//              PAR3_PERFIL_SECRETARIO_ESTADUAL ))){
                
//             $boLibera = true;
//      }

        $anoEmenda = $this->model->pegaUm("SELECT emeano FROM emenda.emenda WHERE emeid = {$arrPost['emeid']}");
        
        $modelIniciativaPlanejamentoItemComposicaoEscola = new Par3_Model_IniciativaPlanejamentoItemComposicaoEscola();
        
        $sql = $this->model->sqlItemComposicaoIniciativaBeneficiarioComVinculoParlamentar($inpid, $ineid, $anoEmenda);
        $dadosLinha = $this->model->carregar($sql);
        $arrRegistros = array();
        
        $anosEditaveis = $anosEstaticos = '';
        if (is_array($dadosLinha)) {
            $tabela = '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="dados-itens-composicao">
			    	<thead>
				    	<tr>
					    	<th>Ano</th>
					    	<th>Item</th>
					    	<th>Unidade de Medida</th>
					    	<th>Valor Referência(R$)</th>
					    	<th style="text-align: center">QTD Planejada</th>
					    	<th style="text-align: center">Valor Planejado</th>
					    	<!--<th style="text-align: center">Indicação?</th>-->
					    	<th style="text-align: center">QTD Indicada</th>
					    	<th style="text-align: center">Valor Indicado</th>
					    	<th style="text-align: center">Valor Total Indicado</th>
				    	</tr>
			    	</thead>
			    	<tbody>';
         
            $totalItemGeral = 0;
            $vlrTotalItemIndicado = 0;
            foreach ($dadosLinha as $itemAno) {
                $qtd = $itemAno['ipiquantidade'];
                
                $attribs['readonly'] = 'readonly';
                $attItemIndicado = array();
                if ($condicaoPorEscola) {
//                     if( $boLibera ){
                        $attribs['onclick'] = "abreModalEscolas({$ineid},{$inpid},{$itemAno['iniano']},{$itemAno['iigid']},{$itemAno['ipiid']},{$inuid},{$mInu->itrid})";
                        $attribs['style'] = 'cursor:pointer; text-align:center;';
                        $attribs['title'] = 'Clique no campo para adicionar a Quantidade de Equipamentos para as Escolas';
                        $attribs['help'] = '<small style="font: 8px">Clique acima para visualizar Escolas</small>';
                        
                        $attItemIndicado['readonly']    = 'readonly';
                        $attItemIndicado['onclick']     = "abreModalEscolas({$ineid},{$inpid},{$itemAno['iniano']},{$itemAno['iigid']},{$itemAno['ipiid']},{$inuid},{$mInu->itrid})";
                        $attItemIndicado['style']       = 'cursor:pointer; text-align:center;';
                        $attItemIndicado['title']       = 'Clique no campo para adicionar a Quantidade de Equipamentos para as Escolas';
                        $attItemIndicado['help']        = '<small style="font: 8px">Clique acima para visualizar Escolas</small>';
//                     } else {
//                         $attribs['style'] = 'cursor:pointer; text-align:center;';
//                      $attItemIndicado['readonly']    = 'readonly';
//                     }
                }
                
                if ($arrPerfilRetorno['vincular'] == 'N') {
                    $attItemIndicado['readonly']    = 'readonly';
                }
                
                $attribs['style']       .= 'text-align:right;';
                $attribs['maxlength']   = 7;
                $attribs['class']       = 'inteiro';
                $attribs['type']        = 'text';
                $attribs['id']          = 'qtd-item-' . $itemAno['iigid'] . '-' . $itemAno['iniano'];
                
                $radioItem = array('N' => 'Não', 'S' => 'Sim', 'T' => 'Sim com Saldo');
                $totalItem = ((float)$itemAno['valorreferencia'] * (int)$qtd);
                
                $descricao = $itemAno['itcdsc'];
                if( $_SESSION['usucpf'] == '' ) $descricao = $itemAno['ipiid'].' - '.$itemAno['itcdsc'];
                
                $itemAno['ieiquantidade'] = str_replace(".", "", $itemAno['ieiquantidade']);

                $attItemIndicado['style'] = 'text-align:right';
                $attItemIndicado['onkeyup'] = 'this.value=mascaraglobal(\'###############\',this.value)';
                $attItemIndicado['onblur'] = 'this.value=mascaraglobal(\'###############\',this.value)';
                
                $vlrIndicadoItem = ($itemAno['ieiquantidade'] ? ($itemAno['ieiquantidade'] * $itemAno['valorreferencia']) : 0);
                
                $tabela .= "<tr>
                				<td>".$itemAno['iniano']."</td>
                				<td id='nome-{$itemAno['iigid']}' style=\"text-align:left\">".$descricao."
                						<input type=\"hidden\" name=\"iigid[{$itemAno['iigid']}]\" value='{$itemAno['iigid']}'>
                						<input type=\"hidden\" name=\"inpid[]\" value='{$inpid}'>
                						<input type=\"hidden\" name=\"iniano[{$itemAno['iigid']}]\" value='{$itemAno['iniano']}'>
                						<input type=\"hidden\" name=\"inuid[]\" value='{$inuid}'>
                						<input type=\"hidden\" id=\"valor_ja_indicado_".$ineid."_".$itemAno['ipiid']."_".$inpid."\" value='".($itemAno['ieiquantidade'] ? $itemAno['ieiquantidade'] : '0')."'>
                						</td>
                				<td style='width: auto; text-align: left;'>".$itemAno['unidsc']."</td>
                				<td style=\"text-align:right\">".formata_valor($itemAno['valorreferencia'])." <input type=\"hidden\" name=\"valor_referencia_{$ineid}_{$itemAno['ipiid']}\" value='".$itemAno['valorreferencia']."'></td>
                				<td><input type=\"hidden\" name=\"ano-{$itemAno['iigid']}\" value='{$itemAno['iniano']}'>
                					".$simec->input('qtd-item_'.$ineid.'_'.$itemAno['ipiid'].'', null, $qtd, $attribs, array('input-size' => '12'))."</td>
                				<td style=\"text-align:right\">".formata_valor($totalItem)."</td>
                						
                				<!-- <td style='width: 150px;'>".$simec->radio('chk-item-aprovado['.$itemAno['iigid'].']['.$itemAno['iniano'].']', null, $itemAno['ipiaprovado'], $radioItem, $attribusCheck, array('type' => 'radio radio-info radio', 'style' => 'inline'))."</td> -->
                						
                				<td>".$simec->input('qtd-item-indicado_'.$ineid.'_'.$itemAno['ipiid'].'_'.$inpid, null, ($itemAno['ieiquantidade'] ? $itemAno['ieiquantidade'] : '0'), $attItemIndicado, array('input-size' => '12'))."</td>
                				<td style=\"text-align:right\"><div id='div_vlr_indicado_".$ineid."_".$itemAno['ipiid']."'>".formata_valor($vlrIndicadoItem)."</div></td>
                				<td style=\"text-align:right\"><div id='div_vlr_total_indicado_".$ineid."_".$itemAno['ipiid']."'>".formata_valor($itemAno['vlr_total_indicado'])."</div></td>
                			</tr>";
                $totalItemGeral = ($totalItemGeral + $totalItem);
                $vlrTotalItemIndicado = ($vlrTotalItemIndicado + $vlrIndicadoItem);
            }
            $tabela .= '<tr>
            				<td colspan="3" style="text-align:right"><b>Totais:</b></td>
            				<td>&nbsp;</td>
            				<td>&nbsp;</td>
            				<td style="text-align:right;">'.formata_valor($totalItemGeral).'</td>
            				<td>&nbsp;</td>
            				<td id="soma_valor_indicado_'. $inpid .'" style="text-align:right;">'.formata_valor($vlrTotalItemIndicado).'</td>
            				<td>&nbsp;</td>
            			</tr>';
            $tabela .= '<tr>
            				<td colspan="3" style="text-align:right"><b>Contrapartida:</b></td>
            				<td style="text-align:right;">
            					<input type="hidden" name="contrapartida" id="contrapartida_'.$inpid.'" value="">
            					<div id="contrapartida_txt_'.$inpid.'"></div>
            				</td>
            				<td>&nbsp;</td>
            				<td>&nbsp;</td>
            				<td>&nbsp;</td>
            				<td>&nbsp;</td>
            				<td style="text-align:right;">&nbsp;</td>
            			</tr>';
            $tabela .= '</tbody></table>';
            
            $tabela .= <<<HTML
				<div class="ibox float-e-margins animated modal" id="modal_escolas" tabdesex="-1" role="dialog" aria-hidden="true">
				    <div class="modal-dialog modal-lg">
				        <form method="post" name="form-escolas" id="form-escolas" class="form form-horizontal">
				            <div class="modal-content">
				                <div class="ibox-title">
				                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
				                    <h3 id="modal_titulo">Itens de Composição por Escola - <span id="itemnomemodal"></span></h3>
				                </div>
				                <div class="ibox-content">
				                    <div id="dadosbuscaescola" class="col-lg-offset-3">
				                        <input type="hidden" name="ineid" value="{$ineid}"/>
				                        <input type="hidden" name="inuid" value="{$inuid}"/>
				                        <input type="hidden" name="inpid" id="inpid" value="{$inpid}"/>
				                        <input type="hidden" name="iniano" id="iniano" value=""/>
				                        <input type="hidden" name="iigid" id="iigid" value=""/>
				                        <input type="hidden" name="ipiid" id="ipiid" value=""/>
HTML;
            if ($mInu->itrid == 1) {
                $modelMunicipio = new Par3_Model_Municipio();
                $sqlMunicipiosUF = $modelMunicipio->montaSQLComboMunicipiosUF($inuid);
                $tabela .= $simec->select('escmuncod', '', '', $sqlMunicipiosUF, array('maxlength' => '255','data-placeholder' => 'Selecione o Município'), array('input-size' => '8'));
            }if ($mInu->itrid == 2) {
                $tabela .= '<input type="hidden" name="escmuncod" id="escmuncod" value="'.$mInu->muncod.'"/>';
            }
                        $tabela .= <<<HTML
						                    </div>
				                    <div id="conteudo-modal" style="height: 500px;overflow-y:scroll;">
				
				                    </div>
				                </div>
				            </div>
				        </form>
				    </div>
				</div>
HTML;
            
            $tabela .= <<<HTML
					<script type="text/javascript">

						jQuery(document).ready(function () {
							jQuery('#contrapartida' + $inpid).val(  number_format(jQuery('#valorcontrapartida_$ineid').val(), 2, ",", ".") );
							jQuery('#contrapartida_txt_' + $inpid).html(  number_format(jQuery('#valorcontrapartida_$ineid').val(), 2, ",", ".") );
						});
					
 						if (typeof atualizaValoresItens != 'function'){
						    var atualizaValoresItens = function (ineid, ipiid, inpid) {
						        var qtd_aprovado = jQuery('[name="qtd-item-indicado_' + ineid + '_' + ipiid + '_' + inpid + '"]').val();
						        var qtd_item = jQuery('[name="qtd-item_' + ineid + '_' + ipiid + '"]').val();
						        var valor_referencia = jQuery('[name="valor_referencia_' + ineid + '_' + ipiid + '"]').val();
						        var vlrIndicado = parseFloat(valor_referencia) * parseFloat(qtd_aprovado);
						
	// 					        if (parseFloat(qtd_aprovado) > parseFloat(qtd_item)) {
	// 					        }
						        obCalc = new Calculo();
						        
						        var vlrIndicadoAntigo = jQuery('#div_vlr_indicado_' + ineid + '_' + ipiid).html();
						        var vlrTotalIndicado  = jQuery('#div_vlr_total_indicado_' + ineid + '_' + ipiid).html();
						        vlrTotalIndicado = obCalc.operacao(vlrTotalIndicado, vlrIndicadoAntigo, '-');
						        vlrTotalIndicado = obCalc.operacao(vlrTotalIndicado, vlrIndicado, '+');
						        
						        jQuery('#div_vlr_indicado_' + ineid + '_' + ipiid).html(number_format(vlrIndicado, 2, ",", "."));
						        jQuery('#div_vlr_total_indicado_' + ineid + '_' + ipiid).html(number_format(vlrTotalIndicado, 2, ",", "."));
	
						        var tableItemComposicao = jQuery('#div_vlr_total_indicado_' + ineid + '_' + ipiid).parents('table:first');
						        var somaVlrIndicadoItens = 0;
						        tableItemComposicao.find('[id*=div_vlr_indicado_]').each(function (){
	    							somaVlrIndicadoItens = obCalc.operacao(somaVlrIndicadoItens, jQuery(this).html(), '+');
	    						});
	    						
	    						tableItemComposicao.parents('tr:first').prev().find('td:last').html( number_format(somaVlrIndicadoItens, 2, ",", ".") );
	    						jQuery('#soma_valor_indicado_' + inpid).html( number_format(somaVlrIndicadoItens, 2, ",", ".") );
	    						//console.log( jQuery('#soma_valor_indicado_' + inpid).html() );
						    }
 					    }
						    
					    jQuery('[name*=qtd-item-indicado]').change(function (){
					    	var name = jQuery(this).attr('name');
					    	name = name.split('_');
					    	ineid = name[1];
					    	ipiid = name[2];
					    	inpid = name[3];
					    	
					    	atualizaValoresItens(ineid, ipiid, inpid);

                            var valorEmpenho = verificaValorEmpenho(inpid);
                            valorEmpenho = number_format(valorEmpenho, 2, ",", ".");
                            valorEmpenho = retiraPontosPAR3Emenda(valorEmpenho);
                            var soma_valor_indicado = retiraPontosPAR3Emenda(jQuery('#soma_valor_indicado_' + inpid).html());
                            var valor_ja_indicado = jQuery('#valor_ja_indicado_'+ineid+'_'+ipiid+'_'+inpid).val();

                            if( parseFloat(soma_valor_indicado) <  parseFloat(valorEmpenho) ){
                        		swal("Ação não Permitida!", "Já existe empenho para este planejamento. O valor indicado de R$ "+number_format(soma_valor_indicado, 2, ",", ".")+" não pode ser menor que o Valor Empenhado: R$ "+number_format(valorEmpenho, 2, ",", ".")+"!", "error");
                        		jQuery('#qtd-item-indicado_'+ineid+'_'+ipiid+'_'+inpid).val(valor_ja_indicado);
                                atualizaValoresItens(ineid, ipiid, inpid);
                            } else {
    					    	var caminho = window.location.href;
    					    	var dado	= new Object();
    					    	dado.requisicao 	= 'SalvarDetalheItensComposicao';
    					    	dado.ieiquantidade 	= jQuery(this).val();
    					    	dado.ineid 			= ineid;
    					    	dado.ipiid 			= ipiid;
    					    	$.ajax({
    						        type: 	"POST",
    						        url: 	caminho,
    						        data: 	dado,
    						        async: 	false,
    						        success: function (resp) {
    						        	if (resp != 'ok'){
    							        	swal({title: "Erro ao salvar o item da Iniciativa", text: "", type: "error"},
    										   function(){
    												location.href = location.href;
    										   }
    										);
    									}else{
                                            jQuery('#valor_ja_indicado_'+ineid+'_'+ipiid+'_'+inpid).val( jQuery('#qtd-item-indicado_'+ineid+'_'+ipiid+'_'+inpid).val() );
    										calcularContraPartida(inpid, ineid);
        								}
    						    	},
    						    	error: function(){
    							        	swal({title: "Erro ao salvar o item da Iniciativa", text: "", type: "error"},
    										   function(){
    												location.href = location.href;
    										   }
    										);
        							}
    						    });
                            }
					    	
						});
						
  					    if (typeof abreModalEscolas != 'function'){
						    var abreModalEscolas = function (ineid,inpid,iniano,iigid,ipiid,inuid,itrid) {
						    	obCalc = new Calculo();
						    	var valorVinculadoParlamentar = jQuery('#valor_vinculado_' + inpid).val();
						    	if (obCalc.comparar(valorVinculadoParlamentar, 0, '<=')){
						    		swal({html:true, title: "Erro", text: "Preencha o \"<b>Valor Vinculado pelo Parlamentar</b>\" antes de preencher a \"<b>QTD indicada</b>\"", type: "error"},
										function(){
											jQuery('#valor_vinculado_' + inpid).focus();
										});
						    		return false;
						    	}
						    
						        var label = $(document).find('#nome-'+iigid).html();
						        $(document).find('#itemnomemodal').html(label);
						
						        $('#dadosbuscaescola').find('[name=ineid]').val(ineid);
						        $('#dadosbuscaescola').find('[name=inpid]').val(inpid);
						        $('#dadosbuscaescola').find('[name=iniano]').val(iniano);
						        $('#dadosbuscaescola').find('[name=iigid]').val(iigid);
						        $('#dadosbuscaescola').find('[name=ipiid]').val(ipiid);
						        
							    var caminho = window.location.href;
						        if(itrid == 2){
							        var inpid  = $('#dadosbuscaescola').find('[name=inpid]').val();
							        var iniano = $('#dadosbuscaescola').find('[name=iniano]').val();
							        var iigid  = $('#dadosbuscaescola').find('[name=iigid]').val();
							        var inuid  = $('#dadosbuscaescola').find('[name=inuid]').val();
							        var muncod = $('#dadosbuscaescola').find('[name=escmuncod]').val();
							        var action  = '&requisicao=buscaModalEscolas&ineid='+ineid+'&inpid='+inpid+'&iniano='+iniano+'&iigid='+iigid+'&inuid='+inuid+'&muncod='+muncod+'&ipiid='+ipiid;
							        $.ajax({
							            type: "POST",
							            url: caminho,
							            data: action,
							            async: false,
							            success: function (resp) {
							                $("#conteudo-modal").html(resp);
							            }
							        });
						        }else if(itrid == 3){
							        var inpid  = $('#dadosbuscaescola').find('[name=inpid]').val();
							        var iniano = $('#dadosbuscaescola').find('[name=iniano]').val();
							        var iigid  = $('#dadosbuscaescola').find('[name=iigid]').val();
							        var inuid  = $('#dadosbuscaescola').find('[name=inuid]').val();
							        var action  = '&requisicao=buscaModalEscolas&ineid='+ineid+'&inpid='+inpid+'&iniano='+iniano+'&iigid='+iigid+'&inuid='+inuid+'&ipiid='+ipiid;
							        $.ajax({
							            type: "POST",
							            url: caminho,
							            data: action,
							            async: false,
							            success: function (resp) {
							                $("#conteudo-modal").html(resp);
							            }
							        });
						        }else{
							        $(document).find('[name=escmuncod]').val('').trigger('chosen:updated');
							        $("#conteudo-modal").hide();
							        $("#conteudo-modal").html('');
						    	}
						    	
						        $("#modal_escolas").modal("show");
									
								$('#modal_escolas').off('hidden.bs.modal');
						        $('#modal_escolas').on('hidden.bs.modal', function (e) {
						        	// Fecha sublinha com o item composição.
									expandirLinha('detalharItensComposicao', [ineid, inpid, inuid], ineid);
						        	// Abre sublinha com o item composição, depois de 1 segundo.
									expandirLinha('detalharItensComposicao', [ineid, inpid, inuid], ineid);
									
									setTimeout(function (){ calcularContraPartida(inpid , ineid); }, 1000);
								});
						    }
						}
		
						if (typeof calcularContraPartida != 'function'){
							var calcularContraPartida = function (inpid, ineid){
								jQuery('#soma_valor_indicado_' + inpid).ready(function (){
									var obCalc = new Calculo();
		    						var somaValorIndicado = jQuery('#soma_valor_indicado_' + inpid).html();
									var valorVinculadoParlamentar = jQuery('#valor_vinculado_' + inpid).val();
											
									var inecontrapartida = obCalc.operacao(somaValorIndicado, valorVinculadoParlamentar, '-');
									inecontrapartida = (inecontrapartida > 0 ? inecontrapartida : 0);
									
									jQuery('#contrapartida' + inpid).val(  number_format(inecontrapartida, 2, ",", ".") );
									jQuery('#contrapartida_txt_' + inpid).html(  number_format(inecontrapartida, 2, ",", ".") );
											
									if (obCalc.comparar(somaValorIndicado, valorVinculadoParlamentar, '>')){
		    							swal({
	 									  html : true,
										  title: "Esta iniciativa terá contrapartida!",
										  text: "A soma dos <b>itens de composicao</b> excederam o <b>valor vinculado pelo parlamentar</b>",
										  type: "info"
										}, function (isConfirm) {
                                            if (isConfirm) {
                                                var caminho = window.location.href;
    											var action  = '&requisicao=salvarContraPartida&ineid='+ineid+'&inpid='+inpid+'&inecontrapartida='+inecontrapartida;
    											jQuery.ajax({
    									            type: "POST",
    									            url: caminho,
    									            data: action,
    									            async: false,
    									            success: function (resp) {}
    									        });
                                            }
                                            return false;
										});
		    						}else{
											var caminho = window.location.href;
											var action  = '&requisicao=salvarContraPartida&ineid='+ineid+'&inpid='+inpid+'&inecontrapartida=0';
											jQuery.ajax({
									            type: "POST",
									            url: caminho,
									            data: action,
									            async: false,
									            success: function (resp) {}
									        });
	    							}
								});
	    					}
						}
				</script>
HTML;
        } else {
            $tabela = '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="dados-itens-composicao">
					    	<thead>
						    	<tr>
							    	<th>Ano</th>
							    	<th>Item</th>
							    	<th>Unidade de Medida</th>
							    	<th>Valor Referência(R$)</th>
							    	<th style="text-align: center">Quantidade</th>
							    	<th style="text-align: center">Total(R$)</th>
							    	<th style="text-align: center">QTD Indicada</th>
							    	<th style="text-align: center">Valor Indicado(R$)</th>
						    	</tr>
					    	</thead>
	        				<tbody>
				        		<tr class="lista-vazia">
				        			<td colspan="10">
				        				<div class="alert alert-info col-lg-8 col-lg-offset-2" role="alert">Nenhum registro encontrado</div>
				        			</td>
					        	</tr>
	        				</tbody>
        				</table>';
        }

        return $tabela;
    }
    
    public function listarObrasBeneficiarioComVinculoParlamentar($arrPost = array())
    {
        global $simec;
        
        $ineid = $arrPost['dados'][0];
        $inpid = $arrPost['dados'][1];
        $inuid = $arrPost['dados'][2];
        $intoid = $arrPost['dados'][3];
        
        
        $sql = $this->model->sqlObrasBeneficiarioComVinculoParlamentar($inpid, $arrPost['emeid'], $arrPost['edeid']);
        $arrRegistros = $this->model->carregar($sql);
        
        $tabela = '<form name="form-dados-obras" id="form-dados-obras" class="form-horizontal" method="post">
                    <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="dados-itens-composicao">
			    	<thead>
				    	<tr>
					    	<th>Ano</th>
					    	<th>Pré-obra</th>
					    	<th>Tipo Obra</th>
					    	<th>Obra</th>
					    	<th>Valor(R$)</th>
					    	<th>Situação Obra</th>
					    	<th>Valor Total Indicado(R$)</th>
					    	<th style="text-align: center">Valor Indicado(R$)</th>
					    	<th style="text-align: center">Contrapartida(R$)</th>
				    	</tr>
			    	</thead>
			    	<tbody>';
        
        if (is_array($arrRegistros) && !empty($arrRegistros[0])) {
            $totalItemGeral = 0;
            $totalGeralIndicado = 0;
            foreach ($arrRegistros as $valor) {
                $tabela .= '<tr>
                				<td>'.$valor['obrano'].'
                                    <input type="hidden" name="obrid[]" value="'.$valor['obrid'].'">
                                    <input type="hidden" name="inpid['.$valor['obrid'].']" value="'.$inpid.'">
                                    <input type="hidden" name="ineid['.$valor['obrid'].']" value="'.$ineid.'">
                                    <input type="hidden" name="obrvalor['.$valor['obrid'].']" value="'.$valor['obrvalor'].'">
                                </td>
                				<td>'.$valor['obrid'].'</td>
                                <td style="width: auto; text-align: left;">'.$valor['otpdsc'].'</td>
                				<td style="width: auto; text-align: left;">'.$valor['obrdsc'].'</td>
                				<td>'.formata_valor($valor['obrvalor']).'</td>
                				<td style="width: auto; text-align: left;">'.$valor['esddsc'].'</td>
                				<td><div id="vlr_total_indicado_'.$valor['obrid'].'">'.formata_valor($valor['vlr_total_indicado']).'</div></td>
                				<td><input name="valorindicadoobra['.$valor['obrid'].']" id="valorindicadoobra_'.$valor['obrid'].'" obrid='.$valor['obrid'].' type="text" maxlength="20"
                                        value="'.formata_valor($valor['ieovalor']).'" size="15" class="form-control valorindicadoobra_'.$inpid.'"
                                        onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value);" onblur="salvarDadosObrasAjax('.$valor['obrid'].', '.$inpid.');" style="text-align: right;"></td>

                				<td><input name="valorcontrapartida['.$valor['obrid'].']" readonly id="valorcontrapartida_'.$valor['obrid'].'" obrid='.$valor['obrid'].' type="text" maxlength="20"
                                    value="'.formata_valor($valor['obrvalor_contrapartida']).'" size="15" class="form-control valorcontrapartida_'.$inpid.'"
                                                        onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value);" style="text-align: right;"></td>
                			</tr>';
                $totalItemGeral = ($totalItemGeral + $valor['obrvalor']);
                $totalGeralIndicado = ($totalGeralIndicado + $valor['vlr_total_indicado']);
            }
            $tabela .= '<tr>
            				<td colspan=3 style="text-align:right;"><b>Totais:</b></td>
            				<td>&nbsp;</td>
            				<td style="text-align:center;">'.formata_valor($totalItemGeral).'</td>
            				<td>&nbsp;</td>
            				<td style="text-align:center;"><div id="total_geral_indicado">'.formata_valor($totalGeralIndicado).'</div></td>
            				<td>&nbsp;</td>
            				<td>&nbsp;</td>
            			</tr>';
            $tabela .= '</tbody></table>';
        } else {
            $tabela .= '<tr class="lista-vazia">
				        			<td colspan="10">
				        				<div class="alert alert-info col-lg-8 col-lg-offset-2" role="alert">Nenhum registro encontrado</div>
				        			</td>
					        	</tr>
	        				</tbody>
        				</table>';
        }
            $tabela .= '</form>';
        return $tabela;
    }

    public function salvarIniciativaEmendaItemComposicao($arrPost = array())
    {
        if ($arrPost['ineid'] && $arrPost['ipiid']) {
            $arrPost['ieiquantidade'] = ($arrPost['ieiquantidade'] ? $arrPost['ieiquantidade'] : 0);
            $sql = "update par3.iniciativa_emenda_item_composicao
					set ieiquantidade={$arrPost['ieiquantidade']}
					where
						ineid={$arrPost['ineid']} and
						ipiid={$arrPost['ipiid']} and
						ieistatus='A'
					returning ieiid";
            $atualizado = $this->model->pegaUm($sql);
                                            
            if ($atualizado == false) {
                $arrDetItem= array();
                $arrDetItem['arChavePrimaria']      = 'ieiid';
                $arrDetItem['stNomeTabela']         = 'par3.iniciativa_emenda_item_composicao';
                $arrDetItem['arAtributosTabela']    = array('ineid', 'ipiid', 'ieiquantidade', 'usucpf');
           
                $arrDetItem['ineid']            = $arrPost['ineid'];
                $arrDetItem['ipiid']            = $arrPost['ipiid'];
                $arrDetItem['ieiquantidade']    = (int) $arrPost['ieiquantidade'];
                $arrDetItem['usucpf']           = $_SESSION['usucpf'];
           
                trata_salvar($arrDetItem);
            }
            
            $this->model->commit();
            echo 'ok';
        }
    }
    
    public function salvarDadosObrasEmendas($arrPost = array())
    {
                
        $obrid = $arrPost['obrid_get'];
        
        $ieovalorBanco = $this->model->pegaUm("SELECT ieovalor FROM par3.iniciativa_emenda_obra WHERE ieostatus = 'A' AND obrid = $obrid and inpid = {$arrPost['inpid'][$obrid]} and ineid = {$arrPost['ineid'][$obrid]}");
        
        $valorindicadoobra = retiraPontos($arrPost['valorindicadoobra'][$obrid]);
        
        if ($valorindicadoobra == 0) {
            $vlrTotalContra = 0;
        }
        
        if ($ieovalorBanco <> $valorindicadoobra) {
            $sql = "UPDATE par3.iniciativa_emenda_obra SET ieostatus = 'I' WHERE obrid = $obrid and inpid = {$arrPost['inpid'][$obrid]} and ineid = {$arrPost['ineid'][$obrid]}";
            $this->model->executar($sql);
            
            $arrObras= array();
            $arrObras['arChavePrimaria']        = 'ieoid';
            $arrObras['stNomeTabela']       = 'par3.iniciativa_emenda_obra';
            $arrObras['arAtributosTabela']  = array('ieoid', 'ineid', 'inpid', 'obrid', 'ieovalor', 'usucpf', 'ieodata', 'ieostatus');
       
            $arrObras['ineid']    = $arrPost['ineid'][$obrid];
            $arrObras['inpid']    = $arrPost['inpid'][$obrid];
            $arrObras['obrid']    = $obrid;
            $arrObras['ieovalor'] = $valorindicadoobra;
            $arrObras['usucpf']   = $_SESSION['usucpf'];
       
            trata_salvar($arrObras);
        }
        
        $vlrTotalContra = $this->model->pegaUm("SELECT (o.obrvalor - coalesce(sum(eo.ieovalor),0)) FROM par3.obra o
                                                	LEFT JOIN par3.iniciativa_emenda_obra eo ON eo.obrid = o.obrid AND eo.ieostatus = 'A'
                                                WHERE  o.obrstatus = 'A' AND o.obrid = $obrid
                                                GROUP BY o.obrvalor");
        $this->model->executar("UPDATE par3.obra SET obrvalor_contrapartida = ".($vlrTotalContra < 0 ? 0 : $vlrTotalContra)." WHERE obrid = $obrid");
        $this->model->commit();
   
        echo 'ok';
    }
    
    public function salvarValorParlamentar($arrPost = array())
    {
                
        $arrPostEmenda = array();
        $arrPostEmenda['arChavePrimaria']   = 'ineid';
        $arrPostEmenda['stNomeTabela']      = 'par3.iniciativa_emenda';
        $arrPostEmenda['arAtributosTabela'] = array('ineid', 'docid', 'inpid', 'edeid', 'emeid', 'inevalor', 'ineresponsavel', 'inestatus');
        
        $arrPost['valor'] = retiraPontos($arrPost['valor']);
        $ineid = $arrPost['ineid'];
        
        if ($arrPost['valor'] > 0) {
            $ineresponsavel = 'M';
        }
        
        $arrPostEmenda['ineid']         = $ineid;
        $arrPostEmenda['inevalor']      = $arrPost['valor'];
        $arrPostEmenda['emeid']         = $arrPost['emeid'];
        $arrPostEmenda['edeid']         = $arrPost['edeid'];
        $arrPostEmenda['inpid']         = $arrPost['inpid'];
        $arrPostEmenda['ineresponsavel'] = $ineresponsavel;
        $arrPostEmenda['inevalor']      = $arrPost['valor'] . " ";
        
        trata_salvar($arrPostEmenda);
   
        echo 'ok';
    }
    
    public function salvarContraPartidaIniciativaEmenda($arrPost = array())
    {
        if ($arrPost['ineid']) {
            $arrPostEmenda = array();
            $arrPostEmenda['arChavePrimaria']   = 'ineid';
            $arrPostEmenda['stNomeTabela']      = 'par3.iniciativa_emenda';
            $arrPostEmenda['arAtributosTabela'] = array('ineid','inecontrapartida');
            
//          $arrPostEmenda['inecontrapartida']  = retiraPontos($arrPost['inecontrapartida']) . ' ';
            $arrPostEmenda['inecontrapartida']  = $arrPost['inecontrapartida'] . ' ';
            $arrPostEmenda['ineid']             = $arrPost['ineid'];
            
            trata_salvar($arrPostEmenda);
        }
        echo 'ok';
    }
    
    public function salvarIniciativaEmendaItemComposicaoEscola($arrPost = array())
    {
        if (!$arrPost['ipeid'] && $arrPost['ipiid'] && $arrPost['inpid'] && $arrPost['escid']) {
            $itemComposicaoEscola = new Par3_Model_IniciativaPlanejamentoItemComposicaoEscola();
            $arrPost['ipeid'] = $itemComposicaoEscola->pegaIpeidPorEscola($arrPost['ipiid'], $arrPost['escid']);
            
            if (!$arrPost['ipeid']) {
                $arrDetItem= array();
                $arrDetItem['arChavePrimaria']      = 'ipeid';
                $arrDetItem['stNomeTabela']         = 'par3.iniciativa_planejamento_item_composicao_escola';
                $arrDetItem['arAtributosTabela']    = array('ipiid', 'ipequantidade', 'escid');
                 
                $arrDetItem['ipiid']            = $arrPost['ipiid'];
                $arrDetItem['escid']            = $arrPost['escid'];
                $arrDetItem['ipequantidade']    = 0;
                 
                $arrPost['ipeid'] = trata_salvar($arrDetItem);
            }
        }
        
        if ($arrPost['ineid'] && $arrPost['ipeid'] && $arrPost['ipiid']) {
            $arrPost['ieequantidade'] = ($arrPost['ieequantidade'] ? $arrPost['ieequantidade'] : 0);
            $sql = "update par3.iniciativa_emenda_item_composicao_escola
					set ieequantidade={$arrPost['ieequantidade']}
					where
						ineid={$arrPost['ineid']} and
						ipeid={$arrPost['ipeid']} and
						ieestatus='A'
					returning ieeid";
            $atualizado = $this->model->pegaUm($sql);
            
            if ($atualizado == false) {
                $arrDetItem= array();
                $arrDetItem['arChavePrimaria']      = 'ieeid';
                $arrDetItem['stNomeTabela']         = 'par3.iniciativa_emenda_item_composicao_escola';
                $arrDetItem['arAtributosTabela']    = array('ineid', 'ipeid', 'ieequantidade', 'usucpf');
           
                $arrDetItem['ineid']            = $arrPost['ineid'];
                $arrDetItem['ipeid']            = $arrPost['ipeid'];
                $arrDetItem['ieequantidade']    = (int) $arrPost['ieequantidade'];
                $arrDetItem['usucpf']           = $_SESSION['usucpf'];
           
                trata_salvar($arrDetItem);
            }
            
            $this->model->commit();
            echo 'ok';
            
            $sql = "select
						coalesce(sum(iee.ieequantidade),0) as ieequantidade
					from
						par3.iniciativa_planejamento_item_composicao_escola ipe
					join par3.iniciativa_emenda_item_composicao_escola iee on iee.ipeid = ipe.ipeid and
																			  iee.ieestatus = 'A'
					where
						iee.ineid = {$arrPost['ineid']} and
						ipe.ipiid = {$arrPost['ipiid']} and
						ipe.ipestatus = 'A'";
            $arrPost['ieiquantidade'] = $this->model->pegaUm($sql);
            
            $this->salvarIniciativaEmendaItemComposicao($arrPost);
        }
    }
    
    public function carregaDetalheEmenda($emeid)
    {
        
        $sql = $this->model->sqlDetalheEmenda($emeid);
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('GND','Modalidade', 'Fonte', 'Valor Total');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->setQuery($sql);
        $listagemSimec->addCallbackDeCampo('emdvalor', 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addCallbackDeCampo(['modalidade', 'gnd', 'fonte'], 'formata_Tipo_Texto_Tabela');
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->setFormOff();
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function carregaBeneficiarioEmenda($emeid)
    {
        
        $sql = $this->model->sqlBeneficiarioEmenda($emeid);
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('CNPJ','Beneficiário', 'GND', 'Modalidade', 'Valor', 'Situação');
        $esconderColunas = array('emeid', 'inuid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->addAcao('edit', array('func' => 'carregarBeneficiarioEmenda', 'extra-params' => array('emeid'), 'titulo' => 'Vincular Iniciativas ao Beneficiário'));
        $listagemSimec->addCallbackDeCampo('edevalor', 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addCallbackDeCampo(['status', 'enbnome'], 'formata_Tipo_Texto_Tabela');
        $listagemSimec->addCallbackDeCampo(['emecod', 'emeano', 'gndcod', 'mapcod'], 'formata_Tipo_Numero_Tabela');
        $listagemSimec->setAcaoComoCondicional('edit', [['campo' => 'inuid', 'valor' => '', 'op' => 'diferente']]);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->setFormOff();
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function pegaBeneficiarioSelecionado($edeid, $inpid = '', $inuid = '')
    {
        
        $sql = $this->model->sqlBeneficiarioSelecionado($edeid, $inpid, $inuid);
        
        $esconderColunas = array();
        if (empty($inpid)) {
            $esconderColunas = array('total_aprovado', 'id');
            $arrayCabecalho = array('Emenda','Ano', 'GND', 'Modalidade', 'CNPJ', 'Beneficiário', 'UF', 'Situação', 'Valor Total da Emenda', 'Valor Designado', 'Valor Indicado');
        } else {
            $arrayCabecalho = array('Emenda', 'Parlamentar', 'Ano', 'GND', 'Modalidade', 'Situação', 'Valor Total da Emenda', 'Valor Designado', 'Valor Indicado', 'Valor Aprovado Análise(Emenda)');
            if ($_REQUEST['boEmpenho'] == 'S') {
                $esconderColunas = array('id', 'inuid', 'inpid', 'anaid', 'cnpj', 'enbnome', 'uf');
            } else {
                $esconderColunas = array('inuid', 'inpid', 'anaid', 'cnpj', 'enbnome', 'uf');
            }
        }
        
        $listagemSimec = new Simec_Listagem();
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        if (!empty($inpid) && $_REQUEST['boEmpenho'] != 'S') {
            $listagemSimec->addAcao('edit', array('func' => 'carregarEmendaAnalise', 'extra-params' => array('inpid', 'anaid', 'emeano', 'inuid'), 'titulo' => 'Carrega análise emenda'));
        }
        $listagemSimec->addCallbackDeCampo(['emevalor', 'edevalor', 'valor_total_indicado_municipio', 'total_aprovado'], 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->addCallbackDeCampo(['status', 'enbnome'], 'formata_Tipo_Texto_Tabela');
        $listagemSimec->addCallbackDeCampo(['emecod', 'emeano', 'gndcod', 'mapcod'], 'formata_Tipo_Numero_Tabela');
        if (!empty($inpid) && $_REQUEST['boEmpenho'] != 'S') {
            $listagemSimec->setAcaoComoCondicional('edit', [['campo' => 'anaid', 'valor' => $_REQUEST['anaid'], 'op' => 'diferente']]);
        }
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->setFormOff();
        $listagemSimec->render(Simec_Listagem::TOTAL_QTD_REGISTROS);
    }
    
    public function carregaIniciativaBeneficiario($edeid, $emeid)
    {
        
        $arrPerfilRetorno = $this->verificaPerfilEmendas($emeid, $edeid);
        
        $sql = $this->model->sqlIniciativaBeneficiario($edeid);
        $arrDados = $this->model->carregar($sql);
        $arrDados = $arrDados ? $arrDados : array();
        
        echo getToobarListagem($sql);
        
        if ($arrPerfilRetorno['tramitar'] == 'N' && $arrPerfilRetorno['vincular'] == 'N') {
            $disabledCheck = 'disabled';
        }
        
        $arrayCabecalho = array('<div class="checkbox checkbox-success">
                        	<input type="checkbox" id="seleciona_todos" value="" '.$disabledCheck.' onclick="selecionaTodos();">
                        	<label for="seleciona_todos"><b>Todos</b></label>
                        	</div>', 'Código', 'Dimensão', 'Etapa', 'Modalidade', 'Iniciativa', 'Tipo de Objeto', 'Categoria Despesa', 'Valor Planejado', 'Situação', 'Valor Destinado');
        
        $html = '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" id="tb_render" data-qtd-acoes="0"><thead><tr>';
        foreach ($arrayCabecalho as $key => $cab) {
            if ($key == 10) {
                $html .= '<th width=200px>'.$cab.'</th>';
            } else {
                $html .= '<th>'.$cab.'</th>';
            }
        }
        $html .= '</tr></thead><tbody>';
        
        if ($arrDados[0]) {
            foreach ($arrDados as $v) {
                $sql = $this->model->sqlDadosIniciativaEmenda($v['inpid'], $edeid);
                $arDadosEmendas = $this->model->pegaLinha($sql);
                
                $disabled = 'disabled';
                $disabledCheck = '';
                $checked = '';
                if (!empty($arDadosEmendas)) {
                    if ( !in_array($arDadosEmendas['esdid'], array(PAR3_ESD_EMENDA_EM_CADASTRAMENTO, PAR3_ESD_EMENDA_AJUSTE_PARLAMENTAR)) ) {
                        $checked = 'checked';
                        $disabledCheck = 'disabled';
                        $disabled = 'disabled';
                    } else {
                        $disabled = '';
                        $checked = 'checked';
                    }
                }
                
                if ( $arrPerfilRetorno['tramitar'] == 'N' && $arrPerfilRetorno['vincular'] == 'N' ) {
                    $disabledCheck = 'disabled';
                }
                
                $html .= '<tr>
                            <td>
                                <div class="checkbox checkbox-success">
                                    <input type="checkbox" name="inpid[]" '.$checked.' '.$disabledCheck.' id="chkiniciativa_'.$v['inpid'].'" value="'.$v['inpid'].'">
                                    <label for="chkiniciativa_'.$v['inpid'].'"> </label>
                                    <input type="hidden" id=valor_planejado_'.$v['inpid'].' value=0>
                                    <input type="hidden" name=ineid['.$v['inpid'].'] value='.$arDadosEmendas['ineid'].'></div>
                                    <input type="hidden" name=tem_analise['.$v['inpid'].'] value='.(int)$arDadosEmendas['tem_analise'].'></div>
                            </td>
                            <td><div class="text-center">'.$v['inpid'].'</div></td>
                            <td><div class="text-center">'.$v['dimcod'].'</div></td>
                            <td><div class="text-center">'.$v['etadsc'].'</div></td>
                            <td><div class="text-center">'.$v['moddsc'].'</div></td>
                            <td><div class="text-center">'.$v['inciativa'].'</div></td>
                            <td><div class="text-center">'.$v['intodsc'].'</div></td>
                            <td><div class="text-center">'.$v['categoria'].'</div></td>
                            <td><div class="text-center">'.formata_valor($v['valor']).'</div></td>
                            <td><div class="text-center">'.$arDadosEmendas['situacao'].'</div></td>
                            <td><div class="text-right"><input name="valordestinado['.$v['inpid'].']" id="valordestinado_'.$v['inpid'].'" '.$disabled.' type="text" maxlength="20" value="'.formata_valor($arDadosEmendas['inevalor']).'" size="30" class="form-control valordestinado"
                                                        onkeyup="this.value=mascaraglobal(\'[.###],##\',this.value); calcularValorDestinado('.$v['inpid'].');" onblur="calcularValorDestinado('.$v['inpid'].');" style="text-align: right;"></div></td>
    				    </tr>';
            }
            $html .= '<tr>
                        <td colspan=10><div class="text-right"><b>Total Destinado</b></div></td>
                        <td><div id="total_destinado" class="text-right"></div></td>
                    </tr></tbody><tfoot>
                    <tr>
                        <td style="text-align:right" colspan="11"><strong>Exibindo registros 1 a '.sizeof($arrDados).', de '.sizeof($arrDados).'</strong></td>
                    </tr>
                </tfoot>
            </table>';
        }
        echo $html;
    }
    
    public function carregaIniciativaBeneficiarioComVinculoParlamentar($edeid)
    {
                
        $valor_vinculado_parlamentar = '<span data-toggle="popover" data-html="true" title="" data-trigger="hover" data-placement="top"
                                            data-content="Valor indicado pelo parlamentar para o planejamento">Valor Vinculado pelo Parlamentar</span>';
        
        $valor_total_vinculado = '<span data-toggle="popover" data-html="true" title="" data-trigger="hover" data-placement="top"
                                            data-content="Soma das emendas de todos os parlamentares para o mesmo planejamento">Valor Total Vinculado</span>';
        
        $valor_indicado = '<span data-toggle="popover" data-html="true" title="" data-trigger="hover" data-placement="top"
                                            data-content="Soma dos valores dos (itens de composição/Obras) indicados pelo município para o planejamento">Valor Indicado</span>';
                
        $sql = $this->model->sqlIniciativaBeneficiarioComVinculoParlamentar($edeid);
        
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Código', 'Dimensão', 'Iniciativa', 'Tipo Objeto', 'Situação', 'Valor Planejado', $valor_vinculado_parlamentar, $valor_total_vinculado, $valor_indicado);
        $esconderColunas = array('iniid', 'inuid', 'intoid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->addAcao('plus', array('func' => 'detalharItensComposicao', 'extra-params' => array('inpid', 'inuid', 'intoid')));
        $listagemSimec->addCallbackDeCampo(['iniciativa', 'situacao'], 'formata_Tipo_Texto_Tabela');
        $listagemSimec->addCallbackDeCampo(['inpid', 'valor_planejado', 'valor_total_vinculado_parlamentar', 'valor_total_indicado_municipio'], 'formata_Tipo_Numero_Tabela');
        $listagemSimec->addCallbackDeCampo(['valor_planejado', 'valor_total_vinculado_parlamentar', 'valor_total_indicado_municipio'], 'formataNumeroMonetarioSemSimbolo');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->setFormOff();
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
    
    public function salvar($arrPost = array())
    {
        $emeid = $arrPost['emeid'];
        $edeid = $arrPost['edeid'];
        
        if (is_array($arrPost['inpid'])) {
            
            $arIneid = array();
            foreach ($arrPost['ineid'] as $inpid => $ineid) {
                if( !empty($ineid) ){
                    if( !in_array($inpid, $arrPost['inpid']) ) {
                        $this->model->executar("UPDATE par3.iniciativa_emenda_item_composicao_escola SET ieestatus = 'I' WHERE ineid = $ineid");
                        $this->model->executar("UPDATE par3.iniciativa_emenda_item_composicao SET ieistatus = 'I' WHERE ineid = $ineid");
                        $this->model->executar("UPDATE par3.iniciativa_emenda_obra SET ieostatus = 'I' WHERE ineid = $ineid");
                        $this->model->executar("UPDATE par3.iniciativa_emenda SET inestatus = 'I' WHERE ineid = $ineid");
                        array_push($arIneid, $ineid);
                    }
                }
            }
            $this->model->commit();
            $arrPostEmenda = array();
            $arrPostEmenda['arChavePrimaria']   = 'ineid';
            $arrPostEmenda['stNomeTabela']      = 'par3.iniciativa_emenda';
            $arrPostEmenda['arAtributosTabela']     = array('ineid', 'docid', 'inpid', 'edeid', 'emeid', 'inevalor', 'ineresponsavel', 'usucpf', 'inestatus');
            
            foreach ($arrPost['inpid'] as $inpid) {
                $valordestinado = $arrPost['valordestinado'][$inpid];
                $ineid = $arrPost['ineid'][$inpid];
                
                if ($valordestinado > 0) {
                    $ineresponsavel = 'P';
                }
                
                $arrPostEmenda['inevalor'] = retiraPontos($valordestinado);
                if( !in_array($ineid, $arIneid) ){
                    $arrPostEmenda['ineid'] = $ineid;
                } else {
                ver($ineid, $arIneid,d);
                }
                $arrPostEmenda['emeid'] = $emeid;
                $arrPostEmenda['edeid'] = $edeid;
                $arrPostEmenda['inpid'] = $inpid;
                $arrPostEmenda['ineresponsavel'] = $ineresponsavel;
                $arrPostEmenda['inestatus'] = 'A';
                $arrPostEmenda['usucpf'] = $_SESSION['usucpf'];
                $arrPostEmenda['docid'] = $this->pegaDocidEmenda('', $inpid, $edeid) ;
                
                trata_salvar($arrPostEmenda);
            }
        }
        if ($arrPost['tramitar'] != 'S') {
            $url = "par3.php?modulo=principal/emendas/vincularIniciativaBeneficiario&acao=A&emeid=$emeid&edeid=$edeid";
            simec_redirecionar($url, 'success', 'Operação Realizada com Sucesso!');
        } else {
            return true;
        }
    }
    
    public function enviarIniciativa($arrPost = array())
    {
        
        if (is_array($arrPost['inpid'])) {
            $emeid = $arrPost['emeid'];
            $edeid = $arrPost['edeid'];
            
            $sql = "SELECT e.ineid, e.edeid, e.inpid, e.inevalor, ed.esdid, ed.esddsc as situacao, d.docid
                FROM par3.iniciativa_emenda e
                	INNER JOIN workflow.documento d ON d.docid = e.docid
                	INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                WHERE e.inestatus = 'A' AND ed.esdid = 2115 AND e.edeid = $edeid AND e.emeid = $emeid";
            $arrDados = $this->model->carregar($sql);
            
            $error = false;
            foreach ($arrDados as $v) {
                $docid = $v['docid'];
                
                $arrParamWork = array();
                if (!wf_alterarEstado($docid, 5313, 'Fluxo de Emendas do PAR3 ', $arrParamWork)) {
                    $error = true;
                }
            }
            $url = 'par3.php?modulo=principal/emendas/vincularIniciativaBeneficiario&acao=A&emeid='.$arrPost['emeid'].'&edeid='.$arrPost['edeid'];
            if ($error) {
                simec_redirecionar($url, 'error');
            } else {
                simec_redirecionar($url, 'success');
            }
        }
    }
    
    public function pegaResponsabilidade()
    {
        $sql = "SELECT ur.autid FROM spoemendas.usuarioresponsabilidade ur
                	INNER JOIN emenda.autor a ON a.autid = ur.autid
                WHERE usucpf = '{$_SESSION['usucpf']}'
                	AND ur.rpustatus = 'A'";
        return $this->model->carregarColuna($sql);
    }
    
    public function validaResponsabilidadeEmenda($emeid)
    {
        
        $arrPerfil = pegaPerfils($_SESSION['usucpf']);
        if ($emeid) {
            if ($this->model->testa_superuser() || in_array(PAR3_PERFIL_ADMINISTRADOR, $arrPerfil) || in_array(PAR3_PERFIL_ANALISTA_CGEST, $arrPerfil) || in_array(PAR3_PERFIL_COORDENADOR_CGEST, $arrPerfil)) {
                return 'S';
            } else {
                $sql = "SELECT count(ur.rpuid) FROM spoemendas.usuarioresponsabilidade ur
                        	INNER JOIN emenda.emenda e ON e.autid = ur.autid
                        WHERE usucpf = '{$_SESSION['usucpf']}'
                        	AND ur.rpustatus = 'A' AND e.emeid = $emeid";
                $boResp = $this->model->pegaUm($sql);
                if ($boResp > 0) {
                    return 'S';
                } else {
                    simec_redirecionar('par3.php?modulo=principal/emendas/listaEmendas&acao=A', 'error', 'Este usuário não possui responsabilidade vinculada a esta emenda!');
                }
            }
        } else {
            simec_redirecionar('par3.php?modulo=principal/emendas/listaEmendas&acao=A', 'error', 'Este usuário não possui responsabilidade vinculada a esta emenda!');
        }
    }
    
    public function pegaResponsabilidadeMunicipio()
    {
        $modelUsuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();
        
        return $modelUsuarioResponsabilidade->recuperarMunicipioUsuarioResponsabilidade();
    }
    
    public function carregarIniciativaEmenda()
    {
        $sql = $this->model->sqlCarregarIniciativaEmenda();
        return $sql;
    }
    
    public function pegaDocidEmenda($ineid = '', $inpid = '', $edeid = '')
    {
        require_once APPRAIZ . '/includes/workflow.php';
        
        if ($ineid) {
            $docid = $this->model->pegaUm("SELECT docid FROM par3.iniciativa_emenda WHERE ineid = $ineid");
        } else {
            // descrição do documento
            $docdsc = "Fluxo de Emendas do PAR3 - inpid: " . $inpid. ' - edeid: '.$edeid;
            
            // cria documento do WORKFLOW
            $docid = wf_cadastrarDocumento(317, $docdsc, 2115);
        }
        
        return $docid;
    }
    
    public function verificaSituacaoAtual($edeid, $esdid = '')
    {
        $sql = $this->model->sqlVerificaSituacaoAtual($edeid, $esdid);
        
        return $this->model->pegaLinha($sql);
    }
    
    public function pegaDadosEmendas($emeid)
    {
        
        return $this->model->pegaLinha("SELECT a.autcod||' - '||a.autnome AS autor, v.acacod||' - '||v.acadsc AS acao FROM emenda.emenda e
                                        	INNER JOIN emenda.autor a ON a.autid = e.autid
                                        	INNER JOIN emenda.v_funcionalprogramatica v ON v.acaid = e.acaid AND v.acastatus = 'A'
                                        WHERE e.etoid = 1
                                        	AND e.emeid = $emeid");
    }
    
    public function pegaParlamenatar($emeid)
    {
        
        $sql = $this->model->sqlPegaParlamenatar($emeid);
        
        return $sql;
    }
    
    public function pegaAcaoWorkFlow()
    {
        
        $sql = $this->model->sqlPegaAcaoWorkFlow();
        
        return $sql;
    }
    
    public function pegaEstadoAtualEmenda($edeid)
    {
        
        $sql = $this->model->sqlPegaEstadoAtualEmenda($edeid);
        $arrDados = $this->model->pegaLinha($sql);
        
        return $arrDados;
    }

    public function recuperarEmendaPorObra($obrid = null, $count = false)
    {
        if (!$obrid) {
            return false;
        }
        $arrPegalinha = array();
        if ($count) {
            $sql = "SELECT count(*) as count FROM par3.iniciativa_emenda_obra ieo inner join par3.iniciativa_emenda ine using(ineid)
                                 where ieo.ieostatus = 'A' and ieo.ieovalor > 0 and obrid = {$obrid} and ine.inestatus = 'A' ";
            return $this->model->pegaUm($sql);
        }
        return $this->model->carregar("SELECT * as count FROM par3.iniciativa_emenda_obra ieo inner join par3.iniciativa_emenda ine using(ineid)
                                 where ieo.ieostatus = 'A' and ieo.ieovalor > 0 and obrid = {$obrid} and ine.inestatus = 'A' ");
    }

    public function listarEmendasPorObra($obrid)
    {
        $campos = [
                'eme.emecod',
                'aut.autnome',
                'coalesce(eme.emevalor,0) AS valor_emenda',
                'coalesce(ine.ieovalor,0) AS valor_emenda_obra',
                'coalesce(emp.eocvalorempenho,0) as valor_empenho'
            ];
        $clausulas = ['ine.ieostatus = \'A\'','ieo.inestatus =\'A\''];
        $sql = $this->model->selectEmendasPorObra($obrid, $campos, $clausulas);
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Emenda', 'Autor', 'Valor Emenda','Valor da Emenda para a Obra','Valor Empenho');
        $esconderColunas = array('iniid', 'inuid', 'intoid');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        $listagemSimec->setQuery($sql);
        $listagemSimec->turnOnPesquisator();
        $listagemSimec->addCallbackDeCampo('valor_emenda', 'par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('valor_emenda_obra', 'par3_mascaraMoeda');
        $listagemSimec->addCallbackDeCampo('valor_empenho', 'par3_mascaraMoeda');
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->setFormOff();
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }

    public function retornarTotalEmendaPorObra($obrid)
    {
        $campos = [
            'coalesce(obr.obrvalor_contrapartida,0) as obra_contrapartida',
            'coalesce(obr.obrvalor,0) as valor_obra',
            'sum(coalesce(eme.emevalor,0)) AS valor_emenda',
            'sum(coalesce(ine.ieovalor,0)) AS valor_emenda_obra',
            'sum(coalesce(emp.eocvalorempenho,0)) as valor_empenho'
        ];
        $clausulas = ['ine.ieostatus = \'A\''];
        $sql = $this->model->selectEmendasPorObra($obrid, $campos, $clausulas, array(1,'valor_obra'));
        global $db;
        return $db->recuperar($sql);
    }
    
    public function pegaMuncodEmenda($inuid)
    {
        $arDados = $this->model->pegaLinha("SELECT muncod, estuf FROM par3.instrumentounidade WHERE inuid = $inuid");
        
        return $arDados;
    }
    
    public function pegaAcaoWorkFlowEmenda($esdid, $esdiddestino = '')
    {
        
        if ($esdiddestino) {
            $where = " AND a.esdiddestino = ".PAR3_ESDID_AGUARDANDO_ANALISE;
        }
        
        $sql = "select
					a.aedid,
		            a.aeddscrealizar,
		            a.esdidorigem,
					ed.esdid as esdiddestino,
					ed.esddsc,
                    a.aedicone,
					(SELECT count(edp.pflcod) FROM workflow.estadodocumentoperfil edp
						INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = edp.pflcod
					WHERE pu.usucpf = '".$_SESSION['usucpf']."' AND edp.aedid = a.aedid) AS boperfil
				from workflow.acaoestadodoc a
					inner join workflow.estadodocumento ed on ed.esdid = a.esdiddestino
				where
					a.esdidorigem = $esdid
					and aedstatus = 'A' $where
				order by a.aedordem ASC";
        
        $arDados = $this->model->carregar($sql);
        
        return $arDados;
    }
    
    public function verificaPerfilEmendas($emeid = '', $edeid = '')
    {
        
        $emeeditapar = $this->model->pegaUm("SELECT emeeditapar FROM emenda.emenda WHERE emeid = $emeid");
        
        $arSituacao = $this->verificaSituacaoAtual($edeid);
        $arTramitar = $this->verificaTramitacaoRetornoParlamentar($edeid);
        
        $arrRetorno = array();
        /*if( $arTramitar['esdidorigem'] == PAR3_ESD_EMENDA_ANALISE_ENTIDADE && $arTramitar['esdiddestino'] == PAR3_ESD_EMENDA_EM_CADASTRAMENTO ){

            $arrPerfil          = pegaPerfils($_SESSION['usucpf']);
            $arrPerfilValido    = array( PAR3_PERFIL_ADMINISTRADOR, PAR3_PERFIL_ANALISTA_CGEST, PAR3_PERFIL_SUPER_USUARIO, PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR );
            $retornoVincular    = checaPerfilValido($arrPerfilValido, $arrPerfil);

            $arrRetorno = array('vincular' => $retornoVincular, 'tramitar' => $retornoVincular);
        } else {*/
        if ($emeeditapar == 'N' || (!in_array($arSituacao['esdid'], array(PAR3_ESD_EMENDA_EM_CADASTRAMENTO)) && !empty($arSituacao) )) {
            $arrRetorno = array('vincular' => 'N', 'tramitar' => 'N');
        } else {
            $arrPerfil          = pegaPerfils($_SESSION['usucpf']);
            $arrPerfilValido    = array( PAR3_PERFIL_PARLAMENTAR, PAR3_PERFIL_SUPER_USUARIO, PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR );
            $retornoVincular    = checaPerfilValido($arrPerfilValido, $arrPerfil);
                
            $retornoTramitar = 'N';
            if (in_array($arSituacao['esdid'], array(PAR3_ESD_EMENDA_EM_CADASTRAMENTO))) {
                $retornoTramitar = 'S';
            }
                
            $arrRetorno = array('vincular' => $retornoVincular, 'tramitar' => $retornoTramitar);
        }
        //}
        
        return $arrRetorno;
    }
    
    public function verificaTramitacaoRetornoParlamentar($edeid)
    {
        $sql = "SELECT hd.aedid, hd.htddata, ac.esdidorigem, ac.esdiddestino
                FROM par3.iniciativa_emenda ie
                	INNER JOIN workflow.historicodocumento hd ON hd.docid = ie.docid
                	INNER JOIN workflow.acaoestadodoc ac ON ac.aedid = hd.aedid
                WHERE ie.edeid = $edeid
                ORDER BY hd.htddata DESC LIMIT 1";
        return $this->model->pegaLinha($sql);
    }
    
    public function verificaPerfilEmendasEntidade($edeid)
    {
        
        $arEstado = $this->pegaEstadoAtualEmenda($edeid);
        $arrRetorno = array();
        if ($arEstado['esdid'] == PAR3_ESD_EMENDA_ANALISE_ENTIDADE) {
            $arrPerfil = pegaPerfils($_SESSION['usucpf']);
            $arrPerfilValido = array( PAR3_PERFIL_PREFEITO, PAR3_PERFIL_DIRIGENTE_MUNICIPAL, PAR3_PERFIL_SECRETARIO_ESTADUAL, PAR3_PERFIL_SUPER_USUARIO, PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR );
            $retornoVincular = checaPerfilValido($arrPerfilValido, $arrPerfil);
            
            $arrPerfilValido = array( PAR3_PERFIL_PREFEITO, PAR3_PERFIL_SECRETARIO_ESTADUAL, PAR3_PERFIL_SUPER_USUARIO, PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR );
            $retornoTramitar = checaPerfilValido($arrPerfilValido, $arrPerfil);
            
            $arrPerfilValido = array( PAR3_PERFIL_CONSULTA_GERAL, PAR3_PERFIL_ADMINISTRADOR, PAR3_PERFIL_PARLAMENTAR );
            $retornoVisualizar = checaPerfilValido($arrPerfilValido, $arrPerfil);
            
            $arrRetorno = array('vincular' => $retornoVincular, 'tramitar' => $retornoTramitar, 'visualiza' => $retornoVisualizar);
        } else {
            $arrRetorno = array('vincular' => 'N', 'tramitar' => 'N', 'visualiza' => 'N');
        }
        return $arrRetorno;
    }
    
    public function tramitarEmendasEntidade($arrPost = array())
    {
        
        $sql = "SELECT DISTINCT e.docid
                FROM par3.iniciativa_emenda e
                	INNER JOIN workflow.documento d ON d.docid = e.docid
                WHERE e.inestatus = 'A' AND e.edeid = {$arrPost['edeid']} and d.esdid = {$arrPost['esdid_atual']}";
        
        $arDocid = $this->model->carregarColuna($sql);
        
        if ($arrPost['esdiddestino'] == PAR3_ESD_EMENDA_ACEITA) { #Aceitar emenda
            $sql = $this->model->sqlTramitePlanejamento($arrPost['edeid'], $arrPost['esdid_atual']);
            $arrPlanejamento = $this->model->carregar($sql);
            $arrPlanejamento = $arrPlanejamento ? $arrPlanejamento : array();
            
            /*
             * Tramita os planejamentos vinculados a emenda para Aguardando Analise
             * */
            foreach ($arrPlanejamento as $v) {
                if ((int)$v['tot_obra'] == (int)0) {
                    if (!empty($v['aedid']) && ($v['esdid'] != PAR3_ESDID_AGUARDANDO_ANALISE)) {
                        $arrParamWork = array('inpid' => $v['inpid']);
                        wf_alterarEstado($v['docid'], $v['aedid'], 'Fluxo de Iniciativas Planejamento do PAR3', $arrParamWork);
                    }
                
                    $tot_analise = $this->model->pegaUm("SELECT count(anaid) FROM par3.analise WHERE edeid = {$arrPost['edeid']} AND anastatus = 'A' and inpid = {$v['inpid']}");
                    
                    if ((int)$tot_analise == (int)0) {
                        $docdsc = "Fluxo de Iniciativas do PAR3 - Análise Planejamento ";
                        $docid_analise = wf_cadastrarDocumento(310, $docdsc, '2070');
                        
                        $sql = "INSERT INTO par3.analise(anaano, docid, anadatacriacao, anastatus, intaid, edeid, inpid)
                                VALUES('".date('Y')."',  $docid_analise, now(), 'A', 2, ".$arrPost['edeid'].", {$v['inpid']})";
                        $this->model->executar($sql);
                        $this->model->commit();
                    }
                }
            }
            
            $boPTA = $this->model->pegaUm("SELECT count(pedid) FROM emenda.ptemendadetalheentidade WHERE edeid = {$arrPost['edeid']}");
            if ((int)$boPTA == (int)0) {
                $arrEnt = $this->model->pegaLinha("SELECT enbid, edevalor FROM emenda.emendadetalheentidade WHERE edeid = {$arrPost['edeid']} AND edestatus = 'A'");
                
                $ptridpai = 'null';
                $ptrcod = ($this->model->pegaUm("SELECT max(ptrcod) + 1 FROM emenda.planotrabalho"));
                
                $resid = 3;
                $mdeid = 3;
                $enbid = $arrEnt['enbid'];
                $edevalor = $arrEnt['edevalor'];
                
                $ptrid = $this->model->pegaUm("SELECT pt.ptrid FROM emenda.ptemendadetalheentidade pt
                                                	INNER JOIN emenda.v_emendadetalheentidade v ON v.edeid = pt.edeid AND v.edestatus = 'A'
                                                WHERE v.emeid = {$arrPost['emeid']}");
                
                if (empty($ptrid)) {
                    $sql = "INSERT INTO emenda.planotrabalho( enbid, ptrexercicio, ptrstatus, resid, mdeid, ptridpai, ptrcod, ptrsituacao, sisid )
    			             VALUES ( {$enbid}, " . date('Y') . ", 'A', {$resid}, {$mdeid}, " . $ptridpai . ", " . $ptrcod . ", 'E', 23) RETURNING ptrid";
                    $ptrid = $this->model->pegaUm($sql);
                }
                
                $sql = "INSERT INTO emenda.ptemendadetalheentidade(ptrid, edeid, pedvalor)
				        VALUES ($ptrid, {$arrPost['edeid']}, {$edevalor})";
                $this->model->executar($sql);
                
                if ($this->model->commit()) {
                    include_once APPRAIZ . 'includes/workflow.php';
                    // cria o docid (workflow) do PTA
                    $tpdid = 8;
                    $docdsc = "Cadastro de PTA (emendas) - n°" . $ptrid;
                    $docid = wf_cadastrarDocumento($tpdid, $docdsc);
                    
                    $sql = "UPDATE emenda.planotrabalho SET docid = " . $docid . " WHERE ptrid = " . $ptrid;
                    $this->model->executar($sql);
                    $this->model->commit();
                }
            }
        }
        /*
         * Tramita as emendas
         * */
        $aedid = $arrPost['aedid'];
        $arrParamWork = array();
        foreach ($arDocid as $docid) {
            wf_alterarEstado($docid, $aedid, 'Fluxo de Iniciativas/Emendas do PAR3', $arrParamWork);
        }
        
        if (!empty($arrPost['inuid']) && !is_array($arrPost['inuid'])) {
            $url = "par3.php?modulo=principal/emendas/detalharIniciativaParlamentar&acao=A&emeid={$arrPost['emeid']}&edeid={$arrPost['edeid']}&inuid={$arrPost['inuid']}";
        } else {
            $url = "par3.php?modulo=principal/emendas/detalharIniciativaParlamentar&acao=A&emeid={$arrPost['emeid']}&edeid={$arrPost['edeid']}";
        }
        
        simec_redirecionar($url, 'success');
    }
    
    public function verificaPreenchimentoIniciativa($edeid)
    {
        $sql = "SELECT ie.ineid, ie.inpid, sum(eic.qtd) AS qtd
                FROM par3.iniciativa_emenda ie
                	LEFT JOIN(
                		SELECT ic.ineid, ic.ieiquantidade AS qtd FROM par3.iniciativa_emenda_item_composicao ic WHERE ic.ieistatus = 'A'
                		UNION ALL
                		SELECT eo.ineid, eo.ieovalor AS qtd FROM par3.iniciativa_emenda_obra eo WHERE eo.ieostatus = 'A'
                	) eic ON eic.ineid = ie.ineid
                WHERE ie.inestatus = 'A'
                	AND ie.edeid = $edeid
                GROUP BY ie.ineid, ie.inpid
                HAVING sum(eic.qtd) > 0";
        $arDados = $this->model->pegaLinha($sql);
        
        return $arDados;
    }
    
    public function verificaTemEmpenho($edeid)
    {
        $sql = "SELECT DISTINCT count(a.anaid)
                FROM par3.analise a
                	INNER JOIN par3.processoparcomposicao pp ON pp.anaid = a.anaid AND pp.inpid = a.inpid
                	INNER JOIN par3.v_empenho_composicao_iniciativa ve ON ve.ppcid = pp.ppcid
                WHERE a.anastatus = 'A'
                	AND pp.ppcstatus = 'A'
                	AND a.edeid = $edeid";
        $boEmpenho = $this->model->pegaUm($sql);
        
        if( (int)$boEmpenho > (int)0 ){
            return 'S';
        } else {
            return 'N';
        }
    }
    
    public function verificaValorEmpenho($inpid, $edeid)
    {
        $sql = "SELECT a.edeid, a.inpid, sum(v.saldo) AS saldo
                FROM par3.v_empenho_composicao_iniciativa v
                	INNER JOIN par3.analise a ON a.anaid = v.anaid AND a.inpid = v.inpid AND a.anastatus = 'A'
                WHERE a.edeid = $edeid
                	AND a.inpid = $inpid
                GROUP BY a.edeid, a.inpid, v.anaid";
        $arEmpenho = $this->model->pegaLinha($sql);
        
        return $arEmpenho['saldo'];
    }
    
    public function pegaCodigoEmenda($anaid)
    {
        return $this->model->pegaUm("SELECT v.emecod FROM par3.analise a
                            	INNER JOIN emenda.v_emendadetalheentidade v ON v.edeid = a.edeid AND v.edestatus = 'A'
                            WHERE anaid = $anaid");
    }
    
    public function carregaRelatorioParlamentar($arrPost = array())
    {
        $sql = $this->model->sqlCarregaRelatorioParlamentar($arrPost);
       
        $listagemSimec = new Simec_Listagem();
        $arrayCabecalho = array('Emenda','Ano', 'Partido', 'Parlamentar', 'CNPJ', 'Beneficiário', 'UF', 'Objeto' , 'Planejamento','Valor da Emenda', 'Valor Designado', 'Valor Indicado', 'Processo', 'Termo', 'Valor Processo', 'Contrapartida', 'Valor Empenhado', 'Valor Pago', 'Situação');
        $listagemSimec->setCabecalho($arrayCabecalho);
        $listagemSimec->esconderColunas($esconderColunas);
        //$listagemSimec->turnOnPesquisator();
        $listagemSimec->setQuery($sql);
        $listagemSimec->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagemSimec->addCallbackDeCampo(['emevalor', 'vlrdesignado', 'valor_indicado', 'vlrempenho', 'vlrpagamento', 'vlr_processo', 'vlr_contrapartida'], 'formataNumeroMonetarioSemSimbolo');
        //$listagemSimec->addCallbackDeCampo(['autor', 'enbnome', 'situacao', 'iniciativa'], 'formata_Tipo_Texto_Tabela');
        $listagemSimec->setFormFiltros('formulario-relatorio');
        $listagemSimec->setTamanhoPagina(50);
        $listagemSimec->setCampos($arrayCabecalho);
        $listagemSimec->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);
    }
}
