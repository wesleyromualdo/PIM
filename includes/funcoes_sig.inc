<?php
/** Monta campo textarea.
 *
 * @param string | array $arrAtributos - define o atributo name ou caso se queira passar todos os atributos na forma de array
 * @param string $obrig - indica se o campo é obrigatório (S = obrigatório | N = não obrigatório)
 * @param string $habil - define se o campo será apresentado habilitado (S = habilitado | N = não habilitado)
 * @param string $label - ???
 * @param string $cols - define a quantidade de colunas do textarea
 * @param string $rows - define a quantidade de linhas do textarea
 * @param string $max - define o tamanho máximo de caracteres
 * @param string $funcao - parâmetro destinado ao acrescimo de algum evento javascript à tag (necessita ser a chamada inteira, por exemplo: onclick="funcao();")
 * @param string $acao - ???
 * @param string $txtdica - define um texto de dica, apresentado quando o usuário passa o mouse por cima do campo
 * @param boolean $tab - habilita digitação do tab ou não (true | false)
 * @param string $title - define o valor do atributo title a ser posto na tag
 * @param string $value - define o valor inicial do campo. Caso exista uma variável com o mesmo nome do atributo name setado fora do escopo da função, esse será setado automaticamente como valor inicial do campo.
 * @param array $param - ???
 * @return string
 **/
function campo_textarea_sig( $arrAtributos, $obrig = "N", $habil = "S", $label = "", $cols = 80, $rows = 5, $max = 2000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $value = null, $param = Array())
{

	if(is_array($arrAtributos)){
		if(!$arrAtributos['name']){
			echo "Favor informar o nome do campo. Ex: arrAtributos['name']=\"campo\"";
			return false;
		}
		foreach($arrAtributos as $chave => $valor){
			if($chave == "name"){
				$var = $valor;
			}
			if($chave == "obrigatorio"){
				$obrig = $valor ? "S" : "N";
			}
			if($chave == "habilitado"){

				$habil = ($valor === true || $valor == 'S') ? "S" : "N";
                   	}
			if($chave == "label"){
				if($arrAtributos['id'])
					$pre_texto = "<label for=\"{$arrAtributos['id']}\" >$valor</label>";
			}
			if($chave == "cols"){
				$cols = $valor;
			}
			if($chave == "rows"){
				$rows = $valor;
			}
			if($chave == "maximovalor"){
				$max = $valor;
			}
			if($chave == "funcao"){
				$funcao = $valor;
			}
			if($chave == "acao"){
				$acao = $valor;
			}
			if($chave == "dica"){
				$txtdica = $valor;
			}
			if($chave == "tabulacao"){
				$tab = $valor ? "S" : "N";
			}
			if($chave == "title"){
				$title = $valor;
			}
			if($chave == "value"){
				$value = $valor;
			}
			if($chave == "parametro"){
				$param = $valor;
			}
            if(strpos($chave, "data-") !== false){
                $complemento .= " $chave=\"$valor\"";
            } 

		}
	}else{
		$var = $arrAtributos;
	}
	global ${$var};

	$id    = (isset($param['id']) && !empty($param['id'])) ? $param['id'] : $var;
	$value = ($value != "" ? $value : ${$var});

	$texto = '';

	if ( $obrig == 'S' ) $class = 'obrigatorio ';

	if($title){
		$title = 'title="'.$title.'"';
	}
	// verifica se há texto de dica e cria span onde o texto ficará visível
	// ao final da função a mesma verificação é realizada para fechar o tag
	if ( $txtdica )
	{
		$texto .= '<span onmouseover="return escape( \'' . $txtdica . '\' );">';
	}
	// inicia o campo textarea
	$texto .= '<textarea  id="' . $id . '" name="' . $var . '" cols="' . $cols . '" rows="' . $rows . '" ' . $funcao . ' '  . $title . ' ' .$complemento;
	$style = ''; // contém os estilos a serem aplicados ao textarea
	// monta atributos do textarea de acordo com a habilitação ou não do campo
	if ( $habil == 'N' )
	{
		$texto .= ' readonly="readonly" disabled="disabled" ';
		$style .= 'border:none;border-left:#888888 3px solid;color:#404040;width:' . $cols . 'ex;text-align:' . $align . ';';
	}
	else
	{
		$txtdica = $txtdica ? 'return escape( \'' . htmlentities( $txtdica ) . '\' );' : '' ;
		$texto .= ' onmouseover="MouseOver( this );' . $txtdica . '"';
		$texto .= ' onfocus="MouseClick( this );" ';
		$texto .= ' onmouseout="MouseOut( this );" ';
		if ($max){
			$texto .= ' onblur="MouseBlur( this ); textCounter( this.form[\'' . $id . '\'], this.form[\'no_' . $id . '\'], ' . $max . ');" ';
		}else {
			$texto .= ' onblur="MouseBlur( this );" ';
		}

		$style .= 'width:' . $cols . 'ex;';
	}
	// habilita tab, quando o tab é habilitado o estilo é alterado para
	// uma classe onde a fonte é monoespeçada
	$keydown = '';
	if ( $tab == true )
	{
		//$keydown = 'return readTabChar( event, this );';
		$style .= 'font-family:monospace;font-size:8pt;';
	}
	// aplica estilos
	$texto .= ' style="' . $style . '" ';
	// verifica se é para habilitar checagem de tamanho do campo
	if ( $max && $habil == 'S' )
	{
		$texto .= ' onkeydown="textCounter( this.form[\'' . $id . '\'], this.form[\'no_' . $id . '\'], ' . $max . ' );' . $keydown . '" ';
		$texto .= ' onkeyup="textCounter( this.form[\'' . $id . '\'], this.form[\'no_' . $id . '\'], ' . $max . ');" ';
	}
	// finaliza criação do campo textarea
	$texto .= ' class="'.$class.'txareanormal" >' . str_replace( '&nbsp;', '&amp;nbsp;', ($value != "") ? $value : ${$var} ) . '</textarea>';
	// mostra indicador de obrigatoriedade do campo

	if ( $obrig == 'S' )$texto = $texto . obrigatorio();
	// mostra tamanho máximo do conteúdo caso haja tamanho máximo
	if ( $max && $habil=='S' )
	{
		$texto .= "<br><input readonly style=\"text-align:right;border-left:#888888 3px solid;color:#808080;\" type=\"text\" name=\"no_" . $id . "\" size=\"6\" maxlength=\"6\" value=\"". ( $max - strlen( $value ) ) . "\" class=\"CampoEstilo\"><font color=\"red\" size=\"1\" face=\"Verdana\"> máximo de caracteres</font>";
	}
	// ???
	if ( $acao )
	{
		$partes = explode( ';', $acao);
		$alvo       = $partes[0];
		$campo      = $partes[1];
		$padrao     = $partes[2];
		$tipo       = $partes[3];
		$origem     = $partes[4];
		$nome_campo = $partes[5];
		$especial   = $partes[6];
		$texto = $texto . '&nbsp;<img border="0" src="/imagens/alterar.gif" title="Editar o campo." onclick="edita_campo( ' . "'$alvo', '$campo', '$padrao', '$tipo', '$origem', '$nome_campo', '$especial'" . ' )">';
	}
	// fecha tag span do texto dica
	// olhar comentário no início da função que iniciar o tag
	if ( $txtdica )
	{
		$texto .= "</span>";
	}
	return
	'<div class="notprint">' .
	$pre_texto.$texto .
	'</div>' .
	'<div id="print_' . $var . '" class="notscreen" style="text-align: left;">' .
	${$var} .
	'</div>';
}

/**
 * Cria um campo data2 no formulário
 *
 * @param string  $dt - Define o atributo name da tag e caso o id não seja setado será o mesmo
 * @param string  $obrig - indica se o campo é obrigatório (S = obrigatório | N = não obrigatório)
 * @param string  $habil - define se o campo será apresentado habilitado (S = habilitado | N = não habilitado)
 * @param string  $label - define o valor do atributo title a ser posto na tag
 * @param string  $formata - define se o valor vindo para edição necessita ser formatado ou não (S = formata | N = não formata)
 * @param string  $txtdica - define um texto de dica, apresentado quando o usuário passa o mouse por cima do campo
 * @param string  $txtOnBlur - define o evento onblur (javascript)
 * @param string  $value - define o valor inicial do campo. Caso exista uma variável com o mesmo nome do atributo name setado fora do escopo da função, esse será setado automaticamente como valor inicial do campo.
 * @param string  $txtOnChange - define o evento onblur (javascript)
 * @param string  $classe - define uma classe do input
 * @param string  $id - define o ID do input
 * @return mixed
 */
function campo_data2_sig($dt,$obrig,$habil,$label,$formata,$txtdica='',$txtOnBlur='', $value = null,$txtOnChange='', $classe='', $id='', $arrAtributos = null )
{
	global ${$dt};
	$txtdica = $txtdica ? "return escape('". $txtdica ."');" : null;

	$title = "";

	if(!$id){
		$id = $dt;
	}

	if ($obrig == 'S'){
		$clsObrig = " obrigatorio ";
	}

	if($label)
		$title = ' title="'.$label.'" ';

	if ($formata=='S')
		$dataVal =' value="'.formata_data_sig(($value) ? $value : ${$dt}).'"';
	else
		$dataVal =' value="'.( ($value) ? $value : ${$dt} ).'"';
        
    if(is_array($arrAtributos)){
        foreach($arrAtributos as $chave => $valor){
            if(strpos($chave, "data-") !== false){
                $complemento .= " $chave=\"$valor\"";
            } 
        }
    }
	if ($habil=='S') {
		$data = '<input '.$title.' type="text" id="'.$id.'" name="'.$dt.'" '.$dataVal.' '. $complemento .'
				  size="12" style="text-align: right;" maxlength="10" value="" class="normal '.$clsObrig.$classe.'"
				  onKeyUp="this.value=mascaraglobal('."'##/##/####',this.value);".'"
				  onchange="' . $txtOnChange . '" onmouseover="MouseOver(this);'. $txtdica .'"
				  onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);"
				  onblur="validando_data(this);' . $txtOnBlur . 'MouseBlur(this);this.value=mascaraglobal(\'##/##/####\',this.value)">';
		$data.= "&nbsp;<img src=\"/includes/JsLibrary/date/displaycalendar/images/calendario.gif\" align=\"absmiddle\" border=\"0\" style=\"cursor:pointer\" title=\"Escolha uma Data\" onclick=\"displayCalendar(document.getElementById('$id'),'dd/mm/yyyy',this)\">";
	} else {
		$data = '<input '.$title.' type="text" id="'.$dt.'" name="'.$dt. '"
				  readonly="readonly" class="normal disabled'.$classe.'" '.$dataVal.' '. $complemento .'
				  size="12" style="text-align:right;BORDER-LEFT:#888888 3px solid; COLOR:#808080;" maxlength="10">';
	}

	if ($obrig == 'S')
		$data = $data . obrigatorio();

	return $data;
}

/**
 * @description cria um campo input text no formulário conforme parametros repassados.
 *
 * @param string | array $arrAtributos - define o atributo name ou caso se queira passar todos os atributos na forma de array
 * @param string $obrig - define se o campo é obrigatório (S = obrigatório | N = não obrigatório)
 * @param string $habil - define se o campo será apresentado habilitado (S = habilitado | N = não habilitado)
 * @param string $label - define o valor do atributo title a ser posto na tag
 * @param number $size - indica o tamanho do campo
 * @param number $max - indica o máximo de caracteres
 * @param string $masc - define a mascara do campo, exemplos de formato em: /simecebserh/www/includes/funcoes.js (função: mascaraglobal(mascara, valor))
 * @param string $hid - cria além do campo texto (diferenciado com o acréscimo do número 1 ao valor do atributo name) também um campo com o nome original, porém do tipo hidden (S = acrescenta o campo hidden | N = não acrescenta)
 * @param string $align - define o alinhamento dentro do campo texto (left = alinhado à esquerda "padrão" | right = alinhado à direita)
 * @param string $txtdica - define um texto de dica, apresentado quando o usuário passa o mouse por cima do campo
 * @param number $acao - cria ao lado do campo texto um icone de edição, que deve ser configurado (não costuma ser utilizado este campo)
 * @param string $complemento - adiciona à tag um texto livre, usado para definir atributos que os outros parâmetros ainda não comportam
 * @param string $evtkeyup - define o evento onKeyUp (javascript)
 * @param string $value - define o valor inicial do campo. Caso exista uma variável com o mesmo nome do atributo name setado fora do escopo da função, esse será setado automaticamente como valor inicial do campo.
 * @param string $evtblur - define o evento onblur (javascript)
 * @param array $arrStyle - define os atributos CSS a serem adicionados ao campo, o indice corresponde ao atributo e o valor ao valor do atributo
 * @return boolean|string
 */
function campo_texto_sig($arrAtributos,$obrig = "N",$habil = "S",$label = "",$size = 40,$max = 255,$masc = "",$hid = "", $align='left', $txtdica='',$acao=0, $complemento='',$evtkeyup='', $value = null, $evtblur='', $arrStyle = null, $placeholder = '', $classesAdicionais = false)
{
	if(is_array($arrAtributos)){
		if(!$arrAtributos['name']){
			echo "Favor informar o nome do campo. Ex: arrAtributos['name']=\"campo\"";
			return false;
		}

		foreach($arrAtributos as $chave => $valor){
			if($chave == "name"){
				$var = $valor;
			}
			if($chave == "obrigatorio"){
				$obrig = $valor ? "S" : "N";
			}
			if($chave == "habilitado"){
				$habil = $valor ? "S" : "N";
			}
			if($chave == "title"){
				$label = $valor;
			}
			if($chave == "label"){
				if($arrAtributos['id'])
					$pre_texto = "<label for=\"{$arrAtributos['id']}\" >$valor</label>";
			}
			if($chave == "size"){
				$size = $valor;
			}
			if($chave == "maxsize"){
				$max = $valor;
			}
			if($chave == "mascara"){
				$masc = $valor;
			}
			if($chave == "hidden"){
				$hid = $valor ? "S" : "N";
			}
			if($chave == "align"){
				$align = $valor;
			}
			if($chave == "dica"){
				$txtdica = $valor;
			}
			if($chave == "acao"){
				$acao = $valor;
			}
			if($chave == "id"){
				$complemento.= " id=\"$valor\" ";
			}
			if($chave == "onchange"){
				$complemento.= " onchange=\"$valor\" ";
			}
			if($chave == "evtkeyup"){
				$evtkeyup = $valor;
			}
			if($chave == "value"){
				$value = htmlentities($valor, ENT_QUOTES,"ISO-8859-1");
			}
			if($chave == "evtblur"){
				$evtblur = $valor;
			}
			if($chave == "style"){
				$arrStyle = $valor;
			}
            if($chave == "placeholder"){
                $placeholder = $valor;
            }
            if($chave == "classesAdicionais" || $chave == "class"){
                $classesAdicionais = $valor;
            }
            if(strpos($chave, "data-") !== false){
                $complemento .= " $chave=\"$valor\"";
            } 
		}
	}else{
		$var = $arrAtributos;
	}
	global ${$var};
	$value = $value != '' ? $value : ${$var};

	if ( $obrig == 'S' ) $class = 'obrigatorio';

	$arrStyle['text-align'] = $align;

	if(is_array($arrStyle)){
		$sty = 'style="';
		foreach($arrStyle as $chaves=>$dados){
			$sty .= $chaves.':'.$dados.';';
		}
		$sty .= '"';
	}

	if ($hid=='S') $dif = '1';

        //Alteração do código para contemplar aspas Duplas e Simples sendo impressa no value do input

        //Se tiver aspas duplas
        if( strpos($value, '"') !== false ){
            $carac_sub = '&quot;';
            if( !empty($max) ){
                $count_carac_sub = strlen($carac_sub);
                $nr_aspas = (int) substr_count($value, '"');
                $max = $max + ($nr_aspas * $count_carac_sub);
            }
            $value = str_replace('"', $carac_sub, $value);
        }
        //Se tiver aspas simples
        elseif( strpos($value, "'") !== false ){
            $carac_sub = '&apos;';
            if( !empty($max) ){
                $count_carac_sub = strlen($carac_sub);
                $nr_aspas = (int) substr_count($value, "'");
                $max = $max + ($nr_aspas * $count_carac_sub);
            }
            $value = str_replace('"', $carac_sub, $value);
        }

	$texto = '<input type="text" '.$sty.' name="'.$var.$dif.'" size="'.($size+1).'" maxlength="'.$max. '" value="'.$value.'"';

    if($placeholder != '')
    {
        $texto .= 'placeholder="'.$placeholder.'" ';
    }

	if ($masc !="")
	{
		if($evtkeyup != "")
		{
			$texto = $texto. ' onKeyUp= "this.value=mascaraglobal(\'' . $masc . '\',this.value);' . $evtkeyup . '"';
		}
		else
		{
			$texto = $texto. ' onKeyUp= "this.value=mascaraglobal(\'' . $masc . '\',this.value);"';
		}
	}
	else
	{
		if($evtkeyup != "")
			$texto = $texto. ' onKeyUp= "' . $evtkeyup . '"';
	}
	$habil != 'N' ? $class.=" normal"  :   $class="disabled";

	if ( $habil == 'N' )
	{
		if ( !$complemento )
			$texto = $texto.' readonly="readonly" style="width:'.($size+3).'ex;text-align : '.$align.';" ';
		else
			$texto = $texto.' ' . $complemento .' readonly="readonly" style="width:'.($size+3).'ex;text-align : '.$align.';" ';
		//if ($habil == 'N') $texto = $texto.' readonly ';
	}
	else
	{
		$texto .= ' onmouseover="MouseOver(this);';
		if ( $txtdica ){
			$texto .= 'return escape(\''.$txtdica.'\');';
		}
		if ( !$complemento ){
			if( $arrStyle ){
				$style = 'style="text-align : '.$align.'; width:'.($size+3).'ex;"';
			} else {
				$style = $sty;
			}
			$texto .= '" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);' . $evtblur . '" '.$style.' ';
		} else {
			$texto .= '" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);' . $evtblur . '" ' . $complemento . ' '.$style.' ';
		}
	}

	$class .= $classesAdicionais ? ' '.$classesAdicionais : '';

	$texto = $texto . " title='$label' class='$class' />";

	if ($obrig == 'S')
		$texto = $texto . obrigatorio();
	if ($hid == 'S'){
		$texto = $texto."<input type='hidden' name='".$var."' value ='".$value."'>";
	}
	if ($acao)
	{
		//  	unidade,unicod,uni,combo,ppaacao,Unidade Orçamentária Responsável, and substr(unicod,1,2) in '26'
		$partes = explode(";", $acao);
		$alvo=$partes[0];
		$campo=$partes[1];
		$padrao=$partes[2];
		$tipo=$partes[3];
		$origem=$partes[4];
		$nome_campo=$partes[5];
		$especial=$partes[6];
		$especial2=$partes[7];

		$texto .= '&nbsp;<img border="0" src="/imagens/alterar.gif" title="Editar o campo." onclick='.'"edita_campo('."'$alvo','$campo','$padrao','$tipo','$origem','$nome_campo'";
		//if ($especial)
		$texto .= ",'$especial'";
		//if ($especial2)
		$texto .= ",'$especial2'";
		$texto .= ')">';


	}
	return $pre_texto.$texto;
}

/**
 * Monta um combo especial com seleção em popups.
 *
 * Monta apenas o campo do formulário. Consulte as observações indicada
 * para cada parâmetro. É necessário que o campo identificador seja
 * renomeado para 'codigo' e a descrição para 'descricao'. Veja restante
 * dos comentário do arquivo www/geral/combopopup.php
 *
 * @param string  $nome - Nome atribuído ao campo no formulário
 * @param string  $sql - Consulta a ser realizada no banco para compor as opções no popup
 * @param string  $titulo - Texto informativo para aparecer no popup
 * @param string  $tamanho_janela - tamanho da janela popup no formato HEIGHTxWIDTH
 * @param integer $maximo_itens - define o máximo de itens que podem ser selecionados ( se igual a 0 quantidade é ilimitada )
 * @param array   $codigos_fixos - define códigos que o usuário não pode remover do combo
 * @param string  $mensagem_fixo - define a mensagem que será exibida caso o usuário tente remover um item fixo
 * @param string  $habilitado - define se o campo está aberto para edição ( S | N )
 * @param boolean $campo_busca_codigo - define se a busca pelo código estará habilitada
 * @param boolean $campo_flag_contem - se "false" oculta a opção "Não contém" do campo
 * @param integer $size - define a quantidade de linhas (tamanho vertical) do textarea
 * @param integer $width - define o tamanho horizontal do textarea
 * @param string  $onpop - define o evento onpop (javascript)
 * @param string  $onpush - define o evento onpush (javascript)
 * @param boolean $param_conexao - define a utilização de conexão com o banco de dados alternativo
 * @param array   $where ex: array( array("codigo" 	  => nomeCampoBanco,
 * 								 		  "descricao" => labelCampoFormulario,
 * 										  "tipo" 	  => 1 = inteiro ou 0 = string),
 * 						   		    array("codigo" 	  => nomeCampoBanco2,
 * 								 		  "descricao" => labelCampoFormulario2
 * 										  "tipo" 	  => 1 = inteiro ou  0 = string),
 * 								 )
 * @param string  $value - define o valor inicial do campo. Caso exista uma variável com o mesmo nome do atributo name setado fora do escopo da função, esse será setado automaticamente como valor inicial do campo.
 * @param boolean $mostraPesquisa - se "false" não mostrará os registros na popup
 * @param boolean $campo_busca_descricao - se "true" define que o campo de busca pelo código também aceite string
 * @param string  $funcaoJS - Função Javascript a ser chamada quando se escolhe uma opção da popup(onclick). É passado por parâmetro para esta função js o objeto(this) (Adicionado por: Felipe Carvalho)
 * @param string  $intervalo - se "S" será adicionado um campo chamado Selecionar intervalo onde, depois que dois registros forem clicados e acionado o botão de intervalo, todos os registros entre esse intervalo serão selecionados. ( Adicionado por: Victor Benzi )
 * @param string  $arrVisivel - Se "descricao" no popup que abre não será apresentado a coluna "código"
 * @param $arrOrdem - ???
 * @return void
 */
function combo_popup_sig( $nome, $sql, $titulo, $tamanho_janela = '400x400', $maximo_itens = 0,
$codigos_fixos = array(), $mensagem_fixo = '', $habilitado = 'S', $campo_busca_codigo = false,
$campo_flag_contem = false, $size = 10, $width = 400 , $onpop = null, $onpush = null, $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false, $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null, $class='')
{

	global ${$nome};
	unset($dados_sessao);
	// prepara parametros
	$maximo_itens = abs( (integer) $maximo_itens );
	$codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
	// prepara sessão
	$dados_sessao = array(
			//'sql' => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
			'sql' 			=> $sql,
			'titulo' 		=> $titulo,
			'indice' 		=> $indice_visivel,
			'maximo' 		=> $maximo_itens,
			'codigos_fixos' => $codigos_fixos,
			'mensagem_fixo' => $mensagem_fixo,
			'param_conexao' => $param_conexao,
			'where'			=> $where,
			'mostraPesquisa'=> $mostraPesquisa,
			'intervalo'     => $intervalo,
			'arrVisivel'    => $arrVisivel,
			'arrOrdem'      => $arrOrdem
	);

	if ( !isset( $_SESSION['indice_sessao_combo_popup'] ) )
	{
		$_SESSION['indice_sessao_combo_popup'] = array();
	}
	unset($_SESSION['indice_sessao_combo_popup'][$nome]);
	$_SESSION['indice_sessao_combo_popup'][$nome] = $dados_sessao;

	// monta html para formulario
	$tamanho    = explode( 'x', $tamanho_janela );
	$onclick    = ' onclick="javascript:combo_popup_alterar_campo_busca( this );" ';

	/*** Adiciona a função Javascript ***/
	$funcaoJS = (is_null($funcaoJS)) ? 'false' : "'" . $funcaoJS . "'";

	$ondblclick = ' ondblclick="javascript:combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' );" ';
	$ondelete   = ' onkeydown="javascript:combo_popup_remove_selecionados( event, \'' . $nome . '\' );" ';
	$onpop		= ( $onpop == null ) ? $onpop = '' : ' onpop="' . $onpop . '"';
	$onpush		= ( $onpush == null ) ? $onpush = '' : ' onpush="' . $onpush . '"';
	$habilitado_select = $habilitado == 'S' ? '' : ' disabled="disabled" ' ;
	$select =
	'<select ' .
	'maximo="'. $maximo_itens .'" tipo="combo_popup" ' .
	'multiple="multiple" size="' . $size . '" ' .
	'name="' . $nome . '[]" id="' . $nome . '" '.
	$onclick . $ondblclick . $ondelete . $onpop . $onpush  .
	'class="CampoEstilo '.($class ? $class : '').'" style="width:' . $width . 'px;" ' .
	$habilitado_select .
	'>';

	if($value && count( $value ) > 0){
		$itens_criados = 0;
		foreach ( $value as $item )
		{
			$select .= '<option value="' . $item['codigo'] . '">' . $item['descricao']  . '</option>';
			$itens_criados++;
			if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
			{
				break;
			}
		}
	} elseif ( ${$nome} && count( ${$nome} ) > 0 ) {
		$itens_criados = 0;
		if( is_array(${$nome}) ){
			foreach ( ${$nome} as $item )
			{
				$select .= '<option value="' . $item['codigo'] . '">' . $item['descricao'] . '</option>';
				$itens_criados++;
				if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
				{
					break;
				}
			}
		}
	}
	else if ( $habilitado == 'S' )
	{
		$select .= '<option value="">Duplo clique para selecionar da lista</option>';
	}
	else
	{
		$select .= '<option value="">Nenhum</option>';
	}
	$select .= '</select>';
	$buscaCodigo = '';

	#Alteração feita por wesley romualdo
	#caso a consulta não seja por descrição e sim por codigo, não permitir digitar string no campo de consulta.
	if($campo_busca_descricao == true ){
		$paramentro = "";
		$complOnblur = "";
	} else {
		$paramentro = "onkeyup=\"this.value=mascaraglobal('[#]',this.value);\"";
		$complOnblur = "this.value=mascaraglobal('[#]',this.value);";
	}

	if ( $campo_busca_codigo == true && $habilitado == 'S' )
	{
		$buscaCodigo .= '<input type="text" id="combopopup_campo_busca_' . $nome . '" onkeypress="combo_popup_keypress_buscar_codigo( event, \'' . $nome . '\', this.value );" '.$paramentro.' onmouseover="MouseOver( this );" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this); '.$complOnblur.'" class="normal" style="margin: 2px 0;" />';
		$buscaCodigo .= '&nbsp;<img title="adicionar" align="absmiddle" src="/imagens/check_p.gif" onclick="combo_popup_buscar_codigo( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value );"/>';
		$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="combo_popup_remover_item( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value, true );"/>';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>';
		$buscaCodigo .= '<br/>';
	}
	#Fim da alteração realizada por wesley romualdo

	$flagContem = '';
	if ( $campo_flag_contem == true )
	{
		$nomeFlagContemGlobal = $nome . '_campo_excludente';
		global ${$nomeFlagContemGlobal};
		$flagContem .= '<input type="checkbox" id="' . $nome . '_campo_excludente" name="' . $nome . '_campo_excludente" value="1" ' . ( ${$nomeFlagContemGlobal} ? 'checked="checked"' : '' ) . ' style="margin:0;" />';
		$flagContem .= '&nbsp;<label for="' . $nome . '_campo_excludente">Não contém</label>';
	}
	$cabecalho = '';
	if ( $buscaCodigo != '' || $flagContem != '' )
	{
		$cabecalho .= '<table width="' . $width . '" border="0" cellspacing="0" cellpadding="0"><tr>';
		$cabecalho .= '<td align="left">' . $buscaCodigo . '</td>';
		$cabecalho .= '<td align="right">' . $flagContem . '</td>';
		$cabecalho .= '</tr></table>';
	}

	print $cabecalho . $select  . (($habilitado == 'S') ? ' <img src="/imagens/pop_p.gif" style="cursor:pointer;" align="absmiddle" onclick="javascript:combo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$funcaoJS.' ); ">' : '');
}

/**
 * Monta o HTTML do combo_popup
 *
 * @param string $stDescricao - Descrição do campo
 * @param string $stNomeCampo - Nome do campo que será criado HTML/PHP
 * @param string $sql_combo - SQL para carregar as opções.
 * 						consulta a ser realizada no banco, sendo os álias (codigo,
 * 																		  descricao)
 *
 * @param string $sql_carregados - SQL para mostrar campos selecionados
 * @param string $stTextoSelecao - Texto informativo para aparecer no popup
 * @param array  $where ex: array( array("codigo" 	  => nomeCampoBanco,
 * 								 		  "descricao" => labelCampoFormulario,
 * 										  "tipo" 	  => 1 = inteiro ou 0 = string),
 * 						   		    array("codigo" 	  => nomeCampoBanco2,
 * 								 		  "descricao" => labelCampoFormulario2
 * 										  "tipo" 	  => 1 = inteiro ou  0 = string),
 * 								 )
 *
 * @param string $funcaoJS - Função Javascript a ser chamada quando se escolhe uma opção da popup(onclick). É passado por parâmetro para esta função js o objeto(this) (Adicionado por: Felipe Carvalho)
 * @param boolean $semTR - Se true, não coloca TR, o que deve ser feito manualmente. Usado para casos em que se deve setar o innerHTML ou outro atributo DOM da TR. (Adicionado por: Felipe Carvalho)
 * @param string $intervalo - se "S" será adicionado um campo chamado Selecionar intervalo onde, depois que dois registros forem clicados e acionado o botão de intervalo, todos os registros entre esse intervalo serão selecionados. ( Adicionado por: Victor Benzi )
 * @param string $arrVisivel - Se "descricao" no popup que abre não será apresentado a coluna "código"
 * @param $arrOrdem - ???
 * @param string $obrig - Define se o campo é obrigatório (S = obrigatório | N = não obrigatório)
 * @param boolean $campoContem - Se "false" oculta a opção "Não contém" do campo
 * @param boolean $campoBusca - Se "false" oculta a opção "Buscar" do campo
 * @author Cristiano Teles
 * @since 09/02/2009
 */
function mostrarComboPopup_sig( $stDescricao, $stNomeCampo, $sql_combo, $sql_carregados, $stTextoSelecao, Array $where=null, $funcaoJS=null, $semTR=false, $intervalo=false , $arrVisivel = null , $arrOrdem = null, $obrig = false, $campoContem = true, $campoBusca = true, $campoDescricao = true){
	global $db, $$stNomeCampo;
	if ( $_REQUEST[$stNomeCampo] && $_REQUEST[$stNomeCampo][0] && !empty( $sql_carregados ) ) {
		if(!is_array($_REQUEST[$stNomeCampo])){
			$_REQUEST[$stNomeCampo][0] = $_REQUEST[$stNomeCampo];
		}
		$sql_carregados = sprintf( $sql_carregados, "'" . implode( "','", $_REQUEST[$stNomeCampo] ) . "'" );
		$$stNomeCampo = $db->carregar( sprintf( $sql_combo, $sql_carregados ) );
	}
	if( !empty($sql_carregados) ){
		$$stNomeCampo = $db->carregar($sql_carregados);
	}

	if(!$semTR)
	{
		echo '<tr id="tr_'.$stNomeCampo.'">';
	}
        $stDescricao = ($campoDescricao)? $stDescricao : '';
        $classTd = ($campoDescricao)? 'width="195" class="SubTituloDireita" valign="top"' : 'style="display:none"';

	echo '<td '.$classTd.' onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );">
			' . $stDescricao . '
			<input type="hidden" id="' . $stNomeCampo . '_campo_flag" name="' . $stNomeCampo . '_campo_flag" value="' . ( empty( $$stNomeCampo ) ? '0' : '1' ) . '"/>
		</td>
		<td>';

	echo '<div id="' . $stNomeCampo . '_campo_off" style="color:#a0a0a0;';
	echo !empty( $$stNomeCampo ) ? 'display:none;' : '';
	echo '" onclick="javascript:onOffCampo( \'' . $stNomeCampo . '\' );"><img src="/imagens/combo-todos.gif" border="0" align="middle"></div>';
	echo '<div id="' . $stNomeCampo . '_campo_on" ';
	echo empty( $$stNomeCampo ) ? 'style="display:none;"' : '';
	echo '>';
	combo_popup_sig( $stNomeCampo, sprintf( $sql_combo, '' ), $stTextoSelecao, '400x400', 0, array(), '', 'S', $campoBusca, $campoContem, 10, 400, null, null, '', $where, null, true, false, $funcaoJS, $intervalo , $arrVisivel, $arrOrdem);

	if( $obrig )
	{
		echo '<img border="0" title="Indica campo obrigatório." src="/imagens/obrig.gif">';
	}

	echo '</div>
			</td>';

	if(!$semTR)	echo '</tr>';
}

/**
 * Monta um combo especial com seleção em popups.
 *
 * Monta apenas o campo do formulário. Consulte as observações indicada
 * para cada parâmetro. É necessário que o campo identificador seja
 * renomeado para 'codigo' e a descrição para 'descricao'. Veja restante
 * dos comentários do arquivo www/geral/combopopup.php
 *
 * @param string  $nome nome atribuido ao campo no formulário
 * @param string  $sql consulta a ser realizada no banco, sendo os álias (codigo,
 * 																		  descricao,
 * 																		  value)
 * 				  OBS.: O "value" é opcional, caso não seja passado o return será o "codigo".
 * 						o return fica setado em um campo "hidden", portanto o campo visto denomina-se por: "nome do campo"_dsc.
 *
 * @param string  $titulo titulo que deverá aparecer no popup
 * @param string  $tamanho_janela tamanho da janela popup no formato HEIGHTxWIDTH
 * @param integer $width
 * @param array   $where ex: array( array("codigo" 	  => nomeCampoBanco,
 * 								 		  "descricao" => labelCampoFormulario,
 * 										  "tipo" 	  => 1 = inteiro ou 0 = string),
 * 						   		    array("codigo" 	  => nomeCampoBanco2,
 * 								 		  "descricao" => labelCampoFormulario2
 * 										  "tipo" 	  => 1 = inteiro ou  0 = string),
 * 						 )
 *
 * @param array   $complemento
 * 				  Como ativar o autocomplete(exemplo: Solicitação de Diárias => evento/modulos/principal/solicitacaoDiarias.inc):
 * 				  - $complemento['class'] = 'campo_popup_autocomplete' e $complemento['whereAuto'] com o campo a ser filtrado
 * 				  Ter incluido o modulo do jQuery + Autocomplete
 * 				  - <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
 * 				  - <script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
 * 				  - <link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
 * 				  Colocar os seguintes script jQuery:
 * 					jQuery.noConflict();
 *					jQuery(document).ready(function() {
 *						jQuery('.campo_popup_autocomplete').each(function (){
 *							var nome = jQuery(this).attr('name').replace('_dsc', '');
 *							jQuery(this).autocomplete("/geral/campopopup.php?nome="+ nome +"&autocomplete=1", {
 *																						 					  	cacheLength	: 50,
 *																						 						width		: 440,
 *																						 						scrollHeight: 220,
 *																						 						delay		: 0,
 *																						 						selectFirst	: true,
 *																						 						autoFill	: false
 *																						 					});
 *					});
 *
 *					jQuery('.campo_popup_autocomplete').result(function(event, data, formatted) {
 *					     if (data) {
 *
 *					     	// Extract the data values
 *					     	var campoHidden = jQuery(this).attr('id').replace('_dsc','');
 *
 *							var descricao = data[0];
 *							var id = data[1];
 *
 *							if( id == '' ){
 *						    	jQuery(this).val('');
 *						    	return false;
 *							}
 *
 *					     	jQuery('#'+campoHidden).val( id );
 *						  	jQuery('#'+campoHidden+'_retorno').val( descricao );
 *						}
 *					});
 *
 *					jQuery('.campo_popup_autocomplete').blur(function (){
 *						jQuery(this).val(jQuery(this).val());
 *					  	var campoHidden = jQuery(this).attr('id').replace('_dsc','');
 *					  	if( jQuery(this).val() != '' && jQuery('#'+campoHidden+'_retorno').val() != '' ){
 *							if( jQuery(this).val() != jQuery('#'+campoHidden+'_retorno').val() ){
 *								jQuery(this).val('Selecione...')
 *								jQuery('#'+campoHidden).val('')
 *								jQuery('#'+campoHidden+'_retorno').val('')
 *							}
 *						}else{
 *							jQuery(this).val('Selecione...')
 *						}
 *					});
 *
 * @return void
 */

/**
 * Monta um campo text especial com seleção em popups.
 *
 * Monta apenas o campo do formulário. Consulte as observações indicada
 * para cada parâmetro. É necessário que o campo identificador seja
 * renomeado para 'codigo' e a descrição para 'descricao'. Veja restante
 * dos comentário do arquivo www/geral/combopopup.php
 *
 * @param string  $nome - Nome atribuído ao campo no formulário
 * @param string  $sql - Consulta a ser realizada no banco para compor as opções no popup
 * @param string  $titulo - Texto informativo para aparecer no popup
 * @param string  $func - define o evento onchange (javascript)
 * @param string  $tamanho_janela - tamanho da janela popup no formato HEIGHTxWIDTH
 * @param integer $width - define o tamanho horizontal do textarea
 * @param array   $where ex: array( array("codigo" 	  => nomeCampoBanco,
 * 								 		  "descricao" => labelCampoFormulario,
 * 										  "tipo" 	  => 1 = inteiro ou 0 = string),
 * 						   		    array("codigo" 	  => nomeCampoBanco2,
 * 								 		  "descricao" => labelCampoFormulario2
 * 										  "tipo" 	  => 1 = inteiro ou  0 = string),
 * 								 )
 * @param integer $typeCampo - define o tipo de campo que será apresentado (1 = campo texto | 0 = campo combobox)
 * @param boolean $limpar - habilita o icone de limpar o campo
 * @param boolean $disabled - define se o campo estará aberto para edição (false = habilitado | true = desabilitado)
 * @param boolean $return - define se a tag construída será retornada na função ou printada diretamente (true = tag retornada | false = tag printada diretamente "padrão")
 * @param string  $obrig - indica se o campo é obrigatório (S = obrigatório | N = não obrigatório)
 * @param array   $complemento - define alguns complementos previstos na função, veja o código abaixo para verificar possibilidades
 * @return void
 */
function campo_popup_sig( $nome, $sql, $titulo, $func=null, $tamanho_janela = '400x400', $width = 30, $where=null, $typeCampo=1, $limpar = true,$disabled = false, $return = false, $obrigatorio = "N", $complemento = Array() )
{
	global $db,${$nome};

	unset($dados_sessao);

	// prepara parametros
	$maximo_itens  = abs( (integer) $maximo_itens );
	$codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
	$where = $where ? $where : "";

	$ClassObrigatorio = $obrigatorio == "S" ? " obrigatorio" : "";

	// prepara sessão
	$dados_sessao = array(
			'sql'    => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
			'titulo' => $titulo,
			'indice' => $indice_visivel,
			'func'	 => $func,
			//							'maximo' => $maximo_itens,
	//							'codigos_fixos' => $codigos_fixos,
	//							'mensagem_fixo' => $mensagem_fixo,
			'where'			=> $where,
			'whereAuto'     => $complemento['whereAuto'],
			'complemento'   => $complemento
	);
	if ( !isset( $_SESSION['indice_sessao_campo_popup'] ) )
	{
		$_SESSION['indice_sessao_campo_popup'] = array();
	}
	$_SESSION['indice_sessao_campo_popup'][$nome] = $dados_sessao;
	// monta html para formulario
	$tamanho    = explode( 'x', $tamanho_janela );
	$onclick    = $disabled ? ' ' : 'onclick="javascript:campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );" ';
	//	$ondblclick = ' ondblclick="javascript:campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );" ';
	$onchange   = ' onchange="javascript:'.$func.'(this.value)" ';
	$habilitado = 'S' ;
	$buscaCodigo = '';
	$desabilitado = $disabled ? ' disabled="disabled" ' : '';
	$class = $complemento['class'];
	$readonly   = $complemento['class'] == 'campo_popup_autocomplete' ? '' : 'readonly="readonly"';
	$ondblclick = ($complemento['class'] == 'campo_popup_autocomplete') && ($onclick != '')  ? 'onclick="if(this.value==\'Selecione...\'){this.value=\'\'}"' : $onclick;
	if ($typeCampo == 1){
		
		$placeholder = ($complemento['class'] == 'campo_popup_autocomplete' ? 'placeholder="Digite para pesquisar"' : '');
		$valuePadrao = ($complemento['class'] == 'campo_popup_autocomplete' ? '' : 'Selecione...');
		
		$selectedVal = (isset(${$nome}['value']) ? ${$nome}['value'] : (isset(${$nome}['codigo']) ? ${$nome}['codigo'] : '') ) ;
		$selectedDesc = ( isset(${$nome}['descricao']) ? ${$nome}['descricao'] : $valuePadrao) . ($selectedVal ? " ( {$selectedVal} )" :"");
		
		$campo =
		'<input ' .
		'name="' . $nome.'_dsc' . '" ' .
		'id="' . $nome.'_dsc' . '" ' .
		'type = "text" ' .
		'value = "' . $selectedDesc . '" ' .
		$ondblclick.
		'class="CampoEstilo'.$ClassObrigatorio.' '.$class.'" '.
		'size="' . $width . '" ' .
		$placeholder .
		$readonly .
		$desabilitado .
		'/>';
		
		if( $complemento['class'] == 'campo_popup_autocomplete' )
		{
			$campo .=
				'<input '.
				'name="' . $nome .'_retorno' . '" ' .
				'id="' . $nome .'_retorno' . '" ' .
				'type="hidden"' .
				'value="' . $selectedDesc . '" ' .
				$habilitado_select .
				'/>';
		}

		$campo .=
			'<input '.
			'name="' . $nome . '" ' .
			'id="' . $nome . '" ' .
			'type="hidden"' .
			'value="' . $selectedVal . '" ' .
			$habilitado_select .
			'/>';

		if(!$disabled)
			$buscaCodigo .= '<img title="abrir lista" id="' . $nome . '_img" align="absmiddle" src="/imagens/seta_combo.gif" onclick="campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>';
		if  ($limpar && !$disabled){
			$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="campo_popup_remover_item( \'' . $nome . '\');'.((!empty($complemento["onclickLimpar"])&&$complemento["onclickLimpar"]!="")?$complemento["onclickLimpar"]:"").'"/>';
		}
                $buscaCodigo .= ($obrigatorio == "S" ? '&nbsp;'.obrigatorio() : "");
	}else{
		$campo ='<select ' .
				'maximo="'. $maximo_itens .'" tipo="combo_popup" ' .
				'size="1" ' .
				'name="' . $nome . '" id="' . $nome . '" '.
				$onchange .
				'class="CampoEstilo'.$ClassObrigatorio.'" style="width:' . $width . 'px;" ' .
				$habilitado_select .
				'>';
		if ($sql){
			$dados = $db->carregar($sql);
			$itens_criados = 0;

			$campo .= '<option value="" >Selecione...</option>';

			foreach ( $dados as $item ){
				$select = ${$nome} ? ${$nome} : '';
				$campo .= '<option value="' . ($item['value'] ? $item['value'] : $item['codigo']) . '">' . htmlentities( $item['descricao'] ) . '</option>';
				$itens_criados++;
			}
		}else{
			$campo .= '<option value="">Nenhum</option>';
		}
		$campo .= '</select>';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="campo_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/> '.($obrigatorio == "S" ? '&nbsp;'.obrigatorio() : "");

	}

	//	$cabecalho .= '<table width="100%" border="0" cellspacing="0" cellpadding="0"><tr>';
	//	$cabecalho .= '<td align="left">' . $campo . $buscaCodigo . '</td>';
	//	$cabecalho .= '<td align="right">' . $flagContem . '</td>';
	//	$cabecalho .= '</tr></table>';
	$cabecalho = "<span style=\"white-space: no-wrap\">".$campo.$buscaCodigo.$flagContem."</span>";

	if($return){
		return $cabecalho;
	}else{
		print $cabecalho;
	}
}

/**
 * @description: converte um valor monetário em string removendo caracteres especiais para salvar no BD
 * @param unknown $vlr
 * @return unknown
 */
function MoedaToBd_sig($vlr){
   $string = str_replace(".","",$vlr);
   $string = str_replace(",",".",$string);

   return $string;
}

/**
 * @description: Formata um cpf removendo os caracteres especiais e retorna uma String para ser salva no BD
 * @param unknown $vlr
 * @return unknown
 */
function formataCPFBySql_sig($vlr){
   $string = str_replace(".","",$vlr);
   $string = str_replace(",",".",$string);
   $string = str_replace("-","",$string);

   return $string;
}

function resumo_sig($texto,$qnt){
	$resumo=substr($texto,'0',$qnt);
    $last=strrpos($resumo," ");
    $resumo=substr($resumo,0,$last);
    return $resumo."...";
}

/**
 * Componente para um popup com uma lista ordenada.
 * @param $params array
 *
 * O parâmetros do componentes são passados por um array, sendo
 * que cada chave do array representa uma funcionalidade específica.
 * Se o array for vazio não imprime nenhum conteúdo.
 *
 * Abaixo a lista com as chaves do array que podem ser utilizadas:
 * 		-> 'nome':
 * 		-> 'valueButton': o texto(value) do botão que abre a popup. Se não for informado o padrão do texto é "Abrir".
 * 		-> 'sql': a query para os dados da lista. Deve ser informado os alias "codigo" e "descricao", para definição do
 * 				  valor(value) do checkbox e do texto que se segue, respectivamente.
 * 		-> 'titulo': título usado na popup para descrever os itens da lista. Se não for informado o padrão é "Selecione o(s) item(ns) desejado(s):".
 */
function popLista_sig( $params = array() )
{
	/*** Se algum parâmetro foi criado... ***/
	if( !empty($params) )
	{
		/*** ***/
		if( $params['nome'] )
		{
			/*** Se o SQL foi definido... ***/
			if( $params['sql'] )
			{
				/*** Joga o SQL na sessão para uso no arquivo popLista.php ***/
				$_SESSION['sql_pop_lista'][$params['nome']] = $params['sql'];
			}

			/*** Cria a variável com o value do botão. Se não houver sido informada, usa-se o padrão ***/
			$valueButton = ( $params['valueButton'] ) ? $params['valueButton'] : 'Abrir';
			/*** Cria variável com o título da popup. Se não houver sido informada, usa-se o padrão ***/
			$titulo = ( $params['titulo'] ) ? $params['titulo'] : 'Selecione o(s) item(ns) desejado(s):';
			/*** Imprime o botão que abrir a pop-up. (Arquivo: www/geral/popLista.php) ***/
			echo '<input type="button" value="'.$valueButton.'" onclick="abrePopLista(\''.urlencode($params['nome']).'\', \''.urlencode($titulo).'\');" />';
		}
	}
}

/*
 Sistema Sistema SIG
 Setor responsável: SPO/MEC
 Desenvolvedor: Desenvolvedores Simec
 Analista: Gilberto Arruda Cerqueira Xavier
 Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
 Módulo: funcoes_sig.inc
 Finalidade: reunião de várias funcoes que não dependem de banco
 Data de criação: 24/06/2005
 */

/**
 * @description: Abre uma janela popup com formulário para envio de email passando um cpf
 * @param unknown $cpf
 * @return string
 */
function emailPara_sig($cpf){
	$html = "<img src='/imagens/email.gif' title='Enviar e-mail' border='0' onclick='emailPara(\"{$cpf}\");' style='cursor:pointer;'>&nbsp;";

	return $html;
}

/**
 * Converte Inteiros em Algarismos Romanos
 * por: Werter Dias Almeida em 11/03/2009
 * @param integer valor
 * @return string
 */
function decimal2romano_sig($decimal)
{
	$romano = "";
	while ($decimal/1000 >= 1) {$romano .= "M"; $decimal = decimal-1000;}
		if ($decimal/900 >= 1) {$romano .= "CM"; $decimal=$decimal-900;}
		if ($decimal/500 >= 1) {$romano .= "D"; $decimal=$decimal-500;}
		if ($decimal/400 >= 1) {$romano .= "CD"; $decimal=$decimal-400;}
	while ($decimal/100 >= 1) {$romano .= "C"; $decimal = $decimal-100;}
		if ($decimal/90 >= 1) {$romano .= "XC"; $decimal=$decimal-90;}
		if ($decimal/50 >= 1) {$romano .= "L"; $decimal=$decimal-50;}
		if ($decimal/40 >= 1) {$romano .= "XL"; $decimal=$decimal-40;}
	while ($decimal/10 >= 1) {$romano .= "X"; $decimal = $decimal-10;}
		if ($decimal/9 >= 1) {$romano .= "IX"; $decimal=$decimal-9;}
		if ($decimal/5 >= 1) {$romano .= "V"; $decimal=$decimal-5;}
		if ($decimal/4 >= 1) {$romano .= "IV"; $decimal=$decimal-4;}
	while ($decimal >= 1) {$romano .= "I"; $decimal = $decimal-1;}

	return $romano;
}

/**
 * função que faz a verificação do CPF
 *
 * @param (string) $cpf
 * @return (true) caso o CPF seja válido
 * @return (false) caso inválido.
 * @author: FelipeChiavicatti;
 */
function validaCPF_sig($cpf) {
	/*
	*/
	$nulos = array("12345678909","11111111111","22222222222","33333333333",
	               "44444444444","55555555555","66666666666","77777777777",
	               "88888888888","99999999999","00000000000");

	/* Retira todos os caracteres que nao sejam 0-9 */
	//$cpf = ereg_replace("[^0-9]", "", $cpf);
	$cpf =  preg_replace("/[^0-9]/","", $cpf);

	if (strlen($cpf) > 11)
		return 0;

	/*Calcula o penúltimo dígito verificador*/
	$acum=0;
	for($i=0; $i<9; $i++) {
	  $acum+= $cpf[$i]*(10-$i);
	}

	$x=$acum % 11;
	$acum = ($x>1) ? (11 - $x) : 0;

	/* Retorna falso se o digito calculado eh diferente do passado na string */
	if ((string)$acum != (string)$cpf[9]){
	  return 0;
	}

	/*Calcula o último dígito verificador*/
	$acum=0;
	for ($i=0; $i<10; $i++){
	  $acum+= $cpf[$i]*(11-$i);
	}

	$x=$acum % 11;
	$acum = ($x > 1) ? (11-$x) : 0;

	/* Retorna falso se o digito calculado eh diferente do passado na string */
	if ((string) $acum != (string)$cpf[10]){
	  return 0;
	}

        if(in_array($cpf, $nulos)){
             return 0;
        }

	/* Retorna verdadeiro se o cpf eh valido */
	return true;
}


function get_rnd_iv_sig($iv_len)
{
	// gera um caracter aleatório em função de um número aleatório gerado pela função mt_rand
	$iv = '';
	while ($iv_len-- > 0) {
		$iv .= chr(mt_rand() & 0xff);
	}
	return $iv;
}

/**
 * Função que retorna true caso contenha o valor procurado em um array ou false caso não encontre
 */
function in_sig($valor, $vetor)
{
	$key = array_search($valor, $vetor);
	if ($key==null and $vetor[0]<>$valor) return false; else return true;
}

/*
 *  A função verifica se a variavel não é nula, vazia ou falsa.
 *  Retorna true  se a variavel não estiver vazia
 */
function nao_nulo_sig($valor = null)
{
    if(is_int($valor)){
        $valor = true;
    }
    if((!is_null($valor)) && (!empty($valor)) && ($valor != false)){
        return true;
    }else{
        return false;
    }
}

function formata_numero_sig($nm) {
	for ($done=strlen($nm); $done > 3;$done -= 3) {
		$returnNum = ".".substr($nm,$done-3,3).$returnNum;
	}
	return substr($nm,0,$done).$returnNum;
}

function formata_double_real_sig($nm) {
     $nm = str_replace('.',',',$nm);
      for ($done=strlen($nm); $done > 3;$done -= 3) {
		$returnNum = ".".substr($nm,$done-3,3).$returnNum;
       }
      $returnNum = substr($nm,0,$done).$returnNum;
      $returnNum = str_replace('.,',',',$returnNum);
      return $returnNum;
}

function aleatorio_sig() {
	$aleatorio = '';
	while (strlen($aleatorio) < 15)
	$aleatorio = $aleatorio.mt_rand(2000,40000);
	Return $aleatorio;
}

/**
 *
 * Verifica se o resultado da subtração de duas datas é maior que um valor de referencia($dif)
 *
 * @param unknown $dtmaior
 * @param unknown $dtmenor
 * @param unknown $dif
 * @return boolean
 */
function verifica_datas_sig($dtmaior,$dtmenor,$dif) {
	if (
	mktime (0,0,0,substr($dtmaior,3,2),substr($dtmaior,0,2),substr($dtmaior,6,4)) -
	mktime (0,0,0,substr($dtmenor,3,2), substr($dtmenor,0,2), substr($dtmenor,6,4))
	> $dif
	)
	{
		return false;
	}
	else {
		return true;
	}
}

/**
 * ajusta uma data para o formato yyyy-mm-dd
 *
 * @param unknown $data
 * @return string
 */
function ajusta_data_sig($data)
{
	// retorna a data no formato yyyy-mm-dd
	return substr($data,6,4).'-'.substr($data,3,2).'-'.substr($data,0,2);
}

function updateArquivo_sig($nome,$unicod,$codResposta){

	global  $db;
	$sql = "update reuni.resposta set rspdsc = $nome where unicod = '$unicod' and rspcod = $codResposta ";
	$db->executar($sql);
	$db->commit();
	return;
}

/**
 * ajusta uma data para o formato yyyy/mm/dd
 *
 * @param unknown $data
 * @return string
 */
function formata_data_sig($data)
{
	// retorna a data no formato yyyy-mm-dd
	$data = substr($data,0,10);
	if ($data) return substr($data,8,10).'/'.substr($data,5,2).'/'.substr($data,0,4);

	//if ($data) return strftime("%d/%m/%Y",strtotime($data));

	else return '';
}

/**
 * BR to US
 * @description retorna uma data no formato yy-mm-dd
 * @param unknown $data
 * @return string
 */
function formata_data_sql_sig($data)
{
	if ($data) return substr($data,6,4).'-'.substr($data,3,2).'-'.substr($data,0,2);
	else return '';
}

/**
 * Transforma o formato ddmmyyyy para yyyymmdd
 * @param $data
 * @return string
 */
function formata_data_string_sql_sig($data)
{
	if ($data) {
		return substr($data, 6, 4) . substr($data, 2, 2) . substr($data, 0, 2);
	} else {
		return '';
	}
}

function formata_datetime_sql_sig($data)
{
	if ($data)
            return substr($data,6,4).'-'.substr($data,3,2).'-'.substr($data,0,2).''.substr($data,10,15);
	else return '';
}

function formata_datetime_sig($data){

    $data = substr($data,0,19);
    if ($data)
         return  substr($data,8,2).'/'.substr($data,5,2).'/'.substr($data,0,4).substr($data,10,19);
       else return '';
}

function formata_date_datetime_sql_sig($data){
	if ($data) return substr($data,6,4).'-'.substr($data,3,2).'-'.substr($data,0,2).' '.date('H:i:s');
	else return '';
}

function controla_navegacao_sig($nl)
{
	if ($_POST['navega'] == 0) $_SESSION['registro'] = 0;
	else if ($_POST['navega'] == -2) $_SESSION['registro'] = $nl;
	else
	{
		$_SESSION['registro'] = $_SESSION['registro'] + $_POST['navega'];
		if ($_SESSION['registro'] < 0) $_SESSION['registro'] = 0;
		if ($_SESSION['registro'] > $nl) $_SESSION['registro'] = $nl;
	}
	if (! $_SESSION['registro'] ) $_SESSION['registro'] = 0;
}

/**
 * @description criptografa uma senha no padrão MD5
 * @param unknown $plain_text
 * @param unknown $password
 * @param number $iv_len
 */
function md5_encrypt_senha_sig($plain_text, $password, $iv_len = 16)
{
	$plain_text .= "\x13";
	$n = strlen($plain_text);
	if ($n % 16) $plain_text .= str_repeat("\0", 16 - ($n % 16));
	$i = 0;
	$enc_text = get_rnd_iv($iv_len);
	$iv = substr($password ^ $enc_text, 0, 512);
	while ($i < $n) {
		$block = substr($plain_text, $i, 16) ^ pack('H*', md5($iv));
		$enc_text .= $block;
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	return base64_encode($enc_text);
	// return base64_encode(trim($plain_text));
}

/**
 * criptográfa senhas no padrão md5
 *
 * @param unknown $plain_text
 * @param string $password
 * @param number $iv_len
 */
function md5_encrypt_sig($plain_text, $password='', $iv_len = 16)
{
	//$plain_text .= "\x13";
	$n = strlen($plain_text);
	if ($n % 16) $plain_text .= str_repeat("\0", 16 - ($n % 16));
	$i = 0;
	$enc_text = get_rnd_iv($iv_len);
	$iv = substr($password ^ $enc_text, 0, 512);
	while ($i < $n) {
		$block = substr($plain_text, $i, 16) ^ pack('H*', md5($iv));
		$enc_text .= $block;
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	//return base64_encode($enc_text);
	return base64_encode(trim($plain_text));
}

/**
 * insere a imagem indicadora de campo obrigatório em determindos itens de uma página
 *
 * @return string
 */
function obrigatorio_sig()
{
	$obrigatorio = " <img border='0' src='/imagens/obrig.gif' title='Indica campo obrigatório.' />";
	return $obrigatorio;
}

/**
 * insere a imagem indicadora de descricao do campo em determindos itens de uma página
 * Rafael Miranda
 *
 * @return string
 */
function campo_descricao_sig($descricao,$paramConfig = array(),$alerta = false)
{
    switch ($paramConfig['lado']) {
            case 'right':
                $lado = "right_";
                break;
            case 'left':
                $lado = "left_";
                break;
            default:
                $lado = "left_";
                break;
        }

        switch ($paramConfig['tamanho']) {
            case 50:
                $tamanho = "50";
                break;
            case 100:
                $tamanho = "100";
                break;
            case 150:
                $tamanho = "150";
                break;
            case 200:
                $tamanho = "200";
                break;
            case 300:
                $tamanho = "300";
                break;
            case 500:
                $tamanho = "500";
                break;

            default:
                $tamanho = "150";
                break;
        }

        $posicao = $lado.$tamanho;
        $img = ($alerta) ? "<img border='0'  src='/imagens/restricao2.png' width='12px'>" : "<img border='0'  src='/imagens/help-azul.png' width='12px'>";
        $obrigatorio = "<a href='javascript:void(0);' class='campo-descricao-ajuda'>
                            <span data-title='".$descricao."' class='tooltipTabela ".$posicao."'>
                                $img
                            </span>
                        </a>";
	return $obrigatorio;
}

function md5_decrypt_sig($enc_text, $password='', $iv_len = 16)
{
	$enc_text = base64_decode($enc_text);
	$n = strlen($enc_text);
	$i = $iv_len;
	$plain_text = '';
	$iv = substr($password ^ substr($enc_text, 0, $iv_len), 0, 512);
	while ($i < $n) {
		$block = substr($enc_text, $i, 16);
		$plain_text .= $block ^ pack('H*', md5($iv));
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	return $enc_text;
	//return preg_replace('/\\x13\\x00*$/', '', $plain_text);
}

/**
 * descriptográfa senhas no padrão md5
 *
 * @param unknown $enc_text
 * @param string $password
 * @param number $iv_len
 */
function md5_decrypt_senha_sig($enc_text, $password, $iv_len = 16)
{
	$enc_text = base64_decode($enc_text);
	$n = strlen($enc_text);
	$i = $iv_len;
	$plain_text = '';
	$iv = substr($password ^ substr($enc_text, 0, $iv_len), 0, 512);
	while ($i < $n) {
		$block = substr($enc_text, $i, 16);
		$plain_text .= $block ^ pack('H*', md5($iv));
		$iv = substr($block . $iv, 0, 512) ^ $password;
		$i += 16;
	}
	return preg_replace('/\\x13\\x00*$/', '', $plain_text);

}


/**
 * Monta um título alinhado ao centro em determinado ponto de uma página
 *
 * @param unknown $linha1
 * @param unknown $linha2
 * @return string
 */
function monta_titulo2_sig($linha1,$linha2)
{
	//print '<center><div id="cabecalho" align="center" style="background-color:#DCDCDC;padding-top:3px;padding-bottom:3px;width:95%;" class="TituloTela">'.$linha1.'</div></center>';
	$retorno = '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	$retorno .= '<tr><td width="100%" align="center" class="TituloTela">'.$linha1.'</td></tr><tr>';
	$retorno .= '<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
	return $retorno;
}

/**
 * Imprime um título alinhado ao centro em determinado ponto de uma página
 *
 * @param unknown $linha1 destinado ao título da página (primeira linha da tabela)
 * @param unknown $linha2 destinado à informações que visem facilitar a navegação do usuário (segunda linha da tabela)
 * @return string
 */
function monta_titulo_sig($linha1,$linha2)
{
    print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
    print '<tr><td width="100%" align="center" class="TituloTela">'.$linha1.'</td></tr>';
    print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >'.$linha2.'</td></tr></table>';
}


/*
 	 * Função  monta_lista_titulo
	 * Método  usado para inserir um titulo e um icone de ajuda para um bloco de implementacoes
	 * @access   public
	 * @author   RAFAEL LIMA MIRANDA
	 * @since    31/10/2014
	 * @param    string $tituloCabecalho - Deve conter o titulo do cabecalho
	 * @param    string $tituloAjuda - Deve conter a descricao de ajuda sobre o que sera apresentado
	 * @param    array $paramConfig - Array com os parametros de posicionamento do icone de ajuda
                            OBS: tamanhos possiveis para o bloco de ajuda: 50,100,150,200,300,500
                                 ex1: array('lado'=>'left','tamanho'=>200)
                                 ex2: array('lado'=>'right','tamanho'=>50)
 */
function monta_titulo_bloco_sig($tituloCabecalho,$tituloAjuda="", Array $paramConfig = array()){


    if (($tituloAjuda != "") && ($tituloAjuda != "")) :

        switch ($paramConfig['lado']) {
            case 'right':
                $lado = "right_";
                break;
            case 'left':
                $lado = "left_";
                break;
            default:
                $lado = "left_";
                break;
        }

        switch ($paramConfig['tamanho']) {
            case 50:
                $tamanho = "50";
                break;
            case 100:
                $tamanho = "100";
                break;
            case 150:
                $tamanho = "150";
                break;
            case 200:
                $tamanho = "200";
                break;
            case 300:
                $tamanho = "300";
                break;
            case 500:
                $tamanho = "500";
                break;

            default:
                $tamanho = "150";
                break;
        }

        $posicao = $lado.$tamanho;

        print '<div class="bloco-header">
                    <h4>'.$tituloCabecalho.'</h4>
                    <div class="bloco-link">
                        <a href="javascript:void(0);">
                            <span data-title="'.$tituloAjuda.'" class="tooltipTabela '.$posicao.'">
                                <img border="0"  src="/imagens/help-b.png">
                            </span>
                        </a>
                    </div>
                </div>';

    elseif ($tituloCabecalho != "") :

        print '<div class="bloco-header">
                <h4>'.$tituloCabecalho.'</h4>
            </div>';
    endif;
}

/**
 * Monta o sql que lista os arquivos permitidos para download
 *
 * @param unknown $campo
 * @param unknown $item
 * @param number $chave
 * @return string
 */
function sql_vincula_arquivo_sig ($campo,$item,$chave=0)
{
	//esta funcao tem por finalidade montar a sql que permitirá listar os arquivos que
	// poderão ser listados para download
	$usucpf=$_SESSION['usucpf'];
	$sql = "select d.usucpf as donoarquivo, d.tpdcod, d.docid,d.docdsc as descricao, d.docnomefisico, case when d.docsigilo='O' then 'Ostensivo' when d.docsigilo='R' then 'Reservado' else 'Confidencial' end as sigilo from public.documento d inner join public.documento_processo dp on d.docid=dp.docid and dp.tabela_processo='$campo' inner join $campo cc on cc.$chave=$item and cc.$chave=dp.proid order by d.docsigilo,d.docdsc asc";
	return $sql;


}

/**
 * Formata um número de CPF inserido a máscara padrão
 *
 * ex: ###.###.###-##
 *
 * @param unknown $cpf
 * @return void|string
 */
function formatar_cpf_sig($cpf) {
	if ( empty($cpf) ) return ;
	else return substr($cpf,0,3).".".substr($cpf,3,3).".".substr($cpf,6,3)."-".substr($cpf,9,2);
}

/**
 *
 *
 * @param unknown $cnpj
 * @return void|string
 */
function formatar_cnpj_sig($cnpj) {
	if ( empty($cnpj) ) return ;
	return substr($cnpj,0,2).".".substr($cnpj,2,3).".".substr($cnpj,5,3)."/".substr($cnpj,8,4)."-".substr($cnpj,12,2);
}

/**
 *
 * retorna um número de cpf ou cnpj sem mascara
 *
 * @param unknown $cnpjcpf
 */
function pega_numero_sig($cnpjcpf) {
	if ( empty($cnpjcpf) ) return ;
	return preg_replace( "/[^\d]/", "", $cnpjcpf );
}

/**
 * Formata um número de CPF ou CNPJ inserido a máscara padrão
 *
 *ex cnpj: ##.###.###/####-##
 *
 *ex cpf: ###.###.###-##
 *
 * @param unknown $cpfcnpj
 * @return void|string
 */
function formatar_cpf_cnpj_sig($cpfcnpj) {
	if ( strlen($cpfcnpj) == 11 )
	return substr($cpfcnpj,0,3).".".substr($cpfcnpj,3,3).".".substr($cpfcnpj,6,3)."-".substr($cpfcnpj,9,2);
	else if ( strlen($cpfcnpj) == 14 )
	return substr($cpfcnpj,0,2).".".substr($cpfcnpj,2,3).".".substr($cpfcnpj,5,3)."/".substr($cpfcnpj,8,4)."-".substr($cpfcnpj,12,2);
	else return;
}

function make_seed_sig() {
	list($usec, $sec) = explode(' ', microtime());
	return (float) $sec + ((float) $usec * 100000);
}

function senha_sig() {
	mt_srand(make_seed());
	for($i=0;$i<8;$i++) {
		$n = (integer)mt_rand(0,35);
		$c = ($n<10) ? ($n+48) : ($n+55);
		$senha.= pack ("c",$c);
	}
	return $senha;
}

/**
 *
 * Função para Envio de E-mail
 *
 * @param unknown $paraquem
 * @param unknown $paraonde
 * @param unknown $assunto
 * @param unknown $message
 * @param string $cc
 * @param string $cco
 * @return boolean
 */
function email_sig($paraquem, $paraonde, $assunto, $message, $cc='',$cco='')
{
	global $db;
	require_once APPRAIZ . 'includes/phpmailer-5.2.6/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer-5.2.6/class.smtp.php';

	//trata mensagem
	if($message) $message = str_replace(chr(13), '<br>', $message);

	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = SMTP_HOST;
        $mensagem->SMTPAuth     = SMTP_AUTH;
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName	= "SIG - ".strtoupper($_SESSION['sisdiretorio'])." - " . $_SESSION['usunome'] . " - " . $_SESSION['usuorgao'];

	// Alteração feita por Felipe Carvalho - 19/02/2010
	//$mensagem->AddReplyTo("cristiano.cabral@mec.gov.br");
	//$mensagem->AddReplyTo($_SESSION["usuemail"]);
	//$mensagem->From = "simec@mec.gov.br";
	$mensagem->From = SMTP_FROM_SIG;

	$mensagem->AddAddress( $paraonde, $paraquem );
	$mensagem->Subject = $assunto;
	$mensagem->Body = $message;
	$mensagem->IsHTML( true );
	foreach ( explode( ";", $cc ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddCC( $end );
		}
	}
	foreach ( explode( ";", $cco ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddBCC( $end );
		}
	}

	return $mensagem->Send();


	/*

	global $db;
	// esta função está sendo alterada para incluir sempre a origem e para enviar um email para a mesma origem
	$sql = "select u.usunome, u.usuemail, un.unidsc,o.orgdsc from seguranca.usuario u left join unidade un on un.unicod=u.unicod left join orgao o on o.orgcod=u.orgcod where u.usucpf='".$_SESSION['usucpf']."' limit 1";
	$res=$db->pegalinha($sql);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	//$message=strip_tags($message);
	$dequem = "SIG - $usunome - $unidsc - $orgdsc"; //trim($_SESSION['sigla']);
	$deonde = "$usuemail;simec@mec.gov.br";//$_SESSION['ittemail'];
	$headers = "Return-Path: simec@mec.gov.br\n";
	$headers .= "Reply-To: simec@mec.gov.br\n";
	$headers .= "X-Sender: simec@mec.gov.br\n";
	$headers .= "Content-type: text/html;\n";
	$headers .= "From: \"".$dequem."\" <".$deonde.">\n";
	$headers .= "X-Mailer: PHP 5.1\n";
	//$headers .= "Reply-To:gilbertoxavier@gmail.com\n";
	if ($cc) $headers .= "Cc:$cc\n";
	if ($cco) $headers .= "Bcc:$cco\n";
	$headers .= "MIME-Version: 1.0\n";
	//dbg($headers,1);
	return mail($paraonde, $assunto, $message, $headers);
	*/
}

/**
 *
 * Função para envio de e-mail tendo como remetente o endereço pessoal
 *
 * @param unknown $dequem
 * @param unknown $deonde
 * @param unknown $paraonde
 * @param unknown $assunto
 * @param unknown $message
 * @param string $cc
 * @param string $cco
 * @return boolean
 */
function email_pessoal_sig($dequem,$deonde, $paraonde, $assunto, $message, $cc='',$cco='')
{

	global $db;
	require_once APPRAIZ . 'includes/phpmailer-5.2.6/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer-5.2.6/class.smtp.php';
	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = SMTP_HOST;
        $mensagem->SMTPAuth     = SMTP_AUTH;
	$mensagem->Mailer       = "smtp";
	$mensagem->FromName	= SMTP_FROM_SIG_NAME;
	$mensagem->AddReplyTo(SMTP_FROM_NOREPLY);
        $mensagem->From = SMTP_FROM_SIG;
	$mensagem->AddAddress( $paraonde );
	$mensagem->Subject = $assunto;
	$mensagem->Body = $message;
	$mensagem->IsHTML( true );
	foreach ( explode( ";", $cc ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddCC( $end );
		}
	}
	foreach ( explode( ";", $cco ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddBCC( $end );
		}
	}
	return $mensagem->Send();
	/*
	 // $dequem = 'SIMEC'; //trim($_SESSION['sigla']);
	 // $deonde = 'simec@mec.gov.br';//$_SESSION['ittemail'];
	 $headers = "Return-Path: simec@mec.gov.br\n";
	 $headers .= "X-Sender: simec@mec.gov.br\n";
	 $headers .= "Reply-To: simec@mec.gov.br\n";
	 $headers .= "Content-type: text/html;\n";
	 $headers .= "From: \"".'SIMEC - '.$dequem."\" <".$deonde.">\n";
	 $headers .= "X-Mailer: PHP 5.1\n";
	 if ($cc) $headers .= "Cc:$cc\n";
	 if ($cco) $headers .= "Bcc:$cco\n";
	 $headers .= "MIME-Version: 1.0\n";
	 return mail($paraonde, $assunto, $message, $headers);
	 */
}

/**
 *
 * Função para envio de email
 *
 * @param string $remetente
 * @param string $destinatario
 * @param string $assunto
 * @param string $conteudo
 * @param string $cc
 * @param string $cco
 * @param array $arquivos
 * @param string $demid
 * @return boolean
 */
function enviar_email_sig( $remetente, $destinatario, $assunto, $conteudo, $cc='',$cco='', $arquivos = array(), $demid = null){

	global $db;
	require_once APPRAIZ . 'includes/phpmailer-5.2.6/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer-5.2.6/class.smtp.php';

	$mensagem = new PHPMailer();
	$mensagem->persistencia = $db;
	$mensagem->Host         = SMTP_HOST;
	$mensagem->SMTPAuth     = SMTP_AUTH;
	$mensagem->Mailer       = "smtp";

	if(!is_array($remetente))
	{
		$arrRemetente['email'] = SMTP_FROM_SIG;
    	$arrRemetente['nome'] = "Administrador SIG";
		$remetente = $arrRemetente;
	}

	$mensagem->From =  SMTP_FROM_SIG;

	if(count($remetente)==2) {
		$mensagem->AddReplyTo($remetente['email'],$remetente['nome']);
		$mensagem->FromName = $remetente['nome'];
	} elseif($remetente != '') {
		$mensagem->FromName = $remetente['nome'];
	} else {
		$mensagem->FromName = "Administrador SIG";
	}
	
	if ( $destinatario )
	{
		if( is_array( $destinatario ) )
		{
			foreach( $destinatario as $email ){
				$mensagem->AddAddress( $email );
			}
		}
		else
		{
			   $arrDestinatario = delimitador_string_to_array($destinatario);
			   if($arrDestinatario)
			   {
				   foreach( $arrDestinatario as $email )
				   {
					   $mensagem->AddAddress( $email );
				   }
			   }
			   else
			   {
				  $mensagem->AddAddress( $destinatario );
			   }
		}		
	} 

	if( $cc )
	{
		if(is_array($cc)) 
		{
			foreach($cc as $email) 
			{
				$mensagem->AddCC( $email );
			}
		} 
		else
		{
			$arrCC = delimitador_string_to_array($cc);
			if($arrCC)
			{
				foreach( $arrCC as $email )
				{
					$mensagem->AddCC( $email );
				}
			}
			else
			{
				$mensagem->AddCC( $cc );
			}
		}
	}
	
	if ( $cco )
	{
		if(is_array($cco)) 
		{
			foreach($cco as $email) 
			{
				$mensagem->AddBCC( $email );
			}
		} 
		else
		{
		   $arrBCC = delimitador_string_to_array($cco);
			if($arrBCC)
			{
				foreach( $arrBCC as $email )
				{
					$mensagem->AddBCC( $email );
				}
			}
			else
			{
				$mensagem->AddBCC( $cco );
			}
		}
	}
	
	if ( $arquivos )
	{
		foreach ( $arquivos as $arquivo )
		{
			if ( !file_exists( $arquivo['arquivo'] ) ) 
			{
				continue;
			}
			$mensagem->AddAttachment( $arquivo['arquivo'], basename( $arquivo['nome'] ) );
		}
	}

	$mensagem->Subject = $assunto;
	$mensagem->Body = $conteudo;
	$mensagem->IsHTML( true );

	// Tratamento de erros e exceções separado para envio de e-mail para não travar o sistema em caso de erro
	$old_error = set_error_handler(function () { return true; });
	$old_exception = set_exception_handler(function() { return true; });

	try
	{
		$resultadoSendEmail = $mensagem->Send();
	}
	catch( Exception $e )
	{
		$resultadoSendEmail = false;
	}

	// Retorna o tratamento de erros e exceções padrão do SIG
	restore_error_handler();
	restore_exception_handler();

	return $resultadoSendEmail;
}

/**
 *
 * Permite enviar um email incluindo como remetente o usuário do SIG
 *
 * @param unknown $remetente
 * @param unknown $destinatario
 * @param unknown $assunto
 * @param unknown $conteudo
 * @param string $cc
 * @param string $cco
 * @return boolean
 */
function enviar_email_usuario_sig( $remetente, $destinatario, $assunto, $conteudo, $cc='',$cco='' ){
	require_once APPRAIZ . 'includes/phpmailer-5.2.6/class.phpmailer.php';
	require_once APPRAIZ . 'includes/phpmailer-5.2.6/class.smtp.php';
	$mensagem = new PHPMailer();
	global $db;
	$mensagem->persistencia = $db;
	$mensagem->Host         = SMTP_HOST;
        $mensagem->SMTPAuth     = SMTP_AUTH;
	$mensagem->Mailer       = "smtp";
	$mensagem->From         = SMTP_FROM_SIG;
        $mensagem->FromName 	= "SIG - ".strtoupper($_SESSION['sisdiretorio'])." - " . $_SESSION['usunome'];
	$mensagem->AddAddress( $destinatario );
	$mensagem->Subject 		= $assunto;
	$mensagem->Body 		= $conteudo;
	$mensagem->IsHTML( true );

	//ver($mensagem);
	foreach ( explode( ";", $cc ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddCC( $end );
		}
	}
	foreach ( explode( ";", $cco ) as $end )
	{
		$end = trim( $end );
		if( $end )
		{
			$mensagem->AddBCC( $end );
		}
	}

	return $mensagem->Send();
}

/**
 *
 *Altera o primeiro caracter de cada palavra de uma expresaão, transformando em maiúsculo, com exceções de artigos, pronomes, etc.
 *
 * @param unknown $variavel
 * @return string
 */
function ulcase_sig($variavel) {
	// altera o primeiro caracter de cada palavra de uma expresaão, transformando em maiúsculo, com exceções de artigos, pronomes, etc.
	setlocale("LC_CTYPE","iso_8859_1");
	$nom=explode(" ",$variavel);
	$nom[0]=ucfirst(strtolower($nom[0]));
	for ($j=1;$nom[$j];$j++) {
		$nom[$j]=strtolower($nom[$j]);
		$nom[$j]=ucfirst($nom[$j]);
		if ( preg_match("/^(Com|Sem|Para|Por|Dos|Das|Ou)$/",$nom[$j],$temp ) ) {
			$nom[$j]=strtolower($nom[$j]);
		}
		if ( preg_match("/^[DN].$/",$nom[$j],$temp ) ) {
			$nom[$j]=strtolower($nom[$j]);
		}
		if ( preg_match("/^[AEO]$/",$nom[$j],$temp ) ) {
			$nom[$j]=strtolower($nom[$j]);
		}
	}
	$nom_int = implode($nom," ");
	if ( substr($nom_int,-3,1) == "/" )
	$nom_int = substr($nom_int,0,strlen($nom_int)-3)." - "
	.strtoupper(substr($nom_int,-2));
	return $nom_int;
}

/**
 *
 * Remove acentuação Gráfica
 *
 * @param unknown $variavel
 * @return unknown
 */
function tirar_acentos_sig($variavel) {
	$busca= array("'Á'","'À'","'Ã'","'Ä'","'Â'","'É'","'È'","'Ê'","'Ë'",
	"'Í'","'Ì'","'Ï'","'Î'","'Ñ'",
	"'Ó'","'Ò'","'Ô'","'Õ'","'Ö'","'Ú'","'Ù'","'Ü'","'Û'",
	"'Ý'","'Ç'",
	"'á'","'à'","'ã'","'ä'","'â'","'é'","'è'","'ê'","'ë'",
	"'í'","'ì'","'ï'","'î'","'ñ'",
	"'ó'","'ò'","'ô'","'õ'","'ö'","'ú'","'ù'","'ü'","'û'",
	"'ý'","'ç'");
	$subst= array("A","A","A","A","A","E","E","E","E","I","I","I","I","N",
	"O","O","O","O","O","U","U","U","U","Y","C",
	"a","a","a","a","a","e","e","e","e","i","i","i","i","n",
	"o","o","o","o","o","u","u","u","u","y","c");
	$result = preg_replace($busca,$subst,$variavel);
	return $result;
}

/**
 *
 * confere digito verificador para CPF ou CNPJ
 *
 * @param unknown $cpfcnpj
 * @return boolean
 */
function dv_cpf_cnpj_ok_sig($cpfcnpj) {
	$dv = false;
	//$cpfcnpj = ereg_replace("[^0-9]","",$cpfcnpj);
	 $cpfcnpj = preg_replace("/[^0-9]/","", $cpfcnpj);
	if ( strlen($cpfcnpj) == 14 ) {
		$cnpj_dv = substr($cpfcnpj,-2);
		for ( $i = 0; $i < 2; $i++ ) {
			$soma = 0;
			for ( $j = 0; $j < 12; $j++ )
			$soma += substr($cpfcnpj,$j,1)*((11+$i-$j)%8+2);
			if ( $i == 1 ) $soma += $digito * 2;
			$digito = 11 - $soma  % 11;
			if ( $digito > 9 ) $digito = 0;
			$controle .= $digito;
		}
		if ( $controle == $cnpj_dv )
		$dv = true;
	}
	if ( strlen($cpfcnpj) == 11 ) {
		$cpf_dv = substr($cpfcnpj,-2);
		for ( $i = 0; $i < 2; $i++ ) {
			$soma = 0;
			for ( $j = 0; $j < 9; $j++ )
			$soma += substr($cpfcnpj,$j,1)*(10+$i-$j);
			if ( $i == 1 ) $soma += $digito * 2;
			$digito = ($soma * 10) % 11;
			if ( $digito == 10 ) $digito = 0;
			$controle .= $digito;
		}
		if ( $controle == $cpf_dv )
		$dv = true;
	}
	return $dv;
}

/**
  * @description Formata string informada como número de telefone necional com 9, 10 ou 11 dígitos, considerando o DDD. Fonte: http://forum.laravel.com.br/viewtopic.php?id=317
  * @author Allan Carlos Ramalho (allan.c.ramalho@gmail.com)
  * @since 27/05/2015
  * @param string $numero - string de entrada para formatação.
  * @return string - número formatado
 **/
function formataFone_sig($numero) {
    $numero = preg_replace('/^[+]\d{2}|[^\d]/', '', $numero);
    return preg_filter('/^(\d{2})(\d{3,5})(\d{4})$/', '($1) $2-$3', $numero);
}

/**
 *
 *Formata número de telefone ou fax incluindo 0 antes do ddd e máscara
 *
 * @param unknown $numero
 * @return string
 */
function formata_fone_fax_sig($numero) {
	//$numero = ereg_replace("[^0-9]","",$numero);
	$numero = preg_replace('#\$([0-9])#', '', $numero);
	//$numero = ereg_replace("^0+","",$numero);
	$numero = preg_replace("#\$([0+])#","",$numero);
	if ( strlen($numero) == 14 or strlen($numero) == 12 )
	$numero = substr($numero,-10);
	if ( strlen($numero) == 13 or strlen($numero) == 11 )
	$numero = substr($numero,-9);
	$res = substr($numero,-4);
	if ( strlen($numero) > 4  and strlen($numero) < 9 )
	$res = substr($numero,0,strlen($numero)-4)."-".$res;
	if ( strlen($numero) > 8  )
	$res = "(0XX".substr($numero,0,2).") ".
	substr($numero,2,strlen($numero)-6)."-".$res;
	return $res;
}


/**
 * @description Formata o número de CEP retiando a máscara
 * @author 		Felipe Tarchiani Cerávolo Chiavicatti (felipe.tcc@gmail.com)
 * @since 		17/07/2014

 * @param  string $numCep - Número de CEP com máscara
 * @return string - Número do CEP sem a máscara aplicada
 */
function CepToBd_sig ($numCep){
	$numCep = str_replace(Array('.','-'), '', $numCep);

	return $numCep;
}

/**
 *
 * formata um número de cep inserindo a devida máscara
 *
 * @param unknown $num
 * @return string
 */
function formata_cep_sig($num) {
	//$num = ereg_replace("[^0-9]","",$num);
	$num = preg_replace("/[^0-9]/","", $num);
	$res = substr($num,0,5);
	if ( strlen($num) > 5 ) $res.="-".substr($num,5,3);
	return $res;
}

/**
 *
 * verifica se um endereço de email é válido
 *
 * @param unknown $email
 */
function verifica_email_sig ($email) {
	return(preg_match("'^[a-z0-9_.=-]+@(?:[a-z0-9-]+\.)+([a-z]{2,3})\$'i",
	$email));
}
function verifica_cep_sig ($cep) {
	$res=false;
	//$cep = ereg_replace("[^0-9]","",$cep);
	$cep = preg_replace("/[^0-9]/","", $cep);
	if ( strlen($cep) == 8 ) $res=true;
	return $res;
}
function verifica_data_sig($data) {
	$dv = false;
	//$data = ereg_replace("[^0-9]","",$data);
	$data = preg_replace("/[^0-9]/","", $data);
	$tam = strlen($data);
	if ( $tam == 8 ) {
		$dia = substr($data,0,2);
		$mes = substr($data,2,2);
		$ano = substr($data,4,4);
		if ( $dia > 0 && $dia < 32 &&
		$mes > 0 && $mes < 13 &&
		$ano > 1900 && $ano < 2100 ) $dv=true;
	}
	return $dv;
}

/**
 * @description verifica qual é o perfil de usuário do sistema
 * @param unknown $pflcods
 * @return boolean
 */
function verificaPerfil_sig( $pflcods )
{
	global $db;
	if ( is_array( $pflcods ) )
	{
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else
	{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 )
	{
		return false;
	}
	$sql = "
		select
			count(*)
		from seguranca.perfilusuario pu
		inner join seguranca.usuario_sistema as us on us.usucpf = pu.usucpf
		where
			pu.usucpf = '" . $_SESSION['usucpf'] . "' and
			pu.pflcod in ( " . implode( ",", $pflcods ) . " ) and
			us.sisid = ".$_SESSION['sisid']."
		";
	return $db->pegaUm( $sql ) > 0;
}

/*
 function dv_banco_11_cb_sig($numero) {
 $numero = ereg_replace("[^0-9]","",$numero);
 $len = strlen($numero);
 for ( $i = 0; $i<$len; $i++ )
 $soma+= (($len-$i-1)%8+2) * intval(substr($numero,$i,1));
 $dv = 11 - $soma%11;
 if ( $dv > 9 ) {
 $dv="1";
 }
 return $dv;
 }
 function dv_banco_11_sig($numero,$x=false) {
 $numero = ereg_replace("[^0-9]","",$numero);
 $len = strlen($numero);
 for ( $i = 0; $i<$len; $i++ )
 $soma+= (9-($len-$i-1)%8) * intval(substr($numero,$i,1));
 $dv = $soma%11;
 if ( $dv == 10 ) {
 if ( $x ) $dv="X";
 else $dv="0";
 }
 return $dv;
 }
 function dv_banco_10_sig($numero) {
 $numero = ereg_replace("[^0-9]","",$numero);
 $len = strlen($numero);
 for ( $i = 0; $i<$len; $i++ ) {
 $s = (2-($len-$i-1)%2) * intval(substr($numero,$i,1));
 if ( $s > 9 ) $s = intval(substr($s,0,1)) + intval(substr($s,1,1));
 $soma+=$s;
 }
 $dv = 10-$soma%10;
 if ( $dv == 10 ) $dv = 0;
 return $dv;
 }
 */

/**
 * @description remove caracteres especiais(mascara) de um cpf
 * @param unknown $cpf
 */
function corrige_cpf_sig($cpf)
{
	//return ereg_replace("[^0-9]", "", $cpf);
	return preg_replace("/[^0-9]/","", $cpf);

}

/**
 * @description função para exibição de erros de formulário
 * @param unknown $erros
 * @return number
 */
function exibir_erros_sig($erros) {
	$num = count($erros);
	if(!$num) return 0;
	echo "Foram encontrado(s) o(s) seguinte(s) erro(s) no seu formulário:<br>\n";
	for($x=0;$x<$num;$x++) {
		echo "<br>&nbsp;&nbsp;&nbsp;&nbsp;"."<font color=red><b>".$erros[$x]."</b></font>"."\n";
	}
	return 1;
}

function fator_vencimento_sig($data) {
	$dat = explode("/",$data);
	return intval((mktime(0,0,0,$dat[1],$dat[0],$dat[2])-mktime(0,0,0,10,7,1997))/86400);
}

function limpar_numero_sig($num) {
	for($x=0;$x<strlen($num);$x++) {
		if(preg_match("/[0-9]/",$num[$x])) $saida .= $num[$x];
	}
	return $saida;
}

/**
 * @description criptografa uma senha no padrão MD5
 * @param unknown $senha
 * @return string
 */
function md5_senha_sig($senha) {
	return "{md5}".base64_encode(pack("H*",md5($senha)));
}

/**
 * @description componente que cria um campo input do tipo Data
 * @param unknown $dt
 * @param unknown $obrig
 * @param unknown $habil
 * @param unknown $label
 * @param unknown $formata
 * @param string $txtdica
 * @param string $txtOnChange
 * @param string $value
 * @return string
 */
function campo_data_sig($dt,$obrig,$habil,$label,$formata,$txtdica='',$txtOnChange='', $value = null, $arrAtributos  = null)
{
	global ${$dt};
	$txtdica = $txtdica ? "return escape('". $txtdica ."');" : null;

	$title = "";

	if($label)
		$title = ' title="'.$label.'" ';

	if ($habil=='S') {
		//dbg(${$dt},0,'37423428787');
		//dbg('rrr'.formata_data(${$dt}),0,'37423428787');

		$data = '<input '.$title.' type="text" id="'.$dt.'" name="'.$dt;
		if ($formata=='S') $data.='" value="'.formata_data_sig(($value) ? $value : ${$dt}); else $data.='" value="'.${$dt};
		$data.='" size="12" style="text-align: right;" maxlength="10" value="" class="normal"
				  onKeyUp="this.value=mascaraglobal('."'##/##/####',this.value);".'"
				  onchange="' . $txtOnChange . '" onmouseover="MouseOver(this);'. $txtdica .'"
				  onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);"
				  onblur="MouseBlur(this);this.value=mascaraglobal('."'##/##/####',this.value);".'VerificaData(this, this.value);">
				  <a href="javascript:show_calendar('."'formulario.".$dt."');\">".
		'<img src="/imagens/calendario.gif" width="16" height="15" border="0" align="absmiddle" alt=""></a>';
	}
	else
	{
		$data = '<input '.$title.' type="text" id="'.$dt.'" name="'.$dt.
		'" readonly class="normal"value="'.formata_data_sig(($value) ? $value : ${$dt}).
		'" size="12" style="text-align:right;BORDER-LEFT:#888888 3px solid; COLOR:#808080;" maxlength="10">';
	}
	if ($obrig == 'S')
	$data = $data . obrigatorio();

	return $data;
}

/**
 *
 * @global type ${$var}
 * @param type $var (1) Nome do input. Usado para buscar um valor global para o input. Pode ser modificado por $hid.
 * @param type $obrig (2) Adiciona ao html do input a imagem que indica obrigatoriedade do campo
 * @param type $habil
 * @param type $label
 * @param type $size (5) Tamanho do input
 * @param type $max (6) Tamanho máximo do texto
 * @param type $masc (7) Máscara do input
 * @param type $hid
 * @param type $align
 * @param type $txtdica
 * @param type $acao
 * @param string $complemento (12) Atributos complementares do input. Ex: 'id="input_id" required'
 * @param type $evtkeyup (13) Javascript executado no evento onKeyUp
 * @param type $value
 * @param type $evtblur
 * @param array $arrStyle: (16) Variavel onde podem ser passados parametros de estilo, sendo o índice a propriedade. ex: array('width' =>'100%')
 * @return string
 */
/*function campo_texto_sig($var,$obrig,$habil,$label,$size,$max,$masc,$hid, $align='left', $txtdica='',$acao=0, $complemento='',$evtkeyup='', $value = null, $evtblur='', $arrStyle = null )
{

	global ${$var};
	$value = $value != '' ? $value : ${$var};

	if ( $obrig == 'S' ) $class = 'obrigatorio';

	$arrStyle['text-align'] = $align;

	if(is_array($arrStyle)){
		$sty = 'style="';
		foreach($arrStyle as $chaves=>$dados){
			$sty .= $chaves.':'.$dados.';';
		}
		$sty .= '"';
	}

	if ($hid=='S') $dif = '1';
	$texto = '<input type="text" '.$sty.' name="'.$var.$dif.'" size="'.($size+1).'" maxlength="'.$max. '" value="'.$value.'"';
	if ($masc !="")
	{
		if($evtkeyup != "")
		{
			$texto = $texto. ' onKeyUp= "this.value=mascaraglobal(\'' . $masc . '\',this.value);' . $evtkeyup . '"';
		}
		else
		{
			$texto = $texto. ' onKeyUp= "this.value=mascaraglobal(\'' . $masc . '\',this.value);"';
		}
	}
	else
	{
		if($evtkeyup != "")
		$texto = $texto. ' onKeyUp= "' . $evtkeyup . '"';
	}
	$habil != 'N' ? $class.=" normal"  :   $class="disabled";

	if ( $habil == 'N' )
	{
		if ( !$complemento )
		$texto = $texto.' readonly="readonly" style="width:'.($size+3).'ex;text-align : '.$align.';" ';
		else
		$texto = $texto.' ' . $complemento .' readonly="readonly" style="width:'.($size+3).'ex;text-align : '.$align.';" ';
		//if ($habil == 'N') $texto = $texto.' readonly ';
	}
	else
	{
		$texto .= ' onmouseover="MouseOver(this);';
		if ( $txtdica ){
			$texto .= 'return escape(\''.$txtdica.'\');';
		}
		if ( !$complemento ){
			if( $arrStyle ){
				$style = 'style="text-align : '.$align.'; width:'.($size+3).'ex;"';
			} else {
				$style = $sty;
			}
			$texto .= '" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);' . $evtblur . '" '.$style.' ';
		} else {
			$texto .= '" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);' . $evtblur . '" ' . $complemento . ' '.$style.' ';
		}
	}


	$texto = $texto . " title='$label' class='$class' />";

	if ($obrig == 'S')
	$texto = $texto . obrigatorio();
	if ($hid == 'S'){
		$texto = $texto."<input type='hidden' name='".$var."' value ='".$value."'>";
	}
	if ($acao)
	{
		//  	unidade,unicod,uni,combo,ppaacao,Unidade Orçamentária Responsável, and substr(unicod,1,2) in '26'
		$partes = explode(";", $acao);
		$alvo=$partes[0];
		$campo=$partes[1];
		$padrao=$partes[2];
		$tipo=$partes[3];
		$origem=$partes[4];
		$nome_campo=$partes[5];
		$especial=$partes[6];
		$especial2=$partes[7];

		$texto .= '&nbsp;<img border="0" src="/imagens/alterar.gif" title="Editar o campo." onclick='.'"edita_campo('."'$alvo','$campo','$padrao','$tipo','$origem','$nome_campo'";
		//if ($especial)
		$texto .= ",'$especial'";
		//if ($especial2)
		$texto .= ",'$especial2'";
		$texto .= ')">';


	}
	return $texto;
}*/
/*
 * Criação do campo_texto com array
 *
 *
 *
 *
 * exemplo 1:
 * $arrAtributos['name'] = "teste";
				 	$arrAtributos['obrigatorio'] = true;
				 	$arrAtributos['habilitado'] = true;

exemplo 2:
 	$arrAtributos = array(
			 			"name" => "teste",
			 			"obrigatorio" => true,
			 			"habilitado" => false,
						 );
 *
 * exemplo 3:
 * campo_texto('usucpfsolicitante', 'N', "S", '', 12, 60, '###.###.###-##', '', 'left', '', 0, 'id="usucpfsolicitante"', '');
 */

function campo_file_sig($name, $id, $obrig = "N",$title = "",$size = 40,$value = '', $evento='', $style = '', $label= null)
{
      $texto = '';
      $obrigatorio = '';
      if ($obrig == 't' || $obrig == 'S') {
         $texto = "<img border='0' src='/imagens/obrig.gif' title='Indica campo obrigatório.' />";
         $obrigatorio = 'class="obrigatorio"';
      }
      if(is_null($style)){
          $style = '';
      }

    $campo = '<input type="file" id="'.$id.'" name="'.$name.'"
                   title="'.$title.'" size="'.$size.'" value="'.$value.'"
                  '.$obrigatorio.' '.$evento.' '.$style.'  />';
    $campo .= $texto;
    if($label){
      $campo .= "&nbsp; <label style='color:red'>{$label}</label>";
    }
    return $campo;
}

function campo_bool_sig($var,$obrig,$habil,$label,$size,$max,$masc,$hid, $align='left', $txtdica='',$acao=0)
{
	global ${$var};
	if (${$var}=='t') ${$var}=$txtdica.'-> Sim'; else ${$var}=$txtdica.'-> Não';
	if ($hid=='S') $dif = '1';
	$texto = '<input type="text" name="'.$var.$dif.
	'" size="'.($size+1).'" maxlength="'.$max. '" value="'.${$var}.'" class="normal"';
	if ($masc <>"")
	$texto = $texto. ' onKeyUp= "this.value=mascaraglobal('."'".$masc."',this.value);".'"';

	if ($habil == 'N') $texto = $texto.' readonly style="BORDER:none;BORDER-LEFT:#888888 3px solid;COLOR:#404040;width:'.$size.'ex;text-align : '.$align.';" ';
	else {$texto .= ' onmouseover="MouseOver(this);';

	if($txtdica) $texto.= 'return escape(\''.$txtdica.'\');'; $texto.= '" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : '.$align.'; width:'.($size+3).'ex;"';}

	$texto = $texto . " title='$label'>";

	if ($obrig == 'S')
	$texto = $texto . obrigatorio();
	if ($hid == 'S'){
		$texto = $texto."<input type='hidden' name='".$var."' value ='".${$var}."'>";
	}
	if ($acao)
	{
		//  	unidade,unicod,uni,combo,ppaacao,Unidade Orçamentária Responsável, and substr(unicod,1,2) in '26'
		$partes = explode(";", $acao);
		$alvo=$partes[0];
		$campo=$partes[1];
		$padrao=$partes[2];
		$tipo=$partes[3];
		$origem=$partes[4];
		$nome_campo=$partes[5];
		$especial=$partes[6];
		$especial2=$partes[7];

		$texto .= '&nbsp;<img border="0" src="/imagens/alterar.gif" title="Editar o campo." onclick='.'"edita_campo('."'$alvo','$campo','$padrao','$tipo','$origem','$nome_campo'";
		//if ($especial)
		$texto .= ",'$especial'";
		//if ($especial2)
		$texto .= ",'$especial2'";
		$texto .= ')">';


	}
	return $texto;
}


/**
 * Faz um input de texto com busca de dados no servidor.
 * A consulta sql precisa ter um %s com o valor que será buscado
 *
 * @param string $nome
 * @param string $label
 * @param mixed $valor
 * @param integer $size
 * @param integer $maxlength
 * @param string $mascara
 * @param string $align
 * @param boolean $obrigatorio
 * @param boolean $habilitado
 * @param boolean $hidden
 * @param string $complemento conteúdo que vai dentro da tag input, os eventos onfocus, onkeyup e onkeydown não podem ser sobreescritos
 * @param string[] $parametros_extra envia na requisição ajax os valores dos campos com os nomes indicados
 * @param boolean $valida
 * @return string
 *
 * @author Adonias Malosso <malosso@gmail.com>
 * @version 1.0
 */
function campo_texto_ajax_sig( $sql, $id, $nome, $label='', $valor='', $size='', $maxlength='', $mascara='', $align='left', $obrigatorio=false, $habilitado=true, $hidden=false, $label_externo = false, $complemento = '', $parametros_extra = array(), $valor_label = '', $valida = false )
{
	// é preciso passar o comando SQL a ser executado com '%s' no lugar do valor do cmapo
	if(strpos($sql, "%s") === false ) {
		return false;
	}

	$jsCssCampo = $extraCampo = '';

	$_SESSION["SQLAJAX"][$id] = $sql;

	$size += $habilitado ? 1 : 0;
	// se usar o campo hidden, altera o nome do campo normal
	$idCampo = $id;
	$nomeCampo	 = $hidden ? $nome . "1" :  $nome;
	// se campo obrigatorio, apresentar indicador
	if ( $label_externo )
	{
		$extraCampo .= '<span id="LSLabel_' . $idCampo . '">' . $valor_label . '</span>';
	}
	$extraCampo .= $obrigatorio ? "&nbsp;<img src=\"/imagens/obrig.gif\" title=\"Campo $label obrigatório.\" border=\"0\">" : "";
	// colocar campo hidden se necessario
	$extraCampo .= $hidden ? "<input type=\"hidden\" name=\"" . $nome . "\" value=\"" . $valor ."\">" : "";
	// adiciona label ao campo


	// acrescenta javascript para a mascara do campo
	if ($mascara <> "")
	$jsCssCampo .= " onKeyUp=\"this.value=mascaraglobal('" . $mascara . "',this.value);\"";

	if(!$habilitado)
	$jsCssCampo .= " readonly style=\"border: none; border-left: #888888 3px solid; color: #404040; width: " . $size . "ex; text-align: ". $align . ";\"";
	else
	$jsCssCampo .= " style=\"text-align: " . $align . "; width:" . $size . "ex;\"";
	//		$jsCssCampo .= " onmouseover=\"MouseOver(this)\" onfocus=\"MouseClick(this);alert(123);liveSearchStart(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" style=\"text-align: " . $align . "; width:" . $size . "ex;\"";

	$campos_extra = '';
	if ( count( $parametros_extra ) )
	{
		$campos_extra .= " '" . implode( "','", $parametros_extra ) . "' ";
	}
	$extra .= 'new Array(' . $campos_extra . ')';
	$valida_valor = $valida ? 'true' : 'false' ;
	$jsImg = <<<EOS
	var campo = document.getElementById( '$idCampo' );
	 if ( liveSearchStart( campo, $extra ) )
	 {
	 	campo.value = '';
	 	lsFazerBuscar( campo );
	 }
	 else
	 {
	 	MouseBlur( this );
	 }
EOS;
	return <<<EOS
	<div class="divcampoajax">
		<input type="text" name="$nomeCampo" id="$idCampo" size="$size" maxlength="$maxlength" value="$valor" class="normal" title="$label" $jsCssCampo onmouseover="MouseOver(this);" onmouseout="MouseOut(this);" onfocus="if ( liveSearchStart( this, $extra ) ) MouseClick( this ); else MouseBlur( this );" $complemento /><img id="lsImagem_$idCampo" src="/imagens/seta_combo.gif" width="14" height="16" style="margin:0;padding:0;" align="absmiddle" onclick="$jsImg" />
		$extraCampo
	</div>
	<div id="LSResult_$idCampo" class="LSResult" style="display: none;"></div>
	<input type="hidden" id="hidden_$nome" name="hidden_$nome" value="$valor_label"/>
	<script language="javascript">
		document.getElementById( '$idCampo' ).valida = $valida_valor;
	</script>
EOS;
}

/**
 * Monta um campo texto ajax em cascata. Caso o campo que ele é
 * dependente seja edita o valor deste é apagado. As dependencias
 * devem ser passadas na ordem de modo que somente o último é verificado.
 *
 * @param string $sql
 * @param string $nome
 * @param string $descricao
 * @param string[] $dependencias
 * @param integer $size
 * @param integer $maxlength
 * @param string $mascara
 * @param string $align
 * @param boolean $obrigatorio
 * @return string
 */
function campo_texto_ajax_cascata_sig( $sql, $nome, $descricao, $dependencias, $size = '', $maxlength = '', $mascara = '', $align = 'left', $obrigatorio = false, $valor = '', $valor_label = '' )
{
	$ultima_dependencia = end( $dependencias );
	// adiciona valor do campo caso ele já tenha iniciado com algum valor
	$jsLabel = $valor ? " lsLabel[$nome] = '$valor_label'; " : '' ;
	$jsLabel = '';
	$javascript = <<<EOS
		<script type="text/javascript">
			var tamanho = lista_campo_cascata.length;
			lista_campo_cascata[tamanho] = new Array();
			lista_campo_cascata[tamanho]['nome'] = '$nome';
			lista_campo_cascata[tamanho]['dependencia'] = '$ultima_dependencia';
			lista_campo_cascata[tamanho]['valor'] = '${$nome}';
			$jsLabel
		</script>
EOS;
			return $javascript . campo_texto_ajax( $sql, $nome, $nome, $descricao, $valor, $size, $maxlength, $mascara, $align, $obrigatorio, true, false, true, '', $dependencias, $valor_label, true );
}


/**
 * Monta campo textarea.
 *
 * @param string $var nome do campo
 * @param string $obrig obrigatório ou não S|N
 * @param string $habil habilitado ou não S|N
 * @param string $label ???
 * @param string $cols quantidade de coluna
 * @param string $rows quantidade de linhas
 * @param string $max tamanho máximo de caracteres
 * @param string $funcao ???
 * @param string $acao ???
 * @param string $txtdica texto de dica, exibido quando o mouse fica sob o textarea
 * @param string $tab habilita digitação do tab ou não true|false
 * @return string
 *//*
function campo_textarea_sig( $var, $obrig, $habil, $label, $cols, $rows, $max, $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $value = null, $param = Array())
{
	global ${$var};

	$id = $param['id'] ? $param['id'] : $var;
	$value = $value != "" ? $value : ${$var};

	$texto = '';

	if ( $obrig == 'S' ) $class = 'obrigatorio ';

	if($title){
		$title = 'title="'.$title.'"';
	}
	// verifica se há texto de dica e cria span onde o texto ficará visível
	// ao final da função a mesma verificação é realizada para fechar o tag
	if ( $txtdica )
	{
		$texto .= '<span onmouseover="return escape( \'' . $txtdica . '\' );">';
	}
	// inicia o campo textarea
	$texto .= '<textarea  id="' . $id . '" name="' . $var . '" cols="' . $cols . '" rows="' . $rows . '" ' . $funcao . ' '  . $title;
	$style = ''; // contém os estilos a serem aplicados ao textarea
	// monta atributos do textarea de acordo com a habilitação ou não do campo
	if ( $habil == 'N' )
	{
		$texto .= ' readonly="readonly" disabled="disabled" ';
		$style .= 'border:none;border-left:#888888 3px solid;color:#404040;width:' . $cols . 'ex;text-align:' . $align . ';';
	}
	else
	{
		$txtdica = $txtdica ? 'return escape( \'' . htmlentities( $txtdica ) . '\' );' : '' ;
		$texto .= ' onmouseover="MouseOver( this );' . $txtdica . '"';
		$texto .= ' onfocus="MouseClick( this );" ';
		$texto .= ' onmouseout="MouseOut( this );" ';
		if ($max){
			$texto .= ' onblur="MouseBlur( this ); textCounter( this.form.' . $id . ', this.form.no_' . $id . ', ' . $max . ');" ';
		}else {
			$texto .= ' onblur="MouseBlur( this );" ';
		}

		$style .= 'width:' . $cols . 'ex;';
	}
	// habilita tab, quando o tab é habilitado o estilo é alterado para
	// uma classe onde a fonte é monoespeçada
	$keydown = '';
	if ( $tab == true )
	{
		//$keydown = 'return readTabChar( event, this );';
		$style .= 'font-family:monospace;font-size:8pt;';
	}
	// aplica estilos
	$texto .= ' style="' . $style . '" ';
	// verifica se é para habilitar checagem de tamanho do campo
	if ( $max && $habil == 'S' )
	{
		$texto .= ' onkeydown="textCounter( this.form.' . $id . ', this.form.no_' . $id . ', ' . $max . ' );' . $keydown . '" ';
		$texto .= ' onkeyup="textCounter( this.form.' . $id . ', this.form.no_' . $id . ', ' . $max . ');" ';
	}
	// finaliza criação do campo textarea
	$texto .= ' class="'.$class.'txareanormal" >' . str_replace( '&nbsp;', '&amp;nbsp;', ($value != "") ? $value : ${$var} ) . '</textarea>';
	// mostra indicador de obrigatoriedade do campo

	if ( $obrig == 'S' )$texto = $texto . obrigatorio();
	// mostra tamanho máximo do conteúdo caso haja tamanho máximo
	if ( $max && $habil=='S' )
	{
		$texto .= "<br><input readonly style=\"text-align:right;border-left:#888888 3px solid;color:#808080;\" type=\"text\" name=\"no_" . $id . "\" size=\"6\" maxlength=\"6\" value=\"". ( $max - strlen( $value ) ) . "\" class=\"CampoEstilo\"><font color=\"red\" size=\"1\" face=\"Verdana\"> máximo de caracteres</font>";
	}
	// ???
	if ( $acao )
	{
		$partes = explode( ';', $acao);
		$alvo       = $partes[0];
		$campo      = $partes[1];
		$padrao     = $partes[2];
		$tipo       = $partes[3];
		$origem     = $partes[4];
		$nome_campo = $partes[5];
		$especial   = $partes[6];
		$texto = $texto . '&nbsp;<img border="0" src="/imagens/alterar.gif" title="Editar o campo." onclick="edita_campo( ' . "'$alvo', '$campo', '$padrao', '$tipo', '$origem', '$nome_campo', '$especial'" . ' )">';
	}
	// fecha tag span do texto dica
	// olhar comentário no início da função que iniciar o tag
	if ( $txtdica )
	{
		$texto .= "</span>";
	}
	return
		'<div class="notprint">' .
	$texto .
		'</div>' .
		'<div id="print_' . $var . '" class="notscreen" style="text-align: left;">' .
	${$var} .
		'</div>';
}*/

/*
 * Criação do campo_texto com array
*
*
*
*
* exemplo 1:
* $arrAtributos['name'] = "teste";
$arrAtributos['obrigatorio'] = true;
$arrAtributos['habilitado'] = true;

exemplo 2:
$arrAtributos = array(
		"name" => "teste",
		"obrigatorio" => true,
		"habilitado" => false,
);
*
* exemplo 3:
* campo_texto('usucpfsolicitante', 'N', "S", '', 12, 60, '###.###.###-##', '', 'left', '', 0, 'id="usucpfsolicitante"', '');
*/

function procura_array_sig($search_value, $the_array)
{
	if (is_array($the_array))
	{
		foreach ($the_array as $key => $value)
		{
			$result = procura_array($search_value, $value);
			if (is_array($result))
			{
				$return = $result;
				array_unshift($return, $key);
				return $return;
			}
			elseif ($result == true)
			{
				$return[] = $key;
				return $return;
			}
		}
		return false;
	}
	else
	{
		if ($search_value == $the_array)
		{
			return true;
		}
		else return false;
	}
}

function monta_arvore_menu_sig($cod)
{
	$sql = 'select * from menu where mnucodpai = '.$cod;
	$saida = $db->recuperar($sql);
	if(is_array($saida)) foreach($saida as $k=>$v) ${$k}=$v;
	if ($mnusnsubmenu == 't') monta_arvore_menu($mnucod);
	else print "<td><input type='checkbox' name='cb'$mnucod</td><td>$mnucod</td><td>$mnudsc</td>";
}

/**
 * Retorna o nome por extenso de um Mês do Ano conforme número que o representa
 * @param unknown $mes
 * @return string
 */
function mes_extenso_sig($mes)
{
	if (strval($mes) == 1) return 'JANEIRO';
	else   if (strval($mes) == 2) return 'FEVEREIRO';
	else   if (strval($mes) == 3) return 'MARÇO';
	else   if (strval($mes) == 4) return 'ABRIL';
	else   if (strval($mes) == 5) return 'MAIO';
	else   if (strval($mes) == 6) return 'JUNHO';
	else   if (strval($mes) == 7) return 'JULHO';
	else   if (strval($mes) == 8) return 'AGOSTO';
	else   if (strval($mes) == 9) return 'SETEMBRO';
	else   if (strval($mes) == 10) return 'OUTUBRO';
	else   if (strval($mes) == 11) return 'NOVEMBRO';
	else   if (strval($mes) == 12) return 'DEZEMBRO';
}

/**
 * @description converte letras minisculas para maiusculas
 * @param unknown $str
 */
function str_to_upper_sig($str){
	return strtr($str,
	"abcdefghijklmnopqrstuvwxyz".
	"\x9C\x9A\xE0\xE1\xE2\xE3".
	"\xE4\xE5\xE6\xE7\xE8\xE9".
	"\xEA\xEB\xEC\xED\xEE\xEF".
	"\xF0\xF1\xF2\xF3\xF4\xF5".
	"\xF6\xF8\xF9\xFA\xFB\xFC".
	"\xFD\xFE\xFF",
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ".
	"\x8C\x8A\xC0\xC1\xC2\xC3\xC4".
	"\xC5\xC6\xC7\xC8\xC9\xCA\xCB".
	"\xCC\xCD\xCE\xCF\xD0\xD1\xD2".
	"\xD3\xD4\xD5\xD6\xD8\xD9\xDA".
	"\xDB\xDC\xDD\xDE\x9F");
}

function grid_sig($name, &$data){

	$row_count = @pg_num_rows($data);
	$column_count = @pg_numfields($data);


	$columns = "var ".$name."_columns = [\n";
	for ($i=0; $i < $column_count; $i++) {
		$columns .= "\"".@pg_field_name($data, $i)."\", ";
	}
	$columns .= "\n];\n";


	$rows = "var ".$name."_data = [\n";
	while ($result = @pg_fetch_array($data)) {
		$rows .= "[";
		for ($i=0; $i < $column_count; $i++) {
			$rows .= "\"".grid_html($result[$i])."\", ";
		}
		$rows .= "],\n";
	}
	$rows .= "];\n";

	$html = "<"."script".">\n";
	$html .= $columns;
	$html .= $rows;
	$html .= "try {\n";
	$html .= "  var $name = new Active.Controls.Grid;\n";
	$html .= "  $name.setId(\"grid1\");";
	$html .= "  $name.setRowCount($row_count);\n";
	$html .= "  $name.setColumnCount($column_count);\n";
	$html .= "  $name.setDataText(function(i, j){return ".$name."_data[i][j]});\n";
	$html .= "  $name.setColumnText(function(i){return ".$name."_columns[i]});\n";
	$html .= "  document.write($name);\n";
	$html .= "}\n";
	$html .= "catch (error){\n";
	$html .= "  document.write(error.description);\n";
	$html .= "}\n";
	$html .= "</"."script".">\n";

	print $html;
}

function grid_html_sig($msg){

	$msg = addslashes($msg);
	$msg = str_replace("\n", "\\n", $msg);
	$msg = str_replace("\r", "\\r", $msg);
	$msg = htmlspecialchars($msg);

	return $msg;
}

/**
 * permite debugar(inspecionar) trechos de código do sistema parando a execução do mesmo ou não
 * @param string $var
 * @param string $morte
 * @param string $usucpf
 */
//function dbg_sig($var=NULL, $morte=FALSE, $usucpf=''){
//	if ( $usucpf AND $usucpf != $_SESSION['usucpforigem'] )	return;
//	print "<div style=\"background-color:#EDE8E8\" >";
//	print "/*<center>-- ----- ----- ----- ----- ----- ----- ----- ----- -----\n ";
//	print "<strong>DEBUG INICIO</strong>";
//	print " ----- ----- ----- ----- ----- ----- ----- ----- ----- --</center>\n";
//	$array = debug_backtrace();
//	print("<center>linha '". $array[0]['line'] ."' do arquivo '". $array[0]['file'] ."'</center>\n");
//	print("<pre><font size='3'>\n");
//	var_dump($var);
//	print("</font></pre>\n");
//	print "<center>---- ----- ----- ----- ----- ----- ----- ----- ----- ----- \n";
//	print "<strong>DEBUG FIM</strong>";
//	print " ----- ----- ----- ----- ----- ----- ----- ----- ----- ----</center>\n";
//	print "</div>";
//	if( $morte )
//	die("<center><font color=\"#ff0000\"><strong>D I E</strong></font></center>\n*/");
//}

//define( "d", "die" );
//define( "v", "var_dump" );
//
//function ver(){
//
//	$parar = false;
//
//	$caminho = array_shift(debug_backtrace());
//	echo '
//		<div style="background-color: #ddd; color: #000; font-size: 14px;">
//		<div>
//			<label>
//				Linha   --> <label class="valorCaminho">'.$caminho['line']	  .'</label><br />
//				Caminho --> <label class="valorCaminho">'.$caminho['file']	  .'</label><br />
//			</label>
//		</div>';
//
//	echo '<div><pre>';
//
//
//	foreach( $caminho['args'] as $indice => $valor ){
//
//		if(  $valor == 'die' ) $parar = true;
//		echo '<label style="color:red; font-weight: bold;">Argumento '.( $indice + 1 ).'</label><br />';
//        if($valor == 'var_dump'){
//    		var_dump( $valor );
//        } else {
//    		print_r( $valor );
//        }
//		echo '<br /><br />';
//
//	}
//
//	echo '</div></pre></div>';
//
//
//	// Verifica se é para dar o die no final
//	if( $parar ){ die; }
//}

function monta_grafico_validacao_sig($vl0,$vl1,$vl2,$vl3,$vltotal)
{
	$vl0p = number_format($vl0*100/$vltotal,2,'.',',');
	$vl1p = number_format($vl1*100/$vltotal,2,'.',',');
	$vl2p = number_format($vl2*100/$vltotal,2,'.',',');
	$vl3p = number_format($vl3*100/$vltotal,2,'.',',');
	$txtgrafico = "$vltotal Ações no Total:<br>$vl1 <img src=\'/imagens/cor1.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Concordância ($vl1p%)<br>$vl2 <img src=\'/imagens/cor2.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Pendente ($vl2p%)<br>$vl3 <img src=\'/imagens/cor3.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Discordância ($vl3p%)<br>$vl0 <img src=\'/imagens/cor0.gif\' style=\'height:7;width:7;border:1px solid #888888;\'> Não Validado ($vl0p%)";
	$imggrafico = "<span onmouseover=\"return escape('$txtgrafico')\"><img src='/imagens/cor1.gif' style='height:10;width:".($vl1p/2).";border:1px solid #888888;border-right:0'><img src='/imagens/cor2.gif' style='height:10;width:".($vl2p/2).";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='/imagens/cor3.gif' style='height:10;width:".($vl3p/2).";border:1px solid #888888;border-left:0;border-right:0'><img src='/imagens/cor0.gif' style='height:10;width:".($vl0p/2).";border:1px solid #888888;border-left:0'></span>";
	return $imggrafico;
}


function monta_grafico_avaliacao_sig( $vl0, $vl1, $vl2, $vl3, $vltotal, $mes, $percent )
{
	$vl0p = number_format($vl0*100/$vltotal,0,'.',',');
	$vl1p = number_format($vl1*100/$vltotal,0,'.',',');
	$vl2p = number_format($vl2*100/$vltotal,0,'.',',');
	$vl3p = number_format($vl3*100/$vltotal,0,'.',',');
	/*
	 $txtgrafico = "<b>$mes</b> - Total: $vltotal Ações<br> ". ( $vl1+$vl2+$vl3 ) ." Avaliada(s) / Preenchida(s) $percent<br><table border=0><tr><td>$vl1</td><td><img src=\'/imagens/cor1.gif\' style=\'height:7;width:$vl1p;border:1px solid #888888;\'> Estável ($vl1p%)</td></tr><tr><td>$vl2</td><td><img src=\'/imagens/cor2.gif\' style=\'height:7;width:$vl2p;border:1px solid #888888;\'> Merece Atenção ($vl2p%)</td></tr><tr><td>$vl3</td><td><img src=\'/imagens/cor3.gif\' style=\'height:7;width:$vl3p;border:1px solid #888888;\'> Crítico ($vl3p%)</td></tr><tr><td>$vl0</td><td><img src=\'/imagens/cor0.gif\' style=\'height:7;width:$vl0p;border:1px solid #888888;\'> Não Avaliada ($vl0p%)</td></tr></table>";
	 $imggrafico = "<span onmouseover=\"return escape('$txtgrafico')\"><img src='/imagens/cor1.gif' style='height:10;width:".($vl1p/3).";border:1px solid #888888;border-right:0'><img src='/imagens/cor2.gif' style='height:10;width:".($vl2p/3).";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='/imagens/cor3.gif' style='height:10;width:".($vl3p/3).";border:1px solid #888888;border-left:0;border-right:0'><img src='/imagens/cor0.gif' style='height:10;width:".($vl0p/3).";border:1px solid #888888;border-left:0'></span>";
	 return $imggrafico;
	 */
	$url = '../' . $_SESSION['sisdiretorio'] . '/geral/infoMonitora.php' .
	'?vl0=' . $vl0 .
	'&vl1=' . $vl1 .
	'&vl2=' . $vl2 .
	'&vl3=' . $vl3 .
	'&vltotal=' . $vltotal .
	'&mes=' . $mes .
	'&percent=' . $percent;
	return "
		<span	onmousemove=\"SuperTitleAjax( '" . $url . "', this );\"
				onmouseout=\"SuperTitleOff( this );\">
			<img src='/imagens/cor1.gif' style='height:10;width:".($vl1p/3).";border:1px solid #888888;border-right:0'><img src='/imagens/cor2.gif' style='height:10;width:".($vl2p/3).";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='/imagens/cor3.gif' style='height:10;width:".($vl3p/3).";border:1px solid #888888;border-left:0;border-right:0'><img src='/imagens/cor0.gif' style='height:10;width:".($vl0p/3).";border:1px solid #888888;border-left:0'>
		</span>";
}

/*
 function monta_grafico_avaliacao_resultado_sig( $vl0,$vl1,$vl2,$vl3,$vltotal,$mes,$percent )
 {
 $vl0p = number_format( $vl0 * 100 / $vltotal, 0, '.', ',' );
 $vl1p = number_format( $vl1 * 100 / $vltotal, 0, '.', ',' );
 $vl2p = number_format( $vl2 * 100 / $vltotal, 0, '.', ',' );
 $vl3p = number_format( $vl3 * 100 / $vltotal, 0, '.', ',' );
 return "<b>$mes</b> - Total: $vltotal Ações<br>$percent  Avaliada(s) / Preenchida(s)<br><table border=0><tr><td>$vl1</td><td><img src=\'/imagens/cor1.gif\' style=\'height:7;width:$vl1p;border:1px solid #888888;\'> Estável ($vl1p%)</td></tr><tr><td>$vl2</td><td><img src=\'/imagens/cor2.gif\' style=\'height:7;width:$vl2p;border:1px solid #888888;\'> Merece Atenção ($vl2p%)</td></tr><tr><td>$vl3</td><td><img src=\'/imagens/cor3.gif\' style=\'height:7;width:$vl3p;border:1px solid #888888;\'> Crítico ($vl3p%)</td></tr><tr><td>$vl0</td><td><img src=\'/imagens/cor0.gif\' style=\'height:7;width:$vl0p;border:1px solid #888888;\'> Não Avaliada ($vl0p%)</td></tr></table>";
 }

 function monta_grafico_avaliacao_sig( $vl0, $vl1, $vl2, $vl3, $vltotal, $mes, $percent )
 {
 $vl0p = number_format( $vl0 * 100 / $vltotal, 0, '.', ',' );
 $vl1p = number_format( $vl1 * 100 / $vltotal, 0, '.', ',' );
 $vl2p = number_format( $vl2 * 100 / $vltotal, 0, '.', ',' );
 $vl3p = number_format( $vl3 * 100 / $vltotal, 0, '.', ',' );
 //$onmouseover = "monta_grafico_avaliacao( '$vl0', '$vl1', '$vl2', '$vl3', '$vltotal', '$mes', '$percent' );";
 //$onmouseover = "alert( 4 );";
 $imggrafico = "<span onmouseover=\"" . $onmouseover . "\"><img src='/imagens/cor1.gif' style='height:10;width:" . ( $vl1p / 3 ) . ";border:1px solid #888888;border-right:0'><img src='/imagens/cor2.gif' style='height:10;width:" . ( $vl2p / 3 ) . ";border:1px solid #888888;border-right:0;border-left:0;border-right:0;border-left:0'><img src='/imagens/cor3.gif' style='height:10;width:".($vl3p/3).";border:1px solid #888888;border-left:0;border-right:0'><img src='/imagens/cor0.gif' style='height:10;width:" . ( $vl0p / 3 ) . ";border:1px solid #888888;border-left:0'></span>";
 return $imggrafico;
 }
 */

/**
 * Monta um campo texto que pode ser preenchido com valores listados em um popup.
 *
 * Monta apenas o campo do formulário. Consulte as observações indicada
 * para cada parâmetro. É necessário que o campo identificador seja
 * renomeado para 'codigo' e a dhttp://www.maujor.com/tutorial/haslayout.phpescrição para 'descricao'. Veja restante
 * dos comentário do arquivo www/geral/textopopup.php
 *
 * @param string $nome nome atribuido ao campo no formulário
 * @param string $sql consulta a ser realizada no banco
 * @param string $titulo titulo que deverá aparecer no popup
 * @param string $maxlength quantidade máxima de caracteres
 * @param string $tamanho_campo tamanho do campo texto
 * @param string $mascara máscara do campo texto
 * @param string $tamanho_janela tamanho da janela popup no formato HEIGHTxWIDTH
 * @param string $complemento complemento para o input
 * @return void
 */
function texto_popup_sig( $nome, $sql, $titulo, $maxlength = '', $tamanho_campo = 20, $mascara = '', $tamanho_janela = '400x400', $complemento = '' , $onblur = '')
{
	global ${$nome};
	// prepara sessão
	$dados_sessao = array(
	'sql' => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
	'titulo' => $titulo,
	);
	$_SESSION['indice_sessao_texto_popup'][$nome] = $dados_sessao;
	if ( !$tamanho_janela )
	{
		$tamanho_janela = '400x400';
	}
	$tamanho = explode( 'x', $tamanho_janela );
	$onclick = ' onclick="javascript:texto_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', \'&mostraCodigo=1&label=0\' );" ';
	$mascara = $mascara == '' ? '' : ' onKeyUp= "if(event.keyCode!=9&&event.keyCode!=13)this.value=mascaraglobal( \'' . $mascara . '\',this.value);" ' ;
	//$mascara = '';
	$maxlength = $maxlength == '' ? '' : ' maxlength=' . $maxlength . ' ';
	print '<input ' . $complemento . ' ' . $mascara . $maxlength . ' onmouseover="MouseOver(this);" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);' . $onblur . '" size="' . $tamanho_campo . '" type="text" name="' . $nome . '" id="' . $nome . '" class="normal" value="' . ${$nome} . '">';
	print '<img id="img_texto_popup_' . $nome . '" src="/imagens/seta_combo.gif" width="14" height="16" style="margin:0;padding:0;" align="absmiddle" ' . $onclick . ' />';
}

/**
 * @description cria uma janela popup com campo hidden
 * @param unknown $nome
 * @param unknown $sql
 * @param unknown $titulo
 * @param string $tamanho_janela
 * @param string $explica
 */
function hidden_popup_sig( $nome, $sql, $titulo, $tamanho_janela = '400x400',$explica='' )
{
	global ${$nome};
	// prepara sessão
	$dados_sessao = array(
	'sql' => (string) $sql, // o sql é armazenado para ser executado posteriormente pela janela popup
	'titulo' => $titulo,
	);
	$_SESSION['indice_sessao_texto_popup'][$nome] = $dados_sessao;
	$tamanho = explode( 'x', $tamanho_janela );
	$onclick = ' onclick="javascript:texto_popup_abre_janela( \'' . $nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', \'&mostraCodigo=0&label=1\' );" ';
	print '<input type="hidden" name="' . $nome . '" id="' . $nome . '" class="normal" value="' . ${$nome} . '">';
	print '<span id="label_popup_'.$nome.'"></span><img src="/imagens/preview.gif"  style="margin:0 0 0 5px;padding:0;cursor:pointer;" align="absmiddle" ' . $onclick . ' />'.$explica;
}

/**
 * Cria os botões de listagem e inclusão de arquivos.
 *
 * @param string $nome Nome para controle dos dados da sessao
 * @param string $sqlListagem Se o valor for diferente de '', insere o botão para listar os arquivos de acordo com a query passada
 * @param string $campo_id_vinculo Nome do campo que será usado para a tabela de assiciação documento_processo (pjeid, ptoid, acaid, etc.)
 * @param integer $inserir Variável para verificação de inclusão ou não do botão de inserção de novos arquivos
 * @param integar $width largura da janela a ser aberta
 * @param integer $height altura da janela a ser aberta
 */
function popup_arquivo_sig( $nome, $sqlListagem = '', $campo_id_vinculo, $valor_id_vinculo, $inserir = 1, $width, $height,$tabela_vinculo='' )
{

	$dados_sessao = array(
	'nome' => $nome,
	'campo_id_vinculo' => $campo_id_vinculo,
	'valor_id_vinculo' => $valor_id_vinculo,
	'tabela_vinculo' => $tabela_vinculo
	);
	if( $sqlListagem )
	{
		print '<input type="button" value="Arquivos associados" onclick="popup_arquivo( \'L\', \'' . $nome .'\', '. $width .', ' . $height . ' )" />&nbsp;&nbsp;';
		$dados_sessao[ 'sql' ] = $sqlListagem;
	}

	if( $inserir )
	{
		print '<input type="button" value="Inserir arquivo" onclick="popup_arquivo( \'I\', \'' . $nome .'\', '. $width .', ' . $height . ' )" />';
	}

	$_SESSION[ 'indice_sessao_popup_arquivo' ][ $nome ] = $dados_sessao;
}

/**
 * Formata um numero real com a mascara ###.###.###,##
 * O segundo parâmetro indica a quantidade de casas decimais
 *
 * @param float $valor
 * @param integer $casas_decimais
 * @return string
 */
function formata_valor_sig( $valor, $casas_decimais = 2, $adicionar_zeros_direita = true )
{
	$valor = sprintf( '%.' . $casas_decimais . 'f', (float) $valor );
	$positivo = $valor > 0;
	$valores = explode( '.', $valor );
	$parte_inteira = @$valores[0];
	$parte_decimal = @$valores[1];
	while ( $parte_inteira{0} == '0' )
	{
		$parte_inteira = substr( $parte_inteira, 1 );
	}
	if ( strlen( $parte_inteira ) == 0 )
	{
		$parte_inteira = '0';
	}
	$sobra = '';
	if ( strlen( $parte_inteira ) % 3 == 2 )
	{
		$sobra .= $parte_inteira{0};
		$parte_inteira = substr( $parte_inteira, 1 );
	}
	if ( strlen( $parte_inteira ) % 3 == 1 )
	{
		$sobra .= $parte_inteira{0};
		$parte_inteira = substr( $parte_inteira, 1 );
	}
	if ( strlen( $sobra ) && $positivo && strlen( $parte_inteira ) )
	{
		$sobra .= '.';
	}
	$parte_inteira = implode( '.', str_split( $parte_inteira, 3 ) );
	if ( $adicionar_zeros_direita == false )
	{
		while ( $parte_decimal[strlen($parte_decimal)-1] == '0' )
		{
			$parte_decimal = substr( $parte_decimal, 0, -1 );
		}
	}
	if ( strlen( $parte_decimal ) > 0 )
	{
		$parte_inteira .= ',';
	}
	$retorna = $sobra . $parte_inteira . $parte_decimal;
	return $retorna;
}

/**
 * Formata um string com a mascara ###.###.###,## para um
 * ponto flutuante.
 *
 * @param float $valor
 * @param integer $casas_decimais
 * @return string
 */
function desformata_valor_sig( $valor )
{
	$valor = str_replace( '.', '', $valor );
	$valor = str_replace( ',', '.', $valor );
	return (float) $valor;
}

function eof_sig()
{
	?>
<html>
<head>
<title>Registro não encontrado</title>
</head>
<body>
<script language="Javascript">
				alert('Erro: Registro não encontrado!');
				history.back();
				</script>
</body>
</html>
	<?php
	exit();
}

/**
 * @description formata um número separando o mesmo em casas decimais. caso o mesmo seja zero mostra apenas um hifen
 * @param unknown $num
 * @param number $dec
 * @param string $simb
 * @return string
 */
function mostra_num_sig($num,$dec=0,$simb='')
{
	if ($num==0) return '-' ; else
	return $simb.' '.number_format($num,$dec,',','.');
}

/**
 * Monta campo radio com as opções determinadas pelo segundo parâmetros.
 * O formato do segundo parâmetro deve ser
 * $opcoes = array(
 *				  		"label1" => array(
 *			 				  			"valor" 	=> "valor1",
 *			 				  			"id" 		=> "id1",
 *			 				  			"callbak" 	=> "nome_da_funcao",
 *			 							)
 *				  );
 *
 * Caso queira que algum valor seja por default, selecionado:
 * $opcoes = array(
 *				  		"label1" => array(
 *			 				  			"valor" 	=> "valor1",
 *			 				  			"id" 		=> "id1",
 *			 				  			"callbak" 	=> "nome_da_funcao",
 *										"default"   => true
 *			 							)
 *				  );
 *
 * @param string $nome - Nome do campo (atributo name)
 * @param array $opcoes - Constrói as opções de radio, vide acima o exemplo.
 * @param string $alinhamento - Disposição dos campos vertical ou horizontal ( 'v'|'h' )
 * @param boolean|string $return
 */
function campo_radio_sig( $nome, $opcoes, $alinhamento, $return = false )
{
	global ${$nome};
	$valor_selecionado = ${$nome};
	if ( !is_array( $opcoes ) || !count( $opcoes ) )
	{
		return;
	}

	$disabled = '';
    $container  = '';
	$container .= '<ul style="margin: 0; padding: 0;">';
	$alinhamento = strtolower( $alinhamento ) == 'h' ? ' float:left; ' : '' ;
	$nome = htmlentities( $nome );
	foreach ( $opcoes as $label => $dados )
	{
		$valor = $dados['valor'];
		$id = $dados['id'] ? $dados['id'] : 'radio_opcao_' . $nome . '_' . $valor ;
		$checked = $valor_selecionado == $valor  ? ' checked="checked" ' : $dados[ 'default' ] ? ' checked="checked" ' : '' ;
		$valor = htmlentities( $valor );
		$onfocus = '';
		if ( $dados['callback'] )
		{
			$onfocus = ' onfocus="this.checked = true; ' . $dados['callback'] . '( this );" ';
		}
		if ( isset($dados['habilitado']) && $dados['habilitado'] == 'N' ) {
			$disabled = 'readonly="readonly" disabled="disabled" style="border:none;border-left:#888888 3px solid;color:#404040;"';
		}
		$container .= '<li style="margin:0;width:auto; list-style-type: none;' . $alinhamento . '">';
		$container .= '<input type="radio" name="' . $nome . '" value="' . $valor . '" id="' . $id . '" ' . $checked . ' ' . $onfocus . ' ' . $disabled . '/>';
		$container .= '<label for="' . $id . '">' . $label . '</label>';
		$container .= '</li>';
	}
	$container .= '</ul>';

    if ($return)
       return $container;

    echo $container;
}

/**
 * Monta campo radio com opções booleana. Os valores dos campos serão '0' ou '1'.
 *
 * @param string $nome - Nome do campo (atributo name)
 * @param string $alinhamento - Disposição dos campos vertical ou horizontal ( 'v'|'h' )
 * @param boolean|string $return
 */
function campo_radio_sim_nao_sig( $nome, $alinhamento = 'v' )
{
	global ${$nome};
	$valor_antigo = ${$nome};
	
	$isTrue = (is_bool(${$nome}) && ${$nome} ) || (is_string(${$nome}) && ${$nome} == 't') || (is_numeric(${$nome}) && ${$nome} == 1 );	
	${$nome} = $isTrue ? '1' : '0' ; // altera de booleano para string '0' ou '1'
	$opcoes = array(
	'Sim' =>
	array(
	'valor' => '1',
	'id' => null
	),
	'Não' =>
	array(
	'valor' => '0',
	'id' => null
	)
	);
	campo_radio( $nome, $opcoes, $alinhamento );
	${$nome} = $valor_antigo; // volta ao valor original
}

/**
 * esta função traduz um nome de um atributo para o seu nome fantasia
 * @param unknown $valor
 * @return string
 */
function tradutor_att_sig($valor)
{
	if (trim($valor)=='acafinalidade') return 'Finalidade';
	if (trim($valor)=='funcod') return 'Função';
	if (trim($valor)=='unicod') return 'Unidade';
	if (trim($valor)=='sfucod') return 'Subfunção';
	if (trim($valor)=='acadescricao') return 'Descrição';
	if (trim($valor)=='procod') return 'Produto';
	if (trim($valor)=='acadscproduto') return 'Descrição do Produto';
	if (trim($valor)=='unmcod') return 'Unidade de Medida';
	if (trim($valor)=='taccod') return 'Tipo de Ação';
	if (trim($valor)=='esfcod') return 'Esfera';
	if (trim($valor)=='acadetalhamento') return 'Detalhamento';
	if (trim($valor)=='acabaselegal') return 'Base legal';
	// programas
	if (trim($valor)=='tprcod') return 'Tipo do Programa';
	if (trim($valor)=='mobcod') return 'Macro objetivo';
	if (trim($valor)=='prgdscobjetivo') return 'Descrição do objetivo';
	if (trim($valor)=='prgdscpublicoalvo') return 'Público alvo';
	if (trim($valor)=='prgdscjustificativa') return 'Justificativa';
	if (trim($valor)=='prgdscestrategia') return 'Estratégia de implementação';
	if (trim($valor)=='prgdscproblema') return 'Problema';
	// indicador
	if (trim($valor)=='percod') return 'Periodicidade';
	if (trim($valor)=='bsgcod') return 'Base geográfica';
	if (trim($valor)=='inddscfonte') return 'Fonte';
	if (trim($valor)=='inddscformula') return 'Fórmula';
	if (trim($valor)=='indvlrapurado') return 'Valor apurado';
	if (trim($valor)=='inddataapuracao') return 'Data de apuração';
	if (trim($valor)=='indvlrapurado') return 'Valor apurado';

	if (trim($valor)=='indvlrfinalppa') return 'Valor ao final do PPA';
	if (trim($valor)=='indvlrfinalprg') return 'Valor final do programa';
	if (trim($valor)=='indvlrreferencia') return 'Valor de referência';
	if (trim($valor)=='indvlrapurado') return 'Valor apurado';

}

function busca_filho_sig($id)
{
	global $db;
	$sql = "select ptoid from monitora.planotrabalho where ptostatus='A'  and ptoid_pai=$id limit 1";
	//dbg($sql,1);
	$filho=$db->pegaUm($sql);
	if ($filho) return true;
	else return false;
}

/**
 * monta um link conforme parametros repassados
 * @param unknown $modulo
 * @param unknown $acao
 * @param string $parametros
 * @return string
 */
function monta_link_sig( $modulo, $acao, $parametros = '' )
{
	return '/' . $_SESSION['sisdiretorio'] . '/' . $_SESSION['sisdiretorio'] . '.php?modulo=' . $modulo . '&acao=' . urlencode( $acao ) . '&' . $parametros;
}

function mostracod_sig($id)
{
	global $db;
	// monta um código com pontos
	$sql = "select ptoid_pai, ptoordemprov from monitora.planotrabalho where ptoid=$id";
	$cod= $db->pegalinha($sql);
	if ($cod['ptoid_pai'])
	{
		// tem pai
		return mostracod($cod['ptoid_pai']).'.'.$cod['ptoordemprov'];
	}
	else
	return $cod['ptoordemprov'];

}

/**
 * @description carrega a descricao de uma unidade conforme código
 * @param unknown $cod
 * @return Ambigous <unknown, boolean, unknown>
 */
function nomeUnidade_sig($cod){
	global  $db;
	$sql = "select unidsc from public.unidade where unicod = '$cod' and unitpocod = 'U'";
	return $db->pegaUm($sql);

}

/**
 * @description verifica se determinado $cpf possui perfil de acesso a um sistema($sisid)
 * @param unknown $cpf
 * @param unknown $sisid
 * @return Ambigous <multitype:, multitype:>
 */
function verficaPerfil_sig($cpf,$sisid){
	global $db;
	$sql = "select p.pflcod as cod from seguranca.perfilusuario pu inner join seguranca.perfil p on pu.pflcod = p.pflcod where pu.usucpf = '$cpf' and p.pflstatus = 'A' and p.sisid = $sisid;";
	return $db->carregarColuna($sql);
}

/**
 * @description verifica se $cpf logado possui perfil de superUser
 * @param unknown $sisid
 * @return boolean
 */
function verficaSuperUsuario_sig($sisid){
	global $db;
	$cpf = $_SESSION['usucpf'];
	$sql = "select count(*) from seguranca.perfilusuario pu inner join seguranca.perfil p on pu.pflcod = p.pflcod where pu.usucpf = '$cpf' and p.pflstatus = 'A' and p.sisid = $sisid and p.pflnivel = 1;";

	if($db->pegaUm($sql) > 0) return true;
	else
	return false;
}


function permissaoUsuario_sig($cod){


	$perfil =  verficaPerfil($_SESSION['usucpf'],$_SESSION['sisid']);

	if(in_array(SESU_ADMINISTRADOR,$perfil)) return true;

	return in_array($cod,$perfil);
}

function permissaoReuni_sig($arrayPerfil,$unicod){
	global $db;

	$sisid = $_SESSION['sisid'];

	if(verficaSuperUsuario($sisid)) return true;

	$fase = statusFase($unicod);


	switch ($fase){
		case 'N' :
			$permissao = array(IFES_APROVACAO,IFES_CADASTRO,IFES_CONSULTA);
			break;
		case 'E' :
			$permissao = array(IFES_APROVACAO,IFES_CADASTRO,IFES_CONSULTA);
			break;
		case 'G' :
			$permissao = array(SESU_APROVACAO,SESU_CONSULTAGERAL,SESU_PARECER);
			break;
		default:
			$permissao = array();
			break;

	}

	$arrayPerfil = array_intersect($permissao,$arrayPerfil);

	foreach ($arrayPerfil as $perfil){

		if(permissaoUsuario($perfil) === true){
			return true;
			break;
		}
	}

	return false;

}

function statusFase_sig($unicod){
	global $db;
	$sql = "select coalesce(u.stscod,'N') as status  from reuni.unidadepropostastatus u inner join reuni.statusproposta s on s.stscod = u.stscod where unicod = '$unicod' order by u.upscod desc" ;
	if(($dado = $db->pegaUm($sql))!=''){
		return $dado;
	}else return 'N';
}

function mudaStatus_sig($projeto,$codUnidade,$tipo,$status){
	global $db;
	$sql = "insert into reuni.unidadepropostastatus (prjcod,unicod,unitpocod,stscod) values ($projeto,'$codUnidade','$tipo','$status')";
	$db->executar($sql);
	$db->commit();
}

function montaLinkManual_sig( $intSisId, $theme ) {
    global $db;
    require_once APPRAIZ . "includes/arquivo.inc";

    $sql = "SELECT
                sismanualonline
            FROM
                seguranca.sistema
            WHERE
                sisid = {$intSisId}";

    $arrDados = $db->pegaLinha( $sql );

    if($arrDados["sismanualonline"] == 't') {

        $sistema = $_SESSION['sisdiretorio'];
        
        $link = sprintf("http://ebserh.gov.br/web/sig/%s/manual-do-usuario", $sistema);
    } else {

        $sql = "SELECT
                    arqid
                FROM
                    public.manual as manual
                WHERE
                    manual.sisid = {$intSisId}";

        $arrDados = $db->pegaLinha( $sql );

        $booArquivoExiste = existeArquivoFisico( $arrDados["arqid"] );

        if(!$booArquivoExiste) {
            $booArquivoExiste = verificaExistenciaArquivo( $arrDados["arqid"] );
        }
        
        if($booArquivoExiste && sizeof( $arrDados ) > 0 && $arrDados["arqid"] > 0 ) {
            $intArqId = $arrDados["arqid"];

            $link = '../mostra_arquivo.php?id='.$intArqId;
        }
    }
    
    if($link) {
        $linkmanual = sprintf('<a class="icon-func" href="%s" title="Manual" target="_blank"><img src="../includes/layout/'. $theme .'/img/bt_manual.png" alt="Manual" /></a>', $link);
    }
    
    return $linkmanual;
}

function montaLinkManual2_sig( $intSisId )
{
	global $db;
	//require_once APPRAIZ . "includes/arquivo.inc";
	$strSql = "	 select
					 arqid
				 from
					 public.manual as manual
				 WHERE
					 manual.sisid = {$intSisId}
				";
	$arrDadosManual = $db->pegaLinha( $strSql );
	//$booArquivoExiste = existeArquivoFisico( $arrDadosManual["arqid"] );
	if( $booArquivoExiste && sizeof( $arrDadosManual ) > 0 && $arrDadosManual["arqid"] > 0 )
	{
		$intArqId = $arrDadosManual["arqid"];

	$linkmanual = '<a href="../mostra_arquivo.php?id='.$intArqId.'&tela_login=1"
	style="color: #555555; margin-right: 10px; margin-bottom: 5px;">Manual do Sistema</a>';

}
else
{
	$linkmanual = false;

}
	return $linkmanual;
}

/**
 * Monta abas no padrao do SIG
 *
 * @param array Um array no seguinte formato:
 *  Array
 *  (
 *      [0] => Array
 *      (
 *          ['descricao'] => TITULO DA ABA 1
 *          ['link']      => LINK DA ABA 1
 *      )
 *
 *      [1] => Array (
 *          ['descricao'] => TITULO DA ABA 1
 *          ['link']      => LINK DA ABA 1
 *          )
 *      )
 *  )
 *
 * @param $boOpenWin Boleano. Quando true esse parametro indica que as abas devem ser abertas em uma nova janela. (09-11-2010)
 */
function montarAbasArray_sig($itensMenu, $url = false, $boOpenWin = false)
{
    $url = $url ? $url : $_SERVER['REQUEST_URI'];

    if (is_array($itensMenu)) {
        $rs = $itensMenu;
    } else {
        global $db;
        $rs = $db->carregar($itensMenu);
    }

    $menu    = '<table width="95%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint abas">'
             . '<tr>'
             . '<td>'
             . '<table cellpadding="0" cellspacing="0" align="left">'
             . '<tr>';

    $nlinhas = count($rs) - 1;

    for ($j = 0; $j <= $nlinhas; $j++) {
        extract($rs[$j]);

        if ($url != $link && $j == 0)
            $gifaba = 'aba_nosel_ini.gif';
        elseif ($url == $link && $j == 0)
            $gifaba = 'aba_esq_sel_ini.gif';
        elseif ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
            $gifaba = 'aba_dir_sel.gif';
        elseif ($url != $link)
            $gifaba = 'aba_nosel.gif';
        elseif ($url == $link)
            $gifaba = 'aba_esq_sel.gif';

        if ($url == $link) {
            $giffundo_aba = 'aba_fundo_sel.gif';
            $cor_fonteaba = '#000055';
        } else {
            $giffundo_aba = 'aba_fundo_nosel.gif';
            $cor_fonteaba = '#4488cc';
        }

        $menu .= '<td height="20" valign="top"><img src="/imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td>'
               . '<td height="20" align="center" valign="middle" background="/imagens/'.$giffundo_aba.'" style="color:'.$cor_fonteaba.'; padding-left: 10px; padding-right: 10px;cursor:pointer;" onclick="'. ($boOpenWin ? 'janela(\''.$link.'\',780,600,\'newtab\')' : 'window.location=\''.$link.'\'' ) . '">';

        if ($link != $url) {
            $menu .= $descricao;
        } else {
            $menu .= $descricao . '</td>';
        }
    }

    if ($gifaba == 'aba_esq_sel_ini.gif' || $gifaba == 'aba_esq_sel.gif')
        $gifaba = 'aba_dir_sel_fim.gif';
    else
        $gifaba = 'aba_nosel_fim.gif';

    $menu .= '<td height="20" valign="top"><img src="/imagens/'.$gifaba.'" width="11" height="20" alt="" border="0"></td></tr></table></td></tr></table>';

    return $menu;
}


function alert_sig($str, $close = false, $escape = true)
{
    if ($escape) {
        $str = str_replace(array("'", "\"", "\\"),
                           array("\\'", "\\\"", "\\\\"), $str);
    }

    $close = $close ? 'self.close();' : '';

    printf('<script type="text/javascript">alert("%s");%s</script>', $str, $close);
}

/* @description window.location para PHP - Criado pois a funcao header redirect do php sobrepoem o alert
 * @author Rodrigo Rodrigues
 * @param string $strUrl
 * @param boolean $escape
 */
function locationRedirect_sig($strUrl, $escape = true, $open = false)
{
    if ($escape) {
        $strUrl = str_replace(array("'", "\"", "\\"),
                           array("\\'", "\\\"", "\\\\"), $strUrl);
    }
    if($open){
      printf('<script type="text/javascript">window.opener.location.href = "%s";</script>', $strUrl);
    }else{
      printf('<script type="text/javascript">window.location.href = "%s";</script>', $strUrl);
    }
}

/**
 * @description Remove acentuação gráfica
 * @param unknown $str
 */
function removeAcentos_sig($str){
	 $arAcentos = array(
				"/[ÂÀÁÄÃ]/"=>"A",
				"/[âãàáä]/"=>"a",
				"/[ÊÈÉË]/"=>"E",
				"/[êèéë]/"=>"e",
				"/[ÎÍÌÏ]/"=>"I",
				"/[îíìï]/"=>"i",
				"/[ÔÕÒÓÖ]/"=>"O",
				"/[ôõòóö]/"=>"o",
				"/[ÛÙÚÜ]/"=>"U",
				"/[ûúùü]/"=>"u",
				"/ç/"=>"c",
				"/Ç/"=> "C");
	// Tira o acento pela chave do array
	return preg_replace(array_keys($arAcentos), array_values($arAcentos), $str);

}


/**
 * Função que carrega os orgãos de acordo com o tipo
 * e com o município informado.
 *
 * @author Fernando A. Bagno da Silva
 * @param string $editavel -> se os campos estao habilitados ou não
 * @param string $usucpf -> cpf do usuário cadastrado, caso exista
 * /
 */
function carrega_orgao_sig($editavel, $usucpf){


	global $muncod, $regcod, $tpocod, $entid, $db;

	if ($editavel == null){
		$editavel = 'S';
	}

	if ($editavel == 'N'){

		// Se o usuário já está cadastrado no SIMEC, busca o orgão
		$sql = "SELECT
					u.entid as codigo,
					CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome
					ELSE ee.entnome END AS descricao
				FROM
					seguranca.usuario u
				INNER JOIN
					entidade.entidade ee ON
					ee.entid = u.entid
				WHERE
					u.usucpf = '{$usucpf}' AND
					ee.entorgcod <> '73000'";

		$db->monta_combo("entid",$sql,$editavel,"&nbsp;",'ajax_carrega_unidade','','','350','S', 'entid');

	}else {
		// Se for Estadual ou Outros, o usuário deverá informar o orgão
		if ( /*$tpocod == 2 ||*/ $tpocod == 4){
			echo campo_texto( 'orgao', '', $editavel, '', 50, 50, '', '', 'left', '', 0, 'id="orgao"' );

		}else{

			// Caso o tipo seja municipal, verifica o municipio escolhido
			$inner = ( $tpocod == 3 || $tpocod == 2 ) ? 'INNER JOIN
																entidade.endereco eed ON
																eed.entid = ee.entid
															 ': '';

			$uniao = ( $tpocod == 3 || $tpocod == 2 ) ? " UNION ALL (
																  SELECT
																  	999999 AS codigo,
																  	'OUTROS' AS descricao
																)" : '';

			if ( $tpocod == 2 ){
				$regcod = ($_REQUEST['regcod']) ? $_REQUEST['regcod'] : $regcod;
				$clausula = "AND eed.estuf = '" . $regcod . "'";
			}elseif ( $tpocod == 3 ){
				$clausula = "AND eed.muncod = '" . $muncod . "'";
			}
//				$clausula = $tpocod == 3 ? "AND eed.muncod = '" . $muncod . "'" : '';

			$sql = "(SELECT
						ee.entid AS codigo,
						CASE WHEN ee.entorgcod is not null THEN ee.entorgcod ||' - '|| ee.entnome
						ELSE ee.entnome END AS descricao
					FROM
						entidade.entidade ee
					INNER JOIN entidade.funcaoentidade ef ON ef.entid = ee.entid
					INNER JOIN public.tipoorgaofuncao tpf ON ef.funid = tpf.funid
						" . $inner . "
					WHERE
						tpf.tpocod = '{$tpocod}'
						" . $clausula . " AND
						( ee.entorgcod is null or ee.entorgcod <> '73000' )

					ORDER BY
						ee.entnome)" . $uniao;

			$db->monta_combo("entid",$sql,$editavel,"&nbsp;",'ajax_carrega_unidade','','','350','S', 'entid');

		}

	}

}

/**
 * Função que carrega as unidades daquele órgão (caso
 * existam)
 *
 * @author Fernando A. Bagno da Silva
 * @param integer $entid -> ID do orgão selecionado
 * @param string $editavel -> se os campos estao habilitados ou não
 * @param string $usucpf -> cpf do usuário cadastrado, caso exista
 */
function carrega_unidade_sig($entid, $editavel, $usucpf){

	global $db;

	if ($editavel == null){
		$editavel = 'S';
	}

	if ( $entid == 999999 ){

		echo campo_texto( 'orgao', '', $editavel, '', 50, 50, '', '', 'left', '', 0, 'id="orgao"' );

	} else {

		if ( $editavel == 'N' ){

			$sql = "SELECT
						u.unicod AS codigo,
						u.unicod||' - '||u.unidsc AS descricao
					FROM
						unidade u
					INNER JOIN
						seguranca.usuario su ON
						u.unicod = su.unicod
					WHERE
						usucpf = '{$usucpf}'";

			$db->monta_combo("unicod",$sql,$editavel,"",'selecionar_unidade_orcamentaria', '', '','','S','unicod');

		}else {

			if ( $entid ){

				$uo_total = $db->pegaUm( "SELECT
											count(u.unicod)
										  FROM
										  	unidade u
										  INNER JOIN
										  	entidade.entidade e ON
										  	u.orgcod = e.entorgcod
										  WHERE
										  	unistatus='A' AND
										  	unitpocod='U' AND
										  	e.entid ='{$entid}'" );

			}

			if ( $uo_total > 0 ){

				$sql = "SELECT DISTINCT
							u.unicod AS codigo,
							u.unicod||' - '||unidsc AS descricao
						FROM
							unidade u
						INNER JOIN
							entidade.entidade e ON
							orgcod = entorgcod
						WHERE
							unistatus='A' AND
							u.unitpocod='U' AND
							entid = '{$entid}'
						ORDER BY
							u.unicod";

				$db->monta_combo("unicod",$sql,$editavel,"&nbsp;",'ajax_unidade_gestora', '', '','350','S','unicod');

			} else {
				echo '<font style="color:#909090;">Este órgão não possui uma unidade.</font>';
			}
		}
	}
}

/**
 * Função que carrega a unidade gestora de uma unidade orçamentária,
 * quando existir
 *
 * @author Fernando A. Bagno da Silva
 * @param integer $unicod -> ID da unidade selecionada
 * @param string $editavel -> se os campos estao habilitados ou não
 * @param string $usucpf -> cpf do usuário cadastrado, caso exista
 */
function carrega_unidade_gestora_sig($unicod, $editavel, $usucpf){

	global $db;

	if ($editavel == null){
		$editavel = 'S';
	}

	if ( $editavel == 'N' ){

		$sql = "SELECT
					ug.ungcod AS codigo,
					ug.ungcod||' - '||ug.ungdsc as descricao
				FROM
					unidadegestora ug
				INNER JOIN
					seguranca.usuario su ON
					ug.ungcod = su.ungcod
				WHERE
					ungstatus = 'A' AND
					unitpocod = 'U' AND
					su.unicod = '".$unicod."' AND
					usucpf = '".$usucpf."'
				ORDER BY
					ungdsc";

		$db->monta_combo("ungcod",$sql,$editavel,"",'','', '', '350', 'S', 'ungcod');

	}else {

// 		if ( $unicod == '26101' || $unicod == '26000' || $unicod == '26252' ){

			$sql = "SELECT
						ungcod AS codigo,
						ungcod||' - '||ungdsc as descricao
					FROM
						unidadegestora
					WHERE
						ungstatus = 'A' AND
						unitpocod = 'U' AND
						unicod = '".$unicod."'
					ORDER BY
						ungdsc";

			if($db->carregar($sql)){
				$db->monta_combo("ungcod",$sql,$editavel,"&nbsp;",'','','', '350', 'S', 'ungcod');
			}else{
				echo '<font style="color: #909090;">Esta unidade não possui uma Unidade Gestora.</font>';
			}
	}
}

/**
 * @description Esta funcionalidade é responsável por gerar automaticamente o
 * cabeçalho conforme os paramêtros informados.
 * Por Default, o sistema gera automaticamente para ser exibido em tela no
 * tamanho 100% retornando o html do cabeçalho.
 *
 *
 * @param array $largura    - Tamanho (comprimento) do cabeçalho (default 100%)
 * @param array $tipo       - Tipo do cabeçalho (tela - gerar para tela, pdf - gerar para pdf ) (default tela)
 * @param array $img        - Endereço da imagem a ser exibida no cabecalho (default ../includes/layout/ebserh/img/logo.png)
 * @param array $usunome    - Nome do usuário a ser exibido no cabeçalho (default usuario da sessão)
 * @param array $hora       - Data e hora da impresão (default now())
 * @param array $detalhe    - Detalhe (emrpesa e departamento) a ser exibido
 *                            (default  SigEBSERH - Sistema de Informações Gerenciais da EBSERH<br>
 *                             DGPTI - Diretoria de Gestão de Processos e Tecnologia da Informação <br>
 *                             EBSERH - Empresa Brasileira de Serviços Hospitalares<br>)
 * @param array $titulo     - Título a ser exbido no cabeçalho
 * @param array $retorno    - Tipo do retorno do cabeçalho (Imprimir - o sistema imprime (echo) o cabeçalho) (default retorna o html)
 *
 * Exemplo
 *
 * array('largura'  => 100,
 *       'tipo'    => 'tela',
 *       'img'     => '../includes/layout/ebserh/img/logo.png',
 *       'usunome' => 'Fulano de Tal',
 *       'hora'    => '16/04/2015 - 16:20:42',
 *       'detalhe' => 'SigEBSERH - Sistema de Informações Gerenciais da EBSERH<br>
 *                     DGPTI - Diretoria de Gestão de Processos e Tecnologia da Informação <br>
 *                     EBSERH - Empresa Brasileira de Serviços Hospitalares<br>',
 *       'titulo'  => 'Meu Relatório',
 *       'retorno' => 'imprimir'
 */
function monta_cabecalho_relatorio_sig( $array = false )
{
    $titulo = '';

    if(!is_array($array)){
        if(!(boolean)$array){
            $largura = 100;
        }

        $largura    = $array;
        $array      = array();

        $array['largura']   = $largura;
        $array['tipo']      = 'tela';
    }

    if(!(boolean)$array['tipo']){
        $array['tipo']  = 'tela';
    }

    if(!(boolean)$array['largura']){
        $array['largura']   = 100;
    }

    if(!(boolean)$array['img']){
//        $array['img'] = '../imagens/banner_relatorios.png';
        $array['img'] = '../imagens/brasao.gif';
    }

    if(!(boolean)$array['usunome']){
        $array['usunome'] = $_SESSION['usunome'];
    }

    if(!(boolean)$array['hora']){
        $array['hora'] = date( 'd/m/Y - H:i:s' );
    }

    if(!(boolean)$array['detalhe']){
        $array['detalhe'] = 'SIMEC - Sistema Integrado de Monitoramento Execução e Controle<br>';
    }

    if((boolean)$_REQUEST["titulo"]){
        $array['titulo']  = $_REQUEST["titulo"];
    }

    if((boolean)$array['titulo']){
        $titulo = '<tr>
                        <td colspan="3" align="center" style="font-size:15px; font-weight: bold;">'.$array['titulo'].'</td>
                  </tr>';
    }

    if($array['tipo'] == 'tela'){
        $cabecalho = '<table style="border-bottom: 1px solid;
                                    padding-bottom: 2px;
                                    width:'.trim($array['largura']).'%;
                                    font-family: Calibri, Verdana, Arial, sans-serif;
                                    font-size:13px;">
                          <tbody>
                          <tr>
                              <td style="width:5%;" valign="bottom">
                                <img src="'.trim($array['img']).'" style="height:50px;">
                              </td>
                              <td style="width:45%;" valign="bottom">
                                 '.trim($array['detalhe']).'
                              </td>
                              <td style="text-align:right;" valign="bottom">
                                    Impresso por: <b>' . trim($array['usunome']) . '</b><br>
                                    Hora da Impressão: ' . trim($array['hora']) . '
                              </td>
                          </tr>
                          '.$titulo.'
                          </tbody>
                      </table>';
    }elseif($array['tipo'] == 'pdf'){
//        $cabecalho2 = '<div style="width:'.trim($array['largura']).'%;
//                                  font-family: Calibri, Verdana, Arial, sans-serif;
//                                  font-size:9px;
//                                  padding-bottom: 2px;">
//                            <div style="width:12%; ">
//                                <img src="'.trim($array['img']).'"  style="height:70px;">
//                            </div>
//                            <div style="width:60%; margin-left:13%;margin-top:-5%">
//                                '.trim($array['detalhe']).'
//                            </div>
//                            <div style="width:60%;margin-left:58%; margin-top:-3.5%" align="right">
//                                Impresso por: <b>'.trim($array['usunome']).'</b><br/>
//                                Horas da Impressão: ' . trim($array['hora']) . '
//                            </div>
//                      </div>';
        
        $cabecalho = '<table width="'.trim($array['largura']).'%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">'
				.'	<tr bgcolor="#ffffff">'
				.'		<td valign="top" width="50" rowspan="2"><img src="'.trim($array['img']).'" width="45" height="45" border="0"></td>'
				.'		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">'
				.'			SIMEC- Sistema Integrado do Ministério da Educação<br/>'
//				.'			Acompanhamento da Execução Orçamentária<br/>'
				.'			MEC / SE - Secretaria Executiva <br />'
				.'		</td>'
				.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'
				.'			Impresso por: <b>'.trim($array['usunome']).'</b><br/>'
				.'			Hora da Impressão:' . trim($array['hora']) . '<br />'
				.'		</td>'
				.'	</tr><tr bgcolor="#ffffff">'
				.'		<td colspan="2" align="center" valign="top" style="padding:5 0 5px 0;">'
				.'			<b><font style="font-size:14px;">' . $_REQUEST["titulo"] . '</font></b>'
				.'		</td>'
				.'	</tr>'
				.'</table>';
         
    }else{
        die('Erro em identificar o tipo do cabeçalho');
    }

    if($array['retorno'] == 'imprimir'){
        echo $cabecalho;
    }else{
        return $cabecalho;
    }
}

/**
 * Função que retorna o próximo valor à partir do parâmetro recebido.
 * O padrão para os valores é o seguinte: '0' à '9', e depois de 'A' à 'Z', dependendo
 * do número de caracteres do parâmetro.
 *
 * Exemplos:
 * 			valor passado por parâmetro: '99'   --->  valor retornado: 'AA'
 * 			valor passado por parâmetro: 'ABA'  --->  valor retornado: 'ABC'
 * 			valor passado por parâmetro: '677'  --->  valor retornado: '678'
 * 			valor passado por parâmetro: 'AMZZ' --->  valor retornado: 'ANAA'
 *
 * @param string $num
 * @author Cristiano Cabral
 * @since 07/04/2009
 */
function retornaseq_sig($num) {
	$qtddig = strlen($num);

    if(is_numeric($num)) {
    	$num = (integer) $num;
        $num = $num+1;

        if(strlen($num)>$qtddig) {
        	$num = str_pad('',$qtddig,'A');
            return $num;
        } else {
           	return str_pad($num,$qtddig,'0',0);
        }
	}
    else {
		//valida se é uma sequencia de caracteres permitida
        foreach (range(0,$qtddig-1) as $i){ validachar(substr($num,$i,1)); }

        $numnovo = '';
        //$num = strrev($numnovo);

        foreach (range(1,$qtddig) as $i) {
        	$ascii = ord(substr($num,$qtddig-$i,1));

            if($ascii==90 && !$para) {
            	$numnovo .= 'A';
                $para=false;
           	}
           	else {
            	if(!$para) {
                	$numnovo .= chr($ascii+1);
                    $para=true;
                } else {
                	$numnovo .= chr($ascii);
                }
            }

            try{if (strrev($numnovo)==str_pad('',$qtddig,'A')) throw new Exception('Fim da Sequencia.');} catch (Exception $e){die('Caught exception: '.  $e->getMessage() . "\n");}
		}

		return strrev($numnovo);
	}
}

/**
 * Função que valida se o caracter é permitido.
 *
 * @param char $char
 * @author Cristiano Cabral
 * @since 07/04/2009
 */
function validachar_sig($char) {
      if(in_array(ord($char),range(65,90))) {return true;} else {try{if (strrev($numnovo)==str_pad('',$qtddig,'A')) throw new Exception('Formato Inválido.');} catch (Exception $e){die('Caught exception: '.  $e->getMessage() . "\n");}}
}

/**
 * Função que coloca em extenso valores monetários.
 *
 * @param float $valor
 * @param boolean $maiusculas
 * @author Felipe de Oliveira Carvalho
 * @since 22/12/2009
 */
function valorMonetarioExtenso_sig($valor=0, $maiusculas=false) {

    $singular 	= array("centavo", "real", "mil", "milhão", "bilhão", "trilhão", "quatrilhão");
    $plural 	= array("centavos", "reais", "mil", "milhões", "bilhões", "trilhões", "quatrilhões");

    $c = array("", "cem", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos", "setecentos", "oitocentos", "novecentos");
    $d = array("", "dez", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta", "oitenta", "noventa");
    $d10 = array("dez", "onze", "doze", "treze", "quatorze", "quinze", "dezesseis", "dezesete", "dezoito", "dezenove");
    $u = array("", "um", "dois", "três", "quatro", "cinco", "seis", "sete", "oito", "nove");

    $z=0;

    $valor = number_format($valor, 2, ".", ".");
    $inteiro = explode(".", $valor);

    for($i=0;$i<count($inteiro);$i++)
        for($ii=strlen($inteiro[$i]);$ii<3;$ii++)
            $inteiro[$i] = "0".$inteiro[$i];

    $fim = count($inteiro) - ($inteiro[count($inteiro)-1] > 0 ? 1 : 2);

    for ($i=0;$i<count($inteiro);$i++) {
        $valor = $inteiro[$i];
        $rc = (($valor > 100) && ($valor < 200)) ? "cento" : $c[$valor[0]];
        $rd = ($valor[1] < 2) ? "" : $d[$valor[1]];
        $ru = ($valor > 0) ? (($valor[1] == 1) ? $d10[$valor[2]] : $u[$valor[2]]) : "";

        $r = $rc.(($rc && ($rd || $ru)) ? " e " : "").$rd.(($rd && $ru) ? " e " : "").$ru;
        $t = count($inteiro)-1-$i;
        $r .= $r ? " ".($valor > 1 ? $plural[$t] : $singular[$t]) : "";
        if ($valor == "000")$z++; elseif ($z > 0) $z--;
        if (($t==1) && ($z>0) && ($inteiro[0] > 0)) $r .= (($z>1) ? " de " : "").$plural[$t];
        if ($r) $rt = $rt . ((($i > 0) && ($i <= $fim) && ($inteiro[0] > 0) && ($z < 1)) ? ( ($i < $fim) ? ", " : " e ") : " ") . $r;
    }

    if(!$maiusculas) {
    	return($rt ? trim($rt) : "zero");
    } else {
    	return (ucwords($rt) ? ucwords(trim($rt)) : "Zero");
	}

}

/**
 * @description : Função para gravar o tema escolhido pelo usuário
 * @category Function
 * @version 1.0
 * @param string $theme		tema do usuário
 * @param string $usucpf	cpf do usuário
 * @author Juliano Meinen
 * @since 01/06/2010
 * @example : gravaThemaUsuario($_POST['theme]);
 */
function gravaThemaUsuario_sig($theme = "verde",$usucpf = null){
	global $db;

	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;

	if(!$usucpf)
		return false;

	if(!$theme)
		return false;

	$sql = "SELECT
				pruvalor
			FROM
				seguranca.parametro_usuario
			WHERE
				usucpf = '$usucpf'
			AND
				prunome = 'tema'
			AND
				prustatus = 'A'";

	$tema = $db->pegaUm($sql);

	if(!$tema){

		$sql = "INSERT
					INTO
				seguranca.parametro_usuario
					(usucpf,prunome,pruvalor,prudesc,prustatus)
				VALUES
					('$usucpf','tema','$theme','Tema do SIMEC selecionado pelo usuário.','A')";

	}else{

		$sql = "UPDATE
					seguranca.parametro_usuario
				SET
					pruvalor = '$theme'
				WHERE
					usucpf = '$usucpf'
				AND
					prunome = 'tema'
				AND
					prustatus = 'A'";
	}

	if($db->executar($sql)){
		$db->commit($sql);
		return true;
	}


}


/**
 * @description : Função que recupera o tema escolhido pelo usuário
 * @category Function
 * @version 1.0
 * @param string $usucpf	cpf do usuário
 * @author Juliano Meinen
 * @since 02/06/2010
 * @return string ( nome do tema)
 * @example : recuperaThemaUsuario();
 */
function recuperaThemaUsuario_sig($usucpf = null){
	global $db;

	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;

	if(!$usucpf)
		return false;

	$sql = "SELECT
				pruvalor
			FROM
				seguranca.parametro_usuario
			WHERE
				usucpf = '$usucpf'
			AND
				prunome = 'tema'
			AND
				prustatus = 'A'";

	$tema = $db->pegaUm($sql);

	if($tema)
		return $tema;
	else
		return false;

}

/**
 * @description : Função valida um xml com a referencia de um xsd
 * @category Function
 * @version 1.0
 * @param string $arqXml	pode ser um arquivo ou um string xml
 * @author Wesley Romualdo da Silva
 * @since 10/06/2010
 * @return array com mensagem de erro, caso ocorra.
 * @example :
 	1 - Exemplo 1
 		$arqXml = <<<XML
 		<?xml version="1.0" encoding="iso-8859-1"?>
		<catalogo>
		    <livro>
		        <isbn>8504006115</isbn>
		        <autor>George Orwell</autor>
		        <titulo>1984</titulo>
		        <paginas>302</paginas>
		    </livro>
		</catalogo>
		XML;
		$xsd = 'catalogo.xsd';
		validaXML($arqXml, $xsd);

	2 - Exemplo 2
		$arqXml = 'catalogo.xml';
		$xsd = 'catalogo.xsd';
		validaXML($arqXml, $xsd);

 */
function validaXML_sig($arqXml, $xsd){
	$dom = new DOMDocument('1.0', 'iso-8859-1');

	if( is_file( $arqXml ) ){
		$dom->load($arqXml);
	} else {
		$dom->loadXML($arqXml);
	}

    //Enable user error handling
    libxml_use_internal_errors(true);
    $arMessage = array();
    if(!$dom->schemaValidate( $xsd )) {
    	$errors = libxml_get_errors();

        foreach ($errors as $error) {
        	$arMessage[] = array('code' => $error->code,
                                 'text' => $error->message );
        }
    }
    return $arMessage;
}


/**
 * @description : Função para associar perfil ao usuário e caso esteja pendente, ativá-lo. (Descontinuada)
 * @category Function
 * @version 1.0
 * @param string/array $usucpf	cpf do usuário
 * @param string/array $perfil	perfil do usuário (normalmente é uma constante com valor numérico)
 * @param string	$sisid	(opcional) identificador do sistema
 * @param array	$arrUsuResp	(opcional) colunas e valores para inclusão na tabela de usuarioresponsabilidade do schema
 * @author Juliano Meinen
 * @since 28/07/2010
 * @return boolean ( true se tudo ocorrer normalmente / falso se houver erros )
 * @example : associaPerfilUsuarioPendente($_SESSION['usucpf'],PERFIL_COD_GESTOR,11,array("atiid" => 1);
 */
function associaPerfilUsuarioPendente_sig($usucpf,$perfil,$sisid = null,$arrUsuResp = null){
	global $db;

	if(!$sisid)
		$sisid = $_SESSION['sisid'];

	if(!$usucpf)
		return false;

	if(!$perfil)
		return false;

	if(!is_array($usucpf))
		$arrUsucpf = array($usucpf);
	else
		$arrUsucpf = $usucpf;

	if(!is_array($perfil))
		$arrPerfil = array($perfil);
	else
		$arrPerfil = $perfil;

	foreach($arrUsucpf as $usucpf):
		if(strlen(limpar_numero($usucpf)) == 11)
			$arrUsucpfTratado[] = limpar_numero($usucpf);
		else
			return false;
	endforeach;

	foreach($arrPerfil as $perfil2):
		$arrPerfilTratado[] = $perfil2;
	endforeach;

	//Passo 1 - Selecionar os Usuários Pendentes (suscod = 'A')
	$sql = "select
				usunome,
				usucpf,
				usuemail,
				ususexo,
				ususenha
			from
				seguranca.usuario
			where
				usucpf in ('".implode("','",$arrUsucpfTratado)."')
			and
				suscod = 'P'";
	$arrDadosUsuario = $db->carregar($sql);

	if(is_array($arrDadosUsuario)):
		foreach($arrDadosUsuario as $dadoUsuario):
			$remetente = array("nome" => $_SESSION['usunome'],"email" => SMTP_FROM_SIG);
			$destinatario = $dadoUsuario['usuemail'];
			$assunto = "Aprovação do Cadastro no SIG";
			$conteudo = "
				<br/>
				<span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span>
				<br/>
				<span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span>
				<br/>
				";
			$conteudo .= sprintf(
									"%s %s<p>Sua conta está ativa.</p>",
									$dadoUsuario['ususexo'] == 'M' ? 'Prezado Sr.' : 'Prezada Sra.',
									$dadoUsuario['usunome']
								);
			//Passo 2 - Atualizar os status dos usuários Pendentes (suscod = 'A')
			alterar_status_usuario( $dadoUsuario['usucpf'], 'A', "Atribuição de responsabilidade em atividade ou projeto.", $sisid );
			//Passo 3 - Enviar e-mails para usuários Pendentes de Ativação
			//enviar_email( $remetente, $destinatario, $assunto, $conteudo );
			$arrUsucpfPendentes[] = $dadoUsuario['usucpf'];
		endforeach;
	endif;

	//Passo 4 - Coletar dados do esquema por meio do sisid
	$sqlSchema = "select
				sisdiretorio
			from
				seguranca.sistema
			where
				sisid = $sisid";
	$schema = $db->pegaUm($sqlSchema);

	if(is_array($arrUsuResp)):
		foreach($arrUsuResp as $campo => $valor):
			$arrAnd[] = "$campo = '$valor'";
			$arrInsert[] = $campo;
			$arrValorInsert[] = "'$valor'";
		endforeach;
	endif;

	//Passo 5 - Desativar todos os usuários que não fazem parte dos usuários passados por parâmetro
	if( count($arrUsucpfTratado) == count($arrPerfilTratado) ):
		$n = 0;
		foreach($arrUsucpfTratado as $usuCpf):
			$sqlClear = "update
							$schema.usuarioresponsabilidade
						set
							rpustatus = 'I'
						where
							usucpf != '$usuCpf'
						and
							pflcod = {$arrPerfilTratado[$n]}
						".(count($arrAnd) ? " AND ".implode(" AND ", $arrAnd) : "")."";
			dbg($sqlClear);//$db->executar($sqlClear);
		$n++;
		endforeach;
	else:
		$sqlClear = "update
						$schema.usuarioresponsabilidade
					set
						rpustatus = 'I'
					where
						usucpf not in ('".implode("','", $arrUsucpfTratado)."')
					and
						pflcod = {$arrPerfilTratado[0]}
					".(count($arrAnd) ? " AND ".implode(" AND ", $arrAnd) : "")."";
		dbg($sqlClear);//$db->executar($sqlClear);
	endif;


	$n = 0;
	foreach($arrUsucpfTratado as $usuCpf):

		$pflcod = count($arrUsucpfTratado) == count($arrPerfilTratado) ? $arrPerfilTratado[$n] : $arrPerfilTratado[0];

		$sqlConsulta = "select
							rpuid
						from
							$schema.usuarioresponsabilidade
						where
							usucpf = '$usuCpf'
						and
							rpustatus = 'I'
						and
							pflcod = $pflcod
						".(count($arrAnd) ? " AND ".implode(" AND ", $arrAnd) : "")."";
		$rpuid = $db->pegaUm($sqlConsulta);
		if($rpuid):
			$arrSql .= "update
							$schema.usuarioresponsabilidade
						set
							rpustatus = 'A'
						where
							rpuid = $rpuid;";
		else:
			$arrSql .= "insert into $schema.usuarioresponsabilidade
							(usucpf,rpustatus,rpudata_inc,pflcod".(count($arrInsert) ? ",".implode(",",$arrInsert) : "").")
						values
							('$usuCpf','A',now(),'$pflcod'".(count($arrValorInsert) ? ",".implode(",",$arrValorInsert) : "").");";
		endif;
		$n++;
	endforeach;
}

function somar_dias_uteis_sig($str_data, $int_qtd_dias_somar = 7) {

    // Caso seja informado uma data do tipo DATETIME - aaaa-mm-dd 00:00:00. Transforma para DATE - aaaa-mm-dd.
    $str_data = substr($str_data,0,10);

    // Se a data estiver no formato brasileiro: dd/mm/aaaa. Converte-a para o padrão americano: aaaa-mm-dd.
    if ( preg_match("@/@",$str_data) == 1 ) {
        $str_data = implode("-", array_reverse(explode("/",$str_data)));
    }

    $array_data = explode('-', $str_data);
    $count_days = 0;
    $int_qtd_dias_uteis = 0;
    while ( $int_qtd_dias_uteis < $int_qtd_dias_somar ) {
        $count_days++;
        if ( ( $dias_da_semana = gmdate('w', strtotime('+'.$count_days.' day', mktime(0, 0, 0, $array_data[1], $array_data[2], $array_data[0]))) ) != '0' && $dias_da_semana != '6' ) {
            $int_qtd_dias_uteis++;
        }
    }
    return gmdate('Y-m-d',strtotime('+'.$count_days.' day',strtotime($str_data)));
}

/**
 * @description : Função substrai dias uteis
 * @param string str_data ou datetime  99/99/9999   99/99/9999 99:99:99
 * @param integer int_qtd_dias_somar  9
 * @author Rodrigo Mariano
 * @return string/date_sql 2014-08-15
 * @example : substrair_dias_uteis('18/08/2014', 1 )
 */
function substrair_dias_uteis_sig($str_data, $int_qtd_substrair = 7) {

    // Caso seja informado uma data do tipo DATETIME - aaaa-mm-dd 00:00:00. Transforma para DATE - aaaa-mm-dd.
    $str_data = substr($str_data,0,10);

    // Se a data estiver no formato brasileiro: dd/mm/aaaa. Converte-a para o padrão americano: aaaa-mm-dd.
    if ( preg_match("@/@",$str_data) == 1 ) {
        $str_data = implode("-", array_reverse(explode("/",$str_data)));
    }

    $array_data = explode('-', $str_data);
    $count_days = 0;
    $int_qtd_dias_uteis = 0;
    while ( $int_qtd_dias_uteis < $int_qtd_substrair ) {
        $count_days++;
        if ( ( $dias_da_semana = gmdate('w', strtotime('-'.$count_days.' day', mktime(0, 0, 0, $array_data[1], $array_data[2], $array_data[0]))) ) != '0' && $dias_da_semana != '6' ) {
            $int_qtd_dias_uteis++;
        }
    }
    return gmdate('Y-m-d',strtotime('-'.$count_days.' day',strtotime($str_data)));
}

/**
 *  rodrigo.rodrigues - Soma dias a uma data
 *  @param string str_data (formato 99/99/9999)
 *  @param int  int_qtd_dias ( quantidade a ser somada)
 *  @param boolean converteDate  (converte YYY-MM-DD para DD/MM/YYYY)
 *  return string (date)
 */
function somar_dias_correntes_sig($str_data, $int_qtd_dias,$converteDate = false)
{
    if($converteDate){
        $str_data = formata_data_sig($str_data);
    }

    $dia = substr($str_data,0,2);
    $mes = substr($str_data,3,2);
    $ano = substr($str_data,6,4);
    $str_data =  mktime(24*$int_qtd_dias, 0, 0, $mes, $dia, $ano);
    return date('Y-m-d',$str_data );

}

/**
 * rodrigo.rodrigues - obtendo dia do ano
 * @param string str_data (formato 9999-99-99) ou null para data atual
 * @param booean converteDate (converte 99/99/9999 para 9999-99-99)
 * return int (dia do ano )
 *
 */
function dia_do_ano_sig($str_data = null, $converteDate = false)
{
    global $db;

    if(!nao_nulo_sig($str_data)){
       $str_data = date('Y-m-d');
       $converteDate = false;
    }

    if($converteDate){
        $str_data = formata_data_sql_sig($str_data);
    }

    $sql = "select to_char(timestamp '{$str_data}', 'DDD') as DIA";
    $strDia = $db->pegaUm($sql);

    return $strDia;
}

/**
 * rodrigo.rodrigues - Retornar diferenca entre duas datas (somente dias uteis)
 * @param string dataInicial (formato 99/99/9999 99:99:99) - caso nao seja informado a funcao pega a data atual
 *        string dataFinal (formato 99/99/9999 99:99:99) - caso nao seja informado a funcao pega a data atual
 *        boolean converteDias  (false retorna a diferenca em segundos / true retorna a diferenca entre 9D 9H 9M )
 * @return mix (int ou String)
 */
function diferenca_entredatas_diasuteis_sig($dataInicial = null, $dataFinal = null, $converteDias = true){
    global $db;

    //Caso nao seja informado uma datafinal pega a data atual
    if(empty($dataInicial)){
        $dataInicial = date('d/m/Y H:i:s');
    }
    if(empty($dataFinal)){
        $dataFinal = date('d/m/Y H:i:s');
    }
    //Conversoes para formato do banco de dados
    $dateTimeInicial = formata_datetime_sql_sig($dataInicial);
    $dateTimeFinal = formata_datetime_sql_sig($dataFinal);
    $dataInicial = formata_data_sql_sig($dataInicial);
    $dataFinal = formata_data_sql_sig($dataFinal);

    // Retirado da contagem feriados e finais de semana
    $sqlDiasUteis =  sprintf("SELECT(
                                    SELECT COUNT('%s'::date+s.a) AS COUNT
                                    FROM GENERATE_SERIES(0,
                                           ( SELECT((( TO_DATE('%s','YYYY-MM-DD')) - (TO_DATE('%s','YYYY-MM-DD'))))),1) s(a)
                                             WHERE EXTRACT('DOW' FROM '%s'::date+s.a)
                                             BETWEEN 1 AND 5
                                            ) - (
                                             SELECT COUNT(EXTRACT(DOW FROM  feddata)) AS COUNT
                                             FROM FERIADOS
                                             WHERE EXTRACT (DOW FROM feddata) BETWEEN 1 AND 5
                                             AND FEDDATA BETWEEN to_date('%s','YYYY-MM-DD')
                                             AND to_date('%s','YYYY-MM-DD')
                                           )",$dataInicial,$dataFinal,$dataInicial,$dataInicial,$dataInicial,$dataFinal);

    $contDiasUteis = $db->pegaUm($sqlDiasUteis);
    // Conversoes de datas para segundos
    $diasEmSegundos = (($contDiasUteis * 24 * 60 * 60) - 86400);
    // Soma date mais time
    $diferencaTime = converteTimeSegundos($dateTimeFinal) - converteTimeSegundos($dateTimeInicial);
    $totalSegundos =  ($diasEmSegundos + $diferencaTime);

     if($converteDias == false){
       // retorna segundos
       return $totalSegundos;
    }else{
       // retorna formatacao
       return converte_segundos_em_periodo($totalSegundos,'d',true);
    }
}

/**
 * rodrigo.rodrigues - Retornar diferenca entre duas datas (dias corridos)
 * @param string dataInicial (formato 99/99/9999 99:99:99) - caso nao seja informado a funcao pega a data atual
 *        string dataFinal (formato 99/99/9999 99:99:99) - caso nao seja informado a funcao pega a data atual
 *        boolean converteDias  (false retorna a diferenca em segundos / true retorna a diferenca entre 9D 9H 9M )
 * @return mix (int ou String)
 */
function diferenca_entredatas_diascorrentes_sig($dataInicial = null, $dataFinal = null, $converteDias = true){
    global $db;

    //Caso nao seja informado uma datafinal pega a data atual
    if(empty($dataInicial)){
        $dataInicial = date('d/m/Y H:i:s');
    }
    if(empty($dataFinal)){
        $dataFinal = date('d/m/Y H:i:s');
    }
    //Conversoes para formato do banco de dados
    $dateTimeInicial = formata_datetime_sql_sig($dataInicial);
    $dateTimeFinal = formata_datetime_sql_sig($dataFinal);
    $dataInicial = formata_data_sql_sig($dataInicial);
    $dataFinal = formata_data_sql_sig($dataFinal);

    // Retirado da contagem feriados e finais de semana
    $sqlDiasUteis =  sprintf("SELECT(
                                    SELECT COUNT('%s'::date+s.a) AS COUNT
                                    FROM GENERATE_SERIES(0,
                                           ( SELECT((( TO_DATE('%s','YYYY-MM-DD')) - (TO_DATE('%s','YYYY-MM-DD'))))),1) s(a)
                                             WHERE EXTRACT('DOW' FROM '%s'::date+s.a)
                                             BETWEEN 0 AND 7
                                            )",$dataInicial,$dataFinal,$dataInicial,$dataInicial);

    $contDiasUteis = $db->pegaUm($sqlDiasUteis);
    // Conversoes de datas para segundos
    $diasEmSegundos = (($contDiasUteis * 24 * 60 * 60) - 86400);
    // Soma date mais time
    $diferencaTime = converteTimeSegundos($dateTimeFinal) - converteTimeSegundos($dateTimeInicial);
    $totalSegundos =  ($diasEmSegundos + $diferencaTime);

     if($converteDias == false){
       // retorna segundos
       return $totalSegundos;
    }else{
       // retorna formatacao
       return converte_segundos_em_periodo($totalSegundos,'d',true);
    }
}

/** rodrigo.rodrigues
 *  @description Converte um dateTime para segundos
 *  @param dateTime $dateTime - formato 99/99/9999 99:99:99
 *  @return int (segundos)
 */
function converteTimeSegundos_sig($dateTime){
   return mktime(substr($dateTime,11,2),substr($dateTime,14,2), substr($dateTime,17,2), 0, 0, 0);
}

/**
 * rodrigo.rodrigues
 * @description Converte uma quantidade de segundos para um determinado periodo 9D 9H 9M
 * @param int    total_segundos  (quantidade de segundos)
 *        string inicio  (o que deseja retornar d(dia),H(hora),i(minutos))
 * @return array de periodo
 */
function converte_segundos_em_periodo_sig($total_segundos, $inicio = 'D',  $converteString = false) {
    $comecou = false;
    $diaspormes =  ((((365*3)+366)/4)/12);
    $inicio = str_to_upper($inicio);

    if($total_segundos == 0){
        return '0 Seg';
    }

    if ($inicio == 'Y'){
    $array['anos'] = floor( $total_segundos / (60*60*24* $diaspormes *12) );
    $total_segundos = ($total_segundos % (60*60*24* $diaspormes *12));
    $comecou = true;
    }

    if (($inicio == 'M') || ($comecou == true)){
    $array['meses'] = floor( $total_segundos / (60*60*24* $diaspormes ) );
    $total_segundos = ($total_segundos % (60*60*24* $diaspormes ));
    $comecou = true;
    }

    if (($inicio == 'D') || ($comecou == true)) {
        $array['dias'] = floor($total_segundos / (60 * 60 * 24));
        $total_segundos = ($total_segundos % (60 * 60 * 24));
        $comecou = true;
    }
    if (($inicio == 'H') || ($comecou == true)) {
        $array['horas'] = floor($total_segundos / (60 * 60));
        $total_segundos = ($total_segundos % (60 * 60));
        $comecou = true;
    }
    if (($inicio == 'I') || ($comecou == true)) {
        $array['minutos'] = floor($total_segundos / 60);
        $total_segundos = ($total_segundos % 60);
        $comecou = true;
    }
    if (($inicio == 'S') || ($comecou == true)) {
        $array['segundos'] = $total_segundos;
        $comecou = true;
    }

    if($converteString == true){
       $periodo = '';
        if((isset($array['anos']))&&($array['anos'] != '')&&($array['ano'] != 0)){
            $periodo = $array['ano'].'Ano ';
        }
       if((isset($array['meses']))&&($array['meses'] != '')&&($array['meses'] != 0)){
            $periodo .= $array['meses'].' Meses ';
        }

        if((isset($array['dias']))&&($array['dias'] != '')&&($array['dias'] != 0)){
            $periodo .= $array['dias'].' Dia(s) ';
        }

        if((isset($array['horas']))&&($array['horas'] != '')&&($array['horas'] != 0)){
            $periodo .= $array['horas'].' Hr(s) ';
        }
        if((isset($array['minutos']))&&($array['minutos'] != '')&&($array['minutos'] != 0)){
            $periodo .= $array['minutos'].' Min ';
        }
        if((isset($array['segundos']))&&($array['segundos'] != '')&&($array['segundos'] != 0)){
            $periodo .= $array['segundos'].' Seg ';
        }
      return $periodo;
    }else{
       return $array;
    }
}

/**
 * @description recupera os perfis de usuario do sistema
 * @param string $usucpf
 * @param string $sisid
 * @param string $bdescricao
 * @param boolean $bignorasessao - ignora variavel de sessao $_SESSION[sistema][perfil_simulado]
 * @return Ambigous <boolean, unknown, multitype:, multitype:>
 */
function pegaPerfilGeral_sig($usucpf = null, $sisid = null, $bdescricao = false, $bignorasessao = false){
	global $db;

	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
	$sisid  = !$sisid  ? $_SESSION['sisid']  : $sisid;
        $bdsisid = descricaoSistema($sisid);
        $andSimulacao = '';

        /* Verifica se existe simulacao de um perfil especifico */
        if(isset($_SESSION[$bdsisid]['perfil_simulado'])&&($bignorasessao == false)){

            $sql = "SELECT
                        p.pflcod
                    FROM
                        seguranca.perfilusuario pu
                    NATURAL JOIN
                        seguranca.perfil p
                    WHERE
                        pu.usucpf = '$usucpf'
                    AND
                        p.sisid = $sisid
                    AND
                        pflstatus = 'A'";
            $arrPerfisUsuario = array_map('current', $db->carregar( $sql ));

            // Caso o perfil simulado seja condizente aos perfis de seu cadastro, adicione a var $andSimulacao.
            // Motivo: Tratativa de segurança para evitar o erro de tentativa de simulação de perfil inexistente a partir do firebug ou chrome.
            if( in_array( $_SESSION[$bdsisid]['perfil_simulado'][0], $arrPerfisUsuario ) ){
                $andSimulacao = ' and p.pflcod in (' .implode($_SESSION[$bdsisid]['perfil_simulado'], ',').') ';
            } else {
                $usunome = $db->pegaUm(" SELECT usunome FROM seguranca.usuario WHERE usucpf = '{$usucpf}' ");
                alert("Atenção $usunome! Não é possível simular o perfil informado.");
                locationRedirect( $_SERVER ['REQUEST_URI'] );
            }
         }

	if ($bdescricao) { //traz a descrição do Perfil para ser exibido no cabeçalho
		$sql = "select
					p.pfldsc
				from
					seguranca.perfilusuario pu
				natural join
					seguranca.perfil p
				where
					pu.usucpf = '$usucpf'
				and
					p.sisid = $sisid
                                and
					pflstatus = 'A'
                                        $andSimulacao";

		$arrPerfil = $db->carregarColuna($sql);

	} else {
		$sql = "select
					pu.pflcod
				from
					seguranca.perfilusuario pu
				inner join
					seguranca.perfil p on p.pflcod = pu.pflcod
				and
					pu.usucpf = '$usucpf'
				and
					p.sisid = $sisid
				and
					pflstatus = 'A'
                                        $andSimulacao";

		$arrPflcod = $db->carregar($sql);

		!$arrPflcod? $arrPflcod = array() : $arrPflcod = $arrPflcod;

		foreach($arrPflcod as $pflcod){
			$arrPerfil[] = $pflcod['pflcod'];
		}

	}

	return $arrPerfil ? $arrPerfil : false;
}

/**
 * @description recupera descricao de um Perfil pelo codigo
 * @param int $pflcod  - codigo do perfil
 * @param string $usucpf
 * @param string $sisid
 * @return Array
 */
function descricaoPerfil_sig($pflcod, $usucpf = null, $sisid = null){
   $usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;
   $sisid  = !$sisid  ? $_SESSION['sisid']  : $sisid;
   global $db;
   $sql = "select
					p.pfldsc
				from
					seguranca.perfilusuario pu
				natural join
					seguranca.perfil p
				where
					pu.pflcod = '$pflcod'
                                  and
					pu.usucpf = '$usucpf'
				  and
					p.sisid = $sisid
			     	  and
					pflstatus = 'A'";

    $arrPerfil = $db->pegaUm($sql);
    return $arrPerfil ? $arrPerfil : false;
}


/**
 * @description recupera descricao de um sistema
 * @param string $sisid
 * @return Array
 */
function descricaoSistema_sig($sisid = null){
   $sisid  = !$sisid  ? $_SESSION['sisid']  : $sisid;
   global $db;
   if($sisid){
      $sql = "select sisdiretorio from seguranca.sistema  where sisid = {$sisid}";
      $strSistema = $db->pegaUm($sql);
   }else{
      $strSistema = 'undefined';
   }
   return $strSistema;
}

function popupAlertaGeral_sig($texto = "Informe o texto.",$largura = "400px",$altura = "200px",$id = "id_popup_alerta", $classeCSS = null)
{ ?>
	<style>
		.popup_alerta{
				width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;}
	</style>
	<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
		<div style="width:100%;text-align:right">
			<img src="/imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
		</div>
		<div style="padding:5px;text-align:justify;">
			<?php echo $texto ?>
		</div>
	</div>
<?php }

function verificaExistenciaArquivo_sig($arqid,$nomeEsquema = null)
{
	$nomeEsquema = !$nomeEsquema ? $_SESSION['sisdiretorio'] : $nomeEsquema;
	$caminho = APPRAIZ."arquivos/".$nomeEsquema.'/'.floor($arqid/1000).'/'.$arqid;
	if(is_file($caminho)){
		return true;
	}else{
		return false;
	}
}

function verificaArquivoUsuarioTabela_sig($schema,$tbl,$usucpf = null)
{
	global $db;

	$usucpf = !$usucpf ? $_SESSION['usucpf'] : $usucpf;

	$arrWhere[] = "a.usucpf = '$usucpf'";
	$arrWhere[] = "(a.arqid / 1000) between 647 and 725";
	$arrWhere[] = "a.arqid not in(select arqid from public.arquivo_recuperado)";

	$sql = "select
				a.arqid
			from
				public.arquivo a
			inner join
				$schema.$tbl tbl ON tbl.arqid = a.arqid
			and
				(a.arqstatus = 'A'::bpchar or a.arqstatus = '1'::bpchar)
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");

	if($db->pegaUm($sql)){
		return true;
	}else{
		return false;
	}

}

function extenso_sig($valor = 0, $maiusculas = false, $masc = true) {

	$singular  = array("", "", "mil", "milhão", "bilhão", "trilhão", "quatrilhão");
	$plural    = array("", "", "mil", "milhões", "bilhões", "trilhões", "quatrilhões");

	$c = array("", "cem", "duzentos", "trezentos", "quatrocentos", "quinhentos", "seiscentos", "setecentos", "oitocentos", "novecentos");
	$d = array("", "dez", "vinte", "trinta", "quarenta", "cinquenta", "sessenta", "setenta", "oitenta", "noventa");
	$d10 = array("dez", "onze", "doze", "treze", "quatorze", "quinze", "dezesseis", "dezesete", "dezoito", "dezenove");
	if($masc){
		$u = array("", "um", "dois", "três", "quatro", "cinco", "seis", "sete", "oito", "nove");
	}else{
		$u = array("", "uma", "duas", "três", "quatro", "cinco", "seis", "sete", "oito", "nove");
	}

	$z = 0;
	$rt = "";

	$valor = number_format($valor, 2, ".", ".");
	$inteiro = explode(".", $valor);
	for($i=0;$i<count($inteiro);$i++)
		for($ii=strlen($inteiro[$i]);$ii<3;$ii++)
			$inteiro[$i] = "0".$inteiro[$i];

	$fim = count($inteiro) - ($inteiro[count($inteiro)-1] > 0 ? 1 : 2);
	for ($i=0;$i<count($inteiro);$i++) {
		$valor = $inteiro[$i];
		$rc = (($valor > 100) && ($valor < 200)) ? "cento" : $c[$valor[0]];
		$rd = ($valor[1] < 2) ? "" : $d[$valor[1]];
		$ru = ($valor > 0) ? (($valor[1] == 1) ? $d10[$valor[2]] : $u[$valor[2]]) : "";

		$r = $rc.(($rc && ($rd || $ru)) ? " e " : "").$rd.(($rd &&
		$ru) ? " e " : "").$ru;
		$t = count($inteiro)-1-$i;
		$r .= $r ? " ".($valor > 1 ? $plural[$t] : $singular[$t]) : "";
		if ($valor == "000")$z++; elseif ($z > 0) $z--;
		if (($t==1) && ($z>0) && ($inteiro[0] > 0)) $r .= (($z>1) ? " de " : "").$plural[$t];
		if ($r) $rt = $rt . ((($i > 0) && ($i <= $fim) && ($inteiro[0] > 0) && ($z < 1)) ? ( ($i < $fim) ? ", " : " e ") : " ") . $r;
	}

	if(!$maiusculas){
		return($rt ? $rt : "zero");
	} else {
		if ($rt) $rt=str_replace(" E "," e ",ucwords($rt));
		return (($rt) ? ($rt) : "Zero");
	}

}

function retornaPflcodFilhos_sig($arrPerfil){
	global $db;
	$sql = "select
				pflcodfilho
			from
				seguranca.perfilpermissao
			where
				pflcodpai in (".implode(",",$arrPerfil).")
			and
				pflcodfilho not in (".implode(",",$arrPerfil).")";
	$arrFilhos = $db->carregarColuna($sql);
	if($arrFilhos){
		foreach($arrFilhos as $f){
			$arrPerfil[] = $f;
		}
		$arrPerfil = retornaPflcodFilhos($arrPerfil);
	}
	return array_unique($arrPerfil);
}

function montaLinkRegra_sig(){
	global $db;

	if( $db->testa_superuser() ){
		$html = '<br><table class="rodape" align=\'center\' border="0" cellspacing="0" cellpadding="2" class="notprint" style="width:100%; color: #ffffff;" bgcolor="#E6AC10">
						<tr>
							<td colspan="2" height="2"></td>
						</tr>
						<tr>
							<td style="text-align: center;" valing="top"><span style="cursor:pointer;" onclick="abrirRegras( '.$_SESSION['mnuid'].' );"><b>Visualizar Regras</b></span></td>
						</tr>
					</table>';
	}
	echo $html;
}

function formata_string_sql_sig( $string ){
	if( is_array( $string ) ){
		$arString = array();
		foreach ($string as $v) {
			$v = trim($v);
			$v = !empty($v) ? "'".$v."'" : 'null';
			array_push( $arString, $v);
		}
		$string = array();
		$string = $arString;
	} else {
		$string = trim($string);
		$string = !empty($string) ? "'".$string."'" : 'null';
	}

	return $string;
}

/**
 * Monta o HTML da combo de municipios
 *
 * @param string $stNome Nome do campo que será criado HTML/PHP
 * @param string $stLabel Descrição do campo
 * @param string $habilitado habilita/desabilita combo
 * @param string $campo_busca_codigo mostra/oculta pesquisa rápida
 * @param string $campo_flag_contem mostra/oculta checkbox contem/não contem
 * @param string $stWidth Largura da combo
 * @param string $boConflict ativa/desativa noConflict jQuery
 * @author Wescley Guedes Lima
 * @since 13/07/2012
 */
function mostrarComboPopupMunicipios_sig( $stNome = 'muncod', $stLabel = 'Municípios', $habilitado = 'S', $campo_busca_codigo = true, $campo_flag_contem = true, $stWidth='400px', $boConflict = false ){

	global $$stNome;

	echo "<script>
			if(typeof jQuery == 'undefined'){
				document.writeln('<script src=\"../includes/JQuery/jquery-1.4.2.js\" type=\"text/javascript\"></sc'+'ript>');
			}
			document.writeln('<script src=\"../includes/jquery-autocomplete/jquery.autocomplete.js\" type=\"text/javascript\"></sc'+'ript>');
			document.writeln('<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/jquery-autocomplete/jquery.autocomplete.css\" />');
		  </script>";

	if($boConflict){
		echo "<script>jQuery.noConflict();</script>";
		$stConflict = "jQuery";
	}else{
		$stConflict = "$";
	}

	// Funções JS do componente
	echo "<script>

			{$stConflict}(function(){

				{$stConflict}('.mostrarCombo').click(function(){
					{$stConflict}('#{$stNome}_campo_off, #{$stNome}_campo_on').toggle();
				});

				{$stConflict}('.mostrarModalMunicipios').dblclick(function(){

					var janela = window.open('../geral/popup_municipios.php?nome={$stNome}', 'popListaMunicipios', 'height=500,width=400,scrollbars=yes,top=50,left=200');
					janela.focus();

				});

				{$stConflict}('.mostrarModalMunicipiosClick').click(function(){

					var janela = window.open('../geral/popup_municipios.php?nome={$stNome}', 'popListaMunicipios', 'height=500,width=400,scrollbars=yes,top=50,left=200');
					janela.focus();

				});

				{$stConflict}('.mostraMunicipios').live('click',function(){
					{$stConflict}('#tr_'+this.id+', #img_mais_'+this.id+', #img_menos_'+this.id).toggle();
				});

				var dominio = location.href.split('/')[2];

				{$stConflict}('#combopopup_campo_busca_{$stNome}').autocomplete('http://'+dominio+'/geral/popup_municipios.php?requisicao=buscarMunicipios', {
					cacheLength:0,
					width: 440,
					scrollHeight: 220,
					selectFirst: true,
					autoFill: true,
					extraParams: { muncod: function(){

											var dados = new Array();
											{$stConflict}('#{$stNome} option').each(function(){
												if({$stConflict}(this).val()){
											   		dados.push({$stConflict}(this).val());
											   	}
											});

											return dados;
									},
								   remover: function(){
								   		return {$stConflict}('#remover_valor_{$stNome}').attr('checked');
									}
								}

				}).result(function(event, data, formatted) {

					var codigo = data[1];
					var descricao = data[0].split(' - ')[1].split('/')[0];

		   			if({$stConflict}('#remover_valor_{$stNome}').attr('checked')){
						combo_popup_remover_item( '{$stNome}', codigo, true );
					}else{
						combo_popup_adicionar_item( '{$stNome}', codigo, descricao, true );
					}

					{$stConflict}('#combopopup_campo_busca_{$stNome}').val('');

			    });

			});

		  </script>";

	$buscaCodigo = '';
	if ( $campo_busca_codigo == true && $habilitado == 'S' )
	{
		$buscaCodigo .= '<input type="text" id="combopopup_campo_busca_' . $stNome . '" onmouseover="MouseOver( this );" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" class="normal" style="margin: 2px 0;" />';
//		$buscaCodigo .= '&nbsp;<img title="adicionar" align="absmiddle" src="/imagens/check_p.gif" onclick="combo_popup_buscar_codigo( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value );"/>';
//		$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="combo_popup_remover_item( \'' . $nome . '\', document.getElementById( \'combopopup_campo_busca_' . $nome . '\' ).value, true );"/>';
		$buscaCodigo .= '&nbsp;<input type="checkbox" name="remover_valor_'.$stNome.'" id="remover_valor_'.$stNome.'" />&nbsp;Remover';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" class="mostrarModalMunicipiosClick"/>';
	}

	$flagContem = '';
	if ( $campo_flag_contem == true )
	{
		$nomeFlagContemGlobal = $stNome . '_campo_excludente';
		global ${$nomeFlagContemGlobal};
		$flagContem .= '<input type="checkbox" id="' . $stNome . '_campo_excludente" name="' . $stNome . '_campo_excludente" value="1" ' . ( ${$nomeFlagContemGlobal} ? 'checked="checked"' : '' ) . ' style="margin:0;" />';
		$flagContem .= '&nbsp;<label for="' . $stNome . '_campo_excludente">Não contém</label>';
	}

	// HTML da tr do filtro
	echo '<tr>
			<td class="subtituloDireita mostrarCombo" valign="top">'.$stLabel.'</td>
			<td>
				<input type="hidden" id="' . $stNome . '_campo_flag" name="' . $stNome . '_campo_flag" value="1"/>
				<div id="'.$stNome.'_campo_off">
					<img border="0" align="middle" src="/imagens/combo-todos.gif" class="mostrarCombo">
				</div>
				<div id="'.$stNome.'_campo_on" style="display:none;">
					<table width="'.$stWidth.'" cellpadding="0" cellmarging="0">
						<tr>
							<td align="left">'.$buscaCodigo.'</td>
							<td align="right">'.$flagContem.'</td>
						</tr>
					</table>
					<select style="width:'.$stWidth.';" class="mostrarModalMunicipios" id="'.$stNome.'" name="'.$stNome.'[]" size="10" multiple="multiple">
						<option value="">Duplo clique para selecionar da lista</option>
					</select>
				</div>
			</td>
		  </tr>';

}

function mascara_global_sig($string,$mascara)
{
   $string = str_replace(" ","",$string);
   for($i=0;$i<strlen($string);$i++)
   {
      $mascara[strpos($mascara,"#")] = $string[$i];
   }
   return $mascara;
}

/**
 * @description Retorna o nome do Ambiente (Produção, Teste, Stage ou Dev 2)
 */
function verificaAmbienteProducao_sig()
{
	/*
	 * Se for no ambiente de Produção, Teste, Stage ou Dev 2 o e-mail deve ser enviado para os desenvolvedores
	*/
	$arrServidoresNaoErro[] = "sig.ebserh.gov.br";
	$arrServidoresNaoErro[] = "sig.ebserh.net";
	$arrServidoresNaoErro[] = "172.17.63.43";
	$arrServidoresNaoErro[] = "172.17.63.44";
	$arrServidoresNaoErro[] = "simec.ebserh.gov.br";
	$arrServidoresNaoErro[] = "simec.ebserh.net";
	return in_array($_SERVER['SERVER_NAME'],$arrServidoresNaoErro);
}

function criaComboWorkflow_sig($tpdid,$arrParametros = array())
{
	global $db;
	$arrParametros['nome'] = $arrParametros['nome'] ? $arrParametros['nome'] : "esdid";
	$arrParametros['habilitado'] = $arrParametros['habilitado'] ? $arrParametros['habilitado'] : "S";
	$arrParametros['obrigatorio'] = $arrParametros['obrigatorio'] ? $arrParametros['habilitado'] : "N";
	$arrParametros['id'] = $arrParametros['id'] ? $arrParametros['id'] : "esdid";
	$arrParametros['valor'] = $arrParametros['valor'] ? $arrParametros['valor'] : $_REQUEST['tpdid'];
	$arrParametros['onchange'] = $arrParametros['onchange'] ? $arrParametros['onchange'] : "";
	$sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = $tpdid order by esdordem";
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	if(is_array($arrParametros['option'])){
		foreach($arrParametros['option'] as $chave => $valor){
			$arrDados[] = array("codigo"=>$chave,"descricao"=>$valor);
		}
	}
	$db->monta_combo($arrParametros['nome'], $arrDados, $arrParametros['habilitado'], "Selecione...", $arrParametros['onchange'], '', '', '',$arrParametros['obrigatorio'], $arrParametros['id'],'',$arrParametros['valor']);
}

/**
* Função para montar combo com estado,mesoregião,micriregião e municipio
* passando os parametros do nome do campo,obrigatório,disabled,value
*
* Deverá utilizar jQuery
* montaComboTerritorio(
* 						"obra",
* 							array(
* 									"estuf"=>array("obrigatorio"=>"S"),
* 									"mescod"=>array("obrigatorio"=>"N","disabled"=>"S"),
* 									"miccod"=>array("disabled"=>"S"),
* 									"muncod"=>array("disabled"=>"S")
* 							)
* 					   );
* Na página colocar:
* if($_POST['requisicaoAjax']){
* 	$_POST['requisicaoAjax']();
* 	die;
* }
}
*/
function montaComboTerritorio_sig($alias,$arrParametros = array()){
	header("Content-Type: text/html; charset=ISO-8859-1",true);
	if($arrParametros){
		$_SESSION['combo_territorio']['arrParametros'][$alias] = $arrParametros;
	}
	$arrParametros['estuf']['obrigatorio'] 	= $arrParametros['estuf']['obrigatorio']  ? $arrParametros['estuf']['obrigatorio']  : "N";
	$arrParametros['estuf']['disabled']    	= $arrParametros['estuf']['disabled'] 	  ? $arrParametros['estuf']['disabled'] 	: "S";
	$arrParametros['estuf']['value'] 	  	= $arrParametros['estuf']['value'] 		  ? $arrParametros['estuf']['value'] 		: "";
	?>
	<script type="text/javascript">
	function <?php echo $alias ?>montaComboFake()
	{
		return true;
	}

	function <?php echo $alias ?>montaComboMunicipio(filtrameso,muncod){
		if($("#<?php echo $alias ?>estuf").val())
			var estuf = $("#<?php echo $alias ?>estuf").val();
		if($("#<?php echo $alias ?>mescod").val())
			var mescod = $("#<?php echo $alias ?>mescod").val();
		if($("#<?php echo $alias ?>miccod").val())
			var miccod = $("#<?php echo $alias ?>miccod").val();
		var erro = 0;
		if(!estuf){
			$("#<?php echo $alias ?>td_muncod").html("Selecione o Estado.");
			$("#<?php echo $alias ?>td_meso").html("Selecione o Estado.");
			$("#<?php echo $alias ?>td_micro").html("Selecione a Mesorregião.");
			erro = 1;
		}
		if(!mescod){
			$("#<?php echo $alias ?>td_micro").html("Selecione a Mesorregião.");
		}

		if(erro == 0){
			$.ajax({
					   type: "POST",
					   url: window.location,
					   data: 'requisicaoAjax=montaComboMunicipio&<?php echo $alias ?>estuf= ' +estuf + '&<?php echo $alias ?>mescod= ' +mescod + '&<?php echo $alias ?>miccod= ' +miccod + '&alias=<?php echo $alias ?>',
					   success: function(msg){
							$('#<?php echo $alias ?>td_muncod').html( msg );
							if(filtrameso){
								<?php echo $alias ?>montaComboMesorregiao();
							}
							if(muncod)
							{
								$('#<?php echo $alias ?>muncod').val( muncod );
							}
					   }
			 });
		}
	}

	function <?php echo $alias ?>montaComboMesorregiao(filtrameso){
		if($("#<?php echo $alias ?>estuf").val())
			var estuf = $("#<?php echo $alias ?>estuf").val();
		$.ajax({
				   type: "POST",
				   url: window.location,
				   data: "requisicaoAjax=montaComboMesorregiao&<?php echo $alias ?>estuf= " +estuf + "&alias=<?php echo $alias ?>",
				   success: function(msg){
						$('#<?php echo $alias ?>td_meso').html( msg );
						<?php echo $alias ?>montaComboMunicipio(filtrameso);

				   }
		 });
	}

	function <?php echo $alias ?>montaComboMicrorregiao(filtrameso){
		if($("#<?php echo $alias ?>mescod").val())
			var mescod = $("#<?php echo $alias ?>mescod").val();
		$.ajax({
				   type: "POST",
				   url: window.location,
				   data: "requisicaoAjax=montaComboMicrorregiao&<?php echo $alias ?>mescod= " +mescod + "&alias=<?php echo $alias ?>",
				   success: function(msg){
						 $('#<?php echo $alias ?>td_micro').html( msg );
						<?php echo $alias ?>montaComboMunicipio(filtrameso);

				   }
		 });
	}
	</script>
	<?php
	global $db;

		$texto = '<tr>
					<td class="SubTituloDireita">Estado:</td>
					<td>';
                if(isset($arrParametros['sgrid'])){
                    $sql = "

                        SELECT DISTINCT (e.estcod), e.estuf AS codigo, e.estdescricao AS descricao
                        FROM territorios.estado e
                        JOIN territorios.mesoregiao m
                                ON e.estuf = m.estuf
                        JOIN obras2.supervisao_grupo_mesoregiao sgm
                                ON sgm.mescod = m.mescod
                        WHERE sgrid = ".$arrParametros['sgrid']."
                        ORDER BY e.estdescricao ASC

                    ";
                } else{
                    $sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
                }

		$texto .=		$db->monta_combo("{$alias}estuf", $sql, $arrParametros['estuf']['disabled'], "Selecione...", $alias.'montaComboMunicipio(true);'.$alias.'montaComboFake', '', '', '',$arrParametros['estuf']['obrigatorio'], $alias."estuf",true,$arrParametros['estuf']['value']);
		$texto .= 	'</td>
				   </tr>
				   <tr>
						<td class="SubTituloDireita">Mesorregião:</td>
						<td id="'.$alias.'td_meso" >'.($arrParametros['estuf']['value'] ? montaComboMesorregiao(true,$alias) : "Selecione o Estado.").'</td>
					</tr>
				   <tr>
						<td class="SubTituloDireita">Microrregião:</td>
						<td id="'.$alias.'td_micro" >'.($arrParametros['mescod']['value'] ? montaComboMicrorregiao(true,$alias) : "Selecione a Mesorregião.").'</td>
					</tr>
					 <tr>
						<td class="SubTituloDireita">Município:</td>
						<td id="'.$alias.'td_muncod" >'.($arrParametros['estuf']['value'] ? montaComboMunicipio(true,$alias) : "Selecione o Estado.").'</td>
					</tr>';
		echo $texto;
}

function montaComboMesorregiao_sig($retorno = false, $alias = null){
	global $db;
	header("Content-Type: text/html; charset=ISO-8859-1",true);
	$alias = $_POST['alias'] ? $_POST['alias'] : $alias;
	$arrParametros = $_SESSION['combo_territorio']['arrParametros'][$alias];
	$arrParametros['mescod']['obrigatorio'] = $arrParametros['mescod']['obrigatorio'] ? $arrParametros['mescod']['obrigatorio'] 	: "N";
	$arrParametros['mescod']['disabled'] 	= $arrParametros['mescod']['disabled']    ? $arrParametros['mescod']['disabled'] 		: "S";
	$arrParametros['mescod']['value'] 		= $arrParametros['mescod']['value'] 	  ? $arrParametros['mescod']['value'] 			: "";
	$_POST[$alias.'estuf'] = $_POST[$alias.'estuf'] ? $_POST[$alias.'estuf'] : $arrParametros['estuf']['value'];
	$sql = "select mescod as codigo,mesdsc as descricao from territorios.mesoregiao where estuf ='".trim($_POST[$alias.'estuf'])."' order by mesdsc";
	$texto .=	$db->monta_combo($alias."mescod", $sql, $arrParametros['mescod']['disabled'],'Selecione...',$alias.'montaComboMicrorregiao(false);'.$alias.'montaComboFake', '', '','',$arrParametros['mescod']['obrigatorio'],$alias.'mescod',true,$arrParametros['mescod']['value']);
	if($retorno){
		return $texto;
	}else{
		echo $texto;
	}
}

function montaComboMicrorregiao_sig($retorno = false, $alias = null){
	global $db;
	header("Content-Type: text/html; charset=ISO-8859-1",true);
	$alias = $_POST['alias'] ? $_POST['alias'] : $alias;
	$arrParametros = $_SESSION['combo_territorio']['arrParametros'][$alias];
	$arrParametros['miccod']['obrigatorio'] = $arrParametros['miccod']['obrigatorio'] ? $arrParametros['miccod']['obrigatorio'] 	: "N";
	$arrParametros['miccod']['disabled'] 	= $arrParametros['miccod']['disabled'] 	  ? $arrParametros['miccod']['disabled'] 		: "S";
	$arrParametros['miccod']['value'] 		= $arrParametros['miccod']['value'] 	  ? $arrParametros['miccod']['value'] 			: "";
	$_POST[$alias.'mescod'] = $_POST[$alias.'mescod'] ? $_POST[$alias.'mescod'] : $arrParametros['mescod']['value'];
	$sql = "select miccod as codigo ,micdsc as descricao from territorios.microregiao where mescod ='".trim($_POST[$alias.'mescod'])."' order by micdsc";
	$texto .=	$db->monta_combo($alias."miccod", $sql, $arrParametros['miccod']['disabled'],'Selecione...',$alias.'montaComboMunicipio(false);'.$alias.'montaComboFake', '', '','',$arrParametros['miccod']['obrigatorio'],$alias.'miccod',true,$arrParametros['miccod']['value']);
	if($retorno){
		return $texto;
	}else{
		echo $texto;
	}
}

/**
 * @description Monta uma combo de municípios
 * @param string $retorno
 * @param string $alias
 * @return Ambigous <boolean, string>
 */
function montaComboMunicipio_sig($retorno = false, $alias = null){
	global $db;
	header("Content-Type: text/html; charset=ISO-8859-1",true);
	$alias = $_POST['alias'] ? $_POST['alias'] : $alias;
	$arrParametros = $_SESSION['combo_territorio']['arrParametros'][$alias];
	$arrParametros['muncod']['obrigatorio'] = $arrParametros['muncod']['obrigatorio']  ? $arrParametros['muncod']['obrigatorio']  : "N";
	$arrParametros['muncod']['disabled']    = $arrParametros['muncod']['disabled']     ? $arrParametros['muncod']['disabled'] 	  : "S";
	$arrParametros['muncod']['value']       = $arrParametros['muncod']['value'] 	   ? $arrParametros['muncod']['value'] 		  : "";
	$_POST[$alias.'mescod'] = $_POST[$alias.'mescod'] ? $_POST[$alias.'mescod'] : $arrParametros['mescod']['value'];
	$_POST[$alias.'miccod'] = $_POST[$alias.'miccod'] ? $_POST[$alias.'miccod'] : $arrParametros['miccod']['value'];
	$_POST[$alias.'estuf'] = $_POST[$alias.'estuf'] ? $_POST[$alias.'estuf'] : $arrParametros['estuf']['value'];
	if(trim($_POST[$alias.'mescod']) && trim($_POST[$alias.'mescod']) != "undefined"){
		$and.= " and mescod = '" .trim($_POST[$alias.'mescod'])."' ";
	}
	if(trim($_POST[$alias.'miccod']) && trim($_POST[$alias.'miccod']) != "undefined"){
		$and.= " and miccod = '" .trim($_POST[$alias.'miccod'])."' ";
	}
	if(trim($_POST[$alias.'estuf']) && trim($_POST[$alias.'estuf']) != "undefined"){
		$and.= " and estuf = '" .trim($_POST[$alias.'estuf'])."' ";
	}
	$sql = "select muncod as codigo ,mundescricao as descricao from territorios.municipio where 1=1 " .$and. " order by mundescricao";
	$texto .=	$db->monta_combo($alias."muncod", $sql, $arrParametros['muncod']['disabled'],'Selecione...','', '', '','',$arrParametros['muncod']['obrigatorio'],$alias.'muncod',true,$arrParametros['muncod']['value']);
	if($retorno){
		return $texto;
	}else{
		echo $texto;
	}
}

/**
 * Função que recupera o id do registro pela constante virtual
 * Esta função retorna um valor inteiro
 *
 * @author Wescley Lima <wescley.lima@ebserh.gov.br>
 * @version 0.1
 * @copyright  EBSERH 2014
 * @access public
 * @param String/Array $arCampos
 * @param String $separador
 * @param Boolean $ignoraSisid Ignora sisid e busca somente pela constante informada como string
 * @return Integer
 * @example elabrev/www/_constantes.php
 */
function getIdConstanteVirtual_sig($arCampos, $separador = '_', $ignoraSisid = false)
{

    if(is_string($arCampos)) {
        $constante = $arCampos;
        unset($arCampos);
        $arCampos['constantevirtual'] = $constante;
    }

    if(empty($arCampos['sisid']) && $_SESSION['sisid']) {
        $arCampos['sisid'] = $_SESSION['sisid'];
    }

	if($ignoraSisid === true) {
		unset($arCampos['sisid']);
	}

    global $db;
    if(empty($db)){
       include_once "config.inc";
       include_once APPRAIZ . "includes/classes_simec.inc";
       $db = new cls_banco();
    }

    if($arCampos){

        $arrConstantes = array(
                           'seguranca_sistema_sisid_' => array('tabela'=>'seguranca.sistema','pk' => 'sisid', 'sisid' => false),
                           'workflow_tipodocumento_tpdid_' => array('tabela'=>'workflow.tipodocumento','pk' => 'tpdid', 'sisid' => true),
                           'workflow_estadodocumento_esdid_' => array('tabela'=>'workflow.estadodocumento','pk' => 'esdid', 'sisid' => false),
                           'workflow_acaoestadodoc_aedid_' => array('tabela'=>'workflow.acaoestadodoc','pk'=> 'aedid', 'sisid' => false),
                           'workflow_tipofluxo_tpfid_' => array('tabela'=>'workflow.tipofluxo','pk'=> 'tpfid', 'sisid' => true),
                           'seguranca_perfil_pflcod_' => array('tabela'=>'seguranca.perfil','pk' => 'pflcod', 'sisid' => true),
                           'seguranca_menu_mnuid_' => array('tabela'=>'seguranca.menu','pk' => 'mnuid', 'sisid' => true),
                           'seguranca_aba_abacod_' => array('tabela'=>'seguranca.aba','pk' => 'abacod', 'sisid' => false),
                           'entidade_funcao_funid_' => array('tabela'=>'entidade.funcao','pk' => 'funid', 'sisid' => false ),
                           'sap_localizacao_locid_' => array('tabela'=>'sap.localizacao','pk' => 'locid', 'sisid' => false),
                           'sap_situacaobem_sbmid_' => array('tabela'=>'sap.situacaobem', 'pk' => 'sbmid', 'sisid' => false)
                           );

      $tabela = '';
      $pk = '';
      $sisid = false;
      $erro = 0;
      $constanteVirtual =  $arCampos['constantevirtual'];
      foreach($arrConstantes as $key => $arContante):
          if(strpos($constanteVirtual,$key) === 0){
             $tabela = $arContante['tabela'];
             $pk = $arContante['pk'];
             $sisid = $arContante['sisid'];
             break;
          }
      endforeach;

      if($tabela != ''){
          $arDados = $arCampos;
          $andWhere = '';
          if(!$sisid) {
             unset($arDados['sisid']);
          }
          unset($arDados['constantevirtual']);
          if($arDados){
              foreach($arDados as $key => $strWhere):
                if(!$strWhere){
                    unset($arDados[$key]);
                }else{
                   $andWhere .= " AND $key = '$strWhere' ";
                }
              endforeach;
          }
         $strSql = "SELECT $pk FROM $tabela WHERE constantevirtual = '$constanteVirtual' $andWhere";
         $arrResultado = $db->carregar($strSql,null,900);
         if($arrResultado && count($arrResultado) == 1){
            return  $arrResultado[0][$pk];
         }else{
           $erro = 1;
         }
      }else{
          $erro = 1;
      }

      /* Metodo do Wescley */
      if($erro == 1){

        $arConstante = explode($separador, $arCampos['constantevirtual']);
        $esquema = $arConstante[0];
        $tabela = $arConstante[1];
        $pk = $arConstante[2];

      $sql = "  SELECT
                    column_name
                FROM
                    information_schema.columns
                WHERE
                table_schema = '{$esquema}'
                AND table_name = '{$tabela}'; ";

                // Recupera todos os campos da tabela
                $rsCamposTabela = $db->carregarColuna($sql);

                if ($rsCamposTabela) {
                    foreach ($arCampos as $campo => $valor) {
                        // Tira os campos que não existem na tabela
                        if (!in_array($campo, $rsCamposTabela)) {
                            unset($arCampos[$campo]);
                        }
                    }

                    $sql = "
                SELECT
                    kc.column_name,
                    kc.constraint_name
                FROM
                    information_schema.table_constraints tc,
                    information_schema.key_column_usage kc
                WHERE
                    tc.constraint_type = 'UNIQUE'
                    AND kc.constraint_name = tc.constraint_name
                    AND kc.table_schema = '{$esquema}'
                    AND kc.table_name = '{$tabela}'; ";

                    // Recupera os campos do tipo unique
                    $rsUnique = $db->carregar($sql);

                    if ($rsUnique) {
                        foreach ($rsUnique as $unique) {
                            // Verifica se todas os campos do tipo unique foram informados
                            if (!array_key_exists($unique['column_name'], $arCampos)) {
                                $arUniqueNaoEncontrada[$unique['constraint_name']][] = $unique['column_name'];
                            }
                            $arUnique[$unique['constraint_name']][] = $unique['column_name'];
                        }
                    }

                    if ($arUniqueNaoEncontrada) {
                        $txtErroUnique = '';
                        foreach ($arUniqueNaoEncontrada as $constraint_name => $column_name) {
                            $txtErroUnique .= '<p><font color="red">O(s) campo(s) "' . implode(', ', $arUniqueNaoEncontrada[$constraint_name]) . '" precisa(m) ser informado(s)!<br/></font>';
                            $txtErroUnique .= '<font color="red" size="-1">&nbsp;&nbsp;TABLE(' . $esquema . '.' . $tabela . ')<br/>';
                            $txtErroUnique .= '&nbsp;&nbsp;UNIQUE(' . implode(',', $arUnique[$constraint_name]) . ')<br/>';
                            $txtErroUnique .= '&nbsp;&nbsp;CONSTRAINT_NAME(' . $constraint_name . ')<br/></font></p>';
                        }

                        return $txtErroUnique;
                    }

                    if ($arCampos) {
                        // Monta array de where
                        foreach ($arCampos as $campo => $valor) {
                            $arWhere[] = " {$campo} = '{$valor}' ";
                        }
                    }

                    // Verifica se pode execurar o SQL
                    if ($arWhere) {
                        $sql = "
                        SELECT
                            {$pk}
                        FROM
                            {$esquema}.{$tabela}
                            " . ($arWhere ? ' WHERE ' . implode(' AND ', $arWhere) : '');

                        return $db->pegaUm($sql);
                    }
                }
         }
    }

    return false;
}

/**
 * Função que monta o campo para as constantes virtuais
 * Esta função imprime somente o html do imput
 * A logica para montar a constante eh:
 * ESQUEMA + SEPARADOR + TABELA + SEPARADOR + CHAVE PRIMARIA + SEPARADOR + TEXTO LIVRE
 *
 * @author Wescley Lima <wescley.lima@ebserh.gov.br>
 * @version 0.1
 * @copyright  EBSERH 2014
 * @access public
 * @param String $esquema
 * @param String $tabela
 * @param String $separador
 * @param Integer $maxlength
 * @param String $value Valor do atributo HTML a ser carregado
 * @param Boolean $valida Indicador para validação de constante virtual existente
 * @return String
 * @example seguranca/modulos/sistema/perfil/cadperfil.inc
 */
function setCampoConstanteVirtual_sig($esquema, $tabela, $separador = '_', $maxlength = 500, $value = null, $valida = true, $sisid = null)
{
    if($esquema && $tabela){

        global $db;

        if(empty($db)){
            include_once "config.inc";
            include_once APPRAIZ . "includes/classes_simec.inc";
            $db = new cls_banco();
        }

        $sql = "
            SELECT EXISTS (
                SELECT
                    1
                FROM
                    pg_catalog.pg_namespace
                WHERE
                    nspname = '{$esquema}'
            );
        ";

        $bo_existe_esquema = $db->pegaUm($sql);

        // Verifica se existe o esquema
        if($bo_existe_esquema!='t'){
            return '<font color="red">O esquema "' . $esquema . '" não encontrado!</font>';
        }

        $sql = "
            SELECT EXISTS (
                SELECT
                    1
                FROM
                    pg_catalog.pg_tables
                WHERE
                    schemaname = '{$esquema}'
                    AND tablename = '{$tabela}'
            );
        ";

        $bo_existe_tabela = $db->pegaUm($sql);

        // Verifica se existe a tabela
        if($bo_existe_tabela!='t'){
            return '<font color="red">Tabela "' . $tabela . '" não encontrada!</font>';
        }

        $sql = "
            SELECT
                kc.column_name
            FROM
                information_schema.table_constraints tc,
                information_schema.key_column_usage kc
            WHERE
                tc.constraint_type = 'PRIMARY KEY'
                AND kc.constraint_name = tc.constraint_name
                AND kc.table_schema = '{$esquema}'
                AND kc.table_name = '{$tabela}';
        ";

        $chave_primaria = $db->pegaUm($sql);

        // Verifica se existe a chave primaria
        if(!$chave_primaria){
            return '<font color="red">A chave primária não existe!</font>';
        }

        $sql = "
            SELECT
                column_name,
                is_nullable,
                data_type,
                character_maximum_length
            FROM
                information_schema.columns
            WHERE
                table_schema = '{$esquema}'
                AND table_name = '{$tabela}';
        ";

        $rsCampos = $db->carregar($sql);

        $bo_existe_campo_sisid = false;
        $bo_existe_campo_constante = false;

        foreach($rsCampos as $campo){
            if($campo['column_name'] == 'constantevirtual'){
                $bo_existe_campo_constante = true;
            }

            if($campo['column_name'] == 'sisid'){
                $bo_existe_campo_sisid = true;
            }
        }

        if(!$bo_existe_campo_constante){

            $script = "
                <img src='/imagens/mais.gif' id='img_constante_mais_menos' onclick='getScriptBancoConstante()' />&nbsp;Gerar script
                    <div style='display:none;color:blue;margin-top:15px;' id='div_script_constante'>
                        ALTER TABLE {$esquema}.{$tabela}<br/>
                        &nbsp;&nbsp;ADD COLUMN constantevirtual character varying({$maxlength});
                        <br/><br/>
                        ALTER TABLE {$esquema}.{$tabela}<br/>
                        &nbsp;&nbsp;ADD CONSTRAINT constantevirtual_sisid_unique UNIQUE(" . ($bo_existe_campo_sisid ? 'sisid,' : '') . "constantevirtual);
                    </div>
            ";

            return '<font color="red">O campo "constantevirtual" não existe na tabela "'.$tabela.'"!</font><br/>'.$script;
        }

        // Monta a parte fixa da constante
        $constantevirtual_fixo = $esquema.$separador.$tabela.$separador.$chave_primaria.$separador;

        // Verifica se foi passado algum valor como parametro para remover a parte fixa da constante
        if(!is_null($value)) {
            $arConstanteVirtual = explode($separador, $value);
            unset($arConstanteVirtual[array_search($esquema, $arConstanteVirtual)]);
            unset($arConstanteVirtual[array_search($tabela, $arConstanteVirtual)]);
            unset($arConstanteVirtual[array_search($chave_primaria, $arConstanteVirtual)]);
            $value = implode($separador, $arConstanteVirtual);
        }

        if($valida) {
            $validador = "verificaConstanteVirtual(this, '{$separador}')";
        } else {
            $validador = "";
        }

        if(is_null($sisid)) {
            $sisid = $_SESSION['sisid'];
        }

        return  '<input type="hidden" name="constantevirtual_erro" id="constantevirtual_erro" value="" />'.
                    '<input type="hidden" name="constantevirtual_pk" id="constantevirtual_pk" value="'.$chave_primaria.'" />'.
                    '<input type="hidden" name="constantevirtual_separador" id="constantevirtual_separador" value="'.$separador.'" />'.
                    '<input type="hidden" name="constantevirtual_sisid" id="constantevirtual_sisid" value="'.$sisid.'" />'.
                    '<input type="hidden" name="constantevirtual_fixo" id="constantevirtual_fixo" value="'.$constantevirtual_fixo.'" />'.
                    '<input type="hidden" name="constantevirtual" id="constantevirtual" value="" />'.$constantevirtual_fixo.
                    campo_texto( 'constantevirtual_texto', 'N', 'S', '', 46, $maxlength-strlen($constantevirtual_fixo), '', '', 'left', '', 0, "id='constantevirtual_texto'", '', $value, $validador ).
                    '<div id="div_constante_virtual_success" style="display:none;color:green;"><img src="/imagens/check.jpg"  />&nbsp;Constante disponível.</div>'.
                    '<div id="div_constante_virtual_erro" style="display:none;color:red;"><img src="/imagens/exclui_p.gif" style="cursor:pointer" alt="Limpar" title="Limpar" onclick="limpaCampoConstanteVirtual()"  />&nbsp;Constante não disponível.</div>'.
                    '<div id="div_constante_virtual" style="display:none;color:red;"><img src="/imagens/carregando.gif" style="margin-right:5px;" height="20" align="absmiddle" />&nbsp;Verificando disponibilidade, por favor aguarde... </div>';

	}

	return 'Os parametros "esquema" e "tabela" são obrigatórios!';
}


function extrairTag_sig($tagIni, $tagFim, $html, $somenteSeEncontrar = true, $incluirTag = false){
	$somenteSeEncontrar = ($somenteSeEncontrar ? $somenteSeEncontrar : false);
	$incluirTag 		= ($incluirTag ? $incluirTag : false);

	$tamTagIni  = strlen($tagIni);
	$iniRecorte = stripos($html, $tagIni);
	$tamTagFim  = strlen($tagFim);
	$fimRecorte = stripos($html, $tagFim);

	if ( ($somenteSeEncontrar == true && ($iniRecorte === false || $fimRecorte === false)) ||  $iniRecorte === false){
		return false;
	}

	if ( $incluirTag == false ){
		return substr($html, ($iniRecorte + $tamTagIni), ($fimRecorte - $tamTagIni));
	}else{
		$fimRecorte = ($fimRecorte ? $fimRecorte : (strlen($html) - $tamTagIni));
		return substr($html, ($iniRecorte), ($fimRecorte + $tamTagFim));
	}
}
/* Insere o um link para a página inicial de acordo com o perfil do usuário */
function setPerfilLink_sig( $perfil, $mnuid, $sisid ){
	global $db;
	$menuId	    		=  $mnuid;
	$verifica           =  (int)$db->pegaUm("select count(1) from seguranca.perfillink where sisid = {$sisid} and pflcod = {$perfil}");
	if($verifica == 0){
	$sql 	    		=   "INSERT into perfillink
							(mnuid, sisid, pflcod)
							VALUES ($menuId, $sisid, $perfil)";
	}else{
	$sql                =   "UPDATE seguranca.perfillink set mnuid = {$mnuid} where sisid = {$sisid} and pflcod = {$perfil}";
	}
	if( $db->executar($sql) ){
		$db->commit();
		return true;
	}else{
		$db->rollback();
		return false;
	}
}

/* Resgata link inicial de acordo com o perfil sistema */
function getPerfilLink_sig($cpf = null, $sisid = null){

    global $db;
    $sisid = ($sisid)? $sisid : $_SESSION['sisid'];
    $cpf = ($cpf)? $cpf : $_SESSION['usucpf'];
    $arrPerfil = pegaPerfilGeral($cpf, $sisid);
    //dbg($arrPerfil,1);

    // Usuário é super usuário e possue apenas um perfil.
 	if($sisid && $cpf){
 	   $verificaSuper = $db->pegaLinha("select p.pflcod, m.mnulink from seguranca.perfil p
										left join seguranca.perfillink pl on pl.sisid = p.sisid
										and pl.pflcod = p.pflcod
										left join seguranca.menu m on
										m.mnuid = pl.mnuid
										where p.pflstatus = 'A'
										and p.sisid = {$sisid}
										and upper(p.pfldsc) = 'SUPER USUÁRIO'");

       if( in_array($verificaSuper['pflcod'], $arrPerfil) ){
           return is_null($verificaSuper['mnulink'])? false : $verificaSuper['mnulink'];

 	   // Possue mais de um perfil ou não é super usuário. Obtém o perfil de menor nível mais relevante.
	   }else{
 	   	   $pegaPerfilMenor = $db->pegaUm("SELECT a.pflcod as idPerfil
												FROM seguranca.perfil a, seguranca.perfilusuario b
											WHERE
												a.pflcod = b.pflcod
												AND a.sisid  = {$sisid}
												AND b.usucpf = '{$cpf}'
														ORDER BY a.pflnivel asc, a.pflcod asc LIMIT 1");
			// Verifica se há algum perfil link.
			$perfilLink = $db->pegaUm ( "SELECT d.mnulink AS link
										 FROM
											seguranca.perfil a, seguranca.perfilusuario b,
											seguranca.perfillink c, seguranca.menu d
										 WHERE
										    a.pflcod = b.pflcod
										    AND b.pflcod = c.pflcod
										    AND c.mnuid = d.mnuid
											AND a.pflcod = {$pegaPerfilMenor}
											AND a.sisid  = {$sisid}
											AND b.usucpf = '{$cpf}' ");
											return (!$perfilLink) ? false : $perfilLink;
 	   	}
 	} else { // Cpf e Sisid inexistente.
 	   return false;
 	}
}

/**
 *  Detecta qual o dispositivo que está acessando o aplicativo
 * @author Marcelo Santos <marcelo.santos@ebserh.gov.br>
 * @version 0.1
 * @copyright  EBSERH 2014
 * @access public
 * @example painel/modulos/principal/painel_controle.inc
 */
function retornarTipoDispositivo_sig() {
	$tablet_browser = 0;
	$mobile_browser = 0;
	$body_class = 'desktop';

	if (preg_match('/(tablet|ipad|playbook)|(android(?!.*(mobi|opera mini)))/i', strtolower($_SERVER['HTTP_USER_AGENT']))) {
		$tablet_browser++;
		$body_class = "tablet";
	}

	if (preg_match('/(up.browser|up.link|mmp|symbian|smartphone|midp|wap|phone|android|iemobile)/i', strtolower($_SERVER['HTTP_USER_AGENT']))) {
		$mobile_browser++;
		$body_class = "mobile";
	}

	if ((strpos(strtolower($_SERVER['HTTP_ACCEPT']),'application/vnd.wap.xhtml+xml') > 0) or ((isset($_SERVER['HTTP_X_WAP_PROFILE']) or isset($_SERVER['HTTP_PROFILE'])))) {
		$mobile_browser++;
		$body_class = "mobile";
	}

	$mobile_ua = strtolower(substr($_SERVER['HTTP_USER_AGENT'], 0, 4));
	$mobile_agents = array(
			'w3c ','acs-','alav','alca','amoi','audi','avan','benq','bird','blac',
			'blaz','brew','cell','cldc','cmd-','dang','doco','eric','hipt','inno',
			'ipaq','java','jigs','kddi','keji','leno','lg-c','lg-d','lg-g','lge-',
			'maui','maxo','midp','mits','mmef','mobi','mot-','moto','mwbp','nec-',
			'newt','noki','palm','pana','pant','phil','play','port','prox',
			'qwap','sage','sams','sany','sch-','sec-','send','seri','sgh-','shar',
			'sie-','siem','smal','smar','sony','sph-','symb','t-mo','teli','tim-',
			'tosh','tsm-','upg1','upsi','vk-v','voda','wap-','wapa','wapi','wapp',
			'wapr','webc','winw','winw','xda ','xda-');

	if (in_array($mobile_ua,$mobile_agents)) {
		$mobile_browser++;
	}

	if (strpos(strtolower($_SERVER['HTTP_USER_AGENT']),'opera mini') > 0) {
		$mobile_browser++;
		//Check for tablets on opera mini alternative headers
		$stock_ua = strtolower(isset($_SERVER['HTTP_X_OPERAMINI_PHONE_UA'])?$_SERVER['HTTP_X_OPERAMINI_PHONE_UA']:(isset($_SERVER['HTTP_DEVICE_STOCK_UA'])?$_SERVER['HTTP_DEVICE_STOCK_UA']:''));
		if (preg_match('/(tablet|ipad|playbook)|(android(?!.*mobile))/i', $stock_ua)) {
			$tablet_browser++;
		}
	}
	if ($tablet_browser > 0) {
		// Si es tablet has lo que necesites
		return "TABLET";
	}
	else if ($mobile_browser > 0) {
		// Si es dispositivo mobil has lo que necesites
		return "CELULAR";
	}
	else {
		// Si es ordenador de escritorio has lo que necesites
		return "DESKTOP";
	}
}

/**
 * sig_json_encode com checagem de codificação
 *
 * @param mixed $dados A serem serializados
 * @return string Dados serializados em json
 */
function sig_json_encode ($dados) {
    if (is_array($dados)){
		foreach ($dados as $k => $val){
			unset( $dados[$k] );
			$k 	 = sig_utf8_encode($k);
			$val = (is_array( $val ) ? sig_json_encode ( $val ) : sig_utf8_encode($val));

			$dados[$k] = $val;
		}
//         array_walk_recursive($dados, function (&$value){
//             $value = sig_utf8_encode($value);
//         });
    } elseif (is_scalar($dados)) {
        $dados = sig_utf8_encode($dados);
    }

    return json_encode($dados);
}

/**
 * sig_utf8_encode - Converte string que não é UTF-8 para UTF-8
 *
 * @param string $str
 * @return string
 */
function sig_utf8_encode ($str) {
    return (mb_detect_encoding($str . 'x', 'UTF-8', TRUE) === FALSE) ? ($str) : $str;
}

/**************
 * NÃO modifique essa função sem antes falar com o FELIPE CHIAVICATTI
 */
function sig_json_encode_iso($dados, $comAspas = false){
	$comAspas = ( $comAspas == false ? $comAspas : (bool) $comAspas);

	if ( is_string( $dados ) ){
		$html = ($comAspas ? '"' . $dados . '"' : "&quot;{$dados}&quot;");
	}elseif ( is_numeric( $dados ) ){
		$html = $dados;
	}elseif ( is_array( $dados ) ){
		$html = array();
		$cont = 0;
		$tipo = 'array';

		foreach ($dados as $k => $v){
			$arValor[] 		= sig_json_encode_iso($v, $comAspas);
			$arChaveValor[] = ($comAspas ? '"' . $k . '":' . sig_json_encode_iso($v, $comAspas) : "&quot;" . $k . "&quot;:" . sig_json_encode_iso($v, $comAspas));

			if ( $k !== $cont ){
				$tipo = 'object';
			}
			$cont++;
		}

		$arValor 	  = (count($arValor) ? $arValor : array());
		$arChaveValor = (count($arChaveValor) ? $arChaveValor : array());

		if ( $tipo == 'array' ){
			$html = "[" . implode(",", $arValor) . "]";
		}else{
			$html = "{" . implode(",", $arChaveValor) . "}";
		}
	}elseif ( is_object( $dados ) ){
		$vars = array_keys( get_object_vars($dados) );
		$html = array();
		foreach ($vars as $var) {
			$html[] = sig_json_encode_iso( (string) $var, $comAspas) . ":" . sig_json_encode_iso($dados->$var, $comAspas);
		}

		$html = "{" . implode(",", $html) . "}";
	}elseif ( is_null( $dados ) ){
		$html = 'null';
	}elseif ( is_bool( $dados ) ){
		$html = ($dados ? 'true' : 'false');
	}

	return $html;
}

/**************
 * NÃO modifique essa função sem antes falar com o FELIPE CHIAVICATTI
*/
function sig_utf8_decode_deep($dados) {
	if (is_string($dados)) {
// 		$dados = ($dados);
		$dados = ((mb_detect_encoding($dados . 'x', 'UTF-8', TRUE) !== FALSE) ? ( $dados ) : $dados);
	} else if (is_array($dados)) {
		foreach ($dados as $k => $v) {
			unset( $dados[$k] );

			$k 		   = sig_utf8_decode_deep( $k );
			$dados[$k] = sig_utf8_decode_deep( $v );
		}
	} else if (is_object($dados)) {
		$vars = array_keys( get_object_vars($dados) );

		foreach ($vars as $var) {
			$attr = sig_utf8_decode_deep( $var );
			$val  = sig_utf8_decode_deep( $dados->$var );
			unset($dados->$var);
			$dados->$attr = $val;
		}
	}

	return $dados;
}

/**************
 * NÃO modifique essa função sem antes falar com o FELIPE CHIAVICATTI
*/
function sig_utf8_encode_deep($dados) {
	if (is_string($dados)) {
// 		$dados = ($dados);
		$dados = ((mb_detect_encoding($dados . 'x', 'UTF-8', TRUE) === FALSE) ? ( $dados ) : $dados);
	} else if (is_array($dados)) {
		foreach ($dados as $k => $v) {
			unset( $dados[$k] );

			$k 		   = sig_utf8_encode_deep( $k );
			$dados[$k] = sig_utf8_encode_deep( $v );
		}
	} else if (is_object($dados)) {
		$vars = array_keys( get_object_vars($dados) );

		foreach ($vars as $var) {
			$attr = sig_utf8_encode_deep( $var );
			$val  = sig_utf8_encode_deep( $dados->$var );
			unset($dados->$var);
			$dados->$attr = $val;
		}
	}

	return $dados;
}

function arrayParaScriptPhp_sig( $array ){
	if ( is_string( $array ) ){
		$script = '"' . $array . '"';
	}elseif ( is_numeric( $array ) ){
		$script = $array;
	}elseif ( is_array( $array ) || is_object( $array ) ){
		$scriptItem = array();
		foreach ( $array as $k => $v ){
			$v = ( is_array( $v ) || is_object( $v ) ? (array) $v : $v );
			$scriptItem[] .= arrayParaScriptPhp( $k ) . ' => ' . arrayParaScriptPhp( $v );
		}
		$script = 'array(' . implode(", ", $scriptItem) . ')';
	}

	return $script;
}

/**
 * Função para formatação de CPF
 * @param string $cpf
 * @return string
 */
function formata_cpf_sig( $cpf ) {
	$numcpf = str_replace(Array('.','-'), '', $cpf); // Remover todos os separadores antes de formatar

	$p1 = substr($numcpf, 0, 3);
	$p2 = substr($numcpf, 3, 3);
	$p3 = substr($numcpf, 6, 3);
	$p4 = substr($numcpf, 9, 2);

	return "{$p1}.{$p2}.{$p3}-{$p4}";
}

/**
 * Função de formatação de CNPJ
 * @param string $cnpj
 * @return string
 */
function formata_cnpj_sig( $cnpj ) {
	$numcnpj = str_replace(Array('.','-','/'), '', $cnpj); // Remover todos os separadores antes de formatar

	$p1 = substr($numcnpj, 0, 2);
	$p2 = substr($numcnpj, 2, 3);
	$p3 = substr($numcnpj, 5, 3);
	$p4 = substr($numcnpj, 8, 4);
	$p5 = substr($numcnpj, 12, 2);

	return "{$p1}.{$p2}.{$p3}/{$p4}-{$p5}";
}

/**
 * rodrigo.rodrigues
 * Funcao para ler xls e xlsx
 * @param strArquivo Ex:  APPRAIZ."exemplo/modulos/relatorio/exemplo_2.xls";
 * @param inicio_row - Inicio pela linha (default 1) - Caso queira eliminar a primeira linha por exemplo
 * @param inicio_col - Inicio pela coluna (default 0) - Caso queira eliminar a primeira coluna por exemplo
 * @return array
 */
function excel_reader_sig($strArquivo, $inicio_row = 1, $inicio_col = 0, $strCelula = null)
{
     if (!file_exists($strArquivo)) {
        echo "<br/>Arquivo inexistente ou diretorio inválido";
        exit();
    }

    require_once APPRAIZ. 'includes/PHPExcel/Classes/PHPExcel/IOFactory.php';
    $inputFileType = PHPExcel_IOFactory::identify($strArquivo);
    $objReader = PHPExcel_IOFactory::createReader($inputFileType);
    $objReader->setReadDataOnly(true);

    /**  Carregando $inputFileName para o PHPExcel Objetos  **/
    $objPHPExcel = $objReader->load($strArquivo);

    $total_sheets=$objPHPExcel->getSheetCount(); //Total de linhas

    $allSheetName=$objPHPExcel->getSheetNames();
    $objWorksheet = $objPHPExcel->setActiveSheetIndex(0);

    $highestRow = $objWorksheet->getHighestRow();
    $highestColumn = $objWorksheet->getHighestColumn();
    $highestColumnIndex = PHPExcel_Cell::columnIndexFromString($highestColumn);
    $arraydata = array();

    if(is_null($strCelula))
    {
      for ($row = $inicio_row; $row <= $highestRow;++$row)
      {
        for ($col = $inicio_col; $col <$highestColumnIndex;++$col)
        {
              $value = $objWorksheet->getCellByColumnAndRow($col, $row)->getValue();
            if ( substr($value,0,1) == "=" ){
                $value = $objWorksheet->getCellByColumnAndRow($col, $row)->getCalculatedValue();
              }
                  $arraydata[$row-1][$col]=$value;
        }
      }
       return $arraydata;
    }else{
       $strRetorno =  $objPHPExcel->getActiveSheet()->getCell($strCelula)->getValue();
       if(substr($strRetorno,0,1) == "="){
         $strRetorno =  $objPHPExcel->getActiveSheet()->getCell($strCelula)->getValueBinder();
       }
       return $strRetorno;
    }

}

    /**
     * functionName multiImplode
     * @author Djalma Aguiar Rodrigues
     *
     * @param integer $glue(separador)
     * @param array $array(array multidimensional a ser implodido)
     *
     * @return Retorna o implode para o array multidimensional.
     * Interessante para montar um conjunto em consultas SQL. Ex: IN( 1,2,3 )
     *
     * @version v1
    */
 function multiImplode_sig( $glue, $array ) {
    $ret = '';
    foreach ($array as $item) {
        if (is_array($item)) {
            $ret .= multiImplode( $glue, $item ) . $glue;
        } else {
            $ret .= $item . $glue;
        }
    }
    $ret = substr($ret, 0, 0-strlen($glue));
    return $ret;
}

/**
 * Retira caracteres especiais
 * @param string $svalor
 * @return string
 */
function retiraCaracterInv_sig($svalor)
{
    //substituição de caracteres inválidos
    $svalor = str_replace("?", "-", $svalor);
    $svalor = str_replace("?", "''", $svalor);
    $svalor = str_replace("?", "''", $svalor);
    $svalor = str_replace("?", "\"", $svalor);
    $svalor = str_replace("?", "\"", $svalor);
    $svalor = str_replace("'","''",$svalor);
    $svalor = str_replace("?", "-", $svalor);
    $svalor = str_replace("?", "-", $svalor);
    $svalor = str_replace("?", "...", $svalor);
    $svalor = str_replace("?", "", $svalor);
    $svalor = str_replace("Ã?", "Ê", $svalor);

    return $svalor;
}

/**
 * Retorna todos os perfis do usuario em um sistema
 * @param string $cpf
 * @param string sisid
 * @return array
 */
function carregarPerfilUsuario_sig($cpf = null,$sisid = null)
{
    global $db;

    $cpf = ( $cpf ? $cpf : $_SESSION['usucpf'] );
    $sisid = ( $sisid ? $sisid : $_SESSION['sisid'] );

    $sql = "select  p.pflcod as cod
                from seguranca.perfilusuario pu
                inner join seguranca.perfil p on pu.pflcod = p.pflcod
                where pu.usucpf = '$cpf'
                and p.pflstatus = 'A'
                and p.sisid = $sisid";
    return $db->carregarColuna($sql);
}

/**
 * Retorna o nome da coluna do tipo DATA de acordo com seu alias
 * @param $strSql Consulta SQL
 * @param $strAlias Apelido da coluna
 * @return $arrMatches
 */
function pegaNomeColunaPeloApelido_sig($strSql, $strAlias)
{
	$arrMatches = array();
	preg_match('/to_char\((.*)([:]|,|, )\'([A-Z0-9_:]*|(DD[-\/.]MM[-\/.](YYYY|YY)(| HH(|24|12)(|:MI)(|:SS))))\'\)(.*)' . $strAlias . '/i', $strSql, $arrMatches);
	# Pega coluna dentro da funcao regexp_replace
	if (!empty($arrMatches) && strripos(reset($arrMatches), 'regexp_replace')) {
		preg_match('/regexp_replace\((.*)::numeric::/i', $arrMatches[1], $arrMatches);
	}
//	dbg($arrMatches, 1);
	return $arrMatches;
}
/**
 * Inclui campo de ordenacao para retorno na consulta
 * @param $strSql Consulta SQL
 * @param $arrMatches Retorno da funcao pegaNomeColunaPeloApelido
 * @param $strAlias Apelido da coluna
 * @return mixed
 */
function montaConsultaOrdenacao_sig($strSql, $arrMatches, $strAlias)
{
	$arrReturn = array();
	if (is_array($arrMatches) && !empty($arrMatches)) {
		$strColuna = reset($arrMatches);
		$strNomeColuna = $arrMatches[1];
		$intTamanhoColuna = strlen($strColuna);
		# Trata aliases com aspas
		$intUltimaPosicao = $intTamanhoColuna - strlen($strAlias) - 1;
		if (strpos($strColuna, '"') && $strColuna[$intUltimaPosicao] == '"') {
			$strColuna .= '"';
		}
		# Busca o nome da coluna incluindo seu alias
		$arrColunaComAlias = array();
		preg_match('/' . preg_quote($strColuna, '/') . '(.*)' . preg_quote($strAlias, '/') . '/i', $strSql,
			$arrColunaComAlias);
		if (!empty($arrColunaComAlias)) {
			$strColuna = reset($arrColunaComAlias);
			$intPrimeiraPosicao = strpos($strSql, reset($arrColunaComAlias));
			$intTamanhoColuna = strlen($strColuna);
		} else {
			$intPrimeiraPosicao = strpos($strSql, $strColuna);
		}
		# Monta o campo para ordenacao
		$strNomeColunaAlias = 'COLUNA_PARA_ORDENAR';
		$strColunaNova = $strColuna . ', ' . $strNomeColuna . ' AS ' . $strNomeColunaAlias;
		# Inclui na consulta final
		$strSqlNew = substr_replace($strSql, $strColunaNova, $intPrimeiraPosicao, $intTamanhoColuna);
		$arrReturn['booRemoverCampoOrdenacao'] = true;
		$arrReturn['sortname'] = $strNomeColunaAlias;
		$arrReturn['query'] = $strSqlNew;
	} else {
		$arrReturn['booRemoverCampoOrdenacao'] = false;
		$arrReturn['sortname'] = $strAlias;
		$arrReturn['query'] = $strSql;
	}
	return $arrReturn;
}

/**
 * Remove campo utilizado apenas para ordenacao
 * @param $arrResult Resultado de uma consulta
 * @param $strSortName Nome da coluna a ser removida
 * @return mixed
 */
function removerCampoOrdenacao_sig($arrResult, $strSortName)
{
	if ($strSortName) {
		$strSortNameLower = strtolower($strSortName);
		foreach ($arrResult as $intKey => $arrRegistro) {
			unset($arrResult[$intKey][$strSortNameLower]);
		}
	}
	return $arrResult;
}

/**
 * functionName mascaraNumeroProtocolo
 * @author Djalma Aguiar Rodrigues
 * @description Retorna a máscara do protocolo de acordo com a quantidade de dígitos informada.
 * @version v1.1
 * @param string $prtnumeroprotocolo
 * Ex: mascaraNumeroProtocolo( '23477033071201582' ).
 */
function mascaraNumeroProtocoloSig_sig( $prtnumeroprotocolo ){
    if(!$prtnumeroprotocolo ):
        return false;
    else:
        if(ctype_digit($prtnumeroprotocolo)): 
           if( strlen($prtnumeroprotocolo) == 12 ): 
               return mascara_global($prtnumeroprotocolo, "######/####-##"); 
           elseif(strlen($prtnumeroprotocolo) == 17 ): // SLTI/MP n° 03, de 16 de maio 2003
               return  mascara_global($prtnumeroprotocolo, "#####.######/####-##");
           elseif(strlen($prtnumeroprotocolo) == 21 ): // PORTARIA 2.321, DE 30 DE DEZEMBRO DE 2014
               return  mascara_global($prtnumeroprotocolo, "#######.########/####-##");
           else:
               return false;
           endif;
        else:
            return false;
        endif;
    endif;
}

/**
 * Verifica delimitadores em uma string e retorna array
 * @param string $string
 * @return array | null
 */
function delimitador_string_to_array_sig($string)
{
   $arrRetorno = array();
   $arrDelimitadores = array(',',";","/",":"," ");
   $string = trim($string);
   foreach($arrDelimitadores as $delimitador):
      if(strpos($string, $delimitador)){
          $arrOcorrencias = explode($delimitador, $string);
          if($arrOcorrencias){
              foreach($arrOcorrencias as $ocorrencia):
                 if(nao_nulo($ocorrencia)){
                   $arrRetorno[] = trim($ocorrencia);
                 }
              endforeach;
          }
      }
   endforeach;
  return ($arrRetorno)? $arrRetorno : NULL;
 }

function pegaTrechoEntreTag_sig($tagIni, $tagFim, $conteudo){
	$posIni = strpos($conteudo, $tagIni);
	$posFim = strpos($conteudo, $tagFim);

	if ( $posIni === false || $posFim === false ){
		return $conteudo;
	}
	
	return substr($conteudo, $posIni, ($posFim - $posIni));
}

/*
 * MaskTexto
 * @param  $strTexto - Texto original
 * @param  $strDelimitador - Delimitador do texto
 * @param  $intInicio - Iniciar a substituicao apos (numero) caracteres
 * @param  $intMeio   - Mostrar os ultimos (numero) caracteres
 * @return $strRetorno
 */
function maskTexto_sig($strTexto, $strDelimitador = null, $intInicio = 4, $intMeio = 0)
{
   if($strDelimitador){ 
        if (strpos($strTexto, $strDelimitador)) {
            $strTextoDelimitador = strstr($strTexto, $strDelimitador, true);
            $strTextoFimDelimitador = strstr($strTexto, $strDelimitador);
            $cont = strlen($strTextoDelimitador);
            $strRetorno = '';
            $strMeio = '';
            if ($cont > $intInicio) {
                $cont = ($cont - $intInicio);
                if ($cont > 0) {
                    $strRetorno = substr($strTextoDelimitador, 0, $intInicio);
                    for ($i = 0; $i < $cont; $i++) {
                        $strRetorno .= '*';
                    }
                    if ($intMeio > 0) {
                        $strMeio = substr($strTextoDelimitador, -$intMeio);
                        $strRetorno = substr($strRetorno, 0, (strlen($strRetorno) - strlen($strMeio))) . $strMeio;
                    }
                } else {
                    $strRetorno = $strTextoDelimitador;
                }
            }
            return $strRetorno . $strTextoFimDelimitador;
        } else {
            return $strTexto;
        }
   }else{
       $cont = strlen($strTexto);
       $strInicio = substr($strTexto,0,$intInicio);
       $strUltimos = ($intMeio > 0)? substr($strTexto, -$intMeio) : '';
       $quantidade = ($cont -  (strlen($strInicio) + strlen($strUltimos)));
       $strRetorno = $strInicio;
       if($quantidade > 0){
           for($i=0;$i < $quantidade; $i++){
               $strRetorno .= '*';
           }
       }
       $strRetorno .= $strUltimos;
       return $strRetorno;
   }
}

/** Djalma Rodrigues
 * @param $valor
 * @return mixed Formata um valor valor monetário para o padrão no banco de dados.
 */
function formatar_valor_sql_sig($valor) {
    $valor = str_replace('.', '', $valor);
    $valor = str_replace(',', '.', $valor);
    return $valor;
}

/**
 * Alterar um valor inteiro para data
 * @param integer 20170926
 * @return string 26/09/2017
*/
function integerDate_sig($data)
{
  $data = substr($data,6,7).'/'.substr($data,4,2).'/'.substr($data,0,4);
  return $data;
}

/*
 *  Verfica se o usuario possui um perfil valido
 *  @param $arrperfilHabilitado  array(PERFIL_SUPER, PERFIL_ADMIN )
 *  @return boolean
 * */
function in_array_perfil_sig($arrPerfilHabilitado){
    $arrPerfil = pegaPerfilGeral();
    $booleano = false;
    if(!is_array($arrPerfilHabilitado)){
        $arrPerfilHabilitado[] = $arrPerfilHabilitado;
    }
    foreach($arrPerfil as $perfil):
        if(in_array($perfil, $arrPerfilHabilitado)){
            $booleano = true;
        }
    endforeach;

    return $booleano;
}

/** Djalma Rodrigues
 * Função para calcular o próximo dia útil de uma data pula feriados e finais de semana.
 * @param $valorData "AAAA-MM-DD"
 * @param string $saida
 * @param int $qtd_dias
 * @return false|string
 */
function proximoDiaUtil_sig( $valorData, $saida = 'd/m/Y', $qtd_dias = 1 ) {
    global $db;

    # Acrescenta um dia a data
    $data = date('Y-m-d', strtotime($valorData . " + {$qtd_dias} days " ) );
    $timestamp = strtotime( $data );
    $dia = date('N', $timestamp );

    # Obtem lista de feriados com base no ano informado.
    $arrData = explode('-',$data);
    $arFeriado = $db->carregarColuna("SELECT to_char(feddata, 'DDMM') || {$arrData[0]} FROM feriados WHERE fedstatus = 'A'");

    # Se o dia incrementado for Sabádo ou domingo retorna segunda feira, se esta for feriado incrementa novamente.
    if ( $dia >= 6 ) {
        $timestamp_final = $timestamp + ((8 - $dia) * 3600 * 24);

        if( in_array( date('dmY', $timestamp_final ), $arFeriado  ) ){
            $timestamp_final = strtotime( $data );
            return proximoDiaUtil_sig( date('Y-m-d', $timestamp_final ), $saida );

        } else {
            $arrData = explode('-', $valorData);

            // Se o dia informado for ano novo.
            if( $arrData[0] !==  date('Y', $timestamp_final ) ){
                return proximoDiaUtil_sig( date('Y-m-d', $timestamp_final ), $saida );

            } else {
                return date( $saida, $timestamp_final );
            }
        }

    # Dia da semana, verifica se o dia incrementado é feriado, caso sim adiciona um dia.
    } else {

        if( in_array( date('dmY', $timestamp), $arFeriado  ) ){
            $data = date('Y-m-d', strtotime($data . " + {$qtd_dias} days "));
            $timestamp_final = strtotime($data);
            $dia = date('N', $timestamp );

            # Caso fim de semana incrementa novamente e segunda for feriado incrementa novamente
            if ( $dia >= 6 ) {
                $timestamp_final = $timestamp + ((8 - $dia) * 3600 * 24);

                if( in_array( date('dmY', $timestamp_final), $arFeriado  ) ){
                    $timestamp_final = strtotime($data);
                    return date($saida, $timestamp_final);
                } else {
                    return date($saida, $timestamp_final);
                }

            } else {
                return date($saida, $timestamp_final);
            }

        } else {
            return date($saida, $timestamp);
        }
    }
    return date($saida, $timestamp);
}

function str_upper_utf8_sig($str){
    return mb_convert_case($str, MB_CASE_UPPER, 'UTF-8');
}