<?php
$sisid = $_SESSION['sisid'];
$inuid = $_REQUEST['inuid'];

$controllerChamado = new Sac_Controller_Chamado();

switch ($_REQUEST['action']) {
    case 'salvar':
        $controllerChamado->salvar($_REQUEST);
        exit();
    case 'salvar-historico':

        if(isset($_REQUEST['finalizar'])){
            $controllerChamado->finalizar($_REQUEST);
            exit();
        }

        $controllerChamado->salvarHistorico($_REQUEST);
        exit();
    case 'verificar-chamados-inuid':
        $filtros = array(
            'inuid' => $_REQUEST['inuid'],
            'chastatus' => array(Sac_Model_Chamado::K_STATUS_FINALIZADO_POSITIVO, Sac_Model_Chamado::K_STATUS_FINALIZADO_NEGATIVO, Sac_Model_Chamado::K_STATUS_FINALIZADO_AUTOMATICO),
        );
        $chamados = $controllerChamado->recuperarChamados(null, $filtros);
        echo count($chamados);
        exit();
    default:
        $modelChamado = new Sac_Model_Chamado($_REQUEST['chaid']);
        break;
}

$tipoPerfil = $controllerChamado->recuperarTipoPerfil();

// O perfil de suporte não pode acessar essa tela quando não existir o chamado
if(!$_REQUEST['chaid'] && $tipoPerfil == Sac_Model_PerfilTipo::K_TIPO_SUPORTE){
    simec_redirecionar('par3.php?modulo=principal/chamado/suporte&acao=A', 'error', 'Você não possui permissão de acesso a essa página');
}

require APPRAIZ . 'includes/cabecalho.inc';
?>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3><?php echo $modelChamado->chaid ? 'Chamado <span style="color: red;">' . $modelChamado->chaid  . '</span>' : 'Novo Chamado'; ?></h3>
            </div>
        </div>
    </div>
    <form method="post" name="formulario-chamado" id="formulario-chamado" class="form form-horizontal">
        <div class="ibox-content">
            <input type="hidden" name="action" id="action" value="<?php echo !$modelChamado->chaid ? 'salvar' : 'salvar-historico'; ?>" />
            <input type="hidden" name="sisid" id="sisid" value="<?php echo $sisid; ?>" />
            <input type="hidden" name="chaid" id="chaid" value="<?php echo $modelChamado->chaid; ?>" />

            <div class="row">
                <?php
                if(!$modelChamado->chaid){
                    echo '<input type="hidden" name="chaurlorigem" id="chaurlorigem" value="' . base64_decode($_REQUEST['urlorigem']) . '" />';
                    echo '<input type="hidden" name="inuid" id="inuid" value="' . $_REQUEST['inuid'] . '" />';

                    $sql = "select catdsc from sac.categoria where catid = " . Sac_Model_Categoria::K_PAR3_APOIO_TECNICO;
                    $categoria = $modelChamado->pegaUm($sql);
                    
                    ?>
                    <div class="form-group ">
                        <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Tipo de Ocorrência: </label>
                        <div class="col-sm-9 col-md-9 col-lg-9 ">
                            <p class="form-control-static"><?php echo $categoria; ?></p>
                            <input type="hidden" name="catid" id="catid" value="<?php echo Sac_Model_Categoria::K_PAR3_APOIO_TECNICO; ?>" />
                        </div>
                    </div>

                    <?php
                    $sql = "select areid as codigo, aredsc as descricao from sac.area order by areordem";
                    echo $simec->select('areid', 'Onde ocorreu', $modelChamado->areid, $sql, array('required'));

                    echo $simec->textarea('chadsc', 'Descrição da Ocorrência', $modelChamado->chadsc, array('required', 'style'=>'height: 150px;'));
                    ?>

                    <div class="text-center">
                        <button type="submit" class="btn btn-primary" >Salvar e Enviar</button>
                        <?php if($_REQUEST['urlorigem']){ ?>
                            <a href="<?php echo base64_decode($_REQUEST['urlorigem']); ?>" class="btn btn-danger" >Voltar para tela de origem</a>
                        <?php } ?>
                        <a href="par3.php?modulo=principal/chamado/index&acao=A&inuid=<?php echo $_REQUEST['inuid'] ?>" class="btn btn-warning bt-chamado">Meus Chamados</a>
                    </div>

                <?php } else {
                    $chamado = $modelChamado->recuperarChamado();
                    ?>

                    <div class="col-md-6 col-md-6 col-lg-6">

                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Situação: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <p class="form-control-static" style="color: red;"><?php echo $chamado['status']; ?></p>
                            </div>
                        </div>

                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Tipo de Ocorrência: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <p class="form-control-static"><?php echo $chamado['catdsc']; ?></p>
                            </div>
                        </div>

                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Onde ocorreu: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <p class="form-control-static"><?php echo $chamado['aredsc']; ?></p>
                            </div>
                        </div>

                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Data de Abertura: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <p class="form-control-static"><?php echo $chamado['chadataabertura']; ?></p>
                            </div>
                        </div>
                    </div>

                    <div class="col-md-6 col-lg-6">

                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Usuário: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <p class="form-control-static"><?php echo $chamado['usunome']; ?></p>
                            </div>
                        </div>

                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Contatos: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <p class="form-control-static"><?php echo $chamado['usuemail'] . ' - ' . $chamado['usufoneddd'] . $chamado['usufonenum']; ?></p>
                            </div>
                        </div>

                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Cargo: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <p class="form-control-static"><?php echo $chamado['cardsc']; ?></p>
                            </div>
                        </div>

                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Localização: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <p class="form-control-static"><?php echo $chamado['estuf'] . ' - ' . $chamado['inudescricao']; ?></p>
                            </div>
                        </div>
                    </div>

                    <div class="clearfix"></div>

                    <div class="col-md-12 col-lg-12">
                        <div class="form-group ">
                            <label for="" class="col-sm-3 col-md-3 col-lg-3 control-label">Descrição da Ocorrência: </label>
                            <div class="col-sm-9 col-md-9 col-lg-9 ">
                                <p class="form-control-static"><?php echo nl2br($chamado['chadsc']); ?></p>
                            </div>
                        </div>
                    </div>

                    <div class="ibox-footer">
                        <div class="row">
                            <div class="text-center">

                                <?php if($modelChamado->usucpfabertura == $_SESSION['usucpforigem'] && $modelChamado->chastatus == Sac_Model_Chamado::K_STATUS_AGUARDANDO_VALIDACAO){ ?>
                                    <div style="margin: 20px 0;">
                                        <h4>Seu problema foi solucionado? </h4>
                                        <button type="submit" name="finalizar" value="FP" class="btn btn-primary" ><span class="fa fa-thumbs-o-up"></span></button>
                                        <button type="submit" name="finalizar" value="FN" class="btn btn-danger" ><span class="fa fa-thumbs-o-down"></span></button>
                                    </div>
                                <?php } ?>

                                <?php if($db->testa_superuser() || $tipoPerfil == Sac_Model_PerfilTipo::K_TIPO_SUPORTE){ ?>
                                    <a href="par3.php?modulo=principal/chamado/suporte&acao=A" class="btn btn-warning bt-chamado">Todos os chamados</a>
                                    <?php if($modelChamado->chaurlorigem){ ?>
                                        <a href="<?php echo $modelChamado->chaurlorigem; ?>" class="btn btn-danger" >Ir para tela de origem</a>
                                    <?php } ?>
                                    <?php if($modelChamado->chastatus == Sac_Model_Chamado::K_STATUS_ABERTO){ ?>
                                        <button type="button" name="enviar-validacao" id="questionar-usuario" title="Adicionar Anotação/Questionamento ao Usuário" class="btn btn-success btn-acoes-chamado" >Questionar usuário</button>
                                        <button type="button" name="enviar-validacao" id="enviar-validacao" title="Responder Questionamento/Anotação" class="btn btn-primary btn-acoes-chamado" >Responder Usuário e Finalizar Chamado</button>
                                    <?php } ?>
                                <?php } else { ?>

                                    <a href="par3.php?modulo=principal/chamado/index&acao=A&inuid=<?php echo $_REQUEST['inuid'] ?>" class="btn btn-warning bt-chamado">Meus chamados</a>

                                    <?php if($modelChamado->chaurlorigem){ ?>
                                        <a href="<?php echo $modelChamado->chaurlorigem; ?>" class="btn btn-danger" >Voltar para tela de origem</a>
                                    <?php } ?>

                                    <?php if($modelChamado->chastatus == Sac_Model_Chamado::K_STATUS_AGUARDANDO_USUARIO){ ?>
                                        <button type="button" name="enviar-validacao" id="responder-questionamento" title="Responder Questionamento/Anotação" class="btn btn-success btn-acoes-chamado" >Responder</button>
                                    <?php } ?>

                                <?php } ?>

                                <div style="text-align: center; display: none;" id="div-anotacao">
                                    <br />
                                    <h4 id="texto-anotacao">Responder Questionamento/Anotação</h4>
                                    <textarea name="hicdsc" id="hicdsc" class="form-control" cols="30" rows="10"></textarea>
                                    <br />
                                    <button type="submit" name="acao-historico" class="btn btn-primary" id="btn-enviar-historico" >Enviar</button>
                                </div>
                            </div>
                        </div>
                    </div>


                <?php } // else ?>
            </div>
        </div>
    </form>
</div>

<?php if($modelChamado->chaid){ ?>
    <div class="ibox">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-6">
                    <h3>Histórico do Chamado</h3>
                </div>
            </div>
        </div>
        <form method="post" name="formulario-chamado" id="formulario-chamado" class="form form-horizontal">
            <div class="ibox-content">
                <?php echo $controllerChamado->listarHistorico($modelChamado->chaid); ?>
            </div>
            <div class="ibox-footer">
                <div class="row">
                    <div class="text-center">
                    </div>
                </div>
            </div>
        </form>
    </div>
<?php } ?>

<div style="clear: both"></div>

<script type="text/javascript">
    $(function(){
        $('#questionar-usuario').click(function(){
            $('#btn-enviar-historico').val('questionar');
            $('#div-anotacao').show();
        });
        $('.btn-acoes-chamado').click(function(){
            $('#texto-anotacao').html($(this).attr('title'));
            $('#btn-enviar-historico').val($(this).attr('id'));
            $('#div-anotacao').show();
        });
    });
</script>