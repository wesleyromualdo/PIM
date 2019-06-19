<?php /* ****arquivo vazio**** */

require APPRAIZ . 'obras2/includes/principal/cadObraRecursos/funcoes.php';




if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];

if( $_GET['acao'] != 'V' ){
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

}else{
    echo'<script language="JavaScript" src="../includes/funcoes.js"></script>
         <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
         <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
         <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

    $db->cria_aba($abacod_tela,$url,$parametros);

}

$obrasArquivos 	    = new ObrasArquivos();
$obras              = new Obras();
$obras->carregarPorIdCache($_SESSION['obras2']['obrid']);
$instrumento = buscaTipoESituacaoInstrumento($obras->obrid, $obras->tooid,$obras->obrid_par3);
$frpid              = $instrumento['frpid'] ? $instrumento['frpid'] : $obras->frpid;
$stiid              = $instrumento['stiid'] ? $instrumento['stiid'] : $obras->stiid;
$dtvigencia         = $instrumento['dtvigencia'];
$medidasexcecao     = $obras->medidasexcecao;
$tooid 	            = $obras->tooid;
$numconvenio        = $obras->numconvenio;
$obrnumprocessoconv = $obras->obrnumprocessoconv;
$obranoconvenio	    = $obras->obranoconvenio;
$orgid              = $_SESSION['obras2']['orgid'];

if( possui_perfil( array(PFLCOD_SUPER_USUARIO)) ){
    $habilitado = true;
    $habilita = 'S';
}else{
    $habilitado = false;
    $habilita = 'N';
}
