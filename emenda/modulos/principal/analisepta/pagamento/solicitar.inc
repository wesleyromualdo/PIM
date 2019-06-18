<?php
// topo

if( $_REQUEST['requisicao'] == 'salvar_conta' ){

	$ptrcontacorrente = $_REQUEST['ptrcontacorrente'];
	$ptrcontacorrente = str_pad($ptrcontacorrente, 10, "0", STR_PAD_LEFT);

	$sql = "update emenda.planotrabalho set ptrcontacorrente = '{$ptrcontacorrente}' where ptrid = {$_REQUEST['ptrid']}";
	$db->executar($sql);
	$db->commit();
	$db->sucesso('principal/analisepta/pagamento/solicitar');
	exit();
}

if( $_REQUEST['requisicao'] == 'mostraDadosConta' ){

	$sql = "select p.ptragenciabancaria, p.ptrnumprocessoempenho, b.bconome
			from emenda.planotrabalho p
				inner join emenda.banco b on b.bcoid = p.bcoid
			where p.ptrid = {$_REQUEST['ptrid']}";
	$arDados = $db->pegaLinha($sql);

	$str = $arDados['ptrnumprocessoempenho'];
	$str = substr($str,0,5).'.'.substr($str,5,6).'/'.substr($str,11,4).'-'.substr($str,15,2);
	?>
	<form name="formulario_conta" id="formulario_conta" method="post">   
    	<input type="hidden" name="requisicao" id="requisicao" value="">
    	<input type="hidden" name="ptrid" id="ptrid" value="<?php echo $_REQUEST['ptrid']?>">
	<table class="tabela" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
	<thead>
		<tr>
			<td class="subtitulodireita" width="30%">Processo:</td>
			<td><?php echo $str; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Banco:</td>
			<td><?php echo $arDados['bconome']; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Agência:</td>
			<td><?php echo $arDados['ptragenciabancaria']; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Conta Corrente:</td>
			<td><?php echo campo_texto( 'ptrcontacorrente', 'S', "S", '', 20, 15, '', '', '', '', 0, 'id="ptrcontacorrente"' ); ?></td>
		</tr>
	</thead>
	</table>
	</form>
	<?php 
	exit();
}

include_once APPEMENDA . 'modulos/principal/analisepta/pagamento/partialsControl/topo.inc';
require_once APPRAIZ . "emenda/classes/WSEmpenho.class.inc";
require_once APPRAIZ . "emenda/classes/ExecucaoFinanceira.class.inc";
require_once APPRAIZ . "emenda/classes/Habilita.class.inc";

unset( $_SESSION['emenda']['msgErro'] );
$_SESSION['emenda']['msgErro'] = array();

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);
// Cria o título da tela
monta_titulo( 'Pagamento', "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");

echo cabecalhoPlanoTrabalho($ptrid, true);
montaVisualizarPTA( $_SESSION["emenda"]["ptridAnalise"], false, $boReformulacao );

$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );	

$arrPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

$arrEstados = array(EM_LIBERACAO_RECURSO_IMPOSITIVO, EM_LIBERACAO_RECURSO, EM_LIBERACAO_RECURSO_REFORMULACAO);

if ( !in_array($estadoAtual, $arrEstados) && ( !in_array( PAGAMENTO, $arrPerfil) || in_array(SUPER_USUARIO, $arrPerfil) ) ) {
	$boDisabled = 'disabled="disabled"';
}
$boPendencia = false;
$label = 'Nº Empenho Original';
if( !$boReformulacao ){
	$federal = buscaEmendaDescentraliza( $ptrid );
	
	$label = 'Nº Empenho Original';
	if( $federal == 'S' ) $label = 'Nota de Crédito';
}
$obHabilita = new Habilita();
$cnpjHabilita = $obHabilita->pegaCnpj($ptrid);

$sql = "SELECT distinct em.empnumeroprocesso||'&nbsp;' as processo, em.empcodigoptres||'&nbsp;' as ptres, em.empcodigopi,
			to_char(pa.pagdatapagamento, 'DD/MM/YYYY') as datapagamento, pa.parnumseqob||'&nbsp;' as ordembancaria, pa.pagsituacaopagamento,
			p.pobvalorpagamento, p.pobpercentualpag
			FROM par.subacaoemendapta sep
			inner join par.subacaodetalhe sd on sd.sbdid = sep.sbdid
			inner join par.subacao s on s.sbaid = sd.sbaid
			inner join par.pagamentosubacao p on p.sbaid = s.sbaid and pobstatus = 'A' -- se tiver nesta tabela teve empenho
			inner join par.pagamento pa on pa.pagid = p.pagid and pa.pagstatus = 'A'
			inner join par.empenho em on em.empid = pa.empid and empstatus = 'A'
			inner join emenda.ptemendadetalheentidade ped on ped.ptrid = sep.ptrid
			inner join emenda.v_emendadetalheentidade ved on ved.edeid = ped.edeid
		WHERE
			sep.ptrid = $ptrid
			and sep.sepstatus = 'A'
			and ved.edestatus = 'A'";

$arrDadosPagPar = $db->carregar( $sql );
$arrDadosPagPar = $arrDadosPagPar ? $arrDadosPagPar : array();


if($arrDadosPagPar[0]){
	echo '<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
				<tr>
					<th colspan="2" class="subtitulocentro">Pagamento PAR</th>
				</tr>
				<tr>
					<th class="subtitulocentro" colspan="2" style="color: red;">Este plano de trabalho foi realizado o pagamento como transferência direta no sistema PAR.</th>
				</tr>
				<tr>
					<th colspan="2" class="subtitulocentro">Dados do Pagamento PAR</th>
				</tr>
			</table>';
	$cabecalho = array('Número do Processo', 'Número do PTRES', 'Número do PI', 'Data Pagamento', 'Nº Ordem bancária', 'Situação', 'Valor', '% Empenhado');
	$db->monta_lista_array($arrDadosPagPar, $cabecalho, 5000, 20, 'S', 'center', '');
} else {

	$arDadosPag = $obPagamento->selectParaPagamento();
	
	$arMSG = array();
	$arMSGExiste = array();
	$arrPendencia = array();
	foreach ($arDadosPag as $v) {
		$arMSG = array();
		if( empty($v['enbcnpj']) ) 				 
	//		if( !in_array( 'enbcnpj', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº do cnpj da entidade não foi encontrado vinculado ao PTA!</span>' );
				array_push( $arMSGExiste, 'enbcnpj' );
				$arrPendencia[$v['exfid']] = $arMSG;
	//		}
		if( empty($v['cocseq_conta']) )
	//		if( !in_array( 'cocseq_conta', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº sequencial da conta corrente não encontrado!</span>' );
				array_push( $arMSGExiste, 'cocseq_conta' );
				$arrPendencia[$v['exfid']] = $arMSG;
	//		}
		if( empty($v['ptrnumprocessoempenho']) ) 
	//		if( !in_array( 'ptrnumprocessoempenho', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº processo empenho não encontrado (aba informações gerais)!</span>' );
				array_push( $arMSGExiste, 'ptrnumprocessoempenho' );
				$arrPendencia[$v['exfid']] = $arMSG;
	//		}
		if( empty($v['ptranoconvenio']) )
	//		if( !in_array( 'ptranoconvenio', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Ano do convênio não cadastrado!</span>' );
				array_push( $arMSGExiste, 'ptranoconvenio' );
				$arrPendencia[$v['exfid']] = $arMSG;
	//		}
		if( empty($v['ptrnumconvenio']) )
	//		if( !in_array( 'ptrnumconvenio', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº do convênio não cadastrado!</span>' );
	//			array_push( $arMSGExiste, 'ptrnumconvenio' );
				$arrPendencia[$v['exfid']] = $arMSG;
	//		}
		if( empty($v['pmcnumconveniosiafi']) ) 	 
	//		if( !in_array( 'pmcnumconveniosiafi', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº do convênio SIAF não cadastrado (É necessário importar o convênio)!</span>' );
	//			array_push( $arMSGExiste, 'pmcnumconveniosiafi' );
				$arrPendencia[$v['exfid']] = $arMSG;
	//		}
		if( empty($v['exfvalor']) ) 			 
	//		if( !in_array( 'exfvalor', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Valor do pagamento não informado (aba informações gerais)!</span>' );
	//			array_push( $arMSGExiste, 'exfvalor' );
				$arrPendencia[$v['exfid']] = $arMSG;
	//		}
		if( empty($v['pubdatapublicacao']) && ( !in_array(APOSTILAMENTO, $arRrefid) ) ) 	 
	//		if( !in_array( 'pubdatapublicacao', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Data de publicação não cadastrada (aba DOU)!</span>' );
	//			array_push( $arMSGExiste, 'pubdatapublicacao' );
				$arrPendencia[$v['exfid']] = $arMSG;
			//}
		if( empty($v['exfnumempenhooriginal']) ) 
			//if( !in_array( 'exfnumempenhooriginal', $arMSGExiste ) ){
				array_push( $arMSG, '<span style="color: red; font-weight: bold;">Nº do empenho original não cadastrado (Empenho não efetivado)!</span>' );
				//array_push( $arMSGExiste, 'exfnumempenhooriginal' );
				$arrPendencia[$v['exfid']] = $arMSG;
			//}
	}
	
	//if( !empty( $arMSG ) || $obHabilita->consultaHabilitaEntidade($cnpjHabilita, true) != 3  ) $boPendencia = true;
	
	$arDados = $obPagamento->carregaDadosEmpenhoPagamento();
	include_once APPEMENDA.'modulos/principal/analisepta/pagamento/views/formDefault.inc';
	include_once APPEMENDA.'modulos/principal/analisepta/pagamento/views/partialsViews/listaSolicitacaoPagamento.inc';
	
	/*foreach ($arParcela as $key => $v) {
		$boSolicitacao = ( $v['orbnumsolicitacao'] ? true : false );
		$numparcela = sizeof($arParcelaPaga);
		if( !$boSolicitacao ){
		echo "<script>
			$('boalterou').value = 'false';
			
			var valorPag = 'valorigual_'+{$v['exfid']};
			var parcela = {$numparcela};
			
			//verifica se o valor da parcela e menor que o valor total do pagamento
			if($(valorPag).value == '1'){
				carregarParcela( '{$v['orbmesparcela']}', '{$v['orbanoparcela']}', '{$v['orbvalorparcela']}', '{$v['orbid']}', '{$v['exfid']}', '', '{$boDisabled}', parcela );
			}
			</script>";
		}
	}*/
}
?>