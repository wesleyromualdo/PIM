<?php

$sql = "SELECT estuf, muncod FROM par3.instrumentounidade WHERE inuid = {$_REQUEST['inuid']}";
global $db;
$dados = $db->pegaLinha($sql);

//instancia a classe
$oSiope = new Par3_Model_Siope();

//receitas
$receitas = $oSiope->listarReceitas($dados);
$receitas = is_array($receitas) ? $receitas : array();
//fim receitas

//despesas
$despesas = $oSiope->listarDespesas($dados);
$despesas = is_array($despesas) ? $despesas : array();
//fim despesas

//cumprimento
$cumprimento = $oSiope->listarCumprimentos($dados);
$cumprimento = is_array($cumprimento) ? $cumprimento : array();
//fim despesas

if($dados['muncod']){
    $esferaTitulo = 'Municipal';
    $esferaTituloPlural = 'Municipais';
    $esferaEntidade = 'Municipio';
}elseif($dados['estuf']=='DF'){
    $esferaTitulo = 'Distrital';
    $esferaTituloPlural = 'Distritais';
    $esferaEntidade = 'Distrito Federal';
}else{
    $esferaTitulo = 'Estadual';
    $esferaTituloPlural = 'Estaduais';
    $esferaEntidade = 'Estado';
}

?>
<style>
    #divImpressao {
        width: 100% !important;
        display: block !important;
        white-space: initial !important;
    }

    #divImpressao p {
        white-space: initial !important;
    }
</style>
<br>
<?php
$arrAno = array(
    0=>"2011",
    1=>"2012",
    2=>"2013",
    3=>"2014",
    4=>"2015",
    5=>"2016"
);

$arrTipoDespesa = array(0=>"Educação Infantil",
    1=>"(Creche + Pré-Escola)",
    2=>"Ensino Fundamental",
    3=>"Ensino Médio",
    4=>"Ensino Superior",
    5=>"Salário Educação",
);

//percent
$totalDespesa = $oSiope->totalDespesa($dados);
$totalDespesa = is_array($totalDespesa) ? $totalDespesa : array();

$anoIni = 2011;
$anoFim = 2016;
?>
<div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">
    <h3>Despesas em Educação</h3>
    <table  cellpadding="2" cellspacing="2" width="100%" class="table table-striped table-bordered table-hover table-condensed tabela-listagem">
        <tr>
            <td align="center" style="background-color: #E0E0E0;">
                <b>Despesas</b>
            </td>
            <?php for ($ano = $anoIni; $ano <= $anoFim; $ano++) {?>
                <td align="center" style="background-color: #E0E0E0;">
                    <b><?echo $ano;?><br>(R$)</b>
                </td>
                <td align="center" style="background-color: #E0E0E0;">
                    <b>(%)</b>
                </td>
            <?php }?>
        </tr>
        <tr>
            <td align="center" style="background-color: #B8CCE2;">
                <div align="left" style="float: left"><img src="../imagens/mais.gif" id="imgmais_infantil" onclick="mostraTabela('infantil')"><img src="../imagens/menos.gif" id="imgmenos_infantil" style="display: none;" onclick="escondeTabela('infantil')"></div>
                <b>Educação Infantil</b>
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='EDUC INFANTIL' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="background-color: #B8CCE2;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="background-color: #B8CCE2;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
            <?php }	?>
        </tr>
        <tr id="tr1_infantil" style="display: none;">
            <td align="center" style="background-color: #EAEAEA">
                <b style="color: #4A8CCC;">Creche</b>&nbsp;
                <i class="fa fa-plus-circle" style="color: #000;"></i>&nbsp;
                <b style="color: #5C984E;">Pré-Escola</b>
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapaCre = '';
                $totEtapaPre = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='EDUC INFANTIL' && $d['no_creche_pre_escola']=='Creche' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapaCre += $d['vl_despesa_empenhada'];
                    }
                    if($d['ds_etapa']=='EDUC INFANTIL' && $d['no_creche_pre_escola']=='Pré-Escola' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapaPre += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="padding: 0px;">
                    <table align="center" width="100%" cellpadding="0" cellspacing="0" style="padding: 0px; line-height: 34px;">
                        <tr>
                            <td align="center" style="background-color: #73A4D4; color: #fff; border-right: 1px solid #E7E7E7; height: 100%; width: 50%">
                                <? echo $totEtapaCre ? number_format($totEtapaCre, 2, ",", ".") : '-'; ?>
                            </td>
                            <td align="center" style="background-color: #8AAF81; color: #fff;">
                                <? echo $totEtapaPre ? number_format($totEtapaPre, 2, ",", ".") : '-'; ?>
                            </td>
                        </tr>
                    </table>
                </td>
                <td align="center" style="background-color: #EAEAEA;">
                    -
                </td>
                <?php
            }
            ?>
        </tr>
        <tr id="tr2_infantil" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Profissionais do Magistério
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapaCre = '';
                $totEtapaPre = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Profissionais do Magistério' && $d['no_creche_pre_escola']=='Creche' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapaCre += $d['vl_despesa_empenhada'];
                    }
                    if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Profissionais do Magistério' && $d['no_creche_pre_escola']=='Pré-Escola' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapaPre += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="padding: 0px;">
                    <table align="center" width="100%" cellpadding="0" cellspacing="0" style="padding: 0px; line-height: 34px;">
                        <tr>
                            <td align="center" style="background-color: #73A4D4; color: #fff; border-right: 1px solid #E7E7E7; height: 100%; width: 50%; font-size: 10px;">
                                <? echo $totEtapaCre ? number_format($totEtapaCre, 2, ",", ".") : '-'; ?>
                            </td>
                            <td align="center" style="background-color: #8AAF81; color: #fff;">
                                <? echo $totEtapaPre ? number_format($totEtapaPre, 2, ",", ".") : '-'; ?>
                            </td>
                        </tr>
                    </table>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    $totEtapa = $totEtapaCre + $totEtapaPre;
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr id="tr3_infantil" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Profissionais de Serviço e Apoio Escolar
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapaCre = '';
                $totEtapaPre = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Profissionais de Serviço e Apoio E' && $d['no_creche_pre_escola']=='Creche' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapaCre += $d['vl_despesa_empenhada'];
                    }
                    if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Profissionais de Serviço e Apoio E' && $d['no_creche_pre_escola']=='Pré-Escola' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapaPre += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="padding: 0px;" >
                    <table align="center" width="100%" cellpadding="0" cellspacing="0"  style="padding: 0px; line-height: 34px;">
                        <tr>
                            <td align="center" style="background-color: #73A4D4; color: #fff; border-right: 1px solid #E7E7E7; height: 100%; width: 50%; font-size: 10px;">
                                <? echo $totEtapaCre ? number_format($totEtapaCre, 2, ",", ".") : '-'; ?>
                            </td>
                            <td align="center" style="background-color: #8AAF81; color: #fff; font-size: 10px;">
                                <? echo $totEtapaPre ? number_format($totEtapaPre, 2, ",", ".") : '-'; ?>
                            </td>
                        </tr>
                    </table>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    $totEtapa = $totEtapaCre + $totEtapaPre;
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr id="tr4_infantil" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Alimentação, Transporte, Material Didático,<br> Equipamentos, Instalações, Obras e Benfeitorias etc.
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapaCre = '';
                $totEtapaPre = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Alimentação, Transporte, Material' && $d['no_creche_pre_escola']=='Creche' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapaCre += $d['vl_despesa_empenhada'];
                    }
                    if($d['ds_etapa']=='EDUC INFANTIL' && $d['ds_profissionais']=='Alimentação, Transporte, Material' && $d['no_creche_pre_escola']=='Pré-Escola' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapaPre += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="padding: 0px;" >
                    <table align="center" width="100%" cellpadding="0" cellspacing="0" style="padding: 0px; line-height: 49px;">
                        <tr>
                            <td align="center" style="background-color: #73A4D4; color: #fff; border-right: 1px solid #E7E7E7; height: 100%; width: 50%; font-size: 10px;">
                                <? echo $totEtapaCre ? number_format($totEtapaCre, 2, ",", ".") : '-'; ?>
                            </td>
                            <td align="center" style="background-color: #8AAF81; color: #fff; font-size: 10px;">
                                <? echo $totEtapaPre ? number_format($totEtapaPre, 2, ",", ".") : '-'; ?>
                            </td>
                        </tr>
                    </table>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    $totEtapa = $totEtapaCre + $totEtapaPre;
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr>
            <td align="center" style="background-color: #D8E4BC;">
                <div align="left" style="float: left"><img src="../imagens/mais.gif" id="imgmais_fundamental" onclick="mostraTabela('fundamental')"><img src="../imagens/menos.gif" id="imgmenos_fundamental" style="display: none;" onclick="escondeTabela('fundamental')"></div>
                <b>Ensino Fundamental</b>
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='ENSINO FUNDAMENTAL' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="background-color: #D8E4BC;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="background-color: #D8E4BC;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
            <?php }	?>
        </tr>
        <tr id="tr1_fundamental" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Profissionais do Magistério
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='ENSINO FUNDAMENTAL' && $d['ds_profissionais']=='Profissionais do Magistério' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="font-size: 10px;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr id="tr2_fundamental" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Profissionais de Serviço e Apoio Escolar
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='ENSINO FUNDAMENTAL' && $d['ds_profissionais']=='Profissionais de Serviço e Apoio E' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="font-size: 10px;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr id="tr3_fundamental" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Alimentação, Transporte, Material Didático,<br> Equipamentos, Instalações, Obras e Benfeitorias etc.
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='ENSINO FUNDAMENTAL' && $d['ds_profissionais']=='Alimentação, Transporte, Material' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="font-size: 10px;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr>
            <td align="center" style="background-color: #FFFACD;">
                <div align="left" style="float: left"><img src="../imagens/mais.gif" id="imgmais_medio" onclick="mostraTabela('medio')"><img src="../imagens/menos.gif" id="imgmenos_medio" style="display: none;" onclick="escondeTabela('medio')"></div>
                <b>Ensino Médio</b>
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='MEDIO' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="background-color: #FFFACD;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="background-color: #FFFACD;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
            <?php }	?>
        </tr>
        <tr id="tr1_medio" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Profissionais do Magistério
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='MEDIO' && $d['ds_profissionais']=='Profissionais do Magistério' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="font-size: 10px;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr id="tr2_medio" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Profissionais de Serviço e Apoio Escolar
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='MEDIO' && $d['ds_profissionais']=='Profissionais de Serviço e Apoio E' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="font-size: 10px;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr id="tr3_medio" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Alimentação, Transporte, Material Didático,<br> Equipamentos, Instalações, Obras e Benfeitorias etc.
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='MEDIO' && $d['ds_profissionais']=='Alimentação, Transporte, Material' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="font-size: 10px;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr>
            <td align="center" style="background-color: #FFE4E1;">
                <div align="left" style="float: left"><img src="../imagens/mais.gif" id="imgmais_superior" onclick="mostraTabela('superior')"><img src="../imagens/menos.gif" id="imgmenos_superior" style="display: none;" onclick="escondeTabela('superior')"></div>
                <b>Ensino Superior</b>
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='SUPERIOR' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="background-color: #FFE4E1;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="background-color: #FFE4E1;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
            <?php }	?>
        </tr>
        <tr id="tr1_superior" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Profissionais do Magistério
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='SUPERIOR' && $d['ds_profissionais']=='Profissionais do Magistério' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="font-size: 10px;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr id="tr2_superior" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Profissionais de Serviço e Apoio Escolar
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='SUPERIOR' && $d['ds_profissionais']=='Profissionais de Serviço e Apoio E' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="font-size: 10px;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr id="tr3_superior" style="display: none;">
            <td align="center" style="font-size: 12px;">
                Alimentação, Transporte, Material Didático,<br> Equipamentos, Instalações, Obras e Benfeitorias etc.
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='SUPERIOR' && $d['ds_profissionais']=='Alimentação, Transporte, Material' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="font-size: 10px;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="font-size: 10px;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
                <?php
            }
            ?>
        </tr>
        <tr>
            <td align="center" style="background-color: #FBC79B;">
                <b>Salário Educação</b>
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totEtapa = '';
                foreach ($despesas as $d) {
                    if($d['ds_etapa']=='SALARIO EDUCACAO' && (int)$d['an_despesa']==(int)$ano ){
                        $totEtapa += $d['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="background-color: #FBC79B;">
                    <? echo $totEtapa ? number_format($totEtapa, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="background-color: #FBC79B;">
                    <?
                    $percent = '';
                    foreach ($totalDespesa as $total) {
                        if((int)$total['an_despesa']==(int)$ano ){
                            if($total['vl_despesa_empenhada']>0){
                                $percent = ($totEtapa/$total['vl_despesa_empenhada'])*100;
                            }else{
                                $percent = '';
                            }
                        }
                    }
                    echo $percent ? number_format($percent, 2, ",", "."): '-';
                    ?>
                </td>
            <?php }	?>
        </tr>
        <tr bgcolor="#CFCFCF">
            <td align="center" style="background-color: #F5F5F5;">
                <b>Total</b>
            </td>
            <?php
            for ($ano = $anoIni; $ano <= $anoFim; $ano++) {
                $totAno = '';
                foreach ($totalDespesa as $total) {
                    if((int)$total['an_despesa']==(int)$ano ){
                        $totAno = $total['vl_despesa_empenhada'];
                    }
                }
                ?>
                <td align="center" style="background-color: #F5F5F5;">
                    <? echo $totAno ? number_format($totAno, 2, ",", ".") : '-'; ?>
                </td>
                <td align="center" style="background-color: #F5F5F5;">
                    100
                </td>
                <?php
            }
            ?>
        </tr>
    </table>
</div>

<br>

<script>
    function mostraTabela(id){
        document.getElementById("imgmais_"+id).style.display = 'none';
        document.getElementById("imgmenos_"+id).style.display = '';
        document.getElementById("tr1_"+id).style.display = '';
        document.getElementById("tr2_"+id).style.display = '';
        document.getElementById("tr3_"+id).style.display = '';
        document.getElementById("tr4_"+id).style.display = '';
    }

    function escondeTabela(id){
        document.getElementById("imgmais_"+id).style.display = '';
        document.getElementById("imgmenos_"+id).style.display = 'none';
        document.getElementById("tr1_"+id).style.display = 'none';
        document.getElementById("tr2_"+id).style.display = 'none';
        document.getElementById("tr3_"+id).style.display = 'none';
        document.getElementById("tr4_"+id).style.display = 'none';
    }
</script>
