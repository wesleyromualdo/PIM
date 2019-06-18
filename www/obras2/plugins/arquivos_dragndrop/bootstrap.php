<?php
require_once "config.inc";

date_default_timezone_set ('America/Sao_Paulo');
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
require_once APPRAIZ . 'includes/classes/modelo/public/Arquivo.class.inc';
require_once APPRAIZ . "obras2/classes/modelo/ArquivoSupervisao.class.inc";

require_once APPRAIZ . "obras2/classes/modelo/ArquivoSupervisao.class.inc";
    
define('DS', DIRECTORY_SEPARATOR);

// Set error reporting
error_reporting(0);
ini_set('display_errors', 'Off');

//error_reporting(-1);
//ini_set('display_errors', 'On');
//ini_set('html_errors', 'On');

function validaPath($path){
    $raiz    = realpath(ARQUIVOS_RAIZ);
    $raizQtd = strlen($raiz);

    $path    = substr($path, 0, $raizQtd);
    return $path === $raiz;
}

// Arruma a codificaÃ§Ã£o dos itens para a tela
function arrumaCodificacaoItens($arFilhos){
    $filhos = array();

    foreach($arFilhos as $k => $filho){
        $filhos[$k] = arrumaCodificacaoItem($filho);
    }

    return $filhos;
}

// CÃ³digo para arrumar a codificaÃ§Ã£o de um Ãºnico item
function arrumaCodificacaoItem($filho){
    $filhoCodificado = $filho;

    if(isset($filhoCodificado['data']['title']))
        $filhoCodificado['data']['title'] = ($filhoCodificado['data']['title']);

    if(isset($filhoCodificado['attr']['id']))
        $filhoCodificado['attr']['id'] = ($filhoCodificado['attr']['id']);

    if(isset($filhoCodificado['attr']['path']))
        $filhoCodificado['attr']['path'] = ($filhoCodificado['attr']['path']);

    if(isset($filhoCodificado['attr']['href']))
        $filhoCodificado['attr']['href'] = ($filhoCodificado['attr']['href']);

    return $filhoCodificado;
}

function codifica($array, $type = 'encode'){
    if($array){
        foreach($array as $k => $value){
            if(!is_array($value)){
                if($type == 'encode'){
                    $array[$k] = ($value);
                }elseif($type == 'decode'){
                    $array[$k] = ($value);
                }
            }
        }
    }

    return $array;
}

// Arruma a codificaÃ§Ã£o
$_REQUEST = codifica($_REQUEST, 'decode');