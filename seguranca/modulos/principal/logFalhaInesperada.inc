<?php

$arrayTipoErro[] = array('codigo'=>'DB', 'descricao'=>'Banco de Dados');
$arrayTipoErro[] = array('codigo'=>'PR', 'descricao'=>'Programação');
$arrayTipoErro[] = array('codigo'=>'QB', 'descricao'=>'Queda no Banco');
$arrayTipoErro[] = array('codigo'=>'WS', 'descricao'=>'WebService');
$arrayTipoErro[] = array('codigo'=>'EN', 'descricao'=>'Encoding no Banco');
$arrayTipoErro[] = array('codigo'=>'PD', 'descricao'=>'Erro na Conexão');
$arrayTipoErro[] = array('codigo'=>'DC', 'descricao'=>'Diretório Cheio');
$arrayTipoErro[] = array('codigo'=>'AI', 'descricao'=>'Arquivo inexistente');
$arrayTipoErro[] = array('codigo'=>'DV', 'descricao'=>'Diversos');

if( !empty($_REQUEST['errid']) ){
    $sql = "SELECT errtipo,
                   CASE
                        WHEN errtipo = 'DB' THEN 'Banco de dados'
                        WHEN errtipo = 'PR' THEN 'Programação'
                        WHEN errtipo = 'QB' THEN 'Queda no banco'
                        WHEN errtipo = 'WS' THEN 'WebService'
                        WHEN errtipo = 'EN' THEN 'Encoding no banco'
                        WHEN errtipo = 'PD' THEN 'Erro na Conexão'
                        WHEN errtipo = 'DC' THEN 'Diretório Cheio'
                        WHEN errtipo = 'AI' THEN 'Arquivo inexistente'
                        WHEN errtipo = 'DV' THEN 'Diversos'
                        ELSE errtipo
                   END as tipo_erro,
                   e.errdescricaocompleta,
                   TO_CHAR(e.errdata, 'DD/MM/YYYY HH24:MI:SS') as errdata,
                   e.errarquivo,
                   e.errlinha,
                   e.usucpf,
                   u.usunome,
                   s.sisdsc 
            FROM seguranca.erro e 
            LEFT JOIN  seguranca.sistema s USING(sisid)
            LEFT JOIN seguranca.usuario u USING(usucpf) 
            WHERE e.errid = {$_REQUEST['errid']}";
    $arr = $db->carregar($sql);
    extract($arr[0]);
    if( strlen($usucpf) == 11 ){
        $usucpf = substr($usucpf, 0, 3).'.'.
                  substr($usucpf, 3, 3).'.'.
                  substr($usucpf, 6, 3).'-'.
                  substr($usucpf, 9, 2);

    }
    echo "<div style='font-size: 14px; font-family: tahoma; font-weight: bold; margin-bottom: 2px'>Tipo de Erro: {$tipo_erro}</div>";
    echo "<div style='font-size: 14px; font-family: tahoma; font-weight: bold; ; margin-bottom: 2px'>Sistema: {$sisdsc}</div>";
    echo "<div style='font-size: 13px; font-family: tahoma; margin-bottom: 2px'>Data do Erro: {$errdata}</div>";
    echo "<div style='font-size: 13px; font-family: tahoma; margin-bottom: 2px'>Erro na Linha: {$errlinha}</div>";
    echo "<div style='font-size: 13px; font-family: tahoma; margin-bottom: 2px'>Erro no Arquivo: {$errarquivo}</div>";
    echo "<div style='font-size: 13px; font-family: tahoma; margin-bottom: 2px'>Usuário: {$usunome} - {$usucpf}</div>";

    echo "<br>";
    echo $errdescricaocompleta;
    die;
}

include APPRAIZ ."includes/cabecalho.inc";
require_once APPRAIZ . 'includes/library/simec/view/Helper.php';

$simec = new Simec_View_Helper();

$_REQUEST['aba'] = ($_REQUEST['aba'] ? $_REQUEST['aba'] : '1');

if($_REQUEST['aba'] == 1){
    $url = 'seguranca/seguranca.php?modulo=principal/logFalhaInesperada&acao=A&aba=1';
}

if($_REQUEST['aba'] == 2){
    $url = 'seguranca/seguranca.php?modulo=principal/logFalhaInesperada&acao=A&aba=2';
}
$abasQuestoes= array(
		0 => array( "descricao" => "Log de Erros", "link"	=> "seguranca.php?modulo=principal/logErros&acao=A&aba=1"),
		1 => array( "descricao" => "Lista Erros Agrupados", "link"	=> "seguranca.php?modulo=principal/logFalhaInesperada&acao=A&aba=2")
);
echo '<br>';
echo $simec->tab ( $abasQuestoes , $url );
echo '<br>';
echo '<br>';

/* montando cabeçalho */
monta_titulo( "Log de Erros", "");

$_REQUEST['errdata'] = (empty($_REQUEST['errdata']) ? date('d/m/Y') : $_REQUEST['errdata']);

$filtro = '';
if( $_REQUEST['requisicao'] == 'pesquisafi' ){
	if( !empty($_REQUEST['sisid']) ){
		$filtro .= " and e.sisid = {$_REQUEST['sisid']} ";
	}
}

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

<form action="" method="post" name="formulariofi" id="formulariofi">
	<input type="hidden" name="requisicao" value="pesquisafi" id="requisicao"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td colspan="4" class="subtitulocentro">Filtros de Pesquisa</td>
		</tr>
        <tr>
            <td class="subtitulodireita" width="15%">Erros agrupados das últimas:</td>
            <td width="33%">
                <input type="text" size="5" value="<?=($_POST['horas'] == ''?'24':$_POST['horas'])?>" name="horas" id="horas" /> horas
            </td>
        </tr>

        <tr>
            <td class="subtitulodireita" width="15%">Tipo de Erro:</td>
            <td>
                <? $db->monta_combo('errtipo', $arrayTipoErro, 'S', 'Selecione', 'comboCursos', '', '', '300', 'N', 'ateid', false, $_REQUEST['errtipo']); ?>
            </td>
        </tr>

		<tr>
                    <td class="subtitulodireita" width="15%">Sistema:</td>
                    <td width="33%">
                            <?php 
                                    $sisid = $_REQUEST["sisid"];
                                    $sql = "select e.sisid as codigo, e.sisdsc as descricao from seguranca.sistema e order by e.sisid";
                                    $db->monta_combo("sisid",$sql,'S',"Todas",'','','','500','N','sisid', '', '', '', 'class="sel_chosen"');
                            ?>
                    </td>
		</tr>
		<tr bgcolor="#D0D0D0">
                    <td colspan="2" align="center">
                            <input type="button" value="Pesquisar" id="btnPesquisar" name="btnPesquisar" onclick="pesquisafiErros();" style="cursor: pointer;"/>
                    </td>
		</tr>
	</table>
</form>
<div id="dialog_recurso" title="Visualizar XML" style="display: none;" >
	<div style="padding:5px;text-align:justify;" id="mostraRetornoRecurso"></div>
</div>

<script language="javascript" type="text/javascript" src="/seguranca/js/logerro.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script language="javascript" type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.min.js"></script>
<script type="text/javascript">
jq('.sel_chosen').chosen({allow_single_deselect:true});

function modalDetalheErro(id){
    jQuery("#modalDE").modal();
    jQuery('#name="errid"]').val(id);
}


function pesquisafiErros(){
    //Confere o preenchimento do campo "Erros agrupados das últimas:"
    if( $('#horas').val() == '' ){
        alert("O campo [Erros agrupados das últimas:] deve ser preenchido!");
        return false;
    }

    jQuery('[name="formulariofi"]').submit();
}
function carregaDados(obj){
        if(obj){
            window.location.href = window.location+'&sisid='+obj;
        }else{
            window.location.href = window.location;
        }
}
</script>
<?php
if( $_REQUEST['requisicao'] == 'pesquisafi' ){

    $where = "errdata >= (now() - INTERVAL '".$_POST['horas']." hours')";

    if( !empty($_POST['sisid']) ){
        $whereSistema = "sisid = {$_REQUEST['sisid']} AND";
    }

    $anoAtual = strftime("%Y", time());
    $anos = ($anoAtual-1).','.$anoAtual;

    if( !empty($_REQUEST['errtipo']) ){
        $whereTipoErro = "errtipo = '{$_REQUEST['errtipo']}' AND";
    }

    $whereGeral = "extract(YEAR FROM e.errdata) IN ({$anos}) AND
                       e.errdescricaocompleta IS NOT NULL AND                       
                       {$whereSistema} 
                       {$whereTipoErro} 
                       {$where}";

    //Preparar um array com os mesmo parametros da consulta para que possamos pegar o errid pela descrição
    //para passar para a função que mostrará a descrição do erro, impossivel criar um indice por errdescricao na tabela
    //seguranca.erro tabela com mais de 4 milhoes de registros, usando array para pegar o errid foi a maneira mais rapida
    $sqlTotal = "SELECT errid, errdescricao, sisid FROM seguranca.erro e 
                 WHERE {$whereGeral} ORDER BY errid DESC";
    $arr = $db->carregar($sqlTotal);
    $arrayConfere = array();
    if( $arr !== false ):
        foreach( $arr as $linha ):
            extract($linha);
            $arrayConfere[$errid] = $errdescricao;
        endforeach;
    endif;
    $sql = "WITH werro AS (
                    SELECT
                        errid, 
                        errtipo,
                        errdescricao as err,                        
                        sisdsc,
                        errlinha        
                    FROM seguranca.erro e
                    LEFT JOIN seguranca.sistema s USING(sisid)                
                    WHERE {$whereGeral} 
                )
                SELECT     
                    '<div align=\"center\">
                        <a href=\"javascript: detalhaErro(\'#errid\');\">                   
                        <img title=\"Detalhar Erro\" src=\"/seguranca/imagens/detalhe.png\" style=\"width: auto; height: 25px; cursor: pointer\">
                        </a>
                    </div>' as acao,
                    ";
    if( empty($_POST['sisid']) ){
        $sql .= "we.sisdsc,";
    }
    $sql .= "
                    we.err,                    
                    count(*) as quantos                    
                FROM werro we
                GROUP BY we.err,";
    if( empty($_POST['sisid']) ) {
        $sql .= "we.sisdsc,";
    }
    $sql .= "
                         we.errlinha
                ORDER BY quantos DESC";

//    dbg($sql, d);

    if( empty($_POST['sisid']) ){
        $cabecalho = array('Ação', 'Sistema', 'Erro', 'Quantidade');
        $tamanho = array("5%", "15%", "70%", "10%");
    }else {
        //Se teve filtro de sistema, retirar a coluna sistema do agrupamento
        $cabecalho = array('Ação', 'Erro', 'Quantidade');
        $tamanho = array("5%", "85%", "10%");
    }

    $arrErrosMostra = array();

    $arrErros = $db->carregar($sql);

    if( $arrErros !== false ):
        foreach( $arrErros as $linha ):
            //Pegando o errid do array preparado acima
            $errid = array_search($linha['err'], $arrayConfere);

            $linha['acao'] = str_replace("#errid", $errid, $linha['acao']);
            $linha['err'] = substr($linha['err'], 0, 200).' ...';
            $arrErrosMostra[] = $linha;
        endforeach;
    endif;

    $db->monta_lista_array($arrErrosMostra, $cabecalho, 10, 4, 'N','','','',$tamanho);
}