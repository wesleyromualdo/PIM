<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 15:34
 */

global $db;
$pceid = $_REQUEST['pceid'];

if(!$pceid){
    echo "
            <script>
                alert('Selecione um estado para acessar esta funcionalidade.');
                window.location.href = 'sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=lista';
            </script>
        ";
    die;
}
$planodecarreiraestado = new PlanodecarreiraEstado();
$planodecarreiraestado->carregarPorId($pceid);
$pfls = arrayPerfil();

if(!$planodecarreiraestado->docid && $pceid){
    $docid = wf_cadastrarDocumento(TPDID_SASE_PLANODECARREIRA, 'Documento Plano de Carreira');
    $planodecarreiraestado->docid = $docid;
    $planodecarreiraestado->alterar();
    $planodecarreiraestado->commit();
}

if(!$planodecarreiraestado->docid2 && $pceid){
    $docid = wf_cadastrarDocumento(TPDID_SASE_SITUACAO_PLANOCARREIRA, 'Documento Situação Adequação Plano de Carreira');
    $planodecarreiraestado->docid2 = $docid;
    $planodecarreiraestado->alterar();
    $planodecarreiraestado->commit();
}

if($planodecarreiraestado->docid) {
    $sql = "select esdid from workflow.documento where docid = " . $planodecarreiraestado->docid;
    $esdid = $db->pegaUm($sql);
}


if($planodecarreiraestado->docid2) {
    $sql = "select esdid from workflow.documento where docid = " . $planodecarreiraestado->docid2;
    $esdid2 = $db->pegaUm($sql);
}

?>
<style>
    #formulario{
        padding-top: 20px;
    }
    .btn{
        margin-left: auto;
        margin-right: auto;
    }
</style>
<script type="text/javascript" src="/library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script>
    function voltar(){
        window.location.href = "sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=lista";
    }
    /**
     * Envia Formulário
     */
    function enviaFormulario(){
       jQuery('[name=acao]').val('salva_arquivo');
       var fileato =  jQuery('[name=fileato]').val();
        if(fileato != '') {
            if (!validoFormatoArquivo(fileato)) {
                alert("Anexar arquivo do tipo PDF!");
                return false;
            };
        }
       jQuery('[name=formCadastroLista]').submit();
    }

    function validoFormatoArquivo(arquivo) {
        var extensoes, ext, valido;
        extensoes = new Array('.pdf');
        ext = arquivo.substring(arquivo.lastIndexOf(".")).toLowerCase();
        valido = false;
        for (var i = 0; i <= arquivo.length; i++) {
            if (extensoes[i] == ext) {
                valido = true;
                break;
            }
        }
        if (valido) {
            return true;
        }
        return false;
    }
    function apagaArquivo(tipo, arqid)
    {
        if (confirm('Deseja deletar este arquivo?')) {
            jQuery('[name=acao]').val('apaga_arquivo');
            jQuery('#' + tipo).val(arqid);
            jQuery('[name=formCadastroLista]').submit();
        }
    }

    function downloadArquivo(tipo, arqid)
    {
        jQuery('[name=acao]').val('download_arquivo');
        jQuery('#' + tipo).val(arqid);
        jQuery('[name=formCadastroLista]').submit();
    }

    $(function(){
        $('input[type=file]').bootstrapFileInput();




    });
</script>
<div id="formulario">
    <form class="form-horizontal" name="formCadastroLista" role="form" method="POST" class=form-vertical enctype="multipart/form-data">
        <input type="hidden" id="acao" name="acao" value=""/>
        <input type="hidden" id="pceid" name="pceid" value="<?= $pceid ?>"/>
        <div class="row">
            <div class="col-lg-offset-2 col-lg-7">
                <div class="well">
                    <legend>Plano de Carreira</legend>

                    <div class="form-group">
                        <label class="col-lg-2 col-md-2 control-label" for="estuf">Estado:</label>
                        <div class="col-lg-9 col-md-9">
                            <select name="estuf" id="estuf" disabled class="form-control">
                                <?php
                                $sql = "select estuf, estdescricao from territorios.estado order by estdescricao ASC";
                                $estados = $db->carregar($sql);
                                foreach( $estados as $chave => $estado ){ ?>
                                    <option <?=($estado['estuf']==$planodecarreiraestado->estuf)?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="divFiltroSituacao">
                        <label for="filtroSituacao" class="col-lg-2 control-label">Plano de carreira:</label>
                        <div class="col-lg-9">
                            <select name="esdid" disabled class="form-control" id="esdid">
                                <option value="">Filtrar situação</option>
                                <?php
                                $sql = "select esdid, spcdsc from sase.sitplancarprofessor where spcstatus = 'A' order by spcid ASC ";
                                $situacoes = $db->carregar( $sql );
                                foreach( $situacoes as $chave => $situacao ){ ?>
                                    <option <?=($situacao['esdid']==$esdid)?'selected':''?> value="<?=$situacao['esdid']?>"><?=$situacao['spcdsc']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="divFiltroAdequacao">
                        <label for="filtroAdequacao" class="col-lg-2 control-label">Adequação</label>
                        <div class="col-lg-9">
                            <select name="filtroAdequacao" id="filtroAdequacao" disabled class="form-control">
                                <?php
                                $sql = "select esdid, spmdsc from sase.sitplanomunicipio where spcstatus = 'A' order by spmid ASC";
                                $situacoes = $db->carregar( $sql );
                                foreach( $situacoes as $chave => $situacao ){ ?>
                                    <option <?=($situacao['esdid']==$esdid2)?'selected':''?> value="<?=$situacao['esdid']?>"><?=$situacao['spmdsc']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                       <label for="filepcr" class="col-lg-2 col-md-2 control-label">PCR:</label>
                       <input type="hidden" name="arqidpcr" id="arqidpcr" value="">
                        <?php if(in_array(PFLCOD_SASE_SUPER_USUARIO, $pfls) || in_array(PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE, $pfls) ) { ?>
                            <div class="col-md-3 inputarea">
                                <input type="file" class="btn btn-primary start" accept=".pdf" name="filepcr" id="filepcr" title="Selecionar arquivo PCR" />
                            </div>
                            <div class="col-md-7" style="padding-top: 10px; padding-left: 50px">
                                <?php if ($arquivopcr['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidpcr', <?=$arquivopcr['arqid']?>)"
                                       style="margin-left: 15px; cursor: pointer;"><?=$arquivopcr['arqdescricao']?></a>
                                    <a onclick="apagaArquivo('arqidpcr', <?=$arquivopcr['arqid']?>)"
                                       style="margin-left: 10px; cursor: pointer;"><img src="/imagens/excluir.gif"/></a>
                                <?php } ?>
                            </div>
                        <?php } else { ?>
                            <div class="col-md-10" style="padding-top: 10px;">
                                <?php if ($arquivopcr['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidpcr', <?=$arquivopcr['arqid']?>)"
                                       style="margin-left: 15px; cursor: pointer;"><?=$arquivopcr['arqdescricao']?></a>
                                <?php } ?>
                            </div>
                        <?php } ?>
                    </div>

                    <div class="form-group">
                        <label for="fileadesao" class="col-lg-2 col-md-2 control-label">Termo de adesão:</label>
                        <input type="hidden" name="arqidadesao" id="arqidadesao" value="">
                        <?php if(in_array(PFLCOD_SASE_SUPER_USUARIO, $pfls) || in_array(PFLCOD_SASE_ADMINISTRADOR, $pfls) || in_array(PFLCOD_SASE_EXECUTIVO, $pfls) || in_array(PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE, $pfls)) { ?>
                            <div class="col-md-3 inputarea">
                                <input type="file" class="btn btn-primary start" accept=".pdf" name="fileadesao" id="fileadesao" title="Selecionar Termo de Adesão" />
                            </div>
                            <div class="col-md-7" style="padding-top: 10px; padding-left: 50px">
                                <?php if ($arquivoadesao['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidadesao',<?=$arquivoadesao['arqid'] ?>)"
                                       style="margin-left: 15px; cursor: pointer;"><?=$arquivoadesao['arqdescricao'] ?></a>
                                    <a onclick="apagaArquivo('arqidadesao',<?=$arquivoadesao['arqid'] ?>)"
                                       style="margin-left: 10px; cursor: pointer;"><img src="/imagens/excluir.gif"/></a>
                                <?php } ?>
                            </div>
                        <?php } else { ?>
                            <div class="col-md-10" style="padding-top: 10px;">
                                <?php if ($arquivoadesao['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidadesao',<?=$arquivoadesao['arqid'] ?>)"
                                       style="margin-left: 15px; cursor: pointer;"><?=$arquivoadesao['arqdescricao'] ?></a>
                                <?php } ?>
                            </div>
                        <?php } ?>
                    </div>
                    <div class="form-group">
                        <label for="fileato" class="col-lg-2 col-md-2 control-label">Ato Legal da Comissão:</label>
                        <input type="hidden" name="arqidato" id="arqidato" value="">
                        <?php if(in_array(PFLCOD_SASE_SUPER_USUARIO, $pfls) || in_array(PFLCOD_SASE_ADMINISTRADOR, $pfls) || in_array(PFLCOD_SASE_EXECUTIVO, $pfls) || in_array(PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE, $pfls) ) { ?>
                            <div class="col-md-3 inputarea">
                                <input type="file" class="btn btn-primary start" accept=".pdf" name="fileato" id="fileato" title="Selecionar Ato Legal da Comissão" />
                            </div>
                            <div class="col-md-7" style="padding-top: 10px; padding-left: 50px">
                                <?php if ($arquivoato['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidato',<?=$arquivoato['arqid'] ?>)"
                                       style="margin-left: 15px; cursor: pointer;"><?=$arquivoato['arqdescricao'] ?></a>
                                    <a onclick="apagaArquivo('arqidato',<?=$arquivoato['arqid'] ?>)"
                                       style="margin-left: 10px; cursor: pointer;"><img src="/imagens/excluir.gif"/></a>
                                <?php } ?>
                            </div>
                        <?php } else { ?>
                            <div class="col-md-10" style="padding-top: 10px;">
                                <?php if ($arquivoato['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidato',<?=$arquivoato['arqid'] ?>)"
                                       style="margin-left: 15px; cursor: pointer;"><?=$arquivoato['arqdescricao'] ?></a>
                                <?php } ?>
                            </div>
                        <?php } ?>
                    </div>

                    <div class="form-group">
                        <label for="filempl" class="col-lg-2 col-md-2 control-label">Minuta do PL:</label>
                        <input type="hidden" name="arqidmpl" id="arqidmpl" value="">
                        <?php if(in_array(PFLCOD_SASE_SUPER_USUARIO, $pfls) || in_array(PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE, $pfls) ) { ?>
                            <div class="col-md-3 inputarea">
                                <input type="file" class="btn btn-primary start" accept=".pdf" name="filempl" id="filempl" title="Selecionar arquivo Minuta do PL" />
                            </div>
                            <div class="col-md-7" style="padding-top: 10px; padding-left: 50px">
                                <?php if ($arquivompl['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidmpl', <?=$arquivompl['arqid']?>)"
                                       style="margin-left: 15px; cursor: pointer;"><?=$arquivompl['arqdescricao']?></a>
                                    <a onclick="apagaArquivo('arqidmpl', <?=$arquivompl['arqid']?>)"
                                       style="margin-left: 10px; cursor: pointer;"><img src="/imagens/excluir.gif"/></a>
                                <?php } ?>
                            </div>
                        <?php } else { ?>
                            <div class="col-md-10" style="padding-top: 10px;">
                                <?php if ($arquivompl['arqid']) { ?>
                                    <a onclick="downloadArquivo('arqidmpl', <?=$arquivompl['arqid']?>)"
                                       style="margin-left: 15px; cursor: pointer;"><?=$arquivompl['arqdescricao']?></a>
                                <?php } ?>
                            </div>
                        <?php } ?>
                    </div>

                    <hr />
                    <div class="form-group">  
                        <div class="col-lg-12 text-center">
                            <input type="button" name="btnSalvar" id="btnSalvar" onclick="enviaFormulario()" value="Salvar" class="btn btn-primary"/>
                            <input onclick="javascript:window.location.href='sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=lista'"
                                   value="Voltar" type="button" class="btn btn-primary"/>
                        </div>
                    </div>

                </div>
            </div>
            <div class="col-lg-1">
                <?php
                    if($planodecarreiraestado->docid && $pceid){
                        wf_desenhaBarraNavegacao($planodecarreiraestado->docid, array('docid'=>$planodecarreiraestado->docid), null, 'Plano de Carreira');
                    }
                ?>
            </div>
            <div class="col-lg-1">
                <?php
                    if($planodecarreiraestado->docid2 && $pceid){
                        wf_desenhaBarraNavegacao($planodecarreiraestado->docid2, array('docid'=>$planodecarreiraestado->docid2), null, 'Adequação');
                    }
                ?>
            </div>
        </div>
    </form>
</div>