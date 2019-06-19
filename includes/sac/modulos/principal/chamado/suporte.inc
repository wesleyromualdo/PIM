<?php
$sisid = $_SESSION['sisid'];
$inuid = $_REQUEST['inuid'];

$controllerChamado = new Sac_Controller_Chamado();

switch($_REQUEST['action']){
    case 'carregarMunicipio':
        $controllerTerritorios = new Territorios_Controller_Territorios();
        ob_clean();
        echo $controllerTerritorios->recuperarMunicipiosPorUf($_REQUEST['estuf']);
        exit();
}

require APPRAIZ . 'includes/cabecalho.inc';
?>


<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3>Chamados</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">

        <form method="post" name="formulario-filtro-chamado" id="formulario-filtro-chamado" class="form form-horizontal">
            <input type="hidden" name="action" id="action" value="filtrar" />
            <div class="row">
                <div class="col-lg-6 col-md-6">
                    <?php
                    echo $simec->input('chaid', 'Código', $_REQUEST['chaid']);
                    echo $simec->select('chastatus[]', 'Situação', $_REQUEST['chastatus'], $controllerChamado->getSituacoes());
                    $sql = "select catid as codigo, catdsc as descricao from sac.categoria order by catordem";
                    $sql = "select estuf as codigo, estuf || ' - ' || estdescricao as descricao from territorios.estado order by descricao";
                    echo $simec->select('estuf[]', 'UF', $_REQUEST['estuf'], $sql);

                    $estuf = (array) $_REQUEST['estuf'];
                    $sql = "select muncod as codigo, estuf || ' - ' || mundescricao as descricao from territorios.municipio where estuf in ('" . implode("', '", $estuf) . "') order by descricao";
                    echo $simec->select('muncod[]', 'Município', $_REQUEST['muncod'], $sql);
                    ?>
                </div>
                <div class="col-lg-6 col-md-6">
                    <?php
                    echo $simec->data('chadataabertura', 'Data de Abertura', $_REQUEST['chadataabertura']);
                    echo $simec->input('usuario', 'Usuário', $_REQUEST['usuario'], array('placeholder'=>'Nome do usuário que teve qualquer interação com o chamado'));
                    $sql = "select catid as codigo, catdsc as descricao from sac.categoria order by catordem";
                    echo $simec->select('catid[]', 'Tipo de Ocorrência', $_REQUEST['catid'], $sql);
                    $sql = "select areid as codigo, aredsc as descricao from sac.area order by areordem";
                    echo $simec->select('areid[]', 'Onde ocorreu', $_REQUEST['areid'], $sql);
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
        <div style="clear: both;"></div>
        <?php echo $controllerChamado->listar(null, $_REQUEST); ?>
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

    $(function(){
        $('#formulario-filtro-chamado').on('change', '#estuf', function(){
            carregarSelectPorJson(
                'par3.php?modulo=principal/chamado/suporte&acao=A&action=carregarMunicipio&estuf='+ $('#estuf').val(),
                '#muncod', 'muncod', 'mundescricao', null, 'Selecione');
        });
    });

</script>