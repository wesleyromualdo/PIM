@extends('layouts.app')
@section('title', 'Listar perfis')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Gerenciar Perfis</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a></li>
            <li>Tabelas de Apoio</li>
            <li class="active"><strong>Gerenciar Perfil</strong></li>
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
            <div class="col-lg-4 text-left">
                <a class="btn btn-primary" href="{{ route('perfil.create') }}"><i class="fa fa-plus"></i>
                    &nbsp; Novo Perfil </a>

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
                    <table id="tb_perfis" class="table table-hover table-condensed table-bordered" style="width:100%">
                        <thead>
                            <tr>
                                <th class="text-center"></th>
                                <th class="text-center">Nível</th>
                                <th class="text-center">Perfil</th>
                                <th class="text-center">Finalidade</th>
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

            var table_ = $('#tb_perfis').DataTable(
                {
                    ajax: "{{ route('perfil.getList') }}",
                    columnDefs: [
                        { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true},
                        { data: 'pflnivel', name: 'pflnivel', targets: 1, visible: true },
                        { data: 'pfldsc', name: 'pfldsc', targets: 2, visible: true },
                        { data: 'pflfinalidade', name: 'pflfinalidade', targets: 3, visible: true },


                        {
                            "targets": 0,
                            "orderable": false,
                            "searchable": false,
                        },
                    ],
                    initComplete: function (settings, json) {
                       // setFilterTop( $('#tb_perfis'), table_ );
                    }
                }
            );
        })
        ;
    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop