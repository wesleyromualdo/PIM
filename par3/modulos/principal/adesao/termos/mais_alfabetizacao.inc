<?php require_once APPRAIZ . 'par3/classes/controller/adesao_programa/maisAlfabetizacao.inc'; ?>

<script src="/par3/js/sistema/adesao/mais_alfabetizacao/mais_alfabetizacao.js"></script>

<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox">
            <div class="ibox-title">
                <div class="row">
                    <div class="col-md-6 input-lg">
                        <h3>Termo de adesão -
                            <?php
                            echo $modalInstrumentoUnidade->inudescricao;
                            ?>
                        </h3>
                    </div>
                    <div class="col-md-6 input-lg">
                        <?php
                        $sql = "SELECT 
                                	to_char(generate_series, 'YYYY') as codigo,
                                	to_char(generate_series, 'YYYY') as descricao
                                FROM 
                                    generate_series(
                                    	date_trunc('year', (SELECT min(adpano_ciclo)||'/01/01' FROM par3.prodesaoprograma)::date),
                                    	date_trunc('year', current_date),
                                    	INTERVAL'1 year');";
                        $arrAttr = ['data-placeholder' => 'Ano'];
                        echo $simec->select('adpano_ciclo', '', $adpano_ciclo, $sql, $arrAttr);
                        ?>
                    </div>
                </div>
            </div>

            <div class="ibox-content" style="padding:40px">
                <div class="tabs-container">

                    <ul class="nav nav-tabs">
                        <li id="li1" class="<?php if ($aceite['adpresposta'] != "S" || $proadesaoDados['pfaid'] != 3) echo "active"; ?>">
                            <a data-toggle="tab" href="#tab-1" aria-expanded="true">Termo</a></li>
                        <?php
                        if (($esdid == WF_ESDID_TERMONAOACEITO_MAISALFABETIZACAO) && in_array(PAR3_PERFIL_DIRIGENTE_MUNICIPAL_DE_EDUCACAO, $perfis) === true): ?>


                        <?php else: ?>

                        <li id="li2" class=""><a data-toggle="tab" href="#tab-2" aria-expanded="false">Escolas</a>
                        <li id="li3" class=""><a data-toggle="tab" href="#tab-3" aria-expanded="false">Coordenador</a>
                        <li id="li4" class=""><a data-toggle="tab" href="#tab-4" aria-expanded="false">Síntese</a>

                            <?php endif; ?>
                    </ul>

                    <div class="tab-content">

                        <div id="tab-1" class="tab-pane <?php if ($aceite['adpresposta'] != "S" || $proadesaoDados['pfaid'] != 3) echo "active" ?>">
                            <?php require_once "mais_alfabetizacao/2018/aba_termo.php" ?>
                        </div>

                        <?php
                        if (($esdid == WF_ESDID_TERMONAOACEITO_MAISALFABETIZACAO) && in_array(PAR3_PERFIL_DIRIGENTE_MUNICIPAL_DE_EDUCACAO, $perfis) === true): ?>


                        <?php else: ?>
                            <div id="tab-2" class="tab-pane">
                                <?php require_once APPRAIZ."par3/modulos/principal/adesao/termos/mais_alfabetizacao/2018/aba_escolas.php" ?>
                            </div>

                            <div id="tab-3" class="tab-pane">
                                <?php require_once APPRAIZ."par3/modulos/principal/adesao/termos/mais_alfabetizacao/2018/aba_coordenador.php" ?>
                            </div>

                            <div id="tab-4" class="tab-pane">
                                <?php require_once APPRAIZ."par3/modulos/principal/adesao/termos/mais_alfabetizacao/2018/aba_sintase.php" ?>
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

    <div class="modal-dialog" style="height: 90%; width: auto; overflow-y: scroll">
        <div class="ibox-title" style="width: auto;">
            <h5 id="html_modal-title" style="width: auto;"></h5>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form" style="width: auto;">
        </div>
    </div>
</div>
<div id="modal-form2" class="modal fade" aria-hidden="true">
    <div class="modal-dialog" style="height: 80%; width: 80%; overflow-y: scroll">
        <div class="ibox-title" style="width: auto;">
            <h5 id="html_modal-title2" style="width: auto;"></h5>
            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        </div>
        <div id="html_modal-form2" style="width: auto;">
        </div>
    </div>
</div>