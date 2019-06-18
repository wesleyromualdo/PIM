<?php
require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'par3/classes/helper/BalancoPar2010.class.inc';

$helperEnteFederado = new Par3_Helper_BalancoPar2010();
$arrEnteFederado 		= $helperEnteFederado->recuperarEnteFederadoElaborados();
$arrEstados 			= $helperEnteFederado->recuperarEstadosElaborados();
$valorMunBenef			= $helperEnteFederado->recuperarMunicipioBeneiciados();
$ValorEstBenef			= $helperEnteFederado->recuperarEstadosBeneiciados();
$totTermo 				= $helperEnteFederado->recuperarTermosFirmados();
$totObras 				= $helperEnteFederado->recuperarQtdObrasFinanciadas();
$valorPactuado 			= $helperEnteFederado->recuperarValorPactuado();
$valorEmpenhado 		= $helperEnteFederado->recuperarValorEmpenhado();
$valorRepassado 		= $helperEnteFederado->recuperarValorRepassado();
$arrObrasAtendidas	 	= $helperEnteFederado->recuperarObrasAtendidas();
$arrProgramasAtendidos 	= $helperEnteFederado->recuperarProgramasAtendidos();
$QtdVeiculos		 	= $helperEnteFederado->recuperarValorVeiculos();
?>
<style>
	.spanTotal{
		font-size: 24px; 
		font-weight: bold; 
		font-style: italic;
	}
	.spanQuadros{
		background-color: #2F4050; 
		color: white;
	}
	.spanAcaoesAtendidas{
		background-color: white; 
		color: black;
	}
	.spanObrasAtendidas{
		background-color: white; 
		color: black;
	}
</style>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
    	<div class="col-md-12">
	    	<div class="ibox-content spanQuadros">
	            <div class="row">
	            	<div class="col-md-12">
	                	<h2 style="font-weight: bold; text-align: center;">Balanço PAR 2011 - 2015</h2>
	            	</div>
	        	</div>
			</div>
		</div>
    </div>
    <br>
    <div class="row">
		<div class="col-md-3">
			<div class="ibox-title spanQuadros" style="height: 60px; !important">
				<div class="row">
					<div class="col-md-12"><h5>QTD de entidades que elaboraram o PAR</h5></div>
				</div>
			</div>
			<div class="row">
				<div class="col-md-12">
					<div class="ibox-content spanQuadros" style="height: 150px; !important">
						<p>Total<br><span class="spanTotal"><?php echo simec_number_format(((int)$arrEnteFederado['mun_elaborado'] + (int)$arrEstados['total_elaborado']),0); ?></span></p>
			    		<p><span style="font-weight: bold; font-style: italic;"><?php echo simec_number_format($arrEnteFederado['mun_elaborado'],0); ?></span> de <span style="font-weight: bold; font-style: italic;"><?php echo simec_number_format($arrEnteFederado['tot_muncipio'],0); ?></span> Municípios</p>
			    		<p><span style="font-weight: bold; font-style: italic;"><?php echo $arrEstados['total_elaborado']; ?></span> de <span style="font-weight: bold; font-style: italic;"><?php  echo $arrEstados['total_estado']; ?></span> Estados</p>
					</div>
				</div>
			</div>
		</div>
		<div class="col-md-3">
			<div class="ibox-title spanQuadros" style="height: 60px; !important">
				<div class="row">
					<div class="col-md-12"><h5>Estados e Municípios Beneficiados com Assitência Financeira</h5></div>
				</div>
			</div>
			<div class="row">
				<div class="col-md-12">
					<div class="ibox-content spanQuadros" style="height: 150px; !important">
						<p>Total<br><span class="spanTotal"><?php echo simec_number_format(((int)$valorMunBenef + (int)$ValorEstBenef),0); ?></span></p>
			    		<p><span style="font-weight: bold; font-style: italic;"><?php echo simec_number_format($valorMunBenef,0); ?></span> de <span style="font-weight: bold; font-style: italic;"><?php echo simec_number_format($arrEnteFederado['tot_muncipio'],0); ?></span> Municípios</p>
			    		<p><span style="font-weight: bold; font-style: italic;"><?php echo $ValorEstBenef; ?></span> de <span style="font-weight: bold; font-style: italic;"><?php echo $arrEstados['total_estado']; ?></span> Estados</p>
					</div>
				</div>
			</div>
		</div>
		<div class="col-md-3">
			<div class="ibox-title spanQuadros" style="height: 60px; !important">
				<div class="row">
					<div class="col-md-12"><h5>Instrumentos de Pactuação Celebrados</h5></div>
				</div>
			</div>
			<div class="row">
				<div class="col-md-12">
					<div class="ibox-content spanQuadros" style="height: 150px; !important">
						<p>Total<br><span class="spanTotal"><?php echo simec_number_format((int)$totTermo,0); ?></span></p>
					</div>
				</div>
			</div>
		</div>
		<div class="col-md-3">
			<div class="ibox-title spanQuadros" style="height: 60px; !important">
				<div class="row">
					<div class="col-md-12"><h5>Obras Pactuadas</h5></div>
				</div>
			</div>
			<div class="row">
				<div class="col-md-12">
					<div class="ibox-content spanQuadros" style="height: 150px; !important">
						<p>Total<br><span class="spanTotal"><?php echo simec_number_format((int)$totObras,0); ?></span></p>
					</div>
				</div>
			</div>
		</div>
	</div>
	<br>	
	<div class="row">
		<div class="col-md-4">
			<div class="row">
				<div class="col-md-12">
					<div class="ibox-title" style="background-color: #1AB394; color: white;">
						<p><span style="float:right;">Valor Pactuado</span><br><span class="spanTotal">R$ <?php echo simec_number_format($valorPactuado,2, ',', '.' ); ?></span></p>
					</div>
				</div>
			</div>
		</div>
		<div class="col-md-4">
			<div class="row">
				<div class="col-md-12">
					<div class="ibox-title" style="background-color: #23C6C8; color: white;">
						<p><span style="float:right;">Valor Empenhado</span><br><span class="spanTotal">R$ <?php echo simec_number_format($valorEmpenhado,2, ',', '.' ); ?></span></p>
					</div>
				</div>
			</div>
		</div>
		<div class="col-md-4">
			<div class="row">
				<div class="col-md-12">
					<div class="ibox-title" style="background-color: #F8AC59; color: white;">
						<p><span style="float:right;">Valor Repassado</span><br><span class="spanTotal">R$ <?php echo simec_number_format($valorRepassado,2, ',', '.' ); ?></span></p>
					</div>
				</div>
			</div>
		</div>
	</div>
	<br>
	<div class="row">
		<div class="col-md-12">
			<div class="row" >
				<div class="col-md-12">
					<div class="ibox-title ">
						<h5>Principais Ações Atendidas </h5>
					</div>
				</div>
			</div>
			<div class="row">
				<div class="col-md-12">
					<div class="ibox-content">
						<div class="row">
							<div class="col-md-3">
								<div class="ibox-title">
									<h5>Mobiliário</h5>
								</div>
								<div class="ibox-content" style="background-color: #F3F3F4">
									<p>Total<br><span class="spanTotal"><?php echo simec_number_format((int)$arrProgramasAtendidos['mobiliario'],0); ?></span></p>
									<!--  <p><span style="font-weight: bold; font-style: italic; color: #1AB394"><?php echo simec_number_format($arrProgramasAtendidos['mobiliario_termo'],2, ',', '.' ); ?></span> de Valor Pactuado</p>
									<p><span style="font-weight: bold; font-style: italic; color: #23C6C8"><?php echo simec_number_format($arrProgramasAtendidos['mobiliario_empenho'],2, ',', '.' ); ?></span> de Valor Empenhado</p>
									<p><span style="font-weight: bold; font-style: italic; color: #F8AC59"><?php echo simec_number_format($arrProgramasAtendidos['mobiliario_pago'],2, ',', '.' ); ?></span> de Valor Repassado</p>-->
								</div>
							</div>
							<div class="col-md-3">
								<div class="ibox-title">
									<h5>Equipamentos</h5>
								</div>
								<div class="ibox-content" style="background-color: #F3F3F4">
									<p>Total<br><span class="spanTotal"><?php echo simec_number_format((int)$arrProgramasAtendidos['equipamento'],0); ?></span></p>
									<!--  <p><span style="font-weight: bold; font-style: italic; color: #1AB394"><?php echo simec_number_format($arrProgramasAtendidos['equipamento_termo'],2, ',', '.' ); ?></span> de Valor Pactuado</p>
									<p><span style="font-weight: bold; font-style: italic; color: #23C6C8"><?php echo simec_number_format($arrProgramasAtendidos['equipamento_empenho'],2, ',', '.' ); ?></span> de Valor Empenhado</p>
									<p><span style="font-weight: bold; font-style: italic; color: #F8AC59"><?php echo simec_number_format($arrProgramasAtendidos['equipamento_pago'],2, ',', '.' ); ?></span> de Valor Repassado</p>-->
								</div>
							</div>
							<div class="col-md-3">
								<div class="ibox-title">
									<h5>Veiculos</h5>
								</div>
								<div class="ibox-content" style="background-color: #F3F3F4">
									<p>Total<br><span class="spanTotal"><?php echo simec_number_format((int)$QtdVeiculos,0); ?></span></p>
									<!--  <p><span style="font-weight: bold; font-style: italic; color: #1AB394"><?php echo simec_number_format($arrProgramasAtendidos['veiculos_termo'],2, ',', '.' ); ?></span> de Valor Pactuado</p>
									<p><span style="font-weight: bold; font-style: italic; color: #23C6C8"><?php echo simec_number_format($arrProgramasAtendidos['veiculos_empenho'],2, ',', '.' ); ?></span> de Valor Empenhado</p>
									<p><span style="font-weight: bold; font-style: italic; color: #F8AC59"><?php echo simec_number_format($arrProgramasAtendidos['veiculos_pago'],2, ',', '.' ); ?></span> de Valor Repassado</p>-->
								</div>
							</div>
							<div class="col-md-3">
								<div class="ibox-title">
									<h5>Tablet</h5>
								</div>
								<div class="ibox-content" style="background-color: #F3F3F4">
									<p>Total<br><span class="spanTotal"><?php echo simec_number_format((int)$arrProgramasAtendidos['tablet'],0); ?></span></p>
									<!--  <p><span style="font-weight: bold; font-style: italic; color: #1AB394"><?php echo simec_number_format($arrProgramasAtendidos['tablet_termo'],2, ',', '.' ); ?></span> de Valor Pactuado</p>
									<p><span style="font-weight: bold; font-style: italic; color: #23C6C8"><?php echo simec_number_format($arrProgramasAtendidos['tablet_empenho'],2, ',', '.' ); ?></span> de Valor Empenhado</p>
									<p><span style="font-weight: bold; font-style: italic; color: #F8AC59"><?php echo simec_number_format($arrProgramasAtendidos['tablet_pago'],2, ',', '.' ); ?></span> de Valor Repassado</p>-->
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
	<br>
	<div class="row">
		<div class="col-md-12">
			<div class="row" >
				<div class="col-md-12">
					<div class="ibox-title">
						<h5>Total de Obras Atendidas Pelo PAR</h5>
					</div>
				</div>
			</div>
			<div class="row">
				<div class="col-md-12">
					<div class="ibox-content">
						<div class="row">
							<div class="col-md-3">
								<div class="ibox-title">
									<h5>Construção de Escolas</h5>
								</div>
								<div class="ibox-content" style="background-color: #F3F3F4">
									<p>Total<br><span class="spanTotal"><?php echo simec_number_format((int)$arrObrasAtendidas['construcao_escola'],0); ?></span></p>
								</div>
							</div>
							<div class="col-md-3">
								<div class="ibox-title">
									<h5>Reforma e Apliação de Escolas</h5>
								</div>
								<div class="ibox-content" style="background-color: #F3F3F4">
									<p>Total<br><span class="spanTotal"><?php echo simec_number_format((int)$arrObrasAtendidas['reforma_apli_escola'],0); ?></span></p>
								</div>
							</div>
							<div class="col-md-3">
								<div class="ibox-title">
									<h5>Construção de Creches - Proinfância</h5>
								</div>
								<div class="ibox-content" style="background-color: #F3F3F4">
									<p>Total<br><span class="spanTotal"><?php echo simec_number_format((int)$arrObrasAtendidas['proinfancia'],0); ?></span></p>
								</div>
							</div>
							<div class="col-md-3">
								<div class="ibox-title">
									<h5>Construção de Quadras/Coberturas</h5>
								</div>
								<div class="ibox-content" style="background-color: #F3F3F4">
									<div class="row">
										<div class="col-md-6">
											<p>Total<br><span class="spanTotal"><?php echo simec_number_format(((int)$arrObrasAtendidas['quadra'] + (int)$arrObrasAtendidas['cobertura']),0); ?></span></p>
										</div>
										<div class="col-md-6">
											<p><span style="font-weight: bold; font-style: italic;"><?php echo simec_number_format((int)$arrObrasAtendidas['quadra'],0); ?></span> Quadras</p>
											<p><span style="font-weight: bold; font-style: italic;"><?php echo simec_number_format((int)$arrObrasAtendidas['cobertura'],0); ?></span> Coberturas</p>
										</div>
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>	
</div>