<?php
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if ($_REQUEST ['download'] == 'S') {
    ob_clean();

    (new FilesSimec(null,null,$_REQUEST['esquema']))
        ->getDownloadArquivo( $_REQUEST ['arqid'] );

    exit();
}