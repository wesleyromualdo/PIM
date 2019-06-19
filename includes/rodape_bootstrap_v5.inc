<?php
    $arquivo = APPRAIZ . "version.txt";
    $myfile = fopen($arquivo, "r") or die("Unable to open file!");
    $versaoSvn = fgets($myfile);
    fclose($myfile);
?>
<!-- Modal -->
</div>

<div class="row">
    <div class="col-md-12">

        <div class="modal fade" id="modal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
        </div><!-- /.modal -->
        <div class="modal fade" id="modal-user" tabindex="0" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" >
            <div class="modal-dialog">
                <div class="panel panel-primary">
                    <div class="panel-heading">
                        <h3 class="panel-title">Informações de usuário</h3>
                    </div>
                    <div class="panel-body">
                        <div class="row">
                            <?php
                            $usuarioNome = $_SESSION['usunome'];
                            $usuarioEmail = $_SESSION['usuemail'];
                            $usuarioCpf = $_SESSION['usucpf'];
                            $usuarioCpfOrigem = $_SESSION['usucpforigem'];
                            ?>
                            <div class="col-md-3 col-lg-3 hidden-xs hidden-sm">
                                <img class="img-circle" src="/photo100.png" alt="User Pic">
                            </div>
                            <div class="col-xs-2 col-sm-2 hidden-md hidden-lg">
                                <img class="img-circle" src="/photo50.png" alt="User Pic">
                            </div>
                            <div class=" col-md-9 col-lg-9 hidden-xs hidden-sm">
                                <strong><?php echo ucwords(strtolower($usuarioNome)); ?></strong><br>
                                <table class="table ">
                                    <tbody>
                                        <tr>
                                            <td>CPF:</td>
                                            <td><?php echo formatar_cpf($usuarioCpf); ?></td>
                                        </tr>
                                        <tr style="border: solid 0px; ">
                                            <td>E-mail:</td>
                                            <td><?php echo $usuarioEmail ?></td>
                                        </tr>
                                        <tr style="border: solid 0px; ">
                                            <td>Perfil:</td>
                                            <td>
                                                <?php $perfisUsuarioOrigem = pegaPerfilPorUsuario($usuarioCpf, $_SESSION['sisid']); ?>
                                                <?php
                                                if (is_array($perfisUsuarioOrigem)):
                                                    foreach ($perfisUsuarioOrigem as $perfilUsuarioOrigem):
                                                        ?>
                                                        <?php if ($perfilUsuarioOrigem['pfldsc'] == 'Super Usuário'): ?>
                                                            <span class="label label-success"><?php echo $perfilUsuarioOrigem['pfldsc'] ?></span>
                                                        <?php else: ?>
                                                            <span class="label label-primary"><?php echo $perfilUsuarioOrigem['pfldsc'] ?></span>
                                                        <?php endif; ?>
                                                    <?php endforeach; ?>
<?php endif; ?>
                                            </td>
                                        </tr>
                                    </tbody>
                                </table>
                            </div>
                            <div class="row" style="height: 300px; overflow:auto; border: 1px solid white" >
                                <br/>
                                <?php
                                $sql = "
                                            SELECT
                                            sis.sisabrev ,
                                            avb.avbmsg   ,
                                            avb.avburl   ,
                                            to_char(avb.dataultimaalteracao, 'dd/mm/yyyy às hh:mi') as datahora,
                                            avb.avbstatus
                                           FROM
                                            public.avisousuario avb
                                           JOIN
                                            seguranca.sistema sis
                                           USING
                                            ( sisid )
                                            WHERE
                                             usucpf = '{$_SESSION['usucpf']}' ORDER BY avb.dataultimaalteracao DESC LIMIT 30";
                                $avisosBootstrap = $db->carregar($sql);
                                #ver($sql, $avisosBootstrap);
                                if (is_array($avisosBootstrap)) {
                                    foreach ($avisosBootstrap as $valor) {
                                        $nova = "";
                                        if ($valor['avbstatus'] == 'A') {
                                            $nova = " background-color:#D3D3D3 !important; ";
                                        }
                                        $saida = "<div style=\"font-size: 12px; padding: 4px; {$nova}\">  Em <b>{$valor['datahora']}</b> no sistema <b>{$valor['sisabrev']}</b> você recebeu a mensagem: ";
                                        $saida .= "{$valor['avbmsg']}";
                                        $saida .= "&nbsp; <a class=\"confirmarAviso\" title=\"Ir para a referência.\" href = \"{$valor['avburl']}\" target=\"_top\" ><span class=\"glyphicon glyphicon-screenshot\" aria-hidden=\"true\"></span></a>";
                                        $saida .= "</div> <hr style=\"padding:0px;  margin: 3px;\">";
                                        echo $saida;
                                    }
                                }
                                ?>
                            </div>
                        </div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                    </div>
                </div><!-- /.modal-content -->
                <!-- /.modal-content -->
            </div><!-- /.modal-dialog -->
        </div><!-- /.modal -->
        <!-- Modal -->
    </div>
    <div class="row">
        <div class="col-md-12">
            <div class="modal fade" id="modal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                <!--deleteByemail('asdiaosdas')-->
            </div><!-- /.modal -->
            <div class="modal fade" id="modal-user-search" tabindex="0" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" >
                <div class="modal-dialog modal-lg">
                    <div id="container-search-users" class="modal-content text-center">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                            <h4 class="modal-title">Simular usuário</h4>
                        </div>
                        <div class="modal-body">
                            <div class="well">
                                <form class="form-horizontal" id="formulario-simular-usuario" method="post">
                                    <input type="hidden" name="carregarUsuariosSistema" value="true">
                                    <fieldset>
                                        <legend>Pesquisar usuário neste módulo</legend>
                                        <div class="form-group">
                                            <label for="nomeUsuario" class="col-lg-2 control-label">Usuário</label>
                                            <div class="col-lg-10">
                                                <input type="text" class="form-control" id="nomeUsuario" name="nomeSimularUsuario" placeholder="Nome, CPF ou E-mail">
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <label for="select" class="col-lg-2 control-label">Perfil</label>
                                            <div class="col-lg-10">
                                                    <?php $perfis = carregarPerfils($_SESSION['sisid']); ?>
                                                <select id="perfisSimularUsuario" name="perfisSimularUsuario[]" multiple class="form-control" data-placeholder="Selecione">
<?php foreach ($perfis as $perfil): ?>
                                                        <option value="<?php echo $perfil['codigo'] ?>"><?php echo $perfil['descricao'] ?></option>
<?php endforeach; ?>
                                                </select>
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <div class="col-lg-12">
                                                <a href="javascript:void(0);" class="btn btn-primary" id="bt-carregar-usuarios-sistema-simular">Procurar</a>
                                            </div>
                                        </div>
                                    </fieldset>
                                </form>
                            </div>
                            <div id="container-lista-simular">

                            </div>
                        </div>
                        <div class="modal-footer">
                            <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                        </div>
                    </div>
                    <!-- /.modal-content -->
                </div><!-- /.modal-dialog -->
            </div><!-- /.modal -->
            <script>
            $1_11(document).ready(function() {
                $1_11('#btSimularUsuario').click(function () {
                    $1_11('#modal-user-search').modal();
                    setTimeout(function () {
                        $1_11('#perfisSimularUsuario').chosen({});
                    }, 300);
                });
                $1_11('#bt-carregar-usuarios-sistema-simular').click(function () {
                    var form = $1_11('#formulario-simular-usuario');
                    $.ajax({
                      type: "POST",
                      data: form.serialize(),
                      success: function(html)
                      {
                        $1_11('#container-lista-simular').hide().html(html).fadeIn();
                      }
                    });
                    // $1_11('#formulario-simular-usuario').ajaxSubmit({type: 'post'}, function (html) {
                    //     $1_11('#container-lista-simular').hide().html(html).fadeIn();
                    // });
                    return false;
                });
            });
            </script>
            <?php
            $dt_inicio = $_REQUEST['dt_inicio'] ? $_REQUEST['dt_inicio'] : '01/01/' . date('Y');
            $dt_fim = $_REQUEST['dt_fim'] ? $_REQUEST['dt_fim'] : date('d/m/Y');
            ?>
            <!-- MODAL - PAINEL WORKFLOW -->
            <div class="modal fade" id="modal-painel-workflow" tabindex="0" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" >
                <div class="modal-dialog-large">
                    <div id="container-search-users" class="modal-content text-center">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                            <h4 class="modal-title">Painel</h4>
                        </div>
                        <div class="modal-body">
                            <div class="well">
                                <form class="form-horizontal" id="form-painel-workflow" method="post">
                                    <input type="hidden" name="carregarUsuariosSistema" value="true">
                                    <fieldset>
                                        <div class="form-group">
                                            <label for="nomeUsuario" class="col-lg-2 control-label">Fluxo</label>
                                            <div class="col-lg-10">
                                                    <?php $tipoDocumentos = carregarTipoDocumentos($_SESSION['sisid']); ?>
                                                <select id="fluxo-painel-workflow" name="tpdid" class="form-control" data-placeholder="Selecione...">
                                                    <option value=""></option>
<?php foreach ($tipoDocumentos as $tipoDocumento): ?>
                                                        <option value="<?php echo $tipoDocumento['codigo'] ?>"><?php echo $tipoDocumento['descricao'] ?></option>
<?php endforeach; ?>
                                                </select>
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <label for="select" class="col-lg-2 control-label">Situação</label>
                                            <div id="situacao-painel-workflow" class="col-lg-10">
<?php $tipoDocumentos = carregarTipoDocumentosPainel($_SESSION['sisid']); ?>
                                                <select id="estado-documento-situacao-workflow" name="esdid[]" class="form-control" data-placeholder="Selecione...">
                                                    <option value=""></option>
                                                </select>
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <label for="select" class="col-lg-2 control-label">Período</label>
                                            <div class="col-lg-12">
                                                <div class="col-lg-2">
                                                    <input name="dt_inicio" type="text" class="form-control" id="dt-inicio-painel-workflow" placeholder="00/00/0000" maxlength="10" value="<?php echo $dt_inicio ?>" data-format="dd/MM/yyyy hh:mm:ss">
                                                </div>
                                                <div class="col-lg-2">
                                                    <input name="dt_fim" type="text" class="form-control" id="dt-fim-painel-workflow" placeholder="00/00/0000" maxlength="10" value="<?php echo $dt_fim ?>" data-format="dd/MM/yyyy hh:mm:ss">
                                                </div>
                                            </div>
                                        </div>
                                        <div class="form-group">
                                            <label for="select" class="col-lg-2 control-label">Exibição</label>
                                            <div class="col-lg-10">
                                                <div class="col-lg-10 text-left">
                                                    <input type="radio" name="tipo_exibicao" id="" value="D" style="margin-left: 10px;">
                                                    Diário
                                                    <input type="radio" name="tipo_exibicao" id="" value="M" checked="checked" style="margin-left: 10px;">
                                                    Mensal
                                                    <input type="radio" name="tipo_exibicao" id="" value="A" style="margin-left: 10px;">
                                                    Anual
                                                </div>
                                            </div>
                                        </div>
                                    </fieldset>
                                </form>
                            </div>
                            <div id="container-grafico-painel-workflow">
                            </div>
                        </div>
                        <div class="modal-footer">
                            <button class="btn btn-primary" id="bt-procurar-painel-workflow" ">Procurar</button>
                            <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                        </div>
                    </div>
                    <!-- /.modal-content -->
                </div><!-- /.modal-dialog -->
            </div><!-- /.modal -->

            <div class="modal fade" id="modal-alert" tabindex="0" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true" >
                <div class="modal-dialog">
                    <div class="modal-content text-center">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                            <h4 class="modal-title" style="text-align:left">Aviso!</h4>
                        </div>
                        <div class="modal-body">

                        </div>
                        <div class="modal-footer">
                            <button type="button" class="btn btn-danger" data-dismiss="modal">Ok</button>
                        </div>
                    </div><!-- /.modal-content -->
                </div><!-- /.modal-dialog -->
            </div><!-- /.modal -->
            <div class="modal fade" id="modal-confirm">
                <div class="modal-dialog">
                    <div class="modal-content">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                            <h4 class="modal-title">Aviso!</h4>
                        </div>
                        <div class="modal-body">
                            <p></p>
                        </div>
                        <div class="modal-footer">
                            <button type="button" class="btn btn-default" data-dismiss="modal">Não</button>
                            <button type="button" class="btn btn-primary">Sim</button>
                        </div>
                    </div><!-- /.modal-content -->
                </div><!-- /.modal-dialog -->
            </div><!-- /.modal -->
        </div>
    </div>
    <div class="row view-rodape">
        <div class="col-md-5">
            Data: <?= date("d/m/Y - H:i:s") ?> - Último acesso (<?= formata_data($_SESSION['usuacesso']); ?>) -
            <a href="javascript:abrirUsuariosOnline();" >
                <span id="rdpUsuariosOnLine"><?= $_SESSION['qtdusuariosonline'][$_SESSION['sisid']]; ?></span>Usuários Online</a>
        </div>
        <div class="col-md-2 text-center">
            <a href="javascript: abrirRegras( <?= $_SESSION['mnuid']; ?> );">Visualizar Regras</a>
        </div>
        <div class="col-md-5 text-right">
            SIMEC - Fale Conosco Manual	| Tx.: 0,2015s / 0,34
            <?php if (1 == $_SESSION['superuser']): ?>
                    | Revisão: <?php echo $versaoSvn; ?>
            <?php endif; ?>
        </div>
    </div>

    <script type="text/javascript" language="javascript">
        function abrirUsuariosOnline()
        {
            window.open(
                    '../geral/usuarios_online.php',
                    'usuariosonline',
                    'height=500,width=600,scrollbars=yes,top=50,left=200'
                    );
        }
    </script>

    <!-- The blueimp Gallery widget -->
    <div id="blueimp-gallery" class="blueimp-gallery blueimp-gallery-controls" data-filter=":even" style="display: none;">
        <div class="slides"></div>
        <h3 class="title"></h3>
        <a class="prev" style="font-size: 30px;" ><span class="glyphicon glyphicon-chevron-left"></span></a>
        <a class="next" style="font-size: 30px;" ><span class="glyphicon glyphicon-chevron-right"></span></a>
        <a class="close">?</a>
        <a class="play-pause"></a>
        <ol class="indicator"></ol>
    </div>
</body>
</html>
<?php
// -- Caso a página requisitada seja uma página existente, realiza o log de estatísticas - Verificação necessária pois
// -- ainda não foi possível reproduzir o erro no sistema financeiro que faz com que todas as imagens do tema do sistema
// -- sejam requeridas pelo browers como um módulo. Esta mesma verificação é feita no controleAcesso no momento de
// -- incluir os arquivos.
if (file_exists(realpath(APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc"))) {
    global $db;
    global $Tinicio;

    if (isset($_SESSION['mnuid']) && !empty($_SESSION['mnuid'])) {
        simec_gravar_estatistica();
    }
}
?>


    <script type="text/javascript">

    $(document).ready(function() {
        $(".date").datepicker({
            numberOfMonths: 1,
            dateFormat: 'dd/mm/yy',
            showWeek: true,
            'showAnim':'drop'
        });
    });

    //$.datepicker.regional[ 'pt-BR' ];
    var config = {
        '.chosen-select': {allow_single_deselect: true},
        '.chosen-select-deselect': {allow_single_deselect: true},
        '.chosen-select-no-single': {disable_search_threshold: 10},
        '.chosen-select-no-results': {no_results_text: 'Oops, nothing found!'},
        '.chosen-select-width': {width: "95%"}
    }
    for (var selector in config) {
        $1_11(selector).chosen(config[selector]);
    }
    /** Função de fazer com que o sistema informe que esta havendo uma requisição ajax */
    $1_11(document).ajaxSend(function (e, jqxhr, settings) {
        $1_11("#loading").fadeIn();

    }).ajaxStop(function () {
        $1_11("#loading").fadeOut();
    });

    /** Mensagem de carregando quando houver requisições em ajax */
    $1_11.ajaxSetup({
        timeout: 0,
        error: function (xhr, status, error) {
            console.log("Ocorrência de erro no retorno AJAX: " + status + "\nError: " + error);
            $1_11("#loading").fadeOut();
            $1_11("#loading").fadeIn();

            setTimeout(function () {
                $1_11("#loading").fadeOut();
            }, 1300);
        }
    });

    // -- Substituindo o alert do browser.
    window.alert = function (texto)
    {
        texto = '<b>' + replaceAll(texto, '\n', '<br/>') + '</b>';
        
        $1_11('#modal-alert .modal-body').html(texto);
        $1_11('#modal-alert').modal();
    };


    $1_11(".dropdown").on('hidden.bs.dropdown', function(e, visibility){
        
        if(visibility == "shown") {
            
        } else {
            $1_11(this).show()
        }
    })

</script>
