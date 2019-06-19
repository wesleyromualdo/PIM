<?php
/**
 * Created by PhpStorm.
 * User: victor
 * Date: 05/07/17
 * Time: 11:33
 */
require_once APPRAIZ . 'includes/library/simec/Importador.php';

class Importador_Segov extends Importador
{

    public function __construct ($model = null)
    {
        parent::__construct($model);
    }

    public function validaColuna ($linha, $key, $cLinha, $delimitador)
    {
        global $tamanhoColunas;
        $linhaChave = $cLinha + 2;

        $coluna = $this->retornaColuna($key);

        switch ($key)
        {
            case 0:
            case 1:
                if (strlen(trim($linha[$key])) != $tamanhoColunas[$key])
                {
                    throw new Exception("Esperado valor com {$tamanhoColunas[$key]} caracteres, recebeu " . strlen(trim($linha[$key])) . " - linha: {$linhaChave}, coluna: {$coluna}");
                }
                break;

            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 9:
            case 10:
            case 11:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 24:
            case 25:
            case 27:
            case 28:
                if (strlen(trim($linha[$key])) > $tamanhoColunas[$key] || !$linha[$key])
                {
                    throw new Exception("Esperado valor com até {$tamanhoColunas[$key]} caracteres, recebeu " . strlen(trim($linha[$key])) . " - linha: {$linhaChave}, coluna:  {$coluna}");
                };
                break;

            case 20:
                if (!$linha[$key] && !$linha[$key + 1] && !$linha[$key + 2] && !$linha[$key + 3])
                {
                    throw new Exception("Ao menos um dos quatro campos deve ser preenchidos: Contrato Repasse, Proposta no SICONV, Convênio SIAFI ou Nº de Referência constante na nota de empenho no SIAFI - linha: {$linhaChave}, coluna:  {$coluna}");
                }
                break;

            case 21:
            case 22:
            case 23:
                continue;
                break;

            case 12:
                if (strlen(trim($linha[$key])) > $tamanhoColunas[$key])
                {
                    throw new Exception("Esperado valor com até {$tamanhoColunas[$key]} caracteres, recebeu " . strlen(trim($linha[$key])) . " - linha: {$linhaChave}, coluna:  {$coluna}");
                };
                break;

            case 7:
            case 8:
            case 13:
            case 26:
                if (is_int((int) trim($linha[$key])) == false)
                {
                    throw new Exception("Campo deveria ser um valor numérico - linha: {$linhaChave}, coluna:  {$coluna}");
                };
                break;

            case 29:
            case 30:
            case 31:
                $possuiLetras = tratarCamposNumericoSegov($linha[$key], $delimitador);
                if ($possuiLetras === false)
                {
                    throw new Exception("Campos que contem valor R$, não podem possuir letras - linha: {$linhaChave}, coluna:  {$coluna}");
                }
                elseif ($possuiLetras == 'vazio')
                {
                    throw new Exception("Campos que contem valor R$, não podem ser vazios - linha: {$linhaChave}, coluna:  {$coluna}");
                };
                break;

            case 32:
                $possuiLetras = tratarCamposNumericoSegov($linha[$key], $delimitador);
                if ($possuiLetras === false)
                {
                    throw new Exception("Campos que contem valor R$, não podem possuir letras - linha: {$linhaChave}, coluna:  {$coluna}");
                }
                elseif ($possuiLetras == 'vazio')
                {
                    throw new Exception("Campos que contem valor R$, não podem ser vazios - linha: {$linhaChave}, coluna:  {$coluna}");
                };
                if ($linha[$key] > $linha[$key - 3])
                {
                    throw new Exception("O campo de valor autorizado não pode ser maior que o valor solicitado - linha: {$linhaChave}, coluna:  {$coluna}");
                }
                break;

            case 33:
                if (validaData($linha[$key]) == false)
                {
                    throw new Exception("Data informada inválida - linha: {$linhaChave}, coluna:  {$coluna}");
                };
                break;
        }
    }

    protected function validaLinhaCSV ($linha)
    {
        unset($this->campos[32]);
        unset($this->campos[33]);
        $this->campos[36] = $this->campos[30];
        $this->campos[37] = $this->campos[31];
        $this->campos[30] = $this->campos[34];
        $this->campos[31] = $this->campos[35];
        unset($this->campos[34]);
        unset($this->campos[35]);
        $this->campos = array_values($this->campos);
        if (count($linha) != count($this->campos))
        {
            throw new Exception('Arquivo CSV não condiz com os campos informados.');
        }
    }

    private function retornaColuna ($key, $restante = 0)
    {
        // ASCII correspondente ao caracter A
        $caracterInicial = 65;

        $saida = '';

        if (($dezena = (int) ($key / 26)) > 0)
        {
            $saida = chr($caracterInicial + ($dezena - 1));
            $key -= ((int) $dezena * 26);
        }

        return $saida . chr($caracterInicial + $key);
    }
}