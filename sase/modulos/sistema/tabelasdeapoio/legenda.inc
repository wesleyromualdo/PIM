<?php
    if($_REQUEST['requisicao'] == 'legendaSub'){
        ob_clean();
        ob_flush();
        echo carregarLegendaSub($_REQUEST['subiddial']);
        echo exibirMeta($_REQUEST['meta']);
        die();
    }

    function carregarLegendaSub($subiddial) {
        global $db;
        if($subiddial){
            $sql = "SELECT distinct subtitulo
                    FROM sase.legendaindicadores leg
                    INNER JOIN sase.submetadial sub ON leg.subiddial = sub.subiddial
                    WHERE leg.subiddial = '{$subiddial}';";
            $subtitulo = $db->pegaUm($sql);
            return $subtitulo;
        }else{
            return false;
        }
    }
    function exibirMeta($subiddial){
            if($subiddial){
                global $db;
                $sqlmeta = " SELECT distinct met.metchamada
                    FROM sase.legendaindicadores leg
                    INNER JOIN sase.submetadial sbd USING(subiddial)
                    INNER JOIN sase.meta met USING(metid)
                    where sbd.subiddial = '{$subiddial}';"; 
                $meta = $db->pegaUm($sqlmeta);
                return $meta;
            }else{
                return false;
            }
    }
 global $db;
include_once '../../sase/classes/Legenda.class.inc';
$legenda = new Legenda();
$pfls     = arrayPerfil();

if($_POST['acao']){
    ob_clean();
    switch($_POST['acao']){
        case 'salvar':
            $legenda->carregarPorId($_POST['legid']);
            $legenda->legid = $_POST['legid'];
            $legenda->subiddial = $_POST['subiddial'];
            $legenda->legfxa1 = str_replace(',', '.', $_POST['legfxa1']);
            $legenda->legfxa2 = str_replace(',', '.', $_POST['legfxa2']);
            $legenda->legcor = $_POST['legcor'];
            $legenda->alterar();
            $db->commit();
            $_REQUEST['legfxa1'] = str_replace(',', '.', $_POST['legfxa1']);
            $_REQUEST['legfxa2'] = str_replace(',', '.', $_POST['legfxa2']);
            echo "
                <script>
                    alert('Legenda alterada com sucesso.');
                    window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/legenda&acao=A';
                </script>";
            die();
        case 'apagar':
            if (!$legenda->excluir($_POST['legid'])) {
                echo "<script>alert('Não foi possível excluir a legenda selecionada.');</script>";
            } else {
                $legenda->commit();
                echo "<script>alert('Legenda removida com sucesso.');</script>";
            }
            echo "<script>window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/legenda&acao=A';</script>";
            die();
        case 'edicao':
            $legenda->carregarPorId($_POST['legid']);
            $arr = array(
                'subiddial' => $legenda->subiddial,
                'legfxa1' => $legenda->legfxa1,
                'legfxa2' => $legenda->legfxa2,
                'legcor' => $legenda->legcor
            );
            echo simec_json_encode($arr);
            exit;
        case 'novo':
            $legenda->popularDadosObjeto($_POST);
            if (!$legenda->salvar()) {
                echo "<script>alert('Não foi possível criar a nova legenda.');</script>";
            } else {
                $legenda->commit();
                echo "<script>alert('Nova legenda criada com sucesso.');</script>";
            }
            echo "<script>window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/legenda&acao=A';</script>";
            die();
    }
}

include APPRAIZ . 'includes/cabecalho.inc';


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Legenda de Indicadores';
?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
    <div id='cabecalhoSistema'>
        <?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
        <center><h3><?=$titulo?></h3></center>
    </div>
</div>

<script>
      jQuery(document).ready(function() {
         <?php if($_REQUEST['subiddial']){ ?>
                 pegaLegendaSub(<?php echo $_POST['subiddial']?>);
         <?php } ?>
      });
    function editLegenda(legid){
        jQuery.ajax({
            url: '',
            type: 'POST',
            data:{acao:'edicao',legid:legid},
            success: function(data){
                var d = JSON.parse(data);
                jQuery('#legid').val(legid);
                jQuery('#subiddial').val(d['subiddial']).removeAttr('disabled');
                jQuery('#legfxa1').val(d['legfxa1']).removeAttr('disabled');
                jQuery('#legfxa2').val(d['legfxa2']).removeAttr('disabled');
                jQuery('#legcor').val(d['legcor']).removeAttr('disabled');
                jQuery('#btnSalvar').removeAttr('disabled');
                jQuery('#btnPesquisar').attr('disabled', true);
                jQuery('#btnNovo').attr('disabled', true);
                jQuery('#btnClear').removeAttr('disabled');
                pegaLegendaSub(d['subiddial']);
                exibeMeta(d['subiddial']);
            }
        });
    }

    function excluirLegenda(legid)
    {
        if (confirm('Tem certeza que deseja remover a legenda selecionada?')) {
            jQuery('#acao').val('apagar');
            jQuery('#legid').val(legid);
            jQuery('#formLegenda').submit();
        }
    }

    function salvaLegenda(){
        jQuery('#acao').val('salvar');
        jQuery('#formLegenda').submit();
    }

    function pesquisaLegenda(){
        jQuery('#acao').val('pesquisa');
        jQuery('#formLegenda').submit();
    }

    function pegaLegendaSub(id){
          jQuery.ajax({
            url: 'sase.php?modulo=sistema/tabelasdeapoio/legenda&acao=A',
            type: 'POST',
            data:{requisicao:'legendaSub',subiddial:id},
            success: function(data){
                jQuery('#legendaSub').html(data);
            }
        });
    }
    function exibeMeta(id){
          jQuery.ajax({
            url: 'sase.php?modulo=sistema/tabelasdeapoio/legenda&acao=A',
            type: 'POST',
            data:{requisicao:'legendaSub',meta:id},
            success: function(data){
                jQuery('#meta').html('<label class="col-sm-2 control-label" for="filtroStadsc">Meta:</label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+data);
            }
        });
    }

    jQuery(function(){
        jQuery('#btnNovo').click(function(){
            if (confirm('Confirma a adição da nova legenda?')) {
                var msg = new Array();
                if (!jQuery('#subiddial').val()) {
                    msg.push('Indicador');
                }
                if (!jQuery('#legfxa1').val()) {
                    msg.push('Faixa 1');
                }
                if (!jQuery('#legfxa2').val()) {
                    msg.push('Faixa 2');
                }
                if (!jQuery('#legcor').val()) {
                    msg.push('Cor');
                }

                if (msg.length) {
                    alert('Antes de prosseguir, os seguintes campos devem ser preenchidos: ' + msg.join(', ') + '.');
                    return false;
                }
            }

            jQuery('#acao').val('novo');
            jQuery('#formLegenda').submit();
        });
    });
</script>
<div id="container">
    <div id="formularioFiltros">
        <form class="form-horizontal" name="formLegenda" id="formLegenda" role="form" method="POST">
            <input type="hidden" name="acao" id="acao" value=""/>
            <input type="hidden" name="legid" id="legid" value="<?=($_POST['legid'])?$_POST['legid']:''?>"/>

                <div class="form-group" id="divFiltroMunicipio">
                    <div class="col-sm-6 inputarea" >
                        <div id="meta" style="margin-left: 119px"></div>
                    </div>
                </div>
            <div class="form-group" id="divFiltroMunicipio">
                <label for="filtroStadsc" class="col-sm-2 control-label">Indicador:</label>
                <div class="col-sm-9 inputarea">
                    <input name="subiddial" id="subiddial" style="width: 100px;" <?php //if(!$_POST['acao']){ echo 'disabled';} ?> value="<?= ($_REQUEST['subiddial']) ? $_REQUEST['subiddial'] : '' ?>" onchange="pegaLegendaSub(this.value);"/>
                    <div id="legendaSub"></div>
                </div>
            </div>
             <div class="form-group" id="divFiltroMunicipio">
                <label for="filtroStadsc" class="col-sm-2 control-label">Faixa 1:</label>
                <div class="col-sm-9 inputarea">
                    <input name="legfxa1" id="legfxa1" style="width: 100px;" <?php //if(!$_POST['acao']){ echo 'disabled';}?> value="<?=(($_REQUEST['legfxa1'] != null) && $_REQUEST['legfxa1']>= 0)?$_REQUEST['legfxa1']:''?>"/>
                </div>
            </div>
             <div class="form-group" id="divFiltroMunicipio">
                <label for="filtroStadsc" class="col-sm-2 control-label">Faixa 2:</label>
                <div class="col-sm-9 inputarea">
                    <input name="legfxa2" id="legfxa2" style="width: 100px;" <?php //if(!$_POST['acao']){ echo 'disabled';}?> value="<?=(($_REQUEST['legfxa2'] != null) && $_REQUEST['legfxa2']>= 0)?$_REQUEST['legfxa2']:''?>"/>
                </div>
            </div>
            <div class="form-group" id="divFiltroMunicipio">
                <label for="filtroStadsc" class="col-sm-2 control-label">Cor:</label>
                <div class="col-sm-9 inputarea">
                     <input name="legcor" id="legcor" style="width: 100px;" value="<?=($_POST['legcor'])?$_POST['legcor']:''?>"/>
                </div>
            </div>

            <div class="form-group">
                <label class="col-sm-2 control-label">&nbsp;</label>
                <div class="col-sm-9">
                    <input type="button" class="btn btn-primary" <?php if($_REQUEST['legid']){ echo 'disabled';}?> onclick="novaLegenda()" id="btnNovo" value="Novo" />
                    <input type="button" class="btn btn-primary" <?php if(!$_REQUEST['legid']){ echo 'disabled';}?> onclick="salvaLegenda()" id="btnSalvar" value="Salvar" />
                    <input type="button" class="btn btn-primary" <?php if($_REQUEST['legid']){ echo 'disabled';}?>  onclick="pesquisaLegenda()" id="btnPesquisar" value="Pesquisar" />
                    <input type="reset" class="btn btn-primary"  <?php //if(!$_POST['acao']){ echo 'disabled';}?> id="btnClear" value="Limpar" onclick="window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/legenda&acao=A';" />
                </div>
            </div>
        </form>
    </div>
    <div id="lista">
        <?php $legenda->getLista('B'); ?>
    </div>
</div>