@extends('layouts.app')
@section('title', 'Listar menus')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Gerenciar Menu </h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a></li>
            <li>Tabelas de Apoio</li>
            <li class="active"><strong>Gerenciar Menu</strong></li>
        </ol>
    </div>
</div>

<div class="wrapper wrapper-content">
    
    
    @if ($message = Session::get('success'))
    <div class="alert alert-success">
        <p>{{ $message }}</p>
    </div>
    @elseif($message = Session::get('warning'))
    <div class="alert alert-warning">
        <p>{{ $message }}</p>
    </div>
    @elseif($message = Session::get('error'))
    <div class="alert alert-danger">
        <p>{{ Session::get('error') }}</p>
    </div>
    @endif

    <div class="ibox-content">
        <div class="row">
            <div class="col-lg-4 text-left">
                <a class="btn btn-primary" href="{{ route('menu.create') }}"><i class="fa fa-plus"></i> &nbsp; Novo Menu </a>
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
                    <table id="tb_menus" class="table table-hover table-condensed table-bordered" style="width:100%">        
                        <thead>
                            <tr>
                                <th class="text-center"></th>
                                <th class="text-center">Código</th>
                                <th class="text-center">Menu / Módulo</th>
                                <th class="text-center">Visível</th>
                                <th class="text-center">Transação</th>
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

            var table_ = $('#tb_menus').DataTable(
                {
                    "paging":false,
                    ajax: "{{ route('menu.getList') }}",
                    columnDefs: [
                        { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true, width: '15%'},
                        { data: 'mnucod', name: 'mnucod', targets: 1, visible: true },
                        { data: 'mnudsc', name: 'mnudsc', targets: 2, visible: true },
                        { data: 'mnushow', name: 'mnushow', targets: 3, visible: true },                        
                        { data: 'mnutransacao', name: 'mnutransacao', targets: 4, visible: true },                        
                        {
                            "targets": 0,
                            "orderable": false,
                            "searchable": false,
                        },
                    ],
                    initComplete: function (settings, json) {
//                        setFilterTop( $('#tb_menus'), table_ );
                    }
                }
            );
        });
    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop