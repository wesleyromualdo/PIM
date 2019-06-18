<h3>Além do remanejamento de alunos das escolas indicadas no item anterior, o município ainda necessita expandir a vaga devido à demanda reprimida na região?</h3>
<div class="text-center">

    <?php
    echo $simec->radio('ampliacao_justificativa', '', 'N', array('S' => 'Sim','N' => 'Não'), array('', 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline', 'input-size' => '6', 'label-size' => 5));
    ?>
</div>
<div id="div-ampliacao_justificativa" style="display: none;">
    <?php echo $simec->textarea('outros', 'Justificativa', '', array('required'), array('label-size' => '2', 'input-size' => '8')); ?>
</div>