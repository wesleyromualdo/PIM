<div class="navbar-right hidden-xs">
    <ul class="header-nav header-nav-profile">
        <li class="dropdown ms-hover">

            <a href="javascript:void(0);" class="dropdown-toggle ink-reaction" data-toggle="dropdown">
                <img alt="image" class="profile-pic" src="/seguranca/imagemperfil.php">
                <span class="profile-info"> 
                    @if ($user->usucpf == $user->usucpforigem)
                        {{$user->usunome}}
                    @else 
                        {{ $_SESSION['dadosusuario'][$_SESSION['usucpf']]['descricao'] }}
                    @endif
                
                </span>
            </a>

            <ul class="dropdown-menu animation-dock">
                @if (!empty($_SESSION['superuser']) && $_SESSION['superuser'])

                    <li class="dropdown-header"> Super Usuário</li>
                @else
                    <li class="dropdown-header">Opções de Usuário</li>
                @endif

                @if ( !empty($user->superuser) && ( $user->superuser == 1 || $user->usucpf != $user->usucpforigem || Modules\Seguranca\Entities\PerfilUsuario::verficarPermissaoEmularUsuario($user->usucpf, $user->sisid)) )
                    <li>
                        @if ($user->usucpf == $user->usucpforigem)
                            <a data-toggle="modal" class="btSimularUsuario" id="btnSimularUsuario" data-target="#modal-simular">
                                <i class="glyphicon glyphicon-eye-open"></i> Simular Usuário
                            </a>
                        @else
                            <a class="voltar_usuario_origem" id="btnVoltarOrigemUsuario">
                                <i class="glyphicon glyphicon-eye-close"></i> Voltar Usuário
                            </a>
                        @endif
                    </li>

                    @if (!empty($_SESSION['unidadesPerfil']) && $_SESSION['sisid'] == 266)
                    <li>
                        <a id="btUO" data-toggle="modal" data-target="#modal-uo">
                            <i class="glyphicon glyphicon-wrench"></i> Atribuir UO a um usuário
                        </a>
                    </li>
                    @endif
                @endif

                <li>
                    <a id="btUser" data-toggle="modal" data-target="#modal-user">
                        <i class="glyphicon glyphicon-wrench"></i> Gerenciar seus dados
                    </a>
                </li>

                <li class="divider"></li>

                <li>
                    <a href="/logout.php">
                        <i class="fa fa-sign-out"></i> Sair
                    </a>
                </li>
            </ul>

        </li>
    </ul>
</div>