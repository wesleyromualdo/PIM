<?php
include "config.inc";
header('content-type: text/html; charset=utf-8');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

if(count($_REQUEST['supobs'])>0) {
	foreach($_REQUEST['supobs'] as $key => $descricao) {
		if(is_numeric($key)) {
			$sql = "UPDATE public.arquivo SET arqdescricao='".simec_addslashes(substr($descricao,0,255))."' WHERE arqid='".$key."'";
			$db->executar($sql);
			$db->commit();
		} else {
			$fp = fopen("../../../arquivos/obras2/imgs_tmp/".$key.".d", 'w');
			fwrite($fp, $descricao);
			fclose($fp);
			echo "<script>parent.inserirobservacaothumnails('".$key."','".simec_addslashes($descricao)."');</script>";
		}
	}
}


if(count($_REQUEST['icoid'])>0) {
    foreach($_REQUEST['icoid'] as $key => $descricao) {
        if(is_numeric($key)) {
            $sql = "UPDATE obras2.fotos SET icoid='".$descricao."' WHERE arqid='".$key."'";
            $db->executar($sql);
            $db->commit();
        } else {
            $fp = fopen("../../../arquivos/obras2/imgs_tmp/".$key.".e", 'w');
            fwrite($fp, $descricao);
            fclose($fp);
        }
    }
}


echo "<script>
		alert('Observações gravadas com sucesso.');
		</script>";
?>