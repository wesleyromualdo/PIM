<?php

global $db;


$pfls = arrayPerfil();

// tratamento de filtros -------------------------
if(in_array(PFLCOD_SASE_PESQUISADOR_SASE,$perfis)){
    $pflcod = PFLCOD_SASE_PESQUISADOR_SASE;
}else{
    $pflcod = PFLCOD_SASE_SECRETARIO;
}
// estado
$sql = "SELECT ur.estuf  AS estuf, e.estdescricao AS estdescricao
					FROM sase.questionarioresponsabilidade ur
					INNER JOIN territorios.estado e ON e.estuf = ur.estuf where ur.rpustatus = 'A'
and ur.usucpf = '" . $_SESSION['usucpf'] . "' and ur.pflcod= {$pflcod} order by e.estdescricao ASC";

$estados = $db->carregar($sql);

// estado /
 if (empty($estados)) {

     echo "<div class=\"alert alert-danger text-center\" >
        <strong > Atenção!</strong > Você não possui estado(s) Atribuido(s)
     </div >";
     die();
 }
// municipio
$sql = "select * from territorios.municipio";
if ($_REQUEST['filtroEstado']) $sql .= " where estuf = '" . $_REQUEST['filtroEstado'] . "' ";
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


            }

}


// exporta xls
if (isset($_POST['funcao'])) {
    switch ($_POST['funcao']) {
        case 'xls':
            $Assessoramento->montaListaXls($filtros);
//			ver($Assessoramento,d);
            exit();
    }
}
// exporta xls /


// tratamento de filtros  ------------------------- /

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
                >
                    <option value="">Filtrar estado</option>
                    <?php
                    if (!empty($estados)) {
                        foreach ($estados as $chave => $estado) { ?>
                            <option <?= ($estado['estuf'] == $_POST['filtroEstado']) ? 'selected' : '' ?>
                                    value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
                        <?php }
                    } ?>
                </select>
            </div>
        </div>


        <div class="form-group" id="divFiltroSituacao">
            <label for="filtroSituacao" class="col-sm-1 control-label">Situação:</label>
            <div class="col-sm-4 inputarea">
                <select name="filtroSituacao" id="filtroSituacao" class="form-control">
                    <option value="0">Filtrar situação</option>
                    <option value="1" <?= (1 == $_POST['filtroSituacao']) ? 'selected' : '' ?> >Não Preenchido</option>
                    <option value="2" <?= (2 == $_POST['filtroSituacao']) ? 'selected' : '' ?>>Preenchido e Não
                        Finalizado
                    </option>
                    <option value="3" <?= (3 == $_POST['filtroSituacao']) ? 'selected' : '' ?>>Finalizado</option>

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

<style>
    .linkquest {
        color: #ffffff;
    }
</style>
<table width="60%" align="left" style="margin-left: 10%" border="0" cellspacing="0" cellpadding="2" class="listagem ">
    <thead>
    <tr>
        <td title="Ordenar por Ação" bgcolor=""><strong>Ação</strong></td>
        <td title="Ordenar por Estado" bgcolor=""><strong>Estado</strong></td>

        <td title="Ordenar por Situação" bgcolor=""><strong>Situação</strong></td>

    </tr>
    </thead>
    <tbody>
<?php

$perfis = pegaPerfilGeral($_SESSION['usucpf']);

if(in_array(PFLCOD_SASE_PESQUISADOR_SASE,$perfis)){
   $verificausuario = '<>';
}else{
    $verificausuario = '=';
}

foreach ($estados as $estado) {


$sql = "SELECT
  qr.estuf,
  qr.muncod,
  e.estuf,
  e.estdescricao,
  (SELECT DISTINCT quersituacao
   FROM sase.questionarioresponsavel
   WHERE
     estuf = qr.estuf AND
     querstatus = 'A'  limit 1 ) AS quersituacao
FROM sase.questionarioresponsabilidade qr
  INNER JOIN territorios.estado e ON e.estuf = qr.estuf
WHERE qr.rpustatus = 'A' and e.estuf = '{$estado["estuf"]}'
      AND qr.usucpf = '" . $_SESSION['usucpf'] . "'
      and qr.pflcod = {$pflcod}
      ";


if (!empty($filtros['estuf'])) {
    $sql .= " and qr.estuf = '" . $filtros['estuf'] . "'";
}


if (!empty($filtros['situacao'])) {


    $sql .= " and coalesce((SELECT quersituacao
                      FROM sase.questionarioresponsavel
                      WHERE
                        estuf = qr.estuf AND
                        querstatus = 'A'
                    ),1) = " . $filtros['situacao'] . " ";

}

$sql .= " order by e.estdescricao ASC";



$arrDados = $db->carregar($sql);

//ver($arrDados);
$situacao = 0;
?>


    <?php if (!empty($arrDados)): ?>
        <?php foreach ($arrDados as $lista): ?>
            <?php
            if (!empty($lista['estdescricao'])) {
                $estado = $lista['estdescricao'];

            } else {


            }


            if ($lista['quersituacao'] == 3) {
                $barra = "Finalizado<div class=\"progress progress-striped active\">
                    <div class=\"progress-bar progress-bar-success\" style=\"width:100%;color: #000;\"> </div>
                </div>";

                if(!in_array(PFLCOD_SASE_PESQUISADOR_SASE,$perfis)){
                    $disabled = "disabled";
                }


                $acao = "<a href=\"sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A&estuf={$lista['estuf']}&quersituacao={$lista['quersituacao']}\"   style=\"color: #ffffff;\" class=\"btn btn-primary {$disabled} linkquest\"><span class=\"glyphicon glyphicon-file\"></span></a>";
            } else if ($lista['quersituacao'] == 1 || empty($lista['quersituacao'])) {
                $barra = "Não Preenchido<div class=\"progress progress-striped active\">
                    <div class=\"progress-bar progress-bar-danger\" style=\"width:100%;color: #000;\"></div>
                </div>";
                $acao = "<a href=\"sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A&estuf={$lista['estuf']}\"   style=\"color: #ffffff;\" class=\"btn btn-primary linkquest\"><span class=\"glyphicon glyphicon-file\"></span></a>";
            } else if ($lista['quersituacao'] == 2) {
                $barra = "Preenchido e Não Finalizado<div class=\"progress progress-striped active\">
                    <div class=\"progress-bar progress-bar-warning\" style=\"width:100%;color: #000;\"> </div>
                </div>";
                $acao = "<a href=\"sase.php?modulo=principal/sistemasdeensino/diagnostico/questionariosase&acao=A&estuf={$lista['estuf']}\"   style=\"color: #ffffff;\" class=\"btn btn-primary linkquest\"><span class=\"glyphicon glyphicon-file\"></span></a>";
            }


            ?>
            <tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
                <td align="left" width="30%" title="Ação">
                    <?= $acao ?>
                </td>
                <td align="left" width="40%" title="Estado"><?= $estado ?></td>

                <td align="left" width="30%" title="Situação">
                    <?= $barra ?>
                </td>

            </tr>
        <?php endforeach; ?>


    <? endif; ?>

    </tbody>
    <?php } ?>
</table>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>


    $(".progress-bar-success").animate({
        width: "100%"
    }, 500);

    $(".progress-bar-warning").animate({
        width: "50%"
    }, 500);

    $(".progress-bar-danger").animate({
        width: "10%"
    }, 500);

</script>
<!-- script js / -->