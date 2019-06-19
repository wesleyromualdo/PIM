<?php
/**
 * Created by PhpStorm.
 * User: jefersonaraujo
 * Date: 23/06/2017
 * Time: 15:10
 */

include APPRAIZ . "includes/cabecalho.inc";
//Controle acesso de abas do perfil
$modelAcessoPerfis = new Spoemendas_Controller_Acessoperfilabas();
if($_POST['request'] == 'invativaabas'){
    ob_clean();
    $modelAcessoPerfis->inativar($_POST['apaid']);
    die;

}

//Sql de busca dos dados
$filtro = (isset($_POST['buscaabas'])) ? $_POST['buscaabas'] : false;
$sql = $modelAcessoPerfis->getPerfilAbasList($filtro);

//Busca os perfis para selecao
$perfis = $modelAcessoPerfis->getPerfis();

//Busca as formas de execucao para selecao
$formaExecucao = $modelAcessoPerfis->getFormaExecucao();

//Formulario para cadastrar acesso a abas
$form = new Simec_View_Form('buscaabas');
$form->carregarDados($filtro);
$form->addCombo('Perfil', 'pflcod', $perfis, [])
    ->addCombo('Forma de Execução', 'etoid', $formaExecucao, [])
    ->addBotoes(['limpar', 'buscar', 'novo'])
    ->setRequisicao('filtro')
    ->render();

$arrColunas = [
    'Perfil',
    'Forma de Execução',
    'Abas'
];
$listagem = new Simec_Listagem();
$listagem->addAcao('delete', 'invativaAba');
$listagem->addAcao('edit', 'alterarAba');
$listagem->turnOnOrdenacao();
$listagem->setCabecalho($arrColunas)
    ->setQuery($sql);
$listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
?>
<style>
    .well,.tb_class_render,.tabela-listagem{
        width: 98%;
        margin-left: 15px;
    }

</style>
<script>
    //Redirecionar para cadastrar um novo registro para abas
   function onBuscaabasNovo(){
       <?php unset($_SESSION['camposAbas'])?>
       $(location).attr('href', 'spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Abas&novo=true');
   }
   function alterarAba(apaid){
       $(location).attr('href', 'spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Abas&novo=true&apaid='+apaid);
   }
    function invativaAba(apaid) {
        bootbox.confirm('Deseja realmente Excluir o registro selecionado?', function (confirmacao) {
            if (confirmacao) {
                $.post("", {request: 'invativaabas', apaid: apaid}, function (data, retorno) {
                    console.log(data, retorno);
                    if (retorno == 'success') {
                        location.reload();
                    }
                });
            }
        });
    }
</script>
