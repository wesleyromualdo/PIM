<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadastro_usuario_elaboracao_responsabilidades.php
   
   */
include "config.inc";
header('content-type: text/html; charset=utf-8');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if(!$pflcod && !$usucpf) {
	?><font color="red">Requisição inválida</font><?
	eixt();
}

$sqlResponsabilidadesPerfil = "SELECT tr.*
	FROM par3.tprperfil p
	INNER JOIN par3.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
	WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
	ORDER BY tr.tprdsc";

$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
$responsabilidadesPerfil = $db->carregar($query);

if (!$responsabilidadesPerfil || count($responsabilidadesPerfil) < 1) {
	print "<font color='red'>Não foram encontrados registros</font>";
} else {
	foreach ($responsabilidadesPerfil as $rp) {
		$sqlRespUsuario = "";
		switch (trim($rp["tprsigla"])) {
			case "E": // Estados
				$aca_prg = "Estados Associados";
				$sqlRespUsuario = "SELECT e.estuf AS codigo, e.estdescricao AS descricao, ur.rpustatus AS status
								   FROM par3.usuarioresponsabilidade ur 
								   INNER JOIN territorios.estado e ON e.estuf = ur.estuf
								   WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus='A'";
				break;
			case "M": // Municípios
				$aca_prg = "Municípios Associados";
				$sqlRespUsuario = "SELECT m.muncod as codigo, m.estuf || ' - ' || m.mundescricao as descricao, ur.rpustatus AS status
								   FROM par3.usuarioresponsabilidade ur
								   INNER JOIN territorios.municipio m ON m.muncod = ur.muncod
								   WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus = 'A'				
								   ORDER BY m.estuf, m.mundescricao";
				break;
			case "U": // Unidades
				$aca_prg = "Unidades Associadas";
				$sqlRespUsuario = 	"SELECT iue.entid as codigo, iue.iuenome as descricao, ur.rpustatus AS status
								  	FROM par3.instrumentounidadeentidade iue
									INNER JOIN par3.usuarioresponsabilidade ur ON iue.entid = ur.entid
								   	WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus = 'A'				
								   	ORDER BY iue.iuenome";
				break;
			case "X": // Executora
				$aca_prg = "Entidade Executora";
				$sqlRespUsuario = 	"SELECT iue.iuecnpj as codigo, iue.iuenome as descricao, ur.rpustatus AS status
								  	FROM par3.instrumentounidadeentidade iue
									INNER JOIN par3.usuarioresponsabilidade ur ON iue.iueid = ur.iueid
								   	WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus = 'A'				
								   	ORDER BY iue.iuenome";
				break;
			case "P": //Programas
				$aca_prg = "Programas Associados";	
				$sqlRespUsuario = "SELECT DISTINCT pr.prgid AS codigo, pr.prgdsc AS descricao, ur.rpustatus AS status
				   		   		   FROM par3.usuarioresponsabilidade ur
				   		   		   INNER JOIN par3.programa pr ON pr.prgid = ur.prgid
				   		   		   INNER JOIN par3.pfadesao pf ON ur.prgid = pf.prgid			
								   WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus='A'";			
				break;
			case "UG": //Unidade Executora
				$aca_prg = "Unidade Executora";	
				$sqlRespUsuario = "SELECT DISTINCT formata_cpf_cnpj(ue.ungcnpj) AS codigo, ue.ungrazao_social AS descricao, ur.rpustatus AS status
                                    FROM par3.usuarioresponsabilidade ur
                                    	INNER JOIN par3.unidade_executora ue ON ue.ungid = ur.ungid
                                    WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND ur.rpustatus='A'";			
				break;
		}
		
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		$respUsuario = $db->carregar($query);
		if (!$respUsuario || @count($respUsuario)<1) {
			print "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font color='red'>Não existem $aca_prg a este Perfil.</font>";
		}
		else {
		?>
		<table class="table" style="margin: -9px;">
			<head>
				<tr>
				  <th colspan="3" > <?=$rp["tprdsc"]?></th>
				</tr>
			</head>
			<tr>
		      <td valign="top" width="12">&nbsp;</td>
			  <td valign="top"><b>Código</b></td>
			  <td valign="top"><b>Descrição</b></td>
		    </tr>
				<?
					foreach ($respUsuario as $ru) {
				?>
			<tr>
		      <td valign="top" width="12"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
			  <td valign="top" width="90" nowrap><?if ($rp["tprsigla"]=='A'){?><a href="simec_er.php?modulo=principal/acao/cadacao&acao=C&acaid=<?=$ru["acaid"]?>&prgid=<?=$ru["prgid"]?>"><?=$ru["codigo"]?></a><?} else {print $ru["codigo"];}?></td>
			  <td valign="top" width="290"><?=$ru["descricao"]?></td>
			</tr>
				<?
				}
				?>
			<tr>
			  <td colspan="4" align="right" >
			    <b>Total: (<?=@count($respUsuario)?>)</b>
			  </td>
			</tr>
		</table>
		<?
			}
		}
	}
	$db->close();
	exit();
	?>