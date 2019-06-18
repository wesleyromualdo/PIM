<?php 
require APPRAIZ . 'obras2/includes/principal/cadValidacao/funcoes.php';

if (empty($_SESSION['obras2']['obrid']) && !empty($_GET['obrid'])) {
    if (!empty($_GET['obrid'])) {
        $_SESSION['obras2']['obrid'] = $_GET['obrid'];
    }
    if (empty($_SESSION['obras2']['empid'])) {
        $o = new Obras();
        $o->carregarPorIdCache($_SESSION['obras2']['obrid']);
        $_SESSION['obras2']['empid'] = $o->empid;
    }
}

// empreendimento || obra || orgao
verificaSessao('obra');

$obrid = $_SESSION['obras2']['obrid'];


switch ($_REQUEST['requisicao']) {
    case "resetchecklist":
        unset($_SESSION['obras2']['checklistfnde']);
        die('
                  <script>
                    window.close();
                  </script>
                ');
        break;
    case "checklist":
        $chkFnde = new ChecklistFnde();
        $tipo = $_GET['tipo'];                                 // tec/adm/2p
        $obrid = $_SESSION['obras2']['obrid'];                  // ID obrid
        $ckfid = (!empty($_GET['ckfid'])) ? $_GET['ckfid'] : 0; // ID ckfid
        $acao = $_GET['acao'];                                 // duplicar/editar/cadastrar/excluir;
        $chkFnde->mostraPopupChecklistQuestionario($tipo, $obrid, $ckfid, $acao);
        die();
        break;
    case "salvar":
        $arCamposNulo = array();

        extract($_POST);

        $obra = new Obras();
        $obra->carregarPorIdCache($obrid);
        $percentual_de_comparacao = $obra->calculaPercentualValidacaoParcela();

        $habilita25 = ($percentual_de_comparacao >= '25') ? true : false;
        $habilita50 = ($percentual_de_comparacao >= '50') ? true : false;

        if (obraMi($obrid)) {
            $habilita25 = true;
            $habilita50 = true;
        }

        if ($vldstatushomologacao) {
            $vldstatushomologacao = $vldstatushomologacao;
            $usucpf_homo = $usucpf_homo ? $usucpf_homo : $_SESSION['usucpf'];
            $vlddtinclusaosthomo = $vlddtinclusaosthomo ? $vlddtinclusaosthomo : "now()";
        } else {
            $vldstatushomologacao = ' ';
            $usucpf_homo = null;
            $vlddtinclusaosthomo = null;
            $arCamposNulo[] = 'vldstatushomologacao';
            $arCamposNulo[] = 'usucpf_homo';
            $arCamposNulo[] = 'vlddtinclusaosthomo';
        }

        if ($vldstatus25exec && $habilita25) {
            $vldstatus25exec = $vldstatus25exec;
            $usucpf_25 = $usucpf_25 ? $usucpf_25 : $_SESSION['usucpf'];
            $vlddtinclusaost25exec = $vlddtinclusaost25exec ? $vlddtinclusaost25exec : "now()";
        } else {
            $vldstatus25exec = ' ';
            $usucpf_25 = null;
            $vlddtinclusaost25exec = null;
            $arCamposNulo[] = 'vldstatus25exec';
            $arCamposNulo[] = 'vlddtinclusaost25exec';
            $arCamposNulo[] = 'vlddtinclusaost25exec';
        }

        if ($vldstatus50exec && $habilita50) {
            $vldstatus50exec = $vldstatus50exec;
            $usucpf_50 = $usucpf_50 ? $usucpf_50 : $_SESSION['usucpf'];
            $vlddtinclusaost50exec = $vlddtinclusaost50exec ? $vlddtinclusaost50exec : "now()";
        } else {
            $vldstatus50exec = ' ';
            $usucpf_50 = null;
            $vlddtinclusaost50exec = null;
            $arCamposNulo[] = 'vldstatus50exec';
            $arCamposNulo[] = 'usucpf_50';
            $arCamposNulo[] = 'vlddtinclusaost50exec';
        }

        $validacao = new Validacao($_POST['vldid']);

        $arDado = array(
            'obrid' => $obrid,
            'usucpf_homo' => $usucpf_homo,
            'usucpf_25' => $usucpf_25,
            'usucpf_50' => $usucpf_50,
            'vldstatushomologacao' => $vldstatushomologacao,
            'vlddtinclusaosthomo' => $vlddtinclusaosthomo,
            'vldstatus25exec' => $vldstatus25exec,
            'vlddtinclusaost25exec' => $vlddtinclusaost25exec,
            'vldstatus50exec' => $vldstatus50exec,
            'vlddtinclusaost50exec' => $vlddtinclusaost50exec,
            'vldobshomologacao' => $vldobshomologacao,
            'vldobs25exec' => $vldobs25exec,
            'vldobs50exec' => $vldobs50exec
        );

        $validacao->popularDadosObjeto($arDado)->salvar(true, true, $arCamposNulo);

        espelhaObrasConvenio($obrid, $arDado, $arCamposNulo);

        $db->commit();
        $db->sucesso('principal/cadValidacao');
        break;
    case 'btnCancelaHmo':
        $validacao = new Validacao($_POST['vldid']);
        $arDado = array('vldstatushomologacao' => null, 'vldobshomologacao' => null);
        $arCamposNulo = array('vldstatushomologacao', 'vldobshomologacao');

        $validacao->popularDadosObjeto($arDado)
            ->salvar(true, true, $arCamposNulo);

        espelhaObrasConvenio($obrid, $arDado, $arCamposNulo);

        $db->commit();
        die('true');
        break;
    case 'btnCancela25':
        $validacao = new Validacao($_POST['vldid']);
        $arDado = array('vldstatus25exec' => null, 'vldobs25exec' => null);
        $arCamposNulo = array('vldstatus25exec', 'vldobs25exec');

        $validacao->popularDadosObjeto($arDado)
            ->salvar(true, true, $arCamposNulo);
        espelhaObrasConvenio($obrid, $arDado, $arCamposNulo);
        $db->commit();
        die('true');
        break;
    case 'btnCancela50':
        $validacao = new Validacao($_POST['vldid']);
        $arDado = array('vldstatus50exec' => null,
            'vldobs50exec' => null);
        $arCamposNulo = array('vldstatus50exec', 'vldobs50exec');

        $validacao->popularDadosObjeto($arDado)
            ->salvar(true, true, $arCamposNulo);
        espelhaObrasConvenio($obrid, $arDado, $arCamposNulo);
        $db->commit();
        die('true');
        break;
    case "download":
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $arqid = $_REQUEST['arqid'];
        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($arqid);
        die();
}

$habilitado = true;
$habilita = 'S';

if (possui_perfil(array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO))) {
    $habilitado = false;
    $habilita = 'N';
}
//retirando permissão de escrita para o perfil CALL CENTER
if (possui_perfil(PFLCOD_CALL_CENTER)){
    $semPermissao = 1;
}
include_once APPRAIZ . "includes/cabecalho.inc";

// Monta as abas e o título da tela
print "<br/>";
if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros);
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
}
echo cabecalhoObra($obrid);
monta_titulo_obras("Validação da Obra", "");

$validacao = new Validacao();
$arrFases = $validacao->pegaDadosComFaseLicitacao(array('obrid' => $obrid));

