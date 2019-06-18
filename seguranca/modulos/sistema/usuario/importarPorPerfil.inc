<?php

function buscarPerfil()
{
	global $db;

	$sql = "SELECT
            	pflcod as codigo,
            	pfldsc as descricao
            FROM seguranca.perfil
            WHERE
            	pflstatus = 'A'
            	AND sisid = {$_REQUEST['sisid']}";

	ob_clean();
	$db->monta_combo( $_REQUEST['tipo'], $sql, 'S', 'Selecione...', '', '' );
}

function importarUsuarios()
{
    global $db;

    if(!$db->testa_superuser()){
        echo "<script>
    	       alert('Operação inválida.');
    	       window.location.href = window.location.href;
    	      </script>";
    	return false;
    }

    if($_REQUEST['limit']){
    	$limit = "LIMIT {$_REQUEST['limit']}";
    }

    if($_REQUEST["origem"] != '' && $_REQUEST["destino"] != ''){

        $sql = " SELECT DISTINCT
                    usuariosistema.sisid,
    		   		usuario.usucpf,
    		   		usuario.usunome as nomeusuario,
    		   		'(' || usuario.usufoneddd || ') '
    		   		|| usuario.usufonenum as fone ,
    		   		usuario.regcod,
    		   		municipio.mundescricao,
    		   		entidade.entnome as orgao,
    				CASE WHEN entidade.entid = 390402 THEN trim(usuario.orgao) ELSE trim(unidadex.unidsc) END as unidsc,
    		   		COALESCE(cargo.cardsc,'')||' / '||COALESCE(usuario.usufuncao,'') as cargo,
    				to_char(usuario.usudataatualizacao,'dd/mm/YYYY HH24:MI') as data
    			FROM
    				seguranca.perfil perfil
				inner join seguranca.perfilusuario  perfilusuario   on perfil.pflcod = perfilusuario.pflcod and perfil.pflcod = {$_REQUEST["origem"]}
				right join seguranca.usuario        usuario         on usuario.usucpf = perfilusuario.usucpf
    				left join
    				(
    				  select
    					unicod,
    					unidsc
    				  from
    					public.unidade
    				  where
    					unitpocod = 'U'
    				) as unidadex on usuario.unicod = unidadex.unicod
    			left join  territorios.municipio municipio 	    	on municipio.muncod = usuario.muncod
    			inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf AND usuariosistema.sisid = perfil.sisid
    			left join  entidade.entidade entidade		    	on usuario.entid = entidade.entid
    			left join  public.cargo cargo		    			on cargo.carid = usuario.carid
    			WHERE
    				usunome is not null  AND usuariosistema.suscod = 'A' AND usuariosistema.sisid = {$_REQUEST['sisid_origem']} AND (perfil.pflcod = {$_REQUEST["origem"]})
    				AND perfilusuario.usucpf NOT IN (SELECT usucpf FROM seguranca.perfilusuario WHERE pflcod = {$_REQUEST["destino"]})
    			GROUP BY
    				usuario.usucpf, usuario.usunome, usuario.usufoneddd,
    				usuario.usufonenum, usuario.regcod, entidade.entid, entidade.entnome,
    				unidadex.unidsc, usuario.orgao, usuario.usudataatualizacao , municipio.mundescricao, cargo.cardsc, usuario.usufuncao, usuariosistema.sisid
    			ORDER BY
    				nomeusuario
				$limit";

        $arrDados = $db->carregar($sql);

        if(!is_array($arrDados)){
            echo "<script>
        	       alert('Não existem usuários com o perfil apontado.');
        	       window.location.href = window.location.href;
        	      </script>";
        }

        foreach ($arrDados as $dados) {

            $sql = "SELECT 'Possui' as teste FROM seguranca.perfilusuario WHERE usucpf = '{$dados['usucpf']}' AND pflcod = {$_REQUEST["destino"]}";

            $temPerfilUsuario = $db->pegaUm($sql);

            if($temPerfilUsuario != 'Possui'){
                $sql_insert[] = "INSERT INTO seguranca.perfilusuario (usucpf,pflcod)
                                 VALUES ('{$dados['usucpf']}',{$_REQUEST["destino"]});";
            }

            $sql = "SELECT 'Possui' as teste FROM seguranca.usuario_sistema WHERE usucpf = '{$dados['usucpf']}' AND sisid = {$_REQUEST['sisid_destino']}";

            $temUsuarioSistema = $db->pegaUm($sql);

            if($temUsuarioSistema != 'Possui'){
                $sql_insert[] = "INSERT INTO seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
                                 VALUES ('{$dados['usucpf']}',{$_REQUEST['sisid_destino']}, 'A', {$_REQUEST["destino"]}, now(), 'A');";
            }

            $sql3 = "SELECT * FROM par.usuarioresponsabilidade
                     WHERE
                        usucpf = '{$dados['usucpf']}'
                        AND pflcod = {$_REQUEST["origem"]}
                        AND rpustatus = 'A'";


            $arrResp = $db->carregar($sql3);
            if (!empty($arrResp)) {
                foreach ($arrResp as $resp) {

                    $resp["prgid"] = $resp["prgid"] ? $resp["prgid"] : 'null';
                    $resp["entid"] = $resp["entid"] ? $resp["entid"] : 'null';

                    $sql = "SELECT 'Possui' as teste FROM par3.usuarioresponsabilidade
                            WHERE usucpf = '{$dados['usucpf']}' AND pflcod = {$_REQUEST["destino"]} AND estuf = '{$resp["estuf"]}' AND muncod = '{$resp["muncod"]}' AND prgid = {$resp["prgid"]} AND entid = {$resp["entid"]} AND rpustatus = 'A'";

                    $temUsuarioSistema = $db->pegaUm($sql);

                    if($temUsuarioSistema != 'Possui'){
                        $sql_insert[] = "INSERT INTO par3.usuarioresponsabilidade (usucpf,pflcod,estuf,muncod,prgid,entid,rpustatus)
                                         VALUES ('{$dados['usucpf']}',{$_REQUEST["destino"]},'{$resp["estuf"]}','{$resp["muncod"]}',{$resp["prgid"]},{$resp["entid"]},'A');";
                    }
                }
            }
        }

        if($sql_insert != ''){

        	$texto = '<label style=color:red; >Scripts não Executados</label>';

            if($_REQUEST['executar']){
            	$db->executar(implode(' ', $sql_insert));
            	$db->commit();

            	$texto = 'Scripts Executados';

            	echo "<script>
            	       alert('$texto!');
            	      </script>";
            }

            include APPRAIZ ."includes/cabecalho.inc";
            print '<br>';

            monta_titulo('Importar Usuário por Perfil', $texto.'<br><input type="submit" class="botao" value="Voltar" onclick="window.location.href=window.location.href">' );

            echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                	<tr>
                		<td>';

            echo implode('<br>', $sql_insert);

            echo '      </td>
                	</tr>
                </table>
                ';

        	echo "<script>
        	       jQuery('#aguarde').hide();
        	      </script>";
        }
    }
}

if($_REQUEST['req']){
    $_REQUEST['req']();
    die();
}

include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo('Importar Usuário por Perfil', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
?>
<body>
    <form method="post" name="formulario">
        <input type="hidden" name="req" value="importarUsuarios" />
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        	<tr bgcolor="#C0C0C0">
        		<td class="SubTituloDireita">Importar</td>
        		<td class="SubTituloDireita" style="text-align:left">De:</td>
        		<td class="SubTituloDireita" style="text-align:left">Para:</td>
        	</tr>
        	<tr>
        		<td align='right' class="SubTituloDireita">Sistema:</td>
        		<td>
            		<?
					$sql = "SELECT
                            	sisid as codigo,
                            	sisdsc as descricao
                            FROM seguranca.sistema
                            WHERE sisstatus = 'A'
                            ORDER BY 2";
					$db->monta_combo( "sisid_origem", $sql, 'S', 'Selecione...', '', '' );
					?>
        		</td>
        		<td>
            		<?
					$sql = "SELECT
                            	sisid as codigo,
                            	sisdsc as descricao
                            FROM seguranca.sistema
                            WHERE sisstatus = 'A'
                            ORDER BY 2";
					$db->monta_combo( "sisid_destino", $sql, 'S', 'Selecione...', '', '' );
					?>
			    </td>
        	</tr>
        	<tr>
        		<td align='right' class="SubTituloDireita">Perfil:</td>
        		<td id="td_origem">Escolha o sistema de origem</td>
        		<td id="td_destino">Escolha o sistema de destino</td>
        	</tr>
        	<tr>
        		<td align='right' class="SubTituloDireita">Executar ou apresentar o script?:</td>
        		<td colspam=2 >
                    <input type=radio name=executar value=1 />Executar &nbsp;
                    <input type=radio name=executar value=0 checked/>Apresentar
        		</td>
        	</tr>
        	<tr>
        		<td align='right' class="SubTituloDireita">LIMIT:</td>
        		<td colspam=2 >
                    <?php echo campo_texto('limit', 'N', 'S', 'LIMIT', 20, 5, '[#]')?>
        		</td>
        	</tr>
        	<tr bgcolor="#C0C0C0">
        		<td width="15%">&nbsp;</td>
        		<td colspan=2 >
                    <input type="submit" class="botao" value="Importar">
        		</td>
        	</tr>
        </table>
    </form>
</body>
<script>
jQuery(document).ready(function()
{
	jQuery('select').attr('required', true);

	jQuery('select').change(function()
	{
		var sisid   = jQuery(this).val();
		var name    = jQuery(this).attr('name');
		var arrNome = name.split('_');
		var tipo    = arrNome[1];
		var data    = new Array();

		if(sisid == ''){
        	jQuery('#td_'+tipo).html('Escolha o sistema de '+tipo);
        	return false;
		}

		data.push({name : 'req'  , value : 'buscarPerfil'},
				  {name : 'sisid', value : sisid},
				  {name : 'tipo' , value : tipo}
				 );

		jQuery.ajax({
	        type   : "POST",
	        url	   : window.location.href,
	        data   : data,
	        async  : false,
	        success: function(msg){
	        	jQuery('#td_'+tipo).html(msg);
            	jQuery('select').attr('required', true);
	        }
	    });
	});
});
</script>