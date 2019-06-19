<?php
switch ($_REQUEST['requisicao']) {
    case 'salvar_regras_termo':
        ob_clean();
        $db->executar("UPDATE par3.configuracao_termo SET cotstatus = 'I' WHERE intoid = {$_REQUEST['intoid']}");
        $sql = "INSERT INTO par3.configuracao_termo(intoid, cotdiasinisolic, cotdiasfimsolic, usucpf, cotdata, cotstatus)
                VALUES({$_REQUEST['intoid']}, {$_REQUEST['cotdiasinisolic']}, {$_REQUEST['cotdiasfimsolic']}, '{$_SESSION['usucpf']}', now(), 'A')";
        $db->executar($sql);
        $db->commit();
        
        $urlM = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=listar_regra_termo';
        simec_redirecionar($urlM, 'success', 'Dados cadastrado com sucesso!');
        exit();
        die;
    break;
}
$arrDados = array();
$url = 'par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cadastrar_regra_termo';

$sqlObjeto = "SELECT intoid AS codigo, intodsc AS descricao FROM par3.iniciativa_tipos_objeto WHERE intostatus = 'A' ORDER BY intodsc";

?>
<div class="ibox">
<div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="center">Regras de Termos</h2>
            </div>
        </div>
    </div>
    <?php echo $simec->tab(criaAbaRegrasTermos(), $url);?>
    <div class="ibox-content">
        <div class="row">
            <form name="formDocumento" id="formDocumento" class="form form-horizontal" method="post">
                <input type="hidden" name="requisicao" id="requisicao" value="">
                <input type="hidden" name="cotid" id="cotid" value="<?php echo $arrDados['cotid']; ?>">
                <?php echo $simec->select('intoid', 'Tipo de Objeto', $arrDados['intoid'], $sqlObjeto, array('maxlength' => '255'), array('input-size' => '7')); ?>
                <?php echo $simec->input('cotdiasinisolic', 'Dias Inicio', $arrDados['cotdiasinisolic'], array('type' => 'number'), array('maxlength' => '255', 'input-size' => '7')); ?>
                <?php echo $simec->input('cotdiasfimsolic', 'Dias Fim', $arrDados['cotdiasfimsolic'], array('type' => 'number'), array('maxlength' => '255', 'input-size' => '7')); ?>
            </form>
        </div>
    </div>
    <div class="ibox-footer">
        <div class="center">
            <button type="button" class="btn btn-primary" id="salvar-modelo"><i class="fa fa-save"></i> Salvar</button>
        </div>
    </div>
     <div class="ibox-content">
        <div id="documentoListagem">
            <?php
                //$obDocumento->listar($_REQUEST);
            ?>
        </div>
    </div>
</div>
<script type="text/javascript">
    jQuery(document).ready(function(){
        
    });

    jQuery('#salvar-modelo').click(function(){
        var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): <br> <br>';
        var validacao = true;
        
        if( jQuery('[name="intoid"]').val() == '' ){
            mensagem += 'Tipo de Objeto <br>';
            validacao = false;
        }
        if( jQuery('[name="cotdiasinisolic"]').val() == '' ){
            mensagem += 'Dias Inicio <br>';
            validacao = false;
        }
        if( jQuery('[name="cotdiasfimsolic"]').val() == '' ){
            mensagem += 'Dias Fim <br>';
            validacao = false;
        }
        
        if( !validacao ){
            alert(mensagem);
            return false;
        }else{
            jQuery('[name="requisicao"]').val('salvar_regras_termo');
            jQuery('[name="formDocumento"]').submit();
        }
    });
    
</script>
