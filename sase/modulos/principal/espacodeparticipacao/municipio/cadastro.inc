<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 07/06/2016
 * Time: 10:25
 */
global $db;
$esp = new Sase_Model_Espacoparticipacao();
$espid = $_REQUEST['espid'];

if(!$espid){
    echo "
            <script>
                alert('Selecione um município para acessar esta funcionalidade.');
                window.location.href = 'sase.php?modulo=principal/espacodeparticipacao&acao=A&tipo=municipio&aba=lista';
            </script>
        ";
    die;
}

$esp->carregarPorId($espid);

if(!$esp->docid1 && $espid){
    $docid = wf_cadastrarDocumento(TPDID_SASE_ESPACOAVALIACAO_FORUM, 'Documento Espaço de Avaliação Forum');
    $esp->docid1 = $docid;
    $esp->sepidforum = $esp->getSituacaoParticipacao($esp->getEstadoDocumento($docid));
    $esp->alterar();
    $esp->commit();
}

if(!$esp->docid2 && $espid){
    $docid = wf_cadastrarDocumento(TPDID_SASE_ESPACOAVALIACAO_CONSELHO, 'Documento Espaço de Avaliação Forum');
    $esp->docid2 = $docid;
    $esp->sepidcons = $esp->getSituacaoParticipacao($esp->getEstadoDocumento($docid));
    $esp->alterar();
    $esp->commit();
}

if(!$esp->docid3 && $espid){
    $docid = wf_cadastrarDocumento(TPDID_SASE_ESPACOAVALIACAO_SISTEMA, 'Documento Espaço de Avaliação Forum');
    $esp->docid3 = $docid;
    $esp->sepidsist = $esp->getSituacaoParticipacao($esp->getEstadoDocumento($docid));
    $esp->alterar();
    $esp->commit();
}


$espforum = $esp->getInfoArquivo('espforum');
$espconselho = $esp->getInfoArquivo('espconselho');
$espsistema = $esp->getInfoArquivo('espsistema');

$estuf = $esp->estuf;
$muncod = $esp->muncod;

/**
 * Carrega o estado
 */
$sql = "select estuf, estdescricao from territorios.estado where estuf = '{$estuf}' order by estuf";
$estados = $db->carregar($sql);

/**
 * Carrega o município
 */
$sql = "select muncod, mundescricao from territorios.municipio where muncod = '{$muncod}'";
$municipios = $db->carregar($sql);


?>
<style>
    span.file-input-name{
        display: none;
    }
</style>
<script type="text/javascript" src="/library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script>
    function enviaFormulario(){
        jQuery('[name=acao]').val('salvar');
        jQuery('[name=form-principal]').submit();
    }

    function apagaArquivo(tipo, arqid, campofile)
    {
        if (confirm('Deseja deletar este arquivo?')) {
            jQuery('[name=acao]').val('apaga_arquivo');
            jQuery('#arqid').val(arqid);
            jQuery('#campofile').val(campofile);
            jQuery('[name=form-principal]').submit();
        }
    }

    function downloadArquivo(tipo, arqid)
    {
        jQuery('[name=acao]').val('download_arquivo');
        jQuery('#arqid').val(arqid);
        jQuery('[name=form-principal]').submit();
    }
    $(function(){
        $('input[type=file]').bootstrapFileInput();
    });
</script>
<div>
    <form class="form-horizontal" name="form-principal" id="form-principal" role="form" method="POST" enctype="multipart/form-data">
        <input type="hidden" name="acao" id="acao" value="salvar"/>
        <input type="hidden" name="espid" id="espid" value="<?= $espid ?>"/>
        <input type="hidden" name="estuf" id="estuf" value="<?= $estuf ?>"/>
        <input type="hidden" name="muncod" id="muncod" value="<?= $muncod ?>"/>
        <input type="hidden" name="arqid" id="arqid" value=""/>
        <input type="hidden" name="campofile" id="campofile" value=""/>

        <div class="row" style="margin-top: 10px">
            <div class="col-lg-offset-2 col-lg-6 ">
                <div class="well">

                    <div class="form-group">
                        <label for="estuf" class="col-lg-2 control-label">Estado:</label>
                        <div class="col-lg-9">
                            <select name="estuf" id="estuf" class="form-control chosen-select" disabled>
                                <option value="">Selecione</option>
                                <?php
                                foreach ($estados as $chave => $estado) {
                                    $sel = $estado['estuf'] == $estuf ? 'selected' : '';
                                    echo <<<HTML
                                        <option value="{$estado['estuf']}" {$sel}>{$estado['estdescricao']}</option>
HTML;
                                }
                                ?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="estuf" class="col-lg-2 control-label">Municipio:</label>
                        <div class="col-lg-9" id="divMunicipio">
                            <select name="estuf" id="estuf" class="form-control chosen-select" disabled>
                                <option value="">Selecione</option>
                                <?php
                                foreach ($municipios as $chave => $municipio) {
                                    $sel = $municipio['muncod'] == $muncod ? 'selected' : '';
                                    echo <<<HTML
                                        <option value="{$municipio['muncod']}" {$sel}>{$municipio['mundescricao']}</option>
HTML;
                                }
                                ?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="estuf" class="col-lg-2 control-label">Forum:</label>
                        <div class="col-lg-9">
                            <div class="col-lg-3 inputarea">
                                <?php if($esp->validaDocid($esp->docid1, '260')){ ?>
                                    <input type="file" class="btn btn-primary start" accept=".pdf" name="espforum" id="espforum" title="Selecionar arquivo" />
                                <?php } else { ?>
                                    <button type="button" class="btn btn-primary" disabled >Selecionar arquivo</button>
                                <?php } ?>
                                <input type="hidden" name="arqidforum" id="arqidforum" value="">
                            </div>
                            <div class="col-lg-9" style="padding-top: 10px;">
                                <?php if ($espforum['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidforum', <?=$espforum['arqid']?>)"
                                       style="margin-left: 15px; cursor: pointer;"><?=$espforum['arqdescricao']?></a>
                                    <a onclick="apagaArquivo('arqidforum', <?=$espforum['arqid']?>, 'espforum')"
                                       style="margin-left: 10px; cursor: pointer;"><img src="/imagens/excluir.gif"/></a>
                                <?php } else { ?>
                                    <span id="spnespforum" style="margin-left: 15px;"></span>
                                <?php } ?>
                            </div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="estuf" class="col-lg-2 control-label">Conselho:</label>
                        <div class="col-lg-9">
                            <div class="col-md-3 inputarea">
                                <?php if($esp->validaDocid($esp->docid2, '261')){ ?>
                                    <input type="file" class="btn btn-primary start" accept=".pdf" name="espconselho" id="espconselho" title="Selecionar arquivo" />
                                <?php } else { ?>
                                    <button type="button" class="btn btn-primary" disabled >Selecionar arquivo</button>
                                <?php } ?>

                                <input type="hidden" name="arqidconselho" id="arqidconselho" value="">
                            </div>
                            <div class="col-lg-9" style="padding-top: 10px;">
                                <?php if ($espconselho['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidconselho', <?=$espconselho['arqid']?>)" id="aespconselho"
                                       style="margin-left: 15px; cursor: pointer;"><?=$espconselho['arqdescricao']?></a>
                                    <a onclick="apagaArquivo('arqidconselho', <?=$espconselho['arqid']?>, 'espconselho')"
                                       style="margin-left: 10px; cursor: pointer;"><img src="/imagens/excluir.gif"/></a>
                                <?php } else { ?>
                                    <span id="spnespconselho" style="margin-left: 15px;"></span>
                                <?php } ?>
                            </div>
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="estuf" class="col-lg-2 control-label">Sistema:</label>
                        <div class="col-lg-9">
                            <div class="col-md-3 inputarea">
                                <?php if($esp->validaDocid($esp->docid3, '262')){ ?>
                                    <input type="file" class="btn btn-primary start" accept=".pdf" name="espsistema" id="espsistema" title="Selecionar arquivo" />
                                <?php } else { ?>
                                    <button type="button" class="btn btn-primary" disabled >Selecionar arquivo</button>
                                <?php } ?>
                                <input type="hidden" name="arqidsistema" id="arqidsistema" value="">
                            </div>
                            <div class="col-lg-9" style="padding-top: 10px;">
                                <?php if ($espsistema['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidsistema', <?=$espsistema['arqid']?>)" id="aespsistema"
                                       style="margin-left: 15px; cursor: pointer;"><?=$espsistema['arqdescricao']?></a>
                                    <a onclick="apagaArquivo('arqidsistema', <?=$espsistema['arqid']?>, 'espsistema')"
                                       style="margin-left: 10px; cursor: pointer;"><img src="/imagens/excluir.gif"/></a>
                                <?php } else { ?>
                                    <span id="spnespsistema" style="margin-left: 15px;"></span>
                                <?php } ?>
                            </div>
                        </div>
                    </div>

                    <div class="form-group">
                        <div class="col-lg-12" align="center">
                            <input type="button" name="btnSalvar" id="btnSalvar" onclick="enviaFormulario()" value="Salvar" class="btn btn-success"/>
                        </div>
                    </div>
                </div>

            </div>
            <div class="col-lg-1">
                <?php
                if($esp->docid1 && $espid){
                    wf_desenhaBarraNavegacao($esp->docid1, array('docid1'=>$esp->docid1, 'docid2'=>$esp->docid2, 'docid3'=>$esp->docid3, 'espid'=>$esp->espid), null, 'Forum');
                }
                ?>
            </div>

            <div class="col-lg-1">
                <?php
                if($esp->docid2 && $espid){
                    wf_desenhaBarraNavegacao($esp->docid2, array('docid1'=>$esp->docid1, 'docid2'=>$esp->docid2, 'docid3'=>$esp->docid3, 'espid'=>$esp->espid), null, 'Conselho');
                }
                ?>
            </div>

            <div class="col-lg-1">
                <?php
                if($esp->docid3 && $espid){
                    wf_desenhaBarraNavegacao($esp->docid3, array('docid1'=>$esp->docid1, 'docid2'=>$esp->docid2, 'docid3'=>$esp->docid3, 'espid'=>$esp->espid), null, 'Sistema');
                }
                ?>
            </div>
        </div>
    </form>
</div>
<script>
    $(function(){
        $('input[type=file]').change(function(){
            val = $(this).val();
            arr = val.split('\\');
            $('#spn'+$(this).attr('id')).html(arr[arr.length  - 1]);
        });
    });
</script>