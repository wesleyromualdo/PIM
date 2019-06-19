<?php
$perfis = pegaPerfilGeral();
$perfis = is_array($perfis) ? $perfis : array();
?>
<div class="ibox float-e-margins animated modal"  id="modalAtendimento" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 1200px; height: auto;">
        <div class="modal-content animated pulse">
        	<div class="ibox-title esconde " tipo="integrantes">
                <h2 class="text-center">Central de Orientações - PAR</h2>
            </div>
            <div class="ibox-content">
                <div class="row">
                    <div class="col-lg-6 col-md-6">
                        <div class="box-pendencia widget style1 yellow-bg" style="background-color: #3f51b5">
                            <div class="row vertical-align">
                                <div class="col-xs-2 col-md-2">
                                    <i class="fa fa-desktop fa-5x"></i>
                                </div>
                                <div class="col-xs-8 col-md-8 text-center .tourn-btn" style="padding-top: 17px;">
                                    <span style="font-size: 20px;">Visita Guiada</span>
                                </div>
                                <div class="col-xs-2 col-md-2" style="padding-top: 10px;">
                                    <h4><i class="fa fa-mouse-pointer fa-3x"></i></h4>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="col-lg-6 col-md-6">
                        <div class="box-pendencia widget style1 yellow-bg manual_sistema" style="background-color: #ed5565">
                            <div class="row vertical-align">
                                <div class="col-xs-2 col-md-2">
                                    <i class="fa fa-file-text-o fa-5x"></i>
                                </div>
                                <div class="col-xs-8 col-md-8 text-center" style="padding-top: 17px;">
                                    <span style="font-size: 20px;">Manual do Sistema</span>
                                </div>
                                <div class="col-xs-2 col-md-2" style="padding-top: 10px;">
                                    <h4><i class="fa fa-download fa-3x"></i></h4>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="ibox-content ">
                <div class=" widget  yellow-bg text-left" style="background-color: <?php echo $arAuxHelperPendencia['widget']; ?>;">
                    <div>
                        <i class="fa fa-phone fa-2x"></i>
                        <span class="font-bold no-margins" style="font-size: 16pt;">Contato com a Equipe Técnica do PAR</span>
                    </div>
                    <div class="row" style="margin-top: 20px;">
                        <div class="col-md-12">
                            <a style="float: left" href="par3.php?modulo=principal/chamado/formulario&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>&urlorigem=<?php echo base64_encode($_SERVER['REQUEST_URI']); ?>" class="btn btn-primary bt-chamado">Tire suas dúvidas</a>
                            <p style="float: left; padding: 7px 0 0 10px;">Entre em contato com a Equipe Técnica do MEC ou FNDE</p>
                            <div style="clear: both;"></div>
                            <br/>
                            <a style="float: left" href="par3.php?modulo=principal/chamado/index&acao=A" class="btn btn-danger bt-chamado">Dúvidas anteriores</a>
                            <p style="float: left; padding: 7px 0 0 10px;">Veja todas as suas dúvidas já encaminhadas</p>
                            <div style="clear: both;"></div>
                            <?php if(in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
                                     in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
                                     in_array(Par3_Model_UsuarioResponsabilidade::PREFEITO, $perfis) ||
                                     in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
                                     in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)){  ?>
                            <br/>
                            <a style="float: left" href="par3.php?modulo=principal/chamado/solicitartreinamento&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>" class="btn btn-success bt-solicitar-treinamento" id="bt-solicitar-treinamento">Informar necessidade de apoio técnico</a>
                            <?php } ?>
                        </div>
                    </div>
                </div>
            </div>
            <div class="ibox-footer">
                <button type="button" class="btn btn-danger" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>
<script>
$(function () {
    $('#bt-solicitar-treinamento').click(function(){

        var permissao = false;
        $.ajax({
            type: "POST",
            url: 'par3.php?modulo=principal/chamado/solicitartreinamento&acao=A&action=verificar-chamados-inuid&inuid=<?php echo $_REQUEST['inuid']; ?>',
            async: false,
            success: function(retorno){
                if(retorno == '0'){
                    swal('Atenção', 'Antes de solicitar treinamento, favor entrar em contato com a nossa equipe técnica, no botão acima, pois é possível que a sua dúvida possa ser resolvida por este canal.', 'warning');
                } else {
                    permissao = true;
                }
            }
        });

        return permissao;
    });

    $('.manual_sistema').click(function(){
        window.open('/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882874');
    });
})
</script>