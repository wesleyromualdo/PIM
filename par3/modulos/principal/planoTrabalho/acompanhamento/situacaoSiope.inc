<?php

global $db;
$sql = "SELECT estuf, muncod, inudescricao FROM par3.instrumentounidade WHERE inuid = {$_REQUEST['inuid']}";
$dados = $db->pegaLinha($sql);

switch ($_REQUEST['req']) {
    case 'xls':
        ob_clean();

        $retorno = array();

        $ano = (int)$_REQUEST['ano'];

        if($ano < 2017){
            $cabecalho = array('Código', 'Município', '2009', '2010', '2011', '2012', '2013', '2014', '2015', '2016');
            $situacao = buscaCumprimentos($dados);
            $situacao = is_array($situacao) ? $situacao : array();

            foreach ($situacao as $k){
                $resultado['codigo'] = isset($k['co_municipio_ibge_rpm']) ? $k['co_municipio_ibge_rpm'] : $k['sg_uf_mde'];
                $resultado['unidade'] = isset($k['no_municipio_rpm']) ? $k['no_municipio_rpm'] : $k['no_uf_mde'];
                $resultado[$k['an_exercicio_mde']] = $k['num_periodo'];
            }


            $retorno['codigo'] = isset($resultado['codigo']) ? $resultado['codigo'] : '';
            $retorno['unidade'] = isset($resultado['unidade']) ? $resultado['unidade'] : '';
            $retorno['2009'] = isset($resultado['2009']) ? 'X' : '';
            $retorno['2010'] = isset($resultado['2010']) ? 'X' : '';
            $retorno['2011'] = isset($resultado['2011']) ? 'X' : '';
            $retorno['2012'] = isset($resultado['2012']) ? 'X' : '';
            $retorno['2013'] = isset($resultado['2013']) ? 'X' : '';
            $retorno['2014'] = isset($resultado['2014']) ? 'X' : '';
            $retorno['2015'] = isset($resultado['2015']) ? 'X' : '';
            $retorno['2016'] = isset($resultado['2016']) ? 'X' : '';

        }
        else{
            $cabecalho = array('Código', 'Município', '1º Bimestre', '2º Bimestre', '3º Bimestre', '4º Bimestre', '5º Bimestre', '6º Bimestre');
            $situacao = buscaCumprimentos($dados, $ano);
            $situacao = is_array($situacao) ? $situacao : array();

            foreach ($situacao as $k){
                $retorno['codigo'] = isset($k['co_municipio_ibge_rpm']) ? $k['co_municipio_ibge_rpm'] : $k['sg_uf_mde'];
                $retorno['unidade'] = isset($k['no_municipio_rpm']) ? $k['no_municipio_rpm'] : $k['no_uf_mde'];
                $retorno[$k['num_periodo']] = 'X';
            }

            $retorno['1'] = isset($retorno['1']) ? 'X' : '';
            $retorno['2'] = isset($retorno['2']) ? 'X' : '';
            $retorno['3'] = isset($retorno['3']) ? 'X' : '';
            $retorno['4'] = isset($retorno['4']) ? 'X' : '';
            $retorno['5'] = isset($retorno['5']) ? 'X' : '';
            $retorno['6'] = isset($retorno['6']) ? 'X' : '';
        }

        $dados = array($retorno);

        ini_set("memory_limit", "7000M");
        echo geraExcel($dados, 'situacao_siope', $cabecalho);
        die;
        break;
   case 'gerapdf':
       ob_clean();
//       html2Pdf("
//
//				<html>
//				  <head>
//				    <style>
//
//
//				    </style>
//				  </head>
//				  <body>
//				    asdasd
//
//				  </body>
//				</html>",
//           "termoAdesao");

        die;
        break;
    case "consultaVeiculacao":
        ob_clean();

        $retorno = array();

        $ano = (int)$_REQUEST['ano'];

        if($ano < 2017){
            $situacao = buscaCumprimentos($dados);
            $situacao = is_array($situacao) ? $situacao : array();

            foreach ($situacao as $k){
                $retorno[$k['an_exercicio_mde']] = $k['num_periodo'];
            }

            $retorno['2009'] = isset($retorno['2009']) ? 'X' : '';
            $retorno['2010'] = isset($retorno['2010']) ? 'X' : '';
            $retorno['2011'] = isset($retorno['2011']) ? 'X' : '';
            $retorno['2012'] = isset($retorno['2012']) ? 'X' : '';
            $retorno['2013'] = isset($retorno['2013']) ? 'X' : '';
            $retorno['2014'] = isset($retorno['2014']) ? 'X' : '';
            $retorno['2015'] = isset($retorno['2015']) ? 'X' : '';
            $retorno['2016'] = isset($retorno['2016']) ? 'X' : '';

        }
        else{
            $situacao = buscaCumprimentos($dados, $ano);
            $situacao = is_array($situacao) ? $situacao : array();

            foreach ($situacao as $k){
                $retorno[$k['num_periodo']] = 'X';
            }

            $retorno['1'] = isset($retorno['1']) ? 'X' : '';
            $retorno['2'] = isset($retorno['2']) ? 'X' : '';
            $retorno['3'] = isset($retorno['3']) ? 'X' : '';
            $retorno['4'] = isset($retorno['4']) ? 'X' : '';
            $retorno['5'] = isset($retorno['5']) ? 'X' : '';
            $retorno['6'] = isset($retorno['6']) ? 'X' : '';
        }

        echo json_encode($retorno);
        die;
        break;
    default:
        break;
}


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

function buscaCumprimentos($dados, $ano = null)
{
    if( !empty($dados['inuid']) ){
        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade( $dados['inuid'] );
        $estuf = $instrumentoUnidade->estuf;
        $muncod = $instrumentoUnidade->muncod;
        $itrid = $instrumentoUnidade->itrid;

        $dados['estuf'] = $estuf;
        $dados['muncod'] = $muncod;
    }

    if(isset($ano)){
        $filtroAno = "AND an_exercicio_mde = " . $ano;
    }

    if($dados['muncod']) {
        $sql = "SELECT an_exercicio_mde, no_municipio_rpm, num_periodo, co_municipio_ibge_rpm
			FROM integracao.cump_vinc_municipal
			WHERE an_exercicio_mde IS NOT NULL AND co_municipio_ibge_rpm = '{$dados['muncod']}' {$filtroAno}
			ORDER BY an_exercicio_mde";
    }else{
        $sql = "SELECT an_exercicio_mde, no_uf_mde, num_periodo, sg_uf_mde
			FROM integracao.cump_vinc_estado
			WHERE an_exercicio_mde IS NOT NULL AND sg_uf_mde = '{$dados['estuf']}' {$filtroAno}
			ORDER BY an_exercicio_mde";
    }

    return adapterConnection::coorporativo()->carregar($sql);
}

function geraExcel($data, $nome_arquivo, array $cabecalho = array()){
    ob_clean();

    // este método transforma uma query em excel
    $extension = '.xls';
    $arquivo = 'SIMEC_' . $nome_arquivo . '_' . date('d-m-Y_his') . $extension;

    //$data = !is_array($sql) ? adapterConnection::coorporativo()->carregar($sql) : $sql;
    $data = is_array($data) ? $data : array();
    array_unshift($data, $cabecalho);

    $xls = new Excel_XML();
    $xls->addWorksheet($nome_arquivo, $data);
    $xls->sendWorkbook($arquivo);

    die;
}



?>
<style>
    #divLegenda{
        width: 20%;
        background-color: #FFFFff ;
        border: 1px solid #000000;
        padding: 10px;
    }

    .table > thead > tr > .tituloBimestral{
        background-color: #D6E2BA;
        font-weight: bold;
        color: #000000;
    }
    .table{
        font-size: 12px;
        text-align: center;
    }
    .tabelaSituacao{
        width: 40%;
        margin-left: 20%;
    }
    #divBotoes{
        width: 40%;
        margin-left: 40%;
    }


</style>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12" >
                <h3 id="entidade">Situação de entrega</h3>
            </div>
        </div>
    </div>

    <div class="ibox-content" id="conteudo">
        <div style="width: 100%;margin:0 auto; padding: 0 0 0 0; overflow: auto;">

            <?php

            if($dados['muncod']){
                $codigo = $dados['muncod'];
                $tituloMunEst = "Município";
            }
            else{
                $codigo = $dados['estuf'];
                $tituloMunEst = "Estado";
            }
            $ano = date(Y);
            echo '<input type="hidden" id="anoAtual" value="'.$ano.'"/>';
            echo '<input type="hidden" id="codigo" value="' . $codigo . '"/>';
            echo '<input type="hidden" id="tituloMunEst" value="' . $tituloMunEst . '"/>';
            echo '<input type="hidden" id="tituloMunEst" value="' . $tituloMunEst . '"/>';
            echo '<input type="hidden" id="anoControle" value="'.$ano.'"/>';
            echo '<input type="hidden" id="inudescricao" value="'.$dados['inudescricao'].'"/>';

            ?>

        <!--    Inicio Situação de entrega-->
            

            <div id="divLegenda">
                    <b>Legenda</b><br/>
                    <span style="color: #00AA00; font-weight: bold">X</span> : Declaração transmitida;<br/>
                    <b>Em branco</b>: Não entregou declaração;<br/>
                    <b>SB</b>: Sem Balanço;<br/>
                </div><br/>

            <div id="divBotoes">

                <button type="button" id="btn-excel" class="btn btn-primary xls" style="width:110px;"><i class="fa fa-file-excel-o"></i> Gerar Excel
                </button>&nbsp;
                <button type="button" id="btn-imprimir" class="btn btn-warning" style="width:110px;"><i class="glyphicon glyphicon-print"></i> Gerar PDF
                </button>
<!--                <a href="/par3/par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&inuid=--><?php //echo $inuid?><!--&menu=situacaoentrega&req=xls" target="_blanck">-->
<!--                    <button type="button"  class="btn btn-warning" style="width:110px;"><i class="glyphicon glyphicon-print"></i> teste-->
<!--                    </button></a>-->
<!--                <br/><br>-->
            </div>

            <div class="tabelaSituacao">

                    <div id="tabelaSituacao">
                        <!-- aqui vai o conteudo variavel da tabela de situação -->
                    </div>



                <button type="button" class="btn btn-success" id="btn-anterior" onclick="carregarTebelaAno(pegaAnoAnterior(),<?php echo  "'" . $codigo ."','". $tituloMunEst."'" ?>)"><i class="fa fa-chevron-left"></i> Anterior</button>
                <button type="button" class="btn btn-success" id="btn-proximo" style="float: right" onclick="carregarTebelaAno(pegaProximoAno(),<?php echo  "'" . $codigo ."','". $tituloMunEst."'"?>)"><i class="fa fa-chevron-right"></i> Proximo</button>
            </div>

            <div id="div-impressao" style="display: none">
                <div id="div_cabecalho" class="row col-lg-offset-2" >
                    <table width="92%" border="0" cellpadding="0" cellspacing="0" style="text-align:center;">
                        <tr bgcolor="#ffffff">
                            <td valign="top" width="50" rowspan="2">
                                <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                            </td>
                            <td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
                                MEC/FNDE <br>
                                Relatório Situação de Entrega<br>
                            </td>
                            <td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
                                Impresso por: <b> <?= $_SESSION['usunome']; ?></b><br/>
                                Hora da Impressão: <?= date('d/m/Y - H:i:s'); ?><br/>
                            </td>
                        </tr>
                    </table>
                    <hr style="color:black; width:95%; margin-left:-18px;">
                </div>
                <div class="clearfix" style="margin-bottom:10px;"></div>
                <div id="impressao-content">

                </div>
                <br><br><br>
                <div id="divLegenda">
                    <b>Legenda</b><br/>
                    <span style="color: #00AA00; font-weight: bold">X</span> : Declaração transmitida;<br/>
                    <b>Em branco</b>: Não entregou declaração;<br/>
                    <b>SB</b>: Sem Balanço;<br/>
                </div><br/>
            </div>
        </div>
    </div>
<!--Fim Situação de entraga-->

<script>

    $(document).ready(function(){

        var ano = $("#anoAtual").val();
        var codigo = $("#codigo").val();
        var tituloMunEst = $("#tituloMunEst").val();

        carregarTebelaAno(ano,codigo, tituloMunEst);
    })


    function pegaProximoAno() {
        var ano = parseInt($("#anoControle").val()) + parseInt(1);
        $("#anoControle").val(ano);
        return ano;
    }

    function pegaAnoAnterior() {
        var ano = $("#anoControle").val() - 1;
        $("#anoControle").val(ano);
        return ano;
    }

	function carregarTebelaAno(ano, codigo, titulo) {

        var dados;
        var descricao = $("#inudescricao").val();

        $.ajax({
            url: window.location,
            type: "POST",
            data: "req=consultaVeiculacao&ano=" + ano,
            dataType: "json",
            async: false,
            success: function (data) {
                dados = data;
                return false;
            }
        });

	    $("#tabelaSituacao").html("");

        var tabelaAno = ' <table cellpadding="2" cellspacing="2" class="table table-bordered">'+
            '<thead>' +
        '<tr>' +
            '<td id="tbAno" colspan="2" style="background-color: #FFFFFF; border: 1px solid #FFFFFF; color: #FFFFFF"></td>'+
            '<td colspan="6" style="background-color: #E4E4E4 ; color: #000000; text-align: center;">' + ano +'</td>'+
        '</tr>'+
        '<tr>'+
            '<td class="tituloBimestral">Código</td>'+
            '<td class="tituloBimestral">'+ titulo +'</td>'+
            '<td class="tituloBimestral">1º</td>'+
            '<td class="tituloBimestral">2º</td>'+
            '<td class="tituloBimestral">3º</td>'+
            '<td class="tituloBimestral">4º</td>'+
            '<td class="tituloBimestral">5º</td>'+
            '<td class="tituloBimestral">6º</td>'+
        '</tr>'+
        '</thead>'+
        '<tbody>'+
            '<tr>'+
                '<td>'+ codigo +'</td>'+
                '<td>'+ descricao +'</td>'+
                '<td>'+ dados['1'] +'</td>'+
                '<td>'+ dados['2'] +'</td>'+
                '<td>'+ dados['3'] +'</td>'+
                '<td>'+ dados['4'] +'</td>'+
                '<td>'+ dados['5'] +'</td>'+
                '<td>'+ dados['6'] +'</td>'+
            '</tr>'+
        '</tbody>'+
        '</table>';


        var tabelaPeriodo =  '<table  cellpadding="2" cellspacing="2" class="table table-bordered">'+
            '<thead>'+
        '<tr>'+
            '<td id="tbAno" colspan="2" style="background-color: #FFFFFF; border: 1px solid #FFFFFF; color: #FFFFFF"></td>'+
            '<td colspan="8" style="background-color: #E4E4E4 ; color: #000000; text-align: center;">Períodos</td>'+
        '</tr>'+
        '<tr>'+
            '<td class="tituloBimestral">Código</td>'+
            '<td class="tituloBimestral">'+ titulo +'</td>'+
            '<td class="tituloBimestral">2009</td>'+
            '<td class="tituloBimestral">2010</td>'+
            '<td class="tituloBimestral">2011</td>'+
            '<td class="tituloBimestral">2012</td>'+
            '<td class="tituloBimestral">2013</td>'+
            '<td class="tituloBimestral">2014</td>'+
            '<td class="tituloBimestral">2015</td>'+
            '<td class="tituloBimestral">2016</td>'+
        '</tr>'+
        '</thead>'+
        '<tbody>'+
        '<tr>'+
        '<td>'+ codigo +'</td>'+
        '<td>'+ descricao +'</td>'+
        '<td>'+ dados['2009'] +'</td>'+
        '<td>'+ dados['2010'] +'</td>'+
        '<td>'+ dados['2011'] +'</td>'+
        '<td>'+ dados['2012'] +'</td>'+
        '<td>'+ dados['2013'] +'</td>'+
        '<td>'+ dados['2014'] +'</td>'+
        '<td>'+ dados['2015'] +'</td>'+
        '<td>'+ dados['2016'] +'</td>'+
        '</tr>'+
        '</tbody>'+
        '</table>';


        //Habilita e desabilita botões de navegação
        var anoAtual = $("#anoAtual").val();
        if(ano >= anoAtual){
            $('#btn-proximo').attr('disabled','disabled');
            $('#btn-anterior').removeAttr('disabled');
        }
        else if(ano < 2017){
            $('#btn-anterior').attr('disabled','disabled');
            $('#btn-proximo').removeAttr('disabled');
        }
        else{
            $('#btn-proximo').removeAttr('disabled');
            $('#btn-anterior').removeAttr('disabled');
        }

        //Verifica se será criado tabela bimestral ou anual
        if(ano < 2017){
            $(tabelaPeriodo).appendTo("#tabelaSituacao");
        }else{
            $(tabelaAno).appendTo("#tabelaSituacao");
        }
    }

    $("#btn-imprimir").on("click", function () {

        $("#impressao-content").html($("#tabelaSituacao").html());
        var container = $("#impressao-content");
        var table = container.find('table');
        var th = container.find('th');
        var td = container.find('td');
        table.css('width', '100%')
        table.css('border-collapse', 'collapse');
        container.find(".popover,button,.modal").remove();
        container.find("table,td,th").css('border', '1px solid black');
        container.find('tr:even').css('background-color', 'silver');
        th.css('height', '50px');
        td.css('text-align', 'center');
        var w = window.open();
        w.document.write($('#div-impressao').html());
        w.document.title = 'Relatório - Situação de Entrega';
        w.print();
        w.close();
        $("#impressao-content").html("");
        return true;
    });

    $("#btn-excel").on("click", function () {
        var ano = $("#anoControle").val();
        var url = window.location
        window.location.assign(url + "&req=xls&" + '&ano=' + ano);
    });

</script>
