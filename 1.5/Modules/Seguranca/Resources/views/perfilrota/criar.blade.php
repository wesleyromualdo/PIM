@extends('layouts.app')
@section('title', ' Criar perfis rotas')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Gerenciar Perfis Rota</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a> </li>
            <li>Perfil</li>
            <li><a href="{{route('seguranca.perfilrota.create')}}">Gerenciar Perfil Rota</a></li>
            <li class="active"> <strong>Novo perfil rota</strong> </li>
        </ol>
    </div>
</div><div class="m-t-sm">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Novo Perfil Rota</h5>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-12">
                    {!! Form::open(array('route' => '.perfilrota.store','method'=>'POST', 'class' => 'form-horizontal')) !!}
                    @include('seguranca::perfilrota.form', ['errors' => $errors])
                    {!! Form::close() !!}
                </div>
            </div>
        </div>
    </div>
</div>

@endsection

@section('myjsfile')
    <script>
      
    </script>
@stop