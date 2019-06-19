<?php
namespace MecCoder\View;

trait ViewCsvTrait
{

    /**
     * Método de publicação de um arquivo para Excel
     * @param resource/array $lines pilha de linhas para a impressão
     * @param string $name nome do arquivo de saída
     */
    public function showExcel($lines, $name = null)
    {
        return $this->showCsv($lines, $name);
    }

    /**
     * Método de publicação de um arquivo para Excel
     * @param resource/array $lines pilha de linhas para a impressão
     * @param string $name nome do arquivo de saída
     */
    public function showCsv($lines, $name = null)
    {
        $name = $name ?: 'simec-par3';

        $arquivo = '/tmp/arquivo-excel-' . microtime();
        $out = fopen($arquivo, 'w');
        $l = 0;
        if (is_array($lines)) {
            foreach ($plan as $line) {
                if (!$l) {
                    fputcsv($out, array_keys($line));
                }
                if (is_array($line)) {
                    fputcsv($out, $line);
                }
                $l++;
            }
        }
        if (is_callable($lines)) {
            while (($line = $lines())) {
                if (!$l) {
                    fputcsv($out, array_keys($line));
                }
                if (is_array($line)) {
                    fputcsv($out, $line);
                }
                $l++;
            }
        }
        fclose($out);

        header("Content-Type: text/csv; charset=iso-8859-1");
        header("Content-Disposition: attachment; filename={$name}.csv");
        header("Expires: 0");
        header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
        header("Cache-Control: private", false);
        echo file_get_contents($arquivo);
        die;
    }
}
