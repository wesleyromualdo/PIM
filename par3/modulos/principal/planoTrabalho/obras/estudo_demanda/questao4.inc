<?php
//Algoritmo calculo conversão DD = d + (min/60) + (sec/3600)
$mOed = new Par3_Model_ObraEstudoDemanda();
$arrOed4 = $mOed->recuperarPorObra($_GET['obrid']);

if($_REQUEST['requisicao'] == 'recuperarpainelescola'){
    if(empty($_REQUEST['escid'])){
        ob_clean();
        die;
    }
    $mEsc = new Par3_Model_ObraEstudoDemandaXescola();
    $etaid = $_GET['etaid'];
    ob_clean();
    $arrEsc = $mEsc->recuperarEscolas($arrOed4['oedid'],$arrOed4['etaid']);
    $escid = array();
    foreach($arrEsc as $escola){
        $escid[] = $escola['escid'];
    }
    $escolas = $mEsc->listarEscolas(
            $arrOed4['oedid'],
            $_GET['etaid'],
            $_REQUEST['escid'],
            $_GET['inuid'],
            true,
            $escid
    );
    require 'escola_panel.inc';
    die;
}
if($_REQUEST['requisicao'] == 'removerescola'){
    $arr['escid'] = $_GET['escid'];
    $arr['oedid'] = $_GET['oedid'];
    $mEsc = new Par3_Model_ObraEstudoDemandaXescola();
    ob_clean();
    echo $mEsc->removerEscola($arr);
    die;
}
$mEsc = new Par3_Model_ObraEstudoDemandaXescola();

$mObra   = new Par3_Model_Obra($_GET['orbid']);
$esd     = wf_pegarEstadoAtual($mObra->docid);

$ordem = $arrOed2['omoid'] == 1 ? 5 : 6;
$mpergunta       = new Par3_Controller_AnaliseEngenhariaObraAbaPerguntas();
$disabledAnalise = $mpergunta->habilitarCamposEdicaoPorAbaOrdem(Par3_Model_ObraAba::ESTUDO_DEMANDA,$ordem,$_GET['obrid']);
$_GET['disabledanalise4'] = $disabledAnalise;
?>
<?php if($arrOed4['oedid']):?>
<div id="content-reordenamento" style="<?php echo ($arrOed4['omoid'] == 1)?'':'display: none;'?>">
    <h1 id="title-aba-4">Origem da demanda</h1>

    <?php if($arrOed2['omoid'] == 1) : ?>
        <input type="hidden" id="ordem-4" value="5">
        <div id="carregar-analise-5"></div>
    <?php endif;?>

    <h3 id="subtitle-aba-4">Considerando tratar-se de reordenamento, explicar a origem da demanda para justificativa da construção da nova escola, ou seja, de qual(is) escola(s) os alunos serão migrados.</h3>
    <div class="row" id="content-aba-4">
        <?php
        $uniid     = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $arrWhere  = $uniid->muncod ? array("muncod = '{$uniid->muncod}'"):array("muncod = '{$uniid->estuf}'");
        $etapa = '';
        if($_GET['etaid'] == 1){
            $arrWhere[] = 'escqtd_alunos_infantil > 0';
        }

        if($_GET['etaid'] == 2){
            $arrWhere[] = 'escqtd_alunos_fundamental > 0';
        }

        if($_GET['etaid'] == 3){
            $arrWhere[] = 'escqtd_alunos_medio > 0';
        }
        ?>
        <div class="col-lg-11">
            <?php
            $escolas = $mEsc->recuperarEscolas($arrOed4['oedid'],$arrOed4['etaid']);
            $escid = array();
            foreach($escolas as $escola){
                $escid[] = $escola['escid'];
            }
            $rsEsc = $mEsc->listarEscolas($arrOed4['oedid'],$_GET['etaid'],null,$_GET['inuid'],true,$escid);
            $rsEscolas = simec_preparar_array($rsEsc);
            if(!empty($rsEscolas)){
                echo $simec->select('escidselect', '',$escid, $rsEscolas, array('maxlength' => '255',$disabled,'title' => 'Selecione uma Escola','required','multiple'=>'multiple','placeholder' =>'Selecione',$disabledAnalise), array('input-size' => '12'));
            }
            ?>
        </div>
        <div>
            <?php if($disabledAnalise != 'disabled'):?>
            <?php if(!empty($rsEscolas)):?>
            <button type="button" class="btn btn-primary" placeholder="Adicionar" id="add-escola-btn"><span class="fa fa-plus"></span></button>
            <? endif; ?>
            <?php endif;?>

            <?php if(empty($rsEscolas)):?>
                <div class="col-lg-12 alert alert-info"><strong class="text-center">Não existem escolas disponíveis para esta modalidade de Ensino.Por favor, escolha outra justificativa no passo 1.</strong></div>
            <? endif; ?>
        </div>
        <br>
        <hr/>
    </div>
</div>

<div id="content-expansao" class="text-center" style="<?php echo ($arrOed4['omoid'] == 2)?'':'display: none;'?>">
    <h2 id="subtitle-aba-4">Justifique a necessidade de expansão de vagas:</h2>

    <?php if($arrOed2['omoid'] == 2) : ?>
        <input type="hidden" id="ordem-4" value="6">
        <div id="carregar-analise-6"></div>
    <?php endif;?>

    <div style="margin-top:80px;">
        <?php if($disabledAnalise != ''):?>
        <script>$('textarea').removeAttr('disabled'); $('textarea').attr('readonly', 'readonly');</script>
        <?php endif;?>
        <?php echo $simec->textarea('oedexpansao_justificativa_dsc', '', $arrOed4['oedexpansao_justificativa_dsc'], array('required','rows' => 5,$disabledAnalise), array('label-size' => '2', 'input-size' => '12')); ?>
    </div>
</div>
<!--        Painel das escolas são carregados aqui-->
<div class="col-lg-12" id="div-escola" style="<?php echo ($arrOed4['omoid'] == 1)?'':'display: none;'?>overflow: scroll;height:800px;">
    <?php
        $escolas = $mEsc->recuperarEscolas($arrOed4['oedid'],$_GET['etaid']);
        if($escolas){
            require_once 'escola_panel.inc';
        }
    ?>
</div>
<?php endif;?>
<script>
    var arrEscid = $.isArray($('#escidselect').val()) ? $('#escidselect').val(): [];
    //Mudança do motivo da construção questão 1
    $(document).on('change','#omoid',function(){
        if($(this).val() == 2) {
            $('#content-reordenamento').hide();
            $('#content-expansao').show();
            $('#div-escola').html('');
            $('#div-escola').hide();
        }else{
            $('#content-reordenamento').show();
            $('#content-expansao').hide();
            $('#div-escola').show();
        }
    });
    //**********************************************************************
    //Lista as escolas adicionadas

    $('#add-escola-btn').on('click',function(e){
        var escidselect = [];
        escidselect = $.isArray($('#escidselect').val()) ? $('#escidselect').val(): [];
        if(arrEscid.length != 0){
            escidselect = array_diff(escidselect,arrEscid);
        }

        var arrDiferenca = array_diff($('#escidselect').val(),arrEscid);
        if(arrDiferenca.length == 0){
            return false;
        }
        $('#loading').hide();
        var caminho = window.location.href;
        var action  = 'requisicao=recuperarpainelescola&escid='+arrDiferenca+'&etaid='+'<?php echo $_GET['etaid']; ?>';
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (panel) {
                $('#loading').hide();
                $('#div-escola').append(panel);
                arrEscid = [];
                $.each($('#escidselect').val(),function(index,data){
                    arrEscid.push(data);
                });
            }
        });
    });

$(document).on('click','.removerEscola',function(){
    var escid = $(this).attr('id');
    var oedid = $(document).find('[name=oedid]').val();
    removerEscola(escid,oedid);
    arrEscid = jQuery.grep(arrEscid, function(value) {
        return value != escid;
    });
});


    function removerEscola(escid,oedid) {
        $('#loading').hide();
        var caminho = window.location.href;
        var action  = 'requisicao=removerescola&escid='+escid+'&oedid='+oedid;
        $.ajax({
            type: "GET",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $('#loading').hide();
            }
        });
    }

    function array_diff(A, B) {
        var diferenca = [];
        $.each(A,function(index,data){
            if ($.inArray(data, B) == -1){ diferenca.push(data);}
        });
        return diferenca;
    }
    //***********************************************************************
</script>