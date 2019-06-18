    <nav class="navbar-default navbar-static-side" role="navigation">
        <div class="sidebar-collapse">
            <?php echo $menuHtml ?>
        </div>
    </nav>

    <div id="page-wrapper" class="gray-bg">
    <div class="row border-bottom">

    <nav class="navbar navbar-fixed-top" role="navigation" style="margin: 0px; height: 71px; border-bottom: 1px solid #273A4A !important; background: #273A4A") no-repeat">
        <div class="navbar-left">
            <ul class="header-nav header-nav-options">
                <li class="header-nav-brand" style="cursor:pointer;" onclick="window.location='<?=$_SESSION['sisarquivo'] ?>.php?modulo=inicio&acao=C';"><img src="/zimec/public/img/logo_sp_horizontal.png" class="img-responsive" style="width: 139px; margin: 10px 0px;"></li>
                <li><a class="navbar-minimalize btn btn-primary" href="#"><i class="fa fa-bars"></i></a></li>
            </ul>
        </div>

    <div class="navbar-center hidden-md hidden-lg">
        <ul class="header-nav header-nav-profile">
            <li class="dropdown ms-hover">
                <a href="javascript:void(0);" class="dropdown-toggle ink-reaction" data-toggle="dropdown">
                    <img alt="image" class="profile-pic" src="/seguranca/imagemperfil.php">
                </a>
                <ul class="dropdown-menu animation-dock">
                    <li>
                        <a class="select-sistemas">
                            <select id="chosen-sistema" onclick="" class="form-control" data-placeholder="Selecione" onchange="javascript:changeSystem(this.value)">
                                <?php foreach ($sistemas as $sistema): ?>
                                    <?php $selected = ($sistema['sisid'] == $_SESSION['sisid']) ? 'selected="selected"' : null; ?>
                                    <option <?php echo $selected; ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                                <?php endforeach ?>
                            </select>
                            <script type="text/javascript">
                                $(document).ready(function() {
                                    $('.dropdown-menu select, .dropdown-menu a.select-sistemas').click(function(e) {
                                        e.stopPropagation();
                                    });
                                })
                            </script>
                        </a>
                    </li>
                    <?php if ($_SESSION['superuser'] == 1): ?>
                        <li class="dropdown-header">Super Usuário</li>
                    <?php else: ?>
                        <li class="dropdown-header">Opções Usuário</li>
                    <?php endif; ?>
                    <?php if ($_SESSION['superuser'] == 1 || $_SESSION['usucpf'] != $_SESSION['usucpforigem'] || $exibirSimular == true || $db->testa_uma() || $_SESSION['usuuma']): ?>
                        <li>
                            <?php if ($_SESSION['usucpf'] == $_SESSION['usucpforigem']): ?>
                                <a data-toggle="modal" class="btSimularUsuario">
                                    <i class="glyphicon glyphicon-eye-open"></i> Simular Usuário
                                </a>
                            <?php else: ?>
                                <a class="voltar_usuario_origem">
                                    <i  class="glyphicon glyphicon-eye-close"></i> Voltar Usuário
                                </a>
                            <?php endif; ?>
                        </li>
                    <?php endif; ?>
                    <li>
                        <a id="btUser" data-toggle="modal" data-target="#modal-user">
                            <i class="glyphicon glyphicon-wrench"></i> Gerenciar seus dados
                        </a>
                    </li>
                    <?php if ($_SESSION['superuser'] == 1 || $exibirGraficoWorflow == true): ?>
                        <li>
                            <?php $sql = "select tpdid from workflow.tipodocumento where sisid = '{$_SESSION['sisid']}'"; ?>
                            <?php $tipoDocumento = $db->pegaUm($sql); ?>
                            <?php if ($tipoDocumento): ?>
                                <a data-toggle="modal" class="btPainelWorkflow">
                                    <i class="glyphicon glyphicon-stats"></i> Painel do workflow
                                </a>
                            <?php endif ?>
                        </li>
                    <?php endif; ?>
                    <li class="divider"></li>
                    <li>
                        <a onclick="javascript:window.location.href = '/logout.php'">
                            <i class="fa fa-sign-out"></i> Sair
                        </a>
                    </li>
                </ul><!--end .dropdown-menu -->
            </li><!--end .dropdown -->
        </ul>
    </div>
    <div class="navbar-center hidden-xs">       
        <ul class="header-nav header-nav-options">
            <li style="width:55%">
                <select id="chosen-sistema" class="form-control chosen-select" data-placeholder="Selecione" onchange="javascript:changeSystem(this.value)">
                    <?php foreach ($sistemas as $sistema): ?>
                        <?php $selected = ($sistema['sisid'] == $_SESSION['sisid']) ? 'selected="selected"' : null; ?>
                        <option <?php echo $selected; ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                    <?php endforeach ?>
                </select>
            </li>
        </ul>
    </div>

    <div class="navbar-right hidden-xs">
        <ul class="header-nav header-nav-profile">
            <li class="dropdown ms-hover">
                <a href="javascript:void(0);" class="dropdown-toggle ink-reaction" data-toggle="dropdown">
                    <img alt="image" class="profile-pic" src="/seguranca/imagemperfil.php">
                    <span class="profile-info">
                                            <?php echo $_SESSION['usunome']; ?>
                        <small><?php echo $_SESSION['usufuncao']; ?></small>
                                        </span>
                </a>
                <ul class="dropdown-menu animation-dock">
                    <?php if ($_SESSION['superuser'] == 1): ?>
                        <li class="dropdown-header">Super Usuário</li>
                    <?php else: ?>
                        <li class="dropdown-header">Opções Usuário</li>
                    <?php endif; ?>
                    <?php if ($_SESSION['superuser'] == 1 || $_SESSION['usucpf'] != $_SESSION['usucpforigem'] || $exibirSimular == true || $db->testa_uma() || $_SESSION['usuuma']): ?>
                        <li>
                            <?php if ($_SESSION['usucpf'] == $_SESSION['usucpforigem']): ?>
                                <a data-toggle="modal" class="btSimularUsuario">
                                    <i class="glyphicon glyphicon-eye-open"></i> Simular Usuário
                                </a>
                            <?php else: ?>
                                <a class="voltar_usuario_origem">
                                    <i  class="glyphicon glyphicon-eye-close"></i> Voltar Usuário
                                </a>
                            <?php endif; ?>
                        </li>
                    <?php endif; ?>
                    <li>
                        <a id="btUser" data-toggle="modal" data-target="#modal-user">
                            <i class="glyphicon glyphicon-wrench"></i> Gerenciar seus dados
                        </a>
                    </li>
  
                    <li class="divider"></li>
                    <li>
                        <a onclick="javascript:window.location.href = '/logout.php'">
                            <i class="fa fa-sign-out"></i> Sair
                        </a>
                    </li>
                </ul><!--end .dropdown-menu -->
            </li><!--end .dropdown -->
        </ul>
        </nav>
    </div>