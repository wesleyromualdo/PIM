<?php
$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
    $_SESSION['obras2']['orgid'] = '';
}
$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
$orgid 						 = $_SESSION['obras2']['orgid'];

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';

#Requisicao Limpar
if ($_POST['req'] == 'limpar') {
    unset($_SESSION['obras2']['alerta']['filtros']);

    echo "<script>window.location.href = window.location.href;</script>";
    exit();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( 'Lista de Alertas', '');
if(empty($_POST) && !empty($_SESSION['obras2']['alerta']['filtros']))
    $_POST = $_SESSION['obras2']['alerta']['filtros'];
else
    $_SESSION['obras2']['alerta']['filtros'] = $_POST;
extract($_POST);
$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

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
                Perfil:
            </td>
            <td>
                <select name="perfil[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    $sql = "SELECT pflcod AS codigo, pfldsc AS descricao FROM seguranca.perfil WHERE sisid = 147 ORDER BY pfldsc";
                    $arDados = $db->carregar($sql);
                    foreach ($arDados as $key):
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($perfil) && in_array($key['codigo'], $perfil)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php
                    endforeach; ?>
                </select>
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
                        array('codigo' => 'A', 'descricao' => 'Ativo'),
                        array('codigo' => 'C', 'descricao' => 'Lido'),
                        array('codigo' => 'I', 'descricao' => 'Inativo')
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
                <input id="gerenciar_assinaturas" type="button" value="Gerenciar Assinaturas"/>
                <input id="novo_alerta" type="button" value="Novo Alerta"/>
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
    $where['situacao'] = "a.situacao IN('" . implode('\',\'', $_REQUEST['situacao']) . "')";
if ($_REQUEST['perfil'] && $_REQUEST['perfil'][0] != '')
    $where['perfil'] = "ap.pflcod IN(" . implode(',', $_REQUEST['perfil']) . ")";

$cabecalho = array('Ação', 'ID', 'Titulo', 'Situação', 'Data de Criação');

$orWhere = is_array($orWhere) ? $orWhere : array();
$where = is_array($where) ? $where : array();

$sql = "
        SELECT
            DISTINCT
            '<center>
                <div style=\"width:100%;\">
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: acessaAlerta(' || a.aid || ');\"
                        title=\"Acessar Alerta\">
                </div>
            </center>' AS acao,
            lpad(a.aid::varchar,7,'0') as id,
            a.titulo,
            '<center>' || CASE a.situacao WHEN 'A' THEN 'Ativo' WHEN 'C' THEN 'Lido' ELSE 'Inativo' END || '</center>' AS situacao,
            '<center>' || TO_CHAR(a.dtcriacao,'DD/MM/YYYY') || '</center>' AS dt
        FROM obras2.alerta a
        LEFT JOIN obras2.alerta_obra ao ON (a.aid = ao.aid)
        LEFT JOIN obras2.obras obr ON (ao.obrid = obr.obrid)
        LEFT JOIN obras2.alerta_perfil ap ON (a.aid = ap.aid)
        WHERE 1 = 1
            " . (count($where) ? (' AND '.implode(' AND ',$where)) : "") . "
		    " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
        ORDER BY id";

$tamanho = Array('4%', '', '', '', '', '', '', '', '', '', '');
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
        $('#gerenciar_assinaturas').click(function() {
            window.location.href = '?modulo=principal/listaAssinaturaGestor&acao=A';
        });
        $('#novo_alerta').click(function() {
            window.location.href = '?modulo=principal/cadAlerta&acao=A';
        });


    });

    $1_11(document).ready(function () {
        $1_11('select[name="situacao[]"]').chosen();
        $1_11('select[name="perfil[]"]').chosen();

    });

    function acessaAlerta(id){
        location.href = '?modulo=principal/cadAlerta&acao=A&id_alerta=' + id;
    }
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
