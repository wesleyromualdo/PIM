<?php
/**
 * Created by PhpStorm.
 * User: jefersonaraujo
 * Date: 23/06/2017
 * Time: 15:09
 */
/**
 * Cabecalho do SIMEC.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

global $url;
//Url para redirecionamento
$url = "spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=".$_GET['aba'];
$titulo = ($_REQUEST['ppeid'])?'Editar Período':'Novo Período';
//Se existir o envio do formulario envia os dados para persistir

$periodoPreenchimento = new Spoemendas_Controller_Periodopreenchimentoemenda();
$msg = new Simec_Helper_FlashMessage('spoemendas/periodoreferencia');
if ($_POST['configperiodo']) {
    $_SESSION['camposPeriodo'] = $_POST['configperiodo'];
    $ppe = $periodoPreenchimento->salvar($_POST['configperiodo']);
    header("Location: spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Periodo&novo=true&ppeid=".$ppe);
    die;
}
//Resultados primarios
$rp = $periodoPreenchimento->getResultadoPrimario();

$dados = $periodoPreenchimento->getPeriodoPreenchimento($_REQUEST['ppeid']);

//Controle acesso de abas do perfil
$modelAcessoPerfis = new Spoemendas_Controller_Acessoperfilabas();

//Busca os perfis para selecao
$perfis = $modelAcessoPerfis->getPerfis();

//Busca as formas de execucao para selecao
$formaExecucao = $modelAcessoPerfis->getFormaExecucao();
echo $msg;

if (!$_GET['ppeid']) {
    $dados = $_SESSION['camposPeriodo'];
} else {
    unset($_SESSION['camposPeriodo']);
}
//Formulario para cadastrar acesso a abas
$form = new Simec_View_Form('configperiodo');
$form->setTitulo($titulo);
$form->carregarDados($dados);
$form->addHidden('prsano',$_SESSION['exercicio'],'prsano',[])
    ->addCombo('Perfil','pflcod',$perfis,[])
    ->addCombo('Forma de Execução','etoid',$formaExecucao,[])
    ->addHidden('ppeid',$dados['ppeid'])
    ->addCombo('Resultado Primario','pperesultprimario',$rp,[])
    ->addData('Período','ppedatainicio',[])
    ->addData('Até','ppedatafim',[])
    ->addSeparador('<b>Tags de Substituição para Mensagem: </b>[_DATA_INICIAL_]&nbsp;&nbsp;&nbsp;[_DATA_FINAL_]&nbsp;&nbsp;&nbsp;[_DIAS_RESTANTES_]', ['tag' => 'h5 style="margin-bottom: 25px;"', 'classes' => 'col-lg-offset-2'])
    ->addTextarea('Mensagem','ppemensagem','500')
    ->addBotoes(['cancelar', 'salvar'])
    ->setRequisicao('salvar')
    ->render();

?>
<script>
    $('.btn-cancelar').click(function(){
        $(location).attr('href', 'spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Periodo');
    });
    $('#configperiodo_ppedatainicio').attr('placeholder','    /   /     ');
    $('#configperiodo_ppedatafim').attr('placeholder','    /   /     ');
    $('#configperiodo_ppedatainicio').mask('##/##/####');
    $('#configperiodo_ppedatafim').mask('##/##/####');
</script>
<style>
    .well,.tb_class_render,.tabela-listagem{
        width: 98%;
        margin-left: 15px;
    }
    #configperiodo_rp_chosen{
        width: 228px!important;
    }
    #configperiodo_ppedatainicio,#configperiodo_ppedatafim{
        width: 100px!important;
    }

</style>
