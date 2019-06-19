<?php

global $db;


$pfls = arrayPerfil();

// tratamento de filtros -------------------------



// estado
$sql = "SELECT
  DISTINCT
  (m.estuf),
  e.estdescricao
FROM sase.questionarioresponsabilidade ur
  INNER JOIN territorios.municipio m ON m.muncod = ur.muncod
  INNER JOIN territorios.estado e ON e.estuf = m.estuf
WHERE ur.rpustatus = 'A'
      AND ur.usucpf = '" . $_SESSION['usucpf'] . "'
ORDER BY e.estdescricao ASC";


$estados = $db->carregar($sql);
// estado /
if (empty($estados)) {

    echo "<div class=\"alert alert-danger text-center\" >
        <strong > Atenção!</strong > Você não possui Município(s) Atribuído(s)
     </div >";
    die();
}
// mu
// municipio
$sql = "SELECT
  m.*
FROM sase.questionarioresponsabilidade ur
  INNER JOIN territorios.municipio m ON m.muncod = ur.muncod

WHERE ur.rpustatus = 'A'
      AND ur.usucpf = '" . $_SESSION['usucpf'] . "'
";
if ($_REQUEST['filtroEstado']) $sql .= " AND m.estuf = '" . $_REQUEST['filtroEstado'] . "' ";
$sql .= " order by mundescricao asc ";

$municipios = $db->carregar($sql);
if ($_GET['filtroAjax']) {
    ob_clean(); ?>
    <select name="filtroMunicipio" id="filtroMunicipio">
        <option value="">Filtrar município:</option>
        <?php
        foreach ($municipios as $chave => $municipio) { ?>
            <option value="<?= $municipio['muncod'] ?>"><?= $municipio['mundescricao'] ?></option>
        <?php } ?>
    </select>
    <?php die();
}
// municipio /

/*// situacao
$sql = "select * from sase.situacaoassessoramento where stastatus = 'A' order by stacod ASC ";
$situacoes = $db->carregar($sql);
// situacao /*/

// para lista
if ($_POST['formulario']) {


    foreach ($_POST as $chave => $valor)
        if ($valor != '')
            switch ($chave) {

                case 'filtroEstado':
                    $filtros['estuf'] = $valor;
                    break;
                case 'filtroSituacao':
                    $filtros['situacao'] = $valor;
                    break;
                case 'filtroMunicipio':
                    $filtros['muncod'] = $valor;
                    break;

            }


}
// para lista /

// exporta xls
if (isset($_POST['funcao'])) {
    switch ($_POST['funcao']) {
        case 'xls':
            $Assessoramento->montaListaXls($filtros);
//			ver($Assessoramento,d);
            exit();
    }
}


?>

<!-- ------------------------------- FORM ------------------------------- -->
<script>
    function geraRelatorioXls() {
        $('#formulario').val('1');
        $('#funcao').val('xls');
        $('#formPesquisaLista').submit();
    }
</script>
<div id="formularioFiltros">
    <form class="form-horizontal" name="formPesquisaLista" id="formPesquisaLista" role="form" method="POST">
        <input type="hidden" name="arqid" id="arqid">
        <input type="hidden" name="arqidpne" id="arqidpne" value="">
        <input type="hidden" name="arqidadesao" id="arqidadesao" value="">
        <input type="hidden" name="acao" id="acao"/>
        <input name="formulario" value="1" type="hidden"/>
        <input type="hidden" name="funcao" id="funcao" value=""/>
        <div class="form-group" id="divFiltroEstado">
            <label for="filtroEstado" class="col-sm-1 control-label">Estado:</label>
            <div class="col-sm-4 inputarea">
                <select name="filtroEstado" id="filtroEstado" class="form-control"
                        onchange="javascript:if(this.value!=''){filtraMunicipios(this.value);}">
                    <option value="">Filtrar estado</option>
                    <?php
                    foreach ($estados as $chave => $estado) { ?>
                        <option <?= ($estado['estuf'] == $_POST['filtroEstado']) ? 'selected' : '' ?>
                                value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
                    <?php } ?>
                </select>
            </div>
        </div>

        <div class="form-group" id="divFiltroMunicipio">
            <label for="filtroMunicipio" class="col-sm-1 control-label">Município:</label>
            <div class="col-sm-4 inputarea">
                <?php if (!$_POST['filtroEstado']) {
                    echo "Selecione um Estado";
                } else { ?>
                    <select name="filtroMunicipio" id="filtroMunicipio" class="form-control">
                        <option value="">Filtrar município</option>
                        <?php
                        foreach ($municipios as $chave => $municipio) { ?>
                            <option <?= ($municipio['muncod'] == $_POST['filtroMunicipio']) ? 'selected' : '' ?>
                                    value="<?= $municipio['muncod'] ?>"><?= $municipio['mundescricao'] ?></option>
                        <?php } ?>
                    </select>
                <?php } ?>
            </div>
        </div>

        <div class="form-group" id="divFiltroSituacao">
            <label for="filtroSituacao" class="col-sm-1 control-label">Situação:</label>
            <div class="col-sm-4 inputarea">
                <select name="filtroSituacao" id="filtroSituacao" class="form-control">
                    <option value="0">Filtrar situação</option>
                    <option value="1" <?= ( 1 == $_POST['filtroSituacao']) ? 'selected' : '' ?> >Não Preenchido</option>
                    <option value="2" <?= ( 2 == $_POST['filtroSituacao']) ? 'selected' : '' ?>>Preenchido e Não Finalizado</option>
                    <option value="3" <?= ( 3 == $_POST['filtroSituacao']) ? 'selected' : '' ?>>Finalizado</option>


                </select>
            </div>
        </div>


        <div class="form-group">
            <label class="col-sm-1 control-label">&nbsp;</label>
            <div class="col-sm-10">
                <input type="button" class="btn btn-primary"
                       onclick="jQuery('[name=formulario]').val('1'); jQuery('[name=\'formPesquisaLista\']').submit()"
                       value="Pesquisar"/>
                &nbsp;&nbsp;

                <!--<input type="button" class="btn btn-primary" onclick="geraRelatorioXls()" value="Exportar XLS"/>-->
            </div>
        </div>
        <hr/>
    </form>
</div>

<!-- / ------------------------------- FORM ------------------------------- -->

<?php



$sql = "SELECT
  qr.muncod,
  m.mundescricao,
  m.estuf,
  e.estdescricao,
  (SELECT  DISTINCT quersituacao
   FROM sase.questionarioresponsavel
   WHERE
     muncod = qr.muncod AND
     querstatus = 'A' limit 1) AS quersituacao
FROM sase.questionarioresponsabilidade qr
  INNER JOIN territorios.municipio m ON m.muncod = qr.muncod
  INNER JOIN territorios.estado e ON e.estuf = m.estuf
WHERE qr.rpustatus = 'A'
      AND qr.usucpf = '" . $_SESSION['usucpf'] . "'";


if (!empty($filtros['estuf'])) {
    $sql .= " and m.estuf = '" . $filtros['estuf'] . "'";
}
if (!empty($filtros['situacao'])) {


    $sql .= " and coalesce((SELECT quersituacao
                                  FROM sase.questionarioresponsavel
                                  WHERE
                                   muncod = qr.muncod AND
                                    querstatus = 'A'
                                ),1) = " . $filtros['situacao'] . " ";

}
if (!empty($filtros['muncod'])) {
    $sql .= " and m.muncod = '" . $filtros['muncod'] . "'";
}
$sql .= " order by e.estdescricao, m.mundescricao ASC";

//ver($sql,d);
$arrDados = $db->carregar($sql);


$situacao = 0;


?>

<style>
    .linkquest {
        color: #ffffff;
    }
</style>


<table width="80%" align="left" style="margin-left: 10%" border="0" cellspacing="0" cellpadding="2" class="listagem ">
    <thead>
    <tr>
        <td title="Ordenar por Ação" bgcolor=""><strong>Ação</strong></td>
        <td title="Ordenar por Estado" bgcolor=""><strong>Estado</strong></td>
        <td title="Ordenar por Município" bgcolor=""><strong>Município</strong></td>
        <td title="Ordenar por Situação" bgcolor=""><strong>Situação</strong></td>

    </tr>
    </thead>
    <tbody>
    <?php if(!empty($arrDados)):?>
    <?php foreach ($arrDados as $lista): ?>
        <?php
        if (!empty($lista['estdescricao'])) {
            $estado = $lista['estdescricao'];

        } else {


        }


            if($lista['quersituacao'] == 3){
                if(!in_array(PFLCOD_SASE_PESQUISADOR_SASE,$perfis)){
                    $disabled = "disabled";
                }
                $barra = "Finalizado<div class=\"progress progress-striped active\">
                    <div class=\"progress-bar progress-bar-success\" style=\"width:100%;color: #000;\"> </div>
                </div>";
                $acao = "<a href=\"sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A&estuf={$lista['estuf']}&muncod={$lista['muncod']}\"   style=\"color: #ffffff;\" class=\"btn btn-primary {$disabled} linkquest\"><span class=\"glyphicon glyphicon-file\"></span></a>";
            }else if( $lista['quersituacao'] == 1 ||  empty($lista['quersituacao'])){
                $barra = "Não Preenchido<div class=\"progress progress-striped active\">
                    <div class=\"progress-bar progress-bar-danger\" style=\"width:100%;color: #000;\"></div>
                </div>";
                $acao = "<a href=\"sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A&estuf={$lista['estuf']}&muncod={$lista['muncod']}\"   style=\"color: #ffffff;\" class=\"btn btn-primary linkquest\"><span class=\"glyphicon glyphicon-file\"></span></a>";
            }else if($lista['quersituacao'] == 2){
                $barra = "Preenchido e Não Finalizado<div class=\"progress progress-striped active\">
                    <div class=\"progress-bar progress-bar-warning\" style=\"width:100%;color: #000;\"> </div>
                </div>";
                $acao = "<a href=\"sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A&estuf={$lista['estuf']}&muncod={$lista['muncod']}\"   style=\"color: #ffffff;\" class=\"btn btn-primary linkquest\"><span class=\"glyphicon glyphicon-file\"></span></a>";
            }


        ?>
        <tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
            <td align="left" width="10%" title="Ação">
              <?=$acao?>
            </td>
            <td align="left" width="20%" title="Estado"><?= $estado ?></td>
            <td align="left" width="40%" title="Município"><?= $lista['mundescricao'] ?></td>

            <td align="left" width="30%" title="Situação">
                <?= $barra ?>
            </td>

        </tr>
    <?php endforeach; ?>


    <?endif;?>
    </tbody>
</table>


<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
    /**
     * Filtro de municipios com a mudanca do estado
     */
    function filtraMunicipios(estuf) {
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, filtroEstado: estuf},
            success: function (resposta) {
                jQuery('#divFiltroMunicipio .inputarea').html(resposta);
                jQuery('#filtroMunicipio').addClass('form-control');
            }
        });


    }


    $(".progress-bar-success").animate({
        width: "100%"
    }, 500 );

    $(".progress-bar-warning").animate({
        width: "50%"
    }, 500 );

    $(".progress-bar-danger").animate({
        width: "10%"
    }, 500 );
</script>
<!-- script js / -->