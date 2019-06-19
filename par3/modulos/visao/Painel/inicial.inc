<style>
    #toast-container.toast-bottom-center > div {
        padding-left: 50px;
        width: 300px;
        margin: auto;
    }
    #toast-container.toast-top-full-width > div,
    #toast-container.toast-bottom-full-width > div {
        width: 100%;
        margin: auto;
    }
    .toast-success {
        background-color: #4caf50;
        opacity: 0.8;
    }
    .ibox-loading-dialog-content-par3 {
        /*background-color: #000;*/
        /*height: 40px;*/
        margin-left: auto;
        margin-right: auto;
        margin-top: 40%;
        vertical-align: middle;
        width: 400px;
        z-index: 999999;
    }

    .ibox-loading-dialog-content-par3 img {
        float: left;
        margin-left: 24px;
        vertical-align: middle;
        width: 24px;
    }

    .ibox-loading-dialog-content-par3 span {
        color: #fff;
        float: left;
        margin-left: 20px;
        text-align: left;
        line-height: 18px;
        font-weight: bold;
    }
    h1,h2,h3,h4,h5,h6 {
        font-weight: initial;!important;
        line-height: initial;!important;
    }

    h1 a,h2 a,h3 a,h4 a,h5 a,h6 a {
        font-weight: initial;!important;
    }

    h1 {
        font-size: initial;!important;
        line-height: initial;!important;
        margin: initial;!important;
    }

    h2 {
        font-size: initial;!important;
        line-height: initial;!important;
        margin: initial;!important;
    }

    h3 {
        font-size: initial;!important;
        line-height: initial;!important;
        margin: initial;!important;
    }

    h4 {
        font-size: initial;!important;
        line-height: initial;!important;
        margin: initial;!important;
    }

    h5 {
        font-size: initial;!important;
        line-height: initial;!important;
        margin: initial;!important;
    }

    h6 {
        font-size: initial;!important;
        line-height: initial;!important;
        margin: initial;!important;
    }

</style>
<script type="text/javascript">

$(document).ready(function() {
  $('#loading').remove();
  $('#caixaPainel').sortable({
    cursor: 'move',
    placeholder: 'ui-state-highlight',
    start: function(e, ui) {
      ui.placeholder.width(ui.item.find('.asdf').width());
      ui.placeholder.height(ui.item.find('.asdf').height());
      ui.placeholder.addClass(ui.item.attr("class"));
    }
  });
});

</script>
<style>
.painel-radius {
    border:1px solid #DDDDDD;
}
</style>
<?php
global $simec;
$ano          = $this->ano?' Ano:'.$this->ano:'';
$inudescricao = $this->inu->inudescricao;
$estuf        = $this->inu->estuf;
$subtitulo    = $this->inu?$this->inu->itrid == 2 ?"Município: {$inudescricao} - {$estuf}":"Estado: {$inudescricao}":'';
?>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-12">
            <h3 class="h3 center">Painel <small> <?= $subtitulo.$ano;?></small></h3>
        <ol class="breadcrumb">
            <li><a href="par3.php?modulo=inicio&amp;acao=C">Página Inicial</a></li>
            <li><a>Principal</a></li>
            <li><a>Painel</a></li>
        </ol>
    </div>
</div>

<form action="" id="form-painel">
    <input type="hidden" name="inuid" value="<?= $this->inu->inuid;?>"/>
    <input type="hidden" name="ano" value="<?= $this->ano;?>"/>
<div id="caixaPainel" class="row resizable-panels" style="display:block;">

    <?php
    for ($i = 0;$i < count($this->quadro);$i++) {
        if($this->quadro[$i]->isVisivel()) {
            $this->quadro[$i]->renderizarHelper();
        }
    }
    ?>
</div>
    <div id="materialButton" class="click-to-toggle fixed-action-btn" >
        <a class="btn_par-floating btn_par-large blue" >
            <i class="fa fa-cogs"></i>
        </a>
        <ul style="left:-43px;">
            <li><a class="btn_par-floating green" id="salvar-confs-panel" title="Salvar Configurações">
                    <i class="fa fa-save"></i>
                </a>
            </li>

            <li><a class="btn_par-floating red" id="carregar-confs" title="Carregar Configurações">
                    <i class="fa fa-gear"></i>
                </a>
            </li>

            <li><a class="btn_par-floating disabled" id="ordenar" title="Ordenar">
                    <i class="fa fa-sort"></i>
                </a>
            </li>

            <?php if(in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO_DESENVOLVEDOR,pegaPerfilGeral($_SESSION['usucpf']))):?>
<!--            <li><a class="btn_par-floating yellow" id="superusuario" title="Opções Superusuário">-->
<!--                    <i class="fa fa-user"></i>-->
<!--                </a>-->
<!--            </li>-->
            <?php endif;?>
        </ul>
    </div>
</form>

<br>
<br>
<br>
<div class="ibox float-e-margins animated modal" id="modal_detalhe" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="ibox-title">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span></button>
                <h5 id="titulo-modal"></h5>
            </div>
            <div class="ibox-content" id="conteudo-modal">
            </div>
            <div class="ibox-footer" id="conteudo-footer">
            </div>
        </div>
    </div>
</div>
<script type="text/javascript">
jQuery(document).ready(function () {
    $("#caixaPainel").sortable("disable");
    var sortable = false;
    $("#ordenar").on('click',function() {
        if(!sortable) {
            $("#caixaPainel").sortable("enable");
            $(this).addClass('amber');
            $(this).removeClass('disabled');
            sortable = true;
        }else {
            $("#caixaPainel").sortable("disable");
            $(this).addClass('disabled');
            $(this).removeClass('amber');
            sortable = false;
        }
    });

    $('.btn_panel').click(function() {
        if( $('#btnConfs').hasClass("active") ){
            $('.click-to-toggle').closeFAB();
            $('#btnConfs').removeClass('active');
        }else{
            $('.click-to-toggle').openFAB();
        }
    });

    jQuery('.slimScrollPainel').each(function (){
        var maxHeight = jQuery(this).attr('max-height');
        if (jQuery(this).height() >= maxHeight) {
            jQuery(this).slimScroll({
                height          : maxHeight,
                opacity         : 0.2,
                railOpacity     : 0.1,
                alwaysVisible   : false,
                railVisible     : true,
                // disableFadeOut  : false,
                allowPageScroll: false
            });
        }
    });
    });

function detalharTermos(id,event) {
    $('#modal_detalhe').find('#conteudo-modal').html('');
    $('#modal_detalhe').modal();

    var arr = [];$("input:checkbox[name='iboxtermos[intaidtermo][]']:checked").each(function(){
        arr.push($(this).val());
    });
    var inuid = $('[name=inuid]').val();
    var ano   = $('[name=ano]').val();
    console.log(inuid,ano);
    $.ajax({
        dataType: "html",
        url: '/par3/par3.php?modulo=painel/termodetalhe&acao=A',
        data: {
            descricao: event.name,
            intaidtermo : arr,
            inuid : inuid,
            ano : ano,
            id: id
        },
        beforeSend: function() {
            $('.loading-dialog-par3').show();
        },
        success: function(data) {
            $('.loading-dialog-par3').hide();
            $('#modal_detalhe').find('#conteudo-modal').html(data);
        },
        complete: function() {
            $('.loading-dialog-par3').hide();
        }
    });
}

function detalharEntidade(id,event) {
    $('#modal_detalhe').find('#conteudo-modal').html('');
    $('#modal_detalhe').modal();

    var arr = [];$("input:checkbox[name='iboxpendencias[tipid][]']:checked").each(function(){
        arr.push($(this).val());
    });
    $.ajax({
        dataType: "html",
        url: '/par3/par3.php?modulo=painel/detalharpendencias&acao=A',
        data: {
            descricao: event.name,
            tipid :arr,
            id: id
        },
        beforeSend: function() {
            $('.loading-dialog-par3').show();
        },
        success: function(data) {
            $('#modal_detalhe').find('#conteudo-modal').html(data);
        },
        complete: function() {
            $('.loading-dialog-par3').hide();
        }
    });
}

$('#carregar-confs').on('click',function(evt) {
    evt.preventDefault();
    var form = $('#form-painel').serialize();

    $.ajax({
        type:'POST',
        dataType: "html",
        url: '/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=loadconfigs&acao=A',
        data: form,
        beforeSend: function() {
            $('.loading-dialog-par3').show();
        },
        success: function(data) {
            $('#modal_detalhe').find('#conteudo-modal').html(data);
            $('#modal_detalhe').find('#titulo-modal').html('Configurações');
            // $('#modal_detalhe').find('#conteudo-footer').html();

            $('#modal_detalhe').modal();
        },
        complete: function() {
            $('.loading-dialog-par3').hide();
        }
    });
});

$('#salvar-confs-panel').on('click',function(evt) {
    evt.preventDefault();
    var form = $('#form-painel').serialize();

    $.ajax({
        type:'POST',
        dataType: "html",
        url: '/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=salvarconfs&acao=A',
        data: form,
        beforeSend: function() {
            $('.loading-dialog-par3').show();
        },
        success: function(data) {
        },
        complete: function() {
            $('.loading-dialog-par3').hide();
            toastr.options = {
                "closeButton": false,
                "debug": false,
                "newestOnTop": false,
                "progressBar": true,
                "positionClass": "toast-bottom-center",
                "preventDuplicates": false,
                "onclick": null,
                "showDuration": "300",
                "hideDuration": "1000",
                "timeOut": "1000",
                "extendedTimeOut": "1000",
                "showEasing": "swing",
                "hideEasing": "linear",
                "showMethod": "fadeIn",
                "hideMethod": "fadeOut"
            }
            toastr.success("Configurações Salvas");

        }
    });
});

$('#salvar-confs-btn').on('click',function(evt) {
    evt.preventDefault();
    var form = $('#form-confs-painel').serialize();

    $.ajax({
        type:'POST',
        dataType: "html",
        url: '/par3/par3.php?modulo=painel/rotas&acao=A&requisicao=salvarconfs&acao=A',
        data: form,
        beforeSend: function() {
            $('.loading-dialog-par3').show();
        },
        success: function(data) {
            $('.loading-dialog-par3').hide();
            msgSuccess('/par3/par3.php?modulo=painel/index&acao=A','Configurações salvas.');
            return false;
        }
    });
});

$('#modal_detalhe').on('hidden.bs.modal', function () {
    $(this).find('#conteudo-modal').html('');
    $(this).find('#titulo-modal').html('');
    $(this).find('#conteudo-footer').html('');
});

$(".remover-ibox").on('click',function () {
    var id = $(this).prop('id');
    console.log(id);
    $("[name='"+id+"[visivel]']").attr('value',false);
    $('#ibox-'+id).hide();
});

function carregarDetalheSituacaoPar(esdid) {
    var inuid = $('[name=inuid]').val();
    var ano   = $('[name=ano]').val();
    $.ajax({
        dataType: "html",
        url: '/par3/par3.php?modulo=painel/rotas&acao=A',
        data: {
            requisicao: 'detalharsituacaopar',
            esdid : esdid,
            ano   : ano,
            inuid : inuid
        },
        beforeSend: function() {
            $('.loading-dialog-par3').show();
        },
        success: function(data) {
            $('.loading-dialog-par3').hide();
            $('#modal_detalhe').modal();
            $('#modal_detalhe').find('#titulo-modal').html('Iniciativas');
            $('#modal_detalhe').find('#conteudo-modal').html(data);

        },
        complete: function(data) {
            $('.loading-dialog-par3').hide();
        }
    });
}


function detalharVigenciaTermos(id,event) {
    $('#modal_detalhe').find('#conteudo-modal').html('');
    $('#modal_detalhe').modal();
    var inuid = $('#inuid').val();
    var ano   = $('#ano').val();
    $.ajax({
        dataType: "html",
        url: '/par3/par3.php?modulo=painel/rotas&acao=A',
        data: {
            requisicao: 'detalharvigenciatermos',
            inuid: inuid,
            ano:   ano,
            id: event.id,
            descricao: event.name,
        },
        beforeSend: function() {
            $('.loading-dialog-par3').show();
        },
        success: function(data) {
            $('#modal_detalhe').find('#conteudo-modal').html(data);
        },
        complete: function() {
            $('.loading-dialog-par3').hide();
        }
    });
}
</script>