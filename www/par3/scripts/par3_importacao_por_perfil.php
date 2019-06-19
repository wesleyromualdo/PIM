<?php
$arrPerfil = pegaPerfilGeral();
$arrPerfil = $arrPerfil ? $arrPerfil : array();
if ($_REQUEST['importarUsuarios'] && $_REQUEST['antigo'] && $_REQUEST['novo'] && $simec->testa_superuser()) {
    //equipe municipal 1344                 460
    //equipe estadual 1345                  461
    //prefeito 1349                         556
    //Secretario estadual 1350              672

//     $perfis[0]["novo"] = 1344;
//     $perfis[0]["antigo"] = 460;

//     $perfis[1]["novo"] = 1345;
//     $perfis[1]["antigo"] = 461;

//     $perfis[2]["novo"] = 1349;
//     $perfis[2]["antigo"] = 556;

//     $perfis[3]["novo"] = 1350;
//     $perfis[3]["antigo"] = 672;

    $antigos = implode(',', $_REQUEST['antigo']);
    $novos   = implode(',', $_REQUEST['novo']);

    foreach($antigos as $k => $antigo){
        $perfis[$k]["novo"] = $novos[$k];
        $perfis[$k]["antigo"] = $antigo;
    }

    foreach ($perfis as $key) {

        $sql = " SELECT DISTINCT
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
				inner join seguranca.perfilusuario perfilusuario    on perfil.pflcod = perfilusuario.pflcod and perfil.pflcod = " . $key["antigo"] . "
				right join seguranca.usuario usuario		    	on usuario.usucpf = perfilusuario.usucpf
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
			inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf
			left join  entidade.entidade entidade		    	on usuario.entid = entidade.entid
			left join  public.cargo cargo		    			on cargo.carid = usuario.carid
			WHERE
				usunome is not null  and usuariosistema.suscod = 'A' and usuariosistema.sisid = '23' and (perfil.pflcod = " . $key["antigo"] . ")
			GROUP BY
				usuario.usucpf, usuario.usunome, usuario.usufoneddd,
				usuario.usufonenum, usuario.regcod, entidade.entid, entidade.entnome,
				unidadex.unidsc, usuario.orgao, usuario.usudataatualizacao , municipio.mundescricao, cargo.cardsc, usuario.usufuncao
			ORDER BY
				nomeusuario";


        $dados2 = $db->carregar($sql);

        foreach ($dados2 as $key2) {
            $sql_insert[] = "insert into seguranca.perfilusuario (usucpf,pflcod) values ('{$key2['usucpf']}',{$key["novo"]});";
            $sql3 = "select * from par.usuarioresponsabilidade where usucpf = '{$key2['usucpf']}' and pflcod = {$key["antigo"]} and rpustatus = 'A'";


            $dados3 = $db->carregar($sql3);
            if (!empty($dados3)) {
                foreach ($dados3 as $key3) {
                    $sql_insert[] = "insert into par3.usuarioresponsabilidade (usucpf,pflcod,estuf,muncod,prgid,entid,rpustatus) values ('{$key2['usucpf']}',{$key["novo"]},'{$key3["estuf"]}',{$key3["muncod"]},{$key3["prgid"]},{$key3["entid"]},'A');";
                }
            }
        }

    }

}

ver($sql_insert, d);

?>