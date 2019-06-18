<?php
$sisid = $_SESSION['sisid'];
$inuid = $_REQUEST['inuid'];

$controllerChamado = new Sac_Controller_Chamado();

require APPRAIZ . 'includes/cabecalho.inc';
?>


<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3>Meus Chamados</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <a href="par3.php?modulo=principal/chamado/formulario&acao=A&inuid=<?php echo $_REQUEST['inuid'] ?>" class="btn btn-warning" style="margin-bottom: 10px;">Novo Chamado</a>
        <div style="clear: both;"></div>
        <?php echo $controllerChamado->listar($_SESSION['sisid'], array('cpf'=>$_SESSION['usucpf'])); ?>
    </div>
    <div class="ibox-footer">
        <div class="row">
            <div class="text-center">
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    // @TODO Implementar de forma geral
    function exibirChamado(chaid){
        window.location.href = 'par3.php?modulo=principal/chamado/formulario&acao=A&chaid=' + chaid;
    }
</script>