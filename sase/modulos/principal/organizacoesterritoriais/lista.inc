<?php
// --------------- Dependências
include_once '../../sase/classes/Territorio.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


global $db;
$Territorio = new Territorio();

// tratamento de filtros -------------------------
// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
$estPre = false;
if (empty($estuf)) {
    if (!$_REQUEST['filtroEstado']) {
        $estuf = '';
    } else {
        $estuf = $_REQUEST['filtroEstado'];
    }
} else {
    $estPre = true;
}
//ver($_POST);
echo "<script src=\"/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js\"></script>";

// estado /
// mesorregião
$sql = "select distinct mes.mesid, mes.mesdsc 
          from sase.mesoregiao mes 
         inner join sase.orgaoestado oes 
            on oes.oesid = mes.oesid
         inner join sase.orgao org 
            on org.orgid = oes.orgid";
$sql .= " where 1 = 1 ";
if ($estuf) {
    $sql .= " and oes.estuf = '" . $estuf . "' ";
}
//ver($_REQUEST);
if ($_REQUEST['orgid']) {
    $sql .= " and org.orgid = '" . $_REQUEST['orgid'] . "' ";
}
if ($_REQUEST['filtroTerStatus']) {
    $sql .= " where org.orgstatus = '{$_REQUEST['filtroTerStatus']}'";
}
$sql .= " order by mesdsc asc ";

$mesoregioes = $db->carregar($sql);
if ($_REQUEST['filtroAjax'] && $_REQUEST['filtroMesorregiao']) {
    ob_clean();
    ?>
    <select name="filtroMesdsc" id="filtroMesdsc" class="form-control"
            onchange="javascript:if (this.value != '') {filtraMunicipiosMesorregiao(this.value);}">
        <option value="">Selecione</option>
        <?php if ($mesoregioes) {
            foreach ($mesoregioes as $chave => $meso) { ?>
                <option value="<?= $meso['mesid'] ?>"><?= $meso['mesdsc'] ?></option>
            <?php }
        } ?>
    </select>
    <?php
    die();
}

$sqlorg = "SELECT 		 DISTINCT org.orgid, orgdsc 
		   FROM   		 sase.orgao org
           LEFT join 	 sase.orgaoestado oe on oe.orgid = org.orgid
		   WHERE		 org.orgstatus = 'A'
           ORDER BY 	 orgdsc ASC";

$rowOrgaos = $db->carregar($sqlorg);
if ($_GET['filtroAjax'] && $_GET['filtroOrgao']) {
    ob_clean(); ?>
    <select name="orgid" id="orgid" class="form-control"
            onchange="javascript:if (this.value != '') {filtrarMesorregiao(this.value);}">
        <option value="">Selecione</option>
        <?php if ($rowOrgaos) {
            foreach ($rowOrgaos as $chave => $org) { ?>
                <option value="<?= $org['orgid'] ?>"><?= $org['orgdsc'] ?></option>
            <?php }
        } ?>
    </select>
    <?php
    die();
}
// mesorregião

// municipio
$sql = "select distinct mun.muncod,mun.mundescricao from territorios.municipio mun ";
if ($estuf && !$_REQUEST['filtroMesdsc']) {
    $sql .= " where estuf = '" . $estuf . "' ";
}
if ($estuf && $_REQUEST['filtroMesdsc']) {
    $sql .= " inner join sase.territorio sat on sat.muncod = mun.muncod
                  inner join sase.mesoregiao mes on sat.mesid = mes.mesid
                  where mes.mesid = '{$_REQUEST['filtroMesdsc']}' and estuf = '" . $estuf . "' ";
}
if (!$estuf && $_REQUEST['filtroMesdsc']) {
    $sql .= " inner join sase.territorio sat on sat.muncod = mun.muncod
                  inner join sase.mesoregiao mes on sat.mesid = mes.mesid
                  where mes.mesid = '{$_REQUEST['filtroMesdsc']}'";
}

$sql .= " order by mundescricao asc ";

$municipios = $db->carregar($sql);

if ($_GET['filtroAjax'] && $_GET['filtroMunicipio']) {
    ob_clean();
    ?>
    <select name="filtroMunicipios[]" id="filtroMunicipios"  multiple="multiple" class="form-control">

        <?php if ($municipios) {
            foreach ($municipios as $chave => $municipio) { ?>
                <option value="<?= $municipio['muncod'] ?>"><?= $municipio['mundescricao'] ?></option>
            <?php }
        } ?>
    </select>
    <script>
        $(function () {
            $('#filtroMunicipios').multiselect({
                includeSelectAllOption: true,
                enableFiltering: true,
                maxHeight: 300,
            });
        });
    </script>
    <?php
    die();
}

// municipio /
// mesoregiao
//$sql = " select * from sase.mesoregiao order by mesdsc asc ";
//$mesoregioes = $db->carregar($sql);
// mesoregiao /
// para lista

if ($estPre) {
    $filtros['e.estuf'] = $estuf;
}
if ($_POST['formulario']) {


    foreach ($_POST as $chave => $valor) {
        if ($valor != '') {
            switch ($chave) {
                case 'filtroMesdsc':
                    $filtros['mes.mesid'] = $valor;
                    break;
                case 'filtroMunicipios':
                    $retirar = in_array('multiselect-all' ,$valor );
                    if($retirar){
                        unset($valor[0]);
                    }
                    $filtros['t.muncod'] = implode("','", $valor);
                    break;
                case 'filtroTerstatus':
                    $filtros['t.terstatus'] = $valor;
                    break;
                case 'filtroEstado':
                    $filtros['e.estuf'] = $valor;
                    break;
                case 'orgid':
                    $filtros['o.orgid'] = $valor;
                    break;
            }
        }
    }
}
// exporta xls
if (isset($_POST['funcao'])) {
    switch ($_POST['funcao']) {
        case 'xls':
            $Territorio->montaListaXls($filtros);
            exit();
    }
}
// exporta xls /
// para lista /
// para exclusao
if ($_GET['excluir']) {
    if ($Territorio->excluirTerritorio($_GET['terid'])) {
        echo "
			<script>
				alert('Território excluído com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A';
			</script>";
        exit;
    }
}
// para exclusao /
// tratamento de filtros ------------------------- /
?>

<script>
    function geraXLS() {
        $('#formulario').val('1');
        $('#funcao').val('xls');
        $('#formPesquisaLista').submit();
        $('#formulario').val('');
        $('#funcao').val('');
    }

    function limparCampos() {

        $('#orgid').prop('selectedIndex', 0);
        $('#filtroEstado').prop('selectedIndex', 0);
        $('#filtroTerstatus').prop('selectedIndex', 0);
        $("input[type='checkbox']").each(function() {
            $(this).prop("checked", false);
        });
        $('#filtroTerstatus').prop('selectedIndex', 0);
    }

</script>
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<div id="container">

    <!-- ------------------------------- FORM ------------------------------- -->

    <div id="formularioFiltros">
        <form class="form-horizontal" name="formPesquisaLista" id="formPesquisaLista" role="form" method="POST">
            <input name="formulario" value="1" type="hidden"/>
            <input type="hidden" name="funcao" id="funcao" value=""/>
            <div class="form-group" id="divFiltroOrgao">
                <label for="orgid" class="col-sm-3 control-label">Tipo de Organização:</label>
                <div class="col-sm-8 inputarea">
                    <select name="orgid" id="orgid" class="form-control" >
                        <option value="">Selecione</option>
                        <?php
                        if (!empty($rowOrgaos)) {
                            foreach ($rowOrgaos as $chave => $org) { ?>
                                <option <?= ($org['orgid'] == $_POST['orgid']) ? 'selected' : '' ?>
                                        value="<?= $org['orgid'] ?>"><?= $org['orgdsc'] ?></option>
                                <?php
                            }
                        }
                        ?>
                    </select>
                </div>
            </div>

            <div class="form-group" id="divFiltroEstado">
                <label for="filtroEstado" class="col-sm-3 control-label">UF:</label>
                <div class="col-sm-8 inputarea">
                    <?php $dsb = $estPre ? "disabled" : ""; ?>
                    <select <?= $dsb; ?> name="filtroEstado" id="filtroEstado" class="form-control" onchange="javascript: if (this.value != '') {
                                                                                            filtraMunicipios(this.value);

                                                                                        }">
                        <option value="">Selecione</option>
                        <?php if ($estados) {
                            foreach ($estados as $chave => $estado) {
                                ?>
                                <option <?= ($estado['estuf'] == $estuf) ? 'selected' : '' ?>
                                        value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
                            <?php }
                        } ?>
                    </select>
                </div>
            </div>

            <div class="form-group" id="divFiltroMunicipio">
                <label for="filtroMunicipio" class="col-sm-3 control-label">Município:</label>
                <div class="col-sm-8 inputarea">
                    <?php if (!$estuf) {
                        echo "<select  class=\"form-control\" ><option value=\"\">Selecione um estado</option></select>";
                    } else { ?>
                        <select name="filtroMunicipios[]"  class="form-control" multiple="multiple" id="filtroMunicipios">

                            <?php if ($municipios) {
                                foreach ($municipios as $chave => $municipio) { ?>
                                    <option <?= ($municipio['muncod'] == $_POST['filtroMunicipio']) ? 'selected' : '' ?>
                                            value="<?= $municipio['muncod'] ?>"><?= $municipio['mundescricao'] ?></option>
                                <?php }
                            } ?>
                        </select>
                    <?php } ?>
                </div>
            </div>

            <div class="form-group" id="divFiltroSituacao">
                <label for="filtroTerstatus" class="col-sm-3 control-label">Status:</label>
                <div class="col-sm-8 inputarea">
                    <select name="filtroTerstatus" class="form-control" id="filtroTerstatus">
                        <option value="">Selecione</option>
                        <option <?= ($_REQUEST['filtroTerstatus'] == 'A') ? 'selected' : '' ?> value="A">Ativo</option>
                        <option <?= ($_REQUEST['filtroTerstatus'] == 'I') ? 'selected' : '' ?> value="I">Inativo
                        </option>
                    </select>
                </div>
            </div>
            <div class="form-group">
                <label class="col-sm-3 control-label">&nbsp;</label>
                <div class="col-sm-8">
                    <input type="button" class="btn btn-primary"
                           onclick="jQuery('[name=\'formPesquisaLista\']').submit()" value="Pesquisar"/>
                    &nbsp;&nbsp;&nbsp;
                    <a href="/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao"
                       class="btn btn-primary">Novo Território</a>
                    &nbsp;&nbsp;&nbsp;
                    <input type="button" class="btn btn-primary" onclick="geraXLS()" value="Exportar XLS"/>
                </div>
            </div>
        </form>
    </div>

    <!-- / ------------------------------- FORM ------------------------------- -->

    <?php
    $cabecalho = array("Ação", "Tipo de Organização", "Nome Território", "Total de Municípios", "Status");
    $alinhamento = array('center', 'left', 'left', 'left', 'left');
    $larguras = array('5%', '25%', '25%', '25%', '10%');

    $db->monta_lista($Territorio->montaListaQuery($filtros), $cabecalho, 30, 5, 'N', '', 'N', 'listaTerritorio',
        $larguras, $alinhamento);
    ?>

</div>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
    //      jQuery(document).ready(function() {
    //        if(jQuery('#filtroEstado').val()){
    //            filtrarMesorregiao(jQuery('#filtroestado').val());
    //        }
    //       });
    function filtraOrgaoEstado(estuf, orgid) {
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {
                filtroAjax: true,
                filtroEstado: estuf,
                filtroOrgao: true,
                orgid: orgid},
            success: function (resposta) {
                jQuery('#divFiltroOrgao .inputarea').html(resposta);
            }
        });

    }
    /**
     * Filtro de municipios com a mudanca do estado
     */
    function filtraMunicipios(estuf) {
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, filtroEstado: estuf, filtroMunicipio: true},
            success: function (resposta) {
                jQuery('#divFiltroMunicipio .inputarea').html(resposta);
            }
        });
    }

    /**
     * Filtro de municipios com a mudanca do estado
     */
    function filtraMunicipiosMesorregiao(mesorregiao) {
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, filtroMesdsc: mesorregiao, filtroMunicipio: true},
            success: function (resposta) {
                jQuery('#divFiltroMunicipio .inputarea').html(resposta);
            }
        });
    }

    function filtrarMesorregiao(orgid) {
        var estuf = $("#filtroEstado").val();
        if (!orgid) orgid = 0;
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, filtroEstado: estuf, filtroMesorregiao: true, filtroOrgao: true, orgid: orgid},
            success: function (resposta) {
                jQuery('#divFiltroTerritorio .inputarea').html(resposta);
            }
        });
    }

    /**
     * Manda para edicao de questoespontuais
     */
    function editarTerritorio(infterid) {
        window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao&infterid=' + infterid;
    }

    /**
     * Excluir logicamente questoespontuais
     */
    function excluirTerritorio(terid) {
        if (confirm("Deseja realmente inativar essa Situação de Questões Pontuais?")) {
            window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&excluir=1&terid=' + terid;
        }
    }

    $(function () {

        $('#filtroMunicipios').multiselect({
            includeSelectAllOption: true,
            enableFiltering: true,
            maxHeight: 300,
        });
    });

    function deletaOrganizacaoTerritorio(infterid) {
        $.ajax({
            url: '',
            type: 'POST',
            data: {
                deletaOrganizacaoTerritorio: true,
                infterid:infterid
            },
            success: function (resposta) {
                window.location.reload();
                if (resposta = 'true'){
                    alert('Deletado com Sucesso');
                    return false;
                }
            }
        });
    }

</script>
<!-- script js / -->