<?php
include_once APPRAIZ . "includes/workflow.php";
if ($_REQUEST['requisicao'] == 'detalhependencia') {
    switch ($_REQUEST['pendencia']) {
        case 'vistoria':
            ob_clean();
            $arrQuestoesPendentes = $mdObra->detalharPendenciaRelatorioVistoria($_GET['obrid']);
            if (is_array($arrQuestoesPendentes)) {
                foreach ($arrQuestoesPendentes as $questao) {
                    echo '<p class="danger"><i class="fa fa-circle"></i> ' . $questao['pertitulo'] . '</p>';
                }
                die;
            }
            echo '<p class="danger"><i class="fa fa-circle"></i> ' . $arrQuestoesPendentes . '</p>';
            die;
            break;
        case 'estudodemanda':
            ob_clean();
            echo '<p class="danger"><i class="fa fa-circle"></i> ' . $mdObra->detalharPendenciaEstudoDemanda($_GET['obrid']).'</p>';
            die;
            break;
        case 'dadosterreno':
            ob_clean();
            $arrDadosTerreno = $mdObra->detalharPendenciaDadosTerreno($_GET['obrid']);
            if (!is_array($arrDadosTerreno)) {
                echo '<p class="danger"><i class="fa fa-circle"></i> ' . $arrDadosTerreno . '</p>';
                die;
            }
            foreach ($arrDadosTerreno as $arrDado) {
                echo '<p class="danger"><i class="fa fa-circle"></i> ' . $arrDado . '</p>';
            }
            die;
            break;
        case 'documentosanexos':
            ob_clean();
            $documentosAnexos = $mdObra->detalharDocumentoAnexo($_GET['obrid']);
            if (is_array($documentosAnexos)) {
                foreach ($documentosAnexos as $documento) {
                    echo '<p class="danger"><i class="fa fa-circle"></i> ' . $documento . '</p>';
                }
                die;
            }
            echo '<p class="danger"><i class="fa fa-circle"></i> ' . $documentosAnexos . '</p>';
            die;
            break;
        case 'fotosterreno':
            ob_clean();
            $pendenciaFotos = $mdObra->detalharPendenciaFotosTerreno($_GET['obrid']);
            echo '<p class="danger"><i class="fa fa-circle"></i> ' . $pendenciaFotos . '</p>';
            die;
            break;
        case 'planilhaorcamentaria':
            ob_clean();
            $pendencia = $mdObra->detalharPendenciaPlanilhaOrcamentaria($_GET['obrid']);
            echo '<p class="danger"><i class="fa fa-circle"></i> ' . $pendencia . '</p>';
            die;
            break;
        case 'cronogramacontrapartida':
            ob_clean();
            $pendencia = $mdObra->detalharPendenciaCronogramaContrapartida($_GET['obrid']);
            echo '<p class="danger"><i class="fa fa-circle"></i> ' . $pendencia . '</p>';
            die;
            break;
        default;
            break;
    }
}
if ($_REQUEST['requisicao'] && $_REQUEST['requisicao'] == 'retornarestadoworkflow') {
    ob_clean();
    if (wf_alterarEstado($_REQUEST['docid'], $_REQUEST['aedid'], '', ['cache_estado_atual'=>0])) {
        echo 1;
    }
    die;
}


$mInp = new Par3_Model_IniciativaPlanejamento();
$arrPendencia = $mdObra->verificarPendencias($_GET['obrid']);
$helperPendencia = new Par3_Helper_PendenciasEntidade();
//$pendenciasImpeditivas = $helperPendencia->controlePendenciasBoxImpeditivas();
//$pendenciasImpeditivas = 0;

$temPendencia = false;
if ($_REQUEST['inuid']) {
    $mdPlanejamento = new Par3_Model_IniciativaPlanejamento($_REQUEST['inpid']);
    $obPendenciaBloqueio = new Par3_Controller_Pendencia();
    if ($mdPlanejamento->iniid) {
        $temPendencia = $obPendenciaBloqueio->verificaPendenciaBloqueio('planejamento', $_REQUEST['inuid'], $inp->iniid, '', false, true, true, true, $_REQUEST['inpid']);
    } else {
        $temPendencia = $obPendenciaBloqueio->verificaPendenciaBloqueio('planejamento', $_REQUEST['inuid'], '', '', false, true, true, true, $_REQUEST['inpid']);
    }
}

/*
 * Verifica se o municipio ou o estado não precisa validar as pendecias para finalizar a iniciativa*/
if ($temPendencia == true) {
    $pendenciasImpeditivas = '';
}

$ocultar = array();
$ocultar['historico'] = true;
$mostrarBarraTramicacao = true;
$pflid = pegaPerfilGeral($_SESSION['usucpf']);
$esd = wf_pegarEstadoAtual($mdObra->docid);
if ($esd['esdid'] != $mdObra::ESDID_EM_CADASTRAMENTO) {
    $dadosEstadoAnterior = $mdObra->pegarDadosEstadoAnterior($mdObra->docid, $esd['esdid']);
}

//verifica se a situação da obra esta aprovada, arquivada, indeferida ou cancela e
//monta uma tabela com os comentarios de transmisão
if ($esd['esdid'] == $mdObra::ESDID_OBRA_APROVADA    ||
   $esd['esdid'] == $mdObra::ESDID_OBRA_ARQUIVADA   ||
   $esd['esdid'] == $mdObra::ESDID_OBRA_INDEFERIDA  ||
   $esd['esdid'] == $mdObra::ESDID_OBRA_CANCELADA) {
    $comentariosSituacaoFinal = $mdObra->pegarDadosComentarioSituacaoFinal($_GET['obrid']);
}

?>


<style>
    .icon-flipped {
        transform: scaleX(-1);
        -moz-transform: scaleX(-1);
        -webkit-transform: scaleX(-1);
        -ms-transform: scaleX(-1);
    }
</style>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Enviar para Análise</h3>
                <h3 class="center"><?= (empty($_GET['obrid']) ? '' : "Pré-Obra ID: {$_GET['obrid']}") ?></h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="col-lg-10">
                <?php if (!empty($pendenciasImpeditivas)) : ?>
                    <div class="widget red-bg p-lg text-center">
                        <div class="row">
                            <div class="col-md-1">
                                <i class="fa fa-warning fa-4x"></i>
                            </div>
                            <div class="col-md-11 text-left">
                                <h3>Pendências do PAR </h3>
                                <h4>Para finalizar Obra, será necessário resolver as Pendências do PAR.</h4>
                                <a class="btn btn-info pull-right" id="visualizarPendencias" role="button">Visualizar Pendências do PAR </a>
                            </div>
                        </div>
                    </div>
                <?php endif;?>
                <?php if (!empty($arrPendencia)) : ?>
                    <h3 class="left">Pendências</h3>
                    <table class="table table-bordered">
                        <thead>
                        <tr>
                            <th class="col-lg-1">Item</th>
                            <th class="col-lg-10">Descrição</th>
                            <th class="col-lg-1">Ajuda</th>
                        </tr>
                        </thead>
                        <tbody>
                        <?php $item = 1;
                        foreach ($arrPendencia as $pendencia) : ?>
                            <tr>
                                <td><b><?php echo $item++ ?></b></td>
                                <td><b><?php echo $pendencia['descricao']; ?></b></td>
                                <td class="center">
                                    <a title="Pendência <?php echo $pendencia['descricao']; ?>" data-toggle="modal"
                                       data-target="#myModal" id="<?php echo $pendencia['label'] ?>"
                                       class="detalhependencia">
                                        <i class="fa fa-book" aria-hidden="true"></i>
                                    </a>
                                </td>
                            </tr>
                        <?php endforeach; ?>
                        </tbody>
                    </table>
                <?php else : ?>
                    <div class="alert alert-success" style="margin-top: 60px;">
                        <h4>Obras</h4>
                        <strong><i class="fa fa-check"></i></strong> Sem Pendências.
                    </div>
                <?php endif; ?>
                
                <?php if (!empty($comentariosSituacaoFinal)) : ?>
                    <div class="col-lg-12">
                    <hr>
                        <?= $comentariosSituacaoFinal->render(); ?>
                    </div>
                <?php endif; ?>
                    
            </div>
            <?php
            $perfis = [Par3_Model_UsuarioResponsabilidade::PREFEITO ,Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL];
            $perfisSuperUsuario = [
                    Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO ,
                    Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,
                    Par3_Model_UsuarioResponsabilidade::ANALISTA_TECNICO_CGEST,
                    Par3_Model_UsuarioResponsabilidade::COORDENADOR_TECNICO_CGEST,
                ];
            ?>

            <?php
            $arrEsd = [
                $mdObra::ESDID_PAR3_AGUARDANDO_ANALISE_FNDE,
                $mdObra::ESDID_EM_ANALISE,
                $mdObra::ESDID_VALIDACAO_DILIGENCIA,
                $mdObra::ESDID_AGUARDANDO_ANALISE_FNDE_RETORNO_DILIGENCIA
            ];

            if (empty($arrPendencia) && empty($pendenciasImpeditivas) && in_array($esd['esdid'], $arrEsd) && array_intersect($pflid, $perfis) && $mostrarBarraTramicacao != false) : ?>
                <div class="col-lg-2">
                    <div class="panel panel-default">
                        <div class="panel-heading">Etapa</div>
                        <div class="panel-body">
                            <dl>
                                <dd>Estado atual</dd>
                                <dt>Aguardando Análise</dt>
                            </dl>
                            <hr style="margin-top:5px">
                            <span class="badge" style="white-space: normal !important; width: 100%">Nenhuma ação disponível para o documento.</span>
                        </div>
                    </div>
                </div>
                <?php
                $mostrarBarraTramicacao = false;
            endif;

            /* Foi pedido para que Em Diligência, se possa enviar para Em Análise/Retorno de Diligência sem validação de pendências, mas exigindo comentário da tramitação */
            if ($mostrarBarraTramicacao) :
                if ((empty($arrPendencia) && empty($pendenciasImpeditivas))
                    || (array_intersect($pflid, $perfisSuperUsuario))
                    || (in_array($mdObra::ESDID_EM_DILIGENCIA, $esd) || in_array($mdObra::ESDID_EM_DILIGENCIA_DE_REFORMULACAO, $esd))) : ?>
                    <div class="col-lg-2">
                        <?php echo wf_desenhaBarraNavegacaoBootstrap($mdObra->docid, array('obrid' => $_GET['obrid']), $ocultar, 'Etapa'); ?>
                    </div>
                <?php endif;
            endif;
            
            if ($esd['esdid'] != $mdObra::ESDID_EM_CADASTRAMENTO &&
                (in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $pflid)
                    || in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $pflid))) {
                ?>
            <div class="col-lg-2 right">
                <button type="button" id="tramitarSituacao"
                        class="btn btn-success">
                    <i class="glyphicon glyphicon-share-alt icon-flipped"></i>  Voltar Situação
                </button>
            </div>
            <?php } ?>
        </div>
    </div>
</div>
<div>
    <!-- Modal -->
    <div id="modalpendencia" class="modal fade" role="dialog">
        <div class="modal-dialog">
            <!-- Modal content-->
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal">&times;</button>
                    <h4 class="modal-title">Pendência</h4>
                </div>
                <div class="modal-body">
                    <p>Some text in the modal.</p>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                </div>
            </div>

        </div>
    </div>
    <script>
        $('.detalhependencia').click(function () {
            console.log($(this).attr('id'));
            var title = $(this).attr('title');
            $('.modal-title').html(title);
            $('#modalpendencia').find('.modal-body').load(window.location.href + '&requisicao=detalhependencia&pendencia=' + $(this).attr('id'));
            $('#modalpendencia').modal('show');
        });


        /*
         Retornar para estado anterior (tramitação reversa), de acordo com o histórico da obra
         */
        $('#tramitarSituacao').click(function(){

            var estado = '<?= $dadosEstadoAnterior['esddscdestino']; ?>';

            if(estado == ''){

                swal({
                    title: "Ação não existente",
                    text: "Não foi encontrada ação de retorno para este estado!",
                    type: "error"
                });
                return false;

            }else {

                var msg = "Tem certeza que deseja retornar o estado da obra para <b>" + estado + "</b>?";
                swal({
                        title: "Confirmar Tramitação",
                        text: msg,
                        type: "warning",
                        html: true,
                        showCancelButton: true,
                        dangerMode: true,
                        confirmButtonText: "Sim, retonar!",
                        cancelButtonText: "Não, cancelar!"
                    },
                    function (isConfirm) {
                        if (isConfirm) {
                            var dados = 'requisicao=retornarestadoworkflow&aedid=' + <?= $dadosEstadoAnterior['aedid'] ?> +"&docid=" + <?= $mdObra->docid ?>;
                            $.ajax({
                                url: window.location.href,
                                type: 'POST',
                                data: dados,
                                success: function (resultado) {
                                    console.log(resultado);
                                    if (resultado == 1) {
                                        var type = 'success',
                                            msg = "Estado alterado com sucesso!";
                                    } else {
                                        var type = 'error',
                                            msg = "Não foi possível retornar o estado da obra.";
                                    }
                                    swal({
                                        title: 'Tramitação',
                                        text: msg,
                                        type: type
                                    });
                                    location.reload();
                                }
                            });
                        }
                    }
                );
            }
        });


        $("#visualizarPendencias").click(function () {
            window.open("par3.php?modulo=principal/planoTrabalho/pendencias&acao=A&inuid=<?=$_GET['inuid']?>#");
        });

        function vizualizarIniciativa(id) {
            window.location.href = "par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=<?=$_GET['inuid']?>&aba=cadastro&inpid=" + id;
        }

        $("#vizualizarIniciativasII").click(function () {
            window.open("par3.php?modulo=principal/planoTrabalho/planejamento&acao=A&inuid=<?=$_GET['inuid']?>&aba=consulta");
        });


    </script>
