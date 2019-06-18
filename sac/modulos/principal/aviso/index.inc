<?php

include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';

$sisid = $_SESSION['sisid'];
$inuid = $_REQUEST['inuid'];

$controllerAviso = new Sac_Controller_Aviso();

switch ($_REQUEST['action']) {
    case 'exibirDestinatarios':
        ob_clean();
        echo $controllerAviso->listarDestinatarios($_REQUEST['aviid']);
        exit();
    case 'inativarAviso':
        ob_clean();
        echo $controllerAviso->inativar($_REQUEST['aviid']);
        exit();
}

?>


<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3>Comunicações</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">

        <form method="post" name="formulario-filtro-chamado" id="formulario-filtro-chamado" class="form form-horizontal">
            <input type="hidden" name="action" id="action" value="filtrar" />
            <div class="row">
                <div class="col-lg-6 col-md-6">
                    <?php
                    echo $simec->input('aviid', 'Código', $_REQUEST['aviid']);
                    echo $simec->input('usuario', 'Usuário', $_REQUEST['usuario'], array('placeholder'=>'Nome do usuário que teve qualquer interação com o chamado'));
                    echo $simec->input('texto', 'Título/Texto', $_REQUEST['texto'], array('placeholder'=>'Trecho do título ou texto da comunicação'));
                    ?>
                </div>
                <div class="col-lg-6 col-md-6">
                    <?php
                    $sql = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where sisid = $sisid order by descricao";
                    echo $simec->select('pflcod[]', 'Perfil', $_REQUEST['pflcod'], $sql);
                    $sql = "select estuf as codigo, estuf || ' - ' || estdescricao as descricao from territorios.estado order by descricao";
                    echo $simec->select('estuf[]', 'UF', $_REQUEST['estuf'], $sql);
                    $estuf = (array) $_REQUEST['estuf'];
                    $sql = "select muncod as codigo, estuf || ' - ' || mundescricao as descricao from territorios.municipio where estuf in ('" . implode("', '", $estuf) . "') order by descricao";
                    echo $simec->select('muncod[]', 'Município', $_REQUEST['muncod'], $sql);
                    ?>
                </div>
                <div class="text-center col-md-12">
                    <button type="submit" class="btn btn-warning" >Pesquisar</button>
                    <button type="submit" class="btn btn-primary" name="gerar-excel" value="1" >Gerar Excel</button>
                    <a href="par3.php?modulo=principal/chamado/suporte&acao=A" class="btn btn-danger" >Limpar Filtros</a>
                </div>
            </div>
        </form>
    </div>
    <div class="ibox-content">
        <a href="par3.php?modulo=sistema/aviso/formulario&acao=A&inuid=<?php echo $_REQUEST['inuid'] ?>" class="btn btn-warning" style="margin-bottom: 10px;">Nova Comunicação</a>
        <div style="clear: both;"></div>
        <?php echo $controllerAviso->listar($_SESSION['sisid'], $_REQUEST); ?>
    </div>
    <div class="ibox-footer">
        <div class="row">
            <div class="text-center">
            </div>
        </div>
    </div>
</div>

<!-- Modal Destinatários -->
<div class="modal fade" id="modal-aviso-destinatarios" tabindex="-1" role="dialog" aria-labelledby="modal-aviso-destinatarios-label">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="modal-aviso-1-label">Usuários</h4>
            </div>
            <div class="modal-body" id="div-body-destinatarios">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    // @TODO Implementar de forma geral (colar isso na página do módulo que faz o include)
    function exibirAviso(aviid){
        window.location.href = 'par3.php?modulo=sistema/aviso/formulario&acao=A&aviid=' + aviid;
    }

    // @TODO Implementar de forma geral (colar isso na página do módulo que faz o include)
    function exibirDestinatarios(aviid){
        $('#div-body-destinatarios').load('par3.php?modulo=sistema/aviso/index&acao=A&action=exibirDestinatarios&aviid=' + aviid, function(){
            $('#modal-aviso-destinatarios').modal();
        });
    }

    function inativarAviso(aviid){
        swal({
            title: "Atenção",
            text: "Deseja mesmo inativar este aviso?\n\nCaso queira somente parar de exibir, altere a data fim de exibição na tela de cadastro",
            type: "warning",
            showCancelButton: true,
            confirmButtonColor: "#18a689",
            cancelButtonColor: "#ec4758",
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function(){
            // @TODO Implementar de forma geral (colar isso na página do módulo que faz o include)
            window.location.href = 'par3.php?modulo=sistema/aviso/index&acao=A&action=inativarAviso&aviid=' + aviid;
        });
        return false;
    }

    $(function(){
        $('#formulario-filtro-chamado').on('change', '#estuf', function(){
            carregarSelectPorJson(
                '/publico/requisicoes.php?action=carregarMunicipio&estuf='+ $('#estuf').val(),
                '#muncod', 'muncod', 'mundescricao', null, 'Selecione');
        });
    });
</script>