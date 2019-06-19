<?php

function monta_sql() {
    global $db;

    extract($_REQUEST);

    $where = array();
    
    if (!empty($obrid)) {
        $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($obrid))));
        $where[] = " ( o.obrnome::CHARACTER VARYING ILIKE ('%" . $obrbuscatextoTemp . "%') OR o.obrid::CHARACTER VARYING ILIKE ('%" . $obrid . "%') ) ";
    }
    
    if (!empty($preid)) {
        $preidbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($preid))));
        $where[] = " ( o.preid::CHARACTER VARYING ILIKE ('%" . $preidbuscatextoTemp . "%')  ) ";
    }

    if (!empty($tpoid)) {
        $where[] = " o.tpoid = $tpoid ";
    }

    if (!empty($estuf)) {
        if (is_array($estuf)) {
            foreach ($estuf as $k => $v) {
                $estuf[$k] = "'{$v}'";
            }
        } else {
            $estuf = array("'{$estuf}'");
        }
        foreach ($estuf as $k => $v) {
            if (trim($v) == '' || $v == "''" || empty($v)) {
                unset($estuf[$k]);
            }
        }
        if (!empty($estuf)) {
            $where[] = " mun.estuf IN ( " . implode(",", $estuf) . " ) ";
        }
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

    if ( !empty($prfid) && count($prfid) > 0 && $prfid[0] !== ''){
        $where[] = "pf.prfid IN('" . implode("', '", $prfid) . "')";
    }

    if ( !empty($tooid) ){
        $where[] = " vm.\"Fonte\" = '".$tooid."'";
    }

    $strWhere = (!empty($where)) ? ' AND '.implode(' AND ', $where) : '';

    $sql = "SELECT
                  vm.*
                FROM obras2.vm_termo_obras vm
                JOIN obras2.obras o ON o.obrid = vm.\"ID Obra\"
                JOIN obras2.empreendimento ep ON ep.empid = o.empid
                LEFT JOIN entidade.endereco ed ON ed.endid = o.endid
                LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
                LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid

                WHERE
                1 = 1
                 ".$strWhere."

				";
    return $sql;
}

// exibe consulta
if ($_REQUEST['tiporelatorio']) {
    
    $sql = monta_sql();
    
//    ver($sql);

    $cabecalho = array("ID Obra", "Preid", "Número do Termo", "Nome", "Município", "UF", "Fonte", "Programa", "Situação", "% Ult. Vistoria",
                       "Dt. Ult. Vistoria", "Data Primeiro Pagamento", "Data Último Pagamento", "Pagamento Solicitado Por", "Início Vigência da Obra",
                       "Fim Vigência da Obra", "Houve Prorrogação?", "Prorrogação Solicitada Por", "Data de Validação do Termo", "Validado Por",
                       "Início Vigência Termo", "Fim Vigência Termo");

    switch ($_REQUEST['tiporelatorio']) {
        case 'xls':
            ob_clean();
            ini_set("memory_limit", "512M");
            header("Content-type: application/excel; name=ListaDeAtividadesDeMonitoramentoEspecial.xls");
            header("Content-Disposition: attachment; filename=ListaDeAtividadesDeMonitoramentoEspecial.xls");
            $db->sql_to_xml_excel($sql, 'rel_obrasmi', $cabecalho);
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
                                    '.monta_cabecalho_relatorio('95').' 
                            </center>
                ';
            $db->monta_lista($sql, $cabecalho, 30, 50, 'N', 'center', 'N', 'N');
            echo '</body>';
            die();
            break;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

//usa a referência para listar o campo situação
require APPRAIZ . 'obras2/includes/principal/listaRestricao/ctrl.php';

echo "<br>";
monta_titulo("Relatório do Vencimento do Termo", 'Selecione os filtros');
?>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<script type="text/javascript">


    function exibeRelatorioGeral(tipo) {
        var formulario = document.formulario;
        /*selectAllOptions(document.getElementById('prfid'));
        selectAllOptions(document.getElementById('estuf'));
        selectAllOptions(document.getElementById('muncod'));*/

        formulario.action = 'obras2.php?modulo=relatorio/relatorioVencimentoTermo&acao=A&tiporelatorio=' + tipo;
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
            <td class="SubTituloDireita" style="width: 490px;">Pré-ID da Obra:</td>
            <td>
                <?php echo campo_texto('preid', 'N', 'S', '', 58, 60, '', '', 'left', '', 0, ''); ?>
            </td>
        </tr>
        
        <tr id="SubTituloDireita">
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
            <td bgcolor="#CCCCCC"></td>
            <td bgcolor="#CCCCCC">
                <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('html');" style="cursor: pointer;"/> 
                <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
            </td>
        </tr>
    </table>
    
</form>