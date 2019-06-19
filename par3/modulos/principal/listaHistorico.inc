<div class="ibox-content">
	<table class="table table-hover dataTable">
		<thead>
			<tr>
				<th colspan=2 ><center>Responsável pela alteração</center></th>
				<th style="background-color: #e7eaec; width: 5px !important; padding: 0px !important;"></th>
				<th colspan=4 ><center>Dados alterados</center></th>
			</tr>
			<tr>
				<th><?php echo $labelId; ?></th>
				<th>Nome</th>
				<th  class="unsorted" style="background-color: #e7eaec; width: 5px !important; padding: 0px !important;" ></th>
				<th><?php echo $labelId; ?></th>
				<th>Nome</th>
				<th>E-mail</th>
				<th>Início Mandato</th>
				<th>Fim Mandato</th>
				<th>Data Inativação</th>
			</tr>
		</thead>
		<?php foreach ($arrHistorico as $historico) : ?>
		<tr>
			<td><?php echo formatar_cpf($historico['usucpf']); ?></td>
			<td><?php echo $historico['usunome']; ?></td>
			<td style="background-color: #e7eaec; width: 5px !important; padding: 0px !important;"></td>
			<td><?php echo formatar_cpf($historico['id']); ?></td>
			<td><?php echo $historico['entnome']; ?></td>
			<td><?php echo $historico['entemail']; ?></td>
			<td><?php echo formata_data($historico['entdt_inicio_mandato']); ?></td>
			<td><?php echo formata_data($historico['entdt_fim_mandato']); ?></td>
			<td><?php echo formata_data($historico['entdtinativacao']); ?></td>
		</tr>
		<?php endforeach;?>
	</table>
</div>