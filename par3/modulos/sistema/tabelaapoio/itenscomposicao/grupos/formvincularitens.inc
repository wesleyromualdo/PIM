<?php
 $controllerGruposItens = new Par3_Controller_ItensComposicaoGruposItens();
//listar
$igrid = $_REQUEST['igrid'];
$mInto = new Par3_Model_IniciativaTiposObjeto();
$grupoTipoObj = new Par3_Model_ItensComposicaoGruposTiposObjeto();

//Recupera os tipos de objeto do grupo
$grupoObj = implode(',',$grupoTipoObj->recuperarPorGrupo($_REQUEST['igrid']));
//Recupera somente os tipos de objeto selecionados pelo grupo
$rsInto   = $mInto->recuperarTodos('intoid as codigo,intodsc as descricao ', array("intoid in ({$grupoObj})","intosituacao = 'A' "), 'intodsc ASC');
?>

<form method="post" name="formitemvinculo" id="formitemvinculo" class="form form-horizontal">
    <div class="form-group">
        <?php
        echo $simec->select('intoid[]', 'Tipo de Objeto', '', simec_preparar_array($rsInto), array('maxlength' => '255','required','title' => 'Selecione um Tipo de Objeto','multiple' => 'multiple','placeholder' =>'Selecione'), array('input-size' => '6'));
        echo $simec->input('itcnome', 'Nome do Item', '', array('maxlength' => '255', 'title' => 'Informe um nome para um grupo de Item'),array('input-size'=>'6'));
        ?>
    </div>
    <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
        <button type="button" class="btn btn-primary" id="pesquisar-itemvinculo" title="Pesquisar por Itens"><i class="fa fa-search"></i>Pesquisar</button>
        <button type="button" class="btn btn-danger"  id="limpar-formitemvinculo" title="Limpar Pesquisa"><i class="fa fa-eraser"></i></button>
    </div>
</form>
<hr>
<div id="listagemitens">
    <?php $controllerGruposItens->listarItens($_REQUEST); ?>
</div>

<script>
    $('.i-checks').iCheck({//Função para utilizar o checkbox estilizado
        checkboxClass: 'icheckbox_square-green',
        radioClass: 'iradio_square-green',
    });

    $('#pesquisar-itemvinculo').click(function(evt){
        evt.preventDefault();
        var form = $('#formitemvinculo').serialize();
        var caminho = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=grupoitens&submenu=form';
        var action = '&reqvincularitem=pesquisar&' + form+'&igrid=<?php echo $igrid; ?>';
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#listagemitens').html(resp);
                $('.i-checks').iCheck({//Função para utilizar o checkbox estilizado
                    checkboxClass: 'icheckbox_square-green',
                    radioClass: 'iradio_square-green',
                });
            }
        });
    });

    jQuery("#limpar-formitemvinculo").click(function(){
        var form = $("#formitemvinculo");
        form.find('#itcnome').val('');
        form.find("#intoid").val('').trigger('chosen:updated');
    });
</script>
