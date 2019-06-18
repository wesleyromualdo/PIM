<?php
require APPRAIZ . 'obras2/includes/principal/listaDiligencia/ctrl.php';

$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid'];
$obrid = $_SESSION['obras2']['obrid'];


if(empty($_SESSION['obras2']['empid'])){
    $objObr = new Obras();
    $objObr->carregarPorIdCache($obrid);
    $empid =  $objObr->empid;
    $_SESSION['obras2']['empid'] = $empid;
}else{
    $empid =  $_SESSION['obras2']['empid'];
}

if($_GET['acao'] != 'L'){
switch ( $_REQUEST['requisicao'] ){
	case 'apagar':

		$diligencia = new Diligencia( $_REQUEST['dlgid'] );

		$diligencia->dlgstatus = 'I';
		$diligencia->salvar();

		if ( ($diligencia->usucpfsuperacao ) && possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) ) ){

			$procedencia = 'Cadastro Manual';
			$superado    = ( $diligencia->usucpfsuperacao ) ? 'Sim' : 'Não';
			$dtSuperado  = ( $diligencia->dlgdtsuperacao ) ? 'Em: ' . formata_data( $diligencia->dlgdtsuperacao ) : '';


			$fasediligencia = new Fasediligencia( $diligencia->fsdid );
			$tipodiligencia = new Tipodiligencia( $diligencia->tpdid );

			$regAtividade                 = new RegistroAtividade();
			$arDado                       = array();
			$arDado['obrid']              = $obrid;
			$arDado['usucpf']             = $_SESSION['usucpf'];
			$arDado['rgaautomatica']      = 't';
			$arDado['rgadscsimplificada'] = 'DILIGÊNCIA APAGADA';
			$arDado['rgadsccompleta']     = "CPF: {$_SESSION['usucpf']}
                                                         Quando: " . date('d/m/Y') . "
                                                         Procedência: {$procedencia}
                                                         Situação da Obra na Ocorrência: {$fasediligencia->fsddsc}
                                                         Tipo: {$tipodiligencia->tpddsc}
                                                         Descrição: {$diligencia->dlgdsc}
                                                         Previsão da Providência: " . formata_data( $diligencia->dlgdtprevisaoregularizacao ) . "
                                                         Providência: {$diligencia->dlgdscprovidencia}
                                                         Superado: {$superado} {$dtSuperado}";

			$regAtividade->popularDadosObjeto( $arDado )
						 ->salvar();

		}elseif ( ($diligencia->usucpfsuperacao ) ){
			$db->rollback();
			die("<script>
                                    alert('Operação não realizada!\n Usuário sem permissão para realização da operação.');
                                    window.location = '?modulo=principal/listaDiligencia&acao={$_GET['acao']}';
                             </script>");
		}

		$db->commit();
		die("<script>
                            alert('Operação realizada com sucesso!'); 
                            window.location = '?modulo=principal/listaDiligencia&acao={$_GET['acao']}';
                     </script>");
}

if( $_GET['acao'] != 'V' ){
	include  APPRAIZ."includes/cabecalho.inc";
	echo "<br>";
	if ( $_GET['acao'] == 'A' ){
		// empreendimento || obra || orgao
		verificaSessao( 'empreendimento' );

		$db->cria_aba(ID_ABA_EMP_CADASTRADO,$url,$parametros);

		$empreendimento = new Empreendimento( $empid );
		$empreendimento->montaCabecalho();

	}else{
		// empreendimento || obra || orgao
		verificaSessao( 'obra' );

		if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
			$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
		}else{
			$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
		}

		echo cabecalhoObra($obrid);

	}

	$habilitado = true;
	$habilita   = 'S';

}
else{
	// empreendimento || obra || orgao
	verificaSessao( 'obra' );
?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<?php
	$db->cria_aba($abacod_tela,$url,$parametros);
	echo cabecalhoObra($obrid);
	$habilitado = false;
	$habilita   = 'N';
    $habilitada_insercao = false;
}
    if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
        $habilitado = false;
        $habilita = 'N';
    }


if( possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) ) ){
	$habilitada_insercao = true;

}

//monta_titulo($titulo_modulo,'');
monta_titulo_obras($titulo_modulo,'');
?>


<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
    <tr>
        <td colspan="2" align="center">
            <?php if($habilitada_insercao && $habilitado && !possui_perfil( array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_GESTOR))): ?>
                <input type="button"
                       value="Inserir Nova Diligência"
                       style="padding: 1px; padding-left: 20px; padding-right: 20px; margin-top:3px; margin-bottom:3px;"
                       onclick="janela = window.open('obras2.php?modulo=principal/cadDiligencia&acao=<?php echo $_GET['acao'] ?>', 'inserirdiligencia', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' ); janela.focus();"/>
            <?php endif; ?>
        </td>
    </tr>
    <tr><td colspan="2" style="text-align: center"></td></tr>
    <tr>
        <td style="text-align: left; width: 70px">
            <span><b>Providência</b> : </span>
        </td>
        <td style="text-align: left">
            <span><img style=" vertical-align:middle" src="/imagens/0_alerta.png" title="Aguardando Providência ou Aguardando Correção" /> Aguardando Providência ou Aguardando Correção | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_inativo.png" title="Aguardando Correção ou Aguardando Correção há mais de 15 dias" /> Aguardando Providência ou Aguardando Correção há mais de 15 dias | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_ativo.png" title="Aguardando Análise FNDE"/> Aguardando Análise FNDE | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_concluido.png" title="Superada" /> Superada | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_concluido_2.png" title="Justificada" /> Justificada | </span>
            <span><img style=" vertical-align:middle" src="/imagens/0_inexistente.png" title="Cancelada" /> Cancelada </span>
        </td>
    </tr>
</table>

<?php
    require APPRAIZ . 'obras2/includes/principal/listaDiligencia/listagem.php';
    require APPRAIZ . 'obras2/includes/principal/listaDiligencia/js.php';
//end if $_GET['acao'] != 'L'
}if($_GET['acao'] == 'L'){
    include_once APPRAIZ . "obras2/modulos/principal/listaDiligenciaPesquisa.inc";

    ?>
<?php } ?>