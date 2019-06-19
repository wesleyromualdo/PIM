<?php

global $servidor_bd, $porta_bd, $nome_bd, $usuario_db, $senha_bd, $configDbPddeinterativo, $linkGlobal, $linkGlobalCluster, $db2;

$servidor_bd_temp = $servidor_bd;
$porta_bd_temp    = $porta_bd;
$nome_bd_temp     = $nome_bd;
$usuario_db_temp  = $usuario_db;
$senha_bd_temp    = $senha_bd;

$servidor_bd = $configDbPddeinterativo->host;
$porta_bd    = $configDbPddeinterativo->port;
$nome_bd     = $configDbPddeinterativo->dbname;
$usuario_db  = $configDbPddeinterativo->user;
$senha_bd    = $configDbPddeinterativo->password;

// Limpando link de conexão para forçar nova conexão em outro banco
$linkGlobal = $linkGlobalCluster = null;

$db2 = new cls_banco();

// Limpando link de conexão para forçar NOVAMENTE nova conexão em outro banco (não está redundante)
$linkGlobal = $linkGlobalCluster = null;

// Retornando valor padrão
$servidor_bd = $servidor_bd_temp;
$porta_bd    = $porta_bd_temp;
$nome_bd     = $nome_bd_temp;
$usuario_db  = $usuario_db_temp;
$senha_bd    = $senha_bd_temp;

/* MECANISMO DE ESPELHO DOS PERFIS - Removendo todos os perfis slaves dos outros sistemas	 */
function removerPerfisSlaves($usucpf, $sisid) {
	global $db, $db2;

	$sql = "SELECT DISTINCT p.pflcodslave, p.servidorslave FROM seguranca.espelhoperfil p WHERE p.sisidmaster = '".$sisid."'";
	$arrslaves = $db->carregar($sql);
	if($arrslaves[0]) {
		foreach($arrslaves as $arrs) {
			if($arrs['servidorslave']=='pdeinterativo') {

				$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '".$usucpf."' AND pflcod='".$arrs['pflcodslave']."'";
				$db2->executar( $sql, false );
				$db2->commit();
			} else {
				$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '".$usucpf."' AND pflcod='".$arrs['pflcodslave']."'";
				$db->executar( $sql );
				$db->commit();
			}
		}
	}
}


/* MECANISMO DE ESPELHO DOS PERFIS - Inserindo todos os perfis slaves dos outros sistemas
Mudança para atender o pdeinterativo 2013 - 20/03/2013 - solicitado pelo Daniel Areas
* */
function inserirPerfisSlaves($usucpf, $pflcod) {

	global $db, $db2;
	$sql = "SELECT DISTINCT p.pflcodslave, p.servidorslave FROM seguranca.espelhoperfil p WHERE p.pflcodmaster = '".$pflcod."'";
	$pfls = $db->carregar($sql);

	if($pfls[0]) {

		foreach($pfls as $pfl) {

			if($pfl['servidorslave']=='pdeinterativo') {

				$existe_us = $db2->pegaUm("SELECT usucpf FROM seguranca.usuario WHERE usucpf='".$usucpf."'");

				if(!$existe_us) {

					$dados_us = $db->pegaLinha("SELECT * FROM seguranca.usuario WHERE usucpf='".$usucpf."'");

					$sql = "INSERT INTO seguranca.usuario(usucpf,regcod,usunome,usuemail,usustatus,usufoneddd,usufonenum,ususenha,usudataultacesso,
							            usunivel,usufuncao,
							            ususexo,
							            orgcod,
							            unicod,
							            usuchaveativacao,
							            usutentativas,
							            usuprgproposto,
							            usuacaproposto,
							            usuobs,
							            ungcod,
							            usudatainc,
							            usuconectado,
							            pflcod,
							            suscod,
							            usunomeguerra,
							            orgao, muncod, usudatanascimento, usudataatualizacao,
							            tpocod, carid)
							    VALUES ('".$dados_us['usucpf']."',
							    		".(($dados_us['regcod'])?"'".$dados_us['regcod']."'":"NULL").",
							    		".(($dados_us['usunome'])?"'".str_replace(array("'"),array(""),$dados_us['usunome'])."'":"NULL").",
							    		".(($dados_us['usuemail'])?"'".$dados_us['usuemail']."'":"NULL").",
							    		".(($dados_us['usustatus'])?"'".$dados_us['usustatus']."'":"NULL").",
							    		".(($dados_us['usufoneddd'])?"'".$dados_us['usufoneddd']."'":"NULL").",
							    		".(($dados_us['usufonenum'])?"'".$dados_us['usufonenum']."'":"NULL").",
							            ".(($dados_us['ususenha'])?"'".$dados_us['ususenha']."'":"NULL").",
							            ".(($dados_us['usudataultacesso'])?"'".$dados_us['usudataultacesso']."'":"NULL").",
							            ".(($dados_us['usunivel'])?"'".$dados_us['usunivel']."'":"NULL").",
							            ".(($dados_us['usufuncao'])?"'".$dados_us['usufuncao']."'":"NULL").",
							            ".(($dados_us['ususexo'])?"'".$dados_us['ususexo']."'":"NULL").",
							            ".(($dados_us['orgcod'])?"'".$dados_us['orgcod']."'":"NULL").",
							            ".(($dados_us['unicod'])?"'".$dados_us['unicod']."'":"NULL").",
							            ".(($dados_us['usuchaveativacao'])?"'".$dados_us['usuchaveativacao']."'":"NULL").",
							            ".(($dados_us['usutentativas'])?"'".$dados_us['usutentativas']."'":"NULL").",
							            ".(($dados_us['usuprgproposto'])?"'".$dados_us['usuprgproposto']."'":"NULL").",
							            ".(($dados_us['usuacaproposto'])?"'".$dados_us['usuacaproposto']."'":"NULL").",
							            ".(($dados_us['usuobs'])?"'".$dados_us['usuobs']."'":"NULL").",
							            ".(($dados_us['ungcod'])?"'".$dados_us['ungcod']."'":"NULL").",
							            ".(($dados_us['usudatainc'])?"'".$dados_us['usudatainc']."'":"NULL").",
							            ".(($dados_us['usuconectado'])?"'".$dados_us['usuconectado']."'":"NULL").",
							            ".(($dados_us['pflcod'])?"'".$dados_us['pflcod']."'":"NULL").",
							            ".(($dados_us['suscod'])?"'".$dados_us['suscod']."'":"NULL").",
							            ".(($dados_us['usunomeguerra'])?"'".$dados_us['usunomeguerra']."'":"NULL").",
							            ".(($dados_us['orgao'])?"'".$dados_us['orgao']."'":"NULL").",
							            ".(($dados_us['muncod'])?"'".$dados_us['muncod']."'":"NULL").",
							            ".(($dados_us['usudatanascimento'])?"'".$dados_us['usudatanascimento']."'":"NULL").",
							            ".(($dados_us['usudataatualizacao'])?"'".$dados_us['usudataatualizacao']."'":"NULL").",
							            ".(($dados_us['tpocod'])?"'".$dados_us['tpocod']."'":"NULL").",
							            ".(($dados_us['carid'])?"'".$dados_us['carid']."'":"NULL").");";
	
						$db2->executar( $sql, false );
						$inseriuUsu = $db2->commit();
	

				}else{

					$dados_us = $db->pegaLinha("SELECT * FROM seguranca.usuario WHERE usucpf='".$usucpf."'");
					$inseriuUsu = true;
					$sql = "update seguranca.usuario set suscod = '{$dados_us['suscod']}' where usucpf = '{$usucpf}'";
					$db2->executar( $sql, false );
					$db2->commit();
				}

				$existe = $db2->pegaUm("SELECT usucpf from seguranca.perfilusuario where pflcod='".$pfl['pflcodslave']."' and usucpf='".$usucpf."'");

				if(!$existe && $inseriuUsu ) {
					$sql = "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '".$usucpf."', '".$pfl['pflcodslave']."');";
					$db2->executar( $sql, false );
					$db2->commit();
				}

			} else {

				$existe = $db->pegaUm("SELECT usucpf from seguranca.perfilusuario where pflcod='".$pfl['pflcodslave']."' and usucpf='".$usucpf."'");

				if(!$existe) {
					$sql = "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '".$usucpf."', '".$pfl['pflcodslave']."');";
					$db->executar( $sql );
					$db->commit();
				}

			}

		}
	}

	$sql = "SELECT DISTINCT p.sisidslave, p.servidorslave FROM seguranca.espelhoperfil p WHERE p.pflcodmaster = '".$pflcod."'";
	$siss = $db->carregar($sql);

	if($siss[0]) {

		foreach($siss as $sis) {

			$suscod = $db->pegaUm("(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='".$usucpf."' AND sisid IN (SELECT DISTINCT sisidmaster FROM seguranca.espelhoperfil p WHERE p.pflcodmaster = ".$pflcod."))");

			if($sis['servidorslave']=='pdeinterativo') {

				$existe = $db2->pegaUm("SELECT usucpf from seguranca.usuario_sistema where sisid='".$sis['sisidslave']."' and usucpf='".$usucpf."'");

				if(!$existe) {
					$sql = "INSERT INTO seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
							SELECT '".$usucpf."' as usucpf, '".$sis['sisidslave']."' as sisid, 'A' as susstatus, NULL::integer as pflcod, NOW() as susdataultacesso, '".$suscod."' as suscod FROM seguranca.usuario WHERE usucpf='".$usucpf."'";
					$db2->executar( $sql, false );
					$db2->commit();
				}

				$sql = "UPDATE seguranca.usuario_sistema SET suscod='".$suscod."' WHERE usucpf='".$usucpf."' AND sisid='".$sis['sisidslave']."'";
				$db2->executar( $sql, false );
				$db2->commit();

			} else {

				$existe = $db->pegaUm("SELECT usucpf from seguranca.usuario_sistema where sisid='".$sis['sisidslave']."' and usucpf='".$usucpf."'");

				if(!$existe) {
					$sql = "INSERT INTO seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
							SELECT '".$usucpf."' as usucpf, '".$sis['sisidslave']."' as sisid, 'A' as susstatus, NULL::integer as pflcod, NOW() as susdataultacesso, '".$suscod."' as suscod FROM seguranca.usuario WHERE usucpf='".$usucpf."'";
					$db->executar( $sql );
					$db->commit();
				}

				$sql = "UPDATE seguranca.usuario_sistema SET suscod='".$suscod."' WHERE usucpf='".$usucpf."' AND sisid='".$sis['sisidslave']."'";
				$db->executar( $sql );
				$db->commit();

			}

		}
	}

}


/* MECANISMO DE ESPELHO DOS PERFIS - Atualizando as responsabilidades nos outros sistemas	 */
function atualizarResponsabilidadesSlaves($usucpf, $pflcod) {
	global $db, $db2;

	$sql = "SELECT DISTINCT p.pflcodmaster, p.pflcodslave, p.urcampo, p.sisidslave, s.sisdiretorio as sisdiretorioslave, s1.sisdiretorio as sisdiretoriomaster, p.servidorslave FROM seguranca.espelhoperfil p
			LEFT JOIN seguranca.sistema s ON s.sisid = p.sisidslave
			LEFT JOIN seguranca.sistema s1 ON s1.sisid = p.sisidmaster
			WHERE p.pflcodmaster = {$pflcod}";

	$registros = $db->carregar($sql);

	if($registros[0]) {
		foreach($registros as $registro) {

			if($registro['servidorslave']=='pdeinterativo') {
				$sql = "select sisdiretorio from seguranca.sistema where sisid = ".$registro['sisidslave'];
				$registro['sisdiretorioslave'] = $db2->pegaUm($sql);
			}

			if($registro['urcampo']) {

				if($registro['servidorslave']=='pdeinterativo') {
					$db2->executar("UPDATE ".$registro['sisdiretorioslave'].".usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='".$usucpf."' AND pflcod='".$registro['pflcodslave']."'", false);

					$regs = $db->carregar("SELECT ".$registro['urcampo']." FROM ".$registro['sisdiretoriomaster'].".usuarioresponsabilidade WHERE rpustatus='A' AND usucpf='".$usucpf."' AND pflcod='".$pflcod."'");

					if($regs[0]) {
						foreach($regs as $r) {
							$sql = "INSERT INTO ".$registro['sisdiretorioslave'].".usuarioresponsabilidade (pflcod, usucpf, rpustatus, rpudata_inc, ".$registro['urcampo'].")
									VALUES ('".$registro['pflcodslave']."', '{$usucpf}', 'A', NOW(), '".$r[$registro['urcampo']]."')";

							$db2->executar($sql, false);

						}
					}
				} else {

					$db->executar("UPDATE ".$registro['sisdiretorioslave'].".usuarioresponsabilidade SET rpustatus='I' WHERE usucpf='".$usucpf."' AND pflcod='".$registro['pflcodslave']."'");

					$regs = $db->carregar("SELECT ".$registro['urcampo']." FROM ".$registro['sisdiretoriomaster'].".usuarioresponsabilidade WHERE rpustatus='A' AND usucpf='".$usucpf."' AND pflcod='".$pflcod."'");

					if($regs[0]) {
						foreach($regs as $r) {
							if( $r[$registro['urcampo']] != '' ){
								$sql = "INSERT INTO ".$registro['sisdiretorioslave'].".usuarioresponsabilidade (pflcod, usucpf, rpustatus, rpudata_inc, ".$registro['urcampo'].")
										VALUES ('".$registro['pflcodslave']."', '{$usucpf}', 'A', NOW(), '".$r[$registro['urcampo']]."')";

								$db->executar($sql);
							}
						}
					}

				}

			}

		}
	}
	$db2->commit();
	$db->commit();

}





?>