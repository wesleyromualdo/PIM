<!DOCTYPE html>
<html>

<head>

    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <title>SIMEC | 404 Error</title>

    <link rel="stylesheet" type="text/css" href="{!! asset('css/bootstrap.min.css') !!}"/>
    <link rel="stylesheet" type="text/css" href="{!! asset('font-awesome/css/font-awesome.css') !!}"/>

    <link rel="stylesheet" type="text/css" href="{!! asset('css/animate.css') !!}"/>
    <link rel="stylesheet" type="text/css" href="{!! asset('css/style.css') !!}"/>

</head>

<body class="gray-bg">


<div class="middle-box text-center animated fadeInDown">
    <h1>503</h1>
    <h3 class="font-bold">Erro do Servidor Interno</h3>

    <div class="error-desc">
        O Serviço está indisponível. Nós pedimos desculpas.
        <br>
        Clique no botão abaixo para voltar.
        <form class="form-inline m-t" role="form">
            <button type="button" onclick="goBack()" class="btn btn-primary">Voltar</button>
        </form>
    </div>
</div>

<script src="{!! asset('js/jquery-3.1.1.min.js') !!}" type="text/javascript"></script>
<script src="{!! asset('js/bootstrap.min.js') !!}" type="text/javascript"></script>

</body>

<script>
    function goBack() {
        window.history.back();
    }
</script>

</html>
