<?php

$fm = new Simec_Helper_FlashMessage('periodo');
$model = new Spoemendas_Model_Periodosolicitacao();

if (array_key_exists('requisicao', $_POST))
{

    switch ($_POST['requisicao'])
    {
        case 'salvarPeriodo':
            $_POST = simec_utf8_decode_recursive($_POST);
            $_POST['prsano'] = $_SESSION['exercicio'];
            $model->popularDadosObjeto($_POST);
            $erro = false;
            $msg = ['tipo' => '', 'texto' => ''];
            $js = '';

            if ($model->dataPreenchida())
            {
                $model->prsdatainicio = ajusta_data($model->prsdatainicio);
                $model->prsdatafim = ajusta_data($model->prsdatafim);

                $timeInicio = 0;
                $timeFinal = 0;

                try
                {
                    $prsdatainicio = new DateTime($model->prsdatainicio);
                    $timeInicio = $prsdatainicio->getTimestamp();
                    if ($timeInicio === false)
                    {
                        throw new Exception();
                    }
                } catch (Exception $e)
                {
                    $msg['texto'] .= 'A Data Início está com o formato incorreto.<br>';
                    $msg['tipo'] = Simec_Helper_FlashMessage::ERRO;
                    $erro = true;
                }

                try
                {
                    $prsdatafim = new DateTime($model->prsdatafim);

                    $timeFinal = $prsdatafim->getTimestamp();
                    if ($timeFinal === false)
                    {
                        throw new Exception();
                    }
                } catch (Exception $e)
                {
                    $msg['texto'] .= 'A Data Fim está com o formato incorreto.<br>';
                    $msg['tipo'] = Simec_Helper_FlashMessage::ERRO;
                    $erro = true;
                }

                if ($timeFinal - $timeInicio < 0)
                {
                    $msg['texto'] .= 'A data fim não pode ser menor que a data início.<br>';
                    $msg['tipo'] = Simec_Helper_FlashMessage::ERRO;
                    $erro = true;
                }
            }
            else
            {
                $msg['texto'] .= 'As datas início e fim são de preencimento obrigatórios.<br>';
                $msg['tipo'] = Simec_Helper_FlashMessage::ERRO;
                $erro = true;
            }

            if (empty($_POST['prsdescricao']))
            {
                $msg['texto'] .= 'A descrição deve ser preenchido.<br>';
                $msg['tipo'] = Simec_Helper_FlashMessage::ERRO;
                $erro = true;
            }

            if (empty($_POST['prsmes']))
            {
                $msg['texto'] .= 'O mês deve ser preenchido.<br>';
                $msg['tipo'] = Simec_Helper_FlashMessage::ERRO;
                $erro = true;
            }

            if (!$erro)
            {
                $model->salvar();
                $model->commit();
                $_POST = [];
                $msg['texto'] .= 'Período salvo com sucesso.<br>';
                $msg['tipo'] = Simec_Helper_FlashMessage::SUCESSO;
                $js = '<script>$(function(){ setTimeout(function(){ location.reload(); }, 1500); });</script>';
            }

            echo <<<HTML
<div class="alert alert-{$msg['tipo']} text-center col-md-12">
    <button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
{$msg['texto']}
</div>
{$js}
HTML;
            die;
            break;
    }
}

if ($id = filter_input(INPUT_GET, id, FILTER_SANITIZE_NUMBER_INT))
{
    header('Content-Type: application/json');
    echo simec_json_encode($model->encontraLinha($id));
    die(0);
}

/**
 * Cabecalho do SIMEC.
 *
 * @see cabecalho.inc
 */
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script type="text/javascript" src="jquery.livequery.js"></script>
<ol class="breadcrumb">
    <li>
        <a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C">
            <?php echo $_SESSION['sisdsc']; ?>
        </a>
    </li>
    <li>Configurar Período</li>
</ol>

<div class="col-xs-12">
    <?php
    echo $fm;

    bootstrapPopup(
        'Adicionar Períodos',
        'add-periodo-financeiro',
        dirname(__FILE__) . '/calendario_financeiro/form.inc',
        [],
        ['tamanho' => 'lg']
    );

    (new Simec_Listagem())
        ->setCabecalho([
            'Descrição',
            'Mês de Referência',
            'Período de Solicitação' => [
                'Início', 'Fim'
            ]
        ])
        ->addAcao('edit', 'editarPeriodo')
        ->setQuery((new Spoemendas_Model_Periodosolicitacao())
            ->recuperarTodos(
                'prsid, prsdescricao, to_date (prsmes || \'/\' || prsano, \'MM/YYYY\') mes, TO_CHAR(prsdatainicio, \'DD/MM/YYYY\') AS prsdatainicio, TO_CHAR(prsdatafim, \'DD/MM/YYYY\') AS prsdatafim',
                ["prsano = '{$_SESSION['exercicio']}'"],
                'prsano , prsmes',
                ['query' => true]
            ))
        ->addToolbarItem(Simec_Listagem_Renderer_Html_Toolbar::ADICIONAR)
        ->addCallbackDeCampo('mes', 'mascaraMesCurtoAno')
        ->setListaColunas([
            'psrdescricao',
            'mes',
            'prsdatainicio',
            'prsdatafim'
        ])
        ->render(Simec_Listagem::SEM_REGISTROS_LISTA_VAZIA);

    ?>
</div>
<script type="text/javascript">
    var editarPeriodo = function editarPeriodo(id) {
        if (id) {

            $("#add-periodo-financeiro").modal("show");

            $.ajax({
                url: 'spoemendas.php?modulo=principal/financeiro/calendario_financeiro&acao=A&id=' + id,
                success: function (data) {

                    $("#add-periodo-financeiro").find(".modal-body .well").find(".alert-danger").remove();

                    $("#periodo_prsid").val(data.prsid);
                    $("#periodo_prsdescricao").val(data.prsdescricao);
                    $("#periodo_prsdatainicio").val(data.prsdatainicio);
                    $("#periodo_prsdatafim").val(data.prsdatafim);
                    $("#periodo_prsmes").val(data.prsmes).trigger('chosen:updated');

                    $("#add-periodo-financeiro").modal("show");
                },
                complete: function () {
                }
            });
        }
    }

    $(function () {
        $(".form-listagem .navbar-listagem")
            .find(".btn-adicionar")
            .on("click", function (e) {

                $("#periodo_prsid").val("");
                $("#periodo_prsdescricao").val("");
                $("#periodo_prsdatainicio").val("");
                $("#periodo_prsdatafim").val("");
                $("#periodo_prsmes").val("").trigger('chosen:updated');

                $("#add-periodo-financeiro").find(".modal-body .well").find(".alert-danger").remove();
                $("#add-periodo-financeiro").modal("show");
            });

        $("#periodo")
            .find(".btn-salvar")
            .on("click", function (e) {
                e.preventDefault();

                $.ajax({
                    url: 'spoemendas.php?modulo=principal/financeiro/calendario_financeiro&acao=A',
                    type: 'post',
                    data: {
                        requisicao: 'salvarPeriodo',
                        prsid: $("#periodo_prsid").val(),
                        prsdescricao: $("#periodo_prsdescricao").val(),
                        prsdatainicio: $("#periodo_prsdatainicio").val(),
                        prsdatafim: $("#periodo_prsdatafim").val(),
                        prsmes: $("#periodo_prsmes").val()
                    },
                    success: function (data) {
                        var $bodyContainer = $("#add-periodo-financeiro").find(".modal-body .well");
                        $bodyContainer.find(".alert-danger").remove();
                        $bodyContainer.prepend(data);
                    },
                    complete: function () {
                    }
                });
            });
    });
</script>

