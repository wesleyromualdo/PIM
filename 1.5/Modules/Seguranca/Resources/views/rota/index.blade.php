@extends('layouts.app')
@section('title', 'Listar perfis')
@section('content')
<?php
$routeCollection = Route::getRoutes();

//dd(count($routeCollection));
foreach ($routeCollection as $value) {
//    var_dump($value->getPath());
}
//?>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Gerenciar Rotas</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a></li>
            <li>Tabelas de Apoio</li>
            <li class="active"><strong>Gerenciar Rotas</strong></li>
        </ol>
    </div>
</div>
<div class="wrapper wrapper-content">

    @if ($message = Session::get('success'))
    <div class="alert alert-success">
        <p>{{ $message }}</p>
    </div>
    @endif

    <div class="ibox-content">

        <div class="row">
            <div class="form-group">
                <label class="col-md-2 control-label text-right p-xxs" for="sistemas">Sistemas:</label>
                <div class="col-md-9">
                    {!! Form::select('sistemas', $sistemas, null, ['class' => 'form-control', 'placeholder' => 'Selecione...', 'id' => 'sistemas'])!!}
                </div>
            </div>

            <div class="row">
                <div class="col-lg-4 text-left">

                </div>
                <div class="col-lg-5"></div>
                <div class="col-lg-3 text-right">
                &nbsp;
                </div>
            </div>
        <hr>
            <div class="row">
                <div class="col-lg-12">
                    <div class="table-responsive">
                        <table id="tb_rota" class="table table-hover table-condensed table-bordered" style="width:100%">
                            <thead>
                                <tr>
                                    <th class="text-center"><input type="checkbox" id="checkAllSis"/></th>
                                    <th class="text-center">Rota</th>
                                    <th class="text-center">Rota Descriçao</th>
                                </tr>
                            </thead>

                        </table>
                    </div>
                </div>
            </div>


            <div class="row">
                <div class="col-lg-12">
                    <div><H3> ROTAS DO BANCO DE DADOS</H3></div>
                    <div class="table-responsive">
                        <table id="tb_rota_b" class="table table-hover table-condensed table-bordered" style="width:100%">
                            <thead>
                            <tr>
                                <th class="text-center"><input type="checkbox" id="checkAllDb"/></th>
                                <th class="text-center">Rota</th>
                                <th class="text-center">Rota Descriçao</th>
                            </tr>
                            </thead>

                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
@endsection

@section('myjsfile')
    <script>
        var dataTable_;
        var dataTable_b;




        /*recupera as rotas do perfil*/
        $('#sistemas').on('change',function () {
            var sisid =  $('#sistemas').val();
            if ($.fn.DataTable.isDataTable('#tb_rota')) {
                dataTable_.clear();
                dataTable_.destroy();
            }
            dataTable_ =  $('#tb_rota').DataTable({
                "processing": true,
                "serverSide": true,
                "paging":   false,
                "ajax": {
                    url: "{{ route('rota.getList') }}",
                    data: function (params) {
                        params.sisid = sisid;
                    },
                },
                columnDefs: [
                    { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true},
                    { data: 'ds_rota', name: 'ds_rota', targets: 1, visible: true },
                    { data: 'ds_rota_descricao', name: 'ds_rota_descricao', targets: 2, visible: true },
                    {
                        "targets": 0,
                        "orderable": false,
                        "searchable": false,
                    },
                ]
            });

        });

        /*acao de selecao*/
        $("#tb_rota").on('click','input[type="checkbox"]',function () {
            var ds_rota =$(this).attr('ds_rota');
            var sisid =  $("#sistemas").val();
            // console.log($(this['checked']).length);
            var checked = $(this['checked']).length;
            $.ajax({
                method: "POST",
                url: "{{ route('rota.atualizarota') }}",
                data: {
                    sisid: sisid,
                    ds_rota: ds_rota,
                    checked: checked
                }
            }).done(function( msg ) {
                console.log( "Registro atualizado: " + msg );
            });

        });

        //Checkboxes - Marcar todos.
        $('#checkAllSis').click(function(){
            $(".chkRotaSis").prop('checked', $(this).prop('checked'));
        });



        //--------- ROTAS DO BANCO------------------------------


        $("#tb_rota_b").on('click','input[type="checkbox"]',function () {
            var co_rota =$(this).attr('co_rota');
            var sisid =  $("#sistemas").val();
            // console.log($(this['checked']).length);
            var checked = $(this['checked']).length;
            $.ajax({
                method: "POST",
                url: "{{ route('rota.atualizaStatusRotaBanco') }}",
                data: {
                    sisid: sisid,
                    co_rota: co_rota,
                    checked: checked
                }
            }).done(function( msg ) {
                console.log( "Registro atualizado: " + msg );
            });

        });


        //Checkboxes - Marcar todos.
        $('#checkAllDb').click(function(){
            $(".chkRotaDb").prop('checked', $(this).prop('checked'));
        });


        /*recupera as rotas do bando de dados*/
        $('#sistemas').on('change',function () {
            var sisid =  $('#sistemas').val();
            if ($.fn.DataTable.isDataTable('#tb_rota_b')) {
                dataTable_b.clear();
                dataTable_b.destroy();
            }
            dataTable_b =  $('#tb_rota_b').DataTable({
                "processing": true,
                "serverSide": true,
                "paging":   false,
                "ajax": {
                    url: "{{ route('rota.getListRotasBanco') }}",
                    data: function (params) {
                        params.sisid = sisid;
                    },
                },
                columnDefs: [
                    { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true},
                    { data: 'ds_rota', name: 'ds_rota', targets: 1, visible: true },
                    { data: 'ds_rota_descricao', name: 'ds_rota_descricao', targets: 2, visible: true },
                    {
                        "targets": 0,
                        "orderable": false,
                        "searchable": false,
                    },
                ]
            });

        });


        //-----------------------------------------


    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop