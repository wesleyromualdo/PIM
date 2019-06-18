<script>
$(document).ready(function(){

	$(document).on('click','.seleciona',function(){
		var nesid = $(this).attr('nesid');
		var adpid = $(this).attr('adpid');
		ajax('requisicao=vincularEscola&nesid='+nesid+'&adpid='+adpid);
	});

	$('.filtra').click(function(){
		var tipo = $(this).attr('tipo');
		ajaxatualizar('requisicao=listarEscolas&'+$('#form_filtro_'+tipo).serialize(),'div_lista_'+tipo);
	});
});
</script>
<div class="panel-body">
    <?php $objOrientacao->getOrientacaoByID(ORIENTACOES_NEM_ABA_ESCOLAS, "Escolas"); ?>
    <hr class="divider">
    <div id="div_escolas" style="<?php echo $displayescolas ?>">
        <input type="hidden" value="<?php echo $esdid ?>" id="esdid_atual" name="esdid_atual"/>
        <div class="col-md-1 input-lg text-right">
            <a onclick="$('#dv_escolas_sorteadas').modal();" title="Visualizar escolas aptas" href="#">
                <span aria-hidden="true" class="glyphicon glyphicon-list success"></span>
            </a>
        </div>
        <div class="col-md-11 input-lg">
            Escolas aptas  Sorteadas para Avaliação de Impacto EMTI
        </div>
        
        <div class="col-md-1 input-lg text-right">
            <a onclick="$('#dv_escolas_aptas').modal();" title="Visualizar Escolas não aptas" href="#">
                <span aria-hidden="true" class="glyphicon glyphicon-list primary"></span>
            </a>
        </div>
        <div class="col-md-11 input-lg">
            Escolas aptas  Elegíveis pela rede
        </div>
        
        <div class="col-md-1 input-lg text-right">
            <a onclick="$('#dv_escolas_nao_aptas').modal();" title="Visualizar Escolas não aptas" href="#">
                <span aria-hidden="true" class="glyphicon glyphicon-list danger"></span>
            </a>
        </div>
        <div class="col-md-11 input-lg">
            Lista de escolas não aptas
        </div>
    </div>
</div>
<div class="ibox float-e-margins animated modal" id="dv_escolas_sorteadas" tabindex="-1" role="dialog" aria-hidden="true">
	<div class="modal-dialog modal-lg">
		<div class="modal-content">
			<div class="ibox-title" id="title_carga">
				<h3>Escolas aptas  Sorteadas para Avaliação de Impacto EMTI</h3>
			</div>
			<div class="ibox-content" id="title_carga">
				<form method="post" name="form_filtro_sorteadas" id="form_filtro_sorteadas" class="form form-horizontal">
					<input type="hidden" name="adpid" 			value="<?php echo $adpid;?>" />
					<input type="hidden" name="inuid" 			value="<?php echo $inuid; ?>" />
					<input type="hidden" name="situacao" 		value="1" />
					<input type="hidden" name="somenteLeitura" 	value="true" />
			<?php 
			     
    			$comboUFs = "SELECT estuf as codigo, estuf as descricao FROM par3.instrumentounidade WHERE inuid = $inuid";
    			echo $simec->select ( 'estuf', 'UF', $_REQUEST['estuf'], $comboUFs, array());
    			
    			$comboMun = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio 
                             WHERE estuf = (SELECT estuf FROM par3.instrumentounidade WHERE inuid = $inuid)";
    			echo $simec->select ( 'muncod', 'Município', $_REQUEST['estuf'], $comboMun, array());
    			
    			$comboDep = array('E' => 'Estadual');
    			echo $simec->select ( 'depcod', 'Dependência Administrativa', 'E', $comboDep, array('readonly'));
    			
    			echo $simec->input('codinep', 'Código INEP', $_REQUEST['codinep'], $attribs, $conf);
    			
    			echo $simec->input('nes_descricao', 'Nome da Escola', $_REQUEST['nes_descricao'], $attribs, $conf);
    			
    			$comboTip = "SELECT DISTINCT nes_tipo_sorteio_cod as codigo, nes_tipo_sorteio as descricao FROM par3.nem_escolas ORDER BY 2";
    			echo $simec->select ( 'nes_tipo_sorteio_cod', 'Tipo da Escola Sorteada', $_REQUEST['nes_tipo_sorteio_cod'], $comboTip, array());
			
    		?>
    				<center>
    	                <button type="button" class="btn btn-success filtra" tipo="sorteadas" >
    	                    <i class="fa fa-search"></i> Filtrar Pesquisa
    	                </button>
    				</center>
    			</form>
    			<div id="div_lista_sorteadas">
    		<?php 
			    $dados = array(
			        'adpid'          => $adpid,
			        'situacao'       => 1,
			        'somenteLeitura' => true,
			        'inuid'          => $inuid
			    );
			    $controllerNovoEnsinoMedio->listaEscolas($dados); 
		    ?>
    			</div>
			</div>
    		<div class="ibox-footer">
    			<div class="form-actions col-md-offset-2">
    				<button type="button" class="btn btn-white" data-dismiss="modal">Fechar</button>
    			</div>
    		</div>
		</div>
	</div>
</div>
<div class="ibox float-e-margins animated modal" id="dv_escolas_aptas" tabindex="-1" role="dialog" aria-hidden="true">
	<div class="modal-dialog modal-lg">
		<div class="modal-content">
			<div class="ibox-title" id="title_carga">
				<h3>Escolas aptas  Elegíveis pela rede</h3>
			</div>
			<div class="ibox-content" id="title_carga">
				<form method="post" name="form_filtro_aptas" id="form_filtro_aptas" class="form form-horizontal">
					<input type="hidden" name="adpid" 			value="<?php echo $adpid;?>" />
					<input type="hidden" name="inuid" 			value="<?php echo $inuid; ?>" />
					<input type="hidden" name="situacao" 		value="2" />
					<input type="hidden" name="somenteLeitura" 	value="false" />
			<?php 
			     
    			$comboUFs = "SELECT estuf as codigo, estuf as descricao FROM par3.instrumentounidade WHERE inuid = $inuid";
    			echo $simec->select ( 'estuf', 'UF', $_REQUEST['estuf'], $comboUFs, array());
    			
    			$comboMun = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio 
                             WHERE estuf = (SELECT estuf FROM par3.instrumentounidade WHERE inuid = $inuid)";
    			echo $simec->select ( 'muncod', 'Município', $_REQUEST['estuf'], $comboMun, array());
    			
    			$comboDep = array('E' => 'Estadual');
    			echo $simec->select ( 'depcod', 'Dependência Administrativa', 'E', $comboDep, array('readonly'));
    			
    			echo $simec->input('codinep', 'Código INEP', $_REQUEST['codinep'], $attribs, $conf);
    			
    			echo $simec->input('nes_descricao', 'Nome da Escola', $_REQUEST['nes_descricao'], $attribs, $conf);
    			
    			$comboTip = "SELECT netid as codigo, net_descricao as descricao FROM par3.nem_escolas_tipo ORDER BY 2";
    			echo $simec->select ( 'netid', 'Tipo da Escola', $_REQUEST['netid'], $comboTip, array());
    			
    			$comboSel = array('S' => 'Sim', 'N' => 'Não');
    			echo $simec->select ( 'selecionadas', 'Selecionadas', $_REQUEST['selecionadas'], $comboSel, array());
			
    		?>
    				<center>
    	                <button type="button" class="btn btn-success filtra" tipo="aptas" >
    	                    <i class="fa fa-search"></i> Filtrar Pesquisa
    	                </button>
    				</center>
    			</form>
    			<?php if($esdid == WF_ESDID_EMCADASTRAMENTO_PNEM){?>
    			<div class="row" >
    				<div class="col-md-12" >
    					<center>
        					<label style="color: red;">
        					Ao marcar/desmarcar as escolas, elas já serão vinculadas/desvinculadas automaticamente.
        					</label>
    					</center>
    				</div>
    			</div>
    			<?php } ?>
    			<div id="div_lista_aptas">
			<?php 
    			$dados['situacao']       = 2;
    			$dados['somenteLeitura'] = $esdid != WF_ESDID_EMCADASTRAMENTO_PNEM;
			    $controllerNovoEnsinoMedio->listaEscolas($dados); 
            ?>
            	</div>
			</div>
    		<div class="ibox-footer">
    			<div class="form-actions col-md-offset-2">
    				<button type="button" class="btn btn-white" data-dismiss="modal">Fechar</button>
    			</div>
    		</div>
		</div>
	</div>
</div>
<div class="ibox float-e-margins animated modal" id="dv_escolas_nao_aptas" tabindex="-1" role="dialog" aria-hidden="true">
	<div class="modal-dialog modal-lg">
		<div class="modal-content">
			<div class="ibox-title" id="title_carga">
				<h3>Lista de escolas não aptas</h3>
			</div>
			<div class="ibox-content" id="title_carga">
				<form method="post" name="form_filtro_naptas" id="form_filtro_naptas" class="form form-horizontal">
					<input type="hidden" name="adpid" 			value="<?php echo $adpid;?>" />
					<input type="hidden" name="inuid" 			value="<?php echo $inuid; ?>" />
					<input type="hidden" name="situacao" 		value="3" />
					<input type="hidden" name="somenteLeitura" 	value="true" />
			<?php 
			     
    			$comboUFs = "SELECT estuf as codigo, estuf as descricao FROM par3.instrumentounidade WHERE inuid = $inuid";
    			echo $simec->select ( 'estuf', 'UF', $_REQUEST['estuf'], $comboUFs, array());
    			
    			$comboMun = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio 
                             WHERE estuf = (SELECT estuf FROM par3.instrumentounidade WHERE inuid = $inuid)";
    			echo $simec->select ( 'muncod', 'Município', $_REQUEST['estuf'], $comboMun, array());
    			
    			$comboDep = array('E' => 'Estadual');
    			echo $simec->select ( 'depcod', 'Dependência Administrativa', 'E', $comboDep, array('readonly'));
    			
    			echo $simec->input('codinep', 'Código INEP', $_REQUEST['codinep'], $attribs, $conf);
    			
    			echo $simec->input('nes_descricao', 'Nome da Escola', $_REQUEST['nes_descricao'], $attribs, $conf);
			
    		?>
    				<center>
    	                <button type="button" class="btn btn-success filtra" tipo="naptas" >
    	                    <i class="fa fa-search"></i> Filtrar Pesquisa
    	                </button>
    				</center>
    			</form>
    			<div id="div_lista_naptas">
			<?php 
    			$dados['situacao'] = 3;
    			$dados['somenteLeitura'] = true;
                $controllerNovoEnsinoMedio->listaEscolas($dados); 
            ?>
            	</div>
			</div>
    		<div class="ibox-footer">
    			<div class="form-actions col-md-offset-2">
    				<button type="button" class="btn btn-white" data-dismiss="modal">Fechar</button>
    			</div>
    		</div>
		</div>
	</div>
</div>