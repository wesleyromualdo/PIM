<?php 

require APPRAIZ . 'obras2/includes/principal/listaObrasTec/funcoes.php';

ini_set("memory_limit", "512M");
ob_start();


$arOrgid = verificaAcessoEmOrgid();
if (!in_array($_SESSION['obras2']['orgid'], $arOrgid)) {
    $_SESSION['obras2']['orgid'] = '';
}
$_SESSION['obras2']['orgid'] = 3; 
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current($arOrgid));
$orgid = $_SESSION['obras2']['orgid'];


switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=utf-8');
        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', 'carregarUnidade', '', '', 200, 'N', 'muncod');
        exit;
    case 'unidade':
        header('content-type: text/html; charset=utf-8');
        ?>
        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="entid[]"]').chosen({width: "300px"});

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
 
