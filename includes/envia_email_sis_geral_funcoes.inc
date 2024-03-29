<?php

if(!function_exists('mime_content_type')) {

	function mime_content_type($filename) {

		$mime_types = array(

				'txt' => 'text/plain',
				'htm' => 'text/html',
				'html' => 'text/html',
				'php' => 'text/html',
				'css' => 'text/css',
				'js' => 'application/javascript',
				'json' => 'application/json',
				'xml' => 'application/xml',
				'swf' => 'application/x-shockwave-flash',
				'flv' => 'video/x-flv',

				// images
				'png' => 'image/png',
				'jpe' => 'image/jpeg',
				'jpeg' => 'image/jpeg',
				'jpg' => 'image/jpeg',
				'gif' => 'image/gif',
				'bmp' => 'image/bmp',
				'ico' => 'image/vnd.microsoft.icon',
				'tiff' => 'image/tiff',
				'tif' => 'image/tiff',
				'svg' => 'image/svg+xml',
				'svgz' => 'image/svg+xml',

				// archives
				'zip' => 'application/zip',
				'rar' => 'application/x-rar-compressed',
				'exe' => 'application/x-msdownload',
				'msi' => 'application/x-msdownload',
				'cab' => 'application/vnd.ms-cab-compressed',

				// audio/video
				'mp3' => 'audio/mpeg',
				'qt' => 'video/quicktime',
				'mov' => 'video/quicktime',

				// adobe
				'pdf' => 'application/pdf',
				'psd' => 'image/vnd.adobe.photoshop',
				'ai' => 'application/postscript',
				'eps' => 'application/postscript',
				'ps' => 'application/postscript',

				// ms office
				'doc' => 'application/msword',
				'rtf' => 'application/rtf',
				'xls' => 'application/vnd.ms-excel',
				'ppt' => 'application/vnd.ms-powerpoint',

				// open office
				'odt' => 'application/vnd.oasis.opendocument.text',
				'ods' => 'application/vnd.oasis.opendocument.spreadsheet',
		);

		$ext = strtolower(array_pop(explode('.',$filename)));
		if (array_key_exists($ext, $mime_types)) {
			return $mime_types[$ext];
		}
		elseif (function_exists('finfo_open')) {
			$finfo = finfo_open(FILEINFO_MIME);
			$mimetype = finfo_file($finfo, $filename);
			finfo_close($finfo);
			return $mimetype;
		}
		else {
			return 'application/octet-stream';
		}
	}
}

require_once APPRAIZ . "includes/Email.php";

class EmailSistema extends Email {
	
	public function enviar( array $destinatarios, $assunto, $conteudo, array $arquivos, $remetenteInformado = array(), $destinoArquivo = null, $condicao = true ) {
		# identifica o remetente
		$remetente = $this->pegarUsuario( $_SESSION['usucpforigem'] );
		if ( !$remetente->usucpf ) {
			return false;
		}	
		$this->Timeout = 60;
		$this->SMTPDebug = true;
		$this->From     = isset( $remetenteInformado["usuemail"] ) ? $remetenteInformado["usuemail"] : $remetente->usuemail;
		$this->FromName = isset( $remetenteInformado["usunome"]  ) ? $remetenteInformado["usunome"]  : $remetente->usunome;
		
		# identifica os destinat?rios
		foreach ( $destinatarios as &$destinatario ) {
			$this->AddBCC( $destinatario["usuemail"], $destinatario["usunome"] );
		}
		# anexa os arquivos
    
        if(!empty($arquivos['anexo']['name']['0'])) {
            foreach ($arquivos as $arquivo) {
                if (sizeof($arquivos['anexo']['name']) > 1) {
                    for ($ar = 0; $ar < sizeof($arquivos['anexo']['name']); $ar++) {

                        if (!empty($arquivo['name'][$ar])) {

                            if ($arquivo["error"][$ar] == UPLOAD_ERR_NO_FILE) {
                                continue;
                            }

                            $this->AddAttachment($destinoArquivo[$ar], basename($destinoArquivo[$ar]), 'base64', mime_content_type($destinoArquivo[$ar]));
                        }
                    }

                } else {
                    if ($arquivo["error"] == UPLOAD_ERR_NO_FILE) {
                        continue;
                    }

                    $this->AddAttachment($destinoArquivo, basename($destinoArquivo), 'base64', mime_content_type($destinoArquivo));
                }
            }
        }
		
		# formata assunto, conteudo e envia a mensagem
		$this->Subject = self::ASSUNTO . str_replace( "\'", "'", $assunto );
		$this->Body    = str_replace( "\'", "'", $conteudo );
		$this->IsHTML( true );
		set_time_limit( 180 );
				
		
		if( !$this->Send() ) {
			return false;
		}
		
		if($condicao){			
			return $this->registrar( $remetente, $destinatarios, $assunto, $conteudo );
		}else{			
			return true;
		}
	}
	
	public static function identificar_destinatarios( $orgao = null, $tipoEnsino = null, array $uo = array(), $ug = 0, array $perfis = array(), $outros = null, $statusUsuario = null, $ideb = null, $arMunicipios = array(), $cargo = null, $tpmid = array(), $esdid = null, $usustatus = null ) {
		global $db;
		
		$statusUsuario = ($statusUsuario ) ? $statusUsuario : 'A';

		//par?metro para filtrar (query) usu?rios s? ativos (envio de e-mail)
		$usustatus = ($usustatus) ? " and (u.usustatus = '".$usustatus."' or u.usustatus is null) " : '';
		
		$suscod = $statusUsuario ? " us.suscod = '". $statusUsuario ."' and ": "";

		# conjunto de regras de restri??o
		$restricao = array();
		# restringe os usu?rios por ?rg?o
		if ( !empty( $orgao ) ) {
			array_push( $restricao, " u.orgcod = '" . $orgao ."' " );
		}
		# restringe os usu?rios por tipo de ensino
		if ( !empty( $tipoEnsino ) ) {
			if ($tipoEnsino == 1){
				$funid = '12';	
			}elseif ($tipoEnsino == 2){
				$funid = '11';
			}
			array_push( $restricao, " (o.orgid = '" . $tipoEnsino ."' OR f.funid in ('" . $funid ."')) " );
		}
		
		# restringe os usu?rios por unidade or?ament?ria
		if ( !empty( $uo ) && !in_array( "", $uo ) ) {
			array_push( $restricao, " unicod in ( '". implode( "','", $uo ) ."' ) " );
		}
		# restringe os usu?rios por unidade gestora
		if ( !empty( $ug ) ) {
			array_push( $restricao, " ungcod = '" . $ug ."' " );
		}
		# restringe os usu?rios por cargo
		if ( !empty( $cargo ) ) {
			array_push( $restricao, " u.carid = '" . $cargo ."' " );
		}
		
		# restringe os usu?rios por perfil
		$join_perfil = '';
		/*if ( !empty( $perfis ) && !in_array( "", $perfis ) ) {
			$esquema = $_SESSION["sisdiretorio"];
			$join_perfil = sprintf(
				" inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf " .
				" inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.sisid = s.sisid and p.pflstatus = 'A' " .
				" and ( (
					p.pflresponsabilidade is not null and exists ( select pflcod from %s.usuarioresponsabilidade ur where ur.usucpf = u.usucpf and ur.pflcod = p.pflcod )
				) or (
					p.pflresponsabilidade is null
				) or p.pflresponsabilidade = 'N')",
				$esquema,
				$esquema,
				$esquema
			);
			
			$stPerfis = implode( ",", $perfis );
			
			//$stWherePerfil = " and (p.pflcod in ( $stPerfis ) or us.pflcod in ( $stPerfis ))";
			$stWherePerfil = " and (p.pflcod in ( $stPerfis ))";
		}*/
        if ( !empty($perfis) && !in_array('', $perfis) ) {
            $esquema = $_SESSION['sisdiretorio'];
            $whereNotNull = " p.pflresponsabilidade is not null ";
            //$whereExists = " exists (SELECT pflcod FROM {$esquema}.usuarioresponsabilidade ur WHERE ur.usucpf = u.usucpf AND ur.pflcod = p.pflcod) ";
            $stPerfis = implode(',', $perfis);

            $stWherePerfil = " AND (p.pflcod in ($stPerfis)) AND {$whereNotNull} /*AND {$whereExists}*/ ";
        }
		
		$joinMunTipoMunicipio = "";
		# restringe os usu?rios por tipo Grupo PAC
		if( !empty( $tpmid ) && !in_array( "", $tpmid ) )
		{
			$tpmid = implode(',', $tpmid);
			$joinMunTipoMunicipio = " inner join territorios.muntipomunicipio mtmn on mtmn.muncod = m.muncod and mtmn.tpmid in (".$tpmid.") ";
		} 
		
		$joinEstadoObra = "";
		# restringe os usu?rios por Estado da Obra
		if( !empty( $esdid ) )
		{
			$joinEstadoObra = " inner join obras.preobra po on po.muncod = m.muncod inner join workflow.documento doc on doc.docid = po.docid and doc.esdid = ".$esdid." ";
		}
		
		$TemarMunicipios	= ( $arMunicipios[0] != '' ) 		    ? TRUE : FALSE;
		$TemPerfis			= (count($perfis) > 0 ) 		? TRUE : FALSE;
		$TemCargo			= ($cargo != '' ) 				? TRUE : FALSE;
		
		if ( ( !$TemarMunicipios ) && ( !$TemPerfis ) && ( !$TemCargo ) && $outros )
		{
			$destinatarios = array();
		}
		else {
			if ( empty( $restricao ) ) {
				array_push( $restricao, " 1 = 1 " );
			}
			
			$joinIdeb = $ideb ? " inner join territorios.muntipomunicipio mtm on mtm.muncod = ur.muncod " : "";
			$clausulaIdeb = $ideb ?  " and mtm.tpmid in ( '". implode("', '", $ideb ) ."' ) " : "";
			
			$innerComplemento = "";

			if( $_SESSION['sisarquivo'] == 'cte' ) $innerComplemento = " left join cte.usuarioresponsabilidade ur on ur.usucpf = u.usucpf "; 
			
			if(!empty($tipoEnsino) ){
				$innerComplemento.= " left join academico.usuarioresponsabilidade ur on (u.usucpf = ur.usucpf )
									  left join academico.orgao o on (ur.orgid = o.orgid)
									  left join entidade.entidade e on ur.entid = e.entid
									  left join entidade.funcaoentidade f on f.entid = e.entid ";
			}

			/*
			$whereUF = "";
			$innerUF = "";
			
			if( count( $arUF ) && !in_array( "", $arUF ) ){
				$innerUF = " inner join territorios.municipio mu on mu.muncod = u.muncod ";  
				$whereUF = " and mu.estuf in ( '". implode( "', '", $arUF ) ."' ) " ;  
			}
			*/			
			$whereMunicipios = "";
			
			if( count( $arMunicipios ) && !in_array( "", $arMunicipios ) ){
				$whereMunicipios = " and u.muncod in ( '". implode( "', '", $arMunicipios ) ."' ) " ;  
			}
			
//			if(!empty($_POST['ptiid']) && $_POST['iniusuario'] && !$_POST['inisomente']){
//				
//				$stWhereEmendas = "AND u.usucpf IN (select distinct
//														ed.edecpfresp														
//													from emenda.ptiniciativa pti
//													inner join emenda.iniciativa 			i   on pti.iniid = i.iniid
//													inner join emenda.iniciativadetalheentidade 	ede on i.iniid = ede.iniid
//													inner join emenda.emendadetalheentidade 	ed  on ed.edeid = ede.edeid
//													where i.iniid = {$_REQUEST['ptiid']})";
//				
//			}
			
			if(!$_POST['inisomente']){

                $sql = sprintf(
                    "
                    SELECT DISTINCT
                        u.usunome, u.usuemail, u.usucpf, u.regcod, m.mundescricao
                    FROM seguranca.perfil p
                    JOIN seguranca.perfilusuario pu            ON p.pflcod = pu.pflcod
                    JOIN seguranca.usuario u                   ON u.usucpf = pu.usucpf
                    JOIN seguranca.usuario_sistema us          ON us.usucpf = u.usucpf
                    JOIN seguranca.sistema s                   ON s.sisid = us.sisid
                    left join territorios.municipio m          ON m.muncod = u.muncod
                    $joinMunTipoMunicipio
					$joinEstadoObra
					$innerUF
					$innerComplemento
					$joinIdeb
					%s
					where $suscod s.sisid = %d
					and s.sisstatus = 'A'
					$usustatus
					and %s $clausulaIdeb
					$whereMunicipios
					$stWherePerfil
					group by u.usunome, u.usuemail, u.usucpf, u.regcod, m.mundescricao",
                    $join_perfil,
                    $_SESSION['sisid'],
                    implode(' and ', $restricao)
                );
				//ver('1',$sql);
				$destinatarios = $db->carregar( $sql );
				$destinatarios = $destinatarios ? $destinatarios : array();
				sort($destinatarios);
			}
			
			/*
			 * FILTRO POR INICIAT?VA
			 */
			if(!empty($_POST['ptiid'])){
				
				$sql = "select distinct
							ed.edenomerep as usunome,
							ed.edemailresp as usuemail,
							ed.edecpfresp as usucpf,	
							u.regcod as regcod,
							m.mundescricao as mundescricao														
						from emenda.ptiniciativa pti
						inner join emenda.iniciativa 				i   on pti.iniid = i.iniid
						inner join emenda.iniciativadetalheentidade ede on i.iniid = ede.iniid
						inner join emenda.emendadetalheentidade 	ed  on ed.edeid = ede.edeid
						left join seguranca.usuario 				u   on u.usucpf = ed.edecpfresp
						left join territorios.municipio 			m   on u.muncod = m.muncod
						where i.iniid = {$_POST['ptiid']}
						and ed.edestatus = 'A'";
				//ver('2',$sql);
				
				$destinatariosIniciativa = $db->carregar( $sql );
				$destinatarios = $destinatarios ? $destinatarios : array();
				$destinatarios = array_merge($destinatariosIniciativa, $destinatarios);
//				$destinatarios = array_intersect($destinatariosIniciativa, $destinatarios);
				sort($destinatarios);
			}
			
			if ( !$destinatarios ) {
				$destinatarios = array();
			}
		}
		//die;
        $outros = self::identificar_outros_destinatarios( $destinatarios, $outros );
        //ver($_REQUEST, count($destinatarios), count($outros), d);
		return $outros;
	}
	
	public static function identificar_outros_destinatarios( array $destinatarios, $outros ) {
		global $db;
		foreach ( explode( ",", $outros ) as $item ) {
			if ( empty( $item ) ) {
				continue;
			}
			if ( strpos( $item, "<" ) ) {
				preg_match("/([\w\pL[:space:]]{0,})/", $item, $saida );
				$nome = strtoupper( $saida[0] );
				preg_match("/<(.*)>/", $item, $saida );
				$email = $saida[1];
			} else {
				$nome = "";
				$email = $item;
			}
			if ( empty( $nome ) ) {
				$nome = strtoupper( substr( $email, 0, strpos( $email, "@" ) ) );
			}
			$registro = array(
				"usucpf" => null,
				"usunome" => trim( $nome ),
				"usuemail" => trim( $email )
			);
			foreach ( $destinatarios as $destinatario ) {
				if ( $destinatario["usuemail"] == $registro["usuemail"] ) {
					continue 2;
				}
			}
			$sql = sprintf( "select usucpf from seguranca.usuario where usuemail = '%s'", $registro["usuemail"] );
			$registro["usucpf"] = $db->pegaUm( $sql );
			array_push( $destinatarios, $registro );
		}
		return $destinatarios;
	}
	
}
?>