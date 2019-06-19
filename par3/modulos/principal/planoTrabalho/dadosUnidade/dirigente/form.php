<?php
include_once APPRAIZ . 'par3/classes/model/seguranca/Usuario.class.inc';
$renderDirigente = new Par3_Controller_Entidade();
$objEntidade = new Par3_Model_InstrumentoUnidadeEntidade($_REQUEST['entid']);
$arrEsconde = array('enttelefonefax' => 'N');

$modelInstrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
if($modelInstrumentoUnidadeEntidade->pegarEntidAtivoPorTipo($inuid, $tenid)) {
    $disableIncluir = 'disabled';
}

$inuid = $_REQUEST['inuid'];
$tenid = $_REQUEST['tenid'] = Par3_Model_InstrumentoUnidadeEntidade::DIRIGENTE;
if($_REQUEST['entid']){
    $titulo = 'Dirigente - Alterar';
}else{
    $titulo = 'Dirigente - Incluir';
}

switch($_REQUEST['requisicao']){
    case 'recuperarporcpf':
        $cpf   = trim(str_replace(array('-','.'),'',$_GET['entcpf']));
        $mEnt  = new Par3_Model_InstrumentoUnidadeEntidade();
    $inuid = $_GET['inuid'];
        $arrDados = array(
                'sexo'            => '',
                'email'           => '',
                'datanasc'        => '',
                'fone'            => '',
                'fonecelular'     => '',
                'rg'              => '',
                'orgexpedidor'    => '',
                'tipousuario'     => '',
        );

        $rsEnt = $mEnt->recuperarEntidadePorCPF($inuid,$cpf,$tenid,'I');
        $arrDados['sexo']         = $rsEnt['entsexo'];
        $arrDados['email']        = $rsEnt['entemail'];
        $arrDados['datanasc']     = $rsEnt['entdtnascimento'];
        $arrDados['fone']         = str_replace('-','',$rsEnt['enttelefonecomercial']);
        $arrDados['fonecelular']  = str_replace(array('-',' '),'',$rsEnt['enttelefonecelular']);
        $arrDados['orgexpedidor'] = $rsEnt['entorgexpedidor'];
        $arrDados['rg']           = $rsEnt['entrg'];
        $arrDados['tipousuario']  = 'ent';
        if(empty($rsEnt)){
            $rsEnt = new Seguranca_Model_Seguranca_Usuario($cpf);
            $arrDados['sexo']     = strtolower($rsEnt->ususexo);
            $arrDados['email']    = $rsEnt->usuemail;
            $arrDados['datanasc'] = $rsEnt->usudatanascimento;
            $arrDados['fone']     = str_replace(array('-',' '),'',$rsEnt->usufoneddd.$rsEnt->usufonenum);
            $arrDados['tipousuario']  = 'sis';
        }
        ob_clean();
        echo simec_json_encode($arrDados);die;
        break;
}

?>
<form name="form-inserir" id="form-inserir" class="form-horizontal" method="post" action="par3.php?modulo=principal/planoTrabalho/dadosUnidade/dirigente&acao=A&req=inserir">
    <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid ?>"/>
    <input type="hidden" name="tenid" id="tenid" value="<?php echo $tenid; ?>"/>
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center"><?=$titulo?></h3>
            </div>
        </div>
    </div>
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12"><h3>Informações Básicas</h3></div>
        </div>
    </div>
    <div class="ibox-content">
    <?php
    $renderDirigente->formPessoaFisica('enable', $objEntidade, 'dirigente', $arrEsconde);
    ?>
    </div>
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12"><h3>Informações Adicionais</h3></div>
        </div>
    </div>
    <div class="ibox-content">
    <?php
    $renderDirigente->formDirigente('enable', $objEntidade, null);
    ?>
    </div>
    <div class="ibox-footer">
        <div class="center">
            <button type="submit" class="btn btn-success" id="btn-incluir" <?php echo $disableIncluir; ?>>Salvar</button>
            <button type="button" class="btn btn-warning" style="width:110px;" id="btn-cancelar">Cancelar</button>
        </div>
    </div>
</form>
<script>

    $(window).on('beforeunload',function(){
        $('.loading-dialog-par3').show();
    });

    $("#btn-cancelar").on("click", function () {
        var caminho = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&menu=dirigente&inuid=' + $('#inuid').val();
        window.location.assign(caminho);
    });

    if ($('#entcursomec-t:checked').val() == 't') {
        $('#entcursomecdescricao').closest('.form-group').removeClass('hidden');
        $('#entcursomecdescricao').attr('required', 'required');
    } else {
        $('#entcursomecdescricao').closest('.form-group').addClass('hidden');
        $('#entcursomecdescricao').val('');
        $('#entcursomecdescricao').removeAttr('required');
    }

    $('input[name="entcursomec"]').change(function () {
        if ($(this).val() == 't') {
            $('#entcursomecdescricao').closest('.form-group').removeClass('hidden');
            $('#entcursomecdescricao').attr('required', 'required');
        } else {
            $('#entcursomecdescricao').closest('.form-group').addClass('hidden');
            $('#entcursomecdescricao').val('');
            $('#entcursomecdescricao').removeAttr('required');
        }
    })
    $('.radio-inline').removeClass('radio-inline');
//    $("#entcpf").change(function(){
//        var entcpf  = $(this).val();
//        var caminho = window.location.href;
//        var action  = "requisicao=recuperarporcpf&entcpf="+entcpf;
//            $.ajax({
//            type: "GET",
//            url: caminho,
//            data: action,
//            async: false,
//            success: function (resp) {
//                var res = $.parseJSON(resp);
//                $("#entemail").val(res.email).prop('readonly', (res.email != ''?true:false));
//
//                $("#enttelefonecomercial").val(res.fone).prop('readonly', (res.fone != ''?true:false));
//                $("#entsexo-"+res.sexo).attr( 'checked', true ).prop('readonly', (res.sexo != ''?true:false));
//                $("#enttelefonecelular").val(res.fonecelular);
//                var datanasc = '';
//                if(res.datanasc != ''){
//                    datanasc = moment(res.datanasc,'YYYY-MM-DD HH:mm').format('DD/MM/YYYY');
//                }
//                $('#entdtnascimento').val(datanasc).prop('readonly', (res.datanasc != ''?true:false));
//                $("#entrg").val(res.rg);
//                $("#entorgexpedidor").val(res.orgexpedidor);
//            }
//        });
//    });
</script>