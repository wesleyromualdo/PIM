<?php if($this->getIsModal() == null):?>  <div class="ibox-content"><?php endif;?>
        <div class="row">
            <?php
            $arraAgrupadorId = array('type' => 'hidden');
            echo $simec->input('agrid','','',$arraAgrupadorId);
            ?>
        </div>
        <div class="row">
            <div class="col-md-offset-2 col-md-8 center">
                <div id="div_preid" class="form-group ">
                    <div class="col-sm-12">
                        <h4>
                            <?php
                            $arrPreId = array('required'=>'required');
                            echo $simec->input('preid','ID da Obra','',$arrPreId);
                            ?>
                        </h4>
                        <div class="row" style="margin-top: -20px;">
                            <div class="col-md-12">
                                <span class="span-error" id="span_preid" style="padding: 20px;"></span>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-md-offset-2 col-md-8 center">
                <div id="div_agrdesc" class="form-group">
                    <div class="col-sm-12">
                        <h4>
                            <?php
                            $arrDesc = array('required'=>'required');
                            echo $simec->input('agrdesc','Agrupador','',$arrDesc);
                            ?>
                        </h4>
                        <div class="row" style="margin-top: -20px;">
                            <div class="col-md-12">
                                <span class="span-error" id="span_agrdesc" style="padding: 20px;"></span>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <?php if($this->getIsModal() == null):?>
        <div class="row" style="margin-top: 20px;">
            <div class="col-md-offset-2 col-md-8 center">
                <div class="form-group ">
                    <div class="col-sm-12">
                        <button class="btn btn-default btn-cancelar-agrupador"><i class="fa fa-times-circle-o"></i> Cancelar</button>
                        <button class="btn btn-success btn-salvar-agrupador" type="button"><i class="fa fa-save"></i> Salvar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <?php endif;?>

