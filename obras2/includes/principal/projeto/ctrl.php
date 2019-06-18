<?php /* ****arquivo vazio**** */

require APPRAIZ . 'obras2/includes/principal/projeto/funcoes.php';



$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];
$projetoExecutivo = new ProjetoExecutivo();
$projetoExecutivo->populaPorObrid($obrid);

$obra = new Obras();
$obra->carregarPorIdCache($obrid);

if( $_GET['acao'] != 'V' && $_GET['acao'] != 'X'){
    //Chamada de programa
    include  APPRAIZ."includes/cabecalho.inc";
    echo "<br>";
    if( !$_SESSION['obras2']['obrid'] && !$_SESSION['obras2']['empid'] ){
        $db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP,$url,$parametros);
    }elseif( $_SESSION['obras2']['obrid'] ){
        if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
        }else{
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
        }
    }else{
        $db->cria_aba(ID_ABA_CADASTRA_OBRA,$url,$parametros);
    }

}
else if($_GET['acao'] == 'X') {
	echo'<script language="JavaScript" src="../includes/funcoes.js"></script>
             <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
             <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
             <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
	$db->cria_aba('57688',$url,$parametros);
}
else{
    echo'<script language="JavaScript" src="../includes/funcoes.js"></script>
         <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
         <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
    $db->cria_aba($abacod_tela,$url,$parametros);
}

if($_POST['requisicao'] == 'download'){
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $arqid = $_REQUEST['arqid'];
    $file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    die();
}

if($_POST){
	
    $arCamposNulo = array();
    $projetoExecutivo->obrid = $obrid;
    $projetoExecutivo->proversao1 = $_POST['proversao1'];
    $projetoExecutivo->proversao2 = $_POST['proversao2'];
    $projetoExecutivo->proversao3 = $_POST['proversao3'];
    $projetoExecutivo->proimplantacao_alterado = ($_POST['proimplantacao_alterado']) ? 't' : 'f';
    $projetoExecutivo->profundacao_alterado = ($_POST['profundacao_alterado']) ? 't' : 'f';
    $projetoExecutivo->proestrutural_alterado = ($_POST['proestrutural_alterado']) ? 't' : 'f';
    $projetoExecutivo->proeletrica_alterado = ($_POST['proeletrica_alterado']) ? 't' : 'f';
    $projetoExecutivo->prohidraulica_alterado = ($_POST['prohidraulica_alterado']) ? 't' : 'f';
    $projetoExecutivo->proarquitetura_alterado = ($_POST['proarquitetura_alterado']) ? 't' : 'f';
    $projetoExecutivo->proartrrt_alterado = ($_POST['proartrrt_alterado']) ? 't' : 'f';
    $projetoExecutivo->usucpfinclusao = $_SESSION['usucpf'];

    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    
    foreach($_FILES as $key => $file){
        if (!empty($file['name'])) {
            $file = new FilesSimec($key, null, 'obras2');
            $file->setPasta('obras2');
            $file->setUpload('', $key, false);
            $arqid = $file->getIdArquivo();
            $projetoExecutivo->{$key} = $arqid;
        }

    }
    
    $projetoExecutivo->salvar(true, true, $arCamposNulo);
    $projetoExecutivo->commit();
}

if( possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE)) ){
    $habilitado = true;
    $habilita = 'S';
}else{
    $habilitado = false;
    $habilita = 'N';
}

echo cabecalhoObra($obrid);
echo "<br>";
//monta_titulo( 'Projeto Executivo', '' );
monta_titulo_obras( 'Projeto Executivo', '' );

$projetos = array(
    'proimplantacao' => array('nome' => 'Implantação', 'checked' => true, 'upload' => true, 'disabled' => true),
    'profundacao' => array('nome' => 'Fundação', 'checked' => false, 'upload' => true, 'disabled' => false),
    'proestrutural' => array('nome' => 'Estrutural', 'checked' => false, 'upload' => false, 'disabled' => false),
    'proeletrica' => array('nome' => 'Elétrica', 'checked' => false, 'upload' => false, 'disabled' => false),
    'prohidraulica' => array('nome' => 'Hidráulica', 'checked' => false, 'upload' => false, 'disabled' => false),
    'proarquitetura' => array('nome' => 'Arquitetura', 'checked' => false, 'upload' => false, 'disabled' => false),
    'proartrrt' => array('nome' => 'ART/RRT dos projetos alterados', 'checked' => true, 'upload' => true, 'disabled' => true)
);

$projetos_proprios = array(
    'proimplantacao' => array('nome' => 'Implantação', 'checked' => true, 'upload' => true, 'disabled' => true),
    'profundacao' => array('nome' => 'Fundação', 'checked' => true, 'upload' => true, 'disabled' => true),
    'proestrutural' => array('nome' => 'Estrutural', 'checked' => true, 'upload' => true, 'disabled' => true),
    'proeletrica' => array('nome' => 'Elétrica', 'checked' => true, 'upload' => true, 'disabled' => true),
    'prohidraulica' => array('nome' => 'Hidráulica', 'checked' => true, 'upload' => true, 'disabled' => true),
    'proarquitetura' => array('nome' => 'Arquitetura', 'checked' => true, 'upload' => true, 'disabled' => true),
    'proartrrt' => array('nome' => 'ART/RRT dos projetos alterados', 'checked' => true, 'upload' => true, 'disabled' => true)
);

if($projetoExecutivo->proid) {
    $projetos['proimplantacao']['checked'] = ($projetoExecutivo->proimplantacao_alterado == 't') ? true : false;
    $projetos['profundacao']['checked']    = ($projetoExecutivo->profundacao_alterado == 't') ? true : false;
    $projetos['proestrutural']['checked']  = ($projetoExecutivo->proestrutural_alterado == 't') ? true : false;
    $projetos['proeletrica']['checked']    = ($projetoExecutivo->proeletrica_alterado == 't') ? true : false;
    $projetos['prohidraulica']['checked']  = ($projetoExecutivo->prohidraulica_alterado == 't') ? true : false;
    $projetos['proarquitetura']['checked'] = ($projetoExecutivo->proarquitetura_alterado == 't') ? true : false;
    $projetos['proartrrt']['checked']      = ($projetoExecutivo->proartrrt_alterado == 't') ? true : false;
}
