<?php
switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=utf-8');

		$municipio = new Municipio();
		echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
		exit;		
}
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório - Painel FNDE', 'Painel FNDE' );

$arrSituacao = montaPainelSituacaoEstado($_POST);
//ver($arrSituacao); exit;
$totcolunas = array();

$estado = new Estado();
$estados = $estado->recuperarTodos('*', null, 'estuf ASC');


//
$ckd_mi  = '';
$ckd_con = '';
if(isset($_POST['tpoid'])){
    $tpoid   = $_POST['tpoid'];
    
    foreach ($tpoid as $key => $value) {
        if($value == 'obra_mi'){
            $ckd_mi = 'checked=checked';
        }
        if($value == 'obra_con'){
            $ckd_con = 'checked=checked';
        }
    }
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<form action="obras2.php?modulo=relatorio/relatorioPainelFnde&acao=A" method="post" name="filtro" id="filtro">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">

        <?php
        // Programa
        $stSql = "SELECT    prfid AS codigo,
                    		prfdesc AS descricao
                  FROM      obras2.programafonte
                 ORDER BY   prfdesc";
        
        if(!empty($_POST['prfid'][0])){
            $stSqlSelecionados = "SELECT        prfid AS codigo,
                        						prfdesc AS descricao
                     			  FROM          obras2.programafonte
                     			  WHERE 		prfid IN (". implode(', ', $_POST['prfid']) .")
                     			  ORDER BY      prfdesc ";
        }
        
        mostrarComboPopup( 'Programa', 'prfid',  $stSql, $stSqlSelecionados, 'Selecione o(s) Programa(s)' );
        
        // Fonte
        $stSql = "SELECT    tooid AS codigo,
                    		toodescricao AS descricao
                 FROM       obras2.tipoorigemobra
                 WHERE      toostatus = 'A'
                 ORDER BY   toodescricao ";
        
        if(!empty($_POST['tooid'][0])){
            $sql_carregados = "SELECT   tooid AS codigo,
                        				toodescricao AS descricao
                     		   FROM     obras2.tipoorigemobra
                     		   WHERE    toostatus = 'A' AND tooid IN (".  implode(', ', $_POST['tooid']).")
                     		   ORDER BY toodescricao ";
        }
        mostrarComboPopup( 'Fonte', 'tooid',  $stSql, $sql_carregados, 'Selecione a(s) Fonte(s)' );
    ?>
    <tr>
        <td bgcolor="#DCDCDC" style="text-align: right">Esfera:</td>
        <td bgcolor="#F5F5F5">
            <?php
            $sql = Array(Array('codigo' => 'E', 'descricao' => 'Estadual'),
                Array('codigo' => 'M', 'descricao' => 'Municipal'));
            $db->monta_combo('esfera', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'esfera');
            ?>
        </td>
    </tr>
    <tr>
        <td bgcolor="#DCDCDC" style="text-align: right">Tipo de Obra:</td>
        <td bgcolor="#F5F5F5">
            <input type="checkbox" name="tpoid[]" id="obra_con" value="obra_con" <?=$ckd_con?> > Metodologia Convencional &nbsp;
            <input type="checkbox" name="tpoid[]" id="obra_mi" value="obra_mi"   <?=$ckd_mi?>> Metodologia Inovadora 
        </td>
    </tr>
    <tr>
        <td bgcolor="#CCCCCC"></td>
        <td bgcolor="#CCCCCC">
            <input type="button" value="Visualizar" onclick="exibeRelatorio();" style="cursor: pointer;"/>
        </td>
    </tr>

    </table>
</form>
<br />
<table style="width: 95%;" align="center" cellpadding="2" cellspacing="0" class="listagem painel-listagem" >
    <thead>
        <tr class="fundo_tr_wap">
            <td onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="fundo_td_wap center bold">Situação</td>
            <?php foreach($estados as $est) : ?>
                <td onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="fundo_td_wap center bold" class="fundo_td_wap center bold"><?=$est['estuf']?></td>
            <?php endforeach; ?>
            <td onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="fundo_td_wap center bold" class="fundo_td_wap center bold" >Total</td>
        </tr>
    </thead>
    <tbody>
    <?php
        if($arrSituacao[0]):
            $count = 0;
            foreach($arrSituacao as $keys => $sit) :
                ?>
                    <?php if($count%2 == 0) $color = ''; else $color = '#F7F7F7';?>
                    <?php
                        $aguardandoEmpenho = $arrSituacao[$keys]['esdid'] == 683;
                        $situacaoobra = $arrSituacao[$keys]['situacaoobra'];
                        $esdId = $arrSituacao[$keys]['esdid'];
                        unset($arrSituacao[$keys]['esdid']);
                    ?>
                    <tr bgColor="<?=$color?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$color?>';">
                    <?php foreach($arrSituacao[$keys] as $key => $col) :
                        
                        $totcolunas[$key] += $col;

                        $cssClass = $relValue = '';

                        if ($esdId == 693 && $situacaoobra == '01-Concluídas até 2010') {
                            $cssClass = 'ate2010';
                            $relValue = 'ate2010';
                        }

                        if ($esdId == 693 && $situacaoobra == '02-Concluídas após 2010') {
                            $cssClass = 'apos2010';
                            $relValue = 'apos2010';
                        }

                        if ($esdId == 693 && $situacaoobra == '02-Sem data de conclusão') {
                            $cssClass = 'semconclusao';
                            $relValue = 'semconclusao';
                        }

                        //--- intervalo de datas
                        if ($esdId == 690 && $situacaoobra == '03-Execução (até 25%)') {
                            $cssClass = '25';
                        }

                        if ($esdId == 690 && $situacaoobra == '04-Execução (de 25% a 50%)') {
                            $cssClass = '25-50';
                        }

                        if ($esdId == 690 && $situacaoobra == '05-Execução (de 50% a 75%)') {
                            $cssClass = '50-75';
                        }

                        if ($esdId == 690 && $situacaoobra == '06-Execução (acima de 75%)') {
                            $cssClass = '75';
                        }
                        
                        $href = '';
                        if (!$aguardandoEmpenho && $count != 0 && $key != 'coluna1' && $key != 'situacaoobra') {
                            $href = "href=\"/obras2/obras2.php?modulo=relatorio/relatorioPainelFnde&acao=A&$key|$esdId\" data-param=\"".$relValue."\" class=\"irParaLista ".$cssClass."\" rel=\"$key|$esdId\" ";
                        }
                        
                        ?>
                        <td>
                            <?php if ($col == 0) { ?>
                                <?=str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-","09-","10-","11-","12-","00-","13-"),"",$col)?>
                            <?php } else if(!empty($href)) { ?>
                                <a <?=$href?>>
                                    <?=str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-","09-","10-","11-","12-","00-","13-"),"",$col)?>
                                </a>
                            <?php } else { ?>
                                <?php if (!$aguardandoEmpenho && $key <> 'situacaoobra') : ?>
                                <a rel="|<?=$esdId;?>" data-param="<?=$relValue?>" class="irParaLista <?=$cssClass?>" href="/obras2/obras2.php?modulo=relatorio/relatorioPainelFnde&acao=A&|<?=$esdId;?>">
                                    <?=str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-","09-","10-","11-","12-","00-","13-"),"",$col)?>
                                </a>
                                <?php else : ?>
                                    <?=str_replace(array("01-","02-","03-","04-","05-","06-","07-","08-","09-","10-","11-","12-","00-","13-"),"",$col)?>
                                <?php endif; ?>
                            <?php } ?>
                        </td>
                    <?php endforeach; ?>
                    </tr>
                <?php
                $count++;
            endforeach;
            $totcolunas['situacaoobra'] = 'TOTAL'; 
        ?>
            <tr bgColor="#e9e9e9" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#e9e9e9';">
            <?php foreach($totcolunas as $key => $col): ?>
                    <td style="font-weight: bold;">
                        <?php if ($key == 'coluna1' || $key == 'situacaoobra' ) { ?>
                            <?=$col;?>
                        <?php } else { ?>
                            <a class="irParaLista" rel="<?=$key;?>|" href="/obras2/obras2.php?modulo=relatorio/relatorioPainelFnde&acao=A&<?=$key;?>|">
                                <?=$col;?>
                            </a>
                        <?php } ?>
                    </td>
            <?php endforeach; ?>
            </tr>
        <?php
        endif;
        ?>
    </tbody>
</table>

<form style="display:none" method="post" id="formListaObra" name="formListaObra" action="/obras2/obras2.php?modulo=principal/listaObras&acao=A&orgid=3">
    <input id="esdid" name="esdid[]" value="" />
    <input id="estuf" name="estuf[]" value="" />
</form>

<script type="text/javascript">

    var formulario = document.getElementById('filtro');
    function exibeRelatorio(){

        if(document.getElementById('prfid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'prfid' ) );
        }
        if(document.getElementById('tooid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'tooid' ) );
        }

        formulario.submit();
    }

   /**
    * Alterar visibilidade de um bloco.
    * 
    * @param string indica o bloco a ser mostrado/escondido
    * @return void
    */
    function onOffBloco( bloco ){
    var div_on = document.getElementById( bloco + '_div_filtros_on' );
    var div_off = document.getElementById( bloco + '_div_filtros_off' );
    var img = document.getElementById( bloco + '_img' );
    var input = document.getElementById( bloco + '_flag' );
        if ( div_on.style.display == 'none' ){
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }else{
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }

   /**
    * Alterar visibilidade de um campo.
    * 
    * @param string indica o campo a ser mostrado/escondido
    * @return void
    */
    function onOffCampo( campo ){
        var div_on = document.getElementById( campo + '_campo_on' );
        var div_off = document.getElementById( campo + '_campo_off' );
        var input = document.getElementById( campo + '_campo_flag' );
        if ( div_on.style.display == 'none' ){
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else{
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
</script>

<script type="text/javascript">
    $('.irParaLista').click(function(e){
        e.preventDefault();
        $('#esdid').val($(this).attr('rel').split('|')[1]);
        $('#estuf').val($(this).attr('rel').split('|')[0]);
        
        if(document.getElementById('prfid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'prfid' ) );
        }
        if(document.getElementById('tooid_campo_flag').value == "1"){
            selectAllOptions( document.getElementById( 'tooid' ) );
        }
        
        $('#formListaObra').append($('#filtro select').clone());
        
        var tipoFilto = $(this).parents('tr:eq(0)').find('td:eq(0)').html().trim();
        
        /*console.log(tipoFilto.indexOf("Execução"));
        console.log(tipoFilto.indexOf("(0%"));*/
        
        if (tipoFilto.indexOf("Execução") > -1) {
            if (tipoFilto.indexOf("(0%") > -1) {
                
                 $('#formListaObra').append($('<input name="percentualinicial" value="0" />'));
                 $('#formListaObra').append($('<input name="percentualfinal" value="80" />'));
            } else if (tipoFilto.indexOf("(0%") == -1) {
                
                $('#formListaObra').append($('<input name="percentualinicial" value = "81" />'));
                $('#formListaObra').append($('<input name="percentualfinal" value = "100" />'));
                var classCss = $(this).attr("class").split(" ")[1];
                if (classCss) {

                    switch(classCss) {
                        case '25':
                            $('#formListaObra').append($('<input name="percentualinicial" value="0" />'));
                            $('#formListaObra').append($('<input name="percentualfinal" value="25" />'));
                            break;
                        case '25-50':
                            $('#formListaObra').append($('<input name="percentualinicial" value="25" />'));
                            $('#formListaObra').append($('<input name="percentualfinal" value="50" />'));
                            break;
                        case '50-75':
                            $('#formListaObra').append($('<input name="percentualinicial" value="50" />'));
                            $('#formListaObra').append($('<input name="percentualfinal" value="75" />'));
                            break;
                        case '75':
                            $('#formListaObra').append($('<input name="percentualinicial" value="75" />'));
                            $('#formListaObra').append($('<input name="percentualfinal" value="100" />'));
                            break;
                    }

                    $('#formListaObra').append($('<input name="not(obridpai)" value="1" />'));
                }
            }
        }
        
        if($(this).attr("data-param")) {
            $('#formListaObra').append($('<input name="dataparam" value="'+$(this).attr("data-param")+'" />'));
        }
        
        $('#formListaObra').submit();
    });
</script>