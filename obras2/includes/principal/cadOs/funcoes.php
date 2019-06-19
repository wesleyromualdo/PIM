<?php 


function supervisaoCancelada($sosid, $empid)
{
    global $db;
    $sql = "SELECT empid FROM obras2.supervisaoempresa sue
            JOIN workflow.documento d ON d.docid = sue.docid
            WHERE sue.sosid = {$sosid} AND d.esdid = 1188 AND sue.empid = {$empid} AND sue.suestatus = 'A'";

        return $db->pegaUm($sql);

}

