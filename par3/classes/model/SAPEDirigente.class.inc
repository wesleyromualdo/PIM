<?php

class Par3_Model_SAPEDirigente extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "integracao.s_entidade_dirigente";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array();

    const SAPE_PREFEITURA = '02';
    const SAPE_ORGAO_ESTADUAL = '03';
    const SAPE_SECRETARIA_ESTADUAL_EDUCACAO = '10';
    const SAPE_SECRETARIA_MUNICIPAL_EDUCACAO = '28';

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'co_tp_entidade'=> null,
        'nu_cgc_entidade'=> null,
        'no_razao_social'=> null,
        'no_abreviado'=> null,
        'ds_endereco_entididade'=> null,
        'ds_compl_endereco_entidade'=> null,
        'no_bairro_entidade character'=> null,
        'sg_uf_entidade'=> null,
        'co_municipio_fnde_entidade'=> null,
        'no_municipio_entidade'=> null,
        'nu_cep_entidade'=> null,
        'nu_ddd_entidade'=> null,
        'nu_telefone_entidade'=> null,
        'ds_e_mail_entidade'=> null,
        'no_tp_entidade'=> null,
        'nu_cpf_dirigente'=> null,
        'no_dirigente'=> null,
        'id_sexo_dirigente'=> null,
        'ds_ender_dirigente'=> null,
        'ds_compl_ender'=> null,
        'no_bairro_dirigente'=> null,
        'sg_uf_dirigente'=> null,
        'co_municipio_fnde_dirigente'=> null,
        'no_municipio_dirigente'=> null,
        'nu_cep_dirigente'=> null,
        'nu_ddd_dirigente'=> null,
        'nu_tel_dirigente'=> null,
        'ds_e_mail_dirigente'=> null,
        'no_cargo'=> null,
        'nu_identidade_dirigente'=> null,
        'sg_orgao_emissor'=> null,
        'dt_carga'=> null,
    	'co_municipio_ibge_entidade'=> null,
    	'co_municipio_ibge_dirigente'=> null,
	);

	public function retonarEntidade($cotp, $estuf, $muncod = null)
	{
	    $where = array();
	    $where[] = "co_tp_entidade = '$cotp'";

	    if ($muncod) {
//     	    $muncod = substr($muncod, 0, -1);
		    $where[] = "co_municipio_ibge_entidade::numeric = '{$muncod}'::numeric";
	    }else{
    	    $where[] = "sg_uf_entidade = '{$estuf}'";
	    }

		$sql = "SELECT
                	nu_cgc_entidade as entcnpj,
                	UPPER(no_razao_social_entidade) as entrazaosocial,
                	UPPER(no_abreviado_entidade) as entnome,
                	ds_e_mail_entidade as entemail,
                	'' as entorgexpedidor,
                	null as entdtnascimento,
                	TRIM(LEADING '0' FROM nu_ddd_entidade)||nu_telefone_entidade as enttelefonecomercial,
                	'' as enttelefonecelular,
                	nu_cep_entidade as endcep,
                	ds_endereco_entididade as endlogradouro,
                	ds_compl_endereco_entidade as endcomplemento,
                	0 as endnumero,
                	no_bairro_entidade as endbairro
                FROM
                	integracao.s_entidade_dirigente
                WHERE ".implode(' AND ', $where)."
                ORDER BY
                	dt_carga DESC
                LIMIT 1";
        //ver($sql);
		$result = adapterConnection::coorporativo()->pegaLinha($sql);

		return is_array($result) ? $result : null;
	}


	public function retonarDirigente($cotp, $estuf, $muncod = null)
	{
	    $where = array();
	    $where[] = "co_tp_entidade = '$cotp'";

	    if ($muncod) {
	        $where[] = "co_municipio_ibge_entidade::numeric = '{$muncod}'::numeric";
	    }else{
	        $where[] = "sg_uf_dirigente = '{$estuf}'";
	    }

	    $sql = "SELECT
        		    nu_cpf_dirigente as entcpf,
                	UPPER(no_dirigente) as entnome,
                	LOWER(id_sexo_dirigente) as entsexo,
                	ds_e_mail_dirigente as entemail,
                	nu_identidade_dirigente as entrg,
                	sg_orgao_emissor_dirigente as entorgexpedidor,
                	null as entdtnascimento,
                	TRIM(LEADING '0' FROM nu_ddd_dirigente)||nu_tel_dirigente as enttelefonecomercial,
                	'' as enttelefonecelular,
                	nu_cep_dirigente as endcep,
                	ds_ender_dirigente as endlogradouro,
                	ds_compl_ender_dirigente as endcomplemento,
                	0 as endnumero,
                	no_bairro_dirigente as endbairro,
                    dt_ini_vinculacao as entdt_inicio_mandato,
                    dt_fim_vinculacao as entdt_fim_mandato
                FROM
                	integracao.s_entidade_dirigente
                WHERE ".implode(' AND ', $where)."
                ORDER BY
                	dt_carga DESC
                LIMIT 1";

	    $result = adapterConnection::coorporativo()->pegaLinha($sql);

	    return is_array($result) ? $result : null;
	}


	public function retonarEXDirigente($cotp, $estuf, $muncod = null)
	{
	    $where = array();
	    $where[] = "co_tp_entidade = '$cotp'";
	    $where[] = "dt_fim_vinculacao IS NOT NULL";

	    if ($muncod) {
	        $where[] = "co_municipio_ibge_entidade::numeric = '{$muncod}'::numeric";
	        $where[] = "(co_municipio_ibge_entidade, dt_fim_vinculacao) IN
	        (SELECT co_municipio_ibge_entidade, MIN(dt_fim_vinculacao)
	        FROM integracao.s_entidade_dirigente
	        WHERE co_tp_entidade = '$cotp' AND dt_fim_vinculacao IS NOT NULL
	        GROUP BY co_municipio_ibge_entidade)";
	    }else{
	        $where[] = "sg_uf_dirigente = '{$estuf}'";
	        $where[] = "(sg_uf_dirigente, dt_fim_vinculacao) IN
	        (SELECT sg_uf_dirigente, MIN(dt_fim_vinculacao)
	        FROM integracao.s_entidade_dirigente
	        WHERE co_tp_entidade = '$cotp' AND dt_fim_vinculacao IS NOT NULL
	        GROUP BY sg_uf_dirigente)";
	        }

	        $sql = "SELECT
	        nu_cpf_dirigente as entcpf,
	        UPPER(no_dirigente) as entnome,
                	LOWER(id_sexo_dirigente) as entsexo,
                	ds_e_mail_dirigente as entemail,
                	nu_identidade_dirigente as entrg,
                	sg_orgao_emissor_dirigente as entorgexpedidor,
                	null as entdtnascimento,
                	TRIM(LEADING '0' FROM nu_ddd_dirigente)||nu_tel_dirigente as enttelefonecomercial,
                	'' as enttelefonecelular,
                	nu_cep_dirigente as endcep,
                	ds_ender_dirigente as endlogradouro,
                	ds_compl_ender_dirigente as endcomplemento,
                	0 as endnumero,
                	no_bairro_dirigente as endbairro,
                    dt_ini_vinculacao as entdt_inicio_mandato,
                    dt_fim_vinculacao as entdt_fim_mandato
                FROM
                	integracao.s_entidade_dirigente
                WHERE ".implode(' AND ', $where)."
                ORDER BY
                	dt_carga DESC
	            LIMIT 1";
	            //ver($sql);
		$result = adapterConnection::coorporativo()->pegaLinha($sql);

			return is_array($result) ? $result : null;
	}
	
	
	public function retornaTotalAtualizacao(  )
	{
		
		$result = adapterConnection::coorporativo()->pegaUm("select count( entcpf )  from integracao.s_entidade_dirigente_atualizacao_rotina");
	
		return $result;
	}

	public function retornaCPFsAtualizados( $dados = Array() )
	{
	    $limit 		= ($dados['limit']) ? "LIMIT " . $dados['limit'] : '';
	    $offset 	= ($dados['offset']) ? "OFFSET " . $dados['offset'] : '';
	    
		$result = adapterConnection::coorporativo()->carregar("select distinct entcpf from integracao.s_entidade_dirigente_atualizacao_rotina {$limit} {$offset}");
		$result = ( is_array($result) ) ? $result : Array();
		if(count($result) > 0 )
		{
			foreach($result as $k => $v )
			{
				
				$retorno[] = "'{$v['entcpf']}'";
			}
			
			return "And u.usucpf IN (" . implode(', ', $retorno) . ")";
		}
		else 
		{
			return '';
		}
	}
	public function retonarTodosDirigentes( $cotp, $dt = Null, $dados )
	{
		$where = array();
		if( is_array($cotp) ){
			$where[] 	= "co_tp_entidade in ('".implode("','", $cotp)."')";
		} else {
			$where[] 	= "co_tp_entidade = '$cotp'";
		}
		// $where[] 	= "to_char(dt_atualizacao, 'YYYY')::INTEGER >= 2017";
		// $where[] 	= "dt_fim_vinculacao IS NULL"; 
		$limit 		= ($dados['limit']) ? "LIMIT " . $dados['limit'] : '';
		$offset 	= ($dados['offset']) ? "OFFSET " . $dados['offset'] : '';

		if($dados['cpf']){
    		$where[] 	= "entcpf = '{$dados['cpf']}'";
		}

		if($dt)
		{
		//	$where[] = "dt_atualizacao >= '$dt'::date";
		}

		$sql = "SELECT
        		    entcpf,
					muncod,
					estuf,
                	UPPER(entnome) AS entnome,
                	LOWER(entsexo) AS entsexo,
                	entemail ,
                	entrg,
                	entorgexpedidor,
                	null as entdtnascimento,
                	entnumdddcomercial,
                	entnumcomercial,
                	enttelefonecelular,
                	endcep,
					co_municipio_ibge_dirigente,
					sg_uf_dirigente,
                	endlogradouro,
                	endcomplemento,
                	0 as endnumero,
                	endbairro,
					co_tp_entidade,
		    		edafonecomercial,
					edafonecelular,
					edacnpj,
					edarazaosocial,
					edamunmandato,
					edadtiniciomandato,
					edafuncao,
					edaendereco,
					edacep,
					edamunicipio,
					edabairro
		    		
		    		
                FROM
                    integracao.s_entidade_dirigente_atualizacao_rotina
                WHERE ".implode(' AND ', $where)."
                ORDER BY
                	entcpf DESC
               	{$limit}
                {$offset}
              ";
		
		$result = adapterConnection::coorporativo()->carregar($sql);

		return is_array($result) ? $result : null;
	}
	
	public function guardaHistoricoRotina(  )
	{
		$sqlInsert = " 
			INSERT INTO integracao.historico_atualizacoes_rotina(
			            co_tp_entidade, nu_cgc_entidade, no_razao_social_entidade, no_abreviado_entidade, 
			            ds_endereco_entididade, ds_compl_endereco_entidade, no_bairro_entidade, 
			            sg_uf_entidade, co_municipio_ibge_entidade, no_municipio_entidade, 
			            nu_cep_entidade, nu_ddd_entidade, nu_telefone_entidade, ds_e_mail_entidade, 
			            no_tp_entidade, nu_cpf_dirigente, no_dirigente, id_sexo_dirigente, 
			            ds_ender_dirigente, ds_compl_ender_dirigente, no_bairro_dirigente, 
			            sg_uf_dirigente, co_municipio_ibge_dirigente, no_municipio_dirigente, 
			            nu_cep_dirigente, nu_ddd_dirigente, nu_tel_dirigente, ds_e_mail_dirigente, 
			            no_cargo_dirigente, nu_identidade_dirigente, sg_orgao_emissor_dirigente, 
			            dt_carga, dt_atualizacao, dt_ini_vinculacao, dt_fim_vinculacao, 
			            dt_nascimento_dirigente, dt_alteracao_dirigente)
			            
			     (select sed.co_tp_entidade, sed.nu_cgc_entidade, sed.no_razao_social_entidade, sed.no_abreviado_entidade, 
			            sed.ds_endereco_entididade, sed.ds_compl_endereco_entidade, sed.no_bairro_entidade, 
			            sed.sg_uf_entidade, sed.co_municipio_ibge_entidade, sed.no_municipio_entidade, 
			            sed.nu_cep_entidade, sed.nu_ddd_entidade, sed.nu_telefone_entidade, sed.ds_e_mail_entidade, 
			            sed.no_tp_entidade, sed.nu_cpf_dirigente, sed.no_dirigente, sed.id_sexo_dirigente, 
			            sed.ds_ender_dirigente, sed.ds_compl_ender_dirigente, sed.no_bairro_dirigente, 
			            sed.sg_uf_dirigente, sed.co_municipio_ibge_dirigente, sed.no_municipio_dirigente, 
			            sed.nu_cep_dirigente, sed.nu_ddd_dirigente, sed.nu_tel_dirigente, sed.ds_e_mail_dirigente, 
			            sed.no_cargo_dirigente, sed.nu_identidade_dirigente, sed.sg_orgao_emissor_dirigente, 
			            sed.dt_carga, sed.dt_atualizacao, sed.dt_ini_vinculacao, sed.dt_fim_vinculacao, 
			            sed.dt_nascimento_dirigente, sed.dt_alteracao_dirigente 
					from 
					integracao.s_entidade_dirigente sed
					left JOIN  integracao.s_entidade_dirigente_espelho esp ON 
					 
					esp.co_tp_entidade = sed.co_tp_entidade
					AND esp.co_municipio_ibge_entidade = sed.co_municipio_ibge_entidade
			
					WHERE to_char(sed.dt_atualizacao, 'YYYY')::INTEGER >= 2017
			
					AND
					(
						(
							esp.nu_cpf_dirigente 				<> sed.nu_cpf_dirigente 			
							OR esp.co_municipio_ibge_entidade 	<> sed.co_municipio_ibge_entidade	
							OR esp.sg_uf_entidade 				<> sed.sg_uf_entidade 			
							OR esp.no_dirigente 				<> sed.no_dirigente			
							OR esp.id_sexo_dirigente 			<> sed.id_sexo_dirigente			
							OR esp.ds_e_mail_dirigente 			<> sed.ds_e_mail_dirigente		
							OR esp.nu_identidade_dirigente 		<> sed.nu_identidade_dirigente		
							OR esp.sg_orgao_emissor_dirigente 	<> sed.sg_orgao_emissor_dirigente	
							OR esp.nu_ddd_dirigente 			<> sed.nu_ddd_dirigente			
							OR esp.nu_ddd_entidade 				<> sed.nu_ddd_entidade			
							OR esp.nu_tel_dirigente  			<> sed.nu_tel_dirigente 			
							OR esp.nu_telefone_entidade  		<> sed.nu_telefone_entidade 			
							OR esp.nu_cep_entidade 				<> sed.nu_cep_entidade 			
							OR esp.ds_endereco_entididade 		<> sed.ds_endereco_entididade		
							OR esp.ds_compl_endereco_entidade 	<> sed.ds_compl_endereco_entidade	
							OR esp.no_bairro_entidade	 		<> sed.no_bairro_entidade		
							OR esp.co_tp_entidade 				<> sed.co_tp_entidade			
							OR esp.dt_nascimento_dirigente 		<> sed.dt_nascimento_dirigente				
						)
					OR
						esp.nu_cpf_dirigente is null
					)
			
			            );
		";
		
		$result = adapterConnection::coorporativo()->executar($sqlInsert);
		return adapterConnection::coorporativo()->commit();
	}
	public function comparaTabelaComEspelho( $cotp, $dt = Null, $dados )
	{
		
		$sql = "
			SELECT
				sed.nu_cpf_dirigente,
				sed.co_municipio_ibge_entidade,
				sed.no_dirigente,
				sed.id_sexo_dirigente,
				sed.ds_e_mail_dirigente,
				sed.nu_identidade_dirigente,
				sed.sg_orgao_emissor_dirigente,
				sed.nu_ddd_dirigente,
				sed.nu_ddd_entidade,
				sed.nu_tel_dirigente,
				sed.nu_telefone_entidade,
				sed.nu_cep_entidade,
				sed.sg_uf_entidade,
				sed.dt_nascimento_dirigente,
				sed.ds_endereco_entididade,
				sed.ds_compl_endereco_entidade,
				sed.no_bairro_entidade,
				sed.co_tp_entidade,
		    	sed.nu_cgc_entidade,
				sed.no_razao_social_entidade,
				sed.no_municipio_entidade,
				sed.dt_ini_vinculacao,
				sed.no_cargo_dirigente,
				sed.nu_ddd_entidade || sed.nu_telefone_entidade as edafonecomercial,
				sed.nu_ddd_dirigente || sed.nu_tel_dirigente as edafonecelular,
				sed.ds_ender_dirigente,
				sed.nu_cep_dirigente,
				sed.no_municipio_dirigente,
				sed.sg_uf_dirigente,
				sed.no_bairro_dirigente
			
			from 
			integracao.s_entidade_dirigente sed
			LEFT JOIN  integracao.s_entidade_dirigente_espelho esp ON
			esp.co_tp_entidade = sed.co_tp_entidade
			AND esp.co_municipio_ibge_entidade = sed.co_municipio_ibge_entidade
			
			WHERE to_char(sed.dt_atualizacao, 'YYYY')::INTEGER >= 2017 AND sed.dt_fim_vinculacao IS NULL
			
			AND
			(	
				(
					esp.nu_cpf_dirigente 				<> sed.nu_cpf_dirigente 			
					OR esp.co_municipio_ibge_entidade 	<> sed.co_municipio_ibge_entidade	
					OR esp.sg_uf_entidade 				<> sed.sg_uf_entidade 			
					/* 	
		    			OR esp.no_dirigente 				<> sed.no_dirigente -- Comentário Necessário como contorno do erro de acentuação no Serviço que disponibiliza os dados 
		    		*/
					OR esp.id_sexo_dirigente 			<> sed.id_sexo_dirigente			
					OR esp.ds_e_mail_dirigente 			<> sed.ds_e_mail_dirigente		
					OR esp.nu_identidade_dirigente 		<> sed.nu_identidade_dirigente		
					OR esp.sg_orgao_emissor_dirigente 	<> sed.sg_orgao_emissor_dirigente	
					OR esp.nu_ddd_dirigente 			<> sed.nu_ddd_dirigente			
					OR esp.nu_ddd_entidade 				<> sed.nu_ddd_entidade			
					OR esp.nu_tel_dirigente  			<> sed.nu_tel_dirigente 			
					OR esp.nu_telefone_entidade  		<> sed.nu_telefone_entidade 			
					OR esp.nu_cep_entidade 				<> sed.nu_cep_entidade 			
					/* 	OR esp.ds_endereco_entididade 		<> sed.ds_endereco_entididade		
						OR esp.ds_compl_endereco_entidade 	<> sed.ds_compl_endereco_entidade	
						OR esp.no_bairro_entidade	 		<> sed.no_bairro_entidade
			    		-- Comentário Necessário como contorno do erro de acentuação no Serviço que disponibiliza os dados
					*/		
					OR esp.co_tp_entidade 				<> sed.co_tp_entidade			
					OR esp.dt_nascimento_dirigente 		<> sed.dt_nascimento_dirigente	
					OR esp.dt_atualizacao 				<> sed.dt_atualizacao						
				)
				OR
				esp.nu_cpf_dirigente is null
			)
				AND sed.co_tp_entidade in ('02','10') /* <-- Por enquanto atualiza apenas o prefeito e o secretário estadual */

		    	AND sed.dt_alteracao_dirigente > (current_date - 60) /* Comentário Necessário como contorno do erro de acentuação no Serviço que disponibiliza os dados, mesmo comentando os campos de acento só iremos pegar atualizados nos ultimos 60 dias como precaução extra */
		    
		    
              ";

		$result = adapterConnection::coorporativo()->carregar($sql);

		return is_array($result) ? $result : array();
	}
	
	public function validaEmailRotina($email) 
	{
		$er = "/^(([0-9a-zA-Z]+[-._+&])*[0-9a-zA-Z_-]+@([-0-9a-zA-Z]+[.])+[a-zA-Z]{2,6}){0,1}$/";
	    if (preg_match($er, $email)){
			return true;
	    } else {
			return false;
	    }
	}
	
	
	/*
	 * Tabela responsável por armazenar os erros que aconteçam
	 *
	 * */
	public function insereLogSucesso( $arrRegistro , $idAcao )
	{
		// Monta as variáveis para inserir
		$nu_cpf_dirigente             = ($arrRegistro['entcpf']) ? "'{$arrRegistro['entcpf']}'" : 'NULL';
		$co_municipio_ibge_entidade   = ($arrRegistro['muncod']) ? "'{$arrRegistro['muncod']}'" : 'NULL';
		$no_dirigente                 = ($arrRegistro['entnome']) ? "'{$arrRegistro['entnome']}'" : 'NULL';
		$sg_uf_entidade               = ($arrRegistro['estuf']) ? "'{$arrRegistro['estuf']}'" : 'NULL';
		$id_sexo_dirigente            = ($arrRegistro['entsexo']) ? "'{$arrRegistro['entsexo']}'" : 'NULL';
		$ds_e_mail_dirigente          = ($arrRegistro['entemail']) ? "'{$arrRegistro['entemail']}'" : 'NULL';
		$nu_identidade_dirigente      = ($arrRegistro['entrg']) ? "'{$arrRegistro['entrg']}'" : 'NULL';
		$sg_orgao_emissor_dirigente   = ($arrRegistro['entorgexpedidor']) ? "'{$arrRegistro['entorgexpedidor']}'" : 'NULL';
		$nu_ddd_dirigente             = ($arrRegistro['entnumdddcomercial']) ? "'{$arrRegistro['entnumdddcomercial']}'" : 'NULL';
		$nu_tel_dirigente             = ($arrRegistro['entnumcomercial']) ? "'{$arrRegistro['entnumcomercial']}'" : 'NULL';
		$nu_cep_entidade              = ($arrRegistro['endcep']) ? "'{$arrRegistro['endcep']}'" : 'NULL';
		$ds_endereco_entididade       = ($arrRegistro['endlogradouro']) ? "'{$arrRegistro['endlogradouro']}'" : 'NULL';
		$ds_compl_endereco_entidade   = ($arrRegistro['endcomplemento']) ? "'{$arrRegistro['endcomplemento']}'" : 'NULL';
		$no_bairro_entidade           = ($arrRegistro['endbairro']) ? "'{$arrRegistro['endbairro']}'" : 'NULL';
		$co_tp_entidade               = ($arrRegistro['co_tp_entidade']) ? "'{$arrRegistro['co_tp_entidade']}'" : 'NULL';
		$dtNascimento	 			  = ($arrNovaEntidade['entdtnascimento'])           ? "'{$arrNovaEntidade['entdtnascimento']}'"       : 'NULL';
		$acao						  = $idAcao;
		
		
		$sqlInsertLogErro =
		"INSERT INTO
		integracao.log_rotina_entidade_dirigente
		(
			entcpf,
			entnome,
			muncod,
			estuf,
			entsexo,
			entemail,
			entrg,
			entorgexpedidor,
			entnumdddcomercial,
			entnumcomercial,
			endcep,
			co_municipio_ibge_dirigente,
			sg_uf_dirigente,
			endlogradouro,
			endcomplemento,
			endbairro,
			co_tp_entidade,
			entdtnascimento,
			alrid
		)
		VALUES
		(
			{$nu_cpf_dirigente},
			{$no_dirigente},
			{$co_municipio_ibge_entidade},
			{$sg_uf_entidade},
			{$id_sexo_dirigente},
			{$ds_e_mail_dirigente},
			{$nu_identidade_dirigente},
			{$sg_orgao_emissor_dirigente},
			{$nu_ddd_dirigente},
			{$nu_tel_dirigente},
			{$nu_cep_entidade},
			{$co_municipio_ibge_entidade},
			{$sg_uf_entidade},
			{$ds_endereco_entididade},
			{$ds_compl_endereco_entidade},
			{$no_bairro_entidade},
			{$co_tp_entidade},
			{$dtNascimento},
			{$acao}
		)";
		// Executa a inserção do log
		$result = adapterConnection::coorporativo()->executar($sqlInsertLogErro);
		return adapterConnection::coorporativo()->commit();
		
	}
	
	/*
	 * Tabela responsável por armazenar os erros que aconteçam
	 * 
	 * */
	public function insereLogErro( $arrRegistro , $motivo = '' )
	{
		// Caso esteja correto
		if(is_array($arrRegistro) && count($arrRegistro) > 0)
		{		
			// Trata os valores para inserir
			$nu_cpf_dirigente             = ($arrRegistro['nu_cpf_dirigente']) ? "'{$arrRegistro['nu_cpf_dirigente']}'" : 'NULL';
			$co_municipio_ibge_entidade   = ($arrRegistro['co_municipio_ibge_entidade']) ? "'{$arrRegistro['co_municipio_ibge_entidade']}'" : 'NULL';
			$no_dirigente                 = ($arrRegistro['no_dirigente']) ? "'{$arrRegistro['no_dirigente']}'" : 'NULL';
			$id_sexo_dirigente            = ($arrRegistro['id_sexo_dirigente']) ? "'{$arrRegistro['id_sexo_dirigente']}'" : 'NULL';
			$ds_e_mail_dirigente          = ($arrRegistro['ds_e_mail_dirigente']) ? "'{$arrRegistro['ds_e_mail_dirigente']}'" : 'NULL';
			$nu_identidade_dirigente      = ($arrRegistro['nu_identidade_dirigente']) ? "'{$arrRegistro['nu_identidade_dirigente']}'" : 'NULL';
			$sg_orgao_emissor_dirigente   = ($arrRegistro['sg_orgao_emissor_dirigente']) ? "'{$arrRegistro['sg_orgao_emissor_dirigente']}'" : 'NULL';
			$nu_ddd_dirigente             = ($arrRegistro['nu_ddd_dirigente']) ? "'{$arrRegistro['nu_ddd_dirigente']}'" : 'NULL';
			$nu_tel_dirigente             = ($arrRegistro['nu_tel_dirigente']) ? "'{$arrRegistro['nu_tel_dirigente']}'" : 'NULL';
			$nu_cep_entidade              = ($arrRegistro['nu_cep_entidade']) ? "'{$arrRegistro['nu_cep_entidade']}'" : 'NULL';
			$sg_uf_entidade               = ($arrRegistro['sg_uf_entidade']) ? "'{$arrRegistro['sg_uf_entidade']}'" : 'NULL';
			$ds_endereco_entididade       = ($arrRegistro['ds_endereco_entididade']) ? "'{$arrRegistro['ds_endereco_entididade']}'" : 'NULL';
			$ds_compl_endereco_entidade   = ($arrRegistro['ds_compl_endereco_entidade']) ? "'{$arrRegistro['ds_compl_endereco_entidade']}'" : 'NULL';
			$no_bairro_entidade           = ($arrRegistro['no_bairro_entidade']) ? "'{$arrRegistro['no_bairro_entidade']}'" : 'NULL';
			$co_tp_entidade               = ($arrRegistro['co_tp_entidade']) ? "'{$arrRegistro['co_tp_entidade']}'" : 'NULL';
			$acao						  = "1";
			$motivo						  = "'{$motivo}'";
			// Monta query
			$sqlInsertLogErro = 
			"INSERT INTO
					integracao.log_rotina_entidade_dirigente
			(
				entcpf,
				entnome,
				muncod,
				estuf,
				entsexo,
				entemail,
				entrg,
				entorgexpedidor,
				entnumdddcomercial,
				entnumcomercial,
				endcep,
				co_municipio_ibge_dirigente,
				sg_uf_dirigente,
				endlogradouro,
				endcomplemento,
				endbairro,
				co_tp_entidade,
				alrid,
				lrdmotivo
			)
			VALUES
			(
				{$nu_cpf_dirigente},
				{$no_dirigente},
				{$co_municipio_ibge_entidade},
				{$sg_uf_entidade},
				{$id_sexo_dirigente},
				{$ds_e_mail_dirigente},
				{$nu_identidade_dirigente},
				{$sg_orgao_emissor_dirigente},
				{$nu_ddd_dirigente},
				{$nu_tel_dirigente},
				{$nu_cep_entidade},
				{$co_municipio_ibge_entidade},
				{$sg_uf_entidade},
				{$ds_endereco_entididade},
				{$ds_compl_endereco_entidade},
				{$no_bairro_entidade},
				{$co_tp_entidade},
				{$acao},
				{$motivo}
			)		
					";
			// Executa a inserção do log
			$result = adapterConnection::coorporativo()->executar($sqlInsertLogErro);
			return adapterConnection::coorporativo()->commit();
		}		
	}
	/**
	 * Deleta os registros de atualização anteriores
	 * */
	public function deletaRegistrosAtualizacao($cpfDirigentes = null)
	{
	    $where = '';
	    if($cpfDirigentes){
	        $implodeCpf = implode(',',array_map(function($cpf){ return "'$cpf'" ;},$cpfDirigentes));
	        $where = " where entcpf  not in({$implodeCpf})";
        }
		/*Esvazia a tabela*/
		$sql = "DELETE FROM integracao.s_entidade_dirigente_atualizacao_rotina {$where}";
		$result = adapterConnection::coorporativo()->executar($sql);
		return adapterConnection::coorporativo()->commit();
	}
	
	/**
	 * Deleta os registros de atualização anteriores
	 * */
	public function deletaRegistrosEspelho(  )
	{
		/*Esvazia a tabela*/
		$sql = "DELETE FROM integracao.s_entidade_dirigente_espelho";
		$result = adapterConnection::coorporativo()->executar($sql);
		return adapterConnection::coorporativo()->commit();		
	}
	/**
	 * Deleta os registros de atualização anteriores
	 * */
	public function insereRegistrosEspelho(  )
	{
		/*Esvazia a tabela*/
		$sql = "INSERT INTO integracao.s_entidade_dirigente_espelho(
		            co_tp_entidade, nu_cgc_entidade, no_razao_social_entidade, no_abreviado_entidade, 
		            ds_endereco_entididade, ds_compl_endereco_entidade, no_bairro_entidade, 
		            sg_uf_entidade, co_municipio_ibge_entidade, no_municipio_entidade, 
		            nu_cep_entidade, nu_ddd_entidade, nu_telefone_entidade, ds_e_mail_entidade, 
		            no_tp_entidade, nu_cpf_dirigente, no_dirigente, id_sexo_dirigente, 
		            ds_ender_dirigente, ds_compl_ender_dirigente, no_bairro_dirigente, 
		            sg_uf_dirigente, co_municipio_ibge_dirigente, no_municipio_dirigente, 
		            nu_cep_dirigente, nu_ddd_dirigente, nu_tel_dirigente, ds_e_mail_dirigente, 
		            no_cargo_dirigente, nu_identidade_dirigente, sg_orgao_emissor_dirigente, 
		            dt_carga, dt_atualizacao, dt_ini_vinculacao, dt_fim_vinculacao, 
		            dt_nascimento_dirigente, dt_alteracao_dirigente)
		            
		     (select co_tp_entidade, nu_cgc_entidade, no_razao_social_entidade, no_abreviado_entidade, 
		            ds_endereco_entididade, ds_compl_endereco_entidade, no_bairro_entidade, 
		            sg_uf_entidade, co_municipio_ibge_entidade, no_municipio_entidade, 
		            nu_cep_entidade, nu_ddd_entidade, nu_telefone_entidade, ds_e_mail_entidade, 
		            no_tp_entidade, nu_cpf_dirigente, no_dirigente, id_sexo_dirigente, 
		            ds_ender_dirigente, ds_compl_ender_dirigente, no_bairro_dirigente, 
		            sg_uf_dirigente, co_municipio_ibge_dirigente, no_municipio_dirigente, 
		            nu_cep_dirigente, nu_ddd_dirigente, nu_tel_dirigente, ds_e_mail_dirigente, 
		            no_cargo_dirigente, nu_identidade_dirigente, sg_orgao_emissor_dirigente, 
		            dt_carga, dt_atualizacao, dt_ini_vinculacao, dt_fim_vinculacao, 
		            dt_nascimento_dirigente, dt_alteracao_dirigente from  integracao.s_entidade_dirigente);";
		$result = adapterConnection::coorporativo()->executar($sql);
		return adapterConnection::coorporativo()->commit();		
	}
	
	
	/**
	 * Insere os dados para a atualização
	 * */
	public function insereDadosParaAtualizacao( $arrModificados )
	{
		/* Referencia dos campos */
		$arrCampos['nu_cpf_dirigente']['nome_campo']				= "entcpf";
		$arrCampos['no_dirigente']['nome_campo']					= "entnome";
		$arrCampos['co_municipio_ibge_entidade']['nome_campo']		= "muncod";
		$arrCampos['sg_uf_entidade']['nome_campo']					= "estuf";
		$arrCampos['dt_nascimento_dirigente']['nome_campo']			= "entdtnascimento";
		$arrCampos['id_sexo_dirigente']['nome_campo']				= "entsexo";
		$arrCampos['ds_e_mail_dirigente']['nome_campo']				= "entemail";
		$arrCampos['nu_identidade_dirigente']['nome_campo']			= "entrg";
		$arrCampos['sg_orgao_emissor_dirigente']['nome_campo']		= "entorgexpedidor";
		$arrCampos['nu_ddd_dirigente']['nome_campo']				= "entnumdddcomercial";
		$arrCampos['nu_tel_dirigente']['nome_campo']				= "entnumcomercial";
		$arrCampos['nu_cep_entidade']['nome_campo']					= "endcep";
		$arrCampos['co_municipio_ibge_entidade']['nome_campo']		= "co_municipio_ibge_dirigente";
		$arrCampos['sg_uf_dirigente']['nome_campo']					= "sg_uf_dirigente";
		$arrCampos['ds_endereco_entididade']['nome_campo']			= "endlogradouro";
		$arrCampos['ds_compl_endereco_entidade']['nome_campo']		= "endcomplemento";
		$arrCampos['no_bairro_entidade']['nome_campo']				= "endbairro";
		$arrCampos['co_tp_entidade']['nome_campo']					= "co_tp_entidade";
		$arrCampos['edafonecomercial']['nome_campo']				= "edafonecomercial";
		$arrCampos['edafonecelular']['nome_campo']					= "edafonecelular";
		$arrCampos['nu_cgc_entidade']['nome_campo']					= "edacnpj";
		$arrCampos['no_razao_social_entidade']['nome_campo']		= "edarazaosocial";
		$arrCampos['no_municipio_entidade']['nome_campo']			= "edamunmandato";
		$arrCampos['dt_ini_vinculacao']['nome_campo']				= "edadtiniciomandato";
		$arrCampos['no_cargo_dirigente']['nome_campo']				= "edafuncao";
		$arrCampos['ds_ender_dirigente']['nome_campo']				= "edaendereco";
		$arrCampos['nu_cep_dirigente']['nome_campo']				= "edacep";
		$arrCampos['no_municipio_dirigente']['nome_campo']			= "edamunicipio";
		$arrCampos['no_bairro_dirigente']['nome_campo']				= "edabairro";
		
		/**
		 * Caso possua registros para serem inseridos, da procedimento
		 * */
		if(is_array($arrModificados) && ($arrModificados > 0))
		{
			/*Cicla os registros que vieram para serem inseridos*/
			foreach($arrModificados as $key => $registro)
			{
			    $cpf = '';
			    $cpf = $registro['nu_cpf_dirigente'];
			    if( $cpf != '' ) {
			        $sqlExiste = "SELECT entcpf FROM integracao.s_entidade_dirigente_atualizacao_rotina where entcpf = '{$cpf}'";
			        $arrExiste = adapterConnection::coorporativo()->pegaLinha($sqlExiste);
			        $existe = (is_array($arrExiste)) ? TRUE : FALSE; 
			        
			        if($existe) {
			            continue;
			        } 
			    }
				// Carrega informações a serem inseridas
				$arrInfoInsert = array();
				// Caso tenham campos a serem inseridos
				if(is_array($registro))
				{
					// Monta array para o insert
					foreach($registro as $campo => $valor)
					{
						// O tratamento para municipio e estado é necessário porque eles preenchem 2 campos e não somente um
						if( $campo == 'co_municipio_ibge_entidade' )
						{
							$arrInfoInsert['muncod'] = 	($valor) ? "'$valor'" : "NULL";
						}
						elseif( $campo == 'sg_uf_entidade' )
						{
							$arrInfoInsert['estuf'] = 	($valor) ? "'$valor'" : "NULL";
						}
						// Os demais campos apenas são atualizados normalmente
						$arrInfoInsert[$arrCampos[$campo]['nome_campo']] = 	($valor) ? "'$valor'" : "NULL";
					}
					// Caso exista dados a serem inseridos
					if(count($arrInfoInsert) > 0 )
					{
						// Para montar os campos com a vírgula
						// Conta o total
						$tot = count($arrInfoInsert);
						// Inicia um contador
						$i = 0;
						// Zera os valores necessários
						$colunas = '';
						$valores = '';
						// Monta as colunas e os valores
						foreach ($arrInfoInsert as $coluna => $val )
						{
							// Conta a linha atual
							$i++;
							// Monta as colunas
							$colunas .= ( $tot == $i ) ? "{$coluna} " : "{$coluna}, " ;
							// Monta os valores
							$valores .= ( $tot == $i ) ? "{$val} " : "{$val}, " ; 
						}
						/* Monta o insert */
						$sqlInsert = "
									INSERT INTO integracao.s_entidade_dirigente_atualizacao_rotina
										({$colunas})
									VALUES
										({$valores})";
						/* Executa o sql */
						$result = adapterConnection::coorporativo()->executar($sqlInsert);
						
					}
				}
			}
			// Executa o commit
			return adapterConnection::coorporativo()->commit();
		}
		else
		{
			return false;
		}
	}
	
	public function validaValoresCarga( $arrModificados )
	{
		$arrInsert = array();
		foreach( $arrModificados as $k => $linha )
		{
			$erro	= false;
			$descErro = '';
			if( (is_array($linha)) && (count($linha) > 0 ))
			{
				// Valida ddd e email (impeditivas)
				$email	= trim($linha['ds_e_mail_dirigente']);
				$ddd	= $linha['nu_ddd_dirigente'];
				$tel	= $linha['nu_tel_dirigente'];
				
				$ddd 	= ereg_replace("[^1-9]", "", $ddd); // Retira todas letras, zero e caracteres especiais
				$tel 	= ereg_replace("[^0-9]", "", $tel); // Retira todas letras, zero e caracteres especiais
				
				// Valida Email, caso errado, guarda no Log
				if( ! $this->validaEmailRotina($email) || ($email == '') )
				{
					// add log com erro
					$descErro = 'E-mail inválido';
					$erro = true;
				}
				
				// Valida telefone trata os valores
				if( $tel == "" )
				{
					// Verifica o outro, caso esteja errado não insere nada
					$tel2 	= $linha['nu_telefone_entidade'];
					$tel2 	= ereg_replace("[^0-9]", "", $tel2);
					$ddd2 = $linha['nu_ddd_entidade'];
					$ddd2 	= ereg_replace("[^1-9]", "", $ddd2);
					if( ( $tel2 == "" ) || ( ! ((is_numeric($ddd2)) && (strlen($ddd2) == 02) && ($ddd2 >= 11 && $ddd2 <= 99 ))) )
					{
						$tel = '';
					}
					else
					{
						$tel = $tel2;
					}
				}
				
				// Valida ddd trata os valores
				if( ! ((is_numeric($ddd)) && (strlen($ddd) == 02) && ($ddd >= 11 && $ddd <= 99 )) )
				{
					// Verifica o outro, caso esteja errado não insere nada
					$tel2 	= $linha['nu_telefone_entidade'];
					$tel2 	= ereg_replace("[^0-9]", "", $tel2);
					$ddd2 = $linha['nu_ddd_entidade'];
					$ddd2 	= ereg_replace("[^1-9]", "", $ddd2);
					if( ( $tel2 == "" ) || ( ! ((is_numeric($ddd2)) && (strlen($ddd2) == 02) && ($ddd2 >= 11 && $ddd2 <= 99 )) ))
					{
						$ddd = '';
					}
					else
					{
						$ddd = $ddd2;
					}
				}
				
				// Caso um dos dois não tenha sido preenchido, zera os valores
				if(($ddd == "") || ($tel == ""))
				{
					$tel = "";
					$ddd = "";
				}
					
				
				// Caso não exista erro no email nem no ddd
				if( ! $erro )
				{
					// Caso tenha dado tudo certo, valida os demais campos
					foreach($linha as $campo => $valor)
					{
						// Textos sem caracteres
						if( ($campo ==  'no_dirigente') || 
							($campo ==  'nu_identidade_dirigente') ||
							($campo ==  'sg_orgao_emissor_dirigente') ||
							($campo ==  'nu_cep_entidade') ||
							($campo ==  'sg_uf_entidade') ||
							($campo ==  'ds_endereco_entididade') ||
							($campo ==  'ds_compl_endereco_entidade') ||
							($campo ==  'no_bairro_entidade') 
						)
						{//
							 $arrInsert[$k][$campo] =  ereg_replace("[^a-zA-Z0-9_, â, Â, à, À, á, Á,., ã, Ã, ê, Ê, è, È, é, É, î, Î, ì, Ì, í, Í, õ, Õ, ô, Ô, ò, Ò, ó, Ó, û, Û, ú, Ú, ù, Ù, ç, Ç,(,),]", "", $valor);
						}
						// Numericos
						else if( ($campo ==  'co_tp_entidade') ||
							($campo ==  'nu_cpf_dirigente')
						)
						{
							$arrInsert[$k][$campo] = ereg_replace("[^0-9]", "", $valor);
						}
						// sexo
						else if( ($campo ==  'id_sexo_dirigente') )
						{
							$sexo = $valor;
							$sexo = ereg_replace("[^a-zA-Z]", "",$sexo );
							if( strlen($sexo) > 1 )
							{
								if ( strpos(strtoupper($sexo), 'M') !== false  )
								{
									$sexo = 'M';
								}
								else if ( strpos(strtoupper($sexo), 'F') !== false  )
								{
									$sexo = 'F';
								}
								else 
								{
									$sexo = '';
								}
							}
							$arrInsert[$k][$campo] = $sexo;
						}
						else if($campo ==  'ds_e_mail_dirigente')
						{
							$arrInsert[$k][$campo] = $email;
						}
						else if($campo ==  'nu_ddd_dirigente')
						{
							$arrInsert[$k][$campo] = $ddd;
						}
						else if($campo ==  'nu_tel_dirigente')
						{
							$arrInsert[$k][$campo] = $tel;
						}
						else
						{
							if( 
								( $campo !=  'nu_ddd_entidade') &&
								( $campo !=  'nu_telefone_entidade')
							)
							{
								$arrInsert[$k][$campo] = $valor;
							}
						}
					}
				}
				else 
				{
					$this->insereLogErro($linha, $descErro);
				}
			}
		}
		return $arrInsert;
	}

	public function buscaCpfDirigentesNaoAtualizados()
    {
        /**
         * COMPARA A TABELA DE ATUALIZAÇÃO COM A TABELA DE DIRIGENTES
         */
        $sqlAualizacaorotina =<<<SQL
    select 
      nu_cpf_dirigente  as cpf,
      co_tp_entidade as tp_entidade,
      ds_e_mail_dirigente,
      nu_tel_dirigente,
      nu_ddd_entidade
    from 
    integracao.s_entidade_dirigente ent_dirigente
    where nu_cpf_dirigente in (
        select entcpf from integracao.s_entidade_dirigente_atualizacao_rotina
        where entcpf         = ent_dirigente.nu_cpf_dirigente
        and   muncod         = ent_dirigente.co_municipio_ibge_entidade
        and   co_tp_entidade = ent_dirigente.co_tp_entidade
    )
SQL;
        $arEntidadeDirigenteAtualizacaoRotina = [];
        $arEntidadeDirigenteAtualizacaoRotina = adapterConnection::coorporativo()->carregar($sqlAualizacaorotina);
        $mEnt             = new Par3_Model_InstrumentoUnidadeEntidade();
        $arDirigentes     = null;
        $arCpfDirigentes  = [];
        if(is_array($arEntidadeDirigenteAtualizacaoRotina)) {
            $arDirigentes = [];
            foreach($arEntidadeDirigenteAtualizacaoRotina as $dirigente) {
                $tenid    = '';
                if($dirigente['tp_entidade'] != '02' && $dirigente['tp_entidade'] != '10') {continue;}
                if($dirigente['tp_entidade'] == '02') {$tenid = $mEnt::PREFEITO;}
                if($dirigente['tp_entidade'] == '10') {$tenid = $mEnt::SECRETARIO_ESTADUAL_EDUCACAO;}

                /**PEGAR DADOS NA TABELA par3.instrumentounidade_entidade*/
                $telefone = $dirigente['nu_tel_dirigente'];
                $ddd      = (strlen($dirigente['nu_ddd_entidade']) == 3? substr($dirigente['nu_ddd_entidade'],1): $dirigente['nu_ddd_entidade']);
                $telefoneFormatado = trim($ddd).trim($telefone);

                $sqlEnt = "select * from par3.instrumentounidade_entidade where entcpf = '%s' and tenid = %d and entstatus = 'A' ";
                $ent = $mEnt->pegaLinha(sprintf($sqlEnt,$dirigente['cpf'],$tenid));

                $endereco = [];
                /**PEGA O ENDEREÇO DO DIRIGENTE*/
                if($ent['entcpf']) {
                    $sqlEndereco =<<<SQL
                    select * from par3.endereco
                    inner join par3.instrumentounidade_entidade using(endid)
                    where entcpf = '{$ent['entcpf']}'
SQL;
                    $endereco = $mEnt->pegaLinha($sqlEndereco);
                }

                if(
                    ($ent['entemail']                  != $dirigente['ds_e_mail_dirigente']) ||
                    ($ent['enttelefonecomercial']      != $telefoneFormatado)
                ) {
                    $arDirigentes[] = $ent;
                }
            }
            $arCpfDirigentes = array_filter(array_map(function($ent){ return $ent['entcpf'];},$arDirigentes));
        }

        return count($arCpfDirigentes) > 0 ? $arCpfDirigentes : null;
    }

    public function compararEntidadesCorporativo($rsEnt,$rsEnd,$co_tp_entidade)
    {
        $enttelefonecomercial = str_replace(array("(",")"," "),array("","",""),$rsEnt['enttelefonecomercial']);
        $arDados[] = str_replace(array("'\'","."),array("",""),$rsEnt['entcnpj']);
        $arDados[] = $co_tp_entidade;
        $arDados[] = $rsEnt['entemail'];
        $arDados[] = substr($enttelefonecomercial,2);
        $arDados[] = substr($enttelefonecomercial,0,2);
        $arDados[] = trim($rsEnd['endlogradouro']);
        $arDados[] = trim($rsEnd['endcomplemento']);
        $arDados[] = str_replace(array("-","."),array("",""),$rsEnd['endcep']);
        $sqlCorporativo = "
        select *
        from integracao.s_entidade_dirigente
        where nu_cgc_entidade          = '%s'
        and co_tp_entidade             = '%s'
        and (
                (ds_e_mail_entidade            <> '%s' and ds_e_mail_entidade         is not null)
                or (REPLACE(nu_telefone_entidade, ' ', '')  not ILIKE '%%%s' and nu_telefone_entidade       is not null)
                or (nu_ddd_entidade            not ILIKE '%%%s' and nu_ddd_entidade     is not null)
                or (ds_endereco_entididade     not ILIKE '%%%s%%' and ds_endereco_entididade     is not null)
                or (ds_compl_endereco_entidade not ILIKE '%%%s%%' and ds_compl_endereco_entidade is not null)
                or (nu_cep_entidade            <> '%s' and nu_cep_entidade            is not null)
            )
        ";
        $sql = vsprintf($sqlCorporativo,$arDados);
//        ver($sql);
        $arDadosCorporativo = adapterConnection::coorporativo()->pegaLinha($sql);
        if(!is_array($arDadosCorporativo)) {
            return false;
        }
        return array_merge($arDadosCorporativo,$rsEnt);
    }

    public function atualizarEntidades($arDadosEntidade,$entid)
    {
        $ddd    = $arDadosEntidade['nu_ddd_entidade'];
        $dddEnt = strpos($ddd,'0') === 0 ? substr($ddd,1): $ddd;
        $mEnt   = new Par3_Model_InstrumentoUnidadeEntidade($entid);
        $arDadosEnt['entid']                = $entid;
        $arDadosEnt['enttelefonecomercial'] = $dddEnt.$arDadosEntidade['nu_telefone_entidade'];
        $arDadosEnt['entemail']             = $arDadosEntidade['ds_e_mail_entidade'];
        $mEnt->popularDadosObjeto($arDadosEnt);
        $mEnt->salvar();
        $mEnt->commit();

        $mEnd = new Par3_Model_Endereco($arDadosEntidade['endid']);
        $arDadosEnd['endid']          = $arDadosEntidade['endid'];
        $arDadosEnd['endlogradouro']  = $arDadosEntidade['ds_endereco_entididade'];
        $arDadosEnd['endcomplemento'] = $arDadosEntidade['ds_compl_endereco_entidade'];
        $arDadosEnd['endcep']         = $arDadosEntidade['nu_cep_entidade'];
        $mEnd->popularDadosObjeto($arDadosEnd);
        $mEnd->salvar();
        $mEnt->commit();
    }

    public function comparaDiferencaEntidades($arEnt = null)
    {
        $limit = 1;
        $sqlEnt = "
        select *
          from par3.instrumentounidade_entidade
        where entstatus = 'A'
        and tenid = %d
        and inuid = %d
        ";

        $mInu      = new Par3_Model_InstrumentoUnidade();
        $mEnt      = new Par3_Model_InstrumentoUnidadeEntidade();
        $mEndereco = new Par3_Model_Endereco();
        $rsUnu     = $mInu->recuperarTodos('inuid,itrid',["inustatus = 'A'"]);

//        $limit = 40;
//        $cont  = 0;
        $tenid = 1;
        $rsEnt = [];
        $entidadesAtualizadas = [];
        $arEnt = [];
        $arEntidadesAtualizadas = [];
        foreach($rsUnu as $inu) {
            $tenid = ($inu['itrid'] == 2 ? $mEnt::PREFEITURA : $mEnt::SECRETARIA_EDUCACAO);
            $rsEnt = $mEnt->pegaLinha(sprintf($sqlEnt,$tenid,$inu['inuid']));
            $rsEnd = $mEndereco->pegaDadosPorEntidade($rsEnt['entid']);
            $co_tp_entidade =  $rsEnt['tenid'] == $mEnt::PREFEITURA ? '02' : '10';
            $entidadesAtualizadas = $this->compararEntidadesCorporativo($rsEnt,$rsEnd,$co_tp_entidade);
            if(is_array($entidadesAtualizadas)) {
                if(count($entidadesAtualizadas) > 0) {
                    $this->atualizarEntidades($entidadesAtualizadas,$rsEnt['entid']);
                    $arEntidadesAtualizadas[] = $rsEnt;
//                    if($cont == $limit) {
//                        break;
//                    }
//                    $cont++;
                }
            }
//            else{
//                ver($rsEnt);
//            }
        }
        return $arEntidadesAtualizadas;
    }

}
