<?php
$perfil = pegaPerfilGeral();

if(empty($perfil)){
	$perfil = array();
}

if (in_array(Par3_Model_UsuarioResponsabilidade::PRESIDENTE_CACS, $perfil) || in_array(Par3_Model_UsuarioResponsabilidade::CONSELHEIRO_CACS, $perfil)) {
    include_once APPRAIZ . 'par3/classes/model/seguranca/Usuario.class.inc';
    $modelUsuario = new Seguranca_Model_Seguranca_Usuario($_SESSION['usucpf']);

    $disabledUf = ( $modelUsuario->regcod ? 'disabled' : '');
    $disabledMuncod = ( $modelUsuario->muncod ? 'disabled' : '');

    if ($_POST['action'] == 'getMunicipios') {
        $modelUsuario->popularDadosObjeto($_POST);
        $mun = $modelUsuario->getComboMunicipios($_POST['regcod']);
        die($mun);
    }

    $modulo = "inicio";

    if (!$_SESSION ['logincadastro']) {

        if ($_POST ['act'] == 'alterar') {
            // checagem de consistencias
            $sql = "select ususenha from seguranca.usuario where usucpf='" . $_SESSION ['usucpf'] . "'";
            $senha_banco = $db->recuperar($sql);
            $senha = md5_decrypt_senha($senha_banco ['ususenha'], '');

            if ($senha != $_POST ['senhaatual']) { ?>
                <!--A senha atual não confere-->
                <script>
                    alert('A senha atual não confere.');
                    history.back();
                </script>

                <? exit ();
            }
        }
    }

    if (($_REQUEST ['act'] == 'alterar') and (!is_array($msgerro))) {
        $saida = $modelUsuario->inserirUsuario($_POST);
        unset($_SESSION['logincadastro']);
        unset($_SESSION['chave']);
        $db->sucesso($modulo, '', 'Usuário atualizado com sucesso!');

    }


    include APPRAIZ . "includes/cabecalho_bootstrap_v3.inc";

    if ($_SESSION ['logincadastro']) {
        print '<section class="alert alert-info col-md-6 col-md-offset-3 text-center">';
        print "<p>Seu cadastro foi realizado com sucesso!</p>";
        print "<p>Entretanto, é necessário que você troque-a no primeiro acesso.</p>";
        print '</section>';
        print '<br>';
    }

    ?>
    <div class="row wrapper border-bottom white-bg page-heading">
        <div class="col-lg-12">
            <h2>Alteração de Senha</h2>
            <ol class="breadcrumb">
                <li>
                    <a href="par3.php?modulo=inicio&amp;acao=C">Página Inicial</a>
                </li>
                <li><a>Principal</a></li>
            </ol>
        </div>
    </div>
    <div class="wrapper wrapper-content animated fadeIn">
        <div class="ibox float-e-margins">
            <div class="ibox-title">
                <h5>Formulario de alteração de senha</h5>
            </div>
            <div class="ibox-content">
                <div class="row">
                    <form method="POST" name="formulario" id="formAltSenha" class="form-horizontal">

                        <input type=hidden name="modulo" value="<?= $modulo ?>">
                        <input type=hidden name="act" id="act" value=0>

                        <div class="col-lg-6">

                            <? if ($_SESSION['logincadastro']) { ?>
                                <input type=hidden name="senhaatual" value="OK">
                            <? } ?>

                            <div class="well">
                                <? if (!$_SESSION['logincadastro']) { ?>
                                    <div class="form-group" id="divsenhaatual">
                                        <label class="control-label col-lg-5" for="senhaatual"> Senha atual:</label>
                                        <div class="col-lg-7">
                                            <input type='password' class="form-control" name='senhaatual' id='senhaatual' size='20'>
                                        </div>
                                    </div>
                                <? } ?>

                                <div class="form-group" id="divnovasenha">
                                    <label class="control-label col-lg-5" for="novasenha"> Nova senha:</label>
                                    <div class="col-lg-7">
                                        <input type='password' class="form-control" name='novasenha' id='novasenha'
                                               size='20'>
                                    </div>
                                </div>

                                <div class="form-group" id="divconfsenha">
                                    <label class="control-label col-lg-5" for="confsenha"> Confirma nova senha:</label>
                                    <div class="col-lg-7">
                                        <input type='password' class="form-control" name='confsenha' id='confsenha' size='20'>
                                    </div>
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="usuemail"
                                       class="col-lg-5 control-label">E-mail:</label>

                                <div class="col-lg-7">
                                    <input type="text" id="usuemail" name="usuemail" class="form-control">
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="usufonenum"
                                       class="col-lg-5 control-label">Telefone:</label>

                                <div class="col-lg-2">
                                    <input type="text" id="usufoneddd" name="usufoneddd" class="form-control"
                                           placeholder="__" maxlength="2" size="2">
                                </div>
                                <div class="col-lg-5">
                                    <input type="text" id="usufonenum" name="usufonenum" class="form-control" placeholder="____-____">
                                </div>
                            </div>
                        </div>

                        <div class="col-lg-6">
                            <div class="form-group">
                                <label class="col-lg-3 control-label" for="regcod">Unidade Federal:</label>

                                <div class="col-lg-8">
                                    <select id="regcod" name="regcod" class="form-control" <?= $disabledUf ?>>
                                        <?php echo $modelUsuario->getComboUfs(); ?>
                                    </select>
                                </div>
                            </div>

                            <div class="form-group" dislplay="none">
                                <label class="col-lg-3 control-label" for="muncod">Município:</label>

                                <div class="col-lg-8">
                                    <select id="muncod" name="muncod" class="form-control" <?= $disabledMuncod ?> >
                                        <option value=""> Selecione uma UF</option>
                                    </select>
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="usudatanascimento" class="col-lg-3 control-label">Data de Nascimento:</label>

                                <div class="col-lg-3">
                                    <input id="usudatanascimento" name="usudatanascimento" type="text"
                                           class="form-control data" maxlength="10" placeholder="dd/mm/aaaa">
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="ususexo" class="col-lg-3 control-label">Sexo:</label>

                                <div class="checked">
                                    <input type="radio" value="M" name="ususexo" checked> Masculino
                                    <input type="radio" value="F" name="ususexo">Feminino
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="col-lg-offset-5 col-lg-7 text-right">
                                <input type="button" class="botao btn btn-primary" name="btalterar" value="Atualizar" onclick="validar_cadastro()"> <br> <br>
                            </div>
                        </div>

                    </form>
                </div>
            </div>
        </div>
    </div>

    <script>
        function validar_cadastro() {
            var mensagemValidacao = 'Os campos abaixo devem ser preenchidos:\n';
            var controleValidacao = true;

            if ($('#senhaatual').val() == '') {
                $("#divsenhaatual").removeClass('form-group').addClass('form-group has-error');
                mensagemValidacao += 'Informe a Senha Atual.\n';
                controleValidacao = false;
            } else {
                $("#divsenhaatual").removeClass('form-group has-error').addClass('form-group');
            }

            if ($('#novasenha').val() == '') {
                $("#divnovasenha").removeClass('form-group').addClass('form-group has-error');
                mensagemValidacao += 'Informe a Nova Senha.\n';
                controleValidacao = false;
            } else {
                $("#divnovasenha").removeClass('form-group has-error').addClass('form-group');
            }

            if ($('#confsenha').val() == '') {
                $("#divconfsenha").removeClass('form-group').addClass('form-group has-error');
                mensagemValidacao += 'Confirma a nova senha.\n';
                controleValidacao = false;
            } else {
                $("#divconfsenha").removeClass('form-group has-error').addClass('form-group');
            }

            if (($('#novasenha').val()) != '' && ($('#novasenha').val().length < 4) || ($('#novasenha').val().length > 12)) {
                mensagemValidacao += "A senha deve ter um mínimo de 4 caracteres e ter um máximo de 12 caracteres.\n";
                controleValidacao = false;
            }
            if ($('#novasenha').val() != $('#confsenha').val()) {
                mensagemValidacao += 'A nova senha não confere com a segunda digitação.\n';
                controleValidacao = false;
            }


            if ($('#usuemail').val() == '') {
                $("#usuemail").closest('.form-group').removeClass('form-group').addClass('form-group has-error');
                mensagemValidacao += 'Informe o E-mail.\n';
                controleValidacao = false;
            } else {
                $("#usuemail").closest('.form-group').removeClass('form-group has-error').addClass('form-group');
            }

            if ($('#usufoneddd').val() == '') {
                $("#usufoneddd").closest('.form-group').removeClass('form-group').addClass('form-group has-error');
                mensagemValidacao += 'Informe o DDD.\n';
                controleValidacao = false;
            } else {
                $("#usufoneddd").closest('.form-group').removeClass('form-group has-error').addClass('form-group');
            }

            if ($('#usufonenum').val() == '') {
                $("#usufonenum").closest('.form-group').removeClass('form-group').addClass('form-group has-error');
                mensagemValidacao += 'Informe o Número de telefone.\n';
                controleValidacao = false;
            } else {
                $("#usufonenum").closest('.form-group').removeClass('form-group has-error').addClass('form-group');
            }

            if ($('#regcod').val() == '') {
                $("#regcod").closest('.form-group').removeClass('form-group').addClass('form-group has-error');
                mensagemValidacao += 'Informe a Unidade Federal.\n';
                controleValidacao = false;
            } else {
                $("#regcod").closest('.form-group').removeClass('form-group has-error').addClass('form-group');
            }

            if ($('#muncod').val() == '') {
                $("#muncod").closest('.form-group').removeClass('form-group').addClass('form-group has-error');
                mensagemValidacao += 'Informe o Município.\n';
                controleValidacao = false;
            } else {
                $("#muncod").closest('.form-group').removeClass('form-group has-error').addClass('form-group');
            }

            if ($('#usudatanascimento').val() == '') {
                $("#usudatanascimento").closest('.form-group').removeClass('form-group').addClass('form-group has-error');
                mensagemValidacao += 'Informe a Data de Nascimento.\n';
                controleValidacao = false;
            } else {
                $("#usudatanascimento").closest('.form-group').removeClass('form-group has-error').addClass('form-group');
            }

            if (controleValidacao) {
                $('#act').val('alterar');
                console.log('sadfasd');
                $('#formAltSenha').submit();
            } else {
                alert(mensagemValidacao);
                return false;
            }

        }

        $(document).ready(function () {
            $("#usucpf").mask("999.999.999-99");
            $("#usufoneddd").mask("99");
            $("#usufonenum").mask("9999-9999");

            $(".data").datepicker({
                defaultDate: "-25y +1w",
                changeMonth: true,
                changeYear: true,
                numberOfMonths: 1,
                showAnim: 'fadeIn',
                language: 'pt-BR',
                format: 'dd/mm/yyyy'
            });

            if( $('#regcod').val() ){
                getMunicipios( $('#regcod').val() );
            }

            $('#regcod').on('change', function (e) {
                getMunicipios($(this).val());
            });


            $("#senhaatual").bind({
                copy: function () {
                    alert('Ao recuperar a senha, não copie e cole no navegador.');
                    return false;
                },
                paste: function () {
                    alert('Ao recuperar a senha, não copie e cole no navegador.');
                    return false;
                },
                cut: function () {
                    alert('Ao recuperar a senha, não copie e cole no navegador.');
                    return false;
                }
            });

            $("#novasenha").bind({
                copy: function () {
                    alert('Ao recuperar a senha, não copie e cole no navegador.');
                    return false;
                },
                paste: function () {
                    alert('Ao recuperar a senha, não copie e cole no navegador.');
                    return false;
                },
                cut: function () {
                    alert('Ao recuperar a senha, não copie e cole no navegador.');
                    return false;
                }
            });

            $("#confsenha").bind({
                copy: function () {
                    alert('Ao recuperar a senha, não copie e cole no navegador.');
                    return false;
                },
                paste: function () {
                    alert('Ao recuperar a senha, não copie e cole no navegador.');
                    return false;
                },
                cut: function () {
                    alert('Ao recuperar a senha, não copie e cole no navegador.');
                    return false;
                }
            });
        });

        function getMunicipios(regcod) {
            $.post(window.location.href, {
                action: 'getMunicipios',
                regcod: regcod
            }, function (data) {
                $('#muncod').html(data);
            });
        }
    </script>
    <?php
} else {
    
    include_once APPRAIZ . 'zimec/public/temas/simec/partial/usuario/senha.phtml';
}
