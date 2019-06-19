<?php
/**
 * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
 */
#Requisicao Limpar
if ($_POST['req'] == 'limpar') {
    unset($_SESSION['obras2']['historico_notificacao']['filtros']);

    echo "<script>window.location.href = window.location.href;</script>";
    exit();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

if(empty($_POST) && !empty($_SESSION['obras2']['historico_notificacao']['filtros']))
    $_POST = $_SESSION['obras2']['historico_notificacao']['filtros'];
else
    $_SESSION['obras2']['historico_notificacao']['filtros'] = $_POST;
extract($_POST);
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<?php
if($_REQUEST['id_alerta']): ?>
    <div class="row">
        <div class="col-md-12">
            <ul class="nav nav-tabs">
                <li>
                    <a id="tab_alerta" href="obras2.php?modulo=principal/cadAlerta&acao=A&id_alerta=<?=$_REQUEST['id_alerta']?>">Notificação</a>
                </li>
                <li>
                    <a id="tab_vinculacao" href="obras2.php?modulo=principal/cadVinculacaoAlerta&acao=A&id_alerta=<?=$_REQUEST['id_alerta']?>">Vincular Obras</a>
                </li>
                <li class="active">
                    <a id="tab_historico" href="obras2.php?modulo=principal/historicoAlerta&acao=A&id_alerta=<?=$_REQUEST['id_alerta']?>">Histórico</a>
                </li>
            </ul>
        </div>
    </div>
<?
endif;
monta_titulo( 'Historico de Notificações', '');
?>
<form method="post" name="formListaAlerta" id="formListaAlerta">
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
        <tr>
            <td class="SubTituloDireita">
                Situação:
            </td>
            <td>
                <select name="situacao[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    $arDados = array(
                        array('codigo' => 'C', 'descricao' => 'Clicado'),
                        array('codigo' => 'L', 'descricao' => 'Lido'),
                    );
                    foreach ($arDados as $key):
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($situacao) && in_array($key['codigo'], $situacao)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php
                    endforeach; ?>
                </select>
            </td>
        </tr>
        <tr>
            <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
                <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
                <input id="button_limpar" type="button" value="Limpar Filtros"/>
            </td>
        </tr>
    </table>
</form>
<?php
$coluns = array();
$join = array();
$where = array();

if ($_REQUEST['obrbuscatexto']) {
    $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($_REQUEST['obrbuscatexto']))));
    $obrbuscatextoTemp = trim($obrbuscatextoTemp);

    if (!strpos($obrbuscatextoTemp, ',')) {
        $where['obrnome'] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                        obr.obrid::CHARACTER VARYING ILIKE ('%" . trim($_REQUEST['obrbuscatexto']) . "%') ) ";
    } else {
        $campos = explode(',', $obrbuscatextoTemp);
        $w = array();
        foreach ($campos as $c) {
            $c = trim($c);
            $w[] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                        obr.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') ) ";
        }

        $w = '(' . implode('OR', $w) . ')';
        $where['obrnome'] = $w;
    }
}

if ($_REQUEST['situacao'] && $_REQUEST['situacao'][0] != '')
    $where['situacao'] = "aol.acao IN('" . implode('\',\'', $_REQUEST['situacao']) . "')";
if ($_REQUEST['perfil'] && $_REQUEST['perfil'][0] != '')
    $where['perfil'] = "ap.pflcod IN(" . implode(',', $_REQUEST['perfil']) . ")";

$cabecalho = array('Usuário', 'Ação', 'ID da(s) Obra(s)', 'Data da Ação');

$sql = "
        SELECT
            usu.usunome,
            CASE WHEN aol.acao = 'L' THEN 'LIDO' ELSE 'CLICADO' END as acao,
            array_to_string(array_agg(ao.obrid),', ') || ' ' as obrid,

            TO_CHAR(aol.dtacao,'DD/MM/YYYY às HH24:MI') AS dtacao
        FROM obras2.alerta_obra_lido aol
        INNER JOIN obras2.alerta_obra ao ON (aol.aoid = ao.aoid)
        INNER JOIN seguranca.usuario usu ON (aol.usucpf = usu.usucpf)
        INNER JOIN obras2.obras obr ON (ao.obrid = obr.obrid)
        WHERE aol.aid = ".$_REQUEST['id_alerta']."
            " . (count($where) ? (' AND '.implode(' AND ',$where)) : "") . "
        GROUP BY usu.usunome, acao, dtacao
        ORDER BY aol.dtacao DESC, acao ASC, usu.usunome ASC";

$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario",$tamanho);
?>
<script type="text/javascript">

    $(document).ready(function (){
        $('.pesquisar').click(function (){
            $('#req').val('');
            $('#formListaAlerta').submit();
        });
        $('#button_limpar').click(function() {
            $('#req').val('limpar');
            $('#formListaAlerta').submit();
        });


    });

    $1_11(document).ready(function () {
        $1_11('select[name="situacao[]"]').chosen();
        $1_11('select[name="perfil[]"]').chosen();

    });
</script>
<style>
    .chosen-container-multi {
        width: 400px !important;
    }

    .chosen-container-multi .chosen-choices {
        width: 400px !important;
    }

    label.btn.active {
        background-image: none;
        outline: 0;
        -webkit-box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        color: #ffffff;
        background-color: #3276b1 !important;
        border-color: #285e8e;
    }
</style>