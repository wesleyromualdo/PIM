<?php
$displayUF  = 'none';
$displayMun = 'none';
if($_REQUEST['tipo_regiao'] == 'M'){ $displayMun = '';$displayUF  = '';}
if($_REQUEST['tipo_regiao'] == 'E') $displayUF = '';
?>
<div class="row" id="linha_<?php echo $_REQUEST['linha'] ?>">
    <div class="ibox-content">
        <div class="row">
            <div class="col-lg-10">
                <div class="form-group">
                    <div class="col-lg-12">
                        <input type=file name="arqid_<?php echo $_REQUEST['linha'] ?>" style="margin-left:60px" required class="input-arqid"/>
                    </div>
                </div>
            </div>
            <div class="col-sm-1">
                <button type="button" class="btn btn-danger" linha="<?php echo $_REQUEST['linha'] ?>" class="excluir_linha" id="excluir" title="Excluir Linha" style="margin-left:30px;width:40px;">
                    <i class="fa fa-times"></i>
                </button>
            </div>
        </div><br>
        <div class="row">
            <div id="selectEstuf_<?php echo $_REQUEST['linha'] ?>" linha="<?php echo $_REQUEST['linha'] ?>" class="col-sm-4" style="display:<?php echo $displayUF?>;margin-left:40px">
                <?php echo $simec->select('estuf['.$_REQUEST['linha'].'][]', '', '', $sqlEstado,array(),array('input-size'=>12)) ; ?>
            </div>
            <div id="selectMuncod_<?php echo $_REQUEST['linha'] ?>" linha="<?php echo $_REQUEST['linha'] ?>" class="col-sm-5"" style="display:<?php echo $displayMun?>">
                <?php echo $simec->select('muncod['.$_REQUEST['linha'].'][]', '', '', $sqlMunicipio,array(),array('input-size'=>12)) ; ?>
            </div>
        </div>
    </div>
</div>