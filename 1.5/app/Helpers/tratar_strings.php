<?php
if (!function_exists('simec_utf8_encode_recursive')) {
    function simec_utf8_encode_recursive($array)
    {
        $utf8_array = array();

        if (is_array($array)) {
            foreach ($array as $key => $val) {
                if (is_array($val))
                    $utf8_array[$key] = simec_utf8_encode_recursive($val);
                else if (is_object($val) || is_numeric($val))
                    $utf8_array[$key] = $val;
                else
                    $utf8_array[$key] = utf8_encode($val);
            }
        } else {
            $utf8_array = $array;
        }

        return $utf8_array;
    }

}

/*
 * simec_utf8_decode_recursive - Funcao utilizada para formatar decode recursivo
 */
if (!function_exists('simec_utf8_decode_recursive')) {

    function simec_utf8_decode_recursive($array)
    {
        $utf8_array = array();

        if (is_array($array)) {
            foreach ($array as $key => $val) {
                if (is_array($val))
                    $utf8_array[$key] = simec_utf8_decode_recursive($val);
                else if (is_object($val))
                    $utf8_array[$key] = $val;
                else
                    $utf8_array[$key] = utf8_decode($val);
            }
        } else {
            $utf8_array = $array;
        }

        return $utf8_array;
    }
}


/*
 * simec_json_encode - Fun��o utilizada para corrigir problema de padr�o de acentua��o
 */
if (!function_exists('simec_json_encode')) {

    function simec_json_encode($value, $options = 0)
    {
        return json_encode(simec_utf8_encode_recursive($value), $options);
    }
}

if (!function_exists('tirar_acentos')) {
    function tirar_acentos($variavel) {
        $busca= array("'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'",
            "'�'","'�'","'�'","'�'","'�'",
            "'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'",
            "'�'","'�'",
            "'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'",
            "'�'","'�'","'�'","'�'","'�'",
            "'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'","'�'",
            "'�'","'�'");
        $subst= array("A","A","A","A","A","E","E","E","E","I","I","I","I","N",
            "O","O","O","O","O","U","U","U","U","Y","C",
            "a","a","a","a","a","e","e","e","e","i","i","i","i","n",
            "o","o","o","o","o","u","u","u","u","y","c");
        $result = preg_replace($busca,$subst,$variavel);
        return $result;
    }
}
