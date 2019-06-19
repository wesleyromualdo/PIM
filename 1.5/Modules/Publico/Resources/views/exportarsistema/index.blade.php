@extends('layouts.app')
@section('title', 'Exportar Sistema')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a></li>
            <li class="active"><strong>Exportar Sistema</strong></li>
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
            {!! Form::open(array('class' => 'form-horizontal')) !!}
            @include('seguranca::exportarsistema.form-pesquisa', ['errors' => $errors])
            {!! Form::close() !!}
        </div>
       <hr>
        <div class="row">
            <div class="col-lg-12">
                <div id="codigosql">

                </div>
            </div>
        </div>

    </div>
</div>
@endsection

@section('myjsfile')
    <script>
        $(function () {

            $('#btn-exportar-sistema').on('click', function () {

               if($('#sisid').val()==''){
                   alert('Escolha um Sistema para exportar.');
                   return false;
               }

                $.ajax({
                    type: "POST",
                    url: "{{ route('exportarsistema.exportarSistema') }}",
                    data: {
                            sisid:$('#sisid').val(),
                            segurancaperfil:$('#segurancaperfil').prop( "checked" ),
                            segurancamenu:$('#segurancamenu').prop( "checked" ),
                            perfilmenu:$('#perfilmenu').prop( "checked" ),
                            perfilusuario:$('#perfilusuario').prop( "checked" ),
                            tipodocumento:$('#tipodocumento').prop( "checked" ),
                            estadodocumento:$('#estadodocumento').prop( "checked" ),
                            acaoestadoducomento:$('#acaoestadoducomento').prop( "checked" ),
                            estadodocumentoperfil:$('#estadodocumentoperfil').prop( "checked" ),
                            usuarioresponsabilidade:$('#usuarioresponsabilidade').prop( "checked" )},
                    success: function(res){
                    //console.log(res);
                    swal("Ação executada com sucesso!", {icon: "success"});
                    $("#codigosql").html('');
                    $("#codigosql").append(res);
                    },
                    error: function(XMLHttpRequest, textStatus, errorThrown) {
                        console.log(XMLHttpRequest);
                    }
                });
            });
        });
    </script>

    <script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@stop