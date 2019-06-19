<!DOCTYPE html>
<html>
<head>
    <title>SIMEC - @yield('title')</title>
    @include('layouts.includes.meta')
    @include('layouts.includes.css')
</head>

<body class="fixed-nav {{$_COOKIE['navbar']}} ">

<div id="wrapper">

    @include('layouts.includes.navigation')

    <div class="loading-dialog notprint" id="loading">
        <div id="overlay" class="loading-dialog-content">
            <div class="ui-dialog-content">
                <img src="/library/simec/img/loading.gif">
                <span> O sistema está processando as informações. <br/>
                Por favor aguarde um momento...
            </span>
            </div>
        </div>
    </div>

    <div id="page-wrapper" class="gray-bg">

        @include('layouts.includes.topnavbar')

        @yield('content')

        @include('layouts.includes.footer')
    </div>

</div>
</body>

@include('layouts.includes.js')
</html>