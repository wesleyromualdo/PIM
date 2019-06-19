@extends('layouts.app')
@section('title', 'Editar sistema')
@section('content')

    <div class="row wrapper border-bottom white-bg page-heading">
        <div class="col-lg-10">
            <h2>Gerenciar Sistema</h2>
            <ol class="breadcrumb">
                <li><a href="#">Sistema</a> </li>
                <li>Tabelas de Apoio </li>
                <li><a href="{{route('seguranca.sistema.index')}}"/>Gerenciar Sistema</a> </li>
                <li class="active"> <strong>Editar</strong> </li>
            </ol>
        </div>
    </div>

    <div class="wrapper wrapper-content">

        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-12">
                    {!! Form::model($sistema, ['method' => 'PATCH', 'route' => [$prefixo. '.sistema.update', $sistema->sisid ], 'class' => 'form-horizontal']) !!}
                    @include('seguranca::sistema.form', ['errors' => $errors])
                    {!! Form::close() !!}
                </div>
            </div>

        </div>
    </div>
@endsection