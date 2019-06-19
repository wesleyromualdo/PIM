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
    unset($_SESSION['obras2']['assinatura_gestor']['filtros']);
    echo "<script>window.location.href = window.location.href;</script>";
    exit();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

monta_titulo( 'Lista de Assinaturas de Gestores', '');
if(empty($_POST) && !empty($_SESSION['obras2']['assinatura_gestor']['filtros']))
    $_POST = $_SESSION['obras2']['assinatura_gestor']['filtros'];
else
    $_SESSION['obras2']['assinatura_gestor']['filtros'] = $_POST;
extract($_POST);

$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<div class="row col-md-12">
    <input type="button" value="Voltar" id="voltar" style="cursor:pointer;" />
    <input type="button" value="Novo" id="novo" style="cursor:pointer;" />
    <br>
</div>
<?php
$coluns = array();
$join = array();
$where = array();

$cabecalho = array('Ação', 'Nome');

$sql = "
        SELECT
            DISTINCT
            '<center>
                <div style=\"width:100%;\">
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: acessaAssinaturaGestor(' || ag.agid || ');\"
                        title=\"Acessar Assinatura Gestor\">
                </div>
            </center>' AS acao,
            ag.nome
        FROM obras2.assinatura_gestor ag
        ";

#$tamanho = Array('4%', '', '', '', '', '', '', '', '', '', '');
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario",$tamanho);
?>
<script type="text/javascript">

    $(document).ready(function (){
        $('#voltar').on('click',function(){
            window.location.href = '?modulo=principal/listaAlerta&acao=A';
        });
        $('#novo').on('click',function(){
            window.location.href = '?modulo=principal/cadAssinaturaGestor&acao=A';
        });
    });



    function acessaAssinaturaGestor(id){
        location.href = '?modulo=principal/cadAssinaturaGestor&acao=A&id=' + id;
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
