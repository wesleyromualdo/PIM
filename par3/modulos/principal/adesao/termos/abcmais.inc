<?php
$_SESSION['par3']['adpano_ciclo'] = $adpano_ciclo = $_REQUEST['adpano_ciclo'] ? $_REQUEST['adpano_ciclo'] : ($_SESSION['par3']['adpano_ciclo'] ? $_SESSION['par3']['adpano_ciclo'] : '2016' );

$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($_GET['prgid'], $adpano_ciclo);
$pfaid = $proadesaoDados['pfaid'];

if(!$pfaid){
    $_SESSION['par3']['adpano_ciclo'] = '2016';
    echo "
        <script>
            alert('Adesão não cadastrada. Contate o Administrador do Sistema.');
            window.location.href = 'par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=$inuid';
        </script>";
    die;
}

if(!empty($_GET['inuid'])) {
    $aceite = $modelAdesaoPrograma->recuperaPorInuid($_GET['inuid'], $pfaid, $adpano_ciclo);
} else {
    $termoincompleto = true;
}

$perfis = pegaPerfilGeral($_SESSION['usucpf']);

$adpid = $aceite['adpid'] ? $aceite['adpid'] : 'null';
$prgid = $_REQUEST['prgid'];
$inuid = $_REQUEST['inuid'];

// Instancia objetos
$objCoordenador				= new Par3_Model_Coordenadorabcmais();
$objOrientacao 				= new Par3_Model_Orientacoesprograma();
$objRespostaCargaHoraria	= new Par3_Model_Respostacargahorariaprograma();
$objAdmEscolas 				= new Par3_Model_Adesaoescolaabcmais();
$objEscolasAbcmais			= new Par3_Model_Escolasabcmais2015();
$objExcecao 				= new Par3_Controller_Excecaoescolasprograma();

$liberaEnvio = false;
// Verifica se existem escolas no grupo 1 e se existem adesao
$arr['inuid'] = $inuid;
$arr['grupo'] = '1';
$arr['adpano_ciclo'] = $adpano_ciclo;
$existeEscolaNoGrupo1 =  $objEscolasAbcmais->existeEscolaNoGrupo($arr);

$arr['inuid'] = $inuid;
$arr['grupo'] = '2';
$arr['adpano_ciclo'] = $adpano_ciclo;
$existeEscolaNoGrupo2 =  $objEscolasAbcmais->existeEscolaNoGrupo($arr);

$restricaoMinimaGrupo1 = false;
$restricaoMinimaGrupo2 = false;
if($existeEscolaNoGrupo1)
{
    $existeAdesao1 = $objAdmEscolas->existeEscolas($inuid, $adpid, '1');
	if( ! $existeAdesao1 )
	{
		$restricaoMinimaGrupo1 = true;
	}
}

if($existeEscolaNoGrupo2)
{
    $existeAdesao2 = $objAdmEscolas->existeEscolas($inuid, $adpid, '2');
	if( ! $existeAdesao2 )
	{
		$restricaoMinimaGrupo2 = true;
	}
}
if (
    ($aceite['adpresposta'] == "S") && 
    ( $objCoordenador->existeCoordenador($inuid, $adpid) )  && 
    ($objAdmEscolas->existeEscolas($inuid, $adpid)) && 
    (!$restricaoMinimaGrupo1) && 
    (!$restricaoMinimaGrupo2) 
){
	$liberaEnvio = true;
}

$arrRestricoes = array();

if( $aceite['adpresposta'] != 'S')
{
	$arrRestricoes[]['dsc']= '<span style="color:red" > Termo não aceito </span>';
}
if( ! $objCoordenador->existeCoordenador($inuid, $adpid) )
{
	$arrRestricoes[]['dsc'] = '<span style="color:red"> Dados do Coordenador não preenchidos </span>';
}
if( ! $objAdmEscolas->existeEscolas($inuid, $adpid) )
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

$dadoCoordenador	= $objCoordenador->recuperarCoordenadorPorInuid($inuid, $adpid);

$arrParamResposta['inuid'] = $inuid;
$arrParamResposta['prgid'] = $prgid;
$arrParamResposta['adpid'] = $adpid;
$arrRespostaCargaHoraria = $objRespostaCargaHoraria->getResposta($arrParamResposta);

//Sobre quantidade escolas já salvas
$dadosSobreEscola['inuid'] = $inuid;
$dadosSobreEscola['grupo'] = '1';
$dadosSobreEscola['adpid'] = $adpid;
$totalEscolasGrupo1 = $objAdmEscolas->retornaQtdSalvoGrupo($dadosSobreEscola);
$dadosSobreEscola['grupo'] = '2';
$totalEscolasGrupo2 = $objAdmEscolas->retornaQtdSalvoGrupo($dadosSobreEscola);
$dadosSobreEscola['grupo'] = '3';
$totalEscolasGrupo3 = $objAdmEscolas->retornaQtdSalvoGrupo($dadosSobreEscola);
$dadosSobreEscola['grupo'] = '4';
$totalEscolasGrupo4 = $objAdmEscolas->retornaQtdSalvoGrupo($dadosSobreEscola);

$result = $modelAdesaoPrograma->recuperaPorInuid($inuid, $pfaid, $adpano_ciclo);

if (
    in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
    in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
    in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
    in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis) 
) {
	$insereDoc = 1;
} else {
	$insereDoc = 0;
}

if( (!$result['docid']) && ($insereDoc) )
{
	//insere
	if (!empty($aceite['adpid'])) {
		$dadosRegistroDoc['adpid'] = $aceite['adpid'];
	}

	$docidInserido = wf_cadastrarDocumento(WF_TPDID_ABCMAIS, 'Documento de Adesão', WF_ESDID_NAO_INICIADO_ABCMAIS );

	if($docidInserido)
	{
	    $dadosInicio['adpano']          = date('Y');
	    $dadosInicio['adpano_ciclo']    = $adpano_ciclo;
	    $dadosInicio['inuid']           = $inuid;
	    $dadosInicio['adpid']           = $adpid != 'null' ? $adpid : null;
	    $dadosInicio['pfaid']           = $pfaid;
        $dadosInicio['docid']           = $docidInserido;
        $dadosInicio['usucpf']          = $_SESSION['usucpf'];
        $dadosInicio['tapid']           = 1;
	}
	$modelAdesaoPrograma->popularDadosObjeto($dadosInicio);
	$modelAdesaoPrograma->salvar();
	$modelAdesaoPrograma->commit();
}
else
{
	$docidInserido = $result['docid'];
}

if( (! $docidInserido)  && ($insereDoc) )
{
	?>
	Faltam dados. <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
	<?php
	die("");
}


$perfis = pegaPerfilGeral($_SESSION['usucpf']);

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "par3/classes/model/seguranca/Usuario.class.inc";


$resposta = $aceite['adpresposta'];
$editavel = false;
if( ($resposta == "C") || ($resposta == "S") ||  ($resposta == "")) {
	$editavel = true;
}

$disabled =  ($editavel) ? '' : 'disabled=disabled';
// Trata o botão do coordenador
if( $dadoCoordenador['camid'] )
{
	$botaoRemoverCoordenador = "<button type=\"button\" onclick=\"descadastrarCoordenador('/par3/par3.php?modulo=principal/adesao/termo&acao=A&requisicao=excluirCoordenador&inuid={$_GET['inuid']}&prgid={$_GET['prgid']}&camid={$dadoCoordenador['camid']}')\" class=\"btn btn-danger\" {$disabled}><i class=\"fa fa-trash\"></i> Remover Coordenador</button>";
}
else
{
	$botaoRemoverCoordenador = '';
}

$situacaoAtual = wf_pegarEstadoAtual( $docidInserido);
$esdid = $situacaoAtual['esdid'];

$bloqueiaGeral      = FALSE;
$bloqueiaValidade   = FALSE;

$vencido = false;

$_SESSION['par3']['mais_escolas']['vigencia'] = array();
$_SESSION['par3']['mais_escolas']['vigencia']['2016'] = '2016-12-31';
$_SESSION['par3']['mais_escolas']['vigencia']['2017'] = '2017-12-15';

if( 
    (
        ( $esdid == WF_ESDID_TERMONAOACEITO_ABCMAIS) || 
        ($esdid == WF_ESDID_ENVIADOPARAOMEC_ABCMAIS)
    ) || 
    (strtotime($_SESSION['par3']['mais_escolas']['vigencia'][$adpano_ciclo]. ' 23:59:00') < strtotime(date('Y-m-d G:i:s')))
){
	$bloqueiaGeral = TRUE;
    if( (strtotime($_SESSION['par3']['mais_escolas']['vigencia'][$adpano_ciclo] . ' 12:00:00') < strtotime(date('Y-m-d G:i:s'))) )
	{
		$bloqueiaValidade = TRUE;
		$vencido = true;
	}	
}

$objUsuario =  new Seguranca_Model_Seguranca_Usuario();

if (
    in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
	in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)) 
{
	$mostrar = 1;
}else{
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

if($itrid == 2){
	$proadesaoDados['pfatermomunicipio'] =  str_replace('\"','"', $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#dirigente#',$objPessoaFisica->entnome, $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#dirigente_cpf#',$objPessoaFisica->entcpf, $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#nome_entidade#', $inunome, $proadesaoDados['pfatermomunicipio']);
	$proadesaoDados['pfatermomunicipio'] =  str_replace('#data_atual#',date('d/m/Y'), $proadesaoDados['pfatermomunicipio']);

	$termoPreenchido = $proadesaoDados['pfatermomunicipio'];
}else{
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
	case 'retorna_dados_escola':
		ob_clean();
		
		$arrParam['inuid']    = $_REQUEST['inuid'];
		$arrParam['adpid']    = $_REQUEST['adpid'];
		$arrParam['cod_inep'] = $_REQUEST['cod_inep'];
		$arrParam['prgid']    = $_REQUEST['prgid'];
		
		$retorno = $objEscolasAbcmais->retornaInfoEscola($arrParam);
		
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
				.header {
    background: #000;
    margin: 0 auto;
    width: 100%;
    padding-top: 2em;
    padding-bottom: 2em;
    position: relative;
    overflow: hidden;
}
		
				</style>
		     
				<div id=\"conteudo_sintese\">
				{$conteudo}
				 
		</div>");
	
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
		$conteudo = str_replace( "a ser denominado", "a ser denominado<br><br><br>", $conteudo);
		
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
				    <div class=\"footer\"> <br><br>{$mensagemAssinatura}<br>{$rodape}</div>
				  </body>
				</html>",
				   "termoAdesao");
	
		break;
	case 'salvar_coordenador':
		
		// REcebe variáveis do coordenador direto do form
		$camid 		 = $_REQUEST['camid'];
		$camcpf 	 = str_replace(array('.','-','/'), '', $_REQUEST['camcpf']); 
		$camnome 	 = $_REQUEST['camnome'];
		$camemail 	 = $_REQUEST['camemail'];
		$camtelefone = str_replace(array('(','-',')',' '), '', $_REQUEST['camtelefone']);;
		
		// Verifica mensagem de erro
		$msgAlert = "";
		// Trata nome (exceção para erro no webservice)
		if($camnome == "")
		{
			$msgAlert = "O campo 'Nome' do Coordenador é obrigatório, caso não esteja sendo carregado verifique o campo CPF";
			simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=3", 'error', $msgAlert);
			die();
		}
		// Tra email
		if( ! $objCoordenador->validaEmailCoordenador($camemail) )
		{
			$msgAlert = "Email do Coordenador inválido.";
			simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=3", 'error', $msgAlert);
			die();
		}
		// Verifica se é edição ou inserção
		if($camid)
		{
			$dadosCoordenador['camid'] 	 = $camid;
		}
		// Carrega variáveis para salvar
		$dadosCoordenador['inuid']		= "{$inuid}";
		$dadosCoordenador['adpid']		= "{$adpid}";
		$dadosCoordenador['camcpf']		= "{$camcpf}";
		$dadosCoordenador['camnome']	= "{$camnome}";
		$dadosCoordenador['camemail']	= "{$camemail}";
		$dadosCoordenador['camtelefone']= "{$camtelefone}";
		$dadosCoordenador['camstatus']= "A";
		
		
		// Salva requisição
		$objCoordenador->popularDadosObjeto($dadosCoordenador);
		
		$objCoordenador->salvar();
		
		if( $objCoordenador->commit() )
		{
			if( $docidInserido )
			{
				if( ($esdid == WF_ESDID_NAO_INICIADO_ABCMAIS) || (! $esdid) )
				{
					wf_alterarEstado( $docidInserido, WF_AEDID_ABCMAIS_NAOINICIADO_CADASTRAMENTO, "Aceitou o termo", array(), array());
				}
			}
			$msgAlert = "Coordenador Salvo com Sucesso";
			simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=3", 'success', $msgAlert);
			die();
		}
		
		
		break;
	case 'salvar_vinculo_escola':
	
		$resultado = $objAdmEscolas->salvaFormularioEscolas($_REQUEST);
		
		simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=2", $resultado['retorno'], $resultado['mensagem']);
		die();
	
		break;
	case 'excluirCoordenador':
		// Tramamento para exclusão do coordenador
		
		// recupera id do coordenador
		$camid = $_REQUEST['camid'];
		// Variável verificadora de erro
		$erro = false;
		// Caso execute
		if($objCoordenador->inativaCoordenador($camid)){
			// Caso esteja tudo certo da mensagem de sucesso
			if( $objCoordenador->commit() ){
				$msgAlert = "Coordenador excluído com Sucesso";
				simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=3", 'success', $msgAlert);
				die();
			}else{
				$erro = true;
				$msgAlert = "Erro ao excluir o Coordenador";
			}
		}else{// Caso falte o camid
			$erro = true;
			$msgAlert = "Erro ao excluir o Coordenador - faltam variáveis";
		}
		// Retorna erro caso tenha ocorrido
		if($erro){
			simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=3", 'error', $msgAlert);
			die();
		}
		break;
	case 'salvar_orientacao':
		
		$orpidSave		= $_REQUEST[orpid];
		$orptextoSave	= $_REQUEST[orptexto];
		
		switch ($orpidSave) {
			case 3:
				$abaAtual = '2';
				break;
			case 4:
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
	case 'verifica_cpf':
		ob_clean();
		$duncpf = str_replace("/", "", str_replace("-", "", str_replace(".", "", $_POST['cpf'])));
		$instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
		// Busca primeiro no nosso banco de dados, antes de consultar a receita federal
		$dadosU = $instrumentoUnidadeEntidade->recuperarNutricionistaResponsavelPorCpf($duncpf);

		if (!empty($dadosU)) {
			$dados2 = simec_json_encode($dadosU);
			echo $dados2;
		} else {
			$usuario = new Seguranca_Model_Usuario();
			$dadosU = $usuario->recuperarPorCPF($duncpf);
			if (!empty($dadosU)) {
				$dadosU = array(array('usunome' => $dadosU["usunome"], 'camemail' => $dadosU["usuemail"], 'origem' => "seguranca"));
				echo simec_json_encode($dadosU);
			} else {
				$resp = recuperarUsuarioReceita($duncpf);
				$dadosU = array(array('usunome' => $resp['dados']['no_pessoa_rf'], 'camemail' => "", 'origem' => "receita"));
				echo simec_json_encode($dadosU);
			}
		}
		die();
		break;
	case 'salva_resposta_carga_horaria':
		/*ob_clean();
		
		$rchid			= $_REQUEST['rchid'];
		$rchresposta 	= $_REQUEST['rchresposta'];
		$rchidJaSalvo = $arrRespostaCargaHoraria['rchid'];
		
		if(($rchid != 0) || ( $rchidJaSalvo))
		{
			if(( $rchidJaSalvo))
			{
				$dadosResposta['rchid'] = $rchidJaSalvo;
			}
			else 
			{
				$dadosResposta['rchid'] = $rchid;
			}
			
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
				if( ($esdid == WF_ESDID_NAO_INICIADO_ABCMAIS) || (! $esdid) )
				{
					wf_alterarEstado( $docidInserido, WF_AEDID_ABCMAIS_NAOINICIADO_CADASTRAMENTO, "Aceitou o termo", array(), array());
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
		die();
		*/
		break;
	default:
		break;
}

if ($_GET['aceite'] == 'C'){
	if($aceite['adpid']){
	    $dadosInicio['adpano'] 				= date('Y');
	    $dadosInicio['adpano_ciclo'] 		= $adpano_ciclo;
		$dadosInicio['adpdataresposta']  	= 'now()';
		$dadosInicio['adpresposta'] 		= 'C';
		$dadosInicio['usucpf'] 				= $_SESSION['usucpf'];
		$dadosInicio['tapid']  				= 1;
		$dadosInicio['adpid'] 				= $aceite['adpid'];
		
		$modelAdesaoPrograma->popularDadosObjeto($dadosInicio);
		$modelAdesaoPrograma->salvar();
		
		if( $modelAdesaoPrograma->commit() ){
			$msgAlert = "Programa 'Novo Mais Educação' retornado para 'Em cadastramento'";
			wf_alterarEstado( $docidInserido, WF_AEDID_ABCMAIS_TERMONAOACEITO_EMCADASTRAMENTO, "Cancelou resposta de 'Não aceito' ", array(), array());
			simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid, 'success', $msgAlert);
		}
	}
}

if($_GET['aceite'] == 'E'){
	if( $esdid == WF_ESDID_EMCADASTRAMENTO_ABCMAIS ){
		$msgAlert = "Adesão da Secretaria de Educação ao Programa Novo Mais Educação realizada com sucesso. Por favor, informe às escolas selecionadas sobre a necessidade de cadastrarem o plano de atendimento no Sistema PDDE Interativo para participação no programa.";
		wf_alterarEstado( $docidInserido, WF_AEDID_ABCMAIS_CADASTRAMENTO_ENVIADOPARAMEC, "Enviou para o MEC", array(), array());
		simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$inuid."&prgid=".$prgid."&abaatual=4", 'success', $msgAlert);
	}
}

if ($_GET['aceite'] == 'S' || $_GET['aceite'] == 'N'){
    
    $dados['adpano_ciclo'] = $_GET['adpano_ciclo'];
    
    $dados['inuid'] = $_GET['inuid'];
    $dados['adpid'] = $_GET['adpid'];
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

    if ($_GET['aceite'] == 'S') {
    	if($docidInserido )
    	{
    		if( ($esdid == WF_ESDID_NAO_INICIADO_ABCMAIS) || (! $esdid) )
    		{
    			wf_alterarEstado( $docidInserido, WF_AEDID_ABCMAIS_NAOINICIADO_CADASTRAMENTO, "Aceitou o termo", array(), array());
    		}
    	}
    	$msgAlert = $proadesaoDados['pfamsgdeaceitacao'];
    } else {
    	
    	
    	$msgAlert = $proadesaoDados['pfamsgdenaoaceitacao'];
    }

    $_SESSION[$_POST['pppid']]['termoaceito'] = true;

    if ($_GET['aceite'] == 'N') {
    	
    	
    	//apaga dados escolas,
    	$objAdmEscolas->deletaGeralPorUnidade($dados['adpid'], $dados['prgid']);
    	//apaga dados coordenador
    	$objCoordenador->deletaGeralPorUnidade($dados['adpid']);
    	
    	if( $docidInserido )
    	{
    		if($esdid == WF_ESDID_EMCADASTRAMENTO_ABCMAIS)
    		{
    			wf_alterarEstado( $docidInserido, WF_AEDID_ABCMAIS_CADASTRAMENTO_TERMONAOACEITO, "Não Aceitou o termo", array(), array());
    		}
    		else
    		{
    			wf_alterarEstado( $docidInserido, WF_AEDID_ABCMAIS_NAOINICIADO_NAOACEITO, "Não Aceitou o termo", array(), array());
    		}
    	}
    	
        simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$dados['inuid']."&prgid=".$dados['prgid'], 'success', $msgAlert);
    } else {
        simec_redirecionar('/par3/par3.php?modulo=principal/adesao/termo&acao=A&inuid='.$dados['inuid']."&prgid=".$dados['prgid'], 'success', $msgAlert);
    }
}
?>

<script>

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

function filtraPesquisa( inuid, adpid, adpciclo_ano, grupo, prgid, tipoPesquisa ){

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
		 var eammunicipio 	 = ($("#eammunicipio").val()) ? $("#eammunicipio").val() : '';
		 var eamrede 		 = ($("#eamrede").val()) 	? $("#eamrede").val() : '';
		 var eamcod 		 = ($("#eamcod").val())		? $("#eamcod").val() : '';
		 var eamnome 		 = ($("#eamnome").val())		? $("#eamnome").val() : '';
		 var checado 		 = ($("#checado").val())		? $("#checado").val() : '';
		 var aeacargahoraria = ($("#aeacargahoraria").val()) ? $("#aeacargahoraria").val() : '';
		 var ordenacao		 = ($('#ordenacao').val()) ? $('#ordenacao').val() : '';
	
		if(tipoPesquisa == 'xls')
		{
			link =	'par3.php?modulo=principal/adesao/popupEscolasAbcMais&acao=A'+'&grupo='+'&grupo='+ grupo+
				 	'&inuid='+inuid+'&adpid='+adpid+'&adpano_ciclo=<?php echo $adpano_ciclo; ?>&prgid='+prgid+
				 	'&eammunicipio='+eammunicipio+'&eamrede='+eamrede+'&eamcod='+eamcod+'&eamnome='+eamnome+
				 	'&tipopesquisa='+tipoPesquisa+'&aeacargahoraria='+aeacargahoraria+'&checado='+checado+'&ordenacao='+ordenacao;
			location.href = link ;
			return false;
		}
		else
		{
		   $.ajax({
		       type: "POST",
		       url: 'par3.php?modulo=principal/adesao/popupEscolasAbcMais&acao=A',
		       data: 	'&grupo='+ grupo + '&inuid='+inuid+'&adpid='+adpid+
	       				'&adpano_ciclo=<?php echo $adpano_ciclo; ?>&prgid='+prgid+'&eammunicipio='+eammunicipio+
	       				'&eamrede='+eamrede+'&eamcod='+eamcod+'&eamnome='+eamnome+'&tipopesquisa='+tipoPesquisa+
	       				'&aeacargahoraria='+aeacargahoraria+'&checado='+checado+'&ordenacao='+ordenacao, 
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

function retornaTooltipPorGrupo(grupo)
{
	var title = new Array();
	title['1'] = 'Escolas com Índice de Desenvolvimento da Educação Básica (IDEB) inferior a 4.4 nos anos iniciais e inferior a 3.0 nos anos finais, concomitantemente.';
	title['2'] = 'Escolas com IDEB inferior a 4.4 nos anos iniciais OU IDEB inferior a 3.0 nos anos finais.';
	title['3'] = 'Escolas com mais de 50% dos alunos oriundos de famílias beneficiárias do Programa Bolsa Família e que não se enquadrarem nos critérios anteriores.';

	return title[grupo];
}


function insereExcecaoEscolas( inuid, adpid, prgid )
{

    $.ajax({
        type: "POST",
        url: 'par3.php?modulo=principal/adesao/popupIncluirEscolasExcecao&acao=A',
        data: '&inuid='+inuid+'&adpid='+adpid+ '&prgid='+prgid, 
        async: false,
        success: function (resp) {
            $('#html_modal-form2').html(resp);
            $('#html_modal-title2').html('Solicitação de inclusão de escolas na adesão');
            $('#modal-form2').modal();
        }
    });
}

function popupEscolas( inuid, adpid, grupo, prgid )
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
        url: 'par3.php?modulo=principal/adesao/popupEscolasAbcMais&acao=A',
        data: '&grupo='+ grupo + '&inuid='+inuid+'&adpid='+adpid+ '&prgid='+prgid+'&adpano_ciclo=<?php echo $adpano_ciclo; ?>', 
        async: false,
        success: function (resp) {
            $('#html_modal-form').html(resp);
            $('#html_modal-title').html(titulo);
            $('#modal-form').modal();
        }
    });
}

function respostaCargaHoraria( inuid, adpid, grupo, prgid, tipoPesquisa, resposta )
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
	
		 var eammunicipio 	 = ($("#eammunicipio").val()) ? $("#eammunicipio").val() : '';
		 var eamrede 		 = ($("#eamrede").val()) 	? $("#eamrede").val() : '';
		 var eamcod 		 = ($("#eamcod").val())		? $("#eamcod").val() : '';
		 var eamnome 		 = ($("#eamnome").val())		? $("#eamnome").val() : '';
		 var checado 		 = ($("#checado").val())		? $("#checado").val() : '';
		 var aeacargahoraria = ($("#aeacargahoraria").val()) ? $("#aeacargahoraria").val() : '';
	
		
	   	$.ajax({
	       type: "POST",
	       url: 'par3.php?modulo=principal/adesao/popupEscolasAbcMais&acao=A',
	       data: '&grupo='+ grupo + '&inuid='+inuid+'&adpid='+adpid+ '&prgid='+prgid+'&eammunicipio='+eammunicipio+'&eamrede='+eamrede+'&eamcod='+eamcod+'&eamnome='+eamnome+'&tipopesquisa='+tipoPesquisa+'&aeacargahoraria='+aeacargahoraria+'&checado='+checado+'&carga_horaria='+resposta, 
	       async: false,
	       success: function (resp) {
	           $('#html_modal-form').html(resp);
	           $('#html_modal-title').html(titulo);
	           $('#modal-form').modal();
	       }
	   	});
	}
	
}

function validateEmail($email) {
  var emailReg = /^([\w-\.]+@([\w-]+\.)+[\w-]{2,4})?$/;
  return emailReg.test( $email );
}
	
$(document).ready(function() {

	$('#adpano_ciclo').change(function(){
		var ano = $(this).val();
		var link = 'par3.php?modulo=principal/adesao/termo&acao=A';
		var campos = '&prgid=<?php echo $_REQUEST['prgid']?>&inuid=<?php echo $_REQUEST['inuid']?>&adpano_ciclo='+ano;
		window.location.href = link+campos;
	});

	$('#modal-form').on('hidden.bs.modal', function () {
		var esdid = $("#esdid_atual").val();
		if( $('#grupoPopUp').val() != 'N' && ((esdid  != 1815) && (esdid != 1817)) )
        {
	     	if( ! confirm('Você está saindo da tela de vinculação de escolas, caso não tenha salvado, as informações preenchidas serão desconsideradas. Deseja continuar?'))
	     	{
	     		$('#modal-form').modal();
	    	}
        }
	});

	$('#formulario-coordenador').submit(function() {

		if( validateEmail( $('#camemail').val() ) )
		{
			return true;
		}	
		else
		{
			alert('E-mail inválido. Insira um endereço de E-mail válido');
		}
		return false;
	});
    
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
	
	$('.btn-xls').hide();
	$('.btn-query').hide();
	
    $('.ibox').on('change', 'input[name=camcpf]', function () {
        if (!validar_cpf($(this).val())) {
            alert("CPF inválido!\nFavor informar um cpf válido!");
            $(this).val('');
            $(this).parent().parent().find('input[name=camnome]').val('');
            $(this).parent().parent().find("label").html('');
            return false;
        }
        var param = new Array();
        param.push({name: 'requisicao', value: 'verifica_cpf'},
            {name: 'cpf', value: $(this).val()});

        var t = $(this);

        $.ajax({
            type: "POST",
            dataType: "json",
            url: window.location.href,
            data: param,
            success: function (data) {

                if (data[0].origem == "instrumentounidade_entidade") {
                    var locais = [];
                    for (i = 0; data.length > i; i++) {
                        locais.push(data[i].inudescricao);
                    }

                    var lista_locais = locais.join(',');

                    swal({
                            title: "Você tem certeza?",
                            text: "O nutricionista <b>(" + data[0].usunome + ")</b> selecionado está vinculado aos município(s): <b>" + lista_locais + "</b>. Deseja fazer o cadastro dele em um novo município!",
                            type: "warning", showCancelButton: true,
                            confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
                            closeOnConfirm: "on",
                            cancelButtonText: "Cancelar",
                            html: true
                        },
                        function (isConfirm) {
                            if (isConfirm) {
                                t.closest(".ibox-content").find('input[name=camnome]').val(data[0].usunome);
                                t.closest(".ibox-content").find('input[name=camemail]').val(data[0].camemail);
                            }
                            else {
                                t.closest(".ibox-content").find('input[name=camcpf]').val("");
                                t.closest(".ibox-content").find('input[name=camemail]').val("");
                                t.closest(".ibox-content").find('input[name=camnome]').val("");
                            }
                        });

                }
                if (data[0].origem == "receita") {
                    t.closest(".ibox-content").find('input[name=camnome]').val(data[0].usunome);
                    t.closest(".ibox-content").find('input[name=camemail]').val("");
                    t.closest(".ibox-content").find('input[name=camtelefone]').val("");
                }
                if (data[0].origem == "seguranca") {
                    t.closest(".ibox-content").find('input[name=camnome]').val(data[0].usunome);
                    t.closest(".ibox-content").find('input[name=camemail]').val(data[0].camemail);
                    t.closest(".ibox-content").find('input[name=camtelefone]').val("");
                }
            },
       		 error: function (data) {
	        	 t.closest(".ibox-content").find('input[name=camnome]').val("");
	             t.closest(".ibox-content").find('input[name=camemail]').val("");
	             t.closest(".ibox-content").find('input[name=camtelefone]').val("");
        	}
        });
    });
});

function descadastrarCoordenador( link )
{
	if(confirm('Tem certeza que deseja remover o Coordenador?'))
	{
		location.href = link ;
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
		//@todo apagar os dados dos formulários
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
        		<div class="row">
        			<div class="col-md-6 input-lg">
                    	<h3>Termo de adesão - 
                    	<?php 
                    	echo $modalInstrumentoUnidade->inudescricao;
                    	?>
                    	</h3>
                   	</div>
        			<div class="col-md-6 input-lg">
        				<?php 
        				$sql = "SELECT 
                                	to_char(generate_series, 'YYYY') as codigo,
                                	to_char(generate_series, 'YYYY') as descricao
                                FROM 
                                    generate_series(
                                    	date_trunc('year', (SELECT min(adpano_ciclo)||'/01/01' FROM par3.prodesaoprograma)::date),
                                    	date_trunc('year', current_date),
                                    	INTERVAL'1 year');";
        				$arrAttr = ['data-placeholder' => 'Ano'];
        				echo $simec->select('adpano_ciclo', '', $adpano_ciclo, $sql, $arrAttr);
        				?>
        			</div>
        		</div>
			</div>
            <div class="ibox-content" style="padding:40px">
				<div class="tabs-container">
                	<ul class="nav nav-tabs">
                    	<li id="li1" class="<?php if ($aceite['adpresposta'] != "S" || $proadesaoDados['pfaid'] != 3) echo "active"; ?>"><a data-toggle="tab" href="#tab-1" aria-expanded="true">Termo</a></li>
                        <li id="li2" class=""><a data-toggle="tab" href="#tab-2" aria-expanded="false">Escolas</a>
                        <li id="li3" class=""><a data-toggle="tab" href="#tab-3" aria-expanded="false">Coordenador</a>
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
								<?php 
								if(
							        (
							             (empty($aceite['adpdataresposta'])) || 
							             ($aceite['adpresposta'] == 'C') 
						            ) && 
								    !$termoincompleto
							    ) { ?>

		                            <div style="text-align: center;margin-top: 20px;">
		                                <?php if ($mostrar && ( ! $vencido) ) { ?>
		                                	<b>Concorda com o termo de adesão ?</b><br><br>
		                                    
		                                        <button onclick='javascript:confirmaAdesao( "S", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>&adpano_ciclo=<?php echo $adpano_ciclo; ?>")' class="btn btn-primary">Sim</button>
		                                    
		                                        <button onclick='javascript:confirmaAdesao( "N", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=N&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>&adpano_ciclo=<?php echo $adpano_ciclo; ?>")' class="btn btn-danger">Não</button>
		                                    
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
                    			<?php }
                    			 else if (!$termoincompleto) { 
                                	if( ($result['adpresposta'] == 'N') && ($esdid == WF_ESDID_TERMONAOACEITO_ABCMAIS) ) 
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
		                                 		                            
		                             			<button onclick='javascript:confirmaAdesao( "C", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=C&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>&adpano_ciclo=<?php echo $adpano_ciclo; ?>")'class="btn btn-danger">Cancelar Resposta</button>
		                             	<?php 
		                                	}?>
		                            </div>

                        		<?php } ?>
                			</div>
                        </div>
	                    <div id="tab-2" class="tab-pane">
							<div class="panel-body">
	                        	<?php 
	                        	$orpid = ORIENTACOES_ABCMAIS_ABA_ESCOLAS_2016;
	                        	if($_SESSION['par3']['adpano_ciclo'] == '2017'){
	                        	    $orpid = ORIENTACOES_ABCMAIS_ABA_ESCOLAS_2017;
                                }
                                $objOrientacao->getOrientacaoByID($orpid, "Escolas");  
	                        	?>
	                             <hr class="divider">
	                            <div id="div_escolas" style="<?php echo $displayescolas ?>">
	                            <input type="hidden" value="<?php echo $esdid ?>" id="esdid_atual" name="esdid_atual" />	
	                            	
	                            	<div class="col-md-1 input-lg text-right">
	                            		 
	                            		<?php if( $totalEscolasGrupo1 < 1 )
	                            		{ ?>
	                            		 	<span alt="Nenhum escola foi vinculada ao GRUPO 1" title="Nenhuma escola foi vinculada ao grupo 1" aria-hidden="true" class="glyphicon glyphicon-exclamation-sign danger fa-lg"></span>
	                            		<?php
	                            		}
	                            		?>
	                            		 <a onclick="popupEscolas('<?php echo $inuid ?>', <?php echo $adpid ?>, '1','<?php echo $prgid ?>')" title="Alterar escolas" href="#">        
	                            		 	<span  aria-hidden="true" class="glyphicon glyphicon-plus-sign success fa-lg"></span>
	                            		 </a>
	                            	</div>
	                            	<div class="col-md-11 input-lg">
	                            	
	                            		Selecionar Escolas do Grupo 1
	                            	</div>
	                            	
	                            	<div class="col-md-1 input-lg text-right">
	                            	<?php if( $totalEscolasGrupo2 < 1 )
	                            	{ ?>
	                            		<span alt="Nenhum escola foi vinculada ao GRUPO 2" title="Nenhuma escola foi vinculada ao grupo 2" aria-hidden="true" class="glyphicon glyphicon-exclamation-sign danger fa-lg"></span>
	                            	<?php 
	                            	}
	                            	?>
	                            		<a onclick="popupEscolas('<?php echo $inuid ?>', <?php echo $adpid ?>, '2','<?php echo $prgid ?>')" title="Alterar escolas" href="#">
	                            			<span aria-hidden="true" class="glyphicon glyphicon-plus-sign success fa-lg"></span>
	                            		</a>  
	                            	</div>
	                            	<div class="col-md-11 input-lg ">
	                            		Selecionar Escolas do Grupo 2
	                            	</div>
	                            	
	                            	<div class="col-md-1 input-lg text-right">
	                            		<?php if( $totalEscolasGrupo3 < 1 )
	                            		{ ?>
	                            			 <span alt="Nenhum escola foi vinculada ao GRUPO 3" title="Nenhuma escola foi vinculada ao grupo 3" aria-hidden="true" class="glyphicon glyphicon-exclamation-sign danger fa-lg"></span>
	                            		<?php
	                            		} ?>
	                            		<a onclick="popupEscolas('<?php echo $inuid ?>', <?php echo $adpid ?>, '3','<?php echo $prgid ?>')" title="Alterar escolas" href="#">
	                            			<span aria-hidden="true" class="glyphicon glyphicon-plus-sign success fa-lg"></span>  
	                            		</a>
	                            	</div>
	                            	<div class="col-md-11 input-lg"> 
	                            		Selecionar Escolas do Grupo 3
	                            	</div>
	                            	<!--  
	                            	<div class="col-md-1 input-lg text-right">
	                            		<?php if( $totalEscolasGrupo4 < 1 )
	                            		{ ?>
	                            			 <span alt="Nenhum escola foi vinculada ao GRUPO 3" title="Nenhuma escola foi vinculada ao grupo 3" aria-hidden="true" class="glyphicon glyphicon-exclamation-sign danger fa-lg"></span>
	                            		<?php
	                            		} ?>
	                            		<a onclick="popupEscolas('<?php echo $inuid ?>', <?php echo $adpid ?>, '4','<?php echo $prgid ?>')" title="Alterar escolas" href="#">
	                            			<span aria-hidden="true" class="glyphicon glyphicon-plus-sign success fa-lg"></span>  
	                            		</a>
	                            	</div>
	                            	<div class="col-md-11 input-lg"> 
	                            		Selecionar Escolas do Grupo 4
	                            	</div>
	                            	-->
	                            	<div class="col-md-1 input-lg text-right">
	                            		<a onclick="popupEscolas('<?php echo $inuid ?>', <?php echo $adpid ?>, 'N','<?php echo $prgid ?>')" title="Alterar escolas" href="#">
	                            			<span aria-hidden="true" class="glyphicon glyphicon-list danger"></span>  
	                            		</a>
	                            	</div>
	                            	<div class="col-md-11 input-lg"> 
	                            		Lista de escolas não aptas
	                            	</div>
	                            	<?php 
	                            	if(
	                            			($esdid == WF_ESDID_ENVIADOPARAOMEC_ABCMAIS)
	                            			&&
	                            			(
                            			        (
                                                    (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
                            					    in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
                                                    in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis))
                            			            &&
                            			            (strtotime('2017-12-31 12:00:00') > strtotime(date('Y-m-d G:i:s')))
                    					        ) ||
	                            			    in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)
                            			    )
	                            			/*&&
	                            			( ! $vencido)*/
	                            	 ) 
	                            	{
	                            	    ?>
	                            	<div class="col-md-1 input-lg text-right">
	                            	
	                            		<a onclick="insereExcecaoEscolas('<?php echo $inuid ?>','<?php echo $adpid ?>','<?php echo $prgid ?>')" title="Solicitar Inclusão de Escolas" href="#">
	                            			<h2 aria-hidden="true" class="glyphicon glyphicon-envelope"></h2>  
	                            		</a>
	                            	</div>
	                            	<div class="col-md-3 input-lg">
    	                            	Solicitar Inclusão de Escolas
	                            	<?php 
	                            	if(in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
	                            	    in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
	                            	    in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis))
	                            	{
	                            	?> 
	                            		<label style="color:red; font-size: 12px;" >
	                            			(Prazo Final: 15/12/2017 12:00)
	                            		</label>
	                            	<?php 
	                            	}
	                            	?>
	                            	</div>
	                            	<div class="col-md-8 input-lg"> 
	                            		
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
		                  				$objOrientacao->getOrientacaoByID(ORIENTACOES_ABCMAIS_ABA_COORDENADOR, "Coordenador");
		                  				?>
		                  		<hr class="divider">
								<form method="post" name="formulario-coordenador" id="formulario-coordenador" class="form form-horizontal">
									<input type="hidden" value="salvar_coordenador" id="requisicao" name="requisicao" />
									<input type="hidden" value="<?php echo $dadoCoordenador['camid'] ?>" id="camid" name="camid" />
		                        	 <div class="">
		                        	
					                   	<center>
					                   		<h3>Dados do Coordenador</h3>
					                   	</center> 
						               
						               
		                  				<?php
		                  				if (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
	                  						in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
	                  						in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
	                  						in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis) ||
	                  						in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)) {
	                  							$mostrarBtn = 1;
	                  						} else {
	                  							$mostrarBtn = 0;
	                  						}
		                  				if($bloqueiaGeral || ( !$mostrarBtn) )
		                  				{
		                  					$disabled1 = 'disabled=disabled';
		                  				}
		                  				
		                  				echo $simec->cpf('camcpf', 'CPF', $dadoCoordenador['camcpf'], array($disabled1, 'data-pessoa' => true, 'required'), array('label-size' => 3, 'input-size' => 9)) ;
		                  				echo $simec->input('camnome', 'Nome', $dadoCoordenador['camnome'], array('maxlength' => '255', true, 'readonly' => 'readonly','required'), array('label-size' => 3, 'input-size' => 9));
		                  				echo $simec->email('camemail', 'E-mail', $dadoCoordenador['camemail'], array('class' => 'email',$disabled1,'required'), Array('required'));
		                  				echo $simec->telefone('camtelefone', 'Telefone', $dadoCoordenador['camtelefone'] , array('class' => 'telefone', 'required', $disabled1));
		                  				
		                  				if((!$bloqueiaGeral) && ($mostrarBtn))
		                  				{
		                  				?>
		                  					<center>
		                  						<button type="submit" class="btn btn-success salvar" <?php echo $disabled;?>><i class="fa fa-save"></i> Salvar</button>
		                  						<?php 
		                  						echo $botaoRemoverCoordenador;
		                  						?>
		                  					</center>
		                  				<?php }?>	
		                  				
		                  				
		                  				 
		                        	</div>
	                        	</form>
							</div>
	                    </div>
	                    <div id="tab-4" class="tab-pane">
							<div class="panel-body">
	                        	<div  class="col-md-11"> 
	                        	<div id="conteudo_sintese">
	                        	
	                        	 <?php 
	                        	 	if (( $itrid== 1) || ( $itrid== 3))
	                        	 	{
	                        	 		$tituloSintese = "Síntese da adesão ao Programa Novo Mais Educação - Estado {$modelInstrumentoUnidade->inudescricao}";
	                        	 	}
	                        	 	else
	                        	 	{
	                        	 		$tituloSintese = "Síntese da adesão ao Programa Novo Mais Educação - Município {$modelInstrumentoUnidade->inudescricao} - {$modelInstrumentoUnidade->estuf}";
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
	                        	$arrayParams['adpid'] = $adpid;
	                        	$arrayParams['grupo'] = '1' ;
	                        	
	                        	$objEscolasAbcmais->getListaEscolasEscolhidas($arrayParams);
	                        	
	                        	?>
	                        	<hr class="divider">
	                        	<center>
			                   		<h3>Escolas selecionadas no grupo 2</h3>
			                   	</center> 
	                        	<?php 
	                        	$arrayParams['grupo'] = '2' ;
	                        	
	                        	$sqlDados = $objEscolasAbcmais->getListaEscolasEscolhidas($arrayParams);
	                        
	                        	?>
	                        	<hr class="divider">
	                        	<center>
			                   		<h3>Escolas selecionadas no grupo 3</h3>
			                   	</center> 
	                        	<?php 
	                        	$arrayParams['grupo'] = '3' ;
	                        	$sqlDados = $objEscolasAbcmais->getListaEscolasEscolhidas($arrayParams);
	                        	
	                        	?>
	                        	<hr class="divider">
	                        	<!--  
	                        	<center>
			                   		<h3>Escolas selecionadas no grupo 4</h3>
			                   	</center> 
	                        	<?php 
	                        	$arrayParams['grupo'] = '4' ;
	                        	$sqlDados = $objEscolasAbcmais->getListaEscolasEscolhidas($arrayParams);
	                        	
	                        	?>
	                        	-->
	                        	<?php  // echo ($editavel) ? 'Editavel' : 'não editavel';?>
	                        	
	                        	<hr class="divider">
								<center>
			                   		<h3>Dados do Coordenador</h3>
			                   	</center> 
								<?php $objCoordenador->retornaListagemCoordenador($adpid);?>
								
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
									if(
								        ( !($esdid == WF_ESDID_ENVIADOPARAOMEC_ABCMAIS) ) && 
									    ($result['adpresposta'] != 'N') && 
									    ($mostrarBtn) && 
									    (! $bloqueiaValidade)
								    ){
										if($liberaEnvio){ ?>
											<button onclick='javascript:confirmaAdesao( "E", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=E&inuid=<?php echo $_GET['inuid'] ?>&adpid=<?php echo $adpid ?>&prgid=<?php echo $_GET['prgid'] ?>&adpano_ciclo=<?php echo $adpano_ciclo; ?>")' class="btn btn-primary">Enviar ao MEC</button>
									   <?php 
										}else{
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