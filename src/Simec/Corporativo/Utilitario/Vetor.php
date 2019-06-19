<?php
namespace Simec\Corporativo\Utilitario;

/**
 * Métodos auxiliadores do controller
 */
class Vetor
{

    /**
     * Retorna um array linear 'valor'=>'descricao' do array passado
     * @param array $array
     * @param string $fieldVal
     * @param string $fieldDesc
     * @return array
     */
    public static function pegarLinear($array, $fieldVal, $fieldDesc)
    {
        $res = [];
        foreach ($array as $item) {
            $res[$item[$fieldVal]] = $item[$fieldDesc];
        }
        return $res;
    }

    /**
     * Retorna um novo array com novos indices
     * @param array $array dados originais
     * @param array $fieldsFrom campos que serão trocados
     * @param array $fieldsTo novos campos
     * @return array
     */
    public static function criarComNovosIndices($array, $fieldsFrom = [], $fieldsTo = [])
    {
        if (count($fieldsFrom) != count($fieldsTo)) {
            throw new \Exception('Para trocar os indices, os vetores de campos precisam ter o mesmo tamanho.');
        }
        $res = [];
        foreach ($array as $idx => $item) {
            $newItem = [];
            foreach ($fieldsFrom as $position => $from) {
                $newItem[$fieldsTo[$position]] = $item[$from];
            }
            $res[$idx] = $newItem;
        }
        return $res;
    }
}
