<?php

class GeraExcel
{

    public $countLinhas = 0;
    public $intLinha = 0;
    public $arrDados = array();

    // define parametros(init)
    function __construct()
    {
        global $nomeDoArquivoXls;
        $this->armazena_dados = ""; // Armazena dados para imprimir(temporario)
        $this->nomeDoArquivoXls = $nomeDoArquivoXls; // Nome do arquivo excel
        $this->ExcelStart();
    }// fim constructor

    // Monta cabecario do arquivo(tipo xls)
    function ExcelStart()
    {
        //inicio do cabecario do arquivo
        $this->armazena_dados = pack("vvvvvv", 0x809, 0x08, 0x00, 0x10, 0x0, 0x0);
    }

    // Fim do arquivo excel
    function FechaArquivo()
    {
        $this->armazena_dados .= pack("vv", 0x0A, 0x00);
    }

    // monta conteudo
    //function MontaConteudoString( $excel_linha, $excel_coluna, $value){
    function MontaConteudoString($excel_linha, $excel_coluna, $value)
    {
        $tamanho = strlen($value);
        $this->armazena_dados .= pack("v*", 0x0204, 8 + $tamanho, $excel_linha, $excel_coluna, 0x00, $tamanho);
        $this->armazena_dados .= $value;
        $this->countLinhas = $this->countLinhas + 1;
        $this->arrDados[$excel_linha][$excel_coluna] = ($value);
    }//Fim, monta Col/Lin

    function MontaConteudoNumero($row, $col, $num)
    {
        $record = 0x0203;
        $length = 0x000E;
        $xf = 0x0F;

        $header = pack("vv", $record, $length);
        $data = pack("vvv", $row, $col, $xf);
        $xl_double = pack("d", $num);
        $this->armazena_dados .= ($header . $data . $xl_double);
    }

    // Gera arquivo(xls)
    function GeraArquivo()
    {
        //Fecha arquivo(xls)
        if ($this->countLinhas <= 65536) {
            $this->FechaArquivo();
            header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
            header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
            header("Pragma: no-cache");
            header("Content-type: application/xls; name=$this->nomeDoArquivoXls" . ".xls");
            header("Content-Disposition: attachment; filename=$this->nomeDoArquivoXls" . ".xls");
            header("Content-Description: MID Gera excel");
            print  ($this->armazena_dados);
        } else {
            //xlsx
            $this->GeraArquivoXlsx();
        }
    }// fecha funcao

    function GeraArquivoXlsx()
    {
        $this->FechaArquivo();
        include "classes/xlsxwriter.class.php";

        $header = $this->arrDados[0];
        $header = array_flip($header);
        foreach ($header as $key => $cabecalho) :
            $header[$key] = 'string';
        endforeach;
        unset($this->arrDados[0]);
        $this->arrDados = array_slice($this->arrDados, 0, count($this->arrDados));

        $filename = 'OFFICE_2010_' . $this->nomeDoArquivoXls . ".xlsx";
        header('Content-disposition: attachment; filename="' . XLSXWriter::sanitize_filename($filename) . '"');
        header("Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
        header('Content-Transfer-Encoding: binary');
        header('Cache-Control: must-revalidate');
        header('Pragma: public');

        $writer = new XLSXWriter();
        $writer->setAuthor('MID Gera excel');
        $writer->writeSheet($this->arrDados, 'Sheet1', $header);
        $writer->writeToStdOut();
        $this->arrDados = array();
    }// fecha funcao
    # Fim da classe que gera excel
}
