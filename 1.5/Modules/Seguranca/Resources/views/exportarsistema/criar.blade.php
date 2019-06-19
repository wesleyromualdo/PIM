@extends('layouts.app')
@section('title', 'Listar perfis')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Gerenciar Perfis</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a> </li>
            <li>Perfil</li>
            <li><a href="{{route('seguranca.perfil.create')}}"/>Gerenciar Perfil</a></li>
            <li class="active"> <strong>Novo</strong> </li>
        </ol>
    </div>
</div>
<div class="m-t-sm">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Novo Perfil</h5>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-12">
                    {!! Form::open(array('route' => '.perfil.store','method'=>'POST', 'class' => 'form-horizontal')) !!}
                    @include('seguranca::perfil.form', ['errors' => $errors])
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