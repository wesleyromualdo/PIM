<?php
/**
 * Lista de Municipios
 *
 * @category Lista
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN: 10000
 * @link     no link
 */
require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';
include_once 'programas/_funcoes_maismedicos.php';


// Municipio.
$_REQUEST ['itrid'] = 2;
$_REQUEST['responsabilidadeMunicipio'] = pegaResponssabilidade('2');

echo $simec->title('ADESÃO ', 'EDITAL DA SELEÇÃO DE MUNICÍPIOS PARA NOVOS CURSOS DE MEDICINA');

$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$muncod = $controleUnidade->pegarMuncod($_REQUEST['inuid']);
$termoAceite = $db->pegaUm("SELECT tcrid FROM maismedicomec.termocompromissoresidencia WHERE prgid = {$_REQUEST['prgid']} AND muncod = '{$muncod}' AND tcraceite = 'A'");
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

if(!$termoAceite){
    $termo = $db->pegaLinha("SELECT tcrid, tcraceite FROM maismedicomec.termocompromissoresidencia WHERE prgid = {$_REQUEST['prgid']} AND muncod = '{$muncod}'");
    $termoAceite = ($termo['tcraceite'] == 'N') ? $termo['tcrid'] : $termoAceite;
}


$ano = date(Y);

$adesao = $db->pegaUm("SELECT adpresposta FROM  par3.prodesaoprograma WHERE adpano = '{$ano}' AND inuid = '{$_REQUEST['inuid']}' order by adpid DESC ");

$prgid = $_REQUEST['prgid'];

if ($termoAceite) {
    $valida_getor_s = buscaDadosGestorMaisMedicos('S', $muncod, $prgid);
    $valida_getor_m = buscaDadosGestorMaisMedicos('M', $muncod, $prgid);
    
    $valida_documento = $db->pegaUm("SELECT a.aacmid FROM maismedicomec.arquivoadesaocursomedicina a
                                 INNER JOIN public.arquivo b ON b.arqid = a.arqid
                                 INNER JOIN seguranca.usuario c ON c.usucpf = a.usucpf 
                                 WHERE a.tpaid = '28' AND a.tcrid = '{$termoAceite}'");
}

echo ' <br/>';

/* * * Array com os itens da aba de primeiros passos ** */
$menu = array(
    0 => array("id" => 1, "descricao" => "Condições de Participação", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=condicao"),
    1 => array("id" => 1, "descricao" => "Termo de Compromisso Residência Médica", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=termo"),
);


if ($termoAceite > 0) {

    // Perfis que poderão visualizar a aba anexos
    $arPerfilAbaAnexo = [ PAR3_PERFIL_SUPER_USUARIO,
                          PAR3_PERFIL_CURSO_MEDICINA_AVALIADOR_INST,
                          PAR3_PERFIL_EQUIPE_MUNICIPAL,
                          PAR3_PERFIL_DIRIGENTE_MUNICIPAL,
                          PAR3_PERFIL_PREFEITO];
    $boolAbaAnexo = (boolean)array_intersect($arPerfilAbaAnexo, $arrayPerfil);

    if( $boolAbaAnexo ){
        $menu = array(
            0 => array("id" => 1, "descricao" => "Condições de Participação", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=condicao"),
            1 => array("id" => 1, "descricao" => "Termo de Compromisso Residência Médica", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=termo"),
            2 => array("id" => 1, "descricao" => "Dados Representantes", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=dados"),
            3 => array("id" => 1, "descricao" => "Documentos", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=documentos"),
            4 => array("id" => 1, "descricao" => "Adesão", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=adesao"),
            5 => array("id" => 1, "descricao" => "Resultado", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=anexos"),
            6 => array("id" => 1, "descricao" => "Recurso", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=resposta")
        );

    } else {
        $menu = array(
            0 => array("id" => 1, "descricao" => "Condições de Participação", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=condicao"),
            1 => array("id" => 1, "descricao" => "Termo de Compromisso Residência Médica", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=termo"),
            2 => array("id" => 1, "descricao" => "Dados Representantes", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=dados"),
            3 => array("id" => 1, "descricao" => "Documentos", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=documentos"),
            4 => array("id" => 1, "descricao" => "Adesão", "link" => "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=adesao"),
        );
    }
}

/* * * Verifica qual aba está ativa ** */
switch ($_GET['aba']) {
    case "condicao":
        $abaAtiva = "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=condicao";
        $pagAtiva = "par3/modulos/principal/adesao/termos/mais_medicos_2017/maisMedicosCondicoesPart.inc";
        break;

    case "termo":
        $abaAtiva = "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=termo";
        $pagAtiva = "par3/modulos/principal/adesao/termos/mais_medicos_2017/maisMedicosInforMunicipio.inc";
        break;

    case "dados":
        $abaAtiva = "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=dados";
        $pagAtiva = "par3/modulos/principal/adesao/termos/mais_medicos_2017/maisMedicosDadosRepres.inc";
        break;

    case "documentos":
        $abaAtiva = "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=documentos";
        $pagAtiva = "par3/modulos/principal/adesao/termos/mais_medicos_2017/maisMedicosDocumentos.inc";
        break;

    case "adesao":
        $abaAtiva = "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=adesao";
        $pagAtiva = "par3/modulos/principal/adesao/termos/mais_medicos_2017/maisMedicosAdesao.inc";
        break;

    case "anexos":
        $abaAtiva = "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=anexos";
        $pagAtiva = "par3/modulos/principal/adesao/termos/mais_medicos_2017/maisMedicosAnexos.inc";
        break;

    case "resposta":
        $abaAtiva = "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=resposta";
        $pagAtiva = "par3/modulos/principal/adesao/termos/mais_medicos_2017/maisMedicosResposta.inc";
        break;

    default :
        $abaAtiva = "par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=condicao";
        $pagAtiva = "par3/modulos/principal/adesao/termos/mais_medicos_2017/maisMedicosCondicoesPart.inc";
        break;

}

/* * * Monta o segundo conjunto de abas ** */
#echo montarAbasArray($menu, $abaAtiva);

echo $simec->tab($menu, $abaAtiva);

/* * * Monta a página do segundo conjunto de abas ** */
include_once APPRAIZ . $pagAtiva;
?>


<script type="text/javascript">

    function imprimirTermoAdesao() {
        return window.open('par3.php?modulo=principal/adesao/termos/mais_medicos_2017/maisMedicosAdesaoImprimir&acao=A&inuid=<?php echo $_REQUEST['inuid']; ?>&prgid=<?=$_REQUEST['prgid']?>', 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
    }

    function sair(){
       window.location.href = "par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?= $_REQUEST['inuid'];?>";
    }

</script>