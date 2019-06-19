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

    <div class="ibox-content">

        
        <div class="row">
            <div class="col-lg-12">
               
                <div class="form-group {{ $errors->has('mnucod') ? 'has-error' : '' }}">
                    
                    <label class="col-md-2 control-label text-right p-xxs" for="mnucod">Associação do Perfil:</label>
                    
                    <div class="col-md-10">
                        <form name="formPerfil" id="formPerfil">
                            <select name="pflcod" id='pflcod' class="form-control">
                                <option value="" selected="selected">.. Selecione ..</option>
                                @foreach ($perfis as $perfil)
                                    <option value="{{ $perfil->pflcod }}">{{ $perfil->pfldsc }}</option>
                                @endforeach
                            </select>
                        </form>
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

        //Ação da seleção do checkbox do datatable
        $('#tb_menus').on('click', 'input[type="checkbox"]', function() {
            
            var pflcod =  $('#pflcod').val();
            var mnuid = $(this).attr('mnuid');
            
                 $.ajax({
                   method: "POST",
                   url: "{{ route('perfilmenu.associarMenu') }}",
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
        
        //Ação - Ao selecionar o Perfil.
        $('#pflcod').on('change', function () {            
            
                if ($.fn.DataTable.isDataTable('#tb_menus')) {                
                    dataTable_.clear();
                    dataTable_.destroy();
                }
            
                dataTable_ =  $('#tb_menus').DataTable({
                "processing": true,
                "serverSide": true,
                "paging":false,
                "ajax": {
                    url: "{{ route('perfilmenu.getList') }}",
                    data: function (parametros) {
                        parametros.pflcod = $('#pflcod').val();                   
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
            
        });       
        
    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop