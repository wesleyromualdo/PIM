<?php

include_once APPRAIZ . 'www/par/_constantes.php';
include_once APPRAIZ . 'seguranca/modulos/monitoramento/TestWs.php';

$testWs = new TestWs();
$contagemLoop = 3;
$contagemValidacao = 2;

$htmlRetorno = '';
/********* TESTE - 01 *****************/
$arrParam = ['wsusuario' => WS_USUARIO_SIGEF, 'wssenha' => WS_SENHA_SIGEF, 'seq_conta_corrente' => '5377060'];
$htmlRetorno .= loopTest($testWs, 'consultaContaCorrente', $arrParam, $contagemLoop, $contagemValidacao);

/********* TESTE - 02 ****************/
$arrParam = ['wsusuario' => WS_USUARIO_SIGEF, 'wssenha' => WS_SENHA_SIGEF, 'nu_processo' => '23400000702201147'];
$htmlRetorno .= loopTest($testWs, 'consultaContaCorrenteProcesso', $arrParam, $contagemLoop, $contagemValidacao);

/********* TESTE - 03 ****************/
$arrParam = ['wsusuario' => WS_USUARIO_SIGEF, 'wssenha' => WS_SENHA_SIGEF, 'nu_seq_ne' => '381591'];
$htmlRetorno .= loopTest($testWs, 'consultarEmpenho', $arrParam, $contagemLoop, $contagemValidacao);

/********* TESTE - 04 ****************/
$arrParam = ['wsusuario' => WS_USUARIO_SIGEF, 'wssenha' => WS_SENHA_SIGEF, 'nu_seq_ob' => '1086162'];
$htmlRetorno .= loopTest($testWs, 'consultarPagamento', $arrParam, $contagemLoop, $contagemValidacao);

/********* TESTE - 05 ****************/
$arrParam = ['wsusuario' => WS_USUARIO_SIGEF, 'wssenha' => WS_SENHA_SIGEF, 'nu_seq_ob' => '1086162'];
$htmlRetorno .= loopTest($testWs, 'testarCpf', $arrParam, $contagemLoop, $contagemValidacao);


function loopTest($testWs, $method, $arrParam, $contagemLoop, $contagemValidacao)
{
    $cont = 0;
    for ($i = 0; $i <= $contagemLoop; $i++) {
        $html1 = $testWs->$method($arrParam);
        if (!empty($html1)) {
            $cont++;
        }
    }
    if ($cont >= $contagemValidacao) {
        return $html1;
    }
}

if (!empty($htmlRetorno)) {
    echo <<<HTML
<table width="100%" border="1" cellspacing="0" cellpadding="0">
    <thead>
        <tr>
            <th> Código de Teste </th>
            <th> Sistemas/Módulos Afetados </th>
            <th> Responsáveis Técnicos </th>
            <th> Descrição do Teste </th>
            <th> Código do Erro </th>
            <th> Mensagem do Erro </th>
            <th> Data Hora </th>
        </tr>
     </thead>
       <tbody>
        {$htmlRetorno}
    </tbody>    
</table>
HTML;
}

