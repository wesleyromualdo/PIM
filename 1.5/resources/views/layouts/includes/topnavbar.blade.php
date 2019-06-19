<div class="row border-bottom">
    <nav class="navbar navbar-fixed-top custom-navbar-simec" role="navigation" style="">

        <div class="navbar-left">
            <ul class="header-nav header-nav-options">
                <li class="header-nav-brand" style="cursor:pointer;">
                <img src="http://simec.mec.gov.br/estrutura/temas/default/img/logo-simec.png" class="img-responsive custom_logo" style="width: 139px; margin: 10px 0px;"></li>
                <li><a class="navbar-minimalize btn btn-primary" href="#"><i class="fa fa-bars"></i></a></li>
            </ul>
        </div>

        @include('layouts.includes.topnavbar_profile_mobile')

        <div class="navbar-center hidden-xs modulos">

            <ul class="header-nav header-nav-options" style="display: flex;">
                <li style="width:55%">
                    <select class="form-control chosen-select mudar_sistema" data-placeholder="Selecione">
                        {!! getOptionSistemas($user->usucpf) !!}
                    </select>
                </li>
                    @if (env('ENV') !== 'production')
                        <li class="hidden-sm hidden-xs">
                            <span class="database-info">
                                <i class="fa fa-database textoInfoBanco"></i> Banco de Dados:<br>
                                {{ $database }}
                            </span>
                        </li>
                    @endif

                <?php /*if($_SESSION['sisdiretorio'] == 'recorcv2' && $_SESSION['superuser'] == 1): ?>
                <li style="width:55%">
                    <select id="mudar_uo" class="form-control chosen-select" data-placeholder="Selecione">
                        {{getUnidadeOrcamentaria()}}
                    </select>
                </li>
                <?php endif;*/ ?>
            </ul>

        </div>

        @include('layouts.includes.topnavbar_profile')

    </nav>
    @if ($user->usucpf != $user->usucpforigem)
        <div id="" class="avisoSimulandoUsuario" style="position: fixed; z-index: 2029; border: 0px solid #E11919; background: #BD5A5A; width: 100%; margin-top: 2px; padding: 3px 0px 3px 15px; color: #FFFFFF;">
            Você está simulando um usuário e por isso, <b>CUIDADO! Pois os dados ainda podem ser salvos.</b><!--<b>NÃO conseguirá salvar os dados</b>.-->
        </div>
        @endif
</div>