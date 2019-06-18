<div class="panel panel-default" id="">
    <div class="panel-heading"><b><?php echo $ctr->otpdsc;?></b></div>
    <div class="panel-body">
        <div class="col-lg-12">
            <label class="col-lg-3 control-label text-right">Tipo de Projeto:</label>
            <div class="col-md-3 col-md-3 col-lg-3 control-label" ><?php echo $ctr->otptipoprojeto == 'PF'?'Projeto FNDE':'Projeto Próprio';?></div>
        </div>

        <div class="col-lg-12">
            <label class="col-lg-3 control-label text-right">Categoria:</label>
            <div class="col-md-3 col-md-3 col-lg-3 control-label" >
                <?php
                    switch($ctr->octid){
                        case 1: echo 'Construção';break;
                        case 2: echo 'Ampliação' ;break;
                        case 3: echo 'Reforma'   ;break;
                    }
                ?>
            </div>
        </div>


        <?php if($ctr->otptipoprojeto == 'PP'):?>
            <div class="col-lg-12">
                <div class="form-group"><label class="col-md-offset-1 col-lg-offset-1 col-sm-2 text-right">Alunos Atendidos:</label>
                    <div class="col-md-3 col-lg-3">
                        <input type="number"
                               min="0"
                               max="9999"
                               data-msg-min="Quantidade mínima permitida: 0"
                               data-msg-max="Quantidade Máxima permitida: 9999"
                               required="required"
                            <?php echo $disabledAnalise;?>
                       name="obralunos_atendidos" id="obralunos_atendidos" value="<?php echo $mdObra->obralunos_atendidos?$mdObra->obralunos_atendidos:0?>" style="text-align:right;" maxlength="255" class="form-control dadostipoobra" />
                    </div>
                </div>
            </div>
            <div class="col-lg-12"></div>
            <div class="col-lg-12"></div>
            <div class="col-lg-12"></div>
            <div class="col-lg-12">
                <div class="form-group"><label class=" col-lg-offset-1 col-sm-2 control-label text-right">Valor Estimado:</label>
                    <div class="col-md-3 col-lg-3">
                        <input type="text" name="obrvalor_estimado" id="obrvalor_estimado" value="<?php echo $mdObra->obrvalor_estimado; ?>" maxlength="255" class="form-control dadostipoobra" <?php echo $disabledAnalise;?>/>
                    </div>
                </div>
            </div>
        <?php else:;?>
            <div class="col-lg-12">
                <label class=" col-lg-3 control-label text-right">Alunos Atendidos:</label>
                <div class="col-md-3 col-md-3 col-lg-3 control-label" ><?php echo $ctr->otpalunosatendidos;?></div>
            </div>
        <?php endif;?>
        <div class="col-lg-12">
            <br/>
            <h3 class="text-center">Detalhes</h3>
        </div>
        <div class="row">
                <?php
                echo $simec->textarea('otpdetalhe', '', $ctr->otpdetalhe, array('class' => 'summernote','disabled',$disabledAnalise),array('input-size' => '12'));
                ?>
        </div>
    </div>
</div>

<script>
    $('#otpdetalhe').summernote({
        shortcuts: false,
        toolbar: []
    });
//    $('#obrvalor_estimado').val(mascaraglobal('###.###.###,##',$('#obrvalor_estimado').val()));
    $("#obrvalor_estimado").inputmask('decimal', {
        'alias': 'numeric',
        'groupSeparator': '.',
        'autoGroup': true,
        'digits': 2,
        'min': 0,
        'max': 999999999999,
        'radixPoint': ",",
        'digitsOptional': false,
        'allowMinus': false,
//        'prefix': 'R$ ',
        'placeholder': ''
    });
</script>
