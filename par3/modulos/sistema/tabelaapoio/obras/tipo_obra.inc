<?php
/**
 * View da representando a tabela obra_tipo
 *
 * @category visao
 * @version $Id$
 * @author   JUNIO PEREIRA DOS SANTOS <junio.santos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 */
$controllerObraTipo = new Par3_Controller_ObraTipo();
$mEtapa = new Par3_Model_EnsinoEtapa();
$rsEtapa = simec_preparar_array($mEtapa->recuperarTodos('etaid as codigo,etadsc as descricao'));
switch ($_POST['acao']) {
    case 'salvar':
        $controllerObraTipo->salvar($_POST);
        break;
}
//exit();
?>
<style>
.modal-backdrop{
    z-index: -1 !important;
}
</style>
<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-2">
                <a href="par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=listar_tipo_obra"><i class="fa fa-arrow-left" aria-hidden="true"></i>voltar para a pesquisa</a>
            </div>
            <div class="col-md-8">
                <h2 class="center">Tipo de Obra </h2>
                <h3 class="center"><?= (empty($_GET['otpid']) ? 'Cadastrar' : "Editar - {$controllerObraTipo->model->otpdsc}") ?></h3>
            </div>
            <div class="col-md-2">
            </div>
        </div>
    </div>
    <form method="post" name="form-tipoObra" id="form-tipoObra" class="form form-horizontal">
        <div class="ibox-content">
            <div class="row">
                <input type="hidden" name="acao" id="acao" value="salvar"/>
                <input type="hidden" name="categoria_ampliacao" id="categoria_ampliacao" value="<?= Par3_Model_ObraCategoria::CATEGORIA_AMPLIACAO; ?>"/>
                <input type="hidden" name="otpid" id="otpid" value="<?php echo $_GET['otpid'] ?>"/>
                <?php
                echo $simec->input('otpid', 'Código', $controllerObraTipo->model->otpid, array('class' => 'inteiro', 'readonly' => 'readonly'), array('input-size' => 2));
                echo $simec->input('otpdsc', 'Descrição', $controllerObraTipo->model->otpdsc, array('required' => 'required', 'maxlength' => '255'));
                echo $simec->select('otpensinoetapa[]', 'Etapa Ensino', $controllerObraTipo->getSQLSelectComboNivelEtapa(),$rsEtapa, array('multiple' => 'multiple','placeholder' =>'Selecione','required' => 'required'));
                echo $simec->select('octid', 'Categoria', $controllerObraTipo->model->octid, $controllerObraTipo->getComboCategoriaObras(), array('required' => 'required', 'maxlength' => '1'));
                echo $simec->select('otptipoampliacao', 'Tipo de Ampliação', $controllerObraTipo->model->otptipoampliacao, $controllerObraTipo->getComboCategoriaAmpliacao(), array('maxlength' => '1'));
                echo $simec->select('otptipoprojeto', 'Tipo de Projeto', $controllerObraTipo->model->otptipoprojeto, array('' => ' .: Selecione :. ', 'PF' => 'Projeto FNDE', 'PP' => 'Projeto Próprio'), array('required' => 'required', 'maxlength' => '1'));
                echo $simec->input('otpalunosatendidos', 'Alunos Atendidos', $controllerObraTipo->model->otpalunosatendidos, array('required' => 'required', 'type' => 'number', 'max' => '50000'), array('input-size' => 2));
                echo $simec->select('otpesfera', 'Esfera', $controllerObraTipo->model->otpesfera, array('' => ' .: Selecione :. ', 'E' => 'Estadual', 'M' => 'Municipal', 'T' => 'Todos'), array('required' => 'required', 'maxlength' => '1'));
               
                echo $simec->select('tpoid', 'Tipologia Obras 2.0', $controllerObraTipo->model->tpoid, $controllerObraTipo->getComboTipologiaObras2(), array('required' => 'required', 'maxlength' => '1'));
                echo $simec->select('igrid', 'Grupo de itens de Composição', $controllerObraTipo->model->igrid, $controllerObraTipo->getComboGrupoItens(), array('maxlength' => '1'));
                echo $simec->textarea('otpdetalhe', 'Detalhe', $controllerObraTipo->model->otpdetalhe, array('class' => 'summernote'));
                echo $simec->radio('otpsituacao', 'Situação', $controllerObraTipo->model->otpsituacao, array('A' => 'Ativo', 'I' => 'Inativo'), array('required' => 'required'));
                ?>

            </div>
        </div>
        <div class="ibox-footer">
            <center>
                <button type="submit" class="btn btn-success salvar"><i class="fa fa-save"></i> Salvar</button>
            </center>
        </div>
    </form>
</div>

<script>
    $(function () {

        $('#otpalunosatendidos').closest('.form-group ').hide();
        $('#otpalunosatendidos').removeAttr("required");

        if($("#octid").val() != '' && $("#otptipoprojeto").val() != ''){
            if($("#octid").val() != '3' && $("#otptipoprojeto").val() != 'PP'){
                $('#otpalunosatendidos').closest('.form-group ').show();
                $('#otpalunosatendidos').prop("required",true);
            }
        }

        $("#octid, #otptipoprojeto").change(function(event) {

            if($("#otptipoprojeto").val() != ''){
                
                var categoria = $("#octid").val();
                var tipoProjeto = $("#otptipoprojeto").val();

                if(tipoProjeto != 'PP'){
                    if(categoria == '1' || categoria == '2'){
                        $('#otpalunosatendidos').closest('.form-group ').show();
                        $('#otpalunosatendidos').prop("required",true);
                    }else{
                        $('#otpalunosatendidos').closest('.form-group ').hide();
                        $('#otpalunosatendidos').removeAttr("required").val("");
                    }
                }else{
                        $('#otpalunosatendidos').closest('.form-group ').hide();
                        $('#otpalunosatendidos').removeAttr("required").val("");
                    }
            }
        });

        //otpalunosatendidos

        <?php if($controllerObraTipo->model->otpsituacao == ''){?>
        $('.otpsituacao_A').click();
        <?php }?>

        if($('#octid').val() != $('#categoria_ampliacao').val()){
            $('#otptipoampliacao').closest('.form-group ').hide();
        }

        $('#octid').on('change', function(e) {
            if($(this).val() == $('#categoria_ampliacao').val() ){
                $('#otptipoampliacao').closest('.form-group ').show();
            }else{
                $('#otptipoampliacao').closest('.form-group ').hide();
                $('#otptipoampliacao').val('').trigger('chosen:updated');
            }
        });

    });

//     $("#loading").fadeIn(function(){
//     	$("#loading").fadeOut();
//     });

</script>
