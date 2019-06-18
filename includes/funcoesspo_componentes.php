<?php
/**
 * Arquivo com a definição dos componentes utilizados pelos sistemas SPO.
 *
 * $Id: funcoesspo_componentes.php 146601 2018-12-04 19:48:53Z thiagofiorenza $
 * @filesource
 */

/**
 * Executa a função cls_banco::monta_combo() para a criação de um combo, imprimindo diretamente na tela, ou retornando
 * o seu HTML.
 *
 * Se o índice 'multiple' for adicionado ao array de opções, o combo se torna um combo de múltiplas opções.
 * A opção 'strict' é uma opção especial, que faz com que a comparação do in_array de values dentro do
 * cls_banco::monta_combo() utilize a opção de strict.

 * @global cls_banco $db
 * @param string $nome O nome da combo de seleção.
 * @param string|array $dados Query ou array com com os dados a serem exibidos na combo de seleção.
 * @param mixed $valor O valor do item que deve ser selecionado na combo.
 * @param string $id O id da combo de seleção para identificação no javascript.
 * @param array $opcoes
 *      Opções adicionais da combo de seleção. Deve-se utilizar o mesmo nome de parametros de cls_banco::monta_combo().
 *      divclass => Classe para a div principal, em volta do input
 *
 * @uses cls_banco::monta_combo()
 * @return string|null
 */
function inputCombo($nome, $dados, $valor, $id, $opcoes = array()) {
    global $db;

    /* B) Deixando funcionalidade 'A' opcional. */
    if (!isset($opcoes['mantemSelecaoParaUm'])) {
        $opcoes['mantemSelecaoParaUm'] = true;
    }
    /* FIM Deixando funcionalidade 'A' opcional. */

    /* A) Deixando selecionado sempre que houver apenas uma opção */
    if (!is_array($dados) && $opcoes['mantemSelecaoParaUm']) {
        $selecionadoApenasUm = $db->carregar($dados);
        if (count($selecionadoApenasUm) == 1) {
            $valor = $selecionadoApenasUm[0]['codigo'];
            $dados = $selecionadoApenasUm;
        }
    }
    /* FIM Deixando selecionado sempre que houver apenas uma opção */

    /* Passando array como Default em caso de anulidade. */
    if ($dados == null) {
        $dados = array();
    }
    /* ----- */
    $opcoesPadrao = array(
        'titulo' => 'Selecione um item',
        'habil' => 'S',
        'acao' => null,
        'opc' => null,
        'txtdica' => '',
        'size' => '',
        'obrig' => 'N',
        'return' => false,
        'title' => null,
        'complemento' => 'style="width=100%"'
        . ($opcoes['multiple'] ? ' multiple' : '')
        . ($opcoes['titulo'] ? ' data-placeholder="' . $opcoes['titulo'] . '"' : ''),
        'classe' => 'form-control' . ($opcoes['multiple'] ? ' chosen-select-no-single' : ' chosen-select')
        . ($opcoes['__strict__'] ? ' __strict__' : ''),
        'mantemSelecaoParaUm' => true
    );

    // -- Ajustes automatico do nome para array qdo o input eh do tipo multiplo
    if ($opcoes['multiple'] && false === strpos($nome, '[]')) {
        $nome = "{$nome}[]";
    }

    // -- Extraíndo as opções solicitadas
    extract($opcoes, EXTR_OVERWRITE);
    // -- Extraíndo as opções padrão (apenas as que não foram definidas em $opcoes)
    extract($opcoesPadrao, EXTR_SKIP);

    $result = $db->monta_combo(
            $nome, $dados, $habil, $titulo, $acao, $opc, $txtdica, $size, $obrig, $id, true, $valor, $title, $complemento, $classe
    );

    // -- label do input
    $result = colocaLabel($id, $flabel, $result, $classdiv, $formatolabel);

    // -- Retorna o html do combo, se necessário
    if (key_exists('return', $opcoes) && !empty($opcoes['return'])) {
        return $result;
    }

    echo $result;
}

function colocaLabel($id, $flabel, $input, $classdiv, $formato) {
    if (empty($flabel)) {
        return $input;
    }

    if ('dual' == $formato) {
        return <<<HTML
<div class="form-group control-group {$classdiv}">
    <label class="col-lg-12 control-label" for="{$id}" style="text-align:left">{$flabel}:</label>
    <div class="col-lg-12">{$input}</div>
</div>
HTML;
    }

    return <<<HTML
<div class="form-group control-group {$classdiv}">
    <label class="col-lg-2 control-label pad-12" for="{$id}">{$flabel}:</label>
    <div class="col-lg-10">{$input}</div>
</div>
HTML;
}

/**
 * Executa a função campo_texto() definida em funcoes.inc para a criação de um campo de texto (ou um hidden)
 * imprimindo-o diretamente na tela.
 * IMPORTANTE: Não esqueça de incluir: <script type="text/javascript" src="/includes/funcoes.js"></script>
 * Opção de retorno do HTML do componente: $opcoes = array('return' => true);
 * Parâmetro adicional: flabel => Cria um label bootstrap para o input.
 *
 * @param string $nome O nome do campo de texto.
 * @param string $valor O valor de preenchimento do campo de texto.
 * @param string $id O id do campo de texto para identificação no javascript.
 * @param int $limite O limite de caracteres do campo de texto.
 * @param bool $ehMonetario
 *      Indica se o campo é do tipo monetário, neste caso, inclui a máscara '###.###.###.###,##'.
 * @param array $opcoes
 *      Opções adicionais do campo de texto. Deve-se utilizar o mesmo nome de parâmetros de funcoes.inc::monta_texto().<br />Parâmetro adicional: classe
 *      divclass => Classe para a div principal, em volta do input
 */
function inputTexto($nome, $valor, $id, $limite, $ehMonetario = true, $opcoes = array()) {
    $opcoesPadrao = array(
        'obrig' => 'N',
        'habil' => 'S',
        'label' => '', // -- atributo title
        'size' => '',
        'masc' => '',
        'hid' => 'N',
        'align' => 'left',
        'txtdica' => '',
        'acao' => 0,
        'complemento' => '',
        'evtkeyup' => '',
        'evtblur' => '',
        'arrStyle' => array(),
        'return' => false,
        'somentetexto' => false
    );

    // -- Extraíndo as opções solicitadas
    extract($opcoes, EXTR_OVERWRITE);
    // -- Extraíndo as opções padrão (apenas as que não foram definidas em $opcoes)
    extract($opcoesPadrao, EXTR_SKIP);

    if ($somentetexto == true) {
        if ($ehMonetario) {
            $valor = number_format($valor, 2, ',', '.');
        }
        $html = <<<HTML
            <p class="form-control-static" id="{$id}">{$valor}</p>
HTML;
        // -- label do input
        $html = colocaLabel($id, $flabel, $html, $classdiv, $formatolabel);
        if ($return) {
            return $html;
        }

        echo $html;
        return;
    }
    if ($ehMonetario) {
        $masc = '###.###.###.###.###,##';
    }
    if ($obrig) {
        $classe .= ' required';
    }

    if (empty($complemento)) {
        $complemento = 'id="' . $id . '" class="normal form-control ' . $classe . '"';
    } else {
        $complemento .= ' id="' . $id . '" class="normal form-control ' . $classe . '"';
    }

    $html = campo_texto(
            $nome, $obrig, $habil, $label, $size, $limite, $masc, $hid, $align, $txtdica, $acao, $complemento, $evtkeyup, $valor, $evtblur, $arrStyle
    );

    // -- label do input
    $html = colocaLabel($id, $flabel, $html, $classdiv, $formatolabel);

    if ($return) {
        return $html;
    }
    echo $html;
}

function inputTextArea($nome, $valor, $id, $limite, $opcoes = array(), $bpClass = true) {
    $opcoesPadrao = array(
        'obrig' => 'N',
        'habil' => 'S',
        'label' => '',
        'cols' => null,
        'rows' => 4,
        'funcao' => '',
        'acao' => 0,
        'txtdica' => '',
        'tab' => false,
        'title' => null,
        'width' => null,
        'id' => null,
        'somentetexto' => false
    );
    // -- Extraíndo as opções solicitadas
    extract($opcoes, EXTR_OVERWRITE);
    // -- Extraíndo as opções padrão (apenas as que não foram definidas em $opcoes)
    extract($opcoesPadrao, EXTR_SKIP);
    if ($somentetexto == true) {
        $html = <<<HTML
            <p class="form-control-static">{$valor}</p>
HTML;

        // -- label do input
        $html = colocaLabel($id, $flabel, $html, $classdiv, $formatolabel);

        if ($opcoes['return']) {
            return $html;
        }

        echo $html;
    }

    $opcoesTextarea = array();
    if (isset($opcoes['info']) && !empty($opcoes['info'])) {
        $opcoesTextarea['info'] = $opcoes['info'];
    }

    $html = campo_textarea(
            $nome, $obrig, $habil, $label, $cols, $rows, $limite, $funcao, $acao, $txtdica, $tab, $title, $valor, $width, $id, $opcoesTextarea
    );

    // -- Javascript de formatação do campo de textarea
    $html .= <<<JAVASCRIPT
<script type="text/javascript" lang="javascript">
$(document).ready(function(){
    jQuery('#{$id}').addClass('form-control').next().remove();
    jQuery('#no_{$id}').addClass('form-control').css('width', '70px').css('margin-top', '5px').next().remove();
JAVASCRIPT;
    if ($complemento && is_array($complemento)) {
        foreach ($complemento as $comp => $valor) {
            switch ($comp) {
                case 'readonly':
                case 'disabled':
                case 'required':
                    $html .= <<<JAVASCRIPT
    $('#{$id}').prop('{$comp}', true);
JAVASCRIPT;
                    break;
                default:
                    $html .= <<<JAVASCRIPT
    $('#{$id}').attr('{$comp}', '{$valor}');
JAVASCRIPT;
            }
        }
    }
    $html .= <<<JAVASCRIPT
});
</script>
JAVASCRIPT;

    // -- label do input
    $html = colocaLabel($id, $flabel, $html, $classdiv, $formatolabel);

    if ($opcoes['return']) {
        return $html;
    }

    echo $html;
}
/*
 *
 */
function inputChoices($nome, array $opcoes, $valorMarcado, $prefixoId, array $config = array()) {

    $configPadrao = array(
        'return' => false,
        'coloffset' => 'col-md-2',
        'colsize' => 'col-md-10'
    );
    
    if (isset($config['class-group'])) {
        $class_group = $config['class-group'];
    } else {
        $class_group = '';
    }
    
    if(isset($config['type'])) {
    	$type = $config['type'];
    	
    	if($type == 'checkbox') {
    		$is_array = '[]';
    	}
    } else {
    	$type = 'radio';
    }
    
    if(isset($config['habil']) && $config['habil'] == 'N') {
    	$disabled = 'disabled';
    } else {
    	$disabled = '';
    }

    // -- Extraíndo as opções solicitadas
    extract($config, EXTR_OVERWRITE);
    // -- Extraíndo as opções padrão (apenas as que não foram definidas em $opcoes)
    extract($configPadrao, EXTR_SKIP);

    $html = <<<HTML
<div class="btn-group {$opcoes['classdiv']}" data-toggle="buttons">
HTML;
    foreach ($opcoes as $label => $value) {
        $active = $checked = '';
        
        if(is_array($valorMarcado)) {
        	if(in_array($value, $valorMarcado)) {
        		$active = ' active';
        		$checked = ' checked="checked"';
        	}
        } else {
        	if ($valorMarcado == $value) {
        		$active = ' active';
        		$checked = ' checked="checked"';
        	}	
        }
        
        $html .= <<<HTML
    <label class="btn btn-default{$active} {$disabled}" style="margin-top:0">
        <input type="{$type}" name="{$nome}{$is_array}" id="{$prefixoId}{$value}"
               value="{$value}"{$checked}/> {$label}
    </label>
HTML;
    }
    $html .= <<<HTML
</div>
HTML;
    if ($flabel) {
        $retorno = <<<HTML
        <div class="form-group {$class_group}">
            <label class="control-label {$coloffset}">{$flabel}:</label>
            <div class="{$colsize}" >
                {$html}
            </div>
        </div>
HTML;
    }
    if ($return) {
        return $retorno ? $retorno : $html;
    }
    echo $retorno ? $retorno : $html;
}

/**
 * Cria inputs de data, utilizando o bootstrap-date.
 * IMPORTANTE: Esta função foi testado apenas como a classe Simec_View_Form e não é recomendado seu uso
 * isoladamente (pode não funcionar corretamente).
 *
 * @staticvar boolean $jsPrinted Indica se o js de carregamento dos scripts do calendário já foi feito.
 * @param string|array $nome Nome(s) dos inputs de data, um array no caso de serem dois campos.
 * @param string|array $valor Valor(es) dos inputs de data, um array no caso de serem dois campos.
 * @param string|array $id Id(s) dos inputs de data, um array no caso de serem dois campos.
 * @param array $opcoes Array de opções, utilize os mesmos parâmetros definidos para inputTexto.
 *      divclass => Classe para a div principal, em volta do input
 * @return string
 * @throws Exception Lança exceção caso os parâmetros $nome, $valor e $id sejam de tipos diferentes.
 *
 * @see Simec_View_Form()
 * @see inputTexto()
 */
function inputData($nome, $valor, $id, array $opcoes = array()) {
    if (is_null($valor)) {
        $valor = '';
    }
    static $jsPrinted = false;

    if (!(gettype($nome) == gettype($valor) && gettype($valor) == gettype($id))) {
        throw new Exception('Os parâmetros $nome, $valor e $id devem ser do mesmo tipo (array ou escalar).');
    }

    $idParaLabel = is_array($id) ? current($id) : $id;
    $label = '';
    if ($opcoes['flabel']) {
        $label = <<<HTML
<label class="control-label col-md-2" for="{$idParaLabel}">{$opcoes['flabel']}:</label>
HTML;
    }

    unset($opcoes['flabel']);
    $opcoes['size'] = 10;
    $opcoes['classe'] = 'datepicker';

    $inputs = array();
    if (is_array($nome)) {
        $inputs[] = inputTexto(array_shift($nome), array_shift($valor), array_shift($id), 10, false, $opcoes);
        $inputs[] = inputTexto(array_shift($nome), array_shift($valor), array_shift($id), 10, false, $opcoes);
    } else {
        $inputs[] = inputTexto($nome, $valor, $id, 10, false, $opcoes);
    }

    $html = '';
    foreach ($inputs as $input) {
        $html .= <<<HTML
    <div class="col-md-2">{$input}</div>
HTML;
    }

    $html = <<<HTML
<div class="form-group {$opcoes['classdiv']}">
    {$label}{$html}
</div>
HTML;

    if (!$jsPrinted) {
        $jsPrinted = true;
        $html .= <<<JAVASCRIPT
<script type="text/javascript" lang="JavaScript">
jQuery(document).ready(function(){
    jQuery.getScript('/planacomorc/js/moment-with-locales.js', function(data,textStatus, jqxhr){ console.log('Status arquivo moment-with-locales: ' + textStatus);}).done(
        function(){
            jQuery.getScript('/planacomorc/js/bootstrap-datetimepicker.min.js', function(data,textStatus, jqxhr){ console.log('Status arquivo bootstrap-datetimepicker: ' + textStatus);}).done(
                function(){
                    jQuery("<link/>", {
                        rel: "stylesheet",
                        type: "text/css",
                        href: "/planacomorc/css/bootstrap-datetimepicker.min.css"
                    }).appendTo("head");
                  $(".datepicker").bind().datepicker({
                        language: "pt-BR",
                        autoclose: true,
                        todayHighlight: true,
                        format: "dd/mm/yyyy"
                    });
                }
            );
        }
    );
});
</script>
JAVASCRIPT;
    }

    if ($opcoes['return']) {
        return $html;
    }

    echo $html;
}

function enviaEmailParaUsuarios() {

# inicializa sistema
    require_once "config.inc";
    include_once APPRAIZ . "includes/classes_simec.inc";
    include_once APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . "includes/envia_email_sis_geral_funcoes.inc";
    /**
     * Classe de criação de listagens.
     * @see Simec_Listagem
     */
    require_once APPRAIZ . 'includes/library/simec/Listagem.php';
    $db = new cls_banco();

    /* configurações */
    ini_set("memory_limit", "2048M");
    set_time_limit(600);


    if ($_REQUEST["enviar"] || $_REQUEST["sql"]) {
        $arDestinatariosSelecao = array();
        if (is_array($_REQUEST["arDestinatarios"])) {
            foreach ($_REQUEST["arDestinatarios"] as $indice => $stDados) {
                // Nome
                $arDestinatariosSelecao[$indice]['usunome'] = $stDados['usunome'];
                // E-mail
                $arDestinatariosSelecao[$indice]['usuemail'] = $stDados['usuemail'];
                // CPF
                $arDestinatariosSelecao[$indice]['usucpf'] = $stDados['usucpf'];
            }
        }

        if ($_REQUEST["stNomeRemetente"] && $_REQUEST["stEmailRemetente"]) {
            $remetente['usunome'] = $_REQUEST["stNomeRemetente"];
            $remetente['usuemail'] = $_REQUEST["stEmailRemetente"];
            $remetente['usucpf'] = "";
        }

        array_push($arDestinatariosSelecao, $remetente);

        $assunto = $_REQUEST["assunto"];
        $conteudo = $_REQUEST["mensagem"];

        //----------------------------------------------------------------------------------
        # envia as mensagens
        $mensagem = new EmailSistema();
        if (!$mensagem->enviar($arDestinatariosSelecao, $assunto, $conteudo, (($_SESSION["FILES"]) ? $_SESSION["FILES"] : array()), $remetente, $_SESSION["destino"], $_SESSION['emanda']['registraemail'])) {
            $db->rollback();
            echo 'Ocorreu uma falha ao enviar a mensagem.';
        } else {
            $db->commit();
            echo 'Operação efetuada com sucesso.';
        }

        die();
    }
}

function enviaEmailSelecaoUsuarios() {

# inicializa sistema
    require_once "config.inc";
    include_once APPRAIZ . "includes/classes_simec.inc";
    include_once APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . "includes/envia_email_sis_geral_funcoes.inc";
    /**
     * Classe de criação de listagens.
     * @see Simec_Listagem
     */
    require_once APPRAIZ . 'includes/library/simec/Listagem.php';
    $db = new cls_banco();

    /* configurações */
    ini_set("memory_limit", "2048M");
    set_time_limit(600);

# captura as informações submetidas
    $orgao = (integer) $_REQUEST["orgao"] > 2 ? $_REQUEST["orgao"] : null;
    $tipoEnsino = (integer) $_REQUEST["tipoensino"] ? $_REQUEST["tipoensino"] : null;
    $uo = (array) $_REQUEST["unidadeorcamentaria"];
    $ug = (integer) $_REQUEST["unidadegestora"] ? $_REQUEST["unidadegestora"] : null;
    $perfis = (array) $_REQUEST["perfil"];
    $ideb = (array) $_REQUEST["ideb"];
    $outros = $_REQUEST["pessoas"];
//$arUF = $_REQUEST["estuf"];
    $arMunicipios = $_REQUEST["municipiosUsuario"];
    $assunto = $_REQUEST["assunto"];
    $conteudo = $_REQUEST["mensagem"];
    $statusUsuario = $_REQUEST["statusUsuario"];
    $cargo = $_REQUEST["cargo"];
    $usustatus = $_REQUEST['usustatus'] ? $_REQUEST['usustatus'] : null; //parâmetro/filtro listar para status do usuário, no caso, ativo.
# identifica os destinatários
    $destinatarios = EmailSistema::identificar_destinatarios($orgao, $tipoEnsino, $uo, $ug, $perfis, $outros, $statusUsuario, $ideb, $arMunicipios, $cargo, array(), null, $usustatus);

    $arrayDestinatarios = array();

//ordenando array para tratar na lista
    foreach ($destinatarios as $key => $value) {
        $arrayDestinatarios[$key]["acao"] = $value["usucpf"];
        $arrayDestinatarios[$key]["usucpf"] = $value["usucpf"];
        $arrayDestinatarios[$key]["usunome"] = $value["usunome"];
        $arrayDestinatarios[$key]["usuemail"] = $value["usuemail"];
        $arrayDestinatarios[$key]["regcod"] = $value["regcod"];
        $arrayDestinatarios[$key]["mundescricao"] = $value["mundescricao"];
    }
    ?>
    <script language="JavaScript" src="../../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <?php if (!empty($arrayDestinatarios)): ?>
        <div class="well col-md-12">
            <form class="form-horizontal" id="formularioListaUsuarios" method="post" name="formularioListaUsuarios" enctype="multipart/form-data" action="">
                <tr>
                <input type="checkbox" name="todos" id="todos" style="margin-left:1%;" value="todos" /><b> Marcar/Desmarcar todos</b><br/>
                </tr>
                <table class='tabela' style="width:100%;"  cellpadding="3">
                    <tbody>

        <?php
        $cabecalho = array('', 'CPF', 'Nome', 'E-mail', 'Estado', 'Município');
        $listagem = new Simec_Listagem();
        $listagem->addCallbackDeCampo('acao', 'addCheckboxEmail')
                ->setCabecalho($cabecalho)
                ->setDados($arrayDestinatarios);

        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        ?>
                    </tbody>
                </table>
                <p style="font-size: 12px; font-weight: bold; margin: 5px;">Total: <?= count($arrayDestinatarios); ?></p>
                <div  style="background-color: #FFF">
                    <input type="hidden" value="<?php echo $_REQUEST["stNomeRemetente"] ?>" name="stNomeRemetente" id="stNomeRemetente" />
                    <input type="hidden" value="<?php echo $_REQUEST["stEmailRemetente"] ?>" name="stEmailRemetente" id="stEmailRemetente" />
                    <input type="hidden" value="<?php echo $_REQUEST["assunto"]; ?>" name="assunto" id="assunto" />
                    <input type="hidden" value="<?php echo str_replace('"', "'", $_REQUEST["mensagem"]) ?>" name="mensagem" id="mensagem" />
                    <input type="button" name="enviar" value="Enviar" onclick="javascript: submeterEnvioEmail()" />
                    <input type="button" value="Voltar" name="voltar" onclick="javascript: $('#envia_email_selecao').modal('hide');" />
                </div>
            </form>
        </div>
    <?php else: ?>
        <table class='tabela' style="width:100%; height: 100%" cellpadding="3" style="background-color: #FFF">
            <tbody>
                <tr>
                    <td style="text-align:center;padding:15px;background-color:#f5f5f5;">
                        Não há destinatários para os filtros indicados.<br />
                        <input type="button" value="Voltar" name="voltar" onclick="javascript: $('#envia_email_selecao').modal('hide');" />
                    </td>
                </tr>
            </tbody>
        </table>
    <?php endif; ?>

    <script language="JavaScript">
        $("#todos").change(function () {
            $("input:checkbox").prop('checked', $(this).prop("checked"));
        });
    </script>
    <?php
}

function addCheckboxEmail($id, $options) {
    $acao = <<<HTML
            <div class="make-switch switch-mini" data-on-label="X" data-off-label="-" data-off="danger">
                <input type="checkbox" class="ckboxChild" data-proid="%s" name="data[id]" value="%s,%s,%s" />
            </div>
HTML;

    $strFinal = sprintf($acao, $id, $id, $options['usunome'], $options['usuemail']);
    return $strFinal;
}

function montaItemAccordion($titulo, $id, $conteudo, $opcoes = array()) {
    $opcoesPadrao = array(
        'accordionID' => 'default',
        'aberto' => false,
        'decoracao' => 'default',
        'retorno' => false // -- Indica se o item vai ser retornado, ou escrito.
    );

    // -- Extraíndo as opções solicitadas
    extract($opcoes, EXTR_OVERWRITE);
    // -- Extraíndo as opções padrão (apenas as que não foram definidas em $opcoes)
    extract($opcoesPadrao, EXTR_SKIP);

    // -- Accordion aberto ou fechado
    $aberto = $aberto ? ' in' : '';

    $saida = <<<HTML
  <div class="panel panel-{$decoracao}">
    <div class="panel-heading">
      <h4 class="panel-title">
        <a data-toggle="collapse" data-parent="#{$accordionID}" href="#{$id}">
          {$titulo}
        </a>
      </h4>
    </div>
    <div id="{$id}" class="panel-collapse collapse{$aberto}">
HTML;

    if (0 !== strpos($conteudo, '<table')) {
        $saida .= <<<HTML
      <style type="text/css">
        .panel-body .table{margin-bottom:0!important}
      </style>
      <div class="panel-body" style="padding:0">
        {$conteudo}
      </div>
HTML;
    } else {
        $saida .= $conteudo;
    }
    $saida .= <<<HTML
    </div>
  </div>
HTML;

    if ($retorno) { // -- $opcoesPadrao
        return $saida;
    }

    echo $saida;
}

/**
 *
 * @param boolean $return - retornar a variável contendo o html, ou imprimir diretamente.
 * @param array(class, value, spClass, text, valueMin, valueMax, activeSuccess) $opcoes:
 *      $class:
 *          - progress-bar (obrigatório, default);
 *          - progress-bar-success || progress-bar-info (default) || progress-bar-warning || progress-bar-danger (escolher entre uma destas opções);
 *          - progress-bar-striped (opcional, até o momento não funciona devido a falta de algum css);
 *          - active (opcional. Não funciona devido a falta de algum script.);
 *          Ex.: 'progress-bar progress-bar-success', 'progress-bar progress-bar-success progress-bar-striped', 'progress-bar progress-bar-success progress-bar-striped active';
 *      $value: valor da barra. Qualquer valor numérico;
 *      $spClass: pode ser passado valor vazio ('') para apresentar o texto na barra com porcentagem. Default: 'sr-only';
 *      $text: texto a ser apresentado caso a $spClass esteja com valor vazio. Default: 'Completo';
 *      $valueMin: valor mínimo da barra. Default: 0;
 *      $valueMax: valor máximo da barra. Default: 100;
 *      $activeSuccess: caso true, altera classe da barra para success quando $value == $valueMax. Default: true;
 *      $activeSuccessClass: class utilizada caso activeSuccess seja true. Os valores são os mesmos do parametro $class. Default: 'progress-bar progress-bar-success';
 * @param boolean $stacked - Default: false. Caso verdadeiro, cria multiplas barras. Obs.: Somente popular as chaves (class, value, spClass, text e activeSuccess (opcional))do array de $opcoes são utilizadas. As chaves Devem ser multiplas.
 *      Ex.: outputBar(true/false,array('class' => array('progress-bar-info', 'progress-bar-info'), 'value' => array(25,30) ..., true);
 *
 * @return type
 */
function outputBar($return = false, $opcoes = array(), $stacked = false) {
    $opcoesPadrao = array(
        'class' => 'progress-bar progress-bar-info',
        'value' => 0,
        'spClass' => 'sr-only',
        'text' => 'Completo',
        'valueMin' => 0,
        'valueMax' => 100,
        'activeSuccess' => true,
        'activeSuccessClass' => 'progress-bar progress-bar-success'
    );

    // -- Extraíndo as opções solicitadas
    extract($opcoes, EXTR_OVERWRITE);
    // -- Extraíndo as opções padrão (apenas as que não foram definidas em $opcoes)
    extract($opcoesPadrao, EXTR_SKIP);
    $html = <<<HTML
        <div class="progress">
HTML;
    if (true === $stacked) {
        $size = count($class);
        $width = 100 / $size;
        for ($i = 0; $i < $size; $i++) {
            $widthIn = ($width * $value[$i]) / 100;
            if (true == $activeSuccess[$i] && $value[$i] == 100) {
                $class[$i] = $activeSuccessClass[$i];
            }
            $html.= <<<HTML
            <div class="{$class[$i]}" style="width: {$widthIn}%;">
                <span class="{$spClass[$i]}">{$value[$i]}% {$text[$i]}</span>
            </div>
HTML;
            if (($val = $width - $widthIn) != 0) {
                $html.= <<<HTML
            <div class="progress-bar progress-bar-default progress-bar-striped active" style="width: {$val}%; >
                <span class="sr-only"></span>
            </div>
HTML;
            }
        }
    } else {
        if (true === $activeSuccess && $value == $valueMax) {
            $class = $activeSuccessClass;
        }
        $html.= <<<HTML
        <div class="{$class}" role="progressbar" aria-valuenow="{$value}" aria-valuemin="{$valueMin}" aria-valuemax="{$valueMax}" style="width: {$value}%;min-width: 2em;">
            <span class="{$spClass}">{$value}% {$text}</span>
        </div>
HTML;
    }

    $html .= <<<HTML
        </div>
HTML;
    if (true === $return) {
        return $html;
    }
    echo $html;
}

/**
 * Importante: Qdo utilizar arquivos inclusos, as variáveis globais devem ser declaradas para uso, ou acessadas
 * através da variável $_GLOBALS.
 *
 * @param string $titulo
 * @param string $id
 * @param string $content Conteúdo HTML da modal, ou o nome de um arquivo para inclusão.
 * @param array $botoes
 * @param array $opcoes
 */
function bootstrapPopup($titulo, $id, $content, array $botoes = array(), array $opcoes = array()) {
    $tamanhoModal = '';
    if (isset($opcoes['tamanho'])) {
        $tamanhoModal = "modal-{$opcoes['tamanho']}";
        echo <<<HTML
<style type="text/css">
.modal-lg{width:70%!important}
</style>
HTML;

    }
    echo <<<JAVASCRIPT
<script type="text/javascript">
$(function(){
    $('.modal-dialog .chosen-container').css('width', '100%');
});
</script>
JAVASCRIPT;

    echo <<<HTML
<div class="modal fade" id="{$id}">
    <div class="modal-dialog {$tamanhoModal}">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title">{$titulo}</h4>
            </div>
            <div class="modal-body">
HTML;
    if (is_file($content)) {
        require_once $content;
    } else {
        echo $content;
    }
    echo <<<HTML
            </div>
            <div class="modal-footer">
HTML;
    foreach ($botoes as $botao) {
        switch ($botao) {
            case 'cancelar':
            case 'voltar':
                $label = ucfirst($botao);
                echo <<<HTML
                <button type="button" class="btn btn-default btn-{$botao}" data-dismiss="modal">{$label}</button>
HTML;
                break;
            case 'confirmar':
            case 'salvar':
                $label = ucfirst($botao);
                echo <<<HTML
                <button type="button" id="buttonIncluir" class="btn btn-primary btn-{$botao}">{$label}</button>
HTML;
                break;
            case 'fechar':
                echo <<<HTML
                <button type="button" class="btn btn-danger btn-fechar" data-dismiss="modal">Fechar</button>
HTML;
                break;
            case 'exportar':
                echo <<<HTML
                <button type="button" class="btn btn-danger btn-exportar" >Exportar XLS</button>
HTML;
                break;
            default:
                echo $botao;
                break;
        }
    }
    echo <<<HTML
      </div>
    </div>
  </div>
</div>
HTML;
    if (in_array('confirmar', $botoes) || in_array('salvar', $botoes)) {
        echo <<<JAVASCRIPT
<script type="text/javascript">
$('#{$id} .btn-confirmar, #{$id} .btn-salvar').click(function(){
    bootbox.confirm('Tem certeza que deseja confirmar as alterações?', function(confirm){
        confirm && $('#{$id} form').submit();
    });
});
</script>
JAVASCRIPT;
    }
}

function bootstrapPanel($titulo, $conteudo, $tipo = 'info', array $opcoes = array()) {
    if (!isset($opcoes['workflow'])) {
        $opcoes['workflow'] = "";
    }
    if (!isset($opcoes['cols'])) {
        $opcoes['cols'] = 8;
    }
    $opcoes['offset'] = (12 - $opcoes['cols']) / 2;

    echo <<<HTML
<div class="col-md-{$opcoes['cols']} col-md-offset-{$opcoes['offset']}">
    <div class="panel panel-{$tipo}">
        <div class="panel-heading">
            <h3 class="panel-title">{$titulo}</h3>
        </div>
HTML;
    if (0 === stripos(trim($conteudo), '<table')) {
        echo $conteudo;
    } else {
        echo <<<HTML
        <div class="panel-body">
            {$conteudo}
        </div>
HTML;
    }
    echo <<<HTML
    </div>
</div>
 {$opcoes['workflow']}
<br style="clear:both" />
HTML;
}

/**
 * @author Lindalberto Filho <lindalbertrvcf@gmail.com>
 * @param string $nome - Nome do input.
 * @param type $valor - Valor do input.
 * @param type $id - Id do input.
 * @param type $opcoes - Array de opções (
 *      return => 'Retornar a variável ou dar echo',
 *      complemento => 'Atributos extras',
 *      coloffset => 'Espaço à esquerda',
 *      colsize => 'Tamanho do campo',
 *      flabel => 'Nome do label'
 *      divclass => Classe para a div principal, em volta do input
 * )
 * Consultar documentação: http://www.bootstraptoggle.com/
 * @return string
 */
function inputCheckbox($nome, $valor, $id, $opcoes = array()) {

    static $jsPrinted = false;

    $opcoesPadrao = array(
        'return' => false,
        'complemento' => 'data-toggle="toggle" data-on="<span class=\'glyphicon glyphicon-ok\'></span>" data-off="<span class=\'glyphicon glyphicon-minus\'></span>" data-size="mini" data-onstyle="success"',
        'coloffset' => 'col-md-2',
        'colsize' => 'col-md-10',
        'flabel' => 'Selecionar'
    );

    // -- Extraíndo as opções solicitadas
    extract($opcoes, EXTR_OVERWRITE);
    // -- Extraíndo as opções padrão (apenas as que não foram definidas em $opcoes)
    extract($opcoesPadrao, EXTR_SKIP);
    $result = '';
    if (!$jsPrinted) {
        $jsPrinted = true;
        $result .= <<<JAVASCRIPT
<script type="text/javascript" lang="JavaScript">
    jQuery.getScript('/library/bootstrap-toggle/js/bootstrap-toggle.min.js', function(data,textStatus, jqxhr){ 
        console.log('Status arquivo bootstrap-toggle.min: ' + textStatus);}).done(
    function(){
        jQuery("<link/>", {
            rel: "stylesheet",
            type: "text/css",
            href: "/library/bootstrap-toggle/css/bootstrap-toggle.min.css"
        }).appendTo("head");
    });
</script>
JAVASCRIPT;
    }
    $title = !empty($title) ? $title.':' : '';
    $result .= <<<HTML
    <div class="form-group {$opcoes['classdiv']}">
        <label for="{$id}" class="{$coloffset} control-label pad-12">{$title}</label>
        <div class="{$colsize}">
            <div class="checkbox {$disabled}">
                <label>
                    <input type="checkbox" {$disabled} name="{$nome}" id="{$id}" value="{$valor}"{$checked} {$complemento}> {$flabel}
                </label>
            </div>
        </div>
    </div>
HTML;

    // -- Retorna o html do combo, se necessário
    if (key_exists('return', $opcoes) && !empty($opcoes['return'])) {
        return $result;
    }

    echo $result;
}
