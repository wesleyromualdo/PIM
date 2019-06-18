<?php
include_once APPRAIZ . "includes/workflow.php";
if($_REQUEST['requisicao'] == 'detalhependencia') {
    switch($_REQUEST['pendencia']){
        case 'estudodemanda':
            ob_clean();
            echo $mdObra->detalharPendenciaDadosTerreno($_GET['obrid']);die;
            break;
        case 'vistoria':
            ob_clean();
            echo $mdObra->detalharPendenciaDadosTerreno($_GET['obrid']);die;
            break;
        case 'dadosterreno':
            ob_clean();
            $arrDadosTerreno =  $mdObra->detalharPendenciaDadosTerreno($_GET['obrid']);
            foreach ($arrDadosTerreno as $arrDado) {
                echo '<p><i class="fa fa-circle"></i> '.$arrDado.'</p>';
            }
            die;
            break;
        case 'documentosanexos':
            ob_clean();
            echo $mdObra->detalharPendenciaDadosTerreno($_GET['obrid']);die;
            break;
        case 'fotosterreno':
            ob_clean();
            echo $mdObra->detalharPendenciaFotosTerreno();die;
            break;
        case 'planilhaorcamentaria':
            ob_clean();
            echo $mdObra->detalharPendenciaDadosTerreno($_GET['obrid']);die;
            break;
        default;
        break;
    }
}
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Enviar para Análise</h3>
                <h3 class="center"><?= (empty($_GET['obrid']) ? '' : "Pré-Obra ID: {$_GET['obrid']}") ?></h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="col-lg-10">
                <h3 class="left">Pendências</h3>
                <?php

                $arrPendencia = $mdObra->verificarPendencias($_GET['obrid']);
                ?>
                <table class="table table-bordered">
                    <thead>
                    <tr>
                        <th class="col-lg-1">Item</th>
                        <th class="col-lg-10">Descrição</th>
                        <th class="col-lg-1">Ajuda</th>
                    </tr>
                    </thead>
                    <tbody>
                    <?php $item = 1;
                    foreach ($arrPendencia as $pendencia): ?>
                        <tr>
                            <td><b><?php echo $item++ ?></b></td>
                            <td><b><?php echo $pendencia['descricao']; ?></b></td>
                            <td class="center">
                                <a title="Pendência <?php echo $pendencia['descricao']; ?>" data-toggle="modal" data-target="#myModal" id="<?php echo $pendencia['label']?>" class="detalhependencia">
                                    <i class="fa fa-book" aria-hidden="true"></i>
                                </a>
                            </td>
                        </tr>
                    <?php endforeach; ?>
                    </tbody>
                </table>
            </div>
            <div class="col-lg-2">
                <?php echo wf_desenhaBarraNavegacaoBootstrap($mdObra->docid, array(), array('historico' => true), 'Etapa'); ?>
            </div>
        </div>
    </div>
</div>
<!-- Modal -->
<div id="modalpendencia" class="modal fade" role="dialog">
    <div class="modal-dialog">
        <!-- Modal content-->
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">&times;</button>
                <h4 class="modal-title">Pendência</h4>
            </div>
            <div class="modal-body">
                <p>Some text in the modal.</p>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
            </div>
        </div>

    </div>
</div>
<script>
    $('.detalhependencia').click(function(){
        console.log($(this).attr('id'));
        $('#modalpendencia').find('.modal-body').load(window.location.href+'&requisicao=detalhependencia&pendencia='+$(this).attr('id'));
        $('#modalpendencia').modal('show');
    });
</script>