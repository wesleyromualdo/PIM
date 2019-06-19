<?php $simec = new Simec_View_Helper(); ?>
<div class="well">
    <fieldset>
        <form action="" id="formModal" name="formPrincipal" method="post"
              class="form-horizontal" enctype="multipart/form-data">
            <input type='hidden' name="modulo_requisicao" id="modulo"
                   value='<?php echo MODULO; ?>'/>
            <input type="hidden" name="requisicao" value="editar"/>
            <input type="hidden" id="angidModal" name="angid" value;>

            <section class="form-group">
                <label class="control-label col-md-2">Perfil:</label>
                <section class="col-md-10">
                    <?php
                    echo inputCombo("perfil", (new Seguranca_Model_Perfil())->montaSQLCombo($_SESSION['sisid']), null, "perfilModal", ['multiple' => true]); ?>
                </section>
            </section>
            <section class="form-group">
                <label class="control-label col-md-2">Unidade Orçamentária:</label>
                <section class="col-md-10">
                    <?php
                    $resultado = (new Public_Model_Unidade())->obtemUnidadesOrcamentariasMec();
                    echo inputCombo("unidadeOrcamentaria", $resultado, null, "unidadeOrcamentariaModal", ['multiple' => true]); ?>
                </section>
            </section>
            <?php echo $simec->periodo(array('datainiModal', 'datafimModal'), 'Período', array('', ''), array('type' => 'date'), array('label-size' => '2', 'input-size' => '10')); ?>
            <div class="col-lg-12" style="padding-left: 219px; padding-bottom: 15px;">
                <input type="checkbox" name="flagPeriodoIndeterminado"
                       id="flagPeriodoIndeterminadoModal" style="font-size: 6px;"/>
                Mostrar mensagem por período indeterminado.
            </div>
            <div class="form-group">
                <label for="mensagem" class="col-lg-2 control-label">Mensagem:</label>
                <div class="col-lg-10">
                    <?php inputTextArea('mensagem', '', 'mensagemModal', 1000, array('obrig' => 'N', 'habil' => 'S', 'rows' => 10)); ?>
                </div>
            </div>
            <div class="form-group">
                <label for="file" class="col-lg-2 control-label">Anexar Arquivos:</label>
                <div class="col-lg-10">
                    <input type="hidden" name="execucao" id="execucao" value="cadastrar">
                    <input type="hidden" name="arqid" id="arqid">

                    <input type="file" class="btn btn-primary start" name="file" id="file"
                           title="Selecionar arquivo"/>
                </div>
            </div>
            <div class="form-group">
                <label for="nomeArquivo" class="col-lg-2 control-label">Nome Arquivo:</label>
                <div class="col-lg-10">
                    <input type="text" maxlength="255" class="normal form-control"
                           name="nomeArquivo" id="nomeArquivoModal"/>
                </div>
            </div>
        </form>
    </fieldset>
</div>
<script>
    $(function(){
       $('#datainiModal').attr('name', 'dataini');
        $('#datafimModal').attr('name', 'datafim');
    });
</script>