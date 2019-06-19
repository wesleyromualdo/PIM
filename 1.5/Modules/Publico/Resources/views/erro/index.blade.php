@extends('layouts.app')
@section('title', 'Listar Erros')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Lista</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a></li>
            <li class="active"><strong>Lista de Erros Agrupados</strong></li>
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
            {!! Form::open(array('route' => 'logErro.getList','method'=>'POST', 'class' => 'form-horizontal')) !!}
            @include('seguranca::erro.form-pesquisa', ['errors' => $errors])
            {!! Form::close() !!}
        </div>
       <hr>
        <div class="row">
            <div class="col-lg-12">
                <div class="table-responsive"> 
                    <table id="tb_erros" class="table table-hover table-condensed table-bordered" style="width:100%">
                        <thead>
                            <tr>
                                <th class="text-center">Ação</th>
                                <th class="text-center">Errid</th>
                                <th class="text-center">Sistema</th>
                                <th class="text-center">Erro</th>
                                <th class="text-center">Quantidade</th>
                            </tr>
                        </thead>            
                        
                    </table>  
                </div>
            </div>
        </div>

    </div>
</div>
@endsection

@section('myjsfile')
    <script>
        $(function () {
            $('#btn-pesquisar-erros').on('click', function () {

                if ($.fn.DataTable.isDataTable('#tb_erros')) {
                    dataTable_.clear();
                    dataTable_.destroy();
                }

                dataTable_ =  $('#tb_erros').DataTable({
                    "processing": true,
                    "serverSide": true,
                    "ajax": {
                        url: "{{ route('logErro.getList') }}",
                        data: function (params) {
                            params.horas = $('#horas').val();
                        },
                    },

                    columnDefs: [

                        { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true},
                        { data: 'errid', name: 'errid', targets: 1, visible: true },
                        { data: 'sisdsc', name: 'sisdsc', targets: 2, visible: true },
                        { data: 'errdescricao', name: 'errdescricao', targets: 3, visible: true },
                        { data: 'qtderros', name: 'qtderros', searchable: false, targets: 4, visible: true },

                        {
                            "targets": 0,
                            "orderable": false,
                            "searchable": false,
                        },
                    ]
                });
            });
        });
    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop