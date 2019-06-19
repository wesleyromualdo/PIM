<?php
  	$instrumentoUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
	$indicador = $controleIndicadorINEP->carregarIndicadores(array('estuf' => $instrumentoUnidade->estuf, 'muncod' => $instrumentoUnidade->muncod, 'itrid' => $instrumentoUnidade->itrid));
	$dscEsfera = $instrumentoUnidade->itrid == 1 ? 'Estadual' : ($instrumentoUnidade->itrid == 2 ? 'Municipal' : 'Distrital');
?>
<div class="ibox float-e-margins">
	<div class="ibox-content">
		<div class="ibox float-e-margins">
			<div class="panel panel-success">
				<div class="panel-heading">
					<h3>Docentes da Rede <?php echo $dscEsfera; ?> de Educação que Atuam na Educação Infantil</h3>
				</div>
				<div class="panel-body">
					<div class="table-responsive">
						<table class="table table-bordered">
							<thead>
								<tr>
									<td colspan="10" class="text-center" style="background-color: #FFF; padding: 2px;">
										<h4>Docentes Quanto à Adequação da Formação <small>(Fonte INEP: Educacenso 2014)</small></h4>
									</td>
								</tr>
								<tr>
									<th colspan="2" width="">Grupo 1*</th>
									<th colspan="2" width="">Grupo 2*</th>
									<th colspan="2" width="">Grupo 3*</th>
									<th colspan="2" width="">Grupo 4*</th>
									<th colspan="2" width="">Grupo 5*</th>
								</tr>
							</thead>
							<tbody>
								<tr>
									<td>
										Docentes com formação superior de licenciatura na mesma disciplina que
										lecionam, ou bacharelado na mesma disciplina com curso de complementação
										pedagógica concluído
									</td>
									<td>%</td>

									<td>
										Docentes com formação superior de bacharelado na disciplina correspondente,
										mas sem licenciatura ou complementação pedagógica
									</td>
									<td>%</td>

									<td>
										Docentes com licenciatura em área diferente daquela que leciona, ou com
										bacharelado nas disciplinas da base curricular comum e complementação
										pedagógica concluída em área diferente daquela que leciona
									</td>
									<td>%</td>

									<td>
										Docentes com outra formação superior não considerada nas categorias anteriores
									</td>
									<td>%</td>

									<td>
										Docentes que não possuem curso superior completo
									</td>
									<td>%</td>
								</tr>
							</tbody>
							<tfoot>
								<tr>
									<th class="text-center"><?php echo $indicador['inieducacaoinfantilgrupo1valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inieducacaoinfantilgrupo1porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inieducacaoinfantilgrupo2valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inieducacaoinfantilgrupo2porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inieducacaoinfantilgrupo3valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inieducacaoinfantilgrupo3porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inieducacaoinfantilgrupo4valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inieducacaoinfantilgrupo4porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inieducacaoinfantilgrupo5valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inieducacaoinfantilgrupo5porcent']; ?>%</th>
								</tr>
								<tr>
									<td colspan="10" class="text-center">
										<sma3ll>*Categorias de adequação da formação dos docentes em relação à disciplina que lecionam - Nota Técnica INEP nº 020/2014</sma3ll>
									</td>
								</tr>
							</tfoot>
						</table>
					</div>
				</div>
			</div>
		</div>

		<div class="ibox float-e-margins">
			<div class="panel panel-success">
				<div class="panel-heading">
					<h3>Docentes da Rede  <?php echo $dscEsfera; ?> de Educação que Atuam nos Anos Iniciais do Ensino Fundamental </h3>
				</div>
				<div class="panel-body">
					<div class="table-responsive">
						<table class="table table-bordered">
							<thead>
								<tr>
									<td colspan="10" class="text-center" style="background-color: #FFF; padding: 2px;">
										<h4>Docentes Quanto à Adequação da Formação <small>(Fonte INEP: Educacenso 2014)</small></h4>
									</td>
								</tr>
								<tr>
									<th colspan="2" width="20%">Grupo 1*</th>
									<th colspan="2" width="20%">Grupo 2*</th>
									<th colspan="2" width="20%">Grupo 3*</th>
									<th colspan="2" width="20%">Grupo 4*</th>
									<th colspan="2" width="20%">Grupo 5*</th>
								</tr>
							</thead>
							<tbody>
								<tr>
									<td>
										Docentes com formação superior de licenciatura na mesma disciplina que
										lecionam, ou bacharelado na mesma disciplina com curso de complementação
										pedagógica concluído
									</td>
									<td>%</td>

									<td>
										Docentes com formação superior de bacharelado na disciplina correspondente,
										mas sem licenciatura ou complementação pedagógica
									</td>
									<td>%</td>

									<td>
										Docentes com licenciatura em área diferente daquela que leciona, ou com
										bacharelado nas disciplinas da base curricular comum e complementação
										pedagógica concluída em área diferente daquela que leciona
									</td>
									<td>%</td>

									<td>
										Docentes com outra formação superior não considerada nas categorias anteriores
									</td>
									<td>%</td>

									<td>
										Docentes que não possuem curso superior completo
									</td>
									<td>%</td>
								</tr>
							</tbody>
							<tfoot>
								<tr>
									<th class="text-center"><?php echo $indicador['inifundamentalinicialgrupo1valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inifundamentalinicialgrupo1porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inifundamentalinicialgrupo2valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inifundamentalinicialgrupo2porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inifundamentalinicialgrupo3valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inifundamentalinicialgrupo3porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inifundamentalinicialgrupo4valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inifundamentalinicialgrupo4porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inifundamentalinicialgrupo5valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inifundamentalinicialgrupo5porcent']; ?>%</th>
								</tr>
								<tr>
									<td colspan="10" class="text-center">
										<small>*Categorias de adequação da formação dos docentes em relação à disciplina que lecionam - Nota Técnica INEP nº 020/2014</small>
									</td>
								</tr>
							</tfoot>
						</table>
					</div>
				</div>
			</div>
		</div>


		<div class="ibox float-e-margins">
			<div class="panel panel-success">
				<div class="panel-heading">
					<h3>Docentes da Rede <?php echo $dscEsfera; ?> de Educação que Atuam nos Anos Finais do Ensino Fundamental</h3>
				</div>
				<div class="panel-body">
					<div class="table-responsive">
						<table class="table table-bordered">
							<thead>
								<tr>
									<td colspan="10" class="text-center" style="background-color: #FFF; padding: 2px;">
										<h4>Docentes Quanto à Adequação da Formação <small>(Fonte INEP: Educacenso 2014)</small></h4>
									</td>
								</tr>
								<tr>
									<th colspan="2" width="20%">Grupo 1*</th>
									<th colspan="2" width="20%">Grupo 2*</th>
									<th colspan="2" width="20%">Grupo 3*</th>
									<th colspan="2" width="20%">Grupo 4*</th>
									<th colspan="2" width="20%">Grupo 5*</th>
								</tr>
							</thead>
							<tbody>
								<tr>
									<td>
										Docentes com formação superior de licenciatura na mesma disciplina que
										lecionam, ou bacharelado na mesma disciplina com curso de complementação
										pedagógica concluído
									</td>
									<td>%</td>

									<td>
										Docentes com formação superior de bacharelado na disciplina correspondente,
										mas sem licenciatura ou complementação pedagógica
									</td>
									<td>%</td>

									<td>
										Docentes com licenciatura em área diferente daquela que leciona, ou com
										bacharelado nas disciplinas da base curricular comum e complementação
										pedagógica concluída em área diferente daquela que leciona
									</td>
									<td>%</td>

									<td>
										Docentes com outra formação superior não considerada nas categorias anteriores
									</td>
									<td>%</td>

									<td>
										Docentes que não possuem curso superior completo
									</td>
									<td>%</td>
								</tr>
							</tbody>
							<tfoot>
								<tr>
									<th class="text-center"><?php echo $indicador['inifundamentalfinalgrupo1valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inifundamentalfinalgrupo1porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inifundamentalfinalgrupo2valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inifundamentalfinalgrupo2porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inifundamentalfinalgrupo3valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inifundamentalfinalgrupo3porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inifundamentalfinalgrupo4valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inifundamentalfinalgrupo4porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inifundamentalfinalgrupo5valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inifundamentalfinalgrupo5porcent']; ?>%</th>
								</tr>
								<tr>
									<td colspan="10" class="text-center">
										<small>*Categorias de adequação da formação dos docentes em relação à disciplina que lecionam - Nota Técnica INEP nº 020/2014</small>
									</td>
								</tr>
							</tfoot>
						</table>
					</div>
				</div>
			</div>
		</div>


		<div class="ibox float-e-margins">
			<div class="panel panel-success">
				<div class="panel-heading">
					<h3>Docentes da Rede <?php echo $dscEsfera; ?> de Educação que Atuam no Ensino Médio</h3>
				</div>
				<div class="panel-body">
					<div class="table-responsive">
						<table class="table table-bordered">
							<thead>
								<tr>
									<td colspan="10" class="text-center" style="background-color: #FFF; padding: 2px;">
										<h4>Docentes Quanto à Adequação da Formação <small>(Fonte INEP: Educacenso 2014)</small></h4>
									</td>
								</tr>
								<tr>
									<th colspan="2" width="20%">Grupo 1*</th>
									<th colspan="2" width="20%">Grupo 2*</th>
									<th colspan="2" width="20%">Grupo 3*</th>
									<th colspan="2" width="20%">Grupo 4*</th>
									<th colspan="2" width="20%">Grupo 5*</th>
								</tr>
							</thead>
							<tbody>
								<tr>
									<td>
										Docentes com formação superior de licenciatura na mesma disciplina que
										lecionam, ou bacharelado na mesma disciplina com curso de complementação
										pedagógica concluído
									</td>
									<td>%</td>

									<td>
										Docentes com formação superior de bacharelado na disciplina correspondente,
										mas sem licenciatura ou complementação pedagógica
									</td>
									<td>%</td>

									<td>
										Docentes com licenciatura em área diferente daquela que leciona, ou com
										bacharelado nas disciplinas da base curricular comum e complementação
										pedagógica concluída em área diferente daquela que leciona
									</td>
									<td>%</td>

									<td>
										Docentes com outra formação superior não considerada nas categorias anteriores
									</td>
									<td>%</td>

									<td>
										Docentes que não possuem curso superior completo
									</td>
									<td>%</td>
								</tr>
							</tbody>
							<tfoot>
								<tr>
									<th class="text-center"><?php echo $indicador['inimediogrupo1valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inimediogrupo1porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inimediogrupo2valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inimediogrupo2porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inimediogrupo3valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inimediogrupo3porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inimediogrupo4valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inimediogrupo4porcent']; ?>%</th>
									<th class="text-center"><?php echo $indicador['inimediogrupo5valor']; ?></th>
									<th class="text-center"><?php echo $indicador['inimediogrupo5porcent']; ?>%</th>
								</tr>
								<tr>
									<td colspan="10" class="text-center">
										<small>*Categorias de adequação da formação dos docentes em relação à disciplina que lecionam - Nota Técnica INEP nº 020/2014</small>
									</td>
								</tr>
							</tfoot>
						</table>
					</div>
				</div>
			</div>
		</div>
	</div>
</div>