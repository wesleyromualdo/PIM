<?php
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "sase/classes/Model/Pneprev.inc";
include_once APPRAIZ . "sase/classes/Model/Pnedial.inc";

if ( $_POST['requisicao'] == 'valida_indicadores' ){
//    include(APPRAIZ."par/modulos/relatorio/listaIndicadoresPendentes.inc");
    include(APPRAIZ."par3/modulos/principal/planoTrabalho/listaIndicadoresPendentes.inc");
    exit;
}

if($_POST['naoQuantificado']){
    $subid = $_POST['subid'];
    $pneano = $_POST['pneano'];
    $muncod = $_POST['muncod'];
    $pnetipo = $_POST['pnetipo'];
    $pneid = $_POST['pneid'];
    $pneanoprevisto = $_POST['pneanoprevisto'];
    $naoqtf = $_POST['naoqtf'];
    naoQuantificado($subid, $pneano, $muncod, $pnetipo, $pneid, $pneanoprevisto, $naoqtf);
    exit;
}

$pneanoBrasil=0;
$pneanoUf=0;
$pneanoRegiao=0;
$pneanoMunicipio=0;
$pneanoMesorregiao=0;

if($_SESSION['par']['muncod']){
    $sql = "select mundescricao from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
    $_SESSION['par']['mundescricao'] = $db->pegaUm($sql);
}
if($_SESSION['par']['estuf']) {
    $sql = "select estdescricao from territorios.estado where estuf = '{$_SESSION['par']['estuf']}'";
    $_SESSION['par']['estdescricao'] = $db->pegaUm($sql);
}

if($_REQUEST['acao']){
    switch($_REQUEST['acao']){
        case 'nao_informado':
            naoInformado();
            exit;

        case 'valida_indicadores':
            validaIndicadores();
            exit;

        case 'altera_grafico':
            alteraGrafico();
            exit;
    }
}

//echo $_SESSION['par']['muncod'];

// CPF do administrador de sistemas
if( !$_SESSION['usucpf'] ){
    $_SESSION['usucpforigem'] = '00000000191';
    $_SESSION['usucpf'] = '00000000191';
}

if( !$db ){
    $db = new cls_banco();
}

if (! $_REQUEST['metid'])
    $_REQUEST['metid'] = 1;


if($_REQUEST['requisicaoAjax']){
    header('content-type: text/html; charset=utf-8');
    $_REQUEST['requisicaoAjax']();
    exit;
}

if($_REQUEST['requisicao']){
    $_REQUEST['requisicao']();
    exit;
}


function mostraTexto(){
    $meta = $_REQUEST['metid'];
    $submeta = $_REQUEST['submeta'];
    $sql = "SELECT s.subnotatecnica,s.subtitulo,m.mettitulo
	FROM sase.submetadial s
	LEFT JOIN sase.meta m ON(s.metid = m.metid)
	WHERE s.metid = $meta AND s.subiddial = $submeta";
    global $db;
    $texto = $db->carregar($sql);
    echo "<html>";
    echo "<body>";
    if($texto){
        foreach($texto as $t){
            echo "<p style=\"text-align: justify;\"><b>Meta $meta</b>: ".$t['mettitulo']."</p>";
            echo "<p style=\"text-align: justify;\"><b>Submeta: ".$t['subtitulo']."</b></p>";
            echo "<p style=\"text-align: justify;\">";
            if(!$t['subnotatecnica']){
                echo "Esta submeta não possui nota técnica.";
            }else{
                echo "<b>Nota Ténica</b><br>";
                echo $t['subnotatecnica'];
            }
            echo "</p>";
        }
        echo "<a onclick=\"window.print();return false;\"><img src=\"/pde/cockpit/images/pne/impressora.png\" alt=\"Imprimir\" title=\"Imprimir\" border=\"0\"></a>";
    }else{
        echo "<p style=\"text-align: justify;\"><b>Meta $meta</b>: ".$t['mettitulo']."</p>";
        echo "<p style=\"text-align: justify;\"><b>Submeta: ".$t['subtitulo']."</b></p>";
        echo "<p style=\"text-align: justify;\">Esta submeta não possui nota técnica</p>";
    }
    echo "</body>";
    echo "</html>";
}

function montarGrafico(){

    include APPRAIZ . "includes/jpgraph/jpgraph.php";
    include APPRAIZ . "includes/jpgraph/jpgraph_line.php";

    $ydata = explode(",", $_REQUEST['brasilCarregados']);
    $xdata = explode(",", $_REQUEST['anosCarregados']);

    if ($_REQUEST['regioes'])
    {
        $_REQUEST['regioes'] = str_replace("\\", "", $_REQUEST['regioes']);
        $regioes = explode(",", $_REQUEST['regioes']);
    }


    if ($_REQUEST['estados'])
        $estados = explode(",", $_REQUEST['estados']);

    if ($_REQUEST['mesoregioes'])
    {
        $_REQUEST['mesoregioes'] = str_replace("\\", "", $_REQUEST['mesoregioes']);
        $mesoregioes = explode(",", $_REQUEST['mesoregioes']);
    }

    $width=600;
    $height=250;

    $graph = new Graph($width,$height);
    $graph->SetScale('intlin');

    $periodo= $xdata[0].'-'.$xdata[sizeof($xdata)-1];

    // Setup margin and titles
    $graph->SetMargin(40,20,20,40);
    $graph->title->Set('Período');
    $graph->subtitle->Set($periodo);
    $graph->xaxis->title->Set('ANO');
    $graph->yaxis->title->Set('PNE');
    $graph->legend->SetFont(FF_FONT0,FS_NORMAL,10);
    $graph->legend->SetLineSpacing(5);
    $graph->legend->Pos(0.01,0.17);
    $graph->legend->SetFrameWeight(1);
    $graph->SetMargin(40, 150 ,0, 0);

    // Create the linear plot
    $lineplot=new LinePlot($ydata,$xdata);
    $lineplot ->SetWeight(3);
    $lineplot->SetColor('#0000ff');
    $lineplot->SetLegend('Brasil');

    // Add the plot to the graph
    $graph->Add($lineplot);

    global $db;

    $arrCores = array('#FF0000','#008000','#663300','#99CCCC','#FFA500','#6800FF','#003300','#CC00CC','#CCCCCC');
    $ctCores = 0;
    $ct = 0;

    if ($estados)
    {
        foreach ($estados as $uf)
        {
            $ct = $ct + 1;

            $ydata = array();

            $sql = "select round( pnevalor , 1) as pnevalor, estuf from sase.pnedial where estuf= '".$uf."'  AND subiddial =". $_REQUEST['subiddial'] . " and pnetipo = 'E' order by pneano";

            $arr = $db->carregar($sql);

            if ($arr)
            {
                foreach($arr as $p)
                {
                    array_push ( $ydata, $p['pnevalor']);
                }
            }
            ${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
            ${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
            ${'lineplot'.$ct} ->SetLegend($arr[0]['estuf']);
            ${'lineplot'.$ct} ->SetWeight(3);
            $graph->Add( ${'lineplot'.$ct});

            $ctCores = $ctCores + 1;

            if ($ctCores == 9 )
                $ctCores = 0;

        }
    }

    if ($regioes)
    {
        foreach ($regioes as $reg)
        {
            $ct = $ct + 1;

            $ydata = array();

            $sql = "select round( pnevalor , 1) as pnevalor, regdescricao from sase.pnedial p
					inner join territorios.regiao r on p.regcod = r.regcod
					where p.regcod= '$reg' AND subiddial =" . $_REQUEST['subiddial'] . " and pnetipo = 'R' order by pneano" ;


            $arr = $db->carregar($sql);

            if ($arr)
            {
                foreach($arr as $p)
                {
                    array_push ( $ydata, $p['pnevalor']);
                }
            }
            ${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
            ${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
            ${'lineplot'.$ct} ->SetLegend($arr[0]['regdescricao']);
            ${'lineplot'.$ct} ->SetWeight(3);
            $graph->Add( ${'lineplot'.$ct});

            $ctCores = $ctCores + 1;

            if ($ctCores == 9 )
                $ctCores = 0;

        }
    }

    if ($mesoregioes)
    {
        foreach ($mesoregioes as $mes)
        {
            $ct = $ct + 1;

            $ydata = array();

            $sql = "select round( pnevalor , 1) as pnevalor, mesdsc from sase.pnedial p
			inner join territorios.mesoregiao m on p.mescod = m.mescod
			where p.mescod= '$mes' AND subiddial =" . $_REQUEST['subiddial'] . " and pnetipo = 'S' order by pneano" ;


            $arr = $db->carregar($sql);

            if ($arr)
            {
                foreach($arr as $p)
                {
                    array_push ( $ydata, $p['pnevalor']);
                }
            }
            ${'lineplot'.$ct}  = new  LinePlot ( $ydata,$xdata );
            ${'lineplot'.$ct} ->SetColor($arrCores[$ctCores]);
            ${'lineplot'.$ct} ->SetLegend($arr[0]['mesdsc']);
            ${'lineplot'.$ct} ->SetWeight(3);
            $graph->Add( ${'lineplot'.$ct});

            $ctCores = $ctCores + 1;

            if ($ctCores == 9 )
                $ctCores = 0;

        }
    }

    $graph->Stroke();
}

function listarNomeMunicipios($municipios, $subiddial)
{

    global $db;

    foreach ($municipios as $mun)
    {
        $sql = "select mundescricao from sase.pnedial p
		inner join territorios.municipio m on m.muncod = p.muncod
		where p.muncod= ".$mun." AND subiddial =". $subiddial . " and pnetipo = 'M' order by pneano" ;

        $arr = $db->carregar($sql);
        if ($arr)
        {
            echo str_replace("\'","",$arr[0]['mundescricao']);
            echo"; ";
        }
    }
}

function listarDadosMunicipios($municipios, $subiddial)
{

    global $db;
    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';

    foreach ($municipios as $mun)
    {
        $sql = "select '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, mundescricao from sase.pnedial p
				inner join territorios.municipio m on m.muncod = p.muncod
				where p.muncod= ".$mun." AND subiddial =". $subiddial . " and pnetipo = 'M' order by pneano" ;

        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo "<tr><td>".str_replace("\'","",$arr[0]['mundescricao'])."</td>";
            foreach($arr as $p)
            {
                $p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

                echo $p['pnevalor'];
            }
            echo"</tr>";
        }
        else
        {
            return '';
        }
    }

    $sql =  "select estuf from territorios.municipio where muncod in (".implode(",", $municipios).") group by estuf order by 1";
    $arr = $db->carregarColuna($sql);

    return implode(",",$arr);

}

function listarDadosRegioes($regioes, $subiddial)
{
    global $db;
    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
    foreach ($regioes as $r)
    {
        $sql = "select pneiddial,   '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, regdescricao from sase.pnedial p
				inner join territorios.regiao r on r.regcod = p.regcod
				where r.regcod= '$r'  AND subiddial = $subiddial and pnetipo = 'R' order by pneano";
        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo "<tr><td>".$arr[0]['regdescricao']."</td>";
            foreach($arr as $p)
            {
                $p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

                echo $p['pnevalor'];
            }
            echo"</tr>";
        }
    }
}

function listarNomeRegioes($regioes, $subiddial)
{
    global $db;

    foreach ($regioes as $r)
    {
        $sql = "select regdescricao from sase.pnedial p
		inner join territorios.regiao r on r.regcod = p.regcod
		where r.regcod= '$r'  AND subiddial = $subiddial and pnetipo = 'R' order by pneano";
        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo $arr[0]['regdescricao'];
            echo "; ";
        }

    }
}

function listarNomeMesoregioes($mesoregioes, $subiddial)
{
    global $db;

    foreach ($mesoregioes as $m)
    {
        $sql = "select mesdsc from sase.pnedial p
		inner join territorios.mesoregiao m on p.mescod = m.mescod
		where p.mescod= '$m'  AND subiddial = $subiddial and pnetipo = 'S' order by pneano";

        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo $arr[0]['mesdsc'];
            echo"; ";
        }

    }
}

function listarDadosMesoregioes($mesoregioes, $subiddial)
{
    global $db;

    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
    foreach ($mesoregioes as $m)
    {
        $sql = "select pneiddial,   '<td>'||round( pnevalor , $valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, mesdsc from sase.pnedial p
		inner join territorios.mesoregiao m on p.mescod = m.mescod
		where p.mescod= '$m'  AND subiddial = $subiddial and pnetipo = 'S' order by pneano";

        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo "<tr><td>".$arr[0]['mesdsc']."</td>";
            foreach($arr as $p)
            {
                $p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

                echo $p['pnevalor'];

                //array_push ( $arrIDEstadosCarregado, $p['pneiddial']);
            }
            echo"</tr>";
        }
    }
}

function listarNomeEstados($estados, $subiddial)
{
    global $db;

    $arrIDEstadosCarregado = array();
    foreach ($estados as $uf)
    {
        $sql = "select estuf from sase.pnedial where estuf= '".$uf."'  AND subiddial =". $subiddial . " and pnetipo = 'E' order by pneano";

        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo $uf;
            echo"; ";
        }

    }

    return $arrIDEstadosCarregado;
}

function listarDadosEstados($estados, $subiddial)
{
    global $db;

    $arrIDEstadosCarregado = array();
    $valor = $_REQUEST['metid']==11 || $_REQUEST['metid']==14?'0':'1';
    foreach ($estados as $uf)
    {
        $sql = "select pneiddial,   '<td>'||round( pnevalor ,$valor)||'</td>' as pnevalor, round( pnevalor , 2) as pnevalorori, estuf from sase.pnedial where estuf= '".$uf."'  AND subiddial =". $subiddial . " and pnetipo = 'E' order by pneano";

        $arr = $db->carregar($sql);

        if ($arr)
        {
            echo "<tr><td>".$uf."</td>";
            foreach($arr as $p)
            {
                $p['pnevalor'] = str_replace(".",",",$p['pnevalor']);

                echo $p['pnevalor'];

                array_push ( $arrIDEstadosCarregado, $p['pneiddial']);
            }
            echo"</tr>";
        }
    }

    return $arrIDEstadosCarregado;
}

function salvarNaoInformado() {
    global $db;
    $_REQUEST = simec_utf8_decode_recursive($_REQUEST);
    $tabela = "sase.metainfcomplementar";
    if ($_SESSION['par']['itrid']==1 || $_SESSION['par']['itrid']==3){
        $sql = "select count(*) as qtd from {$tabela} where metid = {$_REQUEST['metid']} and estuf = '{$_SESSION['par']['estuf']}'";
    }else{
        $sql = "select count(*) as qtd from {$tabela} where metid = {$_REQUEST['metid']} and muncod = '{$_SESSION['par']['muncod']}'";
    }

    $rs = $db->carregar($sql);
    if ($rs[0]['qtd']>0){
        if ($_SESSION['par']['itrid']==1 || $_SESSION['par']['itrid']==3){
            $sql = "update {$tabela}
                       set micinfcomplementar = '" . $_REQUEST['pneinformcomplementar'] . "'
                     where metid = {$_REQUEST['metid']} and estuf = '{$_SESSION['par']['estuf']}'";
        }else{
            $sql = "update {$tabela}
                       set micinfcomplementar = '" . $_REQUEST['pneinformcomplementar'] . "'
                     where metid = {$_REQUEST['metid']} and muncod = '{$_SESSION['par']['muncod']}'";
        }
    }else{
        if ($_SESSION['par']['itrid']==1 || $_SESSION['par']['itrid']==3){
            $sql = "insert into {$tabela} (metid, estuf, micinfcomplementar) ".
                   "values ({$_REQUEST['metid']}, '{$_SESSION['par']['estuf']}', '" . $_REQUEST['pneinformcomplementar'] . "')";
        }else{
            $sql = "insert into {$tabela} (metid, muncod, micinfcomplementar) ".
                   "values ({$_REQUEST['metid']}, '{$_SESSION['par']['muncod']}', '" . $_REQUEST['pneinformcomplementar'] . "')";
        }
    }
    $db->executar($sql);
    if($db->commit()){
        echo 'true';
    } else {
        echo 'false';
    }
}



function salvarMeta18() {
    global $db;
    $pnetipo = ($_SESSION['par']['itrid']==1 || $_SESSION['par']['itrid']==3) ? 'E' : 'M';
    $anoprevisto = !empty($_REQUEST['pneanoprevisto']) ? $_REQUEST['pneanoprevisto'] : '';
    $pneano = $_REQUEST['pneano'];
    $subiddial = $_REQUEST['subiddial'];
    $pnepossuiplanoremvigente = $_REQUEST['pnepossuiplanoremvigente'];
    $pneplanorefcaput = $_REQUEST['pneplanorefcaput'];

    try{
        $pneprev = new Sase_Model_Pneprev();

        if ($pnetipo == 'E') {
            $sql = "insert into sase.pnedial (subiddial, estuf, pneano, pnetipo) " .
                "values ({$_REQUEST['subiddial']}, '{$_SESSION['par']['estuf']}', {$_REQUEST['pneano']}, '{$pnetipo}') returning pneiddial";
        } else {
            $sql = "insert into sase.pnedial (subiddial, muncod, pneano, pnetipo) " .
                "values ({$_REQUEST['subiddial']}, {$_SESSION['par']['muncod']}, {$_REQUEST['pneano']}, '{$pnetipo}') returning pneiddial";
        }
        $pneiddial = $db->pegaUm($sql);
        $db->commit();

        $pneprev->pneiddial = $pneiddial;
        $pneprev->subid = $subiddial;
        $pneprev->pnecpfinclusao = $_SESSION['usucpf'];
        $pneprev->pnedatainclusao = date('d/m/Y H:i:s');
        $pneprev->pnesemvalor = 'f';
        $pneprev->pnepossuiplanoremvigente = $pnepossuiplanoremvigente;
        $pneprev->pneplanorefcaput = $pneplanorefcaput;
        $pneprev->pneanoprevisto = $anoprevisto;
        $success = $pneprev->salvar(true, true, array("pneanoprevisto"));
        $pneprev->commit();
        if(!$success) throw new Exception('Erro ao salvar os dados.');

        echo "Dados salvos com sucesso.";
    }
    catch(Simec_Db_Exception $e){
        echo 'Erro: '.$e->getMessage();
    }
    catch(Exception $e) {
        echo 'Erro: '.$e->getMessage();
    }


//    $msg = Array('result' => true,
//                 'msg'    => 'Registro Salvo!');
//    echo json_encode($msg);
}

function listagemPrincipal(){
    global $db, $simec;
    $tabela = '';
    $contador=0;

    $arr = carregarMetas($_REQUEST['metid'], $_SESSION['par']['itrid']);
    $titulo = selecionaTitulo($_REQUEST['metid']);
    $valor = ($_REQUEST['metid']==11 || $_REQUEST['metid']==14) ?'0':'1';
    //ver($arr, d);
    foreach($arr as $count2 => $s):
        if ($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3) {
            if ($_SESSION['par']['estuf']) {
                if (!in_array($_REQUEST['metid'], array(19,20)) && !empty($s['subiddial'])) {
                    $pneanoUf = selecionaAno($s['subiddial'], 'E');
                }else{
                    $pneanoUf = '';
                }
                $dadosMun = retornaDadosEst($valor, $s['subiddial'], $_REQUEST['metid'], $_SESSION['par']['estuf'], $pneanoUf, $_SESSION['par']['itrid']);
                $existeDadosMun = (bool)$dadosMun;

            }
        } else {
            if ($_SESSION['par']['muncod']) {
                if (!in_array($_REQUEST['metid'], array(13,14,17,19,20))) {
                    $pneanoMunicipio=selecionaAno($s['subiddial'], 'M');
                }else{
                    $pneanoMunicipio='';
                }
                $dadosMun = retornaDadosMun($valor, $s['subiddial'], $_REQUEST['metid'], $_SESSION['par']['muncod'], $pneanoMunicipio, $_SESSION['par']['itrid']);
                $where = " and pne.muncod = '".$_SESSION['par']['muncod']."'";
                $existeDadosMun = (bool)$dadosMun;
            }
        }
        if(!isset($dadosMun['metid'])){
            $dadosMun['metid'] = $_REQUEST['metid'];
        }
        $subtitulo = $s['subtitulo'] === 'http://ideb.inep.gov.br/resultado/home.seam?cid=4212113'? 'Acesse as metas do IDEB em: <a target="_blank" href="http://ideb.inep.gov.br/resultado/home.seam?cid=4212113">ideb.inep.gov.br</a>': $s['subtitulo'];
        $qtdLegenda = 1;
        $subnotatecnica = '';
        $file = 'pne/notas_tecnicas/' . $s['subnotatecnica'];
        if($s['subnotatecnica'] && file_exists($file)){
            $subnotatecnica = '<a target="_blank" href="' . $file . '"><img height="30px" src="img/nt.png" alt="Nota Técnica" title="Nota Técnica" /></a>';
        }
        if($contador==0){
            $html = <<<HTML

    <script>
        $('.icheck-naoqtf').iCheck({
            checkboxClass: 'icheckbox_square-green'
        }).on('ifChecked', function(){
            $('#tbNormal_'+$(this).attr('subid')).hide();
            $('#slider_'+$(this).attr('subid')).slider( "option", "value", 0 );
            $('#txtSlider_'+$(this).attr('subid')).val();
            naoQuantificado($(this).attr('subid'), $(this).attr('pneano'), $(this).attr('muncod'), $(this).attr('pnetipo'), $(this).attr('pneid'), $(this).attr('pneanoprevisto'), $(this).attr('tipometa'), 't', $(this).attr('tipoacao'));
        }).on('ifUnchecked', function(){
            $('#tbNormal_'+$(this).attr('subid')).show();
            $('#slider_'+$(this).attr('subid')).slider( "option", "value", 0 );
            $('#txtSlider_'+$(this).attr('subid')).val();
            naoQuantificado($(this).attr('subid'), $(this).attr('pneano'), $(this).attr('muncod'), $(this).attr('pnetipo'), $(this).attr('pneid'), $(this).attr('pneanoprevisto'), $(this).attr('tipometa'), 'f', $(this).attr('tipoacao'));
        });

        function trataValorDial(maxval, valor, subid){
            if(valor != "" && !isNaN(valor)){
                if(valor <= maxval){
                    $('#slider_'+subid).slider( "option", "value", valor);
                } else {
                    alert('O valor máximo da Meta Prevista, para este indicador, é '+maxval);
                    $('#txtSlider_'+subid).val(maxval);
                    $('#slider_'+subid).slider( "option", "value", maxval);
                }
            }
        }

        $('.txtSlider')
            .keyup(function(e){
                var maxval = parseFloat($(this).attr('maxval'));
                var valor  = parseFloat($(this).val());
                var subid  = parseFloat($(this).attr('subiddial'));
                if(e.which != 190 && e.which != 188){
                    trataValorDial(maxval, valor, subid);
                }
            })
            .change(function(){
                var maxval = parseFloat($(this).attr('maxval'));
                var valor  = parseFloat($(this).val());
                var subid  = parseFloat($(this).attr('subiddial'));

                trataValorDial(maxval, valor, subid);
            });
    </script>

HTML;

            echo $html;
            echo "<table border=0 class=\"tabela_box_azul_escuro\" style=\"margin-top: 10px !important;margin-bottom: 10px !important;\" cellpadding=\"2\" cellspacing=\"1\" width=\"100%\" >";
            echo '    <tr>';
            echo '        <td style=\"font-size:16px;padding:10px 0 5px 5px;font-weight:bold;color: #333;">';
            echo '            <div style="margin-top:12px;"><div id="titulo-meta-some" style="float:left;" class="titulo_box">'.$titulo.' </div>';
            echo '            <div class="alert alert-info" id="titulo-meta-aparece" style="display:block;text-align:justify;">';
	        //echo '                <h2><small><b style="color: #333">Meta '.$_REQUEST['metid'].': '.$s['mettitulo'].'</b></small></h2>';
            $html = <<<HTML
                <h2>
                    <table>
                        <tr>
                            <td style="width: 67px; padding-right: 0px; padding-left: 0px; vertical-align: top;"><small><b style="color: #333">Meta {$_REQUEST['metid']}:</b></small></td>
                            <td><small><b style="color: #333">{$s['mettitulo']}</b></small></td>
                        </tr>
                    </table>
                </h2>
HTML;
            echo $html;
            echo '            </div>';
            echo '        </td>';
            echo '    </tr>';
        }
//        if($_REQUEST['metid'] == 5){
//            include_once('meta5html.inc');
//            break;
//        }



//        if (verificaMetas()) {
//            echo '    <tr>';
//            echo '        <td style=\"font-size:16px;padding:25px 0 5px 5px;font-weight:bold;color: #333;\">';
//            if (!$existeDadosMun) {
//                echo " <span style='padding:10px;'><input type=\"button\" name=\"btnNaoInfo\" id=\"btnNaoInfo\" onclick=\"naoInformado({$valor}, '{$pneano}', ".$s['subiddial'].", ".$_REQUEST['metid'].", '".$dados[0]['pneiddial']."', 'div_grfMun_" . $s['subiddial'] . "', true)\" value=\"Não Informado\" class=\"btn btn-primary\"/></span>";
//            }
//            echo '        </td>';
//            echo '    </tr>';
//        }
        if (!verificaMetas()) {
            $pneanoBrasil = selecionaAno($s['subiddial'], 'B');
            $dados = retornaDadosPne($valor, $_REQUEST['metid'], $s['subiddial'], $pneanoBrasil, $_SESSION['par']['itrid'], '');
            $existeResultado = (bool) $dados;
            if ($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3) {
                $pneano = selecionaAno($s['subiddial'], 'E');
                $meta = 'Estado';
                $tpacao = 'Estuf';
            } else {
                $pneano = selecionaAno($s['subiddial'], 'M');
                $meta = 'Município';
                $tpacao = 'Mun';
            }
            echo '   <tr>';
            echo "    <td style=\"font-size:16px;font-weight:bold;color: #333;\"><div class='well' style='padding: 15px;'>";
            echo "     <table style=\"width: 100%\">";
            echo '        <tr>';
            echo "            <td style=\"width: 80%\">";
            echo                $subnotatecnica." ".$subtitulo;
            echo "            </td>";

            $perDialMun = true;
            if($_SESSION['par']['itrid']==2 && empty($dadosMun['pnevalor2']) && $_REQUEST['metid'] != 11){
                $perDialMun = false;
            }

            if(Par3_Model_UsuarioResponsabilidade::perfilConsulta() ){
                if($dadosMun['pnesemvalor'] == 't'){
                    echo "            <td style=\"width: 19%; text-align: right;\">";
                    echo "Quantificado: ";
                    echo "              </td>";
                    echo "              <td>NÃO</td>";
                }else{
                    echo "            <td style=\"width: 19%; text-align: right;\">";
                    echo "Quantificado: ";
                    echo "              </td>";
                    echo "              <td>SIM</td>";
                }

            }

            if($perDialMun && !Par3_Model_UsuarioResponsabilidade::perfilConsulta() ) {
                echo "            <td style=\"width: 19%; text-align: right;\">";
                echo "Não Quantificado";
                echo "              </td>";
                echo "              <td>";

                $chk = '';
                if ($dadosMun['pnesemvalor'] == 't') $chk = 'checked';
                $html = <<<HTML
                    <input
                        id="checkbox_{$s['subiddial']}"
                         pneid="{$dadosMun['pneiddial']}"
                         pneanoprevisto="{$dadosMun['pneanoprevisto']}"
                        subid="{$dadosMun['subiddial']}"
                        pneano="{$pneano}"
                        muncod="{$_SESSION['par']['muncod']}"
                        pnetipo="{$dadosMun['pnetipo']}"
                        tipometa="{$dadosMun['subtipometabrasil']}"
                        tipoacao="{$tpacao}"
                        class="icheck-naoqtf"
                        type="checkbox" {$chk}>
HTML;
                echo $html;

                echo "              </td>";
            }
            echo '        </tr>';
            echo "     </table>";
            echo "    </div></td>";
            echo "   </tr>";
        }else{
            $dados = retornaDadosPne($valor, $_REQUEST['metid'], $s['subiddial'], '', $_SESSION['par']['itrid'], '');
            $existeResultado = (bool) $dados;
        }
        if($existeResultado){
            if (!verificaMetas()) {
                CarregaMetasDial($meta, $dadosMun, $anoprevisto, $qtdLegenda, $legenda, $dados, $arr, $s, $municipios, $mesoregioes, $count, $count2, $pneano);
            }else if ($_REQUEST['metid']==18){
                CarregaMetId18($dados, $s);
            }else{
                if($dados){
                    foreach ($dados as $dado){
                        CarregaMetasTextarea($dado, $s);
                    }
                }
            }
        }else{
            if (!verificaMetas()) {
                CarregaMetasDial($meta, $dadosMun, $anoprevisto, $qtdLegenda, $legenda, $dados, $arr, $s, $municipios, $mesoregioes, $count, $count2, $pneano);
            }else if ($_REQUEST['metid']==18){
                CarregaMetId18($dados, $s);
            }else{
                CarregaMetasTextarea($dado, $s);
            }
        }
        $contador += 1;
    endforeach;
    echo '</table>';
    echo $tabela;
    ?>

    <head>
        <script type="text/javascript">
            $(function(){
                $('.tabelaPne').hide();
            });
        </script>
    </head>
<?php
}

function criarAbasMetasPNE()
{
    global $db;
    $sql = "select metid, mettitulo from sase.meta where metstatus = 'A' order by metid";
    $arrMetas = $db->carregar($sql);

    $abas = array();

    foreach($arrMetas as $meta)
    {
        $arrMeta = array("descricao" =>  'Meta '.$meta['metid'], "meta" => $meta['metid'], "extenso" => $meta['mettitulo']);
        array_push($abas, $arrMeta);
    }

    return $abas;
}

function listarEstados()
{
    $sql = " Select	estuf AS codigo, estdescricao AS descricao From territorios.estado ";
    $regioes =   $_REQUEST['regioes'];

    if ($regioes)
    {
        $regioes =  str_replace("\\", "", trim($regioes, ',')) ;
        $sql .= "where regcod in(".$regioes.")";
    }

    $estados =   $_REQUEST['estados'];
    if ($estados)
    {
        $estados =  str_replace("\\", "", trim($estados, ',')) ;
        $sqlselecionados = "select estuf as codigo, estdescricao as descricao from territorios.estado where estuf in ($estados) ";

        if ($regioes)
        {
            $sqlselecionados .= " and regcod in ($regioes)";
        }
    }
    $grupoExcluido = array(7,15,19,20);

    if($_REQUEST['metid'] != 7
        && $_REQUEST['metid'] != 15
        && $_REQUEST['metid'] != 181
        && $_REQUEST['metid'] != 19
        && $_REQUEST['metid'] != 20)
    {
        mostrarComboPopupLocal( 'Estado', 'slEstado',  $sql,$sqlselecionados, 'Selecione os Estados', null,'atualizarRelacionadosRegiao(2)',false, $_SESSION['par']['itrid']);
    }
}

function listarMesoregioes()
{
    $sql = " Select	mescod AS codigo, mesdsc AS descricao From territorios.mesoregiao ";
    $regioes =   $_REQUEST['regioes'];

    if ($regioes)
    {
        $regioes =  str_replace("\\", "", trim($regioes, ',')) ;

        $sql .= "where estuf in (select estuf from territorios.estado where regcod in ($regioes))";
    }

    $estados =   $_REQUEST['estados'];

    if ($estados)
    {
        $estados =  str_replace("\\", "", trim($estados, ',')) ;
        if ($regioes)
            $sql .= "and estuf in(".$estados.")";
        else
            $sql .= "where estuf in(".$estados.")";
    }
    $grupoExcluido = array(7,15,18,19,20);
    if($_REQUEST['metid'] != 14
        && $_REQUEST['metid'] != 17
        && $_REQUEST['metid'] != 13
        && $_REQUEST['metid'] != 7
        && $_REQUEST['metid'] != 15
        && $_REQUEST['metid'] != 18
        && $_REQUEST['metid'] != 19
        && $_REQUEST['metid'] != 20){
        mostrarComboPopupLocal( 'Mesorregião', 'slMesoregiao',  $sql,$sqlselecionados, 'Selecione os Estados', null,'atualizarRelacionadosRegiao(3)',false);
    }
}

function listarMunicipios()
{
    $sql = " Select	muncod AS codigo, REPLACE(mundescricao,'\'','') AS descricao From territorios.municipio ";


    $regioes =   $_REQUEST['regioes'];

    if ($regioes)
    {
        $regioes =  str_replace("\\", "", trim($regioes, ',')) ;

        $sql .= "where estuf in (select estuf from territorios.estado where regcod in ($regioes))";
    }


    $estados =   $_REQUEST['estados'];

    if ($estados)
    {
        $estados =  str_replace("\\", "", trim($estados, ',')) ;

        if ($regioes)
            $sql .= "and estuf in(".$estados.")";
        else
            $sql .= "where estuf in(".$estados.")";
    }


    $municipios =   $_REQUEST['municipios'];

    if ($municipios)
    {
        $municipios =  str_replace("\\", "", trim($municipios, ',')) ;
        $sqlselecionados = "select muncod as codigo, REPLACE(mundescricao,'\'','')  as descricao from territorios.municipio where muncod in ($municipios) ";

        if ($estados)
        {
            $sqlselecionados .= " and estuf in ($estados)";
        }
    }

    if($_REQUEST['metid'] != 14
        && $_REQUEST['metid'] != 17
        && $_REQUEST['metid'] != 13
        && $_REQUEST['metid'] != 7
        && $_REQUEST['metid'] != 15
        && $_REQUEST['metid'] != 18
        && $_REQUEST['metid'] != 19
        && $_REQUEST['metid'] != 20)
    {
        mostrarComboPopupLocal( 'Município', 'slMunicipio',  $sql, $sqlselecionados, 'Selecione os Municipios', null, 'atualizarRelacionadosRegiao(3)' ,false);
    }
}

function montarAbasArrayLocal($itensMenu, $url = false, $boOpenWin = false)
{
    $url = $url ? $url : $_SERVER['REQUEST_URI'];
    $inuid = $_REQUEST['inuid'];

    if (is_array($itensMenu))
    {
        $rs = $itensMenu;
    } else
    {
        global $db;
        $rs = $db->carregar($itensMenu);
    }

    $titulo = "";
    if($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3){
        $titulo = $_SESSION['par']['estdescricao'];
    } else {
        $titulo = $_SESSION['par']['mundescricao'];
    }

    $menu = '<div class="ibox ibox float-e-margins" style="margin-bottom: -40px;">'
    			.'<div class="ibox-title titulo_box titulo-grfpne" style="border-width: 0px; min-height: 20px;">'
                            .'<h5>Metas PNE - ' . $titulo . '</h5>'
                            .'<div class="ibox-tools">'
	            		.'<a target="_blank" href="par3.php?modulo=principal/planoTrabalho/impressaoPNE&acao=A&inuid='.$inuid.'" style="color: #fff" title="Imprimir PNE" class="btn btn-success btn-Imprimir-azul">Imprimir <i class="fa fa-print"></i></a>&nbsp;'
	            		.'<button type="button" data-toggle="modal" data-target="#modalInfoPreenchimento" title="Informações de Preenchimento" class="btn btn-success info-preenchimento">Ajuda <i class="fa fa-question"></i></button>&nbsp;'
	            		.'<button title="Lista de Indicadores Pendentes" name="btnValidar" class="btn btn-success" onclick="validaIndicadores('.$_SESSION['par']['itrid'].')"><i class="fa fa-th-list"></i></button>'
                            .'</div>'
    			.'</div>';

    $menu .= '<table id="apres" width="100%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint table">'
        . '		<tr>';
    $menu .= '		<td class="text-center">';
    $menu .= '			<div class="btn-group">';

    $nlinhas = count($rs) - 1;

    for ($j = 0; $j <= $nlinhas; $j++) {
        extract($rs[$j]);

        if ($url != $meta && $j == 0)
            $gifaba = 'aba_nosel_ini2.gif';
        elseif ($url == $meta && $j == 0)
            $gifaba = 'aba_esq_sel_ini2.gif';
        elseif ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
            $gifaba = 'aba_dir_sel.gif';
        elseif ($url != $meta)
            $gifaba = 'aba_nosel.gif';
        elseif ($url == $meta)
            $gifaba = 'aba_esq_sel.gif';

        if ($url == $meta) {
            $giffundo_aba = 'aba_fundo_sel.gif';
            $cor_fonteaba = '#000055';
        } else {
            $giffundo_aba = 'aba_fundo_nosel2.gif';
            $cor_fonteaba = '#4488cc';
        }

        if ($meta != $url)
        {
            $class = '';
            if($meta == 1){
                $class = 'btn-primary-active';
            }
            $menu .= '<a style="width: 70px; padding: 6px 0px;" href="#meta' . $meta . '" onclick="selecionaAba('.$meta.'); $(\'#metid\').val('.$meta.');
					listarSubmetas(pegaSelecionados(\'slRegiao[]\'),pegaSelecionados(\'slEstado[]\'),pegaSelecionados(\'slMesoregiao[]\'),
                                        '.$_SESSION['par']['itrid'].')" class="btn btn-primary btn-menu-meta '.$class.'">' . $descricao . '</a>';
        }
        else
        {
            $menu .= $descricao;
        }
    }

    if ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
        $gifaba = 'aba_dir_sel_fim.gif';
    else
        $gifaba = 'aba_nosel_fim.gif';

    $menu .= '			</div>
					</td>
				</tr>
				</table>';

    return $menu;
}

function mostrarComboPopupLocal( $stDescricao, $stNomeCampo, $sql_combo, $sql_carregados, $stTextoSelecao, Array $where=null, $funcaoJS=null, $semTR=false, $intervalo=false , $arrVisivel = null , $arrOrdem = null, $obrig = false, $campoContem = true ){
    global $db, $$stNomeCampo;
    if ( $_REQUEST[$stNomeCampo] && $_REQUEST[$stNomeCampo][0] && !empty( $sql_carregados ) ) {
        if(!is_array($_REQUEST[$stNomeCampo])){
            $_REQUEST[$stNomeCampo][0] = $_REQUEST[$stNomeCampo];
        }
        $sql_carregados = sprintf( $sql_carregados, "'" . implode( "','", $_REQUEST[$stNomeCampo] ) . "'" );
        $$stNomeCampo = $db->carregar( sprintf( $sql_combo, $sql_carregados ) );
        var_dump($stNomeCampo);
    }
    if( !empty($sql_carregados) ){
        $$stNomeCampo = $db->carregar($sql_carregados);
    }

    if(!$semTR)
    {
        echo '<tr id="tr_'.$stNomeCampo.'">';
    }

    echo '<td width="25%" class="fundo_td" valign="top" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );">
			' . $stDescricao . '
			<input type="hidden" id="' . $stNomeCampo . '_campo_flag" name="' . $stNomeCampo . '_campo_flag" value="' . ( empty( $$stNomeCampo ) ? '0' : '1' ) . '"/>
		</td>
		<td class="fundo_td">';

    echo '<div id="' . $stNomeCampo . '_campo_off" style="color:#a0a0a0;';
    echo !empty( $$stNomeCampo ) ? 'display:none;' : '';
    echo '" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );"><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>';
    echo '<div id="' . $stNomeCampo . '_campo_on" ';
    echo empty( $$stNomeCampo ) ? 'style="display:none;"' : '';
    echo '>';
    combo_popupLocal( $stNomeCampo, sprintf( $sql_combo, '' ), $stTextoSelecao, '400x400', 0, array(), '', 'S', false, false, 10, 400, null, null, '', $where, null, true, false, $funcaoJS, $intervalo , $arrVisivel, $arrOrdem);

    if( $obrig )
    {
        echo '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
    }

    echo '</div>
			</td>';

    if(!$semTR)	echo '</tr>';
}

function combo_popupLocal( $nome, $sql, $titulo, $tamanho_janela = '400x400', $maximo_itens = 0,
                           $codigos_fixos = array(), $mensagem_fixo = '', $habilitado = 'S', $campo_busca_codigo = false,
                           $campo_flag_contem = false, $size = 10, $width = 200 , $onpop = null, $onpush = null, $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null)
{

    global ${$nome};
    unset($dados_sessao);
    // prepara parametros
    $maximo_itens = abs( (integer) $maximo_itens );
    $codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
    // prepara sessão
    $dados_sessao = array(
        'sql' => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
        'titulo' => $titulo,
        'indice' => $indice_visivel,
        'maximo' => $maximo_itens,
        'codigos_fixos' => $codigos_fixos,
        'mensagem_fixo' => $mensagem_fixo,
        'param_conexao' => $param_conexao,
        'where'			=> $where,
        'mostraPesquisa'=> $mostraPesquisa,
        'intervalo'     => $intervalo,
        'arrVisivel'    => $arrVisivel,
        'arrOrdem'     => $arrOrdem
    );

    if ( !isset( $_SESSION['indice_sessao_combo_popup'] ) )
    {
        $_SESSION['indice_sessao_combo_popup'] = array();
    }
    unset($_SESSION['indice_sessao_combo_popup'][$nome]);
    $_SESSION['indice_sessao_combo_popup'][$nome] = $dados_sessao;

    // monta html para formulario
    $tamanho    = explode( 'x', $tamanho_janela );
    $onclick    = ' onclick="javascript:combo_popup_alterar_campo_busca( this );" ';

    /*** Adiciona a função Javascript ***/
    $funcaoJS = (is_null($funcaoJS)) ? 'false' : "'" . $funcaoJS . "'";

    $ondblclick = ' ondblclick="javascript:combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' );" ';
    $ondelete   = ' onkeydown="javascript:combo_popup_remove_selecionados( event, \'' . $nome . '\' );" ';
    $onpop		= ( $onpop == null ) ? $onpop = '' : ' onpop="' . $onpop . '"';
    $onpush		= ( $onpush == null ) ? $onpush = '' : ' onpush="' . $onpush . '"';
    $habilitado_select = $habilitado == 'S' ? '' : ' disabled="disabled" ' ;
    $select =
        '<select ' .
        'maximo="'. $maximo_itens .'" tipo="combo_popup" ' .
        'multiple="multiple" size="' . $size . '" ' .
        'name="' . $nome . '[]" id="' . $nome . '" '.
        $onclick . $ondblclick . $ondelete . $onpop . $onpush  .
        'class="CampoEstilo" style="width:250px;" ' .
        $habilitado_select .
        '>';

    if($value && count( $value ) > 0)
    {
        $itens_criados = 0;
        foreach ( $value as $item )
        {
            $select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
            $itens_criados++;
            if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
            {
                break;
            }
        }
    } elseif ( ${$nome} && count( ${$nome} ) > 0 )
    {
        $itens_criados = 0;
        if( is_array(${$nome}) ){
            foreach ( ${$nome} as $item )
            {
                $select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
                $itens_criados++;
                if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
                {
                    break;
                }
            }
        }
    }
    else if ( $habilitado == 'S' )
    {
        $select .= '<option value="">Duplo clique para selecionar da lista</option>';
    }
    else
    {
        $select .= '<option value="">Nenhum</option>';
    }
    $select .= '</select>';
    $buscaCodigo = '';

    #Alteração feita por wesley romualdo
    #caso a consulta não seja por descrição e sim por codigo, não permitir digitar string no campo de consulta.
    if($campo_busca_descricao == true )
    {
        $paramentro = "";
        $complOnblur = "";
    }else
    {
        $paramentro = "onkeyup=\"this.value=mascaraglobal('[#]',this.value);\"";
        $complOnblur = "this.value=mascaraglobal('[#]',this.value);";
    }

    if ( $campo_busca_codigo == true && $habilitado == 'S' )
    {
        $buscaCodigo .= '<input type="text" id="combopopup_campo_busca_' . $nome . '" onkeypress="combo_popup_keypress_buscar_codigo( event, \'' . $nome . '\', this.value );" '.$paramentro.' onmouseover="MouseOver( this );" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this); '.$complOnblur.'" class="normal" style="margin: 2px 0;" />';
        $buscaCodigo .= '&nbsp;<img title="adicionar" align="absmiddle" src="/imagens/check_p.gif" onclick="combo_popup_buscar_codigo( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value );"/>';
        $buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="combo_popup_remover_item( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value, true );"/>';
        $buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>';
        $buscaCodigo .= '<br/>';
    }
    #Fim da alteração realizada por wesley romualdo

    $flagContem = '';
    if ( $campo_flag_contem == true )
    {
        $nomeFlagContemGlobal = $nome . '_campo_excludente';
        global ${$nomeFlagContemGlobal};
        $flagContem .= '<input type="checkbox" id="' . $nome . '_campo_excludente" name="' . $nome . '_campo_excludente" value="1" ' . ( ${$nomeFlagContemGlobal} ? 'checked="checked"' : '' ) . ' style="margin:0;" />';
        $flagContem .= '&nbsp;<label for="' . $nome . '_campo_excludente">Não contém</label>';
    }
    $cabecalho = '';
    if ( $buscaCodigo != '' || $flagContem != '' )
    {
        $cabecalho .= '<table width="' . $width . '" border="0" cellspacing="0" cellpadding="0"><tr>';
        $cabecalho .= '<td align="left">' . $buscaCodigo . '</td>';
        $cabecalho .= '<td align="right">' . $flagContem . '</td>';
        $cabecalho .= '</tr></table>';
    }
    print $cabecalho . $select;
}

function naoInformado(){
    global $db;
    $pneiddial       = '';
    $subiddial       = $_REQUEST['subiddial'];
    $metid       = $_REQUEST['metid'];
    $pneano      = $_REQUEST['pneano'];
    $valor       = $_REQUEST['valor'];
    $anoprevisto = $_REQUEST['anoprevisto'];

    if ($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3) {
        $sql = "select pneiddial from sase.pnedial where subiddial = {$subiddial} and estuf = '{$_SESSION['par']['estuf']}' and pneano = {$anoprevisto}";
    } else {
        $sql = "select pneiddial from sase.pnedial where subiddial = {$subiddial} and muncod = '{$_SESSION['par']['muncod']}' and pneano = {$anoprevisto}";
    }
    $pneiddial = $db->pegaUm($sql);

    if($pneiddial != ''){
        if($db->commit()){
            if ($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3){
                if ($_SESSION['par']['estuf']) {
                    if (in_array($metid, array(19,20))){
                        $sql = "update sase.metainfcomplementar set micinfcomplementar = null where estuf = '" . $_SESSION['par']['estuf'] . "' and metid = {$metid}";
                        $db->executar($sql);
                    }
                    $sql = "update sase.pnedial set pnevalormeta = null, pnetipometa = null, pnesemvalor = 't' where estuf = '" . $_SESSION['par']['estuf'] . "' and subiddial = {$subiddial}";
                    $db->executar($sql);

                    if ($db->commit()) {
                        $sql = "update sase.pnedial set pnevalormeta = 0, pnesemvalor = 'f' where pneiddial = {$pneiddial}";
                        $db->executar($sql);

                        if($db->commit()) {
                            $dadosMun = retornaDadosEst($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['estuf'], $pneano, $_SESSION['par']['itrid']);
                            if ($dadosMun && is_array($dadosMun)) {
                                $cor = '#236B8E';
                                $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                                $mundescricao = $dadosMun['estdescricao'];
                                $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                                $metaBrasil = $dadosMun['subvalormetabrasil'];
                                $dadosGrafico = array(
                                    'cor' => $cor,
                                    'descricao' => str_replace("'", '', $descricao),
                                    'valor' => $dadosMun['pnevalor2'],
                                    'metaTotal' => $metaTotal,
                                    'metaBrasil' => $metaBrasil,
                                    'tipo' => $dadosMun['subtipometabrasil'],
                                    'plotBandsCor' => '#f7b850',
                                    'plotBandsOuterRadius' => '115%',
                                    'title' => "Meta {$mundescricao}:",
                                    'anoprevisto' => $anoprevisto
                                );
                                echo geraGraficoPNE('grafico_municipio_' . $subiddial, $dadosGrafico);
                            }
                        }
                    }
                }
            } else {
                if ($_SESSION['par']['muncod']) {
                    if (in_array($metid, array(13,14,17,19,20))){
                        $sql = "update sase.metainfcomplementar set micinfcomplementar = null where muncod = '" . $_SESSION['par']['muncod'] . "' and metid = {$metid}";
                        $db->executar($sql);
                    }
                    $sql = "update sase.pnedial set pnevalormeta = null, pnetipometa = null, pnesemvalor = 't' where muncod = '" . $_SESSION['par']['muncod'] . "' and subiddial = {$subiddial}";
                    $db->executar($sql);

                    if ($db->commit()) {
                        $sql = "update sase.pnedial set pnevalormeta = 0, pnesemvalor = 'f' where pneiddial = {$pneiddial}";
                        $db->executar($sql);

                        if($db->commit()) {
                            $dadosMun = retornaDadosEst($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['muncod'], $pneano, $_SESSION['par']['itrid']);
                            if ($dadosMun && is_array($dadosMun)) {
                                $cor = '#236B8E';
                                $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
                                $mundescricao = $dadosMun['mundescricao'];
                                $metaTotal = 'P' == $dadosMun['subtipometabrasil'] ? 100 : $dadosMun['subvalormetabrasil'];
                                $metaBrasil = $dadosMun['subvalormetabrasil'];
                                $dadosGrafico = array(
                                    'cor' => '#f7b850',
                                    'descricao' => str_replace("'", '', $descricao),
                                    'valor' => $dadosMun['pnevalor2'],
                                    'metaTotal' => $metaTotal,
                                    'metaBrasil' => $metaBrasil,
                                    'tipo' => $dadosMun['subtipometabrasil'],
                                    'plotBandsCor' => $cor,
                                    'plotBandsOuterRadius' => '115%',
                                    'title' => "Meta {$mundescricao}:",
                                    'anoprevisto' => $anoprevisto
                                );
                                echo geraGraficoPNE('grafico_municipio_' . $subiddial, $dadosGrafico);
                            }
                        }
                    }
                }
            }
        }
    }else{

        if ($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3){
            if ($_SESSION['par']['estuf']) {
                if (in_array($metid, array(19,20))){

                    $sql = "update sase.metainfcomplementar set micinfcomplementar = null where estuf = '" . $_SESSION['par']['estuf'] . "' and metid = {$metid}";
                    $db->executar($sql);
                    $db->commit();
                }
            }
        } else {
            if ($_SESSION['par']['muncod']) {
                if (in_array($metid, array(13,14,17,19,20))){
                    $sql = "update sase.metainfcomplementar set micinfcomplementar = null where muncod = '" . $_SESSION['par']['muncod'] . "' and metid = {$metid}";
                    $db->executar($sql);
                    $db->commit();
                }
            }
        }
    }

}

/**
 *
 * @global type $db
 * @param type $valor
 * @param type $subiddial
 * @param type $estuf
 * @param type $pneano
 * @return type
 */
function retornaDadosEst($valor, $subiddial, $metid, $estuf, $pneano, $itrid){
    global $db;
    if (
        (in_array($metid, array(13,14,17,19,20))&&($itrid == 2)) ||
        (in_array($metid, array(19,20))&&($itrid == 1 || $itrid == 3)))
    {
        $where = "sub.metid = {$metid}";
    }else{
        $where = "sub.subiddial = {$subiddial}  AND pneano = $pneano ";
    }

    $sql = "
        SELECT
            pne.pneiddial,
            m.estuf || ' - ' || mundescricao as descricao,
            mundescricao,
            5 as ordem,
            ROUND(pnevalor, $valor) as pnevalor2,
            pneano,
            pnetipo,
            pne.subiddial,
            sub.metid,
            sub.subtitulo,
            subtipometabrasil,
            ppd.pnevalormeta,
            ppd.pnesemvalor,
            CASE
                WHEN ppd.pnevalormeta IS NOT NULL THEN ppd.pnevalormeta
                WHEN sub.subvalormetabrasil IS NOT NULL THEN sub.subvalormetabrasil
                ELSE 0
            END as subvalormetabrasil,
            pneanoprevisto,
            ppd.pnecpfinclusao,
            ppd.pnedatainclusao,
            ppd.pnesemvalor
        FROM sase.pnedial  pne
        LEFT JOIN sase.submetadial         sub ON sub.subiddial = pne.subiddial AND pne.estuf = '$estuf' AND pnetipo = 'E'
        LEFT JOIN sase.pneprevdial 		   ppd ON ppd.subid = sub.subiddial AND ppd.pneestuf = '$estuf' AND ppd.pnedatainclusao = (SELECT MAX(pnedatainclusao) FROM sase.pneprevdial WHERE subid = sub.subiddial AND pneestuf = '$estuf')
        LEFT JOIN territorios.estado       e   ON e.estuf = pne.estuf
        LEFT JOIN territorios.municipio    m   ON m.muncod = pne.muncod
        LEFT JOIN territorios.regiao       r   ON r.regcod = pne.regcod
        LEFT JOIN territorios.mesoregiao   mr  ON mr.mescod = pne.mescod
        WHERE $where
        ORDER BY ppd.pnedatainclusao DESC
        LIMIT 1";
//    ver($sql);
    return $db->pegaLinha($sql);
}

/**
 *
 * @global type $db
 * @param type $valor
 * @param type $subiddial
 * @param type $muncod
 * @param type $pneano
 * @return type
 */
function retornaDadosMun($valor, $subiddial, $metid, $muncod, $pneano, $itrid){
    global $db;
    if (
        (in_array($metid, array(13,14,17,19,20))&&($itrid == 2)) ||
        (in_array($metid, array(19,20))&&($itrid == 1 || $itrid == 3))
    ) {
        $where = "sub.metid = {$metid}";
    }else{
        $where = "sub.subiddial = {$subiddial} AND pneano = $pneano";
    }

    $sql = "SELECT
                pne.pneiddial,
                m.estuf || ' - ' || mundescricao as descricao,
                mundescricao,
                5 as ordem,
                ROUND(coalesce(pnevalor,0), $valor) as pnevalor2,
                pneano,
                pnetipo,
                pne.subiddial,
                sub.metid,
                sub.subtitulo,
                subtipometabrasil,
                ppd.pnevalormeta,
                sub.subvalormetabrasil,
                ppd.pnesemvalor,
                pneanoprevisto,
                ppd.pnecpfinclusao,
                ppd.pnedatainclusao
            FROM sase.pnedial  pne
            LEFT JOIN sase.submetadial         sub ON sub.subiddial = pne.subiddial AND pne.muncod = '" . $muncod . "' AND pnetipo = 'M'
            LEFT JOIN sase.pneprevdial 		   ppd ON ppd.subid = sub.subiddial AND ppd.pnemuncod = '$muncod' AND ppd.pnedatainclusao = (SELECT MAX(pnedatainclusao) FROM sase.pneprevdial WHERE subid = sub.subiddial AND pnemuncod = '$muncod')
            LEFT JOIN territorios.estado       e   ON e.estuf = pne.estuf
            LEFT JOIN territorios.municipio    m   ON m.muncod = pne.muncod
            LEFT JOIN territorios.regiao       r   ON r.regcod = pne.regcod
            LEFT JOIN territorios.mesoregiao   mr  ON mr.mescod = pne.mescod
            WHERE $where
            ORDER BY ppd.pnedatainclusao desc
            LIMIT 1";

//    $sql .= "  AND ((pne.muncod = '" . $muncod . "' and pnetipo = 'M'))
//            ORDER BY ordem, sub.subordem, pne.subiddial, pne.pneano, pnetipo, descricao";
    return $db->pegaLinha($sql);
}

function validaIndicadores(){
    global $db;
    $where = '';
    if($_REQUEST['tipo'] == 'est'){
        $where = "where pne.estuf = '{$_SESSION['par']['estuf']}'";
    } else {
        $where = "where pne.muncod = '{$_SESSION['par']['muncod']}'";
    }

    if($where != '') {
        $sql = "select
                    sub.subtitulo
                from sase.pnedial pne
                inner join sase.submetadial sub on sub.subiddial = pne.subiddial and sub.substatus = 'A'
                inner join sase.meta met on met.metid = sub.metid
                {$where}
                and case when
                        met.metid = 1 or
                        met.metid = 2 or
                        met.metid = 3 or
                        met.metid = 5 or
                        met.metid = 8 or
                        met.metid = 9
                    then case
                        when pne.muncod is not null then pne.pneano = 2010
                        when pne.estuf is not null then pne.pneano = 2012
                        else pne.pneano = 2010
                         end
                    when
                        met.metid = 4
                    then pne.pneano = 2010
                    when
                        met.metid = 12 or
                        met.metid = 13 or
                        met.metid = 14 or
                        met.metid = 17
                    then pne.pneano = 2012
                    else pne.pneano = 2013
                    end
                and pne.pnesemvalor = 't'
                and (pne.pnevalormeta is null or pne.pnevalormeta = 0)
                order by met.metid";
        $dados = $db->carregar($sql);
        if(is_array($dados)) {
            $msg = "Os indicadores com inconsistência são:\n";
            foreach ($dados as $d) {
                $msg .= "    " . $d['subtitulo'] . "\n";
            }
            echo $msg;
        } else {
            echo "Não existem indicadores com inconsistência.";
        }
    } else {
        echo "Dados invalidos.";
    }
}

function alteraGrafico(){
    global $db;

    if(
    	!$_REQUEST['subiddial'] ||
    	!$_REQUEST['pneid']
    ){
    	return false;
    }

    $pneano       = $_REQUEST['pneano'];
    $subiddial    = $_REQUEST['subiddial'];
    $valor        = $_REQUEST['valor'];
    $muncod       = $_SESSION['par']['muncod'];
    $pneano       = $_POST['pneano'];
    $anoprevisto  = $_POST['anoprevisto'];
    $alteraPnePrev= $_POST['atualizaPnePrev'];
    $pnevalormeta = str_replace(',', '.', $_POST['pnevalormeta']);
    $pneanoBrasil = selecionaAno($subiddial, 'B');
    $estuf        = $_SESSION['par']['estuf'];

    if ($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3) {
        if ($_SESSION['par']['estuf']) {
            $div = 'grafico_estuf'.$subiddial;
            atualizaDadosEstUf($div, $estuf, $subiddial, $pneano, $pnevalormeta, $anoprevisto, $pneanoBrasil, $valor, $alteraPnePrev);
        }
    } else {
        if ($_SESSION['par']['muncod']) {
            $div = 'grafico_municipio_'.$subiddial;
            atualizaDadosMun($div, $muncod, $subiddial, $pneano, $pnevalormeta, $anoprevisto, $pneanoBrasil, $valor, $alteraPnePrev);
        }
    }
}

/**
 *
 * @param type $metid
 */
function selecionaTitulo($metid){
    switch( $metid ){
        case 1:
            $titulo = "Meta 1  Educação Infantil";
            break;
        case 2:
            $titulo = "Meta 2  Ensino Fundamental";
            break;
        case 3:
            $titulo = "Meta 3  Ensino Médio";
            break;
        case 4:
            $titulo = "Meta 4  Inclusão";
            break;
        case 5:
            $titulo = "Meta 5  Alfabetização Infantil";
            break;
        case 6:
            $titulo = "Meta 6  Educação Integral";
            break;
        case 7:
            $titulo = "Meta 7  Qualidade da Educação Básica/IDEB";
            break;
        case 8:
            $titulo = "Meta 8  Elevação da escolaridade/Diversidade";
            break;
        case 9:
            $titulo = "Meta 9  Alfabetização de jovens e adultos";
            break;
        case 10:
            $titulo = "Meta 10  EJA Integrada";
            break;
        case 11:
            $titulo = "Meta 11  Educação Profissional";
            break;
        case 12:
            $titulo = "Meta 12  Educação Superior";
            break;
        case 13:
            $titulo = "Meta 13  Qualidade da Educação Superior";
            break;
        case 14:
            $titulo = "Meta 14  Pós-Graduação";
            break;
        case 15:
            $titulo = "Meta 15  Profissionais de Educação";
            break;
        case 16:
            $titulo = "Meta 16  Formação";
            break;
        case 17:
            $titulo = "Meta 17  Valorização dos Profissionais do Magistério";
            break;
        case 18:
            $titulo = "Meta 18  Planos de Carreira";
            break;
        case 19:
            $titulo = "Meta 19  Gestão Democrática";
            break;
        case 20:
            $titulo = "Meta 20  Financiamento da Educação";
            break;
    }
}

function selecionaAno($subiddial, $paetipo){
    global $db;
    $sql = "select paeano
              from sase.pneanoexibicaodial
             where subiddial in ({$subiddial})
               and paetipo = '{$paetipo}'
               and paestatus = 'A'";
    $rs = $db->pegaUm($sql);

    if ($rs==''){
        $rs='2015';
    }
    return $rs;
}

/**
 *
 * @param type $valor
 * @param type $subiddial
 * @param type $pneanoBrasil
 * @param type $where
 * @return type
 */
function retornaDadosPne($valor, $metid, $subiddial, $pneanoBrasil, $itrid, $where){
    if ($_SESSION['par']['itrid']==1 || $_SESSION['par']['itrid']==3){
        $pnetipo = 'E';
        $where = " and e.estuf = '".$_SESSION['par']['estuf']."'";
    }else{
        $pnetipo = 'M';
        $where = " and m.muncod = '".$_SESSION['par']['muncod']."'";
    }
    //$where='';
    global $db;
    if ((in_array($metid, array(13,14,17,19,20))&&($itrid == 2))||
        (in_array($metid, array(19,20))&&($itrid == 1 || $itrid == 3))) {
        $sql ="
            SELECT meta.metid,
                   pne.micinfcomplementar
            FROM sase.metainfcomplementar  pne
            inner join sase.meta meta on pne.metid = meta.metid
            left join territorios.estado e on e.estuf = pne.estuf
            left join territorios.municipio m on m.muncod = pne.muncod
            WHERE pne.metid = {$metid} ".$where;
    }else{
        $sql ="
            SELECT
                CASE
                    WHEN pnetipo = 'M' THEN m.estuf || ' - ' || mundescricao
                    WHEN pnetipo = 'R' THEN regdescricao
                    WHEN pnetipo = 'S' THEN mesdsc
                    ELSE coalesce(e.estdescricao, '1Brasil')
                END as descricao,
                CASE
                    WHEN pnetipo = 'M' THEN 5
                    WHEN pnetipo = 'R' THEN 2
                    WHEN pnetipo = 'S' THEN 4
                    WHEN pnetipo = 'E' and coalesce(e.estdescricao, '') != '' THEN 3
                    ELSE 1
                END as ordem,
                        ROUND(pnevalor, $valor) as pnevalor2,
                        pneano,
                        pnetipo,
                        pne.pneiddial,
                        pne.subiddial,
                        sub.metid,
                        sub.subtitulo,
                        subtipometabrasil,
                        subvalormetabrasil,
                        ppd.pnepossuiplanoremvigente,
                        ppd.pneplanorefcaput,
                        ppd.pneanoprevisto,
                        ppd.pnedatainclusao
                    FROM sase.pnedial  pne
                    inner join sase.submetadial sub on sub.subiddial = pne.subiddial
                    left join sase.pneprevdial ppd on pne.pneiddial = ppd.pneiddial
                    left join territorios.estado e on e.estuf = pne.estuf
                    left join territorios.municipio m on m.muncod = pne.muncod
                    left join territorios.regiao r on r.regcod = pne.regcod
                    left join territorios.mesoregiao mr on mr.mescod = pne.mescod
                WHERE sub.subiddial = {$subiddial}";
                if ($metid != 18){
                    $sql .= " AND ((pne.estuf is null and pnetipo = '{$pnetipo}' and pneano = $pneanoBrasil)$where)";
                }else{
                    $sql .= "$where and pnedatainclusao is not null ";
                }
                $sql .= "ORDER BY ppd.pnedatainclusao desc LIMIT 1";
    }

    return $db->carregar($sql);
}

function carregarMetas($metid, $itrid){
    global $db;
    if ($_SESSION['par']['itrid']==1 || $_SESSION['par']['itrid']==3){
        $pnetipo = 'E';
        $where = " and mic.estuf = '".$_SESSION['par']['estuf']."'";
    }else{
        $pnetipo = 'M';
        $where = " and mic.muncod = '".$_SESSION['par']['muncod']."'";
    }
    if ((in_array($metid, array(13,14,17,19,20))&&($itrid == 2))||
        (in_array($metid, array(19,20))&&($itrid == 1 || $itrid == 3))) {
        $sql = "select meta.metid,
                       meta.mettitulo as subtitulo,
                       meta.metfontemunicipio,
                       meta.metfonteestado,
                       meta.mettitulo,
                       '' as subnotatecnica
                  from sase.meta meta
                  left join sase.metainfcomplementar mic
                    on meta.metid = mic.metid ".$where."
                 where meta.metid = ".$metid;
    }else if($metid==18){
        $sql = "select sub.subiddial,
                       sub.subtitulo,
                       sub.subfonteestado as metfonteestado,
                       sub.subfontemunicipio as metfontemunicipio,
                       meta.mettitulo,
                       sub.subnotatecnica
                  from sase.submetadial as sub
                  left join sase.meta meta
                    on (sub.metid = meta.metid)
                 where sub.substatus = 'A'
                   and sub.metid = ".$metid."
                 order by sub.subordem ASC";
    }else{
        $sql = "select sub.subiddial,
                       sub.subtitulo,
                       sub.subfonteestado as metfonteestado,
                       sub.subfontemunicipio as metfontemunicipio,
                       meta.mettitulo,
                       sub.subnotatecnica
                  from sase.submetadial as sub
                  left join sase.meta meta
                    on (sub.metid = meta.metid)
                 where sub.substatus = 'A'
                   and sub.metid = ".$metid."
                 order by sub.subordem ASC";
    }

    return $db->carregar($sql);
}

/**
 *
 * @global type $db
 * @param type $subiddial
 * @param type $pneano
 * @return type
 */
function retornaDadosBrasil($subiddial, $pneano){
    global $db;
    $sql = "select ROUND(pnevalor, 1) as pnevalor,
                   pnetipo,
                   pnevalormeta,
                   pnevalor,
                   subtipometabrasil,
                   subvalormetabrasil
            from sase.submetadial sd
            left join sase.pnedial pd
              on sd.subiddial = pd.subiddial
            left join sase.pneprevdial ppd
              on pd.pneiddial = ppd.pneiddial
           where sd.subiddial= {$subiddial}
             and pneano = {$pneano}
             and pnetipo = 'E'
             and coalesce(estuf, '') = ''";
     //ver($sql);
    return $db->carregar($sql);
}

/**
 *
 * @global type $db
 * @param type $subiddial
 * @param type $pneano
 * @return type
 */
function retornaDadosEstado($subiddial, $pneano, $estuf){
    global $db;
    $sql = "SELECT
                ROUND(pnevalor, 1) as pnevalor,
                pnetipo,
                pnevalormeta,
                subtipometabrasil,
                CASE
                  WHEN ppd.pnevalormeta IS NOT NULL THEN ppd.pnevalormeta
                  ELSE 0
                END as subvalormetabrasil,
                ppd.pnecpfinclusao,
                ppd.pnedatainclusao,
                ppd.pneanoprevisto
            FROM sase.submetadial sd
            LEFT JOIN sase.pnedial      pd ON sd.subiddial = pd.subiddial
            LEFT JOIN sase.pneprevdial  ppd ON pd.pneiddial = ppd.pneiddial
            WHERE sd.subiddial= {$subiddial}
                AND pneano = {$pneano}
                AND pnetipo = 'E'
                AND estuf = '{$estuf}'
 	            AND pnedatainclusao IS NOT NULL
            ORDER BY ppd.pnedatainclusao desc
            LIMIT 1";
//     ver($sql);
    return $db->carregar($sql);
}

/**
 *
 * @return boolean
 */
function verificaMetas($metid = null){
    if (is_null($metid)) {
        $metid = $_REQUEST['metid'];
    }
    if ((in_array($metid, array(13,14,17,18,19,20))&&($_SESSION['par']['itrid'] == 2))||
        (in_array($metid, array(18,19,20))&&($_SESSION['par']['itrid'] == 1 || $_SESSION['par']['itrid'] == 3))){
        return true;
    }else{
        return false;
    }
}

/**
 *
 * @param type $dados
 * @param type $s
 */
function CarregaMetId18($dados, $s){
    global $disabled;
    if ($_SESSION['par']['itrid']==1 || $_SESSION['par']['itrid']==3){
        $pnetipo = 'E';
    }else if ($_SESSION['par']['itrid']==2){
        $pnetipo = 'M';
    }
    $pneano = selecionaAno(25, $pnetipo);
    if ($dados[0]['pnepossuiplanoremvigente']=='t'){
        $checkpnepossuiplanoremvigentetrue='checked';
        $checkpnepossuiplanoremvigentefalse='';
        $checkpnepossuiplanoremvigenteH='true';
        $habilitaRadio = "";
        $habilitaAno = "";
        $disabledAno = 'disabled';
    }else if ($dados[0]['pnepossuiplanoremvigente']=='f'){
        $checkpnepossuiplanoremvigentetrue='';
        $checkpnepossuiplanoremvigentefalse='checked';
        $checkpnepossuiplanoremvigenteH='false';
        $habilitaRadio = "style='display:none;'";
        $habilitaAno = "";
        $disabledAno = '';
    }else{
        $checkpnepossuiplanoremvigentetrue='';
        $checkpnepossuiplanoremvigentefalse='';
        $checkpnepossuiplanoremvigenteH='null';
        $habilitaRadio = "style='display:none;'";
        $habilitaAno = "style='display:none;'";
        $disabledAno = 'disabled';
    }
    if ($dados[0]['pneplanorefcaput']=='t'){
        $checkpneplanorefcaputtrue='checked';
        $checkpneplanorefcaputfalse='';
        $checkpneplanorefcaputH='true';
        $habilitaAno = "style='display:none;'";
        $disabledAno = 'disabled';
    }else if ($dados[0]['pneplanorefcaput']=='f'){
        $checkpneplanorefcaputtrue='';
        $checkpneplanorefcaputfalse='checked';
        $habilitaAno = "";
        $checkpneplanorefcaputH='false';
        $disabledAno = '';
    }else{
        $checkpneplanorefcaputtrue='';
        $checkpneplanorefcaputfalse='';
        $habilitaAno = "style='display:none;'";
        $checkpneplanorefcaputH='null';
        $disabledAno = 'disabled';
    }

    $comboAnos = "";
    $anos = array(2015, 2026);
    $a = $anos[0];
    while($a <= $anos[1]){
        $sel = $dados[0]['pneanoprevisto'] == $a ? 'selected' : '';
        $comboAnos .= "<option value=\"".$a."\" {$sel}>".$a."</option>";
        $a++;
    }


    $html = <<<HTML
        <script>
            $(document).ready(function(){
                $('.icheck').iCheck({
                    radioClass: 'iradio_square-green'
                });

                $('.icheck').on('ifChecked', function(e){
                    switch ($(this).attr('id')){
                        case 'pnepossuiplanoremvigenteS':
                            $('#pneplanorefcaputS').iCheck('enable');
                            $('#pneplanorefcaputN').iCheck('enable');
                            $('#pneanoprevisto').prop("disabled", true);
                            changeRadio(this, 'trplanovigente', 'tranoprevisto', 'pneplanorefcaput')
                            break;
                        case 'pnepossuiplanoremvigenteN':
                            $('#pneplanorefcaputS').iCheck('disable');
                            $('#pneplanorefcaputN').iCheck('disable');
                            $('#pneanoprevisto').prop("disabled", false);
                            changeRadio(this, 'tranoprevisto', '', 'pneplanorefcaput');
                            break;
                        case 'pneplanorefcaputS':
                            $('#pneanoprevisto').prop("disabled", true);
                            changeRadio(this, '','tranoprevisto', '');
                            break;
                        case 'pneplanorefcaputN':
                            $('#pneanoprevisto').prop("disabled", false);
                            changeRadio(this, 'tranoprevisto', '', '');
                            break;
                    }
                });
            });
        </script>
        <style>
            .control-label {
                text-align: right;
            }

            .form-group {
                margin-bottom: 30px;
            }
        </style>
        <tr>
            <td class="tabela_painel">

                <div class="ibox-content">
                    <div class="row">
                        <div class="col-md-12">

                            <div class="form-group">
                                <label for="" class="col-md-6 control-label">Possui um plano de cargos e rmuneração vigente?</label>
                                <div class="col-md-6">
                                    <input type='hidden' {$disabled} id='pnepossuiplanoremvigenteH' value='{$checkpnepossuiplanoremvigenteH}'>
                                    <label>Sim</label>
                                    <input type="radio" name="pnepossuiplanoremvigente" id="pnepossuiplanoremvigenteS" {$checkpnepossuiplanoremvigentetrue} value="true" class="icheck"/>
                                    <label style="margin-left: 10px;">Não</label>
                                    <input type="radio" name="pnepossuiplanoremvigente" id="pnepossuiplanoremvigenteN" {$checkpnepossuiplanoremvigentefalse} value="false" class="icheck"/>
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="" class="col-md-6 control-label">Plano de cargos e remuneração, em vigor, toma como referência o caput da meta 18</label>
                                <div class="col-md-6">
                                    <input type='hidden' {$disabled} id='pneplanorefcaputH' value='{$checkpneplanorefcaputH}'>
                                    <label>Sim</label>
                                    <input type="radio" name="pneplanorefcaput" id="pneplanorefcaputS" {$checkpneplanorefcaputtrue} value="true" class="icheck"/>
                                    <label style="margin-left: 10px;">Não</label>
                                    <input type="radio" name="pneplanorefcaput" id="pneplanorefcaputN" {$checkpneplanorefcaputfalse} value="false" class="icheck"/>
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="" class="col-md-6 control-label">Ano previsto</label>
                                <div class="col-md-6">
                                    <div style="width: 115px;">
                                        <select {$disabledAno} name="pneanoprevisto" id="pneanoprevisto" class="form-control">
                                        {$comboAnos}
                                        </select>
                                    </div>
                                </div>
                            </div>

                            <div class="form-group">
                                <div class="col-md-12">
                                    <input type="button" {$disabled} style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarMeta18({$s['subiddial']}, {$pneano})" value="Salvar Informações Complementares" class="btn btn-primary">
                                </div>
                            </div>

                        </div>
                    </div>
                </div>
            </td>
        </tr>
HTML;


//    echo $html;

    $tabela = "";
    $tabela .= "        <tr>";
    $tabela .= "            <td>";
    $tabela .= "                <table>";
    $tabela .= "                    <tr class='trplanovigente'>";
    $tabela .= "                        <td colspan='2'>";
    $tabela .= "                            Possui um plano de cargos e remuneração vigente?";
    $tabela .= "                        </td>";
    $tabela .= "                    </tr>";
    $tabela .= "                    <tr class='trplanovigente'>";
    $tabela .= "                        <td>";
    $tabela .= "                            <input type='hidden' {$disabled} id='pnepossuiplanoremvigenteH' value='{$checkpnepossuiplanoremvigenteH}'>";
    $tabela .= "                            <input type='radio' {$disabled} id='pnepossuiplanoremvigenteS' name='pnepossuiplanoremvigente' value='true' $checkpnepossuiplanoremvigentetrue onchange='javascript:changeRadio(this, \"trplanovigente\", \"tranoprevisto\", \"pneplanorefcaput\")'>Sim";
    $tabela .= "                        </td>";
    $tabela .= "                        <td>";
    $tabela .= "                            <input type='radio' {$disabled} id='pnepossuiplanoremvigenteN' name='pnepossuiplanoremvigente' value='false' $checkpnepossuiplanoremvigentefalse onchange='javascript:changeRadio(this, \"tranoprevisto\", \"\", \"pneplanorefcaput\");document.getElementById(\"trplanovigente2\").style.display = \"none\";document.getElementById(\"trplanovigente3\").style.display = \"none\";'>Não";
    $tabela .= "                        </td>";
    $tabela .= "                    </tr>";
    $tabela .= "                    <tr class='trplanovigente' id='trplanovigente2' {$habilitaRadio}>";
    $tabela .= "                        <td colspan='2'>";
    $tabela .= "                            Plano de cargos e remuneração, em vigor, toma como referência o caput da meta 18?";
    $tabela .= "                        </td>";
    $tabela .= "                    </tr>";
    $tabela .= "                    <tr class='trplanovigente' id='trplanovigente3' {$habilitaRadio}>";
    $tabela .= "                        <td>";
    $tabela .= "                            <input type='hidden' {$disabled} id='pneplanorefcaputH' value='{$checkpneplanorefcaputH}'>";
    $tabela .= "                            <input type='radio' {$disabled} id='pneplanorefcaputS' name='pneplanorefcaput' value=true $checkpneplanorefcaputtrue onchange='javascript:changeRadio(this, \"\",\"tranoprevisto\", \"\")'>Sim";
    $tabela .= "                        </td>";
    $tabela .= "                        <td>";
    $tabela .= "                            <input type='radio' {$disabled} id='pneplanorefcaputN' name='pneplanorefcaput' value=false $checkpneplanorefcaputfalse onchange='javascript:changeRadio(this, \"tranoprevisto\", \"\", \"\")'>Não";
    $tabela .= "                        </td>";
    $tabela .= "                    </tr>";
    $tabela .= "                    <tr class='tranoprevisto' {$habilitaAno}>";
    $tabela .= "                        <td colspan='2' id='tdtextoanoprevisto'>";
    $tabela .= "                            <div class=\"div_lbl_grfAno\">";
    $tabela .= "                                <label>Ano Previsto:</label>";
    $tabela .= "                            </div>";
    $tabela .= "                            <div class=\"div_combo_grfAno\">";
    $tabela .= "                                <select id=\"pneanoprevisto\" {$disabled} name=\"pneanoprevisto\">";
                                                    $anos = array(2015, 2026);
                                                    $a = $anos[0];
                                                    while($a <= $anos[1]){
                                                        $sel = $dados[0]['pneanoprevisto'] == $a ? 'selected' : '';
                                                        $tabela .= "<option value=\"".$a."\" {$sel}>".$a."</option>";
                                                        $a++;
                                                    }
    $tabela .= "                                </select>";
    $tabela .= "                            </div>";
    $tabela .= "                        </td>";
    $tabela .= "                    </tr>";
    $tabela .= "                </table>";
    $tabela .= '                <input type="button" '.$disabled.' style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarMeta18(' . $s['subiddial'] . ', '.$pneano.')" value="Salvar Informações Complementares" class="btn btn-primary">';
    $tabela .= "            </td>";
    $tabela .= "        </tr>";
    echo $tabela;
}

/**
 *
 * @param type $dado
 * @param type $s
 */
function CarregaMetasTextarea($dado, $s){
    global $disabled;
    $dadosAgrupados[str_replace('1Brasil', 'Brasil', tirar_acentos($dado['descricao'] . '::' . $dado['pnetipo']))] = $dado;
    $anos[$dado['pneano']] = $dado['pneano'];
    $pneano = $dado['pneano'];
    if ($_SESSION['par']['itrid']==1 || $_SESSION['par']['itrid']==3){
        $dadosMun = carregaSubmetas($s['metid'], $_SESSION['par']['estuf']);
    }else{
        $dadosMun = carregaSubmetas($s['metid'], $_SESSION['par']['muncod']);
    }
    $count=0;
    $count2=0;
    echo "    <tr>";
    echo "        <td class=\"tabela_painel\">";
    echo "            <table class=\"tabela_painel\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\">";
    if(is_array($dadosMun)) {
        foreach ($dadosMun as $sub) {
            $pneanoBrasil = selecionaAno($sub['subiddial'], 'B');
            echo "              <tr>";
            echo "                 <td style=\"font-size:16px;padding:5px 0 5px 5px;font-weight:bold;color: #333;\" colspan=4>";
            echo $sub['subtitulo'];
            echo "                 </td>";
            echo "              </tr>";
            echo "              <tr>";
            echo "                  <td align=center  style=\"background-color: #fff;\" width='20px'>";
            echo "                    <table width='30%' align=center>";
            echo "                      <tr>";
            echo "                        <td align=center  style=\"background-color: #fff;\">";
            echo "                          <div style='width: {$qtdLegenda}0%;'>";
            echo "                              <div style='float: left; margin-right: 20px;'>";
            echo "                                  <span style='width: 12px; height: 12px; display: block; background-color: #726D70; float: left; margin-right: 2px'>";
            echo "                                  </span>Meta Prevista";
            echo "                              </div>";
            echo "                          </div>";
            echo "                        </td>";
            echo "                        <td align=center  style=\"background-color: #fff;\">";
            echo "                          <div style='width: {$qtdLegenda}0%;'>";
            echo "                              <div style='float: left; margin-right: 20px;'>";
            echo "                                  <span style='width: 12px; height: 12px; display: block; background-color: #00BF0A; float: left; margin-right: 2px'>";
            echo "                                  </span>Situação Atual";
            echo "                              </div>";
            echo "                          </div>";
            echo "                        </td>";
            echo "                      </tr>";
            echo "                    </table>";
//        echo "                      <div style='width: {$qtdLegenda}0%;'>";
//        echo "                          <div style='float: left; margin-right: 5px;'>";
//        echo "                              <span style='width: 12px; height: 12px; display: block; background-color: #00BF0A; float: left; margin-right: 2px'>";
//        echo "                              </span>Brasil";
//        echo "                          </div>";
//        echo "                      </div>";
            echo "                  </td>";
            echo "                  <td align=center  style=\"background-color: #fff;\" width='20px'>";
            echo "                    <table width='30%' align=center>";
            echo "                      <tr>";
            echo "                        <td align=center  style=\"background-color: #fff;\">";
            echo "                          <div style='width: {$qtdLegenda}0%;'>";
            echo "                              <div style='float: left; margin-right: 20px;'>";
            echo "                                  <span style='width: 12px; height: 12px; display: block; background-color: #726D70; float: left; margin-right: 2px'>";
            echo "                                  </span>Meta Prevista";
            echo "                              </div>";
            echo "                          </div>";
            echo "                        </td>";
            echo "                        <td align=center  style=\"background-color: #fff;\">";
            echo "                          <div style='width: {$qtdLegenda}0%;'>";
            echo "                              <div style='float: left; margin-right: 20px;'>";
            echo "                                  <span style='width: 12px; height: 12px; display: block; background-color: #2843FF; float: left; margin-right: 2px'>";
            echo "                                  </span>Situação Atual";
            echo "                              </div>";
            echo "                          </div>";
            echo "                        </td>";
            echo "                      </tr>";
            echo "                    </table>";
//        echo "                      <div style='width: {$qtdLegenda}0%;'>";
//        echo "                          <div style='float: left; margin-right: 10px;'>";
//        echo "                              <span style='width: 12px; height: 12px; display: block; background-color: #2843FF; float: left; margin-right: 2px'></span>Estado";
//        echo "                          </div>";
//        echo "                      </div>";
            echo "                  </td>";
            echo "                  <td align=center  style=\"background-color: #fff;\" width='805px;'>";
            echo "                  </td>";
//        if ($_SESSION['par']['itrid']!=1){
//            echo "              <td align=center  style=\"background-color: #fff;\">";
//            echo "                  <table width='30%' align=center>";
//            echo "                      <tr>";
//            echo "                          <td align=center  style=\"background-color: #fff;\">";
//            echo "                              <div style='width: {$qtdLegenda}0%;'>";
//            echo "                                  <div style='float: left; margin-right: 20px;'>";
//            echo "                                      <span style='width: 12px; height: 12px; display: block; background-color: #f7b850; float: left; margin-right: 2px'></span>Meta";
//            echo "                                  </div>";
//            echo "                              </div>";
//            echo "                          </td>";
//            echo "                          <td align=center  style=\"background-color: #fff;\">";
//            echo "                              <div style='width: {$qtdLegenda}0%;'>";
//            echo "                                  <div style='float: left; margin-right: 20px;'>";
//            echo "                                      <span style='width: 12px; height: 12px; display: block; background-color: #236B8E; float: left; margin-right: 2px'></span>Atual";
//            echo "                                  </div>";
//            echo "                              </div>";
//            echo "                          </td>";
//            echo "                      </tr>";
//            echo "                  </table>";
//            echo "              </td>";
//        }
            echo "              </tr>";
            echo "              <tr>";
            echo "                  <td align=center  style=\"background-color: #fff;\" witdh='25%'>";
            echo "                      <div>";
            echo "                          <div>";
            echo "                              " . echoGraficoBrasil($s, $sub['subiddial'], $pneanoBrasil, $dadosMun, $count, $count2);
            echo "                          </div>";
            echo "                      </div>";
            echo "                  </td>";
            echo "                  <td align=center style=\"background-color: #fff;\"  witdh='25%'>";
            echo "                      <div>";
            echo "                          <div  id=\"div_grfEstuf_" . $s['subiddial'] . "\" style='float: left; margin-left: 30px;'>";
            echo "                              " . echoGraficoEstado($sub['subiddial'], $pneanoBrasil, $dadosMun, $_REQUEST['metid']);
            echo "                          </div>";
            echo "                      </div>";
            echo "                  </td>";
            echo "                  <td align=center  style=\"background-color: #fff;\">";
            echo "                  </td>";

//        if ($_SESSION['par']['itrid']!=1){
//            $mundescricao = $dadosMun['estdescricao'];
//            echo "        <td style=\"background-color: #fff;\" witdh='25%'>";
//            echo "          <div class=\"div_grfMun\" id=\"div_grfMun_" . $s['subiddial'] . "\">";
//            echo                echoGrafico('grafico_municipio_'.$sub['subiddial'], $sub['subiddial'], $pneanoBrasil, $dadosMun, $mundescricao, $anoprevisto);
//            echo "          </div>";
//            echo "        </td>";
//        }
            echo "              </tr>";

            if($s['subtitulo'] !== "Não foi traçada trajetória para esta meta"){
                if($s['metfontemunicipio'] === $s['metfonteestado']){
                    echo "  <tr>";
                    echo "      <td style=\"padding: 0px 10px 5px; background-color: #fff;\" colspan=\"4\">Fonte: {$s['metfontemunicipio']}</td>";
                    echo "  </tr>";
                }else{
                    echo "  <tr>";
                    echo "      <td style=\"padding: 0px 10px 5px; background-color: #fff;\" colspan=\"4\">Fonte: Estado, Região e Brasil - {$s['metfonteestado']}</td>";
                    echo "  </tr>";
                }
            }

            if($_SESSION['par']['itrid']!=1 && ($s['subtitulo'] != "Não foi traçada trajetória para esta meta") && !empty($dadosMun['pnevalor2'])){
                if($s['metfontemunicipio'] !== $s['metfonteestado']){
                    echo "  <tr>";
                    echo "      <td style=\"padding: 0px 10px 5px; background-color: #fff;\" colspan=\"4\">Fonte: Município - {$s['metfontemunicipio']}</td>";
                    echo "  </tr>";
                }
            }

            $count++;
            $count2++;
        }
    }
    echo "            </table>";
    echo "        </td>";
    echo "  </tr>";
    $tabela .= "        <tr>";
    $tabela .= "            <td>";
    $tabela .= '                <div align="left" class="metanaoinformada metanaoinformada_' . $dado['pneiddial'] . '">';
    $tabela .= "					<textarea {$disabled} class=\"form-control\" placeHolder=\"Transcreva aqui a sua meta ou clique diretamente no botão abaixo caso o PME não contemple esta meta\" data-pneiddial=\"{$dado['pneiddial']}\" id=\"pneinformcomplementar_{$s['metid']}\" name=\"pneinformcomplementar[]\" rows=\"5\" style=\"width: 96%; margin: 10px;\">{$dado['micinfcomplementar']}</textarea>";
    $tabela .= '                    <input '.$disabled.' type="button" style="font-family: Arial; margin-left: 10px; margin-bottom: 10px" id="btnNaoInformado" onclick="salvarNaoInformado(' . $s['metid'] . ')" value="Salvar Informações Complementares" class="btn btn-primary">';
    $tabela .= '                </div>';
    $tabela .= "            </td>";
    $tabela .= "        </tr>";
    echo $tabela;
}


function CarregaMetasDial($meta, $dadosMun, $anoprevisto, $qtdLegenda, $legenda, $dados, $arr, $s, $municipios, $mesoregioes, $count, $count2, $pneano){
    global $disabled;
    $metaTotal = 100;
    $anos = array(2015, 2026);
    $pneanoBrasil = selecionaAno($s['subiddial'], 'B');
    echo <<<HTML
<style>
    .divLegenda {
        width: 140px;
        padding-left: 19px;
        margin-top: 10px;
    }
</style>
HTML;
    echo "    <tr>";
    echo "        <td class=\"tabela_painel\">";
    echo "            <table class=\"tabela_painel\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\">";
    echo "              <tr>";
    echo "                  <td align=center  style=\"background-color: #fff; width: 15%;\" width='20px'>";
    echo "                    <div class='divLegenda'>";
    echo "                       <div style='float: left; margin-right: 20px;'>";
    echo "                           <span style='width: 12px; height: 12px; display: block; background-color: #016401; float: left; margin-right: 2px'>";
    echo "                           </span>Meta Prevista";
    echo "                       </div>";
    echo "                       <div style='float: left; margin-right: 20px;'>";
    echo "                           <span style='width: 12px; height: 12px; display: block; background-color: #00BF0A; float: left; margin-right: 2px'>";
    echo "                           </span>Situação Atual";
    echo "                       </div>";
    echo "                    </div>";
    echo "                  </td>";
    echo "                  <td align=center  style=\"background-color: #fff; width: 15%;\">";
    if ($_SESSION['par']['itrid'] == 1) {
        echo "                    <div class='divLegenda'>";
        echo "                       <div style='float: left; margin-right: 20px;'>";
        echo "                           <span style='width: 12px; height: 12px; display: block; background-color: #0101CD; float: left; margin-right: 2px'>";
        echo "                           </span>Meta Prevista";
        echo "                       </div>";
        echo "                       <div style='float: left; margin-right: 20px;'>";
        echo "                           <span style='width: 12px; height: 12px; display: block; background-color: #1E90FF; float: left; margin-right: 2px'>";
        echo "                           </span>Situação Atual";
        echo "                       </div>";
        echo "                    </div>";
        echo "                  </td>";
    }
    echo "              <td align=center  style=\"background-color: #fff; width: 15%;\">";
    if ($_SESSION['par']['itrid']==2 && (!empty($dadosMun['pnevalor2']) || $dadosMun['metid'] == 11)){
        echo "                    <div class='divLegenda'>";
        echo "                       <div style='float: left; margin-right: 20px;'>";
        echo "                           <span style='width: 12px; height: 12px; display: block; background-color: #B8860B; float: left; margin-right: 2px'>";
        echo "                           </span>Meta Prevista";
        echo "                       </div>";
        echo "                       <div style='float: left; margin-right: 20px;'>";
        echo "                           <span style='width: 12px; height: 12px; display: block; background-color: #f7b850; float: left; margin-right: 2px'>";
        echo "                           </span>Situação Atual";
        echo "                       </div>";
        echo "                    </div>";
    }
        echo "              </td>";
    echo "                          <td align=center  style=\"background-color: #fff; width: 55%;\"><br>";
    echo "                          </td>";
    echo "              </tr>";
    echo "              <tr>";
    echo "                  <td align=center  style=\"background-color: #fff;\">";
    echo "                      <div>";
    echo "                          <div>";
    echo "                              ".echoGraficoBrasil($s, $s['subiddial'], $pneanoBrasil, $dadosMun, $count, $count2);
    echo "                          </div>";
    echo "                      </div>";
    echo "                  </td>";

    echo "                  <td align=center style=\"background-color: #fff;\">";
    echo "                      <div>";
    echo "                          <div  id=\"div_grfEstuf_" . $s['subiddial'] . "\">";
    echo "                              ".echoGraficoEstado($s['subiddial'], $pneanoBrasil, $dadosMun, $_REQUEST['metid']);
    echo "                          </div>";
    echo "                      </div>";
    echo "                  </td>";

        echo "        <td align=\"center\" style=\"background-color: #fff;\">";
        echo "          <div class=\"div_grfMun\" id=\"div_grfMun_" . $s['subiddial'] . "\">";

    if ($_SESSION['par']['itrid']==2 && (!empty($dadosMun['pnevalor2']) || $dadosMun['metid'] == 11)){
        $mundescricao = $dadosMun['estdescricao'];
        echo                echoGrafico('grafico_municipio_'.$s['subiddial'], $s['subiddial'], $pneanoBrasil, $dadosMun, $mundescricao, $anoprevisto);
    }
        echo "          </div>";
        echo "        </td>";

    $maxval = 100;
    $maxval = calculaValorMaximo($_REQUEST['metid'], $dadosMun['pnevalor2'], $dadosMun['subtipometabrasil'], $dadosMun['subvalormetabrasil']);

    $stl = '';
    if($dadosMun['pnesemvalor'] == 't'){
        $stl = "style=\"display: none;\"";
    }

    $disAno = '';
    $anoMet7 = '';
    if($disabled != ''){
        $disAno = $disabled;
    } else {
        if($_REQUEST['metid'] == 7){
            $disAno = 'disabled';
            $anoMet7 = 2021;
        }
    }

    echo "        <td align=\"center\" style=\"background-color: #fff;\">";

    $perDialMun = true;
    if($_SESSION['par']['itrid']==2 && (empty($dadosMun['pnevalor2']) && $dadosMun['metid'] != 11)){
        $perDialMun = false;
    }

    if($_SESSION['par']['itrid']==3){
        $meta = 'DF';
    }

    if($perDialMun) {
        echo "            <table class=\"tbNormal\" id=\"tbNormal_{$s['subiddial']}\" {$stl}>";
        echo "                <tr>";
        echo "                    <td class=\"row\">";
        echo "                        <div class=\"div_lbl_grfMun col-md-2\" style='width: 92px;'>";
        echo "                            <label>Meta {$meta}:</label>";
        echo "                        </div>";
        echo "                        <div class=\"div_slider_grfMun col-md-6\" style=\"margin-top: 4px; margin-left: 10px;\">";
        echo "                            <input type=\"hidden\" id=\"hidID\" value=\"" . $s['subiddial'] . "\"/>";
        echo "                            <div class=\"slider-range\" id=\"slider_" . $s['subiddial'] . "\" pneiddial=\"" . $dadosMun['pneiddial'] . "\" maxval=\"" . $maxval . "\" tipometa=\"" . $dadosMun['subtipometabrasil'] . "\" sliderval=\"" . $dadosMun['pnevalormeta'] . "\" pneano=\"" . $pneano . "\" valor=\"1\" subiddial=\"" . $s['subiddial'] . "\" itrid=\"" . $_SESSION['par']['itrid'] . "\">";
        echo "                            </div>";
        echo "                        </div>";
        echo "                        <div class=\"div_text_grfMun col-md-2\" style=\"margin-top: -6px\">";
        echo "							  <input type=\"number\" step=\"any\" style=\"min-width: 76px;\" {$disabled} class=\"form-control txtSlider\"  subiddial=\"" . $s['subiddial'] . "\" maxval=\"" . $maxval . "\" id=\"txtSlider_" . $s['subiddial'] . "\" />";
        echo "                        </div>";
        echo "                    </td>";
        echo "                    <td>";
        echo "                   	  <div class=\"div_btn_NaoInfo\">";
        echo "                    	  	  <input type=\"button\" {$disabled} name=\"btnSalvaDial\" id=\"btnSalvaDial\" onclick=\"changeValue($('#txtSlider_{$s['subiddial']}'), {$_SESSION['par']['itrid']})\" value=\"Salvar\" class=\"btn btn-primary\"/>";
        echo "                    	  </div>";
        echo "                    </td>";
        echo "                </tr>";
        echo "                <tr>";
        echo "                    <td colspan='2'>";
        echo "                        <div class=\"div_lbl_grfAno\" style='width: 78px;'>";
        echo "                            <label>Ano Previsto:</label>";
        echo "                        </div>";
        echo "                        <div class=\"div_combo_grfAno\">";
        echo "                            <select id=\"selAnoCorrente_" . $s['subiddial'] . "\" {$disAno}  name=\"selAnoCorrente_" . $s['subiddial'] . "\" class=\"form-control\">";
        $a = $anos[0];
        $anoprevisto = $a;
        while ($a <= $anos[1]) {
            $sel = $anoMet7 != '' && $anoMet7 == $a ? 'selected' : $dadosMun['pneanoprevisto'] == $a ? 'selected' : '';
            echo "<option value=\"" . $a . "\" {$sel}>" . $a . "</option>";
            $a++;

        }
        echo "                            </select>";
        echo "                        </div>";
        echo "                    </td>";
        echo "                </tr>";
        echo "            </table>";
    }
    echo "        </td>";
    echo "  </tr>";
    if($arr && ($s['subtitulo'] !== "Não foi traçada trajetória para esta meta")){
        if($s['metfontemunicipio'] === $s['metfonteestado']){
            echo "  <tr>";
            echo "      <td style=\"padding: 0px 10px 5px; background-color: #fff;\" colspan=\"4\">Fonte: {$s['metfontemunicipio']}</td>";
            echo "  </tr>";
        }else{
            echo "  <tr>";
            echo "      <td style=\"padding: 0px 10px 5px; background-color: #fff;\" colspan=\"4\">Fonte: Estado, Região e Brasil - {$s['metfonteestado']}</td>";
            echo "  </tr>";
        }
    }
    if($_SESSION['par']['itrid']==2 && ($_REQUEST['metid'] != 14 && $_REQUEST['metid'] != 17 && $_REQUEST['metid'] != 13) && ($s['subtitulo'] != "Não foi traçada trajetória para esta meta") && !empty($dadosMun['pnevalor2'])){
        if($s['metfontemunicipio'] !== $s['metfonteestado']){
            echo "  <tr>";
            echo "      <td style=\"padding: 0px 10px 5px; background-color: #fff;\" colspan=\"4\">Fonte: Município - {$s['metfontemunicipio']}</td>";
            echo "  </tr>";
        }
    }
    echo "            </table><span style='margin-top: 25px; display: block;'></span>";
    echo "        </td>";
    echo "    </tr>";
}

/**
 *
 * @global type $db
 * @param type $estuf
 * @param type $subid
 * @param type $pneano
 * @return type
 */
function retornaPneIdDial($tipo, $codigo, $subid, $pneano){
    global $db;
    if ($tipo=='E'){
        $sql = "SELECT pneiddial from sase.pnedial where estuf = '{$codigo}' and subiddial = {$subid} and pneano = {$pneano}";
    }else if ($tipo == 'M'){
        $sql = "SELECT pneiddial from sase.pnedial where muncod = '{$codigo}' and subiddial = {$subid} and pneano = {$pneano}";
    }
    return $db->pegaUm($sql);
}

/**
 *
 * @global type $db
 * @param type $pneiddial
 * @param type $anoprevisto
 * @return type
 */
function retornaPnePrevIdDial($pneiddial, $anoprevisto){
    global $db;
    $sql = "SELECT ppdid from sase.pneprevdial where pneiddial = '{$pneiddial}' and pneanoprevisto = {$anoprevisto}";
    return $db->pegaUm($sql);
}

/**
 *
 * @param type $div
 * @param type $subiddial
 * @param type $pneanoBrasil
 * @param type $dadosMun
 * @param type $mundescricao
 * @param type $anoprevisto
 */
function echoGrafico($div, $subiddial, $pneanoBrasil, $dadosMun, $mundescricao, $anoprevisto){
    $descricao = substr($dadosMun['descricao'], 0, strrpos($dadosMun['descricao'], '::'));
    $tipoMeta = $dadosMun['subtipometabrasil'];
    if($anoprevisto == ''){
        $anoprevisto = $dadosMun['pneanoprevisto'];
    }
//    if (trim($dadosMun['subtipometabrasil'])==''){
//        $tipoMeta = 'P';
//    }else{
//        $tipoMeta = $dadosMun['subtipometabrasil'];
//    }
    $metaTotal = 'P' == $tipoMeta ? 100 : $dadosMun['subvalormetabrasil'];
    $dadosGrafico = array(
        'cor' => '#f7b850',
        'descricao' => str_replace("'", '', $descricao),
        'valor' => $dadosMun['pnevalor2'],
        'metaTotal' => $metaTotal != '' ? $metaTotal : 0,
        'metaBrasil' => $dadosMun['pnevalormeta'] != '' ? $dadosMun['pnevalormeta'] : 0,
        'tipo' => $tipoMeta,
        'plotBandsCor' => '#B8860B',
//        'plotBandsOuterRadius' => '115%',
        'title' => "Meta Município:",
        'anoprevisto' => $anoprevisto
    );
    echo geraGraficoPNE($div, $dadosGrafico);
}

/**
 *
 * @param type $estuf
 * @param type $subiddial
 * @param type $pneano
 * @param type $pnevalormeta
 * @param type $anoprevisto
 * @param type $pneanoBrasil
 * @param type $valor
 */
function atualizaDadosEstUf($div, $estuf, $subiddial, $pneano, $pnevalormeta, $anoprevisto, $pneanoBrasil, $valor, $alteraPnePrev = true){
//    global $db;
    $pneiddial = retornaPneIdDial('E', $estuf, $subiddial, $pneano);

    if ($alteraPnePrev == false) {
        $pneprev = new Sase_Model_Pneprev();
        $pneprev->carregaPorPneid($pneiddial);
        $pneprev->pneiddial = $pneiddial;
        $pneprev->subid = $subiddial;
        $pneprev->pneanoprevisto = $anoprevisto;
        $pneprev->pnecpfinclusao = $_SESSION['usucpf'];
        $pneprev->pnedatainclusao = date('d/m/Y H:i:s');
        $pneprev->pnesemvalor = 'f';
        $pneprev->pnevalormeta = $pnevalormeta;
        $success = $pneprev->salvar();
        $pneprev->commit();
    } else {
        $success = true;
    }

    if ($success) {
        $dadosMun = retornaDadosEst($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['estuf'], $pneano, $_SESSION['par']['itrid']);
        $mundescricao = $dadosMun['estdescricao'];
        echoGraficoEstado($subiddial, $pneanoBrasil, $dadosMun, $_REQUEST['metid']);
    }

//    if(!empty($pneiddial)) {
//        $ppdid = retornaPnePrevIdDial($pneiddial, $anoprevisto);
//        if(!empty($ppdid)){
//            if ($pnevalormeta!=0){
//                $sql = "update sase.pneprevdial set pnevalormeta = {$pnevalormeta}, pnesemvalor = false where ppdid = {$ppdid}";
//            }else{
//                $sql = "update sase.pneprevdial set pnevalormeta = 'NULL', pnesemvalor = true where ppdid = {$ppdid}";
//            }
//            $db->executar($sql);
//            if ($db->commit()) {
//                $dadosMun = retornaDadosEst($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['estuf'], $pneano, $_SESSION['par']['itrid']);
//                $mundescricao = $dadosMun['estdescricao'];
//                echoGraficoEstado($subiddial, $pneanoBrasil, $dadosMun, $_REQUEST['metid']);
//            }
//        }else{
//            if ($pnevalormeta!=0){
//                $sql = "insert into sase.pneprevdial (pneiddial, pnevalormeta, pnesemvalor, pneanoprevisto) values ({$pneiddial}, {$pnevalormeta}, false, {$anoprevisto})";
//            }else{
//                $sql = "insert into sase.pneprevdial (pneiddial, pnevalormeta, pnesemvalor, pneanoprevisto) values ({$pneiddial}, NULL, true, {$anoprevisto})";
//            }
//            $db->executar($sql);
//            if ($db->commit()) {
//                $dadosMun = retornaDadosEst($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['estuf'], $pneano, $_SESSION['par']['itrid']);
//                $mundescricao = $dadosMun['estdescricao'];
//                echoGraficoEstado($subiddial, $pneanoBrasil, $dadosMun, $_REQUEST['metid']);
//            }
//        }
//    }else {
//        $sql = "insert into sase.pnedial (subiddial, pneano, estuf, pnetipo) values ({$subiddial}, {$pneano}, '{$_SESSION['par']['estuf']}', 'E') returning pneiddial";
//        $pneiddial = $db->pegaUm($sql);
//        if($db->commit()){
//            if ($pnevalormeta!=0){
//                $sql = "insert into sase.pneprevdial (pneiddial, pnevalormeta, pnesemvalor, pneanoprevisto) values ({$pneiddial}, {$pnevalormeta}, false, {$anoprevisto})";
//            }else{
//                $sql = "insert into sase.pneprevdial (pneiddial, pnevalormeta, pnesemvalor, pneanoprevisto) values ({$pneiddial}, NULL, true, {$anoprevisto})";
//            }
//            $db->executar($sql);
//            $dadosMun = retornaDadosEst($valor, $subiddial, $_REQUEST['metid'], $_SESSION['par']['estuf'], $anoprevisto, $_SESSION['par']['itrid']);
//            $mundescricao = $dadosMun['estdescricao'];
//            echoGrafico($div, $subiddial, $pneanoBrasil, $dadosMun, $mundescricao, $anoprevisto);
//        }
//    }
}


function atualizaDadosMun($div, $muncod, $subiddial, $pneano, $pnevalormeta, $anoprevisto, $pneanoBrasil, $valor, $alteraPneprev = true){
//    global $db;

    $pneiddial = retornaPneIdDial('M', $muncod, $subiddial, $pneano);
    if($alteraPneprev==false) {
        if (empty($pneiddial)) {
            $pnedial = new Sase_Model_Pnedial();
            $pnedial->subiddial = $subiddial;
            $pnedial->pnevalor = 0;
            $pnedial->pneano = $pneano;
            $pnedial->pnetipo = 'M';
            $pnedial->muncod = $muncod;
            $pneiddial = $pnedial->salvar();
        }

        $pneprev = new Sase_Model_Pneprev();
        $pneprev->carregaPorPneid($pneiddial);
        $pneprev->pneiddial = $pneiddial;
        $pneprev->subid = $subiddial;
        $pneprev->pneanoprevisto = $anoprevisto;
        $pneprev->pnecpfinclusao = $_SESSION['usucpf'];
        $pneprev->pnedatainclusao = date('d/m/Y H:i:s');
        $pneprev->pnesemvalor = 'f';
        $pneprev->pnevalormeta = $pnevalormeta;
        $pneprev->pnemuncod = $muncod;
        $success = $pneprev->salvar();
        $pneprev->commit();
    } else {
        $success = true;
    }
    if($success){
        $dadosMun = retornaDadosMun($valor, $subiddial, $_REQUEST['metid'], $muncod, $pneano, $_SESSION['par']['itrid']);
        $mundescricao = $dadosMun['mundescricao'];
        echoGrafico($div, $subiddial, $pneanoBrasil, $dadosMun, $mundescricao, $anoprevisto);
    }

}


/**
 *
 * @param type $s
 * @param type $pneanoBrasil
 * @param type $metaTotalBrasil
 * @param type $metaBrasil
 * @param type $dadosMun
 * @param type $count
 * @param type $count2
 */
function echoGraficoBrasil($s, $subiddial, $pneanoBrasil, $dadosMun, $count, $count2){

    $dadosBrasil = retornaDadosBrasil($subiddial, $pneanoBrasil);
    $descricaoBrasil = substr($descricaoBrasil, 0, strrpos($descricaoBrasil, '::'));
    $metaTotalBrasil = $dadosBrasil[0]['subvalormetabrasil'];
    //if ($_REQUEST['metid']==7){ $ano = '2015'; }else{ $ano=$pneanoBrasil;}
    $ano=$pneanoBrasil;
    $metaBrasil = $dadosBrasil[0]['pnevalor'];
    $tipoMeta = $dadosBrasil[0]['subtipometabrasil'];
//    if (trim($dadosBrasil[0]['subtipometabrasil'])==''){
//        $tipoMeta = 'P';
//    }else{
//        $tipoMeta = $dadosBrasil[0]['subtipometabrasil'];
//    }

    $metaTotal = $tipoMeta == 'P' ? 100 : $dadosBrasil[0]['subvalormetabrasil'];
    $dadosGraficoBrasil = array(
        'cor' => '#00BF0A',
        'descricao' => str_replace("'", '', $descricaoBrasil),
        'valor' => $dadosBrasil[0]['pnevalor'],
        'metaTotal' => $metaTotal,
        'metaBrasil' => $metaTotalBrasil,
        'plotBandsCor' => '#006400',
        'tipo' => $dadosBrasil[0]['subtipometabrasil']/*,
        'anoprevisto' => $ano*/
    );
    echo geraGraficoPNE('grafico_BR' . $count . '_' . $count2, $dadosGraficoBrasil);
}

function calculaValorMaximo($metid, $valor, $tipometa, $valormeta){
    $v = 0;
    switch($metid){
        case 11:
            if($valor == 0){
                $v = 300;
            } else {
                $v = 3 * $valor;
            }
            break;
        case 14:
            $v = 15000;
            break;
        case 15:
            $v = 100;
            break;
        case 7:
            $v = 10;
            break;
        default:
            if($tipometa == 'P'){
                $v = 100;
            } else {
                $v = $valormeta;
            }
            break;
    }
    return $v;
}

/**
 *
 * @param type $s
 * @param type $pneanoBrasil
 * @param type $metaBrasil
 * @param type $dadosMun
 */
function echoGraficoEstado($subid, $pneanoBrasil, $dadosMun, $metid){
    $anoEstado = selecionaAno($subid, 'E');
    $dadosEstado = retornaDadosEstado($subid, $anoEstado, $_SESSION['par']['estuf']);
    $descricaoEstado = substr($descricaoEstado, 0, strrpos($descricaoEstado, '::'));
    $ano = $dadosEstado[0]['pneanoprevisto'];
    /*if ($_REQUEST['metid']==7){
        $ano = '2015';
    }*/
    $metaEstado = $dadosEstado[0]['pnevalor'];
    $tipoMeta = $dadosEstado[0]['subtipometabrasil'];
    $valor = $dadosEstado[0]['pnevalor'];
    if ( ($_SESSION['par']['itrid']==1) || ($_SESSION['par']['itrid']==3) ) {
        $metaTotalEstado = $dadosEstado[0]['pnevalormeta'];
//         $valor = $dadosEstado[0]['pnevalormeta'];
    } else {
        $metaTotalEstado = $dadosEstado[0]['subvalormetabrasil'];
//        $valor = $dadosEstado[0]['pnevalor'];
    }


    //$metaTotal = 'P' != $tipoMeta && $dadosEstado[0]['subvalormetabrasil'] != 0 ? $dadosEstado[0]['subvalormetabrasil'] : 100;
    $title = "Meta Estado:";
    if($_SESSION['par']['estuf'] == 'DF'){
        $title = "Meta DF:";
    }

    $metaTotal = 'P' != $tipoMeta ? calculaValorMaximo($metid, $valor, $dadosEstado[0]['subtipometabrasil'], $dadosEstado[0]['subvalormetabrasil']) : 100;

    $dadosGraficoEstado = array(
        'cor' => '#1E90FF',
        'descricao' => str_replace("'", '', $descricaoEstado),
        'valor' => $valor,
        'metaTotal' => $metaTotal,
        'metaBrasil' => $metaTotalEstado,
        'title' => $title,
        'plotBandsCor' => '#0000CD',
        'tipo' => $dadosEstado[0]['subtipometabrasil'],
        'anoprevisto' => $ano
    );
    if(!empty($dadosGraficoEstado['valor'])){
        echo geraGraficoPNE('grafico_estuf' . $subid, $dadosGraficoEstado);
    }
    
}


function carregaSubmetas($metid){
    global $db;
    $sql = "select sub.subiddial,
                   sub.subtitulo,
                   meta.metfontemunicipio,
                   meta.metfonteestado,
                   meta.mettitulo,
                   sub.subnotatecnica,
                    sub.metid
              from sase.submetadial as sub
              left join sase.meta meta
                on (sub.metid = meta.metid)
             where sub.substatus = 'A'
               and sub.metid = ".$metid."
             order by sub.subordem ASC";
    return $db->carregar($sql);
}

/**
 * Funçoes referentes ao Grafico PNE do PAR
 */

function geraGraficoPNE($nomeUnico, $dados)
{
	$meta = $dados['metaBrasil'];
	$metaTotal = $dados['metaTotal'];
	$metaFormatada = number_format($meta, 0, ',', '.');

	$complemento = $simbolo = '';

	$casasDecimais = 0;

	switch ($dados['tipo']) {
		case ('P'):
			$complemento = $simbolo = '%';
			$metaFormatada = $meta;
			$casasDecimais = 1;
			break;
		case ('A'):
			$complemento = ' anos';
			/**
			 * Victor Martins Machado
			 * Incluído para mostrar o valor com uma casa decimal
			 */
			$metaFormatada = $meta;
			$casasDecimais = 1;
			break;
		case ('M'):
			$complemento = ' matrículas';
			break;
		case ('T'):
			$complemento = ' títulos';
			break;
		default:
			$metaFormatada = $meta;
			$casasDecimais = 1;
			break;
	}

	$formatter = "function () { return '<div style=\"text-align:center\"><span style=\"font-size:20px;color:black; font-weight: normal;\">' + number_format(this.y, " . $casasDecimais .", ',', '.') + '" . $simbolo ."</span><br/>' +
                                       '<span style=\"font-size:13px;color:black; font-weight: normal;\">" . $dados['descricao'] . "</span></div>'; }";

	?>

    <script type="text/javascript">
        jQuery(function () {

            var gaugeOptions = {

                chart: {
                    type: 'solidgauge'
                },

                title: null,


                credits: {
                    enabled: false
                },
                exporting: {
                    enabled: false
                },

                credits: {
                    enabled: false
                },
                exporting: {
                    enabled: false
                },

                pane: {
                    center: ['50%', '85%'],
                    size: '120%',
                    startAngle: -90,
                    endAngle: 90,
                    background: {
                        backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || '#EEE',
                        innerRadius: '60%',
                        outerRadius: '100%',
                        shape: 'arc'
                    }
                },

                tooltip: {
                    enabled: false
                },

                // the value axis
                yAxis: {
                    stops: [
                        [0, '<?php echo $dados['cor']; ?>']
                    ],
                    lineWidth: 0,
                    minorTickInterval: null,
                    tickPixelInterval: 400,
                    tickWidth: 0,
                    title: {
                        y: -55
                    },
                    labels: {
                        enabled: false
                    },
                    plotBands: [{
                        from: 0,
                        to: parseFloat('<?php echo $meta; ?>'),
                        color: '<?php echo $dados['plotBandsCor'] == '' ? '#726D70' : $dados['plotBandsCor']; ?>',
                        innerRadius: '100%',
                        outerRadius: '<?php echo $dados['plotBandsOuterRadius'] == '' ? '110%' : $dados['plotBandsOuterRadius']; ?>'
                    }]
                },

                plotOptions: {
                    solidgauge: {
                        dataLabels: {
                            y: 5,
                            borderWidth: 0,
                            useHTML: true
                        }
                    }
                }
            };

            // The speed gauge
            jQuery('#<?=$nomeUnico?>').highcharts(Highcharts.merge(gaugeOptions, {
                yAxis: {
                    min: 0,
                    max: parseFloat('<?php echo $metaTotal; ?>'),
                    title: {
                        text: '<span style="color:black;"><?php echo $dados['title'] == '' ? 'Meta Brasil:' : $dados['title']; ?> <?php echo $metaFormatada . $complemento; ?><?php echo $dados['anoprevisto'] != '' ? ' - '.$dados['anoprevisto'] : ''; ?></span>'
                    }
                },

                credits: {
                    enabled: false
                },

                series: [{
                    name: 'Meta',
                    data: [0],
                    dataLabels: {
                        y: 25,
                        formatter: <?php echo $formatter; ?>
                    },
                    tooltip: {
                        valueSuffix: ' '
                    }
                }]

            }));

            // Bring life to the dials
            var chart = $('#<?=$nomeUnico?>').highcharts();
            if (chart) {
                var point = chart.series[0].points[0],
                    newVal = parseFloat('<?php echo $dados['valor']; ?>');
                point.update(newVal);
            }
        });
    </script>
    <div id="<?=$nomeUnico?>" style="width: 200px; height: 150px;"></div>
<?
}

function naoQuantificado($subid, $pneano, $muncod, $pnetipo, $pneid, $pneanoprevisto, $naoqtf){
    try{
        if(empty($subid))  throw new Exception('Variável $subid sem valor');
        if(empty($pneano)) throw new Exception('Variável $pneano sem valor');
        if(empty($pnetipo)) throw new Exception('Variável $pnetipo sem valor');
        if(empty($pneid)) throw new Exception('Variável $pneid sem valor');
        if(empty($pneanoprevisto)) throw new Exception('Variável $pneanoprevisto sem valor');
        if(empty($naoqtf)) throw new Exception('Variável $naoqtf sem valor');

        $pneprev = new Sase_Model_Pneprev();
        $pneprev->pneiddial = $pneid;
        $pneprev->subid = $subid;
        $pneprev->pneanoprevisto = $pneanoprevisto;
        $pneprev->pnecpfinclusao = $_SESSION['usucpf'];
        $pneprev->pnedatainclusao = date('d/m/Y H:i:s');
        $pneprev->pnesemvalor = $naoqtf;
        $pneprev->pnevalormeta = null;
        $success = $pneprev->salvar();
        $pneprev->commit();
        if(!$success) throw new Exception('Erro ao salvar os dados.');
    }
    catch(Simec_Db_Exception $e){
        echo 'Erro: '.$e->getMessage();
    }
    catch(Exception $e) {
        echo 'Erro: '.$e->getMessage();
    }
}
?>