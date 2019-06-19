@extends('layouts.app')
@section('title', 'Associar Menus')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Gerenciar Menu</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a></li>
            <li>Tabelas de Apoio</li>
            <li class="active"><strong>Associar Menus</strong></li>
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
            <div class="col-lg-12">
               
                <div class="form-group {{ $errors->has('sisid') ? 'has-error' : '' }}">
                    <label class="col-md-2 control-label text-right p-xxs" for="sisid">Sistema:</label>
                    
                    <div class="col-md-10">                        
                            {!! Form::select('sisid', $sistema, null, ['style' =>'float:left; max-width: 80%', 'class' => 'form-control chosen-select', 'placeholder' => 'Selecione um sistema...', 'id' => 'sisid'])!!}
                    </div>
                </div>
                
                <div class="form-group {{ $errors->has('pflcod') ? 'has-error' : '' }}">
                    <label class="col-md-2 control-label text-right p-xxs" for="pflcod">Perfil:</label>
                    
                    <div class="col-md-10">
                        <select name="pflcod" id='pflcod' class="form-control">                                
                        </select>
                    </div>
                </div>
                
            </div>            
        </div>
        <hr>
        
        <div class="row">
            <div class="col-lg-12">
                <div class="table-responsive"> 
                    <table id="tb_menus" class="table table-hover table-condensed table-bordered" style="width:100%">        
                        <thead>
                            <tr>
                                <th class="text-center"><input type="checkbox" id="checkAll"/></th>                                
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
        <hr>
    </div>
</div>
@endsection


@section('myjsfile')
    <script>
        var dataTable_;
        
        function CarregaDropDownPerfil() {
            $.ajax({
                method: "POST",
                url: "{{ route('perfilmenuglobal.ListarPerfilBySistema') }}",
                data: { sisid: $("#sisid").val()}
            }).done(function( result ) {
                var index;
                var opt = '<option value="" selected="selected">.. Selecione um Perfil..</option>';
                for(index in result) {
                    $("#pflcod").html('');
                    opt += '<option value="'+result[index]['pflcod']+'">'+result[index]['pfldsc']+'</option>';
                    $("#pflcod").html(opt);
                }                
            });
        }
        
        function CarregaTabelaAssociaMenus() {
            if ($.fn.DataTable.isDataTable('#tb_menus')) {
                dataTable_.clear();
                dataTable_.destroy();
            }
            dataTable_ =  $('#tb_menus').DataTable({
            "processing": true,
            "serverSide": true,
            "paging":false,
            "ajax": {
                url: "{{ route('perfilmenuglobal.getList') }}",
                data: function (parametros) {
                    parametros.pflcod = $('#pflcod').val();
                    parametros.sisid = $("#sisid").val();
                },
            },
            columnDefs: [                        
                        { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true},
                        { data: 'mnucod', name: 'mnucod', targets: 1, visible: true },
                        { data: 'mnudsc', name: 'mnudsc', targets: 2, visible: true },
                        { data: 'mnushow', name: 'mnushow', targets: 3, visible: true },                        
                        { data: 'mnutransacao', name: 'mnutransacao', targets: 4, visible: true },                        
                        {
                            "targets": 0,
                            "orderable": false,
                            "searchable": false,
                        },
                    ]
            });
        }
               
        //Ação da seleção do checkbox do datatable
        $('#tb_menus').on('click', 'input[type="checkbox"]', function() {
            var pflcod =  $('#pflcod').val();
            var mnuid = $(this).attr('mnuid');

            console.log('pflcod -> '+pflcod + " mnuid "+mnuid);
            console.log('e bem aqui');return;
                 $.ajax({
                   method: "POST",
                   url: "{{ route('perfilmenuglobal.associarMenu') }}",
                   data: { pflcod: pflcod, mnuid: mnuid}
                 }).done(function( msg ) {                                          
                     console.log( "Registro atualizado: " + msg );                      
                 });
            
        });        

        //Checkboxes - Marcar todos.
        $('#checkAll').click(function(){             
             $(".chkPerfil").prop('checked', $(this).prop('checked'));
        });
        
        //A cada paginação volta o checkbox do header para unchecked.
        $('#tb_menus').on( 'page.dt', function () {
            $("#checkAll").prop('checked', false);
        });  
        
        $("#sisid").change(function () {
            if ($.fn.DataTable.isDataTable('#tb_menus')) {                
                dataTable_.clear();
                dataTable_.destroy();
            }
            CarregaDropDownPerfil();
        });
        
        //Ação - Ao selecionar o Perfil.
        $('#pflcod').on('change', function () {
            if ($(this).val() < 1 && $.fn.DataTable.isDataTable('#tb_menus')) {                
                dataTable_.clear();
                dataTable_.destroy();
            } else {
                CarregaTabelaAssociaMenus();
            }
        });
        
        $(document).ready(function () {
            
        });
    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop