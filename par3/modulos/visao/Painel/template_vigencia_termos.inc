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
    <h3 class="h3">Vigência dos Termos - <?= $this->descricao?></h3>
        <form id="form-termo-vigencia">

        </form>
        <div class="table-responsive">
            <table class="table table-striped table-bordered table-hover table-condensed" id="table-vigencia-termos">
                <thead>
                <tr>
                    <td class="text-center"><b>Entidade</b></td>
                    <td class="text-center"><b>Processo</b></td>
                    <td class="text-center"><b>Termo</b></td>
                    <td class="text-center"><b>Valor do Empenho (R$)</b></td>
                    <td class="text-center"><b>Usuário do último Empenho</b></td>
                    <td class="text-center"><b>Valor Pago (R$)</b></td>
                    <td class="text-center"><b>Usuário do último Pagamento</b></td>
                    <td class="text-center"><b>Vigência</b></td>
                    <td class="text-center"><b>full</b></td>
                </tr>
                </thead>
            </table>
        </div>
        <br>
    <style>
        div.dataTables_paginate {text-align: center}
    </style>
<script>
    $(function() {
        renderTable('teste');
    });
    function renderTable(params) {

        $("#table-vigencia-termos").DataTable({
            destroy : true,
            colReorder: true,
            "lengthMenu": [[10, 25,50], [10, 25,50]],
            processing: true,
            serverSide: true,
            paginate: true,
            searching: true,
            paging: true,
            responsive: true,
            searchDelay: 1000,
            retrieve: true,
            'bSortable' : true,
            "bScrollInfinite": true,
            "bScrollCollapse": true,
            "bProcessing": true,
            'aoColumnDefs' : [
                {"targets" : 0},
                {"targets" : 1},
                {"targets" : 2},
                {"targets" : 3},
                {"targets" : 4,'visible': true},
                {"targets" : 5,'visible': true},
                {"targets" : 6},
                {"targets" : 7},
                {"targets" : 8,'visible': false},
                { "sClass": "center",targets: [0,1,2,3,4,5,6]},
                {
                    "targets": 3,
                    "render": function (data, type, row) {
                        var valor = parseFloat(data);
                        // return valor;
                        return valor.toLocaleString('pt-BR',{ minimumFractionDigits: 2});
                    }
                },
                {
                    "targets": 5,
                    "render": function (data, type, row) {
                        var valor = parseFloat(data);
                        // return valor;
                        return valor.toLocaleString('pt-BR',{ minimumFractionDigits: 2});
                    }
                },
                {
                    "targets": 1,
                    "render": function (data, type, row) {
                        var valor = data.substr(0, 5) +'.' +
                        data.substr(5, 6) + '/' +
                        data.substr(11, 4) + '-' +
                        data.substr(15, 2);
                        return valor;
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
            },
            ajax: {
                url: '/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=listarvigenciatermosjson&descricao=<?php echo $this->id; ?>',
                beforeLoad : function() {

                    $('#loading').hide();
                },
                complete : function(){
                    $('[data-toggle="popover"]').popover({html:true, animation:true, delay:0});
                },
                error: function () {

                },
            }
        });
    }

    function searchData() {
        $('#table-vigencia-termos').DataTable().destroy();
        renderTable('teste');
    }
</script>