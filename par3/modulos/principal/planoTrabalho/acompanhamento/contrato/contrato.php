<?php
/**
 * Nesta tela de acompanhamento, todas as abas são carregadas de uma só vez.
 * Por este motivo, para não ficar pesado, tudo é feito através de requisições ajax, para que a tela não precise recarregar completamente.
 * (So vai recarregar nas tramitassões do workflow)
 * O controle das abas é feito só pelo html/bootstrap
 * Foi escolhido assim para que o sistema ficasse mais rápido aos olhos do usuário
 */

$inuid = (int) $_GET['inuid'];
$proid = (int) $_GET['proid'];

if (empty($proid) && $proid <= 0) {
    simec_redirecionar('par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=documentos&inuid='.$inuid, 'warning', 'Por favor, selecione um termo para acompanhamento.');
}

$controllerExecucaoContrato = new Par3_Controller_ExecucaoContrato();
$controllerDocumentoTermo = new Par3_Controller_DocumentoTermo();
$controllerDocumentoLicitacao = new Par3_Controller_ExecucaoDocumentosLicitacao();
$controllerDesembolso = new Par3_Controller_SolicitacaoDesembolso();
$controllerExecucaoNotaFiscal = new Par3_Controller_ExecucaoNotaFiscal();
//$controllerExecucaoDetalhamento = new Par3_Controller_ExecucaoDetalhamento();
//$controllerExecucaoPagamentos = new Par3_Controller_ExecucaoPagamentos();

/*
 * Dados gerais, utilizados em todas as abas
 */
$dadosProcesso = $controllerExecucaoContrato->recuperarProcesso($proid);
$dotnumero = $dadosProcesso['dotnumero'];
$processo = $dadosProcesso['pronumeroprocesso'];
$num = somenteNumeros($processo);
$processoFormatado = $num ? substr($processo, 0, 5) . '.' . substr($processo, 5, 6) . '/' . substr($processo, 11, 4) . '-' . substr($processo, 15, 2) : $processo;
$tipoObjeto = $dadosProcesso['intoid'];
$dotid = $dadosProcesso['dotid'];

if (isset($_POST['funcao']) && isset($_POST['requisicao'])) {
    ob_clean();
    switch ($_POST['funcao']) {
        case 'contrato':
            exit($controllerExecucaoContrato->controlarRequisicoes($_POST['requisicao'], $proid, $dadosProcesso));
            break;
        case 'documentoslicitacao':
            exit($controllerDocumentoLicitacao->controlarRequisicoes($_POST['requisicao'], $proid));
            break;
        case 'notasfiscais':
            exit($controllerExecucaoNotaFiscal->controlarRequisicoes($_POST['requisicao'], $proid));
            break;
        case 'desembolso':
            exit($controllerDesembolso->controlarRequisicoes($_POST['requisicao']));
            break;
        case 'detalhamento':
//            exit($controllerExecucaoDetalhamento->controlarRequisicoes($_POST['requisicao'], $proid));
            break;
        case 'pagamentos':
//            exit($controllerExecucaoPagamentos->controlarRequisicoes($_POST['requisicao'], $proid));
            break;
    }
} elseif (isset($_POST['requisicao'])) {
    ob_clean();
    switch ($_POST['requisicao']) {
        //requisições dos accordions

        //documentos_licitacao
        case 'carregalistadcumentos':
            $controllerDocumentoLicitacao->listarItensComposicaoPorIniciativa($_POST['id']);
            break;
        case 'listaItensDocumentoEdicao':
            $controllerDocumentoLicitacao->listaItensDocumentoEdicao($_REQUEST['id']);
            break;
        case 'listaItensAnalise':
            $controllerDocumentoLicitacao->listaItensDocumentoEdicao($_REQUEST['id'], true);
            break;
        case 'listaItensDocumentoInclusao':
            $controllerDocumentoLicitacao->listaItensDocumentoInclusao($_REQUEST['id']);
            break;

        //_edit_contrato
        case 'contratolistaitens':
            $controllerExecucaoContrato->listaItens($_POST['id']);
            break;

        //_edit_notas_fiscais
        case 'listaItensContratoNotaFiscal':
            $controllerExecucaoNotaFiscal->listarItensPorContrato($_POST['id']);
            break;

        //requisições gerais
        case 'getPessoaJuridica':
            $return = [];
            require_once APPRAIZ . 'www/includes/webservice/PessoaJuridicaClient.php';
            $pessoaJuridica = new PessoaJuridicaClient("http://ws.mec.gov.br/PessoaJuridica/wsdl");

            $pj = str_replace(array('/', '.', '-'), '', $_POST['cnpj']);
            $xml = $pessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj($pj);
            $xml = str_replace(array("& "), array("&amp; "), $xml);
            $obj = (array) simplexml_load_string($xml);

            if ($obj['PESSOA']->ERRO) {
                $return['sucesso'] = false;
                $return['mensagem'] = 'CNPJ inexistente na base da Receita Federal.';
            } else {
                $empresa  = (array) $obj['PESSOA'];
                $endereco = (array) $obj['PESSOA']->ENDERECOS->ENDERECO;

                $return['sucesso'] = true;
                $return['empresa'] = array_merge($empresa, $endereco);
            }

            die(simec_json_encode($return));
            break;
        case 'imprimirTermoCompromisso':
            $controllerDocumentoTermo->imprimirTermoPDF($_REQUEST['proid']);
            break;
        case 'visualizarTermoPar3':
            $mdoconteudo = $controllerDocumentoTermo->pegaTermoCompromissoArquivo($_REQUEST['dotid']);
            $mdoconteudo = str_ireplace('\"', '"', $mdoconteudo);
            echo simec_html_entity_decode($mdoconteudo);

            exit();
            break;
    }
    exit;
} elseif (isset($_GET['reqdownload'])) {
    switch ($_GET['reqdownload']){
        case 'download':
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            $file = new FilesSimec();
            $file->getDownloadArquivo($_GET['arqid']);
            die;
            break;
        case 'excelescolastermo':
            ob_clean();
            header('content-type: text/html; charset=utf-8');
            $controllerExecucaoContrato->gerarExcelEscolasTermo($proid, $_GET['inuid']);
            die;
            break;
    }
}

$url = 'par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=acompanhamento';
$db->cria_aba(58015, $url, '&inuid=' . $inuid, array());
?>

<style>
    .panel-group .panel-default:hover{
        background-color: #e4e4e4;
    }
</style>

<div class="row">
    <div class="col-lg-12">
        <h2>Acompanhamento</h2>
        <ol class="breadcrumb" style="background-color: transparent;">
            <!--<li>
                <a href="#">Execução</a>
            </li>
            <li>
                <a href="#">Documentos do PAR</a>
            </li>-->
            <li>
                Processo Nº <strong><?= $processoFormatado ?></strong>
            </li>
            <li>
                Visualizar Termo <span class="badge termo_detalhe" id="<?= $dotid ?>"><a><i class="glyphicon glyphicon-search"></i> <?= $dotnumero ?></a>
            </li>
        </ol>
    </div>
</div>

<br>

<div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">

    <div class="tabs-container">

        <ul class="nav nav-tabs" role="tablist">
            <li><a href="#escolas" aria-controls="escolas" role="tab" data-toggle="tab"><i class="fa fa-home"></i> ESCOLAS</a></li>
            <li><a href="#documentoslicitacao" aria-controls="documentoslicitacao" role="tab" data-toggle="tab"><i class="fa fa-file-text-o" aria-hidden="true"></i> Documentos de Licitação</a></li>
            <li  class="active"><a href="#contrato" aria-controls="contrato" role="tab" data-toggle="tab"><i class="fa fa-file-text-o" aria-hidden="true"></i> CONTRATO</a></li>
            <!--            <li><a href="#monitoramento" aria-controls="monitoramento" role="tab" data-toggle="tab"><i class="fa fa-binoculars" aria-hidden="true"></i> MONITORAMENTO</a></li>-->
            <li><a href="#notas_fiscais" aria-controls="notas_fiscais" role="tab" data-toggle="tab"><i class="fa fa-book" aria-hidden="true"></i> NOTAS FISCAIS</a></li>
<!--            <li><a href="#detalhamento" aria-controls="detalhamento" role="tab" data-toggle="tab"><i class="fa fa-info" aria-hidden="true"></i>DETALHAMENTO DO SERVIÇO / ITEM</a></li>-->
<!--            <li><a href="#pagamentos" aria-controls="pagamentos" role="tab" data-toggle="tab"><i class="fa fa-dollar" aria-hidden="true"></i>PAGAMENTOS EFETUADOS</a></li>-->
<!--            <li><a href="#pendencias_finalizar" aria-controls="pendencias_finalizar" role="tab" data-toggle="tab">PENDÊNCIAS/FINALIZAR</a></li>-->
        </ul>

        <div class="tab-content">

            <div role="tabpanel" class="tab-pane active" id="contrato">

                <div class="ibox">
                    <div class="ibox-content">

                        <ul class="nav nav-tabs" role="tabListContrato">
                            <li class="active"><a href="#lista_contrato" aria-controls="contrato" role="tab" data-toggle="tab"><i class="fa fa-list-alt" aria-hidden="true"></i> lista</a></li>
                            <li><a href="#add_contrato" aria-controls="add_contrato" role="tab" data-toggle="tab"><i class="fa fa-pencil-square-o" aria-hidden="true"></i> adicionar/editar contrato</a></li>
                        </ul>

                        <div class="tab-content">

                            <div role="tabListContrato" class="tab-pane active" id="lista_contrato">
                                <?php require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/contrato/_list_contrato.php' ?>
                            </div>

                            <div role="tabListContrato" class="tab-pane" id="add_contrato">
                                <?php require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/contrato/_edit_contrato.php' ?>
                            </div>

                        </div>

                    </div>
                </div>

            </div>

            <div role="tabpanel" class="tab-pane" id="notas_fiscais">
                <div class="ibox">
                    <div class="ibox-content">

                        <ul class="nav nav-tabs" role="tabListNotasFiscais">
                            <li class="active">
                                <a href="#lista_notas_fiscais" aria-controls="lista_notas_fiscais" role="tab" data-toggle="tab">
                                    <i class="fa fa-list-alt" aria-hidden="true"></i> lista
                                </a>
                            </li>
                            <li>
                                <a href="#add_nota_fiscal" aria-controls="add_nota_fiscal" role="tab" data-toggle="tab">
                                    <i class="fa fa-pencil-square-o" aria-hidden="true"></i> adicionar/editar nota fiscal
                                </a>
                            </li>
                        </ul>

                        <div class="tab-content">
                            <div role="tabListNotasFiscais" class="tab-pane active" id="lista_notas_fiscais">
                                <?php require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/contrato/_list_notas_fiscais.php' ?>
                            </div>

                            <div role="tabEditNotasFiscais" class="tab-pane" id="add_nota_fiscal">
                                <?php require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/contrato/_edit_notas_fiscais.php' ?>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <div role="tabpanel" class="tab-pane " id="documentoslicitacao">
                <?php require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/contrato/_list_documentos_licitacao.php' ?>
            </div>

            <div role="tabpanel" class="tab-pane " id="escolas">
                <div class="ibox float-e-margins">

                    <div class="ibox-title text-center">
                        <h2><i class="fa fa-list-alt" aria-hidden="true"></i> Lista de Escolas</h2>
                    </div>

                    <div class="ibox-content">
                            <!-- Conteúdo carregado por ajax, função atualizarListaEscolasTermo (Controller/ExecucaoContrato) -->
                        <?php require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/contrato/_list_escolas_termo.php' ?>
                    </div>
                </div>
            </div>

            <div role="tabpanel" class="tab-pane " id="pagamentos">
<!--                --><?php //require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/contrato/_list_pagamentos.php' ?>
            </div>

            <div role="tabpanel" class="tab-pane " id="detalhamento">
                <div class="ibox float-e-margins">

                    <div class="ibox-title text-center">
                        <h2><i class="fa fa-list-alt" aria-hidden="true"></i> Detalhamento do serviço / item</h2>
                    </div>

                    <div class="ibox-content">
                        <!-- Conteúdo carregado por ajax, função atualizarListaEscolasTermo (Controller/ExecucaoContrato) -->
<!--                        --><?php //require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/contrato/_list_detalhamento.php' ?>
                    </div>
                </div>
            </div>

        </div>
    </div>
</div>

<!-- Modal Termo de Compromisso-->
<div class="ibox float-e-margins animated modal conteudo" id="modal-visualiza-termo" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%" id="printable">
        <div class="modal-content">
            <div class="ibox-footer notprint" style="text-align: center;">
                <button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
                <button type="button" id="btn-imprimir" data-dismiss="modal" class="btn btn-default" onclick="imprimirTermoCompromisso();"><i class="fa fa-times-circle-o"></i> Imprimir</button>
            </div>
            <div class="ibox-content">
                <div class="row">
                    <div class="ibox-content" >
                        <?php echo $controllerDocumentoTermo->montaHtmlTermo(); ?>
                    </div>
                </div>
            </div>
            <div class="ibox-footer notprint" style="text-align: center;">
                <button type="button" id="btn-fechar-modelo" data-dismiss="modal" class="btn btn-default"><i class="fa fa-times-circle-o"></i> Fechar</button>
                <button type="button" id="btn-imprimir" data-dismiss="modal" class="btn btn-default" onclick="imprimirTermoCompromisso();"><i class="fa fa-times-circle-o"></i> Imprimir</button>
            </div>
        </div>
    </div>
</div>

<script>
    $(document).ready(function () {
        //visualizar termo
        $('body').on('click','.termo_detalhe', function(){
            visualizarTermoPar3(this.id);
         });

    });

    function visualizarTermoPar3(id){

        var caminho = window.location.href;
        var action  = '&requisicao=visualizarTermoPar3&dotid='+id;
        jQuery.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                jQuery("#div_conteudo_termo").html(resp);
            }
        });
        jQuery("#modal-visualiza-termo").modal("show");
    }

    function imprimirTermoCompromisso(){
        window.location.href = window.location.href+'&requisicao=imprimirTermoCompromisso';
    }

    /* * * * FUNÇÕES DE APOIO * * * */

    //utilizadas em _list_contrato, _edit_contrato, _edit_notas_fiscais, _list_documentos_licitacao
    function moedaParaFloat(numero) {
        var num = numero;
        if(typeof num != "undefined") {
            num = num.replace('.', '');
            num = num.replace('.', '');
            num = num.replace('.', '');
            num = num.replace('.', '');
            num = num.replace(',', '.');
            return parseFloat(num).toFixed(2);
        }
        return num;
    }

    function floatParaMoeda(num) {
        var num = parseFloat(num).toFixed(2);
        var x = 0;
        if (num < 0) {
            num = Math.abs(num);
            x = 1;
        }
        if (isNaN(num))
            num = "0";
        cents = Math.floor((num * 100 + 0.5) % 100);

        num = Math.floor((num * 100 + 0.5) / 100).toString();

        if (cents < 10)
            cents = "0" + cents;
        for (var i = 0; i < Math.floor((num.length - (1 + i)) / 3); i++)
            num = num.substring(0, num.length - (4 * i + 3)) + '.'
                + num.substring(num.length - (4 * i + 3));
        ret = num + ',' + cents;
        if (x == 1)
            ret = ' - ' + ret;
        return ret;
    }
</script>

<!-- Modal Solicitação desembolso usada em _list_contrato.php e _list_documentos_licitacao.php -->

<div class="ibox float-e-margins animated modal" id="modal_solicitacao" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%;">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                <h3 id="modal_titulo_solicitacao"></h3>
            </div>
            <div class="ibox-content" id="conteudo_modal_solicitacao">
                <!-- Conteúdo -->
            </div>
            <div class="ibox-footer center" id="footerModalSolicitacao">
                <button type="submit"
                        data-dismiss="modal"
                        id="btnFecharModalSolicitacao"
                        class="btn btn-default center">
                    <i class="fa fa-times-circle-o"></i> Fechar
                </button>
            </div>
        </div>
    </div>
</div>
