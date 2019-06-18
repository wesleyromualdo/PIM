<form method="post" name="fmconfirmarempenho" id="fmconfirmarempenho" class="form form-horizontal"  onsubmit="$('#submitempenhar').button('loading');">
<div class="ibox-content">

    <div class="ibox-title">
    <h5><i class="fa fa-angle-double-right"></i> Solicitação de Empenho</h5>
    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
    </div>
    <?php
    $sql = "SELECT
				i.estuf as uf,
                i.inuid,
				CASE WHEN i.muncod IS NOT NULL THEN m.mundescricao
					 WHEN i.muncod IS NULL THEN e.estdescricao END as descricao,
				pro.pronumeroprocesso,
				formata_cpf_cnpj(iu.entcnpj) as entcnpj
		 
			FROM par3.processo pro
			INNER JOIN par3.instrumentounidade i ON i.inuid = pro.inuid
			LEFT JOIN territorios.municipio m ON m.muncod = i.muncod
			LEFT JOIN territorios.estado e ON e.estuf = i.estuf
			LEFT JOIN par3.instrumentounidade_entidade iu ON iu.inuid = i.inuid AND entstatus='A' and CASE WHEN i.muncod IS NOT NULL THEN iu.tenid=".TENID_PREFEITURA."
														       											   WHEN i.estuf IS NOT NULL THEN iu.tenid=".TENID_SECRETARIA_EDUCACAO." END AND iu.entcnpj IS NOT NULL
			WHERE pro.proid='".$_REQUEST['proid']."'";
    
    $dadosprocesso = $db->pegaLinha($sql);
    ?>
    <table class="table">
    <tbody>
    <tr>
        <td align="right" width="30%"><b>UF:</b></td>
        <td><?=$dadosprocesso['uf'] ?></td>
    </tr>
    <tr>
        <td align="right" width="30%"><b>Local:</b></td>
        <td><?=$dadosprocesso['descricao'] ?></td>
    </tr>
    <tr>
        <td align="right" width="30%"><b>Número do Processo:</b></td>
        <td><?=$dadosprocesso['pronumeroprocesso'] ?></td>
    </tr>
    </tbody>
    </table>
    <?php
    
    $empenhos = carregaListaEmpenhoProcesso($dadosprocesso['pronumeroprocesso']);
    
    include_once APPRAIZ.'par3/classes/controller/AnalisePlanejamentoIniciativa.class.inc';
    
    $anaid = $db->pegaUm("SELECT anaid FROM par3.processoparcomposicao WHERE proid = {$_REQUEST['proid']} AND ppcstatus = 'A'");
    $obAnaliseIniciativa = new Par3_Controller_AnalisePlanejamentoIniciativa($anaid);
    $arAnalise = $obAnaliseIniciativa->carregarAnalisePlanejamento($anaid);
    
    ?>
    <div class="ibox">
    <div class="ibox-content">
        <div class="row">
            <div class="col-md-12 orcamento">
                <?php
                if ($arAnalise['intaid'] == 2) {
                    $_REQUEST['iniano'] = $arAnalise['anaano'];
                    $_REQUEST['boEmpenho'] = 'S';
                    $obEmenda = new Par3_Controller_EmendasParlamentares();
                    echo $obEmenda->pegaBeneficiarioSelecionado('', $arAnalise['inpid']);
                    //if( $anaid ) $emecod = $obEmenda->pegaCodigoEmenda($anaid);
                } ?>
            </div>
        </div>
    </div></div>
    
    <?php if ($empenhos[0]) : ?>
    <div class="panel panel-info">
        <div class="panel-heading">Lista de Empenho(s) do Processo</div>
        
        <div class="panel-body">
        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive">
        <thead>
            <tr>
                <th class="td_cancelamento"></th>
                <th>Atualizar</th>
                <th>Visualizar</th>
                <th>Cancelar</th>
                <th>Nota do Empenho</th>
                <th>Situação do Empenho</th>
                <th>Usuário</th>
                <th>CNPJ</th>
                <th>Número do Protocolo</th>
                <th>Valor Solicitado</th>
                <th>Valor Cancelado</th>
                <th>Valor Empenhado</th>
                <th>Valor Pago</th>
            </tr>
        </thead>
        <tbody>
        <?php
        foreach ($empenhos as $emp) {
            
            
            
            echo '<tr>';
            echo '<td class="text-center td_cancelamento" style="width:33px"><a href="javascript:expandirLinha(\'carregaEmpenhoCancelado\', [\''.$emp['empid'].'\'], \''.$emp['empid'].'\');" title="Detalhar" id="arow-'.$emp['empid'].'"><span class="btn btn-primary btn-sm glyphicon glyphicon-plus"></span></a></td>';
            echo '<td><span class="btn btn-success btn-sm glyphicon glyphicon-refresh" onclick="exibirConsultarEmpenho('.$emp['empid'].');"></span></td>';
            echo '<td><span class="btn btn-primary btn-sm glyphicon glyphicon-search" style="cursor:pointer;" onclick="exibirComposicaoEmpenho('.$emp['empid'].');"></span></td>';
            if ($emp['valorsaldo'] > 0) {
                echo '<td><span class="btn btn-warning btn-sm glyphicon glyphicon-remove" onclick="exibirCancelarEmpenho('.$emp['empid'].', '.$emp['numeroprotocolo'].', \''.$emp['valorsaldo'].'\', \''.$emp['numeroempenho'].'\');"></span></td>';
            } else {
                echo '<td>-</td>';
            }
            echo '<td>'.$emp['numeroempenho'].'</td>';
            echo '<td>'.$emp['situacaoempenho'].'</td>';
            echo '<td>'.$emp['usuario'].'</td>';
            echo '<td>'.$emp['cnpj'].'</td>';
            echo '<td>'.$emp['numeroprotocolo'].'</td>';
            echo '<td>'.simec_number_format($emp['vlr_solicitado'], 2, ",", ".").'</td>';
            echo '<td id="td_cancelamento_valor">'.simec_number_format($emp['vlr_cancelado'], 2, ",", ".").'</td>';
            echo '<td>'.simec_number_format($emp['valorsaldo'], 2, ",", ".").'</td>';
            echo '<td>'.simec_number_format($emp['vlr_pagamento'], 2, ",", ".").'</td>';
            echo '</tr>';
        }
        
        ?>
        </tbody>
        </table>

        </div>
    </div>
    <?php endif;
    
    $sql = "SELECT inpid FROM par3.processoparcomposicao WHERE proid = {$_REQUEST['proid']}
            UNION ALL
            SELECT o.inpid FROM par3.processoobracomposicao p
            	INNER JOIN par3.obra o ON o.obrid = p.obrid
            WHERE proid = {$_REQUEST['proid']}";
    $arInpid = $db->carregarColuna($sql);
    if( $arInpid ) $strInpid = implode(',', $arInpid);
    
    $obPendenciaBloqueio = new Par3_Controller_Pendencia();
    $temPendencia = $obPendenciaBloqueio->verificaPendenciaBloqueio('empenho', $dadosprocesso['inuid'], '', $_REQUEST['proid'], false, true, true, true, $strInpid);
    
    $htmlerro = "";
    if( $temPendencia == true ){
        $htmlerro = "Não é possível Solicitar Empenho. Para Solicitar Empenho, será necessário resolver as Pendências do PAR";
        $btn = "<span>-</span>";
        $btnObra = "<span>-</span>";
    } else {
        $btn = "<input type=\"checkbox\" name=\"ppcid[]\" onchange=\"selecionarSubacaoEmpenho(\"'||id||'\");\" class=\"js-switch\" id=\"chk_'||id||'\" value=\"'||id||'\" >";
        $btnObra = "<input type=\"checkbox\" name=\"pocid[]\" onchange=\"selecionarSubacaoEmpenho(\"'||v.pocid||'\");\" class=\"js-switch\" id=\"chk_'||v.pocid||'\" value=\"'||v.pocid||'\" >";
    }
    
    $sql = "(
            SELECT id,
	               CASE WHEN  coalesce(sum(valorempenhado),0) >= totalitem THEN '<span class=\"btn btn-primary btn-sm glyphicon glyphicon-ok\"></span>'
				     		 ELSE '{$btn}' END as btn,
						'<span class=\"btn btn-primary btn-sm glyphicon glyphicon-eye-open\" style=\"cursor:pointer;\" onclick=\"exibirComposicaoIniciativaPar('||id||');\"></span>' as btn2,
	               descricaoitem, anoitem, totalitem,  (totalitem - sum(contrapartida)) as valorfnde, sum(contrapartida) AS contrapartida, codigo, sum(valorempenhado) AS valorempenhado, sum(percentual) AS percentual FROM(
				SELECT DISTINCT
						v.ppcid as id,
						v.iniciativa as descricaoitem,
						v.ppcano as anoitem,
						v.vlriniciativa as totalitem,
                        COALESCE(v.contrapartida,0) as contrapartida,
						v.inpid as codigo,
						v.saldo as valorempenhado,
                        CASE WHEN v.vlriniciativa > 0 THEN ((coalesce((v.saldo),0)/v.vlriniciativa)*100)::numeric(20,2) ELSE 0 END AS percentual
				FROM par3.v_iniciativa_planejamento_processo_empenho v WHERE v.proid = '".$_REQUEST['proid']."'
				) AS foo
				GROUP BY id, descricaoitem, anoitem, totalitem, codigo
				) UNION ALL (
				SELECT DISTINCT
					   v.pocid as id,
					   CASE WHEN sum(v.saldo) >= v.valorobra  THEN '<span class=\"btn btn-primary btn-sm glyphicon glyphicon-ok\"></span>'
						    ELSE '{$btnObra}' END as btn,
					   '<span class=\"btn btn-primary btn-sm glyphicon glyphicon-eye-open\" style=\"cursor:pointer;\" onclick=\"exibirComposicaoIniciativaObra('||v.pocid||');\"></span>' as btn2,
					   v.iniciativa as descricaoitem,
					   v.obrano as anoitem,
					   v.obrvalor as totalitem,
                       v.valorobra as valorfnde,
                       COALESCE(v.obrvalor_contrapartida,0) as contrapartida,
					   v.obrid as codigo,
					   sum(v.saldo) as valorempenhado,
                       CASE WHEN v.valorobra > 0 THEN ((sum(v.saldo)/v.valorobra)*100)::numeric(20,2) ELSE 0 END AS percentual
				FROM par3.v_obras_planejamento_processo_empenho v WHERE v.proid = '".$_REQUEST['proid']."'
				GROUP BY v.pocid, v.valorobra, v.iniciativa, v.obrano, v.obrid, v.obrvalor_contrapartida, v.obrvalor
				)";
    
    $iniciativas = $db->carregar($sql);
    
    if( $htmlerro ){
?>
    <div class="row">
        <div class="alert alert-danger text-center col-md-8 col-md-offset-2">
        	<?php echo $htmlerro;?>
        </div>
    </div>
<?php }?>
    <div class="panel panel-success">
        <div class="panel-heading">Composição do Processo</div>
        
        <div class="panel-body">
        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive">
        <thead>
            <tr>
                <th>&nbsp;</th>
                <th>&nbsp;</th>
                <th>Código</th>
                <th>Ano</th>
                <th>Descrição</th>
                <th>Valor Total</th>
                <th>Valor de Contrapartida</th>
                <th>Valor FNDE</th>
                <th>% Empenhado</th>
                <th>Valor Empenhado</th>
                <th>% a Empenhar</th>
                <th>Valor a Empenhar</th>
                
            </tr>
        </thead>
        <tbody>
        <?php
        if ($iniciativas[0]) {
            foreach ($iniciativas as $ini) {
                echo '<tr>';
                echo '<td>'.$ini['btn'].'</td>';
                echo '<td>'.$ini['btn2'].'</td>';
                echo '<td>'.$ini['codigo'].'</td>';
                echo '<td>'.$ini['anoitem'].'</td>';
                echo '<td>'.$ini['descricaoitem'].'</td>';
                echo '<td>'.simec_number_format($ini['totalitem'], 2, ",", ".").'<input type="hidden" id="totalitem_'.$ini['id'].'" value="'.($ini['totalitem'] - $ini['contrapartida']).'"></td>';
                echo '<td>'.simec_number_format($ini['contrapartida'], 2, ",", ".").'<input type="hidden" id="contrapartida_'.$ini['id'].'" value="'.$ini['contrapartida'].'"></td>';
                echo '<td>'.simec_number_format($ini['valorfnde'], 2, ",", ".").'</td>';
                echo '<td>'.number_format_par3($ini['percentual']).'%<input type="hidden" id="porcempenhado_'.$ini['id'].'" value="'.$ini['percentual'].'"></td>';
                echo '<td>'.simec_number_format($ini['valorempenhado'], 2, ",", ".").'<input type="hidden" id="valorempenhado_'.$ini['id'].'" value="'.$ini['valorempenhado'].'"></td>';
                echo '<td><input name="porc_empenhar['.$ini['id'].']" id="porc_empenhar_'.$ini['id'].'" readonly="readonly" type="text" value="" class="form-control" maxlength="255" style="text-align: right; width: 100px;" oninput="this.value = mascaraglobal(\'###\', this.value); calcularIniciativas(\''.$ini['id'].'\', \'porcentagem\')"></td>';
                echo '<td><input name="valor_empenhar['.$ini['id'].']" id="valor_empenhar_'.$ini['id'].'" readonly="readonly" type="text" value="" class="form-control" maxlength="255" required="required" style="text-align: right; width: 200px;" oninput="this.value = mascaraglobal(\'###.###.###.###,##\', this.value); calcularIniciativas(\''.$ini['id'].'\', \'valor\');"></td>';
                echo '</tr>';
            }
        }
        
        ?>
        </tbody>
        </table>
        </div>
    </div>
    
    
    <div class="ibox-footer">
        <div class="form-actions col-md-offset-5">
<?php if( $temPendencia == false ){?>
            <button type="button" class="btn btn-success" onclick="solicitarEmpenho();"><i class="fa fa-plus-square-o"></i> Solicitar Empenho</button>
<?php } ?>
            <button type="button" class="btn btn-white" data-dismiss="modal">Fechar</button>
        </div>
    </div>
    

</div>


<div class="ibox float-e-margins animated modal" id="dvconfirmarempenho" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-title">
                <h5><i class="fa fa-angle-double-right"></i> Confirmar Empenho</h5>
            </div>

                <div class="ibox-content">
                    <input type="hidden" name="requisicao" id="requisicao" value="confirmarEmpenho">
                    <input type="hidden" name="tipoobjeto" id="tipoobjeto" value="<?php echo $_REQUEST['tipoobjeto']?>">
                    <?php
                        echo $simec->input('entcnpj', 'CNPJ', $dadosprocesso['entcnpj'], array('maxlength' => '255', 'required', 'readonly'));
                        echo $simec->input('pronumeroprocesso', 'Número do processo', $dadosprocesso['pronumeroprocesso'], array('maxlength' => '255', 'required', 'readonly'));
                        echo $simec->input('valortotalempenho', 'Valor Total do Empenho', "", array('maxlength' => '255', 'required', 'readonly'));
                        
                        $recursos = "(SELECT 'C' as codigo, 'Custeio' as descricao) UNION ALL
								  	 (SELECT 'A' as codigo, 'Capital' as descricao)";
                        
                        echo $simec->select('categoriadespesa', 'Categoria de Despesa', "", $recursos, array('required'));
                        
                        $ptres = "SELECT DISTINCT inplintptres as codigo, inplintptres as descricao
			  					  FROM par3.iniciativa_plano_interno p
			  					  LEFT JOIN par3.programa pp ON pp.prgid = p.inplinprog
			  					  WHERE inplintstatus='A' AND inplintsituacao='A' AND
						   		 (p.inplinprog IN( SELECT prgid FROM par3.iniciativa_iniciativas_programas prg
												  INNER join par3.iniciativa_planejamento inp on inp.iniid = prg.iniid AND inp.inpstatus = 'A'
												  INNER join par3.processoparcomposicao ppc on ppc.inpid = inp.inpid AND ppc.ppcstatus = 'A'
										  		WHERE ppc.proid='".$_REQUEST['proid']."' ) OR
						    	  p.inplinprog IN( SELECT prgid FROM par3.iniciativa_iniciativas_programas prg
												  INNER join par3.iniciativa_planejamento inp on inp.iniid = prg.iniid AND inp.inpstatus = 'A'
										  		  INNER JOIN par3.obra obr ON obr.inpid = inp.inpid
										  		  INNER join par3.processoobracomposicao poc on poc.obrid = obr.obrid AND poc.pocstatus = 'A'
										  		WHERE poc.proid='".$_REQUEST['proid']."' ))
								  ORDER BY 2";
                        
                        echo $simec->select('ptres', 'PTRes', "", $ptres, array('required', 'onchange'=>'carregarDadosPtres(this.value,'.$_REQUEST['proid'].');'));
                        
                        
                        echo '<div id="dvplanointerno">';
                        carregarPlanoInterno(array('proid'=>$_REQUEST['proid']));
                        echo '</div>';
                        
                        echo '<div id="dvfonterecurso">';
                        carregarFonteRecurso(array());
                        echo '</div>';
                        
                        $centrogestao = "( SELECT '61400000000' as codigo, '61400000000' as descricao ) UNION ALL
										 ( SELECT '61500000000' as codigo, '61500000000' as descricao ) UNION ALL
										 ( SELECT '61700000000' as codigo, '61700000000' as descricao ) UNION ALL
										 ( SELECT '69500000000' as codigo, '69500000000' as descricao )";
                        
                        echo $simec->select('centrogestao', 'Centro de Gestão', "", $centrogestao, array('required'));
                    ?>
                    
                    <div class="panel panel-default">
                                        <div class="panel-heading">
                                            Autenticação SIGEF
                                        </div>
                                        <div class="panel-body">
                                        <?php
                                        echo $simec->input('wsusuario', 'Usuário', "", array('maxlength' => '255', 'required'));
                                        ?>
                                        <div class="form-group ">
                                            <label for="wssenha" class="col-sm-3 col-md-3 col-lg-3 control-label">Senha: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                                <input name="wssenha" id="wssenha" type="password" value="" class="form-control" maxlength="255" required="required">
                                            </div>
                                            <div style="clear:both"></div>
                                        </div>
                                        
                                        </div>

                                    </div>
                </div>
                <div class="ibox-footer">
                    <div class="form-actions col-md-offset-2">
                        <button type="submit" class="btn btn-success" id="submitempenhar"> <i class="fa fa-plus-square-o"></i> Empenhar</button>
                        <button type="button" class="btn btn-danger" onclick="visualizarXMLSigef();"> <i class="fa fa-eye" aria-hidden="true"></i> Visualizar XML SIGEF</button>
                        <button type="button" class="btn btn-white" onclick="$('#dvconfirmarempenho').modal('toggle');$('#modal-form-large').modal('show');">Fechar</button>
                    </div>
                </div>
        </div>
    </div>
</div>

<div class="ibox float-e-margins animated modal" id="dvcomposicaoiniciativa" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-title"><h5><i class="fa fa-angle-double-right"></i> Composição da Iniciativa Planejamento</h5></div>
            <form method="post" name="teste" id="teste" class="form form-horizontal">
            <div class="ibox-content" id="dvdadoscomposicaoiniciativa"></div>
            </form>
            <div class="ibox-footer" style="text-align: center;">
                <button type="button" class="btn btn-success" onclick="$('#dvcomposicaoiniciativa').modal('toggle');$('#modal-form-large').modal('show');">Fechar</button>
            </div>
        </div>
    </div>
</div>

<div class="ibox float-e-margins animated modal" id="dvcomposicaoempenho" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-title"><h5><i class="fa fa-angle-double-right"></i> Composição do Empenho</h5></div>
            <div class="ibox-content" id="dvdadoscomposicaoempenho"></div>
            <div class="ibox-footer" style="text-align: center;">
                <button type="button" class="btn btn-success" onclick="$('#dvcomposicaoempenho').modal('toggle');$('#modal-form-large').modal('show');">Fechar</button>
            </div>
        </div>
    </div>
</div>


</form>

<style>
        .loading-dialog-par4 {
            position: fixed;
            top : 0;
            bottom : 0;
            left : 0;
            right : 0;
            margin: auto;
            background-color: #3c3c3c;
            filter: alpha(opacity = 80);
            opacity: 0.80;
            -moz-opacity: 0.80;
            cursor: default;
            display: none;
            overflow: hidden;
            padding: 0 0;
            align:center
            text-align: center;
            width: 100%;
            height: 100%;
        }
       </style>
<div class="loading-dialog-par4"> </div>
<div class="ibox float-e-margins animated modal" id="dvautenticacaosigef" tabindex="10000" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <form method="post" name="fmautenticar" id="fmautenticar" class="form form-horizontal">
                <input type="hidden" name="empid" id="empid_autenticacao" value="">
                <input type="hidden" name="protocolo" id="protocolo_autenticacao" value="">
                <input type="hidden" name="valor_empenho_cancel" id="valor_empenho_cancel" value="">
            <div id="div_cancelar" style="display: none">
            <div class="panel panel-info">
                <div class="panel-heading" style="text-align: center;"><h3>Cancelar Empenho</h3></div>
                <div class="panel-body">
                    <div id="lista_empenho"></div>
                </div>
            </div>
            </div>
            <div class="panel panel-success">
                <div class="panel-heading" style="text-align: center;"><h3>Autenticação SIGEF</h3></div>
                    <div class="panel-body">
                        <div class="ibox-content">
                            <?php echo $simec->input('wsusuario', 'Usuário', "", array('maxlength' => '50', 'required')); ?>
                            <div class="form-group ">
                                <label for="wssenha" class="col-sm-3 col-md-3 col-lg-3 control-label">Senha: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                                <div class="col-sm-9 col-md-9 col-lg-9 "><input name="wssenha" id="wssenha" type="password" value="" class="form-control" maxlength="50" required="required"></div>
                            </div>
                            <div class="ibox-footer">
                                <div class="form-actions col-md-offset-2">
                                    <div id="div_botao_consulta">
                                        <button type="button" class="btn btn-success" onclick="consultarEmpenho(<?=$_REQUEST['proid'] ?>);"> <i class="fa fa-plus-square-o"></i> Consultar Empenho</button>
                                        <button type="button" class="btn btn-white" onclick="$('#dvautenticacaosigef').modal('toggle');$('#modal-form-large').modal('show');;jQuery('.loading-dialog-par4').hide();">Fechar</button>
                                    </div>
                                    <div id="div_botao_cancela" style="display: none;">
                                        <button type="button" class="btn btn-success" onclick="cancelarEmpenhoWS(<?=$_REQUEST['proid'] ?>);"> <i class="fa fa-plus-square-o"></i> Cancelar Empenho</button>
                                        <button type="button" class="btn btn-white" onclick="$('#dvautenticacaosigef').modal('toggle');$('#modal-form-large').modal('show');jQuery('.loading-dialog-par4').hide();">Fechar</button>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            </form>
        </div>
    </div>





