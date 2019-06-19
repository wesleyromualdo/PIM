<?php

	require APPRAIZ . 'obras2/includes/principal/cadOs/ctrl.php';
	//Chamada de programa
	include  APPRAIZ."includes/cabecalho.inc";

	criaAbaOS();
	monta_titulo_obras( 'Cadastro de OS', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<style>.link{cursor:pointer}</style>
<form id="formulario_os" name="formulario_os" method="post" enctype="multipart/form-data" action="">
	<input type="hidden" name="sosid" id="sosid" value="<?php echo $sosid ?>" />
	<input type="hidden" name="op" id="op" value="salvar" />
	<input type="hidden" name="hdn_mescod" id="hdn_mescod" value="<?php echo $arMescod ? implode(",",$arMescod) : "" ?>">
	<input type="hidden" name="hdn_orgid" id="hdn_orgid" value="<?php echo $orgid ?>">
	<input type="hidden" name="estuf" id="estuf" value="SP"/>
	<input type="hidden" name="sgrid" id="sgrid" value="12"/>


	<div class="form-filters">
		<div class="row">
			<div class="col-md-12  ">
      	<h4>Dados da OS</h4>
      </div>
    </div>


    <div class="row">
    	<div class="col-md-2">
    		Tipo da OS
    	</div>
    	<div class="col-md-6">
	      <?php
	          $select_c = '';
	          $select_r = '';
	          switch ($sostipo) {
	              case 'C':
	                  $select_c = 'checked="checked"';
	                  break;
	              case 'R':
	                  $select_r = 'checked="checked"';
	                  break;
	              default :
	                  $select_c = 'checked="checked"';
	                  break;
	          }
	      ?>
		      <div class="btn-group" data-toggle="buttons">
	            <label class="btn btn-default <?= !empty($select_c)?'active':'' ?>">
		         		 <input <?php echo ($habil ? '' : 'disabled="disabled"'); ?> type="radio" name="sostipo" id="sostipo_c" value="C" <?php echo $select_c; ?> > Convencional &nbsp;
		         	</label>
	            <label class="btn btn-default <?= !empty($sostipo_r)?'active':'' ?>">
		          	<input <?php echo ($habil ? '' : 'disabled="disabled"'); ?> type="radio" name="sostipo" id="sostipo_r" value="R" <?php echo $select_r; ?> > Retificação
		          </label>

		          <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		      </div>
    	</div>
    </div> 
    <div class="row">
    	<div class="col-md-2">
    		Tipo de Ensino
    	</div>
    	<div class="col-md-6">
    		<?php
            $orgao = new Orgao();
            $dados = $orgao->listaCombo();

             

            $db->monta_combo("orgid", $dados, $somenteLeitura, "Selecione...", "carregaDependenciaGrupoPorOrgid", '', '', '', 'S', 'orgid', false, null, null,  null, 'form-control');
        ?>
    	</div>
    </div> 
    <div class="row">
    	<div class="col-md-2">Empresa
    	</div>
    	<div class="col-md-6" id="tdComboEmpresa">
                    <?php
                    $grupoEmpresa = new Supervisao_Grupo_Empresa();
                    $dados = $grupoEmpresa->listaCombo( array('sgrid' => "12") );

                    $db->monta_combo("sgeid", $dados, $somenteLeitura, "Selecione...", "carregaDependenciaEmpresa", '', '', '', 'S', 'sgeid');
                    ?>
			</div>
		</div>

    <div class="row">
    	<div class="col-md-2">
    		Nota de Empenho
    	</div>
			<div class="col-md-6" id="tdEmpenho">
                    <?php
                        if ( !empty($sgeid) ){
                            $empenho = new Supervisao_Empenho();
                            $dados = $empenho->listaCombo( array('sgeid' => $sgeid) );

                            $db->monta_combo("semid", $dados, $somenteLeitura, "Selecione...", "", '', '', '', 'S', 'semid');
                        }
                    ?>
			</div>
		</div>
    <div class="row">
    	<div class="col-md-2">
    		OS Nº
    	</div>
			<div class="col-md-6" >
        	<?php $sosnum = ($sosnum) ? $sosnum : ($sosid && $sgrid && $sgrid ? $sosid.$sgrid.$sgeid.date("Y") : ""); ?>
				<?=campo_texto('sosnum', 'S', "N", '', 10, 10, '', '', 'right', '', 0, 'id="sosnum"', ''); ?>
      </div>
    </div>
    <div class="row">
    	<div class="col-md-2">
    		Data Emissão
    	</div>
			<div class="col-md-6" >
            <?php
                echo campo_data2( 'sosdtemissao', 'S', $somenteLeitura, '', 'S','','' );
            ?>
      </div>
    </div>
    <div class="row">
    	<div class="col-md-2">
    		Período de Execução do Serviço
    	</div>
			<div class="col-md-6" > 
    		<div>
	            <b>De</b>
                    <?php
                        echo campo_data2( 'sosdtinicio', 'S', (($prorrogacao)?'N':$somenteLeitura), '', 'S','', '', '','controlaPeriodoExecucao();' );
	            ?>
	            <div id="div_sosdttermino" style="display:<?php echo ($sosdttermino ? 'inline' : 'none') ?>;">
                        <b>até</b> <?php echo formata_data( $sosdttermino ); if($prorrogacao) echo " <a title='Ver Lista de Prorrogações' onclick='historicoProrrogacao(".$sosid.")' style='color:red;font-size:10px;'>(Visualizar Prorrogações)</a>"; ?>
	            </div> 
            </div>
        </div>
		</div>

		<div class="row">
			<div class="col-md-2"></div>

			<div class="col-md-6">
				
					    <div class="btn-group" data-toggle="buttons">
                <label class="btn btn-default " for="sosemergencial_10" >
			            <input type="radio" name="sosemergencial" id="sosemergencial_10" value="10" onchange="controlaPeriodoExecucao( this.value )" <?php echo ($sosemergencial == 'f' && $sosdiasexecucao == 10  ? 'checked="checked"' : ''); ?> <?php echo ($habil && !$prorrogacao ? '' : 'disabled="disabled"'); ?>> 10 DIAS
	          		</label>
	          		<label class="btn btn-default " for="sosemergencial_20">	
			            <input type="radio" name="sosemergencial" id="sosemergencial_20" value="20" onchange="controlaPeriodoExecucao( this.value )" <?php echo ($sosemergencial == 'f' && $sosdiasexecucao == 20 ? 'checked="checked"' : ''); ?> <?php echo ($habil && !$prorrogacao ? '' : 'disabled="disabled"'); ?>>
			          	20 DIAS</label> 

			          <label class="btn btn-default " for="sosemergencial_30">		
			            <input type="radio" name="sosemergencial" id="sosemergencial_30" value="30" onchange="controlaPeriodoExecucao( this.value )" <?php echo ($sosemergencial == 'f' && $sosdiasexecucao == 30 ? 'checked="checked"' : ''); ?> <?php echo ($habil && !$prorrogacao ? '' : 'disabled="disabled"'); ?>>
			          30 DIAS</label>

			        <label class="btn btn-default " for="sosemergencial_f">
	            	<input type="radio" name="sosemergencial" id="sosemergencial_t" value="t" onchange="controlaPeriodoExecucao( this.value )" <?php echo ($sosemergencial == 't' ? 'checked="checked"' : ''); ?> <?php echo ($habil && !$prorrogacao ? '' : 'disabled="disabled"'); ?>>
	            EMERGENCIAL</label>

 

	            <input type="hidden" name="sosdttermino" id="sosdttermino" value="<?=formata_data( $sosdttermino )?>">
	            <span id="span_sosdiasexecucao" style="display: <?php echo ($sosemergencial == 't' ? 'inline' : 'none') ?>;"> 
	            		<input type="hidden" id="sosdiasexecucao" onChange="addDiaPeriodoExecucao( this.value )"/> 
	           	</span>

	          </div>
	  	</div> 
		</div>


    <div class="row">
        <div class="col-md-2">Tipo de Supervisão
        </div>
        <div class="col-md-6">
        	<div class="btn-group" data-toggle="buttons">
        		<label class="btn btn-default ">
            <input <?php echo ($habil ? '' : 'disabled="disabled"'); ?> type="radio" name="sosterreno" id="" value="false" <?= ($sosterreno == 'f') ? 'checked="checked"' : '' ?> > Obra &nbsp;
            </label>
            <label class="btn btn-default ">
            <input <?php echo ($habil ? '' : 'disabled="disabled"'); ?> type="radio" name="sosterreno" id="" value="true"  <?= ($sosterreno == 't') ? 'checked="checked"' : '' ?> > Implantação de Obra
            </label>
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
          </div>
        </div>
    </div>
    <div class="row">
	    <div class="col-md-2">
            Valor Total dos Serviços (R$)
            <input type="hidden" name="sosvalortotal" id="sosvalortotal" value="<?php echo $sosvalortotal; ?>">
      </div>
      <div class="col-md-6">
				<span id="valorTotalObra">
				<?php echo ($sosvalortotal ? number_format($sosvalortotal, 2, ',', '.') : '-'); ?>
				</span>
				<span id="qtdTotalObra">
				<?php echo ( count($arEmpidAtivos) ? ' - ' . count($arEmpidAtivos) . ' Obra(s)'  :  '' ) ?>
				</span>
			</div>
		</div>
    <div class="row">
	    <div class="col-md-2">Observação:
	    </div>
	    <div class="col-md-6"> 

	    	<?= campo_textarea( 'sosobs', 'N', 'S', '', null, 4, 1000 , '', 0,'',false, null, null, '100%'); ?>
	    </div>
	  </div>
  <?php
      if ( $habil ):
  ?> 
  <div class="form-filters">
  </div>

  <div class="row ">
	    <div class="col-md-12 text-center">
	    	<?= monta_titulo_obras( 'Obra(s) da OS', '<a class="btn btn-info" onclick="popUpAddObra()"><span class="glyphicon glyphicon-plus"></span> Incluir Obras </a>' ) ?>

 
	    	
	    </div>
	  </div>

  <?php
      endif;
  ?>

    
    <div class="row">
	    <div class="col-md-12">
			<div id="divListaObra" style="height: 300px; overflow: auto;">
		<?php
		if ( $sgeid ){
      $grupoEmpresa  = new Supervisao_Grupo_Empresa();
      $valorUnitario = $grupoEmpresa->pegaValorUnitarioPorSgeid( $sgeid );
      $valorUnitariob = $grupoEmpresa->pegaValorUnitariobPorSgeid($sgeid);
		?>
        <input type="hidden" rel="<?= number_format($valorUnitario, 2, ',', '.'); ?>" name="sgevalorunitario" id="sgevalorunitario" value="<?php echo $valorUnitario; ?>">
        <input type="hidden" rel="<?= number_format($valorUnitariob, 2, ',', '.'); ?>" name="sgevalorunitariob" id="sgevalorunitariob" value="<?php echo $valorUnitariob; ?>">
		<?php
		}
		if ( !empty( $sgrid ) ){
			$supMesoregiao = new Supervisao_Grupo_Mesoregiao();
			$arMescod 	   = $supMesoregiao->pegaMescodPorSgrid( $sgrid );
			if($arMescod){
				echo "<script>$(\"#hdn_mescod\").val('".implode(",",$arMescod)."')</script>";
			}
		}
		?>
		<table class="listagem table table-striped table-lista" width="100%" bgcolor="#FFFFFF" id="lista_obra">
		<?php
		$vlrTotalServico = 0;
		if ( !empty( $sgrid ) ){
			$empreendimento = new Empreendimento();
                        $param = array('mescod' => $arMescod,
						   'orgid'	=> $orgid);

			$arDadoEmp = $empreendimento->listaDados( $param );

            foreach($empreendimento->listaDadosPorEmpid( $arEmpid ) as $emp){
                if( !in_array($emp['mescod'], $arMescod) )
                    $arDadoEmp[] = $emp;
            }

			if ( count($arDadoEmp) ){
		?>
				<thead>
                	<tr style="background-color: #CDCDCD;">
                            <th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                    &nbsp;
                            </th>
                            <th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                    ID Obra
                            </th>
                            <th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                    Nome da Obra
                            </th>
                            <th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                    UF
                            </th>
                            <th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                    Mesorregião
                            </th>
                            <th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                    Microregião
                            </th>
                            <th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                    Município
                            </th>
                            <th  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                    Valor Laudo
                                    <br>
                                    (R$)
                            </th>
                        </tr>
                        </thead>
                        <?php
//                            $osObra  	= new Supervisao_Os_Obra();
//                            $arEmpid  = $osObra->listaEmpidPorOs( $sosid );
                            $i = 0;

                            foreach ( $arDadoEmp as $dadoEmp) {

                                $supCancelada = false;
                                $color = '';//($i % 2 ? '#FFFFFF' : '#FFFFFF');
                                $i++;
                                if (in_array($dadoEmp['empid'], $arEmpid)) {
                                    $supCancelada = supervisaoCancelada($sosid, $dadoEmp['empid']);
                                    if (count($arEmpid) > 10)
                                        $vlrTotalServico += $valorUnitariob;
                                    else
                                        $vlrTotalServico += $valorUnitario;
                                }
                        ?>

                                        <tr style="background-color: <?=$color ?>;display:<?php echo in_array($dadoEmp['empid'], $arEmpid) ? "" : "none" ?>" id="tr_empid_<?php echo $dadoEmp['empid'] ?>">
                                            <td  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                                    <input type="checkbox" onclick="calculaTotalServico();" name="empid[]" id="empid_<?=$dadoEmp['empid'] ?>" value="<?=$dadoEmp['empid'] ?>" <?php echo (in_array($dadoEmp['empid'], $arEmpid) ? 'checked="checked"' : ''); ?> <?php echo ($habil ? '' : 'disabled="disabled"'); ?>>
                                            </td>
                                            <td  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                                <?=$dadoEmp['obrid'] ?>
                                            </td>
                                            <td valign="middle" align="left" style="padding-left: 5px; border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                                    <?='<label for="empid_' . $dadoEmp['empid'] . '">' . $dadoEmp['obrnome'] . '</label>' ?> <? if ($supCancelada) : ?> - <span style="color:red; font-weight: bold">Cancelado</span><?endif;?>
                                            </td>
                                            <td  valign="middle" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                                    <?=$dadoEmp['estuf'] ?>
                                            </td>
                                            <td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                                    <?=$dadoEmp['mesdsc'] ?>
                                            </td>
                                            <td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                                    <?=$dadoEmp['micdsc'] ?>
                                            </td>
                                            <td  valign="middle" align="left" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                                    <?=$dadoEmp['mundescricao'] ?>
                                            </td>
                                            <td class="valorLaudo" valign="middle" align="right" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
                                            <?php
                                                if(count($arEmpid) > 10)
                                                    echo ($valorUnitariob ? number_format($valorUnitariob, 2, ',', '.') : '-');
                                                else
                                                    echo ($valorUnitario ? number_format($valorUnitario, 2, ',', '.') : '-');
                                            ?>
                                            </td>
					</tr>
                        <?php
				}
			}else{
                        ?>
                        <tr style="color: red;">
                            <td>
                                Nenhuma obra encontrada.
                            </td>
                        </tr>
		<?php
			}
		}
		?>
                    </table>
                </div>
		</div>
	</div>
  <div class="row">
  	<div class="col-md-12 text-center">
  		
        <?php
            if ( $habil ):
        ?>
                <button type="button" value="" id="salva_vistoria" class="btn btn-info" onclick="enviaFormulario();">
                	<span class="glyphicon glyphicon-floppy-save"></span> Salvar
                </button>
        <?php
            endif;
        ?>
            <d  class="btn btn-primary"  onclick="location.href='?modulo=principal/listOs&acao=A';">
            	<span class="glyphicon glyphicon-arrow-left"></span> Voltar
            </d>
  	</div>
  </div> 
</div>

</form>
<script>

$1_11(document).ready(function () {
    $1_11('select[name="orgid"]').chosen({ width: '96%' });
    $1_11('select[name="sgeid"]').chosen({ width: '96%' });
    $1_11('select[name="semid"]').chosen({ width: '96%' });
});

$(function() {
    $('#sgrid,#sgeid').change(function(){
        atualizaNumOS();
    });

    $('input[name=sosemergencial]').change(function(){
        var val = $(this).val();
        if ( val == 't' ){
            $('#sosdiasexecucao').val('');
            $('#sosdttermino').val('');
            $('#div_sosdttermino').css('display', 'inline')
                                              .html('');
            $('#span_sosdiasexecucao').show();

            $('#sosdiasexecucao').val('5');
            addDiaPeriodoExecucao( 5 );
            $('#sosdiasexecucao').attr('readonly','readonly');
        }else {
            $('#sosdttermino').val( '' );
            $('#sosdiasexecucao').val(val);
            $('#span_sosdiasexecucao').hide();
            addDiaPeriodoExecucao( parseInt(val) );
        }
    });

    $('#sostipo_r').click(function(){
        $('#valorTotalObra').html( mascaraglobal("###.###.###.###,##", 0) );
    });

});

function calculaTotalServico(){
	var qtdEmpid = $('[name*=empid]:checked').size();
	var valUnit  = (qtdEmpid > 10) ? $('#sgevalorunitariob').val() : 0;
        var valUnitExibicao = (qtdEmpid > 10) ? $('#sgevalorunitariob').attr('rel') : $('#sgevalorunitario').attr('rel');

	var calc 	 = new Calculo();
	var totVal   = calc.operacao(qtdEmpid, valUnit, '*', 2);

	$('#sosvalortotal').val( totVal );
	$('#valorTotalObra').html( mascaraglobal("###.###.###.###,##", totVal) );
	$('#qtdTotalObra').html( ' - ' + qtdEmpid + ' Obras(s)' );
    $('.valorLaudo').html(valUnitExibicao);
}

function enviaFormulario(){

	//$1_11("#loading").fadeIn();
	$.post("?modulo=principal/cadOs&acao=A", {verificaos: true, os: $('#sosnum').val()}, function (data){

		calculaTotalServico();

	    var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	    var validacao = true;

	    if ( $('#sosnum').val() == '' ){
	            mensagem += 'OS Nº \n';
	            validacao = false;
	    }

	    if ( $('#sosdtemissao').val() == '' ){
	            mensagem += 'Data Emissão \n';
	            validacao = false;
	    }

	    if ( $('#sgrid').val() == '' ){
	            mensagem += 'Grupo \n';
	            validacao = false;
	    }

	    if ( $('#sgeid').val() == '' ){
	            mensagem += 'Empresa \n';
	            validacao = false;
	    }

	    if ( $('#semid').val() == '' ){
	            mensagem += 'Nota de Empenho \n';
	            validacao = false;
	    }

	    if ( $('[name*=empid]:checked').size() == 0 ){
	            mensagem += 'Obra(s) \n';
	            validacao = false;
	    }

	    if ( $('#sosdtinicio').val() == '' || $('#sosdttermino').val() == '' ){
	            mensagem += 'Período de Execução do Serviço \n';
	            validacao = false;
	    }

	    var obrMeso = '';
	    $('#divListaObra input[type=checkbox]:checked').each(function(i){
	        obrItemMeso = $(this).parents('tr:first').find('td:eq(4)').html();
	         if (i == 0)
	             obrMeso = $(this).parents('tr:first').find('td:eq(4)').html();

	        if(obrMeso != obrItemMeso){
	            mensagem += 'Não é permitido marcar Obras de diferentes mesorregiões \n';
	            return validacao = false;
	        }
	    }); 

	    if (!validacao){
	        alert(mensagem);
	    }else{
	   		 	//$1_11("#loading").fadeIn();
	       	$('#formulario_os').submit();
	    }

    });
}

function carregaDependenciaEmpresa( sgeid ){
	$('#sosvalortotal').html( '0,00' );
	$('#valorTotalObra').html( '0,00' );
	var sgrid = $('#sgrid').val();
	var sosid = $('#sosid').val();
	if ( sgeid ){
		$()
		$.post("?modulo=principal/cadOs&acao=A", {"sgeid":sgeid, "ajax":"carregaEmpenho"}, function(data){
			$('#tdEmpenho').html( data );
			$1_11('#semid').chosen({ width: '96%' });
			$('#semid').trigger("chosen:updated");
		});

		var orgid = $('#orgid').val();
		$.post("?modulo=principal/cadOs&acao=A", {"sosid":sosid, "sgrid":sgrid, "sgeid":sgeid, "ajax":"carregaEmpresaAndListaObra", "not(comboGrupoEmpresa)":true}, function(data){
			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
			$('#divListaObra').html( listaObra );
		});
	}else{
		$('#tdEmpenho').html('');
		if ( sgrid ){
			carregaDependenciaGrupo( sgrid );
		}
	}
	atualizaNumOS();
}

function carregaDependenciaGrupo( sgrid ){
	$('#sosvalortotal').html( '' );
	$('#valorTotalObra').html( '0,00' );
	if ( sgrid ){
		var orgid = $('#orgid').val();
		$('#hdn_orgid').val(orgid);

		$1_11("#loading").fadeIn();
		$.post("?modulo=principal/cadOs&acao=A", {"sgrid":sgrid, "ajax":"carregaEmpresaAndListaObra", "orgid":orgid}, function(data){
			var comboEmpresa = pegaRetornoAjax('<comboGrupoEmpresa>', '</comboGrupoEmpresa>', data, true);
			$('#tdComboEmpresa').html( comboEmpresa );

			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
			$('#divListaObra').html( listaObra );

			$1_11("#loading").fadeOut();


		});
	}else{
		$('#tdComboEmpresa').html('');
		$('#divListaObra').html('');
	}
	atualizaNumOS();
}

function carregaDependenciaGrupoPorOrgid( orgid ){
	var sgrid = $('#sgrid').val();
	if(orgid){
		$('#hdn_orgid').val(orgid);
	}
	if ( sgrid ){
		$1_11("#loading").fadeIn();
		$.post("?modulo=principal/cadOs&acao=A", {"sgrid":sgrid, "ajax":"carregaEmpresaAndListaObra", "orgid":orgid, "not(comboGrupoEmpresa)":true}, function(data){
			var listaObra = pegaRetornoAjax('<listaObras>', '</listaObras>', data, true);
			$('#divListaObra').html( listaObra );
			$1_11("#loading").fadeOut();
		});
	}
}

function controlaPeriodoExecucao( val ){
	val = val || $('[name=sosemergencial]:checked').val();
	val = val || '';

	if ( val != 't' ){
		$('#sosdttermino').val( '' );
		$('#sosdiasexecucao').val(val);
		$('#span_sosdiasexecucao').hide();
		addDiaPeriodoExecucao( val );

	}else if( val == 't' ){
		$('#sosdiasexecucao').val('');
		$('#sosdttermino').val('');
		$('#div_sosdttermino').css('display', 'inline')
						  .html('');
		$('#span_sosdiasexecucao').show();

        $('#sosdiasexecucao').val('5');
        $('#sosdiasexecucao').attr('readonly','readonly');
        addDiaPeriodoExecucao( 5 ); 
	}
}

function addDiaPeriodoExecucao( numDia ){
    numDia -= 1;
	var obData = new Data();
	var sosdtinicio  = $('#sosdtinicio').val();
	var sosdttermino = (sosdtinicio ? obData.dtAddDia(sosdtinicio, numDia) : '');
	$('#sosdttermino').val( sosdttermino );

	if ( sosdttermino ){
		$('#div_sosdttermino').css('display', 'inline')
						      .html('<b>até</b> ' + sosdttermino);
	}
}

function atualizaNumOS(){
	var os_num = "<?php echo $sosid ? $sosid : $osObra->geraNovoNumOS() ?>";
	var grupo = $('#sgrid').val() ? $('#sgrid').val() : "";
	var empresa = $('#sgeid').val() ? $('#sgeid').val() : "";
	var ano = "<?php echo date("Y") ?>";
	$('#sosnum').val(os_num+grupo+empresa+ano);
}

function popUpAddObra(){
	var sgrid = $('#sgrid').val();
	var orgid = $("#hdn_orgid").val();
	var mescod = $("#hdn_mescod").val();
	if(sgrid){
		return windowOpen( '?modulo=principal/cadOs&acao=E&classe=Empreendimento&requisicao=exibirListaObraPopUp&sosid=<?php echo $sosid ?>&orgid='+orgid+'&mescod='+mescod+'&sgrid='+sgrid,'blank',
						'height=700,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}else{
		alert('Favor selecionar o Grupo.');
	}
}

function removeObraOS(empid){
	$("#tr_empid_" + empid).hide();
	$("#empid_" + empid).attr("checked",false);
	calculaTotalServico();

}

function addObraOS(empid){
	$("#tr_empid_" + empid).show();
	$("#empid_" + empid).attr("checked",true);
	calculaTotalServico();

        if($('[name=sostipo]:checked').val() == 'R'){
            $('#valorTotalObra').html( mascaraglobal("###.###.###.###,##", 0) );
        }
}

function historicoProrrogacao( sosid ){
	popupProrrogacao = window.open( 'obras2.php?modulo=principal/visualizacaoHistoricoProrrogacaoOS&acao=A&sosid='+sosid, 'Prorrogacao', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
}
</script>

<?php ?>
