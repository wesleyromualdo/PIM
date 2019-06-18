<?php

include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';


$objeto = new Par3_Model_PropostaItemComposicao();

switch ($_REQUEST['action']) {
    case 'salvar':
        if(!empty($_REQUEST['pk'])){
            $objeto = new Par3_Model_PropostaItemComposicao($_REQUEST['pk']);
        }else{
            $objeto = new Par3_Model_PropostaItemComposicao();
        }
        $objeto->picdescricao  = $_REQUEST['picdescricao'];
        $objeto->picdetalhe  = $_REQUEST['picdetalhe'];
        $objeto->pictipodespesa  = $_REQUEST['pictipodespesa'];
        $objeto->umiid  = $_REQUEST['umiid'];
        $objeto->picstatus  = "A";
        $objeto->picemendas  = $_REQUEST['picemendas'];
        $objeto->idsigarp  = NULL;
        $objeto->picpregao  = $_REQUEST['picpregao'];

        $objeto->salvar();
        $objeto->commit();
        break;
    case 'editar':
        $objeto = new Par3_Model_PropostaItemComposicao($_REQUEST['pk']);
        break;

    case 'inativar':
        $objeto = new Par3_Model_PropostaItemComposicao($_REQUEST['pk']);
        if($objeto->picstatus  == "I"){
            $objeto->picstatus  = "A";
        }else{
            $objeto->picstatus  = "I";
        }
        $objeto->salvar();
        $objeto->commit();
        $objeto = new Par3_Model_PropostaItemComposicao();
        // exit();
        break;

}

?>


<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3>Tabela de apoio - Item composição
                    <?php if(!empty($objeto->picdescricao)){?>
                        <small>Editando <?php echo $objeto->picdescricao?> </small>
                    <?php } ?>
                </h3>
            </div>
            <?php if(!empty($objeto->picdescricao)){?>
                <div class="col-md-6">
                    <a href="par3.php?modulo=principal/configuracao/apoiopropostaitemcomposicao&acao=A">
                        <button class="btn btn-primary text-right right" >Inserir</button>
                    </a>
                </div>

            <?php }?>
        </div>
    </div>
    <div class="ibox-content">

        <form method="post" name="formulario-filtro-chamado" id="formulario-filtro-chamado" class="form form-horizontal">
            <input type="hidden" name="action" id="action" value="salvar" />
            <input type="hidden" name="pk" id="action" value="<?php echo $objeto->picid?>" />
            <div class="row">
                <div class="col-lg-10 col-md-10 center">
                    <?php
                    echo $simec->input('picdescricao', 'Nome', $objeto->picdescricao,array('required'=>'required'));
                    echo $simec->textarea('picdetalhe', 'Especificação técnica', $objeto->picdetalhe, array('required'=>'required','placeHolder' => 'Especificação técnica', 'maxlengh' => 1200, $boIndicador));

                    $categoria = array(
                        "1" => "Custeio",
                        "2" => "Capital"
                    );

                    echo $simec->select('pictipodespesa', 'Categoria de despesa', $objeto->pictipodespesa, $categoria, array('required'=>'required','placeHolder' => 'Categoria de despesa', 'maxlengh' => 1200, $disabled));
                    $sql = "SELECT
                    	umiid as codigo,
                    	umidescricao as descricao
                    FROM par3.unidademedidadetalhamentoitem
                    WHERE umistatus = 'A'
                   ";
                    echo $simec->select('umiid', 'Unidade de medida', $objeto->umiid, $sql, array('required'=>'required','placeHolder' => 'Categoria de despesa', 'maxlengh' => 1200, $disabled));

                    $options = array('1' => 'Sim', '0' => 'Não');

                    echo $simec->radio('picemendas', 'Pertence também ao emendas?',$objeto->picemendas,$options , array($disabled), array( 'required'=>'required','type' => 'radio radio-info radio'));
                    echo $simec->radio('picpregao', 'É um item de pregão?',$objeto->picpregao,$options , array($disabled), array( 'required'=>'required','type' => 'radio radio-info radio'));

                    ?>
                    
                    
                </div>

                <?php if(!empty($_REQUEST['pk'])){?>
                    <div class="text-center col-md-12">
                        <button type="submit" class="btn btn-warning" >Alterar</button>

                        <a href="par3.php?modulo=principal/configuracao/apoiopropostaitemcomposicao&acao=A">
                            <div class="btn btn-primary" >Inserir Novo</div>
                        </a>
                    </div>
                <?php }else {?>

                    <div class="text-center col-md-12">
                        <button type="submit" class="btn btn-warning" >Salvar</button>
                    </div>

                <?php } ?>
            </div>
        </form>
    </div>
    <div class="ibox-content">

        <?php

        $cabecalho = array( 'Nome',"Detalhe","Tipo despesa", "Unidade do item",'Status',"Pertence também ao emendas?","É um item de pregão?");

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);

        //        $listagem->turnOnPesquisator();
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

        $listagem->setDados($objeto->recuperarLista());
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
        window.location.href = 'par3.php?modulo=principal/configuracao/apoiopropostaitemcomposicao&acao=A&action=editar&pk=' + pk;
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
            window.location.href = 'par3.php?modulo=principal/configuracao/apoiopropostaitemcomposicao&acao=A&action=inativar&pk=' + pk;
        });
        return false;
    }

</script>