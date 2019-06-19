<?php
  
require APPRAIZ . 'obras2/includes/principal/listaObras/funcoes.php';


ini_set("memory_limit", "512M");

ob_start();

$arOrgid = verificaAcessoEmOrgid();

if (!in_array($_SESSION['obras2']['orgid'], $arOrgid)) {
    $_SESSION['obras2']['orgid'] = '';
}
$_SESSION['obras2']['orgid'] = 3;  
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current($arOrgid));
$orgid = $_SESSION['obras2']['orgid'];

switch ($_REQUEST['req']) {
    case 'abaFotos':

        $url = '/slideshow/slideshow/obras2_galeriaGaleriaFotos.php?tipo=abaGaleria&obrid=' . $_GET['obrid'];
        header('Location: ' . $url);
        die();
    case 'abaRestricao':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $obrid = $_SESSION['obras2']['obrid'];
        $obras = new Obras();
        $obras->carregarPorIdCache($obrid);
        
        $_SESSION['obras2']['empid'] = $obras->empid;
        header('Location: ?modulo=principal/listaRestricao&acao=O');
        die();
    case 'abaDocumento':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $obrid = $_SESSION['obras2']['obrid'];
        $obras = new Obras();
        $obras->carregarPorIdCache($obrid);
        
        $_SESSION['obras2']['empid'] = $obras->empid;
        header('Location: ?modulo=principal/cadObraDocumentos&acao=A');
        die();
 
    case 'pegaHtmlPagamento':
        if ($_REQUEST['preid']) {
            
            $sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento
                    FROM par.empenhoobra emo
                    INNER JOIN par3.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A'
                    INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
                    INNER JOIN par3.pagamento pag ON pag.empid = emo.empid
                    WHERE emo.preid = '" . $_REQUEST['preid'] . "' AND pag.pagstatus='A'
                    UNION ALL
                    SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento
                    FROM par.empenhoobrapar emo
                    INNER JOIN par3.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A'
                    INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
                    INNER JOIN par3.pagamento pag ON pag.empid = emo.empid
                    WHERE emo.preid = '" . $_REQUEST['preid'] . "' AND pag.pagstatus='A'";

            $pagamentoobra = $db->pegaLinha($sql);

            if ($pagamentoobra) {
                echo "Pago<br>
                      Valor pagamento(R$): " . number_format($pagamentoobra['pagvalorparcela'], 2, ",", ".") . "<br>
                      Nº da Ordem Bancária: " . $pagamentoobra['pagnumeroob'] . "<br>
                      Data do pagamento: " . $pagamentoobra['pagdatapagamento'] . "<br>
                      Banco: " . $pagamentoobra['probanco'] . ", Agência: " . $pagamentoobra['proagencia'] . "
                        ";
            } else {
                echo "Não pago";
            }
        }
        die();
    case 'listaObrasUnidade':
        $obras = new Obras();
        $filtro = $_POST;
        $filtro['not(obridpai)'] = true;
        $cabecalho = $obras->cabecalhoListaSql();
        $sql = ($_POST ? $obras->listaSql($filtro) : array());
        
        $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', $par2, "formulario");
        die();
    case 'supervisorFNDE':
        $_SESSION['obras2']['obrid'] = $_POST['obrid'];
        $_SESSION['obras2']['empid'] = $_POST['empid'];
        header("Location: obras2.php?modulo=principal/listaSupervisaoFNDE&acao=A");
        exit;
        break;

}

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';

switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=utf-8');
        $estuf = $_REQUEST['estuf'];
        ?>

        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="muncod[]"]').chosen();

            });
        </script>

        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
            <?php   $municipio = new Municipio();
            foreach ($municipio->listaComboMulti($estuf) as $key) {
                ?>
                <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php
        exit;
    case 'unidade':
        header('content-type: text/html; charset=utf-8');
        ?>
        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="entid[]"]').chosen();

            });
        </script>

        <select name="entid[]" class="chosen-select" multiple data-placeholder="Selecione">
            <?php
            $entidade = new Entidade();
            foreach ($entidade->listaComboMulti($_POST['muncod']) as $key) {
                ?>
                <option
                        value="<?php echo $key['codigo'] ?>" <?php if (isset($entid) && in_array($key['codigo'], $entid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php

        exit;
}


/**
 * EXTRAI OS DADOS DA PESQUISA PARA RECARREGAR O FORM
 */
extract($_POST);


