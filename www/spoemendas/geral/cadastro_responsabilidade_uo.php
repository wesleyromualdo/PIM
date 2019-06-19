<?php
// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require (APPRAIZ . 'www/acomporc/_constantes.php');
require_once (APPRAIZ . 'includes/library/simec/Listagem.php');
include APPRAIZ . "includes/funcoesspo.php";
$db = new cls_banco();

function gravarResponsabilidadeUO($dados) {
    global $db;
    $sql = <<<DML
        UPDATE spoemendas.usuarioresponsabilidade SET rpustatus = 'I'
        WHERE usucpf = '{$dados['usucpf']}'
            AND pflcod='{$dados['pflcod']}'
DML;
    $db->executar($sql);

    if ($dados['usuacaresp']) {
        foreach($dados['usuacaresp'] as $unicod) {

            // -- Removendo outras permissões da UO para o perfil monitor interno - só deve haver um usuário por uo
            if (PFL_MONITOR_INTERNO == $dados['pflcod']) {
                $sql = <<<DML
UPDATE spoemendas.usuarioresponsabilidade
  SET rpustatus = 'I'
  WHERE pflcod = '{$dados['pflcod']}'
    AND unicod = '{$unicod}'
DML;
                $db->executar($sql);
            }

            $sql = <<<DML
                INSERT INTO spoemendas.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, unicod)
                VALUES ('{$dados['pflcod']}', '{$dados['usucpf']}', 'A', NOW(), '{$unicod}')
DML;
            $db->executar($sql);
        }
    }

    $db->commit();

    echo "
        <script language=\"javascript\">
            alert(\"Operação realizada com sucesso!\");
            opener.location.reload();
            self.close();
        </script>";
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
?>
<html>
    <head>
        <meta http-equiv="Pragma" content="no-cache">
        <title>Definição de responsabilidades - Unidades Orçamentárias</title>
        <script language="JavaScript" src="/includes/funcoes.js"></script>
        <script src="/library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="/library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="/library/bootstrap-3.0.0/js/bootstrap.min.js" type="text/javascript" charset="utf-8"></script>
        <link rel="stylesheet" href="/library/bootstrap-3.0.0/css/bootstrap.css">
        <link rel='stylesheet' type='text/css' href='/includes/loading.css'/>
    </head>
    <body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff" onload="self.focus()">
        <script>
            function marcarUO(obj) {
                if(obj.checked) {
                    if (!jQuery('#usuacaresp option[value='+obj.value+']')[0]) {
                        jQuery("#usuacaresp").append('<option value='+obj.value+'>'+obj.parentNode.parentNode.cells[1].innerHTML+'</option>');
                    }
                } else {
                    jQuery('#usuacaresp option[value='+obj.value+']').remove();
                }
            }

            $(document).ready(function() {
                $('#ckboxPai').on('click',function(){
                    $('#loading').show();
                    if($('#ckboxPai').prop('checked')){
                        if($('#textFind').val().trim() != ''){
                            $('table td[class=listagem-marcado]').prev().find('input:not(":checked")').each(function(){
                                $(this).click();
                            });
                        }else{
                            $('table td input:not(":checked")').each(function(){
                                $(this).click();
                            });
                        }
                    }else{
                        if($('#textFind').val().trim() != ''){
                            $('table td[class=listagem-marcado]').prev().find('input:checked').each(function(){
                                $(this).click();
                            });
                        }else{
                            $('table td input:checked').each(function(){
                                $(this).click();
                            });
                        }
                    }
                    $('#loading').hide();
                });
            });

            jQuery(function(){
                jQuery("#ckboxPai").on("click", function(){
                    if (jQuery(this).prop("checked") == true) {
                        jQuery(".ckboxChild").prop("checked", true);

                        $(".ckboxChild").each(function(i, e){
                            $("select[multiple]").append('<option value="'+$(e).val()+'">'+$(e).val()+'</option>');
                        });
                    } else {
                        jQuery(".ckboxChild").prop("checked", false);
                        $(".ckboxChild").each(function(i, e){
                            $("select[multiple]").html("");
                        });
                    }
                });
            });
        </script>
        <section style="overflow:auto;width:100%;height:450px;border:2px solid #ececec;background-color:white;">
            <section class="container">
                <!-- begin loader -->
		<div class="loading-dialog notprint" id="loading">
                    <div id="overlay" class="loading-dialog-content">
                        <div class="ui-dialog-content">
                            <img src="/library/simec/img/loading.gif">
                                <span>
                                    O sistema esta processando as informações. <br/>
                                    Por favor aguarde um momento...
                                </span>
                        </div>
                    </div>
                </div>
            </section>
        
<?php
            // -- É feita uma verificação no SQL para saber se aquele ungcod já foi escolhido previamente
            // -- com base nisso, é adicionado o atributo checked ao combo do unicod selecionado previamente.
            $unidadesObrigatorias = UNIDADES_OBRIGATORIAS;
            $sql = <<<DML
                SELECT
                    '<input type="checkbox" class="ckboxChild" name="unicod" id="chk_' || uni.unicod || '" value="' || uni.unicod || '" '
                        || 'onclick="marcarUO(this)"'
                        || case WHEN (SELECT count(urp.rpuid) FROM spoemendas.usuarioresponsabilidade urp WHERE urp.unicod = uni.unicod AND urp.usucpf = '{$usucpf}' AND urp.pflcod = '{$pflcod}' AND rpustatus = 'A') > 0 THEN ' checked' ELSE '' END || '>' AS unicod,
                    uni.unicod || ' - ' || uni.unidsc AS descricao
                FROM public.unidade uni
                WHERE uni.unistatus = 'A'
                    AND orgcod = '26000'
                ORDER BY uni.unicod
DML;
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);            
            $listagem->turnOnPesquisator();            
            $listagem->setTitulo('Definição de responsabilidades - Unidade Orçamentária');
            $listagem->setCabecalho(array("<input type=\"checkbox\" id=\"ckboxPai\">","UO / Descrição"));
            $listagem->setQuery($sql);
            $listagem->addCallbackDeCampo('descricao', 'alinhaParaEsquerda');
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
        </section>
        <div style="overflow:visible;width:100%;height:40px;border:2px solid #ececec;background-color:white;">
        <section class="container">
            <input type="Button" class="btn btn-success" name="ok" value="Salvar"
                    onclick="selectAllOptions(document.getElementById('usuacaresp'));document.formassocia.submit();"
                    id="ok">
        </section>
        </div>
        <section style="overflow:auto;width:100%;height:200px;border:2px solid #ececec;background-color:white;">
            <form name="formassocia" class="form-horizontal" method="POST">
                <input type="hidden" name="usucpf" value="<?=$usucpf?>">
                <input type="hidden" name="pflcod" value="<?=$pflcod?>">
                <input type="hidden" name="requisicao" value="gravarResponsabilidadeUO">
                <section class="form-group">
                    <label class="control-label col-md-2" for="usuacaresp">Opções Marcadas</label>
                    <section class="col-md-10">
                        <select multiple size="8" name="usuacaresp[]" id="usuacaresp" class="form-control">
                        <?php
                        $sql = <<<DML
                            SELECT uni.unicod AS codigo,
                                uni.unicod || ' - ' || uni.unidsc AS descricao
                            FROM spoemendas.usuarioresponsabilidade ur
                            INNER JOIN public.unidade uni USING(unicod)
                            WHERE ur.usucpf = '{$usucpf}'
                                AND ur.pflcod = '{$pflcod}'
                                AND ur.rpustatus = 'A'
                                AND uni.unicod IN ($unidadesObrigatorias)
DML;
                        $usuarioresponsabilidade = $db->carregar($sql);

                        if($usuarioresponsabilidade[0]) {
                            foreach($usuarioresponsabilidade as $ur) {
                                echo '<option value="'.$ur['codigo'].'">'.$ur['descricao'].'</option>';
                            }
                        }
?>
                        </select>
                    </section>
                </section>
            </form>
        </section>
    </body>
</html>