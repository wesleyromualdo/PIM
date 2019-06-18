<?php
$totalEstadosPagamento        = 0;
$totalEstadosEmpenho          = 0;
$totalEstadosPercentPagamento = 0;
$totalEstadosPercentEmpenho   = 0;

$totalRegiaoPagamento        = 0;
$totalRegiaoEmpenho          = 0;
$totalRegiaoPercentPagamento = 0;
$totalRegiaoPercentEmpenho   = 0;
?>

<?php if (!empty($this->arEstados)):?>
        <?php if($this->superuser):?>
            <div class="pull-right">
                <button class="btn btn-info btn-sm" id="btn-sql-lista-regioes" title="SQL">
                    <i class="fa fa-database"></i>
                </button>
            </div>
            <div class="row">
                <div class="col-lg-12" style="display: none;" id="div-sql-lista-regioes">
                    <pre id="sql-modal-lista-regioes"><?= $this->sql;?></pre>
                </div>
            </div>
            <script>
                $('#btn-sql-lista-regioes').on('click',function () {
                    $('#div-sql-lista-regioes').slideToggle();
                });
            </script>
        <?php endif;?>
    <br>
    <h3 class="h3">Distribuição por Região</h3>
    <?php if (!empty($this->arRegioes)):?>
    <div class="table-responsive">
        <table class="table table-striped table-bordered table-hover table-condensed" id="table-regioes">
            <thead>
            <tr>
                <td class="text-center"><b>Região</b></td>
                <td class="text-center"><b>Pagamento (R$)</b></td>
                <td class="text-center"><b>Empenho (R$)</b></td>
                <td class="text-center"><b>% Pagamento</b></td>
                <td class="text-center"><b>% Empenho</b></td>
            </tr>
            </thead>
            <tbody id="table-termo-body">
            <?php foreach ($this->arRegioes as $regiao):?>
                <?php $totalRegiaoPagamento        = bcadd($regiao['vlr_pagamento'],$totalRegiaoPagamento,2);?>
                <?php $totalRegiaoEmpenho          = bcadd($regiao['vlr_empenho'],$totalRegiaoEmpenho,2);?>
                <?php $totalRegiaoPercentPagamento = bcadd($regiao['percent_pagamento'],$totalRegiaoPercentPagamento,2);?>
                <?php $totalRegiaoPercentEmpenho   = bcadd($regiao['percent_empenho'],$totalRegiaoPercentEmpenho,2);?>
                <tr>
                    <td class="text-center" ><?= $regiao['regdescricao'];?></td>
                    <td class="text-center" ><?= $regiao['vlr_pagamento'];?></td>
                    <td class="text-center" ><?= $regiao['vlr_empenho'];?></td>
                    <td class="text-center" ><?= $regiao['percent_pagamento'].'%';?></td>
                    <td class="text-center" ><?= $regiao['percent_empenho'].'%';?></td>
                </tr>
            <?php endforeach;?>
            </tbody>
            <tfoot>
            <tr>
                <td colspan="" class="text-center" ><b>Total:</b></td>
                <td class="text-center" ><b>R$ <?= formata_numero_monetario($totalRegiaoPagamento); ?></b></td>
                <td class="text-center" ><b>R$ <?= formata_numero_monetario($totalRegiaoEmpenho); ?></b></td>
                <td class="text-center" ><b><?= round($totalRegiaoPercentPagamento); ?>%</b></td>
                <td class="text-center" ><b><?= round($totalRegiaoPercentEmpenho); ?>%</b></td>
            </tr>
            </tfoot>
        </table>
    </div>
        <br>
    <?php else:?>
        <div class="alert alert-info"><p align="center">Nenhum registro encontrado</p></div>
    <?php endif;?>
    <hr>
    <h3 class="h3">Distribuição por Estado</h3>
    <br>
<!--    <div style="height: 400px;overflow-y: scroll;" id="div-table-regioes">-->
    <div class="table-responsive">
    <table class="table table-striped table-bordered table-hover table-condensed" id="table-estados">
            <thead>
            <tr>
                <td class="text-center"><b>Região</b></td>
                <td class="text-center"><b>Entidade</b></td>
                <td class="text-center"><b>Pagamento (R$)</b></td>
                <td class="text-center"><b>Empenho (R$)</b></td>
                <td class="text-center"><b>% Pagamento</b></td>
                <td class="text-center"><b>% Empenho</b></td>
            </tr>
            </thead>
            <tbody id="table-termo-body">
    <?php foreach ($this->arEstados as $estados):?>
        <?php foreach($estados as $estado):?>
        <?php $totalEstadosPagamento        = bcadd($estado['vlr_pagamento'],$totalEstadosPagamento,2);?>
        <?php $totalEstadosEmpenho          = bcadd($estado['vlr_empenho'],$totalEstadosEmpenho,2);?>
        <?php $totalEstadosPercentPagamento = bcadd($estado['percent_pagamento'],$totalEstadosPercentPagamento,2);?>
        <?php $totalEstadosPercentEmpenho   = bcadd($estado['percent_empenho'],$totalEstadosPercentEmpenho,2);?>
            <tr>
                <td class="text-center" ><?= $estado['regdescricao'];?></td>
                <td class="text-center" ><?= $estado['estdescricao'];?></td>
                <td class="text-center" ><?= $estado['vlr_pagamento'];?></td>
                <td class="text-center" ><?= $estado['vlr_empenho'];?></td>
                <td class="text-center" ><?= $estado['percent_pagamento'].'%';?></td>
                <td class="text-center" ><?= $estado['percent_empenho'].'%';?></td>
            </tr>
        <?php endforeach;?>
    <?php endforeach;?>
        </tbody>
        <tfoot>
            <tr>
                <td colspan="2" class="text-center" ><b>Total:</b></td>
                <td class="text-center" ><b>R$ <?= formata_numero_monetario($totalEstadosPagamento); ?></b></td>
                <td class="text-center" ><b>R$ <?= formata_numero_monetario($totalEstadosEmpenho); ?></b></td>
                <td class="text-center" ><b><?= round($totalEstadosPercentPagamento); ?>%</b></td>
                <td class="text-center" ><b><?= round($totalEstadosPercentEmpenho); ?>%</b></td>
            </tr>
        </tfoot>
    </table>
    </div>
    <br>
<!--    </div>-->
<?php else:?>
    <div class="alert alert-info"><p align="center">Nenhum registro encontrado</p></div>
<?php endif;?>
<script>
    $(function(){
        $("#table-regioes").DataTable({
            paginate: false,
            searching: true,
            'aoColumnDefs' : [
                {
                    'bSortable' : true,
                    "bPaginate": false
                },
                {"targets" : 0},
                {"targets" : 1},
                {"targets" : 2},
                {"targets" : 3},
                {"targets" : 4},
                { "sClass": "center",targets: [0,1,2,3,4]},
                {
                    "targets": 1,
                    "render": function (data, type, row) {
                        var valor = parseFloat(data);
                        // return valor;
                        return valor.toLocaleString('pt-BR',{ minimumFractionDigits: 2});
                    }
                },
                {
                    "targets": 2,
                    "render": function (data, type, row) {
                        var valor = parseFloat(data);
                        // return valor;
                        return valor.toLocaleString('pt-BR',{ minimumFractionDigits: 2});
                    }
                },
            ],
            language: {
                'sProcessing': "Processando...",
                'sLengthMenu': "Mostrar _MENU_ registros",
                'sZeroRecords': "N&atilde;o foram encontrados resultados",
                'sInfo': "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
                'sInfoEmpty': "Mostrando de 0 at&eacute; 0 de 0 registros",
                'sInfoFiltered': "(filtrado de _MAX_ registros no total)",
                'sInfoPostFix': ".",
                'sSearch': "Pesquisar :&nbsp;&nbsp;",
                'sUrl': "",
                'oPaginate': {
                    'sFirst': "Primeiro",
                    'sPrevious': "Anterior",
                    'sNext': "Seguinte",
                    'sLast': "&Uacute;ltimo"
                }
            }
        });

        $("#table-estados").DataTable({
            paginate: false,
            searching: true,
            'aoColumnDefs' : [
                {
                    'bSortable' : true,
                    "bPaginate": false
                },
                {"targets" : 0},
                {"targets" : 1},
                {"targets" : 2},
                {"targets" : 3},
                {"targets" : 4},
                {"targets" : 5},
                { "sClass": "center",targets: [0,1,2,3,4,5]},
                {
                    "targets": 2,
                    "render": function (data, type, row) {
                        var valor = parseFloat(data);
                        // return valor;
                        return valor.toLocaleString('pt-BR',{ minimumFractionDigits: 2});
                        // return valor.toLocaleString('pt-BR',{ minimumFractionDigits: 2 , style: 'currency', currency: 'BRL' });
                    }
                },
                {
                    "targets": 3,
                    "render": function (data, type, row) {
                        var valor = parseFloat(data);
                        // return valor;
                        return valor.toLocaleString('pt-BR',{ minimumFractionDigits: 2});
                        return valor.toLocaleString('pt-BR',{ minimumFractionDigits: 2 , style: 'currency', currency: 'BRL' });
                    }
                },
            ],
            language: {
                'sProcessing': "Processando...",
                'sLengthMenu': "Mostrar _MENU_ registros",
                'sZeroRecords': "N&atilde;o foram encontrados resultados",
                'sInfo': "Mostrando de _START_ at&eacute; _END_ de _TOTAL_ registros",
                'sInfoEmpty': "Mostrando de 0 at&eacute; 0 de 0 registros",
                'sInfoFiltered': "(filtrado de _MAX_ registros no total)",
                'sInfoPostFix': ".",
                'sSearch': "Pesquisar :&nbsp;&nbsp;",
                'sUrl': "",
                'oPaginate': {
                    'sFirst': "Primeiro",
                    'sPrevious': "Anterior",
                    'sNext': "Seguinte",
                    'sLast': "&Uacute;ltimo"
                }
            }
        });
    });
</script>