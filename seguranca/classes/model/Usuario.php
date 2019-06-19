<?php


include_once APPRAIZ . 'seguranca/classes/model/Historicousuario.php';
include_once APPRAIZ . 'seguranca/classes/model/Usuario_sistema.php';

class Seguranca_Model_Usuario extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.usuario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "usucpf" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
    		'usucpf' => null,
    		'regcod' => null,
    		'usunome' => null,
    		'usuemail' => null,
    		'usustatus' => null,
    		'usufoneddd' => null,
    		'usufonenum' => null,
    		'ususenha' => null,
    		'usudataultacesso' => null,
    		'usunivel' => null,
    		'usufuncao' => null,
    		'ususexo' => null,
    		'orgcod' => null,
    		'unicod' => null,
    		'usuchaveativacao' => null,
    		'usutentativas' => null,
    		'usuprgproposto' => null,
    		'usuacaproposto' => null,
    		'usuobs' => null,
    		'ungcod' => null,
    		'usudatainc' => null,
    		'usuconectado' => null,
    		'pflcod' => null,
    		'suscod' => null,
    		'usunomeguerra' => null,
    		'orgao' => null,
    		'muncod' => null,
    		'usudatanascimento' => null,
    		'usudataatualizacao' => null,
    		'entid' => null,
    		'tpocod' => null,
    		'carid' => null,
	);

	protected $stOrdem = null;

	public function recuperarPorCPF($usucpf)
	{
		$sql = "SELECT *
			    FROM {$this->stNomeTabela}
			    WHERE usuario.usucpf = '{$usucpf}'";

		return $this->pegaLinha($sql);
	}

    /**
	 * Função salvar
	 * Método usado para inserção ou alteração de um registro do banco
	 * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
	 * @access public
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 */
	public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array(), $manterAspas = false)
	{
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();

		if( $boAntesSalvar ){
			if( !$this->antesSalvar() ){ return false; }
		}

		if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor declarar atributo 'protected \$arChavePrimaria = [];' na classe filha!" );

		$stChavePrimaria = $this->arChavePrimaria[0];

		if( $this->testaUsuarioExistente($this->usucpf) ){
			$this->alterar($arCamposNulo);
			$resultado = $this->$stChavePrimaria;
		}else{
            if($manterAspas === false){
                $resultado = $this->inserir($arCamposNulo);
            }else{
                $resultado = $this->inserirManterAspas($arCamposNulo);
            }
		}

		if( $resultado ){
			if( $boDepoisSalvar ){
				$this->depoisSalvar();
			}
		}

		return $resultado;
	} // Fim salvar()

	/**
	 * Função _inserir
	 * Método usado para inserção de um registro do banco
	 * @return int|bool - Retorna um inteiro correspondente ao resultado ou false se hover erro
	 * @access private
	 * @author Orion Teles de Mesquita
	 * @since 12/02/2009
	 */
	public function inserir($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
		if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );
		$arCampos  = array();
		$arValores = array();
		$arSimbolos = array();

		$troca = array("'", "\\");
		foreach( $this->arAtributos as $campo => $valor ){
			if( $valor !== null ){
                if( !$valor && in_array($campo, $arCamposNulo) ){ continue; }
                    $arCampos[]  = $campo;
                    $valor = str_replace($troca, "", $valor);
                    $arValores[] = trim( pg_escape_string( $valor ) );
			}
		}
      
        if( count( $arValores ) ){
            $arr= implode( "','", $arValores  ) ;
            $dados = str_replace("''", 'null', $arr);
            $valor = isset($arValores) ? $arValores : null;
                    isset($post->post_title) ? $post->post_title : $post->title;
			$sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
                                 values ( '$dados') returning {$this->arChavePrimaria[0]}";
                                 //values ( '". implode( "', '", $arValores ) ."' )
			$stChavePrimaria = $this->arChavePrimaria[0];
			return $this->$stChavePrimaria = $this->pegaUm( $sql );
		}
	} // Fim _inserir()


	/* função testaUsuarioExistente()
	 * verifica se existe usuário cadastrado com o cpf informado
	 * */
	public function testaUsuarioExistente($usucpf)
	{
	    $sql = "SELECT TRUE FROM {$this->stNomeTabela} WHERE usucpf = '$usucpf'";

	    return $this->pegaUm($sql) == 't';
	}


	/* função listaHistoricoUsuario()
	 * listahistorico de usuário
	 * */
	public function listaHistoricoUsuario($arrDados)
	{
	    $where = array();

	    if($arrDados['sisid']){
	        $where[] = "hu.sisid IN ({$arrDados['sisid']})";
	    }

	    if($arrDados['usucpf']){
	        $arrDados['usucpf'] = str_replace(array('.','-'), '', $arrDados['usucpf']);
	        $where[] = "usu.usucpf = '{$arrDados['usucpf']}'";
	    }

	    if($arrDados['usunome']){
	        $where[] = "UPPER(usu.usunome) ILIKE UPPER('%{$arrDados['usunome']}%')";
	    }

	    if($arrDados['pflcod']){
	        $where[] = "pfu.pflcod = '{$arrDados['pflcod']}'";
	    }

	    if($where[1] != ''){

    	    $sql = "SELECT DISTINCT sis.sisabrev, usu.usucpf, usu.usunome, to_char( hu.htudata, 'dd/mm/YYYY' ) as data, hu.suscod, hu.htudsc, hu.usucpfadm, coalesce(us1.usunome, '-') as nomeadm
                    FROM seguranca.historicousuario hu
    	            INNER JOIN seguranca.sistema        sis ON sis.sisid = hu.sisid
                    INNER JOIN seguranca.usuario 		usu ON usu.usucpf = hu.usucpf
                    LEFT  JOIN seguranca.usuario 		us1 ON us1.usucpf = hu.usucpfadm
                    INNER JOIN seguranca.perfilusuario 	pfu ON pfu.usucpf = usu.usucpf
                    WHERE ".implode(' AND ', $where)."
                    ORDER BY usu.usunome DESC, data";

    	    $cabecalho = array('Sistema', 'CPF', 'Nome', 'Data', 'Status', 'Observação', 'CPF Administrador', 'Administrador');

    	    $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    	    $list->setCabecalho($cabecalho);
    	    $list->setQuery($sql);
    	    $list->setFormOff();
    	    $list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
	    }
	}

	public function cadastrarHistorico($dados)
    {
		$sql = "INSERT INTO seguranca.historicousuario(
		htudsc, htudata, usucpf, sisid, suscod %s)
		VALUES ('{$dados['htudsc']}', NOW(), '{$dados['usucpf']}', {$dados['sisid']}, '{$dados['suscod']}' %s);";

        if (isset($dados['usucpfadm'])) {
            $sql = sprintf($sql, ',usucpfadm', ",'{$dados['usucpfadm']}'");
        } else {
            $sql = sprintf($sql, '', '');
        }

		$this->executar( $sql );
        $this->commit();
	}

    /**
     * Verifica se o usuário existe na tabela.
     *
     * @param null $usucpf
     * @return bool
     */
    public function exists($usucpf = null){
        $usucpf = empty($usucpf) ? $this->usucpf : $usucpf;
        $res = $this->recuperarTodos('usucpf', ["usucpf = '{$usucpf}'"]);
        return !empty($res);
    }
    public function corrigeSenhasErradas( ){
    	
    	$sql = "
				UPDATE seguranca.usuario SET suscod = 'B' WHERE usucpf in (
				
				SELECT distinct usucpf 
				  FROM seguranca.usuario
				  where substring( ususenha from 1 for 3)='ent' and char_length(ususenha)=7 and suscod = 'A'
				);
		";
    	$executou = $this->executar($sql);
    	return  $this->commit();
        
    }

    /***
     * 
     * */
    public function insereVinculosFalhos( )
    {
    	$sql = "

		INSERT INTO obras2.usuarioresponsabilidade(usucpf, rpustatus, rpudata_inc, pflcod, entid)
				SELECT DISTINCT
				                usu.usucpf,
				                'A' as status,
				                now()::date as data,
				                946 as pflcod, 
				                pref.codigo as entid
				FROM
				                seguranca.usuario usu
				INNER JOIN seguranca.perfilusuario                      pfu ON pfu.usucpf = usu.usucpf
				INNER JOIN seguranca.usuario_sistema                              uss ON uss.usucpf = usu.usucpf AND sisid IN (23, 147) AND uss.suscod = 'A'
				INNER JOIN par3.usuarioresponsabilidade         urs ON urs.usucpf = usu.usucpf AND urs.pflcod = pfu.pflcod AND urs.rpustatus = 'A'
				LEFT JOIN
				                (
				                SELECT DISTINCT
				                               m.muncod,
				                               e.entid as codigo,
				                               CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
				                                               e.entnome || ' - ' || m.mundescricao || ' - ' || ed.estuf ELSE
				                                               e.entnome END as descricao,
				                               ef.funid
				                FROM
				                               entidade.entidade e
				                INNER JOIN entidade.funcaoentidade                 ef ON ef.entid = e.entid
				                LEFT  JOIN entidade.endereco                                 ed ON ed.entid = e.entid
				                LEFT  JOIN territorios.municipio                               m  ON m.muncod = ed.muncod
				                WHERE
				                               ef.funid in (1)
				                               AND e.entstatus = 'A'
				                ) pref ON pref.muncod = urs.muncod
				LEFT  JOIN obras2.usuarioresponsabilidade        rpu ON rpu.usucpf = usu.usucpf AND rpu.pflcod = 946 AND rpu.rpustatus = 'A'
				WHERE
				                pfu.pflcod = 1436
				                AND usu.suscod = 'A'
				                AND rpu.rpuid IS NULL
				ORDER BY
				                usu.usucpf ;
		
		INSERT INTO obras2.usuarioresponsabilidade(usucpf, rpustatus, rpudata_inc, pflcod, entid)
				SELECT DISTINCT
				                usu.usucpf,
				                'A' as status,
				                now()::date as data,
				                946 as pflcod, 
				                pref.codigo as entid
				FROM
				                seguranca.usuario usu
				INNER JOIN seguranca.perfilusuario                      pfu ON pfu.usucpf = usu.usucpf
				INNER JOIN seguranca.usuario_sistema                              uss ON uss.usucpf = usu.usucpf AND sisid IN (23, 147) AND uss.suscod = 'A'
				INNER JOIN par3.usuarioresponsabilidade         urs ON urs.usucpf = usu.usucpf AND urs.pflcod = pfu.pflcod AND urs.rpustatus = 'A'
				LEFT JOIN
				                (
				                SELECT DISTINCT
				                               m.muncod,
				                               e.entid as codigo,
				                               CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
				                                               e.entnome || ' - ' || m.mundescricao || ' - ' || ed.estuf ELSE
				                                               e.entnome END as descricao,
				                               ef.funid
				                FROM
				                               entidade.entidade e
				                INNER JOIN entidade.funcaoentidade                 ef ON ef.entid = e.entid
				                LEFT  JOIN entidade.endereco                                 ed ON ed.entid = e.entid
				                LEFT  JOIN territorios.municipio                               m  ON m.muncod = ed.muncod
				                WHERE
				                               ef.funid in (1)
				                               AND e.entstatus = 'A'
				                ) pref ON pref.muncod = urs.muncod
				LEFT  JOIN obras2.usuarioresponsabilidade        rpu ON rpu.usucpf = usu.usucpf AND rpu.pflcod = 946 AND rpu.rpustatus = 'A'
				WHERE
				                pfu.pflcod = 1433
				                AND usu.suscod = 'A'
				                AND rpu.rpuid IS NULL
				ORDER BY
				                usu.usucpf ;
		
		INSERT INTO obras2.usuarioresponsabilidade(usucpf, rpustatus, rpudata_inc, pflcod, entid)
				SELECT DISTINCT
				                usu.usucpf,
				                'A' as status,
				                now()::date as data,
				                946 as pflcod, 
				                pref.codigo as entid
						
		                
		FROM
		                seguranca.usuario usu
		INNER JOIN seguranca.perfilusuario                      pfu ON pfu.usucpf = usu.usucpf
		INNER JOIN seguranca.usuario_sistema                              uss ON uss.usucpf = usu.usucpf AND sisid IN (23, 147) AND uss.suscod = 'A'
		INNER JOIN par3.usuarioresponsabilidade         urs ON urs.usucpf = usu.usucpf AND urs.pflcod = pfu.pflcod AND urs.rpustatus = 'A'
		LEFT JOIN
		                (
		                SELECT DISTINCT
		                               ed.estuf,
		                               e.entid as codigo,
		                               CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
		                                               e.entnome || ' - ' || m.mundescricao || ' - ' || ed.estuf ELSE
		                                               e.entnome END as descricao,
		                               ef.funid
		                FROM
		                               entidade.entidade e
		                INNER JOIN entidade.funcaoentidade                 ef ON ef.entid = e.entid and fuestatus = 'A'
		                LEFT  JOIN entidade.endereco                                 ed ON ed.entid = e.entid
		                LEFT  JOIN territorios.municipio                               m  ON m.muncod = ed.muncod
		                WHERE 
		                
		                               ef.funid in (6)
		                               AND e.entstatus = 'A'
		                ) pref ON pref.estuf = urs.estuf
		
		               /* 
		                                                
		                                                
		               */
		LEFT  JOIN obras2.usuarioresponsabilidade        rpu ON rpu.usucpf = usu.usucpf AND rpu.pflcod = 946 AND rpu.rpustatus = 'A'
		WHERE
		                pfu.pflcod = 1437
		                AND usu.suscod = 'A'
		                AND rpu.rpuid IS NULL --and usu.usucpf=''
		               
		ORDER BY
		                usu.usucpf
		
		";
    	$executou = $this->executar($sql);
    	$this->commit();
    	return true;
    	
    }
    public function listaPendenciasVinculo( )
    {
    	$sql = "		       	(SELECT DISTINCT
				                usu.usucpf,
				                'Prefeito' as cargo
				FROM
				                seguranca.usuario usu
				INNER JOIN seguranca.perfilusuario                      pfu ON pfu.usucpf = usu.usucpf
				INNER JOIN seguranca.usuario_sistema                              uss ON uss.usucpf = usu.usucpf AND sisid IN (23, 147) AND uss.suscod = 'A'
				INNER JOIN par3.usuarioresponsabilidade         urs ON urs.usucpf = usu.usucpf AND urs.pflcod = pfu.pflcod AND urs.rpustatus = 'A'
				LEFT JOIN
				                (
				                SELECT DISTINCT
				                               m.muncod,
				                               e.entid as codigo,
				                               CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
				                                               e.entnome || ' - ' || m.mundescricao || ' - ' || ed.estuf ELSE
				                                               e.entnome END as descricao,
				                               ef.funid
				                FROM
				                               entidade.entidade e
				                INNER JOIN entidade.funcaoentidade                 ef ON ef.entid = e.entid
				                LEFT  JOIN entidade.endereco                                 ed ON ed.entid = e.entid
				                LEFT  JOIN territorios.municipio                               m  ON m.muncod = ed.muncod
				                WHERE
				                               ef.funid in (1)
				                               AND e.entstatus = 'A'
				                ) pref ON pref.muncod = urs.muncod
				LEFT  JOIN obras2.usuarioresponsabilidade        rpu ON rpu.usucpf = usu.usucpf AND rpu.pflcod = 946 AND rpu.rpustatus = 'A'
				WHERE
				                pfu.pflcod = 1436
				                AND usu.suscod = 'A'
				                AND rpu.rpuid IS NULL
				ORDER BY
				                usu.usucpf )
				union all
		
		


				(SELECT DISTINCT
				                usu.usucpf,
				               'Dirigente Municipal de Educação' as cargo
				FROM
				                seguranca.usuario usu
				INNER JOIN seguranca.perfilusuario                      pfu ON pfu.usucpf = usu.usucpf
				INNER JOIN seguranca.usuario_sistema                              uss ON uss.usucpf = usu.usucpf AND sisid IN (23, 147) AND uss.suscod = 'A'
				INNER JOIN par3.usuarioresponsabilidade         urs ON urs.usucpf = usu.usucpf AND urs.pflcod = pfu.pflcod AND urs.rpustatus = 'A'
				LEFT JOIN
				                (
				                SELECT DISTINCT
				                               m.muncod,
				                               e.entid as codigo,
				                               CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
				                                               e.entnome || ' - ' || m.mundescricao || ' - ' || ed.estuf ELSE
				                                               e.entnome END as descricao,
				                               ef.funid
				                FROM
				                               entidade.entidade e
				                INNER JOIN entidade.funcaoentidade                 ef ON ef.entid = e.entid
				                LEFT  JOIN entidade.endereco                                 ed ON ed.entid = e.entid
				                LEFT  JOIN territorios.municipio                               m  ON m.muncod = ed.muncod
				                WHERE
				                               ef.funid in (1)
				                               AND e.entstatus = 'A'
				                ) pref ON pref.muncod = urs.muncod
				LEFT  JOIN obras2.usuarioresponsabilidade        rpu ON rpu.usucpf = usu.usucpf AND rpu.pflcod = 946 AND rpu.rpustatus = 'A'
				WHERE
				                pfu.pflcod = 1433
				                AND usu.suscod = 'A'
				                AND rpu.rpuid IS NULL
				ORDER BY
				                usu.usucpf )
		union all
		(
				SELECT DISTINCT
				                usu.usucpf,
				                'Secretário Estadual' as cargo
						
		                
		FROM
		                seguranca.usuario usu
		INNER JOIN seguranca.perfilusuario                      pfu ON pfu.usucpf = usu.usucpf
		INNER JOIN seguranca.usuario_sistema                              uss ON uss.usucpf = usu.usucpf AND sisid IN (23, 147) AND uss.suscod = 'A'
		INNER JOIN par3.usuarioresponsabilidade         urs ON urs.usucpf = usu.usucpf AND urs.pflcod = pfu.pflcod AND urs.rpustatus = 'A'
		LEFT JOIN
		                (
		                SELECT DISTINCT
		                               ed.estuf,
		                               e.entid as codigo,
		                               CASE WHEN (m.mundescricao is not null AND ed.estuf is not null) THEN
		                                               e.entnome || ' - ' || m.mundescricao || ' - ' || ed.estuf ELSE
		                                               e.entnome END as descricao,
		                               ef.funid
		                FROM
		                               entidade.entidade e
		                INNER JOIN entidade.funcaoentidade                 ef ON ef.entid = e.entid and fuestatus = 'A'
		                LEFT  JOIN entidade.endereco                                 ed ON ed.entid = e.entid
		                LEFT  JOIN territorios.municipio                               m  ON m.muncod = ed.muncod
		                WHERE 
		                
		                               ef.funid in (6)
		                               AND e.entstatus = 'A'
		                ) pref ON pref.estuf = urs.estuf
		
		               /* 
		                                                
		                                                
		               */
		LEFT  JOIN obras2.usuarioresponsabilidade        rpu ON rpu.usucpf = usu.usucpf AND rpu.pflcod = 946 AND rpu.rpustatus = 'A'
		WHERE
		                pfu.pflcod = 1437
		                AND usu.suscod = 'A'
		                AND rpu.rpuid IS NULL --and usu.usucpf=''
		               
		        

		ORDER BY
		                usu.usucpf )";
    	
    	$dados = $this->carregar($sql);
    	return (is_array($dados) ) ? $dados : array();
    }
    public function inativaPerfisIndevidos( $arrParams = null)
    {
    	$sisid = $arrParams['sisid'];
    	$cargo = $arrParams['id'];
		
    	$tpentidade = [
    			231 => [
    					'02' => PAR3_PERFIL_PREFEITO, /*PREFEITO*/
    					'10' => PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, /*SECRETARIO ESTADUAL*/
    					'28' => PAR3_PERFIL_DIRIGENTE_MUNICIPAL /*DIRIGENTE MUNICIPAL*/
    			],
    			23 => [
    					'02' => PAR_PERFIL_PREFEITO,
    					'10' => PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,
    					'28' => PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO
    			]
    	];
    	$idPerfil = $tpentidade[$sisid][$cargo];
    	
    	if($cargo == '02' || $cargo == '28')
    	{
    		$whereIsNull = "rpu.muncod IS NULL";
    	}
    	else
    	{
    		$whereIsNull = "rpu.estuf IS NULL";
    	}
    	
    	$sqlInativacao = 
    	"
				/* DELETA O PERFILUSUÁRIO*/
				DELETE FROM seguranca.perfilusuario  
				where pflcod = $idPerfil
				and usucpf in (
				
					SELECT
				                usu.usucpf
				FROM
				                seguranca.usuario usu
				INNER JOIN seguranca.usuario_sistema sus ON sus.usucpf = usu.usucpf AND sus.sisid = $sisid
				INNER JOIN seguranca.perfilusuario pfu ON pfu.usucpf = usu.usucpf AND pfu.pflcod = $idPerfil
				LEFT  JOIN par3.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf AND rpu.pflcod = $idPerfil AND rpu.rpustatus = 'A'
				LEFT  JOIN
				                (
				                SELECT p.usucpf, count(p.pflcod) as qtd FROM seguranca.perfilusuario p 
				                INNER JOIN seguranca.perfil pfl ON pfl.pflcod = p.pflcod AND pfl.sisid = $sisid
				                GROUP BY p.usucpf
				                ) foo ON foo.usucpf = usu.usucpf
				WHERE
				                $whereIsNull
				                AND foo.qtd > 1
				);
				
				
				/* INATIVA  O USUÁRIO SISTEMA*/
				UPDATE seguranca.usuario_sistema  SET susstatus = 'I'
				WHERE
				pflcod = $idPerfil
				AND 
				sisid = $sisid
				AND
				usucpf in (
				
					SELECT
				                usu.usucpf
				FROM
				                seguranca.usuario usu
				INNER JOIN seguranca.usuario_sistema sus ON sus.usucpf = usu.usucpf AND sus.sisid = $sisid
				INNER JOIN seguranca.perfilusuario pfu ON pfu.usucpf = usu.usucpf AND pfu.pflcod = $idPerfil
				LEFT  JOIN par3.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf AND rpu.pflcod = $idPerfil AND rpu.rpustatus = 'A'
				LEFT  JOIN
				                (
				                SELECT p.usucpf, count(p.pflcod) as qtd FROM seguranca.perfilusuario p 
				                INNER JOIN seguranca.perfil pfl ON pfl.pflcod = p.pflcod AND pfl.sisid = $sisid
				                GROUP BY p.usucpf
				                ) foo ON foo.usucpf = usu.usucpf
				WHERE
				                $whereIsNull
				                AND foo.qtd > 1
				);
				
				
				/* DELETA O PERFILUSUÁRIO*/
				DELETE FROM seguranca.perfilusuario  
				where pflcod = $idPerfil
				and usucpf in (
				
					SELECT
				                usu.usucpf
				FROM
				                seguranca.usuario usu
				INNER JOIN seguranca.usuario_sistema sus ON sus.usucpf = usu.usucpf AND sus.sisid = $sisid
				INNER JOIN seguranca.perfilusuario pfu ON pfu.usucpf = usu.usucpf AND pfu.pflcod = $idPerfil
				LEFT  JOIN par3.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf AND rpu.pflcod = $idPerfil AND rpu.rpustatus = 'A'
				WHERE
				                $whereIsNull
				                -- Secretário estadual  = estuf
				               
				);
    			
    	";
    	
    	$executou = $this->executar($sqlInativacao);
    	if($executou)
    	{
    		return $this->commit();
    	}
    	else
    	{
    		return false;
    	}
    }
    /**
     * Retorna o status geral do usuário no sistema informado.
     * Os status podem ser:
     *    A - Ativo;
     *    P - Pendente;
     *    B - Bloqueado;
     *
     * @param $sisid
     * @param null $usucpf
     * @return bool|mixed|NULL|string
     */
    public function retornaStatusGeral( $usucpf = null){
        $usucpf = empty($usucpf) ? $this->usucpf : $usucpf;
        $sql = <<<DML
            select suscod from seguranca.usuario where usucpf = '{$usucpf}' 
DML;
        return $this->pegaUm($sql, 'suscod');
    }
    /**
     * Retorna o status geral do usuário no sistema informado.
     * Os status podem ser:
     *    A - Ativo;
     *    P - Pendente;
     *    B - Bloqueado;
     *
     * @param $sisid
     * @param null $usucpf
     * @return bool|mixed|NULL|string
     */
    public function retornaStatusSistema($sisid, $usucpf = null){
        $usucpf = empty($usucpf) ? $this->usucpf : $usucpf;
        $sql = <<<DML
            select suscod from seguranca.usuario_sistema where usucpf = '{$usucpf}' and sisid = {$sisid}
DML;
        return $this->pegaUm($sql, 'suscod');
    }
 
    public function alteraStatusGeral($sisid, $suscod, $justificativa, $usucpfadm = null, $usucpf = null){
        $usucpf = empty($usucpf) ? $this->usucpf : $usucpf;

        /** INSTANCIA OS OBJETOS UTILIZADOS */
        $htu = new Seguranca_Model_Historicousuario();
        $uss = new Seguranca_Model_Usuario_sistema();

        /** CARREGA O REGISTRO DA TABELA USUARIO_SISTEMA PELO ID */
        $uss->carregarPorId([
            'usucpf' => $usucpf,
            'sisid' => $sisid
        ]);

        /** ALTERA O STATUS DO USUÁRIO */
        $uss->usucpf = $usucpf;
        $uss->sisid = $sisid;
        $uss->suscod = $suscod;
        $uss->susdataultacesso = date('Y-m-d H:i:s');
        
        $uss->salvar();

        $htu->usucpf = $usucpf;
        $htu->sisid = $sisid;
        $htu->suscod = $suscod;
        $htu->htudsc = $justificativa;
        $htu->usucpfadm = !empty($usucpfadm) ? $usucpfadm : $_SESSION['usucpf'];

        $htu->salvar();

        if($htu->commit()){
            return true;
        } else {
            return false;
        }
    }

    public function ativarUsuario($usucpf)
    {
        $sql = "UPDATE seguranca.usuario
                  SET suscod = 'A'
                  WHERE usucpf = '{$usucpf}'
                ";
        return $this->executar($sql);
    }

    /**
     * Retorna o perfil do usuário no sistema informado.
     *
     * @param $sisid
     * @param null $usucpf
     * @return array|bool|mixed|NULL
     */
    public function retornaPerfilNoSistema($sisid, $usucpf = null){
        $usucpf = empty($usucpf) ? $this->usucpf : $usucpf;
        $sql = <<<DML
            SELECT
              pfl.pflcod,
              pfl.pfldsc
            FROM seguranca.perfil pfl
              INNER JOIN seguranca.perfilusuario pfu USING (pflcod)
            WHERE usucpf = '{$usucpf}'
              AND pfl.sisid = {$sisid}
DML;
        return $this->pegaLinha($sql);
    }

    /**
     * Verifica na requisição REST Auth se o usuario possui perfil de acesso usuarioWS
     * @param $usucpf
     * @throws \Exception
     */
    public function isUsuarioWs($usucpf,$sisid)
    {
        $usuarioWS = PFL_CGSO.', '.PFL_SUPERUSUARIO;
        $sql = "
            SELECT
                pu.pflcod
            FROM
                seguranca.perfilusuario pu
            INNER JOIN seguranca.perfil p ON
                p.pflcod = pu.pflcod
                AND pu.usucpf = '{$usucpf}'
                AND p.sisid = {$sisid}
                AND pu.pflcod IN ({$usuarioWS})
                AND pflstatus = 'A'";
        if (!$this->carregar($sql)) {
            return false;
        }
        return true;
    }

    public function retornaUsuariosNoSistema($sisid = null) {
        $array = [];
        if (!empty($sisid)) {
            $sql = <<<SQL
                SELECT 
                    usucpf,
                    usunome
                FROM seguranca.usuario usu
                WHERE EXISTS (
                    SELECT 
                        1
                    FROM seguranca.perfilusuario
                    WHERE usucpf = usu.usucpf
                    AND pflcod IN (SELECT pflcod FROM seguranca.perfil WHERE sisid = %d)
                )
                ORDER BY usunome;
SQL;
            $sql = sprintf($sql, $sisid);

            $res = $this->carregar($sql);

            foreach ($res as $item) {
                $array[$item['usucpf']] = $item['usunome'];
            }
        }
        return $array;
    }
}