@extends('layouts.app')
@section('title', 'Associar Menus')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Associar Menus</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a> </li>
            <li>Menu</li>
            <li><a href="{{route('perfilmenu.create')}}"/>Gerenciar Menu</a></li>
            <li class="active"> <strong>Nova Associação</strong> </li>
        </ol>
    </div>
</div>
<div class="m-t-sm">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Novo Associação</h5>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-12">
                    {!! Form::open(array('route' => '.perfilmenu.store','method'=>'POST', 'class' => 'form-horizontal')) !!}
                    @include('seguranca::perfilmenu.form', ['errors' => $errors])
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