<?php
ob_start();
include APPRAIZ . 'includes/cabecalho.inc';
global $db;

$subid = $_POST['subid'];
$metid = $_POST['metid'];
$estuf = $_POST['estuf'];
$ciclo = $_POST['pneciclo'];
$selapresentacao = $_POST['selapresentacao'];
$graficoPdf = $_POST['graficoPdf'];

$html = '<div class="container">
<button class="btn btn-success btn-lg btn-block  pull-right" style="margin: 15px 0px 15px 15px; width: 30%" type="button" onclick="demoFromHTML();"><span class="glyphicon glyphicon-file"></span> Baixar em PDF</button>
<div id="customers">
<h3 style="margin: 80px 0px 15px 15px; width: 30%">RELATÓRIO SITUACIONAL</h3>';

$metid = $_POST['metid'];
$sql = "select metchamada, mettitulo from sase.meta where metid = {$metid}";
$res = $db->carregar($sql);
$html .= " <div  style='margin-bottom: 5px;font-size: 19px;font-weight: bold; color: #1f6b9f;' class=\"well\">";
$html .= "META " . $res[0]['metchamada'];
$html .= " </div>";
$html .= " <div  style='font-size: 16px'>";
$html .= $res[0]['mettitulo'];
$html .= " </div><br>";
$subid = $_POST['subid'];

$sql = "select subtitulo from sase.submetadial where subiddial = {$subid} and substatus = 'A'";
$res = $db->carregar($sql);
$html .= " <div  style='font-size: 14px'>";
$html .= $res[0]['subtitulo'];
$html .= " </div>";

$data = array();

$local = "";
$linhadeBase = "'Linha de Base 2014',";
$ciclo = "'1° ciclo 2016',";
$metaPrevista = "'Meta Prevista',";
$dataConsonancia = [];

foreach ($graficoPdf as $item) {

    $indice = ( (!empty($item['uf_municipio'])) ? $item['uf_municipio'] : 'Brasil');

    $totalPar[$indice] += (!empty($item['pnevalormeta']) ? 1 : 0);
    $totalMunicipios[$indice] += (!empty($item['estdescricao']) ? 1 : 0);
    $totalMetascomtempladas[$indice] += (($item['meta'] == 'positivo')  ? 1 : 0);
    $totalPercentual[$indice] += (($item['totalporcentagem'] == 'positivo')  ? 1 : 0);
    $totalPrazo[$indice] += (($item['totalPrazo'] == 'positivo') ? 1 : 0);


    $data[] = array(
        'valor' => number_format($item['linhabase2014'], 2, ',', '.'),
        'descricao' => $item['estdescricao'],
        'name' => $item['estdescricao'],
        'categoria' => 'Linha de Base 2014'
    );


    $data[] = array(
        'valor' => ((empty($item['pnevalor'])) ? '0.00' : number_format($item['pnevalor'], 2, ',', '.')),
        'descricao' => $item['estdescricao'],
        'name' => $item['estdescricao'],
        'categoria' => '1° ciclo 2016'
    );
    $data[] = array(
        'valor' => $item['pnevalormeta'],
        'descricao' => $item['estdescricao'],
        'name' => $item['estdescricao'],
        'categoria' => 'Meta Prevista'
    );
}

$local = substr_replace($local, '', -1);
$linhadeBase = substr_replace($linhadeBase, '', -1);
$ciclo = substr_replace($ciclo, '', -1);
$metaPrevista = substr_replace($metaPrevista, '', -1);


$cor = "#7CB5EC #434348 #90ED7D #F7A35C #8085E9 #F15C80 #E4D354 #2B908F #F45B5B #91E8E1 #7CB5EC #434348 #90ED7D #F7A35C #8085E9 #F15C80 #E4D354 #2B908F #F45B5B #91E8E1 #7CB5EC #434348 #90ED7D #F7A35C #8085E9 #F15C80 #E4D354 #2B908F #F45B5B #91E8E1 #7CB5EC #434348 #90ED7D #F7A35C #8085E9 #F15C80 #E4D354 #2B908F #F45B5B #91E8E1 #7CB5EC #434348 #90ED7D #F7A35C #8085E9 #F15C80 #E4D354 #2B908F #F45B5B #91E8E1 ";
$cores = explode(" ", $cor);

$grafico = new Grafico(Grafico::K_TIPO_COLUNA_IMAGEM, true);
$grafico->setWidth('60%');
$grafico->setSubtitulo('Gráfico de Evolução');
//$grafico->setSubtitulo('N° de Estados');
$numgrafic = 0;
$html .= '<div style="width: 100%; text-align: center"  class="center">
            <div>
                ' . $grafico->gerarGrafico($data, $numgrafic) . '
                <img id="chart_' . $numgrafic . '"  />
            
            </div>
        </div>
        
        ';

$numgrafic++;
$html .= '
<div style="width: 100%;"  class="center">
    <center>
        <table class="table table-bordered" style="width: 80%;text-align: center">
            <thead>
            <tr>
                <th></th>
                <th>Linha de Base 2014</th>
                <th>1° ciclo 2016</th>
                <th>Meta Prevista</th>
            </tr>
            </thead>
            <tbody>';


$i = 0;
foreach ($graficoPdf as $index => $item) {
    if (empty($item['pnevalormeta'])) {
        $item['pnevalormeta'] = 0.00;
    }
    if (empty($item['linhabase2014'])) {
        $item['linhabase2014'] = 0.00;
    }


    $html .= '<tr>
                <td scope="row" style="text-align: left"><span style="width: 15px ;height: 15px;position: absolute ; background-color: ' . $cores[$i] . ';"></span> <span style="margin-left: 20px">' . $item['estdescricao'] . '</span></td>
                <td>' . number_format($item['linhabase2014'], 2, '.', ',') . '</td>
                 <td>' . ( (empty($item['pnevalor'])) ? '0.00' : number_format($item['pnevalor'], 2, '.', ',') ). '</td>
                <td>' . number_format($item['pnevalormeta'], 2, '.', ',') . '</td>
            </tr>';
    $i++;
}
$html .= '
                
            </tbody>
        </table>
    </center>



';
$grafico->setWidth('60%');


$numgrafic++;

foreach ($estuf as $item) {
    unset($dataConsonancia);
    $dataConsonancia = [];
    if ( $selapresentacao == 'M'){
        if($metid == 11 || $metid == 13 || $metid == 14 || ($metid >= 17)){
            $dataConsonancia[] = array(
                    'valor' => 0,
                    'descricao' => 'Não existem registros para serem apresentados',
                    'name' => 'Não existem registros para serem apresentados ',
                    'categoria' => ' ',

                );
        }else{
            if( ($metid <= 10) || ($metid == 12) || ($metid == 15) || ($metid == 16) ){
                $dataConsonancia[] = array(
                    'valor' => $totalMunicipios[$item],
                    'descricao' => 'Total de Municípios ',
                    'name' => 'Total de Municípios ',
                    'categoria' => ' ',

                );

                $dataConsonancia[] = array(
                    'valor' => $totalPar[$item],
                    'descricao' => 'Total de Municípios que respoderam o PAR',
                    'name' => 'Total de Municípios que respoderam o PAR',
                    'categoria' => ' ',
                );

                $dataConsonancia[] = array(
                    'valor' => $totalMetascomtempladas[$item],
                    'descricao' => 'Total de Municípios com as metas comtempladas ',
                    'name' => 'Total de Municípios com as metas comtempladas ',
                    'categoria' => ' ',
                );
            }
            if( ($metid < 6) ){
                $dataConsonancia[] = array(
                    'valor' => $totalPercentual[$item],
                    'descricao' => 'Total de Municípios com percentual consonante ',
                    'name' => 'Total de Municípios com percentual consonante ',
                    'categoria' => ' ',

                );

                $dataConsonancia[] = array(
                    'valor' => $totalPrazo[$item],
                    'descricao' => 'Total de Municípios com prazo consonante',
                    'name' => 'Total de Municípios com prazo consonante',
                    'categoria' => ' ',

                );
            }
      
        }
    }
    $sql = "select estdescricao from territorios.estado WHERE estuf = '".$item."'";
    $subtitulo = 'Gráfico de Consonância -'.$db->pegaUm($sql);
    $grafico->setSubtitulo($subtitulo);
    $grafico->setLegenda(array('layout' => 'vertical',  'borderWidth'=>'0') );

//    if ( $selapresentacao == 'M'){
//        if(( ($metid >= 6) && ($metid <= 10)) || ($metid == 12) || ($metid == 15) || ($metid == 16)){
//        }
//    }
    $html .= '<div style="width: 100%; text-align: center"  class="center">
                <div>
                    ' . $grafico->gerarGrafico($dataConsonancia, $numgrafic) . '
                    <img id="chart_' . $numgrafic . '"   />
                
                </div>
            </div>
        <br>
        <br>
        <br>
    </div>   
        ';

    $numgrafic++;
}


$html .= " </div>";
echo $html;
?>
<script language="javascript" src="/library/bootbox/bootbox.min.js"></script>
<!--<script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.4/jquery.min.js"></script>-->
<script type="text/javascript" src="http://freakyjolly.com/demo/lib/jspdf.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jspdf-autotable/2.3.0/jspdf.plugin.autotable.js"></script>
<!--<script type="text/javascript" src="http://freakyjolly.com/demo/lib/html2canvas.js"></script>-->
<!--<link href="https://v4-alpha.getbootstrap.com/dist/css/bootstrap.min.css" rel="stylesheet">-->


<script>
    function demoFromHTML() {
        var $numgrafic = <?=$numgrafic?>;

        for (var i = 0; i < $numgrafic; i++) {
            $('#chart_' + i).attr('style', 'width:500px');
        }

        var pdf = new jsPDF('p', 'pt', 'letter');
        // source can be HTML-formatted string, or a reference
        // to an actual DOM element from which the text will be scraped.
        source = $('#customers')[0];

        // we support special element handlers. Register them with jQuery-style
        // ID selector for either ID or node name. ("#iAmID", "div", "span" etc.)
        // There is no support for any other type of selectors
        // (class, of compound) at this time.
        specialElementHandlers = {
            // element with id of "bypass" - jQuery style selector
            '#bypassme': function (element, renderer) {
                // true = "handled elsewhere, bypass text extraction"
                return true
            }
        };
        margins = {
            top: 20,
            bottom: 60,
            left: 40,
            width: 522
        };
        // all coords and widths are in jsPDF instance's declared units
        // 'inches' in this case
        pdf.fromHTML(
            source, // HTML string or DOM elem ref.
            margins.left, // x coord
            margins.top, { // y coord
                'width': margins.width, // max width of content on PDF
                'elementHandlers': specialElementHandlers
            },

            function (dispose) {
                // dispose: object with X, Y of the last line add to the PDF
                //          this allow the insertion of new lines after html
                pdf.save('Monitoramento Estado <?=date("d-m-Y")?>.pdf');
            }, margins);

        bootbox.confirm({
            title: "Fechar Gráfico",
            message: "Deseja fechar o Gráfico?",
            buttons: {
                cancel: {
                    label: '<i class="fa fa-times"></i> Não'
                },
                confirm: {
                    label: '<i class="fa fa-check"></i> Sim'
                }
            },
            callback: function (result) {

                if (result) {
                    window.close();
                }

                for (var i = 0; i < $numgrafic; i++) {

                    $('#chart_' + i).removeAttr( 'style' );
                }
            }
        });
        // setTimeout( window.close() , 3000);
    }
    /* $( document ).ready(function() {
     demoFromHTML();
     });*/
</script>

