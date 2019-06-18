<div class="form-group">
    <label class="col-lg-3 control-label">Código:</label>
    <div class="col-lg-2">
        <input type="text" disabled="" value="<?php echo $model->iarid; ?>" class="form-control">
    </div>
</div>
<input type="hidden" name="iarid" value="<?php echo $model->iarid; ?>"/>
<?php
$rsAreaPai = array();
if($model->iarid){
    $rsAreaPai = $model->pegarSQLSelectComboDescricao(array('iuoid'=>$model->iuoid,'areafilho' => $model->iarid));
}
?>
<?php echo $simec->input('co_unidade_org', 'Código Área', $model->co_unidade_org, array('maxlength' => '10', 'type'=>'number','required', 'title' => 'Insira o código da Área')); ?>
<?php echo $simec->select('iuoid', 'Unidade Orçamentária', $model->iuoid, $controllerUnidadeOrcamentaria->pegarSelectCombo(), array('required')); ?>
<?php echo $simec->select('iarid_pai', 'Área Responsável', $model->iarid_pai,$rsAreaPai); ?>
<?php echo $simec->input('iarsigla', 'Sigla', $model->iarsigla, array('maxlength' => '255', 'required', 'title' => 'Insira a sigla da Área')); ?>
<?php echo $simec->input('iardsc', 'Área', $model->iardsc, array('maxlength' => '500', 'required', 'title' => 'Insira a descrição da Área')); ?>
<?php echo $simec->radio('iarsituacao', 'Situação', (!($model->iarsituacao) ? 'A' : $model->iarsituacao), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline')); ?>
