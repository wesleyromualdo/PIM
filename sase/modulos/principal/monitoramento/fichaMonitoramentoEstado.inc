<?php
global $db;
$fmeid                      = $_GET['fmeid'];
$fichamonitoramentoEstado   = new Fichamonitoramentoestado();

if(!empty($fmeid)) {

    $fichamonitoramentoEstado->carregarPorId($fmeid);

    if($fichamonitoramentoEstado->fmeperavalanual == 't'){
        $checkedanual = 'checked="checked"';
    }
    if($fichamonitoramentoEstado->fmeperavalbianual == 't'){
        $checkedbianual = 'checked="checked"';

    }
    if($fichamonitoramentoEstado->fmeperavaltrianual == 't'){
        $checkedtrianual = 'checked="checked"';
    }
    if($fichamonitoramentoEstado->fmeperavalquadrienal == 't'){
        $checkedquadrienal = 'checked="checked"';
    }
    if($fichamonitoramentoEstado->fmeperavalquinquenal == 't'){
        $checkedquinquenal = 'checked="checked"';
    }
    if($fichamonitoramentoEstado->fmeperavalnaoprevisto == 't'){
        $checkednaoprevisto = 'checked="checked"';
    }
}else{
    echo "<script> alert('Selecione um estado na aba Lista.');window.location.href='sase.php?modulo=principal/monitoramentoEstado&acao=A';</script>";
}

if($_POST['acao_estado']) {
    switch ($_POST['acao_estado']) {
        case 'salvar':
            ob_clean();
            $fmepme                = $_POST['fmepme'];
            $fmeperavalanual       = $_POST['fmeperavalanual'];
            $fmeperavalbianual     = $_POST['fmeperavalbianual'];
            $fmeperavaltrianual    = $_POST['fmeperavaltrianual'];
            $fmeperavalquadrienal  = $_POST['fmeperavalquadrienal'];
            $fmeperavalquinquenal  = $_POST['fmeperavalquinquenal'];
            $fmeperavalnaoprevisto = $_POST['fmeperavalnaoprevisto'];

            $fmeperavalano1        = !empty($_POST['fmeperavalano1']) ? $_POST['fmeperavalano1'] : NULL;

            $fmecomcoordenadora    = $_POST['fmecomcoordenadora'];
            $fmecomnumanoatolegal  = $_POST['fmecomnumanoatolegal'];
            $fmeequipecontato      = $_POST['fmeequipecontato'];
            $fmeequipetelefone     = $_POST['fmeequipetelefone'];
            $fmeequipeemail        = $_POST['fmeequipeemail'];
            $fmeequipetecnica      = $_POST['fmeequipetecnica'];
            $fmeequipeatolegal     = $_POST['fmeequipeatolegal'];
            $fmestatus             = $_POST['fmestatus'];
            $usucpf                = $_SESSION['usucpf'];
            $data                  = date('Y-m-d H:i');

            $fichamonitoramentoEstado->fmepme               = ($fmepme);
            $fichamonitoramentoEstado->fmeperavalanual      = $fmeperavalanual;
            $fichamonitoramentoEstado->fmeperavalbianual    = $fmeperavalbianual;
            $fichamonitoramentoEstado->fmeperavaltrianual   = $fmeperavaltrianual;
            $fichamonitoramentoEstado->fmeperavalquadrienal = $fmeperavalquadrienal;
            $fichamonitoramentoEstado->fmeperavalquinquenal = $fmeperavalquinquenal;
            $fichamonitoramentoEstado->fmeperavalnaoprevisto = $fmeperavalnaoprevisto;
            $fichamonitoramentoEstado->fmeperavalano1       = $fmeperavalano1;
//            $fichamonitoramentoEstado->fmeperavalano2       = $fmeperavalano2;//retirar os campos ano2, 3 e 4 - SS3898
//            $fichamonitoramentoEstado->fmeperavalano3       = $fmeperavalano3;
//            $fichamonitoramentoEstado->fmeperavalano4       = $fmeperavalano4;
            $fichamonitoramentoEstado->fmecomcoordenadora   = ($fmecomcoordenadora);
            $fichamonitoramentoEstado->fmecomnumanoatolegal = ($fmecomnumanoatolegal);
            $fichamonitoramentoEstado->fmeequipecontato     = ($fmeequipecontato);
            $fichamonitoramentoEstado->fmeequipetelefone    = ($fmeequipetelefone);
            $fichamonitoramentoEstado->fmeequipeemail       = ($fmeequipeemail);
            $fichamonitoramentoEstado->fmeequipetecnica     = ($fmeequipetecnica);
            $fichamonitoramentoEstado->fmeequipeatolegal    = ($fmeequipeatolegal);
//            $fichamonitoramentoEstado->fmestatus            = $fmestatus;
            $fichamonitoramentoEstado->usucpf               = $usucpf;

            $r = $fichamonitoramentoEstado->salvarHistorico($fichamonitoramentoEstado);
            if($r){
                $fichamonitoramentoEstado->fmedataalteracao  = $data;
                $fichamonitoramentoEstado->inserirManterAspas(array('fmeperavalano1', 'fmeperavalano2','fmeperavalano3','fmeperavalano4'));
            }else{
                $fichamonitoramentoEstado->fmedatainclusao  = $data;
                $fichamonitoramentoEstado->alterar(array(
                    'fmepme',
                    'fmeperavalanual',
                    'fmeperavalbianual',
                    'fmeperavaltrianual',
                    'fmeperavalquadrienal',
                    'fmeperavalquinquenal',
                    'fmeperavalnaoprevisto',
                    'fmeperavalano1',
                    'fmecomcoordenadora',
                    'fmecomnumanoatolegal',
                    'fmeequipecontato',
                    'fmeequipetelefone',
                    'fmeequipeemail',
                    'fmeequipetecnica',
                    'fmeequipeatolegal',
                    'usucpf',
                    'fmedataalteracao'
                ));
            }
            if(!$fichamonitoramentoEstado->commit()){
                echo "Erro ao salvar a Ficha de Técnica.";
            } else {
                echo "<script>alert('Ficha salva com sucesso!');window.location.href='sase.php?modulo=principal/monitoramentoEstado&acao=A';</script>";

            }
            exit;
        case 'exibir_historico':
            $dadosHistorico = $fichamonitoramentoEstado->dadosHistorico($fmeid);
            break;
    }
}

?>
<style>
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
                                        <label class="col-lg-4 control-label">Estado:</label>
                                        <div class="col-lg-8">
                                            <input type="text" id="estdescricao" name="estdescricao" class="form-control" disabled value="<?= $fichamonitoramentoEstado->estuf ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Plano Estadual de Educação:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: Lei nº 458, de 12 de junho de 2015." type="text" id="fmepme" name="fmepme" class="form-control" maxlength="50" value="<?= trim($fichamonitoramentoEstado->fmepme) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Períodos de Avaliação previstos:</label>
                                        <div class="col-lg-8">
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmeperavalanual" class="radio-periodo" value="fmeperavalanual" <?= $checkedanual ? $checkedanual : '' ?> >Anual</label>
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmeperavalbianual" class="radio-periodo" value="fmeperavalbianual" <?= $checkedbianual ? $checkedbianual : '' ?> >Bianual</label>
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmeperavaltrianual" class="radio-periodo" value="fmeperavaltrianual" <?= $checkedtrianual ? $checkedtrianual : '' ?> >Trianual</label>
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmeperavalquadrienal" class="radio-periodo" value="fmeperavalquadrienal" <?= $checkedquadrienal ? $checkedquadrienal : '' ?> >Quadrienal</label>
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmeperavalquinquenal" class="radio-periodo" value="fmeperavalquinquenal" <?= $checkedquinquenal ? $checkedquinquenal : '' ?> >Quinquenal</label>
                                            <label class="radio-inline"><input type="radio" name="periodo" id="fmeperavalnaoprevisto" class="radio-periodo" value="fmeperavalnaoprevisto" <?= $checkednaoprevisto ? $checkednaoprevisto : '' ?> >Não Previsto</label>
                                        </div>
                                    </div>
                                    <div class="form-group ">
                                        <label class="col-lg-4 control-label">Primeiro Ano da Avaliação:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: 2017" type="text" id="fmeperavalano1"  name="fmeperavalano1" class="form-control" maxlength="4" value="<?= trim($fichamonitoramentoEstado->fmeperavalano1) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Comissão Coordenadora:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: Secretaria de Educação, Conselho de Educação, Colegiados Escolares... (descrever todos) E/OU Comissão Coordenadora responsável pelo monitoramento e avaliação do Plano de Educação." type="text" id="fmecomcoordenadora" name="fmecomcoordenadora" class="form-control" maxlength="5000" value="<?= trim($fichamonitoramentoEstado->fmecomcoordenadora) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Ato Legal (n° e ano):</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: Lei nº 458, de 12 de junho de 2015 OU Portaria nº XXX de 5 de maio de 2015" type="text" id="fmecomnumanoatolegal" name="fmecomnumanoatolegal" class="form-control" maxlength="1000" value="<?= trim($fichamonitoramentoEstado->fmecomnumanoatolegal) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Equipe técnica:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.:  Equipe Técnica da Secretaria de Educação, responsável pelo monitoramento e avaliação do Plano de Educação." type="text" id="fmeequipetecnica" name="fmeequipetecnica" class="form-control" maxlength="5000" value="<?= trim($fichamonitoramentoEstado->fmeequipetecnica) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Ato Legal (n° e ano):</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: Portaria nº XXX de 5 de maio de 2015" type="text" id="fmeequipeatolegal" name="fmeequipeatolegal" class="form-control" maxlength="1000" value="<?= trim($fichamonitoramentoEstado->fmeequipeatolegal) ?>"/>
                                        </div>
                                    </div>
                                </fieldset>
                                <fieldset class="scheduler-border">
                                    <legend class="scheduler-border">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Contatos</legend>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">Telefone:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: (XX) XXXX-XXXX" type="text" id="fmeequipetelefone" name="fmeequipetelefone" class="form-control" maxlength="50" value="<?= trim($fichamonitoramentoEstado->fmeequipetelefone) ?>"/>
                                        </div>
                                    </div>
                                    <div class="form-group">
                                        <label class="col-lg-4 control-label">E-mail:</label>
                                        <div class="col-lg-8">
                                            <input data-toggle="tooltip" title="Ex.: monitora@assisbrasil.ac.gov.br" type="text" id="fmeequipeemail" name="fmeequipeemail" class="form-control" maxlength="50" value="<?= trim($fichamonitoramentoEstado->fmeequipeemail) ?>"/>
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
            <div class="modal fade modalHistorico" id="modalHistorico" tabindex="-1" role="dialog" aria-labelledby="modalHistorico" aria-hidden="true">
                <div class="modal-dialog" style="width:80%;">
                    <div class="modal-content">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal">
                                <span aria-hidden="true">&times;</span>
                                <span class="sr-only">Fechar</span>
                            </button>
                            <h4 class="modal-title">Histórico Ficha Técnica do Plano de Educação Estado</h4>
                        </div>
                        <div class="modal-body" id="modal-body">
                            <?php
                            echo $fichamonitoramentoEstado->dadosHistorico($fichamonitoramentoEstado->estuf);
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
</div>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
    function validaFormSalvar(){

        if($('#fmepme').val() === ''){
            $('#fmepme').focus();
            return 'Informe o campo "Plano Estadual de Educação".';
        }
        if($('input[name=periodo]:checked').length <= 0){
            return 'Informe o campo "Períodos de Avaliação previstos".';
        }
        if($('#fmeperavalano1').val() === ''){
            $('#fmeperavalano1').focus();
            return 'Informe o campo "Primeiro Ano da Avaliação".';
        }
        if($('#fmecomcoordenadora').val() === ''){
            $('#fmecomcoordenadora').focus();
            return 'Informe o campo "Comissão Coordenadora".';
        }
        if($('#fmecomnumanoatolegal').val() === ''){
            $('#fmecomnumanoatolegal').focus();
            return 'Informe o campo "Ato Legal (n° e ano)".';
        }
        if($('#fmeequipetecnica').val() === ''){
            $('#fmeequipetecnica').focus();
            return 'Informe o campo "Equipe técnica".';
        }
        if($('#fmeequipeatolegal').val() === ''){
            $('#fmeequipeatolegal').focus();
            return 'Informe o campo "Ato Legal (n° e ano)".';
        }
        if(jQuery('#fmeequipetelefone').val() === '' &&  jQuery('#fmeequipeemail').val() === ''){
            return 'Informe "E-mail" ou "Telefone".';
        }
        if( !validaEmail(jQuery('#fmeequipeemail').val()) && jQuery('#fmeequipeemail').val() != '' ){
            jQuery('#fmeequipeemail').focus();
            return 'E-mail inválido!';
        }
        return true;
    }

    $('documento').ready(function(){
        //informativo sobre os campos
        $('[data-toggle="tooltip"]').tooltip();

        /**
         * Evento dos radions dos períodos de avaliação previstos
         */

        $('.radio-periodo').click(function(){
            $('.divfmeperavalanual').hide();
            $('.divfmeperavalbianual').hide();
            $('.divfmeperavaltrianual').hide();
            $('.divfmeperavalquadrienal').hide();
            $('.divfmeperavalquinquenal').hide();
            $('.divfmeperavalnaoprevisto').hide();
            $('.div'+$(this).val()).show();
        });

        $("#btn-historico").on("click", function() {
            $('#modalHistorico').modal('show');
        });

        $("#fmeperavalano1").mask("9999");
//        $("#fmeperavalano2").mask("9999");//retirar os campos ano2, 3 e 4 - SS3898
//        $("#fmeperavalano3").mask("9999");
//        $("#fmeperavalano4").mask("9999");
        $("#fmeequipetelefone").mask('(00) 0000-0000#');

        $('#btn-salvar').click(function(){
            var fmepme                = $('#fmepme').val();
            var fmeperavalanual       = $('#fmeperavalanual:checked').length === 1 ? true : false;
            var fmeperavalbianual     = $('#fmeperavalbianual:checked').length === 1 ? true : false;
            var fmeperavaltrianual    = $('#fmeperavaltrianual:checked').length === 1 ? true : false;
            var fmeperavalquadrienal  = $('#fmeperavalquadrienal:checked').length === 1 ? true : false;
            var fmeperavalquinquenal  = $('#fmeperavalquinquenal:checked').length === 1 ? true : false;
            var fmeperavalnaoprevisto = $('#fmeperavalnaoprevisto:checked').length === 1 ? true : false;
            var fmeperavalano1        = $('#fmeperavalano1').val();
//            var fmeperavalano2       = $('#fmeperavalano2').val();//retirar os campos ano2, 3 e 4 - SS3898
//            var fmeperavalano3       = $('#fmeperavalano3').val();
//            var fmeperavalano4       = $('#fmeperavalano4').val();
            var fmecomcoordenadora   = $('#fmecomcoordenadora').val();
            var fmecomnumanoatolegal = $('#fmecomnumanoatolegal').val();
            var fmeequipecontato     = $('#fmeequipecontato').val();
            var fmeequipetelefone    = $('#fmeequipetelefone').val();
            var fmeequipeemail       = $('#fmeequipeemail').val();
            var fmeequipetecnica     = $('#fmeequipetecnica').val();
            var fmeequipeatolegal    = $('#fmeequipeatolegal').val();
            var fmestatus            = $('input[name=fmestatus]:checked').val();
            var validacao            = validaFormSalvar();

            if(validacao === true) {
                $.ajax({
                    url: '',
                    type: 'POST',
                    data: {
                        acao_estado: 'salvar',
                        fmepme: fmepme,
                        fmeperavalanual: fmeperavalanual,
                        fmeperavalbianual: fmeperavalbianual,
                        fmeperavaltrianual: fmeperavaltrianual,
                        fmeperavalquadrienal: fmeperavalquadrienal,
                        fmeperavalquinquenal: fmeperavalquinquenal,
                        fmeperavalnaoprevisto: fmeperavalnaoprevisto,
                        fmeperavalano1: fmeperavalano1,
//                        fmeperavalano2: fmeperavalano2,//retirar os campos ano2, 3 e 4 - SS3898
//                        fmeperavalano3: fmeperavalano3,
//                        fmeperavalano4: fmeperavalano4,
                        fmecomcoordenadora: fmecomcoordenadora,
                        fmecomnumanoatolegal: fmecomnumanoatolegal,
                        fmeequipecontato: fmeequipecontato,
                        fmeequipetelefone: fmeequipetelefone,
                        fmeequipeemail: fmeequipeemail,
                        fmeequipetecnica: fmeequipetecnica,
                        fmeequipeatolegal: fmeequipeatolegal,
                        fmestatus: fmestatus
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
