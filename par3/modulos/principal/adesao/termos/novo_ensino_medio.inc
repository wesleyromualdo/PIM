<?php require_once APPRAIZ . 'par3/classes/controller/adesao_programa/novoEnsinoMedio.inc'; ?>

<script src="/par3/js/par3.js"></script>
<script>
function imprimir(acao)
{
    var caminho = window.location.href;
    var action = '&itrid=<?php echo $itrid ?>&requisicao='+acao+'&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>';
    $.ajax({
        type: "POST",
        url: caminho,
        data: action,
        async: false,
        success: function (resp) {
            $("#impressao-content").html(resp);
            var container = $("#impressao-content");
            var table = container.find('table');
            var th = container.find('th');
            var td = container.find('td');
            table.css('width', '100%')
            table.css('border-collapse', 'collapse');
            container.find(".popover,button,.modal").remove();
            container.find("table,td,th").css('border', '1px solid black');
            container.find('tr:even').css('background-color', 'silver');
            th.css('height', '50px');
            td.css('text-align', 'center');
            var w = window.open();
            w.document.write($('#div-impressao').html());
           // w.document.title = "Feirão de Programas - Termo de adesão ao programa Novo Mais";
            w.print();
            w.close();
            $("#impressao-content").html("");
            return true;
        }
    });
}
$(document).ready(function(){
    $('.aba_sintese').click(function(){
    	ajaxatualizar('requisicao=aba_sintese&itrid=<?php echo $itrid ?>&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>','tab-4');
    });
});
</script>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox">
            <div class="ibox-title">
                <div class="row">
                    <div class="col-md-12">
                        <h5>
                        Programa Novo Ensino Médio - 
                        <?php
                        echo $modalInstrumentoUnidade->inudescricao;
                        ?>
                        </h5>
                    </div>
                    <div class="">                        
                        <?php 
                            $arrayParams['itrid'] = $itrid;
                            $arrayParams['inuid'] = $inuid;
                            $arrayParams['adpid'] = $adpid;
                            $arrayParams['adpano_ciclo'] = $_SESSION['exercicio'];
                        ?>
                    </div>
                </div>
            </div>
            <div class="ibox-content" style="padding:20px">
                <div class="tabs-container">
                    <ul class="nav nav-tabs">
                        <li id="li1" class="<?php if($aceite['adpresposta'] != "S") echo "active"; ?>"><a data-toggle="tab" href="#tab-1" aria-expanded="true">Termo</a></li>
                        <?php 
                        if($esdid != WF_ESDID_NAO_INICIADO_PNEM && $esdid != ''){
                            ?>
                        <li id="li2" class="<?php if($aceite['adpresposta'] == "S" && $esdid == WF_ESDID_EMCADASTRAMENTO_PNEM) echo "active" ?>"><a data-toggle="tab" href="#tab-2" aria-expanded="false">Escolas</a>
                        <li id="li4" class="<?php if($esdid == WF_ESDID_ENVIADOAOMEC_PNEM) echo "active" ?> aba_sintese"><a data-toggle="tab" href="#tab-4" aria-expanded="false">Síntese</a>
                        <?php } ?>
                    </ul>

                    <div class="tab-content">
                        <div id="tab-1" class="tab-pane <?php if($aceite['adpresposta'] != "S") echo "active" ?>">                            
                            <?php require_once APPRAIZ."par3/modulos/principal/adesao/termos/novo_ensino_medio/aba_termo.php" ?>
                        </div>
                        <?php 
                        if($esdid != WF_ESDID_NAO_INICIADO_PNEM && $esdid != ''){ ?>
                            <div id="tab-2" class="tab-pane <?php if($aceite['adpresposta'] == "S" && $esdid == WF_ESDID_EMCADASTRAMENTO_PNEM) echo "active" ?>">
                                <?php require_once APPRAIZ."par3/modulos/principal/adesao/termos/novo_ensino_medio/aba_escolas.php" ?>
                            </div>
                            <div id="tab-4" class="tab-pane <?php if($esdid == WF_ESDID_ENVIADOAOMEC_PNEM) echo "active" ?>">
                                <?php require_once APPRAIZ."par3/modulos/principal/adesao/termos/novo_ensino_medio/aba_sintese.php" ?>
                            </div>
                        <?php } ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div id="div-impressao" style="display: none">
    <div id="div_cabecalho" class="row col-lg-offset-2">
        <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
            <tr bgcolor="#ffffff">
                <td valign="top" width="50" rowspan="2">
                    <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                </td>
                <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                    MEC/FNDE <br>
                    Feirão de Programas<br>
                </td>
                <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                    Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>
                    Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/>
                </td>
            </tr>
        </table>
        <hr style="color:black; width:95%; margin-left:-18px;">
    </div>
    <div class="clearfix" style="margin-bottom:10px;"></div>
    <div id="impressao-content">
    </div>
</div>