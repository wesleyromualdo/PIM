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

    <div class="ibox-content" style=" min-height: 350px">
        <div class="row">       
            
<!--            <label class="col-md-2 control-label text-right p-xxs" for="mnucod">Seleção de Sistema:</label>-->
                    
            <div class="col-md-12  text-left">
                
                <div  style="float:left; max-width: 80%;">
                    <form id="formSistema" action="{{ route('menuglobal.create') }}" method="patch">
                        <button class="btn btn-primary" type="button" id="novoMenu" style="float: left;" ><i class="fa fa-plus"></i> &nbsp; Novo Menu </button>
                {{--<a class="btn btn-info" href="#"><i class="fa fa-filter"></i> &nbsp; Pesquisa Avançada </a>--}}
                        {!! Form::select('sisid', $sistema, null, ['style' =>'float:left; max-width: 80%;', 'class' => 'form-control chosen-select', 'placeholder' => 'Selecione um sistema...', 'id' => 'sisid'])!!}
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
        var table_menus;
        function carregaTabelaMenus() {
            if ($.fn.DataTable.isDataTable('#tb_menus')) {
                    table_menus.clear();
                    table_menus.destroy();
                }
                 table_menus = $('#tb_menus').DataTable({
                    "paging":false,
                        ajax: {
                                url: "{{ route('menuglobal.getList') }}",
                                method: 'POST',
                                data: {
                                        sisid: $("#sisid").val(),
                                      }
                          },
                        columnDefs: [
                            { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true, width: '15%' },
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
                    });
        }

        $(document).ready(function () {
            $("#sisid").change(function () {
                carregaTabelaMenus();
            });            
            
            $("#novoMenu").click(function () {
                if ($("#sisid").val() > 0) {
                    $("#formSistema").submit();
                } else {
                    alert("Atenção! É necessário selecionar um sistema.");
                }
            });
            
            if ($("#sisid").val() > 0) {
                carregaTabelaMenus();
            }
        });
    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop