<?php 
$perfis = pegaPerfilGeral($_SESSION['usucpf']);

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "par3/classes/model/seguranca/Usuario.class.inc";


$prgid = $_REQUEST['prgid'];
$inuid = $_REQUEST['inuid'];
$pfaid = $proadesaoDados['pfaid'];
// Instancia objetos
$objOrientacao 				= new Par3_Model_Orientacoesprograma();
$objUsuario 				= new Seguranca_Model_Seguranca_Usuario();
$objRespostaCargaHoraria	= new Par3_Model_Respostacargahorariaprograma();
$objRespostaIntegracao 		= new Par3_Model_Respostaintegracaocurricularproemi();
$objPerguntaIntegracao 		= new Par3_Model_Perguntaintegracaocurricularproemi();
$objAdmEscolas 				= new Par3_Model_Adesaoescolaproemi();
$objEscolasProemi			= new Par3_Model_Escolasproemi2015();
$objExcecao 				= new Par3_Controller_Excecaoescolasprograma();
							   

$liberaEnvio = false;
// Verifica se existem escolas no grupo 1 e se existem adesao
$arr['inuid'] = $inuid;
$arr['grupo'] = '1';
$existeEscolaNoGrupo1 =  $objEscolasProemi->existeEscolaNoGrupo($arr);

$arr['inuid'] = $inuid;
$arr['grupo'] = '2';
$existeEscolaNoGrupo2 =  $objEscolasProemi->existeEscolaNoGrupo($arr);

$restricaoMinimaGrupo1 = false;
$restricaoMinimaGrupo2 = false;
if($existeEscolaNoGrupo1)
{
	$existeAdesao1 = $objAdmEscolas->existeEscolas($inuid, '1');
	if( ! $existeAdesao1 )
	{
		$restricaoMinimaGrupo1 = true;
	}
	
}

if($existeEscolaNoGrupo2)
{
	$existeAdesao2 = $objAdmEscolas->existeEscolas($inuid, '2');
	if( ! $existeAdesao2 )
	{
		$restricaoMinimaGrupo2 = true;
	}
}


if (($aceite['adpresposta'] == "S") && ( $objRespostaIntegracao->existeCampos($inuid) ) && ( $objAdmEscolas->existeEscolas($inuid)) && (!$restricaoMinimaGrupo1) && (!$restricaoMinimaGrupo2) )
{
	$liberaEnvio = true;
}
$arrRestricoes = array();

if( $aceite['adpresposta'] != 'S')
{
	$arrRestricoes[]['dsc']= '<span style="color:red" > Termo não aceito </span>';
}
if( ! $objRespostaIntegracao->existeCampos($inuid) )
{
	$arrRestricoes[]['dsc'] = '<span style="color:red"> Preenchimento incorreto na aba "Integração Curricular" </span>';
}
if( ! $objAdmEscolas->existeEscolas($inuid) )
{
	$arrRestricoes[]['dsc'] = '<span style="color:red"> Nenhuma escola selecionada </span>';
}
if(  $restricaoMinimaGrupo1 )
{
	$arrRestricoes[]['dsc'] = '<span style="color:red"> Selecione no mínimo uma escola do Grupo 1 </span>';
}
if(  $restricaoMinimaGrupo2 )
{
	$arrRestricoes[]['dsc'] = '<span style="color:red"> Selecione no mínimo uma escola do Grupo 2 </span>';
}


$arrParamResposta['inuid'] = $inuid;
$arrParamResposta['prgid'] = $prgid;
$arrRespostaCargaHoraria = $objRespostaCargaHoraria->getResposta($arrParamResposta);

//Sobre quantidade escolas já salvas
$dadosSobreEscola['inuid'] = $inuid;
$dadosSobreEscola['grupo'] = '1';
$totalEscolasGrupo1 = $objAdmEscolas->retornaQtdSalvoGrupo($dadosSobreEscola);
$dadosSobreEscola['grupo'] = '2';
$totalEscolasGrupo2 = $objAdmEscolas->retornaQtdSalvoGrupo($dadosSobreEscola);
$dadosSobreEscola['grupo'] = '3';
$totalEscolasGrupo3 = $objAdmEscolas->retornaQtdSalvoGrupo($dadosSobreEscola);

$result = $modelAdesaoPrograma->recuperaPorInuid($inuid, $pfaid);

if (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) 
) {
	$insereDoc = 1;
} else {
	$insereDoc = 0;
}

if( (! $result) && ($insereDoc) )
{
	//insere
	if (!empty($aceite['adpid'])) {
		$dadosRegistroDoc['adpid'] = $aceite['adpid'];
	}

	$docidInserido = wf_cadastrarDocumento(WF_TPDID_PROEMI, 'Documento de Adesão', WF_ESDID_NAO_INICIADO_PROEMI);

	if($docidInserido)
	{
		$dadosInicio['adpano'] = date('Y');
		$dadosInicio['inuid']  = $inuid;
		$dadosInicio['pfaid']  = $pfaid;
		$dadosInicio['docid']  = $docidInserido;
		$dadosInicio['usucpf'] = $_SESSION['usucpf'];
		$dadosInicio['tapid']  = 1;
	}
	$modelAdesaoPrograma->popularDadosObjeto($dadosInicio);
	$modelAdesaoPrograma->salvar();
	$modelAdesaoPrograma->commit();
}
else
{
	$docidInserido = $result['docid'];
}

if( (! $docidInserido) && ($insereDoc) )
{
	?>
	Faltam dados. <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
	<?php
	die("");
}

if ($itrid == 2)
{
	$proadesaoDados['pfatermomunicipio'] =  str_replace('\"','"', $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#dirigente#',$objPessoaFisica->entnome, $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#dirigente_cpf#',$objPessoaFisica->entcpf, $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#nome_entidade#', $inunome, $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#data_atual#',date('d/m/Y'), $proadesaoDados['pfatermomunicipio']);

	$termoPreenchido = $proadesaoDados['pfatermomunicipio'];
}
else
{
	$proadesaoDados['pfatermoestado'] =  str_replace('\"','"', $proadesaoDados['pfatermoestado']);
	$proadesaoDados['pfatermoestado'] =  str_replace('#dirigente#',$objPessoaFisica->entnome, $proadesaoDados['pfatermoestado']);
	$proadesaoDados['pfatermoestado'] =  str_replace('#dirigente_cpf#',$objPessoaFisica->entcpf, $proadesaoDados['pfatermoestado']);
	$proadesaoDados['pfatermoestado'] =  str_replace('#nome_entidade#',$inunome, $proadesaoDados['pfatermoestado']);
	$proadesaoDados['pfatermoestado'] =  str_replace('#data_atual#',date('d/m/Y'), $proadesaoDados['pfatermoestado']);

	$termoPreenchido = $proadesaoDados['pfatermoestado'];
}


switch ($_REQUEST['requisicao']) {
	
	case 'salvar_escolas_excecao':
		
		$insercao = $objExcecao->salvaFormulario($_REQUEST);
		if($insercao)
		{
			$msgAlert = "Solicitação para inserção de escolas enviada com sucesso";
			simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=2", 'success', $msgAlert);
		}
		else
		{
			$msgAlert = "Erro ao enviar solicitação para inserção de escolas";
			simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=2", 'error', $msgAlert);
		}
		
	break;
	
	case 'retorna_dados_escola':
		ob_clean();
		
		$arrParam['inuid'] = $_REQUEST['inuid'];
		$arrParam['cod_inep'] = $_REQUEST['cod_inep'];
		$arrParam['prgid'] = $_REQUEST['prgid'];
		
		$retorno = $objEscolasProemi->retornaInfoEscola($arrParam);
		
		if( count($retorno) > 0 )
		{
			$dadosEscola = array(array('nome' => $retorno['nome'], 'resposta_carga_horaria' => $retorno['resposta_carga_horaria'], 'grupo' => $retorno['grupo'], 'status' => 'success'));
			echo simec_json_encode($dadosEscola);
			die();
		}
		else
		{
			$dadosEscola = array(array('status' => 'error'));
			echo simec_json_encode($dadosEscola);
			die();
		}
	break;
	case 'aceita_solicitacao_excecao':
		ob_clean();
		
		foreach($_REQUEST as $k => $v)
		{
			$_REQUEST[$k] = ($v);
		}
		
		$resultado = $objExcecao->alteraStatusSolicitacaoExcecao($_REQUEST);
		
		if($resultado)
		{
			$dadosEscola = array(array('status' => 'success'));
			echo simec_json_encode($dadosEscola);
			die();
		}
		else
		{
			$dadosEscola = array(array('status' => 'error'));
			echo simec_json_encode($dadosEscola);
			die();	
		}
		
	break;
	
	case 'pdf_termo':
					 	
		ob_end_clean();
		
		$mensagemAssinatura = "";
		if($aceite['adpresposta'] == "S")
		{
			$nomeA = $objUsuario->getNome($aceite['usucpf']);
			$cpfA = formatar_cpf($aceite['usucpf']);
			$horaA = formata_data_hora($aceite['adpdataresposta']);
				
			$mensagemAssinatura = '<span style="color:green; font-size:10px;">' . "Assinado por {$nomeA} CPF {$cpfA}. No dia {$horaA} </span> ";
		}
		
		$valor = $termoPreenchido;
		$conteudo = str_replace( "\\", " ", $valor);
		$conteudo = str_replace( "quando em missão de", "quando em missão de<br><br><br>", $conteudo);
	
		$nome = $objUsuario->getNome($_SESSION['usucpf']);
		
		$rodape = '<span style="color:red; font-size:10px;"> Impresso por ' . $nome . ' CPF nº ' . formatar_cpf($_SESSION['usucpf']) .' No dia ' .  date('d/m/Y') . ' as '.  date('H:i:s') .' dentro do módulo de adesões do SIMEC</span>';
		 	
		html2Pdf("
	
				<html>
				  <head>
				    <style>
				      
				      .footer { position: fixed; bottom: 0px; }
				    </style>
				  </head>
				  <body>
				    {$conteudo}
				    <div class=\"footer\"> <br><br>
				    {$mensagemAssinatura}<br>{$rodape}
				    </div>
				  </body>
				</html>",
				   "termoAdesao");
	
		break;
	case 'pdf_sintese':
		
		ob_end_clean();
		$valor = $_REQUEST['conteudo_pdf'];
		$conteudo = str_replace( "\\", " ", $valor);
		
		
		html2Pdf("
				
		<style>
				.table-bordered > thead > tr > th, .table-bordered > tbody > tr > th, .table-bordered > tfoot > tr > th, .table-bordered > thead > tr > td, .table-bordered > tbody > tr > td, .table-bordered > tfoot > tr > td {
		    border: 1px solid #e7e7e7;
		}
		.table-bordered > thead > tr > th, .table-bordered > thead > tr > td {
		    background-color: #f5f5f6;
		    border-bottom-width: 1px;
		}
		.table-bordered > thead > tr > th, .table-bordered > thead > tr > td {
		    border-bottom-width: 2px;
		}
		.table-bordered > thead > tr > th, .table-bordered > tbody > tr > th, .table-bordered > tfoot > tr > th, .table-bordered > thead > tr > td, .table-bordered > tbody > tr > td, .table-bordered > tfoot > tr > td {
		    border: 1px solid #ddd;
		}
		.table-condensed > thead > tr > th, .table-condensed > tbody > tr > th, .table-condensed > tfoot > tr > th, .table-condensed > thead > tr > td, .table-condensed > tbody > tr > td, .table-condensed > tfoot > tr > td {
		    padding: 5px;
		}
		.table > thead > tr > th {
		    border-bottom: 2px solid #ddd;
		    vertical-align: bottom;
		}
		.table > thead > tr > th, .table > tbody > tr > th, .table > tfoot > tr > th, .table > thead > tr > td, .table > tbody > tr > td, .table > tfoot > tr > td {
		    border-top: 1px solid #ddd;
		    line-height: 1.42857;
		    padding: 8px;
		    vertical-align: top;
		}
		td, th {
		    padding: 0;
		}
		th {
		    text-align: left;
		}
		td, th {
		    padding: 0;
		}
		* {
		    box-sizing: border-box;
		}
		*::-moz-placeholder {
		    color: #d1d1d1;
		}
		*::-moz-placeholder {
		    color: #d1d1d1;
		}
		.alert-info {
		    background-color: #d9edf7;
		    border-color: #bce8f1;
		    color: #31708f;
		}
		.alert {
		    border: 1px solid transparent;
		    border-radius: 4px;
		    margin-bottom: 20px;
		    padding: 15px;
		}
		h3, h4, h5 {
		    font-weight: 600;
		    margin-top: 5px;
		}
		h3 {
		    font-size: 16px;
		}
		h1, h2, h3, h4, h5, h6 {
		    font-weight: 100;
		}
		h3 {
		    font-size: 2.92rem;
		    line-height: 110%;
		    margin: 1.46rem 0 1.168rem;
		}
		.table {
		    margin-bottom: 20px;
		    max-width: 100%;
		    width: 100%;
		}
		.table-bordered {
		    border: 1px solid #ebebeb;
		}
		table {
		    border-collapse: collapse;
		    border-spacing: 0;
		}
		table {
		    background-color: transparent;
		}
		</style>
		
		<div id=\"conteudo_sintese\">
	     		{$conteudo}
				</div>");
		
		
		
		break;
	case 'salvar_orientacao':

		$orpidSave		= $_REQUEST[orpid];
		$orptextoSave	= $_REQUEST[orptexto];
		
		switch ($orpidSave) {
			case 1:
				$abaAtual = '2';
				break;
			case 2:
				$abaAtual = '3';
				break;
		}
		
		if( $orpidSave && $orptextoSave )
		{

			$dadosOrientacao['orpid'] 		= $orpidSave;
			$dadosOrientacao['orptexto']    = $orptextoSave;

			$objOrientacao->popularDadosObjeto($dadosOrientacao);
			$objOrientacao->salvar();
			if( $objOrientacao->commit())
			{
				$nomeAba = $objOrientacao->getOrpDescricaoByID($orpidSave);
				$msgAlert = $nomeAba . " salvas com sucesso";
				simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual={$abaAtual}", 'success', $msgAlert);
				die();
			}

				
		}

		break;
	case 'cancelar_respostas':

		$result = $objRespostaIntegracao->retornaOriginal($_REQUEST);
		
		if($result == true )
		{
			$msgAlert = "Modificações Salvas Com sucesso";
			$retorno = 'success';
		}
		else
		{
			$msgAlert = "Erro ao modificar as questões da Integração Curricular";
			$retorno = 'error';
		}
		simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=3", $retorno, $msgAlert);
		die();
		break;
	case 'salvar_integracao_curricular':

		$result = $objRespostaIntegracao->salvaModificacao($_REQUEST);
		
		if($result == true )
		{
			if( $docidInserido )
			{
				if( ($esdid == WF_ESDID_NAO_INICIADO_PROEMI) || (! $esdid) )
				{
					wf_alterarEstado( $docidInserido, WF_AEDID_PROEMI_NAOINICIADO_CADASTRAMENTO, "Aceitou o termo", array(), array());
				}
			}
			$msgAlert = "Modificações Salvas Com sucesso";
			$retorno = 'success';
		}
		else
		{
			$msgAlert = "Erro ao modificar as questões da Integração Curricular";
			$retorno = 'error';
		}
		simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=3", $retorno, $msgAlert);
		die();
		
		break;
	case 'salvar_vinculo_escola':

		$resultado = $objAdmEscolas->salvaFormularioEscolas($_REQUEST);
		
		
		simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=2", $resultado['retorno'], $resultado['mensagem']);
		die();

		break;
	case 'salva_resposta_carga_horaria':
/*
		ob_clean();
		$rchid			= $_REQUEST['rchid'];
		$rchresposta 	= $_REQUEST['rchresposta'];
		$rchidJaSalvo = $arrRespostaCargaHoraria['rchid'];
		
		
		if( $rchidJaSalvo != '')
		{
			$dadosResposta['rchid'] = $rchidJaSalvo;
		}
		if ($rchid != '0' )
		{
			$dadosResposta['rchid'] = $rchid;
		}
		
		$dadosResposta['rchresposta'] = $rchresposta;
		$dadosResposta['inuid'] = $inuid;
		$dadosResposta['prgid'] = $prgid;
		$dadosResposta['rchcpf'] = $_SESSION['usucpf'];
		
		$objRespostaCargaHoraria->popularDadosObjeto($dadosResposta);
		$objRespostaCargaHoraria->salvar();
		
		if($objRespostaCargaHoraria->commit())
		{
			if( $docidInserido )
			{
				if( ($esdid == WF_ESDID_NAO_INICIADO_PROEMI) || (! $esdid) )
				{
					wf_alterarEstado( $docidInserido, WF_AEDID_PROEMI_NAOINICIADO_CADASTRAMENTO, "Aceitou o termo", array(), array());
				}
			}
			if( $rchresposta == 'N' )
			{
				$objAdmEscolas->apagaCargaHoraria($inuid);
			}
			
			$arr = array(array('resultado' => 'ok'));
			
		}
		else
		{
			$arr = array(array('resultado' => 'error'));
		}
		
		echo simec_json_encode($arr);
		die();*/
		break;
	default:
		break;
}



$resposta = $aceite['adpresposta'];
$editavel = false;
if( ($resposta == "C") || ($resposta == "S") ||  ($resposta == "")) {
	$editavel = true;
}


$situacaoAtual = wf_pegarEstadoAtual( $docidInserido);
$esdid = $situacaoAtual['esdid'];

$bloqueiaGeral = FALSE;
$vencido = false;

if( (( $esdid == WF_ESDID_TERMONAOACEITO_PROEMI) || ($esdid == WF_ESDID_ENVIADOPARAOMEC_PROEMI)) || (strtotime($proadesaoDados['pfadatafinal']. ' 12:00:00') < strtotime(date('Y-m-d G:i:s'))) )
{
	$bloqueiaGeral = TRUE;
	if((strtotime($proadesaoDados['pfadatafinal']. ' 12:00:00') < strtotime(date('Y-m-d G:i:s'))))
	{
		$bloqueiaValidade = TRUE;
		$vencido = true;
	}
}


if (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)) {
	$mostrar = 1;
} else {
	$mostrar = 0;
}

if ($itrid == 2) {//municipal
	$where[] = 't1.tp_dependencia = 3';
	$where[] = "t1.estuf = '{$modelInstrumentoUnidade->estuf}'";
	$where[] = "t1.muncod = '{$modelInstrumentoUnidade->muncod}'";
} else {//estadual
	$where[] = 't1.tp_dependencia = 2';
	$where[] = "t1.estuf = '{$modelInstrumentoUnidade->estuf}'";
}


if ($_GET['aceite'] == 'E')
{
	if( $esdid == WF_ESDID_EMCADASTRAMENTO_PROEMI )
	{
		$msgAlert = "Adesão da Secretaria de Educação ao Programa Ensino Médio Inovador - PROEMI realizada com sucesso. Por favor, informe às escolas selecionadas sobre a necessidade de preencherem o formulário de adesão no Sistema PDDE Interativo para participação no programa.";
		wf_alterarEstado( $docidInserido, WF_AEDID_PROEMI_CADASTRAMENTO_ENVIADOPARAMEC, "Enviou para o MEC", array(), array());
		simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=4", 'success', $msgAlert);
	}

}

if ($_GET['aceite'] == 'C')
{
	if($aceite['adpid'])
	{
		$dadosInicio['adpano'] 				= date('Y');
		$dadosInicio['adpdataresposta']  	= 'now()';
		$dadosInicio['adpresposta'] 		= 'C';
		$dadosInicio['usucpf'] 				= $_SESSION['usucpf'];
		$dadosInicio['tapid']  				= 1;
		$dadosInicio['adpid'] 				= $aceite['adpid'];
		
		$modelAdesaoPrograma->popularDadosObjeto($dadosInicio);
		$modelAdesaoPrograma->salvar();
		
		if( $modelAdesaoPrograma->commit() )
		{
			$msgAlert = "Programa 'PROEMI' retornado para 'Em cadastramento'";
			wf_alterarEstado( $docidInserido, WF_AEDID_PROEMI_TERMONAOACEITO_EMCADASTRAMENTO, "Cancelou resposta de 'Não aceito' ", array(), array());
			simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid, 'success', $msgAlert);//die('erro 1');
		}
	}
}

if ($_GET['aceite'] == 'S' || $_GET['aceite'] == 'N')
{
	
    $dados['inuid'] = $_GET['inuid'];
    $dados['prgid'] = $_GET['prgid'];
    $dados["pfaid"] = $pfaid;

    if (empty($pfaid) && !empty($_GET['pfaid'])) {
        $dados['pfaid'] = $_GET['pfaid'];
    }

    $dados['adpresposta'] = $_GET['aceite'];
    $dados['tapid'] = 1;

    if (empty($proadesaoDados['pfaano'])) {
        $dados['adpano']= date('Y');
    } else {
        $dados['adpano']= $proadesaoDados['pfaano'];
    }

    $dados['usucpf'] = $_SESSION['usucpf'];
    $dados['adpdataresposta'] = "'now()'";

    if ($_GET['aceite'] == 'S') {
        $dados['padtermopreenchido'] = $termoPreenchido;
    }

    if (!empty($aceite['adpid'])) {
        $dados['adpid'] = $aceite['adpid'];
    }
	
    $modelAdesaoPrograma->popularDadosObjeto($dados);
    $modelAdesaoPrograma->salvar();
    $modelAdesaoPrograma->commit();

    if ( $_GET['aceite'] == 'S') 
    {
    	
    	if($docidInserido )
    	{
    		if( ($esdid == WF_ESDID_NAO_INICIADO_PROEMI) || (! $esdid) )
    		{
    			wf_alterarEstado( $docidInserido, WF_AEDID_PROEMI_NAOINICIADO_CADASTRAMENTO, "Aceitou o termo", array(), array());
    		}
    	}
    	
    	$msgAlert = $proadesaoDados['pfamsgdeaceitacao'];
    } 
    else 
    {
    	
    	$msgAlert = $proadesaoDados['pfamsgdenaoaceitacao'];
    }

    $_SESSION[$_POST['pppid']]['termoaceito'] = true;


    if ($_GET['aceite'] == 'N')
    {
    	//apaga dados escolas
    	$objAdmEscolas->deletaGeralPorUnidade($dados['inuid'], $dados['prgid']);
    	// Retorna valores originais dos campos de integração
    	$objRespostaIntegracao->retornaPadraoOriginal($dados['inuid']);
    	
    	
    	//@todo zera dados integracao curricular, caso tenham sido alterados
    	if( $docidInserido )
    	{
    		if($esdid == WF_ESDID_EMCADASTRAMENTO_PROEMI ){
    			wf_alterarEstado( $docidInserido, WF_AEDID_PROEMI_CADASTRAMENTO_TERMONAOACEITO, "Não Aceitou o termo", array(), array());
    		}	
    		else
    		{
    			wf_alterarEstado( $docidInserido, WF_AEDID_PROEMI_NAOINICIADO_NAOACEITO, "Não Aceitou o termo", array(), array());
    		}
    	}
    	
        simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$dados['inuid']."&prgid=".$dados['prgid'], 'success', $msgAlert);
    } else {
        simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$dados['inuid']."&prgid=".$dados['prgid'], 'success', $msgAlert);
    }
}
?>

<script>
$(document).ready(function(){
   
     $('#modal-form').on('hidden.bs.modal', function () {

		var esdid = $("#esdid_atual").val();
        if(( $('#grupoPopUp').val() != 'N') && ((esdid  != 1816) && (esdid != 1814)) )
        {
	     	if( ! confirm('Você está saindo da tela de vinculação de escolas, caso não tenha salvado, as informações preenchidas serão desconsideradas. Deseja continuar?'))
	     	{
	     		$('#modal-form').modal();
	    	}
        }
	});


 	$('#formulariointegracao').submit(function() {

 		if( $('#integracao_curricular_s').prop('checked') )
 		{

 			if( ($("#questao_integracao_2").val() == 2) && ($("#questao_integracao_3").val() == 3) && ($("#questao_integracao_4").val() == 4) )
 			{
				alert('Ao escolher a opção "Sim" é necessário substituir um campo de integração');
				return false;
 			}
 			else
 			{
				return true;
 			}  
 		}	
 		else
 		{
 			return true;
 		}
 		return false;
 	});
 	
 	$('.btn-xls').hide();
	$('.btn-query').hide();

	var  abaAtual = $("#aba_atual").val();

	if(abaAtual != '')
	{
		if(abaAtual == 2)
		{
			$("#tab-2").addClass( "active" );
			$("#li2").addClass( "active" );
		}else if(abaAtual == 3)
		{
			$("#tab-3").addClass( "active" );
			$("#li3").addClass( "active" );
		}else if(abaAtual == 4)
		{
			$("#tab-4").addClass( "active" );
			$("#li4").addClass( "active" );
		}
		$("#tab-1").removeClass( "active" );
		$("#li1").removeClass( "active" );
	}
	
});
function gerarPDFSintese( )
{

	$('#conteudo_pdf').val($("#conteudo_sintese").html());

	$("#formulario-pdf-sintese").submit();
	
}

function submeteFormEscolas()
{

	var submete = true;
	if( ( ! $('#carga_horaria_s').prop('checked'))  && ( ! $('#carga_horaria_n').prop('checked')) )
	{
		alert('Selecione a carga horária');
		submete = false;
	}
	
	if($("#formulario_escolas input:checkbox:checked").length  == 0)
	{
		alert('Selecione no mínimo uma escola.');
		submete = false;
	}
	
	if($('#carga_horaria_s').prop('checked'))
	{
		$('#carga_horaria_form').val('S');
		$("#formulario_escolas input:checkbox:checked").each(function() 
		{
			 codinep = $(this).val();
			 			 
			if($("#select_"+codinep).val() == '')
			{
				alert('O campo "Carga Horária" da escola '+ codinep +' tem preenchimento obrigatório');
				submete = false;
			} 
		});
	}	
	if(submete)
	{
		$("#formulario_escolas").submit();
	}
}


function filtraPesquisa( inuid, grupo, prgid, tipoPesquisa ){

	var continua = true;
	
	if( grupo != 'N' )
	{
		if(confirm('Você está alterando os filtros de pesquisa, caso não tenha salvado as informações preenchidas na tabela abaixo, elas serão desconsideradas. Deseja continuar?'))
		{
			var tooltip = retornaTooltipPorGrupo(grupo);
	 		var titulo = 'Vincular Escolas do Grupo '+grupo+' <div class="glyphicon glyphicon-question-sign" title="'+ tooltip +'" style="color:#5BC0DE;" aria-hidden="true"></div>';
		}
		else
		{
			continua = false;
		}
	}
	else
	{
		var titulo = 'Escolas Não Aptas';
	}
	if( continua )
	{
		 var estuf 			 = ($("#estuf").val()) ? $("#estuf").val() : '';
		 var epemunicipio 	 = ($("#epemunicipio").val()) ? $("#epemunicipio").val() : '';
		 var epecod 		 = ($("#epecod").val()) ? $("#epecod").val() : '';
		 var epenome 		 = ($("#epenome").val()) ? $("#epenome").val() : '';
		 var checado 		 = ($("#checado").val()) ? $("#checado").val() : '';
		 var aepcargahoraria = ($("#aepcargahoraria").val()) ? $("#aepcargahoraria").val() : '';
		 var ordenacao		 = ($('#ordenacao').val()) ? $('#ordenacao').val() : '';
	
		 if(tipoPesquisa == 'xls')
		 {
		 	link ='par3.php?modulo=principal/adesao/popupEscolasProemi&acao=A'+'&grupo='+ grupo + '&inuid='+inuid+ '&prgid='+prgid+'&estuf='+estuf+'&epemunicipio='+epemunicipio+'&epecod='+epecod+'&epenome='+epenome+'&tipopesquisa='+tipoPesquisa+'&aepcargahoraria='+aepcargahoraria+'&checado='+checado+'&ordenacao='+ordenacao;
		 	location.href = link ;
		 	return false;
		 }
		 else
		 {
		 
		    $.ajax({ 
		        type: "POST",
		        url: 'par3.php?modulo=principal/adesao/popupEscolasProemi&acao=A',
		        data: '&grupo='+ grupo + '&inuid='+inuid+ '&prgid='+prgid+'&estuf='+estuf+'&epemunicipio='+epemunicipio+'&epecod='+epecod+'&epenome='+epenome+'&tipopesquisa='+tipoPesquisa+'&aepcargahoraria='+aepcargahoraria+'&checado='+checado+'&ordenacao='+ordenacao, 
		        async: false,
		        success: function (resp) {
		            $('#html_modal-form').html(resp);
		            $('#html_modal-title').html(titulo);
		            $('#modal-form').modal();
		        }
		    });
		 }
	}
}

function editaIntegracaoCurricular(pergunta)
{
	document.formulariointegracao.reset();
	$("#questao_integracao_"+pergunta).removeAttr('disabled');
	$('#integracao_curricular_s').prop('checked', true);
	if(pergunta == "2")
	{
		$("#questao_integracao_3").attr('disabled','disabled');
		$("#questao_integracao_4").attr('disabled','disabled');
	}
	else if(pergunta == "3")
	{
		$("#questao_integracao_2").attr('disabled','disabled');
		$("#questao_integracao_4").attr('disabled','disabled');
	}
	if(pergunta == "4")
	{
		$("#questao_integracao_2").attr('disabled','disabled');
		$("#questao_integracao_3").attr('disabled','disabled');
	}
}

function confirmaCancelamentoIntegracao( inuid, prgid )
{
	if(confirm('Tem certeza que deseja cancelar as modificações realizadas na escolha dos Campos de Integração Curricular?'))
	{
		$("#requisicao_integracao").val('cancelar_respostas');
		document.formulariointegracao.submit();
	}	
}

function retornaTooltipPorGrupo(grupo)
{
	var title = new Array();
	title['1'] = 'Escolas que receberam recursos referente ao programa em 2014';
	title['2'] = 'Escolas com Indicador de Nível Socioeconômico baixo ou muito baixo que não se enquadram no 1º grupo';
	title['3'] = 'Demais escolas de ensino médio que poderão aderir ao programa';

	return title[grupo];
}

function insereExcecaoEscolas( inuid, prgid )
{

    $.ajax({
        type: "POST",
        url: 'par3.php?modulo=principal/adesao/popupIncluirEscolasExcecao&acao=A',
        data: '&inuid='+inuid+ '&prgid='+prgid, 
        async: false,
        success: function (resp) {
            $('#html_modal-form2').html(resp);
            $('#html_modal-title2').html('Solicitação de inclusão de escolas na adesão');
            $('#modal-form2').modal();
        }
    });
}

function popupEscolas( inuid, grupo, prgid )
{
	if( grupo != 'N' )
	{
		var tooltip = retornaTooltipPorGrupo(grupo);
	 	var titulo = 'Vincular Escolas do Grupo '+grupo+' <div class="glyphicon glyphicon-question-sign" title="'+ tooltip +'" style="color:#5BC0DE;" aria-hidden="true"></div>';
	}
	else
	{
		var titulo = 'Escolas Não Aptas';
	}

    $.ajax({
        type: "POST",
        url: 'par3.php?modulo=principal/adesao/popupEscolasProemi&acao=A',
        data: '&grupo='+ grupo + '&inuid='+inuid+ '&prgid='+prgid, 
        async: false,
        success: function (resp) {
            $('#html_modal-form').html(resp);
            $('#html_modal-title').html(titulo);
            $('#modal-form').modal();
        }
    });
}

function respostaCargaHoraria( inuid, grupo, prgid, tipoPesquisa, resposta )
{
	var go = true;
	if(resposta == 'N')
	{
		if(confirm('Caso já tenha informado o campo "Carga Horária" para alguma escola, a informação será deletada após salvar o formulário, deseja continuar?'))
		{
			go = true;
		}
		else
		{
			go = false;
			document.formulario_escolas.reset();
			//$("#carga_horaria_s").click();
		}
		
	}
	if( go )
	{
		if( grupo != 'N' )
		{
			var tooltip = retornaTooltipPorGrupo(grupo);
		 	var titulo = 'Vincular Escolas do Grupo '+grupo+' <div class="glyphicon glyphicon-question-sign" title="'+ tooltip +'" style="color:#5BC0DE;" aria-hidden="true"></div>';
		}
		else
		{
			var titulo = 'Escolas Não Aptas';
		}
	
		 var estuf 			 = ($("#estuf").val()) ? $("#estuf").val() : '';
		 var epemunicipio 	 = ($("#epemunicipio").val()) ? $("#epemunicipio").val() : '';
		 var epecod 		 = ($("#epecod").val()) ? $("#epecod").val() : '';
		 var epenome 		 = ($("#epenome").val()) ? $("#epenome").val() : '';
		 var checado 		 = ($("#checado").val()) ? $("#checado").val() : '';
		 var aepcargahoraria = ($("#aepcargahoraria").val()) ? $("#aepcargahoraria").val() : '';
	
	    $.ajax({
	        type: "POST",
	        url: 'par3.php?modulo=principal/adesao/popupEscolasProemi&acao=A',
	        data: '&grupo='+ grupo + '&inuid='+inuid+ '&prgid='+prgid+'&estuf='+estuf+'&epemunicipio='+epemunicipio+'&epecod='+epecod+'&epenome='+epenome+'&tipopesquisa='+tipoPesquisa+'&aepcargahoraria='+aepcargahoraria+'&checado='+checado+'&carga_horaria='+resposta, 
	        async: false,
	        success: function (resp) {
	            $('#html_modal-form').html(resp);
	            $('#html_modal-title').html(titulo);
	            $('#modal-form').modal();
	        }
	    });
	}
 
}
function respostaIntegracaoCurricular( resposta )
{
	$('#salvar_integracao').show();
	
	if(resposta == 'S')
	{
		$('#mod_integracao2').show();
		$('#mod_integracao3').show();
		$('#mod_integracao4').show();
	}
	else
	{	
		document.formulariointegracao.reset();

		$('#integracao_curricular_n').prop('checked', true);
		$('#mod_integracao2').hide();
		$('#mod_integracao3').hide();
		$('#mod_integracao4').hide();

		$("#questao_integracao_2").attr('disabled','disabled');
		$("#questao_integracao_3").attr('disabled','disabled');
		$("#questao_integracao_4").attr('disabled','disabled');
	}
	
}
function confirmaAdesao( decisao, link )
{
	if(decisao == 'C')
	{	
		if(confirm('Tem certeza que deseja voltar para cadastramento?'))
		{
			location.href = link ;
		}	
	}
	if(decisao == 'E')
	{	
		if(confirm('Após confirmação de envio, não será possível realizar alterações no seu processo de adesão ao Programa. Confirma a ação?'))
		{
			location.href = link ;
		}
		
	}
	if(decisao == 'S')
	{	
		location.href = link ;
	}
	if(decisao == 'N')
	{
		if(confirm('Ao escolher a opção de não aceitar a adesão, caso já tenha respondido a algum formulário, os dados serão perdidos. Confirma a ação?'))
		{
			location.href = link ;
		}	
	}
}
</script>



<div class="wrapper wrapper-content animated fadeIn" xmlns="http://www.w3.org/1999/html">
    <div class="row">
		<div class="ibox">
        	<div class="ibox-title">
            	<h3>Termo de adesão</h3>
			</div>
            <div class="ibox-content" style="padding:40px">
				<div class="tabs-container">
                	<ul class="nav nav-tabs">
                    	<li id="li1" class="<?php if ($aceite['adpresposta'] != "S" || $proadesaoDados['pfaid'] != 3) echo "active"; ?>"><a data-toggle="tab" href="#tab-1" aria-expanded="true">Termo</a></li>
                        <li id="li2" class=""><a data-toggle="tab" href="#tab-2" aria-expanded="false">Escolas</a>
                        <li id="li3" class=""><a data-toggle="tab" href="#tab-3" aria-expanded="false">Integração Curricular</a>
                        <li id="li4" class=""><a data-toggle="tab" href="#tab-4" aria-expanded="false">Síntese</a>
					</ul>
                    <div class="tab-content">
                    	<div id="tab-1" class="tab-pane <?php if($aceite['adpresposta'] != "S" || $proadesaoDados['pfaid'] != 3) echo "active"?>">
                        	<div class="panel-body">
                    			<div style="height: 300px;overflow-y: scroll" id="termo">
				                    <?php if (empty($aceite) && !isset($_SESSION[$_POST['pppid']]['termoaceito'])): ?>
				                        <script>
				                            swal({
				                                title: "Bem-Vindo ao <?php echo $programa['prgdsc'];?>!",
				                                text: '<?php echo str_replace(array("\n", "\r"), '', $proadesaoDados['pfamsgboasvindas']); ?>',
				                                html: true
				                            });
				                        </script>
				                    <?php endif; ?>

				                    <?php
					                    if ($aceite['adprrdposta'] == "S") {
					                        echo $aceite['padtermopreenchido'];
					                    } else {
					                        if (!$termoincompleto) {
					                            echo $termoPreenchido;
					                        } else {
					                            echo "<div class='alert alert-danger'>
					                                    É necessário o cadastramento do Dirigente Municipal de Educação na aba Dados da Unidade no PAR para gerar o Termo de Adesão.
					                                  </div>";
					                        }
					                    }
									?>
								</div>

								<?php  if (((empty($aceite['adpdataresposta'])) || ($aceite['adpresposta'] == 'C') ) && !$termoincompleto) { ?>

		                            <div style="text-align: center;margin-top: 20px;">
		                                <?php if ($mostrar && ( ! $vencido) ) { ?>
		                                	<b>Concorda com o termo de adesão ?</b><br><br>
		                                    
		                                        <button onclick='javascript:confirmaAdesao( "S", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>")' class="btn btn-primary">Sim</button>
		                                    
		                                        <button onclick='javascript:confirmaAdesao( "N", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=N&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>")'class="btn btn-danger">Não</button>
		                                    </a>
		                                <?php } ?>
		                                <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
		                            </div>

                        		<?php } elseif ($aceite['adpresposta'] == "S" && !$termoincompleto) { ?>
                        			<div style="margin-top: 5px;" class="text-right">
										<label class="label label-primary">
										 <?php 	
											$nome = $objUsuario->getNome($aceite['usucpf']);
											 	
											$exibeComeco = '';
											if($nome!= '')
											{
											 	$exibeComeco = $nome . ' CPF: ';
											}
										 	if( strftime("%H:%M:%S",strtotime($aceite['adpdataresposta'])) != '00:00:00')
										 	{ ?>
										 		Assinado por <?php
										 		echo $exibeComeco . formatar_cpf($aceite['usucpf']); ?> em <?php echo formata_data_hora($aceite['adpdataresposta']); ?>.
										 	<?php 
										 	}
										 	else
										 	{?>
												Assinado por <?php
												echo $exibeComeco . formatar_cpf($aceite['usucpf']); ?> em <?php echo formata_data($aceite['adpdataresposta']); ?>.
											<?php 
										 	}?>
										</label>
									</div>
			                        <div style="text-align: center;margin-top: 20px;">
			                           <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&itrid=<?php echo $itrid?>&requisicao=pdf_termo&inuid=<?php echo $_GET['inuid']?>&prgid=<?php echo $_GET['prgid']?>" target="_blanck">
			                               <button class="btn btn-primary imprimir">Imprimir</button>
			                           </a>
			                          
			                           <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
			                        </div>
                    			<?php } else if (!$termoincompleto) 
                    			{ 	
		                                if( ($result['adpresposta'] == 'N') && ($esdid == WF_ESDID_TERMONAOACEITO_PROEMI) ) 
		                                {
		                                	?>
										<div style="margin-top: 5px;" class="text-right">
										<label class="label label-danger">
										 <?php 	
											$nome = $objUsuario->getNome($aceite['usucpf']);
											 	
											$exibeComeco = '';
											if($nome!= '')
											{
											 	$exibeComeco = $nome . ' CPF: ';
											}
										 	if( strftime("%H:%M:%S",strtotime($aceite['adpdataresposta'])) != '00:00:00')
										 	{ ?>
										 		Termo não aceito por <?php
										 		echo $exibeComeco . formatar_cpf($aceite['usucpf']); ?> em <?php echo formata_data_hora($aceite['adpdataresposta']); ?>.
										 	<?php 
										 	}
										 	else
										 	{?>
												Termo não aceito por <?php
												echo $exibeComeco . formatar_cpf($aceite['usucpf']); ?> em <?php echo formata_data($aceite['adpdataresposta']); ?>.
											<?php 
										 	}?>
										</label>
									</div>
		                            <div style="text-align: center;margin-top: 20px;">
		                                <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
		                                		                            
		                             			<button onclick='javascript:confirmaAdesao( "C", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=C&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>")'class="btn btn-danger">Cancelar Resposta</button>
		                             	<?php 
		                                	}?>   
		                             </div>

                        		<?php } ?>
                			</div>
                        </div>
	                    <div id="tab-2" class="tab-pane">
							<div class="panel-body">
								
	                        	<?php 
                        	  
                        		$objOrientacao->getOrientacaoByID(ORIENTACOES_PROEMI_ABA_ESCOLAS, "Escolas");  
                        		?>
	                             <hr class="divider">
	                            <div id="div_escolas" style="<?php echo $displayescolas ?>">
	                            
	                            <input type="hidden" value="<?php echo $esdid ?>" id="esdid_atual" name="esdid_atual" />	
	                            	
	                            	<div class="col-md-1 input-lg text-right">
	                            	<?php if( $totalEscolasGrupo1 < 1 )
	                            		{ ?>
	                            		 	<span alt="Nenhuma escolafoi vinculada ao GRUPO 1" title="Nenhuma escola foi vinculada ao grupo 1" aria-hidden="true" class="glyphicon glyphicon-exclamation-sign danger fa-lg"></span>
	                            	<?php 
	                            		}?>	 
	                            		 <a onclick="popupEscolas('<?php echo $inuid ?>', '1','<?php echo $prgid ?>')" title="Alterar escolas" href="#">        
	                            		 	<?php //@todo implementar regra de nenhum ?>
	                            		 	<span  aria-hidden="true" class="glyphicon glyphicon-plus-sign success fa-lg"></span>
	                            		 </a>
	                            	</div>
	                            	<div class="col-md-11 input-lg">
	                            		Selecionar Escolas do Grupo 1
	                            	</div>
	                            	
	                            	<div class="col-md-1 input-lg text-right">
	                            	<?php if( $totalEscolasGrupo2 < 1 )
	                            		{ ?>
	                            			<span alt="Nenhuma escolafoi vinculada ao GRUPO 2" title="Nenhuma escola foi vinculada ao grupo 2" aria-hidden="true" class="glyphicon glyphicon-exclamation-sign danger fa-lg"></span>
	                            	<?php 
	                            		}?>
	                            		<a onclick="popupEscolas('<?php echo $inuid ?>', '2','<?php echo $prgid ?>')" title="Alterar escolas" href="#">
	                            			<span aria-hidden="true" class="glyphicon glyphicon-plus-sign success fa-lg"></span>
	                            		</a>  
	                            	</div>
	                            	<div class="col-md-11 input-lg ">
	                            		Selecionar Escolas do Grupo 2
	                            	</div>
	                            	
	                            	<div class="col-md-1 input-lg text-right">
	                            	<?php if( $totalEscolasGrupo3 < 1 )
	                            		{ ?>
	                            		 <span alt="Nenhuma escolafoi vinculada ao GRUPO 3" title="Nenhuma escola foi vinculada ao grupo 3" aria-hidden="true" class="glyphicon glyphicon-exclamation-sign danger fa-lg"></span>
	                            		<?php 
	                            		}
	                            		?>
	                            		<a onclick="popupEscolas('<?php echo $inuid ?>', '3','<?php echo $prgid ?>')" title="Alterar escolas" href="#">
	                            			<span aria-hidden="true" class="glyphicon glyphicon-plus-sign success fa-lg"></span>  
	                            		</a>
	                            	</div>
	                            	<div class="col-md-11 input-lg"> 
	                            		Selecionar Escolas do Grupo 3
	                            	</div>
	                            	<div class="col-md-1 input-lg text-right">
	                            		<a onclick="popupEscolas('<?php echo $inuid ?>', 'N','<?php echo $prgid ?>')" title="Alterar escolas" href="#">
	                            			<span aria-hidden="true" class="glyphicon glyphicon-list danger"></span>   
	                            		</a>
	                            	</div>
	                            	<div class="col-md-11 input-lg"> 
	                            		Lista de escolas não aptas
	                            	</div>
	                            	<?php 
	                            	if(
	                            			($esdid == WF_ESDID_ENVIADOPARAOMEC_PROEMI)
	                            			&&
	                            			(in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
	                            			in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
	                            			in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
	                            			in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis))
	                            			/*&&
	                            			( ! $vencido)*/
	                            	)
	                            	{?>
	                            	<div class="col-md-1 input-lg text-right">
	                            		<a onclick="insereExcecaoEscolas('<?php echo $inuid ?>','<?php echo $prgid ?>')" title="Solicitar Inclusão de Escolas" href="#">
	                            			<h2 aria-hidden="true" class="glyphicon glyphicon-envelope"></h2>  
	                            		</a>
	                            	</div>
	                            	<div class="col-md-11 input-lg"> 
	                            		
	                            	</div>
	                            	<?php 
	                            	}
	                            	?>
	                            </div>
	                            
							</div>
	                    </div>
	                    <div id="tab-3" class="tab-pane">
							<div class="panel-body">
	                        	<?php
	                        		
	                        		$dadosRespostaIntegracao['inuid'] = $inuid;
	                        		$dadosRespostaIntegracao['prgid'] = $prgid; 

	                        		$respostas = $objRespostaIntegracao->verificaRespostas($dadosRespostaIntegracao);
	                        		$modificado = $objRespostaIntegracao->foiModificado( $respostas );
	                        		
	                        		// Verifica se pode habilitar ou não
	                        		$exibeCancelar 	=  ($modificado) ? 'display:block;' : 'display:none;'; 
	                        		$modificaPerg	=  ($modificado) ? 'disabled=disabled' : ''; 
	                        		
	                        		
	                        		if( ! $respostas )
	                        		{
	                        			$insercao = $objRespostaIntegracao->insereRespostasPadrao($dadosRespostaIntegracao);
	                        			
	                        			$respostas = $objRespostaIntegracao->verificaRespostas($dadosRespostaIntegracao);
	                        			$modificado = $objRespostaIntegracao->foiModificado( $respostas );
	                        			 
	                        			// Verifica se pode habilitar ou não
	                        			$exibeCancelar 	=  ($modificado) ? 'display:block;' : 'display:none;';
	                        			$modificaPerg	=  ($modificado) ? 'disabled=disabled' : '';
	                        			if( ! $insercao)
	                        			{
	                        				die("erro");
	                        			}
	                        		}
	                        		else 
	                        		{
	                        			$respostaEdit	= $respostas[0]['riceditaveis'];
	                        			$resposta2		= $respostas[0]['ricpecid2'];
	                        			$resposta3 		= $respostas[0]['ricpecid3'];
	                        			$resposta4 		= $respostas[0]['ricpecid4'];
	                        			
	                        			$respostaEditS = ($respostaEdit == 'S' ) ? 'checked=checked' : '';
	                        			$respostaEditN = (($respostaEdit == 'N') || ($respostaEdit == '') ) ? 'checked=checked' : '';
	                        		}
	                        		$descricoesPerguntas = $objPerguntaIntegracao->selectDescricoes();
	                        		
	                        		$resposta1 = $descricoesPerguntas[1]['pecdesc'];
	                        		
	                        		$arrRespostas2 = array(2,5,6,7,8);
	                        		$arrRespostas3 = array(3,5,6,7,8);
	                        		$arrRespostas4 = array(4,5,6,7,8);
	                        		
	                        		
	                        		foreach($descricoesPerguntas as $k => $v)
	                        		{
	                        			if(in_array($k, $arrRespostas2)) // @todo e que não esteja marcado, e também caso tenha sido alterado adicionar em outros
	                        			{
	                        				$arrOptions2[] = $v;
	                        			}
	                        			if(in_array($k, $arrRespostas3))
	                        			{
	                        				$arrOptions3[] = $v;
	                        			}
	                        			if(in_array($k, $arrRespostas4))
	                        			{
	                        				$arrOptions4[] = $v;
	                        			}
	                        		}
	                        		
	                        		if (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
                        				in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
                        				in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
                        				in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis) ||
                        				in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)) {
                        					$mostrarBtn = 1;
                        				} else {
                        					$mostrarBtn = 0;
                        				}
	                        		
	                        		if($bloqueiaGeral || (!$mostrarBtn))
	                        		{
	                        			$disabledIntagracao = 'disabled ';
	                        		}
	                        		//Orientação da tela
	                        		$objOrientacao->getOrientacaoByID(ORIENTACOES_PROEMI_ABA_INTEGRACAO_CURRICULAR, "Integração Curricular");
	                        	?>
	                        	<form method="post" name="formulariointegracao" id="formulariointegracao" class="form form-horizontal">
	                        		<input type="hidden" value="salvar_integracao_curricular" id="requisicao_integracao" name="requisicao" />
	                        		 <div style="text-align: center;margin-top: 20px;">
		                               <hr class="divider">
	                                  	<h3><B>Campos de integração curricular obrigatórios para as escolas da rede:</B></h3><br>
	                                     
	                                    	<div class="form-group ">
							                   <label class="col-sm-3 col-md-3 col-lg-3 control-label" for="resp1">
													<b>Deseja substituir um dos campos obrigatórios para as escolas da sua rede?</b><br>
												</label>
							                    <div class="col-sm-9 col-md-9 col-lg-9 ">
							                    	<span class="form-control" style="width: 70%; float: left !important">
							                     		<input type="radio" value="S" <?php echo $disabledIntagracao; echo $respostaEditS .' '. $modificaPerg;?> name="integracao_curricular" id="integracao_curricular_s" onclick="respostaIntegracaoCurricular('S')" > Sim
	                                   					<input type="radio" value="N" <?php echo $disabledIntagracao; echo $respostaEditN .' '. $modificaPerg;?> name="integracao_curricular" id="integracao_curricular_n" onclick="respostaIntegracaoCurricular('N')" > Não
	                                   				</span> 
							                    </div>
							                </div>
	                                    	
	                                    	<div class="form-group ">
							                   <label class="col-sm-3 col-md-3 col-lg-3 control-label" for="resp1">
													Campo 1:
												</label>
							                    <div class="col-sm-9 col-md-9 col-lg-9 ">
							                     	<input type="text"  disabled=disabled value="<?php echo $resposta1;?>" class="form-control" style="width: 70%; float: left !important ;color:rgba(0, 0, 0, 0.3);"> 
							                    </div>
							                </div>
							                
	                                    	<div class="form-group ">
							                   <label class="col-sm-3 col-md-3 col-lg-3 control-label" for="resp1">
													Campo 2:
												</label>
							                    <div class="col-sm-9 col-md-9 col-lg-9 ">
							                     
							                       <select name="questao_integracao_2" id="questao_integracao_2" disabled=disabled class="form-control" style="width: 70%; float: left !important">
				                                    	<?php 
				                                    	
				                                    	
				                                    	
				                                    	foreach($arrOptions2 as $k => $option)
				                                    	{
				                                    		$selected =  ( $option['pecid'] == $resposta2 ) ? 'selected' : '';
				                                    						                                    		
				                                    		echo "<option {$selected} value=\"{$option['pecid']}\"> {$option['pecdesc']}</option>";
				                                    	}
				                                    	
				                                    	?>
				                                    </select>  
				                                   <div style="float: left !important; display: none;" id="mod_integracao2"> 
				                                   		<a href="#questao_integracao_2" onclick="editaIntegracaoCurricular('2')">
				                                   			&nbsp; <h2  class="glyphicon glyphicon-new-window success" > </h2>
				                                   		</a>
				                                   </div>
							                    </div>
							                </div>
	                                    	<div class="form-group ">
							                   <label class="col-sm-3 col-md-3 col-lg-3 control-label" for="resp1">
													Campo 3:
												</label>
							                    <div class="col-sm-9 col-md-9 col-lg-9 " >
							                       
							                        	<select name="questao_integracao_3" id="questao_integracao_3" disabled=disabled class="form-control" style="width: 70%; float: left !important">
					                                    	<?php 
					                                    	foreach($arrOptions3 as $k => $option)
					                                    	{
					                                    		$selected =  ( $option['pecid'] == $resposta3 ) ? 'selected' : '';
					                                    		echo "<option {$selected} value=\"{$option['pecid']}\"> {$option['pecdesc']}</option>";
					                                    	}
					                                    	?>
				                                    	</select> 
					                                  	<div style="float: left !important; display: none;" id="mod_integracao3">
					                                  		<a href="#questao_integracao_3" onclick="editaIntegracaoCurricular('3')">
					                                   			&nbsp; <h2  class="glyphicon glyphicon-new-window success" > </h2>
					                                   		</a>
					                                   	</div>
							                    </div>
							                </div>
	                                    	<div class="form-group ">
							                   <label class="col-sm-3 col-md-3 col-lg-3 control-label" for="resp1">
													Campo 4:
												</label>
							                    <div class="col-sm-9 col-md-9 col-lg-9 ">
							                        <select name="questao_integracao_4" id="questao_integracao_4" disabled=disabled class="form-control" style="width: 70%; float: left !important">
				                                    	<?php 
					                                    	foreach($arrOptions4 as $k => $option)
					                                    	{
					                                    		$selected =  ( $option['pecid'] == $resposta4 ) ? 'selected' : '';
					                                    		echo "<option {$selected} value=\"{$option['pecid']}\"> {$option['pecdesc']}</option>";
					                                    	}
				                                    	?>
				                                     </select> 
					                                 <div style="float: left !important; display: none;" id="mod_integracao4">
					                                 	<a href="#questao_integracao_4" onclick="editaIntegracaoCurricular('4')">
					                                   		&nbsp; <h2  class="glyphicon glyphicon-new-window success" > </h2>
					                                   	</a>
					                                 </div>
							                    </div>
							                </div>
							                
							                <div id="salvar_integracao" style="display:none" >
							                	<button type="submit" class="btn btn-success salvar" <?php echo $disabled;?>><i class="fa fa-save"></i> Salvar
							                	</button>
							                </div>
							                <?php if(! $bloqueiaGeral ){?>
							                <div id="retornar_integracao" style="<?php echo $exibeCancelar; ?>" >
								                <a onclick="confirmaCancelamentoIntegracao( '<?php echo $_GET['inuid'] ?>', '<?php echo $_GET['prgid'] ?>')" href="#">
								                	<button type="button" class="btn btn-danger">Cancelar Modificação</button>
								                </a>
							                </div>
							                <?php }?>
		                            </div>
	                        	</form>
	                        	
							</div>
	                    </div>
	                    <div id="tab-4" class="tab-pane">
							<div class="panel-body">
	                        	<div class="col-md-11">
	                        	<div id="conteudo_sintese"> 
	                        	 <?php 
	                        	 	if (( $itrid== 1) || ( $itrid== 3))
	                        	 	{

	                        	 		$tituloSintese = "Síntese da adesão ao Programa Ensino Médio Inovador - PROEMI - Estado {$modelInstrumentoUnidade->inudescricao}";
	                        	 		
	                        	 	}
	                        	 	else
	                        	 	{
	                        	 		$tituloSintese = "Síntese da adesão ao Programa Ensino Médio Inovador - PROEMI - Município {$modelInstrumentoUnidade->inudescricao} - {$modelInstrumentoUnidade->estuf}";
	                        	 	}
	                        	 ?>
	                        	<center>
			                   		<h3><?php echo $tituloSintese; ?></h3>
			                   	</center> 
			                   	<hr class="divider">
	                        	<center>
			                   		<h3>Escolas selecionadas no grupo 1</h3>
			                   	</center> 
	                        	<?php 
	                        	$arrayParams['inuid'] = $inuid;
	                        	$arrayParams['grupo'] = '1' ;
	                        	
	                        	$sqlDados = $objEscolasProemi->getListaEscolasEscolhidas($arrayParams);
                        		
	                        	?>
	                        	<hr class="divider">
	                        	<center>
			                   		<h3>Escolas selecionadas no grupo 2</h3>
			                   	</center> 
	                        	<?php 
	                        	$arrayParams['inuid'] = $inuid;
	                        	$arrayParams['grupo'] = '2' ;
	                        	$sqlDados = $objEscolasProemi->getListaEscolasEscolhidas($arrayParams);
                        		
                        		
	                        	?>
	                        	<hr class="divider">
	                        	<center>
			                   		<h3>Escolas selecionadas no grupo 3</h3>
			                   	</center> 
	                        	<?php 
	                        	$arrayParams['inuid'] = $inuid;
	                        	$arrayParams['grupo'] = '3' ;
	                        	
	                        	$sqlDados = $objEscolasProemi->getListaEscolasEscolhidas($arrayParams);	
                        		
                        		
	                        	?>
	                        	
	                        	
	                        	<hr class="divider">
								<center>
			                   		<h3>Campos de Integração Curricular Obrigatórios</h3>
			                   	</center> 
								<?php $objRespostaIntegracao->retornaListagemRespostas($inuid);?>
								
								
								<?php 
									if(count($arrRestricoes) > 0)
									{
										?>
										<hr class="divider">
										<center>
					                   		<h3>Restrições</h3>
					                   	</center>
					                   <?php 
					                   
										$objOrientacao->getTabelaRestricoes($arrRestricoes);
									}
								
									?>
									</div>
								<center>
								<?php 
									
									if((! ($esdid == WF_ESDID_ENVIADOPARAOMEC_PROEMI)) && ($result['adpresposta'] != 'N') && ($mostrarBtn) && (! $bloqueiaValidade))
									{
										if($liberaEnvio)
										{
											?>
											<button  onclick='javascript:confirmaAdesao( "E", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=E&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>")' class="btn btn-primary">Enviar ao MEC</button>
										 <?php 
										}
										else 
										{
									   ?>
									   		<button disabled  class="btn btn-primary">Enviar ao MEC</button>
									   <?php 
										}
									}
								   ?>
								      
										<button type="button" onclick='javascript:gerarPDFSintese( )' class="btn btn-warning " >Imprimir</button>
										<form method="post" name="formulario-pdf-sintese" id="formulario-pdf-sintese" class="form form-horizontal">
											<input type="hidden" value="pdf_sintese" id="requisicao" name="requisicao" />
											<input type="hidden" value="" id="conteudo_pdf" name="conteudo_pdf" />
										</form>
								     
								</center>
								
	                        	</div>
	                        	<div class="col-md-1">
									<?php if (!empty($docidInserido)) { ?>
									<?php wf_desenhaBarraNavegacao($docidInserido, array('inuid' => $inuid)); ?>
									<?php } ?>
								</div>
							</div>
	                    </div>
                    </div>
				</div>
			</div>
		</div>
	</div>
</div>
<input type="hidden" id="aba_atual" value="<?php echo $_REQUEST['abaatual'];?>">
<div id="modal-form" class="modal fade"  aria-hidden="true">
    
    <div class="modal-dialog" style="height: 90%; width: auto; overflow-y: scroll">
        <div class="ibox-title" style="width: auto;">
            <h5 id="html_modal-title" style="width: auto;"></h5>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form" style="width: auto;" >
        </div>
    </div>
</div>
<div id="modal-form2" class="modal fade"  aria-hidden="true">
    
    <div class="modal-dialog" style="height: 80%; width: 80%; overflow-y: scroll">
        <div class="ibox-title" style="width: auto;">
            <h5 id="html_modal-title2" style="width: auto;"></h5>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form2" style="width: auto;" >
        </div>
    </div>
</div>