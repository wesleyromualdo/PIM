<?php


include_once "config.inc";
include_once APPRAIZ . "includes/VerImagem.php";
include_once APPRAIZ . 'includes/VerImagemException.php';
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/antiInject.inc";

$newwidth = (int)$_REQUEST['newwidth'];
$newheight = (int)$_REQUEST['newheight'];

$arqid = $_REQUEST['arqid'];

if(!is_numeric($arqid)){
    ?>
        <script>
            window.location.href="/logout.php";
        </script>
    <?
    exit ();
}
if (!empty($arqid)) {
    try {
        $verImagem = new VerImagem($arqid);

        if ($newwidth || $newheight) {
            $verImagem->setAltura($newheight);
            $verImagem->setLargura($newwidth);
            $verImagem->redimenciona();
        }

        $verImagem->redimencionarOuSelecionar();
//    TODO Adicionar marca d'agua nos thumbs
//    if (necessitaMarcaDaguaFNDE($arqid))
//        $verImagem->aplicaMarcaDAgua(APPRAIZ . 'www/imagens/logo_fnde.png');
        $verImagem->exibir();

        unset($verImagem);
    } catch (VerImagemException $e) {
        if ($e->getCode() == VerImagem::ERRO_FORMATO_NAO_SUPORTADO) {
            $file = new FilesSimec();
            $arquivo = $file->getDownloadArquivo($arqid);
        }
    }
}

function necessitaMarcaDaguaFNDE($arqid)
{
    global $db;
    $db = new cls_banco();

    include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";

    $objObra = new Obras();
    $resposta = $objObra->necessitaMarcaDaguaFNDE($arqid);
    return $resposta;
}
