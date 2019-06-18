<div class="form-group">
    <label class="col-lg-3 control-label">Código:</label>
    <div class="col-lg-2">
        <input type="text" disabled="" value="<?php echo $ata->atpid; ?>" class="form-control">
    </div>
</div>
<input type="hidden" name="atpid" value="<?php echo $ata->atpid; ?>">
<?php
$anos = array();
$anoAtual = date('Y') - 2;
$anoFim = $anoAtual + 15;
for ($anoAtual; $anoAtual < $anoFim; $anoAtual++) {
    $anos += array($anoAtual => $anoAtual);
}
$mUf = new Par3_Model_Estado();
$mAte = new Par3_Model_ItenscomposicaoAtaPregaoEstado();
$rsUf = simec_preparar_array($mUf->recuperarTodos('estuf as codigo,estdescricao as descricao ', null, 'estuf ASC'));
$disablePeriodo = '';
if ($ata->atpid) {
    $dataHoje = date('Y-m-d H:i:s');
    $dataFimAntiga = date('Y-m-d H:i:s', strtotime($ata->atpdtfim));
    if ($dataFimAntiga < $dataHoje) {
        $disablePeriodo = 'disabled';
    }
}

$file = new FilesSimec(null, null, 'par3');
echo $simec->input('atpnumeroata', 'Nº Ata', $ata->atpnumeroata, array('maxlength' => '255', 'required', 'title' => 'Informe o número da Ata'));
echo $simec->select('atpanovigencia', 'Ano Ata', $ata->atpanovigencia, $anos, array('maxlength' => '255', 'required', 'title' => 'Informe o ano de vigência da ata'));//,array('class' => 'col-md-3')

echo $simec->periodo('atpperiodo', 'Período', array('', ''), array('required', $disablePeriodo, 'title' => 'Informe o Período de Vigência da Ata'), $attribs = array(), $config = array());
?>
<div class="row">
    <div class=" form-group">
        <div class="col-sm-3 col-md-3 col-lg-3" style="margin-left: 20px;" id="atpperiodo" name="atpperiodo"></div>
    </div>
</div>
<?php
echo $simec->input('atpvigencia', 'Vigência', $ata->atpvigencia, array('maxlength' => '255', 'required', $disablePeriodo, 'title' => 'Informe os dias de vigência da Ata'));
echo $simec->textarea('atpobjeto', 'Objeto', $ata->atpobjeto, array('title' => 'Informe o objeto da Ata.'));
echo $simec->select('estuf[]', 'UF', $mAte->recuperarPorAta($ata->atpid), $rsUf, array('maxlength' => '255','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '9'));
echo $simec->textarea('atpdescricaoanexo', 'Descrição do Anexo', $ata->atpdescricaoanexo, array('', 'title' => 'Informe uma descrição para o anexo.'));
?>

<div class="form-group">
    <label class="col-sm-3 col-md-3 col-lg-3 control-label">Anexo:</label>
    <div class="col-sm-9 col-md-9 col-lg-9">
        <label class="btn btn-primary col-sm-2 col-md-2 col-lg-2" for="input-arqid">
            <input id="input-arqid" type="file" name="arqid" style="display:none;"/>
            <?php echo $ata->arqid ? 'Alterar' : 'Buscar'; ?>
        </label>&nbsp;
        <div class="col-lg-9 col-md-9 col-sm-9">
            <span class="label" id="upload-file-info"><?php echo $ata->arqid ? $file->getNomeArquivo($ata->arqid) : ''; ?></span>
            <div class="">
                <small style="margin-bottom: -400px">*Somente arquivos PDF</small>
            </div>
        </div>
    </div>
    <div class="col-sm-3 col-md-3 col-lg-3" style="margin-left: 20px;" id="arqid" name="arqid"></div>
</div>
<?php
echo $simec->radio('atpsituacao', 'Situação', ($ata->atpsituacao ? $ata->atpsituacao : "A"), array('A' => 'Ativo', 'I' => 'Inativo'), array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
?>
<script>
    $('#estuf').change(function(){
        var e = jQuery.Event("click");
        e.ctrlKey = true;
        $('#estuf').trigger(e);
    });
    $('#estuf').prepend(new Option('Todos', 'TODOS', 'selected'));
    <?php if($ata->atpdtfim && $ata->atpdtinicio): ?>
    $('[name=atpperiodoinicio]').val('<?php echo formata_data($ata->atpdtinicio)?>');
    $('[name=atpperiodofim]').val('<?php echo formata_data($ata->atpdtfim)?>');
    <?php endif; ?>
    $('#input-arqid').change(function () {
        $('#upload-file-info').html($(this).val());
    });
    jQuery(document).find("#formAtaPregao").on('click', "input[name=atpsituacao ][value=I]", function () {
        swal("Atenção.", "Ao alterar a situação para Inativo o registro não estará mais visível para selecinar", "warning");
    });
</script>

