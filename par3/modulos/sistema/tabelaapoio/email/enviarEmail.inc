<?php
set_time_limit(0);

$config = array();

$config[] = array(
	'titulo' => 'Curso GEM - Gestão para Educação Municipal',	
	'html' => APPRAIZ .'par3/modulos/sistema/tabelaapoio/email/MinutaOficioPrefeito.html',
	'sql'  => "SELECT 
				to_char(now(), 'DD') as dia,
				to_char(now(), 'MM') as mes,
				to_char(now(), 'YYYY') as ano,
				ie.entnome as nome,
				ie.entemail as email,
				ender.endlogradouro || ', ' || ender.endnumero || '- ' || endbairro as endereco, 
				ender.endcep as cep,
		        iu.estuf as uf,
		        iu.inudescricao as municipio,
				'http://simec.mec.gov.br/' as servername
				--'http://{$_SERVER['SERVER_NAME']}' as servername
			 FROM par3.instrumentounidade_entidade ie 
			 left join par3.instrumentounidade iu ON iu.inuid = ie.inuid
			 left join par3.endereco ender ON ender.endid = ie.endid
			 WHERE ie.entstatus = 'A' 
			 AND ie.entcpf IS NOT NULL 
			 AND ie.tenid = '2'",
);

$config[] = array(
	'titulo' => 'Curso GEM - Gestão para Educação Municipal',
	'html' => APPRAIZ .'par3/modulos/sistema/tabelaapoio/email/MinutaOficioDME.html',
	'sql'  => " SELECT
					to_char(now(), 'DD') as dia,
					to_char(now(), 'MM') as mes,
					to_char(now(), 'YYYY') as ano,
		            ie.entnome as nome,
		            ie.entemail as email,
		            ender.endlogradouro || ', ' || ender.endnumero || '- ' || endbairro as endereco, 
		            ender.endcep as cep,
		            iu.estuf as uf,
		            iu.inudescricao as municipio,
					'http://simec.mec.gov.br/' as servername
					--'http://{$_SERVER['SERVER_NAME']}' as servername
				 FROM par3.instrumentounidade_entidade ie 
				 left join par3.instrumentounidade iu ON iu.inuid = ie.inuid
				 left join par3.endereco ender ON ender.endid = ie.endid
				 WHERE 
				 	ie.entstatus = 'A' 
				 	AND ie.entcpf IS NOT NULL 
				 	AND ie.tenid = '4'"
);

$func = function ($value){
	return "#".$value."#";
};

if ($_GET['enviar'] == 't'){

	foreach ($config as $i => $c){
		$conteudoOriginal = file_get_contents($c['html']);
		$dado	  = $db->carregar($c['sql']);
		$continue = false;
		foreach ($dado as $d){
			$d['mes'] = strtolower( str_replace('Ç', 'ç', mes_extenso($d['mes'])) );
			$replace = array_values($d);
			$search  = array_keys($d);
			$search  = array_map($func, $search);
			$conteudo = str_replace($search, $replace, $conteudoOriginal);
	
			$remetente = array("nome" => "SIMEC", "email" => "noreply@mec.gov.br");
			
			$strEmailTo = array($d['email']);
			if (IS_PRODUCAO == false || $_GET['emaillocal']=='t') {
				$strEmailTo = array('felipechiavicatti@mec.gov.br', 'thiago.borboleta@mec.gov.br', 'thiagoborboleta@gmail.com');
				
				if ($continue){
					break;
				}
				$continue = true;
			}
			
			$strAssunto = $c['titulo'];
			$strAssunto = "=?ISO-8859-1?B?" . base64_encode($strAssunto) . "?=";
	// 	echo $conteudo;	
			$enviou = enviar_email($remetente, $strEmailTo, $strAssunto, $conteudo);
			
			$nome .= $d['nome'] . ' - ' . $d['email'] . '<br>';
		}
		echo "<b>Documento: {$c['html']}</b><br><br>{$nome}<br><br>";
		$nome = '';
	}

}
?>