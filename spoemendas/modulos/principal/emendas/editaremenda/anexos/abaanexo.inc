<?php
$perfis = pegaPerfilGeral($_SESSION['usucpf']);
if(in_array(PFL_ASPAR,$perfis) || in_array(PFL_SUPER_USUARIO,$perfis)){
    ?>
<div class="col-xs-12 col-sm-12 col-md-12 col-lg-12 form-group">
	<a href="spoemendas.php?modulo=principal/emendas/editaremenda/anexos/anexo&acao=A&emeid=<?= $_REQUEST['emeid'];?>"
		class="btn btn-primary">
		<i class="glyphicon glyphicon-upload"></i>
		Anexar Ofício
	</a>
</div>
<?php
}
?>
<div class="col-xs-12 col-sm-12 col-md-12 col-lg-12">

	<?php
	$query = <<<SQL
				SELECT
					ofi.arqid,
					ofi.ofiid,
					autnome || ' (' || par.parsigla || ')' AS solicitante,
					(
						SELECT 
							string_agg(tps.tpsdsc, ' / ')
						FROM 
							spoemendas.oficiotiposolicitacao oft
						INNER JOIN 
							spoemendas.tiposolicitacao tps ON tps.tpsid = oft.tpsid 
						WHERE 
							oft.ofiid = ofi.ofiid 
						GROUP BY oft.ofiid
					) AS tiposolicitacao,
					usu.usunome,
					TO_CHAR(ofi.oficriacao, 'DD/MM/YYYY HH24:MI:SS'),
					esd.esddsc
				FROM 
					emenda.oficio ofi
				INNER JOIN 
					emenda.autor aut ON ofi.autid = aut.autid
				INNER JOIN 
					emenda.partido par ON aut.parid = par.parid
				INNER JOIN 
					seguranca.usuario usu ON ofi.usucpf = usu.usucpf
				INNER JOIN 
					workflow.documento doc ON ofi.docid = doc.docid
				INNER JOIN 
					workflow.estadodocumento esd ON doc.esdid = esd.esdid
				INNER JOIN
				  public.arquivo arq ON arq.arqid = ofi.arqid
				WHERE arq.arqstatus = 'A' AND ofi.emeid = {$_REQUEST['emeid']}
SQL;

	$list = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO);
	
	$list->setQuery($query)
	->turnOnPesquisator()
	->addAcao('download', 'baixarAnexo');
	
	if(verificaPerfil(array(PFL_CGO_EQUIPE_ORCAMENTARIA, PFL_SUPER_USUARIO, PFL_ASPAR))) {
		$list->addAcao('edit', array('func' => 'editarAnexo', 'extra-params' => array('ofiid')));
	}
	
	$list->setCabecalho(['Solicitante', 'Tipo de Solicitação','Quem anexou', 'Data e hora do anexo', 'Status do anexo'])
	->esconderColunas(['ofiid'])
	->addCallbackDeCampo('esddsc', 'formatarStatusAnexo')
	->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
	?>
</div>

<script>
	function baixarAnexo(arqid) {
	    window.location.assign(
	            'spoemendas.php?modulo=principal/emendas/editaremenda/'+
	            'editaremenda&acao=A&requisicao=baixar_anexo&arqid=' + arqid);
	}
  function editarAnexo(arqid, ofiid) {
		window.location.assign(
	            'spoemendas.php?modulo=principal/emendas/editaremenda/'+
	            'anexos/anexo&acao=A&emeid=<?=$_REQUEST['emeid']?>&ofiid=' + ofiid);
	}
</script>
