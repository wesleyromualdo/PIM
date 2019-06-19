<?php

include_once APPRAIZ . 'spo/ws/siop/Qualitativo.php';
include_once APPRAIZ . 'spo/ws/siop/Quantitativo.php';

function chamarWebService($log, array $filtros, array $camposDeRetorno, $pagina = 0, Simec_Helper_FlashMessage $fm, $db)
{
    $registrosPorPagina = 2000;

    // -- Filtros da consulta
    $filtroExecucaoOrcamentaria = new FiltroExecucaoOrcamentariaDTO();
    foreach ($filtros as $campo => $valor) {
        if (!property_exists($filtroExecucaoOrcamentaria, $campo)) {
            throw new Exception("O filtro '{$campo}' não é válido para o método WSQuantitativo::consultarExecucaoOrcamentaria.");
        }

        $filtroExecucaoOrcamentaria->$campo = $valor;
    }

    // -- Retorno da consulta
    $selecaoRetornoExecucaoOrcamentaria = new SelecaoRetornoExecucaoOrcamentariaDTO();
    foreach ($camposDeRetorno as $ret) {
        if (!property_exists($selecaoRetornoExecucaoOrcamentaria, $ret)) {
            throw new Exception("O retorno '{$ret}' não é válido para o método WSQuantitaivo::consultarExecucaoOrcamentaria.");
        }
        $selecaoRetornoExecucaoOrcamentaria->$ret = true;
    }

    $paginacao = new PaginacaoDTO();
    $paginacao->pagina = $pagina;
    $paginacao->registrosPorPagina = $registrosPorPagina;

    $retorno = (new Spo_Ws_Siop_Quantitativo())
        ->consultarExecucaoOrcamentaria($filtroExecucaoOrcamentaria, $selecaoRetornoExecucaoOrcamentaria, $paginacao, $log)
        ->return;

    $execucaoOrcamentarias = $retorno->execucoesOrcamentarias[0]->execucaoOrcamentaria;

// -- Falha ao executar a consulta
    if ($retorno->sucesso != 'true') {
        if (!is_array($retorno->mensagensErro)) {
            $fm->addMensagem($retorno->mensagensErro, Simec_Helper_FlashMessage::ERRO);
        } else {
            foreach ($retorno->mensagensErro as $mensagem) {
                $fm->addMensagem($mensagem, Simec_Helper_FlashMessage::ERRO);
            }
        }

        echo simec_json_encode(['terminate' => true, 'erros' => $retorno->mensagensErro]);
        die();
    }

// -- processando o retorno da consulta
    $numRegistros = count($execucaoOrcamentarias);

    if ($numRegistros) {

        $queryApagar = "DELETE FROM wssof.ws_execucaoorcamentariadto WHERE anoexercicio = '{$filtros['anoExercicio']}'";
        $db->executar($queryApagar);
        $db->commit();

        $insert = '';
        foreach ($execucaoOrcamentarias as $execucao) {
            $valores = [];
            foreach ($camposDeRetorno as $campo) {
                $valores[] = "'{$execucao->$campo}'";
            }
            $insert .= 'INSERT INTO wssof.ws_execucaoorcamentariadto(' . implode(', ', $camposDeRetorno) . ') VALUES(' . implode(', ', $valores) . ');';
        }
        $db->executar(($insert));
        $db->commit();
    }

    if ($numRegistros < $registrosPorPagina) {
        echo simec_json_encode(['terminate' => true, 'numRegistros' => $numRegistros]);
        die();
    } else {
        echo simec_json_encode(['terminate' => false, 'pagina' => $pagina, 'numRegistros' => $numRegistros]);
        die();
    }
}

/**
 * Arquivo de execução da consulta da execução orçamentária no SIOP.
 *
 * $Id: consultarExecucaoOrcamentaria.inc 148351 2019-01-25 15:29:16Z juniosantos $
 * @filesource
 */

$fm = new Simec_Helper_FlashMessage('consultar-execucao-orcamentaria');

if (isset($_POST['requisicao']) && 'consultarExecucaoOrcamentaria' == $_POST['requisicao']) {

    $dadosForm = $_POST['consultarexecucaoorcamentaria'];
    $log = isset($dadosForm['log']) ? true : false;
    unset($dadosForm['log']);

    $camposDeRetorno = $dadosForm['selecaoretorno'];
    unset($dadosForm['selecaoretorno']);

    $pagina = $dadosForm['pagina'];
    unset($dadosForm['pagina']);

    $apagar = $dadosForm['apagar'];
    unset($dadosForm['apagar']);

    $filtros = $dadosForm;

    if (key_exists('unidadesorcamentarias', $filtros)) {
        $filtros['unidadesOrcamentarias'] = $filtros['unidadesorcamentarias'];
        unset($filtros['unidadesorcamentarias']);
    }

    chamarWebService($log, $filtros, $camposDeRetorno, $pagina, $fm, $db);
    return true;
}

if ($_GET['exportar']) {

    function ZeroAEsquerda($dado)
    {
        list(, , , $extrainfo) = func_get_args();
        switch ($extrainfo['campo']) {
            case 'acao':
            case 'localizador':
            case 'planoorcamentario':
            case 'programa':
                return '"' . str_pad($dado, 4, '0') . '"';
        }
    }

    $cabecalho = [
        'acao',
        'acompanhamentopo',
        'anoexercicio',
        'autorizado',
        'bloqueadoremanejamento',
        'bloqueadosof',
        'categoriaeconomica',
        'creditocontidosof',
        'detalheacompanhamentopo',
        'disponivel',
        'dotatual',
        'dotinicialsiafi',
        'dotacaoantecipada',
        'dotacaoinicial',
        'elementodespesa',
        'empliquidado',
        'empenhadoaliquidar',
        'esfera',
        'fonte',
        'funcao',
        'gruponaturezadespesa',
        'identificadoracompanhamentopo',
        'idoc',
        'iduso',
        'indisponivel',
        'localizador',
        'modalidadeaplicacao',
        'natureza',
        'pago',
        'planointerno',
        'planoorcamentario',
        'programa',
        'projetolei',
        'rapapagarnaoprocessado',
        'rapapagarprocessado',
        'rapcanceladosnaoprocessados',
        'rapcanceladosprocessados',
        'rapexerciciosanteriores',
        'rapinscritonaoprocessado',
        'rapinscritoprocessado',
        'rapnaoprocessadoaliquidar',
        'rapnaoprocessadobloqueado',
        'rapnaoprocessadoliquidadoapagar',
        'rappagonaoprocessado',
        'rappagoprocessado',
        'resultadoprimarioatual',
        'resultadoprimariolei',
        'subelementodespesa',
        'subfuncao',
        'tematicapo',
        'tipoapropriacaopo',
        'tipocredito',
        'unidadegestoraresponsavel',
        'unidadeorcamentaria',
        'anoreferencia',
        'executadoporinscricaoderap',
        'origem',
        'numeroptres',
        'dataultimaatualizacao',
        'numeroemenda'
    ];

    (new Simec_Listagem(Simec_Listagem::RELATORIO_XLS))
        ->setCabecalho($cabecalho)
        ->addCallbackDeCampo(['acao', 'localizador', 'planoorcamentario', 'programa'], 'ZeroAEsquerda')
        ->setQuery('SELECT * FROM wssof.ws_execucaoorcamentariadto')
        ->render();
    die();
}

/**
 * Cabeçalho padrão do Simec.
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<div class="row col-md-12">
    <script type="text/javascript">
        var loadingSimec = true;

        function arrumaLoading() {
            if (loadingSimec) {
                $("#loading").show();
                setTimeout(arrumaLoading, 100);
            }
        }

        $(function () {
            $('.btn-exportar').click(function () {
                window.open('/spo/spo.php?modulo=integracao/sof/quantitativo/consultarExecucaoOrcamentaria&acao=A&exportar=true');
            });
        });

        function consultarExecucaoOrcamentaria() {

            var dados = $('#consultarexecucaoorcamentaria').serialize();
            $('#progress').show();

            loadingSimec = true;
            setTimeout(arrumaLoading, 100);

            $.post(window.location.href, dados, function (data) {
                if (!data.terminate) {
                    console.log(data);
                    $('#consultarexecucaoorcamentaria_pagina').val(
                        parseInt(data.pagina) + 1
                    );
                    $('#progress-pagina')
                        .text($('#consultarexecucaoorcamentaria_pagina').val() + ' Página(s)')
                        .css('width', parseInt($('#consultarexecucaoorcamentaria_pagina').val()) * 3 + '%');
                    consultarExecucaoOrcamentaria();

                } else {
                    if (data.erros) {
                        bootbox.alert(data.erros, function () {
                            window.location.assign(window.location.href);
                        });
                    }
                    $('#progress').hide();
                    $('#progress-pagina').text('Carregando');

                    $('#consultarexecucaoorcamentaria_pagina').val(0);
                    $('#consultarexecucaoorcamentaria_mes').change();

                    loadingSimec = false;
                }

            }, 'json').fail(function (res) {
                console.log('Erro na requisição: ' + res);
                loadingSimec = false;
            });
        }

        function onConsultarexecucaoorcamentariaSubmit(e) {
            if (!$('#consultarexecucaoorcamentaria_selecaoRetorno').val()) {
                alert('O campo "Campos de retorno" não pode ser deixado em branco.');
                return false;
            }

            e.preventDefault();
            consultarExecucaoOrcamentaria();
        }
    </script>
    <?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Integração')
        ->add('Sof Quantitativo')
        ->add('Consultar Execução Orçamentária')
        ->render();

    echo $fm;
    ?>
    <div class="progress" id="progress" style="height:25px;display:none">
        <div class="progress-bar progress-bar-striped .active" id="progress-pagina" role="progressbar"
             aria-valuenow="60" aria-valuemin="5" aria-valuemax="100" style="min-width:5%;padding-top:6px">
            1 Página(s)
        </div>
    </div>
    <?php
    $colunasRetorno = Spo_Ws_Sof_QuantitativoMap::getComponentesSelecaoRetornoExecucaoOrcamentariaDTO();
    $dados = [];
    foreach ($colunasRetorno as $coluna) {
        $dados[] = ['codigo' => $coluna, 'descricao' => $coluna];
    }
    $multiple = ['multiple' => true];

    $form = new Simec_View_Form('consultarexecucaoorcamentaria');
    $form->carregarDados($_GET['consultarexecucaoorcamentaria'])
        ->addCombo('Campos de retorno', 'selecaoRetorno', $dados, $multiple)
        ->addCombo('Unidade(s) Orçamentária(s)', 'unidadesOrcamentarias', Spo_Model_Unidade::queryCombo(), $multiple)
        ->addCombo('Apagar', 'apagar', [
            ['codigo' => 't', 'descricao' => 'Todos'],
            ['codigo' => 'e', 'descricao' => 'Exercicio'],
        ])
        ->addCheckbox('Registrar log do serviço?', 'log')
        ->addBotoes(['limpar', 'importar', 'exportar'])
        ->addHidden('anoExercicio', $_SESSION['exercicio'])
        ->addHidden('pagina', 0)
        ->setRequisicao('consultarExecucaoOrcamentaria')
        ->render();
    ?>
</div>