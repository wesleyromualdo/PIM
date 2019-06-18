<?php

$perfils = pegaPerfils($_SESSION['usucpf']);

$arrPerfil = array(
    Par3_Model_UsuarioResponsabilidade::PREFEITO,
    Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL,
);

?>
<style type="text/css">
    .area-link {
        color: #333;
    }

    .area-link:hover {
        text-decoration: underline;
    }
</style>
<div id="tab-resumo" class="tab-pane" style="display: block;">
    <div class="panel-body">
        <?php

        $pendenciasHelper = new Par3_Helper_PendenciasEntidade();
        $pendencias = $pendenciasHelper->carregarPendenciasPreenchimento($_REQUEST['inuid']);

        if ($pendencias){
            $d = 0;
            if ($existePendenciaGeral){ ?>
        <div class="widget red-bg p-lg text-center">
            <div class="row">
                <div class="col-md-1">
                    <i class="fa fa-warning fa-4x"></i>
                </div>
                <div class="col-md-11 text-left">
                    <h3>PENDÊNCIAS DE PREENCHIMENTO</h3>
                    <h4>Para <?php echo $verificaInicioDiagnostico ? 'iniciar': 'finalizar'; ?> o diagnóstico, será necessário preencher todos os itens indicados abaixo.</h4>
                    <?php //if (in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfils)) : ?>
                        <?php $url_action = 'par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid='.$_REQUEST['inuid'].'&visualizar-sintese-diagnostico=1'; ?>
                        <a class="btn btn-info pull-right" href="<?=$url_action;?>" role="button">Visualizar Diagnóstico</a>
                    <?php //endif; ?>
                </div>
            </div>
        </div>
            <?php
            }else{ ?>
        <div class="widget navy-bg p-lg text-center" id="nao_imprimir">
            <div class="row">
                <div class="col-md-1">
                    <i class="fa fa-thumbs-o-up fa-4x"></i>
                </div>
                <div class="col-md-11 text-left">
                    <h3>ETAPA PREPARATÓRIA CONCLUIDA</h3>
                    <h4>Você completou a etapa preparatória do diagnóstico !!!!!!.</h4>
                </div>
            </div>
        </div>
            <?php
            } ?>
        <br>
        <div class="row">
            <?php
            foreach ($pendencias as $numeroPendencia => $pendencia){
                $d++;
                switch($numeroPendencia) {
                    case 0:
                        $corSintesePendencia = '1ab394';
                        break;
                    case 1:
                        $corSintesePendencia = '1c83c6';
                        break;
                    default:
                        $corSintesePendencia = 'f8ab59';
                        break;
                }
                        ?>
            <div class="col-lg-3">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>
                            <?php echo $pendencia['descricao']; ?>&nbsp;
                            <span class="badge" style=" color: #<?php echo $corSintesePendencia; ?>; background-color: #<?php echo $corSintesePendencia; ?>">1</span>
                        </h5>
                    </div>
                    <div class="ibox-content no-margin">
                        <table class="table table-stripped">
                            <tbody>
                <?php
                if(
                    isset($pendencia['itens']) &&
                    count($pendencia['itens'])
                ){
                    $existePendenciaItem = false;
                    foreach($pendencia['itens'] as $item){ ?>
                                <tr>
                        <?php
                        if($item['percent'] < 100){
                            $existePendenciaItem = true; ?>
                                    <td width="20px">
                                        <i class="fa fa-times-circle text-danger"></i>
                                    </td>
                                    <td>
                                        <a class="text-primary pendencia" href="<?php echo $item['url']; ?>">
                                            <?php echo $item['descricao']; ?> <i class="fa fa-edit"></i>
                                        </a>
                                    </td>
                        <?php
                        }?>
                                </tr>
                    <?php
                    }
                }
                if (!$existePendenciaItem){ ?>
                                    <td colspan="2">
                                        <h4 style="margin: 10px;">
                                            <i class="fa fa-check-circle success fa-2x" style="vertical-align: middle;"></i>
                                            <span>Preenchimento concluido</span>
                                        </h4>
                                    </td>
                <?php
                } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
            <?php
            }
        } ?>
        </div>
        <br>
        <?php //Se for super usuário não mostra alerta de pendencia
        if (!$_SESSION['visualizar-sintese-diagnostico']){

            if (!$verificaInicioDiagnostico && !$existePendenciaGeral){ ?>
        <div class="modal fade" id="_modal-alerta2" tabindex="-1" role="dialog">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                            <span aria-hidden="true">&times;</span>
                        </button>
                        <h4 class="modal-title">
                            <h4>Parabéns!</h4>
                        </h4>
                    </div>
                    <div class="modal-body">
                        <div class="widget navy-bg p-lg text-center in" role="alert">
                            <br />
                            <h1>Você completou a etapa preparatória do diagnóstico!</h1>

                            <h3>Deseja iniciar o dignóstico do seu <?php echo $nivel?>?</h3>
                            <br />
                            <p>
                                <button type="button" class="btn btn-success iniciarDiagnotico">
                                    <label style="font-size: 24px;cursor:pointer;"><b>Sim, quero iniciar!</b></label>
                                </button>
                                <br><br>
                                <button type="button" class="btn btn-default diagnosticoSair" data-dismiss="modal">Não quero iniciar agora.</button>
                            </p>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <script type="text/javascript">
            $(function(){

                $("#_modal-alerta2").modal({backdrop:'static'});

                $(".iniciarDiagnotico").on("click", function(){
                    var param = [];
                    param.push({name: "req", value: "iniciarDiagnostico"});
                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        data: param,
                        success: function() {
                            $('.tourn-btn').trigger('click');
                            window.location.href="par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>&dimid=27";
                        }
                    });
                });

                $(".diagnosticoSair, .close").on("click", function(){
                    window.location.href = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>";
                    return;
                });
            })
        </script>
            <?php
            }
        }?>
    </div>
</div>
