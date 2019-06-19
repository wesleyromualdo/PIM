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

    <div class="ibox-content" style=" min-height: 350px">
        <div class="row">
            <div class="col-md-12  text-left">
                
                <div  style="float: left;  max-width: 80% ">
                    <form id="formSistema" action="{{ route('perfilglobal.create') }}" method="patch">
                        <button class="btn btn-primary" type="button" id="novoPerfil" style="float: left;" ><i class="fa fa-plus"></i> &nbsp; Novo Perfil </button>
                {{--<a class="btn btn-info" href="#"><i class="fa fa-filter"></i> &nbsp; Pesquisa Avançada </a>--}}
                        {!! Form::select('sisid', $sistema, null, ['style' =>'float:left; max-width: 80%', 'class' => 'form-control chosen-select', 'placeholder' => 'Selecione um sistema...', 'id' => 'sisid'])!!}
                    </form>
                </div>
                <a class="btn btn-default" data-url="#" style="float: right;">
                    <i class="fa fa-cloud-download"></i> Exportar
                </a>
                <a class="btn btn-default" data-url="#" style="float: right;">
                    <i class="fa fa-print"></i> Imprimir
                </a>
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
                                <th class="text-center">Super Usuário</th>
                                <th class="text-center">Pode Delegar</th>
                                <th class="text-center">Suporte</th>
                                <th class="text-center">Cumulativo</th>
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
        var table_perfis;
        function carregaTabelaPerfis () {
            if ($.fn.DataTable.isDataTable('#tb_perfis')) {
                table_perfis.clear();
                table_perfis.destroy();
            }
            
            table_perfis = $('#tb_perfis').DataTable(
                {
                    ajax: {
                            url: "{{ route('perfilglobal.getList') }}",
                            method: 'POST',
                            data: {
                                    sisid: $("#sisid").val(),
                                  }
                        },
                    columnDefs: [
                        { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true, width: '15%'},
                        { data: 'pflnivel', name: 'pflnivel', targets: 1, visible: true },
                        { data: 'pfldsc', name: 'pfldsc', targets: 2, visible: true },
                        { data: 'pflsuperuser', name: 'pflsuperuser', searchable: false, targets: 3, visible: true },
                        { data: 'pflinddelegar', name: 'pflinddelegar', searchable: false, targets: 4, visible: true },
                        { data: 'pflsuporte', name: 'pflsuporte', searchable: false, targets: 5, visible: true },
                        { data: 'pflsncumulativo', name: 'pflsncumulativo', searchable: false, targets: 6, visible: true },
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
        }
        
        $(document).ready(function () {
            $("#sisid").change(function () {
                carregaTabelaPerfis();
            });
            
            $("#novoPerfil").click(function () {
                if ($("#sisid").val() > 0) {
                    $("#formSistema").submit();
                } else {
                    alert("Atenção! É necessário selecionar um sistema.");
                }
            });
        });
    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop