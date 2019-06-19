<?php
if (isset($_REQUEST['requisicao']) && 'consultar-usuario' == $_REQUEST['requisicao']) {
    $sql = <<<DML
SELECT usu.usunome
  FROM seguranca.usuario usu
  WHERE usu.usucpf = '%s'
DML;

    $sql = sprintf(
        $sql,
        str_replace(
            ['.', '-'],
            '',
            filter_input(INPUT_GET, 'usucpf', FILTER_SANITIZE_FULL_SPECIAL_CHARS)
        )
    );

    if (!($nome = $db->pegaUm($sql))) {
        $nome = '';
    }

    die(simec_json_encode(['usunome' => $nome]));
}

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$empid = $_SESSION['obras2']['empid'];
$obrid = $_REQUEST['obrid'];
$slcid = trim($_REQUEST['slcid']);
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR, PFLCOD_GESTOR_UNIDADE, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_UNIDADE);
$habilita = 'N';
$habilitaSuper = 'N';
if(possui_perfil($pflcods)) {
	$habilita = 'S';
}
if(possui_perfil(array(PFLCOD_SUPER_USUARIO,PFLCOD_GESTOR_MEC))){
    $habilitaSuper = 'S';
}

if($_REQUEST['carregarAnexo']) {
	$sql = <<<DML
        SELECT
            a.axsid,
            a.arqid,
            ar.arqnome||'.'||ar.arqextensao as arquivo,
            a.axsdsc as descricao
        FROM  obras2.anexosolicitacao a
        INNER JOIN public.arquivo ar on ar.arqid = a.arqid
        WHERE a.slcid = $slcid
            AND a.arqid = {$_POST['arqid']}
            AND a.axstipo = 'PD'
DML;

	$arrDados = $db->pegaLinha($sql);
	$arrDados['descricao'] = ($arrDados['descricao']);
	echo simec_json_encode($arrDados);
	die;
}

if($_REQUEST['requisicao'] == 'excluirAnexo') {
    $sql = "DELETE FROM obras2.anexosolicitacao where arqid=".$_REQUEST['arqidDel']." and axstipo = 'PD'";
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $db->commit();

    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);

    echo simec_json_encode(true);
    //$db->sucesso('principal/solicitacao', '&obrid='.$_REQUEST['obrid'].'&slcid='.$_REQUEST['slcid']);
    die;
}

#manipular anexos de arquivos para envio de email
if($_REQUEST['download'] == 'S') {
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'obras2.php?modulo=principal/solicitacao&acao=A';</script>";
    exit;
}

if($_POST['requisicao'] == 'excluirPedido') {
	$sql = "UPDATE obras2.solicitacao SET slcstatus = 'I' WHERE slcid = {$_POST['slcid']}";
	$db->executar($sql);

	$sql = "UPDATE obras2.anexosolicitacao SET axsstatus = 'I' WHERE slcid = {$_POST['slcid']}";
	$db->executar($sql);

    $db->commit();
    $db->sucesso('principal/solicitacao', '&obrid='.$_REQUEST['obrid']);
    die;
}

if ($_POST['requisicao'] == 'salvarPedido') {
	$slcjustificativa = $_POST['slcjustificativa'] ? substr ($_POST['slcjustificativa'] , 0 , 4000) : 'null';
	$slcobservacao = $_POST['slcobservacao'] ? substr ($_POST['slcobservacao'] , 0 , 4000) : 'null';
	$arqdescricao = $_POST['arqdescricao'] ? $_POST['arqdescricao'] : 'null';
	$slcid = $_POST['slcid'];
	$tslid = $_GET['tslid'][0];
    $slccpf = str_replace(['.', '-'], '', $_REQUEST['slccpf']);
	if (empty($slcid)) {
        $slcobservacao = (strlen($slcobservacao) > 4000) ? substr($slcobservacao, 0, 3998) : $slcobservacao;
        $slcjustificativa = (strlen($slcjustificativa) > 4000) ? substr($slcjustificativa, 0, 3998) : $slcjustificativa;
        $docid = wf_cadastrarDocumento(TPDID_SOLICITACOES, "Fluxo de Solicitação - obrid " . $obrid);
        if ($tslid == 6 || $tslid == 7 || $tslid == 9 || $tslid == 10 || $tslid == 11 || $tslid == Solicitacao::TSLID_CORRECAO_INCONSISTENCIA_NO_SISTEMA)
        {
            $aprovado = 'S';
        }else{
            $aprovado = 'N';
        }

        $solicitacao = new Solicitacao();
        $dados = [
            'obrid' => $obrid,
            'slcobservacao' => $slcobservacao,
            'slcjustificativa' => $slcjustificativa,
            'usucpf' => $_SESSION['usucpf'],
            'docid' => $docid,
            'aprovado' => $aprovado,
            'slcsolicitaralterar' => $_REQUEST['slcsolicitaralterar'],
            'slcsenha' => $_REQUEST['slcsenha'],
            'slcalteracao' => $_REQUEST['slcalteracao'],
            'slcoutros' => $_REQUEST['slcoutros'],
            'slccpf' => $slccpf,
            'slcnome' => $_REQUEST['slcnome']
        ];

        if (Solicitacao::TSLID_CORRECAO_INCONSISTENCIA_NO_SISTEMA == $tslid) {
            $dados['tipoerro'] = $_REQUEST['tipoerro'];
        }

        $solicitacao->popularDadosObjeto($dados);
        $solicitacao->salvar();
		$slcid = $solicitacao->slcid;
	} else {
        $solicitacao = new Solicitacao($slcid);
        $solicitacao->docid = empty($solicitacao->docid) ? wf_cadastrarDocumento(TPDID_SOLICITACOES, "Fluxo de Solicitação - obrid " . $obrid) : $solicitacao->docid;
        $solicitacao->slcobservacao = $slcobservacao;
        $solicitacao->slcjustificativa = $slcjustificativa;
        $solicitacao->slcsolicitaralterar = $_REQUEST['slcsolicitaralterar'];
        $solicitacao->slcsenha = $_REQUEST['slcsenha'];
        $solicitacao->slcalteracao = $_REQUEST['slcalteracao'];
        $solicitacao->slcoutros = $_REQUEST['slcoutros'];
        $solicitacao->slccpf = $slccpf;
        $solicitacao->slcnome = $_REQUEST['slcnome'];
        $solicitacao->salvar();
	}

    $db->executar('DELETE FROM obras2.tiposolicitacao_solicitacao WHERE slcid = ' . $slcid);
    if(is_array($_POST['tslid'])){
        foreach($_POST['tslid'] as $tslid) {
            $db->executar('INSERT INTO obras2.tiposolicitacao_solicitacao (slcid, tslid) VALUES ('.$slcid.', '.$tslid.')');
            if($slcid && $docid && ($tslid == 6 || $tslid == 7 || $tslid == 9 || $tslid == 10 || $tslid == 11 || $tslid == Solicitacao::TSLID_CORRECAO_INCONSISTENCIA_NO_SISTEMA)) {
                $doc = wf_pegarDocumento($docid);
                if($doc['esdid'] == ESDID_SOLICITACOES_CADASTRAMENTO) {
                    $acao = wf_pegarAcao(ESDID_SOLICITACOES_CADASTRAMENTO,ESDID_SOLICITACOES_AGUARDANDO_ANALISE);
                    wf_alterarEstado($docid,$acao['aedid'],'',array(),array());
                }
            }
        }
    }

	if($_FILES["arquivo"]['name'] && !$_POST['arqid']) {
		$campos	= array(
            "slcid"  => $slcid,
            "axsdsc" => "'{$arqdescricao}'",
            "usucpf" => "'{$_SESSION['usucpf']}'",
            "axstipo" => "'PD'"
        );
		$file = new FilesSimec("anexosolicitacao", $campos ,"obras2");
		$arquivoSalvo = $file->setUpload();

	} elseif($_POST['arqid']) {
	    $sql = "UPDATE obras2.anexosolicitacao SET axsdsc = '{$arqdescricao}' where arqid=".$_POST['arqid']." and axstipo = 'PD'";
	    $db->executar($sql);
	}

	$db->commit();
	if( $_POST['btAcao'] == 'anexo' || $slcid ) {
		$db->sucesso('principal/solicitacao', '&obrid='.$_REQUEST['obrid'].'&slcid='.$slcid.'&tslid[]='.$_GET['tslid'][0]);
	} else {
		$db->sucesso('principal/solicitacao', '&obrid='.$_REQUEST['obrid'].'&tslid[]='.$_GET['tslid'][0]);
    	die;
	}
}

if($_REQUEST['slcid']) {
	$urlP = "/obras2/obras2.php?modulo=principal/solicitacao&acao=A&obrid=".$obrid."&slcid=".$slcid;
} else {
	$urlP = "/obras2/obras2.php?modulo=principal/solicitacao&acao=A&obrid=".$obrid;
}
$urlP .= $_GET['tslid'] ? ('&tslid[]='.$_GET['tslid'][0]) : '';

if ($slcid) {
    $where = array();
    $where[] = "slcid = $slcid";

    if (isset($_GET['tslid'][0]) && !empty($_GET['tslid'][0])) {
        $where[] = "s.slcid IN (SELECT slcid FROM obras2.tiposolicitacao_solicitacao WHERE tslid IN ({$_GET['tslid'][0]}) AND slcid = s.slcid)";
    }

    $sql = "
SELECT slcid,
       slcobservacao,
       slcjustificativa,
       slcdatainclusao,
       usucpf,
       docid,
       aprovado,
       slcsolicitaralterar,
       slcsenha,
       slcalteracao,
       slcoutros,
       slccpf,
       slcnome,
       tipoerro
  FROM obras2.solicitacao s
  WHERE slcstatus = 'A'
    AND " . implode(' AND ', $where) . "
    AND s.obrid = {$obrid}
  ORDER BY slcid DESC
";
    $arrPedido = $db->pegaLinha($sql);
    $slcjustificativa = stripslashes($arrPedido['slcjustificativa']);
    $slcobservacao =  stripslashes($arrPedido['slcobservacao']);
    $docid = $arrPedido['docid'];
    $usucpf = $arrPedido['usucpf'];
    $slcid = $arrPedido['slcid'];
    $aprovado = $arrPedido['aprovado'];
    $slccpf = $arrPedido['slccpf'];
    $slcnome = $arrPedido['slcnome'];
}

$arrPedido['slcsolicitaralterar'] = $arrPedido['slcsolicitaralterar']?$arrPedido['slcsolicitaralterar']:'N';
$arrPedido['slcsenha'] = $arrPedido['slcsenha']?$arrPedido['slcsenha']:'N';
$arrPedido['slcalteracao'] = $arrPedido['slcalteracao']?$arrPedido['slcalteracao']:'N';
$arrPedido['slcoutros'] = $arrPedido['slcoutros']?$arrPedido['slcoutros']:'N';
//ver($urlP, $obrid, $slcid, d);
print carregaAbaSolicitacao($urlP, $obrid, $slcid);
//ver("ok",d);
$tipo = '';
if(isset($_GET['tslid'][0]) && !empty($_GET['tslid'][0])){
    $tipo = $db->pegaUm("SELECT tslnome FROM obras2.tiposolicitacao WHERE tslid IN ({$_GET['tslid'][0]})");
}

monta_titulo( "Solicitação $tipo", "" );
$tslidoriginal = $_GET['tslid'][0];

if($docid){
    $mostraWorkflow = false;
    $documento = wf_pegarDocumento($docid);
    if($documento['esdid'] == ESDID_SOLICITACOES_CADASTRAMENTO || ($documento['esdid'] == ESDID_SOLICITACOES_DILIGENCIA)) {
        $mostraWorkflow = true;
    }

    if($documento['esdid'] != ESDID_SOLICITACOES_CADASTRAMENTO && $documento['esdid'] != ESDID_SOLICITACOES_DILIGENCIA ) {
        $habilita = 'N';
    }

    if($_GET['tslid'][0] == 6 && $documento['esdid'] == ESDID_SOLICITACOES_DEFERIDO){
        $res = $db->pegaUm('SELECT obrtravaedicaocronograma FROM obras2.obras WHERE obrid = '.$obrid);
        if($res == 'f'){
            $habilita = 'N';
        }
    }
}

echo cabecalhoObra($obrid, 'simples');

$_REQUEST['tslid'] = !$_REQUEST['tslid'] ? array() : $_REQUEST['tslid'];

?>
<html>
    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

    <script type="text/javascript">    
    var tslid = <?=current($_REQUEST['tslid'])?>;

	$(document).ready(function(){
		$('#tr_obras1').css('display', 'none');
		$('#td_altera').css('display', 'none');
		$('#td_insere').css('display', '');

		if( $('#habilita').val() == 'N' ){
			$('input, select, textarea').attr('disabled', true);
			$('.img_add_foto').hide();
		}

		$("[name='btn_cancelar']").click(function() {
			$('[id=div_sub_foto]').hide();
			$('#td_altera').css('display', 'none');
			$('#td_insere').css('display', '');
		});

		$('[name=btn_upload]').click(function() {
		 	var erro = 0;
		 	if(!$('#formulario [name=arquivo]').val() && $('#td_insere').css('display') != 'none' ){
		 		alert('Favor selecionar o anexo!');
		 		erro = 1;
		 		return false;
		 	}
		 	if(!$('#formulario [name=arqdescricao]').val()){
		 		alert('Favor informar uma descrição!');
		 		$('#[name=arqdescricao]').focus()
		 		erro = 1;
		 		return false;
		 	}
		 	if(erro == 0 && validaCampos() ){
		 		$('[name=btn_upload]').val("Carregando...");
		 		$('[name=btn_cancelar]').val("Carregando...");
		 		$('[name=btn_upload]').attr("disabled","disabled");
		 		$('[name=btn_cancelar]').attr("disabled","disabled");
		 		$('#requisicao').val('salvarPedido');
		 		$('#btAcao').val('anexo');
		 		$('[name=formulario]').submit();
		 	}
		 });

         jQuery('#slccpf').change(function(){
             var usucpf = jQuery(this).val();

             jQuery.ajax({
                 url: window.location.href,
                 dataType: 'json',
                 async: false,
                 data: {requisicao: 'consultar-usuario', usucpf: usucpf},
                 error: function(){
                     jQuery('#slcnome').focus();
                 },
                 success: function(data){
                     jQuery('#slcnome').val(data.usunome);
                 }
             });
         });
	});
	function addAnexos( arqid, slcid ){
		if( arqid ){
			$.ajax({
					type: "POST",
					dataType: "json",
					url: 'obras2.php?modulo=principal/solicitacao&acao=A',
					data: { carregarAnexo: "true", arqid: arqid, slcid: slcid},
					async: false,
					success: function(retornoajax) {
						$('[name="arqdescricao"]').val(retornoajax.descricao);
						$('#td_altera').html(retornoajax.arquivo);
  				}
  			});

  			$('#td_altera').css('display', '');
			$('#td_insere').css('display', 'none');
		} else {
			$('#td_altera').css('display', 'none');
			$('#td_insere').css('display', '');
            $('.popup_alerta').removeClass('hidden');

			$('[name="arqdescricao"]').val('');
			$('[name="arqid"]').val('');
		}
		$('[name=arqid]').val(arqid);
		$('[id=div_sub_foto]').show();
		$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
	}

	function validaCampos()
    {
        if (<?=Solicitacao::TSLID_ALT_FISCALOBRA_GESUNIDADE_CONUNIDADE;?> == tslid) {
            if (!$('#slccpf').val()) {
                alert('Informe o campo "CPF do usuário"!');
                $('#slccpf').focus();
                return false;
            }
            if (!$('#slcnome').val()) {
                alert('Informe o campo "Nome do usuário"!');
                $('#slcnome').focus();
                return false;
            }
        }

        if ((tslid == <?=Solicitacao::TSLID_CORRECAO_INCONSISTENCIA_NO_SISTEMA;?>)
             && (0 === $('input[name="tipoerro"]:checked').length))
        {
            alert('Informe o campo "Tipo de correçao"!');
            return false;
        }

		if( $('#slcjustificativa').val() == '' ){
			alert('Informe o campo "Justificativa"!');
			$('#slcjustificativa').focus();
			return false;
		}
		return true;
	}

    function novaSolicitacao(){
        var url = window.location.href;
        url = url.replace(/(&slcid=[0-9]*)/g,'');
        window.location.href = url;
    }

	function salvarPedido(){
		if (validaCampos()) {
            $('[name=btn_salvar').attr('disabled','disabled');
			$('#requisicao').val('salvarPedido');
			$('[name=formulario]').submit();
		}
	}

	function alterarPedido( slcid, obrid, tslid){
        var parametroExtra = '';
        if(tslid != '') {
            parametroExtra = '&tslid[]='+tslid;
        }
		window.location.href = 'obras2.php?modulo=principal/solicitacao&acao=A&obrid='+obrid+'&slcid='+slcid+''+parametroExtra;
	}

	function excluirAnexo( arqid, slcid, obrid ){
	 	if ( confirm( 'Deseja excluir o Anexo?' ) ) {
	 		//window.location.href = 'obras2.php?modulo=principal/solicitacao&acao=A&obrid='+obrid+'&slcid='+slcid+'&arqidDel='+arqid;
                        if( arqid ){
			$.ajax({
					type: "POST",
					dataType: "json",
					url: 'obras2.php?modulo=principal/solicitacao&acao=A',
					data: { requisicao: "excluirAnexo", arqidDel: arqid, slcid: slcid, obrid:obrid},
					async: false,
					success: function(retornoajax) {
                                            if(retornoajax){
                                                alert("Arquivo excluído com sucesso!");
						window.location.href = window.location.href ;
                                            }
                                        }
                                });
                        }
                }
        }
    function printQuestionario() {
//        var url = "/obras2/obras2.php?modulo=principal/questionarioImpressaoSolicitacoes&acao=A&qrpid=<?//=$qrpid?>//&obrid=<?//=$obrid?>//&slcid=<?//=$slcid?>//&queid=<?//=$queid?>//";
//        popup2 = window.open(
//            url,
//            "_blank",
//            "width=1000,height=650,scrollbars=yes,scrolling=no,resizebled=no"
//        );
        window.print();
    }
    </script>
    <style>
	.field_foto {text-align:right}
	.field_foto legend{font-weight:bold;}
	.img_add_foto{cursor:pointer}
	.hidden{display:none}
	.img_foto{border:0;margin:5px;cursor:pointer;margin-top:-5px}
	.div_foto{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
	.fechar{position:relative;margin-left:104px;top:-6px;cursor:pointer}

	.tabela{
		width: 95%
	}
	#calendarDiv{
		z-index: 100000000000 !important;
	}
    </style>
    <body>
    <?php
    if(!empty($_GET['tslid'][0]) && empty($slcid)) {
        $sql = "
                    SELECT
                        *,
                        slc.slcjustificativa,
                        to_char(slc.slcdatainclusao, 'DD/MM/YYYY HH24:MI:SS'),
                        us.usunome
                    FROM  obras2.solicitacao slc
                    inner join seguranca.usuario us on us.usucpf = slc.usucpf
                    inner join  obras2.tiposolicitacao_solicitacao ts on slc.slcid = ts.slcid
                    left join workflow.documento d ON d.docid = slc.docid
                    left join workflow.estadodocumento e ON e.esdid = d.esdid
                    WHERE slc.slcstatus = 'A'
                    and slc.obrid = $obrid
                    and e.esdid != " . ESDID_SOLICITACOES_DEFERIDO . "
                    and e.esdid != " . ESDID_SOLICITACOES_INDEFERIDO . "
                    and e.esdid != " . ESDID_SOLICITACOES_CANCELADO . "
                    and slc.aprovado != 't'
                    and ts.tslid = " . $_GET['tslid'][0] . "
                ";
        $dados = $db->carregar($sql);
        if (!empty($dados)) {
            $bloquear_insercao = true;
        }
    }
    else{
        $bloquear_insercao = false;
    }
    ?>
        <form id="formulario" name="formulario" method=post enctype="multipart/form-data" >
            <input type="hidden" name="requisicao" id="requisicao" value="" />
            <input type="hidden" name="habilita" id="habilita" value="<?=$habilita; ?>" />
            <input type="hidden" name="btAcao" id="btAcao" value="" />
            <input type="hidden" name="slcid" id="slcid" value="<?=$slcid; ?>" />
            <input type="hidden" name="arqid" id="arqid" value="" />
            <?php if($bloquear_insercao && empty($slcid)){
                $bloquearForm = true;
                if($bloquearForm == true){
                    $bloqueia = "N";
                }else{
                    $bloqueia = "S";
                }
            ?>
                <table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
                    <tbody>
                    <tr>
                        <td width="100%" align="center">
                            <label style="color:#E00606;font-size: 18px">
                              <img src="../imagens/atencao.png">  Não é possível fazer uma nova solicitação pois já existe uma em andamento.
                            </label>
                        </td>
                    </tr>
                    </tbody>
                </table>

            <?php }else{$bloquearForm = false;}?>

            <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
                <tr>
                    <td>
                        <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
                            <tr>
                                <td class="SubTituloDireita" width="20%">Tipo Solicitação:</td>
                                <td>
                                    <?php
                                    $sql = "SELECT tslid, tslnome FROM obras2.tiposolicitacao WHERE tslstatus = 'A'";
                                    $data = $db->carregar($sql);

                                    if(isset($_REQUEST['slcid'])){
                                        $sql = "SELECT tslid FROM obras2.tiposolicitacao_solicitacao WHERE slcid = " . $_REQUEST['slcid'];
                                        $_GET['tslid'] = $db->carregarColuna($sql);
                                    }
                                    if(!$_GET['tslid'] || !$_GET['tslid'][0]) {
                                        echo '<script>alert(\'Tipo de solicitação não selecionado.\');window.close();</script>';
                                    }
                                    foreach($data as $row){
                                        $checked = (in_array($row['tslid'], (array)$_GET['tslid'])) ? 'checked' : '';
                                        echo '<input disabled type="checkbox" '.$checked.' name="tslid[]" value="'.$row['tslid'].'"> '.$row['tslnome'].' <br />';

                                        if($checked)
                                            echo '<input type="hidden" name="tslid[]" value="'.$row['tslid'].'"/>';
                                    }
                                    ?>

                                </td>
                            </tr>
                            <?php if (in_array(11, $_REQUEST['tslid'])):  ?>
                            <tr>
                                <td class="SubTituloDireita">Solicitar/Alterar:</td>
                                <td>
                                    <label><input type="radio" name="slcsolicitaralterar"
                                                  value="FO" <?=('FO' == $arrPedido['slcsolicitaralterar'])?'checked':''; ?> /> Fiscal de Obra</label><br />
                                    <label><input type="radio" name="slcsolicitaralterar"
                                                  value="GU" <?=('GU' == $arrPedido['slcsolicitaralterar'])?'checked':''; ?> /> Gestor Unidade</label><br />
                                    <label><input type="radio" name="slcsolicitaralterar"
                                                  value="CU" <?=('CU' == $arrPedido['slcsolicitaralterar'])?'checked':''; ?> /> Consulta Unidade</label><br />
                                    <label><input type="radio" name="slcsolicitaralterar"
                                                  value="N" <?=('N' == $arrPedido['slcsolicitaralterar'])?'checked':''; ?> /> Não</label>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Problemas com senha?</td>
                                <td>
                                    <label><input type="radio" name="slcsenha"
                                                  value="S" <?=('S' == $arrPedido['slcsenha'])?'checked':''; ?> /> Sim</label><br />
                                    <label><input type="radio" name="slcsenha"
                                                  value="N" <?=('N' == $arrPedido['slcsenha'])?'checked':''; ?> /> Não</label>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Alteração de dados cadastrais?</td>
                                <td>
                                    <label><input type="radio" name="slcalteracao"
                                                  value="S" <?=('S' == $arrPedido['slcalteracao'])?'checked':''; ?> /> Sim</label><br />
                                    <label><input type="radio" name="slcalteracao"
                                                  value="N" <?=('N' == $arrPedido['slcalteracao'])?'checked':''; ?> /> Não</label><br />
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita">Outros:</td>
                                <td>
                                    <label><input type="radio" name="slcoutros"
                                                  value="S" <?=('S' == $arrPedido['slcoutros'])?'checked':''; ?> /> Sim</label><br />
                                    <label><input type="radio" name="slcoutros"
                                                  value="N" <?=('N' == $arrPedido['slcoutros'])?'checked':''; ?> /> Não</label>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"><label>CPF do usuário:</label></td>
                                <td>
                                    <?=campo_texto('slccpf', 'S', $habilita, '', 70, 100, '###.###.###-##', '', '', '', '', 'id="slccpf"', '') ?>
                                </td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita"><label>Nome do usuário:</label></td>
                                <td>
                                    <?=campo_texto('slcnome', 'S', $habilita, '', 70, 100, '', '', '', '', '', 'id="slcnome"', ''); ?>
                                </td>
                            </tr>
                            <?php endif; ?>
                            <?php
                            if (in_array(Solicitacao::TSLID_CORRECAO_INCONSISTENCIA_NO_SISTEMA, $_REQUEST['tslid'])):
                                $tipoCorrecao = [
                                    'Solicitar correção/inserção de números de processos' => 1,
                                    'Solicitar correção de ID?S (ID?s divergentes entre PAR e Obras)' => 2,
                                    'Solicitar correção/inserção de números de Termos de compromissos.' => 3,
                                    'Solicitar correção de tipologia.' => 4,
                                    'Solicitar correção de Endereço.' => 5,
                                    'Solicitar correção do Valor Previsto (valor pactuado).' => 6,
                                    'Solicitar correção ente divergência do percentual informado aba vistoria e lista de obras.' => 7,
                                    'Solicitar correção de vigência do termo no obras 2.0' => 8,
                                    'Solicitar correção divergência entre nome da empresa contratada e o apresentado pelo sistema (atualizar CNPJ)' => 9,
                                    'Solicitar correção: Vistorias validadas e sistema não atualizou.' => 10,
                                    'Solicitar correção: Ao inserir vistoria as fotos não estão carregando.' => 11,
                                    'Solicitar correção: documentos não estão carregando.' => 12,
                                    'Solicitar correção Serviços externos zerados (MI)' => 13,
                                    'Outros Erros' => 14,
                                ];
                                ?>
                            <tr>
                                <td class="SubTituloDireita"><label>Tipo de correção:</label></td>
                                <td>
                                    <?php
                                    foreach ($tipoCorrecao as $desc => $valor) {
                                        $checked = '';
                                        if ($arrPedido['tipoerro'] == $valor) {
                                            $checked = ' checked="checked"';
                                        }

                                        echo <<<HTML
                                    <label style="font-weight:normal">
                                        <input type="radio" name="tipoerro" id="tipoerro_{$valor}" value="{$valor}"{$checked} /> {$desc}
                                    </label><br />
HTML;
                                    }
                                    ?>
                                </td>
                            </tr>
                            <?php endif ?>
                            <tr>
                                <td class="SubTituloDireita" width="20%">Justificativa:</td>
                                <td><?=campo_textarea('slcjustificativa', 'S', "$bloqueia", 'Justificativa', 98, 5, 4000, '', '', '', '', 'Justificativa');?></td>
                            </tr>
                            <tr>
                                <td class="SubTituloDireita" width="20%">Observação:</td>
                                <td><?=campo_textarea('slcobservacao', 'N', "$bloqueia", 'Observação', 98, 5, 4000, '', '', '', '', 'Observação');?></td>
                            </tr>
                            <? if($bloquearForm != true){ ?>
                            <tr>
                                <td class="SubTituloDireita" width="20%">Anexos:</td>
                                <td valign="top">
                                    <img class="img_add_foto" src="../imagens/gif_inclui.gif" title="Adicionar Anexo" onclick="addAnexos()"  /><b><span onclick="addAnexos()" class="img_add_foto" style="cursor: pointer;"> Inserir Anexos</span></b><br>
                                    <table class="listagem" cellSpacing="1" width="80%" cellPadding="3" align="left" id="tabelaAnexo">
                                        <tr>
                                            <th>Ação</th>
                                            <th>Arquivo</th>
                                            <th>Descrição</th>
                                        </tr>
                                        <?php
                                        if( $slcid ){
                                            $sql = "SELECT
                                        a.axsid,
                                        a.arqid,
                                        ar.arqnome||'.'||ar.arqextensao as arquivo,
                                        a.axsdsc as descricao
                                    FROM
                                        obras2.anexosolicitacao a
                                        inner join public.arquivo ar on ar.arqid = a.arqid
                                    WHERE
                                        a.slcid = $slcid
                                        and a.axstipo = 'PD'";
                                            $arrArquivo = $db->carregar($sql);
                                            $arrArquivo = $arrArquivo ? $arrArquivo : array();

                                            $count = 1;
                                            foreach ($arrArquivo as $v) { ?>
                                                <tr>
                                                    <td>
                                                        <?if( $habilita == 'N' ){ ?>
                                                            <img src="../imagens/alterar_01.gif" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
                                                            <img src="../imagens/excluir_01.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
                                                        <?} else { ?>
                                                            <img src="../imagens/alterar.gif" onclick="addAnexos(<?php echo $v['arqid'];?>, <?=$slcid ?>)" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
                                                            <img src="../imagens/excluir.gif" onClick="excluirAnexo('<?php echo $v['arqid']; ?>', <?=$slcid ?>, <?=$obrid ?>);" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
                                                        <?} ?>
                                                    </td>
                                                    <td><?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/solicitacao&acao=A&download=S&arqid=<?php echo $v['arqid'];?>&tipo=D'"><?php echo $v['arquivo'];?></a></td>
                                                    <td><?=$v['descricao'] ?></td>
                                                </tr>
                                                <?
                                                $count++;
                                            }
                                        }
                                        ?>
                                    </table>
                                </td>
                                <?}?>
                            </tr>
                            <tr bgcolor="#DEDEDE">
                                <td colspan="2" align="center">
                                    <?php if((!$bloquear_insercao || possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) && ($habilita == 'S')){?>
                                        <input name="btn_salvar" value="Salvar Pedido" type="button" onclick="salvarPedido();">
                                    <?php } ?>
                                    <button name="btn_fechar" type="button" onclick="window.close()">Fechar</button>
                                    <button type="button" id="print_quest" onclick="printQuestionario()"> Imprimir </button>
                                </td>
                            </tr>
                        </table>
                    </td>
                    <td>
                        <?php
                        // Barra de estado WORKFLOW
                        if ($docid && ($mostraWorkflow || $habilitaSuper === 'S')):
                            wf_desenhaBarraNavegacao($docid, [
                                'obrid' => $obrid,
                                'slcid' => $slcid,
                                'tslid' => $tslidoriginal
                            ]);
                        endif;
                        ?>
                    </td>
                </tr>
            </table>
            <?php
            $html = '
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td width="30%" class="SubtituloDireita" >Anexo:</td>
                        <td id="td_insere"><input type="file" name="arquivo" /></td>
                        <td id="td_altera"></td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" >Descrição:</td>
                        <td>'.campo_texto("arqdescricao","S","S","",40,60,"","","","","","","").'</td>
                    </tr>
                    <tr>
                        <td class="SubtituloDireita" ></td>
                        <td class="SubtituloEsquerda" >
                            <input type="button" name="btn_upload" value="Salvar" />
                            <input type="button" name="btn_cancelar" value="Cancelar" />
                        </td>
                    </tr>
                </table>';
                popupAlertaGeral($html,"400px","205px","div_sub_foto","hidden");
            ?>
        </form>
        <?php
        monta_titulo( "Lista de Solicitações $tipo", "" );
        $tslid = $_GET['tslid'] ? $_GET['tslid'][0] : '';
        $imagem = "'<img src=\"../imagens/alterar.gif\" onclick=\"alterarPedido('||slc.slcid||', '||slc.obrid||', ".$tslid.")\" style=\"border:0; cursor:pointer;\" title=\"Alterar Pedido Desbloqueio\">&nbsp;'";

        $where = array();

        if(isset($_GET['tslid'][0]) && !empty($_GET['tslid'][0])){
            $where['tslid'] = "slc.slcid IN (SELECT slcid FROM obras2.tiposolicitacao_solicitacao WHERE tslid IN ({$_GET['tslid'][0]}) AND slcid = slc.slcid)";
        }
        $situacao = 'e.esddsc';
        if(!possui_perfil(array(PFLCOD_SUPER_USUARIO,PFLCOD_GESTOR_MEC))){
            $situacao = "CASE WHEN slc.aprovado = 'N' THEN (CASE WHEN e.esdid = ".ESDID_SOLICITACOES_CADASTRAMENTO." THEN '<b>Em Cadastramento</b>' WHEN e.esdid = ".ESDID_SOLICITACOES_DILIGENCIA." THEN '<b>Diligência</b>' ELSE '<b>Aguardando Análise</b>' END) ELSE '<b style=\"color:green;\">'||e.esddsc||'</b>' END AS esddsc";
        }

        $sql = "
            SELECT
                $imagem as acoes,
                (SELECT array_to_string(array_agg(tslnome), ',') FROM obras2.tiposolicitacao_solicitacao ts
                    JOIN obras2.tiposolicitacao t ON t.tslid = ts.tslid
                    WHERE ts.slcid = slc.slcid),
                $situacao,
                slc.slcjustificativa,
                to_char(slc.slcdatainclusao, 'DD/MM/YYYY HH24:MI:SS'),
                us.usunome
            FROM  obras2.solicitacao slc
            inner join seguranca.usuario us on us.usucpf = slc.usucpf
            left join workflow.documento d ON d.docid = slc.docid
            left join workflow.estadodocumento e ON e.esdid = d.esdid
            WHERE slc.slcstatus = 'A'
                and slc.obrid = $obrid
                " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "";
        $cabecalho = array('Ações', 'Tipo Solicitação', 'Situação', 'Justificativa', 'Data', 'Usuário');
        $arrHeighTds = array('05%', '15%','15%', '60%', '20%');
        $db->monta_lista($sql, $cabecalho, 1000000, 5, 'N', 'center', '', '', $arrHeighTds);
        ?>

    </body>
</html>