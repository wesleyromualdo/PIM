@extends('layouts.app')
@section('title', 'Associar Menus')
@section('content')

    <div class="row wrapper border-bottom white-bg page-heading">
        <div class="col-lg-10">
            <h2>Gerenciar Menu</h2>
            <ol class="breadcrumb">
                <li><a href="#">Sistema</a> </li>
                <li>Tabelas de Apoio </li>
                <li><a href="{{route('perfilmenu.index')}}"/>Gerenciar Menu</a> </li>
                <li class="active"> <strong>Editar</strong> </li>
            </ol>
        </div>
    </div>

    <div class="wrapper wrapper-content">
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-12">
                    {!! Form::model($menu, ['method' => 'PATCH', 'route' => ['.menu.update', $menu->mnuid ], 'class' => 'form-horizontal']) !!}
                    @include('seguranca::menu.form', ['errors' => $errors])
                    {!! Form::close() !!}
                </div>
            </div>
        </div>
    </div>
@endsection