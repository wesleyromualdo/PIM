<?php require_once APPRAIZ . 'par3/classes/controller/adesao_programa/escolaAcessivel.inc'; ?>

<script src="/par3/js/sistema/adesao/escola_acessivel/escola_acessivel.js"></script>
<style>
    .navbar-listagem {
        width: auto!important;
    }    
</style>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox">
            <div class="ibox-title">
                <div class="row">
                    <div class="col-md-7">
                        <h3>Termo de adesão -
                            <?php
                            echo $modalInstrumentoUnidade->inudescricao;
                            ?>
                        </h3>
                        <h5>Programa Escola Acessível</h5>
                    </div>
                    <div class="col-md-2 text-right">
                        <div class="m-t-sm">
                            <?php
                            $sql = "SELECT 
                                            to_char(generate_series, 'YYYY') as codigo,
                                            to_char(generate_series, 'YYYY') as descricao
                                    FROM 
                                        generate_series(
                                            date_trunc('year', (SELECT min(adpano_ciclo)||'/01/01' FROM par3.prodesaoprograma where pfaid = 12 )::date),
                                            date_trunc('year', current_date),
                                            INTERVAL'1 year');";
                            
                            echo $simec->select('adpano_ciclo', '', $adpano_ciclo, $sql, array('data-placeholder' => 'Ano'), array('label-size' => 0, 'input-size' => 12));
                            ?>
                        </div>
                    </div>
                    <div class="col-md-3 text-right">  
                        
                        <div class="m-t-sm">
                            
                            <?php if(!empty($arquivoResolucao)) : ?>
                                <button type="button" class="btn btn-success" onclick="downloadResolucao(<?php echo $arquivoResolucao; ?>);"><i class="fa fa-upload"></i> Resolução</button>
                            <?php endif; ?>
                            <?php if(!empty($arquivoManual)) : ?>
                                <button type="button" class="btn btn-danger" onclick="downloadManual(<?php echo $arquivoManual; ?>);"><i class="fa fa-file-pdf-o"></i> Manual</button>
                            <?php endif; ?>
                        </div>
                        <?php
                            $arrayParams['itrid'] = $itrid;
                            $arrayParams['inuid'] = $inuid;
                            $arrayParams['adpid'] = $adpid;
                            $arrayParams['adpano_ciclo'] = $adpano_ciclo;
                            $escolasSelecionadasInuid = $objEscolasEscolaAcessivel->getTotalEscolasSelecionadasInuid($arrayParams);
                        ?>
                    </div>
                </div>
            </div>
            <div class="ibox-content" style="padding:20px">
            <?php
                $ocultarInfo = '';
                if ($escolasSelecionadasInuid == 0 ) {
                    $ocultarInfo = "style='display: none'";
                }
            ?>
            <?php if ($escolasSelecionadasInuid == 0) { ?>
                <div class="row">
                    <div class="col-md-12 text-center">
                        <div class="alert alert-info"><i class="fa fa-info-circle"></i> Sua rede não foi contemplada neste programa.</div>
                    </div>
                </div>
            <?php } ?>
                <div class="tabs-container" <?php echo $ocultarInfo; ?>>
                    <ul class="nav nav-tabs">
                        <?php if ($escolasSelecionadasInuid > 0) { ?>
                            <li id="li1" class="<?php if ( ($aceite['adpresposta'] != "S" || $proadesaoDados['pfaid'] != 3)  && $escolasSelecionadasInuid > 0  ) echo "active"; ?>"><a data-toggle="tab" href="#tab-1" aria-expanded="true">Termo</a></li>
                        <?php } ?>
                        <?php
                            if (
                                ($esdid == WF_ESDID_TERMONAOACEITO_ESCOLAACESSIVEL) 
                                && in_array(PAR3_PERFIL_DIRIGENTE_MUNICIPAL_DE_EDUCACAO, $perfis) === true
                               ) { 
                        ?>
                        <?php } else { ?>
                            <li id="li2" class="<?php if ($escolasSelecionadasInuid == 0) echo "active" ?>"><a data-toggle="tab" href="#tab-2" aria-expanded="false">Escolas</a>
                            <?php if ($escolasSelecionadasInuid > 0) { ?>
                                <li id="li4" class=""><a data-toggle="tab" href="#tab-4" aria-expanded="false">Síntese</a>
                            <?php } ?>
                        <?php } ?>
                    </ul>

                    <div class="tab-content">
                        <div id="tab-1" class="tab-pane <?php if ( ($aceite['adpresposta'] != "S" || $proadesaoDados['pfaid'] != 3) && $escolasSelecionadasInuid > 0 ) echo "active" ?>">                            
                            <?php require_once APPRAIZ."par3/modulos/principal/adesao/termos/escola_acessivel/aba_termo.php" ?>
                        </div>
                        <?php if (($esdid == WF_ESDID_TERMONAOACEITO_ESCOLAACESSIVEL) && in_array(PAR3_PERFIL_DIRIGENTE_MUNICIPAL_DE_EDUCACAO, $perfis) === true): ?>
                        
                        <?php else: ?>
                            <div id="tab-2" class="tab-pane <?php if ($escolasSelecionadasInuid == 0) echo "active" ?>">
                                <?php require_once APPRAIZ."par3/modulos/principal/adesao/termos/escola_acessivel/aba_escolas.php" ?>
                            </div>
                            <div id="tab-4" class="tab-pane">
                                <?php require_once APPRAIZ."par3/modulos/principal/adesao/termos/escola_acessivel/aba_sintese.php" ?>
                            </div>
                        <?php endif; ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<input type="hidden" id="aba_atual" value="<?php echo $_REQUEST['abaatual']; ?>">
<input type="hidden" id="inuid" value="<?php echo $_REQUEST['inuid']; ?>">
<input type="hidden" id="prgid" value="<?php echo $_REQUEST['prgid']; ?>">

<div id="modal-form" class="modal fade" aria-hidden="true">

    <div class="modal-dialog" style="height: 90%; width: auto;">
        <div class="ibox-title" style="width: auto;">
            <h5 id="html_modal-title" style="width: auto;"></h5>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form" style="width: auto;">
        </div>
    </div>
</div>
<div id="modal-form2" class="modal fade" aria-hidden="true">
    <div class="modal-dialog" style="height: 80%; width: 80%;">
        <div class="ibox-title" style="width: auto;">
            <h5 id="html_modal-title2" style="width: auto;"></h5>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form2" style="width: auto;">
        </div>
    </div>
</div>
<script>    
    function downloadResolucao(arqid){
        window.location = 'par3.php?modulo=principal/adesao/termo&acao=A&prgid=<?php echo $_GET['prgid'] ?>&inuid=<?php echo $_GET['inuid'] ?>&arquivo_resolucao='+arqid;
    }
    function downloadManual(arqid){
        window.location = 'par3.php?modulo=principal/adesao/termo&acao=A&prgid=<?php echo $_GET['prgid'] ?>&inuid=<?php echo $_GET['inuid'] ?>&arquivo_manual='+arqid;
    }
    
    <?php //if ( !in_array(PAR3_PERFIL_SUPER_USUARIO, $perfis) ): ?>
        <?php if ($escolasSelecionadasInuid == 0 ) : ?>
            $(function () {

                //if ( dataFechamento < dataAtual ){
                    var texto = 
                        "Sua rede não possui escolas contempladas pelo programa.<br>" +
                        "Em função dos limites orçamentários, neste ciclo foram priorizadas escolas com maior número de matrículas de estudantes da educação especial.<br><br><br>" +
                        "Aguarde o novo processo de adesão.<br>" +
                        "<br>" +
                        "Atenciosamente.<br>";
                    swal({
                        title: "",
                        text: texto,
                        type: "warning",
                        confirmButtonColor: "#4caf50", confirmButtonText: "OK",
                        html: true
                    },
                    function (isConfirm) {
                        window.location = '/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>';
                    });
                //} 
            }); 
        <?php endif; ?>
    <?php //endif ?>
</script>