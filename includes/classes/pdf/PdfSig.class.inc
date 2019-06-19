<?php
class PdfSig {

    public function __construct() {
        require_once APPRAIZ . 'includes/mpdf/mpdf.php';
    }

    /**
     * @description este método gera o pdf conforme o HTML informado
     *
     * @param array $Output         - Tipo de saída do PDF ("I" Imprimir na tela, "D" Força o download)
     * @param array $WriteHTML      - HTML do corpo do PDF
     * @param array $FileName       - Nome do arquivo
     * @param array $HTMLFooter     - HTML do rodapé do PDF (se necessário)
     * @param array $HTMLHeader     - HTML do Cabeçalho
     * @param array $CapaFrenteHTML - HTML da capa de frente (se necessário)
     * @param array $CapaTrasHTML   - HTML da capa de tras (se necessário)
     * @param array $CSS            - CSS do HTML
     * @param array $CapaFrente     - Capa de frente do documento
     * @param array $CapaTras       - Capa de trás do documento
     *
     * @param integer $marginHeader - Margem do Topo do corpo do PDF
     *
     * Exemplo de uso:
     *  /patrimonio/modulos/principal/localizacao/pesquisar.inc
     *
     *
     */

    public function printPdfCaderno($array, $marginHeader = 20)
    {

    	$mpdf = new mPDF('c','A4','','',10,10,$marginHeader,6,6,3);
    	//$mpdf = new mPDF('c','A4','','',0,0,0,0,0,0);
    	$mpdf->SetDisplayMode('fullpage');

    	//Aplica CSS
    	if(array_key_exists("CSS", $array)){
    		$mpdf->WriteHTML($array['CSS'],1);
    	}

    	//Capa de frente
    	if(array_key_exists("CapaFrenteHTML", $array))
    	{

    		$CapaFrenteBg = array_key_exists("CapaFrenteBg", $array)? 'background: url(\''.$array['CapaFrenteBg'].'\') top left;':'';
    		$mpdf->WriteHTML('<div class="printcapa" style="'.$CapaFrenteBg.' width:794px;height:1420px; position:absolute; padding:0 0 0 0; top:0x; left:0px;" >'.($array['CapaFrenteHTML']).'</div>');
    		$mpdf->SetHTMLHeader(($array['HTMLHeader']));
    		$mpdf->WriteHTML(("<p style='page-break-after:always;'></p>"));
    	}
    	else
    	{
    		$mpdf->SetHTMLHeader(($array['HTMLHeader']));
    	}

		$mpdf->WriteHTML(($array['WriteHTML']));
		$mpdf->SetHTMLFooter("{$array['HTMLFooter']}");

    	//Capa de tras
    	if(array_key_exists("CapaTrasHTML", $array))
    	{
    		$mpdf->SetHTMLHeader('');
    		$mpdf->SetHTMLFooter('');
    		$mpdf->WriteHTML(("<p style='page-break-after:always;'></p>"));

    		$CapaTrasBg = array_key_exists("CapaTrasBg", $array)? 'background: url(\''.$array['CapaTrasBg'].'\') top left;':'';
    		$mpdf->WriteHTML('
    						<div class="printcapaTras" style="'.$CapaTrasBg.' width:794px;height:1420px; position:absolute; padding:0 0 0 0; top:0x; left:0px;" >
    						'.($array['CapaTrasHTML'])
    				.'</div>'
    		);
    	}

    	$retorno = $mpdf->Output("{$array['FileName']}", "{$array['Output']}");

    	if (in_array($array["Output"], array("I", "D"))) {
    		die();
    	}

    	return $retorno;
    }

    /**
     * @description este método gera o pdf conforme o HTML informado
     *
     * @param array $Output     - Tipo de saída do PDF ("I" Imprimir na tela, "D" Força o download, "F" Força o upload)
     * 							  Caso a opção de saída seja "F" será retornado o arquivo, e deverá ser feito, pelo programador, o upload do arquivo no SIG,
     * 							  através do fileSimec.
     * @param array $WriteHTML  - HTML do corpo do PDF
     * @param array $FileName   - Nome do arquivo
     * @param array $HTMLFooter - HTML do rodapé do PDF (se necessário)
     * @param array $HTMLHeader - HTML do Cabeçalho
     * @param array $CSS        - CSS do HTML
     * @param array $CapaFrente - Capa de frente do documento
     * @param array $CapaTras   - Capa de trás do documento
     * @param array $pageFormat - Formato da pagina ("A4" - A4 Vertical, "A4-L" - A4 Horizontal)
     *
     *
     * @param integer $marginHeader - Margem do Topo do corpo do PDF
     *
     * Exemplo de uso:
     *  /patrimonio/modulos/principal/localizacao/pesquisar.inc
     *
     */

    public function printPdf($array, $marginHeader = 18, $orientation = 'P')
    {
        include_once(APPRAIZ . "includes/mpdf/mpdf.php");

    	$pageFormat = (boolean)$array['pageFormat']?$array['pageFormat']:'A4';

    	$mpdf = new mPDF('c',$pageFormat.'-'.$orientation,'','',10,10,$marginHeader,6,6,3);
    	$mpdf->SetDisplayMode('fullpage');

        //Aplica CSS
        if(array_key_exists("CSS", $array)){
        	$mpdf->WriteHTML($array['CSS'],1);
        }

        if($array['HTMLHeader']){
            $mpdf->SetHTMLHeader(($array['HTMLHeader']));
        }

        if(is_array($array['WriteHTML'])){

            $addPage          = 1;
            $quantidadePagina = count($array['WriteHTML']['tbody']);

            foreach($array['WriteHTML']['tbody'] as $v){

                $html  = $array['WriteHTML']['thead'];
                $html .= $v;
                $html .= $array['WriteHTML']['tfoot'];

                $mpdf->WriteHTML(($html));

                $htmlFooter  = $array['HTMLFooter'];
                $htmlFooter .= '<div style="width:100%;
                                            text-align: right;
                                            font-size: 11px;
                                            font-family: Calibri, Verdana, Arial, sans-serif;">'
                                    .$addPage.' de '.$quantidadePagina.
                                '</div>';

                $mpdf->SetHTMLFooter("{$htmlFooter}");

                if($addPage < $quantidadePagina){
                    $mpdf->AddPage();
                }

                $addPage ++;
            }
        }else{
            $mpdf->WriteHTML(($array['WriteHTML']));
            $mpdf->SetHTMLFooter("{$array['HTMLFooter']}");
        }

        $retorno = $mpdf->Output("{$array['FileName']}", "{$array['Output']}");

        if (in_array($array["Output"], array("I", "D"))) {
            die();
        }

        return $retorno;
    }

    /**
     * @description este método gera o a grid automatizada e chama a método do PDF (printPdf($array))
     *
     * @param array $thead      - Array com os nomes das colunas da tabela
     * @param array $tbody      - Array com os valores das colunas da tabela
     * @param array $width      - Array com os tamanha das colunas da tabela
     * @param array $Output     - Tipo de saída do PDF ("I" Imprimir na tela, "D" Força o download, "F" Força o upload)
     *
     * @param array $FileName   - Nome do arquivo
     * @param array $HTMLFooter - HTML do rodapé do PDF (se necessário)
     * @param array $HTMLHeader - HTML do Cabeçalho
     * @param array $maxPerPage - Quantidade máxima de registro por página
     * @param array $fontSize   - tamanho da fonte na grid
     * @param array countLinha  - Mosta ou não número do posicionamento da linha
     *
     * Exemplo
     *
     * array('thead'        => array('Nome Coluna','Nome Coluna 2'),
     *       'tbody'        => array(0=>'Valor Coluna',1=>'Valor Coluna 2'),
     *       'align'        => array(0 => 'center', 1 =>'left'),
     *       'width'        => array(0 => 50, 1 => 100),
     *       'Output'       => 'I',
     *       'FileName'     => 'relatorio.pdf',
     *       'HTMLFooter'   => '',
     *       'HTMLHeader    => cabecalhoImpresao()) ,
     *       'maxPerPage'   => 25,
     *       'fontSize'     => '13px',
     *       'countLinha'   => true,
     *       'sumColumn'    => array(0, 1), 0 não soma a coluna e 1 soma a coluna
     **/

    public function montaGridPdf($array)
    {
        $colspan        = count($array['thead']);
        $total          = count($array['tbody']);
        $contador       = 0;
        $max            = $array['maxPerPage'];
        $tabela         = array();
        $numRefLinha    = 0;
        $numPage        = (integer) ($total / $max);
        $contadorPage   = 0;
        $countLinha     = 0;

        $html  = '';
        $html .= '<table width="100%" style="font-size:'.$array["fontSize"].'; font-family: Calibri, Verdana, Arial, sans-serif;"><thead><tr>';

        if((boolean)$array['countLinha']){
            $html .= '<th style="background-color: #6F6F6F; border:1px solid #6F6F6F; color: #FFFFFF"></th>';

            $colspan = $colspan+1;
        }

        foreach($array['thead'] as $key => $th){
            $width = '';
            if((boolean)$array['width'][$key]){
                $width = 'width: '.$array['width'][$key].'px;';
            }

            $html .= '<th style="'.$width.' background-color: #6F6F6F; border:1px solid #6F6F6F; color: #FFFFFF">'.$th.'</th>';
        }

        $html .=  '</tr></thead><tbody>';

        $tabela['thead']    = $html;
        $html               = '';
        $totalColuna = array();
        foreach ($array['tbody'] as $tbody){
            $numRefLinha++;

            if($cor == 0){
                $bg = '#F4F4F4;';
                $cor = 1;
            }else{
                $bg = '#DCDCDC;';
                $cor = 0;
            }

            $html .= '<tr>';

            $countLinha++;
            if((boolean)$array['countLinha']){
                $html .='<td style="background-color: '.$bg.' border:1px solid #DCDCDC;">'.$countLinha.'</td>';
            }

            $a=0;

            foreach ($tbody as $key => $td){
                if((boolean)$array['align'][$a]){
                    $align = $array['align'][$a];
                }else{
                    $align = 'center';
                }

                $html .='<td style="background-color: '.$bg.' border:1px solid #DCDCDC; text-align: '.$align.'">'.$td.'</td>';

                if($array['sumColumn'][$a]){
                    $totalColuna[$a] += desformata_valor($td);
                }

                if($a <= count($tbody)){
                   $a ++;
                }else{
                    $a = 0;
                }
            }

            $html .= '</tr>';

            $contador++;
            if ( fmod($numRefLinha, $max) == 0 ){
                $totalPg += $max;
                $contadorPage++;
                $html       .= '<tr><td style="background-color: #6F6F6F; border:1px solid #6F6F6F; color: #FFFFFF" colspan="'.$colspan.'">Total: '.$totalPg.' de '.$total.' registro(s)</td></tr>';
                $tdbody[]   = $html;
                $html       = '';
                $contador   = 0;
            }elseif( $numPage == $contadorPage && $numRefLinha == $total ){
                if($totalColuna){
                    $html .= '<tr>';
                    if((boolean)$array['countLinha']){
                        $html .='<td style="background-color: #bebebe; border:1px solid #bebebe;"></td>';
                    }
                    foreach($array['sumColumn'] as $key => $coluna){

                        if((boolean)$array['align'][$key]){
                            $align = $array['align'][$key];
                        }else{
                            $align = 'center';
                        }

                        $html .= '<td style="background-color: #bebebe; border:1px solid #bebebe; color: #000000 ; text-align: '.$align.'">';
                        if($coluna) {
                            $html .= formata_valor($totalColuna[$key]);
                        }
                        $html .= '</td>';
                    }
                    $html .= '</tr>';
                }
                $html       .= '<tr><td style="background-color: #6F6F6F; border:1px solid #6F6F6F; color: #FFFFFF" colspan="'.$colspan.'">Total: '.$total.' de '.$total.' registro(s)</td></tr>';
                $tdbody[]   = $html;
                $html       = '';
            }
        }

        $tabela['tbody'] = $tdbody;

        $html  = '';
        //$html .= '<tr><td style="background-color: #6F6F6F; border:1px solid #6F6F6F; color: #FFFFFF" colspan="'.$colspan.'">Total de Registro: '.$total.'</td></tr>';
        $html .= '</tbody></table>';

        $tabela['tfoot'] = $html;

        $arPDF = array('Output'     => "{$array['Output']}",
                       'WriteHTML'  => $tabela,
                       'pageFormat' => ((boolean)$array['pageFormat'])?$array['pageFormat']:'A4',
                       'FileName'   => "{$array['FileName']}",
                       'HTMLFooter' => "{$array['HTMLFooter']}",
                       'HTMLHeader' => "{$array['HTMLHeader']}");

        $marginHeader = (boolean)$array['marginHeader']?$array['marginHeader']:18;

        return $this->printPdf($arPDF, $marginHeader);
    }

    public function printPdfObras($array, $marginHeader = 18, $orientation = 'P')
    {
        include_once(APPRAIZ . "includes/mpdf/mpdf.php");
        $pageFormat = (boolean)$array['pageFormat'] ? $array['pageFormat'] : 'A4';

        $mpdf = new mPDF('c', $pageFormat . '-' . $orientation, '', '', 10, 10, $marginHeader, 6, 6, 3);
        $mpdf->SetDisplayMode('fullpage');

        //Aplica CSS
        if (array_key_exists("CSS", $array)) {
            $mpdf->WriteHTML($array['CSS'], 1);
        }

        if ($array['HTMLHeader']) {
            $mpdf->SetHTMLHeader(($array['HTMLHeader']));
        }

        $html = $array['WriteHTML']['thead'];
        $html .= $array['WriteHTML']['tbody'];
        $html .= $array['WriteHTML']['tfoot'];

        $htmlFooter = '<div style="width:100%;
                                            text-align: right;
                                            font-size: 11px;
                                            font-family: Calibri, Verdana, Arial, sans-serif;">
                    {PAGENO} de {nb} </div>';

        $mpdf->SetHTMLFooter("{$htmlFooter}");
        $mpdf->WriteHTML(($html));

        $retorno = $mpdf->Output("{$array['FileName']}", "{$array['Output']}");

        if (in_array($array["Output"], array("I", "D"))) {
            die();
        }

        return $retorno;
    }
}
?>
