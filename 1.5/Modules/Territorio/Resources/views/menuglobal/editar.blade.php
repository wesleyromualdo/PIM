@extends('layouts.app')
@section('title', 'Editar menu')
@section('content')

    <div class="row wrapper border-bottom white-bg page-heading">
        <div class="col-lg-10">
            <h2>Gerenciar Menu</h2>
            <ol class="breadcrumb">
                <li><a href="#">Sistema</a> </li>
                <li>Tabelas de Apoio </li>
                <li><a href="{{route('menuglobal.index')}}"/>Gerenciar Menu</a> </li>
                <li class="active"> <strong>Editar</strong> </li>
            </ol>
        </div>
    </div>

    <div class="wrapper wrapper-content">
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-12">
                    {!! Form::model($menu, ['method' => 'PATCH', 'route' => ['.menuglobal.update', $menu->mnuid ], 'class' => 'form-horizontal']) !!}
                    @include('seguranca::menuglobal.form', ['errors' => $errors])
                    {!! Form::close() !!}
                </div>
            </div>
        </div>
    </div>
@endsection

@section('myjsfile')
    <script>
        function HideShowMenuPai() {
            if ($("#mnutipo").val() >= 2 ) {
                $("#div-mnuidpai").show();
                $('#mnuidpai').prop('disabled', false);
            } else {
                $("#div-mnuidpai").hide();
                $('#mnuidpai').prop('disabled', 'disabled');
            }
        }
        
        $(document).ready(function(){
            HideShowMenuPai();
            $("#mnutipo").change(function(){
                HideShowMenuPai();
            });
        }); 
    </script>
@stop