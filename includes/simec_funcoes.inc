<?php
/*
 * simec_htmlentities - Função utilizada para corrigir problema de padrão de acentuação
 */
function simec_htmlentities($string, $flags = null, $encoding = 'UTF-8', $double_encode = null)
{
    return htmlentities($string, $flags, $encoding, $double_encode);
}

/*
 * simec_htmlspecialchars - Função utilizada para corrigir problema de padrão de acentuação
 */
function simec_htmlspecialchars($string, $flags = ENT_COMPAT, $charset = 'UTF-8')
{
    return htmlspecialchars($string, $flags, $charset);
}

/*
 * simec_html_entity_decode - Função utilizada para corrigir problema de padrão de acentuação
 */
function simec_html_entity_decode($string, $quote_style = ENT_COMPAT, $charset = 'UTF-8')
{
    return html_entity_decode($string, $quote_style, $charset);
}

/*
 * simec_json_encode - Função utilizada para corrigir problema de padrão de acentuação
 */
function simec_json_encode($value, $options = 0)
{
    return json_encode(simec_utf8_encode_recursive($value), $options);
}

/*
 * simec_json_decode - Função utilizada para corrigir problema de padrão de acentuação
 */
function simec_json_decode($value)
{
    return json_decode(simec_utf8_decode_recursive($value));
}

/*
 * simec_get_class - Função utilizada para corrigir problema de padrão do get_class
 */
function simec_get_class($value)
{
    if (is_object($value)) {
        return get_class($value);
    } else {
        return false;
    }
}

function simec_email($remetente, $destinatario, $assunto, $conteudo, $cc = '', $cco = '', $arquivos = Array(), $param = Array())
{
    require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
    require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
    $mensagem = new PHPMailer();
    global $db;
    $mensagem->persistencia = $db;
//	$mensagem->Host         = "localhost";
    $mensagem->Host = "smtp2.mec.gov.br";
    $mensagem->Mailer = "smtp";

    /* Regras definidas pelo Cristiano (11/11/2008)*/
    //$mensagem->From = $remetente['email'];

    /* Regras redefinidas pelo Cristiano (08/10/2009)*/
    $mensagem->From = "simec@mec.gov.br";

    if (count($remetente) == 2) {
        $mensagem->AddReplyTo($remetente['email'], $remetente['nome']);
        $mensagem->FromName = $remetente['nome'];
    } elseif ($remetente != '') {
        $mensagem->FromName = is_array($remetente) ? $remetente['nome'] : "Administrador SIMEC";
    } else {
        $mensagem->FromName = "Administrador SIMEC";
    }
    /* FIM regras*/

    if (is_array($destinatario)) {
        foreach ($destinatario as $email) {
            if (is_array($email)) {
                if (isset($email['usuemail']) && isset($email['usunome'])) {
                    $mensagem->AddAddress($email['usuemail'], $email['usunome']);
                } else {
                    $mensagem->AddAddress($email);
                }
            } else {
                $mensagem->AddAddress($email);
            }
        }
    } else {
        $mensagem->AddAddress($destinatario);
    }

    /*
     * Atualizado pelo Alexandre Dourado
     * recebe um array de email
     */
    if (is_array($cc) || count($cc) > 1) {
        foreach ($cc as $email) {
            $mensagem->AddCC($email);
        }
    } else {
        $mensagem->AddCC($cc);
    }
    /*
     * FIM Atualizado pelo Alexandre Dourado
     */

    /*
     * Atualizado pelo Ruy Ferreira
     * copia oculta
     */
    if (is_array($cco)) {
        foreach ($cco as $valueCco) {
            $valueCco = trim($valueCco);
            if ($valueCco) $mensagem->AddBCC($valueCco);
        }
    } else {
        $valueCco = trim($cco);
        if ($valueCco) $mensagem->AddBCC($valueCco);
    }

    /*
     * FIM Atualizado pelo Ruy Ferreira
     */


    # Atualizado pelo Wesley Romualdo
    //anexa os arquivos
    foreach ($arquivos as $arquivo) {
        if (!file_exists($arquivo['arquivo'])) {
            continue;
        }
        $arqtipo = isset($arquivo['arqtipo']) ? $arquivo['arqtipo'] : 'application/octet-stream';
        $mensagem->AddAttachment($arquivo['arquivo'], basename($arquivo['nome']), 'base64', $arqtipo);
    }
    # Fim Atualizado pelo Wesley Romualdo

    # Configuração extra do envio de email
    foreach ($param as $attr => $value) {
        switch ($value) {
            case 'CustomHeader':
                foreach ($value as $val) {
                    $mensagem->AddCustomHeader($val);
                }
                break;
            default:
                $mensagem->$attr = $value;
                break;
        }
    }
    # FIM Configuração extra do envio de email

    $mensagem->Subject = $assunto;
    $mensagem->Body = $conteudo;
    $mensagem->IsHTML(true);
    return $mensagem->Send();

}

/*
 * simec_number_format - Função utilizada para formatar numeros
 */
function simec_number_format($number, $decimals = 2, $dec_point = ',', $thousands_sep = '.')
{
    if (!is_numeric($number)) {
        return '0,00';
    }

    $decimals = is_numeric($decimals) ? $decimals : 0;
    return number_format($number, $decimals, $dec_point, $thousands_sep);
}

/*
 * simec_utf8_decode_recursive - Função utilizada para formatar decode recursivo
 */
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
                $utf8_array[$key] = ($val);
        }
    } else {
        $utf8_array = $array;
    }

    return $utf8_array;
}

/*
 * simec_utf8_encode_recursive - Função utilizada para formatar encode recursivo
 */
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
                $utf8_array[$key] = ($val);
        }
    } else {
        $utf8_array = $array;
    }

    return $utf8_array;
}

/*
 * simec_utf8_decode_recursive - Função utilizada para formatar decode recursivo
 */
function simec_utf8_decode_recursive_and_key($array)
{
    $utf8_array = array();

    if (is_array($array)) {
        foreach ($array as $key => $val) {
            if (is_array($val))
                $utf8_array[($key)] = simec_utf8_decode_recursive_and_key($val);
            else if (is_object($val))
                $utf8_array[($key)] = $val;
            else
                $utf8_array[($key)] = ($val);
        }
    } else {
        $utf8_array = $array;
    }

    return $utf8_array;
}

/*
 * simec_utf8_encode_recursive - Função utilizada para formatar encode recursivo
 */
function simec_utf8_encode_recursive_and_key($array)
{
    $utf8_array = array();

    if (is_array($array)) {
        foreach ($array as $key => $val) {
            if (is_array($val))
                $utf8_array[($key)] = simec_utf8_encode_recursive_and_key($val);
            else if (is_object($val) || is_numeric($val))
                $utf8_array[($key)] = $val;
            else
                $utf8_array[($key)] = ($val);
        }
    } else {
        $utf8_array = $array;
    }

    return $utf8_array;
}

/*
 * simec_trim - Função utilizada para remover caracteres de uma string
 */
function simec_trim($string, $charList = ' ')
{
    if (is_array($string)) {
        $result = array();

        foreach ($string as $key => $val) {
            if (is_array($val))
                $result[$key] = simec_trim($val, $charList);
            else if (is_object($val) || is_numeric($val))
                $result = $string;
            else {
                $result[$key] = trim($val, $charList);
            }
        }

        return $result;
    } else {
        return trim($string, $charList);
    }
}

/*
 * simec_strip_tags - Função utilizada para remover caracteres especiais de uma string
 */
function simec_strip_tags($string, $allowableTags = null)
{
    if (is_string($string))
        return strip_tags($string, $allowableTags);

    return $string;
}

/*
 * simec_addslashes - Função utilizada para adicionar escape nas aspas
 */
function simec_addslashes($var)
{
    return is_array($var) ? array_map("simec_addslashes", $var) : addslashes(stripslashes($var));
}

/*
 * simec_stripslashes - Função utilizada para retirar escape nas aspas
 */
function simec_stripslashes($var)
{
    return is_array($var) ? array_map("simec_stripslashes", $var) : stripslashes($var);
}

/*
 * simec_magic_quotes - Simula automaticamente o magic_quotes
 */
function simec_magic_quotes()
{
    $_GET = simec_addslashes($_GET);
    $_POST = simec_addslashes($_POST);
    $_COOKIE = simec_addslashes($_COOKIE);
    $_REQUEST = simec_addslashes($_REQUEST);
}

/*
 * simec_gravar_estatistica - Grava as estatísticas do SIMEC
 */
function simec_gravar_estatistica($mnuid = null, $usucpf = null, $esttempoexec = null, $estsession = null, $sisid = null, $estmemusa = null, $estip = null)
{
    return true;
}

/*
 * simec_preparar_array - Prepara array para select
 */
function simec_preparar_array($dados = null, $where = null, $order = null)
{
    $array = array();

    foreach ($dados as $linha) {
        $array[$linha['codigo']] = $linha['descricao'];
    }

    return $array;
}

function simec_redirecionar($url, $tipo = null, $mensagem = null)
{
    if ($tipo || $mensagem) {
        $mensagem = $mensagem ? $mensagem : ($tipo == 'success' ? 'Operação realizada com sucesso' : 'Ocorreu um erro ao executar operação');
        $_SESSION['mensagem_carregar'] = array('type' => $tipo, 'text' => $mensagem);
    }

    ob_clean();
    header("Location: {$url}");
    die;
}

function simec_multi_in_array($value, $array)
{
    foreach ($array AS $item) {
        if (!is_array($item)) {
            if ($item == $value) {
                return true;
            }
            continue;
        }

        if (in_array($value, $item)) {
            return true;
        } else if (simec_multi_in_array($value, $item)) {
            return true;
        }
    }

    return false;
}


/**
 * Função de identificação das classes do SPO
 *
 * @param string $nomeClasse Nome da classe não inclusa, instanciada.
 */
function simec_autoload($nomeClasse)
{
    // -- root do sistema
    $path = $pathZendSimec = APPRAIZ;
    $ds = DIRECTORY_SEPARATOR;

    $componentes = explode('_', $nomeClasse);
    // -- Nome do arquivo de implementação da classe
    $arquivo = array_pop($componentes);

    // -- Modelo, classe de abstração de conexões e classes
    if (false !== strstr($nomeClasse, 'Spo_Ws_Sof_')) {
        require_once "{$path}{$ds}spo{$ds}ws{$ds}sof{$ds}{$arquivo}.php";
        return;
    }

    switch ($arquivo) {
        case 'Modelo':
            require_once("{$path}{$ds}includes{$ds}classes{$ds}Modelo.class.inc");
            return;
        case 'FilesSimec':
            require_once("{$path}{$ds}includes{$ds}classes{$ds}fileSimec.class.inc");
            return;
        case 'Grafico':
            require_once("{$path}{$ds}includes{$ds}library{$ds}simec{$ds}Grafico.php");
            return;
        case 'FlashMessage':
            require_once("{$path}{$ds}includes{$ds}library{$ds}simec{$ds}Helper{$ds}FlashMessage.php");
            return;
        case 'DML':
            require_once "{$path}{$ds}includes{$ds}library{$ds}simec{$ds}DB{$ds}{$arquivo}.php";
            return;
    }

    if (empty($componentes)) {
        $componentes = array($_SESSION['sisdiretorio']);
    }

    $modulo = strtolower(array_shift($componentes));
    switch ($modulo) {
        case 'simec':
            $path .= "includes{$ds}library{$ds}simec{$ds}";
            break;
        case 'zend':
            $path .= "zimec{$ds}library{$ds}Zend{$ds}";
            break;
        default:
            $path .= "{$modulo}{$ds}classes{$ds}";
    }

    $pathZendSimec .= "zimec{$ds}library{$ds}Simec{$ds}";
    // -- Processando os demais componentes do nome das classes
    foreach ($componentes as $_path) {
        $path .= "{$_path}{$ds}";
        $pathZendSimec .= "{$_path}{$ds}";
    }

    $caminhoPadrao = $path;
    $caminhoModel = $path . 'model' . $ds;
    $caminhoModelo = $path . 'modelo' . $ds;
    $caminhoHelper = $path . 'helper' . $ds;
    $caminhoController = $path . 'controller' . $ds;

    $paths = array($caminhoPadrao, $caminhoModel, $caminhoModelo, $caminhoHelper, $caminhoController, $pathZendSimec);
    $extensoes = array('.class.inc', '.inc', '.php');
    foreach ($paths as $path) {
        foreach ($extensoes as $extensao) {
            $file = $path . $arquivo . $extensao;
            if (is_file($file)) {
                require_once($file);
                return;
            } elseif (false !== strpos($file, "{$ds}Helper{$ds}") && is_file(str_replace("{$ds}Helper{$ds}", "{$ds}helper{$ds}", $file))) {
                // -- tratamento "model" de diretórios
                require_once(str_replace("{$ds}Helper{$ds}", "{$ds}helper{$ds}", $file));
                return;
            } elseif (false !== strpos($file, "{$ds}Modelo{$ds}") && is_file(str_replace("{$ds}Modelo{$ds}", "{$ds}modelo{$ds}", $file))) {
                // -- tratamento "model" de diretórios
                require_once(str_replace("{$ds}Modelo{$ds}", "{$ds}modelo{$ds}", $file));
                return;
            } elseif (false !== strpos($file, "{$ds}Model{$ds}") && is_file(str_replace("{$ds}Model{$ds}", "{$ds}model{$ds}", $file))) {
                // -- tratamento "model" de diretórios
                require_once(str_replace("{$ds}Model{$ds}", "{$ds}model{$ds}", $file));
                return;
            } elseif (false !== strpos($file, "{$ds}Controller{$ds}") && is_file(str_replace("{$ds}Controller{$ds}", "{$ds}controller{$ds}", $file))) {
                // -- tratamento "model" de diretórios
                require_once(str_replace("{$ds}Controller{$ds}", "{$ds}controller{$ds}", $file));
                return;
            } elseif (false !== strpos($file, "{$ds}Service{$ds}") && is_file(str_replace("{$ds}Service{$ds}", "{$ds}service{$ds}", $file))) {
                // -- tratamento "service" de diretórios
                require_once(str_replace("{$ds}Service{$ds}", "{$ds}service{$ds}", $file));
                return;
            } elseif (false !== strpos($file, "{$ds}View{$ds}") && is_file(str_replace("{$ds}View{$ds}", "{$ds}view{$ds}", $file))) {
                // -- tratamento "service" de diretórios
                require_once(str_replace("{$ds}View{$ds}", "{$ds}view{$ds}", $file));
                return;
            }
        }
    }
}

function initAutoload()
{
    $funcs = spl_autoload_functions();

    # definindo função do autoload
    spl_autoload_register('simec_autoload');

    if (is_array($funcs) && in_array('__autoload', $funcs)) {
        spl_autoload_register('__autoload');
    }
    if (is_array($funcs) && in_array('spo_autoload', $funcs)) {
        spl_autoload_register('spo_autoload');
    }
}

function simec_html2pdf($html, $nomearquivo)
{
    $content = http_build_query(array(
        'conteudoHtml' => ($html)
    ));

    $context = stream_context_create(array(
        'http' => array(
            'method' => 'POST',
            'content' => $content
        )
    ));

    $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);
    header('Content-Type: application/pdf');
    header("Content-Disposition: attachment; filename={$nomearquivo}.pdf");
    echo $contents;
}