<?php
$condicaoPorEscola = 0;
$controllerIniciativaPlanejamentoItemComposicao = new Par3_Controller_IniciativaPlanejamentoItemComposicao();

$modeloIniciativaPlanejamentoItemComposicao = new Par3_Model_IniciativaPlanejamentoItemComposicao();
if($inp->obrid){ //obras do proinfância
    $arDadosItens    = $modeloIniciativaPlanejamentoItemComposicao->dadosComboKitProinfancia($inp->inpid);
}else{
    $arDadosItens    = $modeloIniciativaPlanejamentoItemComposicao->dadosComboItens($inp->inpid);
}
$rsDadosItens    = $modeloIniciativaPlanejamentoItemComposicao->carregar($arDadosItens);
$arrDadosItens   = simec_preparar_array(!empty($rsDadosItens)?$rsDadosItens:array());
$modelIniciativaTipoObjeto = new Par3_Model_IniciativaTiposObjeto();
$modelIniciativaTiposAtendimento = new Par3_Model_IniciativaTiposAtendimento();

$labeal = 'Detalhamento';

$objeto = $controllerIniciativaPlanejamentoItemComposicao->buscaObjeto($inp->inpid);
if($arrIniciativaDados['intaid'] == $modelIniciativaTiposAtendimento::POR_ESCOLA){
    $condicaoPorEscola = 1;
}
$mInu = new Par3_Model_InstrumentoUnidade($_GET['inuid']);

$disabled = '';
if( $_REQUEST['aba'] == 'analisarPlanejamento' ) $disabled = 'disabled';

if($inp->obrid > 0){
    $selected = ' selected ';
}
?>
<style>
    .table > thead > tr > th, .table > tbody > tr > th, .table > tfoot > tr > th, .table > thead > tr > td, .table > tbody > tr > td, .table > tfoot > tr > td {
        border-top: 1px solid #e7eaec;
        line-height: 1.42857;
        padding: 8px;
        vertical-align: middle;
        text-align:center;
    }
    div#iigid_chosen.chosen-container.chosen-container-single > a.chosen-single.chosen-single-with-deselect{
        width: 650px;
    }
</style>
<form name="form-itens-composicao" id="form-itens-composicao" method="post">
        <input type="hidden" id="condicao-por-escola" name="condicao-por-escola" value="<?=$condicaoPorEscola?>" />
<?php if( $_REQUEST['aba'] != 'analisarPlanejamento' ){?>
    <div class = "col-lg-offset-3 col-lg-6">
        <div class = "input-group">
            <select name="iigid" id="iigid" class="chosen-select form-control" style="text-align: center;" data-placeholder="Selecione Item" required>
                <option></option>
                <?php foreach ($arrDadosItens as $codigo => $valor):?>
                    <option value="<?php echo $codigo ?>" <?= $selected ?> > <?php echo $valor?> </option>
                <?php endforeach; ?>
            </select>
            <span class = "input-group-btn">
                <button class = "btn btn-success" type="button" id="btn-incluir-item-itens-composicao">Adicionar <i class="fa fa-plus"></i></button>
           </span>
        </div><!-- /input-group -->
    </div><!-- /.col-lg-6 -->
<?php } ?>
    <br/>
    <br/>
    <br/>
<!--    <div class="col-lg-3"> <button type="button" class="btn btn-success" id="btn-incluir-item-itens-composicao"></div>-->
<?php
$config = array('label-size' => '1');
if( $_REQUEST['aba'] == 'analisarPlanejamento' ){
	$attribs = array('maxlengh' => 4000, 'readonly' => 'readonly');
	$arParam = array('inpid' => $inp->inpid,
					'inuid' => $_GET['inuid'],
					'iniano' => $_REQUEST['iniano'],
					'anaid' => $_REQUEST['anaid'],
					'esdid' => $esdid,
					'condicaoporescola' => $condicaoPorEscola);
	
	/*$obEmenda = new Par3_Controller_EmendasParlamentares();
	echo $obEmenda->pegaBeneficiarioSelecionado('', $inp->inpid);*/

	echo $controllerIniciativaPlanejamentoItemComposicao->getPlanilhaAnalise($arParam);
} else {
	$attribs = array('maxlengh' => 4000);
    if($inp->obrid > 0){
        $attribs['required'] = 'required';
        $config = array('label-size' => '2');
    }
	echo $controllerIniciativaPlanejamentoItemComposicao->getPlanilha($inp->inpid,$_GET['inuid'],$condicaoPorEscola);
}
?>

    <br>
    <br>
<?
if($objeto == $modelIniciativaTipoObjeto::FORMACAO){ //Altera a lebaeal do campo Detalhamento quando o tipo de Obj é formação
	$labeal = 'Observação';
}

echo $simec->textarea('inpitemcomposicaodetalhamento',$labeal, $inp->inpitemcomposicaodetalhamento, $attribs, $config);

?>
</form>
<?php

if($objeto == $modelIniciativaTipoObjeto::FORMACAO) {
    require_once('formItemComposicaoFormacao.inc');
}
?>


<div class="ibox float-e-margins animated modal" id="modal_escolas" tabdesex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 80%;">
        <form method="post" name="form-escolas" id="form-escolas" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Itens de Composição por Escola - <span id="itemnomemodal"></span></h3>
                </div>
                <div class="ibox-content">
                    <div id="dadosbuscaescola" class="col-lg-offset-3">
                        <input type="hidden" name="inuid" value="<?php echo $_GET['inuid']?>"/>
                        <input type="hidden" name="inpid" id="inpid" value="<?php echo $_GET['inpid']?>"/>
                        <input type="hidden" name="iniano" id="iniano" value=""/>
                        <input type="hidden" name="iigid" id="iigid" value=""/>
                        <?php
                        if($mInu->itrid == 1){
                            $modelMunicipio = new Par3_Model_Municipio();
                            $sqlMunicipiosUF = $modelMunicipio->montaSQLComboMunicipiosUF($_GET['inuid']);
                            echo $simec->select('escmuncod', '','', $sqlMunicipiosUF, array('maxlength' => '255','data-placeholder' => 'Selecione o Município'), array('input-size' => '8'));
                        }if($mInu->itrid == 2){
                            echo '<input type="hidden" name="escmuncod" id="escmuncod" value="'.$mInu->muncod.'"/>';
                        }
                        ?>
                    </div>
                    <div id="conteudo-modal" style="height: 450px; width: 100%; overflow: auto;">

                    </div>
                </div>
                <?php if( $_REQUEST['aba'] != 'analisarPlanejamento' ){?>
                <div class="ibox-footer">
                    <div class="col-sm-offset-5 col-md-offset-5 col-lg-offset-5">
                        <button type="button" id="cancelarEscolas" data-dismiss="modal" class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Fechar
                        </button>

                      <?   //Libera o botão salvar caso o planejamento esteja  da uninade não tenha sido iniciado ou já esteja em elaboração e a iniciativa esteja em cadastramento OU EM DIRIGÊNCIA
                      if (($modelDocumento->esdid == PAR3_ESDID_PLANEJAMENTO_NAO_INICIADO || $modelDocumento->esdid == PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO) && ($mdDocPl->esdid == PAR3_ESDID_CADASTRAMENTO || $mdDocPl->esdid == PAR3_ESDID_EM_DILIGENCIA)) {
                      ?>
                        <!--  
                        <button type="button" id="btn-salvar-escolas" class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Salvar
                        </button>
                        -->
                      <?php } else { ?>
                        <!--  
                          <button type="button" class="btn btn-success" onclick="js_travaSalvarEscolas();"><i class="fa fa-plus"></i>
                              Salvar
                          </button>
                        -->
                      <? } ?>
                    </div>
                </div>
              <?php }?>
            </div>
        </form>
    </div>
</div>

<div class="ibox float-e-margins animated modal" id="modal_diligencia_item" tabdesex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <form method="post" name="form-diligencia-item" id="form-diligencia-item" class="form form-horizontal">
            <div class="modal-content">
                <div class="ibox-title">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h3 id="modal_titulo">Tem certeza que deseja abrir Diligência para a Iniciativa?</h3>
                </div>
                <style>
                	.col-lg-3{
                		width: 10%;
                	}
                </style>
				<div id="dadosbuscadiligencia" class="">
                	<div class="ibox-content">
                        <input type="hidden" name="inuid" value="<?php echo $_GET['inuid']?>"/>
                        <input type="hidden" name="inpid" id="inpid" value="<?php echo $_GET['inpid']?>"/>
                        <?php
                        $radioItem = array('N' => 'Não', 'S' => 'Sim');
                        
                        echo $simec->textarea('inpitemcomposicaodetalhamento7', 'Motivo','', array('maxlengh' => 4000),array('text-align' => 'left'));
                        ?>
                    </div>
 					<div class="form-group ">
                    	<label class="col-sm-3 col-md-3 col-lg-3 control-label  control-label" style="width: 60%;">Deve ser anexado um arquivo comprobatório na resposta da Diligência?: </label>
                    	<div class="col-sm-2 col-md-2 col-lg-2 ">
                        	<div class="radio radio-inline "><input name="chk-item-diligencia" id="chk-item-diligencia-N" type="radio" value="N"><label for="chk-item-diligencia-N">Não</label></div><div class="radio radio-inline "><input name="chk-item-diligencia" id="chk-item-diligencia-S" type="radio" value="S"><label for="chk-item-diligencia-S">Sim</label></div>
                    	</div>
                	</div>
                </div>
                <div class="ibox-footer">
                    <div class="col-sm-offset-4 col-md-offset-4 col-lg-offset-4">
                        <button type="button" id="btn-confirma-nao" data-dismiss="modal" class="btn btn-default">
                            <i class="fa fa-times-circle-o"></i> Não
                        </button>
                        <button type="button" id="btn-confirma-sim" class="btn btn-success">
                            <i class="fa fa-plus-square-o"></i> Sim
                        </button>
                    </div>
                </div>
            </div>
            <input type="hidden"  id="valorTotalHidden"  />
        </form>
    </div>
</div>

<script>
    $(document).ready(function(){
        atualizaListagemItensComposicao();
    });

    $('#btn-confirma-sim').click(function(){
        alert('teste');
    });

<?php 
if (
        ($modelDocumento->esdid == PAR3_ESDID_PLANEJAMENTO_NAO_INICIADO || $modelDocumento->esdid == PAR3_ESDID_PLANEJAMENTO_EM_ELABORACAO) && 
        ($mdDocPl->esdid == PAR3_ESDID_CADASTRAMENTO || $mdDocPl->esdid == PAR3_ESDID_EM_DILIGENCIA)
   ) {
?>
    $('#modal_escolas').on('change','[name*=qtd-item-escola]', function()
	{
    	salvarItemEscola(this);
    });
<?php 
} else {
?>	
    $('#conteudo-modal').on('DOMSubtreeModified', function()
	{
    	$('[name*=qtd-item-escola]').prop('readonly', true);
	});
<?php 
}
?>      
    
    $('#btn-incluir-item-itens-composicao').click(function(){
        var cpe, inuid, inpid, iigid;
        iigid = $('#form-itens-composicao').find('[name=iigid]').val();

        if(iigid == ''){
            alert('Selecione o Item.');
            return false;
        }
        if($('#qtd-total-'+iigid).val() != undefined){
            alert('Esse item ja está na lista.');
            return false;
        }
        if($('#obridproinf').val() > 0){
            if ($('#form-itens-composicao').find('[name=iigidpai]').val() != undefined){
                alert('Esse kit ja foi adicionado.');
                return false;
            }
        }

        inuid = $('#form-iniciativaplanejamento').find('[name=inuid]').val();
        inpid = $('#form-iniciativaplanejamento').find('[name=inpid]').val();
        cpe = $('#form-itens-composicao').find('[name=condicao-por-escola]').val();

        var caminho = window.location.href;
        var action  = '&requisicao=buscaLinhaItemComposicao&iigid='+iigid+'&inuid='+inuid+'&inpid='+inpid+'&condicao-por-escola='+cpe;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                if($('#dados-itens-composicao-tbody tr:last').length == 0){
                    $('#dados-itens-composicao-tbody').append(resp);
                }else{
                    $('#dados-itens-composicao-tbody tr:last').after(resp);
                }
                atualizaListagemItensComposicao();
                return true;
            }
        });
        return false;
    });

    function atualizaListagemItensComposicao()
    {
        var calculo = new Calculo();
        var total = qtdItens = totalAno = vlReferencia = id = ano = 0;

        $( "input[name='iigid[]']" ).each(function () {

            qtdItens = 0;
            id = $(this).val();
            vlReferencia = $('#vl-referencia-'+id).text();
            vlReferencia = calculo.converteMonetario(vlReferencia);
            $( "input[name='ano-"+id+"']" ).each(function () {
                ano = $(this).val();
                $('#vl-item-'+id+'-'+ano).text(formatReal(parseFloat($('#qtd-item-'+id+'-'+ano).val()*vlReferencia).toFixed(2)));
                qtdItens = qtdItens + calculo.converteMonetario($('#qtd-item-'+id+'-'+ano).val());
            });

            $('#qtd-total-'+id).text(qtdItens);
            total = total + (qtdItens*vlReferencia);
            $('#vl-total-'+id).text(formatReal(parseFloat(qtdItens*vlReferencia).toFixed(2)));
        });
        $('#dados-itens-composicao-total').text(formatReal(parseFloat(total).toFixed(2)));

        document.getElementById('valorTotalHidden').value = (parseFloat(total).toFixed(2));

        var form   = $('#form-escolas').serialize();
        var valorTotal = (parseFloat(total).toFixed(2));
        var caminho = '<?php echo $url; ?>';
        var action  = '&requisicao=atualizarValorItemComposicao&'+form+'&valorTotal='+valorTotal;

        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                return true;
            }
        });
    }

    function formatReal(numero) {
        var tmp = numero + '';
        var neg = false;

        // Condição retirada por aumentar o valor numerico na exibição
        /*if (tmp - (Math.round(numero)) == 0) {
            tmp = tmp + '00';
        }*/

        if (tmp.indexOf(".")) {
            tmp = tmp.replace(".", "");
        }

        if (tmp.indexOf("-") == 0) {
            neg = true;
            tmp = tmp.replace("-", "");
        }

        if (tmp.length == 1) tmp = "0" + tmp

        tmp = tmp.replace(/([0-9]{2})$/g, ",$1");
        if (tmp.length > 6)
            tmp = tmp.replace(/([0-9]{3}),([0-9]{2}$)/g, ".$1,$2");

        if (tmp.length > 9)
            tmp = tmp.replace(/([0-9]{3}).([0-9]{3}),([0-9]{2}$)/g, ".$1.$2,$3");

        if (tmp.length = 12)
            tmp = tmp.replace(/([0-9]{3}).([0-9]{3}).([0-9]{3}),([0-9]{2}$)/g, ".$1.$2.$3,$4");

        if (tmp.length > 12)
            tmp = tmp.replace(/([0-9]{3}).([0-9]{3}).([0-9]{3}).([0-9]{3}),([0-9]{2}$)/g, ".$1.$2.$3.$4,$5");

        if (tmp.indexOf(".") == 0) tmp = tmp.replace(".", "");
        if (tmp.indexOf(",") == 0) tmp = tmp.replace(",", "0,");

        return "R$"+(neg ? '-' + tmp : tmp);
    }

    function abreModalEscolas(inpid,iniano,iigid) {
        var label = $(document).find('#nome-'+iigid).html();
        //console.log('teste '+label);
        $(document).find('#itemnomemodal').html(label);

        $('#dadosbuscaescola').find('[name=inpid]').val(inpid);
        $('#dadosbuscaescola').find('[name=iniano]').val(iniano);
        $('#dadosbuscaescola').find('[name=iigid]').val(iigid);
        <?php if($mInu->itrid == 2):?>
        var caminho = window.location.href;
        var inpid  = $('#dadosbuscaescola').find('[name=inpid]').val();
        var iniano = $('#dadosbuscaescola').find('[name=iniano]').val();
        var iigid  = $('#dadosbuscaescola').find('[name=iigid]').val();
        var inuid  = $('#dadosbuscaescola').find('[name=inuid]').val();
        var muncod = $('#dadosbuscaescola').find('[name=escmuncod]').val();
        var action  = '&requisicao=buscaModalEscolas&inpid='+inpid+'&iniano='+iniano+'&iigid='+iigid+'&inuid='+inuid+'&muncod='+muncod;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").html(resp);
            }
        });
        <?php elseif($mInu->itrid == 3):?>
        var caminho = window.location.href;
        var inpid  = $('#dadosbuscaescola').find('[name=inpid]').val();
        var iniano = $('#dadosbuscaescola').find('[name=iniano]').val();
        var iigid  = $('#dadosbuscaescola').find('[name=iigid]').val();
        var inuid  = $('#dadosbuscaescola').find('[name=inuid]').val();
        var action  = '&requisicao=buscaModalEscolas&inpid='+inpid+'&iniano='+iniano+'&iigid='+iigid+'&inuid='+inuid;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").html(resp);
            }
        });
        <?php else:?>
        $(document).find('[name=escmuncod]').val('').trigger('chosen:updated');
        $("#conteudo-modal").hide();
        $("#conteudo-modal").html('');
    <?php endif;?>
        $("#modal_escolas").modal("show");
    }

    jQuery('#btn-enviar-diligencia').click(function(){
    	
        $('#dadosbuscadiligencia').find('[name=inpid]').val(jQuery('[name="inpid[]"]').val());
        $('#dadosbuscadiligencia').find('[name=inuid]').val(jQuery('[name="inuid[]"]').val());
        
        var caminho = window.location.href;
        var inpid  = $('#dadosbuscadiligencia').find('[name=inpid]').val();
        var inuid  = $('#dadosbuscadiligencia').find('[name=inuid]').val();
        var action  = '&requisicao=carregaModalDiligencia&inpid='+inpid+'&inuid='+inuid;
        /*$.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-diligencia").html(resp);
            }
        });*/
        $("#modal_diligencia_item").modal("show");
    });
    

    //Ao Selecionar um município
    $(document).on('change','[name=escmuncod]',function(){
        var caminho = window.location.href;
        var muncod = $(this).val();
        var inpid  = $('#dadosbuscaescola').find('[name=inpid]').val();
        console.log(inpid);
        var iniano = $('#dadosbuscaescola').find('[name=iniano]').val();
        var iigid  = $('#dadosbuscaescola').find('[name=iigid]').val();
        var inuid  = $('#dadosbuscaescola').find('[name=inuid]').val();
        var action  = '&requisicao=buscaModalEscolas&inpid='+inpid+'&iniano='+iniano+'&iigid='+iigid+'&inuid='+inuid+'&muncod='+muncod;
        $.ajax({
            type: "POST",
            url: caminho,
            data: action,
            async: false,
            success: function (resp) {
                $("#conteudo-modal").html(resp);
            }
        });
        $("#conteudo-modal").slideDown();
        return false;
    });

    function atualizaTodasEscolas(){
        var qtd = $('#qtd-item-escola-total').val();
        if(parseFloat(qtd) < 0){''
            qtd = '0';
            $('#qtd-item-escola-total').val('0');
        }
        $('.qtd-escola-item-composicao').val(qtd);
    }

//     $('#btn-salvar-escolas').click(function()
// 	{
//     	salvarItemEscola();    	
//     });

	function salvarItemEscola(obj)
	{
		var obCalc = new Calculo();
		if (obCalc.comparar(obj.value, 0, '<')) {
			obj.value = 0;
		}
		
        var form   			= $('#form-escolas').serialize();
        var iniano 			= $('#form-escolas').find('[name=iniano]').val();
        var iigid 			= $('#form-escolas').find('[name=iigid]').val();
        var vlreferencia 	= $('#form-itens-composicao').find('[name="referencia['+iigid+']"]').val();

        var caminho = '<?php echo $url; ?>';
        var action  = '&requisicao=salvarItemComposicaoEscolas&'+form+'&vlreferencia='+vlreferencia;
        $.ajax({
            type	: "POST",
            url		: caminho,
            data	: action,
            async	: false,
            success	: function (resp)
            {
                $("#qtd-item-"+iigid+"-"+iniano).val(resp);
//                 $("#modal_escolas").modal("hide");
                atualizaListagemItensComposicao();
                return true;
            }
        });
        return false;
	}
    
    function js_travaSalvarEscolas() {
        swal("Ação não Permitida,.", "Esta unidade não iniciou o planejamento ou já foi finalizado!", "error");
    }
</script>