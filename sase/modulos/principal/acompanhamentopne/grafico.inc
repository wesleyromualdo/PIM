<?php
ob_start();
include APPRAIZ . 'includes/cabecalho.inc';
global $db;

$graficoPdf = $_POST['graficoPdf'];
$subid = $_POST['subid']; //indicador
$metid = $_POST['metid']; //meta
$estuf = $_POST['estuf']; //estado
$ciclo = $_POST['pneciclo']; //ciclo
$selapresentacao = $_POST['selapresentacao'];

$html = '<div class="container">
<button class="btn btn-success btn-lg btn-block pull-right" style="margin: 15px 10px 15px 15px; width: 30%" type="button" onclick="demoFromHTML();">
    <span class="glyphicon glyphicon-file"></span> Baixar em PDF</button>
<div id="customers">
<h3 style="margin: 80px 0px 15px 15px; width: 30%"> RELATÓRIO SITUACIONAL</h3>';

$sql = "SELECT metchamada, mettitulo FROM sase.meta WHERE metid = {$metid}";
$res = $db->carregar($sql);
$html .= " <div  style='margin-bottom: 5px;font-size: 19px;font-weight: bold; color: #1f6b9f;' class=\"well\">";
$html .= "META " . $res[0]['metchamada'];
$html .= " </div>";
$html .= " <div  style='font-size: 16px'>";
$html .= $res[0]['mettitulo'];
$html .= " </div><br>";

$sql = "SELECT subtitulo FROM sase.submetadial WHERE subiddial = {$subid} AND substatus = 'A'";
$res = $db->carregar($sql);
$html .= " <div  style='font-size: 14px'>";
$html .= $res[0]['subtitulo'];
$html .= " </div>";


$local = "";
$linhadeBase = "'Linha de Base 2014',";
$ciclo = "'1° ciclo 2016',";
$metaPrevista = "'Meta Prevista',";
$dataConsonancia = [];


$totalPar = 0;
foreach ($graficoPdf as $item) {

    $totalPar += (!empty($item['pnevalormeta']) ? 1 : 0);
    $totalMetascomtempladas += (($item['meta'] == 'positivo')  ? 1 : 0);
    $totalPercentual += (($item['totalporcentagem'] == 'positivo')  ? 1 : 0);
    $totalPrazo += (($item['totalPrazo'] == 'positivo') ? 1 : 0);

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
//ver($local,$linhadeBase,$ciclo,$metaPrevista);


$cor = "#7CB5EC #434348 #90ED7D #F7A35C #8085E9 #F15C80 #E4D354 #2B908F #F45B5B #91E8E1 #7CB5EC #434348 #90ED7D #F7A35C #8085E9 #F15C80 #E4D354 #2B908F #F45B5B #91E8E1 #7CB5EC #434348 #90ED7D #F7A35C #8085E9 #F15C80 #E4D354 #2B908F #F45B5B #91E8E1 #7CB5EC #434348 #90ED7D #F7A35C #8085E9 #F15C80 #E4D354 #2B908F #F45B5B #91E8E1 #7CB5EC #434348 #90ED7D #F7A35C #8085E9 #F15C80 #E4D354 #2B908F #F45B5B #91E8E1 ";
$cores = explode(" ", $cor);
//ver($cores);
$grafico = new Grafico(Grafico::K_TIPO_COLUNA_IMAGEM, true);
$grafico->setWidth('60%');

$grafico->setSubtitulo('Gráfico de Evolução');

$numgrafic = 0;
$html .= '<div style="width: 100%; text-align: center"  class="center">
            <div>
                ' . $grafico->gerarGrafico($data, $numgrafic) . '
                <img id="chart_' . $numgrafic . '"  />
            
            </div>
        </div>
        </html:
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
                    <td scope="row" style="text-align: left">
                        <span style="width: 15px ;height: 15px;position: absolute ; background-color: ' . $cores[$i] . ';"></span> 
                        <span style="margin-left: 20px">' . $item['estdescricao'] . '</span>
                    </td>
                    <td>' . number_format($item['linhabase2014'], 2, '.', ',') . '</td>
                    <td>' . ( (empty($item['pnevalor'])) ? '0.00' : number_format($item['pnevalor'], 2, '.', ',') ). '</td>
                    <td>' . number_format($item['pnevalormeta'], 2, '.', ',') . '</td>
                </tr>';

    $i++;
}
$html .= '</tbody>
        </table>
    </center>';

$grafico->setWidth('60%');
$grafico->setSubtitulo('Gráfico de Consonância');
$grafico->setLegenda(array('layout' => 'vertical',  'borderWidth'=>'0') );

if ($selapresentacao == 'E'){
    if($metid <= 17){
        $dataConsonancia[] = array(
            'valor' => count($estuf),
            'descricao' => 'Total de Estados',
            'name' => 'Total de Estados <br>',
            'categoria' => ' ',

        );

        $dataConsonancia[] = array(
            'valor' => $totalPar,
            'descricao' => 'Total de Estados que respoderam o PAR',
            'name' => 'Total de Estados que respoderam o PAR',
            'categoria' => ' ',
        );

        $dataConsonancia[] = array(
            'valor' => $totalMetascomtempladas,
            'descricao' => 'Total de Estados com as metas comtempladas ',
            'name' => 'Total de Estados  om as metas comtempladas ',
            'categoria' => ' ',
        );
    }
    
    if( $metid <= 5 ){
        $dataConsonancia[] = array(
            'valor' => $totalPercentual,
            'descricao' => 'Total de Estados com percentual consonante ',
            'name' => 'Total de Estados com percentual consonante',
            'categoria' => ' ',

        );

        $dataConsonancia[] = array(
            'valor' => $totalPrazo,
            'descricao' => 'Total de Estados com prazo consonante',
            'name' => 'Total de Estados com prazo consonante',
            'categoria' => ' ',

        );
    }
}

$html .= '<div style="width: 100%; text-align: center"  class="center">
            <div>
                ' . $grafico->gerarGrafico($dataConsonancia, $numgrafic) . '
                <img id="chart_' . $numgrafic . '"/>
            
            </div>
        </div>
        </div>';
$numgrafic++;


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

