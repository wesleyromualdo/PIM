<?php 
	$respostas0 = array('A' => 'Sim', 'I' => 'Não');
	$respostas1 = array('1' => 'A ser elaborada', '2' => 'Em elaboração','3' =>'Já no legislativo');
	$respostas2 = array('1' => '2 anos', '2' => '3 anos','3' =>'4 anos', '4' => 'Outro', '5' => 'Não se aplica');
	$respostas3 = array('1' => '2016', '2' => '2017','3' =>'2018','4' =>'2019','5' =>'Outro','6' =>'Não se aplica');
?>
<table>
	<thead>
		<tr>
			<th align="center" style="width: 40px;">UF</th>
			<th align="left"   style="width: 240px;">Municipio</th>
			<th align="left"   style="width: 140px;">Situação</th>
			<th align="left"   style="width: 680px;">Já possui lei disciplinando a gestão democrática da educação pública conforme art. 9º do PNE?</th>
			<th align="left"   style="width: 230px;">Em que fase se encontra?</th>
			<th align="left"   style="width: 480px;">Qual o período de mandato de direção escolar disciplinado pela rede?</th>
			<th align="left"   style="width: 260px;">Período de mandato de direção escolar</th>
			<th align="left"   style="width: 310px;">Quando haverá a próxima seleção de diretores?</th>
			<th align="left"   style="width: 240px;">Ano para seleção de diretores</th>
		</tr>
	</thead>
	<tbody>
		<?php foreach ($lista as $data): ?>
			<tr>
				<td align="center"><?php echo $data['estuf']; ?></td>
				<td align="left"><?php echo $data['inudescricao']; ?></td>
				<td align="left"><?php echo $data['situacao']; ?></td>
				<td align="left"><?php echo $respostas0[$data['pppresp0']]; ?></td>
				<td align="left"><?php echo $respostas1[$data['pppresp1']]; ?></td>
				<td align="left"><?php echo $respostas2[$data['pppresp2']]; ?></td>
				<td align="left"><?php echo $data['pppresp2outros']; ?></td>
				<td align="left"><?php echo $respostas3[$data['pppresp3']]; ?></td>
				<td align="left"><?php echo $data['pppresp3outros']; ?></td>
			</tr>
		<?php endforeach; ?>
	</tbody>
</table>