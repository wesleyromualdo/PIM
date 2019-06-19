<?php

$obDocumento = new Par3_Controller_DocumentoTermo();

switch ($_REQUEST['requisicao']) {
    case 'aceitarTermoManifesto':

        $url = 'par3.php?modulo=principal/adesao/termos/manifestacao&acao=A&inuid='.$_REQUEST['inuid'];

        if ($_SESSION['usucpf'] != $_SESSION['usucpforigem']) {
            simec_redirecionar($url, 'error', 'Ação não permitida para simulação de usuarios.');
            exit();
        }

        if ($_SESSION['usucpf'] == '00000000191') {
            simec_redirecionar($url, 'error', 'O CPF do usuario carregado na sessão é inválido!');
            exit();
        }

        $estuf = $db->pegaUm("SELECT estuf FROM par3.instrumentounidade WHERE inuid = {$_REQUEST['inuid']}");
        $db->executar("UPDATE par3.documentotermomanifesto SET domopcao = {$_REQUEST['opcao']} WHERE estuf = '{$estuf}' AND domstatus = 'A'");
        $db->commit();

        $sql = "INSERT INTO par3.documentotermovalidacao ( domid, dtvcpf, dtvdatavalidacao, dtvstatus )
				VALUES ( ".$_REQUEST['domid'].", '".$_SESSION['usucpf']."', NOW(), 'A' )";
        $db->executar($sql);
        $db->commit();

        simec_redirecionar($url, 'success', 'O aceite foi confirmado com sucesso!');
        break;
    case 'imprimir':
        $obDocumento->imprimirTermoManifestoPDF($_REQUEST['domid']);
        exit();
        break;
}

require APPRAIZ . 'includes/cabecalho.inc';

$dadosTermo = $obDocumento->recuperaTermoManifestacao($_GET['inuid']);

$perfil = pegaArrayPerfil($_SESSION['usucpf']);
$habilitaAceite = false;
if (in_array(PAR3_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR3_PERFIL_SECRETARIO_ESTADUAL, $perfil)) {
    $habilitaAceite = true;
}

if (!empty($dadosTermo['cpfgestor'])) {
    $habilitaAceite = false;
    $boAceite = 'S';
} else {
    $boAceite = 'N';
}

$html = '<input type="hidden" name="domopcao" value="'.$dadosTermo['domopcao'].'">
         <input type="hidden" name="aceite_manifesto" value="'.$boAceite.'">';

$mdoconteudo = '';
if ($dadosTermo['dtanomearquivo']) {
    $diretorio = APPRAIZ . 'arquivos/par3/documentoTermo/'.$dadosTermo['dtanomearquivo'];
    if (is_file($diretorio)) {
        $mdoconteudo = file_get_contents($diretorio);
    }
}

if($mdoconteudo == ''){
    $habilitaAceite = false;
}

/*if (!empty($dadosTermo['cpfgestor'])) {
    $html .= '<div style="margin-top: 5px;" class="text-right">
                <label class="label label-primary">
                Manifesta&ccedil;&atilde;o aceita em '.$dadosTermo['data'].'&nbsppor '.$dadosTermo['usunome'].'&nbsp;/&nbsp;'.$dadosTermo['cpfgestor'].'
                </label>
              </div>';
}*/

$mdoconteudo = $mdoconteudo . $html;
$mdoconteudo = simec_html_entity_decode(str_ireplace('\"', '"', $mdoconteudo));

$url = 'par3/par3.php?modulo=principal/adesao/termos/manifestacao&acao=A';
global $db;
//$db->cria_aba( 58049, $url , '&inuid=' . $inuid, array());
?>
<div class="wrapper wrapper-content animated fadeIn" xmlns="http://www.w3.org/1999/html">
    <div class="row">
        <div class="ibox">
            <div class="ibox-title">
                <h3>Termo de Manifestação</h3>
            </div>
            <div class="ibox-content" style="padding:40px">
                <div class="tabs-container">
                    <ul class="nav nav-tabs">
                        <li class="active"><a data-toggle="tab" href="#tab-1" aria-expanded="true">Termo</a></li>
                    </ul>
                    <div class="tab-content">
                        <div id="tab-1" class="tab-pane active">
                            <div class="panel-body">
                                <form name="form_gerar_termo" id="form_gerar_termo" class="form-horizontal" method="post">
                                    <div style="height: 300px;overflow-y: scroll;}" id="conteudo">
                                        <div id="termo">
                                            <style>
                                                @media print {
                                                    html {
                                                        margin: 2cm;
                                                    }

                                                    @page {
                                                        margin: 20px;
                                                    }
                                                }
                                            </style>
                                            <?php
                                                echo $obDocumento->montaHtmlTermo($mdoconteudo);
                                            ?>
                                        </div>
                                    </div>

                                    <input type="hidden" name="estuf" id="estuf" value="<?= $dadosTermo['estuf'] ?>">
                                    <input type="hidden" name="_domid" id="_domid" value="<?= $dadosTermo['domid'] ?>">
                                    <input type="hidden" name="mdoconteudo_t" id="mdoconteudo_t" value="<?php echo simec_htmlentities($mdoconteudo); ?>">
                                    <input type="hidden" name="requisicao" id="requisicao" value="salvar-termo">

                                    <?php if (!empty($dadosTermo['cpfgestor'])) { ?>
                                        <div style="margin-top: 5px;" class="text-right">
                                            <label class="label label-primary">
                                                Manifestação aceita em <?= $dadosTermo['data'] .'&nbsppor ' . $dadosTermo['usunome'] . '&nbsp;/&nbsp;' . $dadosTermo['cpfgestor'] ?>
                                            </label>
                                        </div>
                                    <?php } ?>

                                    <div style="text-align: center;margin-top: 20px;">
                                        <button type="button" id="btn-imprimir" data-dismiss="modal" class="btn btn-default" onclick="imprimirTermo();"><i class="fa fa-times-circle-o"></i> Imprimir</button>
                                        <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid']; ?>" class="btn btn-default">Sair</a>
                                        <?php           if ($habilitaAceite) { ?>
                                            <button type="button" id="btn-aceite-termo-manifesto" data-dismiss="modal" class="btn btn-success btn-aceite-termo-manifesto" onclick="btn_aceitarTermo_manifesto();" data-loading-text="Aceite, aguarde ..."><i class="fa fa-times-circle-o"></i> Aceitar</button>
                                        <?php           }?>
                                            <!--<a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&prgid=1&inuid=<?php /*echo $_GET['inuid']; */?>&prgid=<?php /*echo $_GET['prgid']; */?>&adpid=<?php /*echo $aceite['adpid']; */?>">
                                                <button class="btn btn-primary">Aceito</button>
                                            </a>-->
                                    </div>
                                </form>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script>

    $(document).ready(function(){
        var domopcao = jQuery('[name="domopcao"]').val();
        jQuery('#opcao_manifesto_'+domopcao).attr('checked', true);

        var aceite_manifesto = jQuery('[name="aceite_manifesto"]').val();
        if( aceite_manifesto == 'S' ){
            jQuery('.btn-aceite-termo-manifesto').hide();
            jQuery('[name="opcao_manifesto"]').attr('disabled', true);
        }
    });

    function btn_aceitarTermo_manifesto(){

        if( jQuery('[name="opcao_manifesto"]:checked').length == 0 ){
            swal('Atenção!','É necessário selecionar uma opção de Manifestação de Interesse da UF.','warning');
        } else {
            var msg = "Eu li, e estou de acordo com o Termo de Manifestação.";

            jQuery('.btn-aceite-termomanifesto').button('loading');
            swal({
                    title: "Confirmar",
                    text: msg,
                    type: "success",
                    html: true,
                    showCancelButton: true,
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function (isConfirm) {
                    if (isConfirm) {
                        swal.close();
                        aceitarTermoManifesto( jQuery('[name="_domid"]').val() );
                    }
                }
            );
        }
    }

    function aceitarTermoManifesto( domid ){
        divCarregando();

        window.location.href = window.location+'&requisicao=aceitarTermoManifesto&domid='+domid+'&opcao='+jQuery('[name="opcao_manifesto"]:checked').val();
    }

    function imprimirTermo(){
        var domid = jQuery('[name="_domid"]').val();
//        window.location.href = window.location+'&requisicao=imprimir&domid='+domid;
/*
        var janela = window.open();
        janela.document.write($('#conteudo').html());
//        janela.document.write(resp);
        janela.document.close(); // necessary for IE >= 10
        janela.focus(); // necessary for IE >= 10*!/
        janela.print();
        janela.close();

        return;*/

        $.ajax({
            type: "POST",
            url: window.location.href,
            data: {requisicao: 'imprimir', domid: domid},
            async: false,
            success: function (resp) {
                $('title').html('Termo de Manifestação');
                var janela = window.open();
                janela.document.write($('#termo').html());
                janela.document.write(resp);
                janela.document.close(); // necessary for IE >= 10
                janela.focus(); // necessary for IE >= 10*/
                janela.print();
                janela.close();

            }
        });



    }
</script>