<?php

if( $_REQUEST['requisicao'] == 'tramitar' ){
    
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    include_once APPRAIZ . "includes/library/vendor/phpoffice/phpexcel/Classes/PHPExcel/IOFactory.php";
    require_once APPRAIZ . 'includes/workflow.php';
    
    // VERIFICANDO CAMPO VAZIO
    if($_FILES['arquivocsv']['name']=="") {
        echo "<script>alert('Você deve selecionar um arquivo');history.back();</script>";
        exit;
    }
    $upload = new FilesSimec();
    $upload->setUpload('excel', '', false);
    $idArquivo = $upload->getIdArquivo();
    
    $pathxls = APPRAIZ.'arquivos/obras2/'.floor($idArquivo/1000).'/'.$idArquivo;
    $arExcel = PHPExcel_IOFactory::load($pathxls)->getActiveSheet()->toArray();
    $posicao = '';
    if( is_array($arExcel[0]) ){
        foreach ($arExcel[0] as $key => $col) {
            if( str_to_upper($col) == str_to_upper('obrid') ){
                $posicao = $key;
            }
        }
    }
    
    array_shift($arExcel);
    $total = 0;
    if($_FILES['arquivocsv']['tmp_name']){
        if( !empty($posicao) ){
            $usucpf = $_SESSION['usucpf'];
            $_SESSION['usucpf'] = '00000000191';
            foreach ($arExcel as $v) {
                $sql = "
                	SELECT DISTINCT o.obrid, d.docid, es.esdid, es.esddsc,
                		(SELECT (aedid) FROM workflow.acaoestadodoc WHERE esdidorigem = es.esdid AND esdiddestino = 769 AND aedstatus = 'A' LIMIT 1)
                	FROM obras2.obras o
                		INNER JOIN workflow.documento d ON d.docid = o.docid
                		INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                	WHERE o.obrid = '{$v[$posicao]}' and es.esdid not in (769)";
                $arrEstado = $db->pegaLinha($sql);
                if( $arrEstado['obrid'] ){
                    if( empty($arrEstado['aedid']) ){
                        $sql = "INSERT INTO workflow.acaoestadodoc(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada, esdsncomentario, aedposacao, aedvisivel, aedcodicaonegativa)
                                VALUES({$arrEstado['esdid']}, 769, 'Enviar para obra cancelada', 'A', 'Enviado para obra cancelada', true, 'posAcaoCancelarObraParEPac(obrid)', false, true) returning aedid";
                        $aedid = $db->pegaUm($sql);
                        $db->commit();
                    } else {
                        $aedid = $arrEstado['aedid'];
                    }
                    $arrParamWork = array('obrid' => $v[$posicao]);
                    
                    if (!wf_alterarEstado($arrEstado['docid'], $aedid, $_REQUEST['mensagem'], $arrParamWork)) {
                        ver($v, $arrEstado,d);
                    }
                    $total++;
                    
                    posAcaoCancelarObraParEPac( $arrEstado['obrid'] );
                } else{
                    if( empty($v[$posicao]) ){
                        posAcaoCancelarObraParEPac( $v[$posicao] );
                    }
                }
            }
            $_SESSION['usucpf'] = $usucpf;
        } else {
            echo "<script>alert('Não foi encontrada a coluna obrid!');
                window.location.href = 'obras2.php?modulo=principal/tramitarObraCancelada&acao=A';
            </script>";
            exit;
        }
    }
    echo "<script>alert('Obras processadas: ".$total."');
                window.location.href = 'obras2.php?modulo=principal/tramitarObraCancelada&acao=A';
            </script>";
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';

monta_titulo('Tramitar Obras para Cancelada', '');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<div class="row" style="position:static;">
    <form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
        <input type="hidden" name="requisicao" id="requisicao" value="">
        
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        	<tr>
                <td class="SubTituloDireita">Planilha:</td>
                <td><input type="file" name="arquivocsv" id="arquivocsv" accept=".xls, .xlsx"></td>
            </tr>
            <tr>
                <td class="SubtituloDireita">Comentário:</td>
                <td>
                    <textarea name="mensagem" style="width:100%;" rows="5"></textarea>
                </td>
            </tr>
            <tr>
                <td colspan="2" style="text-align: center">
                    <input type="button" value="Tramitar" id="tramitar" style="cursor:pointer;"/>
                </td>
            </tr>
        </table>
    </form>
</div>
<script>
    jQuery(document).ready(function () {
    	jQuery('#tramitar').on('click', function () {
    		jQuery('#requisicao').val('tramitar');
    		jQuery('#formulario').submit();
        });
    });
</script>
