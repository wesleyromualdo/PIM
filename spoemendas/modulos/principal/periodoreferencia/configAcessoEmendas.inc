<?php
/**
 * Arquivo de configuracao de abas que os perfis podem visualizar
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
(new Simec_View_Breadcrumb())
    ->add('Período Preenchimento Emenda')
    ->add((empty($_GET['aba'])) ? 'Periodo' : $_GET['aba'])
    ->render();
echo $fm;

//Abas para configuracao das abas e periodo
$abas = new Simec_Abas('spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A');
if ($_REQUEST['novo']) {
    $abas->adicionarAba('Periodo', '&nbsp; Período', dirname(__FILE__) . '/formPeriodoEmendas.inc', 'calendar');
} else {
    $abas->adicionarAba('Periodo', '&nbsp; Período', dirname(__FILE__) . '/listPeriodoPreenchimentoEmendas.inc', 'calendar');
}
//Verifica se for um novo cadastro, ou se vai para listagem
if ($_REQUEST['novo']) {
    $abas->adicionarAba('Abas', '&nbsp; Abas', dirname(__FILE__) . '/formAbasVisiveisEmendas.inc', 'tags');
} else {
    $abas->adicionarAba('Abas', '&nbsp; Abas', dirname(__FILE__) . '/listAbasVisiveisEmendas.inc', 'tags');
}
    $abas->definirAbaDefault('Periodo');
require $abas->render(true);
?>
