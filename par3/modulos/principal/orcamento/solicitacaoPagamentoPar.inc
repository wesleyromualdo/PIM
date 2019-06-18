<form method="post" name="fmconfirmarpagamento" id="fmconfirmarpagamento" class="form form-horizontal">
<div class="ibox-content">

	<div class="ibox-title">
	<h5><i class="fa fa-angle-double-right"></i> Solicitação de Pagamento</h5>
	<button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
	</div>

	<? 
	
	$sql = "SELECT 
				i.estuf as uf,
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
	
	<? 
	$sql = "SELECT
				   e.empid,
				   COALESCE(e.empnumero,'-') as numeroempenho,
				   e.empsituacao as situacaoempenho,
				   '<small>'||u.usunome||' - '||to_char(e.empdata,'dd/mm/YYYY HH24:MI')||'</small>' as usuario,
				   formata_cpf_cnpj(e.empcnpj) as cnpj,
				   COALESCE(e.empprotocolo,'-') as numeroprotocolo,
				   e.empvalorempenho as valorempenho
				FROM par3.empenho e 
				LEFT JOIN seguranca.usuario u ON u.usucpf = e.usucpf 
				WHERE e.empnumeroprocesso='".$dadosprocesso['pronumeroprocesso']."'";
	
	
	
	$empenhos = $db->carregar($sql);
	
	?>	
	
	<? if($empenhos[0]) : ?> 
	<div class="panel panel-info">
    	<div class="panel-heading">Lista de Empenho(s) do Processo</div>
        
        <div class="panel-body">
        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive">
        <thead>
        	<tr>
        		<th>&nbsp;</th>
        		<th>Nota do Empenho</th>
        		<th>Situação do Empenho</th>
        		<th>Usuário</th>
        		<th>CNPJ</th>
        		<th>Número do Protocolo</th>
        		<th>Valor do Empenho</th>
        	</tr>
        </thead>
        <tbody>
        
		<? 
		
		foreach($empenhos as $emp) {
			echo '<tr>';
			echo '<td><span class="btn btn-primary" id="btn_empenhosel_'.$emp['empid'].'" style="cursor:pointer;" onclick="listaComposicaoEmpenho('.$emp['empid'].');"><i class="fa fa-dollar" aria-hidden="true"></i></span></td>';
			echo '<td>'.$emp['numeroempenho'].'</td>';
			echo '<td>'.$emp['situacaoempenho'].'</td>';
			echo '<td>'.$emp['usuario'].'</td>';
			echo '<td>'.$emp['cnpj'].'</td>';
			echo '<td>'.$emp['numeroprotocolo'].'</td>';
			echo '<td>'.number_format($emp['valorempenho'],2,",",".").'</td>';
			echo '</tr>';
		}
		
		?>
		</tbody>
        </table>

        </div>
	</div>
	<? endif; ?>
	
	<div id="dvComposicaoEmpenho"></div>
	
	
	<div id="dvResult"></div>
	
	<div class="ibox-footer">
		<div class="form-actions col-md-offset-5">
			<button type="button" class="btn btn-white" data-dismiss="modal">Fechar</button>
		</div>
	</div>
	

</div>


</form>


<div class="ibox float-e-margins animated modal" id="dvvisualizarxmlsigef" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
	<div class="modal-dialog modal-lg">
		<div class="modal-content">
			<div class="ibox-title"><h5><i class="fa fa-angle-double-right"></i> Visualizar XML SIGEF</h5></div>
			<div class="ibox-content" id="dvdadosvisualizarxmlsigef"></div>
			<div class="ibox-footer">
				<div class="form-actions col-md-offset-2">
					<button type="button" class="btn btn-white" onclick="$('#dvvisualizarxmlsigef').modal('toggle');$('#dvconfirmarempenho').modal('show');">Fechar</button>
				</div>
			</div>
		</div>
	</div>
</div>


<div class="ibox float-e-margins animated modal" id="dvcomposicaoempenho" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
	<div class="modal-dialog modal-lg">
		<div class="modal-content">
			<div class="ibox-title"><h5><i class="fa fa-angle-double-right"></i> Composição do Empenho</h5></div>
			<div class="ibox-content" id="dvdadoscomposicaoempenho"></div>
			<div class="ibox-footer">
				<div class="form-actions col-md-offset-2">
					<button type="button" class="btn btn-white" onclick="$('#dvcomposicaoempenho').modal('toggle');$('#modal-form-large').modal('show');">Fechar</button>
				</div>
			</div>
		</div>
	</div>
</div>


<div class="ibox float-e-margins animated modal" id="dvcomposicaoiniciativa" tabindex="-1" role="dialog" aria-hidden="true" data-backdrop="static" data-keyboard="false">
	<div class="modal-dialog modal-lg">
		<div class="modal-content">
			<div class="ibox-title"><h5><i class="fa fa-angle-double-right"></i> Composição da Iniciativa Planejamento</h5></div>
			<div class="ibox-content" id="dvdadoscomposicaoiniciativa"></div>
			<div class="ibox-footer">
				<div class="form-actions col-md-offset-2">
					<button type="button" class="btn btn-white" onclick="$('#dvcomposicaoiniciativa').modal('toggle');$('#modal-form-large').modal('show');">Fechar</button>
				</div>
			</div>
		</div>
	</div>
</div>