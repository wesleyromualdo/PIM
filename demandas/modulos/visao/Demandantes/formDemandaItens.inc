<?php
global $simec, $arDado;


$attribs['maxlength']   = 7;
$attribs['size']        = 10;
$attribs['class']       = 'inteiro vlrquantidade';
$attribs['type']        = 'number';

?>				
<div class="panel panel-default">
	<div class="panel-heading offset-lg-5"><h4>Vincular Itens ao Cardápio</h4></div>
    <div class="panel-body">
    	<div class="row">
    		<div class="offset-lg-3 col-lg-4">
                <select name="cidid" id="cidid" class="chosen-select form-control" data-placeholder="Selecione Item">
                    <option></option>
                    <?php foreach ($arDado['arrItensCardapio'] as $v):?>
                        <option value="<?php echo $v['codigo'] ?>" <?= $selected ?> > <?php echo $v['descricao']?> </option>
                    <?php endforeach; ?>
                </select>
			</div>
			<div class="col-lg-2">
				<button class="btn btn-success" type="button" id="btn-incluir-itens-demanda" onclick="incluirItensDemanda()">Adicionar <i class="fa fa-plus"></i></button>
			</div>
		</div>
		<br>
		<div class="row">
    		<div class="col-lg-12 table-responsive">
    			<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem" id="dados-itens-composicao" data-qtd-acoes="4">
				<thead>
					<tr>
						<th style="vertical-align: middle;">Item</th>
						<th style="vertical-align: middle;">Valor Referência(R$)</th>
						<th style="vertical-align: middle;">Quantidade</th>
						<th style="vertical-align: middle;">Total(R$)</th>
					</tr>
				</thead>
				<tbody id="dados-itens-demanda-tbody">
<?php               foreach ($arDado['arrItensDemanda'] as $v) {
                        $attribs['onchange'] = 'calculaTotalItens('.$v['dciid'].')';
                        $totalItemGeral += ($v['valor_referencia'] * $v['deiquantidade']);
                        ?>
					<tr>
						<td width="50%">
							<input type="hidden" name="valor_referencia[<?php echo $v['dciid'];?>]" value="<?php echo $v['valor_referencia']; ?>">
							<input type="hidden" name="dciid[]" value="<?php echo $v['dciid']; ?>">
							<input type="hidden" name="deiid[<?php echo $v['dciid']; ?>]" value="<?php echo $v['deiid']; ?>">	
													
							<?php echo $v['ciddsc']; ?></td>
						<td width="20%"><?php echo formata_valor($v['valor_referencia']); ?></td>
						<td width="10%">
						<input name="deiquantidade[<?php echo $v['dciid'];?>]" id="deiquantidade[<?php echo $v['dciid'];?>]" type="number" value="<?php echo $v['deiquantidade'];?>" class="inteiro vlrquantidade" style="width: 50px; text-align: center;" maxlength="7" size="4" onchange="calculaTotalItens(<?php echo $v['dciid'];?>)">
						
						<?php //echo $simec->input('deiquantidade['.$v['dciid'].']', null, $v['deiquantidade'], $attribs, array('input-size' => '12')); ?></td>
						<td width="20%"><b><div id="valor_total_<?php echo $v['dciid']; ?>"><?php echo formata_valor($v['valor_referencia'] * $v['deiquantidade'])?></div></b></td>
					</tr>
<?php               }?>
				</tbody>
				<tfoot>
					<tr>
						<td colspan="3" style="text-align: right;"><b>Totais:</b></td>
						<td><b><div id="div_valor_geral"><?php echo formata_valor($totalItemGeral); ?></div></b></td>
					</tr>
				</tfoot>
				</table>
    		</div>
    	</div>
    </div>
</div>