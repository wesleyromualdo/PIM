<?

 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com),Marcelo Freire Costa (marcelofreire.freire@gmail.com)
   Módulo:menu.inc
   Finalidade: permitir o cadastro de itens de menu
   */

	function mostrar_querys_executadas()
	{
			/**
	 * Exibe as querys executadas na última ação realizada.
	 *
	 * @return void
	 */
		global $key_sessao_menu_sql;
		print '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center"><tr><td style="font-size: 10pt;">';
		if ( isset( $_SESSION[$key_sessao_menu_sql] ) && count( $_SESSION[$key_sessao_menu_sql] ) )
		{
			foreach ( $_SESSION[$key_sessao_menu_sql] as $menu_sql )
			{
				print '<br/>' . $menu_sql . '<br/><br/>';
			}
		}
		$_SESSION[$key_sessao_menu_sql] = array();
		print '</td></tr></table>';
	}

	include APPRAIZ."includes/cabecalho.inc";

	// DEFINE DADOS GERAIS

	//Atribui o sisid a variável
	$sisid = $_REQUEST['sisid'];
	//Caso o sisid não seja passado atribui-se valor zero
	if( $sisid == '' )
	{
		$sisid = 0;
	}
	//Armazena o caminho de redirecionamento
	$modulo = $_REQUEST['modulo'] ;
	$key_sessao_menu_sql = 'menu_sql';
	if ( !isset( $_SESSION[$key_sessao_menu_sql] ) )
	{
		$_SESSION[$key_sessao_menu_sql] = array();
	}
	// FIM DEFINE DADOS GERAIS

	// caso seja solicitada a inserção faça
	if ($_REQUEST['act'] == 'inserir')
	{
		// verifica se existe módulo igual cadastrado
		$sql = "select count(*) as total from seguranca.menu where mnulink <> '' and mnulink = '" . $_REQUEST['mnulink'] . "' and sisid = " . $sisid;
		$RS = $db->recuperar( $sql,$res );
		if ( $RS['total'] > 0 ) // caso já exista módulo igual cadastrado e não seja alteração mostra a mensagem
		{
			?>
				<script type="text/javascript">
					alert( 'Este Menu/Módulo já se encontra cadastrado!' );
					history.back();
				</script>
			<?
			$db->close();
			exit();
		}
		// FIM verifica se existe módulo igual cadastrado
		// fazer inserção de menu na base de dados.
		if ( !$_REQUEST['abacod'] )
		{
			$_REQUEST['abacod'] = 'null';
		}
		$sql = "insert into seguranca.menu
							(mnucod,mnuidpai,mnudsc,mnutransacao,mnulink,mnutipo,mnustile,
							mnuhtml,mnusnsubmenu,mnushow,mnuacessorapido,mnustatus,abacod,
							sisid) values (";
		$sql .= $_REQUEST['mnucod'] . ",";
		$sql .= ( $_REQUEST['mnuidpai'] ? $_REQUEST['mnuidpai'] . ',' : ' null,' );
		$sql .=
			"'" . $_REQUEST['mnudsc'] . "'," .
			"'" . $_REQUEST['mnutransacao'] . "'," .
			"'" . $_REQUEST['mnulink'] . "'," .
			"'" . $_REQUEST['mnutipo'] . "'," .
			"'" . $_REQUEST['mnustile'] . "'," .
			"'" . $_REQUEST['mnuhtml'] . "'," .
			"'" . $_REQUEST['mnusnsubmenu'] . "'," .
			"'" . $_REQUEST['mnushow'] . "'," .
			"'" . $_REQUEST['mnuacessorapido'] . "'," .
			"'A'," .
			$_REQUEST['abacod'] . "," .
			$sisid . ") returning mnuid";
		array_push( $_SESSION[$key_sessao_menu_sql], $sql );
		$saida = $db->pegaUm( $sql );
		$mnuid = $saida;
		if($mnuid){
			$sql = "delete from seguranca.perfilmenu where mnuid = ".$mnuid.";";
			$db->executar($sql);
		}
		if($mnuid && $_REQUEST['pflcod'][0] != "" && $_REQUEST['pflcod_campo_flag'] == "1"){
			foreach($_REQUEST['pflcod'] as $pflcod){
				$sqlI .= "insert into seguranca.perfilmenu (pflcod,mnuid) values ($pflcod,".$mnuid.");";
			}
  			$db->executar($sqlI);
		}

		$db->commit();
		$db->sucesso($modulo);
		// FIM fazer inserção de menu na base de dados.
	}
	// FIM caso seja solicitada a inserção faça

	// caso seja solicitada alteração faça
	if ( $_REQUEST['act'] == 'alterar' )
	{
// 		$_REQUEST['mnusnsubmenu'] = ( $_REQUEST['mnusnsubmenu'] == 't' ? 'true' : 'false' );
// 		$_REQUEST['mnushow'] = ( $_REQUEST['mnushow'] == 't' ? 'true' : 'false' );
// 		$_REQUEST['mnuacessorapido'] = ( $_REQUEST['mnuacessorapido'] == 't' ? 'true' : 'false' );
		$sql =
			" update seguranca.menu set " .
				" mnucod = " . $_REQUEST['mnucod'] . ", " .
				" mnudsc = '" . $_REQUEST['mnudsc'] . "', " .
				" mnutransacao = '" . $_REQUEST['mnutransacao'] . "', " .
				" mnulink = '" . $_REQUEST['mnulink'] . "', " .
				" mnutipo = '" . $_REQUEST['mnutipo'] . "', " .
				" mnustile = '" . $_REQUEST['mnustile'] . "', " .
				" mnuhtml = '".$_REQUEST['mnuhtml']."', " .
				" mnuidpai = ";
		$sql .= ( $_REQUEST['mnuidpai'] ? $_REQUEST['mnuidpai'] . ',' : ' null,' );
		$sql .= ' abacod = ' . ( $_REQUEST['abacod'] ? $_REQUEST['abacod'] . ',' : ' null,' );
		$sql .=
				" mnusnsubmenu = '" . $_REQUEST['mnusnsubmenu'] . "', " .
				" mnushow = '" . $_REQUEST['mnushow'] . "', " .
				" mnuacessorapido = '" . $_REQUEST['mnuacessorapido'] . "'" .
			"  where mnuid=" . $_REQUEST['mnuid'];
//		dbg( $sql, 1 );
		array_push( $_SESSION[$key_sessao_menu_sql], $sql );
		$saida = $db->executar( $sql );
		if($_REQUEST['mnuid']){
			$sql = "delete from seguranca.perfilmenu where mnuid = ".$_REQUEST['mnuid'].";";
			$db->executar($sql);
		}
		if($_REQUEST['mnuid'] && $_REQUEST['pflcod'][0] != "" && $_REQUEST['pflcod_campo_flag'] == "1"){
			foreach($_REQUEST['pflcod'] as $pflcod){
				$sqlI .= "insert into seguranca.perfilmenu (pflcod,mnuid) values ($pflcod,".$_REQUEST['mnuid'].");";
			}
  			$db->executar($sqlI);
		}
		?>
		<script type="text/javascript">
			alert( 'Operação realizada com sucesso' );
		</script>
		<script type="text/javascript">
			//location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=<?=$modulo.'&acao=A&mnuid='.$_REQUEST['mnucod']?>";
		</script>
		<?
		$db->commit();
		//exit();
	}
	// FIM caso seja solicitada alteração faça

	// caso seja solicitada a exclusão faça
	if ( $_REQUEST['exclui'] > 0 )
	{
		$sql = "delete from seguranca.perfilmenu where mnuid = " . $_REQUEST['exclui'];
		array_push( $_SESSION[$key_sessao_menu_sql], $sql );
		$saida = $db->executar( $sql );
		$sql = "delete from seguranca.menu where mnuid = " . $_REQUEST['exclui'];
		array_push( $_SESSION[$key_sessao_menu_sql], $sql );
		$saida = $db->executar( $sql );
		unset( $_POST['exclui'] );
		$db->commit();
		$db->sucesso( $modulo, '&sisid='. $sisid );
	}
	// FIM caso seja solicitada a exclusão faça

	print '<br/>';
	$parametros = array( '', '', '' );
	$db->cria_aba( $abacod_tela, $url, $parametros );

	// caso seja passado o código do menu para uma ação de inclusão ou alteração faça
	if ( $_REQUEST['acao'] == 'I' or ( $_REQUEST['acao'] == 'A' and $_REQUEST['mnuid'] <> '' ) )
	{
		//título da página
		$titulo_modulo = $_REQUEST['acao'] == 'I' ? 'Incluir Menu' : 'Alterar Menu' ;
		monta_titulo( $titulo_modulo, '<img src="../imagens/obrig.gif" borderorder="0"> Indica Campo Obrigatório.' );
		//mostrar_querys_executadas();
		//Caso seja passado o código do menu e não seja selecionada ação faça
		if ( $_REQUEST['mnuid'] and $_REQUEST["act"] == '' )
		{
			//Pesquisa os dados do menu e sistema selecionados
			$sql= "select * from seguranca.menu where mnuid=".$_REQUEST['mnuid']." and sisid=".$sisid;
			$saida = $db->recuperar($sql,$res);
			if( is_array( $saida ) )
			{
				foreach( $saida as $k => $v ) ${$k} = $v;
			}
		}
		else foreach( $_REQUEST as $k => $v ) ${$k} = $v;
		?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
			<form method="POST"  name="formulario">
			<input type='hidden' name="modulo" value="<?=$modulo?>">
			<input type='hidden' name="mnuid" value=<?=$_REQUEST['mnuid']?>>
			<input type='hidden' name='exclui' value=0>
			<input type='hidden' name='act' value=0>
			<input type='hidden' name='acao' value=<?=$_REQUEST['acao']?>>
			<tr>
				<td align='right' class="SubTituloDireita">Sistema:</td>
				<td>
					<?
					$sisid = $_REQUEST['sisid'];
					//Caso a página de inclusão não tenha sido aberta pelo menu Incluir menu faça
					if( $_REQUEST['menuselecionado'] == '' )
					{
						$sql = "SELECT sisid as codigo, sisdsc as descricao FROM seguranca.sistema order by sisdsc";
						$db->monta_combo( "sisid", $sql, 'S', "Selecione o Sistema", '', '' );
					}
					//Caso tenha sido aberta pela tela de administração de menus faça
					else
					{
						$sql= "select * from seguranca.sistema where sisid=".$sisid;
						//$saida = $db->recuperar($sql,$res);
						if( is_array( $saida ) )
						{
							foreach( $saida as $k => $v ) ${$k} = $v;
						}
						print $sisdsc;
						?>
						<input type ="hidden" name = "sisid" value ="<?= $sisid ?>">
					<?}?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Código:</td>
				<td>
					<? if ( !$_REQUEST['mnuid'] ) $habil='S'; else $habil= 'N'; ?>
					<?= campo_texto( 'mnucod', 'S', $habil, '', 10, 10, '[#]', '' ); ?>
				</td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Descrição:</td>
				<td><?= campo_texto( 'mnudsc', 'S', 'S', '', 50, 50, '', '' ); ?></td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Transação:</td>
				<td><?= campo_texto( 'mnutransacao', 'S', 'S', '', 50, 50, '', '' ); ?></td>
			</tr>
			<tr>
				<td align='right'  class="SubTituloDireita">Tipo:</td>
				<td>
					<select name="mnutipo" onchange="Selecionatipo()" class="CampoEstilo">
						<option value=""></option>
						<option value="1" <? if ( $mnutipo == "1" ) print "selected"; ?>>1</option>
						<option value="2" <? if ( $mnutipo == "2" ) print "selected"; ?>>2</option>
						<option value="3" <? if ( $mnutipo == "3" ) print "selected"; ?>>3</option>
						<option value="4" <? if ( $mnutipo == "4" ) print "selected"; ?>>4</option>
					</select>
					<?=obrigatorio();?>
				</td>
			</tr>
      <?if ($mnutipo <> "" and $mnutipo <> "1") {?>
      <tr>
        <td align='right'  class="SubTituloDireita">Menu Pai:</td>
        <td>
                 <?
		$sql="SELECT mnuid as codigo, mnudsc as descricao FROM seguranca.menu where mnutipo = " . ($mnutipo-1) . " and mnusnsubmenu='t' and sisid=".$sisid."  order by mnudsc";

			$db->monta_combo("mnuidpai",$sql,'S',"Selecione o Menu",'','');





		?>
    	 <?=obrigatorio();?>
        </td>
        </tr>
      <?}?>
      <tr>
        <td align='right'  class="SubTituloDireita">Possui Sub-menu?</td>
        <td>
        <input type="radio" name="mnusnsubmenu" value="t" onchange="submenu('S');" <?=($mnusnsubmenu=='t'?"CHECKED":"")?>> Sim
            <input type="radio" name="mnusnsubmenu" value="f" onchange="submenu('N');" <?=($mnusnsubmenu=='f'?"CHECKED":"")?>> Não
        </td>
      </tr>
      <tr>
        <td id="sub0" align='right'  class="SubTituloDireita">Link:</td>
        <td id="sub1"><?=campo_texto('mnulink','N','S','',50,100,'','');?></td>
      </tr>
    <?
     //Caso seja passado o sisid faça
    if($sisid<> '')
    {
    ?>
      <tr>
        <td id="sub2" align='right'  class="SubTituloDireita">Aba:</td>



        <td id="sub3">

       <?$sql="SELECT abacod as codigo, abadsc as descricao FROM seguranca.aba where sisid=".$sisid;


       $db->monta_combo("abacod",$sql,'S',"Selecione a Aba",'','');


       ?>
	</td>
      </tr>
   <?}?>
	  <tr>

        <td id="sub4" align='right'  class="SubTituloDireita">Faz parte da árvore?</td>
        <td id="sub5"><input type="radio" name="mnushow" value="t" <?=($mnushow=='t'?"CHECKED":"")?>> Sim
            <input type="radio" name="mnushow" value="f" <?=($mnushow=='f'?"CHECKED":"")?>> Não
		</td>
      </tr>
	  <tr id="trAcessoRapido">
        <td id="" align='right'  class="SubTituloDireita">Acesso Rápido?</td>
        <td id="">
        	<input type="radio" name="mnuacessorapido" value="t" <?=($mnuacessorapido=='t'?"CHECKED":"")?>> Sim
            <input type="radio" name="mnuacessorapido" value="f" <?=(($mnuacessorapido=='f' || $mnuacessorapido=='')?"CHECKED":"")?>> Não
		</td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Estilo:</td>
        <td><?=campo_texto('mnustile','N','S','',50,150,'','');?></td>
      </tr>
      <tr>
        <td align='right'  class="SubTituloDireita">Html:</td>
        <td><?=campo_textarea('mnuhtml','N','S','',60,5,500);?></td>
      </tr>
        <?php //PERDIL DE USUÁRIOS
        if($sisid){
	        $sql = "select
						pflcod as CODIGO,
						pfldsc as DESCRICAO
					from
						seguranca.perfil
					where
						pflstatus='A'
					and
						sisid=".$sisid."
					order by
						pfldsc ";
			if($mnuid){
				$sqlCarregado = "select
							pflcod as CODIGO,
							pfldsc as DESCRICAO
						from
							seguranca.perfil
						where
							pflstatus='A'
						and
							sisid=".$sisid."
						and
							pflcod in (select pflcod from seguranca.perfilmenu where mnuid = $mnuid)
						order by
							pfldsc ";
			}
			mostrarComboPopup( 'Perfil Associado', 'pflcod',  $sql, $sqlCarregado, 'Selecione o Perfil' );
		}?>


<? if ($_REQUEST["mnuid"]) { ?>
<tr bgcolor='#CCCCCC'>
   <td></td>
   <td><input type="button" name="btalterar" value="Alterar" onclick="validar_cadastro('A')" class="botao">&nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btcancelar" value="Voltar" onclick="window.location='seguranca.php?modulo=sistema/menu/menu&acao=A';" class="botao"></td>
 </tr>
</table>

<? } else { ?>
<tr bgcolor="#CCCCCC">
   <td></td><td><input type="button" name="btinserir" value="Incluir" onclick="validar_cadastro('I')" class="botao">&nbsp;&nbsp;&nbsp;&nbsp;<input type="button" name="btcancelar" value="Voltar" onclick="history.back();" class="botao"></td>
 </tr>
<?}?>
 </form>
 </table>
<?
}elseif ($_REQUEST['acao']=='A' and !$_REQUEST['mnuid']){?>

<?

//título da página
monta_titulo($titulo_modulo,'Escolha uma ação desejada referente ao Menu / Módulo : 	<img src="../imagens/alterar.gif" border="0" align="absmiddle"> = Alterar / 	<img src="../imagens/excluir.gif" border="0" align="absmiddle"> = Excluir / 	<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"> = Incluir');
//mostrar_querys_executadas();

?>
<form method="POST"   name="formulario">
 <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">          <tr>
        <td align='right' class="SubTituloDireita">Sistema:</td>
        <td>

		<? $sql = "select distinct sisid as CODIGO,sisdsc as DESCRICAO from seguranca.sistema order by DESCRICAO ";
			$db->monta_combo('sisid',$sql,'S',"Selecione o Sistema",'submete_sistema','');?>
	    </td>
   </tr>
 </table>
<input type='hidden' name='mnuidpai'>
<input type='hidden' name='mnutipo'>
<input type='hidden' name="exclui" value=<?=$_REQUEST['mnuid']?>>

<?
//teste utilizando a função Monta Lista
$cabecalho = array('Ações','Código','Menu / Módulo','Visível','Transação');
//$sql = "select acacod, acadsc from acao";
$sql = "select case when mnusnsubmenu=true then '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Menu\" onclick=\"altera_menu('||mnuid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Menu\" onclick=\"excluir_menu('||mnuid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/gif_inclui.gif\" title=\"Incluir Menu em » '||mnudsc||'\" onclick=\"incluir_menu('||mnuid||','||mnutipo||')\">' else '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Menu\" onclick=\"altera_menu('||mnuid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Menu\" onclick=\"excluir_menu('||mnuid||')\">' end as acao, mnucod, case when mnutipo=2 then '&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=3 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc when  mnutipo=4 then '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\">'||mnudsc else mnudsc end as mnudsc, case when mnushow=false then '<font color=#808080>Não</font>' else '<font color=#008000>Sim</font>' end as mnushow, mnutransacao from seguranca.menu where mnustatus = 'A' and sisid=".$sisid."  order by mnucod";
$db->monta_lista_simples($sql,$cabecalho,200,20,'','','');
}
?>
</form>
<script>
<?if ($_REQUEST["mnuid"]){?>
if (document.formulario.mnusnsubmenu[0].checked)
{
 		  document.formulario.mnulink.disabled = true;
		  document.formulario.abacod.disabled = true;
		  //document.formulario.mnushow[0].checked = false;
		  //document.formulario.mnushow[1].checked = false;
		  for (i=0;i<6;i++)
					{
					document.getElementById("sub"+i).style.visibility = "hidden";
					document.getElementById("sub"+i).style.display = "none";
					}
}
<?}?>

function submenu(op)
{
	if (op == 'S') {
		document.formulario.mnulink.disabled = true;
	  	document.formulario.abacod.disabled = true;
	  	document.formulario.mnushow[0].checked = true;
	  	document.formulario.mnushow[1].checked = false;
	  	for (i=0;i<6;i++) {
			document.getElementById("sub"+i).style.visibility = "hidden";
			document.getElementById("sub"+i).style.display = "none";
	  	}

	  	document.getElementById('trAcessoRapido').style.display = "none";
	} else {
		document.formulario.mnulink.disabled = false;
	  	document.formulario.abacod.disabled = false;
	  	for (i=0;i<6;i++) {
			document.getElementById("sub"+i).style.visibility = "visible";
			document.getElementById("sub"+i).style.display = "";
		}

		document.getElementById('trAcessoRapido').style.display = "";
	}
}

  function altera_menu(cod) {

  	sisid = document.formulario.sisid.value;

  	if(document.formulario.sisid.value!='')
  	{

    location.href = 'seguranca.php?modulo=sistema/menu/menu&acao=A&mnuid='+cod + '&menuselecionado=sim' + '&sisid=' + document.formulario.sisid.value;
  	}

  }

  function incluir_menu(codpai, tipo) {
     document.formulario.mnuidpai.value = codpai;
	 document.formulario.mnutipo.value = tipo+1;
	 location.href = 'seguranca.php?modulo=sistema/menu/menu&acao=I&mnuidpai='+document.formulario.mnuidpai.value+'&mnutipo='+document.formulario.mnutipo.value + '&menuselecionado=sim&sisid='+document.formulario.sisid.value;
  }

  function excluir_menu(cod) {
    if( window.confirm( "Confirma a exclusão do ítem "+ cod + " no Menu?") )
    {
//		document.formulario.sisid.value = <?= $sisid ?>;
		document.formulario.exclui.value = cod;
		document.formulario.submit();

    }
    else document.formulario.exclui.value = 0;
  }

    function validar_cadastro(cod) {

		if (!validaBranco(document.formulario.mnucod, 'Código')) return;
		if (!validaBranco(document.formulario.mnudsc, 'Descrição')) return;
		if (!validaBranco(document.formulario.mnutransacao, 'Transação')) return;
		if (!validaBranco(document.formulario.mnutipo, 'Tipo')) return;

		if(document.getElementById( 'pflcod' )){
			selectAllOptions( document.getElementById( 'pflcod' ) );
		}

		if (document.formulario.mnutipo.value != "1" )
		{


		if (!validaBranco(document.formulario.mnuidpai, 'Menu Pai')) return;

		}


		if (!validaRadio(document.formulario.mnusnsubmenu,'Possui sub-menu')) return;



		if (document.formulario.mnusnsubmenu[1].checked)
		{
			if (!validaBranco(document.formulario.mnulink, 'Link')) return;
			if (!validaRadio(document.formulario.mnushow,'Faz Parte da Árvore')) return;


		}else	{
			document.formulario.mnulink.value = '';
			document.formulario.abacod.value = '';
			}



	   	if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
   	   	document.formulario.submit();

     }

     function submete_sistema()
     {


      document.formulario.submit();

     }

     function Selecionatipo(){

	     if(document.formulario.sisid.value=='')
	     {
	     	alert('Selecione o Sistema')
	     }
	     else
	     {
	          		document.formulario.submit();
     	 }

     }

     function onOffCampo( campo ) {
		var div_on  = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input   = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}


</script>
