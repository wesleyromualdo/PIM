<?php
    if( !isset( $_REQUEST['muncod'] ) || $_REQUEST['muncod'] == '' ){
         echo ' <div class="col-md-12">
                <div class="well">
                    <fieldset>
                       Não é possivel cadastrar dados na aba Agenda. Selecione um município na aba Lista.
                    </fieldset>
                </div>
            </div>';
         die();
    }

    include_once APPRAIZ . 'sase/classes/Model/Etapaagendacomcoord.class.inc';

    $cadastro = new Sase_Model_Etapaagendacomcoord();
if($_POST['ajaxeditarEtapam']){



    $isTrue = $cadastro->editarAgendaTrabalho($_POST);
    die();
}

if ($_REQUEST['request'] == 'excluirItemAgenda') {
    $cadastro->excluirItemAgendaTrabalho($_REQUEST['accid']);
    $success = "<script>
                                alert('Operação realizada com sucesso!');
                                   location.href='{$_SERVER['HTTP_REFERER']}';
                            </script>";


    die($success);
}


if ($_POST['action'] != '') {
    switch ($_POST['action']) {

            case 'salvarEtapa':
                $cadastro->salvarEspatasAgendaTrabalho( $_REQUEST );
                $success = "<script>
                              //  alert('Operação realizada com sucesso!');
                                location.href='{$_SERVER['HTTP_REFERER']}';
                            </script>";
                die($success);

            case 'excluirItemAgenda':
                $cadastro->excluirItemAgendaTrabalho($_REQUEST['accid']);
                $success = "<script>
                                alert('Operação realizada com sucesso!');
                                location.href='{$_SERVER['HTTP_REFERER']}';
                            </script>";
            die($success);
//            case 'gerarPdf';
//                
//                $html = gerarHtml($_REQUEST['muncod']);
//                
//                ob_clean();
//
//                $http = new RequestHttp();
//                $http->toPdfDownload( iconv('ISO-8859-1', 'UTF-8', $html), 'agenda_trabalho_municipio' );                
//            die;                
        }
    }

    function redir($url = null, $msg = null)
    {
        $script .= '<script>';
        if (!empty($msg)){
            $script .= '	alert(\'' . $msg . '\');';
        }
        if (!empty($url)){
            $script .= '	location.href=\'' . $url . '\';';
            $script .= '	return false;';
        }else{
            $script .= '	history.go(-1);';
        }
        $script .= '</script>';
        die($script);
    }
    
//    function gerarHtml($muncod)
//    {
//        global $cadastro;
//        $rMunicipio = $cadastro->buscaMunicipio($_REQUEST['muncod']);
//
//        $html  = "<html><body>";
//        
//        $html .= "<center><h3>AGENDA DE TRABALHO DA COMISSÃO COORDENADORA - MUNICÍPIO <br> {$rMunicipio}</h3></center>";
//        
//        $html .= "<div style='text-align:center; color: #333333; background-color: #f5f5f5; border-color: #dddddd; font-size: 14px'>";
//        $html .= "<b>CADASTRO DAS ETAPAS</b>";
//        $html .= "</div>";
//        
//        $html .= "<div style='well'>";
//        
//        $html .= "</body></html>";
//
//        return $html;
//    }


global $db;
$sql = "select count(muncod)  from territorios.municipio WHERE muncod = '{$_GET[muncod]}'";
$muncodesiste = $db->pegaum($sql);
if($muncodesiste !=  1 ) {
    $alerta = "<script> alert('O codigo do município esta incorreto!');
              location.href= 'sase.php?modulo=principal/agendamento_trab&acao=A&aba=lista';
</script>";
    echo $alerta;

}
$ano_passado =  (date("Y")-1);
?>
<style>
    @media (min-width: 568px) {
        .modal-xl {
            width: 70%;
            max-width: 1200px;
        }
    }
</style>
<script>

    function salvaEtapaMunicipio() {
        var etapa = $('#etapa').val();
        var eacid = etapa;
        var muncod = $('#muncod').val();
        var accacao = trim($('#accacao').val());
        var accresponsavel = trim($('#accresponsavel').val());
        var accprazo = $('#accprazo').val();
        var accobservacao = trim($('#accobservacao').val());


        var msg = [];

        if (etapa == 0) {
            msg.push('Campo Etapa é obrigatório');
        }

        if (accacao == '') {
            msg.push('Campo Ação é obrigatório');
        }
        if (accresponsavel == '') {
            msg.push('Campo Responsável é obrigatório');
        }
        if (accprazo == '') {
            msg.push('Campo Previsão de Execução é obrigatório');
        }
//        if (accobservacao == '') {
//            msg.push('Campo Observações é obrigatório');
//        }


        if (msg.length > 0) {
            bootbox.alert('Os seguintes campos são obrigatórios e devem ser preenchidos:<br /><br /><ul><li>' + msg.join(';</li><li>') + '.</li></ul>');
            return false;
        }

        $('#formulario_muncod').submit();


    }


    function editarEtapa(id) {
        var etapa = $('#etapa_'+id).val();
        var eacid = etapa;
        var muncod = $('#muncod_'+id).val();
        var accid = $('#accid_'+id).val();
        var accacao = trim($('#accacao_'+id).val());
        var accresponsavel = trim($('#accresponsavel_'+id).val());
        var accprazo = $('#accprazo_'+id).val();
        var accobservacao = trim($('#accobservacao_'+id).val());
        var accconcluida = trim($('#accconcluida_'+id).val());

        var datainscricao = accprazo.substring(6, 11);
        var msg = [];
        if(<?=$ano_passado?> != datainscricao) {


            if (etapa == 0) {
                msg.push('Campo Etapa é obrigatório');
            }

            if (accacao == '') {
                msg.push('Campo Ação é obrigatório');
            }
            if (accresponsavel == '') {
                msg.push('Campo Responsável é obrigatório');
            }
            if (accprazo == '') {
                msg.push('Campo Previsão de Execução é obrigatório');
            }
            if (accobservacao == '') {
                msg.push('Campo Observações é obrigatório');
            }


            if (msg.length > 0) {
                bootbox.alert('Os seguintes campos são obrigatórios e devem ser preenchidos:<br /><br /><ul><li>' + msg.join(';</li><li>') + '.</li></ul>');
                return false;
            }
        }
        $.ajax({
            url: 'sase.php?modulo=principal/agendamento_trab&acao=A&aba=edit&muncod=<?=$_GET['muncod']?>',
            type: 'POST',
            cache: false,
            data: { eacid: eacid,
                muncod: muncod,
                accid: accid,
                accacao: accacao,
                accresponsavel: accresponsavel,
                accprazo: accprazo,
                accobservacao: accobservacao,
                accconcluida: accconcluida,
                ajaxeditarEtapam: true},
            success: function (response) {
                $('#edtarAtividade_'+id).modal('hide');
                // alert("AGENDA DE TRABALHO DA COMISSÃO COORDENADORA ALTERADO COM SUCESSO!");
                location.reload();
            }
        });


    }
    
    $(document).ready(function(){
    	$("textarea[maxlength]").keydown(function() {
    		var limit   = $(this).attr("maxlength"); 
    	    var value   = $(this).val(); 
    	    var current = value.length;
    	                 
    	    if(limit <= current) {                   
				$(this).val(value.substring(0, limit));
				alert("Somente é permitido 1000 caracteres neste campo.");
    	    }
  	     });
  	});
</script>

<br>

<div id="container">
    <div class="row">
        <div class="col-md-12">
            <?php
            $alerta = '';
            if($_SESSION['msg']) {
                $alerta = '<script> alert("'.$_SESSION[msg].'")</script>';
                echo $alerta;
                unset($_SESSION['msg']);
            }
            ?>

            <div id="formularioFiltros">
                <div class="panel panel-default">
                    <div class="panel-heading">
                        <h3 class="panel-title text-center">
                            <b>CADASTRO DAS ETAPAS</b>
                        </h3>
                    </div>
                </div>
            </div>

            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <button type="button" class="btn btn-primary " data-toggle="modal" data-target="#cadastrarAcao">
                            <span class="glyphicon glyphicon-plus"></span> CADASTRAR AÇÃO
                        </button>
                        <!-- Modal -->
                        <div class="modal fade" id="cadastrarAcao" tabindex="-1" role="dialog"
                             aria-labelledby="cadastrarAcao">
                            <div class="modal-dialog  modal-xl" role="document">
                                <div class="modal-content">
                                    <div class="modal-header">
                                        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                                            <span aria-hidden="true">&times;</span></button>
                                        <h4 class="modal-title text-center" id="myModalLabel">CADASTRADAR AÇÃO DA
                                            ETAPA</h4>
                                    </div>
                                    <div class="modal-body">
                                        <form name="formulario_muncod" id="formulario_muncod" class="form-horizontal" method="POST">
                                            <div class="form-group">
                                                <label class="col-md-2 control-label" for="etapa">Etapa:</label>
                                                <div class="col-md-10">
                                                    <select id="etapa" name="etapa" class="form-control">
                                                        <option value="0">Selecione</option>
                                                        <option value="1"> I. ETAPA - Organizar o Trabalho</option>
                                                        <option value="2"> II. ETAPA - Estudar o Plano</option>
                                                        <option value="3"> III. ETAPA - Monitorar continuamente as metas
                                                            e estratégias
                                                        </option>
                                                        <option value="4"> IV. ETAPA - Avaliar periodicamente o plano
                                                        </option>
                                                    </select>
                                                </div>
                                            </div>
                                            <input type="hidden" id="estuf" name="estuf"
                                                   value="<?= $_REQUEST['estuf'] ?>"/>
                                            <input type="hidden" id="action" name="action" value="salvarEtapa"/>
                                          <div class="form-group">
                                                <label class="col-md-2 control-label" for="accacao">Ação:</label>
                                                <div class="col-md-10">
                                                    <textarea class="form-control" cols="40" id="accacao"
                                                              name="accacao" maxlength="1000"></textarea>
                                                </div>
                                            </div>


                                            <div class="form-group">
                                                <label class="col-md-2 control-label"
                                                       for="accresponsavel">Responsável:</label>
                                                <div class="col-md-10">
                                                    <textarea class="form-control" id="accresponsavel"
                                                              name="accresponsavel"></textarea>
                                                </div>
                                            </div>


                                            <div class="form-group">
                                                <label class="col-md-2 control-label" for="accprazo">Previsão de
                                                    Execução:</label>
                                                <div class="col-md-10">
                                                    <input id="accprazo" maxlength="0" name="accprazo" width="200px"
                                                           type="text" placcholder=""
                                                           class="form-control  campoData2 input-md">

                                                </div>
                                            </div>


                                            <div class="form-group">
                                                <label class="col-md-2 control-label"
                                                       for="accobservacao">Observações:</label>
                                                <div class="col-md-10">
                                                    <textarea class="form-control" id="accobservacao"
                                                              name="accobservacao"></textarea>
                                                </div>
                                            </div>


                                    </div>
                                    <div class="modal-footer">
                                        <button type="button" class="btn btn-default" data-dismiss="modal">Cancelar
                                        </button>
                                        <button type="button" class="btn btn-primary" onclick="salvaEtapaMunicipio()"><span
                                                    class="glyphicon glyphicon-plus"></span> Salvar
                                        </button>
                                    </div>
                                    </form>
                                </div>
                            </div>
                        </div>
                        <!--  -->
                    </fieldset>
                </div>
            </div>

            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <legend>I. ETAPA - Organizar o Trabalho</legend>
                            <div class="form-group">
                                <div class="col-lg-12 col-md-12">
                                    <?php echo $cadastro->ListaTrabalhosAgendados( $_REQUEST['muncod'], 1 ); ?>
                                </div>
                            </div>
                    </fieldset>
                </div>
            </div>
            
            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <legend>II. ETAPA - Estudar o Plano</legend>
                            <div class="form-group">
                                <div class="col-lg-12 col-md-12">
                                    <?php echo $cadastro->ListaTrabalhosAgendados( $_REQUEST['muncod'], 2 ); ?>
                                </div>
                            </div>
                    </fieldset>
                </div>
            </div>
            
            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <legend>III. ETAPA - Monitorar continuamente as metas e estratégias</legend>
                            <div class="form-group">
                                <div class="col-lg-12 col-md-12">
                                    <?php echo $cadastro->ListaTrabalhosAgendados( $_REQUEST['muncod'], 3 ); ?>
                                </div>
                            </div>
                    </fieldset>
                </div>
            </div>
            
            <div class="col-md-12">
                <div class="well">
                    <fieldset>
                        <legend>IV. ETAPA - Avaliar periodicamente o plano</legend>
                            <div class="form-group">
                                <div class="col-lg-12 col-md-12">
                                    <?php echo $cadastro->ListaTrabalhosAgendados( $_REQUEST['muncod'], 4 ); ?>
                                </div>
                            </div>
                    </fieldset>
                </div>
            </div>
        </div>

        <div class="col-lg-12">
            <div class="well" align="center">
                <button class="btn btn-primary" id="btn-historico">Histórico</button>

            </div>
        </div>
    </div>
</div>

<div class="modal fade modalAditivo" id="modalAditivo" tabindex="-1" role="dialog" aria-labelledby="modalAditivo" aria-hidden="true">
    <div class="modal-dialog" style="width:80%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">
                    <span aria-hidden="true">&times;</span>
                    <span class="sr-only">Fechar</span>
                </button>
                <h4 class="modal-title">Histórico Agenda de trabalho da comissão coordenadora</h4>
            </div>
            <div class="modal-body" id="modal-body">
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <legend>I. Etapa - Organizar o Trabalho</legend>
                            <?php echo $cadastro->dadosHistorico( $_REQUEST['muncod'], 1 ); ?>
                        </fieldset>
                    </div>
                </div>
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <legend>II. Etapa - Estudar o Plano</legend>
                            <?php echo $cadastro->dadosHistorico( $_REQUEST['muncod'], 2 ); ?>
                        </fieldset>
                    </div>
                </div>
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <legend>III. Etapa - Monitorar continuamente as metas e estratégias</legend>
                            <?php echo $cadastro->dadosHistorico( $_REQUEST['muncod'], 3 ); ?>
                        </fieldset>
                    </div>
                </div>
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <legend>IV. Etapa - Avaliar periodicamente o plano</legend>
                            <?php echo $cadastro->dadosHistorico( $_REQUEST['muncod'], 4 ); ?>
                        </fieldset>
                    </div>
                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<script>
    jQuery('documento').ready(function(){
        $("#btn-historico").on("click", function() {
            $('#modalAditivo').modal('show');
        });

        $('.campoData2').datepicker();
    });

    function podeAlterar(str, id)
    {
        if(str == 's'){
            $('.frm-disabled'+id).attr('readonly', false);
            $('.btnExcluir'+id).show();
        }else{
            $('.frm-disabled'+id).attr('readonly', true);
            $('.btnExcluir'+id).hide();
        }
    }

    function limparAtividade(id){
        textoConfimacao = 'Deseja excluir o registro?';
        bootbox.confirm(textoConfimacao, function(result){
            if(result){
                $('.frmExcluir_'+id).val('');
            }
        });
    }

    function excluirAtividade(accid)
    {
        textoConfimacao = 'Deseja excluir o registro?';
        bootbox.confirm(textoConfimacao, function(result){
            if(result){
                url = location.href+'&request=excluirItemAgenda&accid='+accid;
                location.href = url;
            }
        });
    }

    function excluirLinha(el)
    {
        textoConfimacao = 'Deseja excluir a linha registro?';
        bootbox.confirm(textoConfimacao, function(result){
            if(result){
                $('#lineExcluir_'+$(el).attr('frm')).remove();
            }
        });
    }
</script>
<!--<div id="footer"></div>-->