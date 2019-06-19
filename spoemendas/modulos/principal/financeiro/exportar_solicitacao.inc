<?php
include_once APPRAIZ . 'includes/library/vendor/phpoffice/phpexcel/Classes/PHPExcel.php';



if ($get = filter_input(INPUT_GET, 'get')) {
    if ($get = 'xls') {
        $params = explode('-', $_GET['esdidPer']);

        (new Spoemendas_Model_Solicitacaofinanceira())->exportarXlsx($params[1], $params[0]);
        exit(0);
    }
}

if ($esdidPer = filter_input(INPUT_GET, 'esdidPer')) {
    $params = explode('-', $esdidPer);
    $solicitacao = new Spoemendas_Model_Solicitacaofinanceira();

    $periodoLinha = (new Spoemendas_Model_Periodosolicitacao())->encontraLinha($params[0]);
    $total = $solicitacao->getTotal();
    $totalPorEstado = $solicitacao->getTotalPorEstado($params[1]);
    $label = ($params[1] == ESD_ENVIADO)?'Recebido':'Pendentes de Envio';

    echo <<<HTML
        <div class="panel panel-default">
            <div class="panel-heading">
                <h3 class="panel-title">Informações dos registros recebidos</h3>
            </div>
            <div class="panel-body">
                <table class="table">
                    <tr>
                        <td colspan="2">
                            <strong>Período de Solicitação:</strong> {$periodoLinha['prsdescricao']} {$periodoLinha['prsdatainicio']} a {$periodoLinha['prsdatafim']}
                        </td>
                    </tr>
                    <tr>
                        <td><strong>Cadastrados:</strong> {$total}</td>
                        <td><strong>{$label}:</strong> {$totalPorEstado}</td>
                    </tr>
                </table>
            </div>
        </div>
HTML;

    $solicitacao->gridSolicitacoesResumidas($params[0],$params[1]);
    die;
}

include APPRAIZ . 'includes/cabecalho.inc';
?>
<style type="text/css">
    .modal-lg{width:70%!important}
</style>
<!-- Apreseta o breadcump -->
<ol class="breadcrumb">
	<li>
		<a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C">
			<?php echo $_SESSION['sisdsc']; ?>
		</a>
	</li>
	<li>Exportar Solicitação</li>
</ol>

<?php
// Tratamento de abas
// verifica qual a aba está ativa
$abaAtiva = empty($_REQUEST['aba'])
    ? 'spoemendas.php?modulo=principal/financeiro/exportar_solicitacao&acao=A&aba=rec'
    : "spoemendas.php?modulo=principal/financeiro/exportar_solicitacao&acao=A&aba={$_REQUEST['aba']}";

// nomeia as abas e configura suas urls
$arrAbas = [
    ['descricao' => 'Recebidos',
    'link' => 'spoemendas.php?modulo=principal/financeiro/exportar_solicitacao&acao=A&aba=rec'],
    ['descricao' => 'Pendentes de Envio',
    'link' => 'spoemendas.php?modulo=principal/financeiro/exportar_solicitacao&acao=A&aba=ped']
];

$esdid = (empty($_REQUEST['aba']) || ($_REQUEST['aba'] == 'rec')) ? ESD_ENVIADO : ESD_NAO_ENVIADO;

// apresenta abas na tela
echo montarAbasArray($arrAbas, $abaAtiva, true, false);
?>

<div class="row">
	<div class="col-lg-12">
		<div class="well">
			<?php
            $coluna = ($_REQUEST['aba'] == 'ped') ? 'Pendentes de Envio' : 'Recebidos';
            $solicitacaoFinanceira = new Spoemendas_Model_Solicitacaofinanceira();
            $solicitacaoFinanceira->buscaTotalPorEstadoPeriodo($esdid, $_SESSION['exercicio'], $coluna);
            ?>
		</div>
	</div>
</div>

<div id="largeModal" class="modal fade" tabindex="-1" role="dialog">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
                <h4 class="modal-title">Detalhamento</h4>
            </div>
            <div class="modal-body">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
function xls(id) {
    location.href = 'spoemendas.php?modulo=principal/financeiro/exportar_solicitacao&acao=A&get=xls&esdidPer='+id
}

function info(id) {
    $.ajax({
        url: 'spoemendas.php?modulo=principal/financeiro/exportar_solicitacao&acao=A&esdidPer='+id,
        type: 'post',
        beforeSend: function() {

        },
        success: function(data) {
            $('#largeModal').find('.modal-body').html(data);
            $('#largeModal').modal('show');
        }
    });
}
</script>
