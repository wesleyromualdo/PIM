<?php

// empreendimento || obra || orgao
if (!empty($_GET['obrid']) && $_SESSION['obras2']['obrid'] != $_GET['obrid']) {
    $_SESSION['obras2']['obrid'] = $_GET['obrid'];
    $objObr                      = new Obras($_SESSION['obras2']['obrid']);
    $_SESSION['obras2']['empid'] = $objObr->empid;
    if(empty($_SESSION['obras2']['orgid'])){
        $objEmp = new Empreendimento($_SESSION['obras2']['empid']);
        $_SESSION['obras2']['orgid'] = $objEmp->orgid;
        unset($objEmp);
    }
        unset($objObr);
}

if($_REQUEST['requisicao'] == 'ajaxCamposArquivo'){
    carregaCampoArquivo();
    die();
}


if($_REQUEST['requisicao'] == 'validaObrid'){

    $i = 0;
    $arrayObridValida = array();
    $arDadoArray= array();
    $arrayObridValida =  explode(",",  $_REQUEST['obrid']);

    foreach($arrayObridValida as $idObra){
        if (!in_array($idObra, $arDadoArray)) {
            array_push($arDadoArray, $idObra);
        }
    }

    $contInvalido = 0;
    $arDadosInvalidos = '';

    foreach ( $arDadoArray as $idObras) {

        if($idObras){



            $arDadoArray[$i] = $_POST;
            $arValidaDadoArray[$i]['obrid'] = trim($idObras);

            if(preg_match('[^0-9]',$arValidaDadoArray[$i]['obrid'])){
                echo "O ID ".$arValidaDadoArray[$i]['obrid']." da obra só pode conter números e devem ser separados por virgula.";
                die();

            }else{

                $objObr = new Obras();
                if($arValidaDadoArray[$i]['obrid'] != ''){
                    $obrid = $objObr->verificaExistenciaObrid($arValidaDadoArray[$i]['obrid']);

                    if($obrid){
                        $arDadoArray[$i]['obrid'] = $obrid['obrid'];
                    }else{
                        $contInvalido++;
                        $arDadosInvalidos .= $arValidaDadoArray[$i]['obrid'].',';
                    }
                }

                $i++;
            }

        }


    }
    $arDadosInvalidos = substr($arDadosInvalidos,0, $size-1);

    if($arDadosInvalidos && $contInvalido == 1){
        echo "O ID ".$arDadosInvalidos." é inválido!";
    }elseif($arDadosInvalidos && $contInvalido > 1){
        echo "Os IDs ".$arDadosInvalidos." são inválido!";
    }else{
        echo 0;
    }

    die();
}

//verificaSessao( 'obra' );

$diligencia = new Diligencia( $_REQUEST['dlgid'] );

switch ($_POST['requisicao']) {
    case 'salvar':

        if ($_POST['idObras']) {
            $i = 0;
            $arrayObridValida = array();
            $arDadoArray= array();
            $arrayObridValida =  explode(",", $_POST['idObras']);


            foreach($arrayObridValida as $idObra){

                if (!in_array($idObra, $arDadoArray) && trim($idObra) != '') {
                    $arDadoArray[] = $idObra;
                }
            }

            $arDadosInvalidos = '';
            foreach ( $arDadoArray as $idObras) {
                $arDadoArray[$i] = $_POST;
                $arValidaDadoArray[$i]['obrid'] = trim($idObras);
                $objObr = new Obras();
                if (preg_match('[^0-9]', $arValidaDadoArray[$i]['obrid'])) {
                    echo "O ID " . $arValidaDadoArray[$i]['obrid'] . " da obra só pode conter números e devem ser separados por virgula.";
                    die();
                } else {
                    $objObr = new Obras();

                    if ($arValidaDadoArray[$i]['obrid'] != '') {
                        $obrid = $objObr->verificaExistenciaObrid($arValidaDadoArray[$i]['obrid']);

                        if ($obrid) {
                            $arDadoArray[$i]['obrid'] = $obrid['obrid'];
                        } else {
                            $contInvalido++;
                            $arDadosInvalidos .= $arValidaDadoArray[$i]['obrid'] . ',';
                        }

                    }
                    $arDadoArray[$i]['empid'] = $objObr->empid;

                    $i++;
                }

            }


            if ($arDadosInvalidos != '') {
                $str = "<script>
                            alert('Obras com IDs inconsistentes.');
                            document.location.href = document.location.href;
                        </script>";
                die($str);
            }

           if ($_POST['only_file'] == 'false' || $_POST['only_file'] == false) {
                if (!empty($_REQUEST['dlgid']) && !possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) {
                    $esdid = $diligencia->getDadosTramitacaoDiligencia($diligencia->docid, 'esd.esdid', '');
                    $esdid = $esdid ['esdid'];
                    if ($esdid == ESDID_DILIGENCIA_SUPERADA) {
                        $str = "<script>
                                alert('A Diligência não pode ser alterada pois se encontra Superada.');
                                //window.opener.location.reload();
                                document.location.href = document.location.href;
                            </script>";
                        die($str);
                    }
                }

                if ($_POST['dlgflressalva'] == 'S' && trim($_POST['dlgobsressalva']) == '') {
                    $str = "<script>
                            alert('Diligência com Ressalva, preencha o campo de observação.');
                            document.location.href = document.location.href;
                        </script>";
                    die($str);
                }
                $arrayDoArraySqlDiligencia = array();
                $arrayErroArqDeligencia = array();


               foreach ($arDadoArray as $arDado) {

                   if ($arDado['obrid']) {

                       $arDado['dlgid'] = (empty($_REQUEST['dlgid']) ? null : $_REQUEST['dlgid']);
                       $dlgid = $diligencia->salvaDiligencia($arDado, $arDado['obrid']);
                       $tipoDiligencia = new TipoDiligencia($arDado['tpdid']);

                       if (is_array($dlgid)) {
                           $str = "<script>
                            alert('Ocorreu um erro inesperado e a Diligência não pode ser salva. " . $dlgid['msg'] . " ');
                            document.location.href = document.location.href;
                        </script>";
                           die($str);
                       }

                       $arDado['dlgid'] = $dlgid;
                       $diligencia->atualizaDocidNullDiligencia();

                       // Modificado para atender a solicitação da Fernanda Ribeiro do FNDE - 10/12/2013
                       if (isset($arDado['dlgid'])) {
                           executaRegistroAtividadeLote($arDado);
                       }

                       $db->commit();

                       if (!empty($_FILES["arquivo"])) {
                           $resp = $diligencia->gravaArquivos($arDado['dlgid'], $arDado);
                           array_push($arrayErroArqDeligencia, $resp['erro']);
                           if ($arrayErroArqDeligencia[0] != 1) {
                               array_push($arrayDoArraySqlDiligencia, $resp['sql']);
                               $resp = $diligencia->gravaArquivosDiligenciaLote($arrayDoArraySqlDiligencia[0], $arDado['dlgid']);
                           }
                           if ($resp['erro']) {
                               $db->commit();
                           };

                       }

                       if (is_array($dlgid)) {
                           $str = "<script>
                            alert('Ocorreu um erro inesperado e Diligência não pode ser salva. " . $dlgid['msg'] . " ');
                            document.location.href = document.location.href;
                        </script>";
                           die($str);
                       }

                       $arDado['dlgid'] = $dlgid;
                       $diligencia->atualizaDocidNullDiligencia();

                       $db->commit();

                       $s = new SolicitacaoDesembolso();
                       $s->verificaTramiteSolicitacoes($arDado['obrid']);

                   }

               }

            } else {

                if (!empty($_FILES["arquivo"])) {
                    $resp = $diligencia->gravaArquivosDiligencia($diligencia->dlgid, $arDado);
                    $msg = $resp['msg'];
                }
            }

            $str = "<script>
                        alert('Operação realizada com sucesso!" . $msg . "');
                        //window.opener.location.reload();
                        document.location.href = document.location.href;
                        window.opener.location.reload();
                    </script>";

            die($str);
        } else {
            $str = "<script>
                            alert('É necessario preencher alguma obra!');
                            document.location.href = document.location.href;
                        </script>";
            die($str);
        }
}

function executaRegistroAtividadeLote($arDado){
    global $db;
    $regAtividade = new RegistroAtividade();


    $arDadoRA['obrid']              = $arDado['obrid'];
    $arDadoRA['usucpf']             = $_SESSION['usucpf'];
    $arDadoRA['empid']              = $arDado['empid'];
    $arDadoRA['rgadscsimplificada'] = 'Deligêcia ('.$arDado['dlgid'].') alterada.';
    $arDadoRA['rgadsccompleta']     = 'O registro de Diligência de ID '.$arDado['dlgid'].' foi alterado pelo GESTOR MEC com o CPF '.$_SESSION['usucpf'].'.';
    $arDadoRA['rgadtinclusao']      = date('Y-m-d');
    $arDadoRA['rgastatus']          = 'A';
    $arDadoRA['rgaautomatica']      = TRUE;

    $arCamposNuloRA   = array();
    $arCamposNuloRA[] = 'arqid';
    $arCamposNuloRA[] = 'rgaid_1';

    $regAtividade->popularDadosObjeto( $arDadoRA )
                 ->salvar(true, true, $arCamposNuloRA);
    $db->commit();

    return true;
}

$dados_diligencia = $diligencia->getDados();

extract( $dados_diligencia );
$dlgdtsuperacao = formata_data($dlgdtsuperacao);

$faseDiligencia = new FaseDiligencia();
$tipoDiligencia = new TipoDiligencia($tpdid);

/*
 * Regra complexa ter cuidado ao mexer
 */
if ( (
        (!$usucpfsuperacao && !($qtsid || $rsqid) && $usucpf == $_SESSION['usucpf']) ||
        ( !$usucpfsuperacao && !($qtsid || $rsqid) && possui_perfil(array(PFLCOD_SUPER_USUARIO)) )
     ) || empty($dlgid) ){
    $somenteLeituraConteudo  = 'S';
    $habilitadoConteudo      = true;
} else{
	$somenteLeituraConteudo  = 'N';
	$habilitadoConteudo 	 = false;
}
if((empty($usucpfsuperacao)) && (  possui_perfil( array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE)) && $usucpf == $_SESSION['usucpf'] )) {
	$somenteLeituraSuperacao = 'S';
	$habilitadoSuperacao 	 = true;
} else if ( possui_perfil( array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO) ) ){
	$somenteLeituraSuperacao = 'S';
	$habilitadoSuperacao 	 = true;
}else{
	$somenteLeituraSuperacao = 'N';
	$habilitadoSuperacao 	 = false;
}

if(possui_perfil( array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE, PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL))){
    $somenteLeituraConteudo  = 'N';
    $habilitadoConteudo      = false;
    $somenteLeituraSuperacao = 'N';
    $habilitadoSuperacao     = false;
    $habilitadoBotao         = false;
}

if(possui_perfil( array(PFLCOD_GESTOR_UNIDADE))){
    $somenteLeituraConteudo  = 'N';
    $habilitadoConteudo      = false;
    $somenteLeituraSuperacao = 'N';
    $habilitadoSuperacao     = false;
    $habilitadoBotao         = true;
}

//VER($somenteLeituraConteudo,$habilitadoConteudo,$somenteLeituraSuperacao,$habilitadoSuperacao);

// Modificado para atender a solicitação da Fernanda Ribeiro do FNDE - 10/12/2013
if ( possui_perfil( array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO) ) ){
    $somenteLeituraConteudo  = 'S';
    $habilitadoConteudo      = true;
    $somenteLeituraSuperacao = 'S';
    $habilitadoSuperacao     = true;
}
//VER($somenteLeituraConteudo,$habilitadoConteudo,$somenteLeituraSuperacao,$habilitadoSuperacao);

$habilitadoBotao = ( !$habilitadoSuperacao && !$habilitadoConteudo ) ? false : true;

/**
 * Verifica se a restrição já está superada, desabilitando a sua edição.
 */
//$diligencia  = new Diligencia();
$esdid = $diligencia->getDadosTramitacaoDiligencia($docid, 'esd.esdid', '');
$esdid = $esdid ['esdid'];

if($esdid == ESDID_DILIGENCIA_SUPERADA || (possui_perfil(PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_PARLAMENTAR)) ){
    $somenteLeituraConteudo  = 'N';
    $habilitadoConteudo      = false;
    $somenteLeituraSuperacao = 'N';
    $habilitadoSuperacao     = false;
    $habilitadoBotao         = false;
}

$habilitaMudarTipo = (!empty($dlgid) && !verficaSuperUsuario(147)) ? false : $habilitadoConteudo;
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<!--<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>-->
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">

    function fechaJanela(){
        document.location.href = document.location.href;
        window.opener.location.reload();
    }

    $(document).ready(function(){

        // Tratamento colocado para retirar o # da URL, pois o mesmo estava impedindo o reload da página após as ações de tramitação e submit do form na sequencia.
        var v = String( document.location.href ).search('#');
        if(v != -1){ document.location.href = String( document.location.href ).replace( "#/", "" ).replace( "#", "" ); }
        var contador = 1;

        $('#addArquivo').click(function(){
            var url   = 'obras2.php?modulo=principal/cadDiligenciaLote&acao=O&requisicao=ajaxCamposArquivo&contador='+contador;
            var htm   = '';
            $.ajax({
                  url: url,
                  cache: false,
                beforeSend: function( xhr ) {

                    sleep(5);
                    $('#addArquivo').hide();
                }
                })
                .done(function( html ) {
                    contador++;
                    htm = htm+' <tr> '+'<td colspan="3">'+ html + '</td>'+ ' </tr>';
                    $( "#camposDinamicosArquivos" ).append( htm );
                    $('#addArquivo').show();
                });
        });


        $('.download').click(function(){
            var id = $(this).attr('id');
//            $('#requisicao').val('download');
//            $('#arqid').val(id);
//            $('#formulario').submit();

            document.location.href = 'obras2.php?modulo=principal/cadDiligenciaLote&acao=<?=$_GET['acao']?>' + '&dlgid=<?=$dlgid?>&requisicao=download&arqid='+id;
        });

        $('.excluir').click(function(){
            var id = $(this).attr('id');
            $('#requisicao').val('excluirArquivo');
            $('#arqid').val(id);
            $('#formulario').submit();
        });

    });


</script>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php

//echo monta_cabecalho_relatorio('95');
//echo cabecalhoObra($_SESSION['obras2']['obrid'], 'simples');
monta_titulo( 'Cadastra Diligência em Lote', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );

?>
<form method="post" id="formulario" name="formulario" onsubmit="return validar(<?php echo $_REQUEST["dlgid"]; ?>);" action="" enctype="multipart/form-data">

        <input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
        <input type="hidden" name="arqid"      id="arqid"      value=""/>
        <input type="hidden" name="subimete"   id="subimete"   value='2'/>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%" id="tabela_dados">
		<tr>

                        <?php
                        if($docid != NULL ){
                            echo '<td rowspan="10">' ;
                                if( possui_perfil(PFLCOD_CALL_CENTER)){
                                    wf_desenhaBarraNavegacao($docid, array('dlgid' =>  $dlgid), array('acoes' => true));
                                }else{
                                    wf_desenhaBarraNavegacao($docid, array('dlgid' =>  $dlgid));
                                }
                            echo '</td>';
                        }else{
                            if(!empty($dlgid)){
                                $dligencia = new Diligencia();
                                $docid    =  $dligencia->atualizaDocidNullDiligencia($dlgid, true);

                                if($docid){
                                   echo '<td rowspan="10">';
                                    wf_desenhaBarraNavegacao($docid, array('dlgid' =>  $dlgid));
                                    echo '</td>';
                                }else{
                                    echo '<td rowspan="10"> &nbsp; </td>';
                                }
                            }else{
                                echo '<td rowspan="10"> &nbsp; </td>';
                            }
                        }

                        ?>
		</tr>
        <tr>
            <td   colspan="2" align="center" style="color: red"><b>Os IDs das Obras devem ser separados por vírgula!</b></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" align="right">IDs das Obras:</td>
            <td valign="top">
                <?= campo_texto('idObras', 'S', 'S', '', 70, '', '', '', '', '', ''); ?>
            </td>
        </tr>

		<tr>
			<td class="SubTituloDireita" align="right">Situação da Obra na Ocorrência:</td>
			<td valign="top">
			<?php
				$db->monta_combo( 'fsdid', $faseDiligencia->listaCombo(), $somenteLeituraConteudo, 'Selecione', '', '', '', '115', 'S', 'fsdid');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" align="right">Tipo:</td>
			<td valign="top">
                <select <?= ($somenteLeituraConteudo == 'N') ? 'name="tpdid_disable" disabled="disabled"' : 'name="tpdid"'; ?> class="CampoEstilo obrigatorio" style="width: auto" id="tprid">
                    <option value="">Selecione</option>
                    <? foreach($tipoDiligencia->listaComboFlag() as $key => $value): ?>
                        <option liberaanexo="<?=$value['liberaanexo']?>" <?= ($value['codigo'] == $tpdid) ? 'selected="selected"' : ''?> value="<?=$value['codigo']?>"><?=$value['descricao']?></option>
                    <? endforeach; ?>
                </select>
			</td>
		</tr>
	    <tr>
	        <td align='right' class="SubTituloDireita">Descrição:</td>
	        <td>
	        <?php $dlgdsc = $diligencia->dlgdsc; ?>
			<?=campo_textarea("dlgdsc",'N',$somenteLeituraConteudo,'', 70, 8, 5000);?>
		    </td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita">Previsão da Providência:</td>
	        <td valign="top">
	        <?php $dlgdtprevisaoregularizacao = formata_data($diligencia->dlgdtprevisaoregularizacao); ?>
		    <?= campo_data2( 'dlgdtprevisaoregularizacao', 'N', $somenteLeituraConteudo, '', 'N' ); ?>
		    </td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita">Providência:</td>
	        <td>
	        <?php $dlgdscprovidencia = $diligencia->dlgdscprovidencia; ?>
			<?=campo_textarea("dlgdscprovidencia", 'N', $somenteLeituraConteudo, '', 70, 8, 5000);?>
		    </td>
	    </tr>

	    <?php
            if($esdid == ESDID_DILIGENCIA_AGUARDANDO_ANALISE_FNDE && possui_perfil( array(PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_MEC, PFLCOD_SUPER_USUARIO)) ){
            ?>
                <tr>
                    <td class="SubTituloDireita" align="right">Ressalva:</td>
                    <td valign="top">
                            <div style="display: inline;">
                                    <input type="radio" name="dlgflressalva" id="dlgflressalva_s" value="S" <?php echo ($dlgflressalva == 'S' ? 'checked="checked"' : '') ?> <?php echo ($habilitadoConteudo == true ? '' : 'disabled="disabled"') ?>>
                                    <label for="dlgflressalva_s">Sim</label>
                                    <input type="radio" name="dlgflressalva" id="dlgflressalva_n" value="N" <?php echo ($dlgflressalva == 'N' ? 'checked="checked"' : '') ?> <?php echo ($habilitadoConteudo == true ? '' : 'disabled="disabled"') ?>>
                                    <label for="dlgflressalva_n">Não</label>
                            </div>
                    </td>
                </tr>
                <tr>
                    <td align='right' class="SubTituloDireita">Observação da Ressalva:</td>
                    <td>
                    <?php $dlgobsressalva = $diligencia->dlgobsressalva; ?>
                            <?=campo_textarea("dlgobsressalva",'N',$somenteLeituraConteudo,'', 70, 8, 5000, '', 0, '', false, NULL);?>
                        </td>
                </tr>
	    <?php
            }elseif($esdid == ESDID_DILIGENCIA_SUPERADA){
                ?>

                <tr>
                    <td class="SubTituloDireita" align="right">Ressalva:</td>
                    <td valign="top">
                            <div style="display: inline;">
                                    <input type="radio" name="dlgflressalva" id="dlgflressalva_s" value="S" <?php echo ($dlgflressalva == 'S' ? 'checked="checked"' : '') ?> <?php echo ($habilitadoConteudo == true ? '' : 'disabled="disabled"') ?> >
                                    <label for="dlgflressalva_s">Sim</label>
                                    <input type="radio" name="dlgflressalva" id="dlgflressalva_n" value="N" <?php echo ($dlgflressalva == 'N' ? 'checked="checked"' : '') ?> <?php echo ($habilitadoConteudo == true ? '' : 'disabled="disabled"') ?>>
                                    <label for="dlgflressalva_n">Não</label>
                            </div>
                    </td>
                </tr>
                <tr>
                    <td align='right' class="SubTituloDireita">Observação da Ressalva:</td>
                    <td>
                    <?php $dlgobsressalva = $diligencia->dlgobsressalva; ?>
                            <?=campo_textarea("dlgobsressalva",'N','N','', 70, 8, 5000, '', 0, '', false, NULL, NULL);?>
                        </td>
                </tr>


        <?php
            }

        $sql = "SELECT
                    a.aeddscrealizada,
                    TO_CHAR(h.htddata, 'DD/MM/YYYY às HH24:MI:SS') as htddata,
                    u.usunome,
                    c.cmddsc,
                    CASE WHEN a.aedid IN (5554,3467,5559,5561,5563,5564) THEN 'FNDE' ELSE 'MUN' END entidade
                FROM workflow.documento d
                JOIN workflow.historicodocumento h ON h.docid = d.docid AND h.aedid IN (5554,3467,5559,5561,5563,5564)
                JOIN workflow.comentariodocumento  c ON c.hstid = h.hstid
                JOIN workflow.acaoestadodoc a ON a.aedid = h.aedid
                JOIN seguranca.usuario u ON u.usucpf = h.usucpf
                WHERE d.docid = {$diligencia->docid}
                ORDER BY h.hstid ASC
            ";
            if($diligencia->docid)
                $historico = $db->carregar($sql);
            if(!empty($historico)){
        ?>
            <tr>
                <td align='right' class="SubTituloDireita">Observações do Trâmite da Diligência:</td>
                <td>
                    <style>
                        .box-comentario{
                            padding: 0 0 0 2px;
                            border: 1px solid #ccc;
                            color: #7B7B7B;
                            margin: 1px;
                            width: 478px;
                            -webkit-border-radius: 3px;
                            -moz-border-radius: 3px;
                            border-radius: 3px;
                        }
                        .box-comentario-fnde{
                            border: 2px solid #97BAF9;
                            color:  #003DAB;
                            width: 476px;
                        }
                    </style>
                    <? foreach($historico as $item): ?>
                        <div style="" title="<?if($item['entidade'] == 'FNDE'):?>Resposta FNDE<?else:?>Resposta Entidade<? endif;?>" class="box-comentario  <?if($item['entidade'] == 'FNDE'):?>box-comentario-fnde<? endif;?>">
                            <p style="font-size:10px"><b style="font-size:12px"><?=$item['aeddscrealizada']?></b> por <?=$item['usunome']?> (<?=$item['htddata']?>):</p>
                            <p><b style="font-size:12px">"<?=$item['cmddsc']?>"</b></p>
                        </div>
                    <? endforeach; ?>
                </td>
            </tr>
        <?php
            }

            if($habilitadoBotao || ($esdid == ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA || $esdid == ESDID_DILIGENCIA_AGUARDANDO_CORRECAO)){
                echo '
                    <tr id="arquivos">
                          <td align="right" class="SubTituloDireita">Arquivos:</td>
                          <td colspan="2">
                              <span id="addArquivo" style="cursor:pointer;">
                                <img src="../imagens/obras/incluir.png" width="20px" border="0"> Adicionar Arquivo
                              </span>
                          </td>
                      </tr>';
            }
	    ?>

            <tbody id="camposDinamicosArquivos">

            </tbody>

            <tr bgcolor="#C0C0C0">
                    <td colspan="3">

                    <center>
                        <?php
                            if(($habilitadoBotao || ($esdid == ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA || $esdid == ESDID_DILIGENCIA_AGUARDANDO_CORRECAO)) && possui_perfil(array(PFLCOD_SUPER_USUARIO,PFLCOD_GESTOR_MEC,PFLCOD_GESTOR_UNIDADE,PFLCOD_SUPERVISOR_UNIDADE))){
                                echo "<input type='submit' class='botao' name='Salvar' id='salvar' value='Salvar' />";
                            }

                            if($habilitadoBotao == false && ($esdid == ESDID_DILIGENCIA_AGUARDANDO_PROVIDENCIA || $esdid == ESDID_DILIGENCIA_AGUARDANDO_CORRECAO)){
                                $only_file = '<input type="hidden" name="only_file" id="only_file" value="true"/>';
                            }else{
                                $only_file = '<input type="hidden" name="only_file" id="only_file" value="false"/>';
                            }
                            echo $only_file;
                        ?>
                        <input type='button' class='botao' value='Voltar' id='btFechar' name='btFechar' onclick='fechaJanela();'/>
                    </center>
                    </td>
            </tr>

	</table>

</form>

<script type="text/javascript">

    $(document).ready(function(){
        $( "#salvar" ).attr('disabled','disabled');

    });

    // $(function(){
    //     $("[input[name='idObras']").hover(
    //         function(){$( "#salvar" ).attr('disabled','disabled')},
    //         function(){$( "#salvar" ).removeAttr('disabled')}
    //     );
    // });

    $(function(){
        $("[input[name='idObras']").focusout(
            function(){$( "#salvar" ).attr('disabled','disabled')},

        );
    });

        $("[input[name='idObras']").focusout(function(){
        var idObras	       = $("[name='idObras']").val();

        var url   = 'obras2.php?modulo=principal/cadDiligenciaLote&acao=O&requisicao=validaObrid';

        $.ajax({
            url: url,
            cache: false,
            data:{obrid:idObras}
        })
            .done(function( html ) {
                if(html == 0){
                    // $( "#salvar" ).show();
                    $( "#salvar" ).removeAttr('disabled');
                }else{
                    // $( "#salvar" ).hide();
                    alert(html);
                    return false;
                }
            });

    });




    $($( "#salvar" ).click(function() {
        var fsdid = $('#fsdid').val();
        var idObras	       = $("[name='idObras']").val();
        var rardesc = new Array();
        var tpaid = new Array();
        var arquivo = new Array();
        var contador = new Array();


        $("input[name='rardesc[]']").each(function () {
            rardesc.push($(this).val());
        });

        $("select[name='tpaid[]']").each(function () {
            tpaid.push($(this).val());
        });

        $("[input[name='arquivo[]']").each(function () {
            arquivo.push($(this).val());
        });

        $("[input[name='contador[]']").each(function () {
            contador.push($(this).val());
        });

        if (!fsdid) {
            alert('A Situação da Obra na Ocorrência deve ser preenchido.');
            return false;
        }

        if (!idObras) {
            alert('É necessario preencher alguma obra!');
            return false;
        }


        var msg = '';
        var i;

        for (i = 0; i < rardesc.length; i++) {

            if (!rardesc[i]) {
                msg += 'É nessesário o preenchimento do campo Descrição do arquivo ('+contador[i]+')\n';
            }
            if (!tpaid[i]) {
                msg += 'É nessesário o preenchimento do Tipo do arquivo ('+contador[i]+')\n';
            }
            if (!arquivo[i]) {
                    msg += 'É nessesário adicionar um arquivo ('+contador[i]+')\n';
             }
        }
            if (msg != '') {
                alert(msg);
                return false;
            }
            $("#salvar").hide();
            return true;
        }));

    $(function(){
        $('#tpdid').change(function(){
            if($(this).find('option:selected').attr('liberaanexo') == 't'){
                $('#arquivos').show();
                $('.tabela_arquivos').show();
            } else {
                $('#arquivos').hide();
                $('.tabela_arquivos').hide();
            }
        }).change();
    });

    function fecharCampoArquivo(id){
       $('#compo_arquivo_'+id).remove();
    }


function validar(dlgid){

	var mensagem           = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao          = true;
	var tpdid              = $('#tpdid').val(); //document.getElementById("tprid").value;
	var dlgdsc	       = document.getElementById("dlgdsc");
	var fsdid	       = document.getElementById("fsdid");
	var dlgdscprovidencia  = document.getElementById("dlgdscprovidencia");

        if($('#requisicao').val() == 'download' && $('#arqid').val() != ''){
            return true;
        }
        if($('#requisicao').val() == 'excluirArquivo' && $('#arqid').val() != ''){
            return true;
        }

        if($('[name=dlgflressalva]:checked').val() == 'S' && $('#dlgobsressalva').val().trim().length == 0 ){
            alert('Diligência com Ressalva, preencha o campo de observação.');
            $('#dlgobsressalva').focus();
            return false;
        }

	if (dlgid){
		var dlgsituacao	   = document.getElementById("dlgsituacao");
	}

	dlgdsc.value = tinyMCE.getContent("dlgdsc");
	var dlgdsc_l = dlgdsc.value.length;

	dlgdscprovidencia.value = tinyMCE.getContent("dlgdscprovidencia");
	var dlgdscprovidencia_l = dlgdscprovidencia.value.length;

	var dlgitemVal = $('[name=dlgitem]:checked').val() || '';
	if( dlgitemVal == "" ){
		mensagem += 'Item \n';
		validacao = false;
	}

	if( fsdid.value == "" ){
		mensagem += 'Situação da Obra na Ocorrência \n';
		validacao = false;
	}

	if (tpdid == "" && tpdid.length == 0){
		mensagem += 'Tipo \n';
		validacao = false;
	}

	if (dlgdsc.value == ""){
		mensagem += 'Descrição \n';
		validacao = false;
	}

	if (dlgdsc_l > 5000){
		alert("O limite da Descrição é de 5000 caracteres foi ultrapassado");
		return false;
	}

	if ( dlgdscprovidencia_l > 5000 ){
		alert('O campo Providência deve ter no máximo 5000 caracteres!');
		return false;
	}

	if (document.formulario.dlgdtprevisaoregularizacao.value != ""){
		if (!validaData(document.formulario.dlgdtprevisaoregularizacao)){
			alert("A data de previsão de regularização informada é inválida");
			document.formulario.dlgdtprevisaoregularizacao.focus();
			return false;
		}
	}

	if (!validacao){
		alert(mensagem);
        $( "#salvar" ).show();
	}

	return validacao;
}


</script>

</body>


<?php

function carregaCampoArquivo(){
    global $db;
?>

    <table align="center" id="compo_arquivo_<?=$_REQUEST['contador']?>" bgcolor="#f5f5f5" border="0" class="tabela tabela_arquivos" cellpadding="3" cellspacing="1">

        <tr>
            <td ><input type="hidden" value="<?=$_REQUEST['contador']?>" name="contador[]" /></td>
            <td align="center" colspan="2" ><span style="color: darkblue; font-size: 12px;"><b>Arquivo - <?=$_REQUEST['contador']?></b></span></td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">Descrição:   </td>
            <td><?php echo campo_texto('rardesc[]','S',$habilita,'',43,100,'','', '', '', '', 'id="oardesc[]"', '');?></td>
            <td align="right">
                    <span >  <img border="0" title="Indica campo obrigatório." width="20px" src="../imagens/obras/fechar.png" onclick="fecharCampoArquivo(<?=$_REQUEST['contador']?>)"></span>

            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo:</td>
            <td colspan="2">
            <?php
                $tipoArquivo = new TipoArquivo();
                $sql = $tipoArquivo->listaCombo();
                $db->monta_combo("tpaid[]", $sql, 'S', "Selecione...", "", '', '', '', 'S', 'tpaid');
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Arquivo:</td>
            <td colspan="2">
                <input type="file" name="arquivo[]" class="obrigatorio"/>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
    </table>

<?php
}




?>

