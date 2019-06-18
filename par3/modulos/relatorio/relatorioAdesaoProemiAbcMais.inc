<?php
/**
 *
 * @category Formulário
 * @package  
 * @date 25 de agosto de 2016
 * @author   Elias Lourenco
 * @license  GNU simec.mec.gov.br
 * @version  SVN:
 * @link     no link
 */

$adesaoPrograma	= new Par3_Model_AdesaoPrograma();

switch ($_REQUEST['requisicao']) {
	case 'exportar':
		$lista = $adesaoPrograma->listaAdesoesProemiAbcMais($_REQUEST['ano']);
		header("Content-Type: application/vnd.ms-excel; charset=utf-8");
		$dataAtual = date('d-m-Y_G-i-s');
		header("Content-Disposition: attachment; filename=relatorioAdesoesProemiNovoMaisEducacao_{$dataAtual}.xls");
		header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
		header("Cache-Control: private",false);
		if( is_array($lista) )
		{
		?>
			<table border="1px">
				<thead>
					<tr>
						<th align="left" style="/*width: 40px;*/">Identificação do Programa</th>
						<th align="left"   style="/*width: 240px;*/">UF</th>
						<th align="left"   style="/*width: 140px;*/">Código Município</th>
						<th align="left"   style="/*width: 140px;*/">Município</th>
						<th align="left"   style="/*width: 680px;*/">Esfera</th>
						<th align="left"   style="/*width: 230px;*/">Situação do termo de adesão</th>
						<th align="left"   style="/*width: 480px;*/">Situação do processo de adesão</th>
						<th align="left"   style="/*width: 260px;*/">Total de escolas carga horária 5 horas</th>
						<th align="left"   style="/*width: 260px;*/">Total de escolas carga horária 7 horas </th>
						<th align="left"   style="/*width: 260px;*/">Total de escolas carga horária 15 horas </th>
						<th align="left"   style="/*width: 260px;*/">Total de escolas sem carga horária</th>
						<th align="left"   style="/*width: 260px;*/">Total de escolas selecionadas</th>
					</tr>
				</thead>
				<tbody>
					<?php foreach ($lista as $data){ ?>
						<tr>
							<td align="center"><?php echo $data['programa']; ?></td>
							<td align="left"><?php echo $data['estuf']; ?></td>
							<td align="left"><?php echo $data['cod_municipio']; ?></td>
							<td align="left"><?php echo $data['municipio']; ?></td>
							<td align="left"><?php echo $data['esfera']; ?></td>
							<td align="left"><?php echo $data['situacao_aceitacao_termo']; ?></td>
							<td align="left"><?php echo $data['situacao_adesao']; ?></td>
							<td align="left"><?php echo $data['total_5']; ?></td>
							<td align="left"><?php echo $data['total_7']; ?></td>
							<td align="left"><?php echo $data['total_15']; ?></td>
							<td align="left"><?php echo $data['total_sem']; ?></td>
							<td align="left"><?php echo $data['total_escolas']; ?></td>
						</tr>
					<?php } ?>
				</tbody>
			</table>
		
		<?php
		}
		die;
		break;
}
require APPRAIZ . 'includes/cabecalho.inc';
?>
<form method="post" name="formulario" id="formulario" class="form form-horizontal">
    <div class="ibox">
    	<div class="ibox-title">
            <div class="row" >
            	<div class="col-md-10" >
        	       <h3 id="entidade">Relatório de adesões aos programas PROEMI e Novo Mais Educação</h3>
                </div>
    		</div>
    	</div>

    	<div class="ibox-footer">
			<div class="row">
				<div class="col-lg-6" align="center">
					 <button type="button" class="btn btn-primary export" ano=2016 ><i class="fa fa-list"></i>Gerar Planilha 2016</button>
				</div>
				<div class="col-lg-6" align="center">
					 <button type="button" class="btn btn-primary export" ano=2017 ><i class="fa fa-list"></i>Gerar Planilha 2017</button>
				</div>
			</div>
    	</div>
    </div>
</form>


<script>
jQuery(document).ready(function() {
	jQuery('.export').click(function() {
		var ano = $(this).attr('ano');
		window.open(window.location.href + "&requisicao=exportar&ano="+ano);
	});
});

</script>