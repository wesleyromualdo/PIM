<nav class="navbar-default navbar-static-side " role="navigation">
  <div class="sidebar-collapse">
      <?php echo $menuHtml ?>
  </div>
  <div class="slimScrollBar"></div>
</nav>

<div id="page-wrapper" class="gray-bg">
  <div class="row border-bottom">
    <nav class="navbar white-bg navbar-fixed-top" role="navigation" style="margin-bottom: 0">
      <div class="navbar-header">
        <a class="navbar-brand logo-sis" href="#">
          <img src="/zimec/public/img/logo-semPapel.png" class="img-responsive" style="max-width: 120px">
        </a>
        <a class="navbar-minimalize minimalize-styl-2 btn btn-primary " href="#" style="display: none;"><i class="fa fa-bars"></i> </a>
        <form class="navbar-form-custom" action="">
          <div class="form-group p-2">
            <select class="form-control" data-placeholder="Selecione" onchange="changeSystem(this.value)" style="border: 1px solid #e5e6e7!important;">
                <?php foreach ($sistemas as $sistema): ?>
                  <?php $selected = ($sistema['sisid'] == $_SESSION['sisid']) ? 'selected="selected"' : null; ?>
                  <option <?php echo $selected; ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                <?php endforeach ?>
            </select>
          </div>
        </form>
      </div>
      <ul class="header-nav header-nav-profile">
        <li class="dropdown ms-hover">
          <a href="javascript:void(0);" class="dropdown-toggle ink-reaction"  data-toggle="dropdown">
            <img alt="image" class="profile-pic" src="/seguranca/imagemperfil.php">
            <span class="profile-info">
            <?php echo $_SESSION['usunome']; ?>
            <small><?php echo $_SESSION['usufuncao']; ?></small>
            </span>
          </a>
          <ul class="dropdown-menu dropdown-menu-usuario animation-dock" >
            <form class="select-modulo-responsivo" style="display: none;">
              <select class="form-control" data-placeholder="Selecione" onchange="javascript:changeSystem(this.value)">
                  <?php foreach ($sistemas as $sistema): ?>
                      <?php $selected = ($sistema['sisid'] == $_SESSION['sisid']) ? 'selected="selected"' : null; ?>
                    <option <?php echo $selected; ?> value="<?php echo $sistema['sisid'] ?>"><?php echo $sistema['sisabrev'] ?></option>
                  <?php endforeach ?>
              </select>
            </form>
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
                <i class="glyphicon glyphicon-cog"></i> Gerenciar seus dados
              </a>
            </li>

            <li class="divider"></li>
            <li>
              <a onclick="javascript:window.location.href = '/logout.php'">
                <i class="fa fa-sign-out"></i> Sair
              </a>
            </li>
          </ul>
        </li>
      </ul>
    </nav>
  </div>