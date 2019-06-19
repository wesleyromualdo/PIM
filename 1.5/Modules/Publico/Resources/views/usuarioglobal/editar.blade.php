@extends('layouts.app')
@section('title', 'Editar Usuario')
@section('content')
    <div class="row wrapper border-bottom white-bg page-heading">
        <div class="col-lg-10">
            <h2>Gerenciar Usuário</h2>
            <ol class="breadcrumb">
                <li><a href="#">Sistema</a> </li>
                <li>Tabelas de Apoio </li>
                <li><a href="{{route('usuarioglobal.index')}}"/>Gerenciar Usuário</a> </li>
                <li class="active"> <strong>Editar</strong> </li>
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
        
        <div class="row">
            <div class="col-lg-12">
                <div class="tabs-container">
                    <ul class="nav nav-tabs" role="tablist">
                        <li class="{{$abaUser}}">
                            <a class="nav-link {{$abaUser}}" data-toggle="tab" href="#dadosUsuario">Dados Usuário</a>
                        </li>
                        <li class="{{$abaUserSis}}">
                            <a class="nav-link" data-toggle="tab" href="#usuarioSistema">Usuário Sistemas</a>
                        </li>
                    </ul>
                    <div class="tab-content">
                        <div role="tabpanel" id="dadosUsuario" class="tab-pane {{$abaUser}}">
                            <div class="panel-body">
                                <div class="ibox-content">
                                    <div class="row">
                                        <div class="col-lg-12">

                                            {!! Form::model($usuario, ['method' => 'PATCH', 'route' => ['.usuarioglobal.update', $usuario->usucpf ], 'class' => 'form-horizontal']) !!}
                                            @include('seguranca::usuarioglobal.form-editar', ['errors' => $errors])
                                            {!! Form::close() !!}
                                        </div>
                                    </div>

                                </div>                                
                            </div>
                        </div>
                        <div role="tabpanel" id="usuarioSistema" class="tab-pane {{$abaUserSis}}">
                            <div class="panel-body">
                                <div class="ibox-content">
                                    <div class="row">
                                        <div class="col-lg-12" id="contentUsrSistema">
                                            @include('seguranca::usuarioglobal.modal-index', ['errors' => $errors])
                                        </div>
                                    </div>

                                </div>
                            </div>
                        </div>
                    </div>


                </div>
            </div>                
        </div>
    </div>
@endsection
@section('myjsfile')
<script src="{!! asset('js/plugins/dataTables/datatables.min.js') !!}"></script>
@parent
@stop

