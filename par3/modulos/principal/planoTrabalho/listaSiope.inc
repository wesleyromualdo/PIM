<?php
$sql = "SELECT estuf, muncod FROM par3.instrumentounidade WHERE inuid = {$_REQUEST['inuid']}";
$dados = $db->pegaLinha($sql);

//instancia a classe
$oSiope = new Par3_Model_Siope();

//receitas
$receitas = $oSiope->listarReceitas($dados);
$receitas = is_array($receitas) ? $receitas : array();
//fim receitas

//despesas
$despesas = $oSiope->listarDespesas($dados);
$despesas = is_array($despesas) ? $despesas : array();
//fim despesas

//cumprimento
$cumprimento = $oSiope->listarCumprimentos($dados);
$cumprimento = is_array($cumprimento) ? $cumprimento : array();
//fim despesas

if($dados['muncod']){
    $esferaTitulo = 'Municipal';
    $esferaTituloPlural = 'Municipais';
    $esferaEntidade = 'Municipio';
}elseif($dados['estuf']=='DF'){
    $esferaTitulo = 'Distrital';
    $esferaTituloPlural = 'Distritais';
    $esferaEntidade = 'Distrito Federal';
}else{
    $esferaTitulo = 'Estadual';
    $esferaTituloPlural = 'Estaduais';
    $esferaEntidade = 'Estado';
}

?>
<style>
	#divImpressao {
		width: 100% !important;
		display: block !important;
		white-space: initial !important;
	}

	#divImpressao p {
		white-space: initial !important;
	}
</style>
<div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
	<h3>Receitas Vinculadas à Educação</h3>
	<table  cellpadding="2" cellspacing="2" width="100%" class="table table-striped table-bordered table-hover table-condensed tabela-listagem">
		<tr>
			<th align="center" style="background-color: #E0E0E0;text-align:center;">
				<b>Quanto ao Cálculo dos 25%</b>
			    <span title="ENTENDENDO AS RECEITAS DO SEU <?php echo strtoupper($esferaEntidade); ?>


Conforme o Art. 212 da Constituição Federal, os Municípios deverão aplicar 25%, no mínimo, da receita resultante de impostos, compreendida a proveniente de transferências, na Manutenção e Desenvolvimento do Ensino (MDE).

Segundo o Art. 70 da Lei Nº 9.394/96 (Lei de Diretrizes e Bases da Educação Nacional), MDE refere-se à consecução dos objetivos básicos das instituições educacionais de todos os níveis, compreendendo:

I - remuneração e aperfeiçoamento do pessoal docente e demais profissionais da educação;
II - aquisição, manutenção, construção e conservação de instalações e equipamentos necessários ao ensino;
III - uso e manutenção de bens e serviços vinculados ao ensino;
IV - levantamentos estatísticos, estudos e pesquisas visando precipuamente ao aprimoramento da qualidade e à expansão do ensino;
V - realização de atividades-meio necessárias ao funcionamento dos sistemas de ensino;
VI - concessão de bolsas de estudo a alunos de escolas públicas e privadas;
VII - amortização e custeio de operações de crédito destinadas a atender ao disposto nos incisos deste artigo;
VIII - aquisição de material didático-escolar e manutenção de programas de transporte escolar.">
					   <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
			    </span>
			</th>
			<th align="center" colspan="3" style="background-color: #B8CCE2;text-align:center">
				<b>Receitas que entram no cálculo dos 25% de aplicação mínima em Manutenção e Desenvolvimento do Ensino (MDE)</b>
			    <span title="ENTENDENDO AS RECEITAS DO SEU <?php echo strtoupper($esferaEntidade); ?>


Conforme o Art. 212 da Constituição Federal, os Municípios deverão aplicar 25%, no mínimo, da receita resultante de impostos, compreendida a proveniente de transferências, na Manutenção e Desenvolvimento do Ensino (MDE).">
					<img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
			</span>
			</th>
			<th align="center" colspan="2" style="background-color: #CADAA8;text-align:center">
				<b>Receitas que não entram no cálculo dos 25% de aplicação mínima em Manutenção e Desenvolvimento do Ensino (MDE)</b>
			</th>
			<th align="center" rowspan="2" style="background-color: #E0E0E0;text-align:center">
				<b>Total de Receitas   Vinculadas à Educação</b>
    			<br><b>(R$)</b>
			</th>
		</tr>
		<tr>
			<th align="center" style="background-color: #F5F5F5;text-align:center">
				<b>Quais são as Receitas</b>
			</th>
			<td align="center" nowrap style="background-color: #EAEAEA;text-align:center">
                            <b>Originárias do FUNDEB</b>
                            <?php if($dados['muncod']){ ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU MUNICÍPIO

       O Art. 3º da Lei Nº 11.494/2007 (Lei do FUNDEB) enumera os impostos e as transferências dos quais 20% são compulsoriamente captados e direcionados para formação do FUNDEB.
       Os recursos desse fundo serão redistribuídos aos Estados e aos seus Municípios proporcionalmente ao número de alunos das diversas etapas e modalidades da educação básica presencial, matriculados nas respectivas redes, nos respectivos âmbitos de atuação prioritária estabelecidos nos §§ 2º e 3º do art. 211 da CF.
       A consequência disso é o efeito redistributivo, que provoca acréscimos ou deduções sobre a contribuição do Município ao Fundo; esses efeitos não são considerados neste campo, visto que seu produto não entra no cálculo dos 25% de aplicação mínima em MDE.

                                ">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } else { ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU ESTADO

       O Art. 3º da Lei Nº 11.494/2007 (Lei do FUNDEB) enumera os impostos e as transferências dos quais 20% são compulsoriamente captados e direcionados para formação do FUNDEB.
       Os recursos desse fundo serão redistribuídos aos Estados e aos seus Municípios proporcionalmente ao número de alunos das diversas etapas e modalidades da educação básica presencial, matriculados nas respectivas redes, nos respectivos âmbitos de atuação prioritária estabelecidos nos §§ 2º e 3º do art. 211 da CF.
       A consequência disso é o efeito redistributivo, que provoca acréscimos ou deduções sobre a contribuição do estado ao Fundo; esses efeitos não são considerados neste campo, visto que seu produto não entra no cálculo dos 25% de aplicação mínima em MDE.

                                ">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } ?>
    			<br><b>(R$)</b>
			</td>
			<td align="center" nowrap style="background-color: #EAEAEA;text-align:center">
                            <b>Impostos <?php echo $esferaTituloPlural;?> Próprios</b>
                            <?php if($dados['muncod']){ ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU MUNICÍPIO

        Conforme o Art. 212 da Constituição Federal, os municípios aplicarão anualmente 25%, no mínimo, da receita resultante de impostos, compreendida a proveniente de transferências, na manutenção e desenvolvimento do ensino (MDE).
        Portanto, 25%, no mínimo, dos seguintes impostos municipais (aqueles que não entram na formação do Fundeb) deverão ser aplicados em MDE: ISS, ITBI, IPTU, Cota-Parte IOF-Ouro, Imposto de Renda Municipal.

                                ">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } else { ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU ESTADO

        Conforme o Art. 212 da Constituição Federal, os estados aplicarão anualmente 25%, no mínimo, da receita resultante de impostos, compreendida a proveniente de transferências, na manutenção e desenvolvimento do ensino (MDE).
        Portanto, 25%, no mínimo, dos seguintes impostos estaduais (aqueles que não entram na formação do Fundeb) deverão ser aplicados em MDE: Imposto de Renda Estadual e Cota-Parte IOF-Ouro.

                                ">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } ?>
    			<br><b>(R$)</b>
			</td>
			<td align="center" nowrap style="background-color: #EAEAEA;text-align:center">
				<b>Outras Receitas</b>
                            <?php if($dados['muncod']){ ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU <?php echo strtoupper($esferaEntidade); ?>

        Receitas de impostos e transferências computados no cálculo dos 25% de aplicação mínima em MDE, mas que não entram na formação do Fundeb, conforme no Art. 1º, I, Lei nº 11.494/2007 (Lei do FUNDEB). O <?php echo strtolower($esferaEntidade); ?>, portanto, dispõe desse recurso para aplicação em MDE, recurso esse que não integra o FUNDEB e, portanto, não está sujeito ao efeito redistributivo.">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } else { ?>
                                <span title="ENTENDENDO AS RECEITAS DO SEU ESTADO

        Receitas de impostos e transferências computados no cálculo dos 25% de aplicação mínima em MDE, mas que não entram na formação do Fundeb, conforme no Art. 1º, I, Lei nº 11.494/2007 (Lei do FUNDEB).
        O estado, portanto, dispõe desse recurso para aplicação em MDE, recurso esse que não integra o FUNDEB e, portanto, não está sujeito ao efeito redistributivo.

                                ">
                                    <img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
                                </span>
                            <?php } ?>
    			<br><b>(R$)</b>
			</td>
			<td align="center" style="background-color: #D5E2BA;text-align:center">
				<b>Salário-Educação</b>
    			<span title="ENTENDENDO AS RECEITAS DO SEU <?php echo strtoupper($esferaEntidade); ?>


        Conforme o Art. 212, § 6, da Constituição Federal, o <?php echo strtolower($esferaEntidade); ?> receberá a Cota <?php echo $esferaTitulo; ?> do salário-educação, que é distribuída proporcionalmente ao número de alunos matriculados na educação básica da sua rede pública de ensino.">
    				<img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;text-align:center">
    			</span>
    			<br><b>(R$)</b>
			</td>
			<td align="center" style="background-color: #D5E2BA;text-align:center">
				<b>Outras Receitas destinadas à Educação</b>
    			<span title="ENTENDENDO AS RECEITAS DO SEU <?php echo strtoupper($esferaEntidade); ?>


        Receitas Adicionais para Financiamento do Ensino:

        Receita da Aplicação Financeira de Outros Recursos de Impostos Vinculados ao Ensino, Transferências Diretas do Programa Dinheiro Direto na Escola (PDDE), Transferências Diretas do Programa Nacional de Alimentação Escolar (PNAE), Transferências Diretas do Programa Nacional de Apoio ao Transporte do Escolar (PNATE), Outras Transferências do FNDE, Aplicação Financeira dos Recursos do FNDE, Receitas de Transferências de Convênios, Receitas de Operação de Crédito e Outras Receitas Para o Financiamento do Ensino. ">
    					<img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
    			</span>
    			<br><b>(R$)</b>
			</td>
		</tr>
		<?php
		foreach($receitas as $r) {
			$totReceitas = $r['vl_orig_fundeb'] + $r['vl_imp_mun_proprio'] + $r['vl_outra_receita'] + $r['vl_sal_educ'] + $r['vl_outra_rec_dest_educ'];
			?>
			<tr bgcolor="<?=$cor?>">
				<td align="center">
					<?echo $r['an_receita'] ? $r['an_receita'] : '-';?>
				</td>
				<td align="center">
					<?echo $r['vl_orig_fundeb'] ? number_format($r['vl_orig_fundeb'],2,",",".") : '-';?>
				</td>
				<td align="center">
					<?
					if($dados['muncod']){
					   echo $r['vl_imp_mun_proprio'] ? number_format($r['vl_imp_mun_proprio'],2,",",".") : '-';
					}else{
					   echo $r['vl_imp_est_proprio'] ? number_format($r['vl_imp_est_proprio'],2,",",".") : '-';
					}
					?>
				</td>
				<td align="center" style="width:13%;">
					<?echo $r['vl_outra_receita'] ? number_format($r['vl_outra_receita'],2,",",".") : '-';?>
				</td>
				<td align="center">
					<?echo $r['vl_sal_educ'] ? number_format($r['vl_sal_educ'],2,",",".") : '-';?>
				</td>
				<td align="center">
					<?echo $r['vl_outra_rec_dest_educ'] ? number_format($r['vl_outra_rec_dest_educ'],2,",",".") : '-';?>
				</td>
				<td align="center">
					<b><?echo $totReceitas ? number_format($totReceitas,2,",",".") : '-';?></b>
				</td>
			</tr>
		<?php }?>
	</table>
</div>
<br>
<?php
	$arrAno = array(
		0=>"2011",
		1=>"2012",
		2=>"2013",
		3=>"2014",
		4=>"2015"
	);

	$arrTipoDespesa = array(0=>"Educação Infantil",
		1=>"(Creche + Pré-Escola)",
		2=>"Ensino Fundamental",
		3=>"Ensino Médio",
		4=>"Ensino Superior",
		5=>"Salário Educação",
	);

	//percent
	$totalDespesa = $oSiope->totalDespesa($dados);
	$totalDespesa = is_array($totalDespesa) ? $totalDespesa : array();

	$anoIni = 2011;
	$anoFim = 2015;
?>
<div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
	<h3>Despesas em Educação</h3>
	<table  cellpadding="2" cellspacing="2" width="100%" class="table table-striped table-bordered table-hover table-condensed tabela-listagem">
		<tr>
			<td align="center" style="background-color: #E0E0E0;">
				<b>Despesas</b>
			</td>
			<?php for ($ano = $anoIni; $ano <= $anoFim; $ano++) {?>
				<td align="center" style="background-color: #E0E0E0;">
					<b><?echo $ano;?><br>(R$)</b>
				</td>
				<td align="center" style="background-color: #E0E0E0;">
					<b>(%)</b>
				</td>
			<?php }?>
		</tr>
		<tr>
			<td align="center" style="background-color: #B8CCE2;">
				<div align="left" style="float: left"><img src="../imagens/mais.gif" id="imgmais_infantil" onclick="mostraTabela('infantil')"><img src="../imagens/menos.gif" id="imgmenos_infantil" style="display: none;" onclick="escondeTabela('infantil')"></div>
				<b>Educação Infantil</b>
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='EDUC INFANTIL' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="background-color: #B8CCE2;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="background-color: #B8CCE2;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php }	?>
		</tr>
		<tr id="tr1_infantil" style="display: none;">
			<td align="center" style="background-color: #EAEAEA">
				<b style="color: #4A8CCC;">Creche</b>&nbsp;
				<i class="fa fa-plus-circle" style="color: #000;"></i>&nbsp;
				<b style="color: #5C984E;">Pré-Escola</b>
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapaCre = '';
				$totEtapaPre = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='EDUC INFANTIL' && $d['no_creche_pre_escola']=='Creche' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapaCre += $d['vl_despesa_empenhada'];
					}
					if($d['ds_etapa']=='EDUC INFANTIL' && $d['no_creche_pre_escola']=='Pré-Escola' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapaPre += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="padding: 0px;">
					<table align="center" width="100%" cellpadding="0" cellspacing="0" style="padding: 0px; line-height: 34px;">
						<tr>
							<td align="center" style="background-color: #73A4D4; color: #fff; border-right: 1px solid #E7E7E7; height: 100%; width: 50%">
								<? echo $totEtapaCre ? number_format($totEtapaCre, 2, ",", ".") : '-'; ?>
							</td>
							<td align="center" style="background-color: #8AAF81; color: #fff;">
								<? echo $totEtapaPre ? number_format($totEtapaPre, 2, ",", ".") : '-'; ?>
							</td>
						</tr>
					</table>
				</td>
				<td align="center" style="background-color: #EAEAEA;">
					-
				</td>
			<?php
			}
			?>
		</tr>
		<tr id="tr2_infantil" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Profissionais do Magistério
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapaCre = '';
				$totEtapaPre = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Profissionais do Magistério' && $d['no_creche_pre_escola']=='Creche' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapaCre += $d['vl_despesa_empenhada'];
					}
					if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Profissionais do Magistério' && $d['no_creche_pre_escola']=='Pré-Escola' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapaPre += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="padding: 0px;">
					<table align="center" width="100%" cellpadding="0" cellspacing="0" style="padding: 0px; line-height: 34px;">
						<tr>
							<td align="center" style="background-color: #73A4D4; color: #fff; border-right: 1px solid #E7E7E7; height: 100%; width: 50%; font-size: 10px;">
								<? echo $totEtapaCre ? number_format($totEtapaCre, 2, ",", ".") : '-'; ?>
							</td>
							<td align="center" style="background-color: #8AAF81; color: #fff;">
								<? echo $totEtapaPre ? number_format($totEtapaPre, 2, ",", ".") : '-'; ?>
							</td>
						</tr>
					</table>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					$totEtapa = $totEtapaCre + $totEtapaPre;
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr id="tr3_infantil" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Profissionais de Serviço e Apoio Escolar
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapaCre = '';
				$totEtapaPre = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Profissionais de Serviço e Apoio E' && $d['no_creche_pre_escola']=='Creche' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapaCre += $d['vl_despesa_empenhada'];
					}
					if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Profissionais de Serviço e Apoio E' && $d['no_creche_pre_escola']=='Pré-Escola' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapaPre += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="padding: 0px;" >
					<table align="center" width="100%" cellpadding="0" cellspacing="0"  style="padding: 0px; line-height: 34px;">
						<tr>
							<td align="center" style="background-color: #73A4D4; color: #fff; border-right: 1px solid #E7E7E7; height: 100%; width: 50%; font-size: 10px;">
								<? echo $totEtapaCre ? number_format($totEtapaCre, 2, ",", ".") : '-'; ?>
							</td>
							<td align="center" style="background-color: #8AAF81; color: #fff; font-size: 10px;">
								<? echo $totEtapaPre ? number_format($totEtapaPre, 2, ",", ".") : '-'; ?>
							</td>
						</tr>
					</table>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					$totEtapa = $totEtapaCre + $totEtapaPre;
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr id="tr4_infantil" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Alimentação, Transporte, Material Didático,<br> Equipamentos, Instalações, Obras e Benfeitorias etc.
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapaCre = '';
				$totEtapaPre = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Alimentação, Transporte, Material' && $d['no_creche_pre_escola']=='Creche' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapaCre += $d['vl_despesa_empenhada'];
					}
					if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Alimentação, Transporte, Material' && $d['no_creche_pre_escola']=='Pré-Escola' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapaPre += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="padding: 0px;" >
					<table align="center" width="100%" cellpadding="0" cellspacing="0" style="padding: 0px; line-height: 49px;">
						<tr>
							<td align="center" style="background-color: #73A4D4; color: #fff; border-right: 1px solid #E7E7E7; height: 100%; width: 50%; font-size: 10px;">
								<? echo $totEtapaCre ? number_format($totEtapaCre, 2, ",", ".") : '-'; ?>
							</td>
							<td align="center" style="background-color: #8AAF81; color: #fff; font-size: 10px;">
								<? echo $totEtapaPre ? number_format($totEtapaPre, 2, ",", ".") : '-'; ?>
							</td>
						</tr>
					</table>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					$totEtapa = $totEtapaCre + $totEtapaPre;
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr>
			<td align="center" style="background-color: #D8E4BC;">
				<div align="left" style="float: left"><img src="../imagens/mais.gif" id="imgmais_fundamental" onclick="mostraTabela('fundamental')"><img src="../imagens/menos.gif" id="imgmenos_fundamental" style="display: none;" onclick="escondeTabela('fundamental')"></div>
				<b>Ensino Fundamental</b>
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='ENSINO FUNDAMENTAL' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="background-color: #D8E4BC;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="background-color: #D8E4BC;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php }	?>
		</tr>
		<tr id="tr1_fundamental" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Profissionais do Magistério
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='ENSINO FUNDAMENTAL' && $d['ds_profissionais']=='Profissionais do Magistério' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="font-size: 10px;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr id="tr2_fundamental" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Profissionais de Serviço e Apoio Escolar
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='ENSINO FUNDAMENTAL' && $d['ds_profissionais']=='Profissionais de Serviço e Apoio E' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="font-size: 10px;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr id="tr3_fundamental" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Alimentação, Transporte, Material Didático,<br> Equipamentos, Instalações, Obras e Benfeitorias etc.
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='ENSINO FUNDAMENTAL' && $d['ds_profissionais']=='Alimentação, Transporte, Material' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="font-size: 10px;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr>
			<td align="center" style="background-color: #FFFACD;">
				<div align="left" style="float: left"><img src="../imagens/mais.gif" id="imgmais_medio" onclick="mostraTabela('medio')"><img src="../imagens/menos.gif" id="imgmenos_medio" style="display: none;" onclick="escondeTabela('medio')"></div>
				<b>Ensino Médio</b>
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='MEDIO' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="background-color: #FFFACD;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="background-color: #FFFACD;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php }	?>
		</tr>
		<tr id="tr1_medio" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Profissionais do Magistério
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='MEDIO' && $d['ds_profissionais']=='Profissionais do Magistério' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="font-size: 10px;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr id="tr2_medio" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Profissionais de Serviço e Apoio Escolar
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='MEDIO' && $d['ds_profissionais']=='Profissionais de Serviço e Apoio E' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="font-size: 10px;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr id="tr3_medio" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Alimentação, Transporte, Material Didático,<br> Equipamentos, Instalações, Obras e Benfeitorias etc.
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='MEDIO' && $d['ds_profissionais']=='Alimentação, Transporte, Material' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="font-size: 10px;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr>
			<td align="center" style="background-color: #FFE4E1;">
				<div align="left" style="float: left"><img src="../imagens/mais.gif" id="imgmais_superior" onclick="mostraTabela('superior')"><img src="../imagens/menos.gif" id="imgmenos_superior" style="display: none;" onclick="escondeTabela('superior')"></div>
				<b>Ensino Superior</b>
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='SUPERIOR' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="background-color: #FFE4E1;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="background-color: #FFE4E1;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php }	?>
		</tr>
		<tr id="tr1_superior" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Profissionais do Magistério
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='SUPERIOR' && $d['ds_profissionais']=='Profissionais do Magistério' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="font-size: 10px;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr id="tr2_superior" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Profissionais de Serviço e Apoio Escolar
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='SUPERIOR' && $d['ds_profissionais']=='Profissionais de Serviço e Apoio E' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="font-size: 10px;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr id="tr3_superior" style="display: none;">
			<td align="center" style="font-size: 12px;">
				Alimentação, Transporte, Material Didático,<br> Equipamentos, Instalações, Obras e Benfeitorias etc.
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='SUPERIOR' && $d['ds_profissionais']=='Alimentação, Transporte, Material' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="font-size: 10px;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="font-size: 10px;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php
			}
			?>
		</tr>
		<tr>
			<td align="center" style="background-color: #FBC79B;">
				<b>Salário Educação</b>
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totEtapa = '';
				foreach ($despesas as $d) {
					if($d['ds_etapa']=='SALARIO EDUCACAO' && (int)$d['an_despesa']==(int)$ano ){
						$totEtapa += $d['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="background-color: #FBC79B;">
					<? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="background-color: #FBC79B;">
					<?
					$percent = '';
					foreach ($totalDespesa as $total) {
						if((int)$total['an_despesa']==(int)$ano ){
							if($total['vl_despesa_empenhada']>0){
								$percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
							}else{
								$percent = '';
							}
						}
					}
					echo $percent ? number_format($percent, 2, ",", "."): '-';
					?>
				</td>
			<?php }	?>
		</tr>
		<tr bgcolor="#CFCFCF">
			<td align="center" style="background-color: #F5F5F5;">
				<b>Total</b>
			</td>
			<?php
			for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
				$totAno = '';
				foreach ($totalDespesa as $total) {
					if((int)$total['an_despesa']==(int)$ano ){
						$totAno = $total['vl_despesa_empenhada'];
					}
				}
				?>
				<td align="center" style="background-color: #F5F5F5;">
					<? echo $totAno ? number_format($totAno, 2, ",", ".") : '-'; ?>
				</td>
				<td align="center" style="background-color: #F5F5F5;">
					100
				</td>
			<?php
			}
			?>
		</tr>
	</table>
</div>

<br>

<div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
	<h3>Cumprimento da Vinculação dos Recursos Destinados à Educação</h3>
	<table  cellpadding="2" cellspacing="2" width="100%" class="table table-striped table-bordered table-hover table-condensed tabela-listagem">
		<tr>
			<td align="left" style="background-color: #E0E0E0;">
				<b>Vinculação</b>
			</td>
			<?php foreach($cumprimento as $c) {?>
				<td align="center" style="background-color: #E0E0E0;">
					<b><?echo $c["an_exercicio_mde"] ? $c["an_exercicio_mde"] : '-';?></b>
				</td>
			<?php }?>
		</tr>
		<tr>
			<td align="left" style="background-color: #B8CCE2;">
				<b>Aplicação da receita de impostos e transferências em Manutenção e Desenvolvimento do Ensino (MDE)</b>
			</td>
			<?php foreach($cumprimento as $c) {?>
				<td align="center" style="background-color: #EAEAEA;">
					<?echo $c['vl_apl_rec_mde'] ? number_format($c['vl_apl_rec_mde'],2,",",".") : '-';?>
				</td>
			<?php }?>
		</tr>
		<tr>
			<td align="left" style="background-color: #B8CCE2;">
				<b>% Aplicação do Município</b>
			</td>
			<?php foreach($cumprimento as $c) {?>
				<td align="center" style="background-color: #EAEAEA;">
					<?echo $c['vl_perc_aplic_med'] ? number_format($c['vl_perc_aplic_med'],2,",",".") : '-';?>
				</td>
			<?php }?>
		</tr>
		<tr>
			<td align="left" style="background-color: #B8CCE2;">
				<b>% Aplicação Obrigatória</b>
			</td>
			<?php foreach($cumprimento as $c) {?>
				<td align="center" style="background-color: #EAEAEA;">
					mínimo de 25%
					<span title="Conforme o Art. 212 da Constituição Federal, os Municípios deverão aplicar 25%, no mínimo, da receita resultante de impostos, compreendida a proveniente de transferências, na manutenção e desenvolvimento do ensino (MDE).">
						<img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
					</span>
				</td>
			<?php }?>
		</tr>
		<tr>
			<td align="left" style="background-color: #CADAA8;">
				<b>Aplicação das receitas do FUNDEB na remuneração dos profissionais do magistério</b>
			</td>
			<?php foreach($cumprimento as $c) {?>
				<td align="center" style="background-color: #D5E2BA;">
					<?echo $c['vl_apl_rem__prof_mag_rpm'] ? number_format($c['vl_apl_rem__prof_mag_rpm'],2,",",".") : '-';?>
				</td>
			<?php }?>
		</tr>
		<tr>
			<td align="left" style="background-color: #CADAA8;">
				<b>% Aplicação do Município</b>
			</td>
			<?php foreach($cumprimento as $c) {?>
				<td align="center" style="background-color: #D5E2BA;">
					<?echo $c['vl_perc_aplic_mag_rpm'] ? number_format($c['vl_perc_aplic_mag_rpm'],2,",",".") : '-';?>
				</td>
			<?php }?>
		</tr>
		<tr>
			<td align="left" style="background-color: #CADAA8;">
				<b>% Aplicação Obrigatória</b>
			</td>
			<?php foreach($cumprimento as $c) {?>
				<td align="center" style="background-color: #D5E2BA;">
					mínimo de 60%
					<span title="Conforme o Art. 22 da Lei 11.949/2007 (Lei do Fundeb), pelo menos 60% dos recursos anuais totais dos Fundeb serão destinados ao pagamento da remuneração dos profissionais do magistério da educação básica em efetivo exercício na rede pública.">
						<img src="../imagens/IconeAjuda.gif" style="cursor: pointer; vertical-align: baseline;">
					</span>
				</td>
			<?php }?>
		</tr>
		<tr>
			<td align="left" style="background-color: #FBC79B;">
				<b>Aplicação das receitas do FUNDEB em despesas com MDE, excluída a remuneração do magistério</b>
			</td>
			<?php foreach($cumprimento as $c) {?>
				<td align="center" style="background-color: #FCD5B4;">
					<?echo $c['vl_apl_exc_rem_mag_erm'] ? number_format($c['vl_apl_exc_rem_mag_erm'],2,",",".") : '-';?>
				</td>
			<?php }?>
		</tr>
		<tr>
			<td align="left" style="background-color: #FBC79B;">
				<b>% Aplicação do Município</b>
			</td>
			<?php foreach($cumprimento as $c) {?>
				<td align="center" style="background-color: #FCD5B4;">
					<?echo $c['vl_perc_aplic_exc_mag_erm'] ? number_format($c['vl_perc_aplic_exc_mag_erm'],2,",",".") : '-';?>
				</td>
			<?php }?>
		</tr>
		<tr>
			<td align="left" style="background-color: #FBC79B;">
				<b>% Aplicação Obrigatória</b>
			</td>
			<?php foreach($cumprimento as $c) {?>
				<td align="center" style="background-color: #FCD5B4;">
					máximo de 40%
				</td>
			<?php }?>
		</tr>
	</table>
</div>

<script>
	function mostraTabela(id){
		document.getElementById("imgmais_"+id).style.display = 'none';
		document.getElementById("imgmenos_"+id).style.display = '';
		document.getElementById("tr1_"+id).style.display = '';
		document.getElementById("tr2_"+id).style.display = '';
		document.getElementById("tr3_"+id).style.display = '';
		document.getElementById("tr4_"+id).style.display = '';
	}

	function escondeTabela(id){
		document.getElementById("imgmais_"+id).style.display = '';
		document.getElementById("imgmenos_"+id).style.display = 'none';
		document.getElementById("tr1_"+id).style.display = 'none';
		document.getElementById("tr2_"+id).style.display = 'none';
		document.getElementById("tr3_"+id).style.display = 'none';
		document.getElementById("tr4_"+id).style.display = 'none';
	}
</script>
