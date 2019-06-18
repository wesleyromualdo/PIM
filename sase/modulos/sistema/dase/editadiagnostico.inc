<?php

ob_start();

// --------------- DependÃªncias

include APPRAIZ . 'includes/cabecalho.inc';
//include_once APPRAIZ . "includes/funcoesspo_componentes.php";


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = ' Disponibilizar Diagnóstico';
global $db;

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
    <?= '<h6><small>' . implode('</small></h6><h6><small>', $cabecalhoSistema) . '</small></h6>' ?>
    <center><h3><?= $titulo ?></h3></center>
</div>
<?php
// --


global $db;

$sql = "SELECT
  questid,
  questdescricao,
  TO_CHAR(datainicio, 'DD/MM/YYYY') as datainicio,
  TO_CHAR(datafim, 'DD/MM/YYYY') as datafim,
  queststatus
FROM sase.questionariosase
WHERE questid =" . $_GET['quest'];
$questnario = $db->pegalinha($sql);


if ($_POST) {

    $_POST['datainicio'] = str_replace('/', '-', $_POST['datainicio']);
    $_POST['datafim'] = str_replace('/', '-', $_POST['datafim']);

    $_POST['datainicio'] = date('Y-m-d', strtotime($_POST['datainicio']));
    $_POST['datafim'] = date('Y-m-d', strtotime($_POST['datafim']));







    $sql = "update  sase.questionariosase set
                questdescricao ='" . $_POST['questdescricao'] . "',
                datainicio ='" . $_POST['datainicio'] . "',
                datafim ='" . $_POST['datafim'] . "',
                questStatus ='" . $_POST['questStatus'] . "'
            WHERE questid = {$_POST['questid']}";

    $db->executar($sql);

    $db->commit();
    echo "<script>alert('Disponibilizar Diagnóstico Alterado!');
                    
              location.href = 'sase.php?modulo=sistema/dase/disponibilizarDiagnostico&acao=A';
                 //   location.reload();
          </script>";

}

// submit /


?>
<script src="/library/jquery/jquery-1.10.2.js"></script>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<link rel='StyleSheet' href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
<link rel="stylesheet" href="/ted/css/bootstrap-datepicker3.min.css" type="text/css"/>
<!--<link  rel='StyleSheet' href="../sase/js/bootstrap-datetimepicker/bootstrap-datetimepicker.css">-->
<!--<script src="/ted/js/bootstrap-datepicker.min.js" type="text/javascript"></script>
<script src="/ted/js/bootstrap-datepicker.pt-BR.min.js" type="text/javascript"></script>-->
<script src="/includes/funcoes.js"></script>
<script src="/sase/js/bootstrap-datetimepicker/moment.min.js"></script>
<script src="/sase/js/bootstrap-datetimepicker/moment.pt-br.js"></script>

<script src="/sase/js/bootstrap-datetimepicker/bootstrap-datetimepicker.min.js"></script>


<!-- ------------------------------- FORM ------------------------------- -->
<form class="form-horizontal" name="formulario" id="formulario" method="post">
    <fieldset>

        <!-- Form Name -->
        <legend>Questionário</legend>

        <!-- Text input-->
        <div class="form-group">
            <label class="col-md-4 control-label" for="questdescricao">Descrição:</label>
            <div class="col-md-6">
                <input id="questdescricao" name="questdescricao" type="text" placeholder=""
                       value="<?= $questnario['questdescricao'] ?>" class="form-control input-md"
                       required="campo Obrigatório">
                <input type="hidden" id="questid" name="questid" value="<?= $questnario['questid'] ?>">
            </div>
        </div>


        <div class="form-group">
            <label class="col-md-4 control-label" for="questdescricao">Data Início:</label>
            <div class='col-md-6'>

                    <div class='input-group date' id='datetimepicker6'>
                        <input type='text' class="form-control"  id="datainicio" name="datainicio" maxlength="0" value="<?=$questnario['datainicio']?>" />
                        <span class="input-group-addon">
                   <span class="glyphicon glyphicon-calendar"></span>
               </span>
                    </div>

            </div>
        </div>
        <div class="form-group">
            <label class="col-md-4 control-label" for="questdescricao">Data Fim:</label>
            <div class='col-md-6'>

                    <div class='input-group date' id='datetimepicker7'>
                        <input type='text' class="form-control" name="datafim"  maxlength="0" value="<?=$questnario['datafim']?>" />
                        <span class="input-group-addon">
                   <span class="glyphicon glyphicon-calendar"></span>
               </span>
                    </div>

            </div>
        </div>

        <!-- Select Basic -->
        <div class="form-group">
            <label class="col-md-4 control-label" for="questStatus">Status:</label>
            <div class="col-md-4">
                <select id="questStatus" name="questStatus" class="form-control">
                    <option value="A" <? echo($questnario['queststatus'] == "A" ? "selected" : ""); ?> >Ativo</option>
                    <option value="I" <? echo($questnario['queststatus'] == "I" ? "selected" : ""); ?>>Inativo</option>
                </select>
            </div>
        </div>

        <!-- Button -->
        <div class="form-group">
            <label class="col-md-4 control-label" for="btnSalvar"></label>
            <div class="col-md-4">
                <button id="btnSalvar" type="button" name="btnSalvar" class="btn btn-warning">Salvar</button>
            </div>
        </div>

    </fieldset>
</form>

<script>


    $(function () {
        $('#datainicio, #datafim').attr('maxLength', 0);
        var now = new Date(),
            minDateJS = now.toISOString().substring(0,10);
        $('#datetimepicker6').datetimepicker({
            locale: 'pt-br',
            format: 'DD/MM/YYYY',
            minDate: minDateJS
        });
        $('#datetimepicker7').datetimepicker({
            useCurrent: false, //Important! See issue #1075
            locale: 'pt-br',
            format: 'DD/MM/YYYY',
            minDate: minDateJS
        });
        $("#datetimepicker6").on("dp.change", function (e) {
            $('#datetimepicker7').data("DateTimePicker").minDate(<?=$questnario['datainicio']?>);
        });
        $("#datetimepicker7").on("dp.change", function (e) {
            $('#datetimepicker6').data("DateTimePicker").maxDate(e.date);
        });
        $('#datainicio').val('<?=$questnario['datainicio']?>');

        $('#btnSalvar').click(function () {

            document.formulario.submit();
        });


        /*$('#datainicio,#datafim').datepicker({
            language: "pt-BR"
        });*/

        /*
         $('#datainicio').datepicker({
         minDate: 0
         });

         $('#datafim').datepicker({
         minDate: 0
         });

         $('#datainicio').change(function(){
         var minDate = this.value;

         $('#datafim').datepicker({minDate: minDate} );

         });*/



    });
</script>


<!-- / ------------------------------- FORM ------------------------------- -->


<div id="footer"></div>
<!-- /html -->