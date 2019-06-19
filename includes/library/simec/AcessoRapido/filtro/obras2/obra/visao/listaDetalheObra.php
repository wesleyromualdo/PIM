<?php 
if (count($dados['sqlListaDetalheObra']) > 0) {
?>
    <table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem ">
    	<thead>
    		<tr>
    		<?php 
    		foreach ($dados['cabecalho'] as $k => $v) {
    		?>
    			<td align="center" valign="top" class="title" 
    				style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; font-weight: bold;" 
    				onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor="">
    				<?php echo $v; ?>
    			</td>
    		<?php 
    		}
    		?>	
    		</tr>
    	</thead>
    	<tbody>
    		<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
    		<?php 
    		foreach($dados['sqlListaDetalheObra'] as $k => $v) {
    		?>
    			<td align="right" style="color: #0066cc;"><?php echo $v['obrid']; ?></td>
    			<td align="right" style="color: #0066cc;"><?php echo $v['preid']; ?></td>
    			<td><?php echo $v['processo']; ?></td>
    			<td><?php echo $v['n_termo_ano']; ?></td>
    			<td><?php echo $v['obrnome']; ?></td>
    			<td><?php echo $v['entnome']; ?></td>
    			<td><?php echo $v['municipio_uf']; ?></td>
    			<td align="center" ><?php echo $v['inicio']; ?></td>
    			<td><?php echo AcessoRapido\filtro\obras2\obra\controle\Obra::tratarSituacaoObra($v['situacao'], $v); ?></td>
    			<td align="center" ><?php echo AcessoRapido\filtro\obras2\obra\controle\Obra::tratarDataUltimaVistoria($v['dtvistoria'], $v); ?></td>
    			<td align="right" style="color: #0066cc;"><?php echo $v['obrpercentultvistoria']; ?></td>
    			<td><?php echo $v['tpodsc']; ?></td>
    		<?php 
    		}
    		?>	
    		</tr>
    	</tbody>
    </table>
<?php
} else {
?>    
    <table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem ">
    	<tr>
    		<td align="center" style="color:#cc0000; padding: 10px">Nenhuma obra encontrada com este ID.</td>
    	</tr>
    </table>
<?php    
}
