@extends('layouts.app')
@section('title', 'Listar Sistemas')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Gerenciar Sistemas</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a></li>
            <li>Tabelas de Apoio</li>
            <li class="active"><strong>Gerenciar Sistema</strong></li>
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
                <a class="btn btn-primary" href="{{ route('sistema.create') }}"><i class="fa fa-plus"></i> &nbsp; Novo Sistema </a>

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
                    <table id="tb_sistemas" class="table table-hover table-condensed table-bordered" style="width:100%">
                        <thead>
                            <tr>
                                <th class="text-center"></th>
                                <th class="text-center">Sistema</th>
                                <th class="text-center">Abreviação</th>
                                <th class="text-center">CPF do Analista</th>
                                <th class="text-center">Status</th>
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

            var table_ = $('#tb_sistemas').DataTable(
                {
                    "paging":   false,
                    ajax: "{{ route('sistema.getList') }}",
                    columnDefs: [
                        { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true},
                        { data: 'sisdsc', name: 'sisdsc', targets: 1, visible: true },
                        { data: 'sisabrev', name: 'sisabrev', targets: 2, visible: true },
                        { data: 'usucpfanalista', name: 'usucpfanalista', targets: 3, visible: true , render: function (data,row) {
                                if(data) {
                                    cpf = data.replace(/[^\d]/g, "");
                                    return cpf.replace(/(\d{3})(\d{3})(\d{3})(\d{2})/, "$1.$2.$3-$4");
                                }
                                return '';
                            }
                        },
                        { data: 'sisstatus', name: 'sisstatus', targets: 4, visible: true , render: function (data,row) {
                                if(data=='A') {
                                    return 'Ativo';
                                }else{
                                    return 'Inativo';
                                }
                            }
                        },


                        {

                            "targets": 0,
                            "orderable": false,
                            "searchable": false,
                        },
                    ],
                    initComplete: function (settings, json) {
//                        setFilterTop( $('#tb_perfis'), table_ );
                    }
                }
            );

        })
        ;
    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop