@extends('layouts.app')
@section('title', 'Editar perfil')
@section('content')
    <div class="row wrapper border-bottom white-bg page-heading">
        <div class="col-lg-10">
            <h2>Gerenciar Usuário</h2>
            <ol class="breadcrumb">
                <li><a href="#">Sistema</a> </li>
                <li>Tabelas de Apoio </li>
                <li><a href="{{route('usuario.index')}}"/>Gerenciar Usuário</a> </li>
                <li class="active"> <strong>Editar</strong> </li>
            </ol>
        </div>
    </div>

    <div class="wrapper wrapper-content">

        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-12">

                    {!! Form::model($usuario, ['method' => 'PATCH', 'route' => ['usuario.update', $usuario->usucpf ], 'class' => 'form-horizontal']) !!}
                    @include('seguranca::usuario.form-editar', ['errors' => $errors])
                    {!! Form::close() !!}
                </div>
            </div>

        </div>
    </div>
@endsection