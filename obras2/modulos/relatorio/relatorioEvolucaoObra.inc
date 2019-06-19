<?php

ini_set('memory_limit','1024M');

$dir_relatorios = APPRAIZ . 'arquivos/obras2/relatorio/evolucaomi';
if($_GET['download']){
    if(file_exists($dir_relatorios . '/' . $_GET['download'])) {
        header("Content-Type: application/vnd.ms-excel; charset=ISO-8859-1");
        header("Content-Disposition: inline; filename=\"" . $_GET['download'] . "\"");
        echo file_get_contents($dir_relatorios . '/' . $_GET['download']);
    }
    exit;
}

function monta_sql() {
    global $db;

    extract($_REQUEST);

    $where = array();

    if (!empty($obrid)) {
        $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($obrid))));
        $where[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                                    o.obrid::CHARACTER VARYING ILIKE ('%" . $obrid . "%') ) ";
    }

    if (!empty($muncod)) {
        if (is_array($muncod)) {
            foreach ($muncod as $k => $v) {
                $muncod[$k] = "'{$v}'";
            }
        } else {
            $muncod = array("'{$muncod}'");
        }
        foreach ($muncod as $k => $v) {
            if (trim($v) == '' || $v == "''" || empty($v)) {
                unset($muncod[$k]);
            }
        }
        if (!empty($muncod)) {
            $where[] = " mun.muncod IN ( " . implode(",", $muncod) . " ) ";
        }
    }

    if (!empty($esdid_obr) && is_array($esdid_obr) && $esdid_obr[0] != '') {
        $where[] = " ed.esdid IN ( " . implode(",", $esdid_obr) . " ) ";
    }

    if ( !empty($prfid) && count($prfid) > 0 && $prfid[0] !== ''){
        $where[] = "pf.prfid IN('" . implode("', '", $prfid) . "')";
    }

    if ( !empty($tooid) && count($tooid) > 0 && $tooid[0] !== ''){
        $where[] = " too.tooid IN('" . implode("', '", $tooid) . "')";
    }

    if ( !empty($tpoid) && count($tpoid) > 0 && $tpoid[0] !== ''){
        $where[] = " tpo.tpoid IN('" . implode("', '", $tpoid) . "')";
    }

    switch ($vistoria) {
        case 'sim' :
            $where[] = " o.obrdtvistoria IS NOT NULL ";
            break;
        case 'nao' : $where[] = " o.obrdtvistoria IS NULL ";
            break;
    }

    $strWhere = (!empty($where)) ? ' AND '.implode(' AND ', $where) : '';

  $sql = <<<DML
         WITH supervisoes_4_ultimas AS (

            SELECT
                obrid, supdata, percent
            FROM
              ( SELECT obrid, supdata, percent ,
                   ROW_NUMBER() OVER (PARTITION BY obrid
                              ORDER BY MAX(supdata) DESC
                             ) AS rn
                FROM obras2.vm_supervisao_obras
                GROUP BY obrid, supdata, percent
              ) AS t
            WHERE
                rn <= 4
            ORDER BY
                obrid, rn
        )
        ,
        supervisoes_120 AS (

            SELECT
                obrid, supdata, percent
            FROM
              ( SELECT obrid, supdata, percent ,
                   ROW_NUMBER() OVER (PARTITION BY obrid
                              ORDER BY MAX(supdata) DESC
                             ) AS rn
                FROM obras2.vm_supervisao_obras
                WHERE supdata <= (NOW() - '120 days'::interval)
                GROUP BY obrid, supdata, percent
                ORDER BY supdata
              ) AS t
            WHERE
                rn <= 1
            ORDER BY
                obrid, rn

        )
        ,
        obras_vistorias as (
            SELECT
                o.obrid,
                ARRAY_TO_STRING(array_agg( s4.percent::text || ';' || s4.supdata ) , '@') as vistorias,
                s120.percent as percent_120,
                s120.supdata as supdata_120

            FROM obras2.obras o
            JOIN supervisoes_4_ultimas s4 ON s4.obrid = o.obrid
            LEFT JOIN supervisoes_120 s120 ON s120.obrid = o.obrid
            WHERE
                o.obridpai IS NULL
                AND o.obrstatus = 'A'
            GROUP BY o.obrid, s120.percent, s120.supdata
        )
        ,
        supervisoes_365 AS (

            SELECT
                obrid, supdata, percent
            FROM
              ( SELECT obrid, supdata, percent ,
                   ROW_NUMBER() OVER (PARTITION BY obrid
                              ORDER BY MAX(supdata) DESC
                             ) AS rn
                FROM obras2.vm_supervisao_obras
                WHERE supdata <= (NOW() - '365 days'::interval)
                GROUP BY obrid, supdata, percent
                ORDER BY supdata
              ) AS t
            WHERE
                rn <= 1
            ORDER BY
                obrid, rn

        )
        ,
        obras_vistorias_365 as (
            SELECT
                o.obrid,
                ARRAY_TO_STRING(array_agg( s4.percent::text || ';' || s4.supdata ) , '@') as vistorias_365,
                s365.percent as percent_365,
                s365.supdata as supdata_365

            FROM obras2.obras o
            JOIN supervisoes_4_ultimas s4 ON s4.obrid = o.obrid
            LEFT JOIN supervisoes_365 s365 ON s365.obrid = o.obrid
            WHERE
                o.obridpai IS NULL
                AND o.obrstatus = 'A'
            GROUP BY o.obrid, s365.percent, s365.supdata
        )

        select
            o.obrid,
            o.obrnome,
            pf.prfdesc as programa,
            too.toodescricao as fonte,
            tpo.tpodsc as tipologia,
            ed.esddsc as situacao,
            mun.estuf,
            mun.mundescricao,
            vistorias,
            percent_120,
            supdata_120,
            vistorias_365,
            percent_365,
            supdata_365
       /* from obras_vistorias ov

          JOIN obras2.obras o ON o.obrid = ov.obrid*/
        from obras2.obras o
        join obras_vistorias ov on ov.obrid = o.obrid

        join obras_vistorias_365 ov_365 on ov_365.obrid = o.obrid


        JOIN obras2.empreendimento          ep ON ep.empid   = o.empid
        LEFT JOIN workflow.documento       doc ON doc.docid  = o.docid
        LEFT JOIN workflow.estadodocumento 	ed ON ed.esdid   = doc.esdid
        LEFT JOIN obras2.tipoorigemobra    too ON too.tooid  = o.tooid
        LEFT JOIN obras2.programafonte      pf ON pf.prfid   = ep.prfid
        LEFT JOIN obras2.tipologiaobra     tpo ON tpo.tpoid  = o.tpoid
        LEFT JOIN entidade.endereco        edo ON edo.endid  = o.endid
        LEFT JOIN territorios.municipio    mun ON mun.muncod = edo.muncod
        WHERE 1 = 1
         {$strWhere}
DML;

    return $sql;
}

// exibe consulta
if ($_REQUEST['tiporelatorio']) {

    $sql = monta_sql();
    $obras = $db->carregar($sql);
    if (!$obras) {
        echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
        echo '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
        echo  '</table>';
        die();
    } else {

        $cabecalho = array(
            "ID",
            "Nome",
            "Programa",
            "Fonte",
            "Tipologia",
            "Situação",
            "UF",
            "Município",
            "Dt. Vistoria 01",
            "% Vistoria 01",
            "Dt. Vistoria 02",
            "% Vistoria 02",
            "Dt. Vistoria 03",
            "% Vistoria 03",
            "Dt. Vistoria 04",
            "% Vistoria 04",
            "Qtd Dias entre as 4 últimas vistorias",
            "% de avanço de obra nas 4 últimas vistorias",
            "% avançado nos últimos 120 dias",
            "% avançado nos últimos 365 dias",
        );




        $arrRelatorio = array();

        foreach ($obras as $obra) {
            $linha = array();

            $linha['obrid'] = $obra['obrid'];
            $linha['obrnome'] = $obra['obrnome'];
            $linha['programa'] = $obra['programa'];
            $linha['fonte'] = $obra['fonte'];
            $linha['tipologia'] = $obra['tipologia'];
            $linha['situacao'] = $obra['situacao'];
            $linha['estuf'] = $obra['estuf'];
            $linha['mundescricao'] = $obra['mundescricao'];

            $vistorias = array();

            $data1 = $perc1 = $data2 = $perc2 = $data3 = $perc3 = $data4 = $perc4 = '';


            foreach (explode('@', $obra['vistorias']) as $vistoria) {
                $dadosVistoria = explode(';', $vistoria);
                $vistorias[] = $dadosVistoria;
            }

            if (!empty($vistorias[0])) {
                if ($vistorias[0][1]) {
                    $dateTime = datetime::createfromformat('Y-m-d H:i:s', $vistorias[0][1]);
                    $data1 = $dateTime->format('d/m/Y');
                }
                $perc1 = $vistorias[0][0];
            }
            $linha['dt_vist_01'] = $data1;
            $linha['perc_vist_01'] = $perc1;

            if (!empty($vistorias[1])) {
                if ($vistorias[1][1]) {
                    $dateTime = datetime::createfromformat('Y-m-d H:i:s', $vistorias[1][1]);
                    $data2 = $dateTime->format('d/m/Y');
                }
                $perc2 = $vistorias[1][0];
            }
            $linha['dt_vist_02'] = $data2;
            $linha['perc_vist_02'] = $perc2;

            if (!empty($vistorias[2])) {
                if ($vistorias[2][1]) {
                    $dateTime = datetime::createfromformat('Y-m-d H:i:s', $vistorias[2][1]);
                    $data3 = $dateTime->format('d/m/Y');
                }
                $perc3 = $vistorias[2][0];
            }
            $linha['dt_vist_03'] = $data3;
            $linha['perc_vist_03'] = $perc3;

            if (!empty($vistorias[3])) {
                if ($vistorias[3][1]) {
                    $dateTime = datetime::createfromformat('Y-m-d H:i:s', $vistorias[3][1]);
                    $data4 = $dateTime->format('d/m/Y');
                }
                $perc4 = $vistorias[3][0];
            }
            $linha['dt_vist_04'] = $data4;
            $linha['perc_vist_05'] = $perc4;

            $maisAntiga = current($vistorias);
            $maisRecente = end($vistorias);
            $percEvolucao = '';
            $diasEvolucao = '';
            $avanco120 = '';
            $avanco365 = '';

            if (!empty($obra['vistorias'])) {
                $dtMaisAntiga = datetime::createfromformat('Y-m-d H:i:s', $maisAntiga[1]);
                $dtMaisRecente = datetime::createfromformat('Y-m-d H:i:s', $maisRecente[1]);

                $percEvolucao = $maisRecente[0] - $maisAntiga[0];
                $diasEvolucao = $dtMaisRecente->diff($dtMaisAntiga)->days;

                if (!empty($obra['percent_120']))
                    $avanco120 = $maisRecente[0] - $obra['percent_120'];
            }

            if (!empty($obra['vistorias_365'])) {
                $dtMaisAntiga = datetime::createfromformat('Y-m-d H:i:s', $maisAntiga[1]);
                $dtMaisRecente = datetime::createfromformat('Y-m-d H:i:s', $maisRecente[1]);

                $percEvolucao = $maisRecente[0] - $maisAntiga[0];
                $diasEvolucao = $dtMaisRecente->diff($dtMaisAntiga)->days;

                if (!empty($obra['percent_365']))
                    $avanco365 = $maisRecente[0] - $obra['percent_365'];
            }

            $linha['dias'] = $diasEvolucao;
            $linha['avanco'] = $percEvolucao;
            $linha['evolucao'] = $avanco120;
            $linha['evolucao365'] = $avanco365;

            $arrRelatorio[] = $linha;
        }
//    ver($arrRelatorio);exit;

        switch ($_REQUEST['tiporelatorio']) {
            case 'xls':

                $db->sql_to_xml_excel($arrRelatorio, 'rel_obrasmi', $cabecalho);
                die();
                break;
            case 'html':

                echo '<html>
                    <head>
                        <title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
                        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                        <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                        <body>
                            <center>
                                    <!--  Cabeçalho Brasão -->
                                    ' . monta_cabecalho_relatorio('95') . '
                            </center>
                ';

                $db->monta_lista($arrRelatorio, $cabecalho, 30, 50, 'N', 'center', 'N', 'N');
                echo '</body>';
                die();
                break;
        }
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
require APPRAIZ . 'obras2/includes/principal/listaRestricao/ctrl.php';
echo "<br>";
monta_titulo("Relatório de Evolução da Obra", 'Selecione os filtros');
?>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<script type="text/javascript">


    function exibeRelatorioGeral(tipo) {
        var formulario = document.formulario;
        selectAllOptions(document.getElementById('prfid'));
        selectAllOptions(document.getElementById('tooid'));
        selectAllOptions(document.getElementById('estuf'));
        selectAllOptions(document.getElementById('muncod'));
        selectAllOptions(document.getElementById('esdid_obr'));

        formulario.action = 'obras2.php?modulo=relatorio/relatorioEvolucaoObra&acao=A&tiporelatorio=' + tipo;
        window.open('', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        formulario.target = 'relatorio';
        formulario.submit();
    }


    /* Função para substituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }

    /**
     * Alterar visibilidade de um bloco.
     *
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco(bloco)
    {
        var div_on = document.getElementById(bloco + '_div_filtros_on');
        var div_off = document.getElementById(bloco + '_div_filtros_off');
        var img = document.getElementById(bloco + '_img');
        var input = document.getElementById(bloco + '_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }

    /**
     * Alterar visibilidade de um campo.
     *
     * @param string indica o campo a ser mostrado/escondido
     * @return void
     */
    function onOffCampo(campo)
    {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }

//-->
</script>
<style>
    .chosen-select{
        width: 400px;
    }
</style>


<form action="" method="post" name="formulario" id="filtro">

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita" style="width: 490px;">Nome da Obra/ID:</td>
            <td>
                <?php echo campo_texto('obrid', 'N', 'S', '', 58, 60, '', '', 'left', '', 0, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Situação da Obra:</td>
            <td>
                <?php
                $sql_obr = "";

                $dados = $db->carregar(getSqlEstadoDoc(TPDID_OBJETO));
                $dados = $dados ? $dados : array();
                ?>
                <select name="esdid_obr[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dados as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['esdid_obr']) && in_array($key['codigo'], $_POST['esdid_obr'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>


        <tr id="idMunicipio">
            <td class="SubTituloDireita ">Municípios(s):</td>
            <td class="td_municipio ">
                <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                    <?php
                    $municipio = new Municipio();
                    foreach ($municipio->listaComboMulti() as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['muncod']) && in_array($key['codigo'], $_POST['muncod'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Programa:</td>
            <td class="tdInputForm">
                <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $programa = new ProgramaFonte();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    foreach ($programa->listacombo($param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['prfid']) && in_array($key['codigo'], $_POST['prfid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Fonte:</td>
            <td class="tdInputForm">
                <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $tipoOrigemObra = new TipoOrigemObra();
                    $param = array();
                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                        ?>
                        <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($_POST['tooid']) && in_array($key['codigo'], $_POST['tooid'])) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipologia:</td>
            <td>
                <select name="tpoid[]" id="tpoid" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php   $tipologiaObra = new TipologiaObra();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    $dados = $tipologiaObra->listaCombo($param, false);
                        foreach ($dados as $key) {
                    ?>
                        <option value="<?php echo $key['codigo'] ?>" <?php if ($tpoid && in_array($key['codigo'], $tpoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
    <tr>        
        <tr>
            <td class="SubTituloDireita">Possui vistoria?</td>
            <td>
                <input type='radio' id='vistoria' name='vistoria' value='sim'   /> Sim
                <input type='radio' id='vistoria' name='vistoria' value='nao'   /> Não
                <input type='radio' id='vistoria' name='vistoria' value='todos' checked="checked" /> Todas
            </td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC"></td>
            <td bgcolor="#CCCCCC">
                <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('html');" style="cursor: pointer;"/>
                <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('xls');" style="cursor: pointer;"/>

            </td>
        </tr>
    </table>

</form>

<script type="text/javascript">
    function downloadUltimoArquivo(file){
        window.open( window.location + '&download=' + file );
    }
</script>