<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 29/03/2016
 * Time: 15:51
 */
global $db;
$fmtid                = $_GET['fmtid'];
$fichamonitoramento   = new Fichamonitoramento();
$municipio            = new Territorios_Model_Municipio();

if(!empty($fmtid)) {
    $fichamonitoramento->carregarPorId($fmtid);
    $municipio->carregarPorId($fichamonitoramento->muncod);

    if($fichamonitoramento->fmtperavalanual == 't'){
        $checkedanual = 'checked="checked"';
    }
    if($fichamonitoramento->fmtperavalbianual == 't'){
        $checkedbianual  = 'checked="checked"';
    }
    if($fichamonitoramento->fmtperavaltrianual == 't'){
        $checkedtrianual = 'checked="checked"';
    }
    if($fichamonitoramento->fmtperavalquadrienal == 't'){
        $checkedquadrienal = 'checked="checked"';
    }
    if($fichamonitoramento->fmtperavalquinquenal == 't'){
        $checkedquinquenal = 'checked="checked"';
    }
    if($fichamonitoramento->fmtperavalnaoprevisto == 't'){
        $checkednaoprevisto = 'checked="checked"';
    }
}else{
    echo "<script> alert('Selecione um município na aba Lista.');window.location.href='sase.php?modulo=principal/monitoramento&acao=A';</script>";
}

if($_POST['acao']) {
    switch ($_POST['acao']) {
        case 'salvar':
            ob_clean();
            $muncod               = $_POST['muncod'];
            $fmtpme               = $_POST['fmtpme'];
            $fmtperavalanual      = $_POST['fmtperavalanual'];
            $fmtperavalbianual    = $_POST['fmtperavalbianual'];
            $fmtperavaltrianual   = $_POST['fmtperavaltrianual'];
            $fmtperavalquadrienal = $_POST['fmtperavalquadrienal'];
            $fmtperavalquinquenal = $_POST['fmtperavalquinquenal'];
            $fmtperavalnaoprevisto = $_POST['fmtperavalnaoprevisto'];

            $fmtperavalano1       = !empty($_POST['fmtperavalano1']) ? $_POST['fmtperavalano1'] : NULL;

            $fmtcomcoordenadora   = $_POST['fmtcomcoordenadora'];
            $fmtcomnumanoatolegal = $_POST['fmtcomnumanoatolegal'];
            $fmtequipecontato     = $_POST['fmtequipecontato'];
            $fmtequipetelefone    = $_POST['fmtequipetelefone'];
            $fmtequipeemail       = $_POST['fmtequipeemail'];
            $fmtequipetecnica     = $_POST['fmtequipetecnica'];
            $fmtequipeatolegal    = $_POST['fmtequipeatolegal'];
            $fmtstatus            = $_POST['fmtstatus'];
            $usucpf               = $_SESSION['usucpf'];
            $data                 = date('Y-m-d H:i');

            $fichamonitoramento->muncod               = $muncod;
            $fichamonitoramento->fmtpme               = ($fmtpme);
            $fichamonitoramento->fmtperavalanual      = $fmtperavalanual;
            $fichamonitoramento->fmtperavalbianual    = $fmtperavalbianual;
            $fichamonitoramento->fmtperavaltrianual   = $fmtperavaltrianual;
            $fichamonitoramento->fmtperavalquadrienal = $fmtperavalquadrienal;
            $fichamonitoramento->fmtperavalquinquenal = $fmtperavalquinquenal;
            $fichamonitoramento->fmtperavalnaoprevisto = $fmtperavalnaoprevisto;
            $fichamonitoramento->fmtperavalano1       = $fmtperavalano1;
//            $fichamonitoramento->fmtperavalano2       = $fmtperavalano2;//retirar os campos ano2, 3 e 4 - SS3898
//            $fichamonitoramento->fmtperavalano3       = $fmtperavalano3;
//            $fichamonitoramento->fmtperavalano4       = $fmtperavalano4;
            $fichamonitoramento->fmtcomcoordenadora   = ($fmtcomcoordenadora);
            $fichamonitoramento->fmtcomnumanoatolegal = ($fmtcomnumanoatolegal);
            $fichamonitoramento->fmtequipecontato     = ($fmtequipecontato);
            $fichamonitoramento->fmtequipetelefone    = ($fmtequipetelefone);
            $fichamonitoramento->fmtequipeemail       = ($fmtequipeemail);
            $fichamonitoramento->fmtequipetecnica     = ($fmtequipetecnica);
            $fichamonitoramento->fmtequipeatolegal    = ($fmtequipeatolegal);
//            $fichamonitoramento->fmtstatus            = $fmtstatus;
            $fichamonitoramento->usucpf               = $usucpf;
            $r = $fichamonitoramento->salvarHistorico($fichamonitoramento);
            if($r){
                $fichamonitoramento->fmtdataalteracao  = $data;
                $true = $fichamonitoramento->inserirManterAspas(array('fmtperavalano1', 'fmtperavalano2','fmtperavalano3','fmtperavalano4'));
            }else{
                $fichamonitoramento->fmtdatainclusao  = $data;
                $true = $fichamonitoramento->alterar(array(
                    'fmtpme',
                    'fmtperavalanual',
                    'fmtperavalbianual',
                    'fmtperavaltrianual',
                    'fmtperavalquadrienal',
                    'fmtperavalquinquenal',
                    'fmtperavalnaoprevisto',
                    'fmtperavalano1',
//                                                    'fmtperavalano2',//retirar os campos ano2, 3 e 4 - SS3898
//                                                    'fmtperavalano3',
//                                                    'fmtperavalano4',
                    'fmtcomcoordenadora',
                    'fmtcomnumanoatolegal',
                    'fmtequipecontato',
                    'fmtequipetelefone',
                    'fmtequipeemail',
                    'fmtequipetecnica',
                    'fmtequipeatolegal',
                    'usucpf',
                    'fmtdataalteracao'
                ));
            }

            if(!$true){
                $fichamonitoramento->rollback();
                echo "Erro ao salvar a Ficha de Técnica.";
                echo "<script> window.location.href='sase.php?modulo=principal/monitoramento&acao=A';</script>";

            } else {
                $fichamonitoramento->commit();
                echo "Ficha salva com sucesso!";
                echo "<script> window.location.href='sase.php?modulo=principal/monitoramento&acao=A';</script>";
            }
            exit;
        case 'exibir_historico':
            $dadosHistorico = $fichamonitoramento->dadosHistorico($fmtid);
            break;


    }
}

?>
<style>
    .divfmtperavalbianual, .divfmtperavaltrianual, .divfmtperavalquadrienal{
        display: none;
    }

    .divfmtperavalanual{
        display: inherit;
    }

    fieldset.scheduler-border {
        border: 1px #CCC !important;
        padding: 0 1.4em 1.4em 1.4em !important;
        margin: 0 0 1.5em 0 !important;
        -webkit-box-shadow:  0px 0px 0px 0px #000;
        box-shadow:  0px 0px 0px 0px #000;
    }

    legend.scheduler-border {
        font-size: 1em !important;
        font-weight: bold !important;
        text-align:  left !important;

    }
</style>
<?php include APPRAIZ . 'includes/cabecalho.inc'; ?>
<div id="container">
    <ul class="nav nav-tabs">
        <li class="active"><a data-toggle="tab" href="#informacao">Informações Gerais</a></li>
        <li><a data-toggle="tab" href="#municipios" id="meta_estratedi">Metas / Estratégias</a></li>
        <li><a data-toggle="tab" href="#monitoramentos" id="aba_monitoramentos">Monitoramento de Metas</a></li>
    </ul>

    <div class="tab-content">
        <div id="informacao" class="tab-pane fade in active" style="margin-top: 2%">
            <form class="form-horizontal" name="formMapa" id="formMapa" role="form" method="POST">
                <div class="row">
                    <div class="col-lg-8 col-lg-offset-2">
                        <div class="well">
                            <fieldset>
                                <fieldset class="scheduler-border">
                                    <legend class="scheduler-border">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Geral</legend>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Município:</label>
                                        <div class="col-lg-8">
                                            <input type="hidden" id="muncod" name="muncod" value="<?= $municipio->muncod ?>"/>
                                            <input type="text" id="mundescricao" name="mundescricao" class="form-control" disabled value="<?= $municipio->mundescricao ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Estado :</label>
                                        <div class="col-lg-8">
                                            <input type="text" id="estdescricao" name="estdescricao" class="form-control" disabled value="<?= $fichamonitoramento->estuf ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Plano Municipal de Educação:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: Lei nº 458, de 12 de junho de 2015." type="text" id="fmtpme" name="fmtpme" class="form-control" maxlength="50" value="<?= trim($fichamonitoramento->fmtpme) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Períodos de Avaliação previstos:</label>
                                        <div class="col-lg-8">
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmtperavalanual" class="radio-periodo" value="fmtperavalanual" <?= $checkedanual ? $checkedanual : '' ?> >Anual</label>
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmtperavalbianual" class="radio-periodo" value="fmtperavalbianual" <?= $checkedbianual ? $checkedbianual : '' ?> >Bianual</label>
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmtperavaltrianual" class="radio-periodo" value="fmtperavaltrianual" <?= $checkedtrianual ? $checkedtrianual : '' ?> >Trianual</label>
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmtperavalquadrienal" class="radio-periodo" value="fmtperavalquadrienal" <?= $checkedquadrienal ? $checkedquadrienal : '' ?> >Quadrienal</label>
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmtperavalquinquenal" class="radio-periodo" value="fmtperavalquinquenal" <?= $checkedquinquenal ? $checkedquinquenal : '' ?> >Quinquenal</label>
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmtperavalnaoprevisto" class="radio-periodo" value="fmtperavalnaoprevisto" <?= $checkednaoprevisto ? $checkednaoprevisto : '' ?> >Não Previsto</label>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Primeiro Ano da Avaliação:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: 2017" type="text" id="fmtperavalano1"  name="fmtperavalano1" class="form-control" maxlength="4" value="<?= trim($fichamonitoramento->fmtperavalano1) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Comissão Coordenadora:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: Secretaria de Educação, Conselho de Educação, Colegiados Escolares... (descrever todos) E/OU Comissão Coordenadora responsável pelo monitoramento e avaliação do Plano de Educação." type="text" id="fmtcomcoordenadora" name="fmtcomcoordenadora" class="form-control" maxlength="5000" value="<?= trim($fichamonitoramento->fmtcomcoordenadora) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Ato Legal (n° e ano):</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: Lei nº 458, de 12 de junho de 2015 OU Portaria nº XXX de 5 de maio de 2015" type="text" id="fmtcomnumanoatolegal" name="fmtcomnumanoatolegal" class="form-control" maxlength="1000" value="<?= trim($fichamonitoramento->fmtcomnumanoatolegal) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Equipe técnica:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.:  Equipe Técnica da Secretaria de Educação, responsável pelo monitoramento e avaliação do Plano de Educação." type="text" id="fmtequipetecnica" name="fmtequipetecnica" class="form-control" maxlength="5000" value="<?= trim($fichamonitoramento->fmtequipetecnica) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Ato Legal (n° e ano):</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: Portaria nº XXX de 5 de maio de 2015"type="text" id="fmtequipeatolegal" name="fmtequipeatolegal" class="form-control" maxlength="1000" value="<?= trim($fichamonitoramento->fmtequipeatolegal) ?>"/>
                                        </div>
                                    </div>
                                </fieldset>
                                <fieldset class="scheduler-border">
                                    <legend class="scheduler-border">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Contatos</legend>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Telefone:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: (XX) XXXX-XXXX" type="text" id="fmtequipetelefone" name="fmtequipetelefone" class="form-control" maxlength="50" value="<?= trim($fichamonitoramento->fmtequipetelefone) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">E-mail:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: monitora@assisbrasil.ac.gov.br" type="text" id="fmtequipeemail" name="fmtequipeemail" class="form-control" maxlength="50" value="<?= trim($fichamonitoramento->fmtequipeemail) ?>"/>
                                        </div>
                                    </div>
                                </fieldset>
                                <div class="form-group">
                                    <div class="col-lg-12" align="center">
                                        <input type="button" value="Salvar" id="btn-salvar" class="btn btn-success"/>
                                        <input type="button" value="Histórico" id="btn-historico" class="btn btn-primary"/>
                                    </div>
                                </div>
                            </fieldset>
                        </div>
                    </div>
                </div>

            </form>
            <div class="modal fade modalAditivo" id="modalAditivo" tabindex="-1" role="dialog" aria-labelledby="modalAditivo" aria-hidden="true">
                <div class="modal-dialog" style="width:80%;">
                    <div class="modal-content">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal">
                                <span aria-hidden="true">&times;</span>
                                <span class="sr-only">Fechar</span>
                            </button>
                            <h4 class="modal-title">Histórico Ficha Técnica do Plano de Educação Município</h4>
                        </div>
                        <div class="modal-body" id="modal-body">
                            <?php
                            echo $fichamonitoramento->dadosHistorico($fichamonitoramento->muncod,$fichamonitoramento->estuf);
                            ?>
                        </div>
                        <div class="modal-footer">
                            <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <div id="municipios" class="tab-pane fade">
            <?php include_once 'metaEstrategia.inc' ?>
        </div>
        <div id="monitoramentos" class="tab-pane fade">
            <?php include_once 'monitoramentos.inc' ?>
        </div>
    </div>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript">

        function validaFormSavlar(){
            if(jQuery('#fmtpme').val() === ''){
                jQuery('#fmtpme').focus();
                return 'Informe o campo "Plano Municipal de Educação."';
            }
            if($('input[name=periodo]:checked').length <= 0){
                return 'Informe o campo "Períodos de Avaliação previstos".';
            }
            if($('#fmtperavalano1').val() === ''){
                $('#fmtperavalano1').focus();
                return 'Informe o campo "Primeiro Ano da Avaliação".';
            }
            if(jQuery('#fmtcomcoordenadora').val() === ''){
                jQuery('#fmtcomcoordenadora').focus();
                return 'Informe o campo "Comissão Coordenadora."';
            }
            if(jQuery('#fmtcomnumanoatolegal').val() === ''){
                jQuery('#fmtcomnumanoatolegal').focus();
                return 'Informe o campo "Ato Legal (n° e ano)."';
            }
            if(jQuery('#fmtequipetecnica').val() === ''){
                jQuery('#fmtequipetecnica').focus();
                return 'Informe o campo "Equipe técnica".';
            }
            if(jQuery('#fmtequipeatolegal').val() === ''){
                jQuery('#fmtequipeatolegal').focus();
                return 'Informe o campo "Ato Legal(n° e ano)".';
            }
            if(jQuery('#fmtequipetelefone').val() === '' &&  jQuery('#fmtequipeemail').val() === ''){
                return 'Informe "E-mail" ou "Telefone".';
            }
            if( !validaEmail(jQuery('#fmtequipeemail').val()) && jQuery('#fmtequipeemail').val() != '' ){
                jQuery('#fmtequipeemail').focus();
                return 'E-mail inválido!';
            }

            return true;
        }

        $('documento').ready(function(){

            $("#fmtperavalano1").mask("9999");
            $("#fmtequipetelefone").mask('(00) 0000-0000#');

            //informativo sobre os campos
            $('[data-toggle="tooltip"]').tooltip();
            /**
             * Evento dos radions dos períodos de avaliação previstos
             */
            $('.radio-periodo').click(function(){
                $('.divfmtperavalanual').hide();
                $('.divfmtperavalbianual').hide();
                $('.divfmtperavaltrianual').hide();
                $('.divfmtperavalquadrienal').hide();
                $('.divfmtperavalquinquenal').hide();
                $('.divfmtperavalnaoprevisto').hide();
                $('.div'+$(this).val()).show();
            });

            jQuery("#btn-historico").on("click", function() {
                jQuery('#modalAditivo').modal('show');
            });


            jQuery('#btn-salvar').click(function(){
                var muncod               = jQuery('#muncod').val();
                var fmtpme               = jQuery('#fmtpme').val();
                var fmtperavalanual      = jQuery('#fmtperavalanual:checked').length === 1 ? true : false;
                var fmtperavalbianual    = jQuery('#fmtperavalbianual:checked').length === 1 ? true : false;
                var fmtperavaltrianual   = jQuery('#fmtperavaltrianual:checked').length === 1 ? true : false;
                var fmtperavalquadrienal = jQuery('#fmtperavalquadrienal:checked').length === 1 ? true : false;
                var fmtperavalquinquenal = jQuery('#fmtperavalquinquenal:checked').length === 1 ? true : false;
                var fmtperavalnaoprevisto = jQuery('#fmtperavalnaoprevisto:checked').length === 1 ? true : false;
                var fmtperavalano1       = jQuery('#fmtperavalano1').val();
                var fmtcomcoordenadora   = jQuery('#fmtcomcoordenadora').val();
                var fmtcomnumanoatolegal = jQuery('#fmtcomnumanoatolegal').val();
                var fmtequipecontato     = jQuery('#fmtequipecontato').val();
                var fmtequipetelefone    = jQuery('#fmtequipetelefone').val();
                var fmtequipeemail       = jQuery('#fmtequipeemail').val();
                var fmtequipetecnica     = jQuery('#fmtequipetecnica').val();
                var fmtequipeatolegal    = jQuery('#fmtequipeatolegal').val();
                var fmtstatus            = jQuery('input[name=fmtstatus]:checked').val();
                var validacao            = validaFormSavlar();

                if(validacao === true) {
                    jQuery.ajax({
                        url: '',
                        type: 'POST',
                        data: {
                            acao: 'salvar',
                            muncod: muncod,
                            fmtpme: fmtpme,
                            fmtperavalanual: fmtperavalanual,
                            fmtperavalbianual: fmtperavalbianual,
                            fmtperavaltrianual: fmtperavaltrianual,
                            fmtperavalquadrienal: fmtperavalquadrienal,
                            fmtperavalquinquenal: fmtperavalquinquenal,
                            fmtperavalnaoprevisto: fmtperavalnaoprevisto,
                            fmtperavalano1: fmtperavalano1,
                            fmtcomcoordenadora: fmtcomcoordenadora,
                            fmtcomnumanoatolegal: fmtcomnumanoatolegal,
                            fmtequipecontato: fmtequipecontato,
                            fmtequipetelefone:fmtequipetelefone,
                            fmtequipeemail: fmtequipeemail,
                            fmtequipetecnica: fmtequipetecnica,
                            fmtequipeatolegal: fmtequipeatolegal,
                            fmtstatus: fmtstatus
                        },
                        success: function (retorno) {
                            var data = retorno;
                            alert(data);
                        }
                    });
                } else {
                    alert(validacao);
                }
            });
        });
    </script>
