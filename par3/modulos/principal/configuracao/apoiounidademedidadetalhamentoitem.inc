<?php
include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';
$objeto = new Par3_Model_UnidadeMedidaDetalhamentoItem();
switch ($_REQUEST['action']) {
    case 'salvar':
        if(!empty($_REQUEST['pk'])){
            $objeto = new Par3_Model_UnidadeMedidaDetalhamentoItem($_REQUEST['pk']);
        }else{
            $objeto = new Par3_Model_UnidadeMedidaDetalhamentoItem();
        }
        $objeto->umidescricao  = $_REQUEST['umidescricao'];
        $objeto->umistatus  = "A";
        $objeto->salvar();
        $objeto->commit();
        break;

    case 'editar':
        $objeto = new Par3_Model_UnidadeMedidaDetalhamentoItem($_REQUEST['pk']);

        break;

    case 'inativar':
        $objeto = new Par3_Model_UnidadeMedidaDetalhamentoItem($_REQUEST['pk']);
        if($objeto->umistatus  == "I"){
            $objeto->umistatus  = "A";
        }else{
            $objeto->umistatus  = "I";
        }
        $objeto->salvar();
        $objeto->commit();
        $objeto = new Par3_Model_UnidadeMedidaDetalhamentoItem();
       // exit();
        break;

}

?>


<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3>Configuração - Unidade de Medida
                    <?php if(!empty($objeto->umidescricao)){?>
                    <small>Editando <?php echo $objeto->umidescricao?> </small>
                <?php } ?>
                </h3>
            </div>
            <?php if(!empty($objeto->umidescricao)){?>
            <div class="col-md-6">
                <a href="par3.php?modulo=principal/configuracao/apoiounidademedidadetalhamentoitem&acao=A">
                    <button class="btn btn-primary text-right right" >Inserir</button>
                </a>
            </div>

            <?php }?>
        </div>
    </div>
    <div class="ibox-content">

        <form method="post" name="formulario-filtro-chamado" id="formulario-filtro-chamado" class="form form-horizontal">
            <input type="hidden" name="action" id="action" value="salvar" />
            <input type="hidden" name="pk" id="action" value="<?php echo $objeto->umiid?>" />
            <div class="row">
                <div class="col-lg-10 col-md-10 center">
                    <?php
                    echo $simec->input('umidescricao', 'Descrição', $objeto->umidescricao,array('required'=>'required'));
                    ?>
                </div>
                <div class="text-center col-md-12">
                    <div class="center">
                        <button type="button" class="btn btn-primary" id="pesquisar-iniciativa"><i class="fa fa-search"></i>
                            Pesquisar
                        </button>
                        <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal" data-target="#modal_iniciativaDesc">
                            <i class="fa fa-plus"></i> Incluir Unidade de Medida
                        </button>
                        |
                        <button type="submit" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> XLS
                        </button>
                        <button type="button" class="btn btn-warning" style="width:110px;" id="imprimir"><i class="glyphicon glyphicon-print"></i> Imprimir
                        </button>
                    </div>
                </div>
            </div>
        </form>
    </div>
    <div class="ibox-content">
        <button type="button" class="btn btn-success" id="btn-incluir" data-toggle="modal" data-target="#modal_iniciativaDesc">
            <i class="fa fa-plus"></i> Incluir Classe de Unidade de Medida
        </button>
        <br><br>
       <?php


        $cabecalho = array( 'Nome', 'Status', 'Todos', 'Iniciativa', 'Itens');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);

        //        $listagem->turnOnPesquisator();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

        $listagem->setDados($objeto->lista());
        $listagem->addAcao('edit', 'editarItem');
        $listagem->addAcao('delete', 'inativarItem');
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
       ?>
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
    function editarItem(pk){
        window.location.href = 'par3.php?modulo=principal/configuracao/apoiounidademedidadetalhamentoitem&acao=A&action=editar&pk=' + pk;
    }

    function inativarItem(pk){
        swal({
            title: "Atenção",
            text: "Deseja mesmo inativar/ativar este item?",
            type: "warning",
            showCancelButton: true,
            confirmButtonColor: "#18a689",
            cancelButtonColor: "#ec4758",
            cancelButtonText: "Não",
            confirmButtonText: "Sim",
            closeOnConfirm: false
        }, function(){
            // @TODO Implementar de forma geral (colar isso na página do módulo que faz o include)
            window.location.href = 'par3.php?modulo=principal/configuracao/apoiounidademedidadetalhamentoitem&acao=A&action=inativar&pk=' + pk;
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