<?php
/**
 * Created by PhpStorm.
 * User: jefersonaraujo
 * Date: 23/06/2017
 * Time: 15:10
 */

include APPRAIZ . "includes/cabecalho.inc";
$flashMensagem = new Simec_Helper_FlashMessage('spoemendas/periodoreferencia');
//Controller periodo de preenchimento
$periodoPreenchimento = new Spoemendas_Controller_Periodopreenchimentoemenda();

if ($_POST['request'] == 'invativaPeriodo') {
    ob_clean();
    $periodoPreenchimento->inativar($_POST['ppeid']);
    die;
}
//Controle acesso de abas do perfil
$modelAcessoPerfis = new Spoemendas_Controller_Acessoperfilabas();

//Sql de busca dos dados
$filtro = (isset($_POST['periodos'])) ? $_POST['periodos'] : false;

//Busca os perfis para selecao
$perfis = $modelAcessoPerfis->getPerfis();

//Busca as formas de execucao para selecao
$formaExecucao = $modelAcessoPerfis->getFormaExecucao();

//Resultados primarios
$rp = $periodoPreenchimento->getResultadoPrimario();

//Periodos de preenchimento criados para listagem
$sql = $periodoPreenchimento->getPeriodosPreenchimento($filtro);
echo $flashMensagem;
//Formulario para filtrar periodos
$form = new Simec_View_Form('periodos');
$form->carregarDados($filtro);
$form->setTitulo('Filtro do Período');
$form->addCombo('Perfil', 'pflcod', $perfis, [])
    ->addCombo('Forma de Execução', 'etoid', $formaExecucao, [])
    ->addCombo('RP', 'pperesultprimario', $rp, [])
    ->addBotoes(['limpar', 'buscar', 'novo'])
    ->setRequisicao('filtro')
    ->render();

$arrColunas = [
    'Perfil',
    'Forma de Execução',
    'RP',
    'Período',
    'Mensagem'
];
$listagem = new Simec_Listagem();
$listagem->addAcao('delete', 'invativaPeriodo');
$listagem->addAcao('edit', 'alterarPeriodo');
$listagem->turnOnOrdenacao();
$listagem->setCabecalho($arrColunas)
    ->setQuery($sql);
$listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
$listagem->setTamanhoPagina(10)
?>
<style>
    .well,.tb_class_render,.tabela-listagem{
        width: 98%;
        margin-left: 15px;
    }
    #periodos_pperesultprimario{
        width: 20%!important;
    }

</style>
<script>
    //Redirecionar para cadastrar um novo registro para abas
   function onPeriodosNovo(){
       <?php unset($_SESSION['camposPeriodo'])?>
       $(location).attr('href', 'spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Periodo&novo=true');
   }
    function invativaPeriodo(ppeid) {
        bootbox.confirm('Deseja realmente Excluir o período selecionado?', function (confirmacao) {
            if (confirmacao) {
                $.post("", {request: 'invativaPeriodo', ppeid: ppeid}, function (data, retorno) {
                    console.log(data, retorno);
                    if (retorno == 'success') {
                        location.reload();
                    }
                });
            }
        });

    }
   function alterarPeriodo(ppeid){
       $(location).attr('href', 'spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Periodo&novo=true&ppeid='+ppeid);
   }
</script>
