<?php

$controllerEvento = new Par3_Controller_Evento();
$modelEvento = new Par3_Model_Evento($dados['eveid']);
$modelEvento->popularDadosObjeto();
$modelEvento->evecor = $dados['evecor'] ? '#' . $dados['evecor'] : $modelEvento->evecor;

$localizacoes = $modelEvento->recuperarLocalizacao();

$regioes = empty(current($localizacoes['regiao'])) ? 0 : 1;
$estados = empty(current($localizacoes['estado'])) ? '' : "['".implode("', '", $localizacoes['estado'])."']";
$municipios = empty(current($localizacoes['municipio'])) ? '' : "['".implode("', '", $localizacoes['municipio'])."']";


$nivelAcesso = $controllerEvento->recuperarNivelAcesso($dados['inuid']);

$podeEditar = (!$dados['eveid'] ||
                ($nivelAcesso == Par3_Controller_Evento::K_NIVEL_ACESSO_CONFIGURACAO && $modelEvento->evetipo != Par3_Model_Evento::K_TIPO_INSTRUMENTO_UNIDADE  && !$dados['inuid']) ||
                ($nivelAcesso == Par3_Controller_Evento::K_NIVEL_ACESSO_UNIDADE      &&  $modelEvento->evetipo == Par3_Model_Evento::K_TIPO_INSTRUMENTO_UNIDADE && $dados['inuid'])
              ) ? 1 : false;
?>

<form method="post" name="formulario-modal" id="formulario-modal" class="form form-horizontal">
    <input type="hidden" name="pode-editar" id="pode-editar" value="<?php echo $podeEditar; ?>"/>

    <input type="hidden" name="action" value="salvar"/>
    <input type="hidden" name="eveid" id="eveid" value="<?php echo $modelEvento->eveid; ?>"/>
    <?php

    $simec->setPodeEditar($podeEditar);



    echo $simec->input('evetitulo', 'Título', $modelEvento->evetitulo, array('required' => 'required', 'maxlength' => '300'));
    echo $simec->data('evedatainicio', 'Data Início', $modelEvento->evedatainicio, array('required' => 'required', 'maxlength' => '300'));
    echo $simec->data('evedatafim', 'Data Fim', $modelEvento->evedatafim);
    echo $simec->input('evecor', 'Cor', $modelEvento->evecor, array('type' => 'color'));
    echo $simec->textarea('evedsc', 'Descrição', $modelEvento->evedsc);

    if(empty($dados['inuid'])){
        ?>
        <h3>Aplicar para</h3>
        <?php
        echo $simec->select('regcod[]', 'Região', $localizacoes['regiao'], array('territorios.regiao', 'trim(regcod)', 'regdescricao'), array('id'=>'id_regiao'));
        echo $simec->select('estuf[]', 'UF', null, array('territorios.estado', 'estuf', "estuf || ' - ' || estdescricao"), array('id'=>'id_uf'));
        echo $simec->select('muncod[]', 'Município', null, array(), array('id'=>'id_municipio'));

        if($modelEvento->eveid){
            echo '<div class="text-center"><button type="button" class="btn btn-danger" id="excluir-evento" data-eveid="' . $modelEvento->eveid . '">Excluir</button></div>';
        }

    }
    else {
        if ($modelEvento->evetipo == "I" && $modelEvento->eveid) {
            echo '<div class="text-center"><button type="button" class="btn btn-danger" id="excluir-evento" data-eveid="' . $modelEvento->eveid . '">Excluir</button></div>';
        }
    }
    ?>
</form>

<script type="text/javascript">
    $(function(){
        <?php if($regioes){ ?>
            $('#id_regiao').change();
        <?php } ?>

        <?php if($estados){ ?>
            setTimeout(function(){
                $('#id_uf').val(<?php echo $estados; ?>);
                $('#id_uf').change();
                $('#id_uf').trigger("chosen:updated");

                setTimeout(function(){
                    $('#id_municipio').val(<?php echo $municipios; ?>);
                    $('#id_municipio').trigger("chosen:updated");
                }, 1000);
            }, 1000);
        <?php } ?>

        $('#excluir-evento').click(function(){
            if(confirm('Deseja realmente excluir este evento?')){
                eveid = $(this).data('eveid');

                $.ajax({
                    url: 'par3.php?modulo=principal/evento/calendario&acao=A&action=excluir-evento&eveid=' + eveid,
                    type: 'POST',
                    dataType: 'json',
                    success: function (response) {
                        if(response){
                            $('#btn-fechar-evento').click();
                            $('#calendar').fullCalendar( 'removeEvents', eveid);
                        }
                    }
                });

            }
        });
    });
</script>