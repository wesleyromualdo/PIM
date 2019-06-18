<?php
$disabledAnalise = $_GET['disabledanalise4'];
$cont = 1;

if($_GET['etaid'] == 1){
    $labelEtapa = 'Ensino Infantil';
    $inputEtapa = 'infantil';
}

if($_GET['etaid'] == 2){
    $labelEtapa = 'Ensino Fundamental';
    $inputEtapa = 'fundamental';
}

if($_GET['etaid'] == 3){
    $labelEtapa = 'Ensino Médio';
    $inputEtapa = 'medio';
}
?>
<?php foreach($escolas as $esc): ;?>
    <?php $cont++ ;?>
<div class="panel panel-default" id="panel-escola-<?php echo $esc['escid']?>">
    <div class="panel-heading"><b><?php echo $esc['escnome'];?></b>
        <?php if(!$disabledAnalise):?>
        <button
            type="button"
            class="close removerEscola"
            data-target="#panel-escola-<?php echo $esc['escid']?>"
            data-dismiss="alert"
            placeholder="Remover"
            id="<?php echo $esc['escid'];?>"
        >
        <span aria-hidden="true">&times;</span><span class="sr-only">Remover</span>
        </button>
    <?php endif;?>
    </div>
    <div class="panel-body">
    <div class="row">
        <input type="hidden" name="escid[]" value="<?php echo $esc['escid']; ?>" />
        <input type="hidden" value="<?php echo $esc['qtd']; ?>" id="qtd-restantes"/>

        <div class="row" id="esccodinep-div">
            <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">INEP: </label>
            <div class="col-md-8 col-md-8 col-lg-8 control-label" >
                <p class="text-left" id="esccodinep">
                    <?php echo $esc['esccodinep']; ?>
                </p>
            </div>
        </div>

        <div class="row" id="esc_situacao_funcionamento-div">
            <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                Situação do Funcionamento:
            </label>
            <div class="col-md-8 col-md-8 col-lg-8 control-label" >
                <p class="text-left" id="esc_situacao_funcionamento">
                    <?php echo $esc['escno_situacao_funcionamento']; ?>
                </p>
            </div>
        </div>

        <div class="row" id="escno_situacao_imovel-div">
            <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                Situação do Imóvel:
            </label>
            <div class="col-md-8 col-md-8 col-lg-8 control-label" >
                <p class="text-left" id="escno_situacao_imovel">
                    <?php echo $esc['escno_situacao_imovel']; ?>
                </p>
            </div>
        </div>

        <div class="row" id="escno_localizacao-div">
            <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                Localização/Zona da Escola:
            </label>
            <div class="col-md-8 col-md-8 col-lg-8 control-label" >
                <p class="text-left" id="escno_localizacao">
                    <?php echo $esc['escno_localizacao']; ?>
                </p>
            </div>
        </div>

        <div class="row" id="escendereco_bairro-div">
            <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                Endereço/Bairro:
            </label>
            <div class="col-md-8 col-md-8 col-lg-8 control-label" >
                <p class="text-left" id="escendereco_bairro">
                    <?php echo $esc['escendereco_bairro']; ?>
                </p>
            </div>
        </div>

        <div class="row" id="predominanciaatentimento-div">
            <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                Predominância de Atendimento:
            </label>
            <div class="col-md-8 col-md-8 col-lg-8 control-label" >
                <p class="text-left" id="predominanciaatentimento">
                    <?php echo $labelEtapa; ?>
                </p>
            </div>
        </div>

        <div class="row" id="escqtd_salas-div">
            <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                Quantidade de Salas de Aula:
            </label>
            <div class="col-md-8 col-md-8 col-lg-8 control-label" >
                <p class="text-left" id="escqtd_salas">
                    <?php echo $esc['escqtd_salas']; ?>
                </p>
            </div>
        </div>
        <br>
        <div class="row" id="div-qtd-escolas">
            <div class="row" id="ederemanejamento_infantil-total-div">
                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                    Quantidade Total de Alunos Ensino Infantil:
                </label>
                <div class="col-md-3 col-md-3 col-lg-3 control-label" >
                    <?php echo $esc['escqtd_alunos_infantil']; ?>
                </div>
            </div>

            <div class="row" id="ederemanejamento_fundamental-total-div">
                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                    Quantidade Total Alunos Ensino Fundamental:
                </label>
                <div class="col-md-3 col-md-3 col-lg-3 control-label" >
                    <?php echo $esc['escqtd_alunos_fundamental']; ?>
                </div>
            </div>

            <div class="row" id="ederemanejaparmento_fundamental-total-div">
                <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                    Quantidade Total Alunos Ensino Médio:
                </label>
                <div class="col-md-3 col-md-3 col-lg-3 control-label" >
                    <?php echo $esc['escqtd_alunos_medio']; ?>
                </div>
            </div>

        </div>

        <?php
        $quantidadeAlunosRestantes = 0;
        switch($_GET['etaid']) {
            case 1:  $quantidadeAlunosRestantes = $esc['qtd'] - $esc['ederemanejamento_infantil'];break;
            case 2:  $quantidadeAlunosRestantes = $esc['qtd'] - $esc['ederemanejamento_fundamental'];break;
            case 3:  $quantidadeAlunosRestantes = $esc['qtd'] - $esc['ederemanejamento_medio'];break;
            default: $quantidadeAlunosRestantes = $esc['qtd'];
        }
        ?>
        <hr/>
        <div class="row" id="ederemanejamento-div">
            <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                Quantidade Restante Alunos <?= $labelEtapa;?>:
            </label>
            <div
                class="col-md-3 col-md-3 col-lg-3 control-label"
                id="qtd-restante-label<?php echo $esc['esccodinep'];?>">
                <?php echo abs($quantidadeAlunosRestantes); ?>
            </div>

            <input
                type="hidden"
                id="qtd-restante-input<?php echo $esc['esccodinep'];?>"
                value="<?php echo $esc['qtd']; ?>"
            />
        </div>
        <div class="row">
            <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">
                Quantidade de alunos remanejados:
            </label>
            <div class="col-md-4 col-md-4 col-lg-4 control-label" >
                <input
                    type="number"
                    min="1" max="<?php echo $esc['qtd']; ?>"
                    required="required" name="ederemanejamento_<?= $inputEtapa;?>[]"
                    id="ederemanejamento_<?= $inputEtapa;?>-<?php echo $esc['esccodinep'];?>"
                    value="<?php echo $esc['ederemanejamento_'.$inputEtapa.'']?>"
                    class="form-control qtdalunos"
                    data-msg-min="Quantidade mínima permitida: 1"
                    <?php echo $disabledAnalise;?>
                    data-msg-max="Quantidade Máxima permitida: <?php echo $esc['qtd']; ?>"
                />
            </div>
        </div>
    </div>
</div>
</div>
    <?php /** Resolve o problema de não perder o evento de change e keyup e não conflita com o jquery validate*/?>
    <script>
        $('#ederemanejamento_<?= $inputEtapa;?>-<?php echo $esc['esccodinep'];?>').bind('change keyup',function () {
            let val = $(this).val();
            let qtdRestante      = $('#qtd-restante-input<?php echo $esc['esccodinep'];?>').val();
            let qtdRestanteLabel = $('#qtd-restante-label<?php echo $esc['esccodinep'];?>');
            let restante = parseInt(qtdRestante) - parseInt(val);
            if(val < 0 || val === '' || isNaN(parseInt(val))) {
                $(this).val('');
                qtdRestanteLabel.html(qtdRestante);
                return;
            }
            if(parseInt(val) > parseInt(qtdRestante)) {
                $(this).val(qtdRestante);
                qtdRestanteLabel.html(qtdRestante);
                return;
            }
            qtdRestanteLabel.html(restante);
        });
        $('#ederemanejamento_<?= $inputEtapa;?>-<?php echo $esc['esccodinep'];?>').bind('focusout',function () {
            let val = $(this).val();
            let qtdRestante      = $('#qtd-restante-input<?php echo $esc['esccodinep'];?>').val();
            let qtdRestanteLabel = $('#qtd-restante-label<?php echo $esc['esccodinep'];?>');
            if(val < 0 || val === '' || isNaN(parseInt(val))){
                $(this).val('');
                qtdRestanteLabel.html(qtdRestante);
            }
        });
    </script>
<?endforeach; ?>