<?php

require_once 'config.inc';
var_dump(EXECUTAR_AUDITORIA);
echo '<br />';
var_dump(IS_TREINAMENTO);
echo '<br />';
var_dump(IS_PRODUCAO);
echo '<br />';
die;
include_once APPRAIZ . "includes/funcoes.inc";
ver(EXECUTAR_AUDITORIA, IS_TREINAMENTO, IS_PRODUCAO, d);
include_once APPRAIZ . "includes/classes_simec.inc";




if($_REQUEST['acao'] == 'E'){
    $db = new cls_banco();
    $sql = "insert into tarefa.entidadesolicitante (entid, esostatus) values('ABC', 'A')";

    $db->executar($sql);
    $db->close();

} else {
    number_format('', 2);
}

