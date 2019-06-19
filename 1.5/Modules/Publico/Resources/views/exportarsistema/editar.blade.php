@extends('layouts.app')
@section('title', 'Editar perfil')
@section('content')

    <div class="row wrapper border-bottom white-bg page-heading">
        <div class="col-lg-10">
            <h2>Lista</h2>
            <ol class="breadcrumb">
                <li><a href="#">Sistema</a></li>
                <li class="active"><strong>Lista de Erros Agrupados</strong></li>
            </ol>
        </div>
    </div>

    <div class="wrapper wrapper-content">
        <div class="form-group">
            <b>Tipo de Erro:</b>   {{$erro::tipoErro($erro->errtipo) }}
        </div>
        <div class="form-group">
            <b>Sistema:</b>   {{ $sistema->sisdsc }}
        </div>
        <div class="form-group">
            Data do Erro:   {{ $erro->errdata }}
        </div>
        <div class="form-group">
            Erro na Linha: {{ $erro->errlinha }}
        </div>
        <div class="form-group">
            Usuário: {{ $usuario->usucpf }} - {{ $usuario->usunome }}
        </div>
       <div class="form-group">
        {{ $erro->errdescricaocompleta }}
        </div>
    </div>
@endsection