@extends('layouts.app')

@section('title', 'Main page')

@section('content')
<div class="wrapper wrapper-content animated fadeInRight">

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
            <div class="text-center m-t-lg">
                <h1>
                    SIMEC 1.5
                </h1>
                <small>
                    Página inicial.
                </small>
            </div>
        </div>
    </div>
</div>
@endsection
