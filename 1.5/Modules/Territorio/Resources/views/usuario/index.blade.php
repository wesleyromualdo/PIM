@extends('layouts.app')
@section('title', 'Listar perfis')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Gerenciar Usuários - {{ $sistema }}</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a></li>
            <li>Tabelas de Apoio</li>
            <li class="active"><strong>Gerenciar Usuários</strong></li>
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
             {!! Form::open(array('route' => 'usuario.consultarUsuarios','method'=>'POST', 'class' => 'form-horizontal')) !!}
                    @include('seguranca::usuario.form-pesquisa', ['errors' => $errors])
             {!! Form::close() !!}            
        </div>
   
        <hr>
        <div class="row">
            <div class="col-lg-12">
                <div class="table-responsive"> 
                    <table id="tb_usuarios" class="table table-hover table-condensed table-bordered" style="width:100%">        
                        <thead>
                            <tr>
                                <th class="text-center"></th>
                                <th class="text-center">CPF</th>
                                <th class="text-center">Nome Completo</th>
                                <th class="text-center">Telefone</th>
                                <th class="text-center">E-mail</th>
                                <th class="text-center">Unidade</th>
                                <th class="text-center">Cargo/Função</th>
                                <th class="text-center">Órgão</th>
                                <th class="text-center">UF</th>
                                <th class="text-center">Município</th>
                                <th class="text-center">Data da Última Atualização</th>
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
        
        var dataTable_;
        
        function setCMBMunicipios(municipios){
            
            var select = $('#municipio');                                    
        
            select.find('option').remove(); 
            select.append(new Option("Selecione...", "0"));
            
            $.each(municipios, function(key,value) {
                $('<option>').val(key).text(value).appendTo(select);
            }); 
            
        }        
        
        $(function () {                        

            $('#unidadeFederativa').on('change', function(){

            var estuf =  $('#unidadeFederativa').val();

                {{--$.ajax({--}}
                {{--    type: "POST",--}}
                {{--    url: "{{ route('territorios.getMunicipios') }}",--}}
                {{--    data: {estuf:estuf},--}}
                {{--    success: function(res){--}}
                {{--      console.log(res); --}}
                {{--      setCMBMunicipios(res);                      --}}
                {{--    }, --}}
                {{--    error: function(XMLHttpRequest, textStatus, errorThrown) {--}}
                {{--        console.log(XMLHttpRequest);--}}
                {{--    }--}}
                {{--});--}}

            });
            
            
              $('#btn-pesquisar-usuario').on('click', function () {              
            
                if ($.fn.DataTable.isDataTable('#tb_usuarios')) {                
                    dataTable_.clear();
                    dataTable_.destroy();
                }
            
                dataTable_ =  $('#tb_usuarios').DataTable({
                "processing": true,
                "serverSide": true,
                "ajax": {
                    url: "{{ route('usuario.getList') }}",
                    data: function (params) {
                        params.usucpf = $('#usucpf').val(); 
                        params.usunome = $('#usunome').val(); 
                        params.unidadeFederativa = $('#unidadeFederativa').val(); 
                        params.municipio = $('#municipio').val(); 
                        params.perfil = $('#perfil').val(); 
                        params.unidadeOrcamentaria = $('#unidadeOrcamentaria').val(); 
                        params.suscod =  $("input[id='suscod']:checked").val();
                        params.usuchaveativacao =  $("input[id='usuchaveativacao']:checked").val(); 
                        params.sitperfil = $("input[id='sitperfil']:checked").val(); 
                    },
                },
                columnDefs: [
                        { data: 'action', name: 'edit', orderable: false, searchable: false, targets: 0, visible: true},
                        { data: 'usucpf', name: 'usucpf', targets: 1, visible: true },
                        { data: 'usunome', name: 'usunome', targets: 2, visible: true },
                        { data: 'telefone', name: 'telefone', searchable: false, targets: 3, visible: true },
                        { data: 'usuemail', name: 'usuemail', searchable: false, targets: 4, visible: true },
                        { data: 'unidade', name: 'unidade', searchable: false, targets: 5, visible: true },
                        { data: 'cargofuncao', name: 'cargofuncao', searchable: false, targets: 6, visible: true },
                        { data: 'orgao', name: 'orgao', searchable: false, targets: 7, visible: true },
                        { data: 'uf', name: 'uf', searchable: false, targets: 8, visible: true },
                        { data: 'municipio', name: 'municipio', searchable: false, targets: 9, visible: true },
                        { data: 'usudataatualizacao', name: 'usudataatualizacao', searchable: false, targets: 10, visible: true },
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