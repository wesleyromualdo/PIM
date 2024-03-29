<?php 
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
require_once APPRAIZ . "emenda/classes/WSContaCorrente.class.inc";
require_once APPRAIZ . "emenda/classes/ContaCorrente.class.inc";
require_once APPRAIZ . "emenda/classes/ContaCorrenteHistorico.class.inc";
require_once APPRAIZ . "emenda/classes/LogErroWS.class.inc";

if($_POST['consultarContaCorrente']){
	$obContaCorrente = new WSContaCorrente();
	$obContaCorrente->consultarContaCorrente($_POST, true);
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

?>
<html>
<head>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script type="text/javascript"><!--

    function consultarContaCorrente(){
		var msg = '';

		if($('usuario').value == ''){
			msg += "O campo Usuário é obrigatório.\n";
		}
		if($('senha').value == ''){
			msg += "O campo Senha é obrigatório.\n";
		}

		if(msg != ''){
			alert(msg);
			return false;
		}

		var usuario = $('usuario').value;
		var senha   = $('senha').value;
		
		var carregando = "<img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\">";
		var url = "/emenda/emenda.php?modulo=principal/consultarContaCorrente&acao=A";
		var pars = 'consultarContaCorrente=1&usuario='+usuario+'&senha='+senha;

		var bkDiv = $('divFormulario').innerHTML;
		
		var myAjax = new Ajax.Request(
				url, 
				{
					method: 'post', 
					parameters: pars, 
					asynchronous: false,
					onLoading: $('divFormulario').update(carregando+' Aguarde enquanto sua solicitação é finalizada. '),
					onComplete: function(r)
					{
						//$('divDebug').update(r.responseText);
						var erro = trim(r.responseText.substring(0,5));
						var tamanho = r.responseText.length;
						if(erro == 'Erro-' ){
							var resultado = trim(r.responseText.substring(5,tamanho));
							$('divFormulario').update(resultado+bkDiv);
							$('divFormulario').style.color = 'red';								
							$('divFormulario').style.fontSize = '14px';								
							$('divFormulario').style.textAlign = 'center';
						} else {
							$('divFormulario').update(r.responseText);
						}
					}
				});
	}

--></script>
</head>
<body>
<div id="divFormulario">
<form name="formulario" id="formulario" method="post">
	<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
	<table id="tb_tabela" class="tabela" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td colspan="2"><b>Informações de Usuário do FNDE</b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Usuário:</b></td>
			<td>
				<input type="text" class="normal" name="usuario" id="usuario" value="<?php echo $usuario;?>" size="40" maxlength="15" title="Usuário">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Senha:</b></td>
			<td>
				<input type="password" class="normal" name="senha" id="senha" value="<?php echo $senha;?>" size="40" maxlength="15" title="Senha">
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" style="text-align: center">
				<input type="button" name="btnSalvar" value="Consultar Conta Corrente" id="btnSalvar" onclick="consultarContaCorrente();">
			</td>
		</tr>
	</table>
</form>
</div>
</body>
</html>
<div id="divDebug"></div>