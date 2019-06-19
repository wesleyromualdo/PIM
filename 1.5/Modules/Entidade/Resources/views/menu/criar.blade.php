@extends('layouts.app')
@section('title', 'Listar menus')
@section('content')

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2>Gerenciar Menu</h2>
        <ol class="breadcrumb">
            <li><a href="#">Sistema</a> </li>
            <li>Menu</li>
            <li><a href="{{route('menu.index')}}"/>Gerenciar Menu</a></li>
            <li class="active"> <strong>Novo</strong> </li>
        </ol>
    </div>
</div>
<div class="m-t-sm">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Novo Menu</h5>
        </div>
        <div class="ibox-content">
            <div class="row">
                <div class="col-lg-12">
                    {!! Form::open(array('route' => 'menu.store','method'=>'POST', 'class' => 'form-horizontal')) !!}
                    @include('seguranca::menu.form', ['errors' => $errors])
                    {!! Form::close() !!}
                </div>
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