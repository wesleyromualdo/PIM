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

//Titulo para apresenta no formulario caso seja um novo ou edicao
$titulo = ($_REQUEST['apaid'])?'Editar Configuração':'Nova Configuração';

//Objeto de envio de mesangens
$msg = new Simec_Helper_FlashMessage('spoemendas/periodoreferencia');

//Se existir o envio do formulario envia os dados para persistir
if ($_POST['configabasvisiveis']) {
    $_SESSION['camposAbas'] = $_POST['configabasvisiveis'];
    $modelAcessoPerfis = new Spoemendas_Controller_Acessoperfilabas($_POST['configabasvisiveis']);
    $apaid = $modelAcessoPerfis->salvar();
    $modelAbasAcesso = new Spoemendas_Controller_Abasacesso($_POST['configabasvisiveis']);
    $modelAbasAcesso->salvar($apaid);
    $msg->addMensagem('Configuração realizada com sucesso!', 'success');
    header("Location: spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Abas&novo=true&apaid=".$apaid);
    die;
}


//Controle acesso de abas do perfil
$modelAcessoPerfis = new Spoemendas_Controller_Acessoperfilabas();

//Busca dos dados por Id
$dados = $modelAcessoPerfis->getPerfilAbasAcesso($_GET['apaid']);

//Busca os perfis para selecao
$perfis = $modelAcessoPerfis->getPerfis();

//Busca as formas de execucao para selecao
$formaExecucao = $modelAcessoPerfis->getFormaExecucao();
echo $msg;

if (!$_GET['apaid']) {
    $dados = $_SESSION['camposAbas'];
} else {
    unset($_SESSION['camposAbas']);
}
//Formulario para cadastrar acesso a abas
$form = new Simec_View_Form('configabasvisiveis');
$form->setTitulo($titulo);
$form->carregarDados($dados);
$form->addCombo('Perfil','pflcod',$perfis,[])
    ->addCombo('Forma de Execução','etoid',$formaExecucao,[])
    ->addHidden('apaid',$dados['apaid'],'apaid',[])
    ->addCheckbox('Iniciativas','iniciativa',['title' => 'Abas','valor'=>'iniciativa'])
    ->addCheckbox('Entidades Beneficiadas','beneficiarios',['Abas','valor'=>'beneficiarios'])
    ->addCheckbox('Passo 1: Responsáveis pela elaboração do Programa de Trabalho','responsaveis',['Abas','valor'=>'responsaveis'])
    ->addCheckbox('Passo 2: Indicação de Iniciativas','iniciativa-beneficiario',['Abas','valor'=>'iniciativa-beneficiario'])
    ->addCheckbox('Passo 3: Subações do PAR','subacoes',['Abas','valor'=>'subacoes'])
    ->addCheckbox('Programa de Trabalho / Impedimento','impedimentos',['Abas','valor'=>'impedimentos'])
    ->addCheckbox('Anexo de Ofícios','anexos-oficio',['Abas','valor'=>'anexos-oficio'])
    ->addBotoes(['cancelar', 'salvar'])
    ->setRequisicao('salvar')
    ->render();

?>
<script>
    //Atualiza o chosen do perfil para fica opcoes desabilitadas
    $('#configabasvisiveis_pflcod').trigger("chosen:updated");
    $('.btn-cancelar').click(function(){
        $(location).attr('href', 'spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Abas');
    });

</script>
<style>
    .well,.tb_class_render,.tabela-listagem{
        width: 98%;
        margin-left: 15px;
    }

</style>
