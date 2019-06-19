<?php
    $controleUnidade = new Par3_Controller_InstrumentoUnidade();
    $pne = new Par3_Controller_Pne();
    $modelUnidade = new Par3_Model_InstrumentoUnidade();

    require APPRAIZ . 'includes/cabecalho_bootstrap_v3_limpo.inc';

    $estuf = $_SESSION['par']['estuf'];
    $muncod = $_SESSION['par']['muncod'];
    $itrid = $_SESSION['par']['itrid'];

    $metas = new Metas();
    $indicadores = $metas->retornarIndicadoresPendentes($estuf, $muncod, $itrid);
    $indicadores = $indicadores ? $indicadores : array();

    echo $simec->title ( 'PAR - Lista de Indicadores Pendentes' );
?>
<div class="row">
    <div class="ibox float-e-margins">
        <div class="ibox-content">
            <table class="table table-striped table-bordered table-hover dataTablePendencias">
                <thead>
                <tr>
                    <th>Meta</th>
                    <th>Indicador</th>
                </tr>
                </thead>
                <tbody>
                    <?php if(is_array($indicadores)) : foreach ($indicadores as $indicador) : ?>
                    <tr>
                        <td><?php echo $indicador['metid']; ?></td>
                        <td><?php echo $indicador['subtitulo']; ?></td>
                    </tr>
                    <?php endforeach; endif; ?>
                </tbody>
            </table>
        </div>
    </div>
</div>

<script>
    $(document).ready(function() {
        if (!$.fn.DataTable.isDataTable('.dataTablePendencias')) {
            $('.dataTablePendencias').DataTable({
                'bFilter': false,
                'bInfo': true,
                'bSort': false,
                'bPaginate': false,
                'aoColumnDefs' : [{
                    'bSortable' : false,
                    'aTargets' : [ 'unsorted' ]
                }],
                'oLanguage' : {
                    'sProcessing' : "Processando...",
                    'sLengthMenu' : "Mostrar _MENU_ registros",
                    'sZeroRecords' : "N&atilde;o foram encontrados resultados",
                    'sInfo' : "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
                    'sInfoEmpty' : "Mostrando de 0 at&eacute; 0 de 0 registros",
                    'sInfoFiltered' : "(filtrado de _MAX_ registros no total)",
                    'sInfoPostFix' : ".",
                    'sSearch' : "Pesquisar :&nbsp;&nbsp;",
                    'sUrl' : "",
                    'oPaginate' : {
                        'sFirst' : "Primeiro",
                        'sPrevious' : "Anterior",
                        'sNext' : "Seguinte",
                        'sLast' : "&Uacute;ltimo"
                    }
                }
            });
        }
    })

</script>

<?php require APPRAIZ . 'includes/rodape_bootstrap_v3_limpo.inc'; ?>
