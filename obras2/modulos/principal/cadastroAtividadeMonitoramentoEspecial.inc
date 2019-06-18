<?php
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

//verificaSessao( 'obra' );
//$empid = $_SESSION['obras2']['empid'];
$obrid = $_REQUEST['obrid'];

$mesid = ($_REQUEST['mesid']) ? $_REQUEST['mesid'] : null;
$monitoramento = new MonitoramentoEspecial($mesid);
$restricao = new Restricao($monitoramento->rstid);

if($_GET['download']){
    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $obraArquivo = new ObrasArquivos();
    $arDados = $obraArquivo->buscaDadosPorArqid( $_GET['download'] );
    $eschema = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');

    $file = new FilesSimec(null,null,$eschema);
    $file->getDownloadArquivo( $_GET['download'] );

    die('<script type="text/javascript">
			window.close();
		  </script>');
}


if($_POST['requisicao'] == 'salvar') {

    $estadoInicial = null;
    $nulos = array();

    switch ($_POST['meoid']){
        case 1 : $estadoInicial = ESDID_DIGAP; break;
        case 2 : $estadoInicial = ESDID_CGEST; break;
        case 3 : $estadoInicial = ESDID_CGIMP; break;
        case 4 : $estadoInicial = ESDID_COMAP; break;
    }

	$monitoramento->meoid = $_POST['meoid'];
	$monitoramento->obrid = (!$monitoramento->obrid) ? $obrid : $monitoramento->obrid;
	$monitoramento->usucpfinclusao = (!$monitoramento->usucpfinclusao) ? $_SESSION['usucpf'] : $monitoramento->usucpfinclusao;
	$monitoramento->mesdtcadastro = formata_data_sql( $_POST['mesdtcadastro'] );
	$monitoramento->mesdtlimite = formata_data_sql( $_POST['mesdtlimite'] );
	$monitoramento->mestitulo = $_POST['mestitulo'];
	$monitoramento->mesdescricao = $_POST['mesdescricao'];
	$monitoramento->prazo_alerta = ($_POST['prazo_alerta'] > 0 && $_POST['repetir_alerta'] == 'S') ? $_POST['prazo_alerta'] : null;
	$monitoramento->prazo_alerta = ($_POST['prazo_alerta_especificado'] && $_POST['repetir_alerta'] == 'S') > 0 ? $_POST['prazo_alerta_especificado'] : $monitoramento->prazo_alerta;
	$monitoramento->docidsituacao = (empty($monitoramento->docidsituacao)) ? wf_cadastrarDocumento(TPDID_SITUACAO_MONITORAMENTO, "Fluxo de Situações do Monitoramento Especial") : $monitoramento->docidsituacao;
	$monitoramento->docidresponsavel = (empty($monitoramento->docidresponsavel)) ? wf_cadastrarDocumento(TPDID_RESPONSAVEL_MONITORAMENTO, "Fluxo do Responsável do Monitoramento Especial", $estadoInicial) : $monitoramento->docidresponsavel;

    if($monitoramento->prazo_alerta == null){
        $nulos[] = 'prazo_alerta';
    }
    if($monitoramento->mesdtcadastro == null){
        $nulos[] = 'mesdtcadastro';
    }
    if($monitoramento->mesdtlimite == null){
        $nulos[] = 'mesdtlimite';
    }

    $monitoramento->salvar(true, true, $nulos);
    $monitoramento->commit();


    if($_POST['restricao']){

        if(!$restricao->docid) {
            $docdsc = "Fluxo de Restrição/Inconformidade do módulo Obras II";
            $docid  = wf_cadastrarDocumento(TPDID_RESTRICAO_INCONFORMIDADE, $docdsc);
        }

        $restricao->rstid = $_POST['rstid'];
        $restricao->obrid = $monitoramento->obrid;
        $restricao->usucpf = $monitoramento->usucpfinclusao;
        $restricao->rstdsc = $_POST['rstdsc'];
        $restricao->rstdscprovidencia = $_POST['rstdscprovidencia'];
        $restricao->rstitem = 'R';
        $restricao->docid = $docid;
        $restricao->tprid = 12;
        $restricao->rstdtprevisaoregularizacao = formata_data_sql( $_POST['rstdtprevisaoregularizacao'] );

        $restricao->salvar();
        $restricao->commit();

        $monitoramento->rstid = $restricao->rstid;
        $monitoramento->salvar();
        $monitoramento->commit();

        $monitoramento->enviaAlerta($monitoramento->mesid);
    }


    if ($_FILES['arquivo']['name'][0] != '') {

        $arquivos = $_FILES;

        foreach ($arquivos['arquivo']['name'] as $key => $value) {

            if(empty($value))
                continue;

            $files = array(
                'arquivo' => array(
                    'name' => $arquivos['arquivo']['name'][$key],
                    'type' => $arquivos['arquivo']['type'][$key],
                    'tmp_name' => $arquivos['arquivo']['tmp_name'][$key],
                    'error' => $arquivos['arquivo']['error'][$key],
                    'size' => $arquivos['arquivo']['size'][$key],
                )
            );
            $_FILES = $files;
            $file = new FilesSimec('arquivo', null, 'obras2');
            $file->setPasta('obras2');
            $file->setUpload($dados['arquivodescricao'][$key], 'arquivo', false);
            $arqid = $file->getIdArquivo();


            if ($arqid) {
                $_POST['arquivo'][] = $arqid;

                $arquivoMonitoramento = new MonitoramentoEspecialArquivos();
                $arquivoMonitoramento->mesid = $monitoramento->mesid;
                $arquivoMonitoramento->arqid = $arqid;
                $arquivoMonitoramento->meastatus = 'A';
                $arquivoMonitoramento->salvar();
            }
        }

    }
    verificaArqid($monitoramento->mesid, $_POST['arquivo']);


    die("<script>
			alert('Operação realizada com sucesso!');
			window.location = '?modulo=principal/cadastroAtividadeMonitoramentoEspecial&acao=A&mesid=".$monitoramento->mesid."';
		 </script>");
}


//include  APPRAIZ."includes/cabecalho.inc";

if(!$mesid)
    monta_titulo_obras( 'Cadastro de Atividade de Monitoramento', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );
else
    monta_titulo_obras( 'Edição de Atividade de Monitoramento - ID ' . $mesid , '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.' );

$habilitado = 'S';

if($mesid){
    $docsituacao = wf_pegarEstadoAtual($monitoramento->docidsituacao);
    $docresponsavel = wf_pegarEstadoAtual($monitoramento->docidresponsavel);

    $histsituacao = wf_pegarHistorico($monitoramento->docidsituacao);
    $histresponsavel = wf_pegarHistorico($monitoramento->docidresponsavel);
    $histsituacao = end($histresponsavel);
    $histresponsavel = end($histresponsavel);
}

echo cabecalhoObra(($monitoramento->obrid) ? $monitoramento->obrid : $obrid, 'simples');
?>
<html>

<!--Inclue do arquivo que carrega o CSS da pagina Inicio do Obras-->
<link rel="stylesheet" type="text/css" media="screen, print" href="../includes/layoutNovoObras2.css">

    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
    <script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

    <body>

        <form id="formulario" name="formulario" method=post enctype="multipart/form-data" >
            <input type="hidden" name="mesid" id="mesid" value="<?=$mesid; ?>" />
            <input type="hidden" name="requisicao" id="requisicao" value="salvar" />
            <input type="hidden" name="rstid" id="rstid" value="<?=$restricao->rstid?>" />

            <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="">
                <tr>
                    <td>
                        <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="">

                            <?if($mesid):?>
                            <tr>
                                <td class="SubTituloDireita" width="20%">ID Atividade:</td>
                                <td>
                                    <b style="font-size: 14px;"><?php echo $mesid?></b>
                                </td>
                            </tr>
                            <?endif;?>


                            <?if($monitoramento->docidsituacao):?>
                                <tr>
                                    <td class="SubTituloDireita" width="20%">Situação da Atividade:</td>
                                    <td>
                                        <b style="font-size: 14px;"><?php echo $docsituacao['esddsc']?></b>
                                    </td>
                                </tr>
                            <?endif;?>


                            <?if($monitoramento->docidresponsavel):?>
                                <tr>
                                    <td class="SubTituloDireita" width="20%">Responsável:</td>
                                    <td>
                                        <b style="font-size: 14px;"><?php echo $docresponsavel['esddsc']?></b>
                                    </td>
                                </tr>
                            <?endif;?>

                            <tr>
                                <td></td>
                                <td></td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita" width="20%">Origem:</td>
                                <td>
                                    <?php $sql = "SELECT meoid as codigo , meonome as descricao FROM obras2.monitoramento_especial_origem"; ?>
                                    <?php echo $db->monta_combo("meoid", $sql, $habilitado, "Selecione", '', '', '', '', 'S', 'meoid', '', $monitoramento->meoid, '', 'required'); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita" width="20%">Assunto:</td>
                                <td>
                                    <?php echo campo_texto('mestitulo', 'S', $habilitado, 'Descrição Ação', 70, 50, '', '', '', '', '', 'id="mestitulo" ', '', $monitoramento->mestitulo); ?>
                                </td>
                            </tr>


                            <tr>
                                <td class="SubTituloDireita" style="width: 190px;">Anexos</td>
                                <td colspan="2">
                                    <div>
                                        <table id="table_anexos" align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                                            <tr>
                                                <td class="">
                                                    <? if ($habilitado): ?>
                                                        <div style=""><img src="/imagens/gif_inclui.gif" alt=""/> <a href="" id="adicionar_anexo">Adicionar</a></div>
                                                    <? endif; ?>
                                                </td>
                                                <td>Nome</td>
                                                <td>Descrição</td>
                                            </tr>

                                            <?
                                            $arquivosV = new MonitoramentoEspecialArquivos();
                                            $arquivos = ($mesid) ? $arquivosV->pegaArquivosPorMesid($mesid) : array();

                                            ?>
                                            <? if (!empty($arquivos)): ?>
                                                <? foreach ($arquivos as $arquivo): ?>
                                                    <tr class="anexos">
                                                        <td class="SubTituloEsquerda" style="width: 56px;">
                                                            <? if ($habilitado): ?>
                                                                <span style=""><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span>
                                                            <? endif; ?>
                                                        </td>
                                                        <td class="SubTituloEsquerda">
                                                            <input type="hidden" value="<?=$arquivo['arqid']?>" name="arquivo[]" id="arquivo"/>
                                                            <a target="_blank" href="/obras2/obras2.php?modulo=principal/cadastroAtividadeMonitoramentoEspecial&acao=A&download=<?=$arquivo['arqid']?>">
                                                                <?=$arquivo['arqnome']?>.<?=$arquivo['arqextensao']?>
                                                            </a>
                                                        </td>
                                                        <td class="SubTituloEsquerda"><input <?= (!$habilitado) ? 'disabled="disabled"' : '' ?>  type="text" maxlength="255" size="50" value="<?=$arquivo['arqdescricao']?>" name="arquivodescricao[]" id="arquivodescricao"/></td>
                                                    </tr>
                                                <? endforeach; ?>
                                            <? endif; ?>
                                            <? if ($habilitado): ?>
                                                <tr class="anexos">
                                                    <td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td>
                                                    <td class="SubTituloEsquerda"><input type="file" name="arquivo[]" id="arquivo"/></td>
                                                    <td class="SubTituloEsquerda"><input maxlength="255" size="50" type="text" name="arquivodescricao[]" id="arquivodescricao"/></td>
                                                </tr>
                                            <? endif; ?>

                                        </table>
                                    </div>
                                </td>
                            </tr>


                            <? if (tramitadoParaResponsavel($mesid)): ?>
                            <tr>
                                <td class="" colspan="2"> </td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita" width="20%">Descrição:</td>
                                <td>
                                    <?php echo campo_textarea('mesdescricao', 'N', $habilitado, 'Descrição Detalhada', 70, 10, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição detalhada', $monitoramento->mesdescricao, array('id' => 'mesdescricao')); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita" width="20%">Providência:</td>
                                <td>
                                    <?php echo campo_textarea('mesdescricao', 'N', $habilitado, 'Descrição Detalhada', 70, 10, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição detalhada', $monitoramento->mesdescricao, array('id' => 'mesdescricao')); ?>
                                </td>
                            </tr>

                            <tr>
                                <td class="SubTituloDireita" width="20%">Data Início:</td>
                                <td>
                                    <input type="text" id="mesdtcadastro" name="mesdtcadastro" value="<?php echo formata_data($monitoramento->mesdtcadastro); ?>" size="11" maxlength="10" class="normal" >
                                </td>
                            </tr>


                            <tr>
                                <td class="SubTituloDireita" width="20%">Data Limite:</td>
                                <td>
                                    <input type="text" id="mesdtlimite" name="mesdtlimite" value="<?php echo formata_data($monitoramento->mesdtlimite); ?>" size="11" maxlength="10" class="normal" >
                                </td>
                            </tr>


                            <tr>
                                <td class="subtituloDireita">Alertar automaticamente enquando não concluir?</td>
                                <td colspan="2">
                                    <?php
                                    $select_s = '';
                                    $select_n = '';

                                    if($monitoramento->prazo_alerta) {
                                        $select_s = 'checked="checked"';
                                    } else {
                                        $select_n = 'checked="checked"';
                                    }
                                    ?>
                                    <input type="radio" name="repetir_alerta" id="repetir_alerta" value="S" <?php echo $select_s; ?> onclick="toogleLinhaDiasRepeticao('show')" > Sim
                                    <input type="radio" name="repetir_alerta" id="repetir_alerta" value="N" <?php echo $select_n; ?> onclick="toogleLinhaDiasRepeticao('hide')" > Não

                                </td>
                            </tr>
                            <tr id="linha_dias_repeticao" style="<? if (!$monitoramento->prazo_alerta): ?>display: none;<?endif;?>">
                                <td class="SubTituloDireita">Prazo em dias para repetição:</td>
                                <td colspan="2">
                                    <?php
                                    $select_01 = '';
                                    $select_07 = '';
                                    $select_10 = '';
                                    $select_30 = '';
                                    $select_60 = '';
                                    $select_90 = '';
                                    $select_espec = '';
                                    $select_especificado = '';
                                    //                                    $display = ' style="display: none" ';
                                    switch ($monitoramento->prazo_alerta) {
                                        case '1':
                                            $select_01 = 'checked="checked"';
                                            break;
                                        case '7':
                                            $select_07 = 'checked="checked"';
                                            break;
                                        case '10':
                                            $select_10 = 'checked="checked"';
                                            break;
                                        case '30':
                                            $select_30 = 'checked="checked"';
                                            break;
                                        case '60':
                                            $select_60 = 'checked="checked"';
                                            break;
                                        case '90':
                                            $select_90 = 'checked="checked"';
                                            break;
                                        default :
                                            $select_espec = 'checked="checked"';
                                            $select_especificado = $monitoramento->prazo_alerta;
                                            $display = ' style="display: inline" ';
                                            break;
                                    }
                                    ?>
                                    <input type="radio" name="prazo_alerta" id="prazo_alerta_01" value="1"  <?php echo $select_01; ?> > 1  &nbsp;&nbsp;
                                    <input type="radio" name="prazo_alerta" id="prazo_alerta_07" value="7"  <?php echo $select_07; ?> > 7  &nbsp;&nbsp;
                                    <input type="radio" name="prazo_alerta" id="prazo_alerta_10" value="10" <?php echo $select_10; ?> > 10 &nbsp;&nbsp;
                                    <input type="radio" name="prazo_alerta" id="prazo_alerta_30" value="30" <?php echo $select_30; ?> > 30 &nbsp;&nbsp;
                                    <input type="radio" name="prazo_alerta" id="prazo_alerta_60" value="60" <?php echo $select_60; ?> > 60 &nbsp;&nbsp;
                                    <input type="radio" name="prazo_alerta" id="prazo_alerta_90" value="90" <?php echo $select_90; ?> > 90 &nbsp;&nbsp;
                                    <input type="radio" name="prazo_alerta" id="prazo_alerta_espec" value="espec" <?php echo $select_espec; ?> > Especificar &nbsp;&nbsp;
                                    <br />
                                    <span id="especificar_dias" <?php echo $display;?>>
                                        <br />Especificar o número de dias:
                                        <?php echo campo_texto('prazo_alerta_especificado','S','S','',43,100,'####','', '', '', '', 'id="prazo_alerta_especificado"', '', $select_especificado); ?>
                                    </span>
                                </td>
                            </tr>



                            <?
                            if($mesid):
                                $historico = $monitoramento->pegaHistorico($mesid);

                                ?>
                                <style>
                                    .box-comentario{
                                        padding: 5px;
                                        border: 1px solid #ccc;
                                        color: #7B7B7B;
                                        margin: 2px 0;
                                        width: 95%;
                                        -webkit-border-radius: 3px;
                                        -moz-border-radius: 3px;
                                        border-radius: 3px;
                                    }
                                    .mini-box-comentario{
                                        width: 5px;
                                        height: 5px;
                                        -webkit-border-radius: 3px;
                                        -moz-border-radius: 3px;
                                        border-radius: 3px;
                                    }
                                    .box-comentario-fnde{
                                        border: 2px solid #97BAF9;
                                        background-color: #97BAF9;
                                        /*color:  #003DAB;*/
                                        color:  #FFFFFF;
                                    }
                                    .box-comentario-diligencia{
                                        border: 2px solid #CECB7D;
                                        /*color:  #616300;*/

                                        background-color: #CECB7D;
                                        color:  #FFFFFF;
                                    }
                                    .box-comentario-restricao{
                                        border: 2px solid #FF7777;
                                        /*color:  #616300;*/

                                        background-color: #FF7777;
                                        color:  #FFFFFF;
                                    }
                                </style>

                                <tr>
                                    <td class="SubTituloDireita" style="width: 190px;">Histórico</td>
                                    <td colspan="2">

                                        <? foreach($historico as $item): ?>
                                            <div style="" title="" class="box-comentario <?if($item['tpdid'] == TPDID_RESPONSAVEL_MONITORAMENTO):?>box-comentario-diligencia<?elseif($item['tpdid'] == TPDID_RESTRICAO_INCONFORMIDADE):?>box-comentario-restricao<?else:?>box-comentario-fnde<?endif;?>">
                                                <?if(!$item['esdid']):?>
                                                    <p style="font-size:11px">
                                                        Inserido por <?=$item['usunome']?> (<?=$item['data']?>):
                                                    </p>
                                                <?else:?>
                                                    <p style="font-size:11px">
                                                        De <b style="font-size:12px"><?=$item['origem']?></b> para <b style="font-size:12px"><?=$item['destino']?></b> por <?=$item['usunome']?> (<?=$item['data']?>):
                                                    </p>
                                                    <?if($item['cmddsc']):?>
                                                        <p style="padding-top: 1px">
                                                            <b style="font-size:12px">"<?=$item['cmddsc']?>"</b>
                                                        </p>
                                                    <?endif;?>
                                                <?endif;?>
                                            </div>
                                        <? endforeach; ?>
                                        <br />
                                        Legenda:
                                        <span class=".mini-box-comentario box-comentario-fnde">Fluxo da Situação do Monitoramento</span>
                                        <span class=".mini-box-comentario box-comentario-diligencia">Fluxo do Responsável</span>
                                        <span class=".mini-box-comentario box-comentario-restricao">Fluxo da Restrição</span>
                                    </td>
                                </tr>

                            <?
                            endif;
                            ?>

                            <tr>
                                <td class="SubTituloDireita" width="20%">Criar Restrição:</td>
                                <td>
                                    <input type="checkbox" <?= ($restricao->rstid) ? "disabled" : ""?> name="restricao" id="check-restricao" value="S" <?= ( $restricao->rstid ? "checked='restricao'" : "") ?>/>

                                    <?if($restricao->rstid):?>
                                        <input type="hidden" name="restricao" id="restricao" value="1" />
                                    <?endif;?>
                                </td>
                            </tr>



                            <tr>
                                <td colspan="2" style="" class="" width="100%">

                                    <table id="tabela-restricao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="<?if(!$restricao->rstid):?>display: none<?endif;?>">
                                        <tr>
                                            <td colspan="2" style="background-color: #DEDEDE; font-weight: bold; text-align: center" class="" width="100%">Restrição - Município</td>
                                        </tr>
                                        <?
                                        $docsituacaoRestricao = wf_pegarEstadoAtual($restricao->docid);
                                        ?>
                                        <?if($restricao->rstid):?>
                                            <tr>
                                                <td class="SubTituloDireita" width="20%">ID Restrição:</td>
                                                <td>
                                                    <b style="font-size: 14px;"><?php echo $restricao->rstid?></b>
                                                </td>
                                            </tr>

                                            <tr>
                                                <td class="SubTituloDireita" width="20%">Situação da Restrição:</td>
                                                <td>
                                                    <img
                                                        align="absmiddle"
                                                        src="/imagens/alterar.gif"
                                                        style="cursor: pointer"
                                                        onclick="javascript: alterarRest('<?= $restricao->rstid ?>');"
                                                        title=""> <b style="font-size: 14px;"><?php echo $docsituacaoRestricao['esddsc']?></b>
                                                </td>
                                            </tr>
                                        <?endif;?>

                                        <tr>
                                            <td class="SubTituloDireita" width="20%">Previsão de Providência:</td>
                                            <td>
                                                <input type="text" id="rstdtprevisaoregularizacao" name="rstdtprevisaoregularizacao" value="<?php echo formata_data($restricao->rstdtprevisaoregularizacao); ?>" size="11" maxlength="10" class="normal" > <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
                                            </td>
                                        </tr>
                                        <tr>
                                            <td class="SubTituloDireita" width="20%">Descrição:</td>
                                            <td>
                                                <?php echo campo_textarea('rstdsc', 'S', $habilitado, 'Descrição Detalhada', 70, 10, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição detalhada', $restricao->rstdsc, array('id' => 'rstdsc')); ?>
                                            </td>
                                        </tr>

                                        <tr>
                                            <td class="SubTituloDireita" width="20%">Providência:</td>
                                            <td>
                                                <?php echo campo_textarea('rstdscprovidencia', 'S', $habilitado, 'Providência', 70, 10, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Providência', $restricao->rstdscprovidencia, array('id' => 'rstdscprovidencia')); ?>
                                            </td>
                                        </tr>
                                    </table>

                                </td>
                            </tr>
                            <? endif; ?>

                            <tr class="divTituloPadrao">
                                <td colspan="2" align="center">
                                    <input name="btn_salvar" value="Salvar" onclick="salvar()" type="button">
                                </td>
                            </tr>
                        </table>
                    </td>



                    <td>
                        <?php
                        // Barra de estado WORKFLOW
                        if ($monitoramento->docidsituacao):
                            wf_desenhaBarraNavegacao($monitoramento->docidsituacao, array('mesid' => $monitoramento->mesid));
                        endif;
                        ?>

                        <?
                        if ($monitoramento->docidresponsavel):
                            wf_desenhaBarraNavegacao($monitoramento->docidresponsavel, array('mesid' => $monitoramento->mesid));
                        endif;
                        ?>
                    </td>



                </tr>
            </table>

        </form>

    </body>
</html>

<script type="text/javascript">
    $(function($) {

        $('#check-restricao').click(function(){
            $('#tabela-restricao').toggle('slow');
            $('html, body').animate({
                scrollTop: $('#tabela-restricao').offset().top
            }, 500);
        })

        $('#adicionar_anexo').click(function (e) {
            $('#table_anexos').append($('<tr class="anexos anexos-base"><td class="SubTituloEsquerda" style="width: 56px;"><span><img src="/imagens/excluir.gif" alt=""/> <a class="excluir_anexo" href="">Excluir</a></span></td><td class="SubTituloEsquerda"><input type="file" name="arquivo[]" id="arquivo"/></td><td class="SubTituloEsquerda"><input <?= (!$habilitado) ? 'disabled="disabled"' : '' ?>   maxlength="255" size="50" type="text" name="arquivodescricao[]" id="arquivodescricao"/></td></tr>').removeClass('anexos-base'));
            e.preventDefault();
        });
        $('.excluir_anexo').live('click',function (e) {
            $(this).parents('tr.anexos').remove();
            e.preventDefault();
        });

        $.datepicker.regional['pt-BR'] = {
            closeText: 'Fechar',
            prevText: '&#x3c;Anterior',
            nextText: 'Pr&oacute;ximo&#x3e;',
            currentText: 'Hoje',
            monthNames: ['Janeiro', 'Fevereiro', 'Mar&ccedil;o', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
            monthNamesShort: ['Jan', 'Fev', 'Mar', 'Abr', 'Mai', 'Jun', 'Jul', 'Ago', 'Set', 'Out', 'Nov', 'Dez'],
            dayNames: ['Domingo', 'Segunda-feira', 'Ter&ccedil;a-feira', 'Quarta-feira', 'Quinta-feira', 'Sexta-feira', 'S&aacute;bado'],
            dayNamesShort: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sab'],
            dayNamesMin: ['Dom', 'Seg', 'Ter', 'Qua', 'Qui', 'Sex', 'Sab'],
            weekHeader: 'Sm',
            dateFormat: 'dd/mm/yy',
            firstDay: 0,
            isRTL: false,
            showMonthAfterYear: false,
            yearSuffix: ''};
        $.datepicker.setDefaults($.datepicker.regional['pt-BR']);
    });

</script>


<script lang="javascript">
    setTimeout(function() {
        $('#mesdtcadastro').mask('99/99/9999');

        $("#mesdtcadastro").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });

        $('#mesdtlimite').mask('99/99/9999');

        $("#mesdtlimite").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });

        $('#rstdtprevisaoregularizacao').mask('99/99/9999');

        $("#rstdtprevisaoregularizacao").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            showAnim: 'drop'
        });
    }, 500);

    function validaCampos(){
        if( $('#meoid').val() == '' ){
            alert('Informe o campo "Origem"!');
            $('#meoid').focus();
            return false;
        }

        if( $('#mestitulo').val() == '' ){
            alert('Informe o campo "Assunto"!');
            $('#mestitulo').focus();
            return false;
        }

//        if( $('#mesdescricao').val() == '' ){
//            alert('Informe o campo "Descrição"!');
//            $('#mesdescricao').focus();
//            return false;
//        }
//
//        if( $('#mesdtcadastro').val() == '' ){
//            alert('Informe o campo "Data Início"!');
//            $('#mesdtcadastro').focus();
//            return false;
//        }
//
//        if( $('#mesdtlimite').val() == '' ){
//            alert('Informe o campo "Data Limite"!');
//            $('#mesdtlimite').focus();
//            return false;
//        }

        if($('#repetir_alerta:checked').val() == 'S' && $('#prazo_alerta_espec:checked').val() == 'espec' && $('#prazo_alerta_especificado').val() == ''){
            alert('O campo Prazo em dias para repetição deve ser preenchido.');
            $('#prazo_alerta_especificado').focus();
            return false;
        }

        return true;
    }

    function salvar(){
        if( validaCampos() ){
            $('[name=btn_salvar').attr('disabled','disabled');
            $('#requisicao').val('salvar');
            $('[name=formulario]').submit();
        }
    }

    function alterarRest( rstid ){
        janela = window.open('?modulo=principal/cadRestricao&acao=<?php echo $_GET['acao'] ?>&rstid=' + rstid, 'inserirRestricao', 'menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=1050,height=550' );
        janela.focus();
    }

    function toogleLinhaDiasRepeticao(acao){
        if(acao == 'show'){
            $('#linha_dias_repeticao').show();
        }else if(acao == 'hide'){
            $('#linha_dias_repeticao').hide();
        }
    }
</script>



<?


function verificaArqid($mesid, $arqid)
{
    if(is_array($arqid)) {
        foreach ($arqid as $key => $id) {
            $arquivo = new Arquivo($id);
            $arquivo->arqdescricao = $_POST['arquivodescricao'][$key];
            $arquivo->salvar();
            $arquivo->commit();
        }
    }

    $arqid = (is_array($arqid)) ? $arqid : array();
    $arquivosV = new MonitoramentoEspecialArquivos();
    $arquivos = $arquivosV->pegaArquivosPorMesid($mesid);

    if(!empty($arquivos)) {
        foreach ($arquivos as $arquivo) {
            if (array_search($arquivo['arqid'], $arqid) === false) {
                $arquivosV->carregarPorId($arquivo['meaid']);
                $arquivosV->meastatus = 'I';
                $arquivosV->salvar();
                $arquivosV->commit();
                $arquivosV->clearDados();
            }
        }
    }
}

function tramitadoParaResponsavel($mesid)
{
    global $db;
    $sql = "
        SELECT
          m.mesid
        FROM  obras2.monitoramento_especial m
        JOIN obras2.obras o ON o.obrid = m.obrid
        JOIN workflow.documento dr ON dr.docid = m.docidresponsavel
        JOIN workflow.historicodocumento h ON h.docid = dr.docid
    ";
    $h = $db->pegaUm($sql);
    return ($h) ? true : false;
}

?>
