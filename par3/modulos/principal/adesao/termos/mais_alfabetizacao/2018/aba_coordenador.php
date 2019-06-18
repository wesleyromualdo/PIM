<div class="panel-body">
    <?php
    $objOrientacao->getOrientacaoByID(ORIENTACOES_MAISALFABETIZACAO_ABA_COORDENADOR, "Coordenador");
    ?>
    <hr class="divider">
    <form method="post" name="formulario-coordenador" id="formulario-coordenador" class="form form-horizontal">
        <input type="hidden" value="salvar_coordenador" id="requisicao" name="requisicao"/>
        <input type="hidden" value="<?php echo $dadoCoordenador['cmaid'] ?>" id="cmaid" name="cmaid"/>
        <div class="">

            <center>
                <h3>Dados do Coordenador</h3>
            </center>

            <?php
            if ($esdid != WF_ESDID_EMCADASTRAMENTO_MAISALFABETIZACAO) {
                $disabled1 = 'disabled=disabled';
            }
            ?>


            <div class="form-group ">
                <label for="cmacpf" class="col-sm-3 col-md-3 col-lg-3 control-label">CPF: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                <div class="col-sm-9 col-md-9 col-lg-9 ">

                    <div class="input-group">
                        <span class="input-group-addon"><span class="fa fa-user"></span></span>
                        <input name="cmacpf" id="cmacpf" <?= $disabled1 ?> value="<?= $dadoCoordenador['cmacpf'] ?>" class="form-control" onkeydown="javascript: fMasc( this, mCPF );" onclick="javascript: fMasc( this, mCPF );" required="required" type="text">
                    </div>

                </div>
                <div style="clear:both"></div>
            </div>

            <?php

            echo $simec->input('cmanome', 'Nome', $dadoCoordenador['cmanome'], array('maxlength' => '255', true, 'readonly' => 'readonly', 'required'), array('label-size' => 3, 'input-size' => 9));
            echo $simec->email('cmaemail', 'E-mail', $dadoCoordenador['cmaemail'], array('class' => 'email', $disabled1, 'required'), Array('required'));
?>
            <div class="form-group ">
                <label for="cmatelefone" class="col-sm-3 col-md-3 col-lg-3 control-label">Telefone: <span class="campo-obrigatorio" title="Campo obrigatório">*</span></label>
                <div class="col-sm-9 col-md-9 col-lg-9 ">

                    <div class="input-group">
                        <span class="input-group-addon"><span class="fa fa-phone"></span></span>
                        <input name="cmatelefone" id="cmatelefone" <?= $disabled1 ?>  value="<?= $dadoCoordenador['cmatelefone'] ?>" class="form-control" onkeydown="javascript: fMasc( this, mTel );" required="required" maxlength="14" type="text">
                    </div>

                </div>
                <div style="clear:both"></div>
            </div>

            <?php
            if ((!$bloqueiaGeral) && ($mostrar) && $esdid == WF_ESDID_EMCADASTRAMENTO_MAISALFABETIZACAO) {
                ?>
                <center>
                    <button type="submit" class="btn btn-success salvar" <?php echo $disabled; ?>><i class="fa fa-save"></i> Salvar</button>
                    <?php
                    echo $botaoRemoverCoordenador;
                    ?>
                </center>
            <?php } ?>


        </div>
    </form>
</div>