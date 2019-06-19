<?php
/**
 * Janela de comentário para o workflow do bootstrap.
 *
 * @version $Id: alterar_estado_comentario_bootstrap.php 110078 2016-04-13 18:08:26Z maykelbraz $
 */

// inicializa sistema
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'includes/funcoesspo.php';
include_once APPRAIZ . "includes/workflow.php";
if (!$db) {
	$db = new cls_banco();
}

$docid = (integer)$_REQUEST['docid'];
$esdid = (integer)$_REQUEST['esdid'];
$aedid = (integer)$_REQUEST['aedid'];
$verificacao = stripcslashes(removeAcentos((string)$_REQUEST['verificacao']));

$documento = wf_pegarDocumento( $docid );
$acao = wf_pegarAcao( $documento['esdid'], $esdid );
if (!$documento || !$acao) {
    die('Documento ou ação inválida.');
}
?>
<style type="text/css">
#modal-alert{z-index:10000}
</style>
<form method="post" action="http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php"
      id="form-comentario-workflow" name="combo_alterar_estado_comentario">

    <input type="hidden" name="docid" value="<?php echo $docid ?>"/>
    <input type="hidden" name="esdid" value="<?php echo $esdid ?>"/>
    <input type="hidden" name="aedid" value="<?php echo $aedid ?>"/>
    <input type="hidden" name="bs" value="s" />
    <input type="hidden" name="verificacao" value="<?php echo htmlentities($verificacao) ?>" />

    <table class="table table-bordered table-condensed">
        <thead>
            <tr>
                <th colspan="2"><?php echo $documento['docdsc']; ?></th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td style="text-align:right;font-weight:bold;width:30%">Estado atual:</td>
                <td width="80%"><?php echo $acao['esddscorigem']; ?></td>
            </tr>
            <tr>
                <td style="text-align:right;font-weight:bold;width:30%">Ação:</td>
                <td><?php echo $acao['aeddscrealizar']; ?></td>
            </tr>
            <tr>
                <td style="text-align:right;font-weight:bold;width:30%">Comentário:</td>
                <td><?php inputTextArea('cmddsc', '', 'cmddsc', 6000); ?></td>
            </tr>
        </tbody>
    </table>
</form>

