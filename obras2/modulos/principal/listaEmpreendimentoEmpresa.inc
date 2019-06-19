<?php


require APPRAIZ . 'obras2/includes/principal/listaEmpreendimentoEmpresa/ctrl.php';


$arAba = getArAba('listaorgaoemprendimentoempresa');
echo montarAbasArray($arAba, "?modulo=principal/listaEmpreendimentoEmpresa&acao=A&orgid=" . $orgid);

monta_titulo_obras( 'Lista de Obras por OS', '<img align="absmiddle" src="/imagens/edit_on.gif" style="margin-right: 3px;" title=\"Ir para a Supervisão\"> Ir para a Supervisão'); 

extract( $_POST );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
$(document).ready(function(){

    <?php
    if ( $abreBuscaAvancada ){
    	echo "exibeBuscaAvancada( " . ($abreBuscaAvancada == 't' ? 'true' : 'false') . " )";
    }
?>

});

</script>
<form method="post" name="formListaObra" id="formListaObra">
	<input type="hidden" name="op" id="op" value="">
	<input type="hidden" name="xls" id="xls" value="">
	<input type="hidden" name="mapa" id="mapa" value="">
        <input type="hidden" name="xlsparalizada" id="xlsparalizada" value="">
	<input type="hidden" name="empid" id="empid" value="">
	<input type="hidden" name="abreBuscaAvancada" id="abreBuscaAvancada" value="">
	<input type="hidden" name="estuf" id="estuf" value="SP">



    <div class="form-filters">
        <div class="row text-center">
            <div class="col-lg-12 position-relative">
                Número da OS:: <?=  campo_texto( 'sosnum', 'N', 'S', '', 50, 100, '###########', '', 'right', '', 0, ''); ?>
            
                <div onclick="javascript:exibeBuscaAvancada(true);" class="btn btn-primary float-right" id="labelBuscaAvancada">Busca Avançada</div>
            </div>
        </div>

        <div  class="row tr_busca_avancada form-filters" style="display: none;">
                <div class="row">
                    <div class="col-md-12 text-center">
                        <div onclick="exibeBuscaAvancada(false);" class="btn btn-danger btn-sm">Fechar Busca Avançada</div>
                    </div>
                </div> 

                <div class="row">
                    <div class="col-lg-6"> 

                		<div class="row">
                            <div class="col-md-4">
                                    
                            </div>
                            <div class="col-md-8">
                                        <?php
                                           campo_texto('empbuscatexto','N','S','',70,100,'','', '', '', '', 'id="empbuscatexto"', '', $_REQUEST['empbuscatexto']);
                                        ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">Situação da OS:
                            </div>
                            <div class="col-md-8">
                                        <?php
                                            criaComboWorkflow(TPDID_OS,array("nome"=>"esdidsituaco","id"=>"esdidsituacao"));
                                        ?>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-md-4">
                                Situação da Obra na Supervisão
                            </div>
                            <div class="col-md-8">

                		                <?php
                				$situacaoObra = new SituacaoObra();
                		                $sql = $situacaoObra->listaCombo();
                		                $db->monta_combo("sobid", $sql, "S", "Selecione...", "ctrlSituacaoObra", '', '', '', 'N', 'sobid');
                		                ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Município            
                            </div>
                            <div class="col-md-8">
                			<?php

                						$municipio2 = new Municipio();
                						$dado 	   = $municipio2->listaCombo( array('estuf' => "SP") );
                						$habMun    = 'S';
                						echo $db->monta_combo("muncod", $dado, $habMun,'Selecione...','', '', '',200,'N','muncod');

                			 ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Empresa responsável
                            </div>
                            <div class="col-md-8">

                                        <?php
                                            $supervisao_Grupo_Empresa = new Supervisao_Grupo_Empresa();
                                            $empresas = $supervisao_Grupo_Empresa->listaComboEntidades();
                                            echo $db->monta_combo("entid", $empresas, 'S','Selecione...','', '', '',200,'N','sgeid');
                                        ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Situação da supervisão:
                            </div>
                            <div class="col-md-8">

                                        <?php
                                            criaComboWorkflow(WF_TPDID_LAUDO_SUPERVISAO_EMPRESA,array("nome"=>"esdidsupervisao","id"=>"esdidsupervisao","option" => array("nao_iniciado"=>"Não Iniciado")), true);
                                        ?>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-md-4">
                                Possui supervisão atrasada?
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" id="atrasado_sim" <?php echo $_POST['atrasado'] == "sim" ? "checked='checked'" : "" ?> name="atrasado" value="sim"/>  Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" id="atrasado_nao" <?php echo $_POST['atrasado'] == "nao" ? "checked='checked'" : "" ?> name="atrasado" value="nao"/>   Não 
                                    </label>
                                    <label class="btn btn-default <?= empty($_POST['atrasado'])?'active':''?>">
                                        <input type="radio" id="atrasado_todos" <?php echo $_POST['atrasado'] == "todos" ? "checked='checked'" : "" ?> name="atrasado" value="todos" class=""/> Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                Supervisão homologada com atraso
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "sim" ? "checked='checked'" : "" ?> value='sim' />Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "nao" ? "checked='checked'" : "" ?> value='nao' />Não
                                    </label>
                                    <label class="btn btn-default <?= empty($_POST['h_atrasada'])?'active':''?>">
                                        <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "todas" ? "checked='checked'" : "" ?> value='todas' />Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                                No cadastro da obra o endereço está correto?
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" id="endereco_obr_sim"   name="endereco_obr" value="sim"   <?php echo $_POST['endereco_obr'] == "sim" ? "checked='checked'" : "" ?> />   Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" id="endereco_obr_nao"   name="endereco_obr" value="nao"   <?php echo $_POST['endereco_obr'] == "nao" ? "checked='checked'" : "" ?> />   Não
                                    </label>
                                    <label class="btn btn-default <?= empty($_POST['endereco_obr'])?'active':''?>">
                                        <input type="radio" id="endereco_obr_todos" name="endereco_obr" value="todos" <?php echo $_POST['endereco_obr'] == "todos" ? "checked='checked'" : "" ?> /> Todas
                                    </label>
                                </div>    
                            </div>
                        </div>
                    </div> <!-- col-md-6 --->
                    <div class="col-md-6 ">

                        <div class="row">
                            <div class="col-md-4">
                            Obras MI?
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="obrami" id="" value="S" <?= ( $_POST["obrami"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="obrami" id="" value="N" <?= ( $_POST["obrami"] == "N" ? "checked='checked'" : "" ) ?> /> Não
                                    </label>
                                    <label class="btn btn-default <?= empty($_POST['obrami'])?'active':''?>">
                                        <input type="radio" name="obrami" id="" value="T" <?= ( $_POST["obrami"] == "T" || $_POST["obrami"] == "" ? "checked='checked'" : "" ) ?>/> Todas
                                    </label>
                                </div>    
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                            Tipo de Supervisão
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="sosterreno" id="" value="f" <?= ($_POST["sosterreno"] == 'f') ? 'checked="checked"' : '' ?> > Obra
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="sosterreno" id="" value="t"  <?= ($_POST["sosterreno"] == 't') ? 'checked="checked"' : '' ?> > Implantação de Obra
                                    </label>
                                    <label class="btn btn-default <?= empty($_POST['sosterreno'])?'active':''?>">
                                        <input type="radio" name="sosterreno" id="" value=""  <?= (empty($_POST["sosterreno"])) ? 'checked="checked"' : '' ?> > Todas
                                    </label>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-4">
                            Problema Grave
                            </div>
                            <div class="col-md-8">
                                <div class="btn-group" data-toggle="buttons">
                                    <label class="btn btn-default">
                                        <input type="radio" name="sueproblema" id="" value="S" <?= ( $_POST["sueproblema"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                                    </label>
                                    <label class="btn btn-default">
                                        <input type="radio" name="sueproblema" id="" value="N" <?= ( $_POST["sueproblema"] == "N" ? "checked='checked'" : "" ) ?> /> Não
                                    </label>
                                    <label class="btn btn-default <?= empty($_POST['sueproblema'])?'active':''?>">
                                        <input type="radio" name="sueproblema" id="" value="T" <?= ( $_POST["sueproblema"] == "T" || $_POST["sueproblema"] == "" ? "checked='checked'" : "" ) ?>/> Todas
                                    </label>
                                </div>
                            </div>
                        </div>


                        <div class="row ">
                            <div class="col-md-4">
                                Tipo de Obra:
                            </div>
                            <div class="col-md-8">

                                <?php
                                $tipoObra = new TipoObra();
                                $db->monta_combo( "tobid", $tipoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "tobid", false, null, null, null, "chosen-select" );
                                ?>
                                            
                            </div>
                        </div>
                        <div class="row ">
                            <div class="col-md-4">
                                Classificação da Obra:
                            </div>
                            <div class="col-md-8">
                                            <?php
                                            $classificacaoObra = new ClassificacaoObra();
                                            $db->monta_combo( "cloid", $classificacaoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "cloid" );
                                            ?>
                            </div>
                        </div>


                        <?php
                        if ( $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL ):
                        ?>
                        <div class="row ">
                            <div class="col-md-4">
                                Tipologia da Obra:
                            </div>
                            <div class="col-md-8">
                                <?php
                                    $tipologiaObra = new TipologiaObra();
                                    $db->monta_combo( "tpoid", $tipologiaObra->listaCombo( array("orgid" => $_SESSION['obras2']['orgid']) ), "S", "Todas", "", "", "", 200, "N", "tpoid" );
                                ?>
                            </div>
                        </div>
                            
                        <?php
                        endif;
                        ?>
                        <div class="row ">
                            <div class="col-md-4">
                                Programa:
                            </div>
                            <div class="col-md-8">
                                        <?php
                                        $programa = new ProgramaFonte();
                                        $db->monta_combo( "prfid", $programa->listaCombo( array("orgid" => $_SESSION['obras2']['orgid']) ), "S", "Todos", "", "", "", 200, "N", "prfid" );
                                        ?>
                            </div>
                        </div>
                        <div class="row ">
                            <div class="col-md-4">
                                Município:
                            </div>
                            <div class="col-md-8" id="td_municipio">

                                    <?php
                                                $municipio = new Municipio();
                                                $dado      = $municipio->listaCombo( array('estuf' => "SP") );
                                                $habMun    = 'S';
                                        echo $db->monta_combo("muncod", $dado, $habMun,'Selecione...','', '', '',200,'N','muncod');
                                    ?>
                            </div>
                        </div>
                        <div class="row ">
                            <div class="col-md-4">
                                Esfera:
                            </div>
                            <div class="col-md-8" >
                                        <?php
                                        $sql = Array(Array('codigo'=>'M', 'descricao'=>'Municipal'),
                                                                 Array('codigo'=>'E', 'descricao'=>'Estadual'),
                                                                 Array('codigo'=>'F', 'descricao'=>'Federal'));
                                        $db->monta_combo('empesfera',$sql, 'S','Selecione...','','','',200,'N', 'empesfera');
                                        ?>
                            </div>
                        </div>    
                        <div class="row ">
                            <div class="col-md-4">
                                Valor da Obra:
                            </div>
                            <div class="col-md-8" >                                
                                        De:&nbsp;
                                        <?php
                                            echo campo_texto( 'empvalorprevisto_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                            echo 'Até:&nbsp;';
                                            echo campo_texto( 'empvalorprevisto_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                        ?>
                            </div>
                        </div>

                    </div> <!-- md-6 ---->
                </div>
        </div> <!--- row tr_avancada--->
    </div> 



    <div class="row form-filters text-center">
        <div class="col">

            <button type="button" name="pesquisar" class="btn btn-success pesquisar">
                <span class="glyphicon glyphicon-search"></span> Pesquisar
            </button>
 
            <button  name="btn_todas" value="Ver Todas" class="btn btn-primary" onclick="window.location.href=window.location" > 
                <span class="glyphicon glyphicon-align-justify"></span> 
                Ver Todas
            </button>
            <? if(possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) )):?>
            <button type="button" name="ver-maps" value="Mapa" class="ver-maps btn btn-primary" >
                <span class="glyphicon glyphicon-map-marker"></span>
                Mapa
            </button>
            <? endif; ?>
            <button type="button" name="exportar_para_xls" value="Exportar XLS" class="exportarxls btn btn-primary" >
                <span class="glyphicon glyphicon-download-alt"></span>
                Exportar XLS
            </button>

            <button type="button" name="exportar_para_xls_paralizadas" value="Exportar XLS Paralisadas" class="exportarxlsparalizadas btn btn-primary" >
                <span class="glyphicon glyphicon-download-alt"></span>
                Exportar XLS Paralisadas
            </button>


        </div>
    </div>    

</form>


<script>
	$1_11(document).ready(function () {
			$1_11('select[name="tobid"]').chosen();
			$1_11('select[name="cloid"]').chosen();
			$1_11('select[name="tpoid"]').chosen();
			$1_11('select[name="prfid"]').chosen();
			$1_11('select[name="estuf"]').chosen();
			$1_11('select[name="muncod"]').chosen();
			$1_11('select[name="empesfera"]').chosen();
			$1_11('select[name="esdidsituaco"]').chosen();
			$1_11('select[name="sobid"]').chosen();
			$1_11('select[name="entid"]').chosen();
			$1_11('select[name="esdidsupervisao"]').chosen();
			$1_11('select[name="esdidsupervisao[]"]').chosen();
	});
</script>
<?php
require APPRAIZ . 'obras2/includes/principal/listaEmpreendimentoEmpresa/js.php';

$empreendimento = new Empreendimento();
$sql 		= $empreendimento->listaEmpreendimentoOS($_POST);
//ver(simec_htmlentities($sql));
if($_POST['xls']) {
    exibeXls($_POST['xlsparalizada'],  $sql);
    
} elseif($_POST['mapa']) {
    exibeMapa($sql);
} elseif($_POST['op'] == 'pesquisar') {
	exibeLista($sql);
}

