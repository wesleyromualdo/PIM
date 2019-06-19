<?php 

require APPRAIZ . 'obras2/includes/principal/cadOs/funcoes.php';

if($_POST['verificaos']) {

	header('Content-Type: application/json');

	$osObra = new Supervisao_Os_Obra();

	$__os = empty($_POST['os'])?0:$_POST['os'];

	$sosnum = $osObra->verificaNumOS($__os);

	if($sosnum) {
		echo json_encode(['sosnum' => $sosnum]);
		exit;
	}

	echo json_encode(['sosnum' => false]);
	exit;
}

if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	die;
}
if($_REQUEST['requisicao']){
	$n = new $_REQUEST['classe'];
	$metodo = $_REQUEST['requisicao'];
	$n->$metodo();
}

$habil          = true;
$somenteLeitura = 'S';
if ( possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC, PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC) ) == false ){
	$habil          = false;
	$somenteLeitura = 'N';
}

if (!empty($_REQUEST['sosid'])) {
    $os = new Supervisao_Os( $_REQUEST['sosid'] );
    $docid = $os->docid;
    if($docid){
        $documento = wf_pegarDocumento($docid);

        if($documento['esdid'] != ESDID_OS_CADASTRAMENTO){
            $habil          = false;
            $somenteLeitura = 'N';
        }
    }
}



switch ( $_POST['op'] ){
	case 'salvar':
		$os = new Supervisao_Os( $_POST['sosid'] );
		$docid = $os->docid;
		if( !$docid ){
			// descrição do documento
			$docdsc = "Fluxo de OS do módulo Obras II - sosid " . $sosid;
			// cria documento do WORKFLOW
			$docid = wf_cadastrarDocumento( TPDID_OS, $docdsc );
		}

        if($_POST['sostipo'] == 'R'){
            $_POST['sosvalortotal'] = 0.00;
        }

		$arDado = array('usucpf' 	 => $_SESSION['usucpf'],
                                'sgeid'  	 => $_POST['sgeid'] ? $_POST['sgeid'] : $_POST['sgeid_disable'],
                                'semid'  	 => $_POST['semid'] ? $_POST['semid'] : $_POST['semid_disable'],
                                'sosnum'  	 => $_POST['sosnum'],
                                'docid'  	 => $docid,
                                'sosvalortotal'  => $_POST['sosvalortotal'],
                                'sosdiasexecucao'=> $_POST['sosdiasexecucao'],
                                'sosemergencial' => ($_POST['sosemergencial'] == 't' ? 't' : 'f'),
                                'sosdtemissao'   => formata_data_sql( $_POST['sosdtemissao'] ),
                                'sosdtinicio'    => formata_data_sql( $_POST['sosdtinicio'] ),
                                'sosobs'         => $_POST['sosobs'],
                                'sostipo'        => $_POST['sostipo'],
                                'sosterreno'        => $_POST['sosterreno'],
                                'sosdttermino'   => formata_data_sql( $_POST['sosdttermino'] ));

		//O número da OS ñ pode ser alterado, apenas criado!
		if($_POST['sosid']){
			unset($arDado['sosnum']);
		}
		$sosid = $os->popularDadosObjeto( $arDado )
			    ->salvar();

		$osObra = new Supervisao_Os_Obra();
		$osObra->apagaPorSosid( $sosid );
		foreach ( $_POST['empid'] as $empid ){
			$arDado = array('sosid' => $sosid,
					'empid' => $empid);
			$osObra->popularDadosObjeto( $arDado )
				   ->salvar();
			$osObra->clearDados();
		}

	    $db->commit();

	    $_SESSION['obras2']['sosid'] = $sosid;

		die("<script>
                            alert('Operação realizada com sucesso!');
                            window.location = '?modulo=principal/cadOs&acao=E';
                     </script>");
}


switch ( $_REQUEST['ajax'] ){
	case 'carregaEmpresaAndListaObra':
		carregaEmpresaAndListaObra();
		die;
	case 'carregaEmpenho':
		$sgeid = $_POST['sgeid'];
		if ( !empty($sgeid) ){
            $empenho = new Supervisao_Empenho();
            $dados = $empenho->listaCombo( array('sgeid' => $sgeid) );
        	$db->monta_combo("semid", $dados, 'S', "Selecione...", "", '', '', '', 'S', 'semid');
		}
		die;
	case 'carregaEmpenhoPorEmpresa':
		$entid = $_POST['entid'];
		$sgrid = $_POST['sgrid'];
		if ( !empty($sgrid) && !empty($entid) ){
                    $empenho = new Supervisao_Empenho();
                    $dados = $empenho->listaComboPorEmpresa( array('sgrid' => $sgrid, 'entid'=>$entid) );
                    $db->monta_combo("semid", $dados, 'S', "Selecione...", "", '', '', '', 'S', 'semid');
		}
		die;
}

if ( $_GET['acao'] == 'A' ){
	$_SESSION['obras2']['sosid'] = '';
}else{
	$sosid  = ($_GET['sosid'] ? $_GET['sosid'] : $_SESSION['obras2']['sosid']);
	$os 	= new Supervisao_Os( $sosid );
	extract( $os->getDados() );
	$grupoEmpresa = new Supervisao_Grupo_Empresa();
	if($sgeid){
		$sgrid = $grupoEmpresa->pegaGrupoIdPorSgeid( $sgeid );
	}
//	$docid = pegaDocidOs( $os->sosid );
}

$osObra  = new Supervisao_Os_Obra();
$arEmpid = $osObra->listaEmpidPorOs( $sosid );
$arEmpidAtivos = $osObra->listaEmpidPorOs( $sosid , true);
// busca prorrogacao
$ProrrogacaoPrazoOs = new ProrrogacaoPrazoOS();
if( $sosid ){
    if( $ProrrogacaoPrazoOs->verificaProrrogacao( $sosid ) ){
        $prorrogacao = 1;
    }
}

