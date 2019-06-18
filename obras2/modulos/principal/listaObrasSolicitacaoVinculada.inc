<?php
$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
if (!in_array($_SESSION['obras2']['orgid'], $arOrgid)) {
    $_SESSION['obras2']['orgid'] = '';
}
$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current($arOrgid));
$orgid = $_SESSION['obras2']['orgid'];

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';

switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=utf-8');

        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";


monta_titulo('Lista Pedido de Criação de Obra Vinculada', 'Filtre as Obras');

extract($_POST);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<form method="post" name="formListaObraSolicitacaoVinculada" id="formListaObraSolicitacaoVinculada">
    <input type="hidden" name="req" id="req" value="">
    <input type="hidden" name="obrid" id="obrid" value="">
    <input type="hidden" name="empid" id="empid" value="">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
            <td>
                <?= campo_texto('obrbuscatexto', 'N', 'S', '', 70, 100, '', '', '', '', '', 'id="obrbuscatexto"'); ?>
            </td>
        </tr>
<!--        <tr>-->
<!--            <td class="SubTituloDireita">Esfera:</td>-->
<!--            <td>-->
<!--                --><?php
//                $sql = Array(Array('codigo' => 'E', 'descricao' => 'Estadual'),
//                    Array('codigo' => 'M', 'descricao' => 'Municipal'));
//                $db->monta_combo('empesfera', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'empesfera');
//                ?>
<!--            </td>-->
<!--        </tr>-->
<!--        <tr>-->
<!--            <td class="SubTituloDireita">UF:</td>-->
<!--            <td>-->
<!--                --><?php
//                $uf = new Estado();
//                $db->monta_combo("estuf", $uf->listaCombo(), 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'N', 'estuf');
//                ?>
<!--            </td>-->
<!--        </tr>-->
        <tr>
            <td class="SubTituloDireita">Município:</td>
            <td id="td_municipio">
                <select name="muncod[]" id="muncod" class="chosen-select municipios" multiple data-placeholder="Selecione">
                    <?php   $municipio = new Municipio();
                    foreach ($municipio->listaComboMulti() as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Situação:</td>
            <td>
                <?php
                criaComboWorkflow(TPDID_SOLICITACAO_VINCULADA, array("nome" => "esdidsituaco", "id" => "esdidsituacao"));
                ?>
            </td>
        </tr>
        <tr>
            <td class="divTituloPadrao" width="15%" colspan="2" align="center">
                <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar" id="pesquisar"  onclick="validaFiltro(this);"/>
                <input type="button" name="xls" class="xls" value="Gerar Excel" id="xls" onclick="validaFiltro(this);" />
            </td>
        </tr>
    </table>
</form>


<?php

if ($obrbuscatexto) {
    $where[] = " ( UPPER(public.removeacento(o.obrnome)) ILIKE ('%" . removeAcentos(trim($obrbuscatexto)) . "%') OR
				   public.removeacento(o.obrid::CHARACTER VARYING) ILIKE ('%" . removeAcentos(trim($obrbuscatexto)) . "%') ) ";
}

//if ($empesfera) {
//    $empesfera = (array)$empesfera;
    $where[] = "emp.empesfera IN('M')";
//}

//if ($estuf) {
//    $estuf = (array)$estuf;
    $where[] = "m.estuf IN('SP')";
//}

//if ($muncod) {
if ($muncod && $muncod[0] != ''){
//    $muncod = (array)$muncod;
    $where[] = "m.muncod IN('" . implode("', '", $muncod) . "')";
}

if ($esdidsituaco) {
    $where[] = " d.esdid = $esdidsituaco ";
}

$acao = '';
if ($_REQUEST['req'] != 'xls') {
    $acao = "'<center>
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/alterar.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: abreSolicitacao(' || sv.slvid  || ',' ||  o.obrid || ');\"
                        title=\"Alterar Obra\">
                 </center>' AS acao,";
}

if (isset($_POST['req']) && !empty($_POST['req'])) {

    $arrFiltros = array(
        $obrbuscatexto = $_POST['obrbuscatexto'],
        $empesfera = $_POST['empesfera'],
        $estuf = $_POST['estuf'],
        $muncod = $_POST['muncod'],
        $esdidsituacao = $_POST['esdidsituaco']
    );

    $ocorrenciasFiltro = 0;
    $filtrosQuatidade = count($arrFiltros);

    for ($i = 0; $i < $filtrosQuatidade; $i++) {

        if ($arrFiltros[$i] != '') {
            $ocorrenciasFiltro++;
        }
    }

    if ($ocorrenciasFiltro > 0) {
        $sql = "
    SELECT * FROM (


            SELECT
                $acao
                o.obrid,
                o.obrnome,
                m.estuf,
                m.mundescricao,
                sv.slvjustificativa,
                u.usunome usunome1,
                sv.slvpercnovocontrato,
                TO_CHAR(sv.slvdatainclusao, 'DD/MM/YYYY') slvdatainclusao,
                e.esddsc,
                (
                    SELECT ud.usunome FROM workflow.historicodocumento h
                    LEFT JOIN workflow.comentariodocumento  c ON c.hstid = c.hstid AND c.docid = d.docid
                    LEFT JOIN seguranca.usuario ud ON ud.usucpf = h.usucpf
                    WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as usunome,
                (
                    SELECT TO_CHAR(h.htddata, 'DD/MM/YYYY') FROM workflow.historicodocumento h WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as htddata,
                (
                    SELECT c.cmddsc FROM workflow.historicodocumento h
                    LEFT JOIN workflow.comentariodocumento  c ON c.hstid = c.hstid AND c.docid = d.docid
                    WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as cmddsc
            FROM obras2.solicitacao_vinculada sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
            JOIN obras2.empreendimento emp ON emp.empid = o.empid
            JOIN entidade.endereco ed ON ed.endid = o.endid
            JOIN territorios.municipio m ON m.muncod = ed.muncod
            JOIN seguranca.usuario u ON u.usucpf = sv.usucpf
            JOIN workflow.documento d ON d.docid = sv.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid
            WHERE sv.slvstatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ', $where) : "") . "

            ) as f ORDER BY 1
            ";

        $cabecalho = array('Ação', 'ID', 'Obra', 'UF', 'Município', 'Justificativa', 'Inserido Por', 'Percentual', 'Data de Cadastro',
            'Situação do Deferimento', 'Inserido Por', 'Data de Cadastro do Deferimento', 'Observação');
        
        if ($_REQUEST['req'] == 'xls') {
            $cabecalho = array('ID', 'Obra', 'UF', 'Município', 'Justificativa', 'Inserido Por', 'Percentual', 'Data de Cadastro',
                'Situação do Deferimento', 'Inserido Por', 'Data de Cadastro do Deferimento', 'Observação');
            $db->sql_to_xml_excel($sql, 'relatorioPedidoCriacaoVinculada', $cabecalho, '');
        } else {
            $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', null, "formulario");
        }
    }else{
        echo '<script>alert("Para realizar a pesquisa deve ser preenchido ao menos um filtro");</script>';
    }

}

?>
<script type="text/javascript">
    function validaFiltro(botao) {

        var obrbuscatexto = document.getElementById('obrbuscatexto').value;
        // var empesfera = document.getElementById('empesfera').value;
        // var estuf = document.getElementById('estuf').value;
        var muncod = $('#muncod').val();
        var esdidsituacao = document.getElementById('esdidsituacao').value;

        var botao = botao.id;

        // var filtros =
        //     [obrbuscatexto,
        //         empesfera,
        //         estuf,
        //         muncod,
        //         esdidsituacao];

        var filtros =
            [obrbuscatexto,
                muncod,
                esdidsituacao];

        var filtrosQuatidade = filtros.length;

        var ocorrenciasFiltro = 0;
        for (i = 0; i < filtrosQuatidade; i++) {

            if (filtros[i] != '') {
                ocorrenciasFiltro++;
            }
        }


        if (ocorrenciasFiltro > 0) {

            if(botao == 'pesquisar'){
                document.getElementById('req').value = 'pesquisar';
                document.formListaObraSolicitacaoVinculada.submit();
            }else if(botao == 'xls'){
                document.getElementById('req').value = 'xls';
                document.formListaObraSolicitacaoVinculada.submit();
            }

        } else {
            alert('Para realizar a pesquisa deve ser preenchido ao menos um filtro.');
            return false;
        }

    }

    $(document).ready(function () {
        // $('.pesquisar').click(function (){
        // 	$('#req').val('');
        // 	$('#formListaObraSolicitacaoVinculada').submit();
        // });

    });

    function abreSolicitacao(slvid, obrid) {
        windowOpen('?modulo=principal/popupSolicitarVinculada&acao=A&slvid=' + slvid + '&obrid=' + obrid, 'telaSolicitacaoVinculada', 'height=700,width=1200,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    // function carregarMunicipio(estuf) {
    //     var td = $('#td_municipio');
    //     if (estuf != '') {
    //         var url = location.href;
    //         $.ajax({
    //             url: url,
    //             type: 'post',
    //             data: {
    //                 ajax: 'municipio',
    //                 estuf: estuf
    //             },
    //             dataType: "html",
    //             async: false,
    //             beforeSend: function () {
    //                 divCarregando();
    //                 td.find('select option:first').attr('selected', true);
    //             },
    //             error: function () {
    //                 divCarregado();
    //             },
    //             success: function (data) {
    //                 td.html(data);
    //                 divCarregado();
    //             }
    //         });
    //     } else {
    //         td.find('select option:first').attr('selected', true);
    //         td.find('select').attr('selected', true)
    //             .attr('disabled', true);
    //     }
    // }

    //-->
</script>


<script>
    $1_11(document).ready(function () {
        $1_11('select[name="muncod[]"]').chosen();
    });


</script>